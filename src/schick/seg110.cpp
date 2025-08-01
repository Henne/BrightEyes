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

void tevent_011(void)
{
	signed short answer;
	Bit8u *hero;

	if ((test_skill((hero = (Bit8u*)get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 8) > 0 && !ds_readb(TEVENT011_FLAG)) ||
		ds_readb(TEVENT011_FLAG) != 0)
	{
		ds_writeb(TEVENT011_FLAG, 1);

		sprintf(g_dtp2,
			get_tx2(24),
			(char*)hero + HERO_NAME2,
			(char*)hero + HERO_NAME2);

		do {
			answer = GUI_radio(g_dtp2, 2,
						get_tx2(25),
						get_tx2(26));
		} while (answer == -1);

		if (answer == 2)
		{
			do {
				answer = GUI_radio(get_tx2(27), 3,
							get_tx2(28),
							get_tx2(29),
							get_tx2(30));
			} while (answer == -1);

			if (answer == 1)
			{
				/* just swim */
				TRV_swim2(0, 5);
				TRV_a_path();

			} else if (answer == 2)
			{
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
void TRV_swim2(signed char mod, signed short percent)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
			!hero_dead(hero))
		{
			/* a swim test */
			if (test_skill(hero, TA_SCHWIMMEN, (signed char)mod) > 0)
			{
				/* skill test succeeded */
				sprintf(g_dtp2, get_tx2(31), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);

			} else {
				/* skill test failed */
				sprintf(g_dtp2, get_tx2(32), (char*)hero + HERO_NAME2,
					(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

				GUI_output(g_dtp2);

				hero_disease_test(hero, 2, 20 - (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))));

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
	signed short answer;

	do {
		answer = GUI_radio(get_tx2(33), 2,
					get_tx2(34),
					get_tx2(35));
	} while (answer == -1);

	if (answer == 1)
	{
		/* follow the path */
		ds_writeb(ROUTE59_FLAG, (gs_current_town == TOWNS_PEILINEN ? 2 : 4));

		/* Original-Glitch:
		 * TRAVEL_DETOUR == 1 is indicating a detour to DNG_TOTENSCHIFF (which has the ID 1).
		 * Probably, this does not make a difference, but still, it would be better to use another number. */
		ds_writeb(TRAVEL_DETOUR, 1);
	} else {
		/* swim back */

		do {
			answer = GUI_radio(get_tx2(36), 2,
						get_tx2(28),
						get_tx2(29));
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
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 3) > 0 && !ds_readb(TEVENT013_FLAG)) ||
		ds_readb(TEVENT013_FLAG) != 0)
	{
		ds_writeb(GATHER_HERBS_SPECIAL, 123);
		TRV_found_herb_place(0);
		ds_writeb(GATHER_HERBS_SPECIAL, -1);
		ds_writeb(TEVENT013_FLAG, 1);
	}
}

void tevent_014(void)
{
	signed short answer;

	if (!ds_readb(TEVENT014_FLAG))
	{
		load_in_head(55);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(43),
						2, get_tx2(44),	get_tx2(45));

		} while (answer == -1);

		if (answer == 1)
		{
			/* examine the corpse */
			loot_corpse(((Bit8u*)p_datseg + TEVENT014_CORPSE), get_tx2(46), p_datseg + TEVENT014_FLAG);
		}
	}
}

void tevent_014_chest(Bit8u* chest)
{
	host_writed((Bit8u*)(chest) + 11, (Bit32u)(p_datseg + TEVENT014_CHEST));

	loot_chest((Bit8u*)(chest), get_tx2(47), get_tx2(48));
}

void tevent_015(void)
{
	TRV_found_inn(TOWNS_VAERMHAG, 67);
}

void tevent_017(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT017_FLAG)) ||
		ds_readb(TEVENT017_FLAG) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(TEVENT017_FLAG, 1);
	}
}

