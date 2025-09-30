/**
 *	Rewrite of DSA1 v3.02_de functions of seg116 (travel events 8 / 10)
 *	Functions rewritten: 17/17 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg116.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg047.h"
#include "seg048.h"
#include "seg065.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
void sub_light_timers(signed short);
#endif

void tevent_130(void)
{
	signed short answer;

	if (TRV_enter_hut_question()) {

		gs_camp_incident = 0;

		gs_current_loctype = LOCTYPE_WILDCAMP;
		do_location();
		gs_current_loctype = LOCTYPE_NONE;

		TRV_load_textfile(-1);
		gs_camp_incident = -1;

		load_in_head(10);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(19), 2, get_tx2(20), get_tx2(21));

		} while (answer == -1);

		if (answer == 1) {

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(22), 3, get_tx2(23), get_tx2(24), get_tx2(25));

			} while (answer == -1);

			if (answer == 1 || answer == 2) {
				GUI_dialog_na(0, answer == 1 ? get_tx2(26) : get_tx2(27));
			} else {
				GUI_dialog_na(0, test_attrib((Bit8u*)get_first_hero_available_in_group(), ATTRIB_CH, 0) > 0 ? get_tx2(27) : get_tx2(26));
			}
		}

		gs_current_loctype = LOCTYPE_WILDCAMP;
		do_location();
		gs_current_loctype = LOCTYPE_NONE;
	}
}

/* Manrek: entrance to the dungeon "pirate cave" */
void tevent_131(void)
{
	signed short answer;

	if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_SINNESSCHAERFE, 8) > 0 && !gs_tevent131_flag) {

		gs_tevent131_flag = 1;

		GUI_output(get_tx2(28));

		do {
			answer = GUI_radio(get_tx2(29), 2, get_tx2(30), get_tx2(31));

		} while (answer == -1);

		if (answer == 1) {
			gs_travel_detour = DUNGEONS_PIRATENHOEHLE;
		}

	} else {

		if (gs_tevent131_flag) {

			do {
				answer = GUI_radio(get_tx2(32), 2, get_tx2(33),	get_tx2(34));

			} while (answer == -1);

			if (answer == 1) {
				gs_travel_detour = DUNGEONS_PIRATENHOEHLE;
			}
		}
	}
}

void tevent_132(void)
{

	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 1) > 0 && !gs_tevent132_flag) || gs_tevent132_flag)
	{

		/* set this camp place as known */
		gs_tevent132_flag = 1;

		if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 1) > 0 && !gs_tevent132_herb_flag) || gs_tevent132_herb_flag) {

			gs_tevent132_herb_flag = 1;
			g_gather_herbs_special = 61;
			TRV_found_camp_place(2);
			g_gather_herbs_special = -1;
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_133(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_tx2(35), 2, get_tx2(36), get_tx2(37));

	} while (answer == -1);

	if (answer == 1) {

		gs_trv_return = 1;

	} else {

		GUI_output(get_tx2(38));

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero))
			{
				timewarp(HOURS(1));

				if (test_attrib(hero, ATTRIB_GE, 4) <= 0) {
					/* test failed */

					timewarp(HOURS(1));

					sprintf(g_dtp2, get_tx2(39), (char*)hero + HERO_NAME2,
						GUI_get_ptr(host_readbs(hero + HERO_SEX), 3),
						GUI_get_ptr(host_readbs(hero + HERO_SEX), 2));

					GUI_output(g_dtp2);

					sub_hero_le(hero, random_schick(8));

					loose_random_item(hero, 50, get_ttx(506));
					loose_random_item(hero, 50, get_ttx(506));
					loose_random_item(hero, 50, get_ttx(506));
				}
			}
		}

		GUI_output(get_tx2(40));
	}
}

void tevent_134(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 6) > 0 && !gs_tevent134_flag) ||
		gs_tevent134_flag)
	{
		g_gather_herbs_special = 157;
		TRV_found_herb_place(0);
		g_gather_herbs_special = -1;
		gs_tevent134_flag = 1;
	}
}

