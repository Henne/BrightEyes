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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static unsigned char g_towns_gossip_max[52] = { 0x1a, 0x0e, 0x0d, 0x05, 0x09, 0x06, 0x06, 0x07, 0x10, 0x07, 0x0b, 0x08, 0x0a, 0x0c, 0x11, 0x12, 0x0e, 0x10, 0x02, 0x0e, 0x00, 0x09, 0x0c, 0x0d, 0x0c, 0x0b, 0x07, 0x0a, 0x0c, 0x0a, 0x0c, 0x06, 0x0a, 0x09, 0x09, 0x08, 0x11, 0x0e, 0x0c, 0x00, 0x02, 0x07, 0x0f, 0x0b, 0x06, 0x0a, 0x08, 0x09, 0x0b, 0x0a, 0x00 }; // ds:0x7c9d

static unsigned char g_towns_gossip_off[52] = { 0x38, 0x06, 0x0a, 0x09, 0x0c, 0x06, 0x04, 0x06, 0x06, 0x0e, 0x05, 0x06, 0x08, 0x05, 0x0e, 0x04, 0x14, 0x1a, 0x05, 0x08, 0x00, 0x09, 0x0c, 0x09, 0x08, 0x0a, 0x05, 0x0d, 0x0c, 0x05, 0x0a, 0x06, 0x0e, 0x05, 0x07, 0x06, 0x0a, 0x0b, 0x18, 0x00, 0x05, 0x0c, 0x0c, 0x09, 0x08, 0x0c, 0x05, 0x0e, 0x04, 0x03, 0x00 }; // ds:0x7cd1

static const char g_gossip_str_triple_wildcard[8] = "%s %s%s"; // ds:0x7d06

/**
 * \brief   get gossip in taverns
 *
 *          This sets also some informers and camp grounds as known.
 *
 * \return              index of the gossip message
 */
