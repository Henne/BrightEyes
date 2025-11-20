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

static struct mouse_action g_action_table_options[10] = {
	{ 0x05, 0x1e, 0x001c, 0x35, 0x81 },
	{ 0x05, 0x3c, 0x001c, 0x53, 0x82 },
	{ 0x05, 0x5a, 0x001c, 0x71, 0x83 },
	{ 0x46, 0xaa, 0x005d, 0xc1, 0x84 },
	{ 0x6e, 0xaa, 0x0085, 0xc1, 0x85 },
	{ 0x96, 0xaa, 0x00ad, 0xc1, 0x86 },
	{ 0xbe, 0xaa, 0x00d5, 0xc1, 0x87 },
	{ 0xec, 0xaa, 0x0103, 0xc1, 0x88 },
	{ 0x00, 0x00, 0x013f, 0xc7, 0x89 },
	{ -0x0001, -0x0001, -0x0001, -0x0001, 0xffff }
}; // ds:0x4bae
static signed int g_tmap_x[10] = { 0x02, 0x76, 0xc1, 0x03, 0x76, 0xc1, 0x00, 0x76, 0xc1, 0x00 }; // ds:0x4c12
static signed int g_tmap_y[10] = { 0x04, 0x04, 0x04, 0x46, 0x46, 0x46, 0x88, 0x88, 0x88, 0x88 }; // ds:0x4c26
unsigned char g_renderbuf_in_use_flag = 0; // ds:0x4c3a

static void (*g_location_handlers[19])(void) = {
	NULL,
	do_location1, /* empty function */
	do_temple,
	do_tavern,
	do_healer,
	do_merchant,
	do_wildcamp,
	do_inn,
	do_smith,
	do_market,
	show_citizen,
	do_harbor,
	enter_map,
	do_informer,
	show_entrance,
	NULL,
	do_house,
	do_special_buildings,
	do_area_camp,
};  // ds:0x4c3b, (void (*)(void))[19];

void show_entrance(void)
{
	draw_main_screen();
	disable_ani();
	load_ani(34);
	init_ani(1);

	if (GUI_bool(get_ttx(760))) {

		init_ani_busy_loop(2);
		DNG_enter_dungeon(gs_town_typeindex);
	} else {

		leave_location();
	}
}

/**
 * \brief   the screen when entering a house in a town
 */
void show_citizen(void)
{
	g_request_refresh = 1;

	do {
		handle_gui_input();

		if (g_request_refresh != 0) {

			draw_main_screen();
			disable_ani();
			load_ani(20);
			init_ani(g_request_refresh = 0);

			strcpy(g_text_output_buf, get_tx(gs_town_locdata));

			if ((gs_year == 15) && (gs_month == 1) && (random_schick(100) <= 20)) {

				if (!show_storytext()) {
					GUI_print_loc_line(g_text_output_buf);
				} else {
					g_action = ACTION_ID_ESC;
				}
			} else {
				GUI_print_loc_line(g_text_output_buf);
				/* REMARK: SPEEDFIX
				vsync_or_key(200);
				*/
			}
		}

	} while ((g_action == 0) && (g_mouse1_event2 == 0));

	g_mouse1_event2 = 0;
	disable_ani();
	copy_palette();
	leave_location();
}

/**
 * \brief   break into a house
 */
