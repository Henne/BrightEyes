/**
 *	Rewrite of DSA1 v3.02_de functions of seg083 (dungeon: orc lair)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg083.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg047.h"
#include "seg049.h"
#include "seg075.h"
#include "seg076.h"
#include "seg083.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct struct_chest g_dng08_specialchests[7] = {
	{ DNG_POS(0,14,1), 0, NULL,			NULL,			DNG08_chest00_loot, 0, 0, 0 },
	{ DNG_POS(0,14,3), 4, use_lockpicks_on_chest,	DNG08_chest01_trap,	DNG08_chest01_loot, 0, 0, 0 },
	{ DNG_POS(0,9,7),  0, DNG08_chest02_open,	NULL,			DNG08_chest02_loot, 0, 0, 0 },
	{ DNG_POS(0,5,13), 5, DNG08_chest03_open,	chest_poisoned1,	NULL, 0, 0, 0 },
	{ DNG_POS(0,6,12), 5, DNG08_chest04_open,	chest_poisoned1,	DNG08_chest04_loot, 0, 0, 0 },
	{ DNG_POS(0,6,14), 5, DNG08_chest05_open,	chest_poisoned1,	DNG08_chest05_loot, 0, 0, 0 },
	{     -1, 0, NULL, NULL, NULL, 0, 0, 0 }
}; // ds:0x9692
static const char g_dng08_str_tairach[8] = "TAIRACH"; // ds:0x9725

signed short DNG08_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short tmp;
	signed short tw_bak;
	struct struct_hero *hero;
	uint8_t *amap_ptr;

	amap_ptr = g_dng_map;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = get_first_hero_available_in_group();

	if (target_pos == DNG_POS(0,1,10) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_00)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_00 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,1,9) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_01)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_01 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,1,8) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_02)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_02 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,1,7) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_03)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_03 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,1,6) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_04)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_04 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,6,6) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_05)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_05 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,6,10) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_06)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_06 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,7,7) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_07)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_07 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,7,1) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_08)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_08 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,11,1) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_09)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_09 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,7,4) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_10)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_10 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,10,5) && target_pos != gs_dng_handled_pos && !gs_dng08_bed_11)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng08_bed_11 = 1;
			DNG08_search_bed();
		}

	} else if (target_pos == DNG_POS(0,8,5) && target_pos != gs_dng_handled_pos && gs_dng08_waterbarrel)
	{
		DNG_waterbarrel(&gs_dng08_waterbarrel);

	} else if (target_pos == DNG_POS(0,3,6) && target_pos != gs_dng_handled_pos && !gs_dng08_timer1)
	{
		if (GUI_bool(get_tx(5)))
		{
			GUI_output(get_tx(6));

			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
					!hero->flags.dead)
				{
					gs_dng08_timer1 = 24;

					hero->hunger = hero->thirst = 0;

					add_hero_le(hero, 2);

					tmp = get_free_mod_slot();

					set_mod_slot(tmp, DAYS(1), (uint8_t*)&get_hero(i)->attrib[ATTRIB_CH].current, -1, (signed char)i);
				}
			}
		}

	} else if (target_pos == DNG_POS(0,8,9) && target_pos != gs_dng_handled_pos && !gs_dng08_timer2)
	{
		if (GUI_bool(get_tx(5)))
		{
			GUI_output(get_tx(6));

			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead)
				{
					gs_dng08_timer2 = 24;

					hero->hunger = hero->thirst = 0;

					add_hero_le(hero, 2);

					tmp = get_free_mod_slot();

					set_mod_slot(tmp, DAYS(1), (uint8_t*)&get_hero(i)->attrib[ATTRIB_CH].current, -1, (signed char)i);
				}
			}
		}

	} else if (target_pos == DNG_POS(0,1,1) && target_pos != gs_dng_handled_pos && !gs_dng08_ballista_done)
	{
		if (GUI_bool(get_tx(7)))
		{
			gs_dng08_ballista_done = 1;

			GUI_output(get_tx(8));

			add_hero_ap_all(10);

			g_fig_escape_position[NORTH] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,2,7,SOUTH);
			g_fig_escape_position[EAST] = DNG_POS_DIR(0,6,3,EAST);

			do_fight(FIGHTS_F108_3B);
		}

	} else if (target_pos == DNG_POS(0,1,4) && target_pos != gs_dng_handled_pos && !gs_dng08_bundles_done)
	{
		if (GUI_bool(get_tx(9)))
		{
			gs_dng08_bundles_done = 1;

			GUI_output(get_tx(10));

			add_hero_ap_all(10);

			g_fig_escape_position[NORTH] = (g_fig_escape_position[SOUTH] = ( g_fig_escape_position[WEST] = (DNG_POS_DIR(0,2,7,SOUTH))));
			g_fig_escape_position[EAST] = (DNG_POS_DIR(0,6,3,EAST));

			do_fight(FIGHTS_F108_3B);
		}


	} else if (target_pos == DNG_POS(0,11,14) && target_pos != gs_dng_handled_pos)
	{
		/* latrine */
		GUI_output(get_tx(12));

	} else if (target_pos == DNG_POS(0,11,11) &&
			 (target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == WEST)
	{
		/* lever, opens door at (8,13)  */
		do {
			i = GUI_radio(get_tx(13), 3, get_tx(14), get_tx(15), get_tx(16));

		} while (i == -1);

		if (i == 2)
		{
			GUI_output(get_tx(17));

		} else if (i == 3)
		{
			hero = get_hero(select_hero_ok_forced(get_tx(18)));

			sprintf(g_dtp2, get_tx(19), hero->alias);

			GUI_output(g_dtp2);

			/* open door at (8,13) */
			*(amap_ptr + MAP_POS(8,13)) &= 0x0f;
			*(amap_ptr + MAP_POS(8,13)) |= DNG_TILE_OPEN_DOOR << 4;

			if (gs_group_member_counts[gs_current_group] > 1) {

				gs_direction_bak = gs_direction;

				for (tmp = 0; gs_group_member_counts[tmp] != 0; tmp++); /* find empty group */

				hero->group_id = (signed char)tmp;
				gs_group_member_counts[tmp]++;
				gs_group_member_counts[gs_current_group]--;

				GRP_save_pos(tmp);
			}

		} else {
			gs_direction_bak = gs_direction;
		}

	} else if ((target_pos == DNG_POS(0,11,10) || target_pos == DNG_POS(0,11,12)) && target_pos != gs_dng_handled_pos)
	{
		/* squares next to lever: check if group at the lever moved away and the door must be closed */
		/* TODO: potential Original-Bug: What if the group at the lever to use Transversalis to teleport away? I guess the door is still open */

		/* check if there is still another group on the square of the lever */
		for (i = tmp = 0; i < 6; i++)
		{
			if (gs_groups_x_target[i] == 11 && gs_groups_y_target[i] == 11 &&
				gs_current_group != i)
			{
				tmp = 1;
			}
		}

		/* if not, close door at (8,13) */
		if (tmp == 0)
		{
			*(amap_ptr + MAP_POS(8,13)) &= 0x0f;
			*(amap_ptr + MAP_POS(8,13)) |= DNG_TILE_CLOSED_DOOR << 4;
		}

	} else if (target_pos == DNG_POS(0,10,12) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(20));

		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
				!hero->flags.dead && test_skill(hero, TA_KLETTERN, 2) <= 0)
			{
				sprintf(g_dtp2, get_tx(21), hero->alias, GUI_get_ptr(hero->sex, 0));
				GUI_output(g_dtp2);

				sub_hero_le(hero, dice_roll(1, 6, 4));
			}
		}

	} else if (target_pos == DNG_POS(0,5,8) && target_pos != gs_dng_handled_pos &&
			gs_direction != gs_direction_bak &&
			gs_direction == WEST &&
			gs_dng08_secret_door != 2)
	{
		if (gs_dng08_secret_door ||
			test_skill((hero = get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 1) > 0)
		{
			gs_dng08_secret_door = 1;

			sprintf(g_dtp2, get_tx(29), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((tmp = test_skill(hero, TA_SCHLOESSER, 2)) > 0 ? get_tx(30): get_tx(31)),
				GUI_get_ptr(hero->sex, 0));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (tmp > 0)
			{
				/* open the secret door */
				amap_ptr[MAP_POS(4,8)] = 0;
				gs_dng08_secret_door = 2;
				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (target_pos == DNG_POS(0,5,7) && target_pos != gs_dng_handled_pos)
	{
		sprintf(g_dtp2, get_tx(22),
			(char*)(test_skill(hero, TA_GOETTER_KULTE, 4) <= 0 ? get_tx(23) : get_tx(24)));

		GUI_output(g_dtp2);

	} else if (target_pos == DNG_POS(0,5,9) && target_pos != gs_dng_handled_pos)
	{
		sprintf(g_dtp2,	get_tx(22),
			(char*)(test_skill(hero, TA_GOETTER_KULTE, 6) <= 0 ? get_tx(23) : get_tx(25)));

		GUI_output(g_dtp2);

	} else if (target_pos == DNG_POS(0,0,14) && target_pos != gs_dng_handled_pos)
	{
		leave_dungeon();

		gs_current_town = ((signed char)gs_travel_destination_town_id);
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

		sprintf(g_dtp2,	get_tx(33), get_ttx(gs_trv_destination + 0xeb));
		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;
	}


	g_textbox_width = tw_bak;

	gs_dng_handled_pos = target_pos;

	return 0;

}

/* static */
void DNG08_search_bed(void)
{
	signed short counter;
	signed short money;
	signed short slot;
	struct struct_hero *hero;

	money = counter = 0;

	hero = get_first_hero_available_in_group();

	slot = get_free_mod_slot();

	set_mod_slot(slot, HOURS(6), (uint8_t*)&hero->attrib[ATTRIB_CH].current, -2, (signed char)get_hero_index(hero));

	if (random_schick(100) <= 10)
	{
		sprintf(g_dtp2, get_tx(3), hero->alias, (char*)GUI_names_grammar(2, 92, 0));

		/* a BRANDY BOTTLE */
		get_item(ITEM_SCHNAPSFLASCHE, 1, 1);

		GUI_output(g_dtp2);

		counter++;
	}

	if (random_schick(100) <= 10)
	{
		sprintf(g_dtp2, get_tx(3), hero->alias, (char*)GUI_names_grammar(2, 14, 0));

		/* a DAGGER */
		get_item(ITEM_DOLCH, 1, 1);

		GUI_output(g_dtp2);

		counter++;
	}

	if (random_schick(100) <= 10)
	{
		money = random_schick(6);

		sprintf(g_text_output_buf, get_tx(4), money);

		sprintf(g_dtp2, get_tx(3), hero->alias, g_text_output_buf);

		GUI_output(g_dtp2);

		add_party_money(money);

		counter++;
	}

	if (!counter)
	{
		GUI_output(get_tx(2));
	}
}

void DNG08_chest01_trap(void)
{
	struct struct_hero *hero = get_first_hero_available_in_group();

	sprintf(g_dtp2, get_tx(11), hero->alias, GUI_get_ptr(hero->sex, 1));
	GUI_output(g_dtp2);

	/* 3W6 damage */
	sub_hero_le(hero, dice_roll(3, 6, 0));
}

void DNG08_chest00_loot(struct struct_chest* chest)
{
	if (!gs_dng08_chest0_looted)
	{
		gs_dng08_chest0_looted = 1;

		GUI_output(get_tx(32));

		/* get 40 FOOD PACKAGES */
		get_item(ITEM_PROVIANTPAKET, 1, 40);
	}
}

void DNG08_chest01_loot(struct struct_chest* chest)
{
	loot_multi_chest(gs_dng08_chest1_content, get_tx(34));
}

void DNG08_chest02_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng08_chest2_content;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG08_chest02_open(struct struct_chest* chest)
{
	if (!gs_dng08_chest2_looted)
	{
		if (test_skill(get_first_hero_available_in_group(), TA_SPRACHEN, 2) > 0)
		{
			GUI_input(get_tx(27), 10);

			if (!strcmp(g_text_input_buf, g_dng08_str_tairach)) {

				chest->loot(chest);

			} else if (chest->trap) {

				chest->trap();
			}

		} else {
			GUI_output(get_tx(26));
		}
	} else {
		GUI_input(get_ttx(522), 0);
	}
}

void DNG08_chest03_open(struct struct_chest* chest)
{
	struct struct_hero *hero = get_first_hero_available_in_group();

	if (get_first_hero_with_item(ITEM_BRONZESCHLUESSEL) != -1 || test_skill(hero, TA_SCHLOESSER, 5) > 0)
	{
		if (!(gs_dng08_chest35_looted & 1)) {

			GUI_output(get_tx(28));

			add_party_money(8530L);

			gs_dng08_chest35_looted |= 1;
		} else {
			GUI_output(get_ttx(522));
		}
	} else {
		chest->trap();
	}
}

void DNG08_chest04_open(struct struct_chest* chest)
{
	struct struct_hero *hero = get_first_hero_available_in_group();

	if (get_first_hero_with_item(ITEM_BRONZESCHLUESSEL) != -1 || test_skill(hero, TA_SCHLOESSER, 5) > 0) {

		chest->loot(chest);

	} else {

		chest->trap();
	}
}

void DNG08_chest05_open(struct struct_chest* chest)
{
	struct struct_hero *hero = get_first_hero_available_in_group();

	if (get_first_hero_with_item(ITEM_BRONZESCHLUESSEL) != -1 || test_skill(hero, TA_SCHLOESSER, 5) > 0) {

		chest->loot(chest);

	} else {

		chest->trap();
	}
}

void DNG08_chest04_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng08_chest4_content;

	loot_simple_chest(chest);

	chest->content = ptr_bak;
}

void DNG08_chest05_loot(struct struct_chest* chest)
{
	uint8_t* ptr_bak = chest->content;

	chest->content = gs_dng08_chest5_content;

	loot_simple_chest(chest);

	chest->content = ptr_bak;

	if (!(gs_dng08_chest35_looted & 2))
	{
		GUI_output(get_tx(35));

		add_party_money(2890L);

		gs_dng08_chest35_looted |= 2;
	}
}

#if !defined(__BORLANDC__)
}
#endif
