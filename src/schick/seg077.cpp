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


	target_pos = DNG_POS(ds_readbs(DUNGEON_LEVEL), ds_readws(X_TARGET), ds_readws(Y_TARGET));

	if (target_pos == DNG_POS(0,1,8))
	{
		DNG_fallpit_test(6);

	} else if (target_pos == DNG_POS(1,3,9))
	{
		dec_ds_ws_post(X_TARGET);
		DNG_fallpit_test(6);

	} else if (target_pos == DNG_POS(1,14,5))
	{
		inc_ds_ws_post(Y_TARGET);
		DNG_fallpit_test(6);

	} else if (target_pos == DNG_POS(2,7,6) || target_pos == DNG_POS(2,7,9))
	{
		DNG_stub6();

	} else if (target_pos == TEVENT132_HERB_FLAG)
	{
		DNG_fallpit_test(6);

	} else if (target_pos == DNG_POS(4,7,6))
	{
		DNG_fallpit_test(6);
		inc_ds_ws_post(Y_TARGET);

	} else if (target_pos == DNG_POS(1,13,5) && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readbs(DNG01_SABRE_TAKEN))
	{
		sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
			get_ttx(528),
			(char*)(Bit8u*)(GUI_names_grammar(0, 3, 0)),
			(char*)(Bit8u*)(GUI_2f2(2, 3, 0)));

		/* ITEM: get a SABRE */
		if (GUI_bool((char*)ds_readd(TEXT_OUTPUT_BUF)) && get_item(ITEM_SABER, 1, 1)) {
			ds_writeb(DNG01_SABRE_TAKEN, 1);
		}

	} else if (target_pos == DNG_POS(3,2,9) && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readbs(DNG01_CROSSBOW_TAKEN))
	{
		sprintf((char*)ds_readd(TEXT_OUTPUT_BUF), get_ttx(528),	(char*)(Bit8u*)(GUI_names_grammar(0, 12, 0)), (char*)(Bit8u*)(GUI_2f2(2, 12, 0)));

		/* ITEM: get a CROSSBOW */
		if (GUI_bool((char*)ds_readd(TEXT_OUTPUT_BUF)) && get_item(ITEM_CROSSBOW, 1, 1)) {

			ds_writeb(DNG01_CROSSBOW_TAKEN, 1);
		}

	} else if (target_pos == DNG_POS(4,2,9) && target_pos != ds_readws(DNG_HANDLED_POS) && !ds_readbs(DNG01_AMULET_TAKEN))
	{
		/* ITEM: a magic AMULET */
		if (GUI_bool(get_tx(7)) && get_item(ITEM_AMULET_GREEN, 1, 1))
		{
			ds_writeb(DNG01_AMULET_TAKEN, 1);
			sub_ds_ds(GODS_ESTIMATION + 4 * GOD_BORON, 100L);
		}

	} else if (target_pos == DNG_POS(4,13,6) && target_pos != ds_readws(DNG_HANDLED_POS))
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
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					sub_hero_le(hero, 2);

					sprintf((char*)ds_readd(TEXT_OUTPUT_BUF), get_tx(12), (char*)hero + HERO_NAME2);

					GUI_output((char*)ds_readd(TEXT_OUTPUT_BUF));
				}
			}
		}

	} else if (target_pos == DNG_POS(5,1,8) && !ds_readbs(DNG01_ARDORA_FREED))
	{
		/* NPC: ARDORA */
		ds_writew(TEXTBOX_WIDTH, 7);

		load_in_head(21);

		if (GUI_dialogbox((unsigned char*)ds_readd(DTP2), get_ttx(756), get_tx(8), 2, get_ttx(2), get_ttx(3)) == 1)
		{
			ds_writew(TEXTBOX_WIDTH, 3);

			GUI_dialogbox((unsigned char*)ds_readd(DTP2), get_ttx(756), get_tx(9), 0);

			if (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE)
			{
				remove_npc(host_readbs(get_hero(6) + HERO_NPC_ID) + 19,
						31,
						host_readbs(get_hero(6) + HERO_NPC_ID) + ARCHIVE_FILE_NSC_LTX,
						NULL, NULL);
			}

			add_npc(ARCHIVE_FILE_NSC_LTX + 4);
		} else {
			ds_writew(TEXTBOX_WIDTH, 3);

			GUI_dialogbox((unsigned char*)ds_readd(DTP2), get_ttx(756), get_tx(10), 0);
		}

		ds_writeb(DNG01_ARDORA_FREED, 1);

		add_hero_ap_all(20);

	} else if (target_pos == DNG_POS(5,14,7) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		if (GUI_bool(get_tx(11)))
		{
			/* check if a ROPE LADDER or a ROPE is available */
			/* Original-Bug: Why not check for a mage with staffspell level >= 3? */
			if (get_first_hero_with_item(ITEM_ROPE) != -1 ||
				get_first_hero_with_item(ITEM_ROPE_LADDER) != -1)
			{
				/* Original-Bug: better get_first_hero_available_in_group() */
				if (test_skill(get_hero(0), TA_KLETTERN, 0) > 0)
				{
					/* Original-Bug: '32 * ' should probably be '16 *'. */
					ds_writeb(DNG_MAP + 32 * ds_readws(Y_TARGET) + ds_readws(X_TARGET), DNG_TILE_PIT_IN_CEILING << 4);
					DNG_dec_level();
					/* Original-Bug: '32 * ' should probably be '16 *'. */
					ds_writeb(DNG_MAP + 32 * ds_readws(Y_TARGET) + ds_readws(X_TARGET), DNG_TILE_PIT << 4);
					inc_ds_ws_post(Y_TARGET);
				}
			} else {
				GUI_output(get_ttx(529));
			}
		}

	} else if (target_pos == DNG_POS(2,8,11) && target_pos != ds_readws(DNG_HANDLED_POS))
	{
		if (GUI_bool(get_tx(24)))
		{
			leave_dungeon();
			ds_writebs(CURRENT_TOWN, (signed char)ds_readws(TRAVEL_DESTINATION_TOWN_ID));
			ds_writew(X_TARGET, ds_readws(TRAVEL_DESTINATION_X));
			ds_writew(Y_TARGET, ds_readws(TRAVEL_DESTINATION_Y));
			ds_writeb(CURRENT_LOCTYPE, LOCTYPE_NONE);
			ds_writeb(DIRECTION, (ds_readws(TRAVEL_DESTINATION_VIEWDIR) + 2) & 3);

			GUI_output(get_tx(15));

			if
#ifndef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 30:
			 * After killing the demon on the death ship, it begins to sink. The party has 30 remaining steps to leave the ship. Succeeding in this, the corresponding message of the sinking death ship (present in the data) is not displayed.
			 * Apparently, the bug is not present in the floppy version. However, there was the bug that an activated dungeon death trap will not be deactivated when leaving the dungeon. Fixing this bug in the CD-version,  Attic has introduced this new (and much less critical) bug.
			 * reported 2014-04-15 by Alrik at https://www.crystals-dsa-foren.de/showthread.php?tid=4589&pid=131934#pid131934 and https://www.crystals-dsa-foren.de/showthread.php?tid=4589&pid=131938#pid131938
			 * fixed 2016-03-06 by NRS https://www.crystals-dsa-foren.de/showthread.php?tid=4589&pid=145465#pid145465

			 * The problem is that 'DEATHTRAP' has been resetted to 0 in the function leave_dungeon() called above. */

				(ds_readws(DEATHTRAP) == 1 && ds_readws(DEATHTRAP_STEPS) != 0)
#else
				(ds_readws(QUEST_DEADSHIP_DONE) == 1)
#endif
			{
				load_ani(18);
				init_ani(1);
				set_var_to_zero();
				GUI_output(get_tx(16));
			} else {
				GUI_output(get_tx(17));
			}

			ds_writeb(SPECIAL_SCREEN, 1);
			timewarp(HOURS(6));
			ds_writeb(FADING_STATE, 3);
		}
	}

	ds_writew(DNG_HANDLED_POS, target_pos);

	return 0;
}

