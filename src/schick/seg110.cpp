/**
 *	Rewrite of DSA1 v3.02_de functions of seg110 (travel events 2 / 10)
 *	Functions rewritten: 35/35 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg110.cpp
 */

#include <stdio.h>

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
#include "seg032.h"
#include "seg047.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg110.h"
#include "seg117.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static struct struct_chest g_tevent014_corpse = {0, 1, NULL, NULL, tevent_014_chest, 0, 0, 0}; // ds:0xb13e

void tevent_011(void)
{
	signed int answer;
	struct struct_hero *hero;

	if ((test_skill((hero = get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 8) > 0 && !gs_tevent011_flag) ||
		gs_tevent011_flag)
	{
		gs_tevent011_flag = 1;

		sprintf(g_dtp2,	get_tx2(24), hero->alias, hero->alias);

		do {
			answer = GUI_radio(g_dtp2, 2, get_tx2(25), get_tx2(26));

		} while (answer == -1);

		if (answer == 2) {

			do {
				answer = GUI_radio(get_tx2(27), 3, get_tx2(28),	get_tx2(29), get_tx2(30));

			} while (answer == -1);

			if (answer == 1) {

				/* just swim */
				TRV_swim2(0, 5);
				TRV_a_path();

			} else if (answer == 2) {

				/* build a raft */
				timewarp(HOURS(1));
				TRV_swim2(0, 0);
				TRV_a_path();
			}
		}
	}
}

/**
 * \brief   all members of the current group try to swim
 *
 *          In seg112.cpp exists a similiar function called TRV_swim() for that purpose.
 *
 * \param   mod         modificator for the swim test
 * \param   percent     probability to loose an item
 */
/* should be static */
void TRV_swim2(const signed int mod, const signed int percent)
{
	signed int i;
	struct struct_hero *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++)
	{
		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead)
		{
			/* a swim test */
			if (test_skill(hero, TA_SCHWIMMEN, (signed char)mod) > 0)
			{
				/* skill test succeeded */
				sprintf(g_dtp2, get_tx2(31), hero->alias);
				GUI_output(g_dtp2);

			} else {
				/* skill test failed */
				sprintf(g_dtp2, get_tx2(32), hero->alias, GUI_get_ptr(hero->sex, 0));
				GUI_output(g_dtp2);

				hero_disease_test(hero, 2, 20 - (hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod));

				if (percent) {
					loose_random_item(hero, percent, get_ttx(506));
				}

				sub_hero_le(hero, random_schick(5));
			}
		}
	}
}

/* Path east from the route Rovamund <-> Peilinen. */
/* should be static */
void TRV_a_path(void)
{
	signed int answer;

	do {
		answer = GUI_radio(get_tx2(33), 2, get_tx2(34), get_tx2(35));

	} while (answer == -1);

	if (answer == 1)
	{
		/* follow the path */
		g_route59_flag = (gs_current_town == TOWNS_PEILINEN ? 2 : 4);

		/* Original-Glitch:
		 * gs_travel_detour == 1 is indicating a detour to DNG_TOTENSCHIFF (which has the ID 1).
		 * Probably, this does not make a difference, but still, it would be better to use another number. */
		gs_travel_detour = 1;
	} else {
		/* swim back */

		do {
			answer = GUI_radio(get_tx2(36), 2, get_tx2(28),	get_tx2(29));

		} while (answer == -1);

		if (answer == 1)
		{
			/* just swim */
			TRV_swim2(0, 5);

		/* TODO: this test is bogus, just an else case */
		} else if (answer == 2)
		{
			/* build a raft */
			timewarp(HOURS(1));
			TRV_swim2(0, 0);
		}

		GUI_output(get_tx2(37));
	}
}

void tevent_013(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 3) > 0 && !gs_tevent013_flag) ||
		gs_tevent013_flag)
	{
		g_gather_herbs_special = 123;
		TRV_found_herb_place(0);
		g_gather_herbs_special = -1;
		gs_tevent013_flag = 1;
	}
}

void tevent_014(void)
{
	signed int answer;

	if (!gs_tevent014_flag)
	{
		load_in_head(55);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(43), 2, get_tx2(44), get_tx2(45));

		} while (answer == -1);

		if (answer == 1)
		{
			/* examine the corpse */
			loot_corpse(&g_tevent014_corpse, get_tx2(46), (Bit8s*)&gs_tevent014_flag);
		}
	}
}

void tevent_014_chest(struct struct_chest* chest)
{
	chest->content = gs_tevent014_chest;

	loot_chest(chest, get_tx2(47), get_tx2(48));
}

void tevent_015(void)
{
	TRV_found_inn(TOWNS_VAERMHAG, 67);
}

