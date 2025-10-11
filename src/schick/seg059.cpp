/**
 *	Rewrite of DSA1 v3.02_de functions of seg059 (tavern: main / harbor wrappers)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg059.cpp
 */

#include <stdio.h>
#include <time.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg060.h"
#include "seg073.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg117.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void do_tavern(void)
{
	signed short i;
	signed short done;
	signed short answer;
	signed short l3;
	Bit32s p_money_before;
	Bit32s p_money_after;
	struct inn_descr *tavern;
	signed short bonus;
	time_t timeval;

	done = 0;
	tavern = &g_tavern_descr_table[gs_current_typeindex];

	GUI_print_loc_line(get_tx(gs_current_locdata));

	g_request_refresh = 1;

	if (tavern->quality >= 6 && tavern->quality <= 13) {

		if (gs_day_timer < HOURS(11) && gs_day_timer > HOURS(3)) {

			GUI_output(get_ttx(801));
			leave_location();
			return;
		}

	} else if (gs_day_timer < HOURS(16) && gs_day_timer > HOURS(3)) {

			GUI_output(get_ttx(481));
			leave_location();
			return;
	}

	draw_loc_icons(g_combo_mode == 0 ? 4 : 5, MENU_ICON_TALK, MENU_ICON_ORDER_FOOD, MENU_ICON_APPLY_SKILL, MENU_ICON_LEAVE, MENU_ICON_INN);

	while (!done) {

		if (g_request_refresh != 0) {

			draw_main_screen();
			set_var_to_zero();
			load_ani(27);
			init_ani(0);
			GUI_print_loc_line(get_tx(gs_current_locdata));
			set_audio_track(ARCHIVE_FILE_INN_XMI);
			g_request_refresh = 0;
		}

		handle_gui_input();

		if (gs_tav_cheated_flags[gs_current_typeindex]) {

			GUI_output(get_ttx(472));
			done = 1;
			g_mouse2_event = (g_action = 0);
		}

		if (gs_day_timer < HOURS(11) && gs_day_timer > HOURS(3)) {

			GUI_output(get_ttx(9));
			done = 1;
			g_mouse2_event = (g_action = 0);
		}

		if (g_mouse2_event || g_action == ACTION_ID_PAGE_UP) {

			answer = GUI_radio(get_ttx(469), g_combo_mode == 0 ? 4 : 5,
						get_ttx(343),
						get_ttx(470),
						get_ttx(212),
						get_ttx(471),
						get_ttx(824)) - 1;

			if (answer != -2) {
				g_action = (answer + ACTION_ID_ICON_1);
			}
		}

		if (g_action == ACTION_ID_ICON_1) {
			/* TALK */

			p_money_before = get_party_money();
			talk_tavern();
			p_money_after = get_party_money();

			if (p_money_before != p_money_after) {

				make_valuta_str(g_text_output_buf, p_money_before - p_money_after);
				sprintf(g_dtp2, get_ttx(825), g_text_output_buf);
				GUI_output(g_dtp2);
			}

			if (gs_tlk_tav_followinformer) {

				tavern_follow_informer();
			}

			g_request_refresh = done = 1;
			g_combo_mode = 0;

		} else if (g_action == ACTION_ID_ICON_2) {
			/* EAT AND DRINK */

			p_money_after = count_heroes_in_group() * (6 - tavern->quality / 4);

			p_money_after += tavern->price_mod * p_money_after / 100;
			sprintf(g_dtp2, get_ttx(473), (signed short)p_money_after);

			if (GUI_bool(g_dtp2)) {

				GUI_output(tavern->quality < 5 ? get_ttx(475) : (tavern->quality < 15 ? get_ttx(476) : get_ttx(477)));

				timewarp(HOURS(1));

				for (i = 0; i <= 6; i++) {

					gs_food_message[i] = g_food_message_shown[i] = 0;

					if (((get_hero(i))->typus != HERO_TYPE_NONE) &&
						((get_hero(i))->group_no == gs_current_group))
					{

						l3 = (21 - tavern->quality) * 20;
						if (l3 > 100) {
							l3 = 100;
						}

						(get_hero(i))->hunger -= (unsigned char)l3;

						if ((get_hero(i))->hunger < 0) {
							(get_hero(i))->hunger = 0;
						}

						l3 = (21 - tavern->quality) * 30;
						if (l3 > 100) {
							l3 = 100;
						}

						(get_hero(i))->thirst -= (unsigned char)l3;

						if ((get_hero(i))->thirst < 0) {
							(get_hero(i))->thirst = 0;
						}
					}
				}


				p_money_before = get_party_money();
				p_money_after *= 10L;

				if (p_money_before < p_money_after) {

					GUI_output(get_ttx(474));

					gs_tav_cheated_flags[gs_current_typeindex] = 1;
					done = 1;
					g_combo_mode = 0;

				} else {

					p_money_before -= p_money_after;
					set_party_money(p_money_before);
				}
			}

		} else if (g_action == ACTION_ID_ICON_3) {
			/* USE SKILL */

			time(&timeval);

			/* skill test will be +50 if the game was saved up to 2 minutes ago.
			 * probably to prevent excessive save & reload */
			bonus = (timeval - g_last_save_time) > 120 ? 0 : 50;

			if (GUI_use_skill2(bonus, get_ttx(395)) == -1) {
				done = 1;
				g_combo_mode = 0;
			}

		} else if (g_action == ACTION_ID_ICON_4) {
			/* LEAVE */

			done = 1;
			g_combo_mode = 0;

		} else if (g_action == ACTION_ID_ICON_5) {
			/* VISIT INN */

			if (g_combo_mode != 0) {

				done = 1;
				g_combo_mode = 2;
			}
		}
	}

	copy_palette();
	leave_location();
}

void octopus_attack_wrapper(void)
{
	octopus_attack();
	g_request_refresh = 1;
}

void pirates_attack_wrapper(void)
{
	pirates_attack();
	g_request_refresh = 1;
}

void prolog_ghostship(void)
{
	signed short answer;
	signed short tw_bak;
	signed short bak1;
	signed short bak2;

	tw_bak = g_textbox_width;
	bak1 = g_basepos_x;
	bak2 = g_basepos_y;
	g_textbox_width = 7;
	g_basepos_x = g_basepos_y = 0;

	load_ani(17);
	draw_main_screen();
	init_ani(1);

	load_tx(ARCHIVE_FILE_DNGS_DTX + 1);

	GUI_output(get_tx(18));
	GUI_output(get_tx(19));

	do {
		answer = GUI_radio(get_tx(20), 2, get_tx(21), get_tx(22));

	} while (answer == -1);

	if (answer == 1) {
		g_request_refresh = 0;
		gs_travel_detour = (DUNGEONS_TOTENSCHIFF);
	} else {
		g_request_refresh = 1;
	}

	set_var_to_zero();
	g_textbox_width = tw_bak;
	g_basepos_x = bak1;
	g_basepos_y = bak2;
}

#if !defined(__BORLANDC__)
}
#endif
