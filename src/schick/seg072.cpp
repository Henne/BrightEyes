/**
 *	Rewrite of DSA1 v3.02_de functions of seg072 (informer scripting)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg072.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg047.h"
#include "seg055.h"
#include "seg072.h"
#include "seg095.h"
#include "seg103.h"
#include "seg105.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   dialog logic for the informers hjore and jurge
 *
 * \param   informer    0 = jurge, 1 = hjore
 * \param   state       state of the dialog
 */
void INF_jurge_hjore(signed short informer, signed short state)
{

	if (!informer) {
		/* JURGE TORFINSSON */

		if (!state) {
			g_dialog_next_state = ((gs_informer_flags[INFORMER_JURGE] == 1 || ds_readb(TLK_TAV_FOLLOWINFORMER) != 0) ? 44 : 43);
		} else if (state == 11) {
			g_dialog_next_state = (has_intro_letter() ? 36 : 37);
		} else if (state == 14) {
			g_dialog_next_state = (has_intro_letter() ? 39 : 40);
		} else if (state == 19) {
			g_dialog_next_state = (ds_readb(TLK_TAV_FOLLOWINFORMER) != 0 ? 1 : (!ds_readb(TLK_TAV_FOLLOWINFORMER) && gs_informer_flags[INFORMER_JURGE] != 0 ? 2 : 3));
		} else if (state == 20) {
			g_dialog_next_state = (has_intro_letter() ? 21 : 22);
		} else if (state == 27) {

			/* get the map */
			g_dialog_next_state = (count_map_parts() ? 29 : 30);

			if (ds_readb(TREASURE_MAPS + 0) == 2) {
				ds_writeb(TMAP_DOUBLE2, 1);
			}

			ds_writeb(TREASURE_MAPS, 1);

			add_hero_ap_all(10);

		} else if (state == 29 || state == 30) {
			show_treasure_map();
		} else if (state == 35) {
			if (!gs_informer_flags[INFORMER_YASMA]) gs_informer_flags[INFORMER_YASMA] = 1;
			if (!gs_informer_flags[INFORMER_RAGNA]) gs_informer_flags[INFORMER_RAGNA] = 1;
			if (!gs_informer_flags[INFORMER_SWAFNILD]) gs_informer_flags[INFORMER_SWAFNILD] = 1;
			if (!gs_informer_flags[INFORMER_ALGRID]) gs_informer_flags[INFORMER_ALGRID] = 1;
		} else if (state == 40 || state == 41) {
			/* need introduction letter ? */
			gs_jurge_awaits_letter = 1;
		} else if (state == 42) {
			gs_informer_flags[INFORMER_JURGE] = 2;
		} else if (state == 44) {
			g_dialog_next_state = (gs_jurge_awaits_letter ? 20 : 19);
		} else if (state == 45) {
			g_dialog_next_state = (!gs_informer_flags[INFORMER_JURGE] ? 6 : 5);
		}

	} else if (informer == 1) {
		/* HJORE AHRENSSON */

		if (!state) {
			g_dialog_next_state = (!gs_informer_flags[INFORMER_HJORE] ? 16 : (gs_informer_flags[INFORMER_HJORE] == 2 ? 1 : 5));
		} else if (state == 5) {
			g_dialog_next_state = (get_first_hero_with_item(ITEM_SCHULDBUCH) != -1 ? 6 : 7);
		} else if (state == 8 || state == 9 || state == 12) {
			gs_informer_flags[INFORMER_HJORE] = 2;
		} else if (state == 10) {
			if (!gs_informer_flags[INFORMER_BEORN]) gs_informer_flags[INFORMER_BEORN] = 1;
		} else if (state == 13) {

			/* get the false map part ?*/
			if (ds_readb(TREASURE_MAPS + 9) == 2) {
				ds_writeb(TMAP_DOUBLE2, 1);
			}

			ds_writeb(TREASURE_MAPS + 9, 1);

			add_hero_ap_all(10);

			show_treasure_map();

		} else if (state == 14) {
			gs_informer_flags[INFORMER_HJORE] = 2;

			if (!gs_informer_flags[INFORMER_YASMA]) gs_informer_flags[INFORMER_YASMA] = 1;
		}
	}
}

/**
 * \brief   dialog logic for the informers yasma, umbrik and isleif
 *
 * \param   informer    0 = yasma, 1 = umbrik, 2 = isleif
 * \param   state       state of the dialog
 */
