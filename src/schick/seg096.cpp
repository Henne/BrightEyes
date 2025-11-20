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

static char g_grammar_article_der[4] = "DER"; // ds:0xa8d4
static char g_grammar_article_die[4] = "DIE"; // ds:0xa8d8
static char g_grammar_article_das[4] = "DAS"; // ds:0xa8dc
static char g_grammar_article_des[4] = "DES"; // ds:0xa8e0
static char g_grammar_article_den[4] = "DEN"; // ds:0xa8e4
static char g_grammar_article_dem[4] = "DEM"; // ds:0xa8e8
static char g_grammar_article_ein[4] = "EIN"; // ds:0xa8ec
static char g_grammar_article_eine[5] = "EINE"; // ds:0xa8f0
static char g_grammar_article_eines[6] = "EINES"; // ds:0xa8f5
static char g_grammar_article_einer[6] = "EINER"; // ds:0xa8fb
static char g_grammar_article_einen[6] = "EINEN"; // ds:0xa901
static char g_grammar_article_eine2[5] = "EINE"; // ds:0xa907
static char g_grammar_article_einem[6] = "EINEM"; // ds:0xa90c
static char g_grammar_article_0[1] = ""; // ds:0xa912
static char g_grammar_article_von[4] = "VON"; // ds:0xa913
static char* g_grammar_articles_index[15] = {
	g_grammar_article_der,
	g_grammar_article_die,
	g_grammar_article_das,
	g_grammar_article_des,
	g_grammar_article_den,
	g_grammar_article_dem,
	g_grammar_article_ein,
	g_grammar_article_eine,
	g_grammar_article_eines,
	g_grammar_article_einer,
	g_grammar_article_einen,
	g_grammar_article_eine2,
	g_grammar_article_einem,
	g_grammar_article_0,
	g_grammar_article_von
}; // ds:0xa917; uint8_t*
static signed int g_grammar_def_table[24] = {
	0, 1, 2, 3, 0, 3,
	4, 1, 2, 5, 0, 5,
	1, 1, 1, 0, 0, 0,
	1, 1, 1, 4, 4, 4
}; // ds:0xa953, [8], by case, then by gender
static signed int g_grammar_indef_table[24] = {
	6, 7, 6, 8, 9, 8,
	10, 11, 6, 12, 9, 12,
	13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13
}; // ds:0xa983, [8], by case, then by gender
static signed int g_grammar_noarticle_table[24] = {
	0x000d, 0x000d, 0x000d, 0x000d, 0x000d, 0x000d,
	0x000d, 0x000d, 0x000d, 0x000d, 0x000d, 0x000d,
	0x000d, 0x000d, 0x000d, 0x000d, 0x000d, 0x000d,
	0x000d, 0x000d, 0x000d, 0x000d, 0x000d, 0x000d
}; // ds:0xa9b3, [8], by case, then by gender

extern char g_str_s_s[6];
extern char g_str_von_s_s[9];

static char *g_str_s_s_ptr = (char*)&g_str_s_s; // ds:0xa9e3, to STR_S_S; uint8_t*
static char *g_str_von_s_s_ptr = (char*)&g_str_von_s_s; // ds:0xa9e7, to STR_VON_S_S; uint8_t*
static signed int g_grammar_buf_no = 0; // ds:0xa9eb
static struct int16_t_3 g_grammar_gender_bitmasks = { 0x1000, 0x2000, 0x3000 }; // ds:0xa9ed, {0x1000, 0x2000, 0x3000}
static char g_grammar_pronouns_er[3] = "ER"; // ds:0xa9f3
static char g_grammar_pronouns_sie[4] = "SIE"; // ds:0xa9f6
static char g_grammar_pronouns_es[3] = "ES"; // ds:0xa9fa
static char g_grammar_pronouns_sein[5] = "SEIN"; // ds:0xa9fd
static char g_grammar_pronouns_ihr[4] = "IHR"; // ds:0xaa02
static char g_grammar_pronouns_ihn[4] = "IHN"; // ds:0xaa06
static char g_grammar_pronouns_ihm[4] = "IHM"; // ds:0xaa0a
static char g_grammar_pronouns_ihnen[6] = "IHNEN"; // ds:0xaa0e
static char *g_grammar_pronouns_index[7] = {
	g_grammar_pronouns_er,
	g_grammar_pronouns_sie,
	g_grammar_pronouns_es,
	g_grammar_pronouns_sein,
	g_grammar_pronouns_ihr,
	g_grammar_pronouns_ihn,
	g_grammar_pronouns_ihm
}; // ds:0xaa14; uint8_t*
static const signed char g_grammar_pronouns_table2[33] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x03, 0x05, 0x01, 0x02, 0x06, 0x04, 0x06, 0x01, 0x01, 0x01, 0x04, 0x04, 0x04, 0x01, 0x01, 0x01, 0x07, 0x07, 0x07, 0x00, -128, -64, -32, -16, -8, -4, -2, -1 }; // ds:0xaa30, by gender and ??

