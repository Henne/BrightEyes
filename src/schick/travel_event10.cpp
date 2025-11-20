/**
 *	Rewrite of DSA1 v3.02_de functions of seg118 (travel events 10 / 10)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg118.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "misc.h"
#include "graphics.h"
#include "random_dice.h"
#include "locations.h"
#include "save_text.h"
#include "file_loader.h"
#include "resource_loader.h"
#include "playmask.h"
#include "talk_date.h"
#include "dungeon_common1.h"
#include "text_output.h"
#include "gui.h"
#include "talents.h"
#include "travel_event1.h"
#include "travel_event5.h"

static const char g_str_rad1[4] = "RAD"; // ds:0xb21e
static const char g_str_rad2[4] = "RAD"; // ds:0xb222
static const char g_str_rad3[4] = "RAD"; // ds:0xb226


/* The griffon */
void tevent_037(void)
{
	signed int answer;
	signed int done = 0;
	signed int hours;
	struct struct_hero *hero;

	if (!gs_tevent037_flag)
	{
		g_event_ani_busy = 1;

		load_ani(28);
		draw_main_screen();
		init_ani(0);

		do {
			answer = GUI_radio(get_tx2(26), 2, get_tx2(27), get_tx2(28));

		} while (answer == -1);

		if (answer == 1) {
			/* wait */

			timewarp(MINUTES(15));

			do {
				answer = GUI_radio(get_tx2(29), 2, get_tx2(30),	get_tx2(28));

			} while (answer == -1);

			if (answer == 1)
			{

				timewarp(MINUTES(15));

				/* loop until you decide to go on */
				do {
					answer = GUI_radio(get_tx2(31), 2, get_tx2(30), get_tx2(28));

				} while (answer == -1 || answer == 1);
			}
		}

		if (answer == 2) {
			/* go on */

			do {
				answer = GUI_radio(get_tx2(32), 2, get_tx2(33), get_tx2(34));

			} while (answer == -1);
		}

		do {

			if (answer == 1)
			{
				/* turn around */
				hours = GUI_input(get_tx2(35), 2);

				if (hours >= 4)
				{
					sprintf(g_dtp2, get_tx2(36), hours);
					GUI_output(g_dtp2);

					done = 1;

				} else {

					/* entered time was too short */
					do {
						answer = GUI_radio(get_tx2(37), 2, get_tx2(33), get_tx2(34));

					} while (answer == -1);

					if (answer == 1)
					{
						timewarp(HOURS(4));

						GUI_output(get_tx2(38));

						done = 1;
					}
				}
			} else {
				do {
					answer = GUI_radio(get_tx2(39), 3, get_tx2(40), get_tx2(41), get_tx2(42));

				} while (answer == -1);

				if (answer == 1)
				{
					/* offer a deposit */
					gs_tevent037_flag = 1;

					/* select a hero randomly */
					hero = get_hero(answer = get_random_hero());

					sprintf(g_dtp2, get_tx2(45), hero->alias, GUI_get_ptr(hero->sex, 2));
					GUI_output(g_dtp2);

					add_hero_ap(hero, 20);

					add_hero_ap_all(20);

					GUI_output(get_tx2(46));

					GUI_input(get_tx2(47), 10);

					/* check if the answer is "RAD" 1st */
					if (strcmp(g_text_input_buf, g_str_rad1))
					{
						GUI_input(get_tx2(48), 10);

						/* check if the answer is "RAD" 2nd */
						if (strcmp(g_text_input_buf, g_str_rad2))
						{
							GUI_input(get_tx2(49), 10);

							/* check if the answer is "RAD" 3rd */
							if (strcmp(g_text_input_buf, g_str_rad3))
							{
								sprintf(g_dtp2, get_tx2(50), hero->alias, GUI_get_article(hero->sex, 0), hero->alias, GUI_get_ptr(hero->sex, 2));

								GUI_output(g_dtp2);

								/* the hero is now lost completely */
								hero_disappear(hero, answer, -1);

								done = 1;
							} else {

								GUI_output(get_tx2(51));

								add_hero_ap_all(10);

								done = 1;
							}
						} else {

							GUI_output(get_tx2(51));

							add_hero_ap_all(10);

							done = 1;
						}
					} else {

						GUI_output(get_tx2(51));

						add_hero_ap_all(10);

						done = 1;
					}

				} else {
					/* talk of hetmann or hyggelik */

					do {
						answer = GUI_radio(get_tx2(43), 2, get_tx2(33), get_tx2(34));

					} while (answer == -1);

					if (answer == 2)
					{
						/* select a hero randomly */
						hero = get_hero(get_random_hero());

						timewarp(HOURS(1));

						sprintf(g_dtp2, get_tx2(44), hero->alias);
						GUI_output(g_dtp2);
					}
				}
			}

		} while (!done);

		disable_ani();

		g_event_ani_busy = 0;
		g_request_refresh = 1;
	}
}

