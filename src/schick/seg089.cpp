/**
 *	Rewrite of DSA1 v3.02_de functions of seg089 (dungeon: ruined castle)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg089.cpp
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
#include "seg029.h"
#include "seg030.h"
#include "seg032.h"
#include "seg065.h"
#include "seg075.h"
#include "seg076.h"
#include "seg089.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct struct_chest g_dng15_specialchests[33] = {
	{ DNG_POS(0,4,11),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(0,8,14),	0, NULL,			NULL,			DNG15_rotten_clothes_chest, 0, 0, 0 },
	{ DNG_POS(0,14,13),	0, NULL,			NULL,			DNG15_rotten_clothes_chest, 0, 0, 0 },
	{ DNG_POS(1,2,3),	0, NULL,			NULL,			DNG15_rotten_clothes_chest, 0, 0, 0 },
	{ DNG_POS(1,3,3),	3, use_lockpicks_on_chest,	NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(1,14,8),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(1,14,9),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(1,11,8),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(1,11,9),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(1,14,14),	8, use_lockpicks_on_chest,	chest_poisoned1,	DNG15_figures_chest, 0, 0, 0 },
	{ DNG_POS(1,5,10),	3, use_lockpicks_on_chest,	NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(1,6,14),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(2,6,3),	3, use_lockpicks_on_chest,	NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(2,6,7),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(2,14,12),	0, NULL,			NULL,			DNG15_cursed_money_chest, 0, 0, 0 },
	{ DNG_POS(2,14,9),	0, NULL,			NULL,			DNG15_cursed_money_chest, 0, 0, 0 },
	{ DNG_POS(2,13,7),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(2,13,6),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(2,13,5),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,7,14),	3, use_lockpicks_on_chest,	NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,4,10),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,5,10),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,6,10),	0, NULL,			NULL,			DNG15_rotten_clothes_chest, 0, 0, 0 },
	{ DNG_POS(3,7,10),	3, use_lockpicks_on_chest,	NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,7,7),	0, NULL,			NULL,			DNG15_smelling_chest, 0, 0, 0 },
	{ DNG_POS(3,5,3),	0, NULL,			NULL,			DNG15_smelling_chest, 0, 0, 0 },
	{ DNG_POS(3,5,4),	0, NULL,			NULL,			DNG15_smelling_chest, 0, 0, 0 },
	{ DNG_POS(3,11,1),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,9,6),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,9,7),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,14,6),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{ DNG_POS(3,14,7),	0, NULL,			NULL,			DNG15_empty_chest, 0, 0, 0 },
	{     -1, 0, NULL, NULL, NULL, 0, 0, 0 }
}; // ds:0x9a4e

signed short DNG15_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short dir;
	signed short tmp;
	signed short tw_bak;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	dir = gs_direction;

	hero = get_first_hero_available_in_group(); /* UNNEEDED */

	if ((((target_pos == DNG_POS(0,6,3) || target_pos == DNG_POS(0,6,6) || target_pos == DNG_POS(0,6,9)) && dir == EAST) ||
		((target_pos == DNG_POS(0,12,10) || target_pos == DNG_POS(0,8,10)) && dir == NORTH)) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: a large hall */
		GUI_output(get_tx(1));

	} else if (((target_pos == DNG_POS(0,3,4) && dir == NORTH) || (target_pos == DNG_POS(0,4,3) && dir == WEST)) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: entering the tower */
		GUI_output(get_tx(2));

	} else if ((target_pos == DNG_POS(0,1,1) || target_pos == DNG_POS(0,2,1) || target_pos == DNG_POS(0,3,1) || target_pos == DNG_POS(0,4,1) ||
			target_pos == DNG_POS(0,1,2) || target_pos == DNG_POS(0,2,2) || target_pos == DNG_POS(0,3,2) || target_pos == DNG_POS(0,4,2) ||
			target_pos == DNG_POS(0,1,3) || target_pos == DNG_POS(0,2,3) || target_pos == DNG_POS(0,3,3) || target_pos == DNG_POS(0,4,3) ||
			target_pos == DNG_POS(0,1,4) || target_pos == DNG_POS(0,2,4) || target_pos == DNG_POS(0,3,4) || target_pos == DNG_POS(0,4,4)) &&
			target_pos != gs_dng_handled_pos)
	{
		/* TRAP: light wounds */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
				!hero->flags.dead && test_attrib(hero, ATTRIB_GE, -3) <= 0)
			{
				tmp = random_schick(3);

				sprintf(g_dtp2,	(char*)(tmp == 3 ? get_tx(3) : (tmp == 2 ? get_tx(4) : get_tx(5))), hero->alias);
				GUI_output(g_dtp2);

				sub_hero_le(hero, 1);
			}
		}

	} else if ((target_pos == DNG_POS(0,10,1) || target_pos == DNG_POS(0,11,1) || target_pos == DNG_POS(0,11,2) ||
			target_pos == DNG_POS(0,12,2) || target_pos == DNG_POS(0,13,2) || target_pos == DNG_POS(0,14,3)) &&
			target_pos != gs_dng_handled_pos)
	{
		/* debris, with wounds and no hidden ladder */
		DNG15_wounds_and_ladders(get_tx(9), 1, 0);

	} else if ((target_pos == DNG_POS(0,11,0) || target_pos == DNG_POS(0,12,0) || target_pos == DNG_POS(0,13,0) ||
			target_pos == DNG_POS(0,15,0) || target_pos == DNG_POS(0,15,1) || target_pos == DNG_POS(0,14,0)) &&
			target_pos != gs_dng_handled_pos)
	{
		/* debris, no wounds and no hidden ladder */
		DNG15_wounds_and_ladders(get_tx(14), 0, 0);

	} else if (target_pos == DNG_POS(0,14,2) && target_pos != gs_dng_handled_pos)
	{
		/* debris, with wounds and hidden ladder */
		DNG15_wounds_and_ladders(get_tx(15), 1, 1);

	} else if ((target_pos == DNG_POS(0,11,1) || target_pos == DNG_POS(0,12,1) || target_pos == DNG_POS(0,13,1) ||
			target_pos == DNG_POS(0,14,1)) && target_pos != gs_dng_handled_pos)
	{
		/* debris, with wounds and no hidden ladder */
		DNG15_wounds_and_ladders(get_tx(16), 1, 0);

	} else if (target_pos == DNG_POS(0,9,1) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[0]);

	} else if (target_pos == DNG_POS(0,14,4) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[1]);

	} else if (target_pos == DNG_POS(1,14,2) && target_pos != gs_dng_handled_pos)
	{
		/* LADDER: upwards */
		if (GUI_bool(get_tx(17))) {

			DNG_dec_level();
			gs_direction = WEST;
			gs_x_target--;
		}

	} else if (target_pos == DNG_POS(1,8,1) &&
			(target_pos != gs_dng_handled_pos || gs_direction_bak != dir) &&
			(!gs_dng15_lever_south || !gs_dng15_lever_north))
	{
		tmp = dir;

		sprintf(g_dtp2, get_tx(23),
			(char*)(tmp == 0 ? get_tx(22) :
				(tmp == 2 ? get_tx(21) :
				(tmp == 3 ? get_tx(20) : get_tx(19)))));

		for (i = tmp = 0; i < 6; i++)
		{
			if (gs_groups_x_target[i] == 8 && gs_groups_y_target[i] == 5 && gs_current_group != i)
			{
				tmp = 1;
			}
		}

		strcat(g_dtp2, (char*)(tmp == 0 ? get_tx(24) : get_tx(25)));
		GUI_output(g_dtp2);

		gs_direction_bak = (signed char)dir;

	} else if (target_pos == DNG_POS(1,8,5) &&
			(target_pos != gs_dng_handled_pos || gs_direction_bak != dir) &&
			(!gs_dng15_lever_south || !gs_dng15_lever_north))
	{
		tmp = dir;

		sprintf(g_dtp2, get_tx(23),
				(char*)(tmp == 0 ? get_tx(21) :
				(tmp == 2 ? get_tx(22) :
				(tmp == 3 ? get_tx(19) : get_tx(20)))));

		for (i = tmp = 0; i < 6; i++)
		{
			if (gs_groups_x_target[i] == 8 && gs_groups_y_target[i] == 1 && gs_current_group != i)
			{
				tmp = 1;
			}
		}

		strcat(g_dtp2, (char*)(tmp == 0 ? get_tx(24) : get_tx(25)));

		GUI_output(g_dtp2);

		gs_direction_bak = (signed char)dir;

	} else if (target_pos == DNG_POS(1,8,2) && target_pos != gs_dng_handled_pos)
	{
		/* check if another group stands on the other side */
		for (i = tmp = 0; i < 6; i++)
		{
			if (gs_groups_x_target[i] == 8 && gs_groups_y_target[i] == 5 && gs_current_group != i)
			{
				tmp = 1;
			}
		}

		if (tmp == 0) {
			/* go through the mirror */
			GUI_output(get_tx(26));
			gs_y_target = 5;
			DNG_update_pos();
		} else {
			/* stay here */
			GUI_output(get_tx(27));
			gs_y_target = gs_y_target_bak;
		}

	} else if (target_pos == DNG_POS(1,8,4) && target_pos != gs_dng_handled_pos)
	{

		/* check if another group stands on the other side */
		for (i = tmp = 0; i < 6; i++)
		{
			if (gs_groups_x_target[i] == 8 && gs_groups_y_target[i] == 1 && gs_current_group != i)
			{
				tmp = 1;
			}
		}

		if (tmp == 0)
		{
			/* go through the mirror */
			GUI_output(get_tx(26));
			gs_y_target = 1;
			DNG_update_pos();
		} else {
			/* stay here */
			GUI_output(get_tx(27));
			gs_y_target = gs_y_target_bak;
		}

	} else if (target_pos == DNG_POS(1,1,14) && target_pos != gs_dng_handled_pos)
	{
		loot_multi_chest(gs_dng15_chest_equips, get_tx(38));

	} else if (target_pos == DNG_POS(1,1,11) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: wooden beams */
		GUI_output(get_tx(39));

	} else if (target_pos == DNG_POS(1,3,10) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[2]);

	} else if (target_pos == DNG_POS(2,4,10) && target_pos != gs_dng_handled_pos && !gs_dng15_took_hoe)
	{
		/* ITEM: a HOE */
		if (GUI_bool(get_tx(50)))
		{
			GUI_output(get_tx(51));

			if (get_item(ITEM_HACKE, 1, 1) != -1)
			{
				gs_dng15_took_hoe = 1;
			}
		}

	} else if ((target_pos == DNG_POS(2,1,9) || target_pos == DNG_POS(2,3,13)) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: end of the corridor */
		GUI_output(get_tx(52));

	} else if (target_pos == DNG_POS(2,3,12) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[3]);

	} else if (target_pos == DNG_POS(2,3,11) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[4]);

	} else if (target_pos == DNG_POS(2,2,9) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[5]);

	} else if (target_pos == DNG_POS(2,7,3) && target_pos != gs_dng_handled_pos && !gs_dng15_cursed_money[0])
	{
		DNG15_cursed_money(&gs_dng15_cursed_money[0]);

	} else if (target_pos == DNG_POS(2,6,6) && target_pos != gs_dng_handled_pos && !gs_dng15_cursed_money[1])
	{
		DNG15_cursed_money(&gs_dng15_cursed_money[1]);

	} else if (target_pos == DNG_POS(2,8,11) && target_pos != gs_dng_handled_pos && !gs_dng15_cursed_money[2])
	{
		DNG15_cursed_money(&gs_dng15_cursed_money[2]);

	} else if (target_pos == DNG_POS(2,9,7) && target_pos != gs_dng_handled_pos && !gs_dng15_cursed_money[5])
	{
		DNG15_cursed_money(&gs_dng15_cursed_money[5]);

	} else if (target_pos == DNG_POS(2,5,1) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[11]);

	} else if (target_pos == DNG_POS(2,9,3) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[12]);

	} else if (target_pos == DNG_POS(2,9,2) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[13]);

	} else if (target_pos == DNG_POS(2,3,2) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[15]);

	} else if (target_pos == DNG_POS(2,3,3) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[16]);

	} else if (target_pos == DNG_POS(2,3,4) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[17]);

	} else if (target_pos == DNG_POS(3,1,12) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[6]);

	} else if (target_pos == DNG_POS(3,1,13) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[7]);

	} else if (target_pos == DNG_POS(3,1,14) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[8]);

	} else if (target_pos == DNG_POS(3,2,13) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[9]);

	} else if (target_pos == DNG_POS(3,2,14) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(&gs_dng15_ceilings[10]);

	} else if (target_pos == 0x3e02 && target_pos != gs_dng_handled_pos && !gs_dng15_cursed_money[3])
	{
		DNG15_cursed_money(&gs_dng15_cursed_money[3]);

	} else if (target_pos == DNG_POS(3,11,5) && target_pos != gs_dng_handled_pos && !gs_dng15_cursed_money[4])
	{
		DNG15_cursed_money(&gs_dng15_cursed_money[4]);

	} else if (target_pos == DNG_POS(3,11,2) && target_pos != gs_dng_handled_pos)
	{
		/* FIGHT: */
		if (random_schick(100) < 10) /* 9% chance */
		{
			g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[WEST] = DNG_POS_DIR(3,9,2,WEST);
			g_fig_escape_position[SOUTH] = DNG_POS_DIR(3,13,5,SOUTH);

			if (!do_fight(FIGHTS_DFIN26))
			{
				gs_dng15_undead_fight = 1;
			}
		}

	} else if (target_pos == DNG_POS(3,13,3) && target_pos != gs_dng_handled_pos)
	{
		/* FIGHT: */
		if (random_schick(100) < 10)
		{
			g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(3,13,5,SOUTH);

			if (!do_fight(FIGHTS_DFIN26))
			{
				gs_dng15_undead_fight = 1;
			}
		}

	} else if ((target_pos == DNG_POS(2,8,9) || target_pos == DNG_POS(2,9,9)) && target_pos != gs_dng_handled_pos)
	{
		/* FIGHT: */
		g_fig_escape_position[NORTH] = DNG_POS_DIR(2,9,7,NORTH);
		g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = DNG_POS_DIR(2,12,11,EAST);
		g_fig_escape_position[WEST] = DNG_POS_DIR(2,6,9,WEST);

		do_fight(!gs_dng15_undead_fight ? 134 : 133);

	} else if ((target_pos == DNG_POS(3,13,5) || target_pos == DNG_POS(3,9,5)) && target_pos != gs_dng_handled_pos)
	{
		/* FIGHT: */
		if (gs_dng15_undead_fight != 2)
		{
			g_fig_escape_position[NORTH] = DNG_POS_DIR(3,13,3,NORTH);
			g_fig_escape_position[EAST] = DNG_POS_DIR(3,14,5,EAST);
			g_fig_escape_position[SOUTH] = DNG_POS_DIR(3,10,10,SOUTH);
			g_fig_escape_position[WEST] = DNG_POS_DIR(3,9,3,NORTH);

			if (!do_fight(!gs_dng15_undead_fight ? 137 : 136))
			{
				gs_dng15_undead_fight = 2;
			}
		}

	} else if ((target_pos == DNG_POS(3,10,10) || target_pos == DNG_POS(3,13,10) || target_pos == DNG_POS(3,12,14)) &&
			target_pos != gs_dng_handled_pos)
	{
		/* QUEST: hyggelik */
		if (gs_dng15_took_cursed_money)
		{
			/* you are cursed */
			do_talk(19, 1);

		} else if (!gs_got_grimring) {

			/* fight the zombies */
			g_fig_escape_position[NORTH] = g_fig_escape_position[EAST] = g_fig_escape_position[SOUTH] = g_fig_escape_position[WEST] = DNG_POS_DIR(3,10,10,NORTH);

			if (!do_fight(FIGHTS_DFIN28))
			{
				/* talk with hyggelik */
				draw_main_screen();
				DNG_update_pos();
				do_talk(19, 0);
				call_mouse_bg();
				show_hyggelik_ani();

				if (1) { } else { }

				/* group gets GRIMRING */
				do { ; } while (!get_item(ITEM_GRIMRING, 1, 1));

				gs_got_grimring = 1;

				add_hero_ap_all(50);

				g_dng_area_loaded = g_area_prepared = -1;
				g_pp20_index = -2;

				draw_main_screen();
				call_mouse();
			}
		}

	} else if (target_pos == DNG_POS(0,0,11) && target_pos != gs_dng_handled_pos) {

		/* EXIT: may be blocked if cursed money has been taken */
		if (gs_dng15_took_cursed_money)
		{
			GUI_output(get_tx(6));

			gs_x_target = gs_x_target_bak;
			gs_y_target = gs_y_target_bak;
		} else {
			leave_dungeon();

			gs_current_town = gs_travel_destination_town_id;
			gs_x_target = gs_travel_destination_x;
			gs_y_target = gs_travel_destination_y;
			gs_current_loctype = LOCTYPE_NONE;
			gs_direction = ((gs_travel_destination_viewdir + 2) & 0x03);

			sprintf(g_dtp2, get_tx(55), get_ttx(gs_trv_destination + 0xeb));
			GUI_output(g_dtp2);

			timewarp(HOURS(2));

			g_fading_state = 3;
		}
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

/**
 * \brief   heroes may get small wounds
 *
 *          Each alive hero in the group makes a GE-3 test.
 *          If it fails he looses 1 LE.
 */
void DNG15_small_wounds(void)
{
	signed short i;
	signed short randval;
	struct struct_hero *hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++)
	{
		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
			!hero->flags.dead && test_attrib(hero, ATTRIB_GE, -3) <= 0)
		{
			randval = random_schick(3);

			sprintf(g_dtp2, (char*)(randval == 3 ? get_tx(3) : (randval == 2 ? get_tx(7) : get_tx(8))), hero->alias);
			GUI_output(g_dtp2);

			sub_hero_le(hero, 1);
		}
	}
}