void tevent_135(void)
/* Einsiedlersee <-> Einsiedlersee: Monolith */
{
	signed short tmp; /* used for (1) fall_height; (2) LE of the hero after falling */
	signed short tmp2; /* used for (1) map tile position (2) falling damage */
	signed short answer;
	signed short done;
	signed short count;
	Bit8u *hero;

	load_ani(1);
	draw_main_screen();
	init_ani(0);

	GUI_output(get_tx2(41));

	do {
		answer = GUI_radio(get_tx2(42), 2, get_tx2(43), get_tx2(44));

	} while (answer == -1);

	if (answer == 1) {

		do {
			done = 0;

			hero = get_hero(select_hero_ok_forced(get_tx2(45)));

			tmp = 1;
			if (test_skill(hero, TA_KLETTERN, -1) > 0) {
				tmp = 2;
				GUI_output(get_tx2(46));

				if (test_skill(hero, TA_KLETTERN, 1) > 0) {
					tmp = 3;
					GUI_output(get_tx2(47));

					if (test_skill(hero, TA_KLETTERN, 0) > 0) {
						tmp = 4;
						GUI_output(get_tx2(48));

						if (test_skill(hero, TA_KLETTERN, 2) > 0) {
							tmp = 5;
							GUI_output(get_tx2(49));

							if (test_skill(hero, TA_KLETTERN, 1) > 0) {

								GUI_output(get_tx2(50));

								sprintf(g_dtp2, get_tx2(54),
									(char*)hero + HERO_NAME2,
									GUI_get_ptr(host_readbs(hero + HERO_SEX), 3));
								GUI_output(g_dtp2);

								load_in_head(45);

								sprintf(g_dtp2 + 0x400, get_tx2(55),
									GUI_get_ptr(host_readbs(hero + HERO_SEX), 0),
									GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));

								GUI_dialog_na(0, (char*)((char*)(g_dtp2 + 0x400)));

								for (tmp2 = count = 0; tmp2 < 9; tmp2++) {
									if (gs_treasure_maps[tmp2] != 0) {
										count++;
									}
								}

								if (count < 5) {
									sprintf(g_dtp2 + 0x400, get_tx2(56),
										(char*)hero + HERO_NAME2, get_tx2(57));
								} else {
									sprintf(g_dtp2 + 0x400, get_tx2(56),
										(char*)hero + HERO_NAME2, get_tx(random_interval(54, 67)));
								}

								GUI_dialog_na(0, (char*)((char*)(g_dtp2 + 0x400)));

								if (!(host_readbs(hero + HERO_START_GEAR) & 0x2)) {

									or_ptr_bs(hero + HERO_START_GEAR, 2);
									inc_ptr_bs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_IN));
									inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_IN));
								}

								sprintf(g_dtp2 + 0x400, get_tx2(58),
									(char*)hero + HERO_NAME2, GUI_get_ptr(host_readbs(hero + HERO_SEX), 2));
								GUI_dialog_na(0, (char*)(g_dtp2 + 0x400));

								tmp = 0;
								done = 1;
							}
						}
					}
				}
			}

			if (tmp) {

				sprintf(g_dtp2, get_tx2(51), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);

				/* depending on fall height stored in tmp: damage in the interval [1..5], [4..13], [7..21], [10..32], [15..40] */
				tmp2 = random_interval(g_tevent135_climb_damage[tmp - 1].lo, g_tevent135_climb_damage[tmp - 1].hi);

				tmp = host_readws(hero + HERO_LE);
				tmp -= tmp2;

				sub_hero_le(hero, tmp2);

				if (tmp <= 0) {
					sprintf(g_dtp2, (char*)(!tmp ? get_tx2(53) : get_tx2(52)), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);
				}

				if (count_heroes_available_in_group()) { /* potential Original-Bug: Does this make sense if there is only the NPC left? (Can it happen?) */

					if (!GUI_bool(get_tx2(59))) {
						done = 1;
					}
				} else {
					done = 1;
				}
			}

		} while (done == 0);
	}
	/* potential Original-Bug: Can it happen that only the NPC survives? What then? */

	set_var_to_zero();
	g_request_refresh = 1;
}

void tevent_137(void)
{
	signed short i;
	signed short answer;
	signed short item_pos;
	Bit8u *hero;

	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 5) > 0 && !gs_tevent137_flag) ||
		gs_tevent137_flag)
	{
		gs_tevent137_flag = 1;

		do {
			answer = GUI_radio(get_tx2(60), 2, get_tx2(61),	get_tx2(62));

		} while (answer == -1);

		if (answer == 1) {

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
					!hero_dead(hero))
				{
					/* each hero gets five FOODPACKAGES */
					give_hero_new_item(hero, ITEM_FOOD_PACKAGE, 1, 5);

					/* each hero gets his first WATERSKIN filled */
					/* potential Original-Bug: Does it make sense that the further WATERSKINs are not filled? */

					if ((item_pos = get_item_pos(hero, ITEM_WATERSKIN)) != -1)
					{
						/* fill waterskin */
#if !defined(__BORLANDC__)
						and_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * item_pos, 0xfb); /* unset 'empty' flag */
						and_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * item_pos, 0xfd); /* unset 'half_empty' flag */
#else
						(*(struct inventory_flags*)(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * item_pos)).half_empty =
							(*(struct inventory_flags*)(hero + HERO_INVENTORY + INVENTORY_FLAGS + SIZEOF_INVENTORY * item_pos)).empty = 0;
#endif
					}

					host_writebs(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));

					add_hero_le((struct struct_hero*)hero, 2);
				}
			}
		}
	}
}

