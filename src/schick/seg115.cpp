/**
 *	Rewrite of DSA1 v3.02_de functions of seg115 (travel events 7 / 10)
 *	Functions rewritten: 13/13 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg115.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg115.h"

/* the raft */
void tevent_016(void)
{
	signed int answer;
	struct struct_hero *hero;

	if (!gs_tevent016_flag)
	{

		load_in_head(46);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(19), 2, get_tx2(20), get_tx2(21));

		} while (answer == -1);

		if (answer == 1)
		{
			/* ignore */
			GUI_dialog_na(0, get_tx2(22));

		} else {
			/* wave */

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(23), 2, get_tx2(24), get_tx2(25));

			} while (answer == -1);

			if (answer == 1)
			{
				/* run away */
				GUI_dialog_na(0, get_tx2(26));
			} else {
				/* go towards them */

				do {
					answer = GUI_dialogbox((unsigned char*)g_dtp2,
							NULL, get_tx2(27), 2,
							get_tx2(28), get_tx2(29));

				} while (answer == -1);

				if (answer == 2)
				{
					/* walk away */
					GUI_dialog_na(0, get_tx2(30));
				} else {
					/* help them */

					GUI_dialog_na(0, get_tx2(31));

					hero = get_hero(get_hero_KK_best());

					/* test KK+3 */
					if (test_attrib(hero, ATTRIB_KK, 3) > 0)
					{
						/* success */
						GUI_dialog_na(0, get_tx2(32));
						GUI_dialog_na(0, get_tx2(36));

						add_hero_ap_all(5);
					} else {
						/* fail */
						GUI_dialog_na(0, get_tx2(33));

						hero = get_hero(get_random_hero());

						/* GE+0 */
						if (test_attrib(hero, ATTRIB_GE, 0) > 0)
						{
							/* success */
							timewarp(MINUTES(15));

							sprintf(g_dtp2 + 0x400, get_tx2(35), hero->alias,
								GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_1ST));

							GUI_dialog_na(0, (char*)(g_dtp2 + 0x400));

							GUI_dialog_na(0, get_tx2(36));
						} else {
							/* fail */
							sprintf(g_dtp2 + 0x400,	get_tx2(34),
								hero->alias,
								GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_1ST),
								GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_4TH),
								GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_3RD),
								GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_4TH));

							GUI_dialog_na(0, (char*)g_dtp2 + 0x400);

							timewarp(HOURS(1));

							sprintf(g_dtp2 + 0x400,	get_tx2(37), hero->alias,
								GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_2ND));

							GUI_dialog_na(0, (char*)g_dtp2 + 0x400);

							add_hero_ap(hero, 5);

							loose_random_item(hero, 10, get_ttx(506));

							hero_disease_test(hero, DISEASE_ID_DUMPFSCHAEDEL, 20 - (hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod));
						}
					}
				}
			}
		}

		gs_tevent016_flag = 1;
	}
}

/* falling rocks */
void tevent_090(void)
{
	signed int i;
	struct struct_hero *hero;

	GUI_output(get_tx2(0));
	GUI_output(get_tx2(1));

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++)
	{
		if ((hero->typus) && (hero->group_id == gs_active_group_id) && !hero->flags.dead &&
			test_talent(hero, TA_GEFAHRENSINN, 0) <= 0)
		{
			/* failed */
			sub_hero_le(hero, random_schick(10));

			loose_random_item(hero, 10, get_ttx(506));
			loose_random_item(hero, 10, get_ttx(506));
		}
	}

	GUI_output(get_tx2(2));
}

void tevent_091(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 5) > 0 && !gs_tevent091_flag) ||	gs_tevent091_flag)
	{
		g_gather_herbs_special = ITEM_SHURINKNOLLE;

		TRV_found_herb_place(0);

		g_gather_herbs_special = -1;

		gs_tevent091_flag = 1;
	}
}

void tevent_093(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 4) > 0 && !gs_tevent093_flag) || gs_tevent093_flag)
	{
		TRV_found_camp_place(0);

		gs_tevent093_flag = 1;
	}
}

