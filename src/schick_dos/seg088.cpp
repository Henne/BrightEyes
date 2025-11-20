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
#include "seg088.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

struct struct_chest g_dng14_specialchests[9] = {
	{ DNG_POS(0,1,1),	0, NULL,			NULL,			DNG14_chest00_loot, 0, 0, 0 },
	{ DNG_POS(0,10,2),	0, NULL,			NULL,			DNG14_chest01_loot, 0, 0, 0 },
	{ DNG_POS(1,3,6),	4, use_lockpicks_on_chest,	chest_protected_normal,	DNG14_chest02_loot, 0, 0, 0 },
	{ DNG_POS(1,4,6),	4, use_lockpicks_on_chest,	chest_protected_normal,	DNG14_chest03_loot, 0, 0, 0 },
	{ DNG_POS(1,10,11),	0, NULL,			NULL,			DNG14_chest04_loot, 0, 0, 0 },
	{ DNG_POS(1,5,8),	6, use_lockpicks_on_chest,	chest_protected_heavy,	DNG14_chest05_loot, 0, 10300, 0 },
	{ DNG_POS(2,13,2),	8, use_lockpicks_on_chest,	NULL,			DNG14_chest06_loot, 0, 0, 0 },
	{ DNG_POS(2,13,12),	6, use_lockpicks_on_chest,	DNG14_chest07_trap,	DNG14_chest07_loot, 0, 0, 0 },
	{     -1, 0, NULL, NULL, NULL, 0, 0, 0 }
}; // ds:0x9990


void DNG14_dive(const signed int diver_pos, const signed char mod, const signed int dest_x)
{
	signed int i;
	struct struct_hero *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++)
	{
		if ((i != diver_pos) && (hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead)
		{

			if (test_talent(hero, TA_SCHWIMMEN, mod) <= 0) {

				/* swimming failed */
				sprintf(g_dtp2,	get_tx(42), hero->alias, GUI_get_ptr(hero->sex, 0), GUI_get_ptr(hero->sex, 0));
				GUI_output(g_dtp2);

				/* loose 2W6 LE */
				sub_hero_le(hero, dice_roll(2, 6, 0));
			} else {

				/* swimming succeeded */
				sprintf(g_dtp2, get_tx(43), hero->alias);
				GUI_output(g_dtp2);
			}
		}
	}

	gs_x_target = dest_x;
	g_dng_refresh_direction = -1;
}

/**
 * \brief   print introductions of dungeon #14 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG14_fight_intro(const signed int fight_id)
{
	if ((fight_id == 172) || (fight_id == 174)) {
		GUI_output(get_tx(56));
	}
}

void DNG14_chest00_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng14_chest_x1;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG14_chest01_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng14_chest_x2;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG14_chest02_loot(struct struct_chest* chest)
{
	loot_multi_chest(gs_dng14_chest_x3, get_tx(62));
}

void DNG14_chest03_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng14_chest_x4;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG14_chest04_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng14_chest_x5;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG14_chest05_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak;
	unsigned char x;
	unsigned char y;

	ptr_bak = chest->content;

	chest->content = gs_dng14_chest_x6;

	loot_simple_chest(chest);

	chest->content = ptr_bak;

	if (gs_dng14_fullmap_flag) {

		gs_dng14_fullmap_flag = 0;

		GUI_output(get_tx(64));

		for (x = 0; x < 16; x++) {
			for (y = 0; y < 16; y++) {
				set_automap_tile(x, y);
			}
		}
	}
}

void DNG14_chest06_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng14_chest_x7;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG14_chest07_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng14_chest_x8;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG14_chest07_trap(void)
{
	struct struct_hero *hero = get_first_hero_available_in_group();

	print_msg_with_first_hero(get_tx(61));

	sub_hero_le(hero, dice_roll(2, 6, 0));
}

void DNG15_riddle(void)
{
	signed int i;
	signed int l_di;
	signed int pos;
	signed int tw_bak;
	uint8_t *ptr;

	tw_bak = g_textbox_width;
	g_textbox_width = 8;

	ptr = g_dng_map;

	pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	/* check if the other group is in position */
	for (i = l_di = 0; i < 6; i++) {

		if (pos == DNG_POS(1,8,1) && gs_groups_x_target[i] == 8 &&
			gs_groups_y_target[i] == 5 && gs_active_group_id != i)
		{
			l_di = 1;
		}

		if (pos == DNG_POS(1,8,5) && gs_groups_x_target[i] == 8 &&
			gs_groups_y_target[i] == 1 && gs_active_group_id != i)
		{
			l_di = 1;
		}
	}

	if (!l_di || !gs_dng15_reached_hands) {

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
				gs_dng15_lever_south = 1;
			} else if (gs_y_target == 5) {
				gs_dng15_lever_north = 1;
			}

			if (gs_dng15_lever_south && gs_dng15_lever_north)
			{
				/* riddle solved: remove the door from the map */
				GUI_output(get_tx(32));

				ptr[MAP_POS(9,3)] = DNG_TILE_OPEN_DOOR << 4;

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
