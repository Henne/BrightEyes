/**
 *	Rewrite of DSA1 v3.02_de functions of seg086 (dungeon: pirate cave on Manrek)
 *	Functions rewritten: 16/16 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg086.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg032.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG11_handler(void)
{
	signed short target_pos;
	signed short test_result;
	signed short answer;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;

	amap_ptr = g_dng_map;
	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = (Bit8u*)get_first_hero_available_in_group();

	if (target_pos == DNG_POS(0,6,12) && target_pos != gs_dng_handled_pos && ds_readb(DNG11_SOUP_SUPPLY) != 0)
	{
		if (GUI_radio(get_tx(1), 2, get_tx(2), get_tx(3)) == 1)
		{
			add_group_le(1);

			dec_ds_bs_post(DNG11_SOUP_SUPPLY);

			if (random_schick(100) <= 50)
			{
				ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,8,15,SOUTH)))));
				do_fight(FIGHTS_F131_01B);
			}
		}

	} else if (target_pos == DNG_POS(0,11,14) && target_pos != gs_dng_handled_pos)
	{
		DNG_waterbarrel(p_datseg + DNG11_WATERBARREL1);

	} else if (target_pos == DNG_POS(0,12,14) && target_pos != gs_dng_handled_pos)
	{
		DNG_waterbarrel(p_datseg + DNG11_WATERBARREL2);

	} else if (target_pos == DNG_POS(0,6,9) && target_pos != gs_dng_handled_pos)
	{
		answer = GUI_radio(get_tx(4), 6,
					get_tx(5),
					get_tx(6),
					get_tx(7),
					get_tx(8),
					get_tx(9),
					get_tx(10));

		ds_writeb(DNG11_LEVER_FLAG, 0);

		if (answer >= 1 && answer <= 5)
		{
			ds_writeb(DNG11_LEVER_FLAG, (signed char)answer);

			GUI_output(get_tx(11));
		}

	} else if (target_pos == DNG_POS(0,9,9))
	{
		if (target_pos != gs_dng_handled_pos && !ds_readb(DNG11_EFFERD_HINT))
		{
			GUI_output(get_tx(12));

		} else if (ds_readb(DNG11_EFFERD_SACRIFICE) != 0 && !ds_readb(DNG11_EFFERD_HINT))
		{
			ds_writeb(DNG11_EFFERD_HINT, 1);
			ds_writeb(DNG11_UNKNOWN2_FLAG, ds_writeb(DNG11_EFFERD_SACRIFICE, 0));

			GUI_output(get_tx(13));
		}

	} else if (target_pos == DNG_POS(0,12,10) && target_pos != gs_dng_handled_pos && ds_readb(DNG11_FIRETRAP1_FLAG) != 0)
	{
		if (ds_readb(DNG11_EFFERD_HINT) != 0 || ds_readb(DNG11_UNKNOWN1_FLAG) != 0)
		{
			GUI_output(get_tx(15));

			dec_ds_bs_post(DNG11_FIRETRAP1_FLAG);

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, test_attrib(hero, ATTRIB_GE, 4) <= 0 ? dice_roll(6, 6, 6) : dice_roll(3, 6, 3));
				}
			}
		}

	} else if (target_pos == DNG_POS(0,11,8) && target_pos != gs_dng_handled_pos && ds_readb(DNG11_FIRETRAP2_FLAG) != 0)
	{
		if (ds_readb(DNG11_UNKNOWN2_FLAG) != 0)
		{
			GUI_output(get_tx(15));

			dec_ds_bs_post(DNG11_FIRETRAP2_FLAG);

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero))
				{
					sub_hero_le(hero, test_attrib(hero, ATTRIB_GE, 4) <= 0 ? dice_roll(6, 6, 6) : dice_roll(3, 6, 3));
				}
			}
		}

	} else if (target_pos == DNG_POS(0,11,3) && target_pos != gs_dng_handled_pos && gs_direction == EAST)
	{
		GUI_output(get_tx(17));

	} else if (((target_pos == DNG_POS(0,12,3) && gs_direction == EAST) ||
			(target_pos == DNG_POS(0,11,2) && gs_direction == NORTH)) &&
			target_pos != gs_dng_handled_pos)
	{
			GUI_output(get_tx(16));

	} else if ((target_pos == DNG_POS(0,6,0) || target_pos == DNG_POS(0,7,0) || target_pos == DNG_POS(0,8,0) || target_pos == DNG_POS(0,9,0)) &&
			target_pos != gs_dng_handled_pos)
	{
		/* boat. if entered, the complete group is drowned */
		if (GUI_radio(get_tx(18), 2, get_tx(19), get_tx(20)) == 1)
		{
			GUI_output(get_tx(21));
			GUI_output(get_tx(22));

			load_ani(18);
			init_ani(0);

			GUI_output(get_tx(23));

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group)
				{
					hero_disappear(hero, answer, -1);
				}
			}

			set_var_to_zero();

		} else {
			gs_x_target = (gs_x_target_bak);
			gs_y_target = (gs_y_target_bak);
		}

	} else if (target_pos == DNG_POS(0,8,12) &&
			// possibly a bug. position (8,12) might indeed be (8,11)
			// see https://www.crystals-dsa-foren.de/showthread.php?tid=1373&pid=96541#pid96541
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == NORTH &&
			ds_readb(DNG11_SECRETDOOR1_FLAG) != 2)
	{
		if (ds_readb(DNG11_SECRETDOOR1_FLAG) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 8) > 0)
		{
			ds_writeb(DNG11_SECRETDOOR1_FLAG, 1);

			sprintf(g_dtp2,
				get_tx(26),
				(char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((ds_readb(DNG11_LEVER_FLAG) == 2 || ds_readb(DNG11_LEVER_FLAG) == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 5)) > 0 ? get_tx(27) : get_tx(28)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + MAP_POS(8,11), (DNG_TILE_CORRIDOR << 4) + 0x0f);

				ds_writeb(DNG11_SECRETDOOR1_FLAG, 2);

				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (target_pos == DNG_POS(0,4,7) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST &&
			ds_readb(DNG11_SECRETDOOR2_FLAG) != 2)
	{
		if (ds_readb(DNG11_SECRETDOOR2_FLAG) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0)
		{
			ds_writeb(DNG11_SECRETDOOR2_FLAG, 1);

			sprintf(g_dtp2,
				get_tx(26),
				(char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((ds_readb(DNG11_LEVER_FLAG) == 1 || ds_readb(DNG11_LEVER_FLAG) == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 3)) > 0 ? get_tx(27) : get_tx(28)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + MAP_POS(5,7), (DNG_TILE_CORRIDOR << 4) + 0x0f);

				ds_writeb(DNG11_SECRETDOOR2_FLAG, 2);

				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (target_pos == DNG_POS(0,9,8) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST &&
			ds_readb(DNG11_SECRETDOOR3_FLAG) != 2)
	{
		if (ds_readb(DNG11_SECRETDOOR3_FLAG) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 6) > 0)
		{
			ds_writeb(DNG11_SECRETDOOR3_FLAG, 1);

			sprintf(g_dtp2,
				get_tx(26),
				(char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((ds_readb(DNG11_LEVER_FLAG) == 5 || ds_readb(DNG11_LEVER_FLAG) == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 5)) > 0 ? get_tx(27) : get_tx(28)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (test_result > 0)
			{
				and_ptr_bs(amap_ptr + MAP_POS(10,8), (DNG_TILE_CORRIDOR << 4) + 0x0f);

				ds_writeb(DNG11_SECRETDOOR3_FLAG, 2);

				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (target_pos == DNG_POS(0,8,15) && target_pos != gs_dng_handled_pos)
	{
		leave_dungeon();

		gs_current_town = (ds_readbs(TRAVEL_DESTINATION_TOWN_ID));
		gs_x_target = (ds_readws(TRAVEL_DESTINATION_X));
		gs_y_target = (ds_readws(TRAVEL_DESTINATION_Y));
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((ds_readbs(TRAVEL_DESTINATION_VIEWDIR) + 2) & 0x03);

		sprintf(g_dtp2,
			get_tx(29),
			get_ttx(ds_readw(TRV_DESTINATION) + 0xeb));

		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG11_chest1(void)
{
	if (!ds_readb(DNG11_PROVIANT1_FLAG))
	{
		/* a chest with 50 food rations */
		GUI_output(get_tx(24));

		ds_writeb(DNG11_PROVIANT1_FLAG, 1);

	} else {

		/* an empty chest */
		GUI_output(get_ttx(522));
	}
}

void DNG11_chest2(void)
{
	if (!ds_readb(DNG11_PROVIANT2_FLAG))
	{
		/* a chest with 21 food rations */
		GUI_output(get_tx(25));

		ds_writeb(DNG11_PROVIANT2_FLAG, 1);

	} else {

		/* an empty chest */
		GUI_output(get_ttx(522));
	}
}

void DNG11_chest3(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST3));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest4(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST4));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest5(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST5));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest6(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST6));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest7(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST7));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest8(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST8));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest9(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST9));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest10(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST10));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest11(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST11));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest12(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST12));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest13(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST13));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG11_chest14(Bit8u* chest)
{
        Bit8u* ptr_bak;

        ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
        host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG11_CHEST14));
        loot_simple_chest((Bit8u*)(chest));
        host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

/**
 * \brief   print introductions of dungeon #11 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG11_fight_intro(signed short fight_id)
{
	if (fight_id == 127) {
		GUI_output(get_tx(14));
	}
}

#if !defined(__BORLANDC__)
}
#endif
