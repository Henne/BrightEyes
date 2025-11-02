/*
 *	Rewrite of DSA1 v3.02_de functions of seg004 (Graphic)
 *	Functions rewritten: 37/37 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg004.cpp
 */

#include <string.h>

#if defined(__BORLANDC__)
#include <DOS.H>
#else
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static Bit16s g_ani_busy = 0; // ds:0x4a90
Bit16s g_pic_copy_flag = 0; // ds:0x4a92
static Bit16u  g_status_bar_colors[2] = { 0xf0, 0xf9 }; // ds:0x4a94, {le_color, 0, ae_color, 0}
static unsigned char g_unkn_035[2] = { 0, 0 }; // ds:0x4a98
static Bit8s  g_status_page_hunger_max_counter = 0; // ds:0x4a9a
static Bit8s  g_status_page_hunger_max_color = 0; // ds:0x4a9b
static Bit8s  g_status_page_thirst_max_counter = 0; // ds:0x4a9c
static Bit8s  g_status_page_thirst_max_color = 0; // ds:0x4a9d
static signed short g_wallclock_pos = 0; // ds:0x4a9e, position of sun/moon
static Bit8s g_wallclock_pos_y[81] = { 0x14, 0x12, 0x11, 0x10, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0b, 0x0a, 0x09, 0x09, 0x08, 0x07, 0x07, 0x06, 0x06, 0x05, 0x05, 0x05, 0x04, 0x04, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x05, 0x06, 0x06, 0x07, 0x07, 0x08, 0x09, 0x09, 0x0a, 0x0b, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x14 }; // ds:0x4aa0
static Bit8u g_wallclock_palette_day[3][3] = { { 0x10, 0x14, 0x3c }, { 0x00, 0x08, 0x38 }, { 0x00, 0x08, 0x34 } }; // ds:0x4af1
static Bit8u g_wallclock_palette_night[3][3] = { { 0x00, 0x00, 0x1a }, { 0x00, 0x00, 0x18 }, { 0x00, 0x00, 0x1c } }; // ds:0x4afa
static struct Bit8u_3 g_color_pal_white = { 0x3f, 0x3f, 0x3f }; // ds:0x4b03, {0x3f,0x3f,0x3f}
static struct Bit8u_32_3 g_palette_allblack = { { 0 } }; // ds:0x4b06


static Bit32s g_gfx_spinlock;			// ds:0xe234
static signed short g_ani_change_dir[10];	// ds:0xe238
static signed short g_ani_area_status[10];	// ds:0xe24c
static signed short g_ani_area_timeout[10]; 	// ds:0xe260
#if defined(__BORLANDC__)
static void interrupt far (*g_bc_timer)(...);	// ds:0xe274
#endif


#if defined(__BORLANDC__)
/* static prototype */
void interrupt timer_isr(void);
#endif

void save_and_set_timer(void)
{
#if defined(__BORLANDC__)
	g_bc_timer = getvect(8);
	setvect(8, (void interrupt far(*)(...))timer_isr);
	/* REMARK: for C-compilation replace (...) with (void) */
#endif
}

void set_timer(void)
{
#if defined(__BORLANDC__)
	setvect(8, (void interrupt far(*)(...))timer_isr);
#endif
}

void reset_timer(void)
{
#if defined(__BORLANDC__)
	setvect(8, (void interrupt far(*)(...))g_bc_timer);
#endif
}

void init_ani(Bit16u v1)
{
	signed short i;

	if (g_current_ani == -1)
		return;

	if ((v1 & 0x7f) != 2) {
		for (i = 0; i < 10; i++) {
			g_ani_area_timeout[i] = 0;
			g_ani_area_status[i] = -1;
			g_ani_change_dir[i] = 1;
		}

		if (v1 & 0x80)
			g_ani_unknown_flag = 0;
		else
			g_ani_unknown_flag = 1;

		call_mouse_bg();

		clear_ani_pal();

		/* set flag for pic_copy() */
		g_pic_copy_flag = 1;

		/* set upper left coordinates */
		g_pic_copy.x1 = g_ani_posx;
		g_pic_copy.y1 = g_ani_posy;

		/* set lower right coordinates */
		g_pic_copy.x2 = g_ani_posx + g_ani_width - 1;
		g_pic_copy.y2 = g_ani_posy + g_ani_height - 1;

		/* copy pointer */
		g_pic_copy.src = g_ani_main_ptr;

		/* copy the main ani picture */
		do_pic_copy(1);

		set_ani_pal((Bit8u*)g_ani_palette);

		/* reset flag for pic_copy() */
		g_pic_copy_flag = 0;

		call_mouse();
	}

	if ((v1 & 0x7f) != 1) {
		wait_for_vsync();
		g_ani_enabled = 1;
	}

	wait_for_vsync();
}