/**
 * \brief   group find debris
 *
 *          The group may find a ladder to descent to the next dungeon level.
 *
 * \param   ladder      (0 = no ladder, 1 = ladder)
 */
void DNG15_debris(signed short ladder)
{
	if (GUI_bool(get_tx(10)))
	{
		if (ladder == 0)
		{
			GUI_output(get_tx(11));
		} else {
			GUI_output(get_tx(12));

			if (GUI_bool(get_tx(13)))
			{
				gs_direction = WEST;
				gs_x_target--;
				DNG_inc_level();
			}
		}
	}
}

void DNG15_empty_chest(struct struct_chest* chest)
{
	GUI_output(get_ttx(522));
}

void DNG15_rotten_clothes_chest(struct struct_chest* chest)
{
	GUI_output(get_tx(34));
}

void DNG15_smelling_chest(struct struct_chest* chest)
{
	struct struct_hero *hero;

	if (GUI_bool(get_tx(35))) {

		hero = get_hero(get_random_hero());

		sprintf(g_dtp2,	get_tx(36), hero->alias);
		GUI_output(g_dtp2);

		sub_hero_le(hero, 4);
	}
}

void DNG15_figures_chest(struct struct_chest* chest)
{
	signed short tw_bak;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;
	GUI_output(get_tx(37));
	g_textbox_width = tw_bak;
}

