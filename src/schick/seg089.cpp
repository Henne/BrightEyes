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

signed short DNG15_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short dir;
	signed short tmp;
	signed short tw_bak;
	Bit8u *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	dir = gs_direction;

	hero = (Bit8u*)get_first_hero_available_in_group();

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
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_attrib(hero, ATTRIB_GE, -3) <= 0)
			{
				tmp = random_schick(3);

				sprintf(g_dtp2,
					(char*)(tmp == 3 ? get_tx(3) : (tmp == 2 ? get_tx(4) : get_tx(5))),
					(char*)hero + HERO_NAME2);

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
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 0));

	} else if (target_pos == DNG_POS(0,14,4) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 1));

	} else if (target_pos == DNG_POS(1,14,2) && target_pos != gs_dng_handled_pos)
	{
		/* LADDER: upwards */
		if (GUI_bool(get_tx(17))) {

			DNG_dec_level();
			gs_direction = (WEST);
			gs_x_target--;
		}

	} else if (target_pos == DNG_POS(1,8,1) &&
			(target_pos != gs_dng_handled_pos || gs_direction_bak != dir) &&
			(!ds_readb(DNG15_LEVER_SOUTH) || !ds_readb(DNG15_LEVER_NORTH)))
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

		gs_direction_bak = ((signed char)dir);

	} else if (target_pos == DNG_POS(1,8,5) &&
			(target_pos != gs_dng_handled_pos || gs_direction_bak != dir) &&
			(!ds_readb(DNG15_LEVER_SOUTH) || !ds_readb(DNG15_LEVER_NORTH)))
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

		gs_direction_bak = ((signed char)dir);

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
			gs_y_target = (5);
			DNG_update_pos();
		} else {
			/* stay here */
			GUI_output(get_tx(27));
			gs_y_target = (gs_y_target_bak);
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
			gs_y_target = (1);
			DNG_update_pos();
		} else {
			/* stay here */
			GUI_output(get_tx(27));
			gs_y_target = (gs_y_target_bak);
		}

	} else if (target_pos == DNG_POS(1,1,14) && target_pos != gs_dng_handled_pos)
	{
		loot_multi_chest(p_datseg + DNG15_CHEST_EQUIPS, get_tx(38));

	} else if (target_pos == DNG_POS(1,1,11) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: wooden beams */
		GUI_output(get_tx(39));

	} else if (target_pos == DNG_POS(1,3,10) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 2));

	} else if (target_pos == DNG_POS(2,4,10) && target_pos != gs_dng_handled_pos && !ds_readb(DNG15_TOOK_HOE))
	{
		/* ITEM: a HOE */
		if (GUI_bool(get_tx(50)))
		{
			GUI_output(get_tx(51));

			if (get_item(ITEM_PICKAXE, 1, 1) != -1)
			{
				ds_writeb(DNG15_TOOK_HOE, 1);
			}
		}

	} else if ((target_pos == DNG_POS(2,1,9) || target_pos == DNG_POS(2,3,13)) && target_pos != gs_dng_handled_pos)
	{
		/* INFO: end of the corridor */
		GUI_output(get_tx(52));

	} else if (target_pos == DNG_POS(2,3,12) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 3));

	} else if (target_pos == DNG_POS(2,3,11) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 4));

	} else if (target_pos == DNG_POS(2,2,9) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 5));

	} else if (target_pos == DNG_POS(2,7,3) && target_pos != gs_dng_handled_pos && !ds_readb((DNG15_CURSED_MONEY_A + 0)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 0));

	} else if (target_pos == DNG_POS(2,6,6) && target_pos != gs_dng_handled_pos && !ds_readb((DNG15_CURSED_MONEY_A + 1)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 1));

	} else if (target_pos == DNG_POS(2,8,11) && target_pos != gs_dng_handled_pos && !ds_readb((DNG15_CURSED_MONEY_A + 2)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 2));

	} else if (target_pos == DNG_POS(2,9,7) && target_pos != gs_dng_handled_pos && !ds_readb((DNG15_CURSED_MONEY_A + 5)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 5));

	} else if (target_pos == DNG_POS(2,5,1) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 11));

	} else if (target_pos == DNG_POS(2,9,3) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 12));

	} else if (target_pos == DNG_POS(2,9,2) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 13));

	} else if (target_pos == DNG_POS(2,3,2) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 15));

	} else if (target_pos == DNG_POS(2,3,3) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 16));

	} else if (target_pos == DNG_POS(2,3,4) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 17));

	} else if (target_pos == DNG_POS(3,1,12) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 6));

	} else if (target_pos == DNG_POS(3,1,13) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 7));

	} else if (target_pos == DNG_POS(3,1,14) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 8));

	} else if (target_pos == DNG_POS(3,2,13) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 9));

	} else if (target_pos == DNG_POS(3,2,14) && target_pos != gs_dng_handled_pos)
	{
		DNG15_collapsing_ceiling(p_datseg + (DNG15_CEILINGS_A + 10));

	} else if (target_pos == TEVENT128_FLAG && target_pos != gs_dng_handled_pos && !ds_readb((DNG15_CURSED_MONEY_A + 3)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 3));

	} else if (target_pos == DNG_POS(3,11,5) && target_pos != gs_dng_handled_pos && !ds_readb((DNG15_CURSED_MONEY_A + 4)))
	{
		DNG15_cursed_money(p_datseg + (DNG15_CURSED_MONEY_A + 4));

	} else if (target_pos == DNG_POS(3,11,2) && target_pos != gs_dng_handled_pos)
	{
		/* FIGHT: */
		if (random_schick(100) < 10) /* 9% chance */
		{
			ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(3,9,2,WEST))));
			ds_writew((FIG_FLEE_POSITION + 4), DNG_POS_DIR(3,13,5,SOUTH));

			if (!do_fight(FIGHTS_DFIN26))
			{
				ds_writeb(DNG15_UNDEAD_FIGHT, 1);
			}
		}

	} else if (target_pos == DNG_POS(3,13,3) && target_pos != gs_dng_handled_pos)
	{
		/* FIGHT: */
		if (random_schick(100) < 10)
		{
			ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(3,13,5,SOUTH)))));

			if (!do_fight(FIGHTS_DFIN26))
			{
				ds_writeb(DNG15_UNDEAD_FIGHT, 1);
			}
		}

	} else if ((target_pos == DNG_POS(2,8,9) || target_pos == DNG_POS(2,9,9)) && target_pos != gs_dng_handled_pos)
	{
		/* FIGHT: */
		ds_writew((FIG_FLEE_POSITION + 0), DNG_POS_DIR(2,9,7,NORTH));
		ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), DNG_POS_DIR(2,12,11,EAST)));
		ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(2,6,9,WEST));

		do_fight(!ds_readb(DNG15_UNDEAD_FIGHT) ? 134 : 133);

	} else if ((target_pos == DNG_POS(3,13,5) || target_pos == DNG_POS(3,9,5)) && target_pos != gs_dng_handled_pos)
	{
		/* FIGHT: */
		if (ds_readb(DNG15_UNDEAD_FIGHT) != 2)
		{
			ds_writew((FIG_FLEE_POSITION + 0), DNG_POS_DIR(3,13,3,NORTH));
			ds_writew((FIG_FLEE_POSITION + 2), DNG_POS_DIR(3,14,5,EAST));
			ds_writew((FIG_FLEE_POSITION + 4), DNG_POS_DIR(3,10,10,SOUTH));
			ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(3,9,3,NORTH));

			if (!do_fight(!ds_readb(DNG15_UNDEAD_FIGHT) ? 137 : 136))
			{
				ds_writeb(DNG15_UNDEAD_FIGHT, 2);
			}
		}

	} else if ((target_pos == DNG_POS(3,10,10) || target_pos == DNG_POS(3,13,10) || target_pos == DNG_POS(3,12,14)) &&
			target_pos != gs_dng_handled_pos)
	{
		/* QUEST: hyggelik */
		if (ds_readb(DNG15_TOOK_CURSED_MONEY) != 0)
		{
			/* you are cursed */
			do_talk(19, 1);

		} else if (!ds_readb(GOT_GRIMRING)) {

			/* fight the zombies */
			ds_writew((FIG_FLEE_POSITION + 0),
				ds_writew((FIG_FLEE_POSITION + 2),
				ds_writew((FIG_FLEE_POSITION + 4),
				ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(3,10,10,NORTH)))));

			if (!do_fight(FIGHTS_DFIN28))
			{
				/* talk with hyggelik */
				draw_main_screen();
				DNG_update_pos();
				do_talk(19, 0);
				update_mouse_cursor();
				show_hyggelik_ani();

				if (1) { } else { }

				/* group gets GRIMRING */
				do { ; } while (!get_item(ITEM_GRIMRING, 1, 1));

				ds_writeb(GOT_GRIMRING, 1);

				add_hero_ap_all(50);

				g_dng_area_loaded = g_area_prepared = -1;
				ds_writeb(PP20_INDEX, -2);

				draw_main_screen();
				refresh_screen_size();
			}
		}

	} else if (target_pos == DNG_POS(0,0,11) && target_pos != gs_dng_handled_pos)
	{
		/* EXIT: may be blocked if cursed money has been taken */
		if (ds_readb(DNG15_TOOK_CURSED_MONEY) != 0)
		{
			GUI_output(get_tx(6));

			gs_x_target = (gs_x_target_bak);
			gs_y_target = (gs_y_target_bak);
		} else {
			leave_dungeon();

			gs_current_town = (ds_readbs(TRAVEL_DESTINATION_TOWN_ID));
			gs_x_target = (ds_readws(TRAVEL_DESTINATION_X));
			gs_y_target = (ds_readws(TRAVEL_DESTINATION_Y));
			gs_current_loctype = LOCTYPE_NONE;
			gs_direction = ((ds_readbs(TRAVEL_DESTINATION_VIEWDIR) + 2) & 0x03);

			sprintf(g_dtp2, get_tx(55), get_ttx(ds_readw(TRV_DESTINATION) + 0xeb));
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
	Bit8u *hero;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
			!hero_dead(hero) &&
			test_attrib(hero, ATTRIB_GE, -3) <= 0)
		{
			randval = random_schick(3);

			sprintf(g_dtp2,
				(char*)(randval == 3 ? get_tx(3) :
						(randval == 2 ? get_tx(7) : get_tx(8))),
				(char*)hero + HERO_NAME2);

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
				gs_direction = (WEST);
				gs_x_target--;
				DNG_inc_level();
			}
		}
	}
}