void tevent_138(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent138_flag) || gs_tevent138_flag)
	{
		gs_tevent138_flag = 1;
		TRV_found_camp_place(1);
	}
}

void tevent_139(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_tx2(63), 2, get_tx2(64), get_tx2(65));

	} while (answer == -1);

	if (answer == 1) {

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
				!hero_dead(hero))
			{
				sub_hero_le(hero, random_schick(2));
			}
		}

		timewarp(HOURS(6));

		GUI_output((i = test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 3)) > 0 ? get_tx2(66) : get_tx2(68));
	} else {
		timewarp(HOURS(2));

		GUI_output((i = test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 5)) > 0 ? get_tx2(67) : get_tx2(69));
	}

	if (i <= 0) {
		if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 3) > 0) {

			timewarp(HOURS(2));

			GUI_output(get_tx2(70));

		} else {

			timewarp(HOURS(3));

			GUI_output(get_tx2(71));

			gs_current_loctype = LOCTYPE_WILDCAMP;
			do_location();
			gs_current_loctype = LOCTYPE_NONE;

		}
	}
}

void tevent_140(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent140_flag) || gs_tevent140_flag) {

		/* set this camp place as known */
		gs_tevent140_flag = 1;

		if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 4) > 0 && !gs_tevent140_herb_flag) || gs_tevent140_herb_flag) {

			gs_tevent140_herb_flag = 1;
			g_gather_herbs_special = -126;
			TRV_found_camp_place(2);
			g_gather_herbs_special = -1;
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_141(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 0) > 0 && !gs_tevent141_flag) || gs_tevent141_flag)
	{
		/* set this camp place as known */
		gs_tevent141_flag = 1;

		if (!TRV_follow_trail_question()) {
			TRV_hunt_generic(21, 73, -1, 5, 10, 15, 5, 5, 10, 50, 0);
		}
	}
}

void tevent_142(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent142_flag) ||
		gs_tevent142_flag)
	{
		/* set this camp place as known */
		gs_tevent142_flag = 1;
		TRV_found_camp_place(1);
	}
}

void tevent_143(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	do {
		answer = GUI_radio(get_tx2(0), 2, get_tx2(1), get_tx2(2));

	} while (answer == -1);

	if (answer == 1) {

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readbs(hero + HERO_GROUP_NO) == gs_current_group))
			{
				sub_hero_le(hero, random_schick(2) + 1);
			}
		}

		timewarp(HOURS(4));

		GUI_output((i = test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 2)) > 0 ? get_tx2(3) : get_tx2(5));
	} else {
		timewarp(HOURS(1));

		GUI_output((i = test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 4)) > 0 ? get_tx2(4) : get_tx2(6));
	}

	if (i <= 0) {

		if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 3) > 0) {

			timewarp(HOURS(1));

			GUI_output(get_tx2(7));

		} else {

			timewarp(HOURS(3));

			GUI_output(get_tx2(8));

			gs_current_loctype = LOCTYPE_WILDCAMP;
			do_location();
			gs_current_loctype = LOCTYPE_NONE;
		}
	}
}