/* Path west from the route Kravik <-> Skelellen. */
void tevent_020(void)
{
	signed short answer;
	Bit8u *hero;

	if ((test_skill((hero = (Bit8u*)get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 7) > 0 && !ds_readb(TEVENT020_FLAG)) ||
		ds_readb(TEVENT020_FLAG) != 0)
	{
		ds_writeb(TEVENT020_FLAG, 1);

		sprintf(g_dtp2,
			get_tx2(49),
			(char*)hero + HERO_NAME2);

		do {
			answer = GUI_radio(g_dtp2, 2,
						get_tx2(50),
						get_tx2(51));
		} while (answer == -1);

		if (answer == 1)
		{
			do {
				answer = GUI_radio(get_tx2(52), 2,
							get_tx2(53),
							get_tx2(54));
			} while (answer == -1);

			if (answer == 1)
			{
			    /* TODO: Original-Bug: CURRENT_TOWN is either Kravik or Skelellen. */
				ds_writeb(ROUTE59_FLAG, (gs_current_town == TOWNS_PEILINEN ? 1 : 3));

				/* Original-Glitch:
				 * TRAVEL_DETOUR == 1 is indicating a detour to DNG_TOTENSCHIFF (which had the ID 1).
				 * Probably, this does not make a difference, but still, it would be better to use another number. */
				ds_writeb(TRAVEL_DETOUR, 1);
			}
		}

		timewarp(MINUTES(15));
	}
}

void tevent_021(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 8) > 0 && !ds_readb(TEVENT021_FLAG)) ||
		ds_readb(TEVENT021_FLAG) != 0)
	{
		ds_writeb(GATHER_HERBS_SPECIAL, 126);
		TRV_found_herb_place(1);
		ds_writeb(GATHER_HERBS_SPECIAL, -1);
		ds_writeb(TEVENT021_FLAG, 1);
	}
}

void tevent_022(void)
	/* Rukian <-> Faehrstation Angbodirtal: windgeschuetzte Stelle */
{
	signed short answer;

	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 0) > 0 && !ds_readb(TEVENT022_FLAG)) ||
		ds_readb(TEVENT022_FLAG) != 0)
	{
		ds_writeb(TEVENT022_FLAG, 1);
		if (TRV_found_camp_place(0))
		{
			if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 5) > 0 && !ds_readb(TEVENT022_TRACK_FLAG)) ||
				ds_readb(TEVENT022_TRACK_FLAG) != 0)
			{
				do {
					answer = GUI_radio(get_tx2(59), 2,
								get_tx2(60),
								get_tx2(61));
				} while (answer == -1);

				if (answer == 1)
				{
					TRV_hunt_generic(25, 62, -2, 2, 8, 3, 7, 2, 4, 30, 6);
					ds_writeb(TEVENT022_TRACK_FLAG, 1);
				}
			}
		}
	}
}

void tevent_024(void)
	/* Auplog <-> Vilnheim: windgeschuetzte Stelle. Idealer Rastplatz */
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT024_FLAG)) ||
		ds_readb(TEVENT024_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT024_FLAG, 1);
	}
}

void tevent_025(void)
	/* Auplog <-> Vilnheim: umgestuerzte, moosbewachsene Baumstaemme. idealer Rastplatz. */
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 1) > 0 && !ds_readb(TEVENT025_FLAG)) ||
		ds_readb(TEVENT025_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT025_FLAG, 1);
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
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT028_FLAG)) ||
		ds_readb(TEVENT028_FLAG) != 0)
	{
		TRV_found_camp_place(1);
		ds_writeb(TEVENT028_FLAG, 1);
	}
}

void tevent_029(void)
{
	signed short i;
	signed short item_pos;
	Bit8u *hero;

	load_in_head(51);

	GUI_dialog_na(0, get_tx2(74));
	GUI_dialog_na(0, get_tx2(75));

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
	{
		hero_disease_test(hero, 2, 20 - (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))));
	}

	if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 3) > 0)
	{
		/* skill test succeeded */
		timewarp(HOURS(3));

		GUI_dialog_na(0, get_tx2(76));
	} else {
		/* skill test failed */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
				!hero_dead(hero))
			{
				sub_hero_le(hero, 2);

				item_pos = get_item_pos(hero, ITEM_FOOD_PACKAGE);

				if (item_pos != -1) {
					/* hero looses the first set of FOOD PACKAGES */
					drop_item(hero, item_pos, host_readws(hero + HERO_INVENTORY + INVENTORY_QUANTITY + SIZEOF_INVENTORY * item_pos));
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
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 0) > 0 && !ds_readb(TEVENT030_FLAG)) ||
		ds_readb(TEVENT030_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT030_FLAG, 1);
	}
}

