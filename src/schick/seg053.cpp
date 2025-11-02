/*
 *	Rewrite of DSA1 v3.02_de functions of seg053 (healer)
 *	Functions rewritten: 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg053.cpp
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
#include "seg047.h"
#include "seg053.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static const struct healer_descr g_healer_descr_table[41] = {
	{  30,  2 },
	{  50, 10 },
	{ -10, 10 },
	{  10, 12 },
	{  30, 12 },
	{  30,  9 },
	{   0, 12 },
	{   0,  4 },
	{ -10,  6 },
	{  40, 15 },
	{   0,  7 },
	{   0, 16 },
	{  20,  3 },
	{  40,  5 },
	{  20, 13 },
	{ -10,  4 },
	{  20, 10 },
	{   0, 14 },
	{  30,  1 },
	{  20,  8 },
	{   0, 12 },
	{  10, 10 },
	{  50,  6 },
	{  20,  2 },
	{ -10,  7 },
	{   0,  7 },
	{  10, 14 },
	{   0, 12 },
	{  20,  7 },
	{  40,  4 },
	{ -20,  8 },
	{  10, 15 },
	{ -50, 12 },
	{  10,  3 },
	{ -20, 10 },
	{ -20, 11 },
	{  30, 12 },
	{  20, 17 },
	{  20,  6 },
	{ -10,  3 },
	{  -1,  0 }
}; // ds:0x66ea

/**
 * \brief   check if hero can healed by a healer
 *
 * \param   hero        pointer to hero
 * \return              1 if hero can be healed, else 0.
 */
signed short is_hero_healable(const struct struct_hero *hero)
{

	if (hero->flags.dead || hero->flags.petrified) {

		/* this hero can not be helped */
		GUI_output(get_ttx(778));

		return 0;
	} else {
		return 1;
	}
}

