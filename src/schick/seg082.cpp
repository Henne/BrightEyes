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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG07_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;
	signed short spell_result;
	signed short lockpick_pos;
	signed short skill_result;

	amap_ptr = p_datseg + DNG_MAP;
	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = (Bit8u*)get_first_hero_available_in_group();

	if (target_pos == DNG_POS(0,13,2) && target_pos != gs_dng_handled_pos)
	{
		if (div16(host_readb(amap_ptr + MAP_POS(13,2))) != DNG_TILE_STAIR_DOWN)
		{
			do {
				i = GUI_radio(get_tx(8), 3,
							get_tx(9),
							get_tx(10),
							get_tx(11));
			} while (i == -1);

			if (i == 1)
			{
				if ((lockpick_pos = hero_has_lockpicks(hero)) != -1)
				{
					if (lockpick_pos != -2)
					{
						skill_result = test_skill(hero, TA_SCHLOESSER, 7);

						if (skill_result == -99) {

							print_msg_with_first_hero(get_ttx(533));
							or_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * lockpick_pos, 0x01); /* set 'broken' flag */
							ds_writew((FIG_FLEE_POSITION + 0),
								ds_writew((FIG_FLEE_POSITION + 2),
								ds_writew((FIG_FLEE_POSITION + 4),
								ds_writew((FIG_FLEE_POSITION + 6), target_pos))));

							do_fight(FIGHTS_F100_03);

						} else if (skill_result <= 0) {

							print_msg_with_first_hero(get_ttx(532));
							ds_writew((FIG_FLEE_POSITION + 0),
								ds_writew((FIG_FLEE_POSITION + 2),
								ds_writew((FIG_FLEE_POSITION + 4),
								ds_writew((FIG_FLEE_POSITION + 6), target_pos))));

							do_fight(FIGHTS_F100_03);

						} else {

							and_ptr_bs(amap_ptr + MAP_POS(13,2), 0x0f);
							or_ptr_bs(amap_ptr + MAP_POS(13,2), DNG_TILE_STAIR_DOWN << 4);

							add_hero_ap(hero, 1L);
						}

					} else {
						print_msg_with_first_hero(get_ttx(531));
					}
				} else {
					print_msg_with_first_hero(get_ttx(530));
				}

			} else if (i == 2)
			{

				hero = get_hero(select_hero_ok_forced(get_ttx(317)));

				if (host_readbs(hero + HERO_TYPE) < HERO_TYPE_WITCH)
				{
					GUI_output(get_ttx(330));
				} else {
					spell_result = test_spell(hero, SP_FORAMEN_FORAMINOR, 5);

					if (spell_result > 0)
					{
						sub_ae_splash(hero, get_spell_cost(SP_FORAMEN_FORAMINOR, 0));

						and_ptr_bs(amap_ptr + MAP_POS(13,2), 0x0f);
						or_ptr_bs(amap_ptr + MAP_POS(13,2), DNG_TILE_STAIR_DOWN << 4);

						add_hero_ap(hero, 1L);

					} else if (spell_result != -99)
					{

						sub_ae_splash(hero, get_spell_cost(SP_FORAMEN_FORAMINOR, 1));

						ds_writew((FIG_FLEE_POSITION + 0),
							ds_writew((FIG_FLEE_POSITION + 2),
							ds_writew((FIG_FLEE_POSITION + 4),
							ds_writew((FIG_FLEE_POSITION + 6), target_pos))));

						do_fight(FIGHTS_F100_03);
					} else
					{
						sprintf(g_dtp2,
							get_ttx(607),
							(char*)hero + HERO_NAME2);

						GUI_output(g_dtp2);
					}
				}
			}

			gs_x_target = gs_x_target_bak;
			gs_y_target = gs_y_target_bak;
			ds_writew(DNG_REFRESH_DIRECTION, -1);

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

	} else if (target_pos == DNG_POS(1,6,1) && target_pos != gs_dng_handled_pos && ds_readb(DNG07_MUELIXIER_FLAG) != 2)
	{
		if (!ds_readb(DNG07_MUELIXIER_FLAG))
		{
			ds_writeb(DNG07_MUELIXIER_FLAG, 1);

			GUI_output(get_tx(2));

			if (GUI_bool(get_tx(3)))
			{
				get_item(ITEM_MU_ELIXIR, 1, 1);

				ds_writeb(DNG07_MUELIXIER_FLAG, 2);
			}

			/* ORIGINAL-BUG: forgot to set hero */
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero))
				{
					add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU), 3); /* MU + 3 */
					or_ptr_bs(hero + HERO_FLAGS2, 0x80); /* set 'encouraged' flag */
				}
			}
		} else {
			if (GUI_bool(get_tx(5)))
			{
				get_item(ITEM_MU_ELIXIR, 1, 1);

				ds_writeb(DNG07_MUELIXIER_FLAG, 2);
			}
		}

	} else if ((target_pos == DNG_POS(2,3,13) || target_pos == DNG_POS(2,2,13)) && target_pos != gs_dng_handled_pos && !ds_readb(DNG07_FLICKER_FLAG))
	{
		ds_writeb(DNG07_FLICKER_FLAG, 1);

		GUI_output(get_tx(4));

	} else if (target_pos == DNG_POS(2,1,11) && target_pos != gs_dng_handled_pos && !ds_readb(DNG07_ANTIMUELIXIER_FLAG))
	{
		if (GUI_bool(get_tx(5)))
		{
			get_item(ITEM_MU_ELIXIR_BAD, 1, 1);

			ds_writeb(DNG07_ANTIMUELIXIER_FLAG, 1);
		}

	} else if (target_pos == DNG_POS(2,10,1) && target_pos != gs_dng_handled_pos)
	{
		if (!do_fight(FIGHTS_F100_13))
		{
			if (gs_treasure_maps[1] == 2)
			{
				ds_writeb(TMAP_DOUBLE2, 1);
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
			sprintf(g_dtp2, get_tx(7), (char*)hero + HERO_NAME2);
			GUI_output(g_dtp2);

			gs_dng07_poison_flag = 1;
			gs_dng07_poison_timer = DAYS(1);
		}

	} else if (target_pos == DNG_POS(0,1,14) && target_pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(15)))
		{
			/* ORIGINAL-BUG: forgot to set hero */
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					hero_encouraged(hero))
				{
					sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU), 3); /* MU - 3 */
					and_ptr_bs(hero + HERO_FLAGS2, 0x7f); /* unset 'encouraged' flag */
				}
			}

			leave_dungeon();

			gs_current_town = (ds_readbs(TRAVEL_DESTINATION_TOWN_ID));
			gs_x_target = (ds_readws(TRAVEL_DESTINATION_X));
			gs_y_target = (ds_readws(TRAVEL_DESTINATION_Y));
			gs_current_loctype = LOCTYPE_NONE;
			gs_direction = ((ds_readb(TRAVEL_DESTINATION_VIEWDIR) + 2) & 3);

			sprintf(g_dtp2, get_tx(14), get_ttx(ds_readws(TRV_DESTINATION) + 0xeb));

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

