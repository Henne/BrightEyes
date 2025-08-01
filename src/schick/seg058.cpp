/**
 *	Rewrite of DSA1 v3.02_de functions of seg058 (smith)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg058.cpp
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
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg031.h"
#include "seg047.h"
#include "seg056.h"
#include "seg058.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg104.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   add all items of a hero to the repair list
 *
 * \param   smith_ptr   pointer to the smith description
 * \param   hero        pointer to the hero
 * \param   item_pos    the position of the item in the inventory
 * \param   smith_pos   the position of the item in the repair list
 */
void add_item_to_smith(Bit8u *smith_ptr, Bit8u *hero, signed short item_pos, signed short smith_pos)
{
	signed short item_id;

	item_id = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * item_pos);

	host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos, item_id);

	if (item_armor(get_itemsdat(item_id)) || item_weapon(get_itemsdat(item_id))) {

		if (inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * item_pos)) {

			host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 2,
				(host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) +
					(host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) * host_readbs(smith_ptr + SMITH_STATS_PRICE_MOD) / 100)) / 2);

			if (host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 2) == 0) {
				host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 2, 1);
			}

			host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 4,
				host_readbs(get_itemsdat(item_id) + ITEM_STATS_PRICE_UNIT));

		} else {

			if (host_readbs(hero + HERO_INVENTORY + INVENTORY_RS_LOST + SIZEOF_INVENTORY * item_pos) != 0) {
				/* armor has degraded RS */

				host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 2,
					(host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) +
						(host_readws(get_itemsdat(item_id) + ITEM_STATS_PRICE) * host_readbs(smith_ptr + SMITH_STATS_PRICE_MOD) / 100)) / 4);

				if (host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 2) == 0) {
					host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 2, 1);
				}

				host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 4,
					host_readbs(get_itemsdat(item_id) + ITEM_STATS_PRICE_UNIT));
			} else {
				host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 2, 0);
				host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 4, 1);
			}
		}
	} else {
		host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 2, 0);
		host_writews((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 4, 1);
	}

	host_writebs((Bit8u*)ds_readd(SELLITEMS) + 7 * smith_pos + 6, (signed char)item_pos);
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

/**
 * \brief   shows the repair-screen an provides interaction
 *
 * \param   smith_ptr   pointer to the smith descriptor
 * \param   smith_id    ID of the smith [0,...,49]
 */
