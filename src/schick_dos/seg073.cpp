/*
 *	Rewrite of DSA1 v3.02_de functions of seg073 (tavern: gossip, drinkmates, quests & informers)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg073.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg007.h"
#include "seg030.h"
#include "seg031.h"
#include "seg071.h"
#include "seg097.h"

static const uint8_t g_town_gossip_max[TOWN_ID__TAIL] = {
/* note that the index is "off by 1" compared to TOWN_ID_... */
	26, // TOWN_ID_THORWAL
	14, // TOWN_ID_SERSKE
	13, // TOWN_ID_BREIDA
	 5, // TOWN_ID_PEILINEN
	 9, // TOWN_ID_ROVAMUND
	 6, // TOWN_ID_NORDVEST
	 6, // TOWN_ID_KRAVIK
	 7, // TOWN_ID_SKELELLEN
	16, // TOWN_ID_MERSKE
	 7, // TOWN_ID_EFFERDUN
	11, // TOWN_ID_TJOILA
	 8, // TOWN_ID_RUKIAN
	10, // TOWN_ID_ANGBODIRTAL
	12, // TOWN_ID_AUPLOG
	17, // TOWN_ID_VILNHEIM
	18, // TOWN_ID_BODON
	14, // TOWN_ID_OBERORKEN
	16, // TOWN_ID_PHEXCAER
	 2, // TOWN_ID_GROENVELDEN
	14, // TOWN_ID_FELSTEYN
	 0, // TOWN_ID_EINSIEDLERSEE
	 9, // TOWN_ID_ORKANGER
	12, // TOWN_ID_CLANEGH
	13, // TOWN_ID_LISKOR
	12, // TOWN_ID_THOSS
	11, // TOWN_ID_TJANSET
	 7, // TOWN_ID_ALA
	10, // TOWN_ID_ORVIL
	12, // TOWN_ID_OVERTHORN
	10, // TOWN_ID_ROVIK
	12, // TOWN_ID_HJALSINGOR
	 6, // TOWN_ID_GUDDASUNDEN
	10, // TOWN_ID_KORD
	 9, // TOWN_ID_TREBAN
	 9, // TOWN_ID_ARYN
	 8, // TOWN_ID_RUNINSHAVEN
	17, // TOWN_ID_OTTARJE
	14, // TOWN_ID_SKJAL
	12, // TOWN_ID_PREM
	 0, // TOWN_ID_DASPOTA
	 2, // TOWN_ID_RYBON
	 7, // TOWN_ID_LJASDAHL
	15, // TOWN_ID_VARNHEIM
	11, // TOWN_ID_VAERMHAG
	 6, // TOWN_ID_TYLDON
	10, // TOWN_ID_VIDSAND
	 8, // TOWN_ID_BRENDHIL
	 9, // TOWN_ID_MANRIN
	11, // TOWN_ID_FAEHRSTATION_TJOILA
	10, // TOWN_ID_FAEHRE_ANGBODIRTAL
	 0, // TOWN_ID_HJALLANDER_HOF
	 0  // TOWN_ID_LEUCHTTURM_RUNIN
}; // ds:0x7c9d