void set_var_to_zero(void)
{
	g_ani_enabled = 0;
}

void init_ani_busy_loop(unsigned short v1)
{
	/* set lock */
	g_ani_busy = 1;

	init_ani(v1);

	 while (g_ani_busy) {
#ifdef M302de_SPEEDFIX
		/*	enter emulation mode frequently,
			that the timer can reset this variable */
		wait_for_vsync();
#endif
	 }
}

void clear_ani(void)
{
	signed short i, j;

	g_ani_width = 0;
	g_ani_height = 0;
	g_ani_areacount = 0;
	g_ani_main_ptr = NULL;
	g_ani_palette = NULL;

	for (i = 0; i < 10; i++) {

		g_ani_area_table[i].x = 0;
		g_ani_area_table[i].y = 0;
		g_ani_area_table[i].width = 0;
		g_ani_area_table[i].height = 0;
		g_ani_area_table[i].cyclic = 0;
		g_ani_area_table[i].pics = 0;
		g_ani_area_table[i].changes = 0;
		g_ani_area_table[i].name[4] = '\0';

		for (j = 0; j < 20; j++) {
			g_ani_area_table[i].pics_tab[j] = NULL;
		}

		for (j = 0; j < 42; j++) {
			g_ani_area_table[i].changes_tb[j].pic = 0;
			g_ani_area_table[i].changes_tb[j].duration = 0;
		}
	 }
}

#if defined(__BORLANDC__)
void interrupt timer_isr(void)
{
	signed short i;
	signed short l_di;
	signed char flag;
	struct ani_area *ptr;
	struct struct_pic_copy pic_copy_bak;

	/* TODO: unused feature */
	g_gfx_spinlock += 1;

	g_random_schick_seed2++;

	if (g_random_schick_seed2 < 0) {
		g_random_schick_seed2 = 0;
	}

	if (g_autofight && (bioskey(1) || g_mouse1_event2)) {

		g_autofight = 2;
		g_mouse1_event2 = 0;
	}

	start_midi_playback_IRQ();

	if (g_delay_timer) {
		g_delay_timer--;
	}

	/* another timer used in fights */
	if ((g_fig_star_timer > 0) && g_fig_continue_print && g_fig_star_printed)
	{
		--g_fig_star_timer;
	}

	if (!g_freeze_timers) {
		do_timers();
	}

	update_status_bars();
	seg002_37c4();
	update_wallclock();
	dec_splash();

	if (g_ani_enabled) {

		/* disable interrupts */
		asm { cli; }

		g_pic_copy_rect.y1 = g_ani_posy;
		g_pic_copy_rect.x1 = g_ani_posx;
		g_pic_copy_rect.y2 = g_ani_posy + 135;
		g_pic_copy_rect.x2 = g_ani_posx + 208;
		pic_copy_bak = g_pic_copy;

		l_di = g_ani_areacount;

		if (!l_di && g_ani_busy) {

			g_ani_enabled = 0;
			g_ani_busy = 0;
		}

		for (i = 0; i < l_di; i++) {

			ptr = &g_ani_area_table[i];

			if (ptr->changes) {

				g_ani_area_timeout[i] -= 5;

				if (g_ani_area_timeout[i] <= 0) {

					g_ani_area_status[i] += g_ani_change_dir[i];

					if (g_ani_area_status[i] == ptr->changes) {

						if (ptr->cyclic) {
							g_ani_change_dir[i] = -1;
							g_ani_area_status[i]--;
						} else {
							g_ani_area_status[i] = 0;
						}

						if (g_ani_busy) {
							g_ani_enabled = 0;
							g_ani_busy = 0;
							break;
						}
					}

					if (!g_ani_area_status[i] && ptr->cyclic)
					{
						g_ani_change_dir[i] = 1;
					}

					g_ani_area_timeout[i] = 2 * ptr->changes_tb[g_ani_area_status[i]].duration;

					flag = 0;

					if ((g_mouse_posx >= g_ani_posx) &&
						(g_ani_posx + g_ani_width >= g_mouse_posx) &&
						(g_mouse_posy >= g_ani_posy) &&
						(g_ani_posy + g_ani_height >= g_mouse_posy))
					{
						flag = 1;
						call_mouse_bg();
					}

					/* set screen coordinates */
					g_pic_copy.x1 = g_ani_posx + ptr->x;
					g_pic_copy.y1 =	g_ani_posy + ptr->y;
					g_pic_copy.x2 = g_ani_posx + ptr->x + ptr->width - 1;
					g_pic_copy.y2 = g_ani_posy + ptr->y + ptr->height - 1;
					g_pic_copy.src = ptr->pics_tab[ptr->changes_tb[g_ani_area_status[i]].pic - 1];

					do_pic_copy(1);

					if (flag != 0) {
						call_mouse();
						flag = 0;
					}
				}
			}
		}

		g_pic_copy = pic_copy_bak;
		g_pic_copy_rect.y1 = 0;
		g_pic_copy_rect.x1 = 0;
		g_pic_copy_rect.y2 = 200 - 1;
		g_pic_copy_rect.x2 = 320 - 1;

		/* enable interrupts */
		asm { sti; }
	}

	/* call the old timer ISR */
	((void interrupt far(*)(...))g_bc_timer)();
}

