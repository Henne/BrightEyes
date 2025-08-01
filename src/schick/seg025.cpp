/**
 *	Rewrite of DSA1 v3.02_de functions of seg025 (locations)
 *	Functions rewritten: 15/15 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg025.cpp
 */
#include <string.h>
#include <stdio.h>

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif


#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg024.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg049.h"
#include "seg061.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#include "seg051.h"
#include "seg052.h"
#include "seg053.h"
#include "seg054.h"
#include "seg055.h"
#include "seg058.h"
#include "seg059.h"
#include "seg063.h"
#include "seg065.h"
#include "seg066.h"
#include "seg122.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif


void show_entrance(void)
{
	draw_main_screen();
	set_var_to_zero();
	load_ani(34);
	init_ani(1);

	if (GUI_bool(get_ttx(760))) {

		init_ani_busy_loop(2);
		DNG_enter_dungeon(gs_current_typeindex);
	} else {

		leave_location();
	}
}

/**
 * \brief   the screen when entering a house in the city
 */
void show_citizen(void)
{
	ds_writew(REQUEST_REFRESH, 1);

	do {
		handle_gui_input();

		if (ds_readw(REQUEST_REFRESH) != 0) {

			draw_main_screen();
			set_var_to_zero();
			load_ani(20);
			init_ani(ds_writew(REQUEST_REFRESH, 0));

			strcpy(g_text_output_buf, get_tx(gs_current_locdata));

			if ((gs_year == 15) && (gs_month == 1) && (random_schick(100) <= 20)) {

				if (!show_storytext()) {
					GUI_print_loc_line(g_text_output_buf);
				} else {
					ds_writew(ACTION, ACTION_ID_ESC);
				}
			} else {
				GUI_print_loc_line(g_text_output_buf);
#ifdef M302de_SPEEDFIX
				delay_or_keypress(200);
#endif
			}
		}

	} while ((ds_readw(ACTION) == 0) && (ds_readw(MOUSE1_EVENT2) == 0));

	ds_writew(MOUSE1_EVENT2, 0);
	set_var_to_zero();
	copy_palette();
	leave_location();
}

/**
 * \brief   break into a house
 */
void do_house(void)
{
	signed short i;
	signed short l_di;
	Bit8u *hero;

	/* prepare the question */
	strcpy((char*)g_dtp2, get_tx(gs_current_locdata));

	strcat((char*)g_dtp2, get_ttx(623));

	ds_writew(MENU_DEFAULT_SELECT, 1);

	if (GUI_bool((char*)g_dtp2)) {

		/* break into the house */

		set_var_to_zero();
		load_ani(10);
		init_ani(0);

		/* print a randomized text */
		GUI_output(get_ttx(random_schick(8) + 623));

		hero = get_hero(0);

		for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
				!hero_dead(hero) && /* Original-Bug: What if petrified, sleeping etc. */
				(test_skill(hero, TA_VERSTECKEN, -2) <= 0))
			{
				/* every hero must pass a sneak -2 test */

				i = ds_readbs(CURRENT_TOWN);

				if ((i == TOWNS_THORWAL) || (i == TOWNS_PREM) || (i == TOWNS_PHEXCAER) || (i == TOWNS_OBERORKEN)) {

					/* sneak test failed in a town with guards */

					GUI_output(get_ttx(632));

					l_di = 0;

					for (i = 0; i < 6; i++) {

						hero = get_hero(i);

						if (check_hero(hero) && !host_readbs(hero + HERO_JAIL)) {
							l_di = 1;
						}
					}

					if ((ds_readbs(GROUP_MEMBER_COUNTS + gs_current_group) < gs_total_hero_counter) && l_di)
					{
						i = 0;

						while (host_readbs(get_hero(i) + HERO_GROUP_NO) == gs_current_group)
						{
							/* imprison hero */
							host_writeb(get_hero(i) + HERO_JAIL, 1);
							i++;
						}

						GRP_switch_to_next(1);

					} else {

						if (gs_day_timer < HOURS(6)) {
							/* before 6:00 turn clock to 0:00 */
							timewarp_until_time_of_day(HOURS(0));
						}

						timewarp_until_time_of_day(HOURS(6));

						GUI_output(get_ttx(633));
					}
				}
				break;
			}
		}

		set_var_to_zero();

		leave_location();

	} else {
		gs_current_loctype = gs_current_loctype_bak;
		ds_writew(X_TARGET, ds_readw(X_TARGET_BAK));
		ds_writew(Y_TARGET, ds_readw(Y_TARGET_BAK));
	}

}

