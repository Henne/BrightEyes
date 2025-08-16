/*
 *	Rewrite of DSA1 v3.02_de functions of seg096 (text output)
 *	Functions rewritten: 23/23 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg096.cpp
 *
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg029.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif


struct dummy {
	signed short a[3];
};

//000
/**
 * \brief   makes a grammatical wordgroup
 *
 * \param   flag        * \param   index        index of the word of which a worgroup should be made
 * \param   type        if index is true the index is an enemy, if not it is an item
 */
Bit8u* GUI_names_grammar(signed short flag, signed short index, signed short type)
{
	signed short *lp1;
	signed short l2 = 0;
	char *p_name;
	signed short l4;
#if !defined(__BORLANDC__)
	struct dummy lp5 = { {0x1000, 0x2000, 0x3000} };
#else
	struct dummy lp5 = *(struct dummy*)(p_datseg + GRAMMAR_GENDER_BITMASKS);
#endif

	if (type == 0) {
		/* string_array_itemnames */
		p_name = get_itemname(index);

		flag += lp5.a[g_items_genders[index]];

		lp1 = &g_items_noplural[0];

		while (((l4 = host_readws((Bit8u*)(lp1++))) != -1) && (l4 != index));

		if (l4 == index) {
			flag += 4;
			flag &= 0x7fff;
			flag |= 0x4000;
			if (flag & 1)
				l2 = 1;
		}
	} else {
		p_name = get_monname(index);
		flag += lp5.a[g_monname_genders[index]];
	}

	lp1 = (flag & 0x8000) ? (signed short*)(p_datseg + GRAMMAR_DEF_TABLE + (flag & 0xf) * 6) :
			((flag & 0x4000) ? (signed short*)(p_datseg + GRAMMAR_NOARTICLE_TABLE) :
				(signed short*)(p_datseg + GRAMMAR_INDEF_TABLE + (flag & 0xf) * 6));


	sprintf(g_grammar_bufs[ds_readw(GRAMMAR_BUF_NO)],
		(l2 == 0 ? (char*)ds_readd(STR_S_S_PTR) : (char*)ds_readd(STR_VON_S_S_PTR)),
		g_grammar_articles_index[host_readws((Bit8u*)lp1 + 2 * (((flag & 0x3000) - 1) >> 12))],
		(char*)GUI_name_plural(flag, p_name));

	p_name = g_grammar_bufs[ds_readw(GRAMMAR_BUF_NO)];

	if (*p_name == 0x20) {
		do {
			l4 = *(++p_name);
			*(p_name - 1) = (signed char)l4;

		} while (l4 != 0);
	}

	l4 = ds_readw(GRAMMAR_BUF_NO);

	if (inc_ds_ws(GRAMMAR_BUF_NO) == 4)
		ds_writew(GRAMMAR_BUF_NO, 0);

	return (Bit8u*)g_grammar_bufs[l4];
}

//1a7
char* GUI_name_plural(signed short v1, char *s)
{
	char *p = g_grammar_bufs[0];
	char tmp;

	while ((tmp = *s++) && (tmp != 0x2e)) {
		*p++ = tmp;
	}

	if (v1 & 4)
		while ((tmp = *s++) && (tmp != 0x2e));

	while ((tmp = *s) && (tmp != 0x2e))
		*p++ = *s++;

	if ((v1 & 0x0f) == 1 && (v1 & 0x3000) != 0x2000) {
		if (*(p-1) == 'B' || *(p-1) == 'D')
			*p++ = 'E';
		*p++ = 'S';
	} else {
		if (((v1 & 0x0f) == 7) && (*(p-1) != 'N') && (*(p-1) != 'S'))
				*p++ = 'N';
	}

	*p = 0;

	return g_grammar_bufs[0];
}

//290
char* GUI_name_singular(char *s)
{
	char *p = g_grammar_bufs[0];
	char tmp;

	while ((tmp = *s++) && (tmp != 0x2e)) {
		*p++ = tmp;
	}

	while ((tmp = *s) && (tmp != 0x2e)) {
		*p++ = *s++;
	}

	*p = 0;
	return g_grammar_bufs[0];
}

//2f2
/**
 * \brief   return a pointer to the pronoun
 */
Bit8u* GUI_2f2(signed short v1, signed short word_id, signed short type)
{
	signed short genus;

	genus = (type == 0) ? g_items_genders[word_id] : g_monname_genders[word_id];

	return (Bit8u*)g_grammar_pronouns_index[g_grammar_pronouns_table2[v1 * 3 + genus]];
}

//330
/**
 * \brief   return a pointer to the personalpronomen
 *
 * \param   genus       gender of the hero
 * \param   causus      the grammatical causus
 */
