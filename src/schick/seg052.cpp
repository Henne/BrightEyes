/**
 *	Rewrite of DSA1 v3.02_de functions of seg052 (citycamp)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg052.cpp
 */
#include <stdio.h>

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
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   camp in a city or a dungeon
 */
void do_citycamp(void)
{
	signed short l_si;
	signed short l_di;
	signed short done;
	signed short answer;
	signed short l3;
	Bit8u* hero;
	signed char hours;
	signed short l5;
	signed short l6;
	signed short l7;
	signed char l8;

	done = 0;

	l3 = ds_writew(REQUEST_REFRESH, 1);

	for (l_si = 0; l_si <= 6; l_si++) {
		ds_writeb(CITYCAMP_MAGICSTATUS + l_si, ds_writeb(CITYCAMP_GUARDSTATUS + l_si, 0));
	}

	for (l_si = 0; l_si < 3; l_si++) {
		ds_writeb(CITYCAMP_GUARDS + l_si, -1);
	}

	draw_loc_icons(5, MENU_ICON_GUARDS, MENU_ICON_APPLY_SKILL, MENU_ICON_MAGIC, MENU_ICON_SLEEP, MENU_ICON_LEAVE);

	while (done == 0) {

		if (ds_readw(REQUEST_REFRESH) != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(36);
			init_ani(0);
			GUI_print_loc_line(get_ttx(306));
			set_audio_track(ARCHIVE_FILE_CAMP_XMI);
			ds_writew(REQUEST_REFRESH, l3 = 0);
		}

		if (l3 != 0) {
			GUI_print_loc_line(get_ttx(306));
			l3 = 0;
		}

		handle_gui_input();

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			/* open citycamp radio menu */
			answer = GUI_radio(get_ttx(307), 5,
						get_ttx(308),
						get_ttx(212),
						get_ttx(310),
						get_ttx(316),
						get_ttx(471)) - 1;

			/* set action on a valid answer */
			if (answer != -2) {
				ds_writew(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_1) {

			answer = -1;

			for (l_si = 0; l_si <= 6; l_si++) {

				if (!ds_readbs(CITYCAMP_MAGICSTATUS + l_si) && check_hero(get_hero(l_si))) {
					ds_writeb(CITYCAMP_GUARDSTATUS + l_si, 0);
					answer = 0;
				}
			}

			if (answer == -1) {
				GUI_output(get_ttx(332));
			} else {

				for (l_si = 0; l_si < 3; l_si++) {

					sprintf(g_dtp2,
						get_ttx(321),
						l_si + 1);

					do {

						answer = select_hero_ok(g_dtp2);

						if (answer != -1 && ds_readbs(CITYCAMP_MAGICSTATUS + answer) != 0) {
							GUI_output(get_ttx(331));
							answer = -1;
						}

						if (answer != -1 && hero_brewing(get_hero(answer))) {
							GUI_output(get_ttx(730));
							answer = -1;
						}

					} while (answer == -1);

					inc_ds_bs_post(CITYCAMP_GUARDSTATUS + answer);
					ds_writebs(CITYCAMP_GUARDS + l_si, (signed char)answer);
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {

			GUI_use_skill2(0, get_ttx(395));

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_3) {

			answer = select_hero_ok(get_ttx(317));

			if (answer != -1 && hero_brewing(get_hero(answer))) {
				GUI_output(get_ttx(730));
				answer = -1;
			}

			if (answer != -1) {

				hero = get_hero(answer);

				if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH) {

					if (ds_readb(CITYCAMP_GUARDSTATUS + answer) != 0) {
						GUI_output(get_ttx(331));
					} else {
						if (ds_readb(CITYCAMP_MAGICSTATUS + answer) != 0) {
							GUI_output(get_ttx(334));
						} else {
							ds_writebs(CITYCAMP_MAGICSTATUS + answer, (signed char)use_magic(hero));
						}
					}
				} else {
					GUI_output(get_ttx(330));
				}
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_4) {

			if (GUI_bool(get_ttx(318))) {

				hours = (signed char)GUI_input(get_ttx(833), 1);

				if (hours > 0) {
					l5 = hours / 3;
					l6 = l5;
					l_di = 0;

					if (ds_readws(CAMP_INCIDENT) == -1) {
						/* with guards: (hours - 1) % chance for an incident */
						/* without guards: (4*hours - 1) % chance for an incident */
						/* For a 1 hour rest with guards this will be 0% chance! */
						/* TODO: maybe change it to random_schick(100) - 1 to fix that */
						if ((ds_readbs(CITYCAMP_GUARDS) == -1 ? 4 * hours : hours) > random_schick(100)) {
							ds_writews(CAMP_INCIDENT, random_schick(3) - 1);
						}
					}

					l8 = 0;
					l7 = hours;

					if (ds_readbs(CITYCAMP_GUARDS + l_di) != -1) {

						sprintf(g_dtp2,
							get_ttx(774),
							(char*)get_hero(ds_readbs(CITYCAMP_GUARDS + l_di)) + 0x10);

						GUI_print_loc_line(g_dtp2);
					}

					do {
						g_food_mod = 1;
						timewarp(HOURS(1));
						g_food_mod = 0;

						l6--;
						l8++;
						l7--;

						if (l_di == ds_readws(CAMP_INCIDENT) && (l5 / 2) >= l6) {
							done = 1;
						}

						if (l6 == 0 && l_di < 2) {

							l6 = l5;
							l_di++;

							if (ds_readbs(CITYCAMP_GUARDS + l_di) != -1) {

								sprintf(g_dtp2,
									get_ttx(774),
									(char*)get_hero(ds_readbs(CITYCAMP_GUARDS + l_di)) + 0x10);

								GUI_print_loc_line(g_dtp2);
							}
						}

					} while (l7 > 0 && done == 0);

					if (done != 0) {

						ds_writew(CAMP_INCIDENT, -1);

						if (ds_readb(CITYCAMP_CITY) == 0) {
							/* in a dungeon */

							g_fig_initiative = 1;
							g_fig_discard = 1;

							do_fight(ds_readws((DCAMPFIGHTS-2) + 2 * random_schick(4)));

							if (ds_readws(GAME_STATE) == GAME_STATE_MAIN) {
								draw_main_screen();
								set_var_to_zero();
								load_ani(36);
								init_ani(0);
								GUI_print_loc_line(get_ttx(306));
								set_audio_track(ARCHIVE_FILE_CAMP_XMI);
								ds_writew(REQUEST_REFRESH, l3 = 0);
							}

						} else {
							/* in a town */
							done = 0;
							loose_random_item(get_hero(get_random_hero()), 100, get_ttx(832));
						}

						if (l7 > 0) {
							g_food_mod = 1;
							timewarp(HOURS(l7));
							g_food_mod = 0;
						}
					}

					if (done == 0) {

						hero = get_hero(0);
						for (l_si = 0; l_si <= 6; l_si++, hero += SIZEOF_HERO) {

							if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
								host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
								ds_readbs(CITYCAMP_GUARDSTATUS + l_si) < 2 &&
								ds_readbs(CITYCAMP_MAGICSTATUS + l_si) != 1)
							{
								GRP_hero_sleep(hero, hours - 10);
							}
						}
					}
				}
				done = 1;
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_5) {
			done = 1;
		}
	}

	gs_current_loctype_bak = LOCTYPE_NONE;

	/* Original-Bug 26: After leaving a camp in town/dungeon-mode, the party is rotated by 180 degrees. This does not make sense. */
	leave_location();
#ifdef M302de_ORIGINAL_BUGFIX
	/* The rotation is performed in the function leave_location().
	 * We fix the bug in a hacky way by simply correcting the rotation afterwards. */
	gs_direction = ((gs_direction + 2) % 4); /* rotate by 180 degrees */
#endif

}

#if !defined(__BORLANDC__)
}
#endif