void INF_yasma_umbrik_isleif(signed short informer, signed short state)
{
	Bit8u *hero;

	if (!informer) {
		/* YASMA THINMARSDOTTER */

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_YASMA] == 2 ? 1 : 2);
		} else if (state == 15) {
			ds_writeb(TEVENT100_FLAG, 1);
		} else if (state == 22) {
			/* make HJORE AHRENSSON known */
			if (!gs_informer_flags[INFORMER_HJORE]) gs_informer_flags[INFORMER_HJORE] = 1;

			/* make UMBRIK SIEBENSTEIN known */
			if (!gs_informer_flags[INFORMER_UMBRIK]) gs_informer_flags[INFORMER_UMBRIK] = 1;

			/* mark YASMA THINMARSDOTTER as done */
			gs_informer_flags[INFORMER_YASMA] = 2;
		}
	} else if (informer == 1) {
		/* UMBRIK SIEBENSTEIN */

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_UMBRIK] == 2 ? 1 : 2);
		} else if (state == 2) {
			g_dialog_next_state = (gs_umbrik_quest_gorah ? 15 : (!gs_informer_flags[INFORMER_UMBRIK] ? 24 : 3));
		} else if (state == 11 || state == 13) {
			/* mark UMBRIK SIEBENSTEIN as done */
			gs_informer_flags[INFORMER_UMBRIK] = 2;
		} else if (state == 14) {
			/* the heroes now know of GORAH */
			gs_umbrik_quest_gorah = 1;
		} else if (state == 15) {
			/* check if the heroes have the RUNENKNOCHEN / BONE WITH RUNE */
			g_dialog_next_state = (get_first_hero_with_item(ITEM_BONE_WITH_RUNE) != -1 ? 16 : 17);
		} else if (state == 19) {
			/* give the RUNENKNOCHEN / BONE WITH RUNE to UMBRIK */
			hero = get_hero(get_first_hero_with_item(ITEM_BONE_WITH_RUNE));
			drop_item(hero, get_item_pos(hero, ITEM_BONE_WITH_RUNE), 1);
		} else if (state == 23) {
			/* mark UMBRIK SIEBENSTEIN as done */
			gs_informer_flags[INFORMER_UMBRIK] = 2;

			/* get EMPFEHLUNGSSCHREIBEN / LETTER OF INTRODUCTION */
			get_item(ITEM_WRITING_OF_SIEBENSTEIN, 1, 1);

			/* make BEORN HJALLASSON known */
			if (!gs_informer_flags[INFORMER_BEORN]) gs_informer_flags[INFORMER_BEORN] = 1;

			/* make TIOMAR SWAFNILDSSON known */
			if (!gs_informer_flags[INFORMER_TIOMAR]) gs_informer_flags[INFORMER_TIOMAR] = 1;
		}

	} else if (informer == 2) {
		/* ISLEIF OLGARDSSON */

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_ISLEIF] == 2 ? 1 : 2);
		} else if (state == 2) {
			g_dialog_next_state = (ds_readb(ISLEIF_JANDAS_REGARDS) != 0 ? 3 : 4);
		} else if (state == 8 || state == 23 || state == 25 || state == 27) {
			/* mark ISLEIF OLGARDSSON as done */
			gs_informer_flags[INFORMER_ISLEIF] = 2;
		} else if (state == 9) {
			g_dialog_next_state = (ds_readb(ISLEIF_JANDAS_REGARDS) != 0 ? 10 : 11);
		} else if (state == 15 || state == 19) {
			/* TODO: check what happens here */
		} else if (state == 16 || state == 20) {
			/* TODO: check what happens here */
		} else if (state == 17 || state == 26) {

			/* check if the heroes already have the map */
			if (ds_readb(TREASURE_MAPS + 2) == 2) ds_writeb(TMAP_DOUBLE2, 1);

			/* the heroes get the map */
			ds_writeb(TREASURE_MAPS + 2, 1);

			/* each of the heroes gets 10 AP */
			add_hero_ap_all(10);

			show_treasure_map();

			/* TODO: what does that mean ? */
			if (state == 17) ds_writeb(ISLEIF_MAP_GIVEN, 1);

		} else if (state == 24) {
			/* TODO: what does that mean ? */
			g_dialog_next_state = (ds_readb(ISLEIF_MAP_GIVEN) != 0 ? 25 : 26);
		}
	}
}

/**
 * \brief   dialog logic for the informers ragna, beorn and algrid
 *
 * \param   informer    0 = ragna, 1 = beorn, 2 = algrid
 * \param   state       state of the dialog
 */
