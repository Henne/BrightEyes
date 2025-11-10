/**
 *	Rewrite of DSA1 v3.02_de functions of seg091 (dungeon: prem mine)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg091.cpp
 */

#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg003.h"
#include "seg007.h"
#include "seg025.h"
#include "seg047.h"
#include "seg091.h"
#include "seg092.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct struct_chest g_dng13_specialchests[3] = {
	{ DNG_POS(0,11,5),	1, DNG13_chest00_open, NULL, DNG13_chest00_loot, 0, 0, 0 },
	{ DNG_POS(0,4,1),	1, DNG13_chest01_open, NULL, DNG13_chest01_loot, 0, 0, 0 },
	{     -1, 0, NULL, NULL, NULL, 0, 0, 0}
}; // ds:0x9d48

signed short DNG13_handler(void)
{
	signed short pos;
	signed short tw_bak;
	int32_t p_money;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = get_first_hero_available_in_group();

	if (!(gs_day_timer % MINUTES(5)) &&
		(pos == DNG_POS(0,5,1) || pos == DNG_POS(0,6,1) || pos == DNG_POS(0,7,1) || pos == DNG_POS(0,8,1)) &&
		random_schick(100) <= 5)
	{
		hero = get_hero(get_random_hero());

		sprintf(g_dtp2,	get_tx(18), hero->alias);
		GUI_output(g_dtp2);

		sub_hero_le(hero, 2);
	}

	if (pos == DNG_POS(0,9,13) && pos != gs_dng_handled_pos && !gs_dng13_lantern_flag)
	{
		if (GUI_bool(get_tx(1)))
		{
			gs_dng13_lantern_flag = 1;

			get_item(ITEM_LATERNE__UNLIT, 1, 1);
		}

	} else if ((pos == DNG_POS(0,1,12) || pos == DNG_POS(0,14,6) || pos == DNG_POS(0,14,13) || pos == DNG_POS(0,3,1)) &&
			pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(2));

	} else if (pos == DNG_POS(0,2,13) && pos != gs_dng_handled_pos)
	{
		GUI_dialog_na(60, get_tx(3));

	} else if (pos == DNG_POS(0,3,12) && pos != gs_dng_handled_pos)
	{
		DNG13_unblock_passage(get_tx(4), &gs_dng13_passage1_flag);

	} else if (pos == DNG_POS(0,4,9) && (pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) && gs_direction == WEST)
	{
		GUI_output(get_tx(6));
		gs_direction_bak = gs_direction;

	} else if (pos == DNG_POS(0,10,2) &&
			 (pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST)
	{
		GUI_output(get_tx(6));
		gs_direction_bak = gs_direction;

	} else if (pos == DNG_POS(0,5,9) && pos != gs_dng_handled_pos)
	{
		DNG13_unblock_passage(get_tx(7), &gs_dng13_passage2_flag);

	} else if (pos == DNG_POS(0,2,9) && pos != gs_dng_handled_pos)
	{
		loot_multi_chest(gs_dng13_chest_equips, get_tx(8));

	} else if (pos == DNG_POS(0,10,6) && pos != gs_dng_handled_pos)
	{
		DNG13_unblock_passage(get_tx(7), &gs_dng13_passage3_flag);

	} else if (pos == DNG_POS(0,11,5) && pos != gs_dng_handled_pos)
	{
		loot_special_chest(0);

	} else if (pos == DNG_POS(0,10,10) && pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(12));

	} else if (pos == DNG_POS(0,14,9) && pos != gs_dng_handled_pos)
	{
		DNG13_unblock_passage(get_tx(7), &gs_dng13_passage4_flag);

	} else if (pos == DNG_POS(0,7,3) && pos != gs_dng_handled_pos)
	{
		DNG13_unblock_passage(get_tx(7), &gs_dng13_passage5_flag);

	} else if (pos == DNG_POS(0,4,1) && pos != gs_dng_handled_pos)
	{
		loot_special_chest(0);

	} else if (pos == DNG_POS(0,5,5) && pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(15));

		GUI_output(get_tx(16));

	} else if (pos == DNG_POS(0,4,7) && pos != gs_dng_handled_pos && !gs_dng13_money_flag)
	{
		GUI_output(get_tx(17));

		gs_dng13_money_flag = 1;

		p_money = get_party_money();
		p_money += 41L;
		set_party_money(p_money);

	} else if ((pos == DNG_POS(0,7,14) || pos == DNG_POS(0,5,12) || pos == DNG_POS(0,1,8) || pos == DNG_POS(0,4,3) || pos == DNG_POS(0,7,5) || pos == DNG_POS(0,14,1) || pos == DNG_POS(0,8,8)) &&
			pos != gs_dng_handled_pos)
	{
		DNG13_collapsing_ceiling();

	} else if ((pos == DNG_POS(0,14,12) || pos == DNG_POS(0,7,1)) && pos != gs_dng_handled_pos)
	{
		DNG13_collapsing_ceiling_easy();

	} else if (pos == DNG_POS(0,7,15) && pos != gs_dng_handled_pos)
	{
		/* leave dungeon */
		/* AP bonus for each collapsed ceiling tile, 8 AP if no hero lost, 5 AP otherwise */
		add_hero_ap_all(gs_dng13_collapsecount * (gs_dng13_herocount == count_heroes_in_group() ? 8 : 5));

		leave_dungeon();

		gs_x_target = 9;
		gs_y_target = 1;
		gs_direction = EAST;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = pos;

	return 0;
}

