/**
 *	Rewrite of DSA1 v3.02_de functions of seg084 (dungeon: dungeon)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg084.cpp
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
#include "seg047.h"
#include "seg049.h"
#include "seg075.h"
#include "seg081.h"
#include "seg082.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG09_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short l3;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;

	amap_ptr = g_dng_map;
	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	/* TODO: not the leader ? */
	hero = get_hero(0);

	if (gs_dng09_pit_flag && !(gs_day_timer / 90 * 20))
	{
		GUI_output(get_tx(32));
	}

	if (target_pos == DNG_POS(0,11,14) && target_pos != gs_dng_handled_pos && gs_direction == EAST)
	{
		GUI_output(get_tx(1));

	} else if (target_pos == DNG_POS(0,10,8) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(2));

	} else if ((target_pos == DNG_POS(0,14,14) || target_pos == DNG_POS(0,14,8)) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(3));

	} else if ((target_pos == DNG_POS(0,3,14) || target_pos == DNG_POS(0,5,14) ||
				target_pos == DNG_POS(0,1,14) ||target_pos == DNG_POS(0,1,1) ||
				target_pos == DNG_POS(0,1,3) ||target_pos == DNG_POS(0,6,1) ||
				target_pos == DNG_POS(0,9,1) ||target_pos == DNG_POS(0,6,4) ||
				target_pos == DNG_POS(0,9,4) ||target_pos == DNG_POS(0,12,6) ||
				target_pos == DNG_POS(1,12,7) ||target_pos == DNG_POS(1,14,4) ||
				target_pos == DNG_POS(1,14,6) ||target_pos == DNG_POS(1,13,14) ||
				target_pos == DNG_POS(1,10,14) ||target_pos == DNG_POS(1,1,14) ||
				target_pos == DNG_POS(1,4,8) ||target_pos == DNG_POS(1,11,1) ||
				target_pos == DNG_POS(1,6,1) ||target_pos == DNG_POS(1,8,1) ||
				target_pos == DNG_POS(1,6,3) ||target_pos == DNG_POS(1,8,3)) &&
			target_pos != gs_dng_handled_pos)
	{
		DNG09_statues(10, 20);

	} else if (target_pos == DNG_POS(0,4,11) && target_pos != gs_dng_handled_pos &&
			gs_dng09_bolttrap1_flag && !gs_dng09_lever1_flag)
	{
		GUI_output(get_tx(11));

		gs_dng09_bolttrap1_flag--;

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_attrib((struct struct_hero*)hero, ATTRIB_GE, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf(g_dtp2, get_tx(12), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);
			}
		}

	} else if (target_pos == DNG_POS(0,1,8) && target_pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(14)))
		{
			GUI_output(get_tx(15));

			gs_x_target += 2;
			gs_direction = EAST;
			DNG_inc_level();
		}

	} else if (target_pos == DNG_POS(0,4,5) && target_pos != gs_dng_handled_pos &&
			gs_dng09_bolttrap2_flag && gs_dng09_lever1_flag)
	{
		gs_dng09_bolttrap2_flag--;

		GUI_output(get_tx(11));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_attrib((struct struct_hero*)hero, ATTRIB_GE, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf(g_dtp2,	get_tx(12), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);
			}
		}

	} else if (target_pos == DNG_POS(0,2,2) && target_pos != gs_dng_handled_pos &&
			gs_dng09_bolttrap3_flag)
	{
		gs_dng09_bolttrap3_flag--;

		GUI_output(get_tx(11));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_attrib((struct struct_hero*)hero, ATTRIB_GE, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf(g_dtp2,	get_tx(12), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);
			}
		}

	} else if (target_pos == DNG_POS(0,4,3) && target_pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(16)))
		{
			GUI_output(get_tx(17));

			gs_dng09_lever1_flag ^= 1;
		}

	} else if (target_pos == DNG_POS(0,8,6) && target_pos != gs_dng_handled_pos)
	{
		for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_skill(hero, TA_GEFAHRENSINN, 2) > 0)
			{
				l3 = 1;
			}
		}

		if (l3 != 0)
		{
			GUI_output(get_tx(18));
		} else {
			GUI_output(get_tx(19));

			gs_x_target--;
			gs_y_target += 2;
			DNG_inc_level();
		}

	} else if (target_pos == DNG_POS(1,13,6) && target_pos != gs_dng_handled_pos &&
			gs_dng09_cultist_flag && !gs_dng09_lever2_flag)
	{
		GUI_output(get_tx(11));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_attrib((struct struct_hero*)hero, ATTRIB_GE, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf(g_dtp2,	get_tx(12), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);
			}
		}

	} else if (target_pos == DNG_POS(1,14,9) && target_pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(16)))
		{
			GUI_output(get_tx(28));
			gs_dng09_lever2_flag ^= 1;
		}

	} else if (target_pos == DNG_POS(1,9,11) && target_pos != gs_dng_handled_pos &&
			gs_dng09_bolttrap4_flag && gs_dng09_secretdoor2 == 2)
	{
		GUI_output(get_tx(11));

		/* TODO: add missing gs_dng09_bolttrap4_flag--; */

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_attrib((struct struct_hero*)hero, ATTRIB_GE, 4) <= 0)
			{
				/* failed FF+4 test */
				sub_hero_le(hero, dice_roll(2, 6, 4));

				sprintf(g_dtp2,	get_tx(12), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);
			}
		}

	} else if ((target_pos == DNG_POS(1,6,7) || target_pos == DNG_POS(1,4,1) ||
			target_pos == DNG_POS(1,4,3)) &&
			target_pos != gs_dng_handled_pos) {

		DNG09_statues(20, 50);

	} else if (target_pos == DNG_POS(1,3,12) && target_pos != gs_dng_handled_pos) {

		DNG09_pitfall();

	} else if (target_pos == DNG_POS(1,2,8) && target_pos != gs_dng_handled_pos) {

		if (GUI_bool(get_tx(38))) {
			gs_x_target--;
			gs_y_target++;
			gs_direction = SOUTH;
			DNG_dec_level();
		}

	} else if (target_pos == DNG_POS(1,7,13) && target_pos != gs_dng_handled_pos &&	!gs_dng09_lever_fast) {
		/* lever, removes wall at (4,5), level 2 */
		do {
			i = GUI_radio(get_tx(39), 3, get_tx(40), get_tx(41), get_tx(42));

		} while (i == -1);

		if (i == 2)
		{
			GUI_output(get_tx(43));

		} else if (i == 3)
		{
			if ((i = select_hero_ok(get_tx(44))) != -1)
			{
				hero = get_hero(i);

				sprintf(g_dtp2, get_tx(45), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);

				/* remove wall at (4,5), lvl 2 */
				and_ptr_bs(amap_ptr + MAP_POS(4,5), (DNG_TILE_CORRIDOR << 4) + MAP_POS(15,0));
				or_ptr_bs(amap_ptr + MAP_POS(3,5), 0xf0); /* clear flags */

				if (gs_group_member_counts[gs_current_group] > 1)
				{
					gs_direction_bak = (gs_direction);
					l3 = 0;
					while (gs_group_member_counts[l3])
					{
						l3++;
					}

					host_writeb(hero + HERO_GROUP_NO, (signed char)l3);
					gs_group_member_counts[l3]++;
					gs_group_member_counts[gs_current_group]--;
					GRP_save_pos(l3);
					gs_dng09_lever_fast = 1;
				}
			} else {
				gs_direction_bak = gs_direction;
			}
		} else
		{
			gs_direction_bak = gs_direction;
		}

	} else if ((target_pos == DNG_POS(1,8,13) || target_pos == DNG_POS(1,7,14)) && target_pos != gs_dng_handled_pos)
	{
		/* squares next to lever: check if group at the lever moved away and the wall must be closed */
		/* TODO: potential Original-Bug: What if the group at the lever to use Transversalis to teleport away? I guess the wall is still open */

		/* check if there is still another group on the square of the lever */
		for (i = l3 = 0; i < 6; i++)
		{
			if (gs_groups_x_target[i] == 7 && gs_groups_y_target[i] == 13 &&
				gs_groups_dng_level[i] == 1 && gs_current_group != i)
			{
				l3 = 1;
			}
		}

		/* if not, close wall at (4,5), lvl 2 */
		if (l3 == 0)
		{
			or_ptr_bs(amap_ptr + MAP_POS(4,5), 0xf0); /* clear flags */
			and_ptr_bs(amap_ptr + MAP_POS(3,5), (DNG_TILE_CORRIDOR << 4) + 0x0f);
			gs_dng09_lever_fast = 0;
		}

	} else if (target_pos == DNG_POS(1,5,2) && target_pos != gs_dng_handled_pos && !gs_dng09_altar_flag)
	{
		load_ani(26);
		init_ani(0);
		delay_or_keypress(100);

		if (GUI_bool(get_tx(46))) {

			gs_dng09_altar_flag = 1;

			add_hero_ap_all(60);

			for (i = 0; i < 14; i++) {
				gs_gods_estimation[i] += 100L;
			}

			/* mark quest as done */
			gs_quest_nameless_done = 1;

			do {
				i = GUI_radio(get_tx(47), 2, get_tx(48), get_tx(49));

			} while (i == -1);

			if (i == 1) {

				get_item(ITEM_BLACK_FIGURINE, 1, 1);

			} else {

				GUI_output(get_tx(50));
			}
		}

		set_var_to_zero();
		g_area_prepared = -1;

	} else if (target_pos == DNG_POS(1,4,2) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == WEST && gs_dng09_secretdoor1 != 2)
	{
		if (gs_dng09_secretdoor1 || test_skill((hero = (Bit8u*)get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 6) > 0)
		{
			gs_dng09_secretdoor1 = 1;

			sprintf(g_dtp2,
				get_tx(51),
				(char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)(((l3 = test_skill(hero, TA_SCHLOESSER, 4)) > 0) ? get_tx(52) : get_tx(54)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));

			strcat(g_dtp2,
				g_text_output_buf);

			GUI_output(g_dtp2);

			if (l3 > 0)
			{
				and_ptr_bs(amap_ptr + MAP_POS(3,2), (DNG_TILE_CORRIDOR << 4) + 0x0f);

				gs_dng09_secretdoor1 = 2;

				DNG_update_pos();

			} else {
				sub_hero_le(hero, dice_roll(2, 6, 0));
			}

			gs_direction_bak = gs_direction;
		}
	} else if (target_pos == DNG_POS(1,5,11)  &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == EAST && gs_dng09_secretdoor2 != 2)
	{
		if (gs_dng09_secretdoor2 || test_skill((hero = (Bit8u*)get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 8) > 0)
		{
			gs_dng09_secretdoor2 = 1;

			sprintf(g_dtp2,	get_tx(51), (char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)(((l3 = test_skill(hero, TA_SCHLOESSER, 6)) > 0) ? get_tx(52) : get_tx(53)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)));

			strcat(g_dtp2,
				g_text_output_buf);

			GUI_output(g_dtp2);

			if (l3 > 0)
			{
				and_ptr_bs(amap_ptr + MAP_POS(6,11), (DNG_TILE_CORRIDOR << 4) + 0x0f);

				gs_dng09_secretdoor2 = 2;

				DNG_update_pos();
			}

			gs_direction_bak = gs_direction;
		}

	} else if (target_pos == DNG_POS(0,7,15) && target_pos != gs_dng_handled_pos)
	{
		leave_dungeon();
		gs_current_town = (signed char)gs_travel_destination_town_id;
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

		sprintf(g_dtp2,	get_tx(55), get_ttx(gs_trv_destination + 0xeb));

		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

/**
 * \brief   print introductions of dungeon #9 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG09_fight_intro(signed short fight_id)
{
	if (fight_id == 94)
	{
		GUI_output(get_tx(13));
	}
}

void DNG09_chest00_loot(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng09_chest0;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG09_chest01_loot(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng09_chest1;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG09_chest02_loot(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng09_chest2;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG09_chest03_loot(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng09_chest3;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG09_chest04_loot(Bit8u* chest)
{
	signed short answer;
	Bit8u *hero;

	if (!gs_dng09_crystal_flag)
	{
		if (GUI_bool(get_tx(20)))
		{
			do {
				answer = GUI_radio(get_tx(21), 3, get_tx(22), get_tx(23), get_tx(24));
			} while (answer == -1);

			hero = (Bit8u*)get_first_hero_available_in_group();

			if (answer == 1)
			{
				sprintf(g_dtp2, get_tx(25), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);

				sub_hero_le(hero, dice_roll(2, 20, 0));

			} else if (answer == 2)
			{
				hero = get_hero(0) + select_hero_ok_forced(get_ttx(317)) * SIZEOF_HERO;
				if (test_spell((struct struct_hero*)hero, SP_DESTRUCTIBO_ARCANITAS, 0) > 0)
				{
					sub_ae_splash((struct struct_hero*)hero, 20);

					/* permanent AE loss of 3 */
					sub_ptr_ws(hero + HERO_AE_ORIG, 3);

					GUI_output(get_tx(27));

					add_hero_ap_all(50);

					gs_dng09_crystal_flag = 1;

				} else {
					sprintf(g_dtp2, get_tx(26), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);

					sub_ae_splash((struct struct_hero*)hero, 10);
				}

			} else if (answer == 3)
			{
				hero = get_hero(0) + select_hero_ok_forced(get_ttx(317)) * SIZEOF_HERO;
				if (test_spell((struct struct_hero*)hero, SP_IGNIFAXIUS_FLAMMENSTRAHL, 0) > 0)
				{
					sub_ae_splash((struct struct_hero*)hero, 35);

					/* permanent AE loss of 3 */
					sub_ptr_ws(hero + HERO_AE_ORIG, 3);

					GUI_output(get_tx(27));

					add_hero_ap_all(50);

					gs_dng09_crystal_flag = 1;

				} else {
					sprintf(g_dtp2,	get_tx(26), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);

					sub_ae_splash((struct struct_hero*)hero, 20);
				}
			}
		}
	} else {
		GUI_output(get_ttx(522));
	}
}

void DNG09_chest05_loot(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng09_chest5;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG09_chest06_loot(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng09_chest6;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG09_chest04_trap(void)
{
	g_fig_flee_position[NORTH] = g_fig_flee_position[EAST] = g_fig_flee_position[SOUTH] = g_fig_flee_position[WEST] = DNG_POS_DIR(1,10,11,SOUTH);

	do_fight(FIGHTS_F126_22);
}

#if !defined(__BORLANDC__)
}
#endif
