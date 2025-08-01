/**
 *	Rewrite of DSA1 v3.02_de functions of seg054 (inn)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg054.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg025.h"
#include "seg027.h"
#include "seg029.h"
#include "seg031.h"
#include "seg047.h"
#include "seg049.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg104.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void talk_inn(void)
{
	do_random_talk(1, 0);
}

/**
 * \brief   returns a pointer to the first brewing hero, who is not in this group, but in this location
 */
Bit8u* get_first_brewing_hero(void)
{
	unsigned char *hero;
	signed short i;

	hero = get_hero(0);
# ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 11: If NPC Curian got separated from the group for brewing a recipe at an inn,
	 * he is stuck in the inn. When the group enters the inn where they left him, no dialog appears.
	 * Also, switch group does not help, as his group cannot be selected.
	 * ("In dieser Gruppe ist momentan niemand in der Lage, etwas zu tun.") */
	for (i = 0; i < 6; i++, hero += SIZEOF_HERO)
# else
	for (i = 0; i < 7; i++, hero += SIZEOF_HERO)
# endif
	{
		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) != gs_current_group &&
			hero_brewing(hero) &&
			host_readbs(hero + HERO_ALCHEMY_INN_ID) == gs_current_typeindex)
		{
			return hero;
		}
	}

	return (Bit8u*)NULL;
}

/**
 * \brief   location handler for inns
 */
void do_inn(void)
{
	signed short i;
	signed short answer;
	signed short done;
	signed short refresh;
	signed short portion_size;
# ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 7: NPC Curian cannot do magic actions in an inn.
	 * Problem: no space allocated for the NPC (= seventh hero). */
	signed char magic_act[6];
# else
	signed char magic_act[7];
# endif
	Bit8u* hero;
	Bit8u *hero2;
	Bit8u *inn_ptr;
	Bit32s party_money;
	Bit32s price;
	signed char stay;
	signed short tw_bak;
	signed short nr_heroes;
	Bit32s price_schlafsaal;
	Bit32s price_einzelzimmer;
	Bit32s price_suite;
	signed short booked_days;
	signed short rested_days;
	Bit8u *tavern_ptr;

	done = 0;
	stay = 0;
	ds_writebs(SLEEP_QUALITY, -1);

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 8:
	 * If a group of >=2 heroes enters an inn where another hero is brewing,
	 * a textbox is shown with the possibility to interrupt the brewing process.
	 * However, this textbox is not shown if a single hero enters the inn. */

	if (gs_group_member_counts[gs_current_group] == 1) {
		/* current group consists only of a single hero */

		hero = get_first_hero_available_in_group();

		if (hero_brewing(hero)) {
			draw_status_line();

			if (host_readbs(hero + HERO_RECIPE_TIMER) != 0) {

				sprintf(g_dtp2,
					get_ttx(733),
					(char*)hero + HERO_NAME2,
					host_readbs(hero + HERO_RECIPE_TIMER),
					(char*)(host_readbs(hero + HERO_RECIPE_TIMER) < 2 ? get_ttx(735) : get_ttx(736)));

				answer = GUI_radio(g_dtp2, 2, get_ttx(734), get_ttx(537));
				/* <HERO> befindet sich inmitten eines alchimistischen Versuchs, der wohl noch <DAYS> Tage dauert.
				 * * Versuch abbrechen
				 * * Weiter brauen lassen
				 */

				if (answer == 1) {
					/* abort brewing */
					do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), 1);
				} else {
					done = 1;
					ds_writew(COMBO_MODE, 0);
					stay = 1;
				}
			} else {
				/* hero brewing, HERO_RECIPE_TIMER == 0. STAFFSPELL_TIMER not checked */
				do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), 0);
			}
		}
	}
	else if ((hero = get_first_brewing_hero())) {
		/* Original-Bug 9:
		 * If a group of heroes enters an inn where more then one hero is brewing (each of them forms his own group),
		 * and the first brewing hero has not spent enough time for the brewing process yet,
		 * you get asked if his brewing process should be aborted.
		 * If you answer 'no', you are not asked for the second brewing hero. Similarly, the other brewing heroes are
		 * not checked for having spent enough time to finalize the brewing. */

		draw_status_line();

		if (host_readbs(hero + HERO_RECIPE_TIMER) != 0) {

			sprintf(g_dtp2,
				get_ttx(733),
				(char*)hero + HERO_NAME2,
				host_readbs(hero + HERO_RECIPE_TIMER),
				(char*)(host_readbs(hero + HERO_RECIPE_TIMER) < 2 ? get_ttx(735) : get_ttx(736)));

			tw_bak = g_textbox_width;
			g_textbox_width = 4;

			answer = GUI_radio(g_dtp2, 2, get_ttx(734), get_ttx(562));
			/* <HERO> befindet sich inmitten eines alchimistischen Versuchs, der wohl noch <DAYS> Tage dauert.
			 * * Versuch abbrechen
			 * * Weiter brauen lassen
			 */

			g_textbox_width = tw_bak;

			if (answer == 1) {
				/* abort brewing */
				do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), 1);

				/* Original-Bug 10:
				 * If a group of heroes enters an inn where more then one hero is brewing
				 * (each of them forms his own group), you get asked if the brewing process of the first hero
				 * should be aborted. If you answer yes, all groups in the inn are merged, which may include
				 * other size-one-groups containing a brewing hero. The brewing process of these other brewing
				 * heroes won't be finalized, no matter if they have spent enough time in the inn or not,
				 * so these heroes are still considered to be brewing. If such an interrupted hero is the
				 * group leader of a group, and that group is selected, strange effects occur. */
				GRP_merge();
			}
		} else {
			/* hero brewing, HERO_RECIPE_TIMER == 0. STAFFSPELL_TIMER not checked */
			do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), 0);

			/* Original-Bug 20:
			 * If a group of heroes enters an inn where more then one hero is brewing
			 * (each of them forms his own group), and one of the brewing heroes has spent enough time in the in
			 * to finish the brewing, his brewing process is finalized. Subsequently, all groups in the inn are
			 * merged, which may include other size-one-groups containing a brewing hero. The brewing process of
			 * these other brewing heroes won't be finalized, no matter if they have spent enough time in the inn
			 * or not, so these heroes are still considered to be brewing. If such an interrupted hero is the
			 * group leader of a group, and that group is selected, strange effects occur. */
			GRP_merge();
		}
	}
