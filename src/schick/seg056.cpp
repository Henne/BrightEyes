/**
 *	Rewrite of DSA1 v3.02_de functions of seg056 (merchant: buy)
 *	Functions rewritten: 3/3 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg056.cpp
 */
#include <stdio.h>
#include <string.h>

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg029.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg104.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static struct struct_shopping_cart *g_buy_shopping_cart; // ds:0xe3f2, to buffer of size 250, each item has 4 bytes

/**
 * \brief   compare function for qsort()
 *
 * \param   p1_in       first pointer
 * \param   p2_in       second pointer
 * \return              -1 -> p1 < p2; 0 -> p1 == p2; 1 -> p1 > p1
 */
int shop_compar(const void *p1_in, const void *p2_in)
{
	Bit32s v1;
	Bit32s v2;
	Bit8u *p1;
	Bit8u *p2;

	p1 = (Bit8u*)p1_in;
	p2 = (Bit8u*)p2_in;

	v1 = host_readws(p1 + 2) * host_readws(p1 + 4);
	v2 = host_readws(p2 + 2) * host_readws(p2 + 4);

	return v1 < v2 ? -1 : (v1 == v2 ? 0 : 1);
}

/**
 * \brief   shows the buy-screen and provides interaction
 */
void buy_screen(void)
{
	signed short l_di;
	signed short j;
	signed short items_x;
	signed short fg_bak;
	signed short bg_bak;
	signed short l3;
	signed short l4;
	signed short percent;
	signed short percent_old = 100;
	signed short l6 = 1;
	signed short l7 = 0;
	signed short done = 0;
	signed short item = 0;
	signed short l8;
	signed short item_id;
	struct c_str_5 fmt_h = g_buy_screen_str_money_h;
	struct c_str_5 fmt_s = g_buy_screen_str_money_s;
	struct c_str_5 fmt_d = g_buy_screen_str_money_d;
	struct Bit16s_3 array3 = g_buy_screen_items_posx;
	//signed short array3[3] = { { 30, 95, 160 } };
	struct Bit16s_5 array5 = g_buy_screen_items_posy;
	//signed short array5[5] = { { 35, 55, 75, 95, 115 } };
	//
	Bit32s price = 0;
	Bit32s l9;
	Bit32s p_money;
	signed short nice = 0;
	signed short free_slots = 0;
	signed short offended = 0;
	Bit8u *hero1;
	Bit8u *hero2;
	signed short width;
	signed short height;
	signed short given_items;
	signed short l12;
	signed short l13;
	struct c_str_6 fmt_d_s = g_buy_screen_str_d_s;
	//char fmt_d_s[6] = { "%d %s" };
	//
	signed short l15 = 0;
	signed short l16;
	signed short l17;
	struct nvf_desc nvf;

	/* TODO: The shopping cart has space for 62.5 items ? Grollo in thorwal sells 69 items. */
	g_buy_shopping_cart = (struct struct_shopping_cart*)g_fig_figure1_buf + 2800;
	memset((Bit8u*)g_buy_shopping_cart, 0, 250);

	g_request_refresh = 1;

	while (done == 0) {

		if (g_request_refresh != 0) {

			free_slots = 0;
			hero2 = get_hero(0);
			for (l_di = 0; l_di <= 6; l_di++, hero2 += SIZEOF_HERO) {

				if (host_readbs(hero2 + HERO_TYPE) &&
					host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group)
				{
					for (j = HERO_INVENTORY_SLOT_KNAPSACK_1; j < NR_HERO_INVENTORY_SLOTS; j++) {
						if (host_readws(hero2 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * j) == ITEM_NONE) {
							free_slots++;
						}
					}
				}
			}

			set_var_to_zero();

			g_pp20_index = -1;
			draw_loc_icons(4, MENU_ICON_BARGAIN, MENU_ICON_SCROLL_RIGHT, MENU_ICON_SCROLL_LEFT, MENU_ICON_LEAVE);
			draw_main_screen();

			/* ICONS */
			l_di = load_archive_file(ARCHIVE_FILE_ICONS);
			seek_archive_file(l_di, 22 * 576L);
			read_archive_file(l_di, g_icon, 576L);
			close(l_di);

			g_pic_copy.x1 = 108;
			g_pic_copy.y1 = 5;
			g_pic_copy.x2 = 131;
			g_pic_copy.y2 = 28;
			g_pic_copy.src = g_icon;
			update_mouse_cursor();
			do_pic_copy(0);
			refresh_screen_size();

			get_textcolor(&fg_bak, &bg_bak);
			set_textcolor(255, 0);

			p_money = get_party_money();
			make_valuta_str(g_dtp2, p_money);
			GUI_print_string(g_dtp2, 104 - GUI_get_space_for_string(g_dtp2, 0), 26);

			l8 = 1;
			g_request_refresh = 0;
		}

		if (l8 != 0) {
			/* refresh goods */
			update_mouse_cursor();

			do_fill_rect(g_vga_memstart, 135, 26, 214, 33, 0);

			make_valuta_str(g_dtp2, price);
			GUI_print_string(g_dtp2, 135, 26);

			do_fill_rect(g_vga_memstart, 29, 34, 214, 133, 0);

			do_v_line(g_vga_memstart, 87, 35, 131, -1);
			do_v_line(g_vga_memstart, 152, 35, 131, -1);

			nvf.dst = g_renderbuf_ptr;
			nvf.src = g_buffer10_ptr;
			nvf.type = 0;
			nvf.width =  (Bit8u*)&width;
			nvf.height = (Bit8u*)&height;

			for (items_x = 0; items_x < 3; items_x++) {

				for (l_di = 0; l_di < 5; l_di++) {

					l3 = 5 * items_x + l_di + item;

					if ((j = host_readws((Bit8u*)g_buyitems + 7 * l3))) {

						g_pic_copy.x1 = array3.a[items_x];
						g_pic_copy.y1 = array5.a[l_di];
						g_pic_copy.x2 = array3.a[items_x] + 15;
						g_pic_copy.y2 = array5.a[l_di] + 15;
						g_pic_copy.src = g_renderbuf_ptr;

						nvf.no = host_readws(get_itemsdat(j));

						process_nvf(&nvf);

						do_pic_copy(0);

						sprintf(g_dtp2,
								host_readws((Bit8u*)g_buyitems + 4 + 7 * l3) == 1 ? fmt_h.a :
								(host_readws((Bit8u*)g_buyitems + 4 + 7 * l3) == 10 ? fmt_s.a : fmt_d.a),
							host_readws((Bit8u*)g_buyitems + 2 + 7 * l3));

						GUI_print_string(g_dtp2, array3.a[items_x] + 20, array5.a[l_di] + 5);
					}
				}
			}

			refresh_screen_size();
			l6 = 1;
			l7 = 0;
			l8 = 0;
		}

		if (g_have_mouse == 2) {
			select_with_mouse((Bit8u*)&l7, (Bit8u*)g_buyitems + 7 * item);
		} else {
			select_with_keyboard((Bit8u*)&l7, (Bit8u*)g_buyitems + 7 * item);
		}

#if !defined(__BORLANDC__)
		/* BE-fix */
		l7 = host_readws((Bit8u*)&l7);
#endif

		g_action_table_secondary = &g_action_table_merchant[0];
		handle_gui_input();
		g_action_table_secondary = NULL;

		if (l6 != l7 || l15 != 0) {

			do_border(g_vga_memstart, array3.a[l6 / 5] - 1, array5.a[l6 % 5] - 1, array3.a[l6 / 5] + 16,
					array5.a[l6 % 5] + 16, 0);

			do_border(g_vga_memstart, array3.a[l7 / 5] - 1, array5.a[l7 % 5] - 1, array3.a[l7 / 5] + 16,
					array5.a[l7 % 5] + 16, -1);

			l6 = l7;

			clear_loc_line();

			item_id = host_readws((Bit8u*)g_buyitems + 7 * (l7 + item));

			l4 = 0;

			for (l_di = 0; l_di < nice; l_di++) {

				if (g_buy_shopping_cart[l_di].item_id == item_id) {
					l4 = g_buy_shopping_cart[l_di].quantity;
					break;
				}
			}

			sprintf(g_dtp2, (char*)fmt_d_s.a, l4, GUI_names_grammar(0x4000 +  (l4 > 1 || l4 == 0 ? 4 : 0), item_id, 0));

			if (item_weapon(get_itemsdat(item_id))) {

				strcat(g_dtp2, g_buy_screen_str_comma_space);

				strcat(g_dtp2, get_ttx(48 + host_readbs(get_itemsdat(item_id) + ITEM_STATS_SUBTYPE)));
			}

			GUI_print_loc_line(g_dtp2);

			hero1 = get_hero(0);

			for (l_di = 0; l_di < 7; l_di++, hero1 += SIZEOF_HERO) {

				do_fill_rect(g_vga_memstart, g_hero_pic_posx[l_di], 190,
						g_hero_pic_posx[l_di] + 41, 197, 0);

				if (host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) {

					copy_forename(g_dtp2, (char*)(hero1 + HERO_NAME2));
					set_textcolor(255, 0);

					if (host_readbs(hero1 + HERO_GROUP_NO) != gs_current_group) {

						set_textcolor(111, 0);

					} else {

						if (!is_in_word_array(item_id, g_wearable_items_index[host_readbs(hero1 + HERO_TYPE) - 1])) {
							set_textcolor(201, 0);
						}
					}

					GUI_print_string(g_dtp2, GUI_get_first_pos_centered(g_dtp2, g_hero_pic_posx[l_di], 43, 0), 190);

					set_textcolor(255, 0);
				}
			}

			l15 = 0;
		}

		if (g_mouse2_event && get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_merchant)) {
			g_action = ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK;
		}

		if ((g_mouse2_event && g_action != ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) || g_action == ACTION_ID_PAGE_UP) {

			l3 = GUI_radio(NULL, 4,	get_ttx(433), get_ttx(435), get_ttx(436), get_ttx(437)) - 1;

			if (l3 != -2) {
				g_action = l3 + ACTION_ID_ICON_1;
			}
		}

		if (g_action == ACTION_ID_CLOSING_SQUARE_BRACKET || g_action == ACTION_ID_SLASH || g_action == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) {

			l3 = 1;

			if (g_action == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) {

				if (g_mouse2_event) {
					g_mouse2_event = 0;
					l3 = 2;
				}
			} else {
				if (g_action == ACTION_ID_SLASH) {
					l3 = 2;
				}
			}

			item_id = host_readws((Bit8u*)g_buyitems + 7 * (l7 + item));
			l16 = -1;
			l17 = 0;

			for (l_di = 0; l_di < nice; l_di++) {

				if (g_buy_shopping_cart[l_di].item_id == item_id) {
					l16 = l_di;
				}

				l17 += (g_buy_shopping_cart[l_di].item_id != 0) && item_stackable(get_itemsdat(item_id)) ?
						g_buy_shopping_cart[l_di].quantity / 100 + 1 :
						g_buy_shopping_cart[l_di].quantity;
			}

			if (l16 != -1) {

				l4 = 1;

				if (item_stackable(get_itemsdat(item_id))) {

					if (l3 == 2) {

						if (g_buy_shopping_cart[l16].quantity > 1) {

							sprintf(g_dtp2,	get_ttx(818), GUI_names_grammar(4, item_id, 0));

							l4 = GUI_input(g_dtp2, 2);
						}
					} else {
						sprintf(g_dtp2,	get_ttx(441), GUI_names_grammar(4, item_id, 0));

						l4 = GUI_input(g_dtp2, 2);
					}
				}

				if (l4 > 0) {

					if (g_buy_shopping_cart[l16].quantity < l4 && l3 == 2) {
						l4 = g_buy_shopping_cart[l16].quantity;
					}

					l9 = (Bit32s)host_readws((Bit8u*)g_buyitems + 7 * (l7 + item) + 2) *
						(Bit32s)host_readws((Bit8u*)g_buyitems + 7 * (l7 + item) + 4) * l4;

					if (l3 == 1 && price + l9 > p_money) {
						GUI_output(get_ttx(401));
					} else if (l3 == 1 && free_slots < l17) {
						GUI_output(get_ttx(438));
					} else {

						if (l3 == 1) {
							price += l9;
							g_buy_shopping_cart[l16].quantity += l4;
						} else {
							price -= l9;
							g_buy_shopping_cart[l16].quantity -= l4;

							if (g_buy_shopping_cart[l16].quantity == 0) {

								/* TODO: check that */
								memmove(&g_buy_shopping_cart[l16], &g_buy_shopping_cart[l16 + 1],	248 - 2 * l16);

								nice--;
							}
						}

						do_fill_rect(g_vga_memstart, 135, 26, 214, 33, 0);

						make_valuta_str(g_dtp2, price);

						GUI_print_string(g_dtp2, 135, 26);
					}

					l15 = 1;
				}
			} else {
				if (l3 == 1) {

					l4 = 1;

					if (item_stackable(get_itemsdat(host_readws((Bit8u*)g_buyitems + 7 * (l7 + item))))) {

						sprintf(g_dtp2,	get_ttx(441),
							GUI_names_grammar(4, host_readws((Bit8u*)g_buyitems + 7 * (l7 + item)), 0));
						l4 = GUI_input(g_dtp2, 2);
					}

					if (l4 > 0) {

						l9 = (Bit32s)host_readws((Bit8u*)g_buyitems + 7 * (l7 + item) + 2) *
							(Bit32s)host_readws((Bit8u*)g_buyitems + 7 * (l7 + item) + 4) * l4;

						if (price + l9 > p_money) {

							GUI_output(get_ttx(401));

						} else if (free_slots < l17) {

							GUI_output(get_ttx(438));

						} else {
							price += l9;

							g_buy_shopping_cart[nice].item_id =
								host_readws((Bit8u*)g_buyitems + 7 * (l7 + item));

							g_buy_shopping_cart[nice].quantity = l4;

							nice++;

							do_fill_rect(g_vga_memstart, 135, 26, 214, 33 ,0);

							make_valuta_str(g_dtp2, price);

							GUI_print_string(g_dtp2, 135, 26);
						}

						l15 = 1;
					}
				}
			}
		}

		if (g_action == ACTION_ID_ICON_3 && item != 0) {
			l8 = 1;
			item -= 15;
		} else if (g_action == ACTION_ID_ICON_2 && host_readws((Bit8u*)g_buyitems + 7 * (item + 15))) {
			l8 = 1;
			item += 15;
		}


		if (g_action == ACTION_ID_ICON_1 && price) {

			j = 0;

			while (offended == 0 && j < 3) {

				make_valuta_str(g_text_output_buf, price);
				sprintf(g_dtp2, get_ttx(439), g_text_output_buf);

				do {
					percent = GUI_input(g_dtp2, 2);

				} while (percent > 50);

				if (percent == 0) {

					GUI_output(get_ttx(440));
					offended = 2;

				} else if (percent >= percent_old) {

					j = 2;

				} else if (percent < 0) {

					break;

				} else {
					g_skilled_hero_pos = get_skilled_hero_pos(TA_FEILSCHEN);

					l3 = select_hero_ok(get_ttx(442));

					if (l3 == -1) {
						break;
					}

					hero1 = get_hero(l3);

					offended = bargain(hero1, nice, price, percent, 0) > 0 ? 1 : 0;
				}

				if (offended > 0) {

					if (offended != 2) {
						GUI_output(get_ttx(444));
					}

					l12 = l13 = 0;

					for (l_di = 0; l_di < nice; l_di++) {

						item_id = g_buy_shopping_cart[l_di].item_id;
						given_items = get_item(item_id, 1, g_buy_shopping_cart[l_di].quantity);

						g_market_itemsaldo_table[item_id] = g_market_itemsaldo_table[item_id] + given_items;

						if (g_market_itemsaldo_table[item_id] >= 10) {

							g_market_itemsaldo_table[item_id] = 0;

							add_ptr_ws(get_itemsdat(item_id) + ITEM_STATS_PRICE, host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) * 10 / 100);
						}

						if (given_items == 0 && !l_di) {
							l12 = 1;
						}

						if (g_buy_shopping_cart[l_di].quantity > given_items) {
							l13 = 1;
							break;
						}
					}

					price = price - price * percent / 100L;
					p_money -= price;

					if (l12 == 0) {

						set_party_money(p_money);

						make_valuta_str(g_text_output_buf, price);

						sprintf(g_dtp2,	get_ttx(819), g_text_output_buf);

						GUI_output(g_dtp2);
					}

					if (l13 != 0) {
						GUI_output(get_ttx(566));
					}

					done = 1;
				} else {

					if (j == 2) {
						GUI_output(get_ttx(445));
						gs_merchant_offended_flags[gs_current_typeindex] = 1;
						done = 1;
					} else {
						GUI_output(get_ttx(443));
						percent_old = percent;
					}

					j++;
				}
			}
		}

		if (g_action == ACTION_ID_ICON_4) {
			done = 1;
		}
	}

	set_textcolor(fg_bak, bg_bak);
	g_request_refresh = 1;
	g_pp20_index = -1;
}