/* Rybon <-> Thoss: entrance to the Daspota treasure dungeon */
void tevent_094(void)
{
	if (gs_tevent094_flag != 0)
	{
		signed int answer;

		GUI_output(get_tx2(3));

		load_in_head(53);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2,
				       NULL, get_tx2(4), 2,
				       get_tx2(5), get_tx2(6));

		} while (answer == -1);

		if (answer == 1) {
			/* enter Daspota dungeon */
			gs_travel_detour = DUNGEON_ID_DASPOTASCHATZ;
		}
	}
}

/* Arete */
void tevent_095(void)
{
	signed int counter_failed;
	signed int done = 0;
	signed int i;
	signed int counter_heroes;
	struct struct_hero *hero;
	signed int ret_spell_test;

	do {

		GUI_output(get_tx2(7));

		hero = get_hero(0);

		for (i = counter_failed = counter_heroes = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) &&
				(hero->group_id == gs_active_group_id) &&
				!hero->flags.dead)
			{
				counter_heroes++;

				if (test_attrib(hero, ATTRIB_HA, -1) > 0)
				{
					timewarp(MINUTES(30));

					sprintf(g_dtp2,	get_tx2(8), hero->alias);
					GUI_output(g_dtp2);

					counter_failed++;
				}
			}
		}

		if (!counter_failed)
		{
			/* no hero failed HA-test */
			GUI_output(get_tx2(16));
			done = 1;

		} else if (counter_failed == counter_heroes)
		{
			/* all heroes failed HA-test */
			do {
				counter_failed = GUI_radio(get_tx2(9), 2, get_tx2(10), get_tx2(11));

			} while (counter_failed == -1);

			if (counter_failed == 2)
			{
				/* make a rest */
				gs_town_loc_type = LOCTYPE_WILDCAMP;
				do_location();
				gs_town_loc_type = LOCTYPE_NONE;

				TRV_load_textfile(-1);
			} else {

#ifndef M302de_ORIGINAL_BUGFIX
				/* Original-Bug 53:
				 * If the party was already traveling backward, the new direction should be forward. */
				gs_journey_direction = done = JOURNEY_DIRECTION_CHANGE_TO_BACKWARD; // this is value 1
#else
				gs_journey_direction = (gs_journey_direction == JOURNEY_DIRECTION_FORWARD ? JOURNEY_DIRECTION_CHANGE_TO_BACKWARD : JOURNEY_DIRECTION_CHANGE_TO_FORWARD);
				done = 1;
#endif
			}
		} else {
			/* at least one hero failed HA-test */
			counter_heroes = 0;

			do {
				do {
					counter_failed = GUI_radio((counter_heroes == 0 ? get_tx2(12) : get_tx2(87)), 3,
									get_tx2(13), get_tx2(14), get_tx2(15));

				} while (counter_failed == -1);

				if (counter_failed == 1)
				{
					/* "on hands and knees" */

					timewarp(HOURS(2));

					GUI_output(get_tx2(16));

					done = 1;

				} else if (counter_failed == 2)
				{
					/* try a spell */
					hero = get_hero(select_hero_ok_forced(get_ttx(317)));

					if (hero->typus < HERO_TYPE_HEXE)
					{
						/* this hero is no magic-user */
						GUI_output(get_ttx(330));
					} else {

						ret_spell_test = test_spell(hero, SP_BANNBALADIN, 0);

						if (ret_spell_test > 0)
						{
							/* spell succeeded */

							/* TODO: magicians with 4th staff spell may pay less */
							sub_ae_splash(hero, get_spell_cost(SP_BANNBALADIN, 0));

							GUI_output(get_tx2(16));

							done = 1;

						} else if (ret_spell_test != -99) {

							/* spell failed */

							/* hero pays the half spell costs */
							sub_ae_splash(hero, get_spell_cost(SP_BANNBALADIN, 1));

							/* TODO: some output for the player */

							counter_heroes = 1;
						} else {
							/* spell failed unluckily */

							/* TODO: this gets output, but no spell costst ??? */
							sprintf(g_dtp2,	get_ttx(607), hero->alias);
							GUI_output(g_dtp2);
						}

						timewarp(MINUTES(30));
					}
				} else {
					/* talk to the heroes */

					/* wait for 4 hours */
					timewarp(HOURS(4));

					GUI_output(get_tx2(16));

					done = 1;
				}

			} while (!done);
		}

	} while (!done);
}

