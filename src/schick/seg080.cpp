/**
 *	Rewrite of DSA1 v3.02_de functions of seg080 (dungeon: wolvcave, cave)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg080.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg032.h"
#include "seg047.h"
#include "seg080.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct struct_chest g_dng04_specialchests[4] = {
	{ DNG_POS(0,6,7),	1, DNG04_chest00_open, NULL, DNG04_chest00_loot, 0, 0, 0 },
	{ DNG_POS(0,1,12),	1, DNG04_chest01_open, NULL, DNG04_chest01_loot, 0, 0, 0 },
	{ DNG_POS(0,6,1),	1, DNG04_chest02_open, NULL, DNG04_chest02_loot, 0, 0, 0 },
	{     -1,		0, NULL, 	       NULL, NULL,		0, 0, 0 }
}; // ds:0x95ba
int16_t g_dng05_trash_flag = 0; // ds:0x960e

/**
 * \brief   dungeon handler of the wolfcave
 */
signed short DNG04_handler(void)
{
	signed short pos;
	signed short i;
	signed short tw_bak;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

        pos = (gs_dungeon_level << 12) + (gs_x_target << 8) + gs_y_target;

	if (pos == DNG_POS(0,6,7) && pos != gs_dng_handled_pos && !gs_dng04_corpse0_flag)
	{
		/* DNG04_corpse0 */
		loot_special_chest(0);

	} else if (pos == DNG_POS(0,8,8) && pos != gs_dng_handled_pos && !gs_dng04_gap_flag)
	{
		/* do you want to grab into the gap ? */
		do {
			i = GUI_radio(get_tx(2), 2, get_tx(3), get_tx(4));

		} while (i == -1);

		if (i == 2)
		{
			hero = get_first_hero_available_in_group();

			sprintf(g_dtp2,	get_tx(5), hero->alias);
			GUI_output(g_dtp2);

			sub_hero_le(hero, 2);

			/* get a magic AMULET */
			get_item(ITEM_TRAVIA_AMULETT, 1, 1);

			gs_dng04_gap_flag = 1;
		}

	} else if (pos == DNG_POS(0,5,11) && pos != gs_dng_handled_pos && !gs_dng04_helmet_flag)
	{
		/* do you want the SILVER HELMET? */
		if (GUI_bool(get_tx(7)))
		{
			/* get a SILVER HELMET */
			if (get_item(ITEM_SILBERHELM, 1, 1))
			{
				/* mark SILVER HELMET as taken */
				gs_dng04_helmet_flag = 1;
			}
		}

	} else if (pos == DNG_POS(0,1,12) && pos != gs_dng_handled_pos)
	{
		/* DNG04_corpse1 */
		loot_special_chest(0);

	} else if (pos == DNG_POS(0,8,3) && pos != gs_dng_handled_pos)
	{
		/* the ceiling may drop on your head */
		GUI_output(get_tx(9));

		hero = get_hero(0);
		for (i = 0; i <= 6 ; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) &&
				(hero->group_id == gs_active_group_id) &&
				!hero->flags.dead &&
				(test_skill(hero, TA_SCHLEICHEN, 2) <= 0))
			{
				sprintf(g_dtp2,	get_tx(10), hero->alias);
				GUI_output(g_dtp2);

				sub_group_le(dice_roll(2, 6, 0));

				break;
			}
		}

	} else if (pos == DNG_POS(0,7,3) && pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(11));

	} else if (pos == DNG_POS(0,5,5) && pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(12));

	} else if (pos == DNG_POS(0,10,13) && pos != gs_dng_handled_pos)
	{
		/* dig into a heap of dirt? */
		if (GUI_bool(get_tx(13)) && GUI_bool(get_tx(14)))
		{
			GUI_output(get_tx(15));

			i = get_free_mod_slot();

			/* Original-BUG: assumption the leader is at pos 0 */
			/* CH-5 for 1 day */
			set_mod_slot(i, DAYS(1), (uint8_t*)&get_hero(0)->attrib[ATTRIB_CH].current, -5, 0);
		}

	} else if (pos == DNG_POS(0,14,14) && pos != gs_dng_handled_pos)
	{
		/* the ceiling may drop on your head */
		GUI_output(get_tx(9));

		hero = get_hero(0);
		for (i = 0; i <= 6 ; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) &&
				(hero->group_id == gs_active_group_id) &&
				!hero->flags.dead &&
				(test_skill(hero, TA_SCHLEICHEN, 4) <= 0))
			{
				sprintf(g_dtp2, get_tx(10), hero->alias);
				GUI_output(g_dtp2);

				sub_group_le(dice_roll(1, 6, 2));

				break;
			}
		}

	} else if (pos == DNG_POS(0,10,6) && pos != gs_dng_handled_pos && !gs_dng04_lantern_flag)
	{
		/* do you want a LANTERN? */
		if (GUI_bool(get_tx(16)))
		{
			/* get a LANTERN */
			if (get_item(ITEM_LATERNE__UNLIT, 1, 1))
			{
				/* mark LANTERN as taken */
				gs_dng04_lantern_flag = 1;
			}
		}

	} else if (pos == DNG_POS(0,10,2) && pos != gs_dng_handled_pos)
	{
		/* a very deep gap */
		i = 1;
		hero = get_first_hero_available_in_group();

		if (!gs_dng04_deepgap_flag || (test_attrib(hero, ATTRIB_GE, 0) > 0))
		{
			gs_dng04_deepgap_flag = 1;

			i = GUI_bool(get_tx(17));
		}

		if (i)
		{
			if (test_attrib(hero, ATTRIB_GE, 0) > 0)
			{
				sprintf(g_dtp2,	get_tx(18), hero->alias,
					GUI_get_ptr(hero->sex, 3),
					GUI_get_ptr(hero->sex, 0));
				GUI_output(g_dtp2);

			} else {

				sprintf(g_dtp2,	get_tx(19), hero->alias,
					GUI_get_ptr(hero->sex, 0), hero->alias);

				GUI_output(g_dtp2);

				hero_disappear(hero, 0, -1);
			}
		}
	}

	if (pos == DNG_POS(0,6,1) && pos != gs_dng_handled_pos && !gs_dng04_corpse2_flag)
	{
		/* DNG04_corpse2 */
		loot_special_chest(0);

	} else if ((pos == DNG_POS(0,13,6) || pos == DNG_POS(0,14,6) || pos == DNG_POS(0,14,5)) && pos != gs_dng_handled_pos)
	{
		/* Probability of 5% to get bitten by a rat */
		if (mod_day_timer(MINUTES(10)))
		{
			hero = get_hero(0);
			for (i = 0; i <= 6 ; i++, hero++)
			{
				if ((random_schick(100) <= 5) &&
					(hero->typus != HERO_TYPE_NONE) &&
					(hero->group_id == gs_active_group_id) &&
					!hero->flags.dead)
				{
					sub_hero_le(hero, 2);

					sprintf(g_text_output_buf, get_tx(21), hero->alias);
					GUI_output(g_text_output_buf);
				}
			}
		}

	} else if (g_visual_field_vals[1] == 2)
	{
		/* Probability of 10% to get bitten by a rat */
		if (mod_day_timer(MINUTES(10)))
		{
			hero = get_hero(0);
			for (i = 0; i <= 6 ; i++, hero++)
			{
				/* Original-Bug: forgot to check if the hero is dead */
				if ((random_schick(100) <= 10) &&
					(hero->typus != HERO_TYPE_NONE) &&
					(hero->group_id == gs_active_group_id))
				{
					sub_hero_le(hero, 2);

					sprintf(g_text_output_buf, get_tx(25), hero->alias);
					GUI_output(g_text_output_buf);
				}
			}
		}

	} else if (pos == DNG_POS(0,7,15) && pos != gs_dng_handled_pos)
	{
		/* the exit of this dungeon */
		leave_dungeon();

		gs_town_id = gs_travel_destination_town_id;
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 0x03);

		sprintf(g_dtp2, get_tx(26), get_ttx(gs_trv_destination + 0xeb));
		GUI_output(g_dtp2);

		timewarp(HOURS(2));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = pos;

	return 0;
}