char* GUI_get_ptr(const int genus, const int causus)
{
	if (genus == 0) {
		return (causus == 0) ? g_grammar_pronouns_er :
			((causus == 1) ? g_grammar_pronouns_sein :
			((causus == 3) ? g_grammar_pronouns_ihm : g_grammar_pronouns_ihn));
	} else {
		return (causus == 0) ? g_grammar_pronouns_sie :
			((causus == 1) ? g_grammar_pronouns_ihr :
			((causus == 3) ? g_grammar_pronouns_ihr : g_grammar_pronouns_sie));
	}
}

#if defined(__BORLANDC__)
//394
/**
 */
char* GUI_get_ptr2(signed short genus, signed short causus)
{
	if (genus == 0) {
		return (causus == 0) ? (g_grammar_article_der) :
			((causus == 1) ? (g_grammar_article_des) :
			((causus == 3) ? (g_grammar_article_dem) : (g_grammar_article_den)));
	} else {
		return (causus == 0) ? (g_grammar_article_die) :
			((causus == 1) ? (g_grammar_article_der) :
			((causus == 3) ? (g_grammar_article_der) : (g_grammar_article_die)));
	}
}
#endif

//3f8
void GUI_write_char_to_screen(Bit8u *dst, signed short char_width, signed short char_height)
{
	Bit8u *ptr;
	signed short y;
	signed short x;

	ptr = &g_gui_text_buffer[0];

	for (y = 0; y < char_width; ptr += 8 - char_height, dst += 320, y++) {
		for (x = 0; x < char_height; x++)
			*(dst + x) = *ptr++;
	}
}

/**
 * \brief   return the number of lines the input string needs on a screen
 *
 * \param   str		the input string
 * \return		the number of lines on the screen
 *
 * \warning The input string str may be modified if it does not
 *          fit on the screen. So EVERY string in this sourcecode,
 *	    which should be printed on screen must not be constant.
 *	    Otherwise this application may crash.
 */
unsigned short GUI_count_lines(char *str)
{
	signed short current_pos;
	signed short last_ws;

	char *str_loc;
	signed short always_zero;
	signed short lines;
	signed short max_line_width;
	signed short width_char;
	signed short width_line;

	lines = 0;

	if (!str) return 0;

	/* replace all CR and LF in the input string with whitespaces */
	for (str_loc = str; *str_loc; str_loc++)
		if (*str_loc == 0x0d || *str_loc == 0x0a)
			*str_loc = 0x20;

	str_loc = str;
	current_pos = last_ws = always_zero = 0;
	max_line_width = g_textline_maxlen;

	if (g_dialogbox_indent_width)
		g_textline_maxlen -= g_dialogbox_indent_width;

	width_line = 0;

	for (; str_loc[current_pos]; current_pos++) {

		GUI_lookup_char_width(str_loc[current_pos], &width_char);
#if !defined(__BORLANDC__)
		/* BE-fix */
		width_char = host_readws((Bit8u*)&width_char);
#endif
		width_line += width_char;

		/* check if the input string str is to long for the line */
		if (width_line >=  g_textline_maxlen) {
			if (last_ws != always_zero) {
				/* 	if a whitespace existed on the current line,
					do a linebreak there */
				str_loc[last_ws] = 0x0d;
				str_loc = str_loc + last_ws;
			} else {
				/* 	if no whitespace existed on the current line,
					break at current pos and overwrite str_loc[current_pos] with CR */
				str_loc[current_pos] = 0x0d;
				str_loc = &str_loc[current_pos + 1];
			}

			if (++lines == g_dialogbox_indent_height)
				g_textline_maxlen += g_dialogbox_indent_width;

			/* reset variables */
			always_zero = current_pos = last_ws = width_line = 0;
		}

		/* remember the position of the last whitespace for linebreaks */
		if (str_loc[current_pos] == 0x20)
			last_ws = current_pos;

		/* a forced break in the text with '@' */
		if (str_loc[current_pos] == 0x40) {
			str_loc = &str_loc[current_pos + 1];
			current_pos = -1;
			always_zero = last_ws = width_line = 0;
			if (++lines == g_dialogbox_indent_height)
				g_textline_maxlen += g_dialogbox_indent_width;
		}
	}

	if (width_line >= g_textline_maxlen) {

		if (always_zero == last_ws)
			str_loc[current_pos - 1] = 0;
		else {
			str_loc[last_ws] = 0x0d;
			if (++lines == g_dialogbox_indent_height)
				g_textline_maxlen += g_dialogbox_indent_width;
		}
	}

	g_textline_maxlen = (max_line_width);
	return ++lines;
}

