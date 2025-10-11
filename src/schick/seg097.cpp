/*
 *      Rewrite of DSA1 v3.02_de functions of seg097 (GUI)
 *      Functions rewritten: 16/16 (complete)
 *
 *      Functions called rewritten 13/13
 *      Functions uncalled rewritten 3/3
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg097.cpp
 */

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "v302de.h"
#include "common.h"

#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg026.h"

#include "seg078.h"
#include "seg080.h"
#include "seg081.h"
#include "seg084.h"
#include "seg086.h"
#include "seg088.h"
#include "seg090.h"
#include "seg091.h"

#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed int g_menu_default_select;	// ds:0xe5ac
signed int g_menu_input_busy;		// ds:0xe5ae
signed int g_menu_selected;		// ds:0xe5b0

#if defined(__BORLANDC__)
void GUI_unused1(char *a1, signed short a2, signed short a3)
{
	signed short l1;
	signed short l2;
	signed char c;

	l1 = 0;

	update_mouse_cursor();

	if (g_gui_text_centered == 1) {
		a2 = GUI_get_first_pos_centered(a1, a2, g_textline_maxlen, 1);
	}

	l2 = a2;

	while ((c = a1[l1++])) {

		if ((c == 0x0d) || (c == 0x40)) {
			a3 += 10;

			a2 = (g_gui_text_centered == 1) ?
				GUI_get_first_pos_centered(a1 + l1, g_textline_posx, g_textline_maxlen, 1) : l2;

		} else if (c == '~') {
			if (a2 < g_txt_tabpos[0]) {
				a2 = g_txt_tabpos[0];
			} else if (a2 < g_txt_tabpos[1]) {
				a2 = g_txt_tabpos[1];
			} else if (a2 < g_txt_tabpos[2]) {
				a2 = g_txt_tabpos[2];
			} else if (a2 < g_txt_tabpos[3]) {
				a2 = g_txt_tabpos[3];
			} else if (a2 < g_txt_tabpos[4]) {
				a2 = g_txt_tabpos[4];
			} else if (a2 < g_txt_tabpos[5]) {
				a2 = g_txt_tabpos[5];
			} else if (a2 < g_txt_tabpos[6]) {
				a2 = g_txt_tabpos[6];
			}
		} else if ((c == (signed char)0xf0) || (c == (signed char)0xf1) || (c == (signed char)0xf2) || (c == (signed char)0xf3)) {
			g_textcolor_index = (unsigned char)c + 0xff10;
		} else {
			a2 += GUI_unused2(c, (Bit8u*)MK_FP(a3, a2));
		}
	}

	refresh_screen_size();
}

signed short GUI_unused2(signed short c, Bit8u* p)
{
	signed short v1;
	signed short v2;

	v2 = GUI_lookup_char_height((signed char)c, &v1);

	GUI_1c2(v2, v1, p);

	return v1;
}
#endif

signed short GUI_lookup_char_height(signed char c, signed short *p)
{
	signed short i;

	for (i = 0; i != 201; i += 3) {
		if (c == ((signed char*)g_gui_char_height)[i]) {

			*p = ((signed char*)g_gui_char_height)[i + 2] & 0xff;
			return ((signed char*)g_gui_char_height)[i + 1] & 0xff;
		}
	}

	if ((c == '~') || (c == (signed char)0xf0) || (c == (signed char)0xf1) || (c == (signed char)0xf2) || (c == (signed char)0xf3))
	{
		return *p = 0;
	}

	*p = 8;

	return 0;
}

#if defined(__BORLANDC__)
void GUI_1c2(signed short index, signed short v2, Bit8u* v3)
{

	GUI_blank_char();
	GUI_font_to_buf(g_splash_buffer + index * 8);
	GUI_write_char_to_screen(v3, 8, v2);
}
#endif

