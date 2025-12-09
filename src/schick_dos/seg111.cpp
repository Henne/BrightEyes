/*
 *	Rewrite of DSA1 v3.02_de functions of seg111 (travel events 3 / 10)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg111.cpp
 */

#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg032.h"
#include "seg047.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg111.h"

struct struct_chest g_tevent064_chest_descr = {0, 12, use_lockpicks_on_chest, chest_poisoned3, tevent_064_chest, 0, 0, 0}; // ds:0xb154

/* Ottarje <-> Skjal: alte Feuerstelle. idealer Rastplatz */
void tevent_053(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 1) > 0 && !gs_tevent053_flag) ||
		gs_tevent053_flag != 0)
	{
		TRV_found_camp_place(0);
		gs_tevent053_flag = 1;
	}
}

/* Prem <-> Skjal: inn "Langschiff" */
void tevent_054(void)
{
	TRV_found_inn(TOWN_ID_BRENDHIL, 68);
}

/* Path ??? */
void tevent_055(void)
{
	signed int season;

	season = get_current_season();

	if (season == SEASON_SPRING || season == SEASON_AUTUMN) {

		load_in_head(52);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(0), 0);

		timewarp(HOURS(3));
	}
}

void tevent_056(void)
{
	TRV_found_inn(TOWN_ID_MANRIN, 69);
}

void tevent_057(void)
{
	signed int i;
	signed int answer;
	struct struct_hero *hero;

	do {
		answer = GUI_radio(get_tx2(1), 2, get_tx2(2), get_tx2(3));

	} while (answer == -1);

	if (answer == 2) {

		if (test_talent(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 2) > 0) {

			timewarp(HOURS(2));

			GUI_output(get_tx2(4));

		} else {

			timewarp(HOURS(6));

			GUI_output(get_tx2(5));

			hero = get_hero(0);

			for (i = 0; i <= 6; i++, hero++) {

				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id))
				{
					sub_hero_le(hero, random_schick(3));
				}
			}
		}
	} else {

		hero = get_hero(0);

		for (i = 0; i <= 6; i++, hero++) {

			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
				!hero->flags.dead && test_talent(hero, TA_KLETTERN, 3) <= 0)
			{

				/* TODO: Original-Bug: that condition does not make sense */
				if (get_first_hero_with_item(ITEM_SEIL) == -1 || get_first_hero_with_item(ITEM_STRICKLEITER) != -1)
				{

					sprintf(g_dtp2, get_tx2(8), hero->alias, GUI_get_ptr(hero->sex, 0));
					GUI_output(g_dtp2);

					sub_hero_le(hero, random_schick(9) + 3);

				} else {
					sprintf(g_dtp2, get_tx2(7), hero->alias);
					GUI_output(g_dtp2);

					sub_hero_le(hero, random_schick(8));
				}
			}
		}

		GUI_output(get_tx2(6));
	}
}

void tevent_058(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent058_flag) || gs_tevent058_flag)
	{
		TRV_found_replenish_place(0);
		gs_tevent058_flag = 1;
	}

}

void tevent_059(void)
{
	signed int answer;

	if (TRV_enter_hut_question())
	{
		gs_camp_incident = 0;
		gs_town_loc_type = LOCTYPE_WILDCAMP;
		do_location();
		gs_town_loc_type = LOCTYPE_NONE;
		gs_camp_incident = -1;

		TRV_load_textfile(-1);

		load_in_head(10);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(9), 2, get_tx2(10), get_tx2(11));

		} while (answer == -1);

		if (answer == 1) {

			if (get_first_hero_with_item(ITEM_SCHNAPSFLASCHE) != -1 || get_first_hero_with_item(ITEM_WEINFLASCHE) != -1)
			{
				do {
					answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(12),
								3, get_tx2(13), get_tx2(14), get_tx2(15));
				} while (answer == -1);

				if (answer == 1) {
					GUI_dialog_na(0, get_tx2(16));
				} else if (answer == 2) {
					GUI_dialog_na(0, get_tx2(17));
				} else if (test_attrib(get_first_hero_available_in_group(), ATTRIB_CH, 0) > 0) {
					GUI_dialog_na(0, get_tx2(17));
				} else {
					GUI_dialog_na(0, get_tx2(16));
				}

			} else {
				GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(12), 1, get_tx2(18));
				GUI_dialog_na(0, get_tx2(17));
			}
		}

		gs_town_loc_type = LOCTYPE_WILDCAMP;
		do_location();
		gs_town_loc_type = LOCTYPE_NONE;
	}
}