void DNG15_cursed_money_chest(struct struct_chest* chest)
{
	int32_t p_money;
	signed short tw_bak;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	if (GUI_bool(get_tx(54)))
	{
		/* You are cursed ... */
		gs_dng15_took_cursed_money = 1;

		/* ... but get much money and ... */
		p_money = get_party_money();
		p_money += 18928L;
		set_party_money(p_money);

		/* ... 50 GOLD JEWELRY. */
		get_item(ITEM_GOLDSCHMUCK, 1, 50);
	}
	g_textbox_width = tw_bak;
}

void DNG15_collapsing_ceiling(uint8_t* ptr)
{
	signed short i;
	signed short cnt;
	struct struct_hero *hero;

	hero = get_hero(0);

	switch (*ptr)
	{
		case 1:
		{
			/* rotting beams */
			GUI_output(get_tx(40));

			/* count failed GE-3 test */
			for (i = cnt = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
					!hero->flags.dead && test_attrib(hero, ATTRIB_GE, -3) <= 0)
				{
					cnt++;
				}
			}

			if (cnt >= 2)
			{
				/* if more that one hero failed, the ceiling cracks */
				(*ptr)++;

				GUI_output(get_tx(41));
			}
			break;
		}
		case 2:
		{
			/* the beams crash imediately */
			GUI_output(get_tx(42));
			(*ptr)++;
			GUI_output(get_tx(43));

			/* each hero gets 1W6 damage on a failed GE test */
			for (i = cnt = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
					!hero->flags.dead && test_attrib(hero, ATTRIB_GE, 0) <= 0)
				{
					sprintf(g_dtp2, get_tx(44), hero->alias, GUI_get_ptr(hero->sex, 0));
					GUI_output(g_dtp2);

					sub_hero_le(hero, random_schick(6));
				}
			}

			/* way is blocked */
			gs_x_target = gs_x_target_bak;
			gs_y_target = gs_y_target_bak;
			break;
		}
		case 4:
		{
			if (random_schick(100) >= 65)
			{
				/* the way is already cleared */
				GUI_output(get_tx(48));
			} else {
				(*ptr)--;

				if (GUI_bool(get_tx(49)))
				{
					DNG15_clear_way(ptr);
				}
			}
				break;
		}
	}
}