void tevent_031(void)
{
	signed short answer;

	if (ds_readb(FIND_HYGGELIK))
	{
		search_ruin1();

	} else if (!ds_readb(TEVENT031_FLAG))
	{
		load_in_head(49);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
						get_tx2(0), 2,
						get_tx2(1),
						get_tx2(2));
		} while (answer == -1);

		if (answer == 2)
		{
			if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_KRIEGSKUNST, 7) <= 0)
			{
				/* skill test failed */
				g_fig_initiative = 1;
				do_fight(FIGHTS_F031);
			}
		} else {
			do_fight(FIGHTS_F031);
		}

		ds_writeb(TEVENT031_FLAG, 1);
	}
}

void tevent_032(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT032_FLAG)) ||
		ds_readb(TEVENT032_FLAG) != 0)
	{
		ds_writeb(TEVENT032_FLAG, 1);
		if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 5) > 0 && !ds_readb(TEVENT032_HERB_FLAG)) ||
			ds_readb(TEVENT032_HERB_FLAG) != 0)
		{
			ds_writeb(TEVENT032_HERB_FLAG, 1);

			ds_writeb(GATHER_HERBS_SPECIAL, 62);
			TRV_found_camp_place(2);
			ds_writeb(GATHER_HERBS_SPECIAL, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_033(void)
{
	signed short answer;

	load_in_head(9);

	do {
		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
					get_tx2(3), 3,
					get_tx2(4),
					get_tx2(5),
					get_tx2(6));
	} while (answer == -1);

	if (answer == 1 || answer == 2)
	{
		timewarp(MINUTES(15));

		GUI_dialog_na(0, (answer == 1 ? get_tx2(7) : get_tx2(8)));
	}
}

void tevent_034(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT034_FLAG)) ||
		ds_readb(TEVENT034_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT034_FLAG, 1);
	}
}

void tevent_035(void)
{
	signed short answer;

	if (!ds_readb(TEVENT035_FLAG))
	{
		do {
			answer = GUI_radio(get_tx2(9), 2,
						get_tx2(10),
						get_tx2(11));
		} while (answer == -1);

		if (answer == 2)
		{
			if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_KRIEGSKUNST, 8) <= 0)
			{
				/* skill test failed */
				g_fig_initiative = 1;
				do_fight(FIGHTS_F035);
			}
		} else {
			do_fight(FIGHTS_F035);
		}

		ds_writeb(TEVENT035_FLAG, 1);
	}
}

void tevent_036(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT036_FLAG)) ||
		ds_readb(TEVENT036_FLAG) != 0)
	{
		ds_writeb(TEVENT036_FLAG, 1);

		if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 6) > 0 && !ds_readb(TEVENT036_HERB_FLAG)) ||
			ds_readb(TEVENT036_HERB_FLAG) != 0)
		{
			ds_writeb(TEVENT036_HERB_FLAG, 1);

			ds_writeb(GATHER_HERBS_SPECIAL, 63);
			TRV_found_camp_place(2);
			ds_writeb(GATHER_HERBS_SPECIAL, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_039(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 4) > 0 && !ds_readb(TEVENT039_FLAG)) ||
		ds_readb(TEVENT039_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT039_FLAG, 1);
	}
}

void tevent_040(void)
/* Auplog <-> Varnheim: alte Feuerstelle. idealer Rastplatz */
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 4) > 0 && !ds_readb(TEVENT040_FLAG)) ||
		ds_readb(TEVENT040_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT040_FLAG, 1);
	}
}

/* Auplog <-> Varnheim: kleiner aber reissender Bach, ueber den sich urspruenglich einmal eine Haengebruecke spannte. bridge building */
void tevent_041(void)
{
	signed short answer;

	TRV_barrier(12);

	if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 0) > 0 && !ds_readb(TEVENT041_FLAG))
	{
		do {
			/* Spuren von Karenen */
			answer = GUI_radio(get_tx2(20), 2, get_tx2(21), get_tx2(22));

		} while (answer == -1);

		if (answer == 1)
		{
			TRV_hunt_generic(21, 23, -2, 2, 8, 3, 7, 2, 4, 15, 8);

			ds_writeb(TEVENT041_FLAG, 1);
		}
	}
}

void tevent_042(void)
	/* Auplog <-> Varnheim: (weiter von Auplog weg als tevent_043) kleiner Bachlauf. Nahrung. */
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT042_FLAG)) ||
		ds_readb(TEVENT042_FLAG) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(TEVENT042_FLAG, 1);
	}
}

void tevent_043(void)
	/* Auplog <-> Varnheim: (nahe an Auplog) kleiner Bachlauf. Nahrung. */
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT043_FLAG)) ||
		ds_readb(TEVENT043_FLAG) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(TEVENT043_FLAG, 1);
	}
}