//static
signed short GUI_enter_text(char* dst, signed short x, signed short y, signed short num, signed short zero)
{
	signed short di;
	register signed short si;
	signed short pos;
	signed short c;
	char *dst_start;
	signed short length;

	dst_start = dst;
	update_mouse_cursor();
	di = x;
	pos = 0;
	length = strlen((char*)dst);

	if (zero == 0) {
		for (si = 0; si < num; si++) {
			if ((g_gui_entering_savegame) && (length >= si)) {
				GUI_print_char(0x20, di, y);
				GUI_print_char(*dst, di, y);
				pos++;
				dst++;
				di += 6;
				x = di;
			} else {
				GUI_print_char(0x20, di, y);
				GUI_print_char(0x5f, di, y);
				di += 6;
			}
		}
		di = x;
	} else {
		GUI_print_char(0x20, di, y);
		GUI_print_char(0x5f, di, y);
	}

	wait_for_keyboard2();
	g_mouse1_event1 = 0;

	c = 0;
	while ((c != 0xd) || (pos == 0)) {
		do {
			goto dummy;
dummy:

			/* This loop is evil */
			do {;} while ((CD_bioskey(1) == 0) && !g_mouse1_event1);

			if (g_mouse1_event1) {
				g_bioskey_event = 0x0d;
				g_mouse1_event1 = g_mouse1_event2 = 0;
			} else {
				g_action = (g_bioskey_event = bioskey(0)) >> 8;
				g_bioskey_event &= 0xff;
			}

		} while ((g_action == 0) && (g_bioskey_event == 0));

		c = g_bioskey_event;

		if (c == 0x0d) {

		} else if (g_action == ACTION_ID_ESC) {
			*dst_start = 0;
			refresh_screen_size();
			g_action = 0;
			return -1;
		} else if (c == 8) {
			if (pos > 0) {

				if ((zero == 1) && (pos != num)) {
					GUI_print_char(0x20, di, y);
				}

				di -= 6;
				pos--;
				dst--;
				GUI_print_char(0x20, di, y);
				GUI_print_char(0x5f, di, y);
			}
		} else

		/* check if character is valid */
		if (((c >= 0x20) && (c <= 0x7a)) ||
			(c == 0x81) || (c == 0x84) || (c == 0x94))
		{
			if (isalpha(c)) c = toupper(c);

			/* ae */
			if (c == 0x84)
				c = (signed char)0x8e;
			/* oe */
			if (c == 0x94)
				c = (signed char)0x99;
			/* ue */
			if (c == 0x81)
				c = (signed char)0x9a;

			/* are we at the end of the input field ? */
			if (pos == num) {
				dst--;
				di -= 6;
				pos--;
			}

			*dst++ = (unsigned char)c;
			GUI_print_char(0x20, di, y);
			GUI_print_char((unsigned char)c, di, y);
			di += 6;
			pos++;

			if ((zero == 1) && (pos != num)) {
				GUI_print_char(0x20, di, y);
				GUI_print_char(0x5f, di, y);
			}
		}
	}

	if (zero == 0) {
		while (pos < num) {
			GUI_print_char(0x20, di, y);
			di += 6;
			pos++;
		}
	}

	*dst = 0;
	refresh_screen_size();

	return 0;
}

//static
void GUI_draw_radio_bg(signed short header, signed short options, signed short width, signed short height)
{
	signed short i;

	/* set upper left coordinates */
	g_pic_copy.x1 = g_textbox_pos_x;
	g_pic_copy.y1 = g_textbox_pos_y;
	/* set lower right coordinates */
	g_pic_copy.x2 = g_textbox_pos_x + width - 1;
	g_pic_copy.y2 = g_textbox_pos_y + height - 1;
	/* set pointer */
	g_pic_copy.src = g_gui_buffer_unkn;
	do_save_rect();

	/* play FX3.VOC */
	if (!g_pregame_state)
		play_voc(ARCHIVE_FILE_FX3_VOC);

	GUI_draw_popup_line(0, 0);

	for (i = 0; i < header; i++)
		GUI_draw_popup_line(i + 1, 1);

	if (options != 0)
		for (i = 0; i < options; i++)
			GUI_draw_popup_line(header + i + 1, 2);

	GUI_draw_popup_line(header + options + 1, 3);

	set_textcolor(0xff, 0xdf);
	wait_for_keyboard1();
}

