/**
 *	Rewrite of DSA1 v3.02_de functions of seg055 (merchant: main)
 *	Functions rewritten: 6/6 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg055.cpp
 */
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg031.h"
#include "seg055.h"
#include "seg056.h"
#include "seg057.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   add an item to the shop
 *
 * \param   shop_ptr    pointer to the shop descrition
 * \param   item_id     item_id
 * \param   pos         position in the shop
 */
void add_item_to_shop(Bit8u *shop_ptr, signed short item_id, signed short pos)
{
	host_writews((Bit8u*)ds_readd(BUYITEMS) + 7 * pos, item_id);

	host_writews((Bit8u*)ds_readd(BUYITEMS) + 7 * pos + 2,
		host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) + host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) * host_readbs(shop_ptr) / 100);

	host_writews((Bit8u*)ds_readd(BUYITEMS) + 7 * pos + 4,
			host_readbs(get_itemsdat(item_id) + ITEM_STATS_PRICE_UNIT));
}

struct dummy7 {
	char a[7];
};

void do_merchant(void)
{
	signed short l_si;
	signed short item_pos;
	signed short done;
	signed short answer;
	signed short refresh;
	signed short armor_pos;
	Bit32s party_money;
	Bit8u *shop_p;

	done = 0;

#if !defined(__BORLANDC__)
	/* Print merchant values */
	const Bit8u typi = gs_current_typeindex;
	const Bit8s price = ds_readbs(SHOP_DESCR_TABLE + 9 * typi);
	const Bit8s h_type = ds_readbs(SHOP_DESCR_TABLE + 1 + 9 * typi);
	const Bit8s sortiment = ds_readbs(SHOP_DESCR_TABLE + 2 + 9 * typi);
	const char h_type0[] = "UNGUELTIG";
	const char h_type1[] = "Waffen";
	const char h_type2[] = "Kraeuter";
	const char h_type3[] = "Kraemer";
	const char *const h_str = (h_type == 1 ? h_type1 :
					(h_type == 2 ? h_type2 :
					(h_type == 3 ? h_type3 : h_type0)));

	D1_INFO("Haendler-no: %2d / Haendlertyp: %s\n", typi, h_str);
	D1_INFO("\tPreise: %3d%% [70, 180]\n", 100 + price);
	D1_INFO("\tAuswahl: %2d [0, 18] (je kleiner der Wert, desto groesser die Auswahl)\n", sortiment);

#endif

	if ((gs_day_timer < HOURS(8) || gs_day_timer > HOURS(19)) && gs_current_loctype != LOCTYPE_MARKET)
	{
		/* shop closed */

		GUI_output(get_ttx(482));
		leave_location();
		return;
	}

	if (gs_merchant_kicked_flags[gs_current_typeindex]) {
		if (ds_readbs((SHOP_DESCR_TABLE + 1) + 9 * gs_current_typeindex) != 3) {
			talk_merchant();
			leave_location();
			return;
		}
	} else if (gs_merchant_offended_flags[gs_current_typeindex]) {
		GUI_output(get_ttx(507));
		leave_location();
		return;
	}

	load_ggsts_nvf();
	refresh = ds_writews(REQUEST_REFRESH, 1);

	ds_writed(BUYITEMS, (Bit32u)g_fig_figure1_buf);
	memset((Bit8u*)ds_readd(BUYITEMS), 0, 3500);
	ds_writew(PRICE_MODIFICATOR, 4);
	shop_p = p_datseg + SHOP_DESCR_TABLE + 9 * gs_current_typeindex;

	for (l_si = 0; l_si < 100; l_si++) {
		host_writews((Bit8u*)ds_readd(BUYITEMS) + 7 * l_si, 0);
	}

	l_si = 1;
	item_pos = 0;
	armor_pos = 70;

	while (host_readws(get_itemsdat(l_si)) != -1) {

		if (host_readbs(shop_p + 2) <= host_readbs(get_itemsdat(l_si) + ITEM_STATS_COMMONNESS)) {

			if (item_armor(get_itemsdat(l_si)) || item_weapon(get_itemsdat(l_si))) {

				if (host_readbs(shop_p + 1) == 1) {

					add_item_to_shop(shop_p, l_si,
						item_weapon(get_itemsdat(l_si)) ? item_pos++ : armor_pos++);
				}

			} else if (item_herb_potion(get_itemsdat(l_si))) {

				if (host_readbs(shop_p + 1) == 2) {

					add_item_to_shop(shop_p, l_si, item_pos++);
				}

			} else {

				if (host_readbs(shop_p + 1) == 3) {

					add_item_to_shop(shop_p, l_si, item_pos++);
				}
			}
		}

		l_si ++;
	}

	for (l_si = 0; l_si < 3; l_si++) {
		if (host_readws(shop_p + 2 * l_si + 3) != 0) {
			add_item_to_shop(shop_p, host_readws(shop_p + 2 * l_si + 3), item_pos++);
		}
	}

	if (host_readbs(shop_p + 1) == 1) {

		qsort((Bit8u*)ds_readd(BUYITEMS), item_pos, 7, shop_compar);
		qsort((Bit8u*)ds_readd(BUYITEMS) + 7 * 70, armor_pos - 70, 7, shop_compar);

		/* copy the rest */
		for (l_si = 0; armor_pos - 70 > l_si; l_si++) {

			*(struct dummy7*)((Bit8u*)ds_readd(BUYITEMS) + 7 * (item_pos + l_si)) =
				*(struct dummy7*)((Bit8u*)ds_readd(BUYITEMS) + 7 * (l_si + 70));
		}
		/* cleanup */
		for (l_si = item_pos + armor_pos - 70; l_si < 100; l_si++) {
			host_writews((Bit8u*)ds_readd(BUYITEMS) + 7 * l_si, 0);
		}

	} else {
		qsort((Bit8u*)ds_readd(BUYITEMS), item_pos, 7, shop_compar);
	}

	while (done == 0 && !gs_merchant_offended_flags[gs_current_typeindex]) {

		if (ds_readws(REQUEST_REFRESH) != 0) {

			draw_loc_icons(4, MENU_ICON_BUY, MENU_ICON_SELL, MENU_ICON_TALK, MENU_ICON_LEAVE);

			draw_main_screen();

			set_var_to_zero();

			load_ani(host_readbs(shop_p + 1) == 1 ? 15 : (host_readbs(shop_p + 1) == 2 ? 22 : 14));

			init_ani(0);

			set_audio_track(ARCHIVE_FILE_TERMS_XMI);

			GUI_print_loc_line(gs_current_loctype == LOCTYPE_MARKET ? get_ttx(679) : (gs_current_typeindex == 93 ?  get_ttx(46) : get_tx(gs_current_locdata)));
			// CURRENT_TYPEINDEX 93 is the merchant from random city event 6.

			ds_writew(REQUEST_REFRESH, refresh = 0);

		}

		if (refresh != 0) {
			GUI_print_loc_line(get_tx(gs_current_locdata));
			refresh = 0;
		}

		handle_gui_input();

		if (gs_current_town == TOWNS_THORWAL ||
			gs_current_town == TOWNS_PREM ||
			gs_current_town == TOWNS_PHEXCAER ||
			gs_current_town == TOWNS_OBERORKEN)
		{

			/* debt logic in shops */

			if ((gs_bank_deposit <= -1000) && !gs_debt_days) {

				GUI_output(get_ttx(761));
				gs_debt_days = 7;
			}

			if (gs_debt_days == -1) {

				if (GUI_bool(get_ttx(762))) {

					party_money = get_party_money();

					if (party_money < 5000) {

						GUI_output(get_ttx(763));
						ds_writews(MOUSE2_EVENT, ds_writews(ACTION, 0));
						done = 1;

					} else {
						gs_bank_deposit += 500;
						if (gs_bank_deposit > -1000) {
							gs_debt_days = 0;
						}

						party_money -= 5000;
						set_party_money(party_money);
					}
				} else {

					GUI_output(get_ttx(763));
					ds_writews(MOUSE2_EVENT, ds_writews(ACTION, 0));
					done = 1;
				}
			}
		}

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			answer = GUI_radio(get_ttx(430), 4,
						get_ttx(431), get_ttx(432),
						get_ttx(343), get_ttx(434)) - 1;

			if (answer != -2) {
				ds_writews(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_4) {

			done = 1;

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_1) {


			buy_screen();

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {

			sell_screen(shop_p);

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_3) {

			talk_merchant();

			if (gs_merchant_kicked_flags[gs_current_typeindex]) {
				done = 1;
			}
		}
	}

	copy_palette();

	leave_location();
}

void talk_merchant(void)
{
	signed short tlk_id;

	switch (ds_readbs(SHOP_DESCR_TABLE + 9 * gs_current_typeindex + 1)) {
		case 1: tlk_id = 16; break;
		case 2: tlk_id = 15; break;
		case 3: tlk_id = 14; break;
	}

	do_random_talk(tlk_id, 0);
}

void TLK_ghandel(signed short state)
{
	if (!state) {
		g_dialog_next_state = (gs_merchant_kicked_flags[gs_current_typeindex] ? 1 : 4);
	} else if (state == 1) {
		/* REMARK: gs_merchant_kicked2_flags[0] is always zero */
		g_dialog_next_state = (gs_merchant_kicked2_flags[gs_current_typeindex] ? 2 : 3);
	} else if (state == 6 && gs_current_typeindex != 90) {
		gs_merchant_kicked_flags[gs_current_typeindex] = 1;
	} else if (state == 10) {
		/* test CH+0 */
		g_dialog_next_state = (test_attrib((Bit8u*)get_first_hero_available_in_group(), ATTRIB_CH, 0) > 0 ? 11 : 12);
	}
}

void TLK_khandel(signed short state)
{
	if (!state) {
		g_dialog_next_state = (gs_merchant_kicked_flags[gs_current_typeindex] ? 1 : 2);
	} else if (state == 5) {
		tumult();
		if (gs_current_typeindex != 90) {
			gs_merchant_kicked_flags[gs_current_typeindex] = 1;
		}

	} else if (state == 7 && gs_current_typeindex != 90) {
		gs_merchant_kicked_flags[gs_current_typeindex] = 1;
	} else if (state == 8) {
		g_dialog_next_state = (random_schick(20) <= 3 ? 9 : -1);
	} else if (state == 11) {
		ds_writew(PRICE_MODIFICATOR, 3);
	} else if (state == 12) {
		/* test CH+4 */
		g_dialog_next_state = (test_attrib((Bit8u*)get_first_hero_available_in_group(), ATTRIB_CH, 4) > 0 ? 13 : 10);
	}
}

void TLK_whandel(signed short state)
{
	if (!state) {
		g_dialog_next_state = (gs_merchant_kicked_flags[gs_current_typeindex] ? 26 : 1);
	} else if (state == 7 || state == 13) {
		tumult();
		if (gs_current_typeindex != 90) {
			gs_merchant_kicked_flags[gs_current_typeindex] = 1;
		}

	} else if ((state == 8 || state == 16) && gs_current_typeindex != 90) {
		gs_merchant_kicked_flags[gs_current_typeindex] = 1;
	} else if (state == 18) {
		/* test CH+0 */
		g_dialog_next_state = (test_attrib((Bit8u*)get_first_hero_available_in_group(), ATTRIB_CH, 0) > 0 ? 19 : -1);
	} else if (state == 25) {

		if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_FEILSCHEN, 0) > 0) {
			g_dialog_next_state = (23);
			ds_writew(PRICE_MODIFICATOR, 3);
		} else {
			g_dialog_next_state = (24);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