void INF_ragna_beorn_algrid(signed short informer, signed short state)
{
	unsigned char tmp;
	signed short beorn_ch_malus;

	if (informer == 0) {
		/* RAGNA FIRUNJASDOTTER */

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_RAGNA] == 2 ? 1 : 3);
		} else if (state == 4) {
			g_dialog_next_state = (count_map_parts() ? 9 : 10);
		} else if (state == 8 || state == 25) {
			/* mark RAGNA FIRUNJASDOTTER as done */
			gs_informer_flags[INFORMER_RAGNA] = 2;
		} else if (state == 12) {
			/* directly asking for map gets a brush-off ("don't know what you
			   are talking about") and is silently noted by ragna!
			   it's not possible to get to see the map after that (see state 17) */
			ds_writeb(RAGNA_ASKED_MAP, 1);
		} else if (state == 14) {
			/* mark ISLEIF OLGARDSSON as known */
			if (!gs_informer_flags[INFORMER_ISLEIF]) gs_informer_flags[INFORMER_ISLEIF] = 1;
			/* mark SWAFNILD EGILSDOTTER as known */
			if (!gs_informer_flags[INFORMER_SWAFNILD]) gs_informer_flags[INFORMER_SWAFNILD] = 1;
		} else if (state == 16) {
			/* mark JURGE TORFINSSON as known */
			if (!gs_informer_flags[INFORMER_JURGE]) gs_informer_flags[INFORMER_JURGE] = 1;
		} else if (state == 17) {
			/* see state 12 */
			g_dialog_next_state = (ds_readb(RAGNA_ASKED_MAP) != 0 ? 18 : 19);
		} else if (state == 21) {

			/* check if the party already has this map piece */
			if (ds_readb(TREASURE_MAPS + 3) == 2) ds_writeb(TMAP_DOUBLE2, 1);
			/* get the map piece */
			ds_writeb(TREASURE_MAPS + 3, 1);
			/* each of the heroes gets 10 AP */
			add_hero_ap_all(10);

		} else if (state == 22) {

			/* test the group leader on KL+5, to get the map */
			if (test_attrib(get_hero(0), ATTRIB_KL, 5) > 0) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 3) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 3, 1);
				/* each of the heroes gets 10 AP */
				add_hero_ap_all(10);
			}

		} else if (state == 23) {

			/* she only shows you the map piece */

			tmp = ds_readb(TREASURE_MAPS + 3);

			/* check if the party already has this map piece */
			if (ds_readb(TREASURE_MAPS + 3) == 2) ds_writeb(TMAP_DOUBLE2, 1);

			/* get the map piece */
			ds_writeb(TREASURE_MAPS + 3, 1);

			show_treasure_map();
			/* remove the map piece */
			ds_writeb(TREASURE_MAPS + 3, tmp);
		}

	} else if (informer == 1) {
			/* BEORN HJALLASSON */

			if (!state) {
				g_dialog_next_state = (gs_informer_flags[INFORMER_BEORN] == 2 ? 1 : 2);
			} else if (state == 2) {
				/* is ERWO in the group ? */
				g_dialog_next_state = (host_readbs(get_hero(6) + HERO_NPC_ID) == NPC_ERWO && is_hero_available_in_group(get_hero(6)) ? 3 : 15);
			} else if (state == 6) {

				/* copy the name */
				strcpy(g_str_beorn_hjall, (char*)(get_hero(6) + HERO_NAME2));
				/* set a pointer */
				g_dialog_title = g_str_beorn_hjall;
				/* copy the picture of the NPC */
				memcpy(g_dtp2, get_hero(6) + HERO_PORTRAIT, 0x400);
				/* remove the NPC from the group */
				remove_npc(24, 31, 231, get_ttx(758), NULL);

				ds_writew(REQUEST_REFRESH, 1);

			} else if (state == 7 || state == 8 || state == 9 || state == 10) {
				timewarp(MINUTES(30));
			} else if (state == 11 || state == 26) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 4) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 4, 1);
				/* each of the heroes gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();
			} else if (state == 12 || state == 29) {
				/* mark BEORN HJALLASSON as done */
				gs_informer_flags[INFORMER_BEORN] = 2;
			} else if (state == 13 || state == 22) {
				/* make HJORE AHRENSSON known */
				if (!gs_informer_flags[INFORMER_HJORE]) gs_informer_flags[INFORMER_HJORE] = 1;
				/* mark RAGNA FIRUNJASDOTTER as known */
				if (!gs_informer_flags[INFORMER_RAGNA]) gs_informer_flags[INFORMER_RAGNA] = 1;
				/* mark SWAFNILD EGILSDOTTER as known */
				if (!gs_informer_flags[INFORMER_SWAFNILD]) gs_informer_flags[INFORMER_SWAFNILD] = 1;
			} else if (state == 18) {
				/* after "NUN, IHR SEID DOCH MIT HYGGELIK VERWANDT..." */
				ds_writeb(BEORN_LITT_BOLD, 1);
			} else if (state == 23) {
				/* after "NA, DAS WAR JA NICHT BESONDERS VIEL!" */
				ds_writeb(BEORN_VERY_BOLD, 1);
			} else if (state == 24) {
			    /* after 22 (new contacts), extremely bold heroes get the map now */
				g_dialog_next_state = (ds_readb(BEORN_LITT_BOLD) != 0 && ds_readb(BEORN_MEDI_BOLD) != 0 && ds_readb(BEORN_VERY_BOLD) != 0 ? 26 : 27);
			} else if (state == 25) {
				/* if asked "WISST IHR VIELLEICHT AUCH ETWAS?" */
				ds_writeb(BEORN_MEDI_BOLD, 1);
			} else if (state == 27) {
			    /* if not extremely bold, the hero is tested for CHARISMA with
			       malus as follows:
                       beorn_ch_malus = 10
                       beorn_ch_malus -= 4*l + 4*m + 6*v
                       if(l+m+v == 2) beorn_ch_malus -= 2
			       (l = BEORN_LITT_BOLD, m = BEORN_MEDI_BOLD, v = BEORN_VERY_BOLD)
			    */

				beorn_ch_malus = 10;

				if (ds_readb(BEORN_VERY_BOLD) && !ds_readb(BEORN_MEDI_BOLD) && !ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = 4;
				} else if (ds_readb(BEORN_VERY_BOLD) && ds_readb(BEORN_MEDI_BOLD) && !ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = -2;
				} else if (ds_readb(BEORN_VERY_BOLD) && !ds_readb(BEORN_MEDI_BOLD) && ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = -2;
				} else if (!ds_readb(BEORN_VERY_BOLD) && ds_readb(BEORN_MEDI_BOLD) && !ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = 6;
				} else if (!ds_readb(BEORN_VERY_BOLD) && !ds_readb(BEORN_MEDI_BOLD) && ds_readb(BEORN_LITT_BOLD)) {
					beorn_ch_malus = 6;
				} else if (ds_readb(BEORN_LITT_BOLD) && ds_readb(BEORN_MEDI_BOLD)) {
					beorn_ch_malus = 0;
				}

				g_dialog_next_state = (test_attrib(get_hero(0), ATTRIB_CH, beorn_ch_malus) > 0 ? 26 : 28);
			}
	} else if (informer == 2) {
		/* ALGRID TRONDESDOTTER */

		if (!state) {
			g_dialog_next_state = (ds_readb(TLK_TAV_FOLLOWINFORMER) ? 23 : (gs_informer_flags[INFORMER_ALGRID] == 2 ? 1 : 22));
		} else if (state == 2) {
			/* mark ALGRID TRONDESDOTTER as done */
			gs_informer_flags[INFORMER_ALGRID] = 2;

			g_dialog_next_state = (gs_informer_flags[INFORMER_JURGE] == 2 ? 3 : 4);
		} else if (state == 3) {
			g_dialog_next_state = (gs_algrid_treborns_hint ? 5 : 6);
		} else if (state == 4) {
			g_dialog_next_state = (gs_algrid_treborns_hint ? 7 : 8);
		} else if (state == 14) {
			/* make TIOMAR SWAFNILDSSON known */
			if (!gs_informer_flags[INFORMER_TIOMAR]) gs_informer_flags[INFORMER_TIOMAR] = 1;

			/* make TREBORN KOLBERG known */
			if (!gs_informer_flags[INFORMER_TREBORN]) gs_informer_flags[INFORMER_TREBORN] = 1;
		}
	}
}

