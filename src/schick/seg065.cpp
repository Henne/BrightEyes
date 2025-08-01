/**
 *	Rewrite of DSA1 v3.02_de functions of seg065 (special animations)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg065.cpp
 */
#include <string.h>

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg055.h"
#include "seg097.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
void sub_light_timers(signed short);
#endif

void do_market(void)
{
	signed short done;
	signed short answer;
	signed short type_bak;
	signed short dir_bak;
	signed short bak1;

	done = 0;
	ds_writew(REQUEST_REFRESH, 1);
	bak1 = gs_direction_bak;
	dir_bak = gs_direction;

	do {

		if (ds_readw(REQUEST_REFRESH) != 0) {
			draw_main_screen();
			set_var_to_zero();
			load_ani(16);
			init_ani(0);
			ds_writew(REQUEST_REFRESH, 0);
		}

		answer = GUI_radio(get_ttx(680), 4,
					get_ttx(676), get_ttx(677),
					get_ttx(678), get_ttx(613));

		if (answer == 4 || answer == -1) {
			done = 1;
		} else {

			/* set up parameters for this merchant */
			ds_writeb(SHOP_DESCR_TABLE + 90 * 9 + 0, ds_readb((MARKET_DESCR_TABLE + 2) + 8 * gs_current_typeindex));
			ds_writeb(SHOP_DESCR_TABLE + 90 * 9 + 2, ds_readb((MARKET_DESCR_TABLE + 6) + 8 * gs_current_typeindex));
			ds_writeb(SHOP_DESCR_TABLE + 90 * 9 + 1, (signed char)answer);
			type_bak = gs_current_typeindex;
			gs_current_typeindex = 90;

			/* visit this merchant */
			do_merchant();

			/* change back to market */
			gs_current_loctype = LOCTYPE_MARKET;

			/* clean up */
			gs_current_typeindex = type_bak;
			gs_direction_bak = ((signed char)bak1);
			gs_direction = ((signed char)dir_bak); /* by this line, the party will *not* be rotated after leaving the market */
			ds_writeb(SHOP_DESCR_TABLE + 90 * 9 + 0, 0);
			ds_writeb(SHOP_DESCR_TABLE + 90 * 9 + 2, 0);
			ds_writeb(SHOP_DESCR_TABLE + 90 * 9 + 1, 0);
		}

	} while (!done);

	gs_current_loctype = LOCTYPE_NONE;
	copy_palette();
}

void final_intro(void)
{
	signed short i;
	signed short handle;
	signed short width;
	signed short height;
	Bit32u len;
	Bit8u *ptr1;
	Bit8u* ptr2;
	struct nvf_desc nvf;

	ds_writebs(PP20_INDEX, (signed char)(ARCHIVE_FILE_DNGS + 12));

	update_mouse_cursor();

	g_wallclock_update = 0;

	/* load FACE.NVF */
	handle = load_archive_file(ARCHIVE_FILE_FACE_NVF);
	len = read_archive_file(handle, (Bit8u*)g_buffer9_ptr, 64000);
	close(handle);

	ptr1 = (Bit8u*)(F_PADD(F_PADD((Bit8u*)g_buffer9_ptr, len), -(96 * 3)));

	do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);

	wait_for_vsync();

	set_palette(ptr1, 0, 0x60);

	ptr2 = (Bit8u*)F_PADD((Bit8u*)g_buffer9_ptr, 80000);

	nvf.dst = g_renderbuf_ptr;
	nvf.src = (Bit8u*)g_buffer9_ptr;
	nvf.no = 0;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

	map_effect(g_renderbuf_ptr);

	nvf.dst = (Bit8u*)ptr2;
	nvf.src = (Bit8u*)g_buffer9_ptr;
	nvf.no = 1;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 20;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 39;
	g_pic_copy.src = ptr2;
	g_pic_copy.dst = g_renderbuf_ptr;

	do_pic_copy(2);

	delay_or_keypress(100);

	map_effect(g_renderbuf_ptr);

	g_pic_copy.dst = (g_vga_memstart);

	delay_or_keypress(250);

	memset(g_renderbuf_ptr, 0, 96 * 3);

	for (i = 0; i < 0x40; i++) {

		pal_fade(ptr1, g_renderbuf_ptr);
		pal_fade(ptr1 + 0x60, g_renderbuf_ptr + 0x60);
		pal_fade(ptr1 + 0xc0, g_renderbuf_ptr + 0xc0);

		wait_for_vsync();

		set_palette(ptr1, 0, 0x60);
	}

	/* TODO: update window */
	memset((void*)(g_vga_memstart), 0, 320 * 200);

	refresh_colors();
	refresh_screen_size();
}

