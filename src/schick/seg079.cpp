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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG03_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short j;
	signed short tw_bak;
	struct struct_hero *hero;
	signed short l3;
	signed short l4;

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

			hero = (struct struct_hero*)get_hero(0);
			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus >= HERO_TYPE_WITCH) && hero->ae != 0) {

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

		hero = (struct struct_hero*)get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus >= HERO_TYPE_WITCH) && hero->ae != 0) {

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

		hero = (struct struct_hero*)get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) &&
				(hero->group_no == gs_current_group) &&
				!hero_dead((Bit8u*)hero) &&
				(test_skill(hero, TA_KLETTERN, 2) <= 0))
			{
				sprintf(g_dtp2,	get_tx(6), hero->alias,	GUI_get_ptr(hero->sex, 0));
				GUI_output(g_dtp2);

				sub_hero_le(hero, dice_roll(1, 6, 3));
			}
		}

	} else if (target_pos == DNG_POS(0,5,14) &&
			gs_direction != gs_direction_bak &&
			gs_direction == SOUTH)
	{
		GUI_input(get_tx(7), 15);

		if (!strcmp(g_text_input_buf, g_dng03_str_mactans))
		{
			GUI_output(get_tx(4));

			gs_x_target = 3;
			gs_y_target = 4;
			gs_direction = SOUTH;
			DNG_update_pos();

			hero = (struct struct_hero*)get_hero(0);
			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus >= HERO_TYPE_WITCH) && hero->ae != 0) {

					sub_ae_splash(hero, random_schick(6));

				} else if (hero->typus != HERO_TYPE_NONE) {

					sub_hero_le(hero, random_schick(6));
				}
			}
		} else {
			gs_direction = WEST;
		}

	} else if (target_pos == DNG_POS(0,9,11) && gs_direction == WEST &&
			target_pos != gs_dng_handled_pos && gs_direction != gs_direction_bak)
	{
		/* LEVER: */
		if (GUI_bool(get_tx(8)))
		{
			gs_dng03_lever_chest11 ^= 1;
			GUI_output(get_tx(9));
		}

	} else if (target_pos == DNG_POS(1,3,11) && gs_direction == EAST &&
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

		if (test_skill((struct struct_hero*)get_first_hero_available_in_group(), TA_GEFAHRENSINN, 4) <= 0)
		{
			j++;
		}

		if ((hero = get_second_hero_available_in_group()) &&
			(hero->typus != HERO_TYPE_NONE) &&
			(hero->group_no == gs_current_group) &&
			!hero_dead((Bit8u*)hero) &&
			(test_skill(hero, TA_GEFAHRENSINN, 4) <= 0))
		{
			j++;
		}

		hero = (struct struct_hero*)get_first_hero_available_in_group();

		if (j != 0 || test_attrib(hero, ATTRIB_GE, 2) <= 0)
		{
			sprintf(g_dtp2,	get_tx(13), hero->alias, GUI_get_ptr(hero->sex, 3));
			GUI_output(g_dtp2);

			sub_hero_le(hero, dice_roll(2, 6, 0));

			timewarp(MINUTES(20));

			if (hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id != ITEM_NONE)
			{
				/* RS of the equipped body armor gets degraded by 3, but not below 0 */
				l3 = g_armors_table[host_readbs(get_itemsdat(hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id) + ITEM_STATS_TABLE_INDEX)].rs
				    - hero->inventory[HERO_INVENTORY_SLOT_BODY].rs_lost;

				l4 = (l3 > 3 ? 3 : (l3 > 0 ? l3 : 0));

				hero->inventory[HERO_INVENTORY_SLOT_BODY].rs_lost += l4;
				hero->rs_bonus1 -= l4;
			}
		}

		if ((hero = get_second_hero_available_in_group()) &&
			(j == 2 ||
			(hero->typus != HERO_TYPE_NONE) &&
			(hero->group_no == gs_current_group) &&
			!hero_dead((Bit8u*)hero) &&
			(test_attrib(hero, ATTRIB_GE, 2) <= 0)))
		{

			sprintf(g_dtp2,	get_tx(13), hero->alias, GUI_get_ptr(hero->sex, 3));
			GUI_output(g_dtp2);

			sub_hero_le(hero, dice_roll(2, 6, 0));

			timewarp(MINUTES(20));

			if (hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id != ITEM_NONE)
			{
				/* RS of the equipped body armor gets degraded by 3, but not below 0 */
				l3 = g_armors_table[host_readbs(get_itemsdat(hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id) + ITEM_STATS_TABLE_INDEX)].rs
				    - hero->inventory[HERO_INVENTORY_SLOT_BODY].rs_lost;

				l4 = (l3 > 3 ? 3 : (l3 > 0 ? l3 : 0));

				hero->inventory[HERO_INVENTORY_SLOT_BODY].rs_lost += l4;
				hero->rs_bonus1 -= l4;
			}
		}

	} else if ((target_pos == DNG_POS(1,2,10) || target_pos == DNG_POS(1,3,13)) &&
		target_pos != gs_dng_handled_pos)
	{
		g_fig_flee_position[NORTH] = g_fig_flee_position[WEST] = DNG_POS_DIR(1,2,8,NORTH);
		g_fig_flee_position[EAST] = g_fig_flee_position[SOUTH] = DNG_POS_DIR(1,5,13,NORTH);

		if (!gs_dng03_highpriest_killed)
		{
			gs_dng03_highpriest_killed = 14;
		}

		do_fight(gs_dng03_highpriest_killed == 14 ? 224 : 225);

	} else if ((target_pos == DNG_POS(1,2,4) || target_pos == DNG_POS(1,5,2)) &&
		target_pos != gs_dng_handled_pos)
	{
		g_fig_flee_position[NORTH] = g_fig_flee_position[EAST] = DNG_POS_DIR(1,7,2,NORTH);
		g_fig_flee_position[SOUTH] = g_fig_flee_position[WEST] = DNG_POS_DIR(1,2,6,NORTH);

		if (!gs_dng03_highpriest_killed)
		{
			gs_dng03_highpriest_killed = 16;
		}

		do_fight(gs_dng03_highpriest_killed == 16 ? 222 : 223);

	} else if (target_pos == DNG_POS(1,1,12) && (target_pos != gs_dng_handled_pos) && !gs_dng03_sanctum_smashed)
	{

		/* check if a hero in this group has crystals */
		i = get_first_hero_with_item(ITEM_CRYSTAL) != -1 ? 0 : 1;

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

			g_fig_flee_position[NORTH] = g_fig_flee_position[EAST] = g_fig_flee_position[SOUTH] = g_fig_flee_position[WEST] = DNG_POS_DIR(1,5,13,NORTH);

			/* drop all crystals from the heroes of that group */
			i = get_first_hero_with_item(ITEM_CRYSTAL);

			do {
				hero = (struct struct_hero*)get_hero(i);

				drop_item((struct struct_hero*)hero, get_item_pos((Bit8u*)hero, ITEM_CRYSTAL), 1);

				i = get_first_hero_with_item(ITEM_CRYSTAL);

			} while (i != -1);

			do_fight(FIGHTS_F051_14C);
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

		hero = (struct struct_hero*)get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus >= HERO_TYPE_WITCH) && hero->ae) {

				sub_ae_splash(hero, random_schick(6));

			} else if (hero->typus != HERO_TYPE_NONE) {

				sub_hero_le(hero, random_schick(6));
			}
		}

	} else if ((target_pos == DNG_POS(1,2,1) && target_pos != gs_dng_handled_pos && GUI_bool(get_tx(23))) ||
			target_pos == DNG_POS(0,0,8))
	{
		leave_dungeon();
		gs_current_town = ((signed char)gs_travel_destination_town_id);
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

		sprintf(g_dtp2, get_tx(30), get_ttx(gs_trv_destination + 0xeb));

		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG03_chest00_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest00_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest01_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest01_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest02_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest02_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest03_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest03_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest04_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest04_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest05_loot(Bit8u*)
{
	signed short answer;

	do {
		answer = GUI_radio(get_tx(10), 2, get_tx(11), get_tx(12));

	} while (answer == -1);

	if (answer == 1) {

		gs_x_target = 10;
		gs_y_target = 12;
		DNG_inc_level();
	}
}

void DNG03_chest06_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest06_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest07_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest07_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest08_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest08_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest09_loot(Bit8u* chest)
{
        Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

        ((struct struct_chest*)chest)->content = gs_dng03_chest09_content;

        loot_simple_chest((struct struct_chest*)chest);

        ((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG03_chest12_loot(Bit8u* chest)
{
	Bit8u* ptr_bak;
	signed char crystals;
	signed char i;
	struct struct_hero *hero;

	crystals = 0;

	hero = (struct struct_hero*)get_first_hero_available_in_group();

	/* count the crystals in the knapsack of the leader */
	for (i = HERO_INVENTORY_SLOT_KNAPSACK_1; i < NR_HERO_INVENTORY_SLOTS; i++)
	{
		if (hero->inventory[i].item_id == ITEM_CRYSTAL)
		{
			crystals++;
		}
	}

	/* this chest can only be opened with a leader with 4 crystals */
	if (crystals == 4)
	{
		ptr_bak = ((struct struct_chest*)chest)->content;

		((struct struct_chest*)chest)->content = gs_dng03_chest12_content;

		loot_simple_chest((struct struct_chest*)chest);

		((struct struct_chest*)chest)->content = ptr_bak;

	} else {
		DNG03_chest12_trap();
	}
}

void DNG03_chest10_loot(Bit8u*)
{
	GUI_output(get_ttx(522));
}

void DNG03_chest11_loot(Bit8u*)
{
	signed short l_si;
	signed short l_di;
	signed short counter;
	signed short mod;
	struct struct_hero *hero;

	if (!gs_dng03_lever_chest11)
	{
		GUI_output(get_tx(25));

	} else {
		hero = (struct struct_hero*)get_hero(0);
		for (l_di = counter = l_si = 0; l_di <= 6; l_di++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) &&
				(hero->group_no == gs_current_group) &&
				!hero_dead((Bit8u*)hero))
			{
				l_si++;

				mod = l_si < 2 ? 6 : (l_si < 4 ? 2 : -2);

				if (test_attrib(hero, ATTRIB_GE, mod) <= 0) {

					counter++;

					sprintf(g_dtp2, get_tx(27), hero->alias);
					GUI_output(g_dtp2);

					sub_hero_le(hero, dice_roll(2, 6, 4));
				}
			}
		}

		if (counter == l_si)
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

#if !defined(__BORLANDC__)
}
#endif