void GUI_copy_smth(unsigned short width, unsigned short height)
{
	g_pic_copy.x1 = g_textbox_pos_x;
	g_pic_copy.y1 = g_textbox_pos_y;
	g_pic_copy.x2 = g_textbox_pos_x + width - 1;
	g_pic_copy.y2 = g_textbox_pos_y + height - 1;
	g_pic_copy.src = g_gui_buffer_unkn;
	do_pic_copy(0);
}

void GUI_output(char *str)
{
	GUI_input(str, 0);
}

signed short GUI_input(char *str, unsigned short num)
{
	signed short retval;
	signed short l2;
	signed short fg_bak;
	signed short bg_bak;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;
	signed short l7;
	signed short l_si;
	signed short l_di;

	retval = 0;

	l7 = g_update_statusline;
	g_update_statusline = 0;

	if (!str || !(*str) || g_autofight != 0)
		return -1;

	l6 = g_wallclock_update;
	g_wallclock_update = 0;
	g_dialogbox_lock = 1;
	g_gui_text_centered = 1;

	l3 = g_textline_posx;
	l4 = g_textline_posy;
	l5 = g_textline_maxlen;

	l_di = (g_textbox_width * 32) + 32;
	g_textbox_pos_x = ((signed short)(320u - l_di) >> 1) + g_basepos_x;

	g_textline_posx = (g_textbox_pos_x + 5);
	g_textline_maxlen = (l_di - 8);

	l_si = GUI_count_lines(str);

	if (num != 0)
		l_si += 2;

	l2 = (l_si + 2) * 8;

	g_textbox_pos_y = ((signed short)(200u - l2) >> 1) + g_basepos_y;
	g_textline_posy = (g_textbox_pos_y + 7);

	get_textcolor(&fg_bak, &bg_bak);

	update_mouse_cursor();

	GUI_draw_radio_bg(l_si, 0, l_di, l2);

	GUI_print_header(str);

	g_mouse2_event = 0;

	refresh_screen_size();

	if (num != 0) {
		if (GUI_enter_text(g_text_input_buf, g_textbox_pos_x + ((signed short)(l_di - num * 6) >> 1), g_textbox_pos_y + l_si * 8 -2, num, 0) != -1) {
			retval = (signed short)atol(g_text_input_buf);
		} else {
			retval = -1;
		}
	} else {
		/* set action table */
		g_action_table_secondary = &g_action_table_menu[0];

		if (g_bioskey_event10 != 0) {
			wait_for_keypress();
		} else {
			delay_or_keypress(l_si * g_textbox_width * 50);
		}

		/* delete action table */
		g_action_table_secondary = NULL;
	}

	set_textcolor(fg_bak, bg_bak);

	update_mouse_cursor();

	GUI_copy_smth(l_di, l2);

	refresh_screen_size();

	g_textline_posx = l3;
	g_textline_posy = l4;
	g_textline_maxlen = l5;

	g_action = 0;
	g_dialogbox_lock = 0;

	g_wallclock_update = l6;
	g_gui_text_centered = 0;
	g_update_statusline = l7;

	return retval;
}

/**
 * \brief   shows a text box with two radio buttons "Ja" (yes) and "Nein" (no).
 *
 * \param   text	the displayed text
 */
signed short GUI_bool(char *text)
{
	signed short ret_radio;

	g_gui_bool_flag = 1;

	ret_radio = GUI_radio(text, 2, get_ttx(2), get_ttx(3));

	g_gui_bool_flag = 0;

	return (ret_radio == 1) ? 1 : 0;
}