void do_healer(void)
{
	signed char motivation;
	signed short leave_healer;
	signed short request_refresh;
	struct struct_hero *hero;
	signed long money;
	signed long price;
	const struct healer_descr *healer;
	signed short poison;

	signed short answer;
	signed short disease;

	motivation = 0;
	leave_healer = 0;

#if !defined(__BORLANDC__)
	const Bit8u typi = gs_current_typeindex;
	const Bit8s h_price = g_healer_descr_table[typi].price_mod;
	const Bit8u h_qual = g_healer_descr_table[typi].quality;

	D1_INFO("Heiler: %2d\n", typi);
	D1_INFO("\tPreis: %d%% [50,150]\n", h_price + 100);
	D1_INFO("\tQualitaet: %2d [1,15]\n", h_qual);
#endif

	request_refresh = g_request_refresh = 1;
	healer = &g_healer_descr_table[gs_current_typeindex];
	draw_loc_icons(4, MENU_ICON_HEAL_WOUNDS, MENU_ICON_HEAL_DISEASE, MENU_ICON_HEAL_POISON, MENU_ICON_LEAVE);

	while (leave_healer == 0) {

		if (g_request_refresh != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(23);
			init_ani(0);

			GUI_print_loc_line(get_tx(gs_current_locdata));

			set_audio_track(ARCHIVE_FILE_HEALER_XMI);

			g_request_refresh = request_refresh = 0;

			if (!motivation) {

				motivation = 1;

				/* from 9.00 pm to 6.00 am the healer gets unkind */
				if (gs_day_timer > HOURS(21) || gs_day_timer < HOURS(6)) {
					GUI_output(get_ttx(484));
					motivation = 2;
				}
			}
		}

		if (request_refresh != 0) {
			GUI_print_loc_line(get_tx(gs_current_locdata));
			request_refresh = 0;
		}

		handle_gui_input();

		if (g_mouse_rightclick_event || g_action == ACTION_ID_PAGE_UP) {

			g_textbox_width = 4;

			answer = GUI_radio(get_ttx(459), 4, get_ttx(455), get_ttx(456), get_ttx(457), get_ttx(458)) - 1;

			g_textbox_width = 3;

			if (answer != -2) {
				g_action = answer + ACTION_ID_ICON_1;
			}
		}

		if (g_action == ACTION_ID_ICON_4) {
			leave_healer = 1;
			continue;
		}

		if (g_action == ACTION_ID_ICON_1) {

			/* Heal Wounds */

			money = get_party_money();
			answer = select_hero_from_group(get_ttx(460));
			if (answer != - 1) {

				hero = get_hero(answer);

				if (is_hero_healable(hero)) {

					/* LEmax >= LE and no permanent LEdamage */
					if ((hero->le >= hero->le_max) && !hero->le_malus) {

						/* Hero seems OK */
						sprintf(g_dtp2,	get_ttx(461), hero->alias);
						GUI_output(g_dtp2);
					} else {

						/* calculate price */
						price = hero->le_malus * 50;
						price += (hero->le_max + hero->le_malus - hero->le) * 5;
						price += healer->price_mod * price / 100;

						if (motivation == 2)
							price *= 2;

						/* ask */
						sprintf(g_dtp2,	get_ttx(464), hero->alias, price);

						if (GUI_bool(g_dtp2)) {

							price *= 10;

							if (money < price) {

								GUI_output(get_ttx(401));

							} else {
								/* remove money */
								money -= price;
								set_party_money(money);

								/* heal permanent damage TODO:LE += */
								hero->le_max += hero->le_malus;
								hero->le_malus = 0;

								/* time passes by (number of missing LE) minutes */
								timewarp(MINUTES((signed long)(hero->le_max - hero->le)));

								/* heal LE */
								add_hero_le(hero, hero->le_max);

								/* prepare output */
								sprintf(g_dtp2,	get_ttx(467), hero->alias);
								GUI_output(g_dtp2);
							}
						}
					}
				}
			}

		} else if (g_action == ACTION_ID_ICON_2) {

			/* Cure Disease */
			money = get_party_money();
			answer = select_hero_from_group(get_ttx(460));
			if (answer != -1) {

				hero = get_hero(answer);

				if (is_hero_healable(hero)) {

					disease = hero_is_diseased(hero);

					if (!disease) {

						/* Hero is not diseased */
						sprintf(g_dtp2,	get_ttx(462), hero->alias);
						GUI_output(g_dtp2);

					} else {
						/* calculate price */
						price = g_disease_prices[disease] * 10;

						price += healer->price_mod * price / 100;

						if (motivation == 2)
							price *= 2;

						/* prepare output */
						sprintf(g_dtp2, get_ttx(465), hero->alias, get_ttx(disease + 0x193), price);

						if (GUI_bool(g_dtp2)) {

							price *= 10;

							if (money < price) {
								GUI_output(get_ttx(401));
							} else {
								timewarp(HOURS(1));

								if (random_schick(100) <= (120 - 10 * healer->quality) + g_disease_delays[disease]) {
									/* heal the disease */
									hero->sick[disease][0] = 1;
									hero->sick[disease][1] = 0;

									/* prepare output */
									sprintf(g_dtp2,	get_ttx(467), hero->alias);
									GUI_output(g_dtp2);
								} else {
									price /= 2;
									GUI_output(get_ttx(468));
								}

								/* remove money */
								money -= price;
								set_party_money(money);
							}
						}
					}
				}
			}

		} else if (g_action == ACTION_ID_ICON_3) {

			/* Heal Poison */
			money = get_party_money();
			answer = select_hero_from_group(get_ttx(460));
			if (answer != -1) {

				hero = get_hero(answer);

				if (is_hero_healable(hero)) {

					poison = hero_is_poisoned(hero);

					if (poison == 0) {

						/* Hero is not poisoned */
						sprintf(g_dtp2,	get_ttx(463), hero->alias);
						GUI_output(g_dtp2);
					} else {
						/* calculate price */
						price = g_poison_prices[poison] * 20;
						price += healer->price_mod * price / 100;
						if (motivation == 2)
							price *= 2;

						/* prepare output */
						sprintf(g_dtp2, get_ttx(466), price, hero->alias);

						if (GUI_bool(g_dtp2)) {

							price *= 10;

							if (money < price) {

								GUI_output(get_ttx(401));

							} else {
								timewarp(HOURS(1));

								if (random_schick(100) <= (120 - 5 * healer->quality) + g_poison_delays[poison]) {

									/* cure the poison */
									hero->poison[poison][1] = 0;
									hero->poison[poison][0] = 1;

									/* prepare output */
									sprintf(g_dtp2,	get_ttx(467), hero->alias);
									GUI_output(g_dtp2);

								} else {
									price /= 2;
									GUI_output(get_ttx(468));
								}

								money -= price;
								set_party_money(money);
							}
						}
					}
				}
			}
		}
	}

	copy_palette();
	leave_location();

}

#if !defined(__BORLANDC__)
}
#endif