void tevent_017(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent017_flag) ||
		gs_tevent017_flag)
	{
		TRV_found_replenish_place(0);
		gs_tevent017_flag = 1;
	}
}

/* Path west from the route Kravik <-> Skelellen. */
void tevent_020(void)
{
	signed int answer;
	struct struct_hero *hero;

	if ((test_skill((hero = get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 7) > 0 && !gs_tevent020_flag) ||
		gs_tevent020_flag)
	{
		gs_tevent020_flag = 1;

		sprintf(g_dtp2, get_tx2(49), hero->alias);

		do {
			answer = GUI_radio(g_dtp2, 2, get_tx2(50), get_tx2(51));

		} while (answer == -1);

		if (answer == 1)
		{
			do {
				answer = GUI_radio(get_tx2(52), 2, get_tx2(53), get_tx2(54));

			} while (answer == -1);

			if (answer == 1)
			{
			    /* TODO: Original-Bug: CURRENT_TOWN is either Kravik or Skelellen. */
				g_route59_flag = (gs_current_town == TOWNS_PEILINEN ? 1 : 3);

				/* Original-Glitch:
				 * gs_travel_detour == 1 is indicating a detour to DNG_TOTENSCHIFF (which had the ID 1).
				 * Probably, this does not make a difference, but still, it would be better to use another number. */
				gs_travel_detour = 1;
			}
		}

		timewarp(MINUTES(15));
	}
}

void tevent_021(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 8) > 0 && !gs_tevent021_flag) ||
		gs_tevent021_flag)
	{
		g_gather_herbs_special = 126;
		TRV_found_herb_place(1);
		g_gather_herbs_special = -1;

		gs_tevent021_flag = 1;
	}
}

void tevent_022(void)
	/* Rukian <-> Faehrstation Angbodirtal: windgeschuetzte Stelle */
{
	signed int answer;

	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 0) > 0 && !gs_tevent022_flag) ||
		gs_tevent022_flag)
	{
		gs_tevent022_flag = 1;
		if (TRV_found_camp_place(0))
		{
			if ((test_skill(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 5) > 0 && !gs_tevent022_track_flag) ||
				gs_tevent022_track_flag)
			{
				do {
					answer = GUI_radio(get_tx2(59), 2, get_tx2(60), get_tx2(61));

				} while (answer == -1);

				if (answer == 1)
				{
					TRV_hunt_generic(25, 62, -2, 2, 8, 3, 7, 2, 4, 30, 6);
					gs_tevent022_track_flag = 1;
				}
			}
		}
	}
}

void tevent_024(void)
	/* Auplog <-> Vilnheim: windgeschuetzte Stelle. Idealer Rastplatz */
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent024_flag) ||
		gs_tevent024_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent024_flag = 1;
	}
}

void tevent_025(void)
	/* Auplog <-> Vilnheim: umgestuerzte, moosbewachsene Baumstaemme. idealer Rastplatz. */
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 1) > 0 && !gs_tevent025_flag) ||
		gs_tevent025_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent025_flag = 1;
	}
}

void tevent_026(void)
{
	TRV_cross_a_ford(get_tx2(72), 60, 0);
}

void tevent_027(void)
{
	TRV_cross_a_ford(get_tx2(73), 30, 0);
}

void tevent_028(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent028_flag) ||
		gs_tevent028_flag)
	{
		TRV_found_camp_place(1);
		gs_tevent028_flag = 1;
	}
}

void tevent_029(void)
{
	signed int i;
	signed int item_pos;
	struct struct_hero *hero;

	load_in_head(51);

	GUI_dialog_na(0, get_tx2(74));
	GUI_dialog_na(0, get_tx2(75));

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++)
	{
		hero_disease_test(hero, 2, 20 - (hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod));
	}

	if (test_skill(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 3) > 0)
	{
		/* skill test succeeded */
		timewarp(HOURS(3));

		GUI_dialog_na(0, get_tx2(76));
	} else {
		/* skill test failed */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead)
			{
				sub_hero_le(hero, 2);

				item_pos = get_item_pos(hero, ITEM_FOOD_PACKAGE);

				if (item_pos != -1) {
					/* hero looses the first set of FOOD PACKAGES */
					drop_item(hero, item_pos, hero->inventory[item_pos].quantity);
				}

				item_pos = hero_count_item(hero, ITEM_WATERSKIN);

				if (item_pos) {
					/* hero looses the first WATERSKIN */
					drop_item(hero, get_item_pos(hero, ITEM_WATERSKIN), item_pos - 1);
				}
			}
		}

		timewarp(HOURS(5));

		GUI_dialog_na(0, get_tx2(77));
	}
}