#if defined(__BORLANDC__)
static
#endif
Bit8u* hyg_ani_1(signed short nvf_no, Bit8u *ptr)
{
	HugePt retval;
	struct nvf_desc nvf;

	nvf.dst = (Bit8u*)(host_readd(ptr));
	nvf.src = g_renderbuf_ptr;
	nvf.no = nvf_no;
	nvf.type = 3;
	nvf.width = ptr + 4;
	nvf.height = ptr + 6;

	process_nvf(&nvf);

	retval = F_PADD((Bit8u*)host_readd(ptr),
			host_readws(ptr + 4) * host_readws(ptr + 6));

	return (Bit8u*)retval;
}

#if defined(__BORLANDC__)
static
#endif
void hyg_ani_2(Bit8u *ptr, signed short x, signed short y)
{
	g_pic_copy.x1 = x;
	g_pic_copy.y1 = y;
	g_pic_copy.x2 = x + host_readws(ptr + 4) - 1;
	g_pic_copy.y2 = y + host_readws(ptr + 6) - 1;

	//ds_writed(PIC_COPY_SRC, host_readd(ptr));
	g_pic_copy.src = (unsigned char*)(*((unsigned char*)ptr));
	g_pic_copy.dst = g_renderbuf_ptr;

	do_pic_copy(2);
}

#if defined(__BORLANDC__)
static
#endif
void hyg_ani_3(void)
{
	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 199;
	g_pic_copy.src = (F_PADD((Bit8u*)g_buffer9_ptr, 0x1fbd0));
	g_pic_copy.dst = g_renderbuf_ptr;

	do_pic_copy(0);
}

#if defined(__BORLANDC__)
static
#endif
void hyg_ani_4(void)
{
	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 199;
	g_pic_copy.src = g_renderbuf_ptr;
	g_pic_copy.dst = g_vga_memstart;

	do_pic_copy(0);
}