//5d7
signed short GUI_print_header(char *str)
{
	signed short retval = 1;

	update_mouse_cursor();
	retval = GUI_count_lines(str);
	GUI_print_string(str, g_textline_posx, g_textline_posy);
	refresh_screen_size();

	return retval;
}

//614
void GUI_print_loc_line(char *str)
{
	signed short tmp1;
	signed short tmp2;
	signed short l1;
	signed short l2;
	signed short l3;

	get_textcolor(&tmp1, &tmp2);
	set_textcolor(0xff, 0);

	l1 = g_textline_posx;
	l2 = g_textline_posy;
	l3 = g_textline_maxlen;

	g_textline_posx = (6);
	g_textline_posy = (143);
	g_textline_maxlen = (307);

	clear_loc_line();
	GUI_print_header(str);

	g_textline_posx = (l1);
	g_textline_posy = (l2);
	g_textline_maxlen = (l3);

	set_textcolor(tmp1, tmp2);
}

//691
void GUI_print_string(char *str, signed short x, signed short y)
{
	signed short l1;
	signed short l2;
	signed short l3;
	unsigned char l4;

	l1 = 0;
	l2 = 0;

	update_mouse_cursor();

	if (g_gui_text_centered == 1) {
		x = GUI_get_first_pos_centered(str, x, g_textline_maxlen, 0);
	} else
		if (g_dialogbox_indent_width)
			x += g_dialogbox_indent_width;
	l3 = x;

	while ((l4 = str[l2++])) {

		/* handle line breaks */
		if ((l4 == 0x0d) || (l4 == 0x40)) {

			if (++l1 == g_dialogbox_indent_height) {
				g_textline_maxlen += g_dialogbox_indent_width;
				l3 -= g_dialogbox_indent_width;
			}

			y += 7;
			x = (g_gui_text_centered == 1) ?
				GUI_get_first_pos_centered(str + l2, g_textline_posx, g_textline_maxlen, 0) : l3;

		} else	if (l4 == '~') {

			if (x < ds_readws(TXT_TABPOS1))
				x = ds_readws(TXT_TABPOS1);
			else if (x < ds_readws(TXT_TABPOS2))
				x = ds_readws(TXT_TABPOS2);
			else if (x < ds_readws(TXT_TABPOS3))
				x = ds_readws(TXT_TABPOS3);
			else if (x < ds_readws(TXT_TABPOS4))
				x = ds_readws(TXT_TABPOS4);
			else if (x < ds_readws(TXT_TABPOS5))
				x = ds_readws(TXT_TABPOS5);
			else if (x < ds_readws(TXT_TABPOS6))
				x = ds_readws(TXT_TABPOS6);
			else if (x < ds_readws(TXT_TABPOS7))
				x = ds_readws(TXT_TABPOS7);

		} else if (l4 == (unsigned char)0xf0 ||
				l4 == (unsigned char)0xf1 ||
				l4 == (unsigned char)0xf2 ||
				l4 == (unsigned char)0xf3)
		{
			/* changes of the text color are only control bytes */
			g_textcolor_index = l4 - 0xf0;
		} else	{

			if (l4 == 0x3c) {
				l4 = 0x3e;
			} else { }

			x += GUI_print_char(l4, x, y);
		}
	}

	refresh_screen_size();
}

//7f0
signed short GUI_print_char(unsigned char c, unsigned short x, unsigned short y)
{
	signed short char_width;
	signed short font_index;

	g_gui_print_char = c;

	font_index = GUI_lookup_char_width(c, &char_width);

	GUI_write_fonti_to_screen(font_index, char_width, x, y);

	return char_width;
}

//82b
signed short GUI_lookup_char_width(signed char c, signed short *p)
{
	signed short i;

	for (i = 0; i != 75 * 3; i += 3) {

		if (c == ds_readbs(GUI_CHAR_WIDTH + i)) {

			host_writew((Bit8u*)p,
				ds_readbs(GUI_CHAR_WIDTH + i + 2) & 0xff);

			return ds_readbs(GUI_CHAR_WIDTH + i + 1) & 0xff;
		}
	}

	if ((c == (signed char)'~')
		|| (c == (signed char)0xf0)
		|| (c == (signed char)0xf1)
		|| (c == (signed char)0xf2)
		|| (c == (signed char)0xf3))
	{
		return host_writews((Bit8u*)p, 0);
	}

	host_writew((Bit8u*)p, 5);
	return 0;
}

//88f
void GUI_write_fonti_to_screen(unsigned short font_index, unsigned short char_width, unsigned short x, unsigned short y)
{
	GUI_blank_char();
	GUI_font_to_buf(g_buf_font6 + font_index * 8);
	GUI_write_char_to_screen_xy(x, y, 7, char_width);
}
//8c5
/**
 * \brief   sets the area of a char to a color
 */
