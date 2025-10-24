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

static struct smith_descr g_smith_descr_table[42] = { /* 'quality' is apparently unused. */
	{  50,  1 },
	{  20,  4 },
	{   0, 10 },
	{ -20, 15 },
	{   0, 10 },
	{  30, 12 },
	{ -10,  9 },
	{  30, 10 },
	{  20,  2 },
	{   0,  9 },
	{ -40, 16 },
	{   0,  9 },
	{   0, 11 },
	{  50,  1 },
	{  70,  1 },
	{  10,  4 },
	{ -20,  9 },
	{  40,  6 },
	{ -10, 15 },
	{   0,  5 },
	{  50,  4 },
	{  10, 16 },
	{  30,  3 },
	{ -30, 11 },
	{  20,  1 },
	{ -50, 14 },
	{   0,  9 },
	{   0,  5 },
	{  20, 17 },
	{  10, 13 },
	{   0,  3 },
	{  10, 12 },
	{  30,  4 },
	{   0, 11 },
	{  10,  6 },
	{ -20, 13 },
	{  20,  2 },
	{   0, 13 },
	{ -10, 15 },
	{  30,  4 },
	{  30,  4 },
	{  -1,  0 }
}; // ds:0x6c10
static struct c_str_5 g_smith_str_money_h = { "H %d" }; // ds:0x6c64
static struct c_str_5 g_smith_str_money_s = { "S %d" }; // ds:0x6c69
static struct c_str_5 g_smith_str_money_d = { "D %d" }; // ds:0x6c6e
static struct Bit16s_3 g_smith_items_posx = { 30, 95, 160 }; // ds:0x6c73
static struct Bit16s_5 g_smith_items_posy = { 35, 55, 75, 95, 115 }; // ds:0x6c79
//unsigned char g_unkn_048[1] = { 0x00 }; // ds:0x6c83

signed short g_price_modificator; // ds:0xe3f6, price modificator for smith and sell

/**
 * \brief   add all items of a hero to the repair list
 *
 * \param   smith   pointer to the smith description
 * \param   hero        pointer to the hero
 * \param   item_pos    the position of the item in the inventory
 * \param   smith_pos   the position of the item in the repair list
 */
void add_item_to_smith(struct smith_descr *smith, struct struct_hero *hero, const signed int item_pos, signed short smith_pos)
{
	const signed short item_id = hero->inventory[item_pos].item_id;

	g_sellitems[smith_pos].item_id = item_id;

	if (g_itemsdat[item_id].flags.armor || g_itemsdat[item_id].flags.weapon) {

		if (((struct struct_hero*)hero)->inventory[item_pos].flags.broken) {

			g_sellitems[smith_pos].shop_price =
				(g_itemsdat[item_id].price + g_itemsdat[item_id].price * smith->price_mod / 100) / 2;

			if (g_sellitems[smith_pos].shop_price == 0) {

				g_sellitems[smith_pos].shop_price = 1;
			}

			g_sellitems[smith_pos].price_unit = g_itemsdat[item_id].price_unit;

		} else {

			if (hero->inventory[item_pos].rs_lost) {

				/* armor has degraded RS */

				g_sellitems[smith_pos].shop_price =
					(g_itemsdat[item_id].price + g_itemsdat[item_id].price * smith->price_mod / 100) / 4;

				if (g_sellitems[smith_pos].shop_price == 0) {

					g_sellitems[smith_pos].shop_price = 1;
				}

				g_sellitems[smith_pos].price_unit = g_itemsdat[item_id].price_unit;

			} else {
				/* price => 1 HELLER */
				g_sellitems[smith_pos].shop_price = 0;
				g_sellitems[smith_pos].price_unit = 1;
			}
		}
	} else {
		/* price => 1 HELLER */
		g_sellitems[smith_pos].shop_price = 0;
		g_sellitems[smith_pos].price_unit = 1;
	}

	g_sellitems[smith_pos].item_pos = item_pos;
}

/**
 * \brief   shows the repair-screen an provides interaction
 *
 * \param   smith_ptr   pointer to the smith descriptor
 * \param   smith_id    ID of the smith [0,...,49]
 */