/* dying adventuress */
void tevent_044(void)
{
	signed short answer;
	Bit8u *hero;

	if (!ds_readb(TEVENT094_FLAG)) {

		ds_writeb(TEVENT094_FLAG, 1);

		load_in_head(1);

		hero = (Bit8u*)get_first_hero_available_in_group();

		sprintf((char*)(g_dtp2 + 0x400), get_tx2(33), (char*)hero + HERO_NAME2, (char*)GUI_get_ptr(host_readbs(hero + HERO_SEX), 3));

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
	signed short answer;

	if (ds_readw(TRV_DESTINATION) == TOWNS_DASPOTA)
	{
		load_in_head(11);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
						get_tx2(36), 3,
						get_tx2(37),
						get_tx2(38),
						get_tx2(39));
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
	signed short answer;
	signed short enter_inn;
	Bit8u *hero;

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
		ds_writew(CAMP_INCIDENT, 1);
		gs_current_loctype = LOCTYPE_WILDCAMP;
		do_location();
		gs_current_loctype = LOCTYPE_NONE;
		TRV_load_textfile(-1);

		hero = (ds_readws(CAMP_INCIDENT) != -1 ? get_hero(ds_readw(CAMP_INCIDENT)) : (Bit8u*)get_first_hero_available_in_group());
		ds_writew(CAMP_INCIDENT, -1);

		if (test_skill(hero, TA_SINNESSCHAERFE, 0) > 0)
		{
			sprintf(g_dtp2,
				get_tx2(45),
				(char*)hero + HERO_NAME2,
				(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

			do {
				answer = GUI_radio(g_dtp2, 2,
							get_tx2(46),
							get_tx2(47));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(TRAVEL_DETOUR, DUNGEONS_VERFALLENE_HERBERGE);
				enter_inn = 1;
			}
		}

		if (!enter_inn)
		{
			sprintf(g_dtp2,
				get_tx2(48),
				(char*)hero + HERO_NAME2);

			GUI_output(g_dtp2);

			g_fig_discard = 1;

			TRV_fight_event(FIGHTS_F046, 46);

			do {
				answer = GUI_radio(get_tx2(49), 2,
							get_tx2(50),
							get_tx2(51));
			} while (answer == -1);

			if (answer == 1)
			{
				ds_writeb(TRAVEL_DETOUR, DUNGEONS_VERFALLENE_HERBERGE);
			}
		}
	}

	set_var_to_zero();
	/* resume traveling */
	g_event_ani_busy = 0;
	ds_writew(REQUEST_REFRESH, 1);
}

/* Daspota <-> Ottarje: alte Feuerstelle. idealer Rastplatz. */
void tevent_048(void)
{
	signed short answer;

	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT048_FLAG)) ||
		ds_readb(TEVENT048_FLAG) != 0)
	{
		ds_writeb(TEVENT048_FLAG, 1);

		if (TRV_found_camp_place(0))
		{
			if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 3) > 0 && !ds_readb(TEVENT048_TRACK_FLAG)) ||
				ds_readb(TEVENT048_TRACK_FLAG) != 0)
			{
				/* Spuren einer kleinen Karenherde */
				do {
					answer = GUI_radio(get_tx2(52), 2,
								get_tx2(53),
								get_tx2(54));
				} while (answer == -1);

				if (answer == 1)
				{
					TRV_hunt_generic(21, 55, -2, 2, 8, 3, 5, 2, 4, 25, 12);
					ds_writeb(TEVENT048_TRACK_FLAG, 1);
				}
			}
		}
	}
}

/* Ottarje <-> Skjal */
void tevent_049(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT049_FLAG)) ||
		ds_readb(TEVENT049_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT049_FLAG, 1);
	}
}

/* Ottarje <-> Skjal */
void tevent_050(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !ds_readb(TEVENT050_FLAG)) ||
		ds_readb(TEVENT050_FLAG) != 0)
	{
		ds_writeb(TEVENT050_FLAG, 1);

		if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 4) > 0 && !ds_readb(TEVENT050_HERB_FLAG)) ||
			ds_readb(TEVENT050_HERB_FLAG) != 0)
		{
			TRV_found_replenish_place(ds_writeb(TEVENT050_HERB_FLAG, 1));
		} else {
			TRV_found_replenish_place(0);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
