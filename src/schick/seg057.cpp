/**
 *	Rewrite of DSA1 v3.02_de functions of seg057 (merchant: sell)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg057.cpp
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
#include "seg029.h"
#include "seg047.h"
#include "seg056.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg104.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct dummy3 {
	signed short a[3];
};

struct dummy5 {
	signed short a[5];
};

struct dummy_c5 {
	char a[5];
};

/**
 * \brief   shows the sell-screen an provides interaction
 */
void sell_screen(Bit8u *shop_ptr)
{
	signed short l_di;
	signed short j;
	signed short items_x;
	signed short fg_bak;
	signed short bg_bak;
	signed short answer;
	signed short nice;
	signed short percent;
	signed short percent_old = 100;
	signed short item_id;
	signed short l5;
	signed short l6;
	signed short done = 0;
	signed short item = 0;
	signed short l8;
	signed short l20;
	signed short l10 = 1;
	signed short l11 = 1;
	signed short hero_pos_old = 1;
	signed short hero_pos = -1;
#if !defined(__BORLANDC__)
	struct dummy_c5 fmt_h = { { "H %d" } };
	struct dummy_c5 fmt_s = { { "S %d" } };
	struct dummy_c5 fmt_d = { { "D %d" } };
	struct dummy3 array3 = { { 30, 95, 160 } };
	struct dummy5 array5 = { { 35, 55, 75, 95, 115 } };
#else
	struct dummy_c5 fmt_h = *(struct dummy_c5*)(p_datseg + SELL_SCREEN_STR_MONEY_H);
	struct dummy_c5 fmt_s = *(struct dummy_c5*)(p_datseg + SELL_SCREEN_STR_MONEY_S);
	struct dummy_c5 fmt_d = *(struct dummy_c5*)(p_datseg + SELL_SCREEN_STR_MONEY_D);
	struct dummy3 array3 = *(struct dummy3*)(p_datseg + SELL_SCREEN_ITEMS_POSX);
	struct dummy5 array5 = *(struct dummy5*)(p_datseg + SELL_SCREEN_ITEMS_POSY);
#endif
	Bit32s p_money;
	Bit32s price = 0;
	signed short l12 = 0;
	Bit8u *hero1;
	Bit8u *hero2;
	Bit8u *hero3;
	signed short width;
	signed short height;
	signed short l15;
	struct nvf_desc nvf;
	signed short tmp[7][23];


	g_heroswap_allowed = 0;
	l8 = g_request_refresh = 1;
	g_sellitems = (g_fig_figure1_buf + 2100);

	while (done == 0) {

		if (g_request_refresh != 0) {

			set_var_to_zero();
			g_pp20_index = -1;
			memset((Bit8u*)g_sellitems, 0, 2100);

			for (items_x = 0; items_x <= 6; items_x++) {
				for (l_di = 0; l_di < 23; l_di++) {
					tmp[items_x][l_di] = 0;
				}
			}

			price = 0;
			draw_loc_icons(5, MENU_ICON_BARGAIN, MENU_ICON_SCROLL_RIGHT, MENU_ICON_SCROLL_LEFT, MENU_ICON_HERO, MENU_ICON_LEAVE);
			draw_main_screen();

			/* ICONS */
			l_di = load_archive_file(ARCHIVE_FILE_ICONS);
			seek_archive_file(l_di, 24 * 576L);
			read_archive_file(l_di, g_icon, 576L);
			close(l_di);

			g_pic_copy.x1 = 108;
			g_pic_copy.y1 = 5;
			g_pic_copy.x2 = 131;
			g_pic_copy.y2 = 28;
			g_pic_copy.src = g_icon;
			do_pic_copy(0);

			get_textcolor(&fg_bak, &bg_bak);
			set_textcolor(255, 0);

			l5 = -1;
			l6 = 0;
			l10 = 1;
			g_request_refresh = 0;
		}

		if (l8 != 0 || l10 != 0 || l11 != 0) {

			/* refresh goods */

			if (l10 != 0) {

				hero_pos = select_hero_ok(get_ttx(448));

				if (hero_pos == -1 || host_readbs(get_hero(hero_pos) + HERO_NR_INVENTORY_SLOTS_FILLED) == 0) {
					break;
				}

				hero1 = get_hero(hero_pos);

				deselect_hero_icon(hero_pos_old);
				select_hero_icon(hero_pos);
				hero_pos_old = hero_pos;
				l10 = 0;
				l11 = 1;

			}

			if (l11 != 0) {

				l20 = 0;
				for (l_di = 0; l_di < NR_HERO_INVENTORY_SLOTS; l_di++) {
					if (host_readws(hero1 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * l_di) != ITEM_NONE) {
						insert_sell_items(shop_ptr, hero1, l_di, l20++);
					}
				}

				for (l_di = l20; l_di < 23; l_di++) {
					host_writew((Bit8u*)g_sellitems + 7 * l_di, 0);
				}

				l11 = 0;
				l5 = -1;
				l6 = item = l12 = 0;
				percent_old = 100;
				p_money = get_party_money();

				do_fill_rect(g_vga_memstart, 26, 26, 105, 33, 0);

				make_valuta_str(g_dtp2, p_money);
				GUI_print_string(g_dtp2, 104 - GUI_get_space_for_string(g_dtp2, 0), 26);
			}

			update_mouse_cursor();

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

					answer = 5 * items_x + l_di + item;

					if ((j = host_readws((Bit8u*)g_sellitems + 7 * answer))) {

						g_pic_copy.x1 = array3.a[items_x];
						g_pic_copy.y1 = array5.a[l_di];
						g_pic_copy.x2 = array3.a[items_x] + 15;
						g_pic_copy.y2 = array5.a[l_di] + 15;
						g_pic_copy.src = g_renderbuf_ptr;

						nvf.no = host_readws(get_itemsdat(j));

						process_nvf(&nvf);

						do_pic_copy(0);

						if (item_stackable(get_itemsdat(j))) {

							if ((nice = host_readws(hero1 + (HERO_INVENTORY + INVENTORY_QUANTITY) + SIZEOF_INVENTORY * host_readbs((Bit8u*)g_sellitems + 7 * answer + 6))) > 1)
							{
								my_itoa(nice, g_dtp2, 10);

								GUI_print_string(g_dtp2,
									array3.a[items_x] + 16 - GUI_get_space_for_string(g_dtp2, 0),
									array5.a[l_di] + 9);

								if (tmp[hero_pos][host_readbs((Bit8u*)g_sellitems + 7 * answer + 6)] != 0)
								{
									set_textcolor(201, 0);
									my_itoa(tmp[hero_pos][host_readbs((Bit8u*)g_sellitems + 7 * answer + 6)], g_dtp2, 10);

									GUI_print_string(g_dtp2,
										array3.a[items_x] + 16 - GUI_get_space_for_string(g_dtp2, 0),
										array5.a[l_di] + 1);

									set_textcolor(255, 0);
								}
							}
						}

						sprintf(g_dtp2,
							host_readws((Bit8u*)g_sellitems + 4 + 7 * answer) == 1 ? fmt_h.a :
								(host_readws((Bit8u*)g_sellitems + 4 + 7 * answer) == 10 ? fmt_s.a : fmt_d.a),
							host_readws((Bit8u*)g_sellitems + 2 + 7 * answer));

						if (tmp[hero_pos][host_readbs((Bit8u*)g_sellitems + 7 * answer + 6)]) {
							set_textcolor(201, 0);
						}

						GUI_print_string(g_dtp2, array3.a[items_x] + 20, array5.a[l_di] + 5);
						set_textcolor(255, 0);
					}
				}
			}

			do_fill_rect(g_vga_memstart, 135, 26, 214, 33, 0);

			make_valuta_str(g_dtp2, price);
			GUI_print_string(g_dtp2, 135, 26);

			l5 = -1;

			refresh_screen_size();
			l8 = 0;
		}

		if (g_have_mouse == 2) {
			select_with_mouse((Bit8u*)&l6, (Bit8u*)g_sellitems + 7 * item);
		} else {
			select_with_keyboard((Bit8u*)&l6, (Bit8u*)g_sellitems + 7 * item);
		}