void tevent_060(void)
{
	signed int answer;
	signed int i;
	signed int done;
	signed int nr_items;
	signed int has_magic_rope;
	struct struct_hero *hero;

	done = 0;
	load_in_head(57);

	do {
		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(19), 2, get_tx2(20), get_tx2(21));

		} while (answer == -1);

		if (answer == 1) {

			if (test_talent(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 4) > 0) {

				sub_group_le(1);

				timewarp(HOURS(3));

				GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(22), 0);

				done = 1;
			} else {

				sub_group_le(1);

				timewarp(HOURS(4));

				do {
					answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(24), 2, get_tx2(26), get_tx2(27));

				} while (answer == -1);

				if (answer == 1) {

					timewarp(HOURS(1));

					hero = get_hero(0);

					for (i = 0; i <= 6; i++, hero++) {

						if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
							!hero->flags.dead && test_talent(hero, TA_KLETTERN, 0) <= 0)
						{
							sub_hero_le(hero, random_schick(10));
						}
					}

					GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(29), 0);

					done = 1;
				} else {
					timewarp(HOURS(2));
				}
			}

		} else {


			if (test_talent(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 2) > 0) {

				timewarp(HOURS(3));

				GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(23), 0);

				done = 1;
			} else {

				sub_group_le(1);

				timewarp(HOURS(4));

				do {
					answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(25), 2, get_tx2(27), get_tx2(28));
				} while (answer == -1);

				if (answer == 2) {

					hero = get_hero(0);

					for (i = has_magic_rope = nr_items = 0; i <= 6 ; i++, hero++){

						if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
							!hero->flags.dead)
						{
							nr_items += hero_count_item(hero, ITEM_SEIL);
							nr_items += hero_count_item(hero, ITEM_STRICKLEITER);

							if (hero->staff_level >= 3)
							{
								has_magic_rope = 1;
							}
						}
					}

					if (nr_items >= 3) {

						sub_group_le(3);

						for (i = 0; i < 3; i++) {

							answer = inv_slot_of_item((hero = get_hero(get_first_hero_with_item(ITEM_SEIL))), ITEM_SEIL);
							if (answer == -1) {
								answer = inv_slot_of_item((hero = get_hero(get_first_hero_with_item(ITEM_STRICKLEITER))), ITEM_STRICKLEITER);
							}

							drop_item(hero, answer, 1);
						}

						timewarp(HOURS(1));

						GUI_dialog_na(0, get_tx2(32));

						done = 1;

					} else if (nr_items == 2 && has_magic_rope != 0) {

						sub_group_le(3);

						timewarp(HOURS(1));

						GUI_dialog_na(0, get_tx2(31));

						done = 1;
					} else {

						GUI_dialog_na(0, get_tx2(30));

						timewarp(HOURS(2));

						GUI_dialog_na(0, get_tx2(33));
					}
				} else {
					timewarp(HOURS(2));
				}
			}
		}

	} while (done == 0);
}