void repair_screen(struct smith_descr *smith, signed short smith_id)
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
	signed short item_pos;
	signed short done = 0;
	signed short item = 0;
	signed short l8;
	signed short smith_pos;
	signed short l10 = 1;
	signed short l11 = 1;
	signed short hero_pos_old = 1;
	signed short hero_pos = -1;
	struct c_str_5 fmt_h = g_smith_str_money_h;
	struct c_str_5 fmt_s = g_smith_str_money_s;
	struct c_str_5 fmt_d = g_smith_str_money_d;
	struct Bit16s_3 array3 = g_smith_items_posx;
	//signed short array3[3] = { { 30, 95, 160 } };
	struct Bit16s_5 array5 = g_smith_items_posy;
	//signed short array5[5] = { { 35, 55, 75, 95, 115 } };

	Bit32s price;
	Bit32s p_money;
	signed short l12 = 0;
	struct struct_hero *hero2;
	struct struct_hero *hero_bargain;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	/* check if this smith has an item in repair */
	if (gs_smith_repairitems[smith_id].item_id) {

		if (gs_smith_repairitems[smith_id].pickup_time > gs_day_timer) {

			/* not ready yet */
			GUI_output(get_ttx(485));

		} else if (get_item(gs_smith_repairitems[smith_id].item_id, 1, 1)) {

			sprintf(g_dtp2, get_ttx(486), GUI_names_grammar((signed short)0x8002, gs_smith_repairitems[smith_id].item_id, 0));

			GUI_output(g_dtp2);

			gs_smith_repairitems[smith_id].pickup_time = 0;
			gs_smith_repairitems[smith_id].item_id = 0;
		}
	} else {

		set_var_to_zero();
		g_pp20_index = -1;

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

		g_sellitems = (struct shop_item*)g_fig_figure1_buf;
		memset((Bit8u*)g_sellitems, 0, 50 * sizeof(struct shop_item));

		get_textcolor(&fg_bak, &bg_bak);
		set_textcolor(255, 0);

		l8 = 1;

		while (done == 0) {

			if (l8 != 0 || l10 != 0 || l11 != 0) {

				/* refresh goods */

				if (l10 != 0) {

					hero_pos = select_hero_ok_forced(get_ttx(495));

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
						if (hero2->inventory[l_si].item_id != ITEM_NONE) {
							add_item_to_smith(smith, hero2, l_si, smith_pos++);
						}
					}

					for (l_si = smith_pos; l_si < 23; l_si++) {
						g_sellitems[l_si].item_id = 0;
					}

					l11 = 0;
					l6 = 1;
					item_pos = item = l12 = 0;
					percent_old = 100;

					do_fill_rect(g_vga_memstart, 26, 26, 105, 33, 0);

					make_valuta_str(g_dtp2, hero2->money);
					GUI_print_string(g_dtp2, 104 - GUI_get_space_for_string(g_dtp2, 0), 26);
				}

				update_mouse_cursor();

				do_fill_rect(g_vga_memstart, 29, 34, 214, 133, 0);

				do_v_line(g_vga_memstart, 87, 35, 131, -1);
				do_v_line(g_vga_memstart, 152, 35, 131, -1);

				nvf.dst = g_renderbuf_ptr;
				nvf.src = g_buffer10_ptr;
				nvf.type = 0;
				nvf.width =  &width;
				nvf.height = &height;

				for (items_x = 0; items_x < 3; items_x++) {

					for (l_si = 0; l_si < 5; l_si++) {

						answer = 5 * items_x + l_si + item;

						if ((j = g_sellitems[answer].item_id)) {

							g_pic_copy.x1 = array3.a[items_x];
							g_pic_copy.y1 = array5.a[l_si];
							g_pic_copy.x2 = array3.a[items_x] + 15;
							g_pic_copy.y2 = array5.a[l_si] + 15;
							g_pic_copy.src = g_renderbuf_ptr;

							nvf.no = g_itemsdat[j].gfx;

							process_nvf(&nvf);

							do_pic_copy(0);

							if (g_itemsdat[j].flags.stackable) {

								if ((val = hero2->inventory[g_sellitems[answer].item_pos].quantity) > 1)
								{
									my_itoa(val, g_dtp2, 10);

									GUI_print_string(g_dtp2,
										array3.a[items_x] + 16 - GUI_get_space_for_string(g_dtp2, 0),
										array5.a[l_si] + 9);

								}
							}

							sprintf(g_dtp2,	g_sellitems[answer].price_unit == 1 ? fmt_h.a :
										(g_sellitems[answer].price_unit == 10 ? fmt_s.a : fmt_d.a),
									g_sellitems[answer].shop_price);


							GUI_print_string(g_dtp2, array3.a[items_x] + 20, array5.a[l_si] + 5);
						}
					}
				}

				refresh_screen_size();
				l6 = 1;
				item_pos = 0;
				l8 = 0;

			}

			g_action_table_secondary = &g_action_table_merchant[0];
			handle_input();
			g_action_table_secondary = NULL;

			if (g_have_mouse == 2) {
				select_with_mouse(&item_pos, &g_sellitems[item]);
			} else {
				select_with_keyboard(&item_pos, &g_sellitems[item]);
			}

			if (l6 != item_pos) {

				do_border(g_vga_memstart,
					array3.a[l6 / 5] - 1,
					array5.a[l6 % 5] - 1,
					array3.a[l6 / 5] + 16,
					array5.a[l6 % 5] + 16,
					0);

				do_border(g_vga_memstart,
					array3.a[item_pos / 5] - 1,
					array5.a[item_pos % 5] - 1,
					array3.a[item_pos / 5] + 16,
					array5.a[item_pos % 5] + 16,
					-1);

				l6 = item_pos;

				clear_loc_line();

				GUI_print_loc_line(GUI_name_singular(get_itemname(g_sellitems[item_pos + item].item_id)));
			}

			if (g_mouse2_event  || g_action == ACTION_ID_PAGE_UP) {

				answer = GUI_radio(NULL, 5, get_ttx(433), get_ttx(435), get_ttx(436), get_ttx(446), get_ttx(437)) - 1;

				if (answer != -2) {
					g_action = answer + ACTION_ID_ICON_1;
				}
			}

			if (g_action == ACTION_ID_ICON_3 && item) {

				l8 = 1;
				item -= 15;

			} else if (g_action == ACTION_ID_ICON_2 && g_sellitems[item + 15].item_id) {

				l8 = 1;
				item += 15;
			}

			if (g_action == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK || g_action == ACTION_ID_ICON_1 || g_action == ACTION_ID_RETURN) {
				/* Is ACTION == ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK possible at all?
				 * ACTION_ID_DECREASE_ITEM_COUNT_BY_RIGHT_CLICK can be written to ACTION in buy_screen(), but where should it show up in repair_screen()?? */

				item_id = g_sellitems[item_pos + item].item_id;

				p_money = get_party_money();

				if (g_sellitems[item_pos + item].shop_price == 0) {

					GUI_output(get_ttx(487));

				} else {

					j = 0;

					while (l12 == 0 && j < 3) {


						price = (g_sellitems[item_pos + item].shop_price
							* g_sellitems[item_pos + item].price_unit * g_price_modificator) / 4;

						make_valuta_str(g_text_output_buf, price);

						sprintf(g_dtp2, get_ttx(488), GUI_names_grammar((signed short)0x8002, item_id, 0), g_text_output_buf);

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

							hero_bargain = get_hero(answer);

							l12 = bargain(hero_bargain, 1, price, percent, 2) > 0 ? 1 : 0;
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

								drop_item(hero2, g_sellitems[item_pos + item].item_pos, 1);
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

			if (g_action >= 241 && g_action <= 247) {

				hero_pos = g_action - 241;
				hero2 = get_hero(hero_pos);
				deselect_hero_icon(hero_pos_old);
				select_hero_icon(hero_pos);
				hero_pos_old = hero_pos;
				l11 = 1;
			}

			if (g_action == ACTION_ID_ICON_4) {
				l10 = 1;
			}

			if (g_action == ACTION_ID_ICON_5) {
				done = 1;
			}

		}

		set_textcolor(fg_bak, bg_bak);
		g_request_refresh = 1;
		g_pp20_index = -1;
	}
}

/**
 * \brief   handler for entering smith location
 */
void do_smith(void)
{
	signed short done = 0;
	signed short answer;
	struct smith_descr *smith;

	if (gs_day_timer < HOURS(6) || gs_day_timer > HOURS(20)) {

		GUI_output(get_ttx(483));
		leave_location();
		return;
	}

	if (gs_smith_kicked_flags[gs_current_typeindex] ||
		gs_smith_flogged_flags[gs_current_typeindex] ||
		(gs_current_typeindex == 1 && gs_dng14_cellarexit_flag)) {

		talk_smith();
		leave_location();
		return;
	}

	load_ggsts_nvf();
	g_request_refresh = 1;
	smith = &g_smith_descr_table[gs_current_typeindex];
	g_price_modificator = 4;

	while (!done) {

		if (g_request_refresh != 0) {

			draw_loc_icons(3, MENU_ICON_TALK, MENU_ICON_REPAIR, MENU_ICON_LEAVE);
			draw_main_screen();
			set_var_to_zero();
			load_ani(5);
			init_ani(0);
			GUI_print_loc_line(get_tx(gs_current_locdata));
			set_audio_track(ARCHIVE_FILE_SMITH_XMI);
			g_request_refresh = 0;
		}

		handle_gui_input();

		if (g_mouse2_event || g_action == ACTION_ID_PAGE_UP) {

			g_textbox_width = 4;

			answer = GUI_radio(get_ttx(496), 3, get_ttx(343), get_ttx(497), get_ttx(498)) - 1;

			/* TODO: why should it be 3??? Better make a backup */
			g_textbox_width = 3;

			if (answer != -2) {
				g_action = answer + ACTION_ID_ICON_1;
			}
		}

		if (g_action == ACTION_ID_ICON_3) {
			done = 1;
		} else if (g_action == ACTION_ID_ICON_1) {

			talk_smith();
			g_request_refresh = 1;

			if (gs_smith_kicked_flags[gs_current_typeindex] ||
				gs_smith_flogged_flags[gs_current_typeindex] ||
				gs_dungeon_index != DUNGEONS_NONE)
			{
				done = 1;
			}
		} else if (g_action == ACTION_ID_ICON_2) {
			repair_screen(smith, gs_current_typeindex);
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
					(gs_current_typeindex == 1 && gs_dng14_cellarexit_flag ? 28 : 4)));
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
		g_price_modificator = 3;
	} else if (state == 30) {

		DNG_enter_dungeon(DUNGEONS_ZWINGFESTE);

		gs_dungeon_level = 3;
		gs_x_target_bak = gs_x_target = 11;
		gs_y_target_bak = gs_y_target = 2;
		gs_direction = 2;
	}
}

#if !defined(__BORLANDC__)
}
#endif
