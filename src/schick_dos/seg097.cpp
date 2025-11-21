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

static const struct struct_char_height g_gui_char_height[67] = {
	{        ' ',  0, 8 },
	{        'A',  1, 8 },
	{        'B',  2, 8 },
	{        'C',  3, 7 },
	{        'D',  4, 7 },
	{        'E',  5, 7 },
	{        'F',  6, 7 },
	{        'G',  7, 8 },
	{        'H',  8, 8 },
	{        'I',  9, 6 },
	{        'J', 10, 8 },
	{        'K', 11, 8 },
	{        'L', 12, 6 },
	{        'M', 13, 8 },
	{        'N', 14, 8 },
	{        'O', 15, 8 },
	{        'P', 16, 7 },
	{        'Q', 17, 8 },
	{        'R', 18, 8 },
	{        'S', 19, 7 },
	{        'T', 20, 7 },
	{        'U', 21, 8 },
	{        'V', 22, 8 },
	{        'W', 23, 8 },
	{        'X', 24, 7 },
	{        'Y', 25, 7 },
	{        'Z', 26, 7 },
	{ (char)0x8e, 27, 8 },
	{ (char)0x99, 28, 8 },
	{ (char)0x9a, 29, 8 },
	{        ',', 30, 5 },
	{        '.', 31, 4 },
	{        '-', 32, 6 },
	{        '(', 33, 6 },
	{        ')', 34, 6 },
	{        '!', 35, 4 },
	{        '?', 36, 8 },
	{        '0', 37, 8 },
	{        '1', 38, 6 },
	{        '2', 39, 8 },
	{        '3', 40, 8 },
	{        '4', 41, 8 },
	{        '5', 42, 8 },
	{        '6', 43, 8 },
	{        '7', 44, 7 },
	{        '8', 45, 8 },
	{        '9', 46, 8 },
	{       '\'', 47, 5 },
	{        ':', 48, 5 },
	{ (char)0xc8, 49, 8 },
	{ (char)0xc9, 50, 8 },
	{ (char)0xca, 51, 6 },
	{ (char)0xcb, 52, 5 },
	{ (char)0xcc, 53, 6 },
	{ (char)0xcd, 54, 6 },
	{ (char)0xce, 55, 7 },
	{ (char)0xcf, 56, 6 },
	{ (char)0xd0, 57, 3 },
	{ (char)0xd1, 58, 8 },
	{ (char)0xd2, 59, 8 },
	{ (char)0xd3, 60, 6 },
	{ (char)0xd4, 61, 8 },
	{ (char)0xd5, 62, 5 },
	{ (char)0xd6, 63, 8 },
	{ (char)0xd7, 64, 6 },
	{ (char)0xd8, 65, 8 },
	{ (char)0xd9, 66, 6 }
}; // ds:0xab42
static signed int g_gui_bool_flag = 0; // ds:0xac0b


signed int g_menu_default_select;	// ds:0xe5ac
signed int g_menu_input_busy;		// ds:0xe5ae
signed int g_menu_selected;		// ds:0xe5b0

#if defined(__BORLANDC__)
void GUI_unused1(char *str, signed int x, signed int y)
{
	signed int i = 0;
	signed int x_bak;
	signed char c;

	call_mouse_bg();

	if (g_gui_text_centered == 1) {
		x = GUI_get_first_pos_centered(str, x, g_textline_maxlen, 1);
	}

	x_bak = x;

	while ((c = str[i++])) {

		if ((c == 0x0d) || (c == '@')) {

			y += 10;

			x = (g_gui_text_centered == 1) ?
				GUI_get_first_pos_centered(str + i, g_textline_posx, g_textline_maxlen, 1) : x_bak;

		} else if (c == '~') {
			if (x < g_txt_tabpos[0]) {
				x = g_txt_tabpos[0];
			} else if (x < g_txt_tabpos[1]) {
				x = g_txt_tabpos[1];
			} else if (x < g_txt_tabpos[2]) {
				x = g_txt_tabpos[2];
			} else if (x < g_txt_tabpos[3]) {
				x = g_txt_tabpos[3];
			} else if (x < g_txt_tabpos[4]) {
				x = g_txt_tabpos[4];
			} else if (x < g_txt_tabpos[5]) {
				x = g_txt_tabpos[5];
			} else if (x < g_txt_tabpos[6]) {
				x = g_txt_tabpos[6];
			}
		} else if ((c == (signed char)0xf0) || (c == (signed char)0xf1) || (c == (signed char)0xf2) || (c == (signed char)0xf3)) {
			g_textcolor_index = (unsigned char)c + 0xff10;
		} else {
			x += GUI_unused2(c, x, y);
		}
	}

	call_mouse();
}

