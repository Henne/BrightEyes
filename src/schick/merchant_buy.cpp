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
#if defined(_WIN32)
#include <io.h>
#else
#include <unistd.h>
#endif
#endif

#include "v302de.h"
#include "common.h"

#include "misc.h"
#include "graphics.h"
#include "random_dice.h"
#include "playmask.h"
#include "heroes_group.h"
#include "text_output.h"
#include "gui.h"
#include "talents.h"
#include "alchemy_cure.h"
#include "inventory_main.h"

static struct c_str_5 g_buy_screen_str_money_h = { "H %d" }; // ds:0x6bc8
static struct c_str_5 g_buy_screen_str_money_s = { "S %d" }; // ds:0x6bcd
static struct c_str_5 g_buy_screen_str_money_d = { "D %d" }; // ds:0x6bd2
static struct int16_t_3 g_buy_screen_items_posx = { 30, 95, 160 }; // ds:0x6bd7
static struct int16_t_5 g_buy_screen_items_posy = { 35, 55, 75, 95, 115 }; // ds:0x6bdd
static struct c_str_6 g_buy_screen_str_d_s = { "%d %s" }; // ds:0x6be7
static char g_buy_screen_str_comma_space[3] = ", "; // ds:0x6bed

static struct struct_shopping_cart_item *g_buy_shopping_cart; // ds:0xe3f2, to buffer of size 250, each item has 4 bytes

/**
 * \brief   compare function for qsort()
 *
 * \param   p1_in       first pointer
 * \param   p2_in       second pointer
 * \return              -1 -> p1 < p2; 0 -> p1 == p2; 1 -> p1 > p1
 */
signed int item_selector_item_compare(const void *p_item_1, const void *p_item_2)
{
	int32_t price1;
	int32_t price2;
	const struct item_selector_item *item1 = (const struct item_selector_item*)p_item_1;
	const struct item_selector_item *item2 = (const struct item_selector_item*)p_item_2;

	price1 = item1->price * item1->price_unit;
	price2 = item2->price * item2->price_unit;

	return price1 < price2 ? -1 : (price1 == price2 ? 0 : 1);
}

/**
 * \brief   shows the buy-screen and provides interaction
 */