void DNG01_chest1_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG01_CHEST1_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest2_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG01_CHEST2_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest3_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG01_CHEST3_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest5_x1(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG01_CHEST5_CONTENT));
	loot_simple_chest((Bit8u*)(chest));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest7_x1(RealPt chest)
{
	loot_corpse(chest, get_tx(4), p_datseg + DNG01_CORPSE_LOOTED);
}

void DNG01_chest7_x2(RealPt chest)
{
	RealPt ptr_bak;

	ptr_bak = (RealPt)host_readd((Bit8u*)(chest) + 11);
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + DNG01_CHEST7_CONTENT));
	loot_chest((Bit8u*)(chest), get_tx(5), get_tx(6));
	host_writed((Bit8u*)(chest) + 11, (Bit32u)ptr_bak);
}

void DNG01_chest6_x3(RealPt chest)
{
#ifdef M302de_ORIGINAL_BUGFIX
	/* Enabling the extra loot in function DNG01_chest6_x1() also causes the special chest handler to call this function instead of DNG01_chest6_x1().
	   Without this additional check the player will always get another golden key and the same text as the first time the chest is successfully opened.*/
	if (!ds_readbs(DNG01_KEY_TAKEN))
	{
#endif
	/* ITEM: the GOLDEN KEY */
	get_item(ITEM_KEY_GOLDEN_2, 1, 1);

	/* Original-Bug: The string 14 from SHIP.DTX needs a pointer to the name of the hero, not an integer.
	 */
#ifdef M302de_ORIGINAL_BUGFIX
	sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
		get_tx(14),
		(char*)(Bit8u*)get_first_hero_available_in_group());
#else
	sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
		get_tx(14), 10);
