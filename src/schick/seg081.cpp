/**
 *	Rewrite of DSA1 v3.02_de functions of seg081 (dungeon: cave2)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg081.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg032.h"
#include "seg049.h"
#include "seg075.h"
#include "seg076.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG06_handler(void)
{
	signed short i;
	signed short target_pos;
	signed short l3;
	signed short l4;
	signed short tw_bak;
	Bit32s p_money;
	Bit8u *hero;
	Bit8u *hero_first;
	Bit8u *hero_second;
	Bit8u *amap_ptr;

	amap_ptr = g_dng_map;
	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	if (target_pos == DNG_POS(0,14,5) && target_pos != gs_dng_handled_pos && !gs_dng06_book_flag)
	{
		if (GUI_bool(get_tx(1)) && get_item(ITEM_BOOK_2, 1, 1))
		{
			gs_dng06_book_flag = 1;
		}

	}

	if (target_pos == DNG_POS(0,9,9) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(35));

	} else if (target_pos == DNG_POS(0,4,13) && target_pos != gs_dng_handled_pos)
	{
		if (GUI_bool(get_tx(2)))
		{
			strcpy(g_dtp2, get_tx(3));

			if (!gs_dng06_money_flag)
			{
				gs_dng06_money_flag = 1;

				hero = (Bit8u*)get_first_hero_available_in_group();

				i = dice_roll(3, 20, 0);

				add_party_money(100L * i);

				sprintf(g_text_output_buf, get_tx(4), (char*)hero + HERO_NAME2, i);

				strcat(g_dtp2, g_text_output_buf);
			}

			GUI_output(g_dtp2);
		}

	} else if (target_pos == DNG_POS(0,5,3) && target_pos != gs_dng_handled_pos && !gs_dng06_proviant_flag)
	{
		if (GUI_bool(get_tx(5)))
		{
			get_item(ITEM_FOOD_PACKAGE, 1, 40);
			gs_dng06_proviant_flag = 1;
		}

	} else if (target_pos == DNG_POS(0,2,1) && target_pos != gs_dng_handled_pos)
	{
		hero = get_hero(0);
		for (i = l3 = 0; i < 2; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero))
			{
				l3++;
			}
		}

		hero_first = (Bit8u*)get_first_hero_available_in_group();
		if (l3 > 1)
		{
			hero_second = (Bit8u*)get_second_hero_available_in_group();
		}

		sprintf(g_dtp2, (char*)(l3 == 1 ? g_str_s_war : g_str_s_und_s_waren),
			(char*)hero_first + HERO_NAME2,	(char*)hero_second + HERO_NAME2);

		strcat(g_dtp2, get_tx(6));

		GUI_output(g_dtp2);

		sub_hero_le((struct struct_hero*)hero_first, random_schick(6));
		if (l3 > 1)
		{
			sub_hero_le((struct struct_hero*)hero_second, random_schick(6));
		}

	} else if (target_pos == DNG_POS(0,9,7) && target_pos != gs_dng_handled_pos && !gs_dng06_pitdoor_flag)
	{
		hero = get_hero(0);
		for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_skill((struct struct_hero*)hero, TA_GEFAHRENSINN, 5) > 0)
			{
				l3 = 1;
				break;
			}
		}

		if (l3 != 0)
		{
			if (GUI_bool(get_tx(7)))
			{
				GUI_output(get_tx(8));

				hero = get_hero(0);
				for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
						!hero_dead(hero) &&
						test_skill((struct struct_hero*)hero, TA_GEFAHRENSINN, 5) > 0)
					{
						l3 = 1;
						break;
					}
				}

				if (l3 != 0 && GUI_bool(get_tx(9)))
				{
					gs_dng06_pitdoor_flag = 1;

					GUI_output(get_tx(10));
				}

				if (!gs_dng06_pitdoor_flag)
				{
					DNG_fallpit_test(6);
				}
			} else {
				if (!gs_dng06_pitdoor_flag)
				{
					DNG_fallpit_test(6);
				}
			}
		} else {
			if (!gs_dng06_pitdoor_flag)
			{
				DNG_fallpit_test(6);
			}
		}

	} else if (target_pos == DNG_POS(0,8,7) && target_pos != gs_dng_handled_pos && gs_dng06_goldkey_flag != 2)
	{
		hero = get_hero(0);
		for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_skill((struct struct_hero*)hero, TA_SINNESSCHAERFE, 10) > 0)
			{
				l3++;
			}
		}

		if (gs_dng06_goldkey_flag || l3 != 0)
		{
			gs_dng06_goldkey_flag = 1;

			if (GUI_bool(get_tx(11)))
			{
				gs_dng06_goldkey_flag = 2;

				get_item(ITEM_KEY_GOLDEN_1, 1, 1);

				/* TODO: This is not neccessary */
				hero = (Bit8u*)get_first_hero_available_in_group();

				add_party_money(3000L);
			}
		}

	} else if (target_pos == DNG_POS(0,11,6) && target_pos != gs_dng_handled_pos && gs_dng06_countdown_timer)
	{
		if (div16(amap_ptr[MAP_POS(11,1)]) != DNG_TILE_CLOSED_DOOR ||
			div16(amap_ptr[MAP_POS(2,6)]) != DNG_TILE_CLOSED_DOOR ||
			div16(amap_ptr[MAP_POS(11,4)]) != DNG_TILE_CLOSED_DOOR)
		{
			gs_dng06_countdown_timer--;

			l3 = random_schick(5);

			hero = (Bit8u*)get_first_hero_available_in_group();

			sprintf(g_dtp2, (l3 == 1 ? get_tx(12) : get_tx(13)), (char*)hero + HERO_NAME2);
			GUI_output(g_dtp2);

			sub_hero_le((struct struct_hero*)hero, dice_roll(l3, 6, 0));
		}

	} else if (target_pos == DNG_POS(1,13,5) && target_pos != gs_dng_handled_pos)
	{
		do {
			i = GUI_radio(get_tx(14), 3, get_tx(15), get_tx(16), get_tx(17));

		} while (i == -1);

		hero = (Bit8u*)get_first_hero_available_in_group();

		if (i == 1 || i == 3)
		{
			sprintf(g_dtp2,	(i == 1 ? get_tx(18) : get_tx(19)), (char*)hero + HERO_NAME2,
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0),
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0),
				(char*)hero + HERO_NAME2,
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 1));

			GUI_output(g_dtp2);

			sub_hero_le((struct struct_hero*)hero, dice_roll(3, 6, 0));
		} else {

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero) &&
					test_skill((struct struct_hero*)hero, TA_KOERPERBEHERRSCHUNG, host_readbs(hero + HERO_RS_BONUS1)) <= 0)
				{
					sprintf(g_dtp2,	get_tx(20), (char*)hero + HERO_NAME2,
						GUI_get_ptr(host_readbs(hero + HERO_SEX), 0),
						GUI_get_ptr(host_readbs(hero + HERO_SEX), 3));

					GUI_output(g_dtp2);

					sub_hero_le((struct struct_hero*)hero, dice_roll(3, 6, 0));
				}
			}
		}

	} else if (target_pos == DNG_POS(1,1,2) && target_pos != gs_dng_handled_pos)
	{
		/* Hole in the wall: want to reach into it? want to move the lever? */
		if (GUI_bool(get_tx(21)) && GUI_bool(get_tx(22)))
		{
			for (i = l3 = 0; i < 6; i++)
			{
				if (gs_groups_x_target[i] == 3 && gs_groups_y_target[i] == 6 &&
					gs_current_group != i)
				{
					hero = get_hero(0);
					for (l4 = 0; l4 <= 6; l4++, hero += SIZEOF_HERO)
					{
						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == i)
						{
							l3 = 1;
							break;
						}
					}
				}
			}

			if (l3 != 0)
			{
				/* some heroes are in the right position: hear gentle click */
				GUI_output(get_tx(24));

				gs_dng06_lever_flag = 1;
			} else {
				/* liquid from ceiling causes damage */
				hero = (Bit8u*)get_first_hero_available_in_group();

				sprintf(g_dtp2, get_tx(23), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);

				sub_hero_le((struct struct_hero*)hero, 2);
			}
		}

	} else if (target_pos == DNG_POS(1,3,6) && target_pos != gs_dng_handled_pos)
	{
		/* Hole in the wall: want to reach into it? want to move the lever? */
		if (GUI_bool(get_tx(21)) && GUI_bool(get_tx(22)))
		{
			for (i = l3 = 0; i < 6; i++)
			{
				if (gs_groups_x_target[i] == 1 && gs_groups_y_target[i] == 2 &&
					gs_current_group != i)
				{
					hero = get_hero(0);
					for (l4 = 0; l4 <= 6; l4++, hero += SIZEOF_HERO)
					{
						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == i)
						{
							l3 = 1;
							break;
						}
					}
				}
			}

			if (l3 != 0)
			{
				/* some heroes are in the right position: hear gentle click */
				GUI_output(get_tx(24));

				gs_dng06_lever_flag = 1;
			} else {
				/* liquid from ceiling causes damage */
				hero = (Bit8u*)get_first_hero_available_in_group();

				sprintf(g_dtp2,	get_tx(23), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);

				sub_hero_le((struct struct_hero*)hero, 2);
			}
		}

	} else if (target_pos == DNG_POS(1,5,8) && target_pos != gs_dng_handled_pos && !gs_dng06_lever_flag)
	{
		hero = (Bit8u*)get_first_hero_available_in_group();

		/* large disk comes down from ceiling */
		sprintf(g_dtp2,	get_tx(25), (char*)hero + HERO_NAME2);
		GUI_output(g_dtp2);

		if (test_skill((struct struct_hero*)hero, TA_KOERPERBEHERRSCHUNG, 0) > 0 && test_skill((struct struct_hero*)hero, TA_GEFAHRENSINN, 0) > 0)
		{
			/* evasion succeeds */
			sprintf(g_dtp2, get_tx(26), GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));
			GUI_output(g_dtp2);

		} else {
			/* disk causes heavy damage */
			sprintf(g_dtp2,	get_tx(27), GUI_get_ptr(host_readbs(hero + HERO_SEX), 2));
			GUI_output(g_dtp2);

			sub_hero_le((struct struct_hero*)hero, dice_roll(3, 6, 0));
		}

	} else if (target_pos == DNG_POS(1,7,13) && target_pos != gs_dng_handled_pos && !gs_dng06_fight19_flag)
	{
		if (GUI_bool(get_tx(28)))
		{
			hero = (Bit8u*)get_first_hero_available_in_group();

			sprintf(g_dtp2,	get_tx(29), (char*)hero + HERO_NAME2,
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0),
				GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));

			GUI_output(g_dtp2);

			sub_hero_le((struct struct_hero*)hero, 2);
		}

	} else if (target_pos == DNG_POS(1,6,11) && target_pos != gs_dng_handled_pos && !gs_dng06_dasptreas_found)
	{
		/* treasure of Daspota found */
		/* REMARK: should be modified, in a way that looting is still possible */
		GUI_output(get_tx(36));

		p_money = get_party_money();
		p_money += 50000L;
		set_party_money(p_money);

		loot_multi_chest(gs_dng06_chest_dasptreas, get_tx(37));

		gs_dng06_dasptreas_found = 1;

		add_hero_ap_all(30);

	} else if (target_pos == DNG_POS(1,8,14) && target_pos != gs_dng_handled_pos)
	{
		g_fig_flee_position[NORTH] = g_fig_flee_position[WEST] = DNG_POS_DIR(1,6,14,WEST);
		g_fig_flee_position[SOUTH] = g_fig_flee_position[EAST] = DNG_POS_DIR(1,13,13,NORTH);

		if (!do_fight(FIGHTS_F094_19))
		{
			gs_dng06_fight19_flag = 1;
		}

	} else if (target_pos == DNG_POS(0,13,15) && target_pos != gs_dng_handled_pos)
	{
		leave_dungeon();

		gs_current_town = gs_travel_destination_town_id;
		gs_x_target = gs_travel_destination_x;
		gs_y_target = gs_travel_destination_y;
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

		sprintf(g_dtp2,	get_tx(38), get_ttx(gs_trv_destination + 0xeb));
		GUI_output(g_dtp2);

		timewarp(HOURS(2));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}


