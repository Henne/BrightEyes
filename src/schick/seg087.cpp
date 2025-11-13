/**
 *	Rewrite of DSA1 v3.02_de functions of seg087 (dungeon: thorwal 1 / 2)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg087.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg029.h"
#include "seg031.h"
#include "seg032.h"
#include "seg047.h"
#include "seg075.h"
#include "seg088.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed int DNG14_handler(void)
{
	signed int pos;
	signed int tmp; /* multiple use: mod_slot, item_count, result of skill test */
	signed int hero_pos;
	signed int tw_bak;
	int32_t p_money;
	struct struct_hero *hero;
	uint8_t *amap_ptr;

	amap_ptr = g_dng_map;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	pos = (gs_dungeon_level << 12) + (gs_x_target << 8) + gs_y_target;

	hero = get_first_hero_available_in_group();

	if (!(gs_day_timer % MINUTES(10)) &&
		(gs_dng14_poisontrap == 2) &&
		(pos == DNG_POS(0,13,6) || pos == DNG_POS(0,14,6) || pos == DNG_POS(0,13,7) || pos == DNG_POS(0,14,7)))
	{
		sub_group_le(1);
	}

	if (pos == DNG_POS(0,2,11) && pos != gs_dng_handled_pos && random_schick(100) <= 10) {

		g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = DNG_POS_DIR(0,3,8,EAST);
		g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,2,14,SOUTH);

		do_fight(FIGHTS_DTHO03);

	} else if (pos == DNG_POS(0,2,4) && pos != gs_dng_handled_pos && !gs_dng14_money_flag) {

		if (GUI_bool(get_tx(1))) {

			/* mark as visited */
			gs_dng14_money_flag = 1;

			/* add money */
			p_money = get_party_money();
			p_money += 2210L;
			set_party_money(p_money);

			/* set gods estimation for PHEX to -100 */
			gs_gods_estimation[GOD_PHEX] = -100L;

			GUI_output(get_tx(2));
		}

	} else if (pos == DNG_POS(0,5,3) && pos != gs_dng_handled_pos && !gs_dng14_alarm_flag) {

		if (test_skill(hero, TA_SINNESSCHAERFE, 6) <= 0) {

			GUI_output(get_tx(3));

			gs_dng14_alarm_flag = 1;
		}

	} else if (pos == DNG_POS(0,6,2) && pos != gs_dng_handled_pos && gs_dng14_alarm_flag) {

		g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,5,4,SOUTH);

		do_fight(FIGHTS_DTHO06);

	} else if (pos == DNG_POS(0,7,6) && pos != gs_dng_handled_pos && gs_direction == NORTH) {

		loot_multi_chest(gs_dng14_chest_pantry, get_tx(4));

	} else if (pos == DNG_POS(0,6,11) && pos != gs_dng_handled_pos && gs_direction == EAST) {

		loot_multi_chest(gs_dng14_chest_gear, get_tx(5));

	} else if ((pos == DNG_POS(0,9,13) || pos == DNG_POS(0,9,9)) && pos != gs_dng_handled_pos && random_schick(100) <= 20) {

		g_fig_escape_position[NORTH] = DNG_POS_DIR(0,9,8,NORTH);
		g_fig_escape_position[EAST] = DNG_POS_DIR(0,11,11,EAST);
		g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,9,14,SOUTH);

		do_fight(FIGHTS_DTHO09);

	} else if (pos == DNG_POS(0,11,14) && pos != gs_dng_handled_pos && random_schick(100) <= 10) {

		g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,8,14,WEST);

		do_fight(FIGHTS_DTHO10);

	} else if (pos == DNG_POS(0,11,14) && pos != gs_dng_handled_pos && gs_direction == EAST) {

		GUI_output(get_tx(6));

	} else if (pos == DNG_POS(0,13,7) && pos != gs_dng_handled_pos && (gs_dng14_poisontrap != 0)) {

		GUI_output(get_tx(10));

		GUI_output(get_tx(11));

		gs_dng14_poisontrap = 2;

		amap_ptr[MAP_POS(12,7)] |= DNG_TILE_REMOVABLE_WALL << 4;

	} else if (pos == DNG_POS(0,13,5) && pos != gs_dng_handled_pos) {

		gs_dng14_poisontrap = 0;

	} else if (pos == DNG_POS(0,11,10) && pos != gs_dng_handled_pos && random_schick(100) <= 10) {

		g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,9,11,WEST);

		do_fight(FIGHTS_DTHO13);

	} else if (pos == DNG_POS(0,10,3) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(12));

	} else if (pos == DNG_POS(0,4,11) && (pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST && gs_dng14_secretdoor1 != 2) {

		if (gs_dng14_secretdoor1 != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0) {
			gs_dng14_secretdoor1 = 1;

			sprintf(g_dtp2, get_tx(7), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((tmp = test_skill(hero, TA_SCHLOESSER, -6)) > 0 ? get_tx(8): get_tx(9)),
				GUI_get_ptr(hero->sex, 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (tmp > 0) {
				amap_ptr[MAP_POS(5,11)] &= (DNG_TILE_CORRIDOR << 4) + 0x0f;
				gs_dng14_secretdoor1 = 2;
				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (pos == DNG_POS(0,11,7) &&
			(pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST &&
			gs_dng14_secretdoor2 != 2) {

		if (gs_dng14_secretdoor2 != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0) {
			gs_dng14_secretdoor2 = 1;

			sprintf(g_dtp2, get_tx(7), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((tmp = test_skill(hero, TA_SCHLOESSER, 6)) > 0 ? get_tx(8): get_tx(9)),
				GUI_get_ptr(hero->sex, 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (tmp > 0) {
				amap_ptr[MAP_POS(12,7)] &= (DNG_TILE_CORRIDOR << 4) + 0x0f;
				gs_dng14_secretdoor2 = 2;
				DNG_update_pos();
			} else {
				gs_dng14_poisontrap = 1;
			}

			gs_direction_bak = gs_direction;
		}

	} else if (pos == DNG_POS(1,1,8) && pos != gs_dng_handled_pos && random_schick(100) <= 30) {

		g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,1,6,NORTH);

		do_fight(FIGHTS_DTHO18);

	} else if (pos == DNG_POS(1,1,11) && pos != gs_dng_handled_pos && random_schick(100) <= 5) {

		g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = DNG_POS_DIR(1,1,9,NORTH);
		g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,1,14,SOUTH);

		do_fight(FIGHTS_DTHO19);

	} else if (pos == DNG_POS(1,1,14) && pos != gs_dng_handled_pos && random_schick(100) <= 10) {

		g_fig_escape_position[NORTH] = DNG_POS_DIR(1,1,12,NORTH);
		g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = DNG_POS_DIR(1,5,14,EAST);
		g_fig_escape_position[WEST] = DNG_POS_DIR(1,1,14,NORTH);

		do_fight(FIGHTS_DTHO20);

	} else if (pos == DNG_POS(1,13,13) && pos != gs_dng_handled_pos) {

		/* a Ladder */

		if (GUI_bool(get_tx(13))) {

			GUI_output(get_tx(14));

			gs_y_target++;
			gs_direction = SOUTH;
			DNG_update_pos();
		}

	} else if ((pos == DNG_POS(1,10,8) || pos == DNG_POS(1,11,10) || pos == DNG_POS(1,7,10) || pos == DNG_POS(1,9,12)) &&
		 pos != gs_dng_handled_pos && !gs_dng14_lvl2_fight) {

		/* the main fight in Level 2 */

		g_fig_escape_position[NORTH] = DNG_POS_DIR(1,10,6,NORTH);
		g_fig_escape_position[EAST] = DNG_POS_DIR(1,11,10,EAST);
		g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,9,14,SOUTH);

		if (!do_fight(random_schick(100) <= 50 ? FIGHTS_DTHO21A : FIGHTS_DTHO21B))
		{
			gs_dng14_lvl2_fight = 1;
		}

	} else if (pos == DNG_POS(1,8,9) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(15));

	} else if (pos == DNG_POS(1,10,6) && pos != gs_dng_handled_pos && random_schick(100) <= 50) {

		g_fig_escape_position[NORTH] = DNG_POS_DIR(1,10,1,NORTH);
		g_fig_escape_position[EAST] = DNG_POS_DIR(1,12,6,EAST);
		g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,10,9,SOUTH);

		do_fight(FIGHTS_DTHO23);

	} else if ((pos == DNG_POS(1,8,2) || pos == DNG_POS(1,8,5)) && pos != gs_dng_handled_pos && gs_direction == WEST) {

		GUI_output(get_tx(16));

	} else if (pos == DNG_POS(1,12,2) && pos != gs_dng_handled_pos && gs_direction == EAST) {

		GUI_output(get_tx(16));

	} else if (pos == DNG_POS(1,12,6) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(16));

		if (!gs_dng14_booty_flag) {

			loot_multi_chest(gs_dng14_chest_barrel, get_tx(17));

			gs_dng14_booty_flag = 1;
		}

	} else if (pos == DNG_POS(1,12,10) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(18));

	} else if (pos == DNG_POS(1,13,10) && pos != gs_dng_handled_pos && !gs_dng14_ugdalf_done) {

		GUI_output(get_tx(19));

		gs_dng14_ugdalf_done = 1;

		gs_quest_ugdalf = 3;

	} else if (pos == DNG_POS(1,1,9) &&
		(pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
		gs_direction == SOUTH && gs_dng14_secretdoor3 != 2) {

		if (gs_dng14_secretdoor3 != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0) {

			/* acticate trap */
			gs_dng14_secretdoor3 = 1;

			sprintf(g_dtp2, get_tx(7), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((tmp = test_skill(hero, TA_SCHLOESSER, -6)) > 0 ? get_tx(8) : get_tx(9)),
				GUI_get_ptr(hero->sex, 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (tmp > 0) {
				/* test was sucessful => disable trap */
				amap_ptr[MAP_POS(1,10)] &= (DNG_TILE_CORRIDOR << 4) + 0x0f;
				gs_dng14_secretdoor3 = 2;

				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (pos == DNG_POS(1,7,10) &&
		(pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
		gs_direction == WEST && gs_dng14_secretdoor4 != 2) {

		if (gs_dng14_secretdoor4 != 0 || test_skill(hero, TA_SINNESSCHAERFE, 8) > 0) {

			/* acticate trap */
			gs_dng14_secretdoor4 = 1;

			sprintf(g_dtp2, get_tx(7), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((tmp = test_skill(hero, TA_SCHLOESSER, 7)) > 0 ? get_tx(8) : get_tx(9)),
				GUI_get_ptr(hero->sex, 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (tmp > 0) {
				/* test was sucessful => disable trap */
				amap_ptr[MAP_POS(6,10)] &= (DNG_TILE_CORRIDOR << 4) + 0x0f;
				gs_dng14_secretdoor4 = 2;

				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (pos == DNG_POS(2,3,6) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(20));


	} else if (pos == DNG_POS(2,4,13) && pos != gs_dng_handled_pos) {

		if (GUI_bool(get_tx(21))) {

			tmp = group_count_item(ITEM_SEIL);
			tmp += group_count_item(ITEM_STRICKLEITER);

			hero = get_hero(0);
			for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero++) {

				if ((hero->typus != HERO_TYPE_NONE) &&
					(hero->group_id == gs_active_group_id) &&
					!hero->flags.dead &&
					(hero->typus == HERO_TYPE_MAGIER) &&
					(hero->staff_level > 2))
				{
					/* mage with staffspell-level > 2 => can transform staff to rope */
					tmp++;
				}
			}

			do {
				hero_pos = GUI_radio(get_tx(22), (tmp >= 5 ? 3 : 2), get_tx(23), get_tx(24), get_tx(25));

			} while (hero_pos == -1);

			if (hero_pos == 3) {

				hero = get_hero(0);
				for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero++) {

					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
						!hero->flags.dead && (test_skill(hero, TA_KLETTERN, 0) <= 0))
					{
						sprintf(g_dtp2, get_tx(26), hero->alias, GUI_get_ptr(hero->sex, 2));
						GUI_output(g_dtp2);

						/* 1W6 damage */
						sub_hero_le(hero, random_schick(6));
					}
				}

				/* drop down to level 4 */
				gs_y_target--;
				DNG_inc_level();

			} else if (hero_pos == 1) {

				hero = get_hero(0);
				for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero++) {

					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
						!hero->flags.dead && (test_skill(hero, TA_KLETTERN, 4) <= 0))
					{
						sprintf(g_dtp2, get_tx(27), hero->alias);
						GUI_output(g_dtp2);

						/* 2W6+4 damage */
						sub_hero_le(hero, dice_roll(2, 6, 4));
					}
				}

				/* drop down to level 4 */
				gs_y_target--;
				DNG_inc_level();
			}
		}

	} else if (pos == DNG_POS(2,2,14) && pos != gs_dng_handled_pos && !gs_dng14_torches_flag) {

		if (GUI_bool(get_tx(28))) {

			gs_dng14_torches_flag = 1;

			give_new_item_to_group(ITEM_FACKEL__UNLIT, 1, 4);
		}

	} else if (pos == DNG_POS(2,3,1) && pos != gs_dng_handled_pos && !gs_dng14_spooky_flag) {
		gs_dng14_spooky_flag = 1;

		GUI_output(get_tx(29));

		hero = get_hero(0);
		for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero++) {

			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
				!hero->flags.dead && test_attrib(hero, ATTRIB_TA, 0) <= 0)
			{
				tmp = get_free_mod_slot();

				set_mod_slot(tmp, HOURS(6), (uint8_t*)&hero->attrib[ATTRIB_MU].current, -2, (signed char)hero_pos);
			}
		}

	} else if (pos == DNG_POS(2,1,2) && pos != gs_dng_handled_pos && !gs_dng14_spear_flag) {

		if (GUI_bool(get_tx(30))) {

			gs_dng14_spear_flag = 1;

			give_new_item_to_group(ITEM_SPEER, 1, 1);
		}


	} else if (pos == DNG_POS(2,9,7) && pos != gs_dng_handled_pos) {

		if (GUI_bool(get_tx(31))) {

			timewarp(HOURS(12));

			GUI_output(get_tx(32));
		}

		gs_x_target = gs_x_target_bak;
		gs_y_target = gs_y_target_bak;

	} else if (pos == DNG_POS(2,7,7) && pos != gs_dng_handled_pos) {

		GUI_dialog_na(60, get_tx(33));

	} else if (pos == DNG_POS(2,13,7) && pos != gs_dng_handled_pos) {

		GUI_dialog_na(60, get_tx(34));

	} else if (pos == DNG_POS(2,14,5) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(36));

		/* each hero gets 2W6 damage */
		sub_group_le(dice_roll(2, 6, 0));

	} else if (pos == DNG_POS(2,14,3) && pos != gs_dng_handled_pos && gs_direction == NORTH) {

		GUI_output(get_tx(37));

	} else if (pos == DNG_POS(2,14,5) &&
			(pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == NORTH &&
			gs_dng14_secretdoor5 != 2 &&
			(gs_dng14_secretdoor5 != 0 || test_skill(hero, TA_SINNESSCHAERFE, 6) > 0))
	{
			gs_dng14_secretdoor5 = 1;

			sprintf(g_dtp2,	get_tx(7), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((tmp = test_skill(hero, TA_SCHLOESSER, 4)) > 0 ? get_tx(8) : get_tx(9)),
				GUI_get_ptr(hero->sex, 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (tmp > 0) {
				/* disable trap */
				amap_ptr[MAP_POS(14,4)] &= (DNG_TILE_CORRIDOR << 4) + 0x0f;

				gs_dng14_secretdoor5 = 2;

				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
	}

	if (pos == DNG_POS(3,4,12) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(35));

	} else if (((pos == DNG_POS(3,10,10) && gs_direction == EAST) || (pos == 0x3e0a && gs_direction == WEST)) &&
			pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(38))) {

			if (get_first_hero_with_item(ITEM_SEIL) != - 1 || get_first_hero_with_item(ITEM_STRICKLEITER) != -1) /* TODO: why not staffspell >= 3 ? */
			{

				if ((hero_pos = select_hero_ok(get_tx(39))) != -1) {

					hero = get_hero(hero_pos);

					if (test_skill(hero, TA_SCHWIMMEN, 8) <= 0) {

						sprintf(g_dtp2, get_tx(40), hero->alias, GUI_get_ptr(hero->sex, 0));
						GUI_output(g_dtp2);

						/* 1W6 damage */
						sub_hero_le(hero, random_schick(6));

						gs_x_target = (pos == DNG_POS(3,10,10) ? 9 : 13);

						if (hero->flags.dead) {

							sprintf(g_dtp2,	get_tx(63), hero->alias);
							GUI_output(g_dtp2);

							draw_status_line();
						}
					} else {
						sprintf(g_dtp2, get_tx(41), hero->alias, GUI_get_ptr(hero->sex, 0));
						GUI_output(g_dtp2);

						DNG14_dive(hero_pos, 2, (pos == DNG_POS(3,10,10) ? 13 : 9));
					}
				}
			} else {
				/* no tools for diving */
				DNG14_dive(-1, 8, (pos == DNG_POS(3,10,10) ? 13 : 9));
			}
		}

	} else if (pos == DNG_POS(3,6,11) && pos != gs_dng_handled_pos) {

		GUI_dialog_na(55, get_tx(44));

	} else if ((pos == DNG_POS(3,4,1) || pos == DNG_POS(3,12,7) || pos == DNG_POS(3,1,3) || pos == DNG_POS(3,6,7)) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(45));

	} else if (pos == DNG_POS(3,4,13) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(47));

	} else if (pos == DNG_POS(3,3,3) && pos != gs_dng_handled_pos && !gs_dng14_hatchet_flag) {

		if (GUI_bool(get_tx(46))) {

			gs_dng14_hatchet_flag = 1;

			give_new_item_to_group(ITEM_HACKE, 1, 1);
		}

	} else if (pos == DNG_POS(3,11,13) && pos != gs_dng_handled_pos && gs_direction == EAST) {

		GUI_output(get_tx(48));

	} else if (pos == DNG_POS(3,12,12) && pos != gs_dng_handled_pos) {

		sprintf(g_dtp2, get_tx(49), hero->alias, hero->alias);
		GUI_output(g_dtp2);

		/* 1W6 damage */
		sub_hero_le(hero, 3);

	} else if (pos == DNG_POS(3,7,3) && pos != gs_dng_handled_pos && !gs_dng14_ring_flag) {

		if (GUI_bool(get_tx(50))) {

			gs_dng14_ring_flag = 1;

			give_new_item_to_group(ITEM_RING__BLUE, 1, 1);
		}

	} else if (pos == DNG_POS(3,11,1) && pos != gs_dng_handled_pos) {

		if (GUI_bool(get_tx(51))) {

			GUI_output(get_tx(52));

			if (!gs_dng14_cellarexit_flag) {

				GUI_output(get_tx(53));
				GUI_output(get_tx(54));
				GUI_output(get_tx(55));

				drink_while_drinking(100);

				gs_dng14_cellarexit_flag = 1;
			}

			leave_dungeon();
			gs_x_target = 22;
			gs_y_target = 5;
			gs_current_loctype = LOCTYPE_NONE;
			gs_direction = SOUTH;
		}

	} else if (pos == DNG_POS(4,5,13) && pos != gs_dng_handled_pos && !gs_dng14_orknase_flag) {

		if (GUI_bool(get_tx(57))) {

			gs_dng14_orknase_flag = 1;

			give_new_item_to_group(ITEM_ORKNASE, 1, 1);
		}

	} else if (pos == DNG_POS(4,12,9) && pos != gs_dng_handled_pos && gs_direction == SOUTH) {

		GUI_output(get_tx(58));

	} else if (pos == DNG_POS(4,5,9) && pos != gs_dng_handled_pos && gs_direction == NORTH) {

		if (!GUI_bool(get_tx(59))) {

			gs_x_target = gs_x_target_bak;
			gs_y_target = gs_y_target_bak;

			GUI_output(get_tx(60));
		}

	} else if (pos == DNG_POS(0,0,14) && pos != gs_dng_handled_pos) {
		/* regular exit */
		leave_dungeon();
		gs_x_target = 2;
		gs_y_target = 13;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = NORTH;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = pos;

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