void show_hyggelik_ani(void)
{
	signed short i;
	signed short handle;
	Bit32s filelen;
	Bit8u *src;
	Bit8u* ptr1;
	Bit8u* ptr2;
	Bit8u array[30*8];

	g_wallclock_update = 0;
	ptr1 = (Bit8u*)g_buffer9_ptr;
	ptr2 = (Bit8u*)F_PADD((Bit8u*)g_buffer9_ptr, 0x1fbd0);

	handle = load_archive_file(ARCHIVE_FILE_HYGBACK_NVF);
	filelen = read_archive_file(handle, g_renderbuf_ptr, 64000);
	close(handle);
	src = &(g_renderbuf_ptr[filelen - 0xc0]);

	do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);
	memcpy((void*)g_dtp2, src, 192);
	src = (Bit8u*)g_dtp2;

	wait_for_vsync();
	set_palette(src, 0 , 0x40);
	host_writed(array + 0, (Bit32u)ptr2);

	hyg_ani_1(0, array);

	handle = load_archive_file(ARCHIVE_FILE_HYGGELIK_NVF);
	filelen = read_archive_file(handle, g_renderbuf_ptr, 64000);
	close(handle);
	host_writed(array + 0, (Bit32u)ptr1);

	for (i = 0; i < 26; i++) {
		host_writed(array + 8 * i + 8, (Bit32u)hyg_ani_1(i, array + 8 * i));
	}

	update_mouse_cursor();

	hyg_ani_3();
	hyg_ani_2(array + 0 * 8, 145, 39);
	hyg_ani_2(array + 7 * 8, 142, 86);
	hyg_ani_2(array + 10 * 8, 82, 67);
	hyg_ani_2(array + 20 * 8, 186, 67);

	map_effect(g_renderbuf_ptr);

	for (i = 0; i < 7; i++) {
		hyg_ani_3();
		hyg_ani_2(array + 7 * 8, 142, 86);
		hyg_ani_2((array + 0 * 8) + 8 * i, 145, 39);
		hyg_ani_2(array + 10 * 8, 82, 67);
		hyg_ani_2(array + 20 * 8, 186, 67);
		hyg_ani_4();
		delay_or_keypress(3);
	}

	delay_or_keypress(6);

	for (i = 0; i < 5; i++) {
		hyg_ani_3();
		hyg_ani_2(array + 6 * 8, 145, 39);
		hyg_ani_2(array + 7 * 8, 142, 86);
		hyg_ani_2(array + 20 * 8, 186, 67);
		hyg_ani_2(array + 10 * 8 + 8 * i, ds_readb(HYG_ANI_X0 + i), 67);
		hyg_ani_4();
		delay_or_keypress(3);
	}

	hyg_ani_3();
	hyg_ani_2(array + 5 * 8, 145, 39);
	hyg_ani_2(array + 7 * 8, 142, 86);
	hyg_ani_2(array + 15 * 8, ds_readb(HYG_ANI_X1), 67);
	hyg_ani_2(array + 20 * 8, ds_readb(HYG_ANI_X5), 67);
	hyg_ani_4();
	delay_or_keypress(3);

	hyg_ani_3();
	hyg_ani_2(array + 5 * 8, 145, 39);
	hyg_ani_2(array + 7 * 8, 142, 86);
	hyg_ani_2(array + 16 * 8, ds_readb(HYG_ANI_X2), 67);
	hyg_ani_2(array + 21 * 8, ds_readb(HYG_ANI_X6), 67);
	hyg_ani_4();
	delay_or_keypress(3);

	for (i = 0; i < 3; i++) {
		hyg_ani_3();
		hyg_ani_2(array + 6 * 8, 145, 39);
		hyg_ani_2(array + 8 * 8, 144, ds_readb(HYG_ANI_X9 + i));
		hyg_ani_2(array + 17 * 8 + 8 * i, ds_readb(HYG_ANI_X3 + i), 67);
		hyg_ani_2(array + 22 * 8 + 8 * i, ds_readb(HYG_ANI_X7 + i), 67);
		hyg_ani_4();
		delay_or_keypress(3);
	}

	hyg_ani_3();
	hyg_ani_2(array + 6 * 8, 145, 39);
	hyg_ani_2(array + 9 * 8, 125, 104);
	hyg_ani_2(array + 19 * 8, ds_readb((HYG_ANI_X3+2)), 67);
	hyg_ani_2(array + 24 * 8, ds_readb((HYG_ANI_X7+2)), 67);
	hyg_ani_4();
	delay_or_keypress(100);

	/* clear the screen */
	do_fill_rect(g_renderbuf_ptr, 0, 0, 319, 199, 0);

	hyg_ani_2(array + 25 * 8, 100, 0);
	g_pic_copy.dst = (g_vga_memstart);
	map_effect(g_renderbuf_ptr);
	delay_or_keypress(500);

	memset(g_renderbuf_ptr, 0, 0xc0);

	for (i = 0; i < 64; i++) {
		pal_fade(src, g_renderbuf_ptr);
		pal_fade(src + 0x60, g_renderbuf_ptr + 0x60);
		wait_for_vsync();
		set_palette(src, 0, 0x40);
	}

	refresh_screen_size();
	/* TODO: update window */
	memset((void*)(g_vga_memstart), 0, 320 * 200);
	refresh_colors();
}