/**
 * \brief   dialog logic for the informers eliane and tiomar
 *
 * \param   informer    0 = eliane, 1 = tiomar
 * \param   state       state of the dialog
 */
void INF_eliane_tiomar(signed short informer, signed short state)
{
	if (!informer) {
		/* ELIANE WINDENBECK */

		if (!state) {
			g_dialog_next_state = (ds_readb(ELIANE_QUEST_NAMELESS) || gs_got_main_quest == 0 ? 1 : 6);
		} else if (state == 1) {
			g_dialog_next_state = (ds_readb(QUEST_NAMELESS_DONE) && gs_informer_flags[INFORMER_ELIANE] != 2 ? 2 : 3);
		} else if (state == 5 || state == 27) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 5) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 5, 1);
				/* each of the heroes gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();

				/* mark ELIANE WINDENBECK as done */
				gs_informer_flags[INFORMER_ELIANE] = 2;
		} else if (state == 19) {
			g_dialog_next_state = (ds_readb(QUEST_NAMELESS_DONE) ? 20 : 30);
			ds_writeb(ELIANE_QUEST_NAMELESS, 1);
		} else if (state == 16) {
			/* mark YASMA THINMARSDOTTER as known */
			if (!gs_informer_flags[INFORMER_YASMA]) gs_informer_flags[INFORMER_YASMA] = 1;
			/* mark SWAFNILD EGILSDOTTER as known */
			if (!gs_informer_flags[INFORMER_SWAFNILD]) gs_informer_flags[INFORMER_SWAFNILD] = 1;
			/* mark ASGRIMM THURBOLDSSON as known */
			if (!gs_informer_flags[INFORMER_ASGRIMM]) gs_informer_flags[INFORMER_ASGRIMM] = 1;
		} else if (state == 24) {
			/* the group has the SCHWARZE STATUETTE/BLACK FIGURINE */
			g_dialog_next_state = (get_first_hero_with_item(ITEM_BLACK_FIGURINE) != -1 ? 27 : 28);
		}
	} else if (informer == 1) {
		/* TIOMAR SWAFNILDSSON */

		if (!state) {
			g_dialog_next_state = ((ds_readb(TLK_TAV_FOLLOWINFORMER) != 0 ? 44
		                : (!gs_informer_flags[INFORMER_TIOMAR] || gs_informer_flags[INFORMER_TIOMAR] == 2 ? 2 : 1)));
		} else if (state == 1) {
			g_dialog_next_state = (ds_readb(TIOMAR_AWAITS_LETTER) != 0 ? 36 : 3);
		} else if (state == 4) {
			g_dialog_next_state = (get_first_hero_with_item(ITEM_WRITING_OF_SIEBENSTEIN) != -1 ? 6 : 7);
		} else if (state == 12 || state == 42) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 8) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 8, 1);
				/* each of the heroes gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();

				/* mark TIOMAR SWAFNILDSSON as done */
				gs_informer_flags[INFORMER_TIOMAR] = 2;
		} else if (state == 17) {
			/* mark JURGE TORFINSSON as known */
			if (!gs_informer_flags[INFORMER_JURGE]) gs_informer_flags[INFORMER_JURGE] = 1;
			/* mark ISLEIF OLGARDSSON as known */
			if (!gs_informer_flags[INFORMER_ISLEIF]) gs_informer_flags[INFORMER_ISLEIF] = 1;
		} else if (state == 20) {
			/* drink with TIOMAR */
			timewarp(HOURS(1));
			g_dialog_next_state = (test_skill(get_hero(ds_writeb(TIOMAR_DRINKMATE, (unsigned char)get_random_hero())), TA_ZECHEN, 0) > 0 ? 21 : 22);
		} else if (state == 22) {
			/* TIOMARS drinkmate gets drunken */
			hero_get_drunken(get_hero(ds_readb(TIOMAR_DRINKMATE)));
		} else if (state == 31) {
			/* mark JURGE TORFINSSON as known */
			if (!gs_informer_flags[INFORMER_JURGE]) gs_informer_flags[INFORMER_JURGE] = 1;
			/* mark ISLEIF OLGARDSSON as known */
			if (!gs_informer_flags[INFORMER_ISLEIF]) gs_informer_flags[INFORMER_ISLEIF] = 1;
			/* mark UMBRIK SIEBENSTEIN as known */
			if (!gs_informer_flags[INFORMER_UMBRIK]) gs_informer_flags[INFORMER_UMBRIK] = 1;
		} else if (state == 34) {
			ds_writeb(TIOMAR_AWAITS_LETTER, 1);
		} else if (state == 36) {
			g_dialog_next_state = (get_first_hero_with_item(ITEM_WRITING_OF_SIEBENSTEIN) != -1 ? 37 : 2);
		} else if (state == 45) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_UMBRIK] == 2 ? 46 : 47);
		}

	}
}