/**
 * \brief   print introductions of dungeon #6 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG06_fight_intro(signed short fight_id)
{
	if (fight_id == FIGHTS_F094_22)
	{
		/* this is true all the time */
		sprintf(g_dtp2,	get_tx(30), (!gs_estorik_known ? get_tx(32) : get_tx(31)));
		GUI_output(g_dtp2);
	}
}


void DNG06_chest00_loot(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng06_chest1_content;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}


void DNG06_chest01_loot(Bit8u* chest)
{
	signed short i;
	Bit8u *hero;

	sprintf(g_dtp2, get_tx(33));

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
			!hero_dead(hero) &&
			test_skill((struct struct_hero*)hero, TA_LESEN, 0) > 0)
		{
			strcat(g_dtp2, get_tx(34));
			break;
		}
	}

	GUI_output(g_dtp2);
}

/**
 * \brief   handles the pit in Kultstaette des Namenlosen, level 2, square (3,12)
 *
 * 	heroes in the pit are separated into a new group.
 * 	This group will have an entry GROUPS_DNG_LEVEL = 2,
 * 	which indicates that it is in the pit.
 */

void DNG09_pitfall(void)
{
	signed short i;
	signed short l3;
	Bit8u *hero;
	Bit8u *hero_first;
	Bit8u *hero_second;

	hero = (Bit8u*)get_first_hero_available_in_group();

	if (!gs_dng09_pit_flag)
	{
		for (i = l3 = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_skill((struct struct_hero*)hero, TA_GEFAHRENSINN, 4) > 0)
				/* TODO: potential Original-Bug: Why should 'petrified' or 'uncouscious' (or maybe other properties ) be o.k. here?? */
			{
				l3 = 1;
			}
		}

		if (l3 != 0)
		{
			GUI_output(get_tx(29));

		} else {
			hero_first = (Bit8u*)get_first_hero_available_in_group();
			hero_second = (Bit8u*)get_second_hero_available_in_group();

			if (gs_group_member_counts[gs_current_group] >= 2)
			{
				/* the current group has at least two heroes */

				/* print message */
				sprintf(g_dtp2, get_tx(30), (char*)hero_first + HERO_NAME2, (char*)hero_second + HERO_NAME2);
				GUI_output(g_dtp2);

				/* each of these two heroes looses 3W6+3 LE */
				sub_hero_le((struct struct_hero*)hero_first, dice_roll(3, 6, 3));
				sub_hero_le((struct struct_hero*)hero_second, dice_roll(3, 6, 3));

				/* find an empty group */
				l3 = 0;
				while (gs_group_member_counts[l3] != 0) l3++;

				/* put these heroes in empty group */
				host_writeb(hero_first + HERO_GROUP_NO, (signed char)l3);
				host_writeb(hero_second + HERO_GROUP_NO, (signed char)l3);
				gs_group_member_counts[l3] += 2;
				gs_group_member_counts[gs_current_group] -= 2;

				GRP_save_pos(l3);

				gs_groups_dng_level[l3] = gs_dungeon_level + 1;

				gs_dng09_pit_flag = 2;
			} else {
				/* the current group has only one hero */

				/* print message */
				sprintf(g_dtp2, get_tx(31), (char*)hero_first + HERO_NAME2, GUI_get_ptr(host_readbs(hero_first + HERO_SEX), 0));
				GUI_output(g_dtp2);

				/* this hero looses 3W6+3 LE */
				sub_hero_le((struct struct_hero*)hero_first, dice_roll(3, 6, 3));

				/* find an empty group */
				l3 = 0;
				while (gs_group_member_counts[l3] != 0) l3++;

				/* put this hero in an empty group */
				host_writeb(hero_first + HERO_GROUP_NO, (signed char)l3);
				gs_group_member_counts[l3]++;
				gs_group_member_counts[gs_current_group]--;

				GRP_save_pos(l3);

				gs_groups_dng_level[l3] = gs_dungeon_level + 1;

				gs_dng09_pit_flag = 1;
			}
		}

	} else if (GUI_bool(get_tx(33))) {

		if ((i = DNG_check_climb_tools()) != -1)
		{
			l3 = group_count_item(ITEM_ROPE);
			l3 += group_count_item(ITEM_ROPE_LADDER);

			if (l3 >= 2 || (l3 == 1 && i))
			{
				host_writeb((Bit8u*)g_dtp2, 0);

				if (i) {
					sprintf(g_dtp2, get_ttx(768), (char*)get_hero(i - 1) + HERO_NAME2);
				}

				strcat(g_dtp2, (char*)(gs_dng09_pit_flag == 2 ? get_tx(36) : get_tx(37)));

				GUI_output(g_dtp2);

				for (i = 0; i < 6; i++)	{

					if (gs_groups_dng_index[i] == 9 && gs_groups_dng_level[i] == 2) {
						gs_groups_dng_level[i] = 1;
					}
				}

				GRP_merge();

				gs_dng09_pit_flag = 0;

			} else {
				GUI_output(get_tx(35));
			}
		} else {
			GUI_output(get_tx(34));
		}
	}
}


#if !defined(__BORLANDC__)
}
#endif