signed int GUI_unused2(const int c, const int x, const int y)
{
	signed int v1;
	const signed int v2 = GUI_lookup_char_height((signed char)c, &v1);

	GUI_1c2(v2, v1, x, y);

	return v1;
}
#endif

signed int GUI_lookup_char_height(const signed char c, signed int *p)
{
	signed int i;

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
void GUI_1c2(const signed int index, const signed int v2, const signed int x, const signed int y)
{

	GUI_blank_char();
	GUI_font_to_buf(g_splash_buffer + index * 8);
	GUI_write_char_to_screen_xy(x, y, 8, v2);
}
#endif

//should be static
signed int GUI_enter_string(char* dst, signed int x, const signed int y, const signed int num, const signed int zero)
{
	signed int x_pos;
	register signed int si;
	signed int pos;
	signed int c;
	char *dst_start = dst;
	signed int length;

	call_mouse_bg();

	x_pos = x;

	pos = 0;

	length = strlen((char*)dst);

	if (zero == 0) {
		for (si = 0; si < num; si++) {
			if ((g_gui_entering_savegame) && (length >= si)) {
				GUI_print_char(' ', x_pos, y);
				GUI_print_char(*dst, x_pos, y);
				pos++;
				dst++;
				x_pos += 6;
				x = x_pos;
			} else {
				GUI_print_char(' ', x_pos, y);
				GUI_print_char('_', x_pos, y);
				x_pos += 6;
			}
		}
		x_pos = x;
	} else {
		GUI_print_char(' ', x_pos, y);
		GUI_print_char('_', x_pos, y);
	}

	flush_keyboard_queue();
	g_mouse_leftclick_event = 0;

	c = 0;
	while ((c != 0xd) || (pos == 0)) {
		do {
			goto dummy;
dummy:

			/* This loop is evil */
			do {;} while ((CD_bioskey(1) == 0) && !g_mouse_leftclick_event);

			if (g_mouse_leftclick_event) {
				g_bioskey_event = 0x0d;
				g_mouse_leftclick_event = g_mouse1_event2 = 0;
			} else {
				g_action = (g_bioskey_event = bioskey(0)) >> 8;
				g_bioskey_event &= 0xff;
			}

		} while ((g_action == 0) && (g_bioskey_event == 0));

		c = g_bioskey_event;

		if (c == 0x0d) {

		} else if (g_action == ACTION_ID_ESC) {
			*dst_start = 0;
			call_mouse();
			g_action = 0;
			return -1;
		} else if (c == 8) {
			if (pos > 0) {

				if ((zero == 1) && (pos != num)) {
					GUI_print_char(0x20, x_pos, y);
				}

				x_pos -= 6;
				pos--;
				dst--;
				GUI_print_char(' ', x_pos, y);
				GUI_print_char('_', x_pos, y);
			}
		} else

		/* check if character is valid */
		if (((c >= 0x20) && (c <= 0x7a)) || (c == 0x81) || (c == 0x84) || (c == 0x94))
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
				x_pos -= 6;
				pos--;
			}

			*dst++ = (unsigned char)c;
			GUI_print_char(' ', x_pos, y);
			GUI_print_char((unsigned char)c, x_pos, y);
			x_pos += 6;
			pos++;

			if ((zero == 1) && (pos != num)) {
				GUI_print_char(' ', x_pos, y);
				GUI_print_char('_', x_pos, y);
			}
		}
	}

	if (zero == 0) {
		while (pos < num) {
			GUI_print_char(' ', x_pos, y);
			x_pos += 6;
			pos++;
		}
	}

	*dst = 0;
	call_mouse();

	return 0;
}

//static
void GUI_draw_radio_bg(const signed int header, const signed int options, const signed int width, const signed int height)
{
	signed int i;

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
	flush_keyboard_queue_alt();
}

void GUI_copy_smth(const signed int width, const signed int height)
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

