/**
 *	Rewrite of DSA1 v3.02_de functions of seg085 (dungeon: cave4)
 *	Functions rewritten: 7/7 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg085.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg032.h"
#include "seg075.h"
#include "seg076.h"
#include "seg085.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct struct_chest g_dng10_specialchests[6] = {
	{ DNG_POS(0,5,2), 1, DNG10_chest00_open, 	NULL,			DNG10_chest00_loot, 0, 0, 0 },
	{ DNG_POS(0,3,5), 0, NULL,			NULL,			DNG10_chest01_loot, 0, 0, 0 },
	{ DNG_POS(0,1,5), 0, NULL,			NULL,			DNG10_chest02_loot, 0, 0, 0 },
	{ DNG_POS(1,1,1), 0, use_lockpicks_on_chest,	chest_ignifax_brutal,	DNG10_chest03_loot, 0, 0, 0 },
	{ DNG_POS(1,2,7), 0, NULL,			NULL,			DNG10_chest04_loot, 0, 0, 0 },
	{     -1, 0, NULL, NULL, NULL, 0, 0, 0 }
}; // ds:0x97d6

signed int DNG10_handler(void)
{
	signed int target_pos;
	signed int answer;
	signed int result;
	signed int tw_bak;
	struct struct_hero *hero;
	uint8_t *amap_ptr;
	int32_t p_money;

	amap_ptr = g_dng_map;
	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = get_first_hero_available_in_group();

	if ((target_pos == DNG_POS(0,1,1) || target_pos == DNG_POS(0,3,2)) && target_pos != gs_dng_handled_pos)
	{
		/* TRAP: a hole in a wall; leader gets 2 LE damage */
		if (GUI_bool(get_tx(1)))
		{
			sprintf(g_dtp2, get_tx(2), hero->alias);
			GUI_output(g_dtp2);

			sub_hero_le(hero, 2);
		}

	} else if (target_pos == DNG_POS(0,5,2) && target_pos != gs_dng_handled_pos)
	{
		/* a dead dwarf */
		loot_special_chest(0);

	} else if (target_pos == DNG_POS(0,8,1) && target_pos != gs_dng_handled_pos)
	{
		/* another hole in a wall with a lever for a trap */
		if (gs_dng10_lever_found || test_skill(hero, TA_SINNESSCHAERFE, 7) > 0)
		{
			gs_dng10_lever_found |= 1;

			if (GUI_bool((!(gs_dng10_lever_found & 2) ? get_tx(6) : get_tx(41))))
			{
				if (GUI_bool(get_tx(7))) {

					gs_dng10_lever_found |= 2;
					gs_dng10_lever_state ^= 1;

					GUI_output(get_tx(8));
				}
			}
		}

	} else if ((target_pos == DNG_POS(0,8,4) && target_pos != gs_dng_handled_pos && !gs_dng10_lever_state) ||
			(target_pos == DNG_POS(0,7,4) && target_pos != gs_dng_handled_pos && gs_dng10_lever_state))
	{
		/* TRAP: terrible pain; radom hero gets 3W6+4 LE damage */
		hero = get_hero(get_random_hero());

		answer = dice_roll(3, 6, 4);

		sprintf(g_dtp2,	get_tx(9), hero->alias);

		/* check if the hero will survive */
		if (hero->le > answer)
		{
			sprintf(g_text_output_buf, get_tx(10), GUI_get_ptr(hero->sex, 0));

			strcat(g_dtp2, g_text_output_buf);
		}

		GUI_output(g_dtp2);

		sub_hero_le(hero, answer);

	} else if (target_pos == DNG_POS(0,3,10) && (target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == WEST)
	{
		/* TRAP: a loose stone in a wall */
		if (gs_dng10_hole_state || test_skill(hero, TA_SINNESSCHAERFE, 5) > 0)
		{
			/* set hole found */
			gs_dng10_hole_state = 1;

			if (gs_dng10_hole_state == 2 || GUI_bool(get_tx(6)))
			{
				gs_dng10_hole_state = 2;

				sprintf(g_dtp2,	get_tx(11), hero->alias);

				if (GUI_bool(g_dtp2))
				{
					sprintf(g_dtp2,	get_tx(12), hero->alias);

					GUI_output(g_dtp2);

					result = random_schick(6);

					gs_dng10_hole_damage += result;

					sub_hero_le(hero, result);

					if (gs_dng10_hole_damage >= 7)
					{
						/* the secret wall can now be passed forward */
						*(amap_ptr + MAP_POS(3,7)) = (DNG_TILE_SEMIPERMEABLE_WALL << 4) + 0x0f; /* set flags 0,1,2,3 => can be entered from all directions */
					}
				}

				gs_direction_bak = gs_direction;

			} else {
			}

			gs_dng_handled_pos = target_pos;
		}
	} else if (target_pos == DNG_POS(0,1,8) && target_pos != gs_dng_handled_pos)
	{
		/* TRAP: a floorplate */
		if (gs_dng10_floorplate_found || test_skill(hero, TA_SINNESSCHAERFE, 5) > 0)
		{
			gs_dng10_floorplate_found = 1;

			/* Original-Bug: ???*/
			/* Damage only happens here when the leader of the group tries to disable this trap.
			   If the trap is not found or left alone nobody gets damaged. Weird! */
			if (GUI_bool(get_tx(13)) && test_skill(hero, TA_SCHLOESSER, 7) <= 0)
			{
				if (gs_dng10_floorplate_loads)
				{
					sprintf(g_dtp2,	get_tx(14), hero->alias);

					gs_dng10_floorplate_loads--;

					sub_hero_le(hero, dice_roll(3, 6, 0));
				} else {
					strcpy(g_dtp2, get_tx(15));
				}

				GUI_output(g_dtp2);
			}
		}

	} else if (target_pos == DNG_POS(0,1,12) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: you see three holes in the wall */
		GUI_output(get_tx(16));

	} else if (target_pos == DNG_POS(0,3,14) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: collection bowl */
		if (GUI_bool(get_tx(17)))
		{
			answer = GUI_input(get_tx(18), 6) * 10;

			if (get_party_money() < answer)
			{
				GUI_output(get_ttx(401));
			} else {
				GUI_output( (answer >= 1000) ? get_tx(20) : get_tx(19));

				set_party_money( get_party_money() - answer);
			}
		}

	} else if (target_pos == DNG_POS(0,6,12) && target_pos != gs_dng_handled_pos && gs_direction == SOUTH)
	{
		/* INFO: an empty room */
		GUI_output(get_tx(21));

	} else if (target_pos == DNG_POS(0,10,13) && target_pos != gs_dng_handled_pos && !gs_dng10_heshtot)
	{
		/* FIGHT: scared heshtot */
		if (GUI_bool(get_tx(22)))
		{
			g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,10,13,NORTH);
			gs_dng_handled_pos = 0;

			if (!do_fight(FIGHTS_F129_17)) {
				gs_dng10_heshtot = 1;
			}

		} else {
			GUI_output(get_tx(23));
			sub_group_le(random_schick(6));

			gs_x_target = 1;
			gs_y_target = 3;
			gs_direction = WEST;
			DNG_update_pos();
		}

	} else if ((target_pos == DNG_POS(1,14,8) || target_pos == DNG_POS(1,14,9) || target_pos == DNG_POS(1,14,10) || target_pos == DNG_POS(1,14,7) ||
			target_pos == DNG_POS(1,14,6) || target_pos == DNG_POS(1,14,5)) &&
			target_pos != gs_dng_handled_pos)
	{
		/* TRAP: column of fire, probability 10%, damage 1W6 for each hero in the party */
		if (random_schick(100) <= 10)
		{
			GUI_output(get_tx(24));

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead)
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		}
	} else if (target_pos == DNG_POS(1,14,2) && target_pos != gs_dng_handled_pos)
	{
		/* LEVER: enables/disables fight 111, leader get 2 LE damage */
		if (GUI_bool(get_tx(25)))
		{
			gs_dng10_mummy_lever ^= 1;

			sprintf(g_dtp2, get_tx(26), hero->alias);
			GUI_output(g_dtp2);

			sub_hero_le(hero, 2);
		}

	} else if ((target_pos == DNG_POS(1,10,3) || target_pos == DNG_POS(1,7,3)) && target_pos != gs_dng_handled_pos && gs_dng10_mummy_lever)
	{
		/* FIGHT: four mummies again and again */
		g_fig_escape_position[NORTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,5,3,WEST);
		g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = DNG_POS_DIR(1,12,3,EAST);
		g_fig_discard = 1;

		do_fight(FIGHTS_F129_21);

	} else if (target_pos == DNG_POS(1,1,12) && target_pos != gs_dng_handled_pos && gs_direction == SOUTH)
	{
		/* INFO: glowing walls */
		GUI_output(get_tx(27));

	} else if (target_pos == DNG_POS(1,9,12) && target_pos != gs_dng_handled_pos && gs_dng10_dragon_quest)
	{
		/* FIGHT: get PLATINKEY for the dragon */
		g_fig_escape_position[NORTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(1,9,10,NORTH);
		g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = DNG_POS_DIR(1,9,10,NORTH);

		do_fight(FIGHTS_F129_29);

	} else if (target_pos == DNG_POS(2,12,12) && target_pos != gs_dng_handled_pos)
	{
		/* QUEST: the dragon */
		/* TIP: plunder sucessfully, do the quest and get the reward */
		if (!gs_dng10_dragon_quest)
		{
			load_in_head(58);

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(29), 2, get_tx(30), get_tx(31));

			} while (answer == -1);

			if (answer == 1)
			{
				/* try to fight the dragon */
				gs_dng10_dragon_quest = 1;

				if (GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(32), 2, get_ttx(2), get_ttx(3)) == 1)
				{
					GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(33), 0);

				} else {
					GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(34), 0);

					sub_group_le(5000);
				}
			} else {
				/* try to plunder the hoard */
				hero = get_hero(0);

				for (answer = result = 0; answer <= 6; answer++, hero++)
				{
					if ((hero->typus != HERO_TYPE_NONE) &&
						(hero->group_id == gs_active_group_id) &&
						!hero->flags.dead &&
						(test_skill(hero, TA_SCHLEICHEN, hero->rs_bonus + 3) <= 0))
					{
						result++;
					}
				}

				if (result == 0 && !gs_dng10_hoard_plundered)
				{
					gs_dng10_hoard_plundered = 1;

					GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(35), 0);

					/* 2x HEALING POTION, MAGIC POTION, THROWING DAGGER and ...*/
					give_new_item_to_group(ITEM_HEILTRANK, 1, 2);
					give_new_item_to_group(ITEM_ZAUBERTRANK, 1, 1);
					give_new_item_to_group(ITEM_WURFDOLCH__MAGIC, 1, 1);

					/* 200 Ducats */
					p_money = get_party_money();
					p_money += 20000L;
					set_party_money(p_money);

				} else {
					gs_dng10_dragon_quest = 1;

					if (GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(32), 2,
								get_ttx(2), get_ttx(3)) == 1)
					{
						GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(33), 0);
					} else {
						GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(34), 0);
						sub_group_le(5000);
					}
				}
			}

		} else if (gs_dng10_dragon_quest == 1) {

			load_in_head(58);

			gs_x_target = gs_y_target = 12;

			if ((answer = get_first_hero_with_item(ITEM_PLATINSCHLUESSEL)) != -1)
			{
				hero = get_hero(answer);
				result = inv_slot_of_item(hero, ITEM_PLATINSCHLUESSEL);
				drop_item(hero, result, 1);

				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(36), 0);
				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(37), 0);

				/* 2x HEALING POTION, MAGIC POTION, THROWING DAGGER, CRYSTAL BALL and ...*/
				give_new_item_to_group(ITEM_HEILTRANK, 1, 2);
				give_new_item_to_group(ITEM_ZAUBERTRANK, 1, 1);
				give_new_item_to_group(ITEM_WURFDOLCH__MAGIC, 1, 1);
				give_new_item_to_group(ITEM_KRISTALLKUGEL, 1, 1);

				/* ... 200 Ducats and ... */
				p_money = get_party_money();
				p_money += 20000L;
				set_party_money(p_money);

				/* ... 50 AP */
				add_hero_ap_all(50);

				gs_dng10_dragon_quest = 2;

			} else {
				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(38), 0);
			}
		}

	} else if (target_pos == DNG_POS(2,0,5) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: some fools you are */
		GUI_output(get_tx(39));

	} else if (target_pos == DNG_POS(0,0,3) && target_pos != gs_dng_handled_pos)
	{
		leave_dungeon();

		gs_town_id = gs_travel_destination_town_id;
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 0x03);

		sprintf(g_dtp2,	get_tx(40), get_ttx(gs_trv_destination + 0xeb));
		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG10_chest00_open(struct struct_chest* chest)
{
	loot_corpse(chest, get_tx(3), (int8_t*)&gs_dng10_corpse_flag);
}

void DNG10_chest00_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng10_chest0_content;

	loot_chest(chest, get_tx(4), get_tx(5));

	chest->content = ptr_bak;
}

void DNG10_chest01_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng10_chest1_content;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG10_chest02_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng10_chest2_content;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG10_chest03_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng10_chest3_content;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG10_chest04_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng10_chest4_content;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

#if !defined(__BORLANDC__)
}
#endif