static const struct struct_char_width g_gui_char_width[75] = {
	{        ' ',  0, 6 },
	{        'A',  1, 6 },
	{        'B',  2, 6 },
	{        'C',  3, 6 },
	{        'D',  4, 6 },
	{        'E',  5, 6 },
	{        'F',  6, 6 },
	{        'G',  7, 6 },
	{        'H',  8, 6 },
	{        'I',  9, 4 },
	{        'J', 10, 6 },
	{        'K', 11, 6 },
	{        'L', 12, 5 },
	{        'M', 13, 6 },
	{        'N', 14, 6 },
	{        'O', 15, 6 },
	{        'P', 16, 6 },
	{        'Q', 17, 6 },
	{        'R', 18, 6 },
	{        'S', 19, 6 },
	{        'T', 20, 6 },
	{        'U', 21, 6 },
	{        'V', 22, 6 },
	{        'W', 23, 6 },
	{        'X', 24, 6 },
	{        'Y', 25, 6 },
	{        'Z', 26, 6 },
	{ (char)0x8e, 27, 6 },
	{ (char)0x99, 28, 6 },
	{ (char)0x9a, 29, 6 },
	{        ',', 30, 3 },
	{        '.', 31, 2 },
	{        '-', 32, 6 },
	{        '(', 33, 4 },
	{        ')', 34, 4 },
	{        '!', 35, 3 },
	{        '?', 36, 5 },
	{        '0', 37, 6 },
	{        '1', 38, 3 },
	{        '2', 39, 6 },
	{        '3', 40, 6 },
	{        '4', 41, 6 },
	{        '5', 42, 6 },
	{        '6', 43, 6 },
	{        '7', 44, 6 },
	{        '8', 45, 6 },
	{        '9', 46, 6 },
	{       '\'', 47, 3 },
	{        ':', 48, 3 }, /* NOT IN HEIGHT TABLE */
	{        '#', 49, 6 },
	{        '&', 50, 5 },
	{        '%', 51, 5 },
	{        '<', 53, 5 },
	{        '>', 52, 5 },
	{        '/', 54, 6 },
	{        '_', 55, 6 }, /* NOT IN HEIGHT TABLE */
	{ (char)0xc8, 56, 6 },
	{ (char)0xc9, 57, 6 },
	{ (char)0xca, 58, 6 },
	{ (char)0xcb, 59, 5 },
	{ (char)0xcc, 60, 5 },
	{ (char)0xcd, 61, 4 },
	{ (char)0xce, 62, 5 },
	{ (char)0xcf, 63, 5 },
	{ (char)0xd0, 64, 2 },
	{ (char)0xd1, 65, 6 },
	{ (char)0xd2, 66, 6 },
	{ (char)0xd3, 67, 5 },
	{ (char)0xd4, 68, 6 },
	{ (char)0xd5, 69, 5 },
	{ (char)0xd6, 70, 6 },
	{ (char)0xd7, 71, 5 },
	{ (char)0xd8, 72, 6 },
	{ (char)0xd9, 73, 5 },
	{        '+',  0, 6 } /* NOT IN HEIGHT TABLE */
}; // ds:0xaa51
char g_str_s_s[6] = "%s %s"; // ds:0xab32
char g_str_von_s_s[9] = "VON%s %s"; // ds:0xab38


static char g_gui_print_char;		// ds:0xe4d8
signed int g_dialogbox_indent_height;	// ds:0xe4d9
signed int g_dialogbox_indent_width;	// ds:0xe4db
unsigned char g_unkn_092[6];		// ds:0xe4dd
static char g_grammar_bufs[5][40];	// ds:0xe4e3


//000
/**
 * \brief   makes a grammatical wordgroup
 *
 * \param   flag
 * \param   index	index of the word of which a worgroup should be made
 * \param   type	if index is true the index is an enemy, if not it is an item
 */