void do_informer(void)
{
	signed short no = gs_current_typeindex - 1;

	if (no == INFORMER_JURGE)	do_talk(6, 0); else
	if (no == INFORMER_HJORE)	do_talk(6, 1); else
	if (no == INFORMER_YASMA)	do_talk(7, 0); else
	if (no == INFORMER_UMBRIK)	do_talk(7, 1); else
	if (no == INFORMER_ISLEIF)	do_talk(7, 2); else
	if (no == INFORMER_RAGNA)	do_talk(8, 0); else
	if (no == INFORMER_BEORN)	do_talk(8, 1); else
	if (no == INFORMER_ASGRIMM)	do_talk(10, 1); else
	if (no == INFORMER_ELIANE)	do_talk(9, 0); else
	if (no == INFORMER_OLVIR)	do_talk(10, 0); else
	if (no == INFORMER_SWAFNILD)	do_talk(12, 0); else
	if (no == INFORMER_TREBORN)	do_talk(11, 0); else
	if (no == INFORMER_UNICORN)	do_talk(11, 2); else
	if (no == INFORMER_ALGRID)	do_talk(8, 2); else
	if (no == INFORMER_TIOMAR)	do_talk(9, 1);

	leave_location();
}

void enter_map(void)
{
	ds_writew(CURRENT_SIGNPOST, gs_current_typeindex);

	gs_current_typeindex = ds_readbs(CURRENT_TOWN);

	gs_current_loctype = ds_writeb(CURRENT_TOWN, TOWNS_NONE);
	ds_writeb(SHOW_TRAVEL_MAP, 1);
}