//static
void GUI_fill_radio_button(signed short old_pos, unsigned short new_pos,
	unsigned short offset)
{
	signed short i;
	signed short x;
	signed short y;

	update_mouse_cursor();

	y = g_textbox_pos_x + 6;

	if (old_pos != -1) {

		x = g_textbox_pos_y + (offset + old_pos) * 8 + 2;

		/* clear the old button */
		for (i = 0; i < 4; i++)
			do_v_line(g_vga_memstart, y + i, x, x + 3, (signed char)0xd8);
	}

	x = g_textbox_pos_y + (offset + new_pos) * 8 + 2;

	/* fill the new button */
	for (i = 0; i < 4; i++)
		do_v_line(g_vga_memstart, y + i, x, x + 3, (signed char)0xd9);

	refresh_screen_size();
}

signed short GUI_dialogbox(Bit8u* picture, char *name, char *text, signed short options, ...)
{
	va_list arguments;
	signed short i;
	signed short l2, l3, l4, l5;
	signed short tw_bak;
	signed short fg_bak, bg_bak;
	signed short l7, l8, l9, l10;
	signed short retval;
	signed short l11, l12, l13;
	signed short l_si, l_di;

	l13 = g_ani_enabled;
	l12 = g_update_statusline;
	g_update_statusline = 0;

	set_var_to_zero();

	l11 = g_wallclock_update;
	g_wallclock_update = 0;
	g_dialogbox_lock = 1;
	l7 = g_textline_posx;
	l8 = g_textline_posy;
	l9 = g_textline_maxlen;
	tw_bak = g_textbox_width;
	g_textbox_width = 9;

	l_di = g_textbox_width * 32 + 32;
	g_textbox_pos_x = ((signed short)(320 - l_di) >> 1) + g_basepos_x;
	g_textline_posx = (g_textbox_pos_x + 5);
	g_textline_maxlen = (l_di - 8);
	l10 = g_txt_tabpos[0];
	g_txt_tabpos[0] = (g_textline_posx + g_textline_maxlen - 24);
	g_dialogbox_indent_width = 40;
	g_dialogbox_indent_height = 5;

	l_si = GUI_count_lines(text) - 1;

	if (name)
		l_si += 2;

	if (l_si < g_dialogbox_indent_height)
		l_si = g_dialogbox_indent_height - 1;

	l4 = l_si + (signed char)options;
	l5 = (l4 + 2) * 8;
	g_textbox_pos_y = (200 - (l5 + 2)) >> 1;
	g_textline_posy = (g_textbox_pos_y + 5);

	update_mouse_cursor();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(l_si, (signed char)options, l_di, l5);

	if (picture != 0) {

		/* draw a frame */
		do_border(g_vga_memstart, g_textbox_pos_x + 5, g_textbox_pos_y + 6, g_textbox_pos_x + 38, g_textbox_pos_y + 39, (signed char)0xff);

		/* set the coordinates */
		g_pic_copy.x1 = g_textbox_pos_x + 6;
		g_pic_copy.y1 = g_textbox_pos_y + 7;
		g_pic_copy.x2 = g_textbox_pos_x + 37;
		g_pic_copy.y2 = g_textbox_pos_y + 38;
		g_pic_copy.src = picture;

		do_pic_copy(0);
	}

	if (name) {
		/* set text color */
		g_textcolor_index = 1;

		GUI_print_string(name, g_textline_posx, g_textline_posy);

		/* set text color */
		g_textcolor_index = 1;

		g_textline_posy += 14;
		g_dialogbox_indent_height -= 2;
	}

	if (l_si != 0) {
		GUI_print_header(text);
	}

	g_dialogbox_indent_width = g_dialogbox_indent_height = 0;

	if ((signed char)options != 0) {

		l2 = g_textline_posx + 8;
		l3 = g_textbox_pos_y + (l_si + 1) * 8;

		va_start(arguments, options);
		for (i = 0; i < (signed char)options; l3 += 8, i++) {
			GUI_print_string((char*)va_arg(arguments, char*), l2, l3);
		}
	}

	va_end(arguments);

	retval = GUI_menu_input((signed char)options, l_si + 1, l_di);

	GUI_copy_smth(l_di, l5);

	refresh_screen_size();
	set_textcolor(fg_bak, bg_bak);

	g_textline_posx = l7;
	g_textline_posy = l8;
	g_textline_maxlen = l9;

	g_textbox_width = tw_bak;

	g_txt_tabpos[0] = l10;

	g_wallclock_update = l11;

	g_action = (g_dialogbox_lock = 0);

	g_update_statusline = l12;

	if (l13 != 0)
		init_ani(2);

	return retval;
}