/* unicorn 1st time */
void tevent_038(void)
{
	if (!gs_met_unicorn_flag && gs_got_main_quest)
	{
		do_talk(11, 2);

		gs_met_unicorn_flag = 1;
	}
}

void tevent_078(void)
{
	/* This event happens only in winter */
	if (!get_current_season())
	{
		signed int tmp;
		signed int answer;
		signed int found_path;
		signed int days;
		struct struct_hero *hero;

		do {
			answer = GUI_radio(get_tx2(78), 2, get_tx2(79), get_tx2(80));

		} while (answer == -1);

		load_in_head(57);

		if (answer == 1)
		{
			/* try to clean the path */

			/* with SHOVEL/SCHAUFEL 5 Hours, without 8 Hours */
			if (get_first_hero_with_item(ITEM_SCHAUFEL) != -1) {
				tmp = 5;
			} else {
				tmp = 8;
			}

			timewarp(HOURS(tmp));

			GUI_dialog_na(0, get_tx2(81));

			/* each hero in the group looses tmp / 2 LE */
			sub_group_le(tmp >> 1);

			g_wildcamp_sleep_quality = -6;

			gs_town_loc_type = LOCTYPE_WILDCAMP;
			do_location();
			gs_town_loc_type = LOCTYPE_NONE;

			TRV_load_textfile(-1);

#ifdef M302de_ORIGINAL_BUGFIX
			/* if a campfight occurs, the picture needs to be reloaded */
			load_in_head(57);
#endif
			GUI_dialog_na(0, get_tx2(84));

		} else {
			/* try to find another path */
			found_path = days = 0;

			do {
				/* ORIENTATION + 8 */
				if (test_talent(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 8) > 0)
				{
					/* success, you found a way */
					timewarp(HOURS(5));

					sub_group_le(2);

					GUI_dialog_na(0, !answer ? get_tx2(82) : get_tx2(85));

					found_path = 1;
				} else {
					/* fail */
					timewarp(HOURS(8));

					GUI_dialog_na(0, get_tx2(83));

					sub_group_le(4);

					g_wildcamp_sleep_quality = -6;
					/* make a camp */
					gs_town_loc_type = LOCTYPE_WILDCAMP;
					do_location();
					gs_town_loc_type = LOCTYPE_NONE;

					TRV_load_textfile(-1);

					answer = 1;
				}
#ifdef M302de_ORIGINAL_BUGFIX
				/* if a campfight occurs, the picture needs to be reloaded */
				load_in_head(57);
#endif

			} while (found_path == 0 && ++days < 6);

			if (found_path == 0)
			{
				GUI_dialog_na(0, get_tx2(86));

				timewarp(HOURS(8));

				/* TODO: Original-Bug: all heroes die, even if they are not in the current group */
				hero = get_hero(0);
				for (tmp = 0; tmp <= 6; tmp++, hero++)
				{
#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 16: all heroes die, even if they are not in the current group */
					hero_disappear(hero, tmp, -1);
#else
					if ((hero->typus != HERO_TYPE_NONE) &&
						(hero->group_id == gs_active_group_id))
					{
						hero_disappear(hero, tmp, -1);
					}
#endif
				}
			}
		}

		g_wildcamp_sleep_quality = 0;
	}
}

void tevent_079(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 4) > 0 && !gs_tevent079_flag) ||
		gs_tevent079_flag != 0)
	{
		TRV_found_camp_place(0);
		gs_tevent079_flag = 1;
	}
}

/* Ottarje <-> Skal: entrance to spider cave */
void tevent_051(void)
{
	signed int answer;

	if (test_talent(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 4) > 0 && !gs_tevent051_flag)
	{
		gs_tevent051_flag = 1;

		if (!TRV_follow_trail_question())
		{

			GUI_output(get_tx2(65));
			GUI_output(get_tx2(66));

			load_in_head(53);

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(67), 2, get_tx2(68), get_tx2(69));

			} while (answer == -1);

			if (answer == 1)
			{
				gs_travel_detour = DUNGEON_ID_SPINNENHOEHLE;
			}
		}

	} else if (gs_tevent051_flag != 0) {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(70), 2, get_tx2(71), get_tx2(72));

		} while (answer == -1);

		if (answer == 1)
		{
			gs_travel_detour = DUNGEON_ID_SPINNENHOEHLE;
		}
	}
}