void show_treasure_map(void)
{
	signed short l_si;
	signed short tw_bak;
	signed short count;	/* #collected treasure map parts */
	signed short width;
	signed short height;
	signed short pp20_index_bak;
	Bit32s length;
	struct nvf_desc nvf;

	/* count the collected treasure map parts */
	for (l_si = count = 0; l_si < 9; l_si++) {
		if (gs_treasure_maps[l_si]) {
			count++;
		}
	}

	if (count == 0) {
		/* no treasure map parts found */
		GUI_output(get_ttx(609));
	} else {
		g_special_screen = 1;
		pp20_index_bak = ds_readbs(PP20_INDEX);
		ds_writeb(PP20_INDEX, 0xff);
		set_var_to_zero();

		/* load SKARTE.NVF */
		l_si = load_archive_file(ARCHIVE_FILE_SKARTE_NVF);

		read_archive_file(l_si, (Bit8u*)g_buffer9_ptr, 30000);

		length = get_readlength2(l_si);

		close(l_si);

		/* clear the screen */
		wait_for_vsync();

		set_palette(g_palette_allblack2, 0, 0x20);

		do_fill_rect((Bit8u*)ds_readd(FRAMEBUF_PTR), 0, 0, 319, 199, 0);

		update_mouse_cursor();

		for (l_si = 0; l_si < 10; l_si++) {

			if (gs_treasure_maps[l_si] && (l_si != 9 || (l_si == 9 && !gs_treasure_maps[6])))
			{
				/* decompress picture */
				nvf.dst = (Bit8u*)(F_PADD((Bit8u*)g_buffer9_ptr, 30000));
				nvf.src = (Bit8u*)g_buffer9_ptr;
				nvf.no = l_si;
				nvf.type = 0;
				nvf.width = (Bit8u*)&width;
				nvf.height = (Bit8u*)&height;

				process_nvf(&nvf);

				#if !defined(__BORLANDC__)
				/* BE-fix */
				width = host_readws((Bit8u*)&width);
				height = host_readws((Bit8u*)&height);
				#endif

				/* copy to screen */
				ds_writew(PIC_COPY_X1, ds_readws(TMAP_X + 2 * l_si));
				ds_writew(PIC_COPY_Y1, ds_readws(TMAP_Y + 2 * l_si));
				ds_writew(PIC_COPY_X2, ds_readws(TMAP_X + 2 * l_si) + width - 1);
				ds_writew(PIC_COPY_Y2, ds_readws(TMAP_Y + 2 * l_si) + height - 1);
				ds_writed(PIC_COPY_SRC, (Bit32u)F_PADD((Bit8u*)g_buffer9_ptr, 30000));
				ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));
				do_pic_copy(0);
			}
		}

		wait_for_vsync();

		set_palette((Bit8u*)(F_PADD(F_PADD((Bit8u*)g_buffer9_ptr, length), -0x60)), 0, 0x20);

		refresh_screen_size();

		if (ds_readb(TMAP_DOUBLE1) != 0) {

			/* unicorn brought a piece you already have */
			tw_bak = g_textbox_width;
			g_textbox_width = 3;

			GUI_output(get_ttx(808));

			g_textbox_width = tw_bak;
			ds_writeb(TMAP_DOUBLE1, 0);
		}

		if (ds_readb(TMAP_DOUBLE2) != 0) {
			/* you got a piece you already have from the unicorn */
			tw_bak = g_textbox_width;
			g_textbox_width = 3;

			GUI_output(get_ttx(809));

			g_textbox_width = tw_bak;
			ds_writeb(TMAP_DOUBLE2, 0);
		}

		if (count >= 7 && !ds_readb(FIND_HYGGELIK)) {
			/* the way can now be found */

			tw_bak = g_textbox_width;
			g_textbox_width = 3;

			/* */
			sprintf((char*)g_text_output_buf, get_ttx(727),	(char*)get_hero(get_random_hero()) + HERO_NAME2);
			GUI_output(g_text_output_buf);

			g_textbox_width = tw_bak;

			ds_writeb(FIND_HYGGELIK, 1);
		}

		delay_or_keypress(1000);

		if (ds_readb(RENDERBUF_IN_USE_FLAG) != 0) {
			/* copy to screen */
			ds_writew(PIC_COPY_X1, 0);
			ds_writew(PIC_COPY_Y1, 0);
			ds_writew(PIC_COPY_X2, 319);
			ds_writew(PIC_COPY_Y2, 199);
			ds_writed(PIC_COPY_SRC, (Bit32u)g_renderbuf_ptr);
			ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

			update_mouse_cursor();
			wait_for_vsync();

			set_palette(g_renderbuf_ptr + 64000 + 2, 0, 0x20);

			do_pic_copy(0);

			refresh_screen_size();

			ds_writeb(RENDERBUF_IN_USE_FLAG, 0);
			g_special_screen = 0;
			ds_writeb(PP20_INDEX, pp20_index_bak);
		} else {
			g_current_ani = -1;
			ds_writew(REQUEST_REFRESH, 1);
			ds_writew(AREA_PREPARED, -1);
			g_special_screen = 0;
			draw_main_screen();
		}
	}
}

