/**
 *	Rewrite of DSA1 v3.02_de functions of seg088 (dungeon: thorwal 2 / 2)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg088.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif


void DNG14_dive(signed short diver_pos, signed char mod, signed short dest_x)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (i != diver_pos &&
			host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
			!hero_dead(hero))
		{

			if (test_skill(hero, TA_SCHWIMMEN, mod) <= 0) {
				/* swimming failed */
				sprintf(g_dtp2,
					get_tx(42),
					(char*)hero + HERO_NAME2,
					(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(g_dtp2);

				/* loose 2W6 LE */
				sub_hero_le(hero, dice_roll(2, 6, 0));
			} else {
				/* swimming succeeded */

				sprintf(g_dtp2,
					get_tx(43),
					(char*)hero + HERO_NAME2);

				GUI_output(g_dtp2);
			}
		}
	}

	gs_x_target = (dest_x);
	ds_writews(DNG_REFRESH_DIRECTION, -1);

}

/**
 * \brief   print introductions of dungeon #14 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG14_fight_intro(signed short fight_id)
{
	if ((fight_id == 172) || (fight_id == 174)) {
		GUI_output(get_tx(56));
	}
}

void DNG14_chest_x1(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd(chest + 11);
	host_writed(chest + 11, (Bit32u)(p_datseg + DNG14_CHEST_X1));
	loot_simple_chest(chest);
	host_writed(chest + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x2(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd(chest + 11);
	host_writed(chest + 11, (Bit32u)(p_datseg + DNG14_CHEST_X2));
	loot_simple_chest(chest);
	host_writed(chest + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x3(Bit8u* chest)
{
	loot_multi_chest(p_datseg + DNG14_CHEST_X3, get_tx(62));
}

void DNG14_chest_x4(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd(chest + 11);
	host_writed(chest + 11, (Bit32u)(p_datseg + DNG14_CHEST_X4));
	loot_simple_chest(chest);
	host_writed(chest + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x5(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd(chest + 11);
	host_writed(chest + 11, (Bit32u)(p_datseg + DNG14_CHEST_X5));
	loot_simple_chest(chest);
	host_writed(chest + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x6(Bit8u* chest)
{
	Bit8u* ptr_bak;
	unsigned char x;
	unsigned char y;

	ptr_bak = (Bit8u*)host_readd(chest + 11);
	host_writed(chest + 11, (Bit32u)(p_datseg + DNG14_CHEST_X6));
	loot_simple_chest(chest);
	host_writed(chest + 11, (Bit32u)ptr_bak);

	if (ds_readb(DNG14_FULLMAP_FLAG) != 0) {

		ds_writeb(DNG14_FULLMAP_FLAG, 0);

		GUI_output(get_tx(64));

		for (x = 0; x < 16; x++) {
			for (y = 0; y < 16; y++) {
				set_automap_tile(x, y);
			}
		}
	}
}

void DNG14_chest_x7(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd(chest + 11);
	host_writed(chest + 11, (Bit32u)(p_datseg + DNG14_CHEST_X7));
	loot_simple_chest(chest);
	host_writed(chest + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x8(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd(chest + 11);
	host_writed(chest + 11, (Bit32u)(p_datseg + DNG14_CHEST_X8));
	loot_simple_chest(chest);
	host_writed(chest + 11, (Bit32u)ptr_bak);
}

void DNG14_chest_x9(void)
{
	Bit8u *hero;

	hero = (Bit8u*)get_first_hero_available_in_group();
	print_msg_with_first_hero(get_tx(61));
	sub_hero_le(hero, dice_roll(2, 6, 0));
}

void DNG15_riddle(void)
{
	signed short i;
	signed short l_di;
	signed short pos;
	signed short tw_bak;
	Bit8u *ptr;

	tw_bak = g_textbox_width;
	g_textbox_width = 8;

	ptr = p_datseg + DNG_MAP;

	pos = (gs_dungeon_level << 12) + (gs_x_target << 8) + gs_y_target;

	/* check if the other group is in position */
	for (i = l_di = 0; i < 6; i++) {

		if (pos == DNG_POS(1,8,1) && gs_groups_x_target[i] == 8 &&
			gs_groups_y_target[i] == 5 && gs_current_group != i)
		{
			l_di = 1;
		}

		if (pos == DNG_POS(1,8,5) && gs_groups_x_target[i] == 8 &&
			gs_groups_y_target[i] == 1 && gs_current_group != i)
		{
			l_di = 1;
		}
	}

	if (!l_di || !ds_readb(DNG15_REACHED_HANDS)) {

		/* INFO: you hear a soft cracking noise */
		GUI_output(get_tx(28));

	} else {

		/* ask which lever should be pulled */
		do {
			i = GUI_radio(get_tx(29), 2, get_tx(30), get_tx(31));

		} while (i == -1);

		if (i == 2) {
			/* pull on the other side */

			/* set the corresponding lever */
			if (gs_y_target == 1) {
				ds_writeb(DNG15_LEVER_SOUTH, 1);
			} else if (gs_y_target == 5) {
				ds_writeb(DNG15_LEVER_NORTH, 1);
			}

			if (ds_readb(DNG15_LEVER_SOUTH) != 0 && ds_readb(DNG15_LEVER_NORTH) != 0)
			{
				/* riddle solved: remove the door from the map */
				GUI_output(get_tx(32));

				host_writeb(ptr + MAP_POS(9,3), DNG_TILE_OPEN_DOOR << 4);

				add_hero_ap_all(10);
			} else {
				/* INFO: you hear a soft cracking noise */
				GUI_output(get_tx(28));
			}
		} else {
			/* pull on the this side */
			/* INFO: you hear a soft cracking noise */
			GUI_output(get_tx(28));
		}
	}

	g_textbox_width = tw_bak;
}

#if !defined(__BORLANDC__)
}
#endif