#if !defined(__BORLANDC__)
		/* BE-fix */
		l6 = host_readws((Bit8u*)&l6);
#endif

		g_action_table_secondary = &g_action_table_merchant[0];
		handle_gui_input();
		g_action_table_secondary = NULL;

		if (l5 != l6) {

			if (l5 != -1) {
				do_border(g_vga_memstart,
					array3.a[l5 / 5] - 1,
					array5.a[l5 % 5] - 1,
					array3.a[l5 / 5] + 16,
					array5.a[l5 % 5] + 16,
					0);
			 }

			do_border(g_vga_memstart,
				array3.a[l6 / 5] - 1,
				array5.a[l6 % 5] - 1,
				array3.a[l6 / 5] + 16,
				array5.a[l6 % 5] + 16,
				-1);

			l5 = l6;

			clear_loc_line();


			GUI_print_loc_line((GUI_name_singular(get_itemname(host_readws((Bit8u*)g_sellitems + 7 * (l6 + item))))));
		}

		if (g_mouse2_event  || g_action == ACTION_ID_PAGE_UP) {

			answer = GUI_radio(NULL, 5,
					get_ttx(433),
					get_ttx(435),
					get_ttx(436),
					get_ttx(446),
					get_ttx(437)) - 1;

			if (answer != -2) {
				g_action = (answer + ACTION_ID_ICON_1);
			}
		}

		if (g_action == ACTION_ID_ICON_3 && item != 0) {
			l8 = 1;
			item -= 15;
		} else if (g_action == ACTION_ID_ICON_2 && host_readws((Bit8u*)g_sellitems + 7 * (item + 15))) {
			l8 = 1;
			item += 15;
		}

		if (g_action == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK || g_action == ACTION_ID_RETURN) {
			/* Is ACTION == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK possible at all?
			 * ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK can be written to ACTION in buy_screen(), but where should it show up in sell_screen()?? */


			item_id = host_readws((Bit8u*)g_sellitems + 7 * (l6 + item));

			if (host_readws((Bit8u*)g_sellitems + 7 * (l6 + item) + 2) == 0) {
				GUI_output(get_ttx(452));
			} else {

				if (item_undropable(get_itemsdat(item_id))) {
					sprintf(g_dtp2,
						get_ttx(454),
						(char*)(GUI_names_grammar((signed short)0x8002, item_id, 0)));
					GUI_output(g_dtp2);
				} else {

					nice = 1;
					l15 = host_readbs((Bit8u*)g_sellitems + 7 * (l6 + item) + 6);

					if (tmp[hero_pos][l15] != 0) {

						if (item_stackable(get_itemsdat(item_id)) && host_readws(hero1 + (HERO_INVENTORY + INVENTORY_QUANTITY) + SIZEOF_INVENTORY * l15) > 1) {

							sprintf(g_dtp2,
								get_ttx(447),
								(char*)(GUI_names_grammar(4, item_id, 0)));

							nice = GUI_input(g_dtp2, 2);

							if (nice < 0) {
								nice = 0;
							}

							if (host_readws(hero1 + (HERO_INVENTORY + INVENTORY_QUANTITY) + SIZEOF_INVENTORY * l15) < nice) {
								nice = host_readws(hero1 + (HERO_INVENTORY + INVENTORY_QUANTITY) + SIZEOF_INVENTORY * l15);
							}

							price -= ((Bit32s)host_readws((Bit8u*)g_sellitems + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws((Bit8u*)g_sellitems + 7 * (l6 + item) + 4) *
									tmp[hero_pos][l15] * g_price_modificator) / 4L;

							tmp[hero_pos][l15] = nice;

							price += ((Bit32s)host_readws((Bit8u*)g_sellitems + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws((Bit8u*)g_sellitems + 7 * (l6 + item) + 4) *
									tmp[hero_pos][l15] * g_price_modificator) / 4L;
						} else {
							tmp[hero_pos][l15] = 0;

							price -= ((Bit32s)host_readws((Bit8u*)g_sellitems + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws((Bit8u*)g_sellitems + 7 * (l6 + item) + 4) *
									nice * g_price_modificator) / 4L;
						}
					} else {
						if (item_stackable(get_itemsdat(item_id)) && host_readws(hero1 + (HERO_INVENTORY + INVENTORY_QUANTITY) + SIZEOF_INVENTORY * l15) > 1) {

							sprintf(g_dtp2,
								get_ttx(447),
								(char*)(GUI_names_grammar(4, item_id, 0)));

							nice = GUI_input(g_dtp2, 2);

							if (nice < 0) {
								nice = 0;
							}

							if (host_readws(hero1 + (HERO_INVENTORY + INVENTORY_QUANTITY) + SIZEOF_INVENTORY * l15) < nice) {
								nice = host_readws(hero1 + (HERO_INVENTORY + INVENTORY_QUANTITY) + SIZEOF_INVENTORY * l15);
							}

							price -= ((Bit32s)host_readws((Bit8u*)g_sellitems + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws((Bit8u*)g_sellitems + 7 * (l6 + item) + 4) *
									tmp[hero_pos][l15] * g_price_modificator) / 4L;

							tmp[hero_pos][l15] = nice;

							price += ((Bit32s)host_readws((Bit8u*)g_sellitems + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws((Bit8u*)g_sellitems + 7 * (l6 + item) + 4) *
									tmp[hero_pos][l15] * g_price_modificator) / 4L;
						} else {
							tmp[hero_pos][l15] = 1;
							price += ((Bit32s)host_readws((Bit8u*)g_sellitems + 7 *(l6 + item) + 2) *
									(Bit32s)host_readws((Bit8u*)g_sellitems + 7 * (l6 + item) + 4) *
									nice * g_price_modificator) / 4L;
						}
					}
				}
			}
			l8 = 1;
		}


		if (g_action == ACTION_ID_ICON_1 && price) {

			j = nice = 0;
			for (items_x = 0; items_x <= 6; items_x++) {
				for (l_di = 0; l_di < 23; l_di++) {
					nice += tmp[items_x][l_di];
				}
			}

			/* 0xe83 - 11ba */
			while (l12 == 0 && j < 3) {

				make_valuta_str(g_text_output_buf, price);
				sprintf(g_dtp2,
					get_ttx(449),
					g_text_output_buf);


				do {
					percent = GUI_input(g_dtp2, 2);

				} while (percent > 50);

				if (percent == 0) {
					GUI_output(get_ttx(440));
					l12 = 2;
				} else if (percent >= percent_old) {
					j = 2;
				} else if (percent < 0) {
					break;
				} else {
					g_skilled_hero_pos = get_skilled_hero_pos(TA_FEILSCHEN);

					answer = select_hero_ok_forced(get_ttx(442));

					hero2 = get_hero(answer);

					l12 = bargain(hero2, nice, price, percent, 0) > 0 ? 1 : 0;
				}

				if (l12 > 0) {

					hero3 = get_hero(0);
					for (items_x = 0; items_x <= 6; items_x++, hero3 += SIZEOF_HERO) {
						for (l_di = 0; l_di < NR_HERO_INVENTORY_SLOTS; l_di++) {

							if (tmp[items_x][l_di] != 0) {
								item_id = host_readws(hero3 + (HERO_INVENTORY + INVENTORY_ITEM_ID) + SIZEOF_INVENTORY * l_di);
								drop_item(hero3, l_di, tmp[items_x][l_di]);
								g_market_itemsaldo_table[item_id] = g_market_itemsaldo_table[item_id] - tmp[items_x][l_di];

								if (g_market_itemsaldo_table[item_id] <= -10) {
									g_market_itemsaldo_table[item_id] = 0;
									sub_ptr_ws(get_itemsdat(item_id) + ITEM_STATS_PRICE, host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) * 10 / 100);
								}
							}
						}
					}

					price = price + percent * price / 100L;

					if (price > 0) {
						add_party_money(price);
					}

					if (l12 != 2) {
						GUI_output(get_ttx(451));

						make_valuta_str(g_text_output_buf, price);

						sprintf(g_dtp2, get_ttx(819), g_text_output_buf);
						GUI_output(g_dtp2);
					}

					l11 = 1;
					price = 0;

					for (items_x = 0; items_x <= 6; items_x++) {
						for (l_di = 0; l_di < 23; l_di++) {
							tmp[items_x][l_di] = 0;
						}
					}
					continue;
				}

				if (j == 2) {
					GUI_output(get_ttx(453));
					gs_merchant_offended_flags[gs_current_typeindex] = 1;
					done = 1;
				} else {
					GUI_output(get_ttx(450));
					percent_old = percent;
				}

				j++;
			}
		}

		if (g_action >= 241 && g_action <= 247) {

			hero_pos = g_action - 241;
			hero3 = get_hero(hero_pos);

			if ((host_readbs(hero3 + HERO_TYPE) != HERO_TYPE_NONE) &&
				host_readbs(hero3 + HERO_GROUP_NO) == gs_current_group)
			{
				hero1 = get_hero(hero_pos);
				deselect_hero_icon(hero_pos_old);
				select_hero_icon(hero_pos);
				hero_pos_old = hero_pos;
				l11 = 1;
			}
		}

		if (g_action == ACTION_ID_ICON_4) {
			l10 = 1;
		}

		if (g_action == ACTION_ID_ICON_5) {
			done = 1;
		}

		if (host_readbs(hero1 + HERO_NR_INVENTORY_SLOTS_FILLED) == 0) {
			done = 1;
		}
	}

	set_textcolor(fg_bak, bg_bak);
	g_request_refresh = 1;
	g_pp20_index = -1;
	g_heroswap_allowed = 1;
}

#if !defined(__BORLANDC__)
}
#endif