/**
 * \brief   inserts the items of the hero into a sales array
 *
 * \param   shop_ptr    pointer to the shop description
 * \param   hero        pointer to the hero
 * \param   item_pos    position of the item in the heroes inventory
 * \param   shop_pos    position if the item in the sales array
 */
void insert_sell_items(Bit8u *shop_ptr, Bit8u *hero, signed short item_pos, signed short shop_pos)
{
	signed short item_id;
	signed short sellable = 0;

	item_id = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * item_pos);
	host_writew((Bit8u*)g_sellitems + 7 * shop_pos, item_id);

	if (item_armor(get_itemsdat(item_id)) || item_weapon(get_itemsdat(item_id))) {

		/* WEAPON SHOP */
		if (host_readbs(shop_ptr + 1) == 1) {
			sellable = 1;
		}

	} else if (item_herb_potion(get_itemsdat(item_id))) {

		/* HERB SHOP */
		if (host_readbs(shop_ptr + 1) == 2) {
			sellable = 1;
		}

	} else {

		/* CHANDLER SHOP */
		if (host_readbs(shop_ptr + 1) == 3) {
			sellable = 1;
		}
	}

	if (!sellable) {

		/* this item cannot be sold here */
		host_writew((Bit8u*)g_sellitems + 7 * shop_pos + 2, 0);
		host_writew((Bit8u*)g_sellitems + 7 * shop_pos + 4, 1);

	} else if (inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * item_pos) ||
			 host_readbs(hero + (HERO_INVENTORY + INVENTORY_RS_LOST) + SIZEOF_INVENTORY * item_pos) != 0)
	{
		/* this item is broken or RS of an armor got degraded */
		host_writew((Bit8u*)g_sellitems + 7 * shop_pos + 2, 1);
		host_writew((Bit8u*)g_sellitems + 7 * shop_pos + 4, 1);

	} else {
		/* calculate the price */
		host_writew((Bit8u*)g_sellitems + 7 * shop_pos + 2,
			(host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) + (host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) * host_readbs(shop_ptr) / 100) ) / 2);
		/* adjust price to 1 if zero */
		if (host_readws((Bit8u*)g_sellitems + 7 * shop_pos + 2) == 0) {
			host_writew((Bit8u*)g_sellitems + 7 * shop_pos + 2, 1);
		}

		host_writew((Bit8u*)g_sellitems + 7 * shop_pos + 4,
			host_readbs(get_itemsdat(item_id) + ITEM_STATS_PRICE_UNIT));
	}

	host_writebs((Bit8u*)g_sellitems + 7 * shop_pos + 6, (signed char)item_pos);
}

#if !defined(__BORLANDC__)
}
#endif
