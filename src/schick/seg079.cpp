/**
 *	Rewrite of DSA1 v3.02_de functions of seg079 (dungeon: cave of the spiders)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg079.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg075.h"
#include "seg079.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

struct struct_chest g_dng03_specialchests[14] = {
	{ DNG_POS(0,5,3),	0, NULL,			NULL,			DNG03_chest00_loot, 15, 0, 0 },
	{ DNG_POS(0,9,1),	0, NULL,			NULL,			DNG03_chest01_loot, 0, 0, 0 },
	{ DNG_POS(0,11,1),	0, NULL,			NULL,			DNG03_chest02_loot, 0, 0, 48 },
	{ DNG_POS(0,10,6),	6, use_lockpicks_on_chest,	chest_protected_heavy,	DNG03_chest03_loot, 0, 0, 0 },
	{ DNG_POS(0,11,11),	4, use_lockpicks_on_chest,	chest_poisoned2,	DNG03_chest04_loot, 0, 0, 0 },
	{ DNG_POS(0,9,12),	5, use_lockpicks_on_chest,	chest_ignifax_normal,	DNG03_chest05_loot, 0, 0, 0 },
	{ DNG_POS(0,11,12),	4, use_lockpicks_on_chest,	chest_protected_heavy,	DNG03_chest06_loot, 0, 0, 0 },
	{ DNG_POS(1,14,14),	0, NULL,			NULL,			DNG03_chest07_loot, 0, 0, 0 },
	{ DNG_POS(1,1,14),	1, use_lockpicks_on_chest,	chest_ignifax_brutal,	DNG03_chest08_loot, 0, 0, 0 },
	{ DNG_POS(1,1,3),	6, use_lockpicks_on_chest,	chest_protected_brutal, DNG03_chest09_loot, 0, 20000, 0 },
	{ DNG_POS(1,14,0),	0, NULL,			NULL,			DNG03_chest10_loot, 0, 0, 0 },
	{ DNG_POS(1,9,11),	0, NULL,			NULL,			DNG03_chest11_loot, 0, 0, 0 },
	{ DNG_POS(1,10,7),	6, use_lockpicks_on_chest,	DNG03_chest12_trap,	DNG03_chest12_loot, 0, 0, 0 },
	{     -1,		0, NULL,			NULL,			NULL, 0, 0, 0 }
}; // ds:0x9480
const char g_dng03_str_spinnennetz[12] = "SPINNENNETZ"; // ds:0x95a6
const char g_dng03_str_mactans[8] = "MACTANS"; // ds:0x95b2


signed int DNG03_handler(void)
{
	signed int target_pos;
	signed int i;
	signed int j;
	signed int tw_bak;
	struct struct_hero *hero;
	signed int armor_rs;
	signed int armor_malus;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	if ((target_pos == DNG_POS(0,4,7) || target_pos == DNG_POS(0,4,9) || target_pos == DNG_POS(0,6,8)) &&
		target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(1));

		gs_x_target = gs_y_target = 1;
		DNG_inc_level();

	} else if (target_pos == DNG_POS(0,3,4) && target_pos != gs_dng_handled_pos && gs_direction == NORTH)
	{
		GUI_input(get_tx(2), 15);

		if (!strcmp(g_text_input_buf, g_dng03_str_spinnennetz))
		{
			GUI_output(get_tx(4));

			gs_x_target = 9;
			gs_y_target = 8;
			DNG_inc_level();

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus >= HERO_TYPE_HEXE) && hero->ae != 0) {

					sub_ae_splash(hero, random_schick(6));

				} else if (hero->typus != HERO_TYPE_NONE) {

					sub_hero_le(hero, random_schick(6));
				}
			}
		}

	} else if (target_pos == DNG_POS(0,11,6) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(4));

		gs_x_target = 1;
		gs_y_target = 8;
		gs_direction = WEST;

		DNG_update_pos();

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus >= HERO_TYPE_HEXE) && hero->ae != 0) {

				sub_ae_splash(hero, random_schick(6));

			} else if (hero->typus != HERO_TYPE_NONE) {

				sub_hero_le(hero, random_schick(6));
			}
		}

	} else if ((target_pos == DNG_POS(0,6,10) || target_pos == DNG_POS(0,7,9)) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak))
	{
		if ((target_pos == DNG_POS(0,6,10) && gs_direction == EAST) ||
			(target_pos == DNG_POS(0,7,9) && gs_direction == SOUTH))
		{
			DNG_update_pos();
			GUI_output(get_tx(3));

			gs_direction_bak = gs_direction;
		}

	} else if ((target_pos == DNG_POS(0,10,9) || target_pos == DNG_POS(0,9,14)) &&
			gs_dng03_lever_trap &&
			target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(5));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) &&
				(hero->group_id == gs_active_group_id) &&
				!hero->flags.dead &&
				(test_talent(hero, TA_KLETTERN, 2) <= 0))
			{
				sprintf(g_dtp2,	get_tx(6), hero->alias,	GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_1ST));
				GUI_output(g_dtp2);

				sub_hero_le(hero, dice_roll(1, 6, 3));
			}
		}

	} else if ((target_pos == DNG_POS(0,5,14)) && (gs_direction != gs_direction_bak) && (gs_direction == SOUTH))
	{
		GUI_input(get_tx(7), 15);

		if (!strcmp(g_text_input_buf, g_dng03_str_mactans))
		{
			GUI_output(get_tx(4));

			gs_x_target = 3;
			gs_y_target = 4;
			gs_direction = SOUTH;
			DNG_update_pos();

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus >= HERO_TYPE_HEXE) && hero->ae != 0) {

					sub_ae_splash(hero, random_schick(6));

				} else if (hero->typus != HERO_TYPE_NONE) {

					sub_hero_le(hero, random_schick(6));
				}
			}
		} else {
			gs_direction = WEST;
		}

	} else if (target_pos == DNG_POS(0,9,11) && (gs_direction == WEST) &&
			target_pos != gs_dng_handled_pos && gs_direction != gs_direction_bak)
	{
		/* LEVER: */
		if (GUI_bool(get_tx(8)))
		{
			gs_dng03_lever_chest11 ^= 1;
			GUI_output(get_tx(9));
		}

	} else if (target_pos == DNG_POS(1,3,11) && (gs_direction == EAST) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak))
	{
		if (GUI_bool(get_tx(8)))
		{
			gs_dng03_lever_trap ^= 1;
			GUI_output(get_tx(9));
		}

	} else if ((target_pos == DNG_POS(1,14,13) || target_pos == DNG_POS(1,13,4) || target_pos == DNG_POS(1,11,2)) &&
		target_pos != gs_dng_handled_pos)
	{
		j = 0;

		if (test_talent(get_first_hero_available_in_group(), TA_GEFAHRENSINN, 4) <= 0)
		{
			j++;
		}

		if ((hero = get_second_hero_available_in_group()) &&
			(hero->typus != HERO_TYPE_NONE) &&
			(hero->group_id == gs_active_group_id) &&
			!hero->flags.dead &&
			(test_talent(hero, TA_GEFAHRENSINN, 4) <= 0))
		{
			j++;
		}

		hero = get_first_hero_available_in_group();

		if (j != 0 || test_attrib(hero, ATTRIB_GE, 2) <= 0)
		{
			sprintf(g_dtp2,	get_tx(13), hero->alias, GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_3RD));
			GUI_output(g_dtp2);

			sub_hero_le(hero, dice_roll(2, 6, 0));

			timewarp(MINUTES(20));

			if (hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id != ITEM_ID_NONE)
			{
				/* RS of the equipped body armor gets degraded by 3, but not below 0 */
				armor_rs = g_armor_stats_table[g_itemsdat[hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id].item_type_stats_id].rs
				    - hero->inventory[HERO_INVENTORY_SLOT_BODY].rs_lost;

				armor_malus = (armor_rs > 3 ? 3 : (armor_rs > 0 ? armor_rs : 0));

				hero->inventory[HERO_INVENTORY_SLOT_BODY].rs_lost += armor_malus;
				hero->rs_bonus -= armor_malus;
			}
		}

		if ((hero = get_second_hero_available_in_group()) &&
			(j == 2 ||
			(hero->typus != HERO_TYPE_NONE) &&
			(hero->group_id == gs_active_group_id) &&
			!hero->flags.dead &&
			(test_attrib(hero, ATTRIB_GE, 2) <= 0)))
		{

			sprintf(g_dtp2,	get_tx(13), hero->alias, GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_3RD));
			GUI_output(g_dtp2);

			sub_hero_le(hero, dice_roll(2, 6, 0));

			timewarp(MINUTES(20));

			if (hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id != ITEM_ID_NONE)
			{
				/* RS of the equipped body armor gets degraded by 3, but not below 0 */
				armor_rs = g_armor_stats_table[g_itemsdat[hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id].item_type_stats_id].rs
				    - hero->inventory[HERO_INVENTORY_SLOT_BODY].rs_lost;

				armor_malus = (armor_rs > 3 ? 3 : (armor_rs > 0 ? armor_rs : 0));

				hero->inventory[HERO_INVENTORY_SLOT_BODY].rs_lost += armor_malus;
				hero->rs_bonus -= armor_malus;
			}
		}

	} else if ((target_pos == DNG_POS(1,2,10) || target_pos == DNG_POS(1,3,13)) &&
		target_pos != gs_dng_handled_pos)
	{
		g_fig_escape_position[NORTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,2,8,NORTH);
		g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = DNG_POS_DIR(1,5,13,NORTH);

		if (!gs_dng03_highpriest_killed)
		{
			gs_dng03_highpriest_killed = 14;
		}

		do_fight(gs_dng03_highpriest_killed == 14 ? 224 : 225);

	} else if ((target_pos == DNG_POS(1,2,4) || target_pos == DNG_POS(1,5,2)) &&
		target_pos != gs_dng_handled_pos)
	{
		g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = DNG_POS_DIR(1,7,2,NORTH);
		g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,2,6,NORTH);

		if (!gs_dng03_highpriest_killed)
		{
			gs_dng03_highpriest_killed = 16;
		}

		do_fight(gs_dng03_highpriest_killed == 16 ? 222 : 223);

	} else if (target_pos == DNG_POS(1,1,12) && (target_pos != gs_dng_handled_pos) && !gs_dng03_sanctum_smashed)
	{

		/* check if a hero in this group has crystals */
		i = get_first_hero_with_item(ITEM_ID_KRISTALL) != -1 ? 0 : 1;

		do {
			j = GUI_radio(get_tx(14), 2, get_tx(15), !i ? get_tx(16) : get_tx(29));

		} while (j == -1);

		if (j == 1)
		{
			GUI_output(get_tx(18));
			gs_dng03_sanctum_smashed = 1;

		} else if (j == 2 && !i)
		{
			GUI_output(get_tx(17));

			g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,5,13,NORTH);

			/* drop all crystals from the heroes of that group */
			i = get_first_hero_with_item(ITEM_ID_KRISTALL);

			do {
				hero = get_hero(i);

				drop_item(hero, inv_slot_of_item(hero, ITEM_ID_KRISTALL), 1);

				i = get_first_hero_with_item(ITEM_ID_KRISTALL);

			} while (i != -1);

			do_fight(FIGHT_ID_F051_14C);
		}

	} else if (target_pos == DNG_POS(1,1,1) && target_pos != gs_dng_handled_pos &&
			!gs_dng03_spidereggs_burned)
	{
		do {
			j = GUI_radio(get_tx(19), 2, get_tx(20), get_tx(21));

		} while (j == -1);

		if (j == 1) {

			GUI_output(get_tx(22));

			add_hero_ap_all(50);

			gs_dng03_spidereggs_burned = 1;

			/* activate DEATHTRAP #2 with 15 steps */
			gs_deathtrap_steps = 15;
			gs_deathtrap = 2;
		}

	} else if (target_pos == DNG_POS(1,10,4) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(24));

	} else if (target_pos == DNG_POS(1,10,8) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(4));
		gs_x_target = 5;
		gs_y_target = 14;
		DNG_dec_level();
		gs_direction = NORTH;

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus >= HERO_TYPE_HEXE) && hero->ae) {

				sub_ae_splash(hero, random_schick(6));

			} else if (hero->typus != HERO_TYPE_NONE) {

				sub_hero_le(hero, random_schick(6));
			}
		}

	} else if ((target_pos == DNG_POS(1,2,1) && target_pos != gs_dng_handled_pos && GUI_bool(get_tx(23))) ||
			target_pos == DNG_POS(0,0,8))
	{
		leave_dungeon();
		gs_town_id = gs_travel_destination_town_id;
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_town_loc_type = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

		sprintf(g_dtp2, get_tx(30), get_ttx(gs_journey_destination_town_id + 0xeb));

		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG03_chest00_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest00_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest01_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest01_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest02_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest02_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest03_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest03_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest04_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest04_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest05_loot(struct struct_chest*)
{
	signed int answer;

	do {
		answer = GUI_radio(get_tx(10), 2, get_tx(11), get_tx(12));

	} while (answer == -1);

	if (answer == 1) {

		gs_x_target = 10;
		gs_y_target = 12;
		DNG_inc_level();
	}
}

void DNG03_chest06_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest06_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest07_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest07_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest08_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest08_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest09_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng03_chest09_content;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG03_chest12_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak;
	signed char crystals = 0;
	signed char i;
	struct struct_hero *hero = get_first_hero_available_in_group();

	/* count the crystals in the knapsack of the leader */
	for (i = HERO_INVENTORY_SLOT_KNAPSACK_1; i < NR_HERO_INVENTORY_SLOTS; i++)
	{
		if (hero->inventory[i].item_id == ITEM_ID_KRISTALL)
		{
			crystals++;
		}
	}

	/* this chest can only be opened with a leader with 4 crystals */
	if (crystals == 4)
	{
		ptr_bak = chest->content;

		chest->content = gs_dng03_chest12_content;

		loot_simple_chest(chest);

		chest->content = ptr_bak;

	} else {
		DNG03_chest12_trap();
	}
}

