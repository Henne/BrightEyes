/**
 *	Rewrite of DSA1 v3.02_de functions of seg077 (dungeon: the ship of death)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg077.cpp
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
#include "seg095.h"
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

/**
 * \brief   dungeon handler of the death ship
 */
signed short DNG01_handler(void)
{
	signed short target_pos;
	signed short i;
	Bit8u *hero;


	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	if (target_pos == DNG_POS(0,1,8))
	{
		DNG_fallpit_test(6);

	} else if (target_pos == DNG_POS(1,3,9))
	{
		gs_x_target--;
		DNG_fallpit_test(6);

	} else if (target_pos == DNG_POS(1,14,5))
	{
		gs_y_target++;
		DNG_fallpit_test(6);

	} else if (target_pos == DNG_POS(2,7,6) || target_pos == DNG_POS(2,7,9))
	{
		DNG_stub6();

	} else if (target_pos == 0x3e07)
	{
		DNG_fallpit_test(6);

	} else if (target_pos == DNG_POS(4,7,6))
	{
		DNG_fallpit_test(6);
		gs_y_target++;

	} else if ((target_pos == DNG_POS(1,13,5)) && (target_pos != gs_dng_handled_pos) && !gs_dng01_sabre_taken)
	{
		sprintf(g_text_output_buf, get_ttx(528), GUI_names_grammar(0, 3, 0), (char*)GUI_2f2(2, 3, 0));

		/* ITEM: get a SABRE */
		if (GUI_bool(g_text_output_buf) && get_item(ITEM_SABER, 1, 1)) {
			gs_dng01_sabre_taken = 1;
		}

	} else if ((target_pos == DNG_POS(3,2,9)) && (target_pos != gs_dng_handled_pos) && !gs_dng01_crossbow_taken)
	{
		sprintf(g_text_output_buf, get_ttx(528), GUI_names_grammar(0, 12, 0), (char*)GUI_2f2(2, 12, 0));

		/* ITEM: get a CROSSBOW */
		if (GUI_bool(g_text_output_buf) && get_item(ITEM_CROSSBOW, 1, 1)) {

			gs_dng01_crossbow_taken = 1;
		}

	} else if ((target_pos == DNG_POS(4,2,9)) && (target_pos != gs_dng_handled_pos) && !gs_dng01_amulet_taken)
	{
		/* ITEM: a magic AMULET */
		if (GUI_bool(get_tx(7)) && get_item(ITEM_AMULET_GREEN, 1, 1))
		{
			gs_dng01_amulet_taken = 1;
			gs_gods_estimation[GOD_BORON] -= 100L;
		}

	} else if ((target_pos == DNG_POS(4,13,6)) && (target_pos != gs_dng_handled_pos))
	{
		seg092_06b4(0);

	} else if (ds_readbs((VISUAL_FIELD_VALS + 1)) == 2)
	{
		/* TRAP: heroes may loose 2 LE with probability 10% */
		if (mod_day_timer(MINUTES(10)))
		{
			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
			{
				if (random_schick(100) <= 10 &&
					host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group)
				{
					sub_hero_le(hero, 2);

					sprintf(g_text_output_buf, get_tx(12), (char*)hero + HERO_NAME2);

					GUI_output(g_text_output_buf);
				}
			}
		}

	} else if ((target_pos == DNG_POS(5,1,8)) && !gs_dng01_ardora_freed)
	{
		/* NPC: ARDORA */
		g_textbox_width = 7;

		load_in_head(21);

		if (GUI_dialogbox((unsigned char*)g_dtp2, get_ttx(756), get_tx(8), 2, get_ttx(2), get_ttx(3)) == 1)
		{
			g_textbox_width = 3;

			GUI_dialogbox((unsigned char*)g_dtp2, get_ttx(756), get_tx(9), 0);

			if (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE)
			{
				remove_npc(host_readbs(get_hero(6) + HERO_NPC_ID) + 19,	31,
						host_readbs(get_hero(6) + HERO_NPC_ID) + ARCHIVE_FILE_NSC_LTX,
						NULL, NULL);
			}

			add_npc(ARCHIVE_FILE_NSC_LTX + 4);
		} else {
			g_textbox_width = 3;

			GUI_dialogbox((unsigned char*)g_dtp2, get_ttx(756), get_tx(10), 0);
		}

		gs_dng01_ardora_freed = 1;

		add_hero_ap_all(20);

	} else if ((target_pos == DNG_POS(5,14,7)) && (target_pos != gs_dng_handled_pos))
	{
		if (GUI_bool(get_tx(11)))
		{
			/* check if a ROPE LADDER or a ROPE is available */
			/* Original-Bug: Why not check for a mage with staffspell level >= 3? */
			if (get_first_hero_with_item(ITEM_ROPE) != -1 || get_first_hero_with_item(ITEM_ROPE_LADDER) != -1)
			{
				/* Original-Bug: better get_first_hero_available_in_group() */
				if (test_skill(get_hero(0), TA_KLETTERN, 0) > 0)
				{
					/* Original-Bug: '32 * ' should probably be '16 *'. */
					g_dng_map[32 * gs_y_target + gs_x_target] = DNG_TILE_PIT_IN_CEILING << 4;

					DNG_dec_level();

					/* Original-Bug: '32 * ' should probably be '16 *'. */
					g_dng_map[32 * gs_y_target + gs_x_target] = DNG_TILE_PIT << 4;

					gs_y_target++;
				}
			} else {
				GUI_output(get_ttx(529));
			}
		}

	} else if ((target_pos == DNG_POS(2,8,11)) && (target_pos != gs_dng_handled_pos))
	{
		if (GUI_bool(get_tx(24)))
		{
			leave_dungeon();
			gs_current_town = ((signed char)gs_travel_destination_town_id);
			gs_x_target = gs_travel_destination_x;
			gs_y_target = gs_travel_destination_y;
			gs_current_loctype = LOCTYPE_NONE;
			gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

			GUI_output(get_tx(15));

			if
#ifndef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 30:
			 * After killing the demon on the death ship, it begins to sink. The party has 30 remaining steps to leave the ship. Succeeding in this, the corresponding message of the sinking death ship (present in the data) is not displayed.
			 * Apparently, the bug is not present in the floppy version. However, there was the bug that an activated dungeon death trap will not be deactivated when leaving the dungeon. Fixing this bug in the CD-version,  Attic has introduced this new (and much less critical) bug.
			 * reported 2014-04-15 by Alrik at https://www.crystals-dsa-foren.de/showthread.php?tid=4589&pid=131934#pid131934 and https://www.crystals-dsa-foren.de/showthread.php?tid=4589&pid=131938#pid131938
			 * fixed 2016-03-06 by NRS https://www.crystals-dsa-foren.de/showthread.php?tid=4589&pid=145465#pid145465

			 * The problem is that 'gs_deathtrap' has been resetted to 0 in the function leave_dungeon() called above. */

				(gs_deathtrap == 1 && gs_deathtrap_steps != 0)
#else
				(gs_quest_deadship_done == 1)
#endif
			{
				load_ani(18);
				init_ani(1);
				set_var_to_zero();
				GUI_output(get_tx(16));
			} else {
				GUI_output(get_tx(17));
			}

			g_special_screen = 1;
			timewarp(HOURS(6));
			g_fading_state = 3;
		}
	}

	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG01_chest1_x1(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng01_chest1_content;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG01_chest2_x1(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng01_chest2_content;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG01_chest3_x1(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng01_chest3_content;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG01_chest5_x1(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng01_chest5_content;

	loot_simple_chest((struct struct_chest*)chest);

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG01_chest7_open(struct struct_chest* chest)
{
	loot_corpse(chest, get_tx(4), &gs_dng01_corpse_looted);
}

void DNG01_chest7_x2(Bit8u* chest)
{
	Bit8u* ptr_bak = ((struct struct_chest*)chest)->content;

	((struct struct_chest*)chest)->content = gs_dng01_chest7_content;

	loot_chest((struct struct_chest*)chest, get_tx(5), get_tx(6));

	((struct struct_chest*)chest)->content = ptr_bak;
}

void DNG01_chest6_x3(Bit8u* chest)
{
#ifdef M302de_ORIGINAL_BUGFIX
	/* Enabling the extra loot in function DNG01_chest6_x1() also causes the special chest handler to call this function instead of DNG01_chest6_x1().
	   Without this additional check the player will always get another golden key and the same text as the first time the chest is successfully opened.*/
	if (!gs_dng01_key_taken)
	{
#endif
	/* ITEM: the GOLDEN KEY */
	get_item(ITEM_KEY_GOLDEN_2, 1, 1);

	/* Original-Bug: The string 14 from SHIP.DTX needs a pointer to the name of the hero, not an integer.
	 */
#ifdef M302de_ORIGINAL_BUGFIX
	sprintf(g_text_output_buf, get_tx(14), (char*)get_first_hero_available_in_group());
#else
	sprintf(g_text_output_buf, get_tx(14), 10);
#endif
	print_msg_with_first_hero(g_text_output_buf);

	gs_dng01_key_taken = 1;

#ifdef M302de_ORIGINAL_BUGFIX
	/* We also need to close the if-block...*/
	}
	else 
	{
		/* ... and tell the player that the chest is now empty.*/
		GUI_output(get_ttx(522));
	}
#endif
}

void DNG01_chest6_x2(void)
{
	GUI_output(get_tx(3));
}

void DNG01_chest6_open(struct struct_chest* chest)
{
	if (!gs_dng01_key_taken)
	{
		GUI_input(get_tx(2), 10);

		/* compare if the user wrote MARBO */
		if (!strcmp(g_text_input_buf, g_dng01_str_marbo))
		{
			// correct answer
			chest->loot((Bit8u*)chest);

			// Original-Bug: enable the extra loot, here 10 AP
#ifdef M302de_ORIGINAL_BUGFIX
			g_get_extra_loot = 1;
#endif
		} else if (chest->trap){

			// wrong answer
			chest->trap();
		}

	} else {
		GUI_output(get_ttx(522));
	}
}

void DNG01_chest0_x1(Bit8u* chest)
{
	if (!gs_deadship_final && GUI_bool(get_tx(1)))
	{
		gs_deadship_final = 1;

		/* enable deathtrap. the ship begins to sink... */
		gs_deathtrap_steps = 30;
		gs_deathtrap = 1;

		/* fight the demon */
		g_fig_flee_position[NORTH] = g_fig_flee_position[EAST] = g_fig_flee_position[SOUTH] = g_fig_flee_position[WEST] = DNG_POS_DIR(1,13,10,WEST);

		if (!do_fight(FIGHTS_SHIP10_1))
		{
			add_hero_ap_all(50);
		}

		/* this quest is done */
		gs_quest_deadship_done = 1;
	}
}

#if !defined(__BORLANDC__)
}
#endif
