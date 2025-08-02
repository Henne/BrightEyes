/*
 *	Rewrite of DSA1 v3.02_de functions of seg024 (diary)
 *	Functions rewritten: 3/3 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg024.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg024.h"
#include "seg027.h"
#include "seg096.h"

#if !defined (__BORLANDC__)
namespace M302de {
#endif

#if 0
/* That comes in a later phase of development */

/* DIARY_STRING1-7 */
static const char diary_fmt[][30] = {
	/* V3.02de */
	"%2d.~%-8s~%s.",
	"%2d.~%-8s~IN %s.",
	"%2d.~%-8s~BESUCHEN VON %s.",
	"~~%s.",
	"~~IN %s.",
	"~~ABENTEUER IN %s.",
	"~~RUMH\x8eNGEN IN %s."
};
#endif

void diary_show(void)
{
	signed short fg_bak;
	signed short bg_bak;
	Bit16u bak1, bak2, txt_tabpos1_bak, txt_tabpos2_bak;
	signed short tw_bak;
	signed short i;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;

	g_special_screen = 1;
	g_wallclock_update = 0;
	ds_writew(AREA_PREPARED, 0xffff);
	ds_writed(CURRENT_CURSOR, (Bit32u)(p_datseg + DEFAULT_MOUSE_CURSOR));

	load_pp20(ARCHIVE_FILE_BUCH_DAT);
	ds_writeb(PP20_INDEX, ARCHIVE_FILE_BUCH_DAT);

	get_textcolor(&fg_bak, &bg_bak);

	g_vga_backbuffer = g_buffer9_ptr;
	bak1 = ds_readw(TEXTLINE_MAXLEN);
	bak2 = ds_readw(TEXTLINE_POSX);
	txt_tabpos1_bak = ds_readw(TXT_TABPOS1);
	txt_tabpos2_bak = ds_readw(TXT_TABPOS2);
	ds_writew(TEXTLINE_MAXLEN, 200);
	ds_writew(TEXTLINE_POSX, 65);
	ds_writew(TXT_TABPOS1, 83);
	ds_writew(TXT_TABPOS2, 130);

	set_textcolor(4, 0);

	/* print all diary entries */
	i = 0;
	do {
		i = diary_print_entry(i);
	} while (i < ds_readws(DIARY_ENTRY_COUNTER));

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 199;
	g_pic_copy.src = g_renderbuf_ptr;
	g_pic_copy.dst = g_vga_memstart;

	update_mouse_cursor();

	set_palette(g_renderbuf_ptr + 0xfa02, 0, 0x20);

	do_pic_copy(0);

	refresh_screen_size();

	set_textcolor(fg_bak, bg_bak);

	g_pic_copy.dst = ((g_vga_backbuffer = g_vga_memstart));

	ds_writew(TEXTLINE_POSX, bak2);
	ds_writew(TEXTLINE_MAXLEN, bak1);
	ds_writew(TXT_TABPOS1, txt_tabpos1_bak);
	ds_writew(TXT_TABPOS2, txt_tabpos2_bak);
	g_textbox_width = tw_bak;

	delay_or_keypress(5000);
}

void diary_new_entry(void)
{
	Bit8u *ptr;

	/* move all entries if the list is full */
	if (ds_readw(DIARY_ENTRY_COUNTER) == 23) {
		memcpy(p_datseg + DIARY_ENTRIES, p_datseg + DIARY_ENTRIES+8, (22*8));
		dec_ds_ws(DIARY_ENTRY_COUNTER);
	}

	/* make a pointer to the last entry */
	ptr = p_datseg + (DIARY_ENTRIES-8) + ds_readw(DIARY_ENTRY_COUNTER) * 8;

	/* avoid double entries for the same town */
	if (gs_current_town != host_readw(ptr + 6)) {

		/* make a pointer to the current entry */
		ptr = p_datseg + DIARY_ENTRIES + ds_readw(DIARY_ENTRY_COUNTER) * 8;

		/* deccrement entry counter */
		inc_ds_ws(DIARY_ENTRY_COUNTER);

		/* Write day of month */
		host_writew(ptr, gs_day_of_month);
		/* Write month */
		host_writew(ptr + 2, gs_month);
		/* Write year */
		host_writew(ptr + 4, gs_year);
		/* Write city */
		host_writew(ptr + 6, gs_current_town);
	}
}

/**
 * \brief   print a diary entry
 *
 * \param   line        the line to start the entry
 */
Bit16u diary_print_entry(Bit16u line)
{
	signed short day;
	signed short month;
	signed short startline;
	Bit8u *ptr;
	char *city_name;
	signed short di = 0;

	memset((Bit8u*)g_buffer9_ptr, 0, 64000);

	ptr = p_datseg + DIARY_ENTRIES + line * 8;

	startline = line;

	do {
		day = host_readw(ptr);
		month = host_readw(ptr + 2);
		city_name = get_ttx(host_readw(ptr + 6) + 0xeb);

		if (di == 0) {
			if ((signed short)strlen(city_name) > 24) {

				sprintf((char*)g_dtp2, (char*)(p_datseg + DIARY_STRING1),
					host_readw(ptr), get_ttx(host_readw(ptr + 2) + 0x15), city_name);

			} else if ((signed short)strlen(city_name) > 15 ) {

				sprintf((char*)g_dtp2, (char*)(p_datseg + DIARY_STRING2),
					host_readw(ptr), get_ttx(host_readw(ptr + 2) + 0x15), city_name);

			} else {
				sprintf((char*)g_dtp2, (char*)(p_datseg + DIARY_STRING3),
					host_readw(ptr), get_ttx(host_readw(ptr + 2) + 0x15), city_name);
			}
		} else {
			if ((signed short)strlen(city_name) > 24) {

				sprintf((char*)g_dtp2, (char*)(p_datseg + DIARY_STRING4), city_name);

			} else if ((signed short)strlen(city_name) > 15) {

				sprintf((char*)g_dtp2, (char*)(p_datseg + DIARY_STRING5), city_name);

			} else if ((signed short)strlen(city_name) > 6) {

				sprintf((char*)g_dtp2, (char*)(p_datseg + DIARY_STRING6), city_name);

			} else {

				sprintf((char*)g_dtp2, (char*)(p_datseg + DIARY_STRING7), city_name);
			}
		}

		GUI_print_string(g_dtp2, 65, (line - startline) * 7);
		ptr += 8;
		line++;
		di++;

	} while ((host_readws(ptr) == day) && (host_readws(ptr + 2) == month));

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = line * 7;
	g_pic_copy.src = g_buffer9_ptr;
#if !defined(__BORLANDC__)
	g_pic_copy.dst = g_renderbuf_ptr + startline * 2240 + 9600;
#else
#define RENDERBUF_PTR (0xd303)
/* TODO: ugly hack, BASM does not like 16bit immediate values with imul */
#define calc_twodim_array_ptr(start, width, disp, off, dst) \
asm { mov ax,disp; db 0x69,0xc0,0xc0,0x08; mov dx, [start + 2]; add ax, [start]; add ax, off; mov[dst + 2],dx; mov [dst],ax }

	/* TODO: ugly hack */
	/*	this calculation of the address of
		a two-dimensional array is done	here with inline assembly */
	calc_twodim_array_ptr(RENDERBUF_PTR, 0x8c0, startline, 9600, PIC_COPY_DST);
#endif
	do_pic_copy(2);

	return line;

}

#if !defined (__BORLANDC__)
}
#endif