signed short game_options(void)
{
	signed short done;
	signed short answer;
	signed short fg_bak;
	signed short bg_bak;
	signed short bak1;
	signed short bak2;
	signed short tw_bak;
	signed short game_state;
	signed short new_delay;

	done = 0;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;
	g_special_screen = 1;
	g_wallclock_update = 0;
	ds_writew(AREA_PREPARED, -1);
	ds_writed(CURRENT_CURSOR, (Bit32u)(p_datseg + DEFAULT_MOUSE_CURSOR));

	load_pp20(ARCHIVE_FILE_BUCH_DAT);
	ds_writeb(PP20_INDEX, ARCHIVE_FILE_BUCH_DAT);

	get_textcolor(&fg_bak, &bg_bak);

	ds_writed(PRINT_STRING_BUFFER, (Bit32u)g_buffer9_ptr);

	bak1 = ds_readws(TEXTLINE_MAXLEN);
	bak2 = ds_readws(TEXTLINE_POSX);
	ds_writew(TEXTLINE_MAXLEN, 200);
	ds_writew(TEXTLINE_POSX, 70);

	set_textcolor(4, 0);

	memset((Bit8u*)g_buffer9_ptr, 0, 20000);

	prepare_date_str();

	GUI_print_header(g_dtp2);

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 61);
	ds_writed(PIC_COPY_SRC, (Bit32u)g_buffer9_ptr);
	ds_writed(PIC_COPY_DST, (Bit32u)(g_renderbuf_ptr + 9600));
	do_pic_copy(2);

	memset((Bit8u*)g_buffer9_ptr, 0, 28000);

	if (ds_readbs(CURRENT_TOWN) != TOWNS_NONE) {
		/* if the party is in a town */
		load_tx(ARCHIVE_FILE_MAPTEXT_LTX);

		GUI_print_header(get_tx(ds_readbs(CURRENT_TOWN) - 1));

		load_tx(ds_readbs(CURRENT_TOWN) + (ARCHIVE_FILE_CITY_DAT-1));

		ds_writew(PIC_COPY_X1, 0);
		ds_writew(PIC_COPY_Y1, 0);
		ds_writew(PIC_COPY_X2, 319);
		ds_writew(PIC_COPY_Y2, 86);
		ds_writed(PIC_COPY_SRC, (Bit32u)g_buffer9_ptr);
		ds_writed(PIC_COPY_DST, (Bit32u)(g_renderbuf_ptr + 22400));
		do_pic_copy(2);
	}

	/* draw the icons */
	draw_icon(MENU_ICON_LOAD_GAME, 5, 30);
	draw_icon(MENU_ICON_SAVE_GAME, 5, 60);
	draw_icon(MENU_ICON_DELETE_HERO, 5, 90);

	draw_icon(MENU_ICON_HYGELLIK_MAP, 70, 170);
	draw_icon(MENU_ICON_DIARY, 110, 170);
	draw_icon(MENU_ICON_BATTLE_FRAME_RATE, 150, 170);
	draw_icon(MENU_ICON_SOUND, 190, 170);
	draw_icon(MENU_ICON_QUIT_GAME, 236, 170);

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 199);
	ds_writed(PIC_COPY_SRC, (Bit32u)g_renderbuf_ptr);
	ds_writed(PIC_COPY_DST, ds_readd(FRAMEBUF_PTR));

	update_mouse_cursor();
	wait_for_vsync();

	set_palette(g_renderbuf_ptr + 64002, 0, 32);

	do_pic_copy(0);
	refresh_screen_size();

	set_textcolor(fg_bak, bg_bak);

	ds_writed(PIC_COPY_DST, ds_writed(PRINT_STRING_BUFFER, ds_readd(FRAMEBUF_PTR)));

	ds_writew(TEXTLINE_POSX, bak2);
	ds_writew(TEXTLINE_MAXLEN, bak1);
	g_gui_buffer_unkn = (unsigned char*)g_buffer9_ptr;

	do {
		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)(p_datseg + ACTION_TABLE_OPTIONS));
		handle_input();
		ds_writed(ACTION_TABLE_SECONDARY, (Bit32u)0);

		if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			/* use the radio menu */
			answer = GUI_radio(get_ttx(590), 9,
						get_ttx(228),
						get_ttx(229),
						get_ttx(293),
						get_ttx(608),
						get_ttx(830),
						get_ttx(828),
						get_ttx(831),
						get_ttx(588),
						get_ttx(589)) - 1;

			if (answer != -2) {
				ds_writew(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if (ds_readws(ACTION) == ACTION_ID_ICON_1) {

			do {
				game_state = load_game_state();
			} while (game_state == -1);

			if (game_state != 0) {
				done = 1;
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {

			if (save_game_state()) {
				done = 1;
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_3) {

			ds_writeb(RENDERBUF_IN_USE_FLAG, 1);
			char_erase();
			ds_writeb(RENDERBUF_IN_USE_FLAG, 0);

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_4) {

			ds_writeb(RENDERBUF_IN_USE_FLAG, 1);
			show_treasure_map();
			g_special_screen = 1;

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_5) {

			diary_show();
			done = 1;
		} else if (ds_readws(ACTION) == ACTION_ID_ICON_6) {

			sprintf((char*)g_dtp2, get_ttx(827), g_delay_factor);

			new_delay = GUI_input((char*)g_dtp2, 2);

			if (new_delay != -1) {
				g_delay_factor = new_delay;
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_7) {

			sound_menu();

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_8) {

			if (GUI_bool(get_ttx(299))) {

				done = -1;
				ds_writew(GAME_STATE, GAME_STATE_QUIT);
			}

		} else if (ds_readws(ACTION) == ACTION_ID_ICON_9) {
			done = 1;
		}

	} while (!done);

	g_gui_buffer_unkn = g_renderbuf_ptr;

	g_fig_figure1 = g_fig_figure2 = g_current_ani = ds_writebs(PP20_INDEX, -1);
	ds_writew(REQUEST_REFRESH, 1);
	g_special_screen = 0;

	if (ds_readbs(CURRENT_TOWN) != TOWNS_NONE) {
		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;

	return done == -1 ? 1 : 0;
}

void draw_icon(signed short id, signed short x, signed short y)
{
	signed short handle;
	Bit8u* ptr_bak;

	handle = load_archive_file(ARCHIVE_FILE_ICONS);

	seek_archive_file(handle, id * 576L, 0);

	read_archive_file(handle, g_icon, 576);

	close(handle);

	ptr_bak = (Bit8u*)ds_readd(PIC_COPY_DST);

	ds_writed(PIC_COPY_SRC, (Bit32u)g_icon);
	ds_writew(PIC_COPY_X1, x);
	ds_writew(PIC_COPY_Y1, y);
	ds_writew(PIC_COPY_X2, x + 23);
	ds_writew(PIC_COPY_Y2, y + 23);
	ds_writed(PIC_COPY_DST, (Bit32u)g_renderbuf_ptr);
	do_pic_copy(2);

	ds_writed(PIC_COPY_DST, (Bit32u)ptr_bak);
}

/* 0xd54 */
/**
 * \brief   show storytexts
 *
 * \return              1 if dialog was shown / 0 if had already been shown
 * These were introduced in V3.00 (de and en) to find better into the story.
 */
/* static */
signed short show_storytext(void)
{
	char *ptr;
	signed short person;
	signed short icon;

	load_tx(ARCHIVE_FILE_STORY_LTX);

	person = random_schick(17) - 1;

	ptr = get_tx(person);

	switch (person) {

		case 0:	icon = 0x3f; break;
		case 1: icon = 0x44; break;
		case 2: icon = 0x44; break;
		case 3: icon = 0x45; break;
		case 4: icon = 0x43; break;
		case 5: icon = 0x46; break;
		case 6: icon = 0x41; break;
		case 7: icon = 0x42; break;
		case 8: icon = 0x40; break;
		case 9: icon = 0x46; break;
		case 10: icon = 0x40; break;
		case 11: icon = 0x45; break;
		case 12: icon = 0x43; break;
		case 13: icon = 0x40; break;
		case 14: icon = 0x43; break;
		case 15: icon = 0x41; break;
		case 16: icon = 0x44; break;

	}

	if (!ds_readbs(KNOWN_PERSONS + person)) {
		GUI_dialog_na(icon, ptr);
		ds_writeb(KNOWN_PERSONS + person, 1);
		return 1;
	} else {
		return 0;
	}
}

void do_location(void)
{
	signed short bak1;
	signed short bak2;
	signed short tw_bak;
	signed short tm_bak;
	void (*func)(void);

	tm_bak = ds_readb(SHOW_TRAVEL_MAP);
	tw_bak = g_textbox_width;
	bak1 = ds_readws(BASEPOS_X);
	bak2 = ds_readws(BASEPOS_Y);

	ds_writew(BASEPOS_X, 0);
	ds_writew(BASEPOS_Y, 0);
	ds_writeb(SHOW_TRAVEL_MAP, 0);
	g_textbox_width = 3;

	func = g_location_handlers[gs_current_loctype];

	ds_writed(CURRENT_CURSOR, (Bit32u)(p_datseg + DEFAULT_MOUSE_CURSOR));

	if (func) {
		func();
	}

	ds_writew(BASEPOS_X, bak1);
	ds_writew(BASEPOS_Y, bak2);
	g_textbox_width = tw_bak;

	if (!ds_readb(SHOW_TRAVEL_MAP)) {
		ds_writeb(SHOW_TRAVEL_MAP, tm_bak);
	}

	ds_writebs(TRAVEL_MAP_LOADED, -1);
}

/**
 * \brief   leaves a location, including a turn around (rotate by 180 degrees) of the party
 */
void leave_location(void)
{
	set_var_to_zero();

	/* reset location */
	gs_current_loctype = gs_current_loctype_bak;

	/* set target  coordinates*/
	ds_writew(X_TARGET, ds_readw(X_TARGET_BAK));
	ds_writew(Y_TARGET, ds_readw(Y_TARGET_BAK));

	/* rotate party by 180 degrees */
	ds_writeb(DIRECTION, (ds_readbs(DIRECTION) + 2) % 4);

	set_to_ff();

	ds_writew(REQUEST_REFRESH, g_special_screen = 1);
}

void leave_dungeon(void)
{
	signed short i;
	Bit8u *ptr;

	DNG_lights();
	ptr = (Bit8u*)g_text_output_buf;

	memset(g_renderbuf_ptr, 0, 0xc0);

	for (i = 0; i < 64; i++) {

		pal_fade(ptr, g_renderbuf_ptr);
		pal_fade(ptr + 0x60, g_renderbuf_ptr + 0x60);
		wait_for_vsync();
		set_palette(ptr, 0x80, 0x40);
	}

	gs_current_loctype = gs_current_loctype_bak = LOCTYPE_NONE;
	ds_writeb(CURRENT_TOWN, ds_readb(CURRENT_TOWN_BAK));
	ds_writeb(DUNGEON_INDEX_BAK, ds_readb(DUNGEON_INDEX));
	ds_writeb(DUNGEON_INDEX, ds_writeb(DUNGEON_LEVEL, ds_writeb(DUNGEON_LIGHT, 0)));
	ds_writebs(CITY_AREA_LOADED, -1);
	g_fading_state = ds_writew(REQUEST_REFRESH, 1);

	do_fill_rect(g_renderbuf_ptr, 0, 0, 319, 199, 0);

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 240);
	ds_writew(PIC_COPY_Y2, 136);
	ds_writed(PIC_COPY_SRC, (Bit32u)g_renderbuf_ptr);

	update_mouse_cursor();

	do_pic_copy(1);
	refresh_screen_size();
	wait_for_vsync();
	set_palette(g_renderbuf_ptr, 0 , 0x20);

	/* disable the deathtrap */
	gs_deathtrap = 0;
}

/**
 * \brief   party gets 1W6 LE damage
 */
void tumult(void)
{
	signed short tw_bak;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	/* print message */
	GUI_output(get_ttx(764));

	/* each hero in the group looses 1W6 LE */
	sub_group_le(random_schick(6));

	/* the guards or a mob */
	sprintf((char*)g_dtp2, get_ttx(765),
		((ds_readb(CURRENT_TOWN) == TOWNS_PREM ||
			ds_readb(CURRENT_TOWN) == TOWNS_PHEXCAER ||
			ds_readb(CURRENT_TOWN) == TOWNS_THORWAL ||
			ds_readb(CURRENT_TOWN) == TOWNS_OBERORKEN)
				? get_ttx(766) : get_ttx(767)));

	GUI_output((char*)g_dtp2);

	g_textbox_width = tw_bak;
}

/**
 * \brief   fade when leaving a location
 */
void fade_into(void)
{
	Bit8u *ptr;
	signed short i;

	ptr = g_renderbuf_ptr + 0xfa00;

	memset(g_renderbuf_ptr, 0, 0xc0);

	wait_for_vsync();

	set_palette(g_renderbuf_ptr, 0x80, 0x40);

	for (i = 0; i < 0x20; i++) {

		pal_fade(ptr, g_renderbuf_ptr);

		pal_fade(ptr, g_renderbuf_ptr);

		wait_for_vsync();

		set_palette(ptr, 0, 0x20);
	}
}

void copy_palette(void)
{
	memcpy(g_renderbuf_ptr + 0xfa00, (Bit8u*)ds_readd(ANI_PALETTE), 0x60);
	g_fading_state = 2;
}

#if !defined(__BORLANDC__)
}
#endif