/* Ottarje <-> Skjal: narrow pass blocked by a tree */
void tevent_052(void)
{
	signed int done = 0;
	signed int i;
	signed int answer;

	load_in_head(52);

	do {

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(73), 2, get_tx2(74), get_tx2(75));

		} while (answer == -1);

		if (answer == 1)
		{
			timewarp(HOURS(1));

			GUI_dialog_na(0, get_tx2(76));

			done = 1;
		} else {

			i = 0;

			do {
				/* check for a chopping tool */
				if (get_first_hero_with_item(g_travel_event_axes[i]) != -1)
				{
					done = 1;
				}

			} while (!done && (g_travel_event_axes[++i] != 0xff));

			if (done)
			{
				timewarp(HOURS(3));

				GUI_dialog_na(0, get_tx2(77));
			} else {
				GUI_dialog_na(0, get_tx2(78));
			}
		}

	} while (!done);
}

void tevent_120(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !gs_tevent120_flag) || gs_tevent120_flag)
	{
		gs_tevent120_flag = 1;
		TRV_found_camp_place(1);
	}
}

void tevent_121(void)
{
	TRV_cross_a_ford(get_tx(53), 30, 0);
}

void tevent_124(void)
{
	signed int i;
	signed int counter;
	signed int answer;
	signed int have_climb_tools;
	signed int talent_ret;
	struct struct_hero *hero;

	have_climb_tools = 0;

	GUI_output(get_tx2(49));

	if (DNG_check_climb_tools() != -1)
	{
		have_climb_tools = 1;

		GUI_output(get_tx2(50));
	}

	do {
		answer = GUI_radio(get_tx2(51), 2, get_tx2(52), get_tx2(53));

	} while (answer == -1);

	if (answer == 2)
	{
		gs_trv_return = 1;

	} else {

		hero = get_hero(0);
		for (i = counter = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) &&
				(hero->group_id == gs_active_group_id) &&
				!hero->flags.dead &&
				(talent_ret = test_talent(hero, TA_KLETTERN, -2)) <= 0)
			{
				/* the climb test failed */
				counter++;

				/* Original-Bug: a talent test with a fatal result returns -99, not -1 */
#ifdef M302de_ORIGINAL_BUGFIX
				if (talent_ret == -99 && have_climb_tools == 0)
#else
				if (talent_ret == -1 && have_climb_tools == 0)
#endif
				{
					/* fatal */
					sprintf(g_dtp2, get_tx2(56), hero->alias, GUI_get_ptr(hero->sex, 0));
					GUI_output(g_dtp2);

					hero_disappear(hero, i, -1);

				} else {

					sprintf(g_dtp2, get_tx2(55), hero->alias);
					GUI_output(g_dtp2);

					sub_hero_le(hero, random_schick(5));
				}
			}
		}

		if (!counter) {
			GUI_output(get_tx2(54));
		}
	}
}

void tevent_125(void)
{
	signed int answer;

	load_in_head(17);

	do {
		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(57), 3, get_tx2(58), get_tx2(59), get_tx2(60));

	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		GUI_dialog_na(0, answer == 1 ? get_tx2(61) : get_tx2(62));

		timewarp(MINUTES(30));
	}
}

/* Reach street (either Kravik-Skelellen or Peilinen-Rovamund). */
void tevent_145(void)
{
	signed int answer;

	load_area_description(0);

	if (!(g_route59_flag & 1))
	{
		/* g_route_flag in {0, 2} */
		do {
			answer = GUI_radio(get_tx2(42), 2, get_tx2(43), get_tx2(44));

		} while (answer == -1);

		if (answer == 1)
		{
			gs_trv_destination = TOWN_ID_KRAVIK;
			gs_town_id = TOWN_ID_SKELELLEN;

		} else {
			gs_trv_destination = TOWN_ID_SKELELLEN;
			gs_town_id = TOWN_ID_KRAVIK;
		}

	} else {
		/* g_route_flag in {1, 3} */
		do {
			answer = GUI_radio(get_tx2(39), 2, get_tx2(40),	get_tx2(41));

		} while (answer == -1);

		if (answer == 1) {

			gs_trv_destination = TOWN_ID_PEILINEN;
			gs_town_id = TOWN_ID_ROVAMUND;

		} else {
			gs_trv_destination = TOWN_ID_ROVAMUND;
			gs_town_id = TOWN_ID_PEILINEN;
		}
	}

	sprintf(g_dtp2, get_tx2(45), get_ttx(gs_trv_destination + 0xeb));
	GUI_output(g_dtp2);
}
