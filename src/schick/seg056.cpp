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

struct dummy3 {
	signed short a[3];
};

struct dummy5 {
	signed short a[5];
};

struct dummy_c5 {
	char a[5];
};

struct dummy_c6 {
	char a[6];
};


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
#if !defined(__BORLANDC__)
	struct dummy_c5 fmt_h = { { "H %d" } };
	struct dummy_c5 fmt_s = { { "S %d" } };
	struct dummy_c5 fmt_d = { { "D %d" } };
	struct dummy3 array3 = { { 30, 95, 160 } };
	struct dummy5 array5 = { { 35, 55, 75, 95, 115 } };
#else
	struct dummy_c5 fmt_h = *(struct dummy_c5*)(p_datseg + BUY_SCREEN_STR_MONEY_H);
	struct dummy_c5 fmt_s = *(struct dummy_c5*)(p_datseg + BUY_SCREEN_STR_MONEY_S);
	struct dummy_c5 fmt_d = *(struct dummy_c5*)(p_datseg + BUY_SCREEN_STR_MONEY_D);
	struct dummy3 array3 = *(struct dummy3*)(p_datseg + BUY_SCREEN_ITEMS_POSX);
	struct dummy5 array5 = *(struct dummy5*)(p_datseg + BUY_SCREEN_ITEMS_POSY);
#endif
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
#if !defined(__BORLANDC__)
	struct dummy_c6 fmt_d_s = { "%d %s" };
#else
	struct dummy_c6 fmt_d_s = *(struct dummy_c6*)(p_datseg + BUY_SCREEN_STR_D_S);
#endif
	signed short l15 = 0;
	signed short l16;
	signed short l17;
	struct nvf_desc nvf;

	ds_writed(BUY_SHOPPING_CART, (Bit32u)((Bit8u*)ds_readd(FIG_FIGURE1_BUF) + 2800));
	memset((Bit8u*)ds_readd(BUY_SHOPPING_CART), 0, 250);

	ds_writew(REQUEST_REFRESH, 1);

	while (done == 0) {

		if (ds_readws(REQUEST_REFRESH) != 0) {

			free_slots = 0;
			hero2 = get_hero(0);
			for (l_di = 0; l_di <= 6; l_di++, hero2 += SIZEOF_HERO) {

				if (host_readbs(hero2 + HERO_TYPE) &&
					host_readbs(hero2 + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					for (j = HERO_INVENTORY_SLOT_KNAPSACK_1; j < NR_HERO_INVENTORY_SLOTS; j++) {
						if (host_readws(hero2 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * j) == ITEM_NONE) {
							free_slots++;
						}
					}
				}
			}

			set_var_to_zero();

			ds_writeb(PP20_INDEX, 0xff);
			draw_loc_icons(4, MENU_ICON_BARGAIN, MENU_ICON_SCROLL_RIGHT, MENU_ICON_SCROLL_LEFT, MENU_ICON_LEAVE);
			draw_main_screen();

			/* ICONS */
			l_di = load_archive_file(ARCHIVE_FILE_ICONS);
			seek_archive_file(l_di, 22 * 576L);
			read_archive_file(l_di, (Bit8u*)ds_readd(ICON), 576L);
			close(l_di);

			ds_writew(PIC_COPY_X1, 108);
			ds_writew(PIC_COPY_Y1, 5);
			ds_writew(PIC_COPY_X2, 131);
			ds_writew(PIC_COPY_Y2, 28);
			ds_writed(PIC_COPY_SRC, ds_readd(ICON));
			update_mouse_cursor();
			do_pic_copy(0);
			refresh_screen_size();

			get_textcolor(&fg_bak, &bg_bak);
			set_textcolor(255, 0);

			p_money = get_party_money();
			make_valuta_str((char*)ds_readd(DTP2), p_money);
			GUI_print_string((char*)ds_readd(DTP2),
				104 - GUI_get_space_for_string((char*)ds_readd(DTP2), 0),
				26);

			l8 = 1;
			ds_writew(REQUEST_REFRESH, 0);
		}

		if (l8 != 0) {
			/* refresh goods */
			update_mouse_cursor();

			do_fill_rect((Bit8u*)ds_readd(FRAMEBUF_PTR), 135, 26, 214, 33, 0);

			make_valuta_str((char*)ds_readd(DTP2), price);
			GUI_print_string((char*)ds_readd(DTP2), 135, 26);

			do_fill_rect((Bit8u*)ds_readd(FRAMEBUF_PTR), 29, 34, 214, 133, 0);

			do_v_line((Bit8u*)ds_readd(FRAMEBUF_PTR), 87, 35, 131, -1);
			do_v_line((Bit8u*)ds_readd(FRAMEBUF_PTR), 152, 35, 131, -1);

			nvf.dst = (Bit8u*)ds_readd(RENDERBUF_PTR);
			nvf.src = (Bit8u*)ds_readd(BUFFER10_PTR);
			nvf.type = 0;
			nvf.width =  (Bit8u*)&width;
			nvf.height = (Bit8u*)&height;

			for (items_x = 0; items_x < 3; items_x++) {

				for (l_di = 0; l_di < 5; l_di++) {

					l3 = 5 * items_x + l_di + item;

					if ((j = host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * l3))) {

						ds_writew(PIC_COPY_X1, array3.a[items_x]);
						ds_writew(PIC_COPY_Y1, array5.a[l_di]);
						ds_writew(PIC_COPY_X2, array3.a[items_x] + 15);
						ds_writew(PIC_COPY_Y2, array5.a[l_di] + 15);
						ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));

						nvf.no = host_readws(get_itemsdat(j));

						process_nvf(&nvf);

						do_pic_copy(0);

						sprintf((char*)ds_readd(DTP2),
							host_readws((Bit8u*)ds_readd(BUYITEMS) + 4 + 7 * l3) == 1 ? fmt_h.a :
								(host_readws((Bit8u*)ds_readd(BUYITEMS) + 4 + 7 * l3) == 10 ? fmt_s.a : fmt_d.a),
							host_readws((Bit8u*)ds_readd(BUYITEMS) + 2 + 7 * l3));

						GUI_print_string((char*)ds_readd(DTP2), array3.a[items_x] + 20, array5.a[l_di] + 5);
					}
				}
			}

			refresh_screen_size();
			l6 = 1;
			l7 = 0;
			l8 = 0;
		}

		if (ds_readws(HAVE_MOUSE) == 2) {
			select_with_mouse((Bit8u*)&l7, (Bit8u*)ds_readd(BUYITEMS) + 7 * item);
		} else {
			select_with_keyboard((Bit8u*)&l7, (Bit8u*)ds_readd(BUYITEMS) + 7 * item);
		}