void DNG04_chest00_open(struct struct_chest* chest)
{
	loot_corpse(chest, get_tx(1), (int8_t*)&gs_dng04_corpse0_flag);
}

void DNG04_chest01_open(struct struct_chest* chest)
{
	loot_corpse(chest, get_tx(8), (int8_t*)&gs_dng04_corpse1_flag);
}

void DNG04_chest02_open(struct struct_chest* chest)
{
	loot_corpse(chest, get_tx(20), (int8_t*)&gs_dng04_corpse2_flag);
}

void DNG04_chest00_loot(struct struct_chest* chest)
{
	uint8_t* bak = chest->content;

	chest->content = gs_dng04_chest_corpse0;

	loot_chest(chest, get_tx(22), get_tx(6));

	chest->content = bak;
}

void DNG04_chest01_loot(struct struct_chest* chest)
{
	uint8_t* bak = chest->content;

	chest->content = gs_dng04_chest_corpse1;

	loot_chest(chest, get_tx(23), get_tx(6));

	chest->content = bak;
}

void DNG04_chest02_loot(struct struct_chest* chest)
{
	uint8_t* bak = chest->content;

	chest->content = gs_dng04_chest_corpse2;

	loot_chest(chest, get_tx(24), get_tx(6));

	chest->content = bak;
}