void DNG15_empty_chest(Bit8u* chest)
{
	GUI_output(get_ttx(522));
}

void DNG15_rotten_clothes_chest(Bit8u* chest)
{
	GUI_output(get_tx(34));
}

void DNG15_smelling_chest(Bit8u* chest)
{
	Bit8u *hero;
	if (GUI_bool(get_tx(35)))
	{
		hero = get_hero(get_random_hero());

		sprintf(g_dtp2,
			get_tx(36),
			(char*)hero + HERO_NAME2);
		GUI_output(g_dtp2);

		sub_hero_le(hero, 4);
	}
}

void DNG15_figures_chest(Bit8u* chest)
{
	signed short tw_bak;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;
	GUI_output(get_tx(37));
	g_textbox_width = tw_bak;
}

void DNG15_cursed_money_chest(Bit8u* chest)
{
	Bit32s p_money;
	signed short tw_bak;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	if (GUI_bool(get_tx(54)))
	{
		/* You are cursed ... */
		ds_writeb(DNG15_TOOK_CURSED_MONEY, 1);

		/* ... but get much money and ... */
		p_money = get_party_money();
		p_money += 18928L;
		set_party_money(p_money);

		/* ... 50 GOLD JEWELRY. */
		get_item(ITEM_GOLD_JEWELRY, 1, 50);
	}
	g_textbox_width = tw_bak;
}