signed int GUI_input(char *str, const signed int digits)
{
	signed int retval = 0;
	signed int l2;
	signed int fg_bak;
	signed int bg_bak;
	signed int l3;
	signed int l4;
	signed int l5;
	signed int l6;
	signed int usl_bak;
	signed int lines;
	signed int width;

	usl_bak = g_update_statusline;
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

	width = (g_textbox_width * 32) + 32;
	g_textbox_pos_x = ((signed int)(320u - width) >> 1) + g_basepos_x;

	g_textline_posx = g_textbox_pos_x + 5;
	g_textline_maxlen = width - 8;

	lines = GUI_count_lines(str);

	if (digits != 0)
		lines += 2;

	l2 = (lines + 2) * 8;

	g_textbox_pos_y = ((signed int)(200u - l2) >> 1) + g_basepos_y;
	g_textline_posy = g_textbox_pos_y + 7;

	get_textcolor(&fg_bak, &bg_bak);

	call_mouse_bg();

	GUI_draw_radio_bg(lines, 0, width, l2);

	GUI_print_header(str);

	g_mouse_rightclick_event = 0;

	call_mouse();

	if (digits != 0) {
		if (GUI_enter_string(g_text_input_buf, g_textbox_pos_x + ((signed int)(width - digits * 6) >> 1), g_textbox_pos_y + lines * 8 -2, digits, 0) != -1) {
			retval = (signed int)atol(g_text_input_buf);
		} else {
			retval = -1;
		}
	} else {
		/* set action table */
		g_action_table_secondary = &g_action_table_menu[0];

		if (g_bioskey_event10 != 0) {
			wait_for_keypress();
		} else {
			vsync_or_key(lines * g_textbox_width * 50);
		}

		/* delete action table */
		g_action_table_secondary = NULL;
	}

	set_textcolor(fg_bak, bg_bak);

	call_mouse_bg();

	GUI_copy_smth(width, l2);

	call_mouse();

	g_textline_posx = l3;
	g_textline_posy = l4;
	g_textline_maxlen = l5;

	g_action = 0;
	g_dialogbox_lock = 0;

	g_wallclock_update = l6;
	g_gui_text_centered = 0;
	g_update_statusline = usl_bak;

	return retval;
}

/**
 * \brief   shows a text box with two radio buttons "Ja" (yes) and "Nein" (no).
 *
 * \param   text	the displayed text
 */
signed int GUI_bool(char *header)
{
	signed int ret_radio;

	g_gui_bool_flag = 1;

	ret_radio = GUI_radio(header, 2, get_ttx(2), get_ttx(3));

	g_gui_bool_flag = 0;

	return (ret_radio == 1) ? 1 : 0;
}


//static
void GUI_fill_radio_button(const signed int old_pos, const signed int new_pos, const signed int offset)
{
	signed int i;
	signed int x;
	signed int y;

	call_mouse_bg();

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

	call_mouse();
}