void repair_screen(Bit8u *smith_ptr, signed short smith_id)
{
	signed short l_si;
	signed short j;
	signed short items_x;
	signed short fg_bak;
	signed short bg_bak;
	signed short answer;
	signed short val;
	signed short percent;
	signed short percent_old = 100;
	signed short item_id;
	signed short l6;
	signed short l7;
	signed short done = 0;
	signed short item = 0;
	signed short l8;
	signed short smith_pos;
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
	struct dummy_c5 fmt_h = *(struct dummy_c5*)(p_datseg + SMITH_STR_MONEY_H);
	struct dummy_c5 fmt_s = *(struct dummy_c5*)(p_datseg + SMITH_STR_MONEY_S);
	struct dummy_c5 fmt_d = *(struct dummy_c5*)(p_datseg + SMITH_STR_MONEY_D);
	struct dummy3 array3 = *(struct dummy3*)(p_datseg + SMITH_ITEMS_POSX);
	struct dummy5 array5 = *(struct dummy5*)(p_datseg + SMITH_ITEMS_POSY);
#endif

	Bit32s price;
	Bit32s p_money;
	signed short l12 = 0;
	Bit8u *hero2;
	Bit8u *hero1;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	/* check if this smith has an item in repair */
	if (gs_smith_repairitems[smith_id].item_id) {

		if (gs_smith_repairitems[smith_id].pickup_time > gs_day_timer) {

			/* not ready yet */
			GUI_output(get_ttx(485));

		} else if (get_item(gs_smith_repairitems[smith_id].item_id, 1, 1)) {

			sprintf(g_dtp2, get_ttx(486),
				(char*)GUI_names_grammar((signed short)0x8002, gs_smith_repairitems[smith_id].item_id, 0));

			GUI_output(g_dtp2);

			gs_smith_repairitems[smith_id].item_id = 0;
			gs_smith_repairitems[smith_id].pickup_time = 0;
		}
	} else {

		set_var_to_zero();
		ds_writeb(PP20_INDEX, 0xff);

		draw_loc_icons(5, MENU_ICON_BARGAIN, MENU_ICON_SCROLL_RIGHT, MENU_ICON_SCROLL_LEFT, MENU_ICON_HERO, MENU_ICON_LEAVE);
		draw_main_screen();

		/* ICONS */
		l_si = load_archive_file(ARCHIVE_FILE_ICONS);
		seek_archive_file(l_si, 18 * 576L);
		read_archive_file(l_si, g_icon, 576L);
		close(l_si);

		g_pic_copy.x1 = 108;
		g_pic_copy.y1 = 5;
		g_pic_copy.x2 = 131;
		g_pic_copy.y2 = 28;
		g_pic_copy.src = g_icon;
		do_pic_copy(0);

		ds_writed(SELLITEMS, (Bit32u)g_fig_figure1_buf);
		memset((Bit8u*)ds_readd(SELLITEMS), 0, 350);

		get_textcolor(&fg_bak, &bg_bak);
		set_textcolor(255, 0);

		l8 = 1;

		while (done == 0) {

			if (l8 != 0 || l10 != 0 || l11 != 0) {

				/* refresh goods */

				if (l10 != 0) {

					hero_pos = select_hero_ok(get_ttx(495));

					hero2 = get_hero(hero_pos);

					deselect_hero_icon(hero_pos_old);
					select_hero_icon(hero_pos);
					hero_pos_old = hero_pos;
					l10 = 0;
					l11 = 1;
				}

				if (l11 != 0) {

					smith_pos = 0;
					for (l_si = 0; l_si < NR_HERO_INVENTORY_SLOTS; l_si++) {
						if (host_readws(hero2 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * l_si) != ITEM_NONE) {
							add_item_to_smith(smith_ptr, hero2, l_si, smith_pos++);
						}
					}

					for (l_si = smith_pos; l_si < 23; l_si++) {
						host_writew((Bit8u*)ds_readd(SELLITEMS) + 7 * l_si, 0);
					}

					l11 = 0;
					l6 = 1;
					l7 = item = l12 = 0;
					percent_old = 100;

					do_fill_rect(g_vga_memstart, 26, 26, 105, 33, 0);

					make_valuta_str(g_dtp2, host_readds(hero2 + HERO_MONEY));
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

					for (l_si = 0; l_si < 5; l_si++) {

						answer = 5 * items_x + l_si + item;

						if ((j = host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * answer))) {

							g_pic_copy.x1 = array3.a[items_x];
							g_pic_copy.y1 = array5.a[l_si];
							g_pic_copy.x2 = array3.a[items_x] + 15;
							g_pic_copy.y2 = array5.a[l_si] + 15;
							g_pic_copy.src = g_renderbuf_ptr;

							nvf.no = host_readws(get_itemsdat(j));

							process_nvf(&nvf);

							do_pic_copy(0);

							if (item_stackable(get_itemsdat(j))) {

								if ((val = host_readws(hero2 + (HERO_INVENTORY + INVENTORY_QUANTITY) + SIZEOF_INVENTORY * host_readbs((Bit8u*)ds_readd(SELLITEMS) + 7 * answer + 6))) > 1)
								{
									my_itoa(val, g_dtp2, 10);

									GUI_print_string(g_dtp2,
										array3.a[items_x] + 16 - GUI_get_space_for_string(g_dtp2, 0),
										array5.a[l_si] + 9);

								}
							}

							sprintf(g_dtp2,
								host_readws((Bit8u*)ds_readd(SELLITEMS) + 4 + 7 * answer) == 1 ? fmt_h.a :
									(host_readws((Bit8u*)ds_readd(SELLITEMS) + 4 + 7 * answer) == 10 ? fmt_s.a : fmt_d.a),
								host_readws((Bit8u*)ds_readd(SELLITEMS) + 2 + 7 * answer));


							GUI_print_string(g_dtp2, array3.a[items_x] + 20, array5.a[l_si] + 5);
						}
					}
				}

				refresh_screen_size();
				l6 = 1;
				l7 = 0;
				l8 = 0;

			}

			ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)(p_datseg + ACTION_TABLE_MERCHANT));
			handle_input();
			ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)0L);

			if (ds_readws(HAVE_MOUSE) == 2) {
				select_with_mouse((Bit8u*)&l7, (Bit8u*)ds_readd(SELLITEMS) + 7 * item);
			} else {
				select_with_keyboard((Bit8u*)&l7, (Bit8u*)ds_readd(SELLITEMS) + 7 * item);
			}

			if (l6 != l7) {

				do_border(g_vga_memstart,
					array3.a[l6 / 5] - 1,
					array5.a[l6 % 5] - 1,
					array3.a[l6 / 5] + 16,
					array5.a[l6 % 5] + 16,
					0);

				do_border(g_vga_memstart,
					array3.a[l7 / 5] - 1,
					array5.a[l7 % 5] - 1,
					array3.a[l7 / 5] + 16,
					array5.a[l7 % 5] + 16,
					-1);

				l6 = l7;

				clear_loc_line();

				GUI_print_loc_line((GUI_name_singular(get_itemname(host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * (l7 + item))))));
			}

			if (ds_readws(MOUSE2_EVENT) != 0  || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

				answer = GUI_radio(NULL, 5,
						get_ttx(433),
						get_ttx(435),
						get_ttx(436),
						get_ttx(446),
						get_ttx(437)) - 1;

				if (answer != -2) {
					ds_writew(ACTION, answer + ACTION_ID_ICON_1);
				}
			}

			if (ds_readws(ACTION) == ACTION_ID_ICON_3 && item != 0) {
				l8 = 1;
				item -= 15;
			} else if (ds_readws(ACTION) == ACTION_ID_ICON_2 && host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * (item + 15))) {
				l8 = 1;
				item += 15;
			}

			if (ds_readws(ACTION) == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK || ds_readws(ACTION) == ACTION_ID_ICON_1 || ds_readws(ACTION) == ACTION_ID_RETURN) {
				/* Is ACTION == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK possible at all?
				 * ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK can be written to ACTION in buy_screen(), but where should it show up in repair_screen()?? */

				item_id = host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * (l7 + item));

				p_money = get_party_money();

				if (host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * (l7 + item) + 2) == 0) {
					GUI_output(get_ttx(487));
				} else {


					j = 0;

					while (l12 == 0 && j < 3) {


						price = (host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * (l7 + item) + 2)
							* host_readws((Bit8u*)ds_readd(SELLITEMS) + 7 * (l7 + item) + 4)) * ds_readws(PRICE_MODIFICATOR) / 4;

						make_valuta_str(g_text_output_buf, price);

						sprintf(g_dtp2, get_ttx(488),
							(char*)GUI_names_grammar((signed short)0x8002, item_id, 0),
							g_text_output_buf);


						do {
							percent = GUI_input(g_dtp2, 2);

						} while (percent > 50);

						price -= (percent * price) / 100L;

						if (percent == 0 && p_money > price) {
							GUI_output(get_ttx(489));
							l12 = 2;
						} else if (percent >= percent_old) {
							j = 2;
						} else if (percent < 0) {
							break;
						} else {

							answer = select_hero_ok_forced(get_ttx(442));

							hero1 = get_hero(answer);

							l12 = bargain(hero1, 1, price, percent, 2) > 0 ? 1 : 0;
						}

						if (l12 > 0) {

							if (p_money < price) {
								GUI_output(get_ttx(401));
							} else {
								if (l12 != 2) {
									GUI_output(get_ttx(492));
								}

								gs_smith_repairitems[smith_id].item_id = item_id;

								if (gs_day_timer > HOURS(14)) {

									gs_smith_repairitems[smith_id].pickup_time = HOURS(23);
									GUI_output(get_ttx(490));
								} else {
									gs_smith_repairitems[smith_id].pickup_time = gs_day_timer + HOURS(6);
									GUI_output(get_ttx(491));
								}

								drop_item(hero2, host_readbs((Bit8u*)ds_readd(SELLITEMS) + 6 + 7 * (l7 + item)), 1);
								p_money -= price;
								set_party_money(p_money);

							}
							done = 1;
							continue;

						}
						 if (j == 2) {
							GUI_output(get_ttx(493));
							done = 1;
						} else {
							GUI_output(get_ttx(494));
							percent_old = percent;
						}

						j++;
					}
				}
			}

			if (ds_readws(ACTION) >= 241 && ds_readws(ACTION) <= 247) {

				hero_pos = ds_readws(ACTION) - 241;
				hero2 = get_hero(hero_pos);
				deselect_hero_icon(hero_pos_old);
				select_hero_icon(hero_pos);
				hero_pos_old = hero_pos;
				l11 = 1;
			}

			if (ds_readws(ACTION) == ACTION_ID_ICON_4) {
				l10 = 1;
			}

			if (ds_readws(ACTION) == ACTION_ID_ICON_5) {
				done = 1;
			}

		}

		set_textcolor(fg_bak, bg_bak);
		ds_writew(REQUEST_REFRESH, 1);
		ds_writeb(PP20_INDEX, 0xff);
	}
}

