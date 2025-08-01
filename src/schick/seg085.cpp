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
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG10_handler(void)
{
	signed short target_pos;
	signed short answer;
	signed short result;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;
	Bit32s p_money;

	amap_ptr = p_datseg + DNG_MAP;
	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = (Bit8u*)get_first_hero_available_in_group();

	if ((target_pos == DNG_POS(0,1,1) || target_pos == DNG_POS(0,3,2)) && target_pos != gs_dng_handled_pos)
	{
		/* TRAP: a hole in a wall; leader gets 2 LE damage */
		if (GUI_bool(get_tx(1)))
		{
			sprintf(g_dtp2,
				get_tx(2),
				(char*)hero + HERO_NAME2);

			GUI_output(g_dtp2);

			sub_hero_le(hero, 2);
		}

	} else if (target_pos == DNG_POS(0,5,2) && target_pos != gs_dng_handled_pos)
	{
		/* a dead dwarf */
		seg092_06b4(0);

	} else if (target_pos == DNG_POS(0,8,1) && target_pos != gs_dng_handled_pos)
	{
		/* another hole in a wall with a lever for a trap */
		if ((ds_readb(DNG10_LEVER_FOUND) != 0) || test_skill(hero, TA_SINNESSCHAERFE, 7) > 0)
		{
			or_ds_bs(DNG10_LEVER_FOUND, 1);

			if (GUI_bool((!(ds_readb(DNG10_LEVER_FOUND) & 2) ? get_tx(6) : get_tx(41))))
			{
				if (GUI_bool(get_tx(7)))
				{
					or_ds_bs(DNG10_LEVER_FOUND, 2);
					xor_ds_bs(DNG10_LEVER_STATE, 1);

					GUI_output(get_tx(8));
				}
			}
		}

	} else if ((target_pos == DNG_POS(0,8,4) && target_pos != gs_dng_handled_pos && !ds_readb(DNG10_LEVER_STATE)) ||
			(target_pos == DNG_POS(0,7,4) && target_pos != gs_dng_handled_pos && ds_readb(DNG10_LEVER_STATE) != 0))
	{
		/* TRAP: terrible pain; radom hero gets 3W6+4 LE damage */
		hero = get_hero(get_random_hero());

		answer = dice_roll(3, 6, 4);

		sprintf(g_dtp2,
			get_tx(9),
			(char*)hero + HERO_NAME2);

		/* check if the hero will survive */
		if (host_readws(hero + HERO_LE) > answer)
		{
			sprintf(g_text_output_buf,
				get_tx(10),
				(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat(g_dtp2,
				g_text_output_buf);
		}

		GUI_output(g_dtp2);

		sub_hero_le(hero, answer);

	} else if (target_pos == DNG_POS(0,3,10) &&
			(target_pos != gs_dng_handled_pos || gs_direction != gs_direction_bak) &&
			gs_direction == WEST)
	{
		/* TRAP: a loose stone in a wall */
		if (ds_readb(DNG10_HOLE_STATE) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 5) > 0)
		{
			/* Original-Bug: ??? */
			ds_writeb(DNG10_HOLE_STATE, 1);

			if (ds_readb(DNG10_HOLE_STATE) == 2 || GUI_bool(get_tx(6)))
			{
				ds_writeb(DNG10_HOLE_STATE, 2);

				sprintf(g_dtp2,
					get_tx(11),
					(char*)hero + HERO_NAME2);

				if (GUI_bool(g_dtp2))
				{
					sprintf(g_dtp2,
						get_tx(12),
						(char*)hero + HERO_NAME2);

					GUI_output(g_dtp2);

					result = random_schick(6);

					add_ds_bs(DNG10_HOLE_DAMAGE, result);

					sub_hero_le(hero, result);

					if (ds_readb(DNG10_HOLE_DAMAGE) >= 7)
					{
						/* the secret wall can now be passed forward */
						host_writeb(amap_ptr + MAP_POS(3,7), (DNG_TILE_SEMIPERMEABLE_WALL << 4) + 0x0f); /* set flags 0,1,2,3 => can be entered from all directions */
					}
				}

				gs_direction_bak = (gs_direction);

			} else {
			}

			gs_dng_handled_pos = target_pos;
		}
	} else if (target_pos == DNG_POS(0,1,8) && target_pos != gs_dng_handled_pos)
	{
		/* TRAP: a floorplate */
		if (ds_readb(DNG10_FLOORPLATE_FOUND) != 0 || test_skill(hero, TA_SINNESSCHAERFE, 5) > 0)
		{
			ds_writeb(DNG10_FLOORPLATE_FOUND, 1);

			/* Original-Bug: ???*/
			/* Damage only happens here when the leader of the group tries to disable this trap.
			   If the trap is not found or left alone nobody gets damaged. Weird! */
			if (GUI_bool(get_tx(13)) && test_skill(hero, TA_SCHLOESSER, 7) <= 0)
			{
				if (ds_readb(DNG10_FLOORPLATE_LOADS) != 0)
				{

					sprintf(g_dtp2,
						get_tx(14),
						(char*)hero + HERO_NAME2);

					dec_ds_bs_post(DNG10_FLOORPLATE_LOADS);

					sub_hero_le(hero, dice_roll(3, 6, 0));
				} else {
					strcpy(g_dtp2,
						get_tx(15));
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

	} else if (target_pos == DNG_POS(0,10,13) && target_pos != gs_dng_handled_pos && !ds_readb(DNG10_HESHTOT))
	{
		/* FIGHT: scared heshtot */
		if (GUI_bool(get_tx(22)))
		{
			ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,10,13,NORTH)))));
			gs_dng_handled_pos = 0;

			if (!do_fight(FIGHTS_F129_17))
			{
				ds_writeb(DNG10_HESHTOT, 1);
			}
		} else {
			GUI_output(get_tx(23));
			sub_group_le(random_schick(6));

			gs_x_target = (1);
			gs_y_target = (3);
			gs_direction = (WEST);
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
			for (answer = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero))
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
			xor_ds_bs(DNG10_MUMMY_LEVER, 1);

			sprintf(g_dtp2,
				get_tx(26),
				(char*)hero + HERO_NAME2);

			GUI_output(g_dtp2);

			sub_hero_le(hero, 2);
		}

	} else if ((target_pos == DNG_POS(1,10,3) || target_pos == DNG_POS(1,7,3)) && target_pos != gs_dng_handled_pos && ds_readb(DNG10_MUMMY_LEVER) != 0)
	{
		/* FIGHT: four mummies again and again */
		ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,5,3,WEST)));
		ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), DNG_POS_DIR(1,12,3,EAST)));
		g_fig_discard = 1;
		do_fight(FIGHTS_F129_21);

	} else if (target_pos == DNG_POS(1,1,12) && target_pos != gs_dng_handled_pos && gs_direction == SOUTH)
	{
		/* INFO: glowing walls */
		GUI_output(get_tx(27));

	} else if (target_pos == DNG_POS(1,9,12) && target_pos != gs_dng_handled_pos && ds_readb(DNG10_DRAGON_QUEST) != 0)
	{
		/* FIGHT: get PLATINKEY for the dragon */
		ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,9,10,NORTH)));
		ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), DNG_POS_DIR(1,9,10,NORTH)));
		do_fight(FIGHTS_F129_29);

	} else if (target_pos == DNG_POS(2,12,12) && target_pos != gs_dng_handled_pos)
	{
		/* QUEST: the dragon */
		/* TIP: plunder sucessfully, do the quest and get the reward */
		if (!ds_readb(DNG10_DRAGON_QUEST))
		{
			load_in_head(58);

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28),
						get_tx(29), 2,
						get_tx(30), get_tx(31));
			} while (answer == -1);

			if (answer == 1)
			{
				/* try to fight the dragon */
				ds_writeb(DNG10_DRAGON_QUEST, 1);

				if (GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28),
						get_tx(32), 2,
						get_ttx(2), get_ttx(3)) == 1)
				{
					GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(33), 0);
				} else {
					GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(34), 0);

					sub_group_le(5000);
				}
			} else {
				/* try to plunder the hoard */
				hero = get_hero(0);

				for (answer = result = 0; answer <= 6; answer++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
						!hero_dead(hero) &&
						test_skill(hero, TA_SCHLEICHEN, host_readbs(hero + HERO_RS_BONUS1) + 3) <= 0)
					{
						result++;
					}
				}

				if (result == 0 && !ds_readb(DNG10_HOARD_PLUNDERED))
				{
					ds_writeb(DNG10_HOARD_PLUNDERED, 1);

					GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(35), 0);

					/* 2x HEALING POTION, MAGIC POTION, THROWING DAGGER and ...*/
					get_item(ITEM_LE_POTION, 1, 2);
					get_item(ITEM_AE_POTION, 1, 1);
					get_item(ITEM_THROWING_DAGGER_MAGIC, 1, 1);

					/* 200 Ducats */
					p_money = get_party_money();
					p_money += 20000L;
					set_party_money(p_money);

				} else {
					ds_writeb(DNG10_DRAGON_QUEST, 1);

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

		} else if (ds_readb(DNG10_DRAGON_QUEST) == 1) {

			load_in_head(58);

			gs_x_target = (gs_y_target = (12));

			if ((answer = get_first_hero_with_item(ITEM_KEY_PLATIN)) != -1)
			{
				hero = get_hero(answer);
				result = get_item_pos(hero, ITEM_KEY_PLATIN);
				drop_item(hero, result, 1);

				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(36), 0);
				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(28), get_tx(37), 0);

				/* 2x HEALING POTION, MAGIC POTION, THROWING DAGGER, CRYSTAL BALL and ...*/
				get_item(ITEM_LE_POTION, 1, 2);
				get_item(ITEM_AE_POTION, 1, 1);
				get_item(ITEM_THROWING_DAGGER_MAGIC, 1, 1);
				get_item(ITEM_CRYSTAL_BALL, 1, 1);

				/* ... 200 Ducats and ... */
				p_money = get_party_money();
				p_money += 20000L;
				set_party_money(p_money);

				/* ... 50 AP */
				add_hero_ap_all(50);

				ds_writeb(DNG10_DRAGON_QUEST, 2);
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

		gs_current_town = (ds_readbs(TRAVEL_DESTINATION_TOWN_ID));
		gs_x_target = (ds_readws(TRAVEL_DESTINATION_X));
		gs_y_target = (ds_readws(TRAVEL_DESTINATION_Y));
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((ds_readbs(TRAVEL_DESTINATION_VIEWDIR) + 2) & 0x03);

		sprintf(g_dtp2,
			get_tx(40),
			get_ttx(ds_readw(TRV_DESTINATION) + 0xeb));

		GUI_output(g_dtp2);

		timewarp(HOURS(3));

		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG10_chest0_x1(Bit8u* ptr)
{
	loot_corpse(ptr, get_tx(3), p_datseg + DNG10_CORPSE_LOOTED);
}

void DNG10_chest0_x2(Bit8u* chest)
{
	Bit8u* ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG10_CHEST0_CONTENT));
	loot_chest((Bit8u*)(chest), get_tx(4), get_tx(5));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG10_chest1_x1(Bit8u* chest)
{
	Bit8u* ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG10_CHEST1_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG10_chest2_x1(Bit8u* chest)
{
	Bit8u* ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG10_CHEST2_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG10_chest3_x1(Bit8u* chest)
{
	Bit8u* ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG10_CHEST3_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG10_chest4_x1(Bit8u* chest)
{
	Bit8u* ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG10_CHEST4_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

#if !defined(__BORLANDC__)
}
#endif