//static
signed short GUI_menu_input(signed short positions, signed short h_lines, signed short width)
{
	volatile signed short l1, l2, l3, l4, l5, l6;
	signed short done;
	signed short retval;

	l5 = -1;
	done = 0;

	g_menu_selected = g_menu_input_busy = 1;
	g_menu_selected += g_menu_default_select;

	if (positions != 0) {
		l6 = h_lines * 8;
		l3 = g_mouse_posx;
		l4 = g_mouse_posy;
		g_mouse_posx_bak = g_mouse_posx = g_textbox_pos_x + 90;
		l1 = g_textbox_pos_y + l6;

		g_mouse_posy_bak = g_mouse_posy = l2 = l1 + 8 * g_menu_default_select;

		mouse_move_cursor(g_mouse_posx, g_mouse_posy);

		g_mouse_posx_max = g_textbox_pos_x + width - 16;
		g_mouse_posx_min = g_textbox_pos_x;
		g_mouse_posy_min = g_textbox_pos_y + l6;

		g_mouse_posy_max = l6 + g_textbox_pos_y - 1 + positions * 8;
		refresh_screen_size();

		g_mouse1_event2 = g_mouse1_event1 = g_mouse2_event = 0;

		while (!done) {
			g_action_table_secondary = &g_action_table_menu[0];
			handle_input();
			g_action_table_secondary = NULL;

			if (l5 != g_menu_selected) {
				GUI_fill_radio_button(l5, g_menu_selected, h_lines - 1);
				l5 = g_menu_selected;
			}

			if (g_mouse2_event || g_action == ACTION_ID_ESC || g_action == ACTION_ID_PAGE_DOWN) {
				/* close menu */

				retval = -1;
				done = 1;
				g_mouse2_event = 0;
			}

			if (g_action == ACTION_ID_RETURN) {
				retval = g_menu_selected;
				done = 1;
			}

			/* TODO: ... */
			if (g_action == ACTION_ID_UP) {
				if (g_menu_selected-- == 1)
					g_menu_selected = positions;
			}

			if (g_action == ACTION_ID_DOWN) {
				if (g_menu_selected++ == positions)
					g_menu_selected = 1;
			}

			if (g_mouse_posy != l2) {
				l2 = g_mouse_posy;
				g_menu_selected = ((l2 - l1) >> 3) + 1;
			}

			if (g_gui_bool_flag) {
				/* in yes-no-mode, answer "Ja" (yes) can be selected with the 'J' key,
				 * and answer "Nein" (no) can be selected with the 'N' key. */
				if (g_action == ACTION_ID_J) {
					retval = done = 1;
				} else if (g_action == ACTION_ID_N) {
					retval = 2;
					done = 1;
				}
			}
		}

		update_mouse_cursor();

		g_mouse_posx_bak = g_mouse_posx = l3;
		g_mouse_posy_bak = g_mouse_posy = l4;
		g_mouse_posx_max = 319;
		g_mouse_posx_min = 0;
		g_mouse_posy_min = 0;
		g_mouse_posy_max = 199;

		mouse_move_cursor(g_mouse_posx, g_mouse_posy);

	} else {
		do {
			delay_or_keypress(10000);

		} while (g_action == 0);

		retval = -1;
	}

	g_menu_default_select = g_menu_input_busy = 0;

	return retval;
}

