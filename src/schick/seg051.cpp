/*
 *	Rewrite of DSA1 v3.02_de functions of seg051 (wilderness camp)
 *	Functions rewritten: 3/3 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg051.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg032.h"
#include "seg047.h"
#include "seg049.h"
#include "seg051.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg104.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void do_wildcamp(void)
{
	signed short l_si;
	signed short l_di;
	signed short i;
	signed short done;
	signed short answer;
	signed char stock_tries;
	signed char herb_tries;
	signed char herb_hours;
	Bit8u* hero;
	signed char l3;
	signed short l4;
	signed short l5;
	signed short l6;
	signed char have_guards;
	signed char l8;

	done = 0;
	stock_tries = 0;
	herb_tries = 0;
	l_di = g_request_refresh = 1;

	for (i = 0; i <= 6; i++) {

		g_wildcamp_herbstatus[i] = g_wildcamp_replstatus[i] =
			g_wildcamp_magicstatus[i] = g_wildcamp_guardstatus[i] = 0;
	}

	for (i = 0; i < 3; i++) {
		g_wildcamp_guards[i] = -1;
	}

	i = !g_good_camp_place ? 6 : 7;
	draw_loc_icons(i, MENU_ICON_GUARDS, MENU_ICON_REPLENISH_SUPPLIES, MENU_ICON_APPLY_SKILL, MENU_ICON_MAGIC, MENU_ICON_GATHER_HERBS, MENU_ICON_SLEEP, MENU_ICON_QUIT_CAMP);

	while (done == 0) {

		if (g_request_refresh) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(2);
			init_ani(0);
			GUI_print_loc_line(get_ttx(306));
			set_audio_track(ARCHIVE_FILE_CAMP_XMI);
			g_request_refresh = l_di = 0;
		}

		if (l_di) {
			GUI_print_loc_line(get_ttx(306));
			l_di = 0;
		}

		handle_gui_input();

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			i = !g_good_camp_place ? 6 : 7;

			answer = GUI_radio(get_ttx(307), (signed char)i,
						get_ttx(308), get_ttx(309),
						get_ttx(212), get_ttx(310),
						get_ttx(315), get_ttx(316),
						get_ttx(814)) -1;

			if (answer != -2) {
				ds_writews(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_1) {

			answer = -1;

			for (i = 0; i <= 6; i++) {

				if (!g_wildcamp_magicstatus[i] && !g_wildcamp_herbstatus[i] &&
					!g_wildcamp_replstatus[i] && is_hero_available_in_group(get_hero(i)))
				{
					g_wildcamp_guardstatus[i] = 0;
					answer = 0;
				}
			}

			if (answer == -1) {
				GUI_output(get_ttx(332));
			} else {

				for (i = 0; i < 3; i++) {

					sprintf(g_dtp2, get_ttx(321), i + 1);

					do {
						answer = select_hero_ok(g_dtp2);

						/* Original-Bug: not checked answer for following options */
						if ((answer != -1 &&
							g_wildcamp_magicstatus[answer] != 0) ||
							g_wildcamp_herbstatus[answer] != 0 ||
							g_wildcamp_replstatus[answer] != 0)
						{
							GUI_output(get_ttx(331));
							answer = -1;
						}

						if (answer != -1) {
							if (hero_brewing(get_hero(answer))) {
								GUI_output(get_ttx(730));
								answer = -1;
							}
						}

					} while (answer == -1);

					g_wildcamp_guardstatus[answer]++;
					g_wildcamp_guards[i] = answer;
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {

			if (g_good_camp_place == 99) {
				l_di = replenish_stocks(g_replenish_stocks_mod + 99, stock_tries);
			} else {
				l_di = replenish_stocks(g_replenish_stocks_mod, stock_tries);
			}

			if (l_di) {
				stock_tries++;
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_3) {

			GUI_use_skill2(0, get_ttx(395));

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_4) {

			answer = select_hero_ok(get_ttx(317));

			if (answer != -1) {

				if (hero_brewing(get_hero(answer))) {
					GUI_output(get_ttx(730));
					answer = -1;
				}
			}

			if (answer != -1) {

				hero = get_hero(answer);

				if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH) {

					if (g_wildcamp_guardstatus[answer] != 0 ||
						g_wildcamp_herbstatus[answer] != 0 ||
						g_wildcamp_replstatus[answer] != 0)
					{
						GUI_output(get_ttx(331));

					} else {

						if (g_wildcamp_magicstatus[answer] != 0) {

							GUI_output(get_ttx(334));

						} else {
							g_wildcamp_magicstatus[answer] = use_magic(hero);
						}
					}
				} else {
					GUI_output(get_ttx(330));
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_5) {
			/* COLLECT HERBS */

			g_skilled_hero_pos = get_skilled_hero_pos(TA_PFLANZENKUNDE);

			answer = select_hero_ok(get_ttx(326));

			if (answer != -1 && hero_brewing(get_hero(answer))) {
				GUI_output(get_ttx(730));
				answer = -1;
			}

			if (answer != -1) {

				if (g_wildcamp_replstatus[answer])
				{

					sprintf(g_dtp2,	get_ttx(803), (char*)get_hero(answer) + HERO_NAME2);
					GUI_output(g_dtp2);

				} else if (g_wildcamp_guardstatus[answer] != 0 ||
						g_wildcamp_replstatus[answer] != 0 ||
						g_wildcamp_magicstatus[answer] != 0)
				{
					GUI_output(get_ttx(331));

				} else {

					if (herb_tries < 1)
					{
						hero = get_hero(answer);

						herb_hours = (signed char)GUI_input(get_ttx(327), 1);

						if (herb_hours > 0)
						{
							g_wildcamp_herbstatus[answer] = herb_tries = (signed char)(l_di = 1);

							if (g_good_camp_place == 99) {
								gather_herbs(hero, herb_hours - 1, g_gather_herbs_mod + 99);
							} else {
								gather_herbs(hero, herb_hours - 1, g_gather_herbs_mod);
							}
						}
					} else {
						GUI_output(get_ttx(336));
					}
				}
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_6) {
			/* Sleep */

			if (GUI_bool(get_ttx(318))) {

				l3 = (signed char)(gs_day_timer / HOURS(1));

				l3 = gs_day_timer < HOURS(8) ? 8 - l3 : 24 - l3 + 8;

				l4 = l3 / 3;
				l5 = l4;
				l_si = 0;
				have_guards = 0;

				if (gs_camp_incident == -1) {

					if (((g_wildcamp_guards[0] == -1 ? 60 : 10) > random_schick(100)) && !gs_ingame_timers[INGAME_TIMER_TRAVIA_SAFE_REST])
					{
						gs_camp_incident = random_schick(3) - 1;
					}
				} else {
					have_guards = 1;
				}

				l8 = 0;
				l6 = l3;

				if (g_wildcamp_guards[l_si] != -1) {

					sprintf(g_dtp2, get_ttx(774),
						(char*)get_hero(g_wildcamp_guards[l_si]) + HERO_NAME2);

					GUI_print_loc_line(g_dtp2);
				}

				do {

					g_food_mod = 1;
					timewarp(HOURS(1));
					g_food_mod = 0;
					l5--;
					l8++;
					l6--;

					if (l_si == gs_camp_incident && l4 / 2 >= l5) {
						done = 1;
					}

					if (l5 == 0 && l_si < 2) {

						l5 = l4;
						l_si++;

						if (g_wildcamp_guards[l_si] != -1) {

							sprintf(g_dtp2,	get_ttx(774),
								(char*)get_hero(g_wildcamp_guards[l_si]) + HERO_NAME2);
							GUI_print_loc_line(g_dtp2);
						}
					}

				} while (l6 > 0 && done == 0);

				if (done == 0) {

					hero = get_hero(0);

					for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {
						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
							g_wildcamp_guardstatus[i] < 2 &&
							g_wildcamp_magicstatus[i] != 1)
						{
							GRP_hero_sleep(hero, g_wildcamp_sleep_quality);
						}
					}

				} else if (!have_guards) {

					gs_camp_incident = -1;
					g_fig_initiative = 1;
					g_fig_discard = 1;

					/* pick a random campfight out of 4 possibilities */
					do_fight(ds_readws((CAMPFIGHTS-2) + 2 * random_schick(4)));

					if (gs_travel_detour != 99 && g_game_state == GAME_STATE_MAIN) {

						draw_main_screen();
						set_var_to_zero();
						load_ani(2);
						init_ani(0);
						GUI_print_loc_line(get_ttx(306));
						set_audio_track(ARCHIVE_FILE_CAMP_XMI);

						g_request_refresh = l_di = 0;

						if (l6 > 0) {
							g_food_mod = 1;
							timewarp_until_time_of_day(HOURS(8));
							g_food_mod = 0;
						}
					}
				} else {
					gs_camp_incident = g_wildcamp_guards[gs_camp_incident];
				}

				done = 1;
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_7) {
			done = 1;
		}
	}

	for (i = 0; i <= 6; i++) {
		g_wildcamp_herbstatus[i] = g_wildcamp_replstatus[i] = 0;
	}

	leave_location();
}

signed short gather_herbs(Bit8u *hero, signed short hours, signed short handicap)
{
	signed short herb_index;
	signed short unique_herbs_count;
	Bit8u *ptr;
	signed char herb_count[12];

	memset(herb_count, 0 , 12);

	timewarp(HOURS((hours + 1)));

	ptr = p_datseg + GATHER_HERBS_TABLE;

	for (unique_herbs_count = herb_index = 0; herb_index < 12; herb_index++, ptr += SIZEOF_GATHER_HERBS) {

		/* check if this is a special place for collecting the considered herb.. */
		if (host_readb(ptr + GATHER_HERBS_ITEM_ID) == ds_readb(GATHER_HERBS_SPECIAL)) {
			/* dirty code follows. The original herbs table is modified. */
			add_ptr_bs(ptr + GATHER_HERBS_CHANCE, 10); // 10% higher chance to find the herb
			inc_ptr_bs(ptr + GATHER_HERBS_MAX_COUNT);  // increase maximum count of single herbs by 1.
		}

		if (random_schick(100) <= host_readb(ptr + GATHER_HERBS_CHANCE) &&
			test_skill(hero, TA_PFLANZENKUNDE, host_readb(ptr + GATHER_HERBS_HANDICAP) - hours + handicap) > 0) {

			herb_count[herb_index] = (signed char)give_hero_new_item(hero, host_readb(ptr + GATHER_HERBS_ITEM_ID), 0, random_schick(host_readb(ptr + GATHER_HERBS_MAX_COUNT))); // collect a random amount between 1 and max_count herbs.

			if (herb_count[herb_index] != 0) {
				unique_herbs_count++;
			}
		}

		if (host_readb(ptr) == ds_readb(GATHER_HERBS_SPECIAL)) {
			/* The herbs table is reverted to original state. */
			sub_ptr_bs(ptr + GATHER_HERBS_CHANCE, 10);
			dec_ptr_bs(ptr + GATHER_HERBS_MAX_COUNT);
		}
	}

	if (unique_herbs_count) {

		/* print a sentence with all the herb names */
		sprintf(g_dtp2,
			get_ttx(328),
			(char*)hero + HERO_NAME2);

		for (herb_index = 0; herb_index < 12; herb_index++) {

			if (herb_count[herb_index] != 0) {

				sprintf(g_text_output_buf,
					(char*)p_datseg + GATHER_HERBS_STR_FOUND, /* "%d %s" */
					herb_count[herb_index],
					(Bit8u*)(GUI_names_grammar((herb_count[herb_index] > 1 ? 4 : 0) + 0x4002, ds_readb(GATHER_HERBS_TABLE + 4 * herb_index + GATHER_HERBS_ITEM_ID), 0)));

				strcat(g_dtp2, g_text_output_buf);

				if (--unique_herbs_count > 1) {
					/* add a comma ", " */
					strcat(g_dtp2, (char*)p_datseg + GATHER_HERBS_STR_COMMA);
				} else if (unique_herbs_count == 1) {
					/* add an and " UND " */
					strcat(g_dtp2, (char*)p_datseg + GATHER_HERBS_STR_AND);
				}
			}
		}

		/* add a dot "." */
		strcat(g_dtp2, (char*)p_datseg + GATHER_HERBS_STR_DOT);

	} else {

		/* no herbs found */

		sprintf(g_dtp2,
			get_ttx(342),
			(char*)hero + HERO_NAME2);
	}

	GUI_output(g_dtp2);

	return 0;
}

/**
 * \brief   replenish the stocks (water and food)
 *
 * \param   mod         modificator for the skill test
 * \param   tries       how often was tried to replenish stocks
 * \return              0 if replenish was not possible or 1 if replenish was possible
 */
signed short replenish_stocks(signed short mod, signed short tries)
{
	signed short hero_pos;
	signed short l_di;
	signed short retval;
	signed short j;
	Bit8u* hero;
	Bit8u *hero2;

	retval = 0;
	mod += 5;

	g_skilled_hero_pos = get_skilled_hero_pos(TA_WILDNISLEBEN);
	hero_pos = select_hero_ok(get_ttx(322));

	if (hero_pos != -1 && hero_brewing(get_hero(hero_pos))) {
		GUI_output(get_ttx(730));
		hero_pos = -1;
	}

	if (hero_pos != -1) {

		if (g_wildcamp_replstatus[hero_pos] != 0) {

			sprintf(g_dtp2, get_ttx(802), (char*)get_hero(hero_pos) + HERO_NAME2);
			GUI_output(g_dtp2);

		} else {

			if (g_wildcamp_replstatus[hero_pos] != 0 ||
				g_wildcamp_magicstatus[hero_pos] != 0 ||
				g_wildcamp_guardstatus[hero_pos] != 0)
			{
				GUI_output(get_ttx(331));

			} else {

				if (tries < 2) {

					timewarp(HOURS(1));
					gs_main_acting_hero = hero = get_hero(hero_pos);
					g_wildcamp_replstatus[hero_pos] = 1;
					retval = 1;

					/* search for water */
					if ((test_skill(hero, TA_WILDNISLEBEN, (signed char)mod) > 0) || gs_ingame_timers[INGAME_TIMER_EFFERD_FIND_WATER]) {

						/* found water */
						sprintf(g_dtp2, get_ttx(324), (char*)hero + HERO_NAME2);

						/* fill up all waterskins and remove thirst of all living heroes in the current group */
						hero2 = get_hero(0);
						for (l_di = 0; l_di <= 6; l_di++, hero2 += SIZEOF_HERO) {
							if (host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group &&
								!hero_dead(hero2))
							{
								host_writebs(hero2 + HERO_THIRST, 0);

								for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {
									if (host_readws(hero2 + SIZEOF_INVENTORY * j + HERO_INVENTORY + INVENTORY_ITEM_ID) == ITEM_WATERSKIN) {
										and_ptr_bs(hero2 + SIZEOF_INVENTORY * j + HERO_INVENTORY + INVENTORY_FLAGS, 0xfb); /* unset 'empty' flag */
										and_ptr_bs(hero2 + SIZEOF_INVENTORY * j + HERO_INVENTORY + INVENTORY_FLAGS, 0xfd); /* unset 'half_empty' flag */
									}
								}
							}
						}
					} else {

						sprintf(g_dtp2, get_ttx(340), (char*)hero + HERO_NAME2);
					}

					GUI_print_loc_line(g_dtp2);
					delay_or_keypress(200);

					/* search for food */
					if ((test_skill(hero, TA_FAEHRTENSUCHEN, (signed char)mod) > 0) || gs_ingame_timers[INGAME_TIMER_FIRUN_HUNT]) {

						/* remove hunger of all living heroes in the current group */
						hero2 = get_hero(0);
						for (l_di = 0; l_di <= 6; l_di++, hero2 += SIZEOF_HERO) {
							if (host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group &&
								!hero_dead(hero2))
							{
								host_writebs(hero2 + HERO_HUNGER, 0);
							}
						}

						/* the group may get three food packages */
						if (!get_item(ITEM_FOOD_PACKAGE, 1, 3)) {
							strcpy(g_dtp2, get_ttx(306));
							g_request_refresh = 1;
						} else {
							sprintf(g_dtp2,
								get_ttx(325),
								(char*)hero + HERO_NAME2);
						}

					} else {

						sprintf(g_dtp2,
							get_ttx(341),
							(char*)hero + HERO_NAME2);
					}

					GUI_print_loc_line(g_dtp2);
					delay_or_keypress(200);

				} else {
					GUI_output(get_ttx(323));
				}
			}
		}
	}

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