#endif
	print_msg_with_first_hero((char*)ds_readd(TEXT_OUTPUT_BUF));

	ds_writeb(DNG01_KEY_TAKEN, 1);
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

void DNG01_chest6_x2(RealPt chest)
{
	GUI_output(get_tx(3));
}

void DNG01_chest6_x1(RealPt chest)
{
	if (!ds_readbs(DNG01_KEY_TAKEN))
	{
		GUI_input(get_tx(2), 10);

		/* compare if the user wrote MARBO */
		if (!strcmp((char*)ds_readd(TEXT_INPUT_BUF), (char*)p_datseg + DNG01_STR_MARBO))
		{
			// correct answer
#if defined(__BORLANDC__)
			((void (*)(RealPt))((RealPt)host_readd((Bit8u*)(chest) + 11)))(chest);
#else
			t_map(chest, 11)(chest);
#endif

			// Original-Bug: enable the extra loot, here 10 AP
#ifdef M302de_ORIGINAL_BUGFIX
			ds_writew(GET_EXTRA_LOOT, 1);
#endif
		} else if ((RealPt)host_readd((Bit8u*)(chest) + 7) != 0){
#if defined(__BORLANDC__)
			((void (*)(void))((RealPt)host_readd((Bit8u*)(chest) + 7)))();
#else
			// wrong answer
			((treasure_trap)(t_map(chest, 7)))();
#endif
		}

	} else {
		GUI_output(get_ttx(522));
	}
}

void DNG01_chest0_x1(RealPt chest)
{
	if (!ds_readb(DEADSHIP_FINAL) && GUI_bool(get_tx(1)))
	{
		ds_writeb(DEADSHIP_FINAL, 1);

		/* enable deathtrap. the ship begins to sink... */
		ds_writew(DEATHTRAP_STEPS, 30);
		ds_writew(DEATHTRAP, 1);

		/* fight the demon */
		ds_writew((FIG_FLEE_POSITION + 0), ds_writew((FIG_FLEE_POSITION + 2), ds_writew((FIG_FLEE_POSITION + 4), ds_writew((FIG_FLEE_POSITION + 6), DNG_POS_DIR(1,13,10,WEST)))));

		if (!do_fight(FIGHTS_SHIP10_1))
		{
			add_hero_ap_all(50);
		}

		/* this quest is done */
		ds_writeb(QUEST_DEADSHIP_DONE, 1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