void DNG09_statues(signed short prob, signed short bonus)
{
	signed short i;
	signed short randval;
	Bit8u *hero;
	Bit8u *amap_ptr;

	hero = (Bit8u*)get_first_hero_available_in_group();

	amap_ptr = p_datseg + DNG_MAP;

	if (host_readbs(amap_ptr + MAP_POS(gs_x_target, gs_y_target)) == 4)
	{
		/* TODO: no forced decision here ? */
		i = GUI_radio(get_tx(4), 3,
				get_tx(5),
				get_tx(6),
				get_tx(7));

		if (i == 1)
		{
			/* praise the nameless god */
			if (random_schick(100) <= prob)
			{
				if (random_schick(100) < 50 &&
					!hero_gods_pissed(hero) &&
					!ds_readb(NAMELESS_DESTROYED))
				{
					/* increase one attribute of the leader permanently */
					randval = random_schick(7) - 1;

					inc_ptr_bs(hero + HERO_ATTRIB_ORIG + 3 * randval);
					inc_ptr_bs(hero + HERO_ATTRIB + 3 * randval);

					/* ... but the twelve won't grant miracles any more */
					or_ptr_bs(hero + HERO_FLAGS2, 0x20); /* set 'gods_pissed' flag */

					sprintf(g_dtp2,
						get_tx(8),
						(char*)hero + HERO_NAME2);

					GUI_output(g_dtp2);
				} else {
					/* loose 1W6 LE */
					sub_hero_le(hero, random_schick(6));

					sprintf(g_dtp2,
						get_tx(9),
						(char*)hero + HERO_NAME2);

					GUI_output(g_dtp2);
				}
			}

		} else if (i == 2) {

			/* destroy the statue */

			/* remove the statue from the map */
			and_ptr_bs(amap_ptr + MAP_POS(gs_x_target, gs_y_target), 0xfb); /* clear flag 2 */

			GUI_output(get_tx(10));

			/* increase estimation */
			for (i = 0; i < 14; i++) {
				gs_gods_estimation[i] += bonus;
			}

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero))
				{
					/* the twelve will grant miracles again */
					and_ptr_bs(hero + HERO_FLAGS2, 0xdf); /* unset 'gods_pissed' flag */
				}
			}

			/* no statue can be praised any longer */
			ds_writeb(NAMELESS_DESTROYED, 1);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