/**
 * \brief   dialog logic for the informers olvir and asgrimm
 *
 * \param   informer    0 = olvir, 1 = asgrimm
 * \param   state       state of the dialog
 */
void INF_olvir_asgrimm(signed short informer, signed short state)
{
	Bit8u *hero;

	if (!informer) {
		/* OLVIR GUNDRIDSSON */

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_OLVIR] == 2 ? 1 : 2);
		} else if (state == 2) {
			/* mark OLVIR GUNDRIDSSON as done */
			gs_informer_flags[INFORMER_OLVIR] = 2;
		} else if (state == 8) {
			ds_writeb(OLVIR_START_HETMANN, ds_writeb(OLVIR_SINGING_HETMANN, 1));
		} else if (state == 9) {
			ds_writeb(OLVIR_START_HYGGELIK, ds_writeb(OLVIR_SINGING_HYGGELIK, 1));
		} else if (state == 10) {
			ds_writeb(OLVIR_START_HASGAR, ds_writeb(OLVIR_SINGING_HASGAR, 1));
		} else if (state == 12 || state == 13 || state == 23 || state == 24 || state == 29 || state == 30) {
			timewarp(MINUTES(30));
		} else if (state == 14 || state == 15 || state == 21 || state == 22 || state == 28) {
			timewarp(HOURS(1));
		} else if (state == 16) {
			g_dialog_next_state = (ds_readb(OLVIR_INTERRUPTED) != 0 ? 19 : 13);
			ds_writeb(OLVIR_INTERRUPTED, 1);
		} else if (state == 17) {
			g_dialog_next_state = (ds_readb(OLVIR_INTERRUPTED) != 0 ? 19 : 14);
			ds_writeb(OLVIR_INTERRUPTED, 1);
		} else if (state == 18) {
			g_dialog_next_state = (ds_readb(OLVIR_INTERRUPTED) != 0 ? 19 : 15);
			ds_writeb(OLVIR_INTERRUPTED, 1);
		} else if (state == 26) {
			g_dialog_next_state = (ds_readb(OLVIR_INTERRUPTED) != 0 ? 19 : 22);
			ds_writeb(OLVIR_INTERRUPTED, 1);
		} else if (state == 27) {
			g_dialog_next_state = (ds_readb(OLVIR_INTERRUPTED) != 0 ? 19 : 23);
			ds_writeb(OLVIR_INTERRUPTED, 1);
		} else if (state == 31) {
			g_dialog_next_state = (ds_readb(OLVIR_INTERRUPTED) != 0 ? 19 : 29);
			ds_writeb(OLVIR_INTERRUPTED, 1);
		} else if (state == 32) {
			g_dialog_next_state = (ds_readb(OLVIR_INTERRUPTED) != 0 ? 19 : 30);
			ds_writeb(OLVIR_INTERRUPTED, 1);
		} else if (state == 33) {
		        /* whenever one of the songs is over, the name of an informer is given */
			g_dialog_next_state = (ds_readb(OLVIR_SINGING_HETMANN) != 0 ? 11 : (ds_readb(OLVIR_SINGING_HYGGELIK) != 0 ? 35 : 36));
		} else if (state == 34) {
			ds_writeb(OLVIR_SINGING_HETMANN, 0);
			ds_writeb(OLVIR_SINGING_HYGGELIK, 1);
		} else if (state == 35) {
			ds_writeb(OLVIR_SINGING_HYGGELIK, 0);
			ds_writeb(OLVIR_SINGING_HASGAR, 1);
		} else if (state == 37) {
			g_dialog_next_state = (ds_readb(OLVIR_START_HYGGELIK) != 0 ? 39 : 40);
		} else if (state == 39) {
			g_dialog_next_state = (ds_readb(OLVIR_START_HETMANN) != 0 ? 42 : 41);
		}
	} else if (informer == 1) {
		/* ASGRIMM THURBOLDSSON */

		if (!state) {
			g_dialog_next_state = (ds_readb(TLK_TAV_FOLLOWINFORMER) != 0 ? 22 : (gs_got_main_quest == 0 || gs_informer_flags[INFORMER_ASGRIMM] == 2 ? 1 : 2));
		} else if (state == 2) {
			/* mark ASGRIMM THURBOLDSSON as done */
			gs_informer_flags[INFORMER_ASGRIMM] = 2;
		} else if (state == 7) {
			signed short i;
			/* ASGRIMM takes a meal with the heroes */
			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
					!hero_dead(hero))
				{
					/* set hunger and thirst to 0 */
					host_writebs(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));
				}
			}
		} else if (state == 16) {
			/* make HJORE AHRENSSON known */
			if (!gs_informer_flags[INFORMER_HJORE]) gs_informer_flags[INFORMER_HJORE] = 1;
			/* mark RAGNA FIRUNJASDOTTER as known */
			if (!gs_informer_flags[INFORMER_RAGNA]) gs_informer_flags[INFORMER_RAGNA] = 1;
		}
	}
}