static const uint8_t g_town_gossip_off[TOWN_ID__TAIL] = {
/* note that the index is "off by 1" compared to TOWN_ID_... */
	56, // TOWN_ID_THORWAL
	 6, // TOWN_ID_SERSKE
	10, // TOWN_ID_BREIDA
	 9, // TOWN_ID_PEILINEN
	12, // TOWN_ID_ROVAMUND
	 6, // TOWN_ID_NORDVEST
	 4, // TOWN_ID_KRAVIK
	 6, // TOWN_ID_SKELELLEN
	 6, // TOWN_ID_MERSKE
	14, // TOWN_ID_EFFERDUN
	 5, // TOWN_ID_TJOILA
	 6, // TOWN_ID_RUKIAN
	 8, // TOWN_ID_ANGBODIRTAL
	 5, // TOWN_ID_AUPLOG
	14, // TOWN_ID_VILNHEIM
	 4, // TOWN_ID_BODON
	20, // TOWN_ID_OBERORKEN
	26, // TOWN_ID_PHEXCAER
	 5, // TOWN_ID_GROENVELDEN
	 8, // TOWN_ID_FELSTEYN
	 0, // TOWN_ID_EINSIEDLERSEE
	 9, // TOWN_ID_ORKANGER
	12, // TOWN_ID_CLANEGH
	 9, // TOWN_ID_LISKOR
	 8, // TOWN_ID_THOSS
	10, // TOWN_ID_TJANSET
	 5, // TOWN_ID_ALA
	13, // TOWN_ID_ORVIL
	12, // TOWN_ID_OVERTHORN
	 5, // TOWN_ID_ROVIK
	10, // TOWN_ID_HJALSINGOR
	 6, // TOWN_ID_GUDDASUNDEN
	14, // TOWN_ID_KORD
	 5, // TOWN_ID_TREBAN
	 7, // TOWN_ID_ARYN
	 6, // TOWN_ID_RUNINSHAVEN
	10, // TOWN_ID_OTTARJE
	11, // TOWN_ID_SKJAL
	24, // TOWN_ID_PREM
	 0, // TOWN_ID_DASPOTA
	 5, // TOWN_ID_RYBON
	12, // TOWN_ID_LJASDAHL
	12, // TOWN_ID_VARNHEIM
	 9, // TOWN_ID_VAERMHAG
	 8, // TOWN_ID_TYLDON
	12, // TOWN_ID_VIDSAND
	 5, // TOWN_ID_BRENDHIL
	14, // TOWN_ID_MANRIN
	 4, // TOWN_ID_FAEHRSTATION_TJOILA
	 3, // TOWN_ID_FAEHRE_ANGBODIRTAL
	 0, // TOWN_ID_HJALLANDER_HOF
	 0  // TOWN_ID_LEUCHTTURM_RUNIN
}; // ds:0x7cd1

static const char g_gossip_str_triple_wildcard[8] = "%s %s%s"; // ds:0x7d06

/**
 * \brief   get gossip in taverns
 *
 *          This sets also some informers and camp grounds as known.
 *
 * \return              index of the gossip message
 */