signed short GUI_radio(char *text, signed char options, ...)
{
	signed short i;
	signed short l_di;

	va_list arguments;
	signed short l3, l4, l5, l6;
	signed short fg_bak, bg_bak;
	signed short l7, l8, l9, l10, l11;
	signed short retval;
	signed short l12;

	l12 = g_update_statusline;
	g_update_statusline = 0;

	if (!options) {
		GUI_output(text);
		return 0;
	}


	g_dialogbox_lock = 1;
	l7 = g_textline_posx;
	l8 = g_textline_posy;
	l9 = g_textline_maxlen;

	l11 = g_textbox_width * 32 + 32;
	g_textbox_pos_x = ((320 - l11) >> 1) + g_basepos_x;
	g_textline_posx = (g_textbox_pos_x + 5);
	g_textline_maxlen = (l11 - 8);

	l10 = g_txt_tabpos[0];
	g_txt_tabpos[0] = (g_textline_posx + g_textline_maxlen - 24);

	l_di = GUI_count_lines(text);
	l5 = l_di + options;
	l6 = (l5 + 2) * 8;
	g_textbox_pos_y = ((200 - l6 + 2) >> 1) + g_basepos_y;
	g_textline_posy = (g_textbox_pos_y + 7);

	update_mouse_cursor();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(l_di, options, l11, l6);

	if (l_di != 0)
		GUI_print_header(text);

	l3 = g_textline_posx + 8;
	l4 = g_textbox_pos_y + (l_di + 1) * 8;

	va_start(arguments, options);
	for (i = 0; i < options; l4 += 8, i++) {

		/* highlight special option */
		if ((g_game_mode == GAME_MODE_BEGINNER) && (g_skilled_hero_pos == i))
			set_textcolor(0xc9, 0xdf);

		GUI_print_string((char*)va_arg(arguments, char*), l3, l4);

		/* reset highlight special option */
		if ((g_game_mode == GAME_MODE_BEGINNER) && (g_skilled_hero_pos == i))
			set_textcolor(0xff, 0xdf);
	}

	retval = GUI_menu_input(options, l_di + 1, l11);

	GUI_copy_smth(l11, l6);
	refresh_screen_size();
	set_textcolor(fg_bak, bg_bak);

	g_textline_posx = l7;
	g_textline_posy = l8;
	g_textline_maxlen = l9;
	g_txt_tabpos[0] = l10;
	g_action = (g_dialogbox_lock = 0);
	g_update_statusline = l12;

	return retval;
}

/**
 * \brief   shows a fight intro message
 *
 * \param   fight_id    ID of the fight
 */
void GUI_print_fight_intro_msg(signed short fight_id)
{
	signed short tw_bak = g_textbox_width;
	g_textbox_width = 7;

	if (gs_dungeon_index == DUNGEONS_VERFALLENE_HERBERGE) {
		DNG02_fight_intro(fight_id);
	}
	if (gs_dungeon_index == DUNGEONS_GOBLINHOEHLE) {
		DNG5_fight_intro(fight_id);
	}
	if (gs_dungeon_index == DUNGEONS_DASPOTASCHATZ) {
		DNG06_fight_intro(fight_id);
	}
	if (gs_dungeon_index == DUNGEONS_KULTSTAETTE_DES_NAMENLOSEN) {
		DNG09_fight_intro(fight_id);
	}
	if (gs_dungeon_index == DUNGEONS_PIRATENHOEHLE) {
		DNG11_fight_intro(fight_id);
	}
	if (gs_dungeon_index == DUNGEONS_ZWERGENFESTE) {
		DNG12_fight_intro(fight_id);
	}
	if (gs_dungeon_index == DUNGEONS_VERLASSENE_MINE) {
		DNG13_fight_intro(fight_id);
	}
	if (gs_dungeon_index == DUNGEONS_ZWINGFESTE) {
		DNG14_fight_intro(fight_id);
	}

	g_textbox_width = tw_bak;
}

/**
 * \brief   print a dialog window without answer options
 *
 * \param   head_index  the number of a head, if another should be loaded
 * \param   text        the text
 */
void GUI_dialog_na(unsigned short head_index, char *text)
{

	if (head_index != 0)
		load_in_head(head_index);

	GUI_dialogbox((unsigned char*)g_dtp2, NULL, text, 0);

}

#if !defined(__BORLANDC__)
}
#endif