#else
	/* fix Original-Bug 8, 9, 10, 20 */
	hero = get_first_hero_available_in_group();
	if (hero_brewing(hero)) {
		/* situation: 'switch groups' just switched to a group consisting of a single hero which has been separated for brewing a long recipe in an inn */
		/* ASSERT */
		/*
		if (gs_group_member_counts[gs_current_group] != 1) {
			D1_INFO("FEHLER: Gruppengroesse eines brauenden Helden ist nicht 1.");
		}
		*/
		draw_status_line();

		if (host_readbs(hero + HERO_RECIPE_TIMER) != 0) {

			sprintf(g_dtp2,
					get_ttx(733),
					(char*)hero + HERO_NAME2,
					host_readbs(hero + HERO_RECIPE_TIMER),
					(char*)(host_readbs(hero + HERO_RECIPE_TIMER) < 2 ? get_ttx(735) : get_ttx(736)));

			answer = GUI_radio(g_dtp2, 2, get_ttx(734), get_ttx(537));
			/* <HERO> befindet sich inmitten eines alchimistischen Versuchs, der wohl noch <DAYS> Tage dauert.
			 * * Versuch abbrechen
			 * * Weiter brauen lassen
			 */

			if (answer == 1) {
				/* abort brewing */
				do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), 1);
			} else {
				done = 1;
				ds_writew(COMBO_MODE, 0);
				stay = 1;
			}
		} else {
			/* hero brewing, HERO_RECIPE_TIMER == 0. STAFFSPELL_TIMER not checked */
			do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), 0);
		}
	} else {
		hero = get_hero(0);
		for (i = 0; i < 7; i++, hero += SIZEOF_HERO) {
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) != gs_current_group &&
					hero_brewing(hero) &&
					host_readbs(hero + HERO_ALCHEMY_INN_ID) == gs_current_typeindex)
			{
				draw_status_line();

				signed int finalize_alchemy = -1;

				if (host_readbs(hero + HERO_RECIPE_TIMER) != 0) {

					sprintf(g_dtp2,
							get_ttx(733),
							(char*)hero + HERO_NAME2,
							host_readbs(hero + HERO_RECIPE_TIMER),
							(char*)(host_readbs(hero + HERO_RECIPE_TIMER) < 2 ? get_ttx(735) : get_ttx(736)));

					tw_bak = g_textbox_width;
					g_textbox_width = 4;

					answer = GUI_radio(g_dtp2, 2, get_ttx(734), get_ttx(562));
					/* <HERO> befindet sich inmitten eines alchimistischen Versuchs, der wohl noch <DAYS> Tage dauert.
					 * * Versuch abbrechen
					 * * Weiter brauen lassen
					 */

					g_textbox_width = tw_bak;

					if (answer == 1) {
						/* abort brewing */
						finalize_alchemy = 1;
					}
				} else {
					/* hero brewing, HERO_RECIPE_TIMER == 0. STAFFSPELL_TIMER not checked */
					finalize_alchemy = 0;
				}
				if (finalize_alchemy != -1) {
					do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), finalize_alchemy);

					signed char group_nr = host_readbs(hero + HERO_GROUP_NO);

					/* ASSERT */
					/*
					if (gs_group_member_counts[group_nr] != 1) {
						D1_INFO("FEHLER: Gruppengroesse eines brauenden Helden ist nicht 1.");
					}
					*/

					/* the following lines are taken (& adjusted) from function GRP_merge (seg049.cpp) */
					gs_groups_direction[group_nr] = 0;
					gs_groups_x_target[group_nr] = 0;
					gs_groups_y_target[group_nr] = 0; /* TODO: remove one of them */
					gs_groups_y_target[group_nr] = 0;
					gs_groups_town[group_nr] = 0;
					gs_groups_dng_index[group_nr] = 0;
					gs_groups_dng_level[group_nr] = 0;
					gs_groups_direction_bak[group_nr] = 0;
					gs_groups_x_target_bak[group_nr] = 0;
					gs_groups_y_target_bak[group_nr] = 0;
					gs_groups_current_loctype_bak[group_nr] = LOCTYPE_NONE;
					gs_groups_town_bak[group_nr] = 0;
					gs_groups_dng_index_bak[group_nr] = 0;
					gs_groups_dng_level_bak[group_nr] = 0;
					gs_group_member_counts[group_nr] = 0;

					gs_group_member_counts[gs_current_group]++;
					host_writeb(hero + HERO_GROUP_NO, gs_current_group);

					GRP_sort_heroes();
				}
			}
		}
	}