/* Ottarje <-> Orvil: entrance to the dungeon "wolf cave" */
void tevent_061(void)
{
	signed int answer;

	/* TODO: not needed here */
	answer = 0;

	if (test_talent(get_first_hero_available_in_group(), TA_SINNESSCHAERFE, 6) > 0 &&
		test_talent(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 3) > 0 &&
		!gs_tevent061_flag)
	{
		gs_tevent061_flag = 1;

		/* TODO: is this correct ? */
		if (!TRV_follow_trail_question()) {

			GUI_output(get_tx2(34));
			GUI_output(get_tx2(35));

			load_in_head(53);

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(36),
							2, get_tx2(37), get_tx2(38));

			} while (answer == -1);

			if (answer == 1) {
				gs_travel_detour = DUNGEON_ID_WOLFSHOEHLE;
			}
		}

	} else if (gs_tevent061_flag != 0) {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(39),
						2, get_tx2(40),	get_tx2(41));

		} while (answer == -1);

		if (answer == 1) {
			gs_travel_detour = DUNGEON_ID_WOLFSHOEHLE;
		}
	}
}

/* Orvil <-> Skjal: dummy */
void tevent_062(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Reiseevent %d ist nicht implementiert\n", 62);
#endif
}

/* Orvil <-> Skjal: dead animal */
void tevent_063(void)
{
	signed int i; /* multiple use: hero_pos, mod_slot */
	signed int max;
	signed int proof;
	signed int vomiter;
	struct struct_hero *hero;

	max = 9999;

	/* intro message */
	GUI_output(get_tx2(46));

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead)
		{
			/* MU+0 */
			if ((proof = test_attrib(hero, ATTRIB_MU, 0)) < max) {
				max = proof;
				vomiter = i;
			}
		}
	}

	hero = get_hero(vomiter);

	sprintf(g_dtp2, get_tx2(47), hero->alias);

	/* print who vomits */
	GUI_output(g_dtp2);

	/* LE - 2 */
	sub_hero_le(hero, 2);

	i = get_free_mod_slot();

	/* MU -2 for 24 hours */
	set_mod_slot(i, DAYS(1), (uint8_t*)&hero->attrib[ATTRIB_MU].current, -2, (signed char)vomiter);

	/* outro message */
	GUI_output(get_tx2(48));
}

/* Gorahs place */
void tevent_064(void)
{
	signed int answer;
	signed int result;
	signed int i;
	signed int approach_flag;
	signed int silentium_flag;
	struct struct_hero *hero;

	approach_flag = 0;
	silentium_flag = 0;

	if (!gs_tevent064_flag && gs_umbrik_quest_gorah) {

		do {

			do {
				answer = GUI_radio(silentium_flag != 0 ? get_tx2(102) : get_tx2(49), 3,
							get_tx2(50), get_tx2(51), get_tx2(52));

			} while (answer == -1);

			if (answer == 1) {

				hero = get_hero(select_hero_ok_forced(get_ttx(317)));

				if (hero->typus < HERO_TYPE_HEXE) {
					GUI_output(get_ttx(330));
				} else {
					approach_flag = 1;
				}

				if (approach_flag == 0) {
					approach_flag = 1;
				} else {
					if (test_spell(hero, SP_SILENTIUM_SILENTILLE, 0) > 0) {

						sub_ae_splash(hero, 2);

						gs_tevent064_silent_flag = 1;

						GUI_output(get_tx2(53));
					} else {
						sub_ae_splash(hero, 1);

						GUI_output(get_ttx(606));
					}
				}

			} else if (answer == 2) {

				approach_flag = 1;

				GUI_output(get_tx2(54));

				hero = get_hero(0);

				for (i = result = 0; i <= 6; i++, hero++) {

					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
						!hero->flags.dead && test_talent(hero, TA_SCHLEICHEN, 0) <= 0)
					{
						result++;
					}
				}

				if (result > 1) {
					/* test failed for at least one hero */
					gs_tevent064_silent_flag = 1;
				}

			} else {

				approach_flag = 1;

				do {
					answer = GUI_radio(get_tx2(55), 2, get_tx2(56), get_tx2(57));

				} while (answer == -1);

				if (answer == 2)  return;
			}

		} while (approach_flag == 0);

		do {
			answer = GUI_radio(get_tx2(58), 2, get_tx2(59), get_tx2(60));

		} while (answer == -1);

		if (answer == 1) {

			if (!gs_tevent064_silent_flag) {
				GUI_output(get_tx2(61));
			}

			GUI_output(get_tx2(62));

			result = TRV_fight_event(FIGHTS_F064, 64);

			gs_tevent064_silent_flag = 1;

		} else {
			GUI_output(gs_tevent064_silent_flag != 0 ? get_tx2(62) : get_tx2(63));
			result = TRV_fight_event(FIGHTS_F064, 64);
		}

		if (!result) {

			gs_tevent064_flag = 1;

			add_hero_ap_all(100);

			do {
				do {
					answer = GUI_radio(get_tx2(64), 2, get_tx2(65), get_tx2(66));

				} while (answer == -1);

				if (answer == 1) {

					loot_multi_chest(gs_tevent064_hut_content, get_tx2(71));

					do {
						answer = GUI_radio(get_tx2(67), 2, get_tx2(68),	get_tx2(69));

					} while (answer == -1);

					if (answer == 1) {
						use_lockpicks_on_chest(&g_tevent064_chest_descr);
					}
				}

			} while (answer != 2);

			GUI_output(get_tx2(70));

			gs_journey_direction = (gs_journey_direction == JOURNEY_DIRECTION_FORWARD ? JOURNEY_DIRECTION_CHANGE_TO_BACKWARD : JOURNEY_DIRECTION_CHANGE_TO_FORWARD);
		}

	} else if (gs_tevent064_flag != 0) {

		do {
			do {
				answer = GUI_radio(get_tx2(64), 2, get_tx2(65),	get_ttx(814));

			} while (answer == -1);

			if (answer == 1) {

				loot_multi_chest(gs_tevent064_hut_content, get_tx2(71));

				if ((signed char)gs_tevent064_chest[0] != -1) {

					do {
						answer = GUI_radio(get_tx2(67), 2, get_tx2(68),	get_ttx(814));

					} while (answer == -1);

					if (answer == 1) {
						use_lockpicks_on_chest(&g_tevent064_chest_descr);
					}
				}
			}

		} while (answer != 2);
	}
}