unsigned short get_tavern_gossip(void)
{
	/* Orig-Bug: Informants can be reenabled if they had been set to 2 */
	unsigned short r_si;
	unsigned short r_di;

	r_di = g_towns_gossip_max[gs_town_id - 1];

	r_si = r_di = random_schick(r_di) - 1;

	switch (gs_town_id - 1) {
	/* Thorwal */
	case (TOWN_ID_THORWAL - 1): {
		if (r_si == 6)
			update_informer_cond(INFORMER_ELIANE);	/* Eliane Windenbeck */
		else if (r_si == 7)
			update_informer_cond(INFORMER_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Serske */
	case (TOWN_ID_SERSKE - 1): {
		if (r_si == 1)
			gs_tevent005_flag = 1;
		else if (r_si == 2)
			gs_tevent004_flag = 1;
		else if (r_si == 9)
			update_informer_cond(INFORMER_ELIANE);	/* Eliane Windenbeck */
		else if (r_si == 10)
			update_informer_cond(INFORMER_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Breida */
	case (TOWN_ID_BREIDA - 1): {
		if (r_si == 1)
			gs_tevent005_flag = 1;
		else if (r_si == 3)
			gs_tevent004_flag = 1;
		else if (r_si == 4)
			gs_tevent008_flag = 1;
		else if (r_si == 5)
			gs_tevent009_flag = 1;

		break;
	}
	/* Peilinen */
	case (TOWN_ID_PEILINEN - 1): {
		if (r_si == 1)
			gs_tevent009_flag = 1;
		else if (r_si == 3)
			gs_tevent011_flag = 1;

		break;
	}
	/* Rovamund */
	case (TOWN_ID_ROVAMUND - 1): {
		if (r_si == 1)
			gs_tevent013_flag = 1;
		else if (r_si == 3)
			gs_tevent011_flag = 1;
		else if (r_si == 4)
			gs_tevent119_flag = 1;
		else if (r_si == 5)
			gs_tevent120_flag = 1;

		break;
	}
	/* Nordvest */
	case (TOWN_ID_NORDVEST - 1): {
		if (r_si == 1)
			gs_tevent013_flag = 1;
		else if (r_si == 3)
			gs_tevent017_flag = 1;

		break;
	}
	/* Kravik */
	case (TOWN_ID_KRAVIK - 1): {
		if (r_si == 1)
			gs_tevent017_flag = 1;	/* Restplaces */
		else if (r_si == 2)
			gs_tevent020_flag = 1;
		else if (r_si == 3)
			gs_tevent021_flag = 1;

		break;
	}
	/* Skelellen */
	case (TOWN_ID_SKELELLEN - 1): {
		if (r_si == 1)
			gs_tevent021_flag = 1;	/* Restplaces */
		else if (r_si == 2)
			gs_teventu07_flag = 1;
		else if (r_si == 3)
			gs_tevent106_flag = 1;
		else if (r_si == 4) {
			gs_tevent112_flag = gs_tevent112_herb_flag = 1;
		}

		break;
	}
	/* Merske (has no tavern) */
	case (TOWN_ID_MERSKE - 1): {
		if (r_si == 1)
			gs_tevent118_flag = 1;
		else if (r_si == 2)
			gs_tevent119_flag = 1;
		else if (r_si == 3)
			gs_tevent120_flag = 1;
		else if (r_si == 11)
			update_informer_cond(INFORMER_ELIANE);	/* Eliane Windenbeck */
		else if (r_si == 12)
			update_informer_cond(INFORMER_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Tjoila */
	case (TOWN_ID_TJOILA - 1): {
		if (r_si == 2)
			gs_tevent008_flag = 1;
		else if (r_si == 7)
			update_informer_cond(INFORMER_ELIANE);	/* Eliane Windenbeck */
		else if (r_si == 8)
			update_informer_cond(INFORMER_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Rukian and Angbodirtal */
	case (TOWN_ID_RUKIAN - 1):
	case (TOWN_ID_ANGBODIRTAL - 1):
	 {
		if (r_si == 1)
			gs_tevent022_flag = 1;	/* Restplaces */
		break;
	}
	/* Auplog */
	case (TOWN_ID_AUPLOG - 1): {
		if (r_si == 1) {
			/* Restplaces */
			gs_tevent024_flag = gs_tevent025_flag = 1;
		}
		else if (r_si == 2)
			gs_tevent040_flag = 1;
		else if (r_si == 3) {
			gs_teventu02_flag = gs_tevent040_flag = 1;
		}

		break;
	}
	/* Vilnheim */
	case (TOWN_ID_VILNHEIM - 1): {
		if (r_si == 1) {
			/* Restplaces */
			gs_tevent024_flag = gs_tevent025_flag = 1;
		}
		else if (r_si == 2)
			gs_tevent028_flag = 1;
		else if (r_si == 3) {
			gs_tevent030_flag = 1;
		}
		else if (r_si == 4) {
			gs_tevent032_flag = gs_tevent032_herb_flag = 1;
		}
		else if (r_si == 5) {
			gs_tevent034_flag = 1;
		}

		break;
	}
	/* Bodon */
	case (TOWN_ID_BODON - 1): {
		if (r_si == 3) {
			/* Restplaces */
			gs_tevent024_flag = gs_tevent025_flag = 1;
		}
		else if (r_si == 4)
			gs_tevent028_flag = 1;
		else if (r_si == 5) {
			gs_tevent030_flag = 1;
		}
		else if (r_si == 6) {
			gs_tevent032_flag = gs_tevent032_herb_flag = 1;
		}
		else if (r_si == 7) {
			gs_tevent034_flag = 1;
		}

		break;
	}
	/* Oberorken */
	case (TOWN_ID_OBERORKEN - 1): {
		if (r_si == 2)
			gs_tevent072_flag = 1;	/* Restplaces */
		else if (r_si == 3)
			gs_tevent083_flag = 1;

		break;
	}
	/* Phexcaer */
	case (TOWN_ID_PHEXCAER - 1): {
		if (r_si == 1)
			gs_tevent034_flag = 1;	/* Restplaces */
		else if (r_si == 2) {
			gs_tevent032_flag = gs_tevent032_herb_flag = 1;
		}
		else if (r_si == 3)
			gs_tevent030_flag = 1;
		else if (r_si == 4)
			gs_tevent028_flag = 1;
		else if (r_si == 5)
			gs_tevent036_flag = 1;
		else if (r_si == 6)
			gs_teventu07_flag = 1;
		else if (r_si == 7)
			gs_tevent116_flag = 1;

		break;
	}
	/* Felsteyn */
	case (TOWN_ID_FELSTEYN - 1): {
		if (r_si == 1)
			gs_tevent072_flag = 1;	/* Restplaces */
		else if (r_si == 2)
			gs_tevent075_flag = 1;
		else if (r_si == 3)
			gs_tevent091_flag = 1;
		else if (r_si == 4)
			gs_tevent093_flag = 1;

		break;
	}
	/* Orkanger */
	case (TOWN_ID_ORKANGER - 1): {
		if (r_si == 1)
			gs_tevent075_flag = 1;	/* Restplaces */
		else if (r_si == 2)
			gs_tevent079_flag = 1;

		break;
	}
	/* Clanegh */
	case (TOWN_ID_CLANEGH - 1): {
		if (r_si == 1)
			gs_tevent079_flag = 1;	/* Restplaces */
		else if (r_si == 2)
			gs_tevent081_flag = 1;

		break;
	}
	/* Thoss */
	case (TOWN_ID_THOSS - 1): {
		if (r_si == 1) {
			/* Restplaces */
			gs_tevent069_flag = gs_tevent070_flag = gs_tevent070_herb_flag = 1;
		}
		else if (r_si == 2)
			gs_teventu08_flag = 1;

		break;
	}
	/* Ala */
	case (TOWN_ID_ALA - 1): {
		if (r_si == 1)
			gs_tevent066_track_flag = 1;	/* Restplaces */
		else if (r_si == 2) {
			gs_tevent069_flag = gs_tevent070_flag = gs_tevent070_herb_flag = 1;
		}

		break;
	}
	/* Orvil */
	case (TOWN_ID_ORVIL - 1): {
		if (r_si == 1) {
			/* Restplaces */
			gs_tevent058_flag = gs_teventu03_flag = 1;
		}
		else if (r_si == 2)
			gs_tevent066_track_flag = 1;
		else if (r_si == 3) {
			/* Restplaces */
			gs_teventu10_flag = gs_teventu11_flag = 1;
		}
		else if (r_si == 4)
			gs_teventu12_flag = 1;

		break;
	}
	/* Rovik */
	case (TOWN_ID_ROVIK - 1): {
		if (r_si == 1)
			gs_teventu12_flag = 1; /* Restplaces */

		break;
	}
	/* Kord */
	case (TOWN_ID_KORD - 1): {
		if (r_si == 1)
			gs_teventu05_flag = 1;	/* Restplaces */

		break;
	}
	/* Runinshaven */
	case (TOWN_ID_RUNINSHAVEN - 1): {
		if (r_si == 1) {
			/* Restplaces */
			gs_tevent128_flag = gs_teventu09_flag = 1;
		}

		break;
	}

	/* Ottarje */
	case (TOWN_ID_OTTARJE - 1): {
		if (r_si == 1)
			gs_teventu04_flag = 1;	/* Restplaces */
		else if (r_si == 2)
			gs_tevent048_flag = 1;
		else if (r_si == 3) {
			gs_tevent049_flag = gs_tevent053_flag = 1;
		}
		else if (r_si == 4)
			gs_tevent050_flag = 1;
		else if (r_si == 5) {
			/* Yes this was done twice */
			gs_tevent058_flag = gs_tevent058_flag = 1;
		}

		break;
	}
	/* Skjal */
	case (TOWN_ID_SKJAL - 1): {
		/* Restplaces */
		if (r_si == 1)
			gs_tevent049_flag = gs_tevent053_flag = 1;
		else if (r_si == 2)
			gs_tevent050_flag = 1;
		else if (r_si == 3)
			gs_teventu06_flag = 1;
		else if (r_si == 4) {
			gs_teventu10_flag = gs_teventu11_flag = 1;
		}

		break;
	}
	/* Prem */
	case (TOWN_ID_PREM - 1): {
		if (r_si == 2)
			gs_teventu06_flag = 1;	/* Restplaces */
		else if (r_si == 3)
			gs_teventu05_flag = 1;

		break;
	}
	/* Rybon */
	case (TOWN_ID_RYBON - 1): {
		if (r_si == 0)
			gs_teventu08_flag = 1;	/* Restplaces */

		break;
	}
	/* Varnheim */
	case (TOWN_ID_VARNHEIM - 1): {
		if (r_si == 2)
			gs_tevent040_flag = 1;	/* Restplaces */
		else if (r_si == 3) {
			/* hunt and Restplace */
			gs_teventu02_flag = gs_tevent040_flag = 1;
		}
		else if (r_si == 9)
			update_informer_cond(INFORMER_ELIANE);	/* Eliane Windenbeck */
		else if (r_si == 10)
			update_informer_cond(INFORMER_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Vaermhag */
	case (TOWN_ID_VAERMHAG - 1): {
		if (r_si == 7)
			update_informer_cond(INFORMER_ELIANE);	/* Eliane Windenbeck */
		else if (r_si == 8)
			update_informer_cond(INFORMER_ASGRIMM);	/* Asgrimm Thurboldsson */

		break;
	}
	/* Tyldon */
	case (TOWN_ID_TYLDON - 1): {
		if (r_si == 1)
			gs_tevent081_flag = 1;	/* Restplaces */

		break;
	}
	/* Brendhil */
	case (TOWN_ID_BRENDHIL - 1): {
		if (r_si == 1)
			gs_teventu13_flag = 1;	/* Restplaces */

		break;
	}
	/* Manrin */
	case (TOWN_ID_MANRIN - 1): {
		if (r_si == 1)
			gs_teventu13_flag = 1;	/* Restplaces */
		else if (r_si == 2)
			gs_tevent132_herb_flag = 1;

		break;
	}
	}

	return r_di + g_towns_gossip_off[gs_town_id - 1];

}

/**
 * \brief   generates the name of a drinkmate and copies it to a buffer
 */
char* get_drinkmate(void)
{
	signed short name;
	signed short surname;

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
signed short tavern_quest_infos(void)
{
	signed short l_si; /* TODO: should be initialized with 0 */

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

	} else if ((gs_town_id == TOWN_ID_THORWAL) && !gs_quest_deadship && (gs_current_typeindex == 6))
	{
		/* print the message about a ghost ship */
		GUI_output(get_tx(209));

		/* remember that */
		gs_quest_deadship = l_si = 1;

	} else if ((gs_town_id == TOWN_ID_SKJAL) &&	(gs_current_typeindex == 69) &&
			(gs_informer_flags[INFORMER_JURGE] != 2) && (gs_informer_flags[INFORMER_JURGE] != 0) &&
			!gs_jurge_awaits_letter)
	{
		/* meet Informer Jurge */

		if (random_schick(100) <= 30) {
			gs_current_informer = l_si = 1;
		}

	} else if ((gs_town_id == TOWN_ID_VIDSAND) && (gs_current_typeindex == 84) &&
			(gs_informer_flags[INFORMER_RAGNA] != 2) && (gs_informer_flags[INFORMER_RAGNA] != 0))
	{
		/* meet Informer Ragna */

		if (random_schick(100) <= 30) {
			gs_current_informer = l_si = 6;
		}

	} else if ((gs_town_id == TOWN_ID_ANGBODIRTAL) && ((gs_current_typeindex == 27) || (gs_current_typeindex == 28)) &&
			(gs_informer_flags[INFORMER_BEORN] != 2) && (gs_informer_flags[INFORMER_BEORN] != 0))
	{
		/* meet Informer Beorn */

		if (((gs_current_typeindex == 27) && (random_schick(100) <= 50)) ||
			((gs_current_typeindex == 28) && (random_schick(100) <= 20)))
		{
			gs_current_informer = l_si = 7;
		}


	} else if ((gs_town_id == TOWN_ID_BREIDA) && ((gs_current_typeindex == 14) || (gs_current_typeindex == 15)) &&
			(gs_informer_flags[INFORMER_ASGRIMM] != 2) &&
			(gs_informer_flags[INFORMER_ASGRIMM] != 0) &&
			(gs_got_main_quest != 0))
	{
		/* meet Informer Asgrimm */

		if (random_schick(100) <= 50) {
			gs_current_informer = l_si = 8;
		}

	} else if ((gs_town_id == TOWN_ID_HJALSINGOR) &&
			((gs_current_typeindex == 61) || (gs_current_typeindex == 62)) &&
			(gs_informer_flags[INFORMER_ALGRID] != 2) &&
			(gs_informer_flags[INFORMER_ALGRID] != 0))
	{
		/* meet Informer Algrid */

		if (((gs_current_typeindex == 61) && (random_schick(100) <= 50)) ||
			((gs_current_typeindex == 62) && (random_schick(100) <= 20)))
		{
			gs_current_informer = l_si = 14;
		}

	} else if ((gs_town_id == TOWN_ID_PHEXCAER) && gs_alrik_derondan) {

		/* meet Alrik Derondan */
		PHX_alrik_derondan();

		/* Original-Bug: which return value here? */
	}

	return l_si ? 1 : 0;
}

/**
 * \brief   follow a informer from the tavern to his/her home
 */
void tavern_follow_informer(void)
{

	switch (get_town_lookup_entry() + 1) {
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

#if !defined(__BORLANDC__)
}
#endif