void DNG15_collapsing_ceiling(Bit8u* ptr)
{
	signed short i;
	signed short cnt;
	Bit8u *hero;

	hero = get_hero(0);

	switch (host_readb(ptr))
	{
		case 1:
		{
			/* rotting beams */
			GUI_output(get_tx(40));

			/* count failed GE-3 test */
			for (i = cnt = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero) &&
					test_attrib(hero, ATTRIB_GE, -3) <= 0)
				{
					cnt++;
				}
			}

			if (cnt >= 2)
			{
				/* if more that one hero failed, the ceiling cracks */
				inc_ptr_bs(ptr);

				GUI_output(get_tx(41));
			}
			break;
		}
		case 2:
		{
			/* the beams crash imediately */
			GUI_output(get_tx(42));
			inc_ptr_bs(ptr);
			GUI_output(get_tx(43));

			/* each hero gets 1W6 damage on a failed GE test */
			for (i = cnt = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero) &&
					test_attrib(hero, ATTRIB_GE, 0) <= 0)
				{
					sprintf(g_dtp2,
						get_tx(44),
						(char*)hero + HERO_NAME2,
						(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					GUI_output(g_dtp2);

					sub_hero_le(hero, random_schick(6));
				}
			}

			/* way is blocked */
			gs_x_target = (gs_x_target_bak);
			gs_y_target = (gs_y_target_bak);
			break;
		}
		case 4:
		{
			if (random_schick(100) >= 65)
			{
				/* the way is already cleared */
				GUI_output(get_tx(48));
			} else {
				dec_ptr_bs(ptr);

				if (GUI_bool(get_tx(49)))
				{
					DNG15_clear_way(ptr);
				}
			}
				break;
		}
	}
}

void DNG15_clear_way(Bit8u* ptr)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	i = 0;

	/* With all of the following items SHOVEL, HOE, CROWBAR, FRANCESCA ...*/
	if ((get_first_hero_with_item(ITEM_SHOVEL) != -1) &&
		(get_first_hero_with_item(ITEM_PICKAXE) != -1) &&
		(get_first_hero_with_item(ITEM_CROWBAR) != -1) &&
		(get_first_hero_with_item(ITEM_FRANCESCA) != -1))
	{
		i = 1;
	}

	/* ... the group saves 30 minutes ingame time. */
	timewarp(!i ? MINUTES(30) : HOURS(1));

	/* With aprobability of 60% the ceiling crashes down. */
	if (random_schick(100) <= 60)
	{
		GUI_output(get_tx(46));

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_attrib(hero, ATTRIB_GE, 0) <= 0)
			{
				sprintf(g_dtp2,
					get_tx(44),
					(char*)hero + HERO_NAME2,
					(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(g_dtp2);

				sub_hero_le(hero, random_schick(6));
			}
		}

		gs_x_target = (gs_x_target_bak);
		gs_y_target = (gs_y_target_bak);
	} else {
		inc_ptr_bs(ptr);
		GUI_output(get_tx(47));
	}
}

void DNG15_cursed_money(Bit8u* ptr)
{
	Bit32s p_money;

	if (GUI_bool(get_tx(53)))
	{
		host_writebs(ptr, ds_writeb(DNG15_TOOK_CURSED_MONEY, 1));

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