signed int GUI_dialogbox(uint8_t* picture, char *name, char *text, signed int options, ...)
{
	va_list arguments;
	signed int i;
	signed int l2;
	signed int l3;
	signed int l4;
	signed int l5;
	signed int tw_bak;
	signed int fg_bak;
	signed int bg_bak;
	signed int x_bak;
	signed int y_bak;
	signed int ml_bak;
	signed int l10;
	signed int retval;
	signed int wc_bak;
	signed int usl_bak;
	const signed int ani_bak = g_ani_enabled;
	signed int lines;
	signed int width;

	usl_bak = g_update_statusline;
	g_update_statusline = 0;

	disable_ani();

	wc_bak = g_wallclock_update;
	g_wallclock_update = 0;
	g_dialogbox_lock = 1;
	x_bak = g_textline_posx;
	y_bak = g_textline_posy;
	ml_bak = g_textline_maxlen;
	tw_bak = g_textbox_width;
	g_textbox_width = 9;

	width = g_textbox_width * 32 + 32;
	g_textbox_pos_x = ((signed int)(320 - width) >> 1) + g_basepos_x;
	g_textline_posx = g_textbox_pos_x + 5;
	g_textline_maxlen = width - 8;
	l10 = g_txt_tabpos[0];
	g_txt_tabpos[0] = (g_textline_posx + g_textline_maxlen - 24);
	g_dialogbox_indent_width = 40;
	g_dialogbox_indent_height = 5;

	lines = GUI_count_lines(text) - 1;

	if (name)
		lines += 2;

	if (lines < g_dialogbox_indent_height)
		lines = g_dialogbox_indent_height - 1;

	l4 = lines + (signed char)options;
	l5 = (l4 + 2) * 8;
	g_textbox_pos_y = (200 - (l5 + 2)) >> 1;
	g_textline_posy = g_textbox_pos_y + 5;

	call_mouse_bg();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(lines, (signed char)options, width, l5);

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
		g_textcolor_index = 0;

		g_textline_posy += 14;
		g_dialogbox_indent_height -= 2;
	}

	if (lines != 0) {
		GUI_print_header(text);
	}

	g_dialogbox_indent_width = g_dialogbox_indent_height = 0;

	if ((signed char)options != 0) {

		l2 = g_textline_posx + 8;
		l3 = g_textbox_pos_y + (lines + 1) * 8;

		va_start(arguments, options);
		for (i = 0; i < (signed char)options; l3 += 8, i++) {
			GUI_print_string((char*)va_arg(arguments, char*), l2, l3);
		}
	}

	va_end(arguments);

	retval = GUI_menu_input((signed char)options, lines + 1, width);

	GUI_copy_smth(width, l5);

	call_mouse();
	set_textcolor(fg_bak, bg_bak);

	g_textline_posx = x_bak;
	g_textline_posy = y_bak;
	g_textline_maxlen = ml_bak;

	g_textbox_width = tw_bak;

	g_txt_tabpos[0] = l10;

	g_wallclock_update = wc_bak;

	g_action = g_dialogbox_lock = 0;

	g_update_statusline = usl_bak;

	if (ani_bak != 0)
		init_ani(2);

	return retval;
}