void tevent_096(void)
{
	signed int answer;

	do {
		answer = GUI_radio(get_tx2(17), 2, get_tx2(18),	get_tx2(19));

	} while (answer == -1);

	if (answer == 1)
	{
		/* try to keep on track */

		if (test_talent(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 2) > 0)
		{
			timewarp(HOURS(3));

			GUI_output(get_tx2(20));
		} else {
			timewarp(HOURS(4));

			GUI_output(get_tx2(22));

			answer = -1;
		}
	} else {
		/* try to go arround */

		if (test_talent(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 4) > 0)
		{
			timewarp(HOURS(4));

			GUI_output(get_tx2(21));
		} else {
			timewarp(HOURS(5));

			GUI_output(get_tx2(23));

			answer = -1;
		}
	}

	if (answer == -1)
	{
		/* lost the way */

		if (test_talent(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 3) > 0)
		{
			/* find the way again */
			timewarp(HOURS(3));

			GUI_output(get_tx2(24));
		} else {
			/* lost the way completely */
			timewarp(HOURS(4));

			GUI_output(get_tx2(25));

			gs_town_loc_type = LOCTYPE_WILDCAMP;
			do_location();
			gs_town_loc_type = LOCTYPE_NONE;

			TRV_load_textfile(-1);

			GUI_output(get_tx2(26));
		}
	}
}

/* Hjalland: entrance to the dungeon "temple of the one without name" */
void tevent_126(void)
{
	signed int answer;

	do {
		answer = GUI_radio(get_tx2(0), 2, get_tx2(1), get_tx2(2));

	} while (answer == -1);

	if (answer == 1)
	{
		do {
			answer = GUI_radio(get_tx2(3), 2, get_tx2(4), get_tx2(5));

		} while (answer == -1);

		if (answer == 1)
		{
			gs_travel_detour = DUNGEON_ID_KULTSTAETTE_DES_NAMENLOSEN;
		}
	}
}

void tevent_127(void)
{
	signed int answer;

	load_in_head(14);

	do {
		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(6), 3, get_tx2(7),get_tx2(8), get_tx2(9));

	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		GUI_dialog_na(0, (answer == 1 ? get_tx2(10) : get_tx2(11)));

		timewarp(MINUTES(30));
	}
}

void tevent_128(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent128_flag) ||
		gs_tevent128_flag)
	{
		gs_tevent128_flag = 1;

		if ((test_talent(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 4) > 0 && !gs_tevent128_replen_flag) ||
			gs_tevent128_replen_flag)
		{
			gs_tevent128_replen_flag = 1;
			TRV_found_replenish_place(1);
		} else {
			TRV_found_replenish_place(0);
		}
	}
}

/* Runin: entrance to the dungeon "dragon cave" */
void tevent_129(void)
{
	signed int answer;

	if (test_talent(get_first_hero_available_in_group(), TA_SINNESSCHAERFE, 4) > 0 && !gs_tevent129_flag)
	{
		gs_tevent129_flag = 1;

		GUI_output(get_tx2(12));

		load_in_head(53);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(13), 2, get_tx2(14), get_tx2(15));

		} while (answer == -1);

		if (answer == 1) {
			gs_travel_detour = DUNGEON_ID_DRACHENHORT;
		}

	} else if (gs_tevent129_flag) {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2,
				       NULL, get_tx2(16), 2, get_tx2(17), get_tx2(18));

		} while (answer == -1);

		if (answer == 1) {
			gs_travel_detour = DUNGEON_ID_DRACHENHORT;
		}
	}
}