/* Orcs' military camp */
void tevent_144(void)
{
	signed short grimring_hero_pos;
	signed short right_time_flag;
	Bit8u *hero;

	right_time_flag = 0;

	if ((grimring_hero_pos = get_first_hero_with_item(ITEM_GRIMRING)) != -1) {

		if ((gs_year == 17) && (gs_month == 10) && (gs_day_of_month >= 10)) {
			right_time_flag = 1;
		} else {
			if (gs_orcdocument_read_flag && GUI_bool(get_tx2(46))) {
			/* the heroes are too early and they know about it since they have read the orc document.
			 * they decided to wait...forward time  */

				GUI_output(get_tx2(47));

				/* set date */
				gs_year = 17;
				gs_month = 10;
				gs_day_of_month = 10;

				/* seems like the timers are simply forwarded by an arbitrary time span. */
				sub_ingame_timers(MONTHS(1));
				sub_mod_timers(MONTHS(1));
				sub_heal_staffspell_timers(MONTHS(1)/MINUTES(5)); /* note that the argument is given in units of 5 minutes */
				sub_light_timers(HOURS(25)/MINUTES(15)); /* note that the argument is given in units of 15 minutes */
				right_time_flag = 1;
			}
		}

		if (right_time_flag) {

			load_in_head(44);
			memmove(g_buffer10_ptr, g_dtp2, 0x400);

			hero = get_hero(grimring_hero_pos);

			sprintf(g_dtp2, get_tx2(10), (char*)hero + HERO_NAME2);

			GUI_output(g_dtp2);

			sprintf(g_dtp2, get_tx2(11), (char*)hero + HERO_NAME2);

			GUI_dialogbox(g_buffer10_ptr, NULL, g_dtp2, 0);
			GUI_dialogbox(g_buffer10_ptr, NULL, get_tx2(12), 0);
			GUI_dialogbox(g_buffer10_ptr, NULL, get_tx2(13), 0);

			sprintf(g_dtp2, get_tx2(14), (char*)hero + HERO_NAME2);

			GUI_dialogbox(g_buffer10_ptr, NULL, g_dtp2, 0);
			GUI_dialogbox(g_buffer10_ptr, NULL, get_tx2(15), 0);

			do {
				status_menu(grimring_hero_pos);

				grimring_hero_pos = get_first_hero_with_item(ITEM_GRIMRING);

				if (grimring_hero_pos == -1) {
					grimring_hero_pos = 0;
				}

				if (grimring_hero_pos == 6) {

					sprintf(g_dtp2, get_tx2(38), (char*)get_hero(6) + HERO_NAME2);

					GUI_dialogbox((unsigned char*)get_hero(6) + HERO_PORTRAIT, (char*)get_hero(6) + HERO_NAME2,
							g_dtp2, 0);
				}

			} while (grimring_hero_pos == 6);

			gs_main_acting_hero = get_hero(grimring_hero_pos);

			final_intro();
			if (!TRV_fight_event(FIGHTS_F144, 144)) {
				/* you won the final fight */
				GUI_output(get_tx2(17));

				g_game_state = (GAME_STATE_VICTORY);

				add_hero_ap_all(500);

				gs_current_loctype = LOCTYPE_NONE;
				gs_npc_months = 200;
			} else {
				/* you lost the final fight */
				GUI_output(get_tx2(18));
				g_game_state = (GAME_STATE_DEAD);
			}

#if !defined(__BORLANDC__)
			gs_main_acting_hero = NULL;
#endif
		}
	}
}

/* TODO: this function is unused */
void tevent_144_unused(void)
{
	GUI_output(get_tx2(16));
}

void TLK_old_woman(signed short state)
{
	signed short l_di;
	signed short counter;
	Bit8u *hero;

	if (state == 3) {

		hero = get_hero(0);
		for (l_di = counter = 0; l_di <= 6; l_di++, hero += SIZEOF_HERO) {
			/* Original-Bug: check if this is realy a hero in the current group and alive before test_skill() */
			if (test_skill(hero, TA_VERSTECKEN, -5) > 0) {
				counter++;
			}
		}

		g_dialog_next_state = (count_heroes_in_group() == counter ? 4 : 5);

	} else if (state == 4 || state == 14 || state == 21) {

		timewarp(MINUTES(15));

	} else if (state == 6 || state == 30) {

		timewarp(MINUTES(30));

	} else if (state == 23) {

		hero = get_hero(0);
		for (l_di = counter = 0; l_di <= 6; l_di++, hero += SIZEOF_HERO) {
			/* Original-Bug: check if this is realy a hero in the current group and alive before test_skill() */
			if (test_skill(hero, TA_VERSTECKEN, -5) > 0) {
				counter++;
			}
		}
#if defined(__BORLANDC__)
		gs_random_tlk_hero = get_hero(get_random_hero());
#endif

		g_dialog_next_state = (count_heroes_in_group() == counter ? 24 : 25);

	} else if (state == 33) {

		/* Infinity-Loop */
		do {
		} while (1);

	} else if (state == 34) {
		g_dialog_next_state = (gs_current_town == TOWNS_FELSTEYN ? 35 : 39);
	} else if (state == 37) {
		gs_tevent093_flag = (gs_tevent088_flag = (1));
	} else if (state == 38) {
		timewarp(HOURS(1));
	} else if (state == 41) {
		gs_tevent093_flag = (gs_tevent091_flag = ((gs_informer_flags[INFORMER_ISLEIF] = gs_isleif_jandas_regards = 1)));
	}
}

void tevent_136(void)
{
	if (gs_met_unicorn_flag && gs_got_main_quest && gs_unicorn_get_map && !gs_unicorn_timer) {

		do_talk(12, 1);
		gs_met_unicorn_flag = 1;
		gs_unicorn_get_map = 0;
	}
}

#if !defined(__BORLANDC__)
}
#endif