void do_house(void)
{
	signed int i; /* dual use as a town_id and a hero_pos */
	signed int unarrested;
	struct struct_hero *hero;

	/* prepare the question */
	strcpy(g_dtp2, get_tx(gs_town_locdata));

	strcat(g_dtp2, get_ttx(623));

	g_menu_default_select = 1;

	if (GUI_bool(g_dtp2)) {

		/* break into the house */

		disable_ani();
		load_ani(10);
		init_ani(0);

		/* print a randomized text */
		GUI_output(get_ttx(random_schick(8) + 623));

		hero = get_hero(0);

		for (i = 0; i < 6; i++, hero++) {

			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
				!hero->flags.dead && /* Original-Bug: What if petrified, sleeping etc. */
				(test_talent(hero, TA_VERSTECKEN, -2) <= 0))
			{
				/* every hero must pass a sneak -2 test */

				i = gs_town_id;

				if ((i == TOWN_ID_THORWAL) || (i == TOWN_ID_PREM) || (i == TOWN_ID_PHEXCAER) || (i == TOWN_ID_OBERORKEN)) {

					/* sneak test failed in a town with guards */

					GUI_output(get_ttx(632));

					unarrested = 0;

					for (i = 0; i < 6; i++) {

						hero = get_hero(i);

						if (check_hero(hero) && !hero->jail) {
							unarrested = 1;
						}
					}

					if ((gs_group_member_counts[gs_active_group_id] < gs_total_hero_counter) && unarrested)
					{
						i = 0;

						while (get_hero(i)->group_id == gs_active_group_id)
						{
							/* imprison hero */
							get_hero(i)->jail = 1;
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

		disable_ani();

		leave_location();

	} else {
		gs_town_loc_type = gs_town_loc_type_bak;
		gs_x_target = gs_x_target_bak;
		gs_y_target = gs_y_target_bak;
	}
}

void do_informer(void)
{
	const signed int informer_id = gs_town_typeindex - 1;

	if (informer_id == INFORMER_JURGE)	do_talk(6, 0); else
	if (informer_id == INFORMER_HJORE)	do_talk(6, 1); else
	if (informer_id == INFORMER_YASMA)	do_talk(7, 0); else
	if (informer_id == INFORMER_UMBRIK)	do_talk(7, 1); else
	if (informer_id == INFORMER_ISLEIF)	do_talk(7, 2); else
	if (informer_id == INFORMER_RAGNA)	do_talk(8, 0); else
	if (informer_id == INFORMER_BEORN)	do_talk(8, 1); else
	if (informer_id == INFORMER_ASGRIMM)	do_talk(10, 1); else
	if (informer_id == INFORMER_ELIANE)	do_talk(9, 0); else
	if (informer_id == INFORMER_OLVIR)	do_talk(10, 0); else
	if (informer_id == INFORMER_SWAFNILD)	do_talk(12, 0); else
	if (informer_id == INFORMER_TREBORN)	do_talk(11, 0); else
	if (informer_id == INFORMER_UNICORN)	do_talk(11, 2); else
	if (informer_id == INFORMER_ALGRID)	do_talk(8, 2); else
	if (informer_id == INFORMER_TIOMAR)	do_talk(9, 1);

	leave_location();
}

void enter_map(void)
{
	gs_current_signpost_typeindex = gs_town_typeindex;

	gs_town_typeindex = gs_town_id;

	gs_town_loc_type = gs_town_id = TOWN_ID_NONE;

	gs_show_travel_map = 1;
}

void show_treasure_map(void)
{
	signed int image_num;	/* REMARK: also reused as handle */
	signed int tw_bak;
	signed int count;	/* #collected treasure map parts */
	signed int width;
	signed int height;
	signed int pp20_index_bak;
	int32_t length;
	struct nvf_extract_desc nvf;

	/* count the collected treasure map parts */
	for (image_num = count = 0; image_num < 9; image_num++) {
		if (gs_treasure_maps[image_num]) {
			count++;
		}
	}

	if (count == 0) {
		/* no treasure map parts found */
		GUI_output(get_ttx(609));
	} else {
		g_special_screen = 1;
		pp20_index_bak = g_pp20_index;
		g_pp20_index = -1;
		disable_ani();

		/* load SKARTE.NVF */
		image_num = load_archive_file(ARCHIVE_FILE_SKARTE_NVF);

		read_archive_file(image_num, (uint8_t*)g_buffer9_ptr, 30000);

		length = get_readlength2(image_num);

		close(image_num);

		/* clear the screen */
		wait_for_vsync();

		set_palette(g_palette_allblack2, 0, 0x20);

		do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);

		call_mouse_bg();

		for (image_num = 0; image_num < 10; image_num++) {

			if (gs_treasure_maps[image_num] && (image_num != 9 || (image_num == 9 && !gs_treasure_maps[6])))
			{
				/* decompress picture */
				nvf.dst = (uint8_t*)(((HugePt)g_buffer9_ptr) + 30000L);
				nvf.src = (uint8_t*)g_buffer9_ptr;
				nvf.image_num = image_num;
				nvf.compression_type = 0;
				nvf.width = &width;
				nvf.height = &height;

				process_nvf_extraction(&nvf);

				/* copy to screen */
				g_pic_copy.x1 = g_tmap_x[image_num];
				g_pic_copy.y1 = g_tmap_y[image_num];
				g_pic_copy.x2 = g_tmap_x[image_num] + width - 1;
				g_pic_copy.y2 = g_tmap_y[image_num] + height - 1;
				g_pic_copy.src = g_buffer9_ptr + 30000L;
				g_pic_copy.dst = g_vga_memstart;
				do_pic_copy(0);
			}
		}

		wait_for_vsync();

		set_palette((uint8_t*)((g_buffer9_ptr + length) -0x60L), 0, 0x20);

		call_mouse();

		if (g_tmap_double1) {

			/* unicorn brought a piece you already have */
			tw_bak = g_textbox_width;
			g_textbox_width = 3;

			GUI_output(get_ttx(808));

			g_textbox_width = tw_bak;
			g_tmap_double1 = 0;
		}

		if (g_tmap_double2) {
			/* you got a piece you already have from the unicorn */
			tw_bak = g_textbox_width;
			g_textbox_width = 3;

			GUI_output(get_ttx(809));

			g_textbox_width = tw_bak;
			g_tmap_double2 = 0;
		}

		if (count >= 7 && !gs_find_hyggelik) {

			/* the way can now be found */

			tw_bak = g_textbox_width;
			g_textbox_width = 3;

			sprintf(g_text_output_buf, get_ttx(727), get_hero(get_random_hero())->alias);
			GUI_output(g_text_output_buf);

			g_textbox_width = tw_bak;

			gs_find_hyggelik = 1;
		}

		vsync_or_key(1000);

		if (g_renderbuf_in_use_flag) {

			/* copy to screen */
			g_pic_copy.x1 = 0;
			g_pic_copy.y1 = 0;
			g_pic_copy.x2 = 319;
			g_pic_copy.y2 = 199;
			g_pic_copy.src = g_renderbuf_ptr;
			g_pic_copy.dst = g_vga_memstart;

			call_mouse_bg();
			wait_for_vsync();

			set_palette(g_renderbuf_ptr + 64000 + 2, 0, 0x20);

			do_pic_copy(0);

			call_mouse();

			g_renderbuf_in_use_flag = 0;

			g_special_screen = 0;
			g_pp20_index = pp20_index_bak;
		} else {
			g_current_ani = -1;
			g_request_refresh = 1;
			g_area_prepared = AREA_TYPE_NONE;
			g_special_screen = 0;
			draw_main_screen();
		}
	}
}

signed int game_options(void)
{
	signed int done = 0;
	signed int answer;
	signed int fg_bak;
	signed int bg_bak;
	signed int bak1;
	signed int bak2;
	signed int tw_bak;
	signed int game_state;
	signed int new_delay;

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
	g_textline_maxlen = 200;
	g_textline_posx = 70;

	set_textcolor(4, 0);

	memset((uint8_t*)g_buffer9_ptr, 0, 20000);

	prepare_date_str();

	GUI_print_header(g_dtp2);

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 61;
	g_pic_copy.src = g_buffer9_ptr;
	g_pic_copy.dst = g_renderbuf_ptr + 9600;
	do_pic_copy(2);

	memset((uint8_t*)g_buffer9_ptr, 0, 28000);

	if (gs_town_id != TOWN_ID_NONE) {

		/* if the party is in a town */
		load_tx(ARCHIVE_FILE_MAPTEXT_LTX);

		GUI_print_header(get_tx(gs_town_id - 1));

		load_tx(gs_town_id + (ARCHIVE_FILE_CITY_DAT-1));

		g_pic_copy.x1 = 0;
		g_pic_copy.y1 = 0;
		g_pic_copy.x2 = 319;
		g_pic_copy.y2 = 86;
		g_pic_copy.src = g_buffer9_ptr;
		g_pic_copy.dst = g_renderbuf_ptr + 22400;
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

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 199;
	g_pic_copy.src = g_renderbuf_ptr;
	g_pic_copy.dst = g_vga_memstart;

	call_mouse_bg();
	wait_for_vsync();

	set_palette(g_renderbuf_ptr + 64002, 0, 32);

	do_pic_copy(0);
	call_mouse();

	set_textcolor(fg_bak, bg_bak);

	g_pic_copy.dst = g_vga_backbuffer = g_vga_memstart;

	g_textline_posx = bak2;
	g_textline_maxlen = bak1;
	g_gui_buffer_unkn = (unsigned char*)g_buffer9_ptr;

	do {
		g_action_table_secondary = &g_action_table_options[0];
		handle_input();
		g_action_table_secondary = NULL;

		if (g_mouse_rightclick_event || g_action == ACTION_ID_PAGE_UP) {

			/* use the radio menu */
			answer = GUI_radio(get_ttx(590), 9,
						get_ttx(228), get_ttx(229), get_ttx(293),
						get_ttx(608), get_ttx(830), get_ttx(828),
						get_ttx(831), get_ttx(588), get_ttx(589)) - 1;

			if (answer != -2) {
				g_action = answer + ACTION_ID_ICON_1;
			}
		}

		if (g_action == ACTION_ID_ICON_1) {

			do {
				game_state = load_game_state();

			} while (game_state == -1);

			if (game_state != 0) {
				done = 1;
			}

		} else if (g_action == ACTION_ID_ICON_2) {

			if (save_game_state()) {
				done = 1;
			}

		} else if (g_action == ACTION_ID_ICON_3) {

			g_renderbuf_in_use_flag = 1;
			char_erase();
			g_renderbuf_in_use_flag = 0;

		} else if (g_action == ACTION_ID_ICON_4) {

			g_renderbuf_in_use_flag = 1;
			show_treasure_map();
			g_special_screen = 1;

		} else if (g_action == ACTION_ID_ICON_5) {

			diary_show();
			done = 1;

		} else if (g_action == ACTION_ID_ICON_6) {

			sprintf(g_dtp2, get_ttx(827), g_delay_factor);

			new_delay = GUI_input(g_dtp2, 2);

			if (new_delay != -1) {
				g_delay_factor = new_delay;
			}

		} else if (g_action == ACTION_ID_ICON_7) {

			sound_menu();

		} else if (g_action == ACTION_ID_ICON_8) {

			if (GUI_bool(get_ttx(299))) {

				done = -1;
				g_game_state = GAME_STATE_QUIT;
			}

		} else if (g_action == ACTION_ID_ICON_9) {

			done = 1;
		}

	} while (!done);

	g_gui_buffer_unkn = g_renderbuf_ptr;

	g_fig_figure1 = g_fig_figure2 = g_current_ani = g_pp20_index = -1;
	g_request_refresh = 1;
	g_special_screen = 0;

	if (gs_town_id != TOWN_ID_NONE) {
		g_fading_state = 3;
	}

	g_textbox_width = tw_bak;

	return done == -1 ? 1 : 0;
}

void draw_icon(const signed int icon_id, const signed int x, const signed int y)
{
	signed int handle;
	uint8_t* dst_bak;

	handle = load_archive_file(ARCHIVE_FILE_ICONS);

	seek_archive_file(handle, icon_id * 576L, 0);

	read_archive_file(handle, g_icon, 576);

	close(handle);

	dst_bak = g_pic_copy.dst;

	g_pic_copy.src = g_icon;
	g_pic_copy.x1 = x;
	g_pic_copy.y1 = y;
	g_pic_copy.x2 = x + 23;
	g_pic_copy.y2 = y + 23;
	g_pic_copy.dst = g_renderbuf_ptr;
	do_pic_copy(2);

	g_pic_copy.dst = dst_bak;
}

/* 0xd54 */
/**
 * \brief   show storytexts
 *
 * \return              1 if dialog was shown / 0 if had already been shown
 * These were introduced in V3.00 (de and en) to find a better way into the story.
 */
/* static */
signed int show_storytext(void)
{
	char *ptr;
	signed int person;
	signed int icon;

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

	if (!gs_known_persons[person]) {

		GUI_dialog_na(icon, ptr);

		gs_known_persons[person] = 1;

		return 1;
	} else {
		return 0;
	}
}

void do_location(void)
{
	signed int bak1;
	signed int bak2;
	signed int tw_bak;
	signed int tm_bak;
	void (*func)(void);

	tm_bak = gs_show_travel_map;
	tw_bak = g_textbox_width;
	bak1 = g_basepos_x;
	bak2 = g_basepos_y;

	g_basepos_x = 0;
	g_basepos_y = 0;
	gs_show_travel_map = 0;
	g_textbox_width = 3;

	func = g_location_handlers[gs_town_loc_type];

	g_current_cursor = &g_default_mouse_cursor;

	if (func) {
		func();
	}

	g_basepos_x = bak1;
	g_basepos_y = bak2;
	g_textbox_width = tw_bak;

	if (!gs_show_travel_map) {
		gs_show_travel_map = tm_bak;
	}

	g_travel_map_loaded = -1;
}

/**
 * \brief   leaves a location, including a turn around (rotate by 180 degrees) of the party
 */
void leave_location(void)
{
	disable_ani();

	/* reset location */
	gs_town_loc_type = gs_town_loc_type_bak;

	/* set target  coordinates*/
	gs_x_target = gs_x_target_bak;
	gs_y_target = gs_y_target_bak;

	/* rotate party by 180 degrees */
	gs_direction = (gs_direction + 2) % 4;

	clear_menu_icons();

	g_request_refresh = g_special_screen = 1;
}

void leave_dungeon(void)
{
	signed int i;
	uint8_t *ptr;

	DNG_lights();
	ptr = (uint8_t*)g_text_output_buf;

	memset(g_renderbuf_ptr, 0, 0xc0);

	for (i = 0; i < 64; i++) {

		pal_fade((int8_t*)ptr, (int8_t*)g_renderbuf_ptr);
		pal_fade((int8_t*)ptr + 0x60, (int8_t*)g_renderbuf_ptr + 0x60);
		wait_for_vsync();
		set_palette(ptr, 0x80, 0x40);
	}

	gs_town_loc_type = gs_town_loc_type_bak = LOCTYPE_NONE;
	gs_town_id = gs_town_id_bak;
	gs_dungeon_id_bak = gs_dungeon_id;
	gs_dungeon_id = gs_dungeon_level = gs_dungeon_light = 0;
	g_town_loaded_town_id = -1;
	g_fading_state = g_request_refresh = 1;

	do_fill_rect(g_renderbuf_ptr, 0, 0, 319, 199, 0);

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 240;
	g_pic_copy.y2 = 136;
	g_pic_copy.src = g_renderbuf_ptr;

	call_mouse_bg();

	do_pic_copy(1);
	call_mouse();
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
	signed int tw_bak;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	/* print message */
	GUI_output(get_ttx(764));

	/* each hero in the group looses 1W6 LE */
	sub_group_le(random_schick(6));

	/* the guards or a mob */
	sprintf(g_dtp2, get_ttx(765),
		((gs_town_id == TOWN_ID_PREM || gs_town_id == TOWN_ID_PHEXCAER ||
	  		gs_town_id == TOWN_ID_THORWAL || gs_town_id == TOWN_ID_OBERORKEN)
				? get_ttx(766) : get_ttx(767)));

	GUI_output(g_dtp2);

	g_textbox_width = tw_bak;
}

/**
 * \brief   fade when leaving a location
 */
void fade_into(void)
{
	uint8_t *ptr;
	signed int i;

	ptr = g_renderbuf_ptr + 320 * 200;

	memset(g_renderbuf_ptr, 0, 0xc0);

	wait_for_vsync();

	set_palette(g_renderbuf_ptr, 0x80, 0x40);

	for (i = 0; i < 0x20; i++) {

		pal_fade((int8_t*)ptr, (int8_t*)g_renderbuf_ptr);

		pal_fade((int8_t*)ptr, (int8_t*)g_renderbuf_ptr);

		wait_for_vsync();

		set_palette(ptr, 0, 0x20);
	}
}

void copy_palette(void)
{
	memcpy(g_renderbuf_ptr + 320 * 200, (uint8_t*)g_ani_palette, 0x60);
	g_fading_state = 2;
}
