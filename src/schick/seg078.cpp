/**
 *	Rewrite of DSA1 v3.02_de functions of seg078 (dungeon: a tumbledown inn)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1- Yo seg078.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg003.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg032.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg104.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG02_handler(void)
{
	signed short target_pos;
	signed short mod_slot;
	signed short i;
	signed short hero_weight;
	Bit32s weight_sum;
	Bit8u *hero;
	Bit8u *amap_ptr;
	signed char flag;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	amap_ptr = g_dng_map;

	if ((target_pos == DNG_POS(0,1,2) || target_pos == DNG_POS(0,2,1)))
	{
		if (gs_day_timer >= HOURS(8) && gs_day_timer <= HOURS(20))
		{
		} else {

			g_fig_flee_position[NORTH] = g_fig_flee_position[EAST] = g_fig_flee_position[SOUTH] = g_fig_flee_position[WEST] = DNG_POS_DIR(0,2,6,NORTH);

			do_fight(FIGHTS_F046_01);
		}

	} else if (target_pos == DNG_POS(0,3,6) && target_pos != gs_dng_handled_pos && gs_direction == SOUTH)
	{
		strcpy(g_dtp2, get_tx(4));

		strcat(g_dtp2,  (char*)((test_spell_group(SP_ODEM_ARCANUM, 0) == 0) ? get_tx(5) : get_tx(6)));

		GUI_output(g_dtp2);

	} else if (target_pos == DNG_POS(0,4,10) && target_pos != gs_dng_handled_pos && gs_direction == SOUTH)
	{
		GUI_output(get_tx(7));

	} else if ((target_pos == DNG_POS(0,7,13) || target_pos == DNG_POS(1,6,5) || target_pos == DNG_POS(1,4,14)) && target_pos != gs_dng_handled_pos)
	{
		if (test_skill((hero = (Bit8u*)get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 6) > 0)
		{
			GUI_output(get_tx(8));

		} else {
			sprintf(g_dtp2,
				get_tx(9),
				(char*)hero + HERO_NAME2,
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			GUI_output(g_dtp2);

			flag = 0;

			while (host_readws(hero + HERO_LE) > 10 && !flag)
			{
				/* KK+4 */
				if (test_attrib(hero, ATTRIB_KK, 4) <= 0)
				{
					sprintf(g_dtp2,
						get_tx(10),
						(char*)hero + HERO_NAME2,
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					GUI_output(g_dtp2);

					sub_hero_le(hero, 1);

				} else {
					flag = 1;
				}
			}

			if (!flag)
			{
				sprintf(g_dtp2,
					get_tx(11),
					(char*)hero + HERO_NAME2);

				GUI_output(g_dtp2);

				sub_hero_le(hero, (host_readws(hero + HERO_LE) > 6 ? random_schick(6) : host_readws(hero + HERO_LE) - 1));

			}
		}

	} else if (target_pos == DNG_POS(0,6,10))
	{
		DNG_fallpit_test(2);

	} else if (target_pos == DNG_POS(0,14,14) && target_pos != gs_dng_handled_pos)
	{
		hero = (Bit8u*)get_first_hero_available_in_group();
		weight_sum = get_hero_weight(hero);

		if ((hero = (Bit8u*)get_second_hero_available_in_group()))
		{
			weight_sum += get_hero_weight(hero);
		}

		if (weight_sum >= 7000L)
		{
			DNG_fallpit(6);
		}

	} else if ((target_pos == DNG_POS(0,10,12) || target_pos == DNG_POS(0,10,10)) && target_pos != gs_dng_handled_pos)
	{
		hero = get_hero(0);
		for (i = (signed short)(weight_sum = 0); i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group)
			{
				weight_sum += get_hero_weight(hero);
			}
		}

		for (i = 0; i < 6; i++) {

			if (gs_groups_x_target[i] == 10 && gs_groups_y_target[i] == 11 &&
				gs_groups_dng_level[i] == gs_dungeon_level &&
				gs_groups_dng_index[i] == gs_dungeon_index)
			{
				weight_sum = 0L;
			}
		}

		hero_weight = get_hero_weight((Bit8u*)(get_heaviest_hero()));

		host_writebs(amap_ptr + MAP_POS(10,11), (hero_weight >= weight_sum ? (DNG_TILE_CORRIDOR << 4) : (DNG_TILE_WALL << 4)));

		play_voc(305);

	} else if ((target_pos == DNG_POS(0,10,13) || target_pos == DNG_POS(0,10,9)) && target_pos != gs_dng_handled_pos)
	{
		host_writeb(amap_ptr + MAP_POS(10,11), DNG_TILE_CORRIDOR << 4);

		play_voc(305);

	} else if ((target_pos == DNG_POS(0,7,6) || target_pos == DNG_POS(0,8,6) || target_pos == DNG_POS(0,8,8)))
	{
		if (gs_dng02_sphere_active)
		{
			/* Sphaerenriss */
			GUI_output(get_tx(41));

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero)) /* TODO: Why !dead? */
				{
					hero_disappear(hero, i, -1);
				}
			}

		} else {
			hero = get_hero(0);
			for (i = mod_slot = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero) &&
					!hero_seen_phantom(hero))
				{
					mod_slot = get_free_mod_slot();
					set_mod_slot(mod_slot, HOURS(5), hero + (HERO_ATTRIB + 3 * ATTRIB_MU), -3, (signed char)i);
					{
						hero_seen_phantom_set(hero, (mod_slot = 1)); /* set 'seen_phantom' flag */
					}
				}
			}

			if (mod_slot != 0)
			{
				GUI_output(get_tx(12));
			}
		}

	} else if (target_pos == DNG_POS(0,7,1) && !gs_dng02_raven_destroyed && target_pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(13)))
		{
			/* destroy the raven */
			gs_dng02_raven_destroyed = 1;

			sub_group_le(random_schick(20));

			GUI_output(get_tx(14));

			host_writebs(amap_ptr + MAP_POS(5,2), DNG_TILE_CORRIDOR << 4);
		}

	} else if (target_pos == DNG_POS(0,11,6) && target_pos != gs_dng_handled_pos)
	{
		hero = get_hero(0);
		for (i = (signed short)(weight_sum = 0); i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group)
			{
				weight_sum += get_hero_weight(hero);
			}
		}

		if (weight_sum >= 4000)
		{
			GUI_output(get_tx(15));

			host_writeb(amap_ptr + MAP_POS(11,8), DNG_TILE_CORRIDOR << 4);
		}

	} else if ((target_pos == DNG_POS(0,10,6) || target_pos == DNG_POS(0,11,5)) && target_pos != gs_dng_handled_pos)
	{
		for (i = 0; i < 6; i++)
		{
			weight_sum = 0;

			if (gs_groups_x_target[i] == 11 && gs_groups_y_target[i] == 6 &&
				gs_groups_dng_level[i] == gs_dungeon_level &&
				gs_groups_dng_index[i] == gs_dungeon_index)
			{
				hero = get_hero(0);
				for (mod_slot = 0; mod_slot <= 6; mod_slot++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == i)
					{
						weight_sum += get_hero_weight(hero);
					}
				}

				break;

			} else if (gs_groups_x_target[i] == 11 &&
					gs_groups_y_target[i] == 8 &&
					gs_groups_dng_level[i] == gs_dungeon_level &&
					gs_groups_dng_index[i] == gs_dungeon_index)
			{
				weight_sum = 5000L;
			}
		}

		host_writeb(amap_ptr + MAP_POS(11,8), (weight_sum >= 4000L ? (DNG_TILE_CORRIDOR << 4) : (DNG_TILE_WALL << 4)));

	} else if (target_pos == DNG_POS(1,12,5) && gs_direction == NORTH &&
			 (target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak))
	{
		gs_direction_bak = (gs_direction);

		GUI_output(get_tx(16));

	} else if (target_pos == DNG_POS(1,4,5) && gs_direction == SOUTH &&
			 (target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak))
	{
		gs_direction_bak = (gs_direction);

		GUI_output(get_tx(17));

	} else if (target_pos == DNG_POS(1,7,1) && !gs_dng02_sphere_known)
	{
		load_in_head(43);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(18), 0);

		gs_dng02_sphere_timer = 7;
		gs_dng02_sphere_known = 1;

	} else if ((target_pos == DNG_POS(1,1,11) || target_pos == DNG_POS(1,1,8)) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(22));

	} else if (target_pos == DNG_POS(1,3,7) && target_pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(23)))
		{
			GUI_output(get_tx(24));
		}

	} else if (target_pos == DNG_POS(1,8,9) && target_pos != gs_dng_handled_pos && !gs_dng02_apparature_destroyed)
	{
		/* petrification trap */
		GUI_output(get_tx(25));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				host_readbs(hero + HERO_MR) < 8)
			{
				or_ptr_bs(hero + HERO_FLAGS1, 0x04); /* set 'petrified' flag */

				sprintf(g_dtp2,
					get_tx(42),
					(char*)hero + HERO_NAME2,
					(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
					(char*)hero + HERO_NAME2);

				GUI_output(g_dtp2);
			}
		}

		check_group();

	} else if (target_pos == DNG_POS(1,13,11) && target_pos != gs_dng_handled_pos)
	{
		do {
			i = GUI_radio(get_tx(26), 2,
					get_tx(27),
					get_tx(28));
		} while (i == -1);

		if (i == 1)
		{
			DNG_dec_level();
			target_pos = DNG_POS(0,13,11);

			do {
				i = GUI_radio(get_tx(29), 2,
						get_tx(30),
						get_tx(31));
			} while (i == -1);

			if (i == 2)
			{
				DNG_inc_level();
				target_pos = DNG_POS(1,13,11);
			}
		}

	} else if (target_pos == DNG_POS(0,13,11) && target_pos != gs_dng_handled_pos)
	{
		do {
			i = GUI_radio(get_tx(43), 2,
					get_tx(27),
					get_tx(28));
		} while (i == -1);

		if (i == 1)
		{
			DNG_inc_level();
			target_pos = DNG_POS(1,13,11);

			do {
				i = GUI_radio(get_tx(26), 2,
						get_tx(27),
						get_tx(28));
			} while (i == -1);

			if (i == 1)
			{
				DNG_dec_level();
				target_pos = DNG_POS(0,13,11);
			}
		}

	} else if ((target_pos == DNG_POS(0,12,11) || target_pos == DNG_POS(0,14,11) || target_pos == DNG_POS(0,13,10) || target_pos == DNG_POS(0,13,12)) && target_pos != gs_dng_handled_pos)
	{
		g_fig_flee_position[NORTH] = g_fig_flee_position[EAST] = g_fig_flee_position[SOUTH] = g_fig_flee_position[WEST] = DNG_POS_DIR(0,12,11,NORTH);

		if (!do_fight(FIGHTS_F046_31))
		{
#if defined(__BORLANDC__)
			gs_always_one2 = 1;
#endif

			add_hero_ap_all(30);
		}

	} else if (target_pos == DNG_POS(1,1,1) && target_pos != gs_dng_handled_pos)
	{
		seg092_06b4(0);

	} else if (target_pos == DNG_POS(1,9,14) && target_pos != gs_dng_handled_pos && gs_direction == EAST)
	{
		GUI_output(get_tx(35));

	} else if (target_pos == DNG_POS(0,1,8) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			(gs_direction == SOUTH) &&
			(gs_dng02_secret_door1 != 2))
	{
		/* Original-Bug: this should be the leader, not hero no 0 */
		hero = get_hero(0);

		if (gs_dng02_secret_door1 || test_skill(hero, TA_SINNESSCHAERFE, 6) > 0)
		{
			gs_dng02_secret_door1 = 1;

			sprintf(g_dtp2, get_tx(37), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((i = test_skill(hero, TA_SCHLOESSER, 4)) > 0 ? get_tx(39) : get_tx(38)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (i > 0)
			{
				/* unlike other similar code positions, the lower 4 bits of the map entry are preserved here. Is there a reason? */
				and_ptr_bs(amap_ptr + MAP_POS(1,9), (DNG_TILE_CORRIDOR << 4) + 0x0f);
				gs_dng02_secret_door1 = 2;
				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (target_pos == DNG_POS(0,4,8) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			(gs_direction == SOUTH) &&
			(gs_dng02_secret_door2 != 2))
	{
		/* Original-Bug: this should be the leader, not hero no 0 */
		hero = get_hero(0);

		if (gs_dng02_secret_door2 || test_skill(hero, TA_SINNESSCHAERFE, 2) > 0)
		{
			gs_dng02_secret_door2 = 1;

			sprintf(g_dtp2,	get_tx(37), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((i = test_skill(hero, TA_SCHLOESSER, 2)) > 0 ? get_tx(39) : get_tx(38)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			if (i > 0)
			{
				host_writeb(amap_ptr + MAP_POS(4,9), DNG_TILE_CORRIDOR << 4);
				gs_dng02_secret_door2 = 2;
				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (((target_pos == DNG_POS(1,4,9) && gs_direction == EAST) ||
			(target_pos == DNG_POS(1,6,9) && gs_direction == WEST)) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			(gs_dng02_secret_door3 != 2))
	{
		hero = (Bit8u*)get_first_hero_available_in_group();

		if (gs_dng02_secret_door3 || (test_skill(hero, TA_SINNESSCHAERFE, 5) > 0))
		{
			gs_dng02_secret_door3 = 1;

			sprintf(g_dtp2,	get_tx(37), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((i = test_skill(hero, TA_SCHLOESSER, 4)) > 0 ? get_tx(39) : get_tx(38)),
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));

			strcat(g_dtp2, g_text_output_buf);

			if (i > 0)
			{
				host_writeb(amap_ptr + MAP_POS(5,9), DNG_TILE_CORRIDOR << 4);
				gs_dng02_secret_door3 = 2;
				DNG_update_pos();
			}

			i = random_schick(6) + 4;

			sprintf(g_text_output_buf, get_tx(40), (char*)hero + HERO_NAME2, i);

			strcat(g_dtp2, g_text_output_buf);

			GUI_output(g_dtp2);

			sub_hero_le(hero, i);

			gs_direction_bak = gs_direction;
		}

	} else if (target_pos == DNG_POS(0,1,0) && target_pos != gs_dng_handled_pos)
	{
		leave_dungeon();
		gs_current_town = ((signed char)gs_travel_destination_town_id);
		gs_x_target = (gs_travel_destination_x);
		gs_y_target = (gs_travel_destination_y);
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

		sprintf(g_dtp2,
			get_tx(44),
			get_ttx(gs_trv_destination + 0xeb));

		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;

	}

	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG02_chest00_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)gs_dng02_chest00_content);
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest01_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)gs_dng02_chest01_content);
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest02_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)gs_dng02_chest02_content);
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest03_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)gs_dng02_chest03_content);
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest04_func3(Bit8u*)
{
	signed short i;
	signed short answer;
	signed short tw_bak;
	Bit8u *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	if (!gs_dng02_apparature_destroyed)
	{
		do {
			answer = GUI_radio(get_tx(1), 2, get_tx(2), get_tx(3));

		} while (answer == -1);

		if (answer == 1)
		{
			chest_petrified();
		} else {

			gs_dng02_apparature_destroyed = 1;

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group)
				{
					sub_hero_le(hero, random_schick(6));
				}
			}
		}
	} else {
		GUI_output(get_ttx(522));
	}

	g_textbox_width = tw_bak;
}

void DNG02_chest05_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)gs_dng02_chest05_content);
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG02_chest06_open(struct struct_chest* chest)
{
	loot_corpse((Bit8u*)chest, get_tx(19), (Bit8s*)&gs_dng02_corpse_flag);
}

void DNG02_chest06_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)gs_dng02_chest06_content);
	loot_chest((Bit8u*)(chest), get_tx(20), get_tx(21));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

/**
 * \brief   print introductions of dungeon #2 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG02_fight_intro(signed short fight_id)
{
	if (fight_id == 62) {
		GUI_output(get_tx(32));
	} else if (fight_id == 66) {
		GUI_output(get_tx(33));
	} else if (fight_id == 68) {
		GUI_output(get_tx(34));
	} else if (fight_id == 72) {
		GUI_output(get_tx(36));
	}
}

#if !defined(__BORLANDC__)
}
#endif