uint8_t* GUI_names_grammar(int16_t flag, const signed int index, const signed int type)
{
	signed int *lp1;
	signed int bit_set = 0;
	char *p_name;
	signed int tmp;
	struct int16_t_3 lp5 = g_grammar_gender_bitmasks;
	//int16_t lp5[3] = { {0x1000, 0x2000, 0x3000} };

	if (type == 0) {
		/* string_array_itemnames */
		p_name = g_itemsname[index];

		flag += lp5.a[g_item_name_genders[index]];

		lp1 = &g_items_noplural[0];

		while (((tmp = *lp1++) != -1) && (tmp != index));

		if (tmp == index) {
			flag += 4;
			flag &= 0x7fff;
			flag |= 0x4000;
			if (flag & 1)
				bit_set = 1;
		}
	} else {
		p_name = g_monnames_index[index];
		flag += lp5.a[g_monster_name_genders[index]];
	}

	lp1 = (flag & 0x8000) ? &g_grammar_def_table[(flag & 0xf) * 3] :
		((flag & 0x4000) ? &g_grammar_noarticle_table[0] : &g_grammar_indef_table[(flag & 0xf) * 3]);


	sprintf(g_grammar_bufs[g_grammar_buf_no + 1], (bit_set == 0 ? g_str_s_s_ptr : g_str_von_s_s_ptr),
		g_grammar_articles_index[lp1[((flag & 0x3000) - 1) >> 12]], GUI_name_plural(flag, p_name));

	p_name = g_grammar_bufs[g_grammar_buf_no + 1];

	if (*p_name == 0x20) {
		do {
			tmp = *(++p_name);
			*(p_name - 1) = tmp;

		} while (tmp != 0);
	}

	tmp = g_grammar_buf_no;

	if (++g_grammar_buf_no == 4)
		g_grammar_buf_no = 0;

	return (uint8_t*)g_grammar_bufs[1 + tmp];
}

//1a7
char* GUI_name_plural(const signed int v1, char *s)
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
uint8_t* GUI_2f2(const signed int v1, const signed int word_id, const signed int type)
{
	signed int genus = (type == 0 ? g_item_name_genders[word_id] : g_monster_name_genders[word_id]);

	return (uint8_t*)g_grammar_pronouns_index[g_grammar_pronouns_table2[v1 * 3 + genus]];
}

//330
/**
 * \brief   return a pointer to the personalpronomen
 *
 * \param   genus       gender of the hero
 * \param   causus      the grammatical causus
 */
char* GUI_get_ptr(const signed int genus, const signed int causus)
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

//394
/**
 */
char* GUI_get_article(const signed int genus, const signed int causus)
{
	if (genus == 0) {
		return (causus == 0) ? g_grammar_article_der :
			((causus == 1) ? g_grammar_article_des :
			((causus == 3) ? g_grammar_article_dem : g_grammar_article_den));
	} else {
		return (causus == 0) ? g_grammar_article_die :
			((causus == 1) ? g_grammar_article_der :
			((causus == 3) ? g_grammar_article_der : g_grammar_article_die));
	}
}

//3f8
void GUI_write_char_to_screen(uint8_t *dst, const signed int char_width, const signed int char_height)
{
	uint8_t *ptr;
	signed int y;
	signed int x;

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
signed int GUI_count_lines(char *str)
{
	signed int current_pos;
	signed int last_ws;
	char *str_loc;
	signed int always_zero;
	signed int lines = 0;
	signed int max_line_width;
	signed int width_char;
	signed int width_line;

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

	g_textline_maxlen = max_line_width;
	return ++lines;
}

//5d7
signed int GUI_print_header(char *str)
{
	signed int retval = 1;

	call_mouse_bg();
	retval = GUI_count_lines(str);
	GUI_print_string(str, g_textline_posx, g_textline_posy);
	call_mouse();

	return retval;
}

//614
void GUI_print_loc_line(char *str)
{
	signed int fg_bak;
	signed int bg_bak;
	signed int tlx_bak;
	signed int tly_bak;
	signed int ml_bak;

	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	tlx_bak = g_textline_posx;
	tly_bak = g_textline_posy;
	ml_bak = g_textline_maxlen;

	g_textline_posx = 6;
	g_textline_posy = 143;
	g_textline_maxlen = 307;

	clear_loc_line();
	GUI_print_header(str);

	g_textline_posx = tlx_bak;
	g_textline_posy = tly_bak;
	g_textline_maxlen = ml_bak;

	set_textcolor(fg_bak, bg_bak);
}

//691
void GUI_print_string(char *str, signed int x, signed int y)
{
	signed int l1 = 0;
	signed int i = 0;
	signed int x_bak;
	unsigned char c;

	call_mouse_bg();

	if (g_gui_text_centered == 1) {
		x = GUI_get_first_pos_centered(str, x, g_textline_maxlen, 0);
	} else
		if (g_dialogbox_indent_width)
			x += g_dialogbox_indent_width;
	x_bak = x;

	while ((c = str[i++])) {

		/* handle line breaks */
		if ((c == 0x0d) || (c == '@')) {

			if (++l1 == g_dialogbox_indent_height) {
				g_textline_maxlen += g_dialogbox_indent_width;
				x_bak -= g_dialogbox_indent_width;
			}

			y += 7;
			x = (g_gui_text_centered == 1) ?
				GUI_get_first_pos_centered(str + i, g_textline_posx, g_textline_maxlen, 0) : x_bak;

		} else	if (c == '~') {

			if (x < g_txt_tabpos[0])
				x = g_txt_tabpos[0];
			else if (x < g_txt_tabpos[1])
				x = g_txt_tabpos[1];
			else if (x < g_txt_tabpos[2])
				x = g_txt_tabpos[2];
			else if (x < g_txt_tabpos[3])
				x = g_txt_tabpos[3];
			else if (x < g_txt_tabpos[4])
				x = g_txt_tabpos[4];
			else if (x < g_txt_tabpos[5])
				x = g_txt_tabpos[5];
			else if (x < g_txt_tabpos[6])
				x = g_txt_tabpos[6];

		} else if (c == (unsigned char)0xf0 ||	c == (unsigned char)0xf1 || c == (unsigned char)0xf2 ||	c == (unsigned char)0xf3)
		{
			/* changes of the text color are only control bytes */
			g_textcolor_index = c - 0xf0;
		} else	{

			if (c == '<') {
				c = '>';
			} else { }

			x += GUI_print_char(c, x, y);
		}
	}

	call_mouse();

#if !defined(__BORLANDC__)
	sdl_forced_update();
#endif
}

//7f0
signed int GUI_print_char(const char c, const signed int x, const signed int y)
{
	signed int char_width;
	signed int font_index;

	g_gui_print_char = c;

	font_index = GUI_lookup_char_width(c, &char_width);

	GUI_write_fonti_to_screen(font_index, char_width, x, y);

	return char_width;
}

//82b
signed int GUI_lookup_char_width(const signed char c, signed int *p)
{
	signed int i;

	/* REMARK: This array of struct should be seen as signed char[75*3] */
	for (i = 0; i != 75 * 3; i += 3) {

		if (c == ((signed char*)g_gui_char_width)[i]) {

			*p = ((signed char*)g_gui_char_width)[i + 2] & 0xff;

			return ((signed char*)g_gui_char_width)[i + 1] & 0xff;
		}
	}

	if ((c == '~') || (c == (signed char)0xf0) || (c == (signed char)0xf1) || (c == (signed char)0xf2) || (c == (signed char)0xf3))
	{
		return *p = 0;
	}

	*p = 5;
	return 0;
}

//88f
void GUI_write_fonti_to_screen(const signed int font_index, const signed int char_width, const signed int x, const signed int y)
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
	uint8_t *ptr = &g_gui_text_buffer[0];
	signed int i;
	signed int j;

	for (i = 0; i < 8; ptr += 8, i++) {
		for (j = 0; j < 8; j++)
			*(ptr + j) = g_textcolor_bg;
	}
}