void DNG03_chest10_loot(struct struct_chest*)
{
	GUI_output(get_ttx(522));
}

void DNG03_chest11_loot(struct struct_chest*)
{
	signed int heroes_num;
	signed int i;
	signed int test_failed_num;
	signed int handicap;

	if (!gs_dng03_lever_chest11)
	{
		GUI_output(get_tx(25));

	} else {
		struct struct_hero *hero = get_hero(0);

		for (i = test_failed_num = heroes_num = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead)
			{
				heroes_num++;

				handicap = heroes_num < 2 ? 6 : (heroes_num < 4 ? 2 : -2);

				if (test_attrib(hero, ATTRIB_GE, handicap) <= 0) {

					test_failed_num++;

					sprintf(g_dtp2, get_tx(27), hero->alias);
					GUI_output(g_dtp2);

					sub_hero_le(hero, dice_roll(2, 6, 4));
				}
			}
		}

		if (test_failed_num == heroes_num)
		{
			/* end of game */
			GUI_output(get_tx(26));

			g_game_state = GAME_STATE_DEAD;
		} else {
			timewarp(HOURS(1));

			GUI_output(get_tx(28));
		}
	}
}

void DNG03_chest12_trap(void)
{
	if (gs_dng03_chest12_loads) {

		gs_dng03_chest12_loads--;

		chest_poisoned2();
	}
}
