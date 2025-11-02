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

signed char g_tmap_double1 = 0; // ds:0x7c9a, 1 = unicorn brought a piece you already have
signed char g_tmap_double2 = 0; // ds:0x7c9b, 1 = you got a piece you already have from the unicorn
signed char g_swafnild_traveled = 0; // ds:0x7c9c

static char g_str_beorn_hjall[20]; // ds:0xe42e

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
			g_dialog_next_state = ((gs_informer_flags[INFORMER_JURGE] == 1 || gs_tlk_tav_followinformer) ? 44 : 43);
		} else if (state == 11) {
			g_dialog_next_state = (has_intro_letter() ? 36 : 37);
		} else if (state == 14) {
			g_dialog_next_state = (has_intro_letter() ? 39 : 40);
		} else if (state == 19) {
			g_dialog_next_state = (gs_tlk_tav_followinformer ? 1 : (!gs_tlk_tav_followinformer && gs_informer_flags[INFORMER_JURGE] != 0 ? 2 : 3));
		} else if (state == 20) {
			g_dialog_next_state = (has_intro_letter() ? 21 : 22);
		} else if (state == 27) {

			/* get the map */
			g_dialog_next_state = (count_map_parts() ? 29 : 30);

			if (gs_treasure_maps[0] == 2) {
				g_tmap_double2 = 1;
			}

			gs_treasure_maps[0] = 1;

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
			if (gs_treasure_maps[9] == 2) {
				g_tmap_double2 = 1;
			}

			gs_treasure_maps[9] = 1;

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
	struct struct_hero *hero;

	if (!informer) {
		/* YASMA THINMARSDOTTER */

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_YASMA] == 2 ? 1 : 2);
		} else if (state == 15) {
			gs_tevent100_flag = 1;
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
			g_dialog_next_state = (gs_isleif_jandas_regards ? 3 : 4);
		} else if (state == 8 || state == 23 || state == 25 || state == 27) {
			/* mark ISLEIF OLGARDSSON as done */
			gs_informer_flags[INFORMER_ISLEIF] = 2;
		} else if (state == 9) {
			g_dialog_next_state = (gs_isleif_jandas_regards ? 10 : 11);
		} else if (state == 15 || state == 19) {
			/* TODO: check what happens here */
		} else if (state == 16 || state == 20) {
			/* TODO: check what happens here */
		} else if (state == 17 || state == 26) {

			/* check if the heroes already have the map */
			if (gs_treasure_maps[2] == 2) g_tmap_double2 = 1;

			/* the heroes get the map */
			gs_treasure_maps[2] = 1;

			/* each of the heroes gets 10 AP */
			add_hero_ap_all(10);

			show_treasure_map();

			/* TODO: what does that mean ? */
			if (state == 17) gs_isleif_map_given = 1;

		} else if (state == 24) {
			/* TODO: what does that mean ? */
			g_dialog_next_state = (gs_isleif_map_given ? 25 : 26);
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
			gs_ragna_asked_map = 1;

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
			g_dialog_next_state = (gs_ragna_asked_map ? 18 : 19);
		} else if (state == 21) {

			/* check if the party already has this map piece */
			if (gs_treasure_maps[3] == 2) g_tmap_double2 = 1;
			/* get the map piece */
			gs_treasure_maps[3] = 1;
			/* each of the heroes gets 10 AP */
			add_hero_ap_all(10);

		} else if (state == 22) {

			/* test the group leader on KL+5, to get the map */
			if (test_attrib(get_hero(0), ATTRIB_KL, 5) > 0) {
				/* check if the party already has this map piece */
				if (gs_treasure_maps[3] == 2) g_tmap_double2 = 1;
				/* get the map piece */
				gs_treasure_maps[3] = 1;
				/* each of the heroes gets 10 AP */
				add_hero_ap_all(10);
			}

		} else if (state == 23) {

			/* she only shows you the map piece */

			tmp = gs_treasure_maps[3];

			/* check if the party already has this map piece */
			if (gs_treasure_maps[3] == 2) g_tmap_double2 = 1;

			/* get the map piece */
			gs_treasure_maps[3] = 1;

			show_treasure_map();
			/* remove the map piece */
			gs_treasure_maps[3] = tmp;
		}

	} else if (informer == 1) {
			/* BEORN HJALLASSON */

			if (!state) {
				g_dialog_next_state = (gs_informer_flags[INFORMER_BEORN] == 2 ? 1 : 2);
			} else if (state == 2) {
				/* is ERWO in the group ? */
				g_dialog_next_state = (get_hero(6)->npc_id == NPC_ERWO && is_hero_available_in_group(get_hero(6)) ? 3 : 15);
			} else if (state == 6) {

				/* copy the name */
				strcpy(g_str_beorn_hjall, get_hero(6)->alias);
				/* set a pointer */
				g_dialog_title = g_str_beorn_hjall;
				/* copy the picture of the NPC */
				memcpy(g_dtp2, get_hero(6)->pic, 0x400);
				/* remove the NPC from the group */
				remove_npc(24, 31, 0xe7, get_ttx(758), NULL);

				g_request_refresh = 1;

			} else if (state == 7 || state == 8 || state == 9 || state == 10) {
				timewarp(MINUTES(30));
			} else if (state == 11 || state == 26) {
				/* check if the party already has this map piece */
				if (gs_treasure_maps[4] == 2) g_tmap_double2 = 1;
				/* get the map piece */
				gs_treasure_maps[4] = 1;
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
				gs_beorn_litt_bold = 1;
			} else if (state == 23) {
				/* after "NA, DAS WAR JA NICHT BESONDERS VIEL!" */
				gs_beorn_very_bold = 1;
			} else if (state == 24) {
			    /* after 22 (new contacts), extremely bold heroes get the map now */
				g_dialog_next_state = (gs_beorn_litt_bold != 0 && gs_beorn_medi_bold != 0 && gs_beorn_very_bold != 0 ? 26 : 27);
			} else if (state == 25) {
				/* if asked "WISST IHR VIELLEICHT AUCH ETWAS?" */
				gs_beorn_medi_bold = 1;
			} else if (state == 27) {
			    /* if not extremely bold, the hero is tested for CHARISMA with
			       malus as follows:
                       beorn_ch_malus = 10
                       beorn_ch_malus -= 4*l + 4*m + 6*v
                       if(l+m+v == 2) beorn_ch_malus -= 2
			       (l = BEORN_LITT_BOLD, m = BEORN_MEDI_BOLD, v = BEORN_VERY_BOLD)
			    */

				beorn_ch_malus = 10;

				if (gs_beorn_very_bold && !gs_beorn_medi_bold && !gs_beorn_litt_bold) {
					beorn_ch_malus = 4;
				} else if (gs_beorn_very_bold && gs_beorn_medi_bold && !gs_beorn_litt_bold) {
					beorn_ch_malus = -2;
				} else if (gs_beorn_very_bold && !gs_beorn_medi_bold && gs_beorn_litt_bold) {
					beorn_ch_malus = -2;
				} else if (!gs_beorn_very_bold && gs_beorn_medi_bold && !gs_beorn_litt_bold) {
					beorn_ch_malus = 6;
				} else if (!gs_beorn_very_bold && !gs_beorn_medi_bold && gs_beorn_litt_bold) {
					beorn_ch_malus = 6;
				} else if (gs_beorn_litt_bold && gs_beorn_medi_bold) {
					beorn_ch_malus = 0;
				}

				g_dialog_next_state = (test_attrib(get_hero(0), ATTRIB_CH, beorn_ch_malus) > 0 ? 26 : 28);
			}
	} else if (informer == 2) {
		/* ALGRID TRONDESDOTTER */

		if (!state) {
			g_dialog_next_state = (gs_tlk_tav_followinformer ? 23 : (gs_informer_flags[INFORMER_ALGRID] == 2 ? 1 : 22));
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
			g_dialog_next_state = (gs_eliane_quest_nameless || gs_got_main_quest == 0 ? 1 : 6);
		} else if (state == 1) {
			g_dialog_next_state = (gs_quest_nameless_done && gs_informer_flags[INFORMER_ELIANE] != 2 ? 2 : 3);
		} else if (state == 5 || state == 27) {
				/* check if the party already has this map piece */
				if (gs_treasure_maps[5] == 2) g_tmap_double2 = 1;
				/* get the map piece */
				gs_treasure_maps[5] = 1;
				/* each of the heroes gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();

				/* mark ELIANE WINDENBECK as done */
				gs_informer_flags[INFORMER_ELIANE] = 2;
		} else if (state == 19) {
			g_dialog_next_state = (gs_quest_nameless_done ? 20 : 30);
			gs_eliane_quest_nameless = 1;
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
			g_dialog_next_state = ((gs_tlk_tav_followinformer ? 44
		                : (!gs_informer_flags[INFORMER_TIOMAR] || gs_informer_flags[INFORMER_TIOMAR] == 2 ? 2 : 1)));
		} else if (state == 1) {
			g_dialog_next_state = (gs_tiomar_awaits_letter ? 36 : 3);
		} else if (state == 4) {
			g_dialog_next_state = (get_first_hero_with_item(ITEM_WRITING_OF_SIEBENSTEIN) != -1 ? 6 : 7);
		} else if (state == 12 || state == 42) {
				/* check if the party already has this map piece */
				if (gs_treasure_maps[8] == 2) g_tmap_double2 = 1;
				/* get the map piece */
				gs_treasure_maps[8] = 1;
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
			g_dialog_next_state = (test_skill(get_hero((gs_tiomar_drinkmate = (unsigned char)get_random_hero())), TA_ZECHEN, 0) > 0 ? 21 : 22);
		} else if (state == 22) {
			/* TIOMARS drinkmate gets drunken */
			hero_get_drunken(get_hero(gs_tiomar_drinkmate));
		} else if (state == 31) {
			/* mark JURGE TORFINSSON as known */
			if (!gs_informer_flags[INFORMER_JURGE]) gs_informer_flags[INFORMER_JURGE] = 1;
			/* mark ISLEIF OLGARDSSON as known */
			if (!gs_informer_flags[INFORMER_ISLEIF]) gs_informer_flags[INFORMER_ISLEIF] = 1;
			/* mark UMBRIK SIEBENSTEIN as known */
			if (!gs_informer_flags[INFORMER_UMBRIK]) gs_informer_flags[INFORMER_UMBRIK] = 1;
		} else if (state == 34) {
			gs_tiomar_awaits_letter = 1;
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
	struct struct_hero *hero;

	if (!informer) {
		/* OLVIR GUNDRIDSSON */

		if (!state) {
			g_dialog_next_state = (gs_informer_flags[INFORMER_OLVIR] == 2 ? 1 : 2);
		} else if (state == 2) {
			/* mark OLVIR GUNDRIDSSON as done */
			gs_informer_flags[INFORMER_OLVIR] = 2;
		} else if (state == 8) {
			gs_olvir_start_hetmann = gs_olvir_singing_hetmann = 1;
		} else if (state == 9) {
			gs_olvir_start_hyggelik = gs_olvir_singing_hyggelik = 1;
		} else if (state == 10) {
			gs_olvir_start_hasgar = gs_olvir_singing_hasgar = 1;
		} else if (state == 12 || state == 13 || state == 23 || state == 24 || state == 29 || state == 30) {
			timewarp(MINUTES(30));
		} else if (state == 14 || state == 15 || state == 21 || state == 22 || state == 28) {
			timewarp(HOURS(1));
		} else if (state == 16) {
			g_dialog_next_state = (gs_olvir_interrupted ? 19 : 13);
			gs_olvir_interrupted = 1;
		} else if (state == 17) {
			g_dialog_next_state = (gs_olvir_interrupted ? 19 : 14);
			gs_olvir_interrupted = 1;
		} else if (state == 18) {
			g_dialog_next_state = (gs_olvir_interrupted ? 19 : 15);
			gs_olvir_interrupted = 1;
		} else if (state == 26) {
			g_dialog_next_state = (gs_olvir_interrupted ? 19 : 22);
			gs_olvir_interrupted = 1;
		} else if (state == 27) {
			g_dialog_next_state = (gs_olvir_interrupted ? 19 : 23);
			gs_olvir_interrupted = 1;
		} else if (state == 31) {
			g_dialog_next_state = (gs_olvir_interrupted ? 19 : 29);
			gs_olvir_interrupted = 1;
		} else if (state == 32) {
			g_dialog_next_state = (gs_olvir_interrupted ? 19 : 30);
			gs_olvir_interrupted = 1;
		} else if (state == 33) {
		        /* whenever one of the songs is over, the name of an informer is given */
			g_dialog_next_state = (gs_olvir_singing_hetmann ? 11 : (gs_olvir_singing_hyggelik ? 35 : 36));
		} else if (state == 34) {
			gs_olvir_singing_hetmann = 0;
			gs_olvir_singing_hyggelik = 1;
		} else if (state == 35) {
			gs_olvir_singing_hyggelik = 0;
			gs_olvir_singing_hasgar = 1;
		} else if (state == 37) {
			g_dialog_next_state = (gs_olvir_start_hyggelik ? 39 : 40);
		} else if (state == 39) {
			g_dialog_next_state = (gs_olvir_start_hetmann ? 42 : 41);
		}
	} else if (informer == 1) {
		/* ASGRIMM THURBOLDSSON */

		if (!state) {
			g_dialog_next_state = (gs_tlk_tav_followinformer ? 22 : (gs_got_main_quest == 0 || gs_informer_flags[INFORMER_ASGRIMM] == 2 ? 1 : 2));
		} else if (state == 2) {
			/* mark ASGRIMM THURBOLDSSON as done */
			gs_informer_flags[INFORMER_ASGRIMM] = 2;
		} else if (state == 7) {
			signed short i;
			/* ASGRIMM takes a meal with the heroes */
			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero++) {

				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead)
				{
					/* set hunger and thirst to 0 */
					hero->hunger = hero->thirst = 0;
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
			if (gs_treasure_maps[7] == 2) g_tmap_double2 = 1;
			/* get the map piece */
			gs_treasure_maps[7] = 1;
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
			gs_current_typeindex = 91;
			do_merchant();
		} else if (state == 21) {
			gs_treborn_date = 1;
		} else if (state == 23) {
			g_dialog_next_state = (enough_money ? 25 : 24);
		}
	} else if (informer == 1) {
		/* TREBORN KOLBERG (second meeting) */

		enough_money = money >= 7500 ? 1 : 0;

		if (!state) {
			g_dialog_next_state = (!gs_treborn_date ? -1 : 1);
		} else if (state == 2) {
			g_dialog_next_state = (enough_money ? 3 : 4);
		} else if (state == 5) {
			g_dialog_next_state = (enough_money ? 6 : 7);
		} else if (state == 12) {
			/* buy the map */
			money -= 7500;
			set_party_money(money);

			/* check if the party already has this map piece */
			if (gs_treasure_maps[7] == 2) g_tmap_double2 = 1;
			/* get the map piece */
			gs_treasure_maps[7] = 1;
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
			gs_unicorn_hero_ptr = (Bit8u*)get_hero((gs_unicorn_hero_pos = get_hero_CH_best()));
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
			g_dialog_next_state = (test_attrib((struct struct_hero*)gs_unicorn_hero_ptr, ATTRIB_GE, 2) > 0 ? 11 : 14);
		} else if (state == 11) {
			/* test FF+5 */
			g_dialog_next_state = (test_attrib((struct struct_hero*)gs_unicorn_hero_ptr, ATTRIB_GE, 5) > 0 ? 12 : 13);
		} else if (state == 15) {
			g_dialog_next_state = (random_schick(100) <= 50 ? 16 : 17);
		} else if (state == 16) {
			/* the hero disappears */
			hero_disappear((struct struct_hero*)gs_unicorn_hero_ptr, gs_unicorn_hero_pos, -1);
		} else if (state == 17) {
			/* the hero gets heavily wounded, 1 LE left */
			sub_hero_le((struct struct_hero*)gs_unicorn_hero_ptr, ((struct struct_hero*)gs_unicorn_hero_ptr)->le - 1);
			/* the party opens a camp */
			gs_current_loctype = LOCTYPE_WILDCAMP;
			do_location();
		} else if (state == 18) {
			timewarp(HOURS(2));
		} else if (state == 25) {
			/* the UNICORN will get the map */
			gs_unicorn_get_map = 1;

			/* the hero gets 100 AP */
			add_hero_ap((struct struct_hero*)gs_unicorn_hero_ptr, 100);

			/* set the unicorn timer (in days) */
			gs_unicorn_timer = random_schick(24) + 36;
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
			g_dialog_next_state = (gs_swafnild_nomap ? 38 : 39);
		} else if (state == 7) {

			if ((gs_current_town >= TOWNS_THORWAL && gs_current_town <= TOWNS_EFFERDUN) ||
				(gs_current_town >= TOWNS_DASPOTA && gs_current_town <= TOWNS_VAERMHAG))
			{
				gs_swafnild_tp1 = (TOWNS_PREM);
				gs_swafnild_tp2 = (TOWNS_SKJAL);
				gs_swafnild_tp3 = (TOWNS_OTTARJE);
				gs_swafnild_tp4 = (TOWNS_ARYN);

			} else if (gs_current_town >= TOWNS_ROVIK && gs_current_town <= TOWNS_TREBAN) {
				gs_swafnild_tp1 = (TOWNS_OTTARJE);
				gs_swafnild_tp2 = (TOWNS_SKJAL);
				gs_swafnild_tp3 = (TOWNS_PREM);
				gs_swafnild_tp4 = (TOWNS_LJASDAHL);
			} else {
				gs_swafnild_tp1 = (TOWNS_KORD);
				gs_swafnild_tp2 = (TOWNS_TREBAN);
				gs_swafnild_tp3 = (TOWNS_RUNINSHAVEN);
				gs_swafnild_tp4 = (TOWNS_GUDDASUNDEN);
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
			gs_swafnild_nomap = 1;
		} else if (state == 22) {
			/* test CH+3 */
			g_dialog_next_state = (test_attrib(get_hero(0), ATTRIB_CH, 3) > 0 ? 24 : 23);
		} else if (state == 24 || state == 41) {
			/* mark SWAFNILD EGILSDOTTER as done */
			gs_informer_flags[INFORMER_SWAFNILD] = 2;
		} else if (state == 32) {
				/* check if the party already has this map piece */
				if (gs_treasure_maps[6] == 2) g_tmap_double2 = 1;
				/* get the map piece */
				gs_treasure_maps[6] = 1;
				/* each of the heroes gets 10 AP */
				add_hero_ap_all(10);

				show_treasure_map();

		} else if (state == 37) {

			gs_current_town = (gs_swafnild_destination == 1 ? gs_swafnild_tp1 :
						(gs_swafnild_destination == 2 ? gs_swafnild_tp2 : gs_swafnild_tp3));

			switch (gs_current_town) {
				case TOWNS_PREM: gs_x_target_bak        = 22; gs_y_target_bak =  8; break;
				case TOWNS_KORD: gs_x_target_bak        =  4; gs_y_target_bak =  3; break;
				case TOWNS_OTTARJE: gs_x_target_bak     =  9; gs_y_target_bak = 10; break;
				case TOWNS_SKJAL: gs_x_target_bak       = 11; gs_y_target_bak = 11; break;
				case TOWNS_TREBAN: gs_x_target_bak      =  4; gs_y_target_bak = 12; break;
				case TOWNS_RUNINSHAVEN: gs_x_target_bak =  6; gs_y_target_bak =  6; break;
			}

			gs_current_loctype_bak = LOCTYPE_NONE;
			g_swafnild_traveled = 1;

			timewarp_until_midnight();
		} else if (state == 42) {
			gs_swafnild_destination = 1;
		} else if (state == 43) {
			gs_swafnild_destination = 2;
		} else if (state == 44) {
			gs_swafnild_destination = 3;
		}

	} else if (informer == 1) {
		/* EINHORN / UNICORN (second meeting) */

		if (state == 3) {
			signed short i;

			/* search a map part the party does not have */
			for (i = 0; i < 9; i++) {
				if (!gs_treasure_maps[i]) {
					gs_treasure_maps[i] = 2;
					i = 99;
					break;
				}
			}

			/* check if the group had all map parts */
			if (i != 99) {
				g_tmap_double1 = 1;
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
		if (gs_treasure_maps[i] != 0)
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