void tevent_030(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 0) > 0 && !gs_tevent030_flag) ||
		gs_tevent030_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent030_flag = 1;
	}
}

void tevent_031(void)
{
	signed int answer;

	if (gs_find_hyggelik) {

		search_ruin1();

	} else if (!gs_tevent031_flag) {

		load_in_head(49);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(0), 2, get_tx2(1), get_tx2(2));

		} while (answer == -1);

		if (answer == 2)
		{
			if (test_skill(get_first_hero_available_in_group(), TA_KRIEGSKUNST, 7) <= 0)
			{
				/* skill test failed */
				g_fig_initiative = 1;
				do_fight(FIGHTS_F031);
			}
		} else {
			do_fight(FIGHTS_F031);
		}

		gs_tevent031_flag = 1;
	}
}

void tevent_032(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent032_flag) ||
		gs_tevent032_flag)
	{
		gs_tevent032_flag = 1;

		if ((test_skill(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 5) > 0 && !gs_tevent032_herb_flag) ||
			gs_tevent032_herb_flag)
		{
			gs_tevent032_herb_flag = 1;

			g_gather_herbs_special = 62;
			TRV_found_camp_place(2);
			g_gather_herbs_special = -1;
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_033(void)
{
	signed int answer;

	load_in_head(9);

	do {
		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(3), 3,
					get_tx2(4), get_tx2(5), get_tx2(6));

	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		timewarp(MINUTES(15));

		GUI_dialog_na(0, (answer == 1 ? get_tx2(7) : get_tx2(8)));
	}
}

void tevent_034(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent034_flag) ||
		gs_tevent034_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent034_flag = 1;
	}
}

void tevent_035(void)
{
	signed int answer;

	if (!gs_tevent035_flag)
	{
		do {
			answer = GUI_radio(get_tx2(9), 2, get_tx2(10), get_tx2(11));

		} while (answer == -1);

		if (answer == 2)
		{
			if (test_skill(get_first_hero_available_in_group(), TA_KRIEGSKUNST, 8) <= 0)
			{
				/* skill test failed */
				g_fig_initiative = 1;
				do_fight(FIGHTS_F035);
			}
		} else {
			do_fight(FIGHTS_F035);
		}

		gs_tevent035_flag = 1;
	}
}

void tevent_036(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !gs_tevent036_flag) ||
		gs_tevent036_flag)
	{
		gs_tevent036_flag = 1;

		if ((test_skill(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 6) > 0 && !gs_tevent036_herb_flag) ||
			gs_tevent036_herb_flag)
		{
			gs_tevent036_herb_flag = 1;

			g_gather_herbs_special = 63;
			TRV_found_camp_place(2);
			g_gather_herbs_special = -1;
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_039(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 4) > 0 && !gs_tevent039_flag) ||
		gs_tevent039_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent039_flag = 1;
	}
}

void tevent_040(void)
/* Auplog <-> Varnheim: alte Feuerstelle. idealer Rastplatz */
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 4) > 0 && !gs_tevent040_flag) ||
		gs_tevent040_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent040_flag = 1;
	}
}

/* Auplog <-> Varnheim: kleiner aber reissender Bach, ueber den sich urspruenglich einmal eine Haengebruecke spannte. bridge building */
void tevent_041(void)
{
	signed int answer;

	TRV_barrier(12);

	if (test_skill(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 0) > 0 && !gs_tevent041_flag)
	{
		do {
			/* Spuren von Karenen */
			answer = GUI_radio(get_tx2(20), 2, get_tx2(21), get_tx2(22));

		} while (answer == -1);

		if (answer == 1)
		{
			TRV_hunt_generic(21, 23, -2, 2, 8, 3, 7, 2, 4, 15, 8);

			gs_tevent041_flag = 1;
		}
	}
}

void tevent_042(void)
	/* Auplog <-> Varnheim: (weiter von Auplog weg als tevent_043) kleiner Bachlauf. Nahrung. */
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent042_flag) ||
		gs_tevent042_flag)
	{
		TRV_found_replenish_place(0);
		gs_tevent042_flag = 1;
	}
}

void tevent_043(void)
	/* Auplog <-> Varnheim: (nahe an Auplog) kleiner Bachlauf. Nahrung. */
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !gs_tevent043_flag) ||
		gs_tevent043_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent043_flag = 1;
	}
}