//static
signed int GUI_menu_input(const signed int positions, const signed int h_lines, const signed int width)
{
	signed int l1;
	signed int l2;
	signed int l3;
	signed int l4;
	signed int l5 = -1;
	signed int height;
	signed int done = 0;
	signed int retval;

	g_menu_selected = g_menu_input_busy = 1;
	g_menu_selected += g_menu_default_select;

	if (positions != 0) {
		height = h_lines * 8;
		l3 = g_mouse_posx;
		l4 = g_mouse_posy;
		g_mouse_posx_bak = g_mouse_posx = g_textbox_pos_x + 90;
		l1 = g_textbox_pos_y + height;

		g_mouse_posy_bak = g_mouse_posy = l2 = l1 + 8 * g_menu_default_select;

		mouse_move_cursor(g_mouse_posx, g_mouse_posy);

		g_mouse_posx_max = g_textbox_pos_x + width - 16;
		g_mouse_posx_min = g_textbox_pos_x;
		g_mouse_posy_min = g_textbox_pos_y + height;

		g_mouse_posy_max = height + g_textbox_pos_y - 1 + positions * 8;
		call_mouse();

		g_mouse1_event2 = g_mouse_leftclick_event = g_mouse_rightclick_event = 0;

		while (!done) {
			g_action_table_secondary = &g_action_table_menu[0];
			handle_input();
			g_action_table_secondary = NULL;

			if (l5 != g_menu_selected) {
				GUI_fill_radio_button(l5, g_menu_selected, h_lines - 1);
				l5 = g_menu_selected;
			}

			if (g_mouse_rightclick_event || g_action == ACTION_ID_ESC || g_action == ACTION_ID_PAGE_DOWN) {
				/* close menu */

				retval = -1;
				done = 1;
				g_mouse_rightclick_event = 0;
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

		call_mouse_bg();

		g_mouse_posx_bak = g_mouse_posx = l3;
		g_mouse_posy_bak = g_mouse_posy = l4;
		g_mouse_posx_max = 319;
		g_mouse_posx_min = 0;
		g_mouse_posy_min = 0;
		g_mouse_posy_max = 199;

		mouse_move_cursor(g_mouse_posx, g_mouse_posy);

	} else {
		do {
			vsync_or_key(10000);

		} while (g_action == 0);

		retval = -1;
	}

	g_menu_default_select = g_menu_input_busy = 0;

	return retval;
}

#if defined(__BORLANDC__)
signed int GUI_radio(char *header, const signed char options, ...)
#else
signed int GUI_radio(char *header, const signed int options, ...)
#endif
{
	signed int i;
	signed int lines_header;

	va_list arguments;
	signed int l3;
	signed int l4;
	signed int lines;
	signed int height;
	signed int fg_bak;
	signed int bg_bak;
	signed int x_bak;
	signed int y_bak;
	signed int ml_bak;
	signed int l10;
	signed int width;
	signed int retval;
	const signed int usl_bak = g_update_statusline;

	g_update_statusline = 0;

	if (!options) {
		GUI_output(header);
		return 0;
	}

	g_dialogbox_lock = 1;
	x_bak = g_textline_posx;
	y_bak = g_textline_posy;
	ml_bak = g_textline_maxlen;

	width = g_textbox_width * 32 + 32;
	g_textbox_pos_x = ((320 - width) >> 1) + g_basepos_x;
	g_textline_posx = g_textbox_pos_x + 5;
	g_textline_maxlen = width - 8;

	l10 = g_txt_tabpos[0];
	g_txt_tabpos[0] = g_textline_posx + g_textline_maxlen - 24;

	lines_header = GUI_count_lines(header);
	lines = lines_header + options;
	height = (lines + 2) * 8;
	g_textbox_pos_y = ((200 - height + 2) >> 1) + g_basepos_y;
	g_textline_posy = g_textbox_pos_y + 7;

	call_mouse_bg();
	get_textcolor(&fg_bak, &bg_bak);

	GUI_draw_radio_bg(lines_header, options, width, height);

	if (lines_header != 0)
		GUI_print_header(header);

	l3 = g_textline_posx + 8;
	l4 = g_textbox_pos_y + (lines_header + 1) * 8;

	va_start(arguments, options);
	for (i = 0; i < options; l4 += 8, i++) {

		/* highlight special option */
		if ((g_game_mode == GAME_MODE_BEGINNER) && (g_talented_hero_pos == i))
			set_textcolor(0xc9, 0xdf);

		GUI_print_string((char*)va_arg(arguments, char*), l3, l4);

		/* reset highlight special option */
		if ((g_game_mode == GAME_MODE_BEGINNER) && (g_talented_hero_pos == i))
			set_textcolor(0xff, 0xdf);
	}

	retval = GUI_menu_input(options, lines_header + 1, width);

	GUI_copy_smth(width, height);
	call_mouse();
	set_textcolor(fg_bak, bg_bak);

	g_textline_posx = x_bak;
	g_textline_posy = y_bak;
	g_textline_maxlen = ml_bak;
	g_txt_tabpos[0] = l10;
	g_action = g_dialogbox_lock = 0;
	g_update_statusline = usl_bak;

	return retval;
}

/**
 * \brief   shows a fight intro message
 *
 * \param   fight_id    ID of the fight
 */
void GUI_print_fight_intro_msg(const signed int fight_id)
{
	const signed int tw_bak = g_textbox_width;
	g_textbox_width = 7;

	if (gs_dungeon_id == DUNGEON_ID_VERFALLENE_HERBERGE) {
		DNG02_fight_intro(fight_id);
	}
	if (gs_dungeon_id == DUNGEON_ID_GOBLINHOEHLE) {
		DNG05_fight_intro(fight_id);
	}
	if (gs_dungeon_id == DUNGEON_ID_DASPOTASCHATZ) {
		DNG06_fight_intro(fight_id);
	}
	if (gs_dungeon_id == DUNGEON_ID_KULTSTAETTE_DES_NAMENLOSEN) {
		DNG09_fight_intro(fight_id);
	}
	if (gs_dungeon_id == DUNGEON_ID_PIRATENHOEHLE) {
		DNG11_fight_intro(fight_id);
	}
	if (gs_dungeon_id == DUNGEON_ID_ZWERGENFESTE) {
		DNG12_fight_intro(fight_id);
	}
	if (gs_dungeon_id == DUNGEON_ID_VERLASSENE_MINE) {
		DNG13_fight_intro(fight_id);
	}
	if (gs_dungeon_id == DUNGEON_ID_ZWINGFESTE) {
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
void GUI_dialog_na(const signed int head_index, char *text)
{

	if (head_index != 0)
		load_in_head(head_index);

	GUI_dialogbox((unsigned char*)g_dtp2, NULL, text, 0);

}