void GUI_blank_char(void)
{
	Bit8u *ptr = &g_gui_text_buffer[0];
	signed short i;
	signed short j;

	for (i = 0; i < 8; ptr += 8, i++) {
		for (j = 0; j < 8; j++)
			*(ptr + j) = g_textcolor_bg;
	}
}

//8f8
void GUI_font_to_buf(Bit8u *fc)
{
	Bit8u *p;
	char c;
	short i;
	short j;

	/* current text position */
	p = &g_gui_text_buffer[0];

	if (g_gui_print_char == 0x3a)
		fc++;

	for (i = 0; i < 8; p += 8, i++) {
		c = *fc++;
		for (j = 0; j < 8; j++)
			if ((0x80 >> j) & c)
				 p[j] = g_textcolor_fg[g_textcolor_index];
	}
}

//956
void GUI_write_char_to_screen_xy(unsigned short x, unsigned short y, unsigned short char_height, unsigned short char_width)
{
	/* screen_start */
	Bit8u* dst = g_vga_backbuffer + y * 320 + x;

	GUI_write_char_to_screen(dst, char_height, char_width);
}

/**
 * \brief   sets the textcolor
 *
 * \param   fg          foreground color index
 * \param   bg          background color index
 */
void set_textcolor(signed short fg, signed short bg)
{
	g_textcolor_fg[0] = fg;
	g_textcolor_bg = bg;
}

/**
 * \brief   gets the textcolor
 *
 * \param   fg          foreground color index
 * \param   bg          background color index
 */
void get_textcolor(signed short *fg, signed short *bg)
{
	host_writew((Bit8u*)fg, g_textcolor_fg[0]);
	host_writew((Bit8u*)bg, g_textcolor_bg);
}

unsigned short GUI_unused(Bit8u *str)
{
	unsigned short lines = 0;

	while (*str) {
		if (*str++ == 0x0d)
			lines++;
	}

	return lines;
}

//9D6
signed short GUI_get_space_for_string(char *p, signed short dir)
{
	signed short sum;
	signed short tmp;

	for (sum = 0; *p; sum += tmp) {
		if (dir) {
			GUI_lookup_char_height(*p++, &tmp);
		} else {
			GUI_lookup_char_width(*p++, &tmp);
		}
#if !defined(__BORLANDC__)
		/* BE-fix */
		tmp = host_readws((Bit8u*)&tmp);
#endif
	}

	return sum;
}

//A26
signed short GUI_get_first_pos_centered(char *p, signed short x, signed short v2, unsigned short dir)
{
	register signed short i;
	register signed short c;
	signed short tmp;

	for (i = 0;  (c = *p) && (c != 0x40) && (c != 0x0d); i += tmp) {
		if (dir)
			GUI_lookup_char_height(*p++, &tmp);
		else
			GUI_lookup_char_width(*p++, &tmp);

#if !defined(__BORLANDC__)
		/* BE-fix */
		tmp = host_readws((Bit8u*)&tmp);
#endif
	}

	v2 -= i;
	v2 >>= 1;
	v2 += x;

	return v2;
}

//A93
/**
 * \brief   draws a line of a popup window
 *
 * \param   line        number of the current line
 * \param   type        type of line 0 = top / 1 = middle normal / 2 = middle radio / 3 =bottom
 */
void GUI_draw_popup_line(signed short line, signed short type)
{
	short popup_left;
	short i;
	short popup_middle;
	short popup_right;
	short y;
	short x;

	/* set the offsets in the popup.dat buffer */
	switch (type) {
		case 0:
			popup_left = 0;
			popup_middle = 0x380;
			popup_right = 0x80;
			break;
		case 1:
			popup_left = 0x100;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		case 2:
			popup_left = 0x200;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		case 3:
			popup_left = 0x280;
			popup_middle = 0x580;
			popup_right = 0x300;
			break;
	}

	x = g_textbox_pos_x;
	y = g_textbox_pos_y + (line * 8);
	g_pic_copy.x1 = x;
	g_pic_copy.y1 = y;
	g_pic_copy.x2 = x + 15;
	g_pic_copy.y2 = y + 7;
	g_pic_copy.src = g_popup + popup_left;

	do_pic_copy(0);

	g_pic_copy.src = g_popup + popup_middle;

	x += 16;

	for (i = 0; i < g_textbox_width; i++) {
		g_pic_copy.x1 = x;
		g_pic_copy.x2 = x + 31;
		do_pic_copy(0);
		x += 32;
	}

	g_pic_copy.src = g_popup + popup_right;
	g_pic_copy.x1 = x;
	g_pic_copy.x2 = x + 15;

	do_pic_copy(0);
}

#if !defined(__BORLANDC__)
}
#endif