signed int get_tavern_gossip(void)
{
	/* Orig-Bug: Informants can be reenabled if they had been set to 2 */
	signed int randval;
	signed int max_gossip_texts;

	max_gossip_texts = g_town_gossip_max[gs_town_id - 1];

	randval = max_gossip_texts = random_schick(max_gossip_texts) - 1;

	switch (gs_town_id - 1) {
	/* Thorwal */
	case (TOWN_ID_THORWAL - 1): {
		if (randval == 6)
			update_informer_cond(INFORMER_ID_ELIANE);	/* Eliane Windenbeck */
		else if (randval == 7)
			update_informer_cond(INFORMER_ID_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Serske */
	case (TOWN_ID_SERSKE - 1): {
		if (randval == 1)
			gs_tevent005_flag = 1;
		else if (randval == 2)
			gs_tevent004_flag = 1;
		else if (randval == 9)
			update_informer_cond(INFORMER_ID_ELIANE);	/* Eliane Windenbeck */
		else if (randval == 10)
			update_informer_cond(INFORMER_ID_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Breida */
	case (TOWN_ID_BREIDA - 1): {
		if (randval == 1)
			gs_tevent005_flag = 1;
		else if (randval == 3)
			gs_tevent004_flag = 1;
		else if (randval == 4)
			gs_tevent008_flag = 1;
		else if (randval == 5)
			gs_tevent009_flag = 1;

		break;
	}
	/* Peilinen */
	case (TOWN_ID_PEILINEN - 1): {
		if (randval == 1)
			gs_tevent009_flag = 1;
		else if (randval == 3)
			gs_tevent011_flag = 1;

		break;
	}
	/* Rovamund */
	case (TOWN_ID_ROVAMUND - 1): {
		if (randval == 1)
			gs_tevent013_flag = 1;
		else if (randval == 3)
			gs_tevent011_flag = 1;
		else if (randval == 4)
			gs_tevent119_flag = 1;
		else if (randval == 5)
			gs_tevent120_flag = 1;

		break;
	}
	/* Nordvest */
	case (TOWN_ID_NORDVEST - 1): {
		if (randval == 1)
			gs_tevent013_flag = 1;
		else if (randval == 3)
			gs_tevent017_flag = 1;

		break;
	}
	/* Kravik */
	case (TOWN_ID_KRAVIK - 1): {
		if (randval == 1)
			gs_tevent017_flag = 1;	/* Restplaces */
		else if (randval == 2)
			gs_tevent020_flag = 1;
		else if (randval == 3)
			gs_tevent021_flag = 1;

		break;
	}
	/* Skelellen */
	case (TOWN_ID_SKELELLEN - 1): {
		if (randval == 1)
			gs_tevent021_flag = 1;	/* Restplaces */
		else if (randval == 2)
			gs_teventu07_flag = 1;
		else if (randval == 3)
			gs_tevent106_flag = 1;
		else if (randval == 4) {
			gs_tevent112_flag = gs_tevent112_herb_flag = 1;
		}

		break;
	}
	/* Merske (has no tavern) */
	case (TOWN_ID_MERSKE - 1): {
		if (randval == 1)
			gs_tevent118_flag = 1;
		else if (randval == 2)
			gs_tevent119_flag = 1;
		else if (randval == 3)
			gs_tevent120_flag = 1;
		else if (randval == 11)
			update_informer_cond(INFORMER_ID_ELIANE);	/* Eliane Windenbeck */
		else if (randval == 12)
			update_informer_cond(INFORMER_ID_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Tjoila */
	case (TOWN_ID_TJOILA - 1): {
		if (randval == 2)
			gs_tevent008_flag = 1;
		else if (randval == 7)
			update_informer_cond(INFORMER_ID_ELIANE);	/* Eliane Windenbeck */
		else if (randval == 8)
			update_informer_cond(INFORMER_ID_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Rukian and Angbodirtal */
	case (TOWN_ID_RUKIAN - 1):
	case (TOWN_ID_ANGBODIRTAL - 1):
	 {
		if (randval == 1)
			gs_tevent022_flag = 1;	/* Restplaces */
		break;
	}
	/* Auplog */
	case (TOWN_ID_AUPLOG - 1): {
		if (randval == 1) {
			/* Restplaces */
			gs_tevent024_flag = gs_tevent025_flag = 1;
		}
		else if (randval == 2)
			gs_tevent040_flag = 1;
		else if (randval == 3) {
			gs_teventu02_flag = gs_tevent040_flag = 1;
		}

		break;
	}
	/* Vilnheim */
	case (TOWN_ID_VILNHEIM - 1): {
		if (randval == 1) {
			/* Restplaces */
			gs_tevent024_flag = gs_tevent025_flag = 1;
		}
		else if (randval == 2)
			gs_tevent028_flag = 1;
		else if (randval == 3) {
			gs_tevent030_flag = 1;
		}
		else if (randval == 4) {
			gs_tevent032_flag = gs_tevent032_herb_flag = 1;
		}
		else if (randval == 5) {
			gs_tevent034_flag = 1;
		}

		break;
	}
	/* Bodon */
	case (TOWN_ID_BODON - 1): {
		if (randval == 3) {
			/* Restplaces */
			gs_tevent024_flag = gs_tevent025_flag = 1;
		}
		else if (randval == 4)
			gs_tevent028_flag = 1;
		else if (randval == 5) {
			gs_tevent030_flag = 1;
		}
		else if (randval == 6) {
			gs_tevent032_flag = gs_tevent032_herb_flag = 1;
		}
		else if (randval == 7) {
			gs_tevent034_flag = 1;
		}

		break;
	}
	/* Oberorken */
	case (TOWN_ID_OBERORKEN - 1): {
		if (randval == 2)
			gs_tevent072_flag = 1;	/* Restplaces */
		else if (randval == 3)
			gs_tevent083_flag = 1;

		break;
	}
	/* Phexcaer */
	case (TOWN_ID_PHEXCAER - 1): {
		if (randval == 1)
			gs_tevent034_flag = 1;	/* Restplaces */
		else if (randval == 2) {
			gs_tevent032_flag = gs_tevent032_herb_flag = 1;
		}
		else if (randval == 3)
			gs_tevent030_flag = 1;
		else if (randval == 4)
			gs_tevent028_flag = 1;
		else if (randval == 5)
			gs_tevent036_flag = 1;
		else if (randval == 6)
			gs_teventu07_flag = 1;
		else if (randval == 7)
			gs_tevent116_flag = 1;

		break;
	}
	/* Felsteyn */
	case (TOWN_ID_FELSTEYN - 1): {
		if (randval == 1)
			gs_tevent072_flag = 1;	/* Restplaces */
		else if (randval == 2)
			gs_tevent075_flag = 1;
		else if (randval == 3)
			gs_tevent091_flag = 1;
		else if (randval == 4)
			gs_tevent093_flag = 1;

		break;
	}
	/* Orkanger */
	case (TOWN_ID_ORKANGER - 1): {
		if (randval == 1)
			gs_tevent075_flag = 1;	/* Restplaces */
		else if (randval == 2)
			gs_tevent079_flag = 1;

		break;
	}
	/* Clanegh */
	case (TOWN_ID_CLANEGH - 1): {
		if (randval == 1)
			gs_tevent079_flag = 1;	/* Restplaces */
		else if (randval == 2)
			gs_tevent081_flag = 1;

		break;
	}
	/* Thoss */
	case (TOWN_ID_THOSS - 1): {
		if (randval == 1) {
			/* Restplaces */
			gs_tevent069_flag = gs_tevent070_flag = gs_tevent070_herb_flag = 1;
		}
		else if (randval == 2)
			gs_teventu08_flag = 1;

		break;
	}
	/* Ala */
	case (TOWN_ID_ALA - 1): {
		if (randval == 1)
			gs_tevent066_track_flag = 1;	/* Restplaces */
		else if (randval == 2) {
			gs_tevent069_flag = gs_tevent070_flag = gs_tevent070_herb_flag = 1;
		}

		break;
	}
	/* Orvil */
	case (TOWN_ID_ORVIL - 1): {
		if (randval == 1) {
			/* Restplaces */
			gs_tevent058_flag = gs_teventu03_flag = 1;
		}
		else if (randval == 2)
			gs_tevent066_track_flag = 1;
		else if (randval == 3) {
			/* Restplaces */
			gs_teventu10_flag = gs_teventu11_flag = 1;
		}
		else if (randval == 4)
			gs_teventu12_flag = 1;

		break;
	}
	/* Rovik */
	case (TOWN_ID_ROVIK - 1): {
		if (randval == 1)
			gs_teventu12_flag = 1; /* Restplaces */

		break;
	}
	/* Kord */
	case (TOWN_ID_KORD - 1): {
		if (randval == 1)
			gs_teventu05_flag = 1;	/* Restplaces */

		break;
	}
	/* Runinshaven */
	case (TOWN_ID_RUNINSHAVEN - 1): {
		if (randval == 1) {
			/* Restplaces */
			gs_tevent128_flag = gs_teventu09_flag = 1;
		}

		break;
	}

	/* Ottarje */
	case (TOWN_ID_OTTARJE - 1): {
		if (randval == 1)
			gs_teventu04_flag = 1;	/* Restplaces */
		else if (randval == 2)
			gs_tevent048_flag = 1;
		else if (randval == 3) {
			gs_tevent049_flag = gs_tevent053_flag = 1;
		}
		else if (randval == 4)
			gs_tevent050_flag = 1;
		else if (randval == 5) {
			/* Yes this was done twice */
			gs_tevent058_flag = gs_tevent058_flag = 1;
		}

		break;
	}
	/* Skjal */
	case (TOWN_ID_SKJAL - 1): {
		/* Restplaces */
		if (randval == 1)
			gs_tevent049_flag = gs_tevent053_flag = 1;
		else if (randval == 2)
			gs_tevent050_flag = 1;
		else if (randval == 3)
			gs_teventu06_flag = 1;
		else if (randval == 4) {
			gs_teventu10_flag = gs_teventu11_flag = 1;
		}

		break;
	}
	/* Prem */
	case (TOWN_ID_PREM - 1): {
		if (randval == 2)
			gs_teventu06_flag = 1;	/* Restplaces */
		else if (randval == 3)
			gs_teventu05_flag = 1;

		break;
	}
	/* Rybon */
	case (TOWN_ID_RYBON - 1): {
		if (randval == 0)
			gs_teventu08_flag = 1;	/* Restplaces */

		break;
	}
	/* Varnheim */
	case (TOWN_ID_VARNHEIM - 1): {
		if (randval == 2)
			gs_tevent040_flag = 1;	/* Restplaces */
		else if (randval == 3) {
			/* hunt and Restplace */
			gs_teventu02_flag = gs_tevent040_flag = 1;
		}
		else if (randval == 9)
			update_informer_cond(INFORMER_ID_ELIANE);	/* Eliane Windenbeck */
		else if (randval == 10)
			update_informer_cond(INFORMER_ID_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Vaermhag */
	case (TOWN_ID_VAERMHAG - 1): {
		if (randval == 7)
			update_informer_cond(INFORMER_ID_ELIANE);	/* Eliane Windenbeck */
		else if (randval == 8)
			update_informer_cond(INFORMER_ID_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Tyldon */
	case (TOWN_ID_TYLDON - 1): {
		if (randval == 1)
			gs_tevent081_flag = 1;	/* Restplaces */

		break;
	}
	/* Brendhil */
	case (TOWN_ID_BRENDHIL - 1): {
		if (randval == 1)
			gs_teventu13_flag = 1;	/* Restplaces */

		break;
	}
	/* Manrin */
	case (TOWN_ID_MANRIN - 1): {
		if (randval == 1)
			gs_teventu13_flag = 1;	/* Restplaces */
		else if (randval == 2)
			gs_tevent132_herb_flag = 1;

		break;
	}
	}

	return max_gossip_texts + g_town_gossip_off[gs_town_id - 1];
}

/**
 * \brief   generates the name of a drinkmate and copies it to a buffer
 */
char* get_drinkmate(void)
{
	signed int name;
	signed int surname;

	name = ((gs_tlk_tav_informersex - 1) == 0 ? 0xa7 : 0xbb);
	name += random_schick(20) - 1;

	surname = random_schick(2) - 1;

	surname = (surname == 0 ? 0xa7 : 0xbb);
	surname += random_schick(20) - 1;

	sprintf(g_text_output_buf, g_gossip_str_triple_wildcard, get_tx(name), get_tx(surname),
		get_tx((gs_tlk_tav_informersex - 1) == 0 ? 207 : 208));

	return g_text_output_buf;
}

/**
 * \brief   get quest information in taverns / meet informants
 *
 * \return              TODO {0, 1}
 */
signed int tavern_quest_infos(void)
{
	signed int relevant_info; /* TODO: should be initialized with 0 */

	if (!gs_heard_announce) {

		/* print the announcement from the hetmann */
		GUI_output(get_tx(210));

		/* remember you heard the announcement */
		gs_heard_announce = 1;

		/* calculate the day in a week */
		/* not used in the game */
		/* TODO: Does this work with the nameless days ? */
		gs_announce_day = gs_day_of_month + 7;

		if (gs_announce_day > 30) {

			gs_announce_day -= 30;
		}

		/* Original-Bug: which return value here? */

	} else if ((gs_town_id == TOWN_ID_THORWAL) && !gs_quest_deadship && (gs_town_typeindex == 6))
	{
		/* print the message about a ghost ship */
		GUI_output(get_tx(209));

		/* remember that */
		gs_quest_deadship = relevant_info = 1;

	} else if ((gs_town_id == TOWN_ID_SKJAL) &&	(gs_town_typeindex == 69) &&
			(gs_informer_flags[INFORMER_ID_JURGE] != 2) && (gs_informer_flags[INFORMER_ID_JURGE] != 0) &&
			!gs_jurge_awaits_letter)
	{
		/* meet Informer Jurge */

		if (random_schick(100) <= 30) {
			gs_current_informer = relevant_info = 1;
		}

	} else if ((gs_town_id == TOWN_ID_VIDSAND) && (gs_town_typeindex == 84) &&
			(gs_informer_flags[INFORMER_ID_RAGNA] != 2) && (gs_informer_flags[INFORMER_ID_RAGNA] != 0))
	{
		/* meet Informer Ragna */

		if (random_schick(100) <= 30) {
			gs_current_informer = relevant_info = 6;
		}

	} else if ((gs_town_id == TOWN_ID_ANGBODIRTAL) && ((gs_town_typeindex == 27) || (gs_town_typeindex == 28)) &&
			(gs_informer_flags[INFORMER_ID_BEORN] != 2) && (gs_informer_flags[INFORMER_ID_BEORN] != 0))
	{
		/* meet Informer Beorn */

		if (((gs_town_typeindex == 27) && (random_schick(100) <= 50)) ||
			((gs_town_typeindex == 28) && (random_schick(100) <= 20)))
		{
			gs_current_informer = relevant_info = 7;
		}


	} else if ((gs_town_id == TOWN_ID_BREIDA) && ((gs_town_typeindex == 14) || (gs_town_typeindex == 15)) &&
			(gs_informer_flags[INFORMER_ID_ASGRIMM] != 2) &&
			(gs_informer_flags[INFORMER_ID_ASGRIMM] != 0) &&
			(gs_got_main_quest != 0))
	{
		/* meet Informer Asgrimm */

		if (random_schick(100) <= 50) {
			gs_current_informer = relevant_info = 8;
		}

	} else if ((gs_town_id == TOWN_ID_HJALSINGOR) &&
			((gs_town_typeindex == 61) || (gs_town_typeindex == 62)) &&
			(gs_informer_flags[INFORMER_ID_ALGRID] != 2) &&
			(gs_informer_flags[INFORMER_ID_ALGRID] != 0))
	{
		/* meet Informer Algrid */

		if (((gs_town_typeindex == 61) && (random_schick(100) <= 50)) ||
			((gs_town_typeindex == 62) && (random_schick(100) <= 20)))
		{
			gs_current_informer = relevant_info = 14;
		}

	} else if ((gs_town_id == TOWN_ID_PHEXCAER) && gs_alrik_derondan) {

		/* meet Alrik Derondan */
		PHX_alrik_derondan();

		/* Original-Bug: which return value here? */
	}

	return relevant_info ? 1 : 0;
}

/**
 * \brief   follow an informer from the tavern to his/her home
 */
void tavern_follow_informer(void)
{

	switch (informer_id_from_current_town() + 1) {
	case 1: {
		do_talk(6, 0);
		gs_x_target_bak = 8;
		gs_y_target_bak = 6;
		gs_direction = 3;
		break;
	}
	case 6: {
		do_talk(8, 0);
		gs_x_target_bak = 5;
		gs_y_target_bak = 4;
		gs_direction = 3;
		break;
	}
	case 7: {
		do_talk(8, 1);
		gs_x_target_bak = 12;
		gs_y_target_bak = 10;
		gs_direction = 1;
		break;
	}
	case 8: {
		do_talk(10, 1);
		gs_x_target_bak = gs_y_target_bak = 7;
		gs_direction = 1;
		break;
	}
	case 14: {
		do_talk(8, 2);
		gs_x_target_bak = 5;
		gs_y_target_bak = 8;
		gs_direction = 0;
		break;
	}
	}
}