/**
 * \brief   dialog logic for the informers Treborn Kolberg and unicorn (1st meeting)
 *
 * \param   informer    0 = treborn, 1 = treborn, 2 = unicorn
 * \param   state       state of the dialog
 */
void INF_treborn_unicorn(signed short informer, signed short state)
{
	Bit32s money;
	signed short enough_money;

	money = get_party_money();

	if (!informer) {
		/* TREBORN KOLBERG (first meeting) */

		enough_money = money >= 6000 ? 1 : 0;

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_TREBORN] != 0 ? -1 : 1);
		} else if (state == 1) {
			/* mark TREBORN KOLBERG as met */
			if (!gs_informer_flags[INFORMER_TREBORN]) gs_informer_flags[INFORMER_TREBORN] = 1;
		} else if (state == 5) {
			g_dialog_next_state = (enough_money ? 7 : 8);
		} else if (state == 9) {

			/* buy the map */
			money -= 6000;
			set_party_money(money);

			/* check if the party already has this map piece */
			if (ds_readb(TREASURE_MAPS + 7) == 2) ds_writeb(TMAP_DOUBLE2, 1);
			/* get the map piece */
			ds_writeb(TREASURE_MAPS + 7, 1);
			/* each of the heroes gets 10 AP */
			add_hero_ap_all(10);

			/* mark ALGRID TRONDESDOTTER as known */
			/* Original-BUG: this would reactivate algrid */
			gs_informer_flags[INFORMER_ALGRID] = gs_algrid_treborns_hint = 1;

			show_treasure_map();

		} else if (state == 11) {
			/* mark JURGE TORFINSSON as known */
			if (!gs_informer_flags[INFORMER_JURGE]) gs_informer_flags[INFORMER_JURGE] = 1;
		} else if (state == 13) {
			g_dialog_next_state = (enough_money ? 14 : 15);
		} else if (state == 17) {
			g_dialog_next_state = (enough_money ? 19 : 20);
		} else if (state == 18) {
			/* visit the shop of Treborn Kolberg */
			ds_writew(CURRENT_TYPEINDEX, 91);
			do_merchant();
		} else if (state == 21) {
			ds_writeb(TREBORN_DATE, 1);
		} else if (state == 23) {
			g_dialog_next_state = (enough_money ? 25 : 24);
		}
	} else if (informer == 1) {
		/* TREBORN KOLBERG (second meeting) */

		enough_money = money >= 7500 ? 1 : 0;

		if (!state) {
			g_dialog_next_state = (!ds_readb(TREBORN_DATE) ? -1 : 1);
		} else if (state == 2) {
			g_dialog_next_state = (enough_money ? 3 : 4);
		} else if (state == 5) {
			g_dialog_next_state = (enough_money ? 6 : 7);
		} else if (state == 12) {
			/* buy the map */
			money -= 7500;
			set_party_money(money);

			/* check if the party already has this map piece */
			if (ds_readb(TREASURE_MAPS + 7) == 2) ds_writeb(TMAP_DOUBLE2, 1);
			/* get the map piece */
			ds_writeb(TREASURE_MAPS + 7, 1);
			/* each of the heroes gets 10 AP */
			add_hero_ap_all(10);

			/* mark ALGRID TRONDESDOTTER as known */
			/* Original-BUG: this would reactivate algrid */
			gs_informer_flags[INFORMER_ALGRID] = gs_algrid_treborns_hint = 1;

			show_treasure_map();

		} else if (state == 14) {
			/* mark JURGE TORFINSSON as known */
			if (!gs_informer_flags[INFORMER_JURGE]) gs_informer_flags[INFORMER_JURGE] = 1;
		}

	} else if (informer == 2) {
		/* EINHORN / UNICORN (first meeting) */

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_UNICORN] == 2 ? 1 : 2);
		} else if (state == 2) {
			/* select the hero with the highest CH value */
			/* REMARK: what if the NPC is choosen ? */
			/* REMARK: what if the positions are changed ? */
			/* REMARK: what if the game is saved and the heroes are at another mem location ? */
			ds_writed(UNICORN_HERO_PTR, (Bit32u)get_hero(ds_writeb(UNICORN_HERO_POS, (unsigned char)get_hero_CH_best())));
		} else if (state == 7) {
			timewarp(HOURS(1));
		} else if (state == 8) {
			timewarp(HOURS(1));
			/* mark UNICORN as done */
			gs_informer_flags[INFORMER_UNICORN] = 2;
		} else if (state == 9) {
			/* mark UNICORN as done */
			gs_informer_flags[INFORMER_UNICORN] = 2;
		} else if (state == 10) {
			/* test FF+2 */
			g_dialog_next_state = (test_attrib((Bit8u*)ds_readd(UNICORN_HERO_PTR), ATTRIB_GE, 2) > 0 ? 11 : 14);
		} else if (state == 11) {
			/* test FF+5 */
			g_dialog_next_state = (test_attrib((Bit8u*)ds_readd(UNICORN_HERO_PTR), ATTRIB_GE, 5) > 0 ? 12 : 13);
		} else if (state == 15) {
			g_dialog_next_state = (random_schick(100) <= 50 ? 16 : 17);
		} else if (state == 16) {
			/* the hero disappears */
			hero_disappear((Bit8u*)ds_readd(UNICORN_HERO_PTR), ds_readb(UNICORN_HERO_POS), -1);
		} else if (state == 17) {
			/* the hero gets heavily wounded, 1 LE left */
			sub_hero_le((Bit8u*)ds_readd(UNICORN_HERO_PTR), host_readws((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_LE) - 1);
			/* the party opens a camp */
			gs_current_loctype = LOCTYPE_WILDCAMP;
			do_location();
		} else if (state == 18) {
			timewarp(HOURS(2));
		} else if (state == 25) {
			/* the UNICORN will get the map */
			ds_writeb(UNICORN_GET_MAP, 1);

			/* the hero gets 100 AP */
			add_hero_ap((Bit8u*)ds_readd(UNICORN_HERO_PTR), 100);

			/* set the unicorn timer (in days) */
			ds_writeb(UNICORN_TIMER, random_schick(24) + 36);
		}
	}
}

