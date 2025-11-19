/**
 *	Rewrite of DSA1 v3.02_de functions of seg082 (dungeon: mageruin)
 *	Functions rewritten: 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg082.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg047.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"

/* REMARK: These 500 D might result in an integer overflow. */
struct struct_chest g_dng07_specialchests[2] = {
	{ DNG_POS(2,8,5), 5, use_lockpicks_on_chest, chest_ignifax_normal, NULL, 0, 50000L, 0 },
	{     -1, 0, NULL, NULL, NULL, 0, 0, 0 }
}; // ds:0x9668

signed int DNG07_handler(void)
{
	signed int target_pos;
	signed int i;
	signed int tw_bak;
	struct struct_hero *hero;
	uint8_t *amap_ptr;
	signed int spell_result;
	signed int lockpick_inv_slot;
	signed int skill_result;

	amap_ptr = g_dng_map;
	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = get_first_hero_available_in_group();

	if (target_pos == DNG_POS(0,13,2) && target_pos != gs_dng_handled_pos)
	{
#if !defined(__BORLANDC__)
		if (div16((uint16_t)amap_ptr[MAP_POS(13,2)]) != DNG_TILE_STAIR_DOWN)
#else
		/* REMARK: enforce a cast to uint16_t */
		if (div16((_AL = amap_ptr[MAP_POS(13,2)], _AH = 0, _AX)) != DNG_TILE_STAIR_DOWN)
#endif
		{
			do {
				i = GUI_radio(get_tx(8), 3, get_tx(9), get_tx(10), get_tx(11));

			} while (i == -1);

			if (i == 1)
			{
				if ((lockpick_inv_slot = hero_has_lockpicks(hero)) != -1)
				{
					if (lockpick_inv_slot != -2) // !=-1 and !=-2 means: hero has at least one non-broken set of lockpicks
					{
						skill_result = test_skill(hero, TA_SCHLOESSER, 7);

						if (skill_result == -99) {

							print_msg_with_first_hero(get_ttx(533));

							hero->inventory[lockpick_inv_slot].flags.broken = 1;

							g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = target_pos;

							do_fight(FIGHTS_F100_03);

						} else if (skill_result <= 0) {

							print_msg_with_first_hero(get_ttx(532));

							g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = target_pos;

							do_fight(FIGHTS_F100_03);

						} else {

							*(amap_ptr + MAP_POS(13,2)) &= 0x0f;
							*(amap_ptr + MAP_POS(13,2)) |= DNG_TILE_STAIR_DOWN << 4;

							add_hero_ap(hero, 1L);
						}

					} else {
						// all lockpicks are broken
						print_msg_with_first_hero(get_ttx(531));
					}
				} else {
					// hero does not have lockpicks
					print_msg_with_first_hero(get_ttx(530));
				}

			} else if (i == 2)
			{

				hero = get_hero(select_hero_ok_forced(get_ttx(317)));

				if (hero->typus < HERO_TYPE_HEXE)
				{
					GUI_output(get_ttx(330));
				} else {
					spell_result = test_spell(hero, SP_FORAMEN_FORAMINOR, 5);

					if (spell_result > 0) {

						sub_ae_splash(hero, get_spell_cost(SP_FORAMEN_FORAMINOR, 0));

						*(amap_ptr + MAP_POS(13,2)) &= 0x0f;
						*(amap_ptr + MAP_POS(13,2)) |= DNG_TILE_STAIR_DOWN << 4;

						add_hero_ap(hero, 1L);

					} else if (spell_result != -99) {

						sub_ae_splash(hero, get_spell_cost(SP_FORAMEN_FORAMINOR, 1));

						g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = target_pos;

						do_fight(FIGHTS_F100_03);

					} else {

						sprintf(g_dtp2,	get_ttx(607), hero->alias);
						GUI_output(g_dtp2);
					}
				}
			}

			gs_x_target = gs_x_target_bak;
			gs_y_target = gs_y_target_bak;
			g_dng_refresh_direction = -1;

		} else {

			gs_x_target++;
			DNG_inc_level();
		}

	} else if (target_pos == DNG_POS(1,12,3) && target_pos != gs_dng_handled_pos && gs_treasure_maps[1] != 1)
	{
		GUI_output(get_tx(1));

	} else if (target_pos == DNG_POS(1,13,2) && target_pos != gs_dng_handled_pos)
	{

		gs_x_target++;

		DNG_dec_level();

	} else if (target_pos == DNG_POS(1,6,1) && target_pos != gs_dng_handled_pos && (gs_dng07_mu_elixir_flag != 2))
	{
		if (!gs_dng07_mu_elixir_flag)
		{
			gs_dng07_mu_elixir_flag = 1;

			GUI_output(get_tx(2));

			if (GUI_bool(get_tx(3)))
			{
				give_new_item_to_group(ITEM_MU_ELIXIER, 1, 1);

				gs_dng07_mu_elixir_flag = 2;
			}

			/* ORIGINAL-BUG: forgot to set hero */
			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead)
				{
					hero->attrib[ATTRIB_MU].current += 3;

					hero->flags.encouraged = 1;
				}
			}
		} else {
			if (GUI_bool(get_tx(5)))
			{
				give_new_item_to_group(ITEM_MU_ELIXIER, 1, 1);

				gs_dng07_mu_elixir_flag = 2;
			}
		}

	} else if ((target_pos == DNG_POS(2,3,13) || target_pos == DNG_POS(2,2,13)) && target_pos != gs_dng_handled_pos && !gs_dng07_flicker_flag)
	{
		gs_dng07_flicker_flag = 1;

		GUI_output(get_tx(4));

	} else if (target_pos == DNG_POS(2,1,11) && target_pos != gs_dng_handled_pos && !gs_dng07_anti_mu_elixir_flag)
	{
		if (GUI_bool(get_tx(5)))
		{
			give_new_item_to_group(ITEM_MU_ELIXIER__BAD, 1, 1);

			gs_dng07_anti_mu_elixir_flag = 1;
		}

	} else if (target_pos == DNG_POS(2,10,1) && target_pos != gs_dng_handled_pos)
	{
		if (!do_fight(FIGHTS_F100_13))
		{
			if (gs_treasure_maps[1] == 2)
			{
				g_tmap_double2 = 1;
			}

			gs_treasure_maps[1] = 1;

			add_hero_ap_all(10L);

			show_treasure_map();
		}

	} else if (target_pos == DNG_POS(2,14,13) && target_pos != gs_dng_handled_pos)
	{
		gs_x_target = 7;

		gs_direction = ((gs_direction + 2) & 3);

	} else if (target_pos == DNG_POS(2,8,13) && target_pos != gs_dng_handled_pos)
	{
		gs_x_target = 13;

		gs_direction = ((gs_direction + 2) & 3);

	} else if (target_pos == DNG_POS(2,1,2) && target_pos != gs_dng_handled_pos && !gs_dng07_poison_flag)
	{
		if (GUI_bool(get_tx(6)))
		{
			sprintf(g_dtp2, get_tx(7), hero->alias);
			GUI_output(g_dtp2);

			gs_dng07_poison_flag = 1;
			gs_dng07_poison_timer = DAYS(1);
		}

	} else if (target_pos == DNG_POS(0,1,14) && target_pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(15)))
		{
			/* ORIGINAL-BUG: forgot to set hero */
			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && hero->flags.encouraged)
				{
					hero->attrib[ATTRIB_MU].current -= 3;

					hero->flags.encouraged = 0;
				}
			}

			leave_dungeon();

			gs_town_id = gs_travel_destination_town_id;
			gs_x_target = gs_travel_destination_x;
			gs_y_target = gs_travel_destination_y;
			gs_town_loc_type = LOCTYPE_NONE;
			gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

			sprintf(g_dtp2, get_tx(14), get_ttx(gs_trv_destination + 0xeb));

			GUI_output(g_dtp2);

			timewarp(HOURS(3));

			g_fading_state = 3;
		} else
		{
			gs_y_target = gs_y_target_bak;
			gs_x_target = gs_x_target_bak;
		}

	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG09_statues(const signed int prob, const signed int bonus)
{
	signed int i;
	signed int randval;
	struct struct_hero *hero;
	uint8_t *amap_ptr;

	hero = get_first_hero_available_in_group();

	amap_ptr = g_dng_map;

	if (*(amap_ptr + MAP_POS(gs_x_target, gs_y_target)) == 4)
	{
		/* TODO: no forced decision here ? */
		i = GUI_radio(get_tx(4), 3, get_tx(5), get_tx(6), get_tx(7));

		if (i == 1)
		{
			/* praise the nameless god */
			if (random_schick(100) <= prob)
			{
				if (random_schick(100) < 50 && !hero->flags.gods_pissed && !gs_nameless_destroyed)
				{
					/* increase one attribute of the leader permanently */
					randval = random_schick(7) - 1;

					hero->attrib[randval].normal++;
					hero->attrib[randval].current++;

					/* ... but the twelve won't grant miracles any more */
					hero->flags.gods_pissed = 1;

					sprintf(g_dtp2, get_tx(8), hero->alias);
					GUI_output(g_dtp2);

				} else {
					/* loose 1W6 LE */
					sub_hero_le(hero, random_schick(6));

					sprintf(g_dtp2,	get_tx(9), hero->alias);
					GUI_output(g_dtp2);
				}
			}

		} else if (i == 2) {

			/* destroy the statue */

			/* remove the statue from the map */
			*(amap_ptr + MAP_POS(gs_x_target, gs_y_target)) &= 0xfb; /* clear flag 2 */

			GUI_output(get_tx(10));

			/* increase estimation */
			for (i = 0; i < 14; i++) {
				gs_gods_estimation[i] += bonus;
			}

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead)
				{
					/* the twelve will grant miracles again */
					hero->flags.gods_pissed = 0;
				}
			}

			/* no statue can be praised any longer */
			gs_nameless_destroyed = 1;
		}
	}
}
