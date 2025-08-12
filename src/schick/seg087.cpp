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

signed short DNG14_handler(void)
{
	signed short pos;
	signed short l_di;
	signed short hero_pos;
	signed short tw_bak;
	Bit32s p_money;
	Bit8u *hero;
	Bit8u *amap_ptr;

	amap_ptr = g_dng_map;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	pos = (gs_dungeon_level << 12) + (gs_x_target << 8) + gs_y_target;

	hero = (Bit8u*)get_first_hero_available_in_group();

	if (!(gs_day_timer % MINUTES(10)) &&
		ds_readb(DNG14_POISONTRAP) == 2 &&
		(pos == DNG_POS(0,13,6) || pos == DNG_POS(0,14,6) || pos == DNG_POS(0,13,7) || pos == DNG_POS(0,14,7)))
	{
		sub_group_le(1);

	}
	if (pos == DNG_POS(0,2,11) && pos != gs_dng_handled_pos && random_schick(100) <= 10) {

		ds_writew(FIG_FLEE_POSITION, ds_writew((FIG_FLEE_POSITION + 2), DNG_POS_DIR(0,3,8,EAST)));
		ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,2,14,SOUTH)));

		do_fight(FIGHTS_DTHO03);

	} else if (pos == DNG_POS(0,2,4) && pos != gs_dng_handled_pos && !ds_readb(DNG14_MONEY_FLAG)) {

		if (GUI_bool(get_tx(1))) {

			/* mark as visited */
			ds_writeb(DNG14_MONEY_FLAG, 1);

			/* add money */
			p_money = get_party_money();
			p_money += 2210L;
			set_party_money(p_money);

			/* set gods estimation for PHEX to -100 */
			gs_gods_estimation[GOD_PHEX] = -100L;

			GUI_output(get_tx(2));
		}

	} else if (pos == DNG_POS(0,5,3) && pos != gs_dng_handled_pos && !ds_readb(DNG14_ALARM_FLAG)) {

		if (test_skill(hero, TA_SINNESSCHAERFE, 6) <= 0) {

			GUI_output(get_tx(3));

			ds_writeb(DNG14_ALARM_FLAG, 1);
		}

	} else if (pos == DNG_POS(0,6,2) && pos != gs_dng_handled_pos && ds_readb(DNG14_ALARM_FLAG) != 0) {

		ds_writew(FIG_FLEE_POSITION, ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,5,4,SOUTH)))));

		do_fight(FIGHTS_DTHO06);

	} else if (pos == DNG_POS(0,7,6) && pos != gs_dng_handled_pos && gs_direction == NORTH) {

		loot_multi_chest(p_datseg + DNG14_CHEST_PANTRY, get_tx(4));

	} else if (pos == DNG_POS(0,6,11) && pos != gs_dng_handled_pos && gs_direction == EAST) {

		loot_multi_chest(p_datseg + DNG14_CHEST_GEAR, get_tx(5));

	} else if ((pos == DNG_POS(0,9,13) || pos == DNG_POS(0,9,9)) && pos != gs_dng_handled_pos && random_schick(100) <= 20) {

		ds_writew(FIG_FLEE_POSITION, DNG_POS_DIR(0,9,8,NORTH));
		ds_writew((FIG_FLEE_POSITION + 2), DNG_POS_DIR(0,11,11,EAST));
		ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,9,14,SOUTH)));

		do_fight(FIGHTS_DTHO09);

	} else if (pos == DNG_POS(0,11,14) && pos != gs_dng_handled_pos && random_schick(100) <= 10) {

		ds_writew(FIG_FLEE_POSITION, ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,8,14,WEST)))));

		do_fight(FIGHTS_DTHO10);

	} else if (pos == DNG_POS(0,11,14) && pos != gs_dng_handled_pos && gs_direction == EAST) {

		GUI_output(get_tx(6));

	} else if (pos == DNG_POS(0,13,7) && pos != gs_dng_handled_pos && ds_readb(DNG14_POISONTRAP) != 0) {

		GUI_output(get_tx(10));

		GUI_output(get_tx(11));

		ds_writeb(DNG14_POISONTRAP, 2);

		or_ptr_bs(amap_ptr + MAP_POS(12,7), DNG_TILE_REMOVABLE_WALL << 4);

	} else if (pos == DNG_POS(0,13,5) && pos != gs_dng_handled_pos) {

		ds_writeb(DNG14_POISONTRAP, 0);

	} else if (pos == DNG_POS(0,11,10) && pos != gs_dng_handled_pos && random_schick(100) <= 10) {

		ds_writew(FIG_FLEE_POSITION, ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,9,11,WEST)))));

		do_fight(FIGHTS_DTHO13);

	} else if (pos == DNG_POS(0,10,3) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(12));

	} else if (pos == DNG_POS(0,4,11) &&
			(pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST &&
			ds_readb(DNG14_SECRETDOOR1) != 2) {

		if (ds_readb(DNG14_SECRETDOOR1) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0) {

			ds_writeb(DNG14_SECRETDOOR1, 1);

			sprintf(g_dtp2, get_tx(7), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((l_di = test_skill(hero, TA_SCHLOESSER, -6)) > 0 ? get_tx(8): get_tx(9)),
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (l_di > 0) {
				and_ptr_bs(amap_ptr + MAP_POS(5,11), (DNG_TILE_CORRIDOR << 4) + 0x0f);
				ds_writeb(DNG14_SECRETDOOR1, 2);
				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (pos == DNG_POS(0,11,7) &&
			(pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST &&
			ds_readb(DNG14_SECRETDOOR2) != 2) {

		if (ds_readb(DNG14_SECRETDOOR2) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0) {

			ds_writeb(DNG14_SECRETDOOR2, 1);

			sprintf(g_dtp2, get_tx(7), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((l_di = test_skill(hero, TA_SCHLOESSER, 6)) > 0 ? get_tx(8): get_tx(9)),
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (l_di > 0) {
				and_ptr_bs(amap_ptr + MAP_POS(12,7), (DNG_TILE_CORRIDOR << 4) + 0x0f);
				ds_writeb(DNG14_SECRETDOOR2, 2);
				DNG_update_pos();
			} else {
				ds_writeb(DNG14_POISONTRAP, 1);
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (pos == DNG_POS(1,1,8) && pos != gs_dng_handled_pos && random_schick(100) <= 30) {

		ds_writew(FIG_FLEE_POSITION, ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,1,6,NORTH)))));

		do_fight(FIGHTS_DTHO18);

	} else if (pos == DNG_POS(1,1,11) && pos != gs_dng_handled_pos && random_schick(100) <= 5) {

		ds_writew(FIG_FLEE_POSITION, ds_writew((FIG_FLEE_POSITION + 2), DNG_POS_DIR(1,1,9,NORTH)));
		ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,1,14,SOUTH)));

		do_fight(FIGHTS_DTHO19);

	} else if (pos == DNG_POS(1,1,14) && pos != gs_dng_handled_pos && random_schick(100) <= 10) {

		ds_writew(FIG_FLEE_POSITION, DNG_POS_DIR(1,1,12,NORTH));
		ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), DNG_POS_DIR(1,5,14,EAST)));
		ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,1,14,NORTH));

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
		 pos != gs_dng_handled_pos && !ds_readb(DNG14_LVL2_FIGHT)) {

		/* the main fight in Level 2 */

		ds_writew(FIG_FLEE_POSITION, DNG_POS_DIR(1,10,6,NORTH));
		ds_writew((FIG_FLEE_POSITION + 2), DNG_POS_DIR(1,11,10,EAST));
		ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,9,14,SOUTH)));

		if (!do_fight(random_schick(100) <= 50 ? FIGHTS_DTHO21A : FIGHTS_DTHO21B)) {
			ds_writeb(DNG14_LVL2_FIGHT, 1);
		}

	} else if (pos == DNG_POS(1,8,9) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(15));

	} else if (pos == DNG_POS(1,10,6) && pos != gs_dng_handled_pos && random_schick(100) <= 50) {

		ds_writew(FIG_FLEE_POSITION, DNG_POS_DIR(1,10,1,NORTH));
		ds_writew((FIG_FLEE_POSITION + 2), DNG_POS_DIR(1,12,6,EAST));
		ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,10,9,SOUTH)));

		do_fight(FIGHTS_DTHO23);

	} else if ((pos == DNG_POS(1,8,2) || pos == DNG_POS(1,8,5)) && pos != gs_dng_handled_pos && gs_direction == WEST) {

		GUI_output(get_tx(16));

	} else if (pos == DNG_POS(1,12,2) && pos != gs_dng_handled_pos && gs_direction == EAST) {

		GUI_output(get_tx(16));

	} else if (pos == DNG_POS(1,12,6) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(16));

		if (!ds_readb(DNG14_BOOTY_FLAG)) {

			loot_multi_chest(p_datseg + DNG14_CHEST_BARREL, get_tx(17));

			ds_writeb(DNG14_BOOTY_FLAG, 1);
		}

	} else if (pos == DNG_POS(1,12,10) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(18));

	} else if (pos == DNG_POS(1,13,10) && pos != gs_dng_handled_pos && !gs_dng14_ugdalf_done) {

		GUI_output(get_tx(19));

		gs_dng14_ugdalf_done = 1;

		gs_quest_ugdalf = 3;

	} else if (pos == DNG_POS(1,1,9) &&
		(pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
		gs_direction == SOUTH &&
		ds_readb(DNG14_SECRETDOOR3) != 2) {

		if (ds_readb(DNG14_SECRETDOOR3) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0) {

			/* acticate trap */
			ds_writeb(DNG14_SECRETDOOR3, 1);

			sprintf(g_dtp2, get_tx(7), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((l_di = test_skill(hero, TA_SCHLOESSER, -6)) > 0 ? get_tx(8) : get_tx(9)),
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (l_di > 0) {
				/* test was sucessful => disable trap */
				and_ptr_bs(amap_ptr + MAP_POS(1,10), (DNG_TILE_CORRIDOR << 4) + 0x0f);
				ds_writeb(DNG14_SECRETDOOR3, 2);
				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (pos == DNG_POS(1,7,10) &&
		(pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
		gs_direction == WEST &&
		ds_readb(DNG14_SECRETDOOR4) != 2) {

		if (ds_readb(DNG14_SECRETDOOR4) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 8) > 0) {

			/* acticate trap */
			ds_writeb(DNG14_SECRETDOOR4, 1);

			sprintf(g_dtp2, get_tx(7), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((l_di = test_skill(hero, TA_SCHLOESSER, 7)) > 0 ? get_tx(8) : get_tx(9)),
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (l_di > 0) {
				/* test was sucessful => disable trap */
				and_ptr_bs(amap_ptr + MAP_POS(6,10), (DNG_TILE_CORRIDOR << 4) + 0x0f);
				ds_writeb(DNG14_SECRETDOOR4, 2);
				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (pos == DNG_POS(2,3,6) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(20));


	} else if (pos == DNG_POS(2,4,13) && pos != gs_dng_handled_pos) {

		if (GUI_bool(get_tx(21))) {

			l_di = group_count_item(ITEM_ROPE);
			l_di += group_count_item(ITEM_ROPE_LADDER);

			hero = get_hero(0);
			for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero += SIZEOF_HERO) {

				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero) &&
					host_readbs(hero + HERO_TYPE) == HERO_TYPE_MAGE &&
					host_readbs(hero + HERO_STAFFSPELL_LVL) > 2)
				{
					/* mage with staffspell-level > 2 => can transform staff to rope */
					l_di++;
				}
			}

			do {
				hero_pos = GUI_radio(get_tx(22), (l_di >= 5 ? 3 : 2), get_tx(23), get_tx(24), get_tx(25));

			} while (hero_pos == -1);

			if (hero_pos == 3) {

				hero = get_hero(0);
				for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero += SIZEOF_HERO) {

					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
						!hero_dead(hero) &&
						test_skill(hero, TA_KLETTERN, 0) <= 0)
					{
						sprintf(g_dtp2, get_tx(26), (char*)hero + HERO_NAME2,
							GUI_get_ptr(host_readbs(hero + HERO_SEX), 2));
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
				for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero += SIZEOF_HERO) {

					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
						!hero_dead(hero) &&
						test_skill(hero, TA_KLETTERN, 4) <= 0)
					{
						sprintf(g_dtp2, get_tx(27), (char*)hero + HERO_NAME2);
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

	} else if (pos == DNG_POS(2,2,14) && pos != gs_dng_handled_pos && !ds_readb(DNG14_TORCHES_FLAG)) {

		if (GUI_bool(get_tx(28))) {

			ds_writeb(DNG14_TORCHES_FLAG, 1);

			get_item(ITEM_TORCH_OFF, 1, 4);
		}

	} else if (pos == DNG_POS(2,3,1) && pos != gs_dng_handled_pos && !ds_readb(DNG14_SPOOKY_FLAG)) {

		ds_writeb(DNG14_SPOOKY_FLAG, 1);

		GUI_output(get_tx(29));

		hero = get_hero(0);
		for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero += SIZEOF_HERO) {

			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_attrib(hero, ATTRIB_TA, 0) <= 0)
			{
				l_di = get_free_mod_slot();

				set_mod_slot(l_di, HOURS(6), hero + (HERO_ATTRIB + 3 * ATTRIB_MU), -2, (signed char)hero_pos);
			}
		}

	} else if (pos == DNG_POS(2,1,2) && pos != gs_dng_handled_pos && !ds_readb(DNG14_SPEAR_FLAG)) {

		if (GUI_bool(get_tx(30))) {

			ds_writeb(DNG14_SPEAR_FLAG, 1);

			get_item(ITEM_SPEAR, 1, 1);
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
			ds_readb(DNG14_SECRETDOOR5) != 2 &&
			(ds_readb(DNG14_SECRETDOOR5) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 6) > 0))
	{
			ds_writeb(DNG14_SECRETDOOR5, 1);

			sprintf(g_dtp2,	get_tx(7), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((l_di = test_skill(hero, TA_SCHLOESSER, 4)) > 0 ? get_tx(8) : get_tx(9)),
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (l_di > 0) {
				/* disable trap */
				and_ptr_bs(amap_ptr + MAP_POS(14,4), (DNG_TILE_CORRIDOR << 4) + 0x0f);

				ds_writeb(DNG14_SECRETDOOR5, 2);

				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
	}

	if (pos == DNG_POS(3,4,12) && pos != gs_dng_handled_pos) {

		GUI_output(get_tx(35));

	} else if (((pos == DNG_POS(3,10,10) && gs_direction == EAST) || (pos == TEVENT138_FLAG && gs_direction == WEST)) &&
			pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(38))) {

			if (get_first_hero_with_item(ITEM_ROPE) != - 1 || get_first_hero_with_item(ITEM_ROPE_LADDER) != -1) /* TODO: why not staffspell >= 3 ? */
			{

				if ((hero_pos = select_hero_ok(get_tx(39))) != -1) {

					hero = get_hero(hero_pos);

					if (test_skill(hero, TA_SCHWIMMEN, 8) <= 0) {

						sprintf(g_dtp2, get_tx(40), (char*)hero + HERO_NAME2,
							GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));
						GUI_output(g_dtp2);

						/* 1W6 damage */
						sub_hero_le(hero, random_schick(6));

						gs_x_target = (pos == DNG_POS(3,10,10) ? 9 : 13);

						if (hero_dead(hero)) {

							sprintf(g_dtp2,	get_tx(63), (char*)hero + HERO_NAME2);
							GUI_output(g_dtp2);

							draw_status_line();
						}
					} else {
						sprintf(g_dtp2, get_tx(41), (char*)hero + HERO_NAME2,
							GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));
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

	} else if (pos == DNG_POS(3,3,3) && pos != gs_dng_handled_pos && !ds_readb(DNG14_HATCHET_FLAG)) {

		if (GUI_bool(get_tx(46))) {

			ds_writeb(DNG14_HATCHET_FLAG, 1);

			get_item(ITEM_PICKAXE, 1, 1);
		}

	} else if (pos == DNG_POS(3,11,13) && pos != gs_dng_handled_pos && gs_direction == EAST) {

		GUI_output(get_tx(48));

	} else if (pos == DNG_POS(3,12,12) && pos != gs_dng_handled_pos) {

		sprintf(g_dtp2, get_tx(49), (char*)hero + HERO_NAME2, (char*)hero + HERO_NAME2);
		GUI_output(g_dtp2);

		/* 1W6 damage */
		sub_hero_le(hero, 3);

	} else if (pos == DNG_POS(3,7,3) && pos != gs_dng_handled_pos && !ds_readb(DNG14_RING_FLAG)) {

		if (GUI_bool(get_tx(50))) {

			ds_writeb(DNG14_RING_FLAG, 1);

			get_item(ITEM_RING_BLUE, 1, 1);
		}

	} else if (pos == DNG_POS(3,11,1) && pos != gs_dng_handled_pos) {

		if (GUI_bool(get_tx(51))) {

			GUI_output(get_tx(52));

			if (!ds_readb(DNG14_CELLAREXIT_FLAG)) {

				GUI_output(get_tx(53));
				GUI_output(get_tx(54));
				GUI_output(get_tx(55));

				drink_while_drinking(100);

				ds_writeb(DNG14_CELLAREXIT_FLAG, 1);
			}

			leave_dungeon();
			gs_x_target = 22;
			gs_y_target = 5;
			gs_current_loctype = LOCTYPE_NONE;
			gs_direction = SOUTH;
		}

	} else if (pos == DNG_POS(4,5,13) && pos != gs_dng_handled_pos && !ds_readb(DNG14_ORKNASE_FLAG)) {

		if (GUI_bool(get_tx(57))) {

			ds_writeb(DNG14_ORKNASE_FLAG, 1);

			get_item(ITEM_ORKNASE, 1, 1);
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
