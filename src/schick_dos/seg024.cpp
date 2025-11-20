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
	signed int fg_bak;
	signed int bg_bak;
	signed int bak1;
	signed int bak2;
	signed int txt_tabpos1_bak;
	signed int txt_tabpos2_bak;
	signed int tw_bak;
	signed int i;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;

	g_special_screen = 1;
	g_wallclock_update = 0;
	g_area_prepared = AREA_TYPE_NONE;

	g_current_cursor = &g_default_mouse_cursor;

	load_pp20(ARCHIVE_FILE_BUCH_DAT);
	g_pp20_index = ARCHIVE_FILE_BUCH_DAT;

	get_textcolor(&fg_bak, &bg_bak);

	g_vga_backbuffer = (uint8_t*)g_buffer9_ptr;
	bak1 = g_textline_maxlen;
	bak2 = g_textline_posx;
	txt_tabpos1_bak = g_txt_tabpos[0];
	txt_tabpos2_bak = g_txt_tabpos[1];
	g_textline_maxlen = 200;
	g_textline_posx = 65;
	g_txt_tabpos[0] = 83;
	g_txt_tabpos[1] = 130;

	set_textcolor(4, 0);

	/* print all diary entries */
	i = 0;
	do {
		i = diary_print_entry(i);

	} while (i < gs_diary_entry_counter);

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 199;
	g_pic_copy.src = g_renderbuf_ptr;
	g_pic_copy.dst = g_vga_memstart;

	call_mouse_bg();

	set_palette(g_renderbuf_ptr + 0xfa02, 0, 0x20);

	do_pic_copy(0);

	call_mouse();

	set_textcolor(fg_bak, bg_bak);

	g_pic_copy.dst = g_vga_backbuffer = g_vga_memstart;

	g_textline_posx = bak2;
	g_textline_maxlen = bak1;
	g_txt_tabpos[0] = txt_tabpos1_bak;
	g_txt_tabpos[1] = txt_tabpos2_bak;
	g_textbox_width = tw_bak;

	vsync_or_key(5000);
}

void diary_new_entry(void)
{
	struct struct_diary_entry *entry;

	/* if all entries are occupied, move all entries one slot forward to have the last slot free */
	if (gs_diary_entry_counter == 23) {
		memcpy(&gs_diary_entries[0], &gs_diary_entries[1], (22 * sizeof(struct_diary_entry)));
		gs_diary_entry_counter--;
	}

	/* make a pointer to the last entry */
	entry = &gs_diary_entries[gs_diary_entry_counter - 1];

	/* avoid double entries for the same town */
	if (gs_town_id != entry->town_id) {

		/* make a pointer to the current entry */
		entry = &gs_diary_entries[gs_diary_entry_counter];

		/* increment entry counter */
		gs_diary_entry_counter++;

		/* Write day of month */
		entry->day = gs_day_of_month;
		/* Write month */
		entry->month = gs_month;
		/* Write year */
		entry->year = gs_year;
		/* Write town */
		entry->town_id = gs_town_id;
	}
}

/**
 * \brief   print a diary entry
 *
 * \param   line        the line to start the entry
 */
signed int diary_print_entry(signed int line)
{
	signed int day;
	signed int month;
	signed int startline;
	struct struct_diary_entry *entry;
	char *town_name;
	signed int di = 0;

	memset((uint8_t*)g_buffer9_ptr, 0, 64000);

	entry = &gs_diary_entries[line];

	startline = line;

	do {
		day = entry->day;
		month = entry->month;
		town_name = get_ttx(entry->town_id + 0xeb);

		if (di == 0) {
			if ((signed int)strlen(town_name) > 24) {

				sprintf(g_dtp2, g_diary_string1, entry->day, get_ttx(entry->month + 0x15), town_name);

			} else if ((signed int)strlen(town_name) > 15) {

				sprintf(g_dtp2, g_diary_string2, entry->day, get_ttx(entry->month + 0x15), town_name);

			} else {
				sprintf(g_dtp2, g_diary_string3, entry->day, get_ttx(entry->month + 0x15), town_name);
			}
		} else {
			if ((signed int)strlen(town_name) > 24) {

				sprintf(g_dtp2, g_diary_string4, town_name);

			} else if ((signed int)strlen(town_name) > 15) {

				sprintf(g_dtp2, g_diary_string5, town_name);

			} else if ((signed int)strlen(town_name) > 6) {

				sprintf(g_dtp2, g_diary_string6, town_name);

			} else {

				sprintf(g_dtp2, g_diary_string7, town_name);
			}
		}

		GUI_print_string(g_dtp2, 65, (line - startline) * 7);
		entry++;
		line++;
		di++;

	} while ((entry->day == day) && (entry->month == month));

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = line * 7;
	g_pic_copy.src = (uint8_t*)g_buffer9_ptr;
	g_pic_copy.dst = &g_renderbuf_ptr[(startline * 2240) + 9600];
	do_pic_copy(2);

	return line;

}