/**
 * \brief   handler for entering smith location
 */
void do_smith(void)
{
	signed short done = 0;
	signed short answer;
	Bit8u *smith_ptr;

	if (gs_day_timer < HOURS(6) || gs_day_timer > HOURS(20)) {

		GUI_output(get_ttx(483));
		leave_location();
		return;
	}

	if (gs_smith_kicked_flags[gs_current_typeindex] ||
		gs_smith_flogged_flags[gs_current_typeindex] ||
		(gs_current_typeindex == 1 && ds_readb(DNG14_CELLAREXIT_FLAG))) {

		talk_smith();
		leave_location();
		return;
	}

	load_ggsts_nvf();
	ds_writew(REQUEST_REFRESH, 1);
	smith_ptr = p_datseg + SMITH_DESCR_TABLE + SIZEOF_SMITH_STATS * gs_current_typeindex;
	ds_writew(PRICE_MODIFICATOR, 4);

	while (!done) {

		if (ds_readws(REQUEST_REFRESH) != 0) {

			draw_loc_icons(3, MENU_ICON_TALK, MENU_ICON_REPAIR, MENU_ICON_LEAVE);
			draw_main_screen();
			set_var_to_zero();
			load_ani(5);
			init_ani(0);
			GUI_print_loc_line(get_tx(gs_current_locdata));
			set_audio_track(ARCHIVE_FILE_SMITH_XMI);
			ds_writew(REQUEST_REFRESH, 0);
		}

		handle_gui_input();

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			g_textbox_width = 4;

			answer = GUI_radio(get_ttx(496), 3,
						get_ttx(343),
						get_ttx(497),
						get_ttx(498)) - 1;

			/* TODO: why should it be 3??? Better make a backup */
			g_textbox_width = 3;

			if (answer != -2) {
				ds_writew(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_3) {
			done = 1;
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_1) {

			talk_smith();
			ds_writew(REQUEST_REFRESH, 1);

			if (gs_smith_kicked_flags[gs_current_typeindex] ||
				gs_smith_flogged_flags[gs_current_typeindex] ||
				gs_dungeon_index != DUNGEONS_NONE)
			{
				done = 1;
			}
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {
			repair_screen(smith_ptr, gs_current_typeindex);
		}
	}

	leave_location();
	copy_palette();
}

void talk_smith(void)
{
	do_random_talk(13, 0);
}

void TLK_schmied(signed short state)
{
	if (!state) {
		g_dialog_next_state = (gs_smith_kicked_flags[gs_current_typeindex] ? 1 :
					(gs_current_typeindex == 17 ? 27 :
					(gs_current_typeindex == 1 && ds_readb(DNG14_CELLAREXIT_FLAG) != 0 ? 28 : 4)));
	} else if (state == 1) {
		g_dialog_next_state = (gs_smith_flogged_flags[gs_current_typeindex] ? 2 : 3);
	} else if (state == 3) {
		gs_smith_flogged_flags[gs_current_typeindex] = 1;
	} else if (state == 6 || state == 26) {
		tumult();
		gs_smith_kicked_flags[gs_current_typeindex] = gs_smith_flogged_flags[gs_current_typeindex] = 1;
	} else if (state == 11 || state == 14 || state == 16 || state == 23) {
		gs_smith_kicked_flags[gs_current_typeindex] = 1;
	} else if (state == 19 || state == 31) {
		ds_writew(PRICE_MODIFICATOR, 3);
	} else if (state == 30) {

		DNG_enter_dungeon(DUNGEONS_ZWINGFESTE);
		gs_dungeon_level = 3;
		gs_x_target_bak = (gs_x_target = (11));
		gs_y_target_bak = (gs_y_target = (2));
		gs_direction = (2);
	}
}

#if !defined(__BORLANDC__)
}
#endif
