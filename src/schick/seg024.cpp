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
	Bit16u bak1, bak2, txt_tabpos1_bak, txt_tabpos2_bak, textbox_width_bak;
	signed short i;

	textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);

	ds_writeb(SPECIAL_SCREEN, 1);
	ds_writew(WALLCLOCK_UPDATE, 0);
	ds_writew(AREA_PREPARED, 0xffff);
	ds_writed(CURRENT_CURSOR, (Bit32u)(p_datseg + DEFAULT_MOUSE_CURSOR));

	load_pp20(ARCHIVE_FILE_BUCH_DAT);
	ds_writeb(PP20_INDEX, ARCHIVE_FILE_BUCH_DAT);

	get_textcolor(&fg_bak, &bg_bak);

	ds_writed(PRINT_STRING_BUFFER, ds_readd(BUFFER9_PTR));
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

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 199);
	ds_writed(PIC_COPY_SRC, ds_readd(RENDERBUF_PTR));
	ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

	update_mouse_cursor();

	set_palette((Bit8u*)ds_readd(RENDERBUF_PTR) + 0xfa02, 0, 0x20);

	do_pic_copy(0);

	refresh_screen_size();

	set_textcolor(fg_bak, bg_bak);

	ds_writed(PIC_COPY_DST, ds_writed(PRINT_STRING_BUFFER, ds_readd(FRAMEBUF_PTR)));

	ds_writew(TEXTLINE_POSX, bak2);
	ds_writew(TEXTLINE_MAXLEN, bak1);
	ds_writew(TXT_TABPOS1, txt_tabpos1_bak);
	ds_writew(TXT_TABPOS2, txt_tabpos2_bak);
	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);

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
	if (ds_readbs(CURRENT_TOWN) != host_readw(ptr + 6)) {

		/* make a pointer to the current entry */
		ptr = p_datseg + DIARY_ENTRIES + ds_readw(DIARY_ENTRY_COUNTER) * 8;

		/* deccrement entry counter */
		inc_ds_ws(DIARY_ENTRY_COUNTER);

		/* Write day of month */
		host_writew(ptr, ds_readbs(DAY_OF_MONTH));
		/* Write month */
		host_writew(ptr + 2, ds_readbs(MONTH));
		/* Write year */
		host_writew(ptr + 4, ds_readbs(YEAR));
		/* Write city */
		host_writew(ptr + 6, ds_readbs(CURRENT_TOWN));
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

	memset((Bit8u*)ds_readd(BUFFER9_PTR), 0, 64000);

	ptr = p_datseg + DIARY_ENTRIES + line * 8;

	startline = line;

	do {
		day = host_readw(ptr);
		month = host_readw(ptr + 2);
		city_name = get_ttx(host_readw(ptr + 6) + 0xeb);

		if (di == 0) {
			if ((signed short)strlen(city_name) > 24) {

				sprintf((char*)ds_readd(DTP2), (char*)(p_datseg + DIARY_STRING1),
					host_readw(ptr), get_ttx(host_readw(ptr + 2) + 0x15), city_name);

			} else if ((signed short)strlen(city_name) > 15 ) {

				sprintf((char*)ds_readd(DTP2), (char*)(p_datseg + DIARY_STRING2),
					host_readw(ptr), get_ttx(host_readw(ptr + 2) + 0x15), city_name);

			} else {
				sprintf((char*)ds_readd(DTP2), (char*)(p_datseg + DIARY_STRING3),
					host_readw(ptr), get_ttx(host_readw(ptr + 2) + 0x15), city_name);
			}
		} else {
			if ((signed short)strlen(city_name) > 24) {

				sprintf((char*)ds_readd(DTP2), (char*)(p_datseg + DIARY_STRING4), city_name);

			} else if ((signed short)strlen(city_name) > 15) {

				sprintf((char*)ds_readd(DTP2), (char*)(p_datseg + DIARY_STRING5), city_name);

			} else if ((signed short)strlen(city_name) > 6) {

				sprintf((char*)ds_readd(DTP2), (char*)(p_datseg + DIARY_STRING6), city_name);

			} else {

				sprintf((char*)ds_readd(DTP2), (char*)(p_datseg + DIARY_STRING7), city_name);
			}
		}

		GUI_print_string((char*)ds_readd(DTP2), 65, (line - startline) * 7);
		ptr += 8;
		line++;
		di++;

	} while ((host_readws(ptr) == day) && (host_readws(ptr + 2) == month));

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, line * 7);
	ds_writed(PIC_COPY_SRC, ds_readd(BUFFER9_PTR));
#if !defined(__BORLANDC__)
	ds_writed(PIC_COPY_DST,
		(Bit32u)(((Bit8u*)ds_readd(RENDERBUF_PTR) + startline * 2240) + 9600));
#else
	/* TODO: ugly hack */
	/*	this calculation of the address of
		a twodimiensional array is done
		here with inline assembly */
	calc_twodim_array_ptr(RENDERBUF_PTR, 0x8c0, startline, 9600, PIC_COPY_DST);
#endif
	do_pic_copy(2);

	return line;

}

#if !defined (__BORLANDC__)
}
#endif