#endif

	if (done == 0) {

		refresh = ds_writews(REQUEST_REFRESH, 1);

		draw_loc_icons(ds_readws(COMBO_MODE) == 0 ? 7 : 8, MENU_ICON_TALK, MENU_ICON_ORDER_FOOD, MENU_ICON_BOOK_BED, MENU_ICON_APPLY_SKILL, MENU_ICON_MAGIC, MENU_ICON_SLEEP, MENU_ICON_LEAVE, MENU_ICON_TAVERN);

# ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 7: NPC Curian cannot do magic actions in an inn. */
		for (i = 0; i < 6; i++)
# else
		for (i = 0; i < 7; i++)
# endif
		{
			magic_act[i] = 0;
		}
	}

	while (done == 0) {

		if (ds_readws(REQUEST_REFRESH) != 0) {

			draw_main_screen();

			set_var_to_zero();

			load_ani(4);

			init_ani(0);

			GUI_print_loc_line(get_tx(gs_current_locdata));

			ds_writews(REQUEST_REFRESH, refresh = 0);
		}

		if (refresh != 0) {

			GUI_print_loc_line(get_tx(gs_current_locdata));

			refresh = 0;
		}

		inn_ptr = p_datseg + INN_DESCR_TABLE + SIZEOF_INN_STATS * gs_current_typeindex;

		handle_gui_input();

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			answer = GUI_radio(get_ttx(345), ds_readws(COMBO_MODE) == 0 ? 7 : 8,
						get_ttx(343), get_ttx(470),
						get_ttx(344), get_ttx(212),
						get_ttx(310), get_ttx(316),
						get_ttx(347), get_ttx(823)) - 1;

			if (answer != -2) {
				ds_writews(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_1) {
			talk_inn();
			ds_writews(REQUEST_REFRESH, 1);
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) { /* order food */

			price = count_heroes_in_group() * (6L - host_readws(inn_ptr + INN_STATS_QUALITY) / 4L); /* higher food quality -> higher price */
			price += (price * host_readws(inn_ptr + INN_STATS_PRICE_MOD)) / 100L;

			sprintf(g_dtp2,
				get_ttx(473),
				(signed short)price);

			if (GUI_bool(g_dtp2)) {

				party_money = get_party_money();
				price *= 10;

				if (price < 0) {
					price = 1;
				}

				if (party_money < price) {
					GUI_output(get_ttx(401));
				} else {

					GUI_output(host_readws(inn_ptr + INN_STATS_QUALITY) < 5 ? get_ttx(475) :
							(host_readws(inn_ptr + INN_STATS_QUALITY) < 15 ? get_ttx(476) : get_ttx(477)));

					for (i = 0, hero2 = get_hero(0); i <= 6; i++, hero2 += SIZEOF_HERO) {

						if (host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group &&
							!hero_dead(hero2))
						{
							portion_size = (21 - host_readws(inn_ptr + INN_STATS_QUALITY)) * 20;
							if (portion_size > 100) {
								portion_size = 100;
							}

							sub_ptr_bs(hero2 + HERO_HUNGER, (unsigned char)portion_size);

							if (host_readbs(hero2 + HERO_HUNGER) < 0) {
								host_writebs(hero2 + HERO_HUNGER, 0);
							}

							portion_size = (21 - host_readws(inn_ptr + INN_STATS_QUALITY)) * 30;
							if (portion_size > 100) {
								portion_size = 100;
							}

							sub_ptr_bs(hero2 + HERO_THIRST, (unsigned char)portion_size);

							if (host_readbs(hero2 + HERO_THIRST) < 0) {
								host_writebs(hero2 + HERO_THIRST, 0);
							}
						}
					}

					party_money -= price;
					set_party_money(party_money);
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_3 && ds_readbs(SLEEP_QUALITY) == -1) {

			price_schlafsaal = 5;
			price_einzelzimmer = 30;
			price_suite = 100;
			price_schlafsaal += price_schlafsaal * host_readws(inn_ptr + INN_STATS_PRICE_MOD) / 100;
			price_einzelzimmer += price_schlafsaal * host_readws(inn_ptr + INN_STATS_PRICE_MOD) / 100;
			price_suite += price_schlafsaal * host_readws(inn_ptr + INN_STATS_PRICE_MOD) / 100;

			tw_bak = g_textbox_width;
			g_textbox_width = 5;

			sprintf(g_dtp2,
				get_ttx(397),
				(signed short)price_schlafsaal);

			sprintf(g_dtp2 + 50,
				get_ttx(398),
				(signed short)price_einzelzimmer);

			sprintf(g_dtp2 + 100,
				get_ttx(399),
				(signed short)price_suite);

			ds_writebs(SLEEP_QUALITY, (signed char)GUI_radio(get_ttx(396), 3,
							g_dtp2,
							g_dtp2 + 50,
							g_dtp2 + 100));

			if (ds_readbs(SLEEP_QUALITY) != -1) {
				ds_writebs(BOOKED_INN_DAYS, (signed char)GUI_input(get_ttx(826), 2));
			}

			if (ds_readbs(BOOKED_INN_DAYS) <= 0) {
				ds_writebs(SLEEP_QUALITY, -1);
			}

			g_textbox_width = tw_bak;

			if (ds_readbs(SLEEP_QUALITY) != -1) {

				nr_heroes = count_heroes_in_group();

				price_schlafsaal *= nr_heroes;
				price_einzelzimmer *= nr_heroes;
				price_suite *= nr_heroes;
				party_money = get_party_money();

				price = ds_readbs(SLEEP_QUALITY) == 1 ? price_schlafsaal : (ds_readbs(SLEEP_QUALITY) == 2 ? price_einzelzimmer : price_suite);

				price *= ds_readbs(BOOKED_INN_DAYS);

				if (price < 0) {
					price = 1;
				}

				if (party_money < price) {
					GUI_output(get_ttx(401));
					ds_writebs(SLEEP_QUALITY, -1);
				} else {
					party_money -= price;
					set_party_money(party_money);
					GUI_output(get_ttx(402));
				}
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_4) {

			if (ds_readbs(SLEEP_QUALITY) != -1) {
				GUI_use_skill2(0, get_ttx(395));
				refresh = 1;
			} else {
				GUI_output(get_ttx(346));
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_5) {

			if (ds_readbs(SLEEP_QUALITY) != -1) {

				answer = select_hero_ok(get_ttx(317));

				if (answer != -1) {

					hero = get_hero(answer);

					if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH) {

						if (magic_act[answer] != 0) {
							GUI_output(get_ttx(334));
						} else {
							magic_act[answer] = (signed char)use_magic(hero);
						}
					} else {
						GUI_output(get_ttx(330));
					}
				}
			} else {
				GUI_output(get_ttx(346));
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_6) {

			if (ds_readbs(SLEEP_QUALITY) != -1 && ds_readbs(BOOKED_INN_DAYS) > 0) {

				if (GUI_bool(get_ttx(318))) {
					booked_days = ds_readbs(BOOKED_INN_DAYS);

					if (host_readws(inn_ptr + INN_STATS_QUALITY) < 8) {
						inc_ds_bs_post(SLEEP_QUALITY);
					} else if (host_readws(inn_ptr + INN_STATS_QUALITY) > 15) {
						dec_ds_bs_post(SLEEP_QUALITY);
					}

					done = 1;
					ds_writews(COMBO_MODE, 0);
					g_food_mod = 1;

					do {
						timewarp_until_time_of_day(HOURS(8));
					} while (dec_ds_bs(BOOKED_INN_DAYS));

					g_food_mod = 0;

					hero = get_hero(0);
					for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == gs_current_group)
						{
							if (booked_days > 1) {
								/* Original-Bug 7:
								 * magic_act[6] = 0 writes out of boundary.
								 * Fixed above. */
								magic_act[i] = 0;
							}

							if (magic_act[i] != 1) {

								for (rested_days = 0; rested_days < booked_days; rested_days++)
								{
									GRP_hero_sleep(hero, ds_readbs(SLEEP_QUALITY));
								}
							}
						}
					}
				}
			} else {
				GUI_output(get_ttx(346));
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_7) {

			if (ds_readbs(SLEEP_QUALITY) != -1) {

				if (GUI_bool(get_ttx(400))) {
					done = 1;
					ds_writews(COMBO_MODE, 0);
				}
			} else {
				done = 1;
				ds_writews(COMBO_MODE, 0);
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_8 && ds_readws(COMBO_MODE) != 0) {

			tavern_ptr = p_datseg + TAVERN_DESCR_TABLE + 4 * gs_current_typeindex;

			if (host_readws(tavern_ptr) >= 6 && host_readws(tavern_ptr) <= 13 &&
				gs_day_timer < HOURS(11) && gs_day_timer > HOURS(3)) {
				GUI_output(get_ttx(801));
			} else if ((host_readws(tavern_ptr) < 6 || host_readws(tavern_ptr) > 13) &&
				gs_day_timer < HOURS(16) && gs_day_timer > HOURS(3)) {
				GUI_output(get_ttx(481));
			} else if (ds_readbs(SLEEP_QUALITY) != -1) {

				if (GUI_bool(get_ttx(400))) {
					done = 1;
					ds_writews(COMBO_MODE, 2);
				}
			} else {
				done = 1;
				ds_writews(COMBO_MODE, 2);
			}
		}
	}

	if (!stay) {
		copy_palette();
		leave_location();
	} else {
		GRP_switch_to_next(1);
		draw_status_line();
	}
}

void TLK_herberg(signed short state)
{
	Bit8u *hero = (Bit8u*)get_first_hero_available_in_group();

	if (!state) {
		g_dialog_next_state = (gs_herberg_kicked_flags[gs_current_typeindex] ? 1 : 2);
	} else if (state == 1 || state == 14) {
		gs_herberg_kicked_flags[gs_current_typeindex] = 1;
	} else if (state == 11) {
		tumult();
		gs_town_outlawed_flags[gs_current_town] = 1;
		gs_herberg_kicked_flags[gs_current_typeindex] = 1;
	} else if (state == 12) {
		/* CH + 5 */
		g_dialog_next_state = (test_attrib(hero, ATTRIB_CH, 5) > 0 ? 14 : 11);
	} else if (state == 13) {
		/* CH + 0 */
		g_dialog_next_state = (test_attrib(hero, ATTRIB_CH, 0) > 0 ? 14 : 7);
	} else if (state == 15) {
		/* CH - 3 */
		g_dialog_next_state = (test_attrib(hero, ATTRIB_CH, -3) > 0 ? 16 : 17);
	} else if (state == 17) {
		ds_writew(ACTION, ACTION_ID_ICON_2);
	}
}

#if !defined(__BORLANDC__)
}
#endif