/* dying adventuress */
void tevent_044(void)
{
	signed int answer;
	struct struct_hero *hero;

	if (!gs_tevent094_flag) {

		gs_tevent094_flag = 1;

		load_in_head(1);

		hero = get_first_hero_available_in_group();

		sprintf((char*)(g_dtp2 + 0x400), get_tx2(33), hero->alias, GUI_get_ptr(hero->sex, 3));

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, (char*)(g_dtp2 + 0x400), 2, get_tx2(34), get_tx2(35));

		} while (answer == -1);

		if (answer == 2) {

			/* bury her */
			gs_gods_estimation[GOD_BORON] += 50L;

			timewarp(HOURS(1));

			add_hero_ap_all(10);
		}
	}
}

/* Ottarje -> Daspota: a farmer */
void tevent_045(void)
{
	signed int answer;

	if (gs_trv_destination == TOWNS_DASPOTA)
	{
		load_in_head(11);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(36), 3,
						get_tx2(37), get_tx2(38), get_tx2(39));

		} while (answer == -1);

		if (answer == 1 || answer == 2)
		{
			timewarp(MINUTES(15));

			GUI_dialog_na(0, (answer == 1 ? get_tx2(40) : get_tx2(41)));
		}
	}
}

/* Daspota <-> Ottarje: entrance to the dungeon "verlassene Herberge"("abandoned inn") */
void tevent_046(void)
{
	signed int answer;
	signed int enter_inn;
	struct struct_hero *hero;

	enter_inn = 0;

	/* pause traveling */
	g_event_ani_busy = 1;

	load_ani(12);
	draw_main_screen();
	init_ani(0);

	do {
		answer = GUI_radio(get_tx2(42), 2, get_tx2(43),	get_tx2(44));

	} while (answer == -1);

	if (answer == 1)
	{
		/* make a camp */
		gs_camp_incident = 1;
		gs_current_loctype = LOCTYPE_WILDCAMP;
		do_location();
		gs_current_loctype = LOCTYPE_NONE;
		TRV_load_textfile(-1);

		hero = (struct struct_hero*)(gs_camp_incident != -1 ? (Bit8u*)get_hero(gs_camp_incident) : (Bit8u*)get_first_hero_available_in_group());
		gs_camp_incident = -1;

		if (test_skill(hero, TA_SINNESSCHAERFE, 0) > 0)
		{
			sprintf(g_dtp2,	get_tx2(45), hero->alias, GUI_get_ptr(hero->sex, 0));

			do {
				answer = GUI_radio(g_dtp2, 2, get_tx2(46), get_tx2(47));

			} while (answer == -1);

			if (answer == 1)
			{
				gs_travel_detour = DUNGEONS_VERFALLENE_HERBERGE;
				enter_inn = 1;
			}
		}

		if (!enter_inn)
		{
			sprintf(g_dtp2, get_tx2(48), hero->alias);
			GUI_output(g_dtp2);

			g_fig_discard = 1;

			TRV_fight_event(FIGHTS_F046, 46);

			do {
				answer = GUI_radio(get_tx2(49), 2, get_tx2(50), get_tx2(51));

			} while (answer == -1);

			if (answer == 1)
			{
				gs_travel_detour = DUNGEONS_VERFALLENE_HERBERGE;
			}
		}
	}

	disable_ani();
	/* resume traveling */
	g_event_ani_busy = 0;
	g_request_refresh = 1;
}

/* Daspota <-> Ottarje: alte Feuerstelle. idealer Rastplatz. */
void tevent_048(void)
{
	signed int answer;

	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !gs_tevent048_flag) ||
		gs_tevent048_flag)
	{
		gs_tevent048_flag = 1;

		if (TRV_found_camp_place(0))
		{
			if ((test_skill(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 3) > 0 && !gs_tevent048_track_flag) ||
				gs_tevent048_track_flag)
			{
				/* Spuren einer kleinen Karenherde */
				do {
					answer = GUI_radio(get_tx2(52), 2, get_tx2(53), get_tx2(54));

				} while (answer == -1);

				if (answer == 1)
				{
					TRV_hunt_generic(21, 55, -2, 2, 8, 3, 5, 2, 4, 25, 12);
					gs_tevent048_track_flag = 1;
				}
			}
		}
	}
}

/* Ottarje <-> Skjal */
void tevent_049(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent049_flag) ||
		gs_tevent049_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent049_flag = 1;
	}
}

/* Ottarje <-> Skjal */
void tevent_050(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !gs_tevent050_flag) ||
		gs_tevent050_flag)
	{
		gs_tevent050_flag = 1;

		if ((test_skill(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 4) > 0 && !gs_tevent050_herb_flag) ||
			gs_tevent050_herb_flag)
		{
			TRV_found_replenish_place(gs_tevent050_herb_flag = 1);
		} else {
			TRV_found_replenish_place(0);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