#if !defined(__BORLANDC__)
		/* BE-fix */
		l7 = host_readws((Bit8u*)&l7);
#endif

		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)(p_datseg + ACTION_TABLE_MERCHANT));
		handle_gui_input();
		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)0L);

		if (l6 != l7 || l15 != 0) {

			do_border((Bit8u*)ds_readd(FRAMEBUF_PTR),
				array3.a[l6 / 5] - 1,
				array5.a[l6 % 5] - 1,
				array3.a[l6 / 5] + 16,
				array5.a[l6 % 5] + 16,
				0);

			do_border((Bit8u*)ds_readd(FRAMEBUF_PTR),
				array3.a[l7 / 5] - 1,
				array5.a[l7 % 5] - 1,
				array3.a[l7 / 5] + 16,
				array5.a[l7 % 5] + 16,
				-1);

			l6 = l7;

			clear_loc_line();

			item_id = host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * (l7 + item));

			l4 = 0;

			for (l_di = 0; l_di < nice; l_di++) {

				if (host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di) == item_id) {
					l4 = host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di + 2);
					break;
				}
			}

			sprintf((char*)ds_readd(DTP2),
				(char*)fmt_d_s.a,
				l4,
				(char*)(Bit8u*)(GUI_names_grammar(0x4000 +  (l4 > 1 || l4 == 0 ? 4 : 0), item_id, 0)));

			if (item_weapon(get_itemsdat(item_id))) {

				strcat((char*)ds_readd(DTP2),
					(char*)p_datseg + BUY_SCREEN_STR_COMMA_SPACE);

				strcat((char*)ds_readd(DTP2),
					get_ttx(48 + host_readbs(get_itemsdat(item_id) + ITEM_STATS_SUBTYPE)));
			}

			GUI_print_loc_line((char*)ds_readd(DTP2));

			hero1 = get_hero(0);

			for (l_di = 0; l_di < 7; l_di++, hero1 += SIZEOF_HERO) {

				do_fill_rect((Bit8u*)ds_readd(FRAMEBUF_PTR),
						ds_readws(HERO_PIC_POSX + 2 * l_di),
						190,
						ds_readws(HERO_PIC_POSX + 2 * l_di) + 41,
						197, 0);

				if (host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) {
					copy_forename((char*)ds_readd(DTP2), hero1 + HERO_NAME2);
					set_textcolor(255, 0);

					if (host_readbs(hero1 + HERO_GROUP_NO) != ds_readbs(CURRENT_GROUP)) {
						set_textcolor(111, 0);
					} else {

						if (!is_in_word_array(item_id, (short*)(Bit8u*)((ds_readds((WEARABLE_ITEMS_INDEX - 4) + 4 * host_readbs(hero1 + HERO_TYPE)))))) {
							set_textcolor(201, 0);
						}
					}

					GUI_print_string((char*)ds_readd(DTP2),
						GUI_get_first_pos_centered((char*)ds_readd(DTP2), ds_readws(HERO_PIC_POSX + 2 *l_di), 43, 0),
						190);

					set_textcolor(255, 0);
				}
			}

			l15 = 0;
		}

		if ((ds_readws(MOUSE2_EVENT) != 0) && get_mouse_action(ds_readws(MOUSE_POSX), ds_readws(MOUSE_POSY), p_datseg + ACTION_TABLE_MERCHANT)) {
			ds_writew(ACTION, ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK);
		}

		if ((ds_readws(MOUSE2_EVENT) != 0 && ds_readws(ACTION) != ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			l3 = GUI_radio(NULL, 4,
					get_ttx(433),
					get_ttx(435),
					get_ttx(436),
					get_ttx(437)) - 1;

			if (l3 != -2) {
				ds_writew(ACTION, l3 + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_CLOSING_SQUARE_BRACKET || ds_readws(ACTION) == ACTION_ID_SLASH || ds_readws(ACTION) == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) {

			l3 = 1;

			if (ds_readws(ACTION) == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK) {

				if (ds_readws(MOUSE2_EVENT) != 0) {
					ds_writew(MOUSE2_EVENT, 0);
					l3 = 2;
				}
			} else {
				if (ds_readws(ACTION) == ACTION_ID_SLASH) {
					l3 = 2;
				}
			}

			item_id = host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * (l7 + item));
			l16 = -1;
			l17 = 0;

			for (l_di = 0; l_di < nice; l_di++) {

				if (host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di) == item_id) {
					l16 = l_di;
				}

				l17 += (host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di) != 0) && item_stackable(get_itemsdat(item_id)) ?
						host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di + 2) / 100 + 1 :
						host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di + 2);
			}

			if (l16 != -1) {

				l4 = 1;

				if (item_stackable(get_itemsdat(item_id))) {

					if (l3 == 2) {

						if (host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l16 + 2) > 1) {

							sprintf((char*)ds_readd(DTP2),
								get_ttx(818),
								(char*)(Bit8u*)(GUI_names_grammar(4, item_id, 0)));

							l4 = GUI_input((char*)ds_readd(DTP2), 2);
						}
					} else {
						sprintf((char*)ds_readd(DTP2),
							get_ttx(441),
							(char*)(Bit8u*)(GUI_names_grammar(4, item_id, 0)));

						l4 = GUI_input((char*)ds_readd(DTP2), 2);
					}
				}

				if (l4 > 0) {

					if (host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l16 + 2) < l4 && l3 == 2) {
						l4 = host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l16 + 2);
					}

					l9 = (Bit32s)host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 *(l7 + item) + 2) *
						(Bit32s)host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 *(l7 + item) + 4) * l4;

					if (l3 == 1 && price + l9 > p_money) {
						GUI_output(get_ttx(401));
					} else if (l3 == 1 && free_slots < l17) {
						GUI_output(get_ttx(438));
					} else {

						if (l3 == 1) {
							price += l9;
							add_ptr_ws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l16 + 2, l4);
						} else {
							price -= l9;
							sub_ptr_ws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l16 + 2, l4);

							if (host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l16 + 2) == 0) {

								memmove((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l16,
									(Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * (l16 + 1),
									248 - 2 * l16);

								nice--;
							}
						}

						do_fill_rect((Bit8u*)ds_readd(FRAMEBUF_PTR),
								135, 26, 214, 33, 0);

						make_valuta_str((char*)ds_readd(DTP2), price);

						GUI_print_string((char*)ds_readd(DTP2), 135, 26);
					}

					l15 = 1;
				}
			} else {
				if (l3 == 1) {

					l4 = 1;

					if (item_stackable(get_itemsdat(host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * (l7 + item))))) {

						sprintf((char*)ds_readd(DTP2),
							get_ttx(441),
							(char*)(Bit8u*)(GUI_names_grammar(4, host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * (l7 + item)), 0)));

						l4 = GUI_input((char*)ds_readd(DTP2), 2);
					}

					if (l4 > 0) {

						l9 = (Bit32s)host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * (l7 + item) + 2) *
							(Bit32s)host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * (l7 + item) + 4) * l4;

						if (price + l9 > p_money) {
							GUI_output(get_ttx(401));
						} else if (free_slots < l17) {
							GUI_output(get_ttx(438));
						} else {
							price += l9;

							host_writews((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * nice,
								host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * (l7 + item)));

							host_writews((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * nice + 2, l4);

							nice++;

							do_fill_rect((Bit8u*)ds_readd(FRAMEBUF_PTR), 135, 26, 214, 33 ,0);

							make_valuta_str((char*)ds_readd(DTP2), price);

							GUI_print_string((char*)ds_readd(DTP2), 135, 26);
						}

						l15 = 1;
					}
				}
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_3 && item != 0) {
			l8 = 1;
			item -= 15;
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2 && host_readws((Bit8u*)ds_readd(BUYITEMS) + 7 * (item + 15))) {
			l8 = 1;
			item += 15;
		}


		if (ds_readws(ACTION) == ACTION_ID_ICON_1 && price) {

			j = 0;

			while (offended == 0 && j < 3) {

				make_valuta_str((char*)ds_readd(TEXT_OUTPUT_BUF), price);
				sprintf((char*)ds_readd(DTP2),
					get_ttx(439),
					(char*)ds_readd(TEXT_OUTPUT_BUF));


				do {
					percent = GUI_input((char*)ds_readd(DTP2), 2);

				} while (percent > 50);

				if (percent == 0) {
					GUI_output(get_ttx(440));
					offended = 2;
				} else if (percent >= percent_old) {
					j = 2;
				} else if (percent < 0) {
					break;
				} else {
					ds_writew(SKILLED_HERO_POS, get_skilled_hero_pos(TA_FEILSCHEN));

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

						item_id = host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di);
						given_items = get_item(item_id, 1, host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di + 2));

						ds_writeb(MARKET_ITEMSALDO_TABLE + item_id, ds_readbs(MARKET_ITEMSALDO_TABLE + item_id) + given_items);

						if (ds_readbs(MARKET_ITEMSALDO_TABLE + item_id) >= 10) {

							ds_writeb(MARKET_ITEMSALDO_TABLE + item_id, 0);

							add_ptr_ws(get_itemsdat(item_id) + ITEM_STATS_PRICE, host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) * 10 / 100);
						}

						if (given_items == 0 && !l_di) {
							l12 = 1;
						}

						if (host_readws((Bit8u*)ds_readd(BUY_SHOPPING_CART) + 4 * l_di + 2) > given_items) {
							l13 = 1;
							break;
						}
					}

					price = price - price * percent / 100L;
					p_money -= price;

					if (l12 == 0) {

						set_party_money(p_money);

						make_valuta_str((char*)ds_readd(TEXT_OUTPUT_BUF), price);

						sprintf((char*)ds_readd(DTP2),
							get_ttx(819),
							(char*)ds_readd(TEXT_OUTPUT_BUF));

						GUI_output((char*)ds_readd(DTP2));
					}

					if (l13 != 0) {
						GUI_output(get_ttx(566));
					}

					done = 1;
				} else {

					if (j == 2) {
						GUI_output(get_ttx(445));
						ds_writeb(MERCHANT_OFFENDED_FLAGS + ds_readw(CURRENT_TYPEINDEX), 1);
						done = 1;
					} else {
						GUI_output(get_ttx(443));
						percent_old = percent;
					}

					j++;
				}
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_4) {
			done = 1;
		}
	}

	set_textcolor(fg_bak, bg_bak);
	ds_writew(REQUEST_REFRESH, 1);
	ds_writeb(PP20_INDEX, 0xff);
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
	host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos, item_id);

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
		host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 2, 0);
		host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 4, 1);

	} else if (inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * item_pos) ||
			 host_readbs(hero + (HERO_INVENTORY + INVENTORY_RS_LOST) + SIZEOF_INVENTORY * item_pos) != 0)
	{
		/* this item is broken or RS of an armor got degraded */
		host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 2, 1);
		host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 4, 1);

	} else {
		/* calculate the price */
		host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 2,
			(host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) + (host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) * host_readbs(shop_ptr) / 100) ) / 2);
		/* adjust price to 1 if zero */
		if (host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 2) == 0) {
			host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 2, 1);
		}

		host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 4,
			host_readbs(get_itemsdat(item_id) + ITEM_STATS_PRICE_UNIT));
	}

	host_writebs((Bit8u*)ds_readd(SELLITEMS) + 7 * shop_pos + 6, (signed char)item_pos);
}

#if !defined(__BORLANDC__)
}
#endif