void buy_screen(void)
{
	signed int l_di; /* multi use: handle, hero_pos */
	signed int j;
	signed int items_x;
	signed int fg_bak;
	signed int bg_bak;
	signed int l3; /* multi use */
	signed int l4;
	signed int percent;
	signed int percent_old = 100;
	signed int l6 = 1;
	signed int item_selector_pos = 0;
	signed int done = 0;
	signed int item_selector_page_offset = 0;
	signed int l8;
	signed int item_id;
	struct c_str_5 fmt_h = g_buy_screen_str_money_h;
	struct c_str_5 fmt_s = g_buy_screen_str_money_s;
	struct c_str_5 fmt_d = g_buy_screen_str_money_d;
	struct int16_t_3 array3 = g_buy_screen_items_posx;
	//signed int array3[3] = { { 30, 95, 160 } };
	struct int16_t_5 array5 = g_buy_screen_items_posy;
	//signed int array5[5] = { { 35, 55, 75, 95, 115 } };
	//
	int32_t price = 0;
	int32_t l9;
	int32_t p_money;
	signed int nice = 0;
	signed int free_slots = 0;
	signed int offended = 0;
	struct struct_hero *hero1;
	struct struct_hero *hero2;
	signed int width;
	signed int height;
	signed int num_given_items;
	signed int l12;
	signed int l13;
	struct c_str_6 fmt_d_s = g_buy_screen_str_d_s;
	//char fmt_d_s[6] = { "%d %s" };
	//
	signed int l15 = 0;
	signed int l16;
	signed int num_slots_needed;
	struct nvf_extract_desc nvf;

	/* TODO: The shopping cart has space for 62.5 items ? Grollo in thorwal sells 69 items. */
	g_buy_shopping_cart = (struct struct_shopping_cart_item*)g_fig_figure1_buf + 2800 / 4;
	memset((uint8_t*)g_buy_shopping_cart, 0, 250);

	g_request_refresh = 1;

	while (done == 0) {

		if (g_request_refresh != 0) {

			free_slots = 0;
			hero2 = get_hero(0);
			for (l_di = 0; l_di <= 6; l_di++, hero2++) {

				if ((hero2->typus) && (hero2->group_id == gs_active_group_id))
				{
					for (j = HERO_INVENTORY_SLOT_KNAPSACK_1; j < NR_HERO_INVENTORY_SLOTS; j++) {

						if (hero2->inventory[j].item_id == ITEM_NONE) {
							free_slots++;
						}
					}
				}
			}

			disable_ani();

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
			call_mouse_bg();
			do_pic_copy(0);
			call_mouse();

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
			call_mouse_bg();

			do_fill_rect(g_vga_memstart, 135, 26, 214, 33, 0);

			make_valuta_str(g_dtp2, price);
			GUI_print_string(g_dtp2, 135, 26);

			do_fill_rect(g_vga_memstart, 29, 34, 214, 133, 0);

			do_v_line(g_vga_memstart, 87, 35, 131, -1);
			do_v_line(g_vga_memstart, 152, 35, 131, -1);

			nvf.dst = g_renderbuf_ptr;
			nvf.src = g_buffer10_ptr;
			nvf.compression_type = 0;
			nvf.width = &width;
			nvf.height = &height;

			for (items_x = 0; items_x < 3; items_x++) {

				for (l_di = 0; l_di < 5; l_di++) {

					l3 = 5 * items_x + l_di + item_selector_page_offset;

					if ((j = g_item_selector_buy[l3].item_id)) {

						g_pic_copy.x1 = array3.a[items_x];
						g_pic_copy.y1 = array5.a[l_di];
						g_pic_copy.x2 = array3.a[items_x] + 15;
						g_pic_copy.y2 = array5.a[l_di] + 15;
						g_pic_copy.src = g_renderbuf_ptr;

						nvf.image_num = g_itemsdat[j].gfx;

						process_nvf_extraction(&nvf);

						do_pic_copy(0);

						sprintf(g_dtp2,
							g_item_selector_buy[l3].price_unit == 1 ? fmt_h.a :
									(g_item_selector_buy[l3].price_unit == 10 ? fmt_s.a : fmt_d.a),
								g_item_selector_buy[l3].price);

						GUI_print_string(g_dtp2, array3.a[items_x] + 20, array5.a[l_di] + 5);
					}
				}
			}

			call_mouse();
			l6 = 1;
			item_selector_pos = 0;
			l8 = 0;
		}

		if (g_have_mouse == 2) {
			item_selector_mouse_select(&item_selector_pos, &g_item_selector_buy[item_selector_page_offset]);
		} else {
			item_selector_keyboard_select(&item_selector_pos, &g_item_selector_buy[item_selector_page_offset]);
		}

		g_action_table_secondary = &g_action_table_merchant[0];
		handle_gui_input();
		g_action_table_secondary = NULL;

		if (l6 != item_selector_pos || l15 != 0) {

			do_border(g_vga_memstart, array3.a[l6 / 5] - 1, array5.a[l6 % 5] - 1, array3.a[l6 / 5] + 16,
					array5.a[l6 % 5] + 16, 0);

			do_border(g_vga_memstart, array3.a[item_selector_pos / 5] - 1, array5.a[item_selector_pos % 5] - 1, array3.a[item_selector_pos / 5] + 16,
					array5.a[item_selector_pos % 5] + 16, -1);

			l6 = item_selector_pos;

			clear_loc_line();

			item_id = g_item_selector_buy[item_selector_pos + item_selector_page_offset].item_id;

			l4 = 0;

			for (l_di = 0; l_di < nice; l_di++) {

				if (g_buy_shopping_cart[l_di].item_id == item_id) {
					l4 = g_buy_shopping_cart[l_di].quantity;
					break;
				}
			}

			sprintf(g_dtp2, (char*)fmt_d_s.a, l4, GUI_names_grammar(0x4000 +  (l4 > 1 || l4 == 0 ? 4 : 0), item_id, 0));

			if (g_itemsdat[item_id].flags.weapon) {

				strcat(g_dtp2, g_buy_screen_str_comma_space);

				strcat(g_dtp2, get_ttx(48 + g_itemsdat[item_id].subtype));
			}

			GUI_print_loc_line(g_dtp2);

			hero1 = get_hero(0);

			for (l_di = 0; l_di < 7; l_di++, hero1++) {

				do_fill_rect(g_vga_memstart, g_hero_pic_posx[l_di], 190,
						g_hero_pic_posx[l_di] + 41, 197, 0);

				if (hero1->typus != HERO_TYPE_NONE) {

					copy_forename(g_dtp2, hero1->alias);
					set_textcolor(255, 0);

					if (hero1->group_id != gs_active_group_id) {

						set_textcolor(111, 0);

					} else {

						if (!is_in_int_array(item_id, g_forbidden_item_ids_table[hero1->typus - 1])) {

							set_textcolor(201, 0);
						}
					}

					GUI_print_string(g_dtp2, GUI_get_first_pos_centered(g_dtp2, g_hero_pic_posx[l_di], 43, 0), 190);

					set_textcolor(255, 0);
				}
			}

			l15 = 0;
		}

		if (g_mouse_rightclick_event && get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_merchant)) {

			g_action = ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK;
		}

		if ((g_mouse_rightclick_event && g_action != ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) || g_action == ACTION_ID_PAGE_UP) {

			l3 = GUI_radio(NULL, 4,	get_ttx(433), get_ttx(435), get_ttx(436), get_ttx(437)) - 1;

			if (l3 != -2) {
				g_action = l3 + ACTION_ID_ICON_1;
			}
		}

		if (g_action == ACTION_ID_CLOSING_SQUARE_BRACKET || g_action == ACTION_ID_SLASH || g_action == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) {

			l3 = 1;

			if (g_action == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) {

				if (g_mouse_rightclick_event) {
					g_mouse_rightclick_event = 0;
					l3 = 2;
				}
			} else {
				if (g_action == ACTION_ID_SLASH) {
					l3 = 2;
				}
			}

			item_id = g_item_selector_buy[item_selector_pos + item_selector_page_offset].item_id;
			l16 = -1;
			num_slots_needed = 0;

			for (l_di = 0; l_di < nice; l_di++) {

				if (g_buy_shopping_cart[l_di].item_id == item_id) {
					l16 = l_di;
				}

				num_slots_needed += (g_buy_shopping_cart[l_di].item_id != 0) && g_itemsdat[item_id].flags.stackable ?
					/* Original-Bug: the following formula is wrong.
					 * 199 stackable items need 3 slots (99 + 99 + 1), not 2. */
						g_buy_shopping_cart[l_di].quantity / 100 + 1 :
						g_buy_shopping_cart[l_di].quantity;
			}

			if (l16 != -1) {

				l4 = 1;

				if (g_itemsdat[item_id].flags.stackable) {

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

					l9 = (int32_t)g_item_selector_buy[item_selector_pos + item_selector_page_offset].price * g_item_selector_buy[item_selector_pos + item_selector_page_offset].price_unit * l4;

					if (l3 == 1 && price + l9 > p_money) {

						GUI_output(get_ttx(401));

					} else if (l3 == 1 && free_slots < num_slots_needed) {

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
								memmove(&g_buy_shopping_cart[l16], &g_buy_shopping_cart[l16 + 1], 248 - 2 * l16);

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

					if (g_itemsdat[g_item_selector_buy[item_selector_pos + item_selector_page_offset].item_id].flags.stackable) {

						sprintf(g_dtp2,	get_ttx(441), GUI_names_grammar(4, g_item_selector_buy[item_selector_pos + item_selector_page_offset].item_id, 0));

						l4 = GUI_input(g_dtp2, 2);
					}

					if (l4 > 0) {

						l9 = (int32_t)g_item_selector_buy[item_selector_pos + item_selector_page_offset].price *	g_item_selector_buy[item_selector_pos + item_selector_page_offset].price_unit * l4;

						if (price + l9 > p_money) {

							GUI_output(get_ttx(401));

						} else if (free_slots < num_slots_needed) {

							GUI_output(get_ttx(438));

						} else {
							price += l9;

							g_buy_shopping_cart[nice].item_id = g_item_selector_buy[item_selector_pos + item_selector_page_offset].item_id;

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

		if (g_action == ACTION_ID_ICON_3 && item_selector_page_offset) {

			l8 = 1;
			item_selector_page_offset -= 15;

		} else if (g_action == ACTION_ID_ICON_2 && g_item_selector_buy[item_selector_page_offset + 15].item_id) {

			l8 = 1;
			item_selector_page_offset += 15;
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
					g_talented_hero_pos = get_talented_hero_pos(TA_FEILSCHEN);

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
						num_given_items = give_new_item_to_group(item_id, 1, g_buy_shopping_cart[l_di].quantity);

						g_market_itemsaldo_table[item_id] = g_market_itemsaldo_table[item_id] + num_given_items;

						if (g_market_itemsaldo_table[item_id] >= 10) {

							g_market_itemsaldo_table[item_id] = 0;

							/* REMARK: write to g_itemsdat ? */
							g_itemsdat[item_id].price += g_itemsdat[item_id].price * 10 / 100;
						}

						if (num_given_items == 0 && !l_di) {
							l12 = 1;
						}

						if (g_buy_shopping_cart[l_di].quantity > num_given_items) {
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
						gs_merchant_offended_flags[gs_town_typeindex] = 1;
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
 * \param   inv_slot    position of the item in the heroes inventory
 * \param   item_selector_pos    position if the item in the sales array
 */

void add_item_to_sell_selector(const struct merchant_descr *merchant, const struct struct_hero *hero, const signed int inv_slot, const signed int item_selector_pos)
{
	signed int item_id;
	signed int sellable = 0;

	item_id = hero->inventory[inv_slot].item_id;

	g_item_selector_sell[item_selector_pos].item_id = item_id;

	if (g_itemsdat[item_id].flags.armor || g_itemsdat[item_id].flags.weapon) {

		if (merchant->type == MERCHANT_WEAPONS) sellable = 1;

	} else if (g_itemsdat[item_id].flags.herb_potion) {

		if (merchant->type == MERCHANT_HERBS) sellable = 1;

	} else {

		if (merchant->type == MERCHANT_GENERAL) sellable = 1;
	}

	if (!sellable) {

		/* this item cannot be sold here => 0 HELLER */
		g_item_selector_sell[item_selector_pos].price = 0;
		g_item_selector_sell[item_selector_pos].price_unit = 1;

	} else if (hero->inventory[inv_slot].flags.broken || (hero->inventory[inv_slot].rs_lost != 0))
	{
		/* this item is broken or RS of an armor got degraded => 1 HELLER */
		g_item_selector_sell[item_selector_pos].price = 1;
		g_item_selector_sell[item_selector_pos].price_unit = 1;

	} else {
		/* calculate the price */
		g_item_selector_sell[item_selector_pos].price =
			(g_itemsdat[item_id].price + g_itemsdat[item_id].price * merchant->price_mod / 100) / 2;

		/* adjust price to 1 if zero */
		if (g_item_selector_sell[item_selector_pos].price == 0) {
			g_item_selector_sell[item_selector_pos].price = 1;
		}

		g_item_selector_sell[item_selector_pos].price_unit = g_itemsdat[item_id].price_unit;
	}

	g_item_selector_sell[item_selector_pos].inv_slot = inv_slot;
}