/* Daspota <-> Ottarje: a cutter */
void tevent_047(void)
{
	signed int answer;
	signed int l_di;

	l_di = 0;

	do {
		answer = GUI_radio(get_tx2(0), 2, get_tx2(1), get_tx2(2));

	} while (answer == -1);

	if (answer == 1)
	{
		/* ignore */
		GUI_output(get_tx2(3));

	} else {
		/* wave */
		do {
			answer = GUI_radio(get_tx2(4), 2, get_tx2(5), get_tx2(6));

		} while (answer == -1);

		if (answer == 1)
		{
			/* run away */
			do {
				answer = GUI_radio(get_tx2(7), 2, get_tx2(9), get_tx2(8));

			} while (answer == -1);

			if (answer == 1)
			{
				/* run */
				GUI_output(get_tx2(10));
			} else {
				/* wait */
				l_di = 1;
			}
		}

		if (answer == 2)
		{
			/* wait */
			load_in_head(42);

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2,
							NULL, (!l_di ? get_tx2(15) : get_tx2(11)), 2,
							get_tx2(12), get_tx2(13));

			} while (answer == -1);

			if (answer == 1)
			{
				/* deny */
				GUI_dialog_na(0, get_tx2(14));
			} else {
				/* accept */

				l_di = random_schick(2) - 1;

				if (!l_di)
				{
					do {
						answer = GUI_dialogbox((unsigned char*)g_dtp2,
								NULL, get_tx2(16), 3,
								get_tx2(17), get_tx2(18), get_tx2(19));
					} while (answer == -1);

					if (answer == 3)
					{
						/* no thanks */
						GUI_dialog_na(0, get_tx2(20));
					} else {
						/* 1 = LJASDAHL, 2 = OTTARJE */
						GUI_dialog_na(0, answer == 1 ? get_tx2(23) : get_tx2(24));
						GUI_dialog_na(0, get_tx2(25));

						if (answer == 1)
						{
							gs_town_id = TOWN_ID_LJASDAHL;
							gs_x_target = 7;
							gs_y_target = 3;
						} else {
							gs_town_id = TOWN_ID_OTTARJE;
							gs_x_target = 9;
							gs_y_target = 10;
						}

						gs_travel_detour = 99;
					}
				} else {
					do {
						answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(21), 2, get_tx2(22), get_tx2(19));

					} while (answer == -1);

					if (answer == 2)
					{
						/* deny */
						GUI_dialog_na(0, get_tx2(20));
					} else {
						/* travel to VARNHEIM */
						GUI_dialog_na(0, get_tx2(23));
						GUI_dialog_na(0, get_tx2(25));

						gs_town_id = TOWN_ID_VARNHEIM;
						gs_x_target = 4;
						gs_y_target = 10;
						gs_travel_detour = 99;
					}
				}
			}
		}
	}
}

/* Rybon <-> Thoss: entrance to the dungeon "ruin of the black wizard" */
void tevent_100(void)
{
	signed int tmp; /* multiple use: answer, mod_slot */

	if (gs_tevent100_flag) {

		do {
			tmp = GUI_radio(get_tx2(52), 3, get_tx2(53), get_tx2(54), get_tx2(55));

		} while (tmp == -1);

		if (tmp == 1) {

			GUI_output(get_tx2(56));

			g_event_ani_busy = 1;

			load_ani(11);
			draw_main_screen();
			init_ani(0);

			GUI_output(get_tx2(57));

			do {
				tmp = GUI_radio(get_tx2(58), 2, get_tx2(59), get_tx2(60));

			} while (tmp == -1);

			if (tmp == 1) {

				gs_travel_detour = DUNGEON_ID_RUINE_DES_SCHWARZMAGIERS;

			} else {

				GUI_output(get_tx2(67));

				/* FF+4 */
				if (test_attrib(get_first_hero_available_in_group(), ATTRIB_GE, 4) > 0)
				{
					/* success */
					GUI_output(get_tx2(68));

				} else {
					/* fail */
					struct struct_hero *hero = get_first_hero_available_in_group();

					tmp = get_free_mod_slot();

					set_mod_slot(tmp, DAYS(1), (uint8_t*)&hero->attrib[ATTRIB_GE].current, -2, 0);

					timewarp(MINUTES(15));

					sprintf(g_dtp2,	get_tx2(69), hero->alias, GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_1ST), GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_1ST));

					GUI_output(g_dtp2);
				}

				GUI_output(get_tx2(70));
			}

			g_request_refresh = 1;

		} else if (tmp == 3) {

#ifndef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 53:
			 * If the party was already traveling backward, the new direction should be forward. */
			gs_journey_direction = JOURNEY_DIRECTION_CHANGE_TO_BACKWARD;
#else
			gs_journey_direction = (gs_journey_direction == JOURNEY_DIRECTION_FORWARD ? JOURNEY_DIRECTION_CHANGE_TO_BACKWARD : JOURNEY_DIRECTION_CHANGE_TO_FORWARD);
#endif
		}
	}

	disable_ani();
	g_event_ani_busy = 0;
}