void tevent_064_chest(struct struct_chest*)
{
	loot_multi_chest(gs_tevent064_chest, get_tx2(74));
}

/* Orvil <-> Ala: rider */
void tevent_065(void)
{
	signed int answer;

	/* load dialog head */
	load_in_head(56);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(75),
				3, get_tx2(76),	get_tx2(77), get_tx2(78));

	} while (answer == -1);

	/* talk to the rider for 15 min */
	if (answer == 1 || answer == 2) {

		timewarp(MINUTES(15));

		GUI_dialog_na(0, answer == 1 ? get_tx2(79): get_tx2(80));

		if (answer == 2) {
			/* Make Umbrik Siebenstein known */

			/* Original-Bug: if Umbrik is done the value 2 is overwritten with 1 */
			update_informer_cond(INFORMER_ID_UMBRIK);
		}
	}
}

void tevent_066(void)
{
	signed int answer;
	signed int i;
	signed int count;
	struct struct_hero *hero;

	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent066_flag) || gs_tevent066_flag)
	{
		TRV_found_replenish_place(0);
		gs_tevent066_flag = 1;

		if ((test_talent(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 4) > 0 && !gs_tevent066_track_flag))
		{

			do {
				answer = GUI_radio(get_tx2(81), 2, get_tx2(82), get_tx2(83));

			} while (answer == -1);

			if (answer == 1) {

				GUI_output(get_tx2(84));

				hero = get_hero(0);
				for (i = count = 0; i <= 6; i++, hero++)
				{
					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
						!hero->flags.dead && test_talent(hero, TA_SCHLEICHEN, -2) <= 0)
					{
						count++;
					}
				}

				if (count != 0) {

					GUI_output(get_tx2(85));

				} else {
					do {
						answer = GUI_radio(get_tx2(86), 2, get_tx2(87), get_tx2(88));

					} while (answer == -1);

					if (answer == 1) {

						GUI_output(get_tx2(89));
						return;
					}
				}

				if (!do_fight(FIGHTS_F066)) {
					gs_tevent066_track_flag = 1;
					add_hero_ap_all(50);
				}
			}
		}
	}
}