//8f8
void GUI_font_to_buf(uint8_t *fc)
{
	uint8_t *p;
	char c;
	signed int i;
	signed int j;

	/* current text position */
	p = &g_gui_text_buffer[0];

	if (g_gui_print_char == 0x3a)
		fc++;

	for (i = 0; i < 8; p += 8, i++) {

		c = *fc++;

		for (j = 0; j < 8; j++) {
			if ((0x80 >> j) & c)
				 p[j] = g_textcolor_fg[g_textcolor_index];
		}
	}
}

//956
void GUI_write_char_to_screen_xy(const signed int x, const signed int y, const signed int char_height, const signed int char_width)
{
	/* screen_start */
	uint8_t* dst = g_vga_backbuffer + y * 320 + x;

	GUI_write_char_to_screen(dst, char_height, char_width);
}

/**
 * \brief   sets the textcolor
 *
 * \param   fg          foreground color index
 * \param   bg          background color index
 */
void set_textcolor(const signed int fg, const signed int bg)
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
void get_textcolor(signed int *fg, signed int *bg)
{
	*fg = g_textcolor_fg[0];
	*bg = g_textcolor_bg;
}

signed int GUI_unused(char *str)
{
	signed int lines = 0;

	while (*str) {

		if (*str++ == 0x0d) {
			lines++;
		}
	}

	return lines;
}

//9D6
signed int GUI_get_space_for_string(char *p, const signed int dir)
{
	signed int sum;
	signed int tmp;

	for (sum = 0; *p; sum += tmp) {
		if (dir) {
			GUI_lookup_char_height(*p++, &tmp);
		} else {
			GUI_lookup_char_width(*p++, &tmp);
		}
	}

	return sum;
}

//A26
signed int GUI_get_first_pos_centered(char *p, const signed int x, signed int v2, const signed int dir)
{
	register signed int i;
	register signed int c;
	signed int tmp;

	for (i = 0; (c = *p) && (c != '@') && (c != 0x0d); i += tmp) {
		if (dir)
			GUI_lookup_char_height(*p++, &tmp);
		else
			GUI_lookup_char_width(*p++, &tmp);

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
void GUI_draw_popup_line(const signed int line, const signed int type)
{
	signed int popup_left;
	signed int i;
	signed int popup_middle;
	signed int popup_right;
	signed int y;
	signed int x;

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
