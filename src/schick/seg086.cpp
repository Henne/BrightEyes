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
#include "seg086.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct struct_chest g_dng11_specialchests[15] = {
	{ DNG_POS(0,11,12),	0, NULL,			NULL,			DNG11_chest00_loot, 0, 0, 50 },
	{ DNG_POS(0,12,12),	0, NULL,			NULL,			DNG11_chest01_loot, 0, 0, 21 },
	{ DNG_POS(0,14,12),	6, use_lockpicks_on_chest,	chest_crossbow_bolts,	DNG11_chest02_loot, 0, 0, 0 },
	{ DNG_POS(0,14,14),	6, use_lockpicks_on_chest,	chest_crossbow_bolts,	DNG11_chest03_loot, 0, 0, 0 },
	{ DNG_POS(0,1,13),	0, NULL,			NULL,			DNG11_chest04_loot, 0, 230, 0 },
	{ DNG_POS(0,1,10),	3, use_lockpicks_on_chest,	NULL,			DNG11_chest05_loot, 0, 680, 0 },
	{ DNG_POS(0,1,7),	0, NULL,			NULL,			DNG11_chest06_loot, 0, 0, 0 },
	{ DNG_POS(0,1,4),	4, use_lockpicks_on_chest,	NULL,			DNG11_chest07_loot, 0, 440, 0 },
	{ DNG_POS(0,1,1),	0, NULL,			NULL,			DNG11_chest08_loot, 0, 0, 0 },
	{ DNG_POS(0,1,2),	0, NULL,			NULL,			DNG11_chest09_loot, 0, 0, 0 },
	{ DNG_POS(0,6,10),	4, use_lockpicks_on_chest,	chest_ignifax_heavy,	DNG11_chest10_loot, 0, 2000, 0 },
	{ DNG_POS(0,11,5),	6, use_lockpicks_on_chest,	chest_ignifax_brutal,	DNG11_chest11_loot, 0, 10000, 0 },
	{ DNG_POS(0,12,5),	6, use_lockpicks_on_chest,	chest_ignifax_brutal,	DNG11_chest12_loot, 0, 0, 0 },
	{ DNG_POS(0,9,5),	4, use_lockpicks_on_chest,	NULL,			DNG11_chest13_loot, 0, 0, 0 },
	{     -1, 0, NULL, NULL, NULL, 0, 0, 0 }
}; // ds:0x9854