/**
 * \brief   dungeon handler of a cave
 */
signed short DNG05_handler(void)
{
	signed short pos;
	signed short tmp;
	signed short tw_bak;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	pos = DNG_POS(gs_dungeon_level, gs_x_target, + gs_y_target);

	if (pos == DNG_POS(0,7,14) && pos != gs_dng_handled_pos && !g_dng05_trash_flag)
	{
		if (GUI_bool(get_tx(1)) && GUI_bool(get_tx(15)))
		{
			g_dng05_trash_flag = 1;

			hero = get_first_hero_available_in_group();

			GUI_output(get_tx(16));

			tmp = get_free_mod_slot();
			set_mod_slot(tmp, DAYS(1), (uint8_t*)&hero->attrib[ATTRIB_CH].current, -5, 0);

			add_party_money(20L);
		}

	} else if (pos == DNG_POS(0,5,12) && pos != gs_dng_handled_pos && !gs_dng05_proviant_flag)
	{
		if (GUI_bool(get_tx(2)))
		{
			GUI_output(get_tx(3));

			get_item(ITEM_PROVIANTPAKET, 1, 10);

			gs_dng05_proviant_flag = 1;
		}

	} else if (pos == DNG_POS(0,9,7) && pos != gs_dng_handled_pos && !gs_dng05_bats_flag)
	{
		if (GUI_bool(get_tx(4)))
		{
			GUI_output(get_tx(5));

			gs_dng05_bats_flag = 1;
		}

	} else if (pos == DNG_POS(0,3,9) && pos != gs_dng_handled_pos && !gs_dng05_god_flag)
	{
		do {
			tmp = GUI_radio(get_tx(6), 2, get_tx(7), get_tx(8));

		} while (tmp == -1);

		gs_gods_estimation[GOD_TRAVIA] += (tmp == 1 ? -15 : 15);

		GUI_output(get_tx(9));

		gs_dng05_god_flag = 1;

	} else if (pos == DNG_POS(0,8,5) && pos != gs_dng_handled_pos)
	{
		if (random_schick(100) < 30)
		{
			g_max_enemies = (random_schick(100) < 10 ? 3 : 2);
			g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,8,7,SOUTH);

			do_fight(FIGHTS_F061_4B);
		}

	} else if (pos == DNG_POS(0,11,1) && pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(11));

	} else if (pos == DNG_POS(0,6,1) && pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(12)))
		{
			hero = get_first_hero_available_in_group();

			sprintf(g_dtp2,	get_tx(13), hero->alias);
			GUI_output(g_dtp2);

			sub_hero_le(hero, dice_roll(1, 3, 2));

			hero_disease_test(hero, 1, 65);
		}

	} else if (pos == DNG_POS(0,3,14) && pos != gs_dng_handled_pos)
	{
		load_ani(32);
		init_ani(1);

		GUI_output(get_tx(14));

		disable_ani();

		gs_x_target = 5;
		g_area_prepared = AREA_TYPE_NONE;

	} else if (pos == DNG_POS(0,6,15) && pos != gs_dng_handled_pos)
	{
		/* the exit of this dungeon */
		leave_dungeon();

		gs_town_id = gs_travel_destination_town_id;
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 0x03);

		sprintf(g_dtp2, get_tx(17), get_ttx(gs_trv_destination + 0xeb));
		GUI_output(g_dtp2);

		timewarp(HOURS(2));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = pos;

	return 0;
}

/**
 * \brief   print introductions of dungeon #5 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG5_fight_intro(signed short fight_id)
{
	if (fight_id == 76) {
		GUI_output(get_tx(10));
	}
}

#if !defined(__BORLANDC__)
}
#endif