static void unused_gfx_spinlock(void)
{
	Bit32s v = g_gfx_spinlock;

	while (v == g_gfx_spinlock) { ; }
}
#endif

void update_status_bars(void)
{
	signed short i;
	struct struct_hero *hero;

	g_unused_spinlock_flag = 0;

	if (g_update_statusline) {

		if (g_pp20_index == ARCHIVE_FILE_ZUSTA_UK) {
			/* in the status menu */

			hero = get_hero(g_status_page_hero);

			/* adjust hunger to 100% */
			if (hero->hunger >= 100) {
				hero->hunger = g_status_page_hunger = 100;
			}

			/* adjust thirst to 100% */
			if (hero->thirst >= 100) {
				hero->thirst = g_status_page_thirst = 100;
			}

			/* hunger and thirst are at 100% */
			if ((g_status_page_hunger == 100) && (g_status_page_thirst == 100)) {
				g_status_page_hunger_max_counter = g_status_page_thirst_max_counter;
				g_status_page_hunger_max_color = g_status_page_thirst_max_color;
			}

#if !defined(__BORLANDC__)
			//CPU_CLI();
#else
			asm { cli };
#endif

			if (g_status_page_hunger == 100) {

				if (g_status_page_hunger_max_counter++ == 25) {

					g_status_page_hunger_max_color ^= 1;

					call_mouse_bg();

					for (i = 0; i < 6; i++) {
						do_h_line(g_vga_memstart, 260, 310, i + 36, g_status_page_hunger_max_color ? 9 : 10);
					}

					call_mouse();

					g_status_page_hunger_max_counter = 0;
				}

			} else if (hero->hunger != g_status_page_hunger) {

				g_status_page_hunger = hero->hunger;

				call_mouse_bg();

				for (i = 0; i < 6; i++) {
						do_h_line(g_vga_memstart, 260, g_status_page_hunger / 2 + 260, i + 36, 9);
						do_h_line(g_vga_memstart, g_status_page_hunger / 2 + 260, 310, i + 36, 10);
				}

				call_mouse();
			}

			if (g_status_page_thirst == 100) {

				if (g_status_page_thirst_max_counter++ == 25) {

					g_status_page_thirst_max_color ^= 1;

					call_mouse_bg();

					for (i = 0; i < 6; i++) {
						do_h_line(g_vga_memstart, 260, 310, i + 43, g_status_page_thirst_max_color ? 11 : 12);
					}

					call_mouse();

					g_status_page_thirst_max_counter = 0;
				}

			} else if (hero->thirst != g_status_page_thirst) {

				g_status_page_thirst = hero->thirst;

				call_mouse_bg();

				for (i = 0; i < 6; i++) {
					do_h_line(g_vga_memstart, 260, g_status_page_thirst / 2 + 260, i + 43, 11);
					do_h_line(g_vga_memstart, g_status_page_thirst / 2 + 260, 310, i + 43, 12);
				}

				call_mouse();
			}

#if !defined(__BORLANDC__)
			//CPU_STI();
#else
			asm { sti };
#endif
		} else if (g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {

			/* in the screen with the playmask */

			for (i = 0; i <= 6; i++) {

				if (get_hero(i)->typus != HERO_TYPE_NONE) {

					hero = get_hero(i);

					/* draw LE bars */
					if ((g_char_status_bars[i][1] != hero->le) ||
						(g_char_status_bars[i][0] != hero->le_max))
					{
						draw_bar(0, i, hero->le, hero->le_max, 0);
						g_char_status_bars[i][0] = hero->le_max;
						g_char_status_bars[i][1] = hero->le;
					}

					/* draw AE bars */
					if ((g_char_status_bars[i][3] != hero->ae) ||
						(g_char_status_bars[i][2] != hero->ae_max))
					{
						draw_bar(1, i, hero->ae, hero->ae_max, 0);
						g_char_status_bars[i][2] = hero->ae_max;
						g_char_status_bars[i][3] = hero->ae;
					}
				} else {
					if (g_char_status_bars[i][0]) {
						draw_bar(0, i, 0, 0, 0);
						g_char_status_bars[i][0] = g_char_status_bars[i][1] = 0;
					}

					if (g_char_status_bars[i][2]) {
						draw_bar(1, i, 0, 0, 0);
						g_char_status_bars[i][2] = g_char_status_bars[i][3] = 0;
					}
				}
			}
		}
	}
}

/**
 * \brief   draws a bar to visualize LE or AE
 *
 * \param   type        type of bar, 0 = LE / 1 = AE
 * \param   hero        number of hero when mode is zero
 * \param   pts_cur     current points
 * \param   pts_max     maximum points
 * \param   mode        0 on game mask, 1 in fight
 */
/* Remark:
	It should be used, either hero or mode is zero,
	since in fight mode only the active hero is shown.
 */
void draw_bar(unsigned short type, signed short hero, signed short pts_cur, signed short pts_max, signed short mode)
{
	signed short i;
	signed short y_min;
	signed short x;
	signed short lost;
	Bit8u* dst;

	if (mode == 0)
		call_mouse_bg();

	if (mode == 0) {
		x = g_hero_pic_posx[hero] + type * 4 + 34;
		y_min = 188;
		dst = g_vga_memstart;
	} else {
		x = type * 4 + 36;
		y_min = 42;
		dst = (Bit8u*)g_renderbuf_ptr;
	}

	if (pts_cur == 0) {
		/* draw 4 black vertical lines */
		for (i = 0; i < 3; i++) {
			do_v_line(dst, x + i, y_min - 30, y_min, 0);
		}
	} else {
		if (pts_cur == pts_max) {
			/* draw 4 full lines in the color of the type */
			for (i = 0; i < 3; i++) {
				do_v_line(dst, x + i, y_min - 30, y_min, g_status_bar_colors[type]);
			}
		} else {
			lost = 30;
			lost *= pts_cur;
			lost /= pts_max;

			if (lost == 0) {
				lost = 1;
			}

			/* draw visible part */
			for (i = 0; i < 3; i++) {
				do_v_line(dst, x + i, y_min - lost, y_min, g_status_bar_colors[type]);
			}

			/* draw black part */
			for (i = 0; i < 3; i++) {
				do_v_line(dst, x + i, y_min - 30, y_min - lost - 1, 0);
			}
		}
	}

	if (mode == 0) {
		call_mouse();
	}
}

void restore_rect(Bit8u *dst, Bit8u *src, unsigned short x, unsigned short y, signed char n, signed char m)
{
	signed short i;
	signed short j;
	signed char c;
	Bit8u* p;

	call_mouse_bg();

	p = dst;
	p += y * 320 + x;

	for (i = 0; i < m; p += 320, i++) {
		for (j = 0; j < n; j++) {
			c = *src++;
			if (c)
				*(p + j) = c;
		}
	}

	call_mouse();
}

void restore_rect_rle(Bit8u *dst, Bit8u *src, unsigned short x, unsigned short y, signed char width, signed char height, unsigned short v1)
{
	signed short si;
	signed short di;
	signed short i;
	signed char c;
	unsigned char cnt;
	signed char tmp;
	Bit8u *p = dst;

	p += y * 320 + x;
	call_mouse_bg();

	for (i = 0; i < height; p += 320, i++) {

		si = 0;
		while (si < width) {
			if ((c = *src++) == 0x7f) {
				cnt = *src++;
				tmp = *src++;
				if (tmp || v1 != 2)
					for (di = 0; di < cnt; di++)
						*(p + si + di) = tmp;
				si += cnt;
				continue;
			}
			if (c || v1 != 2)
				*(p + si) = c;
			si++;
		}
	}

	call_mouse();
}

void mouse_cursor_draw(void)
{
	register signed short mask;
	signed short x;
	signed char i;
	signed char j;
	Bit8u *dst;
	unsigned short *mouse_cursor;
	signed short y;
	signed short width;
	signed short height;

	dst = g_vga_memstart;
	mouse_cursor = &g_current_cursor->mask[0];

	x = g_mouse_posx - g_mouse_pointer_offsetx;
	y = g_mouse_posy - g_mouse_pointer_offsety;

	width = height = 16;

	if (x > 304)
		width = 320 - x;

	if (y > 184)
		height = 200 - y;

	dst += y * 320 + x;

	for (i = 0; i < height; dst += 320, i++) {

		mask = *(mouse_cursor++);

		for (j = 0; j < width; j++)
			if ((0x8000 >> j) & mask)
				*(dst + j) = 0xff;
	}
}

void mouse_save_bg(void)
{
	Bit8u *src;
	signed short realpos_x;
	signed short realpos_y;
	signed short realwidth;
	signed short realheight;
	signed short delta_y;
	signed short delta_x;

	src = g_vga_memstart;

	realpos_x = g_mouse_posx - g_mouse_pointer_offsetx;
	realpos_y = g_mouse_posy - g_mouse_pointer_offsety;

	realwidth = realheight = 16;

	if (realpos_x > 304) {
		realwidth = 320 - realpos_x;
	}

	if (realpos_y > 184) {
		realheight = 200 - realpos_y;
	}

	src += realpos_y * 320 + realpos_x;

	for (delta_y = 0; delta_y < realheight; src += 320, delta_y++) {
		for (delta_x = 0; delta_x < realwidth; delta_x++) {
			g_mouse_bg_bak[delta_y * 16 + delta_x] = *(src + delta_x);
		}
	}
}

void restore_mouse_bg(void)
{
	signed short delta_y;
	signed short realpos_x;
	signed short delta_x;

	Bit8u *dst;
	signed short realpos_y;
	signed short realwidth;
	signed short realheight;


	/* gfx memory */
	dst = g_vga_memstart;
	realpos_x = g_mouse_posx_bak - g_mouse_pointer_offsetx_bak;
	realpos_y = g_mouse_posy_bak - g_mouse_pointer_offsety_bak;
	realwidth = realheight = 16;

	if (realpos_x > 320 - 16)
		realwidth = 320 - realpos_x;

	if (realpos_y > 200 - 16)
		realheight = 200 - realpos_y;

	dst += realpos_y * 320 + realpos_x;

	for (delta_y = 0; delta_y < realheight; dst += 320, delta_y++)
		for (delta_x = 0; delta_x < realwidth; delta_x++)
			*(dst + delta_x) = g_mouse_bg_bak[16 * delta_y + delta_x];
}

void load_wallclock_nvf(void)
{
	struct nvf_desc nvf;
	signed short handle; /* REMARK: reused differently */

	handle = load_archive_file(ARCHIVE_FILE_OBJECTS_NVF);
	read_archive_file(handle, g_renderbuf_ptr, 2000);
	close(handle);

	nvf.src = g_renderbuf_ptr;
	nvf.type = 0;
	nvf.width = &handle;
	nvf.height = &handle;

	/* sky background */
	nvf.dst = g_objects_nvf_buf;
	nvf.no = 12;
	process_nvf(&nvf);

	/* mountains */
	nvf.dst = g_objects_nvf_buf + 0x683;
	nvf.no = 13;
	process_nvf(&nvf);

	/* sun */
	nvf.dst = g_objects_nvf_buf + 0xcaf;
	nvf.no = 14;
	process_nvf(&nvf);

	/* moon */
	nvf.dst = g_objects_nvf_buf + 0xcef;
	nvf.no = 15;
	process_nvf(&nvf);

	/* shift palette by 0xe0 */
	array_add(g_objects_nvf_buf, 0xd3f, 0xe0, 2);
}

void update_wallclock(void)
{
	signed short night;
	Bit32s d;

	if ((g_wallclock_update) && ((g_pp20_index == ARCHIVE_FILE_PLAYM_UK) || (g_pp20_index == ARCHIVE_FILE_KARTE_DAT)) && !g_dialogbox_lock) {

		if ((gs_day_timer >= HOURS(7)) && (gs_day_timer <= HOURS(19))) {
			/* day */
			d = gs_day_timer - HOURS(7);
		} else {
			if (gs_day_timer < HOURS(7)) {
				/* morning */
				d = gs_day_timer + HOURS(5);
			} else {
				/* evening */
				d = gs_day_timer + HOURS(-19);
			}
		}

		if (((d / 771L) != g_wallclock_pos) || g_wallclock_redraw) {

			g_wallclock_redraw = 0;
			night = ((gs_day_timer >= HOURS(7)) && (gs_day_timer <= HOURS(19))) ? 0 : 1;
			draw_wallclock((signed short)(d / 771), night);
			g_wallclock_pos = d / 771;
		}
	}
}

/**
 * \brief   draws the clock in day- or nighttime
 *
 * \param   pos         position of the sun/moon
 * \param   night       0 = day / 1 = night
 *
 */
void draw_wallclock(signed short pos, signed short night)
{
	signed short y;
	signed short mouse_updated;
	struct struct_rect rect_bak;
	struct struct_pic_copy pic_copy_bak;

	mouse_updated = 0;

	/* make backups */
	pic_copy_bak = g_pic_copy;
	rect_bak = g_pic_copy_rect;

	/* set pointer */
	g_pic_copy.dst = g_vga_memstart;

	/* calculate y value */
	/* Original-Bug: off-by-one with pos > 80 */
	y = g_wallclock_y + g_wallclock_pos_y[pos];

	/* calculate x value */
	pos += g_wallclock_x - 2;

	/* set window */
	g_pic_copy_rect.y1 = g_wallclock_y;
	g_pic_copy_rect.x1 = g_wallclock_x;
	g_pic_copy_rect.y2 = g_wallclock_y + 22;
	g_pic_copy_rect.x2 = g_wallclock_x + 78;

	/* set palette (night/day) */
#if !defined(__BORLANDC__)
	set_palette((Bit8u*)(!night ? &g_wallclock_palette_day : &g_wallclock_palette_night), 0xfa, 3);
#else
	set_palette((Bit8u*)MK_FP(_DS, (!night ? FP_OFF(g_wallclock_palette_day) : FP_OFF(g_wallclock_palette_night))), 0xfa, 3);
#endif

	/* check if mouse is in that window */
	if (is_mouse_in_rect(g_wallclock_x - 6,
				g_wallclock_y - 6,
				g_wallclock_x + 85,
				g_wallclock_y + 28)) {

			call_mouse_bg();
			mouse_updated = 1;
	}

	/* set coordinates */
	g_pic_copy.x1 = g_wallclock_x;
	g_pic_copy.y1 = g_wallclock_y;
	g_pic_copy.x2 = g_wallclock_x + 78;
	g_pic_copy.y2 = g_wallclock_y + 20;
	g_pic_copy.src = g_objects_nvf_buf;

	/* draw backgroud */
	do_pic_copy(2);

	/* set coordinates */
	g_pic_copy.x1 = pos;
	g_pic_copy.y1 = y;
	g_pic_copy.x2 = pos + 7;
	g_pic_copy.y2 = y + 6;
	g_pic_copy.src = (!night ? (g_objects_nvf_buf + 0xcaf) : (g_objects_nvf_buf + 0xcef));

	/* draw sun/moon */
	do_pic_copy(2);


	/* set coordinates */
	g_pic_copy.x1 = g_wallclock_x;
	g_pic_copy.y1 = g_wallclock_y + 3;
	g_pic_copy.x2 = g_wallclock_x + 78;
	g_pic_copy.y2 = g_wallclock_y + 22;
	g_pic_copy.src = g_objects_nvf_buf + 0x683;

	/* draw backgroud */
	do_pic_copy(2);

	/* restore structure */
	g_pic_copy_rect = rect_bak;

	/* happens in travel mode */
	if (g_pp20_index == ARCHIVE_FILE_KARTE_DAT) {

		/* set coordinates */
		g_pic_copy.x1 = g_wallclock_x - 5;
		g_pic_copy.y1 = g_wallclock_y - 4;
		g_pic_copy.x2 = g_wallclock_x + 85;
		g_pic_copy.y2 = g_wallclock_y + 28;
		g_pic_copy.src = g_buffer9_ptr + 0x4650L;

		/* draw backgroud */
		do_pic_copy(2);
	}

	if (mouse_updated != 0) {
		call_mouse();
	}

	/* restore gfx */
	g_pic_copy = pic_copy_bak;
}

/**
 * \brief   adds op to each element of an array
 *
 * \param   dst         pointer to array
 * \param   len         length of array
 * \param   op          operator
 * \param   flag        if 2, op will not be added if array element is 0
 */
void array_add(Bit8u *dst, signed short len, signed short op, signed short flag)
{
	signed int i;

	for (i = 0; i < len; i++) {

		if (flag == 2) {
			if (dst[i] != 0) {
				dst[i] += op;
			}
		} else {
			dst[i] += op;
		}
	}
}

void schick_set_video(void)
{
	struct Bit8u_3 pal_white = g_color_pal_white;;

	set_video_mode(0x13);
	set_color((Bit8u*)&pal_white, 0xff);
}

void schick_reset_video(void)
{
#if defined(__BORLANDC__)
	set_video_mode(g_video_mode_bak);
	set_video_page(g_video_page_bak);
#endif
}

void clear_ani_pal(void)
{
	struct Bit8u_32_3 pal = g_palette_allblack;

	wait_for_vsync();

	/* REMARK: memset would be better */
	set_palette((Bit8u*)&pal, 0, 0x20);
}

void set_ani_pal(Bit8u *pal)
{
	wait_for_vsync();
	set_palette(pal, 0, 0x20);
}

#if defined(__BORLANDC__)
/* a bunch of EGA routines */

void unused_ega1(unsigned char a)
{
	outportb(0x3ce, 5);
	outportb(0x3cf, a);
}

void unused_ega2(unsigned char a)
{
	outportb(0x3c4, 2);
	outportb(0x3c5, a);
}

void unused_ega3(unsigned char a)
{
	outportb(0x3ce, 0);
	outportb(0x3cf, a);
}

void unused_ega4(unsigned char a)
{
	outportb(0x3ce, 1);
	outportb(0x3cf, a);
}

void unused_ega5(unsigned char a)
{
	outportb(0x3ce, 4);
	outportb(0x3cf, a);
}

void unused_ega6(unsigned char a)
{
	outportb(0x3ce, 8);
	outportb(0x3cf, a);
}

#endif

void do_h_line(Bit8u* ptr, signed short x1, signed short x2, signed short y, signed char color)
{
	signed short tmp;
	signed short count;
	Bit8u* dst;

	if (x1 == x2)
		return;

	if (x1 > x2) {
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}

	count = x2 - x1 + 1;
	dst = y * 320 + x1 + ptr;

	draw_h_line(dst, count, color);
}

void do_v_line(Bit8u* ptr, signed short y, signed short x1, signed short x2, signed char color)
{
	signed short tmp;
	signed short count;
	Bit8u* dst;

	if (x1 == x2)
		return;

	if (x1 > x2) {
		tmp = x2;
		x2 = x1;
		x1 = tmp;
	}

	count = x2 - x1 + 1;
	dst = x1 * 320 + y + ptr;

	draw_h_spaced_dots(dst, count, color, 320);
}

void do_border(Bit8u* dst, signed short x1, signed short y1, signed short x2, signed short y2, signed char color)
{
	call_mouse_bg();
	do_h_line(dst, x1, x2, y1, color);
	do_h_line(dst, x1, x2, y2, color);
	do_v_line(dst, x1, y1, y2, color);
	do_v_line(dst, x2, y1, y2, color);
	call_mouse();
}

void do_pic_copy(unsigned short mode)
{
	short x1, y1;
	short x2, y2;
	short v1, v2, v3, v4;
	short width, height;
	Bit8u *src;
	Bit8u* dst;

	x1 = g_pic_copy.x1;
	y1 = g_pic_copy.y1;
	x2 = g_pic_copy.x2;
	y2 = g_pic_copy.y2;

	v1 = g_pic_copy.v1;
	v2 = g_pic_copy.v2;
	v3 = g_pic_copy.v3;
	v4 = g_pic_copy.v4;

	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	src = g_pic_copy.src;
	dst = g_pic_copy.dst;

	pic_copy(dst, x1, y1, x2, y2, v1, v2, v3, v4, width, height, src, mode);
}

void do_save_rect(void)
{
	signed short x1,y1;
	signed short width,height;
	signed short x2,y2;
	Bit8u* src;
	Bit8u* dst;

	x1 = g_pic_copy.x1;
	y1 = g_pic_copy.y1;

	x2 = g_pic_copy.x2;
	y2 = g_pic_copy.y2;

	src = g_pic_copy.src;
	dst = g_pic_copy.dst;

	dst += y1 * 320 + x1;
	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

#if defined(__BORLANDC__)
	save_rect(FP_SEG(dst), FP_OFF(dst), src, width, height);
#else
	save_rect(dst, src, width, height);
#endif
}

void do_fill_rect(Bit8u* dst, signed short x, signed short y, signed short w, signed short h, signed short color)
{
	signed short width, height;

	width = w - x + 1;
	height = h - y + 1;

	dst += y * 320 + x;

	call_mouse_bg();
#if defined(__BORLANDC__)
	fill_rect(FP_SEG(dst), FP_OFF(dst), color, width, height);
#else
	fill_rect(dst, color, width, height);
#endif
	call_mouse();
}

void wait_for_vsync(void)
{
#if defined(__BORLANDC__)
	signed short tmp;

	outportb(0x3d4, 0x11);

	tmp = inportb(0x3d5);
	tmp &= 0xffdf;

	outportb(0x3d4, 0x11);
	outportb(0x3d5, tmp);

	do {
		tmp = inportb(0x3da);

	} while ((tmp & 0x8) || (tmp & 0x1));

	do {
		tmp = inportb(0x3da);

	} while (!(tmp & 0x8) || !(tmp & 0x1));
#else
	//SDL_Delay(16);
#endif
}

/**
 * \brief   the snow effect for the map screen
 *
 * \param   src         pointer to a picture
 */
void map_effect(Bit8u *src)
{
	unsigned short si;
	unsigned short i;
	signed short seed;
	signed short wallclock_update_bak;

	seed = 0;

	wallclock_update_bak = g_wallclock_update;
	g_wallclock_update = 0;

	wait_for_vsync();

	call_mouse_bg();

	for (i = 0; i < 64000; i++) {

		do {
			si = (seed * 17 + 87) & 0xffff;

			seed = si;

		} while (si >= 64000);


		*(g_vga_memstart + si) = *(src + si);

#ifdef M302de_SPEEDFIX
		/* this too fast,  we slow it down a bit */
		if ((i & 0x1ff) == 0x1ff)
			wait_for_vsync();
#endif
	}

	call_mouse();

	g_wallclock_update = wallclock_update_bak;
}

#if !defined(__BORLANDC__)
}
#endif