/**
 * \brief   unblock a passage if its blocked
 *
 * \param   text        text for the output
 * \param   flag        pointer to the flag (0 = blocked / 1 = free)
 */
void DNG13_unblock_passage(char* text, uint8_t* flag)
{
	signed short has_items;

	/* check if passage is blocked */
	if (!(*flag))
	{
		/* ask if the heroes want to try */
		if (GUI_bool(text))
		{
			/* check if the group has ALL of these items:
			 * 	Schaufel, Hacke, Brecheisen, Wurfbeil
			 * 	(= shovel, pickaxe, crowbar, francesca) */

			has_items = 0;

			if (get_first_hero_with_item(ITEM_SCHAUFEL) != -1 &&
				get_first_hero_with_item(ITEM_HACKE) != -1 &&
				get_first_hero_with_item(ITEM_BRECHEISEN) != -1 &&
				get_first_hero_with_item(ITEM_WURFBEIL) != -1)
			{
				has_items = 1;
			}

			/* if all items are available, it takes 2 hours instead of 6 */
			timewarp(!has_items ? HOURS(6) : HOURS(2));

			GUI_output(get_tx(5));

			/* mark this passage as free */
			*flag = 1;

		} else {

			gs_x_target = gs_x_target_bak;
			gs_y_target = gs_y_target_bak;
		}
	}
}

void DNG13_chest00_open(struct struct_chest* chest)
{
	loot_corpse(chest, get_tx(9), &gs_dng13_corpse0_flag);
}

void DNG13_chest00_loot(struct struct_chest* chest)
{
	uint8_t* bak = chest->content;

	chest->content = gs_dng13_chest0_content;

	loot_chest(chest, get_tx(10), get_tx(11));

	chest->content = bak;
}

void DNG13_chest01_open(struct struct_chest* chest)
{
	loot_corpse(chest, get_tx(14), &gs_dng13_corpse1_flag);
}

void DNG13_chest01_loot(struct struct_chest* chest)
{
	uint8_t* bak = chest->content;

	chest->content = gs_dng13_chest1_content;

	loot_chest(chest, get_tx(10), get_tx(11));

	chest->content = bak;
}

/**
 * \brief   print introductions of dungeon #13 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG13_fight_intro(signed short fight_id)
{
	if (fight_id == FIGHTS_DPRE10_1)
	{
		GUI_output(get_tx(13));
	}
}

void DNG13_collapsing_ceiling(void)
{
	signed short i;
	signed short fails;
	signed short has_items;
	struct struct_hero *hero;

	gs_dng13_collapsecount++;

	hero = get_hero(0);
	for (i = fails = 0; i <= 6; i++, hero++)
	{
		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
			!hero->flags.dead && test_skill(hero, TA_SCHLEICHEN, -4) <= 0)
		{
			fails++;
		}
	}

	if (fails > 1)
	{
		/* at least two heroes failed in the skill test */
		GUI_output(get_tx(19));

		sub_group_le(dice_roll(1, 6, 4)); /* 1D6 + 4 */

		GUI_output(get_tx(20));

		/* check if the group has ALL of these items:
		 * 	Schaufel, Hacke, Brecheisen, Wurfbeil
		 * 	(= shovel, pickaxe, crowbar, francesca) */

		has_items = 0;

		if (get_first_hero_with_item(ITEM_SCHAUFEL) != -1 &&
			get_first_hero_with_item(ITEM_HACKE) != -1 &&
			get_first_hero_with_item(ITEM_BRECHEISEN) != -1 &&
			get_first_hero_with_item(ITEM_WURFBEIL) != -1)
		{
			has_items = 1;
		}

		/* if all items are available, it takes 6 hours instead of one day */
		timewarp(has_items == 0 ? HOURS(6) : DAYS(1));
	}
}

void DNG13_collapsing_ceiling_easy(void)
{
	signed short i;
	signed short fails;
	signed short has_items;
	struct struct_hero *hero;

	gs_dng13_collapsecount++;

	hero = get_hero(0);
	for (i = fails = 0; i <= 6; i++, hero++)
	{
		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
			!hero->flags.dead && test_skill(hero, TA_SCHLEICHEN, -1) <= 0)
		{
			fails++;
		}
	}

	if (fails > 1)
	{
		/* at least two heroes failed in the skill test */
		GUI_output(get_tx(19));

		sub_group_le(dice_roll(1, 6, 5)); /* 1D6 + 5 */

		GUI_output(get_tx(20));

		/* check if the group has ALL of these items:
		 * 	Schaufel, Hacke, Brecheisen, Wurfbeil
		 * 	(= shovel, pickaxe, crowbar, francesca) */

		has_items = 0;

		if (get_first_hero_with_item(ITEM_SCHAUFEL) != -1 &&
			get_first_hero_with_item(ITEM_HACKE) != -1 &&
			get_first_hero_with_item(ITEM_BRECHEISEN) != -1 &&
			get_first_hero_with_item(ITEM_WURFBEIL) != -1)
		{
			has_items = 1;
		}

#ifndef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 18 */
		/* if all items are available, it takes 18 hours instead of 12.
		 * So in the presence of tools, it takes longer to clear the way. This doesn't make sense at all. */
		timewarp(has_items == 0 ? HOURS(18) : HOURS(12));
#else
		/* Fix: Reverse the two cases */
		timewarp(has_items == 0 ? HOURS(12) : HOURS(18));
#endif
	}
}

#if !defined(__BORLANDC__)
}
#endif
