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
#include "t_map.h"
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG08_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short tmp;
	signed short tw_bak;
	Bit8u *hero;
	Bit8u *amap_ptr;

	amap_ptr = g_dng_map;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	hero = (Bit8u*)get_first_hero_available_in_group();

	if (target_pos == DNG_POS(0,1,10) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_00))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_00, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,1,9) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_01))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_01, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,1,8) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_02))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_02, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,1,7) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_03))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_03, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,1,6) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_04))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_04, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,6,6) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_05))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_05, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,6,10) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_06))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_06, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,7,7) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_07))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_07, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,7,1) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_08))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_08, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,11,1) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_09))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_09, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,7,4) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_10))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_10, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,10,5) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BED_11))
	{
		if (GUI_bool(get_tx(1)))
		{
			ds_writeb(DNG08_BED_11, 1);
			DNG08_search_bed();
		}

	} else 	if (target_pos == DNG_POS(0,8,5) && target_pos != gs_dng_handled_pos && gs_dng08_waterbarrel)
	{
		DNG_waterbarrel(&gs_dng08_waterbarrel);

	} else 	if (target_pos == DNG_POS(0,3,6) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_TIMER1))
	{
		if (GUI_bool(get_tx(5)))
		{
			GUI_output(get_tx(6));

			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero))
				{
					ds_writeb(DNG08_TIMER1, 24);

					host_writebs(hero + HERO_HUNGER,
						host_writebs(hero + HERO_THIRST, 0));

					add_hero_le(hero, 2);

					tmp = get_free_mod_slot();

					set_mod_slot(tmp, DAYS(1), get_hero(i) + (HERO_ATTRIB + 3 * ATTRIB_CH), -1, (signed char)i);
				}
			}
		}

	} else if (target_pos == DNG_POS(0,8,9) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_TIMER2))
	{
		if (GUI_bool(get_tx(5)))
		{
			GUI_output(get_tx(6));

			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero))
				{
					ds_writeb(DNG08_TIMER2, 24);

					host_writebs(hero + HERO_HUNGER,
						host_writebs(hero + HERO_THIRST, 0));

					add_hero_le(hero, 2);

					tmp = get_free_mod_slot();

					set_mod_slot(tmp, DAYS(1), get_hero(i) + (HERO_ATTRIB + 3 * ATTRIB_CH), -1, (signed char)i);
				}
			}
		}

	} else if (target_pos == DNG_POS(0,1,1) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BALLISTA_DONE))
	{
		if (GUI_bool(get_tx(7)))
		{
			ds_writeb(DNG08_BALLISTA_DONE, 1);

			GUI_output(get_tx(8));

			add_hero_ap_all(10);

			ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,2,7,SOUTH))));
			ds_writew((FIG_FLEE_POSITION + 2), DNG_POS_DIR(0,6,3,EAST));

			do_fight(FIGHTS_F108_3B);
		}

	} else if (target_pos == DNG_POS(0,1,4) && target_pos != gs_dng_handled_pos && !ds_readb(DNG08_BUNDLES_DONE))
	{
		if (GUI_bool(get_tx(9)))
		{
			ds_writeb(DNG08_BUNDLES_DONE, 1);

			GUI_output(get_tx(10));

			add_hero_ap_all(10);

			ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(0,2,7,SOUTH))));
			ds_writew((FIG_FLEE_POSITION + 2), DNG_POS_DIR(0,6,3,EAST));

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
			i = GUI_radio(get_tx(13), 3,
					get_tx(14),
					get_tx(15),
					get_tx(16));
		} while (i == -1);

		if (i == 2)
		{
			GUI_output(get_tx(17));

		} else if (i == 3)
		{
			hero = get_hero(select_hero_ok_forced(get_tx(18)));

			sprintf(g_dtp2,
				get_tx(19),
				(char*)hero + HERO_NAME2);

			GUI_output(g_dtp2);

			/* open door at (8,13) */
			and_ptr_bs(amap_ptr + MAP_POS(8,13), 0x0f);
			or_ptr_bs(amap_ptr + MAP_POS(8,13), DNG_TILE_OPEN_DOOR << 4);

			if (gs_group_member_counts[gs_current_group] > 1) {

				gs_direction_bak = (gs_direction);

				for (tmp = 0; gs_group_member_counts[tmp] != 0; tmp++); /* find empty group */

				host_writebs(hero + HERO_GROUP_NO, (signed char)tmp);
				gs_group_member_counts[tmp]++;
				gs_group_member_counts[gs_current_group]--;

				GRP_save_pos(tmp);
			}

		} else {
			gs_direction_bak = (gs_direction);
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
			and_ptr_bs(amap_ptr + MAP_POS(8,13), 0x0f);
			or_ptr_bs(amap_ptr + MAP_POS(8,13), DNG_TILE_CLOSED_DOOR << 4);
		}

	} else if (target_pos == DNG_POS(0,10,12) && target_pos != gs_dng_handled_pos)
	{
		GUI_output(get_tx(20));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_skill(hero, TA_KLETTERN, 2) <= 0)
			{
				sprintf(g_dtp2,
					get_tx(21),
					(char*)hero + HERO_NAME2,
					(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));
				GUI_output(g_dtp2);

				sub_hero_le(hero, dice_roll(1, 6, 4));
			}
		}

	} else if (target_pos == DNG_POS(0,5,8) && target_pos != gs_dng_handled_pos &&
			gs_direction != gs_direction_bak &&
			gs_direction == WEST &&
			ds_readbs(DNG08_SECRET_DOOR) != 2)
	{
		if (ds_readbs(DNG08_SECRET_DOOR) != 0 ||
			test_skill((hero = (Bit8u*)get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 1) > 0)
		{
			ds_writeb(DNG08_SECRET_DOOR, 1);

			sprintf(g_dtp2,
				get_tx(29),
				(char*)hero + HERO_NAME2);

			sprintf(g_text_output_buf,
				(char*)((tmp = test_skill(hero, TA_SCHLOESSER, 2)) > 0 ? get_tx(30): get_tx(31)),
				(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			strcat(g_dtp2,
				g_text_output_buf);

			GUI_output(g_dtp2);

			if (tmp > 0)
			{
				/* open the secret door */
				host_writeb(amap_ptr + MAP_POS(4,8), 0);
				ds_writeb(DNG08_SECRET_DOOR, 2);
				DNG_update_pos();
			}

			gs_direction_bak = (gs_direction);
		}

	} else if (target_pos == DNG_POS(0,5,7) && target_pos != gs_dng_handled_pos)
	{
		sprintf(g_dtp2,
			get_tx(22),
			(char*)(test_skill(hero, TA_GOETTER_KULTE, 4) <= 0 ? get_tx(23) : get_tx(24)));

		GUI_output(g_dtp2);

	} else if (target_pos == DNG_POS(0,5,9) && target_pos != gs_dng_handled_pos)
	{
		sprintf(g_dtp2,
			get_tx(22),
			(char*)(test_skill(hero, TA_GOETTER_KULTE, 6) <= 0 ? get_tx(23) : get_tx(25)));

		GUI_output(g_dtp2);

	} else if (target_pos == DNG_POS(0,0,14) && target_pos != gs_dng_handled_pos)
	{
		leave_dungeon();

		gs_current_town = ((signed char)gs_travel_destination_town_id);
		gs_x_target = (gs_travel_destination_x);
		gs_y_target = (gs_travel_destination_y);
		gs_current_loctype = LOCTYPE_NONE;
		gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

		sprintf(g_dtp2,
			get_tx(33),
			get_ttx(gs_trv_destination + 0xeb));

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
	Bit8u *hero;

	money = counter = 0;

	hero = (Bit8u*)get_first_hero_available_in_group();

	slot = get_free_mod_slot();

	set_mod_slot(slot, HOURS(6), hero + (HERO_ATTRIB + 3 * ATTRIB_CH), -2, (signed char)get_hero_index(hero));

	if (random_schick(100) <= 10)
	{
		sprintf(g_dtp2,
			get_tx(3),
			(char*)hero + HERO_NAME2,
			(char*)(GUI_names_grammar(2, 92, 0)));

		/* a BRANDY BOTTLE */
		get_item(ITEM_BRANDY, 1, 1);

		GUI_output(g_dtp2);

		counter++;
	}

	if (random_schick(100) <= 10)
	{
		sprintf(g_dtp2,
			get_tx(3),
			(char*)hero + HERO_NAME2,
			(char*)(GUI_names_grammar(2, 14, 0)));

		/* a DAGGER */
		get_item(ITEM_DAGGER, 1, 1);

		GUI_output(g_dtp2);

		counter++;
	}

	if (random_schick(100) <= 10)
	{
		money = random_schick(6);

		sprintf(g_text_output_buf,
			get_tx(4),
			money);

		sprintf(g_dtp2,
			get_tx(3),
			(char*)hero + HERO_NAME2,
			g_text_output_buf);

		GUI_output(g_dtp2);

		add_party_money(money);

		counter++;
	}

	if (!counter)
	{
		GUI_output(get_tx(2));
	}
}

void DNG08_chest1_func2(Bit8u*)
{
	Bit8u *hero;

	hero = (Bit8u*)get_first_hero_available_in_group();

	sprintf(g_dtp2,
		get_tx(11),
		(char*)hero + HERO_NAME2,
		(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));

	GUI_output(g_dtp2);

	/* 3W6 damage */
	sub_hero_le(hero, dice_roll(3, 6, 0));
}

void DNG08_chest0_func3(Bit8u*)
{
	if (!ds_readb(DNG08_CHEST0_LOOTED))
	{
		ds_writeb(DNG08_CHEST0_LOOTED, 1);

		GUI_output(get_tx(32));

		/* get 40 FOOD PACKAGES */
		get_item(ITEM_FOOD_PACKAGE, 1, 40);
	}
}

void DNG08_chest1_func3(Bit8u*)
{
	loot_multi_chest(p_datseg + DNG08_CHEST1_CONTENT, get_tx(34));
}

void DNG08_chest2_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG08_CHEST2_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG08_chest2_func1(Bit8u* chest)
{
	if (!ds_readb(DNG08_CHEST2_LOOTED))
	{
		if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_SPRACHEN, 2) > 0)
		{
			GUI_input(get_tx(27), 10);

			if (!strcmp(g_text_input_buf, (char*)p_datseg + DNG08_STR_TAIRACH))
			{
#if defined(__BORLANDC__)
			((void (*)(Bit8u*))((Bit8u*)host_readd((Bit8u*)(chest) + 11)))(chest);
#else
			t_map(chest, 11)(chest);
#endif

			} else if ((Bit8u*)host_readd((Bit8u*)(chest) + 7) != 0)
			{
#if defined(__BORLANDC__)
			((void (*)(void))((Bit8u*)host_readd((Bit8u*)(chest) + 7)))();
#else
			((treasure_trap)(t_map(chest, 7)))();
#endif
			}
		} else {
			GUI_output(get_tx(26));
		}
	} else {
		GUI_input(get_ttx(522), 0);
	}
}

void DNG08_chest3_func1(Bit8u* chest)
{
	Bit8u *hero;

	hero = (Bit8u*)get_first_hero_available_in_group();

	if (get_first_hero_with_item(ITEM_KEY_BRONZE) != -1 ||
		test_skill(hero, TA_SCHLOESSER, 5) > 0)
	{
		if (!(ds_readb(DNG08_CHEST35_LOOTED) & 1))
		{
			GUI_output(get_tx(28));

			add_party_money(8530L);

			or_ds_bs(DNG08_CHEST35_LOOTED, 1);
		} else {
			GUI_output(get_ttx(522));
		}
	} else {
#if defined(__BORLANDC__)
			((void (*)(void))((Bit8u*)host_readd((Bit8u*)(chest) + 7)))();
#else
			((treasure_trap)(t_map(chest, 7)))();
#endif
	}
}

void DNG08_chest4_func1(Bit8u* chest)
{
	Bit8u *hero;
	hero = (Bit8u*)get_first_hero_available_in_group();

	if (get_first_hero_with_item(ITEM_KEY_BRONZE) != -1 ||
		test_skill(hero, TA_SCHLOESSER, 5) > 0)
	{
#if defined(__BORLANDC__)
		((void (*)(Bit8u*))((Bit8u*)host_readd((Bit8u*)(chest) + 11)))(chest);
#else
		t_map(chest, 11)(chest);
#endif

	} else {
#if defined(__BORLANDC__)
		((void (*)(void))((Bit8u*)host_readd((Bit8u*)(chest) + 7)))();
#else
		((treasure_trap)(t_map(chest, 7)))();
#endif
	}
}

void DNG08_chest5_func1(Bit8u* chest)
{
	Bit8u *hero;
	hero = (Bit8u*)get_first_hero_available_in_group();

	if (get_first_hero_with_item(ITEM_KEY_BRONZE) != -1 ||
		test_skill(hero, TA_SCHLOESSER, 5) > 0)
	{
#if defined(__BORLANDC__)
		((void (*)(Bit8u*))((Bit8u*)host_readd((Bit8u*)(chest) + 11)))(chest);
#else
		t_map(chest, 11)(chest);
#endif

	} else {
#if defined(__BORLANDC__)
		((void (*)(void))((Bit8u*)host_readd((Bit8u*)(chest) + 7)))();
#else
		((treasure_trap)(t_map(chest, 7)))();
#endif
	}
}

void DNG08_chest4_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG08_CHEST4_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG08_chest5_func3(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG08_CHEST5_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);

	if (!(ds_readb(DNG08_CHEST35_LOOTED) & 2))
	{
		GUI_output(get_tx(35));

		add_party_money(2890L);

		or_ds_bs(DNG08_CHEST35_LOOTED, 2);
	}
}

#if !defined(__BORLANDC__)
}
#endif