void DNG15_clear_way(uint8_t* ptr)
{
	signed short i;
	struct struct_hero *hero = get_hero(0);
	i = 0;

	/* With all of the following items SHOVEL, HOE, CROWBAR, FRANCESCA ...*/
	if ((get_first_hero_with_item(ITEM_SCHAUFEL) != -1) &&
		(get_first_hero_with_item(ITEM_HACKE) != -1) &&
		(get_first_hero_with_item(ITEM_BRECHEISEN) != -1) &&
		(get_first_hero_with_item(ITEM_WURFBEIL) != -1))
	{
		i = 1;
	}

	/* ... the group saves 30 minutes ingame time. */
	timewarp(!i ? MINUTES(30) : HOURS(1));

	/* With aprobability of 60% the ceiling crashes down. */
	if (random_schick(100) <= 60)
	{
		GUI_output(get_tx(46));

		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
				!hero->flags.dead && test_attrib(hero, ATTRIB_GE, 0) <= 0)
			{
				sprintf(g_dtp2, get_tx(44), hero->alias, GUI_get_ptr(hero->sex, 0));
				GUI_output(g_dtp2);

				sub_hero_le(hero, random_schick(6));
			}
		}

		gs_x_target = gs_x_target_bak;
		gs_y_target = gs_y_target_bak;
	} else {
		(*ptr)++;
		GUI_output(get_tx(47));
	}
}

void DNG15_cursed_money(uint8_t* ptr)
{
	int32_t p_money;

	if (GUI_bool(get_tx(53)))
	{
		*ptr = gs_dng15_took_cursed_money = 1;

		p_money = get_party_money();
		p_money += random_interval(10, 25) * 100;
		set_party_money(p_money);
	}
}

/**
 * \brief
 * \param text		pointer to the output text
 * \param wounds      group may get wounds (0/1)
 * \param ladder      here is a ladder to the next level
 */
void DNG15_wounds_and_ladders(char* text, signed short wounds, signed short ladder)
{
	GUI_output(text);

	if (wounds != 0)
	{
		DNG15_small_wounds();
	}

	DNG15_debris(ladder);
}

#if !defined(__BORLANDC__)
}
#endif