signed short DNG11_handler(void)
{
	signed short target_pos;
	signed short test_result;
	signed short answer;
	signed short tw_bak;
	struct struct_hero *hero;
	uint8_t *amap_ptr;

	amap_ptr = g_dng_map;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = get_first_hero_available_in_group();

	if (target_pos == DNG_POS(0,6,12) && target_pos != gs_dng_handled_pos && gs_dng11_soup_supply)
	{
		if (GUI_radio(get_tx(1), 2, get_tx(2), get_tx(3)) == 1)
		{
			add_group_le(1);

			gs_dng11_soup_supply--;

			if (random_schick(100) <= 50)
			{
				g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(0,8,15,SOUTH);

				do_fight(FIGHTS_F131_01B);
			}
		}

	} else if (target_pos == DNG_POS(0,11,14) && target_pos != gs_dng_handled_pos)
	{
		DNG_waterbarrel(&gs_dng11_waterbarrel1);

	} else if (target_pos == DNG_POS(0,12,14) && target_pos != gs_dng_handled_pos)
	{
		DNG_waterbarrel(&gs_dng11_waterbarrel2);

	} else if (target_pos == DNG_POS(0,6,9) && target_pos != gs_dng_handled_pos) {

		answer = GUI_radio(get_tx(4), 6, get_tx(5), get_tx(6), get_tx(7),
						 get_tx(8), get_tx(9), get_tx(10));

		gs_dng11_lever_flag = 0;

		if (answer >= 1 && answer <= 5)
		{
			gs_dng11_lever_flag = (signed char)answer;

			GUI_output(get_tx(11));
		}

	} else if (target_pos == DNG_POS(0,9,9))
	{
		if (target_pos != gs_dng_handled_pos && !gs_dng11_efferd_hint)
		{
			GUI_output(get_tx(12));

		} else if (gs_dng11_efferd_sacrifice && !gs_dng11_efferd_hint)
		{
			gs_dng11_efferd_hint = 1;
			gs_dng11_unknown2_flag = gs_dng11_efferd_sacrifice = 0;

			GUI_output(get_tx(13));
		}

	} else if (target_pos == DNG_POS(0,12,10) && target_pos != gs_dng_handled_pos && gs_dng11_firetrap1_flag)
	{
		if (gs_dng11_efferd_hint || gs_dng11_unknown1_flag)
		{
			GUI_output(get_tx(15));

			gs_dng11_firetrap1_flag--;

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
					!hero->flags.dead)
				{
					sub_hero_le(hero, test_attrib(hero, ATTRIB_GE, 4) <= 0 ? dice_roll(6, 6, 6) : dice_roll(3, 6, 3));
				}
			}
		}

	} else if (target_pos == DNG_POS(0,11,8) && target_pos != gs_dng_handled_pos && gs_dng11_firetrap2_flag)
	{
		if (gs_dng11_unknown2_flag)
		{
			GUI_output(get_tx(15));

			gs_dng11_firetrap2_flag--;

			hero = get_hero(0);
			for (answer = 0; answer <= 6; answer++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead)
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
			for (answer = 0; answer <= 6; answer++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id))
				{
					hero_disappear(hero, answer, -1);
				}
			}

			disable_ani();

		} else {
			gs_x_target = gs_x_target_bak;
			gs_y_target = gs_y_target_bak;
		}

	} else if (target_pos == DNG_POS(0,8,12) &&
			// possibly a bug. position (8,12) might indeed be (8,11)
			// see https://www.crystals-dsa-foren.de/showthread.php?tid=1373&pid=96541#pid96541
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == NORTH &&
			gs_dng11_secretdoor1_flag != 2)
	{
		if (gs_dng11_secretdoor1_flag || test_skill(hero, TA_SINNESSCHAERFE, 8) > 0)
		{
			gs_dng11_secretdoor1_flag = 1;

			sprintf(g_dtp2,	get_tx(26), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((gs_dng11_lever_flag == 2 || gs_dng11_lever_flag == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 5)) > 0 ? get_tx(27) : get_tx(28)),
				GUI_get_ptr(hero->sex, 3));

			strcat(g_dtp2, g_text_output_buf);
			GUI_output(g_dtp2);

			if (test_result > 0)
			{
				amap_ptr[MAP_POS(8,11)] &= (DNG_TILE_CORRIDOR << 4) + 0x0f;

				gs_dng11_secretdoor1_flag = 2;

				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (target_pos == DNG_POS(0,4,7) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST &&
			gs_dng11_secretdoor2_flag != 2)
	{
		if (gs_dng11_secretdoor2_flag != 0 || test_skill(hero, TA_SINNESSCHAERFE, 4) > 0)
		{
			gs_dng11_secretdoor2_flag = 1;

			sprintf(g_dtp2, get_tx(26), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((gs_dng11_lever_flag == 1 || gs_dng11_lever_flag == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 3)) > 0 ? get_tx(27) : get_tx(28)),
				GUI_get_ptr(hero->sex, 3));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (test_result > 0)
			{
				amap_ptr[MAP_POS(5,7)] &= (DNG_TILE_CORRIDOR << 4) + 0x0f;

				gs_dng11_secretdoor2_flag = 2;

				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (target_pos == DNG_POS(0,9,8) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST &&
			gs_dng11_secretdoor3_flag != 2)
	{
		if (gs_dng11_secretdoor3_flag != 0 || test_skill(hero, TA_SINNESSCHAERFE, 6) > 0)
		{
			gs_dng11_secretdoor3_flag = 1;

			sprintf(g_dtp2,	get_tx(26), hero->alias);

			sprintf(g_text_output_buf,
				(char*)((gs_dng11_lever_flag == 5 || gs_dng11_lever_flag == 3) && (test_result = test_skill(hero, TA_SCHLOESSER, 5)) > 0 ? get_tx(27) : get_tx(28)),
				GUI_get_ptr(hero->sex, 3));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (test_result > 0)
			{
				amap_ptr[MAP_POS(10,8)] &= (DNG_TILE_CORRIDOR << 4) + 0x0f;

				gs_dng11_secretdoor3_flag = 2;

				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (target_pos == DNG_POS(0,8,15) && target_pos != gs_dng_handled_pos)
	{
		leave_dungeon();

		gs_town_id = gs_travel_destination_town_id;
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 0x03);

		sprintf(g_dtp2, get_tx(29), get_ttx(gs_trv_destination + 0xeb));
		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG11_chest00_loot(struct struct_chest*)
{
	if (!gs_dng11_proviant1_flag)
	{
		/* a chest with 50 food rations */
		GUI_output(get_tx(24));

		gs_dng11_proviant1_flag = 1;

	} else {

		/* an empty chest */
		GUI_output(get_ttx(522));
	}
}

void DNG11_chest01_loot(struct struct_chest*)
{
	if (!gs_dng11_proviant2_flag) {

		/* a chest with 21 food rations */
		GUI_output(get_tx(25));

		gs_dng11_proviant2_flag = 1;

	} else {

		/* an empty chest */
		GUI_output(get_ttx(522));
	}
}

void DNG11_chest02_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest3;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest03_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest4;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest04_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest5;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest05_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest6;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest06_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = &gs_dng11_chest7;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest07_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest8;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest08_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest9;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest09_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest10;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest10_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest11;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest11_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest12;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest12_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest13;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
}

void DNG11_chest13_loot(struct struct_chest* chest)
{
        uint8_t* ptr_bak = chest->content;

        chest->content = gs_dng11_chest14;

        loot_simple_chest(chest);

        chest->content = ptr_bak;
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