/**
 * \brief   dialog logic for the informers swafnild and unicorn (2nd meeting)
 *
 * \param   informer    0 = Swafnild, 1 = unicorn
 * \param   state       state of the dialog
 */
void INF_swafnild_unicorn(signed short informer, signed short state)
{
	if (!informer) {
		/* SWAFNILD EGILSDOTTER */

		if (state == 1) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_SWAFNILD] == 2 ? 3 : 7);
		} else if (state == 4) {
			g_dialog_next_state = (ds_readb(SWAFNILD_NOMAP) != 0 ? 38 : 39);
		} else if (state == 7) {

			if ((ds_readbs(CURRENT_TOWN) >= TOWNS_THORWAL && ds_readbs(CURRENT_TOWN) <= TOWNS_EFFERDUN) ||
				(ds_readbs(CURRENT_TOWN) >= TOWNS_DASPOTA && ds_readbs(CURRENT_TOWN) <= TOWNS_VAERMHAG))
			{
				ds_writeb(SWAFNILD_TP1, TOWNS_PREM);
				ds_writeb(SWAFNILD_TP2, TOWNS_SKJAL);
				ds_writeb(SWAFNILD_TP3, TOWNS_OTTARJE);
				ds_writeb(SWAFNILD_TP4, TOWNS_ARYN);

			} else if (ds_readbs(CURRENT_TOWN) >= TOWNS_ROVIK && ds_readbs(CURRENT_TOWN) <= TOWNS_TREBAN) {
				ds_writeb(SWAFNILD_TP1, TOWNS_OTTARJE);
				ds_writeb(SWAFNILD_TP2, TOWNS_SKJAL);
				ds_writeb(SWAFNILD_TP3, TOWNS_PREM);
				ds_writeb(SWAFNILD_TP4, TOWNS_LJASDAHL);
			} else {
				ds_writeb(SWAFNILD_TP1, TOWNS_KORD);
				ds_writeb(SWAFNILD_TP2, TOWNS_TREBAN);
				ds_writeb(SWAFNILD_TP3, TOWNS_RUNINSHAVEN);
				ds_writeb(SWAFNILD_TP4, TOWNS_GUDDASUNDEN);
			}
		} else if (state == 17) {
			/* mark RAGNA FIRUNJASDOTTER as known */
			if (!gs_informer_flags[INFORMER_RAGNA]) gs_informer_flags[INFORMER_RAGNA] = 1;
			/* make BEORN HJALLASSON known */
			if (!gs_informer_flags[INFORMER_BEORN]) gs_informer_flags[INFORMER_BEORN] = 1;
			/* make TIOMAR SWAFNILDSSON known */
			if (!gs_informer_flags[INFORMER_TIOMAR]) gs_informer_flags[INFORMER_TIOMAR] = 1;
		} else if (state == 18) {
			/* test CH+5 */
			g_dialog_next_state = (test_attrib(get_hero(0), ATTRIB_CH, 5) > 0 ? 19 : 20);
		} else if (state == 21) {
			/* mark SWAFNILD EGILSDOTTER as done */
			gs_informer_flags[INFORMER_SWAFNILD] = 2;
			ds_writeb(SWAFNILD_NOMAP, 1);
		} else if (state == 22) {
			/* test CH+3 */
			g_dialog_next_state = (test_attrib(get_hero(0), ATTRIB_CH, 3) > 0 ? 24 : 23);
		} else if (state == 24 || state == 41) {
			/* mark SWAFNILD EGILSDOTTER as done */
			gs_informer_flags[INFORMER_SWAFNILD] = 2;
		} else if (state == 32) {
				/* check if the party already has this map piece */
				if (ds_readb(TREASURE_MAPS + 6) == 2) ds_writeb(TMAP_DOUBLE2, 1);
				/* get the map piece */
				ds_writeb(TREASURE_MAPS + 6, 1);
				/* each of the heroes gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();

		} else if (state == 37) {

			ds_writeb(CURRENT_TOWN, ds_readb(SWAFNILD_DESTINATION) == 1 ? ds_readb(SWAFNILD_TP1) :
						(ds_readb(SWAFNILD_DESTINATION) == 2 ? ds_readb(SWAFNILD_TP2) : ds_readb(SWAFNILD_TP3)));

			switch (ds_readbs(CURRENT_TOWN)) {
				case TOWNS_PREM: ds_writew(X_TARGET_BAK, 22); ds_writew(Y_TARGET_BAK,  8); break;
				case TOWNS_KORD: ds_writew(X_TARGET_BAK,  4); ds_writew(Y_TARGET_BAK,  3); break;
				case TOWNS_OTTARJE: ds_writew(X_TARGET_BAK,  9); ds_writew(Y_TARGET_BAK, 10); break;
				case TOWNS_SKJAL: ds_writew(X_TARGET_BAK, 11); ds_writew(Y_TARGET_BAK, 11); break;
				case TOWNS_TREBAN: ds_writew(X_TARGET_BAK,  4); ds_writew(Y_TARGET_BAK, 12); break;
				case TOWNS_RUNINSHAVEN: ds_writew(X_TARGET_BAK,  6); ds_writew(Y_TARGET_BAK,  6); break;
			}

			gs_current_loctype_bak = LOCTYPE_NONE;
			ds_writeb(SWAFNILD_TRAVELED, 1);

			timewarp_until_midnight();
		} else if (state == 42) {
			ds_writeb(SWAFNILD_DESTINATION, 1);
		} else if (state == 43) {
			ds_writeb(SWAFNILD_DESTINATION, 2);
		} else if (state == 44) {
			ds_writeb(SWAFNILD_DESTINATION, 3);
		}

	} else if (informer == 1) {
		/* EINHORN / UNICORN (second meeting) */

		if (state == 3) {
			signed short i;

			/* search a map part the party does not have */
			for (i = 0; i < 9; i++) {
				if (!ds_readbs(TREASURE_MAPS + i)) {
					ds_writeb(TREASURE_MAPS + i, 2);
					i = 99;
					break;
				}
			}

			/* check if the group had all map parts */
			if (i != 99) {
				ds_writeb(TMAP_DOUBLE1, 1);
			}

			show_treasure_map();
		}
	}
}

/**
 * \brief   counts the collected parts of the treasure map
 *
 * \return              the number of map parts
 */
signed short count_map_parts(void)
{
	int i;
	int parts;

	for (i = parts = 0; i < 9; i++) {
		if (ds_readb(TREASURE_MAPS + i) != 0)
			parts++;
	}

	return parts;
}

/**
 * \brief   check if a hero in the group has the letter of introduction
 *
 * \return              0 = no letter, 1 = have the letter
 */
signed short has_intro_letter(void)
{
	/* check for the introduction letter / Empfehlungsschreiben */
	return (get_first_hero_with_item(ITEM_WRITING_OF_HETMAN) != -1) ? 1 : 0;
}

#if !defined(__BORLANDC__)
}
#endif

