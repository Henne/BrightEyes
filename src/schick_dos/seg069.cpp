/**
 *	Rewrite of DSA1 v3.02_de functions of seg069 (special buildings: Thorwal 2/2)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg069.cpp
 */
#include "v302de.h"
#include "common.h"

#include "seg004.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg032.h"
#include "seg054.h"
#include "seg059.h"
#include "seg097.h"
#include "seg105.h"

signed int g_combo_mode; // ds:0xe42c

void THO_hetmann(void)
{
	signed int answer;
	signed int i;
	signed int map_parts;

	load_in_head(66);

	if (!gs_got_letter_het && gs_got_main_quest && (gs_jurge_awaits_letter || gs_need_letter))
	{

		/* count already collected parts of the map */
		for (i = map_parts = 0; i < 9; i++) {
			if (gs_treasure_maps[i] != 0) {
				map_parts++;
			}
		}

		load_in_head(61);

		GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), get_tx(82), 0);

		GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), get_tx(83), 0);

		GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), gs_min_map_parts <= map_parts ? get_tx(84) : get_tx(85), 0);

		answer = gs_min_map_parts <= map_parts ? 10 : 50;

		for (i = 0; i < 14; i++) {
			gs_gods_estimation[i] += answer;
		}

		GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), get_tx(86), 0);

		gs_got_letter_het = 1;
		give_new_item_to_group(ITEM_ID_EMPFEHLUNGSSCHREIBEN__HETMAN, 1, 1);

	} else if (!gs_heard_announce || gs_got_main_quest != 0) {

		GUI_output(get_tx2(0));

	} else {
		gs_subvention = 0;

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
					get_tx2(1), 3,
					get_tx2(2),
					get_tx2(3),
					get_tx2(4));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(5), 0);

		} else if (answer == 2) {

			GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(6), 0);

			gs_subvention += 2;

		} else {

			GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(7), 0);

			gs_subvention++;
		}

		load_in_head(61);

		GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), get_tx2(8), 0);

		gs_heard_announce = 3;

		GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), get_tx2(9), 0);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85),
					get_tx2(10), 3,
					get_tx2(11),
					get_tx2(12),
					get_tx2(13));
		} while (answer == -1);

		if (answer == 1) {

			GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), get_tx2(14), 0);

			gs_informer_flags[INFORMER_ID_ISLEIF] = 1;
			gs_subvention++;
			gs_got_main_quest = 1;
			gs_quested_months = 0;

		} else if (answer == 2) {

			GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), get_tx2(15), 0);

			gs_informer_flags[INFORMER_ID_ISLEIF] = 1;
			gs_got_main_quest = 1;
			gs_quested_months = 0;
			gs_got_letter_het = 1;
			give_new_item_to_group(ITEM_ID_EMPFEHLUNGSSCHREIBEN__HETMAN, 1, 1);

		} else if (answer == 3) {

			gs_subvention += 2;

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85),
						get_tx2(16), 2,
						get_tx2(17),
						get_tx2(18));
			} while (answer == -1);

			if (answer == 1) {
				GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(85), get_tx2(19), 0);

				gs_got_main_quest = 1;
				gs_quested_months = 0;
			}
		}
	}
}

void THO_windriders(void)
{
	const signed int answer = GUI_radio(get_tx2(20), 3, get_tx2(21), get_tx2(22), get_tx2(23));

	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_tx2(24));

		} else if (answer == 2 || answer == 3) {

			if (!gs_otta_windriders) {

				GUI_output(answer == 2 ? get_tx2(25): get_tx2(26));

			} else if (gs_otta_windriders == 1) {

				GUI_output(get_tx2(27));

			} else {

				GUI_output(get_tx2(28));

				g_fig_discard = 1;
				do_fight(FIGHT_ID_THOR8);
			}

			gs_otta_windriders++;
		}
	}
}

void THO_tav_inn_combi(void)
{
	signed int answer;
	signed int type_bak;

	disable_ani();
	load_ani(4);
	init_ani(0);

	answer = GUI_radio(get_ttx(673), 3, get_ttx(674), get_ttx(675), get_ttx(347));

	/* save the combo typeindex */
	type_bak = gs_town_typeindex;

	do {

		/* restore the combo typeindex */
		gs_town_typeindex = type_bak;

		if (answer == 1) {

			/* enter TAVERN */

			/* set combo_mode active */
			g_combo_mode = 1;

			/* set the typeindex of the corresponding tavern */
			answer = gs_town_typeindex;
			gs_town_typeindex = (answer == 11 ? 0 : (answer == 14 ? 1 : (answer == 17 ? 2 : 6)));

			g_textbox_width = 3;

			gs_town_loc_type = LOCTYPE_TAVERN;
			do_tavern();

			/* leave the loop or enter the inn in the next iteration */
			answer = 0;
			if (g_combo_mode == 2) {
				answer = 2;
			}

		} else if (answer == 2) {

			/* enter INN */

			/* set combo_mode active */
			g_combo_mode = 1;

			/* set the typeindex of the corresponding inn */
			answer = gs_town_typeindex;
			gs_town_typeindex = (answer == 11 ? 70 : (answer == 14 ? 71 : (answer == 17 ? 72 : 73)));

			g_textbox_width = 3;

			gs_town_loc_type = LOCTYPE_INN;
			do_inn();

			/* leave the loop or enter the tavern in the next iteration */
			answer = 0;
			if (g_combo_mode == 2) {
				answer = 1;
			}

		} else {
			disable_ani();
			return;
		}

	} while (answer != 0);

	disable_ani();
	leave_location();
}


void THO_stormchildren(void)
{
	const signed int answer = GUI_radio(get_tx2(29), 3, get_tx2(30), get_tx2(31), get_tx2(32));

	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_tx2(33));

		} else if (answer == 2 || answer == 3) {

			if (gs_otta_stormchildren == 0) {

				GUI_output(answer == 2 ? get_tx2(34): get_tx2(35));

			} else if (gs_otta_stormchildren == 1) {

				GUI_output(get_tx2(36));

			} else {

				GUI_output(get_tx2(37));

				g_fig_discard = 1;
				do_fight(FIGHT_ID_THOR8);
			}

			gs_otta_stormchildren++;
		}
	}
}

void THO_garaldsson(void)
{
	const signed int answer = GUI_radio(get_tx2(38), 3, get_tx2(39), get_tx2(40), get_tx2(41));

	if (answer != -1) {

		if (answer == 1) {

			GUI_output(get_tx2(42));

		} else if (answer == 2 || answer == 3) {

			if (gs_otta_garaldsson == 0) {

				GUI_output(answer == 2 ? get_tx2(43): get_tx2(44));

			} else if (gs_otta_garaldsson == 1) {

				GUI_output(get_tx2(45));

			} else {

				GUI_output(get_tx2(46));

				g_fig_discard = 1;
				do_fight(FIGHT_ID_THOR8);
			}

			gs_otta_garaldsson++;
		}
	}
}