void show_times_up(void)
{
	signed short fi_bak;
	signed short tw_bak;
	signed short bak1;
	signed short bak2;

	fi_bak = g_text_file_index;
	load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
	set_audio_track(ARCHIVE_FILE_VICTORY_XMI);
	set_var_to_zero();
	draw_main_screen();
	load_ani(35);
	init_ani(0);

	delay_or_keypress(200);

	tw_bak = g_textbox_width;
	bak1 = g_basepos_x;
	bak2 = g_basepos_y;

	g_textbox_width = 7;
	g_basepos_x = 0;
	g_basepos_y = 55;

	GUI_output(get_tx2(55));
	GUI_output(get_tx2(56));
	GUI_output(get_tx2(57));

	g_basepos_x = bak1;
	g_basepos_y = bak2;
	g_textbox_width = tw_bak;

	/* restore text file except for CHARTEXT.LTX, TAVERN.TLK and except for dialogs */
	if (fi_bak != -1 && fi_bak != ARCHIVE_FILE_CHARTEXT_LTX
	    && fi_bak != ARCHIVE_FILE_TAVERN_TLK
	    && !(fi_bak >= 156 && fi_bak <= 176)) {
		load_tx2(fi_bak);
	}
}

void show_outro(void)
{
	signed short j;
	signed short handle;
	signed short width;
	signed short height;
	unsigned short len;
	Bit8u *pal_ptr;
	Bit8u *hero;
	signed short i;
	struct nvf_desc nvf;

	g_textbox_width = 7;
	g_basepos_x = 0;
	g_basepos_y = 60;

	load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
	set_audio_track(ARCHIVE_FILE_VICTORY_XMI);
	set_var_to_zero();

	/* load OUTRO1.NVF */
	handle = load_archive_file(ARCHIVE_FILE_OUTRO1_NVF);
	len = read_archive_file(handle, (Bit8u*)g_buffer9_ptr, 64000);
	close(handle);

	pal_ptr = (Bit8u*)(F_PADD(F_PADD((Bit8u*)g_buffer9_ptr, len), - 0xc0));
	do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);
	wait_for_vsync();
	set_palette(pal_ptr, 0, 0x40);

	nvf.dst = g_renderbuf_ptr;
	nvf.src = (Bit8u*)g_buffer9_ptr;
	nvf.no = 0;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	g_pic_copy.x1 = (320 - width) / 2;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = (320 - width) / 2 + width - 1;
	g_pic_copy.y2 = height - 1;
	g_pic_copy.src = g_renderbuf_ptr;
	do_pic_copy(0);

	delay_or_keypress(200);

	GUI_output(get_tx2(58));

	/* load OUTRO2.NVF */
	handle = load_archive_file(ARCHIVE_FILE_OUTRO2_NVF);
	len = read_archive_file(handle, (Bit8u*)g_buffer9_ptr, 64000);
	close(handle);

	pal_ptr = (Bit8u*)(F_PADD(F_PADD((Bit8u*)g_buffer9_ptr, len), - 0xc0));
	do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);
	wait_for_vsync();
	set_palette(pal_ptr, 0, 0x40);

	nvf.dst = g_renderbuf_ptr;
	nvf.src = (Bit8u*)g_buffer9_ptr;
	nvf.no = 0;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	g_pic_copy.x1 = (320 - width) / 2;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = (320 - width) / 2 + width - 1;
	g_pic_copy.y2 = height - 1;
	g_pic_copy.src = g_renderbuf_ptr;
	do_pic_copy(0);

	delay_or_keypress(200);

	GUI_output(get_tx2(59));

	/* load OUTRO3.NVF */
	handle = load_archive_file(ARCHIVE_FILE_OUTRO3_NVF);
	len = read_archive_file(handle, (Bit8u*)g_buffer9_ptr, 64000);
	close(handle);

	pal_ptr = (Bit8u*)(F_PADD(F_PADD((Bit8u*)g_buffer9_ptr, len), - 0xc0));
	do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);
	wait_for_vsync();
	set_palette(pal_ptr, 0, 0x40);

	nvf.dst = g_renderbuf_ptr;
	nvf.src = (Bit8u*)g_buffer9_ptr;
	nvf.no = 0;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	g_pic_copy.x1 = (320 - width) / 2;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = (320 - width) / 2 + width - 1;
	g_pic_copy.y2 = height - 1;
	g_pic_copy.src = g_renderbuf_ptr;
	do_pic_copy(0);

	delay_or_keypress(200);

	GUI_output(get_tx2(60));
	GUI_output(get_tx2(61));
	GUI_output(get_tx2(62));
	GUI_output(get_tx2(63));
	GUI_output(get_tx2(64));
	GUI_output(get_tx2(65));

	/* reset the timers */
	sub_ingame_timers(DAYS(30));
	sub_mod_timers(DAYS(30));
	sub_light_timers(100);

	/* give the heroes the reward and restore them */
	hero = get_hero(0);
	for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE)) {

			/* get 50D */
			add_party_money(5000);

			and_ptr_bs(hero + HERO_FLAGS1, 0xfd); /* unset 'asleep' flag */
			and_ptr_bs(hero + HERO_FLAGS1, 0xfb); /* unset 'petrified' flag */
			and_ptr_bs(hero + HERO_FLAGS1, 0xf7); /* unset 'brewing' flag */
			and_ptr_bs(hero + HERO_FLAGS1, 0xdf); /* unset 'renegade' flag */

			/* reset every disease */
			for (j = 0; j < 8; j++) {

				host_writeb(hero + (0xae + 0) + 5 * j, 0);
				host_writeb(hero + (0xae + 1) + 5 * j, 0);
				host_writeb(hero + (0xae + 2) + 5 * j, 0);
				host_writeb(hero + (0xae + 3) + 5 * j, 0);
				host_writeb(hero + (0xae + 4) + 5 * j, 0);
			}

			/* reset every poison */
			for (j = 0; j < 10; j++) {

				host_writeb(hero + (0xd6 + 0) + 5 * j, 0);
				host_writeb(hero + (0xd6 + 1) + 5 * j, 0);
				host_writeb(hero + (0xd6 + 2) + 5 * j, 0);
				host_writeb(hero + (0xd6 + 3) + 5 * j, 0);
				host_writeb(hero + (0xd6 + 4) + 5 * j, 0);
			}

#ifdef M302de_ORIGINAL_BUGFIX
			/* Original-Bug: restore permanent LE-damage at the end of game */
			/*  Famous heroes get healed for free */
			if (host_readbs(hero + HERO_LE_MOD) > 0)
			{
				add_ptr_ws(hero + HERO_LE_ORIG, host_readbs(hero + HERO_LE_MOD));
				host_writebs(hero + HERO_LE_MOD, 0);
			}
#endif

			/* set LE to the max */
			add_hero_le(hero, host_readws(hero + HERO_LE_ORIG));
			/* set AE to the max */
			add_hero_ae(hero, host_readws(hero + HERO_AE_ORIG));

			/* ??? */
#ifndef M302de_ORIGINAL_BUGFIX
			host_writeb(hero + HERO_LE_MOD, 0);
#endif
			host_writeb(hero + HERO_HUNGER, 0);
			host_writeb(hero + HERO_THIRST, 0);
			host_writeb(hero + HERO_TEMPLE_ID, 0);
			host_writeb(hero + HERO_RECIPE_ID, 0);
			host_writeb(hero + HERO_RECIPE_TIMER, 0);
			host_writeb(hero + HERO_ALCHEMY_INN_ID, 0);
			host_writeb(hero + HERO_JAIL, 0);
			host_writeb(hero + HERO_DRUNK, 0);
			host_writeb(hero + HERO_RUHE_KOERPER, 0);

			for (j = 0; j <= 13; j++) {

				host_writeb(hero + HERO_ATTRIB + 3 * j, host_readbs(hero + HERO_ATTRIB_ORIG + 3 * j));
				host_writeb(hero + HERO_ATTRIB_MOD + 3 * j, 0);
			}

			host_writed(hero + HERO_HEAL_TIMER, 0);
			host_writed(hero + HERO_STAFFSPELL_TIMER, 0);

			host_writeb(hero + HERO_GROUP_POS, i + 1);
		}
	}

	/* share all money */
	set_party_money(get_party_money());

	/* mark the game as done */
	gs_datseg_status_start = 99;

	g_basepos_x = 0;
	g_basepos_y = 0;

	/* save the game */
	save_game_state();
}

#if !defined(__BORLANDC__)
}
#endif
