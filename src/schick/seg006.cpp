/*
 *      Rewrite of DSA1 v3.02_de functions of seg006 (Fight)
 *      Functions rewritten: 16/16 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg006.cpp
 */

#include <stdlib.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg006.h"
#include "seg027.h"
#include "seg096.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   get the pointer to the fighter with id fighter_id
 *
 * \param   fighter_id  id of the fighter
 * \return              a pointer to the fighter with id fighter_id
 */
Bit8u* FIG_get_ptr(signed char fighter_id)
{
	Bit8u* fighter_ptr = (Bit8u*)ds_readd(FIG_LIST_HEAD);

	while (host_readbs((Bit8u*)(fighter_ptr) + FIGHTER_ID) != fighter_id) {

		/* check if its the last element */
		if (host_readd((Bit8u*)(fighter_ptr) + FIGHTER_NEXT) == 0) {
			/* return list head */
			return (Bit8u*)ds_readd(FIG_LIST_HEAD);
		}

		/* set fighter_ptr to the next element */
		fighter_ptr = (Bit8u*)host_readd((Bit8u*)(fighter_ptr) + FIGHTER_NEXT);
	}

	return fighter_ptr;
}

/* static */
signed char FIG_set_array(void)
{
	signed char i = 0;

	/* find first element that is zero */
	while (ds_readb(FIG_LIST_ARRAY + i) != 0) {

		i++;
	}

	/* make it 1 */
	ds_writeb(FIG_LIST_ARRAY + i, 1);

	/* return the number of the index */
	return i;
}

void FIG_draw_figures(void)
{
	signed short l1, l2;
	Bit8u *list_i;
	struct screen_rect screen_mode;
	Bit8u* gfx_dst_bak;
	signed short l_si, l_di;

	l1 = 10;
	l2 = 118;

	gfx_dst_bak = (Bit8u*)ds_readd(PIC_COPY_DST);
	ds_writed(PIC_COPY_DST, (Bit32u)g_renderbuf_ptr);

	/* backup a structure */
	screen_mode = *((struct screen_rect*)(p_datseg + PIC_COPY_DS_RECT));

	list_i = (Bit8u*)ds_readd(FIG_LIST_HEAD);

	do {

		if (host_readb(list_i + FIGHTER_VISIBLE) == 1) {
			l_si =	(l1 - host_readbs(list_i + 8) / 2) +
				(host_readbs(list_i + 3) + host_readbs(list_i + 4)) * 10;

			l_di =	(l2 - host_readbs(list_i + 7)) +
				(host_readbs(list_i + 3) - host_readbs(list_i + 4)) * 5;

			l_si += host_readbs(list_i + 5);
			l_di += host_readbs(list_i + 6);

			ds_writew(PIC_COPY_X1, l_si);
			ds_writew(PIC_COPY_Y1, l_di);
			ds_writew(PIC_COPY_X2, l_si + host_readbs(list_i + 8) - 1);
			ds_writew(PIC_COPY_Y2, l_di + host_readbs(list_i + 7) - 1);
			/* set gfx_src */
			ds_writed(PIC_COPY_SRC, host_readd(list_i + FIGHTER_GFXBUF));

			ds_writew(PIC_COPY_DS_RECT,
				l_di + host_readbs(list_i + FIGHTER_Y1));
			if (ds_readws(PIC_COPY_DS_RECT) < 0)
				ds_writew(PIC_COPY_DS_RECT, 0);

			ds_writew((PIC_COPY_DS_RECT + 2),
				l_si + host_readbs(list_i + 9));
			if (ds_readws((PIC_COPY_DS_RECT + 2)) < 0)
				ds_writew((PIC_COPY_DS_RECT + 2), 0);

			ds_writew((PIC_COPY_DS_RECT + 4),
				l_di + host_readbs(list_i + FIGHTER_Y2));
			if (ds_readws((PIC_COPY_DS_RECT + 4)) > 199)
				ds_writew((PIC_COPY_DS_RECT + 4), 199);

			ds_writew((PIC_COPY_DS_RECT + 6),
				l_si + host_readbs(list_i + FIGHTER_X2));
			if (ds_readws((PIC_COPY_DS_RECT + 6)) > 319)
				ds_writew((PIC_COPY_DS_RECT + 6), 319);

			do_pic_copy(2);
		}

	} while (list_i = (Bit8u*)(Bit8u*)((Bit8u*)host_readd(list_i + FIGHTER_NEXT)));

	/* restore a structure */
	//struct_copy(p_datseg + PIC_COPY_DS_RECT, screen_mode, 8);
	*((struct screen_rect*)(p_datseg + PIC_COPY_DS_RECT)) = screen_mode;
	ds_writed(PIC_COPY_DST, (Bit32u)gfx_dst_bak);
}

void FIG_set_gfx(void)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)ds_readd(PIC_COPY_DST);

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 199);
	ds_writed(PIC_COPY_SRC, (Bit32u)g_renderbuf_ptr);
	ds_writed(PIC_COPY_DST, ds_readd(VGA_MEMSTART));
	update_mouse_cursor();
	do_pic_copy(0);
	refresh_screen_size();

	ds_writed(PIC_COPY_DST, (Bit32u)ptr_bak);
}

void FIG_call_draw_pic(void)
{
	FIG_draw_pic();
}

void FIG_draw_pic(void)
{
	memcpy((void*)g_renderbuf_ptr, (void*)g_buffer8_ptr, 64000);

#if defined(__BORLANDC__)
	g_always_one = 1;
#endif

	if (g_fig_char_pic) {
		FIG_draw_char_pic(0, g_fig_char_pic);
	} else if (g_fig_enemy_pic) {
		FIG_draw_enemy_pic(0, g_fig_enemy_pic);
	}
}

Bit8u* FIG_get_hero_ptr(signed short v1)
{
	signed short i;

	for (i = 0; i <= 6; i++) {
		if (host_readbs(get_hero(i) + HERO_FIGHTER_ID) == v1)
			return get_hero(i);
	}

	return get_hero(0);
}

Bit8u* FIG_get_enemy_sheet(signed short fighter_id)
{
	signed short i;

	for (i = 0; i < 20; i++) {
		if (fighter_id == ds_readbs(ENEMY_SHEETS + ENEMY_SHEET_FIGHTER_ID + (i * SIZEOF_ENEMY_SHEET)))
#if !defined(__BORLANDC__)
			return ((Bit8u*)p_datseg + ENEMY_SHEETS + i * SIZEOF_ENEMY_SHEET);
#else
			return (Bit8u*)&(((struct enemy_sheets*)(p_datseg + ENEMY_SHEETS))[i]);
#endif
	}

	return 0;
}

void FIG_set_sheet(signed char fighter_id, signed char val)
{
	Bit8u *ptr = (Bit8u*)ds_readd(FIG_LIST_HEAD);

	while (host_readbs(ptr + FIGHTER_ID) != fighter_id) {

		/* check for end of list */
		if (host_readd(ptr + FIGHTER_NEXT) == 0) {
			return;
		}

		/* set ptr to ptr->next */
		ptr = (Bit8u*)(host_readd(ptr + FIGHTER_NEXT));
	}

	host_writeb(ptr + FIGHTER_SHEET, val);

	/* set presence flag */
	host_writeb(ptr + FIGHTER_VISIBLE, 1);
}

/**
 * \brief   hides an element from the chessboard without removing it
 *
 * \param   fighter_id  identificates the element to hide
 */
void FIG_make_invisible(signed char fighter_id)
{
	Bit8u *ptr1, *ptr2;

	ptr1 = (Bit8u*)ds_readd(FIG_LIST_HEAD);

	while (host_readb(ptr1 + FIGHTER_ID) != fighter_id) {

		if (host_readd(ptr1 + FIGHTER_NEXT) == 0) {
			return;
		}

		ptr1 = (Bit8u*)(host_readd(ptr1 + FIGHTER_NEXT));
	}

	host_writeb(ptr1 + FIGHTER_VISIBLE, 0);

	if (host_readbs(ptr1 + FIGHTER_TWOFIELDED) != -1) {

		ptr2 = (Bit8u*)ds_readd(FIG_LIST_HEAD);

		while (ds_readb(FIG_TWOFIELDED_TABLE + host_readbs(ptr1 + FIGHTER_TWOFIELDED)) != host_readb(ptr2 + FIGHTER_ID)) {
			ptr2 = (Bit8u*)(host_readd(ptr2 + FIGHTER_NEXT));
		}
		host_writeb(ptr2 + FIGHTER_VISIBLE, 0);
	}
}

/**
 * \brief   unhides an element from the chessboard
 *
 * \param   fighter_id  identificates the element to unhide
 */
void FIG_make_visible(signed short fighter_id)
{
	Bit8u *ptr1, *ptr2;

	ptr1 = (Bit8u*)ds_readd(FIG_LIST_HEAD);

	while (host_readb(ptr1 + FIGHTER_ID) != (signed char)fighter_id) {

		if (host_readd(ptr1 + FIGHTER_NEXT) == 0){
			return;
		}

		ptr1 = (Bit8u*)(host_readd(ptr1 + FIGHTER_NEXT));
	}

	host_writeb(ptr1 + FIGHTER_VISIBLE, 1);

	if (host_readbs(ptr1 + FIGHTER_TWOFIELDED) != -1) {

		ptr2 = (Bit8u*)ds_readd(FIG_LIST_HEAD);

		while (ds_readb(FIG_TWOFIELDED_TABLE + host_readbs(ptr1 + FIGHTER_TWOFIELDED)) != host_readb(ptr2 + FIGHTER_ID)) {

			ptr2 = (Bit8u*)(host_readd(ptr2 + FIGHTER_NEXT));

		}

		host_writeb(ptr2 + FIGHTER_VISIBLE, 1);
	}
}

void FIG_set_weapon_sheet(signed char fighter_id, signed char val)
{
	Bit8u *ptr = (Bit8u*)ds_readd(FIG_LIST_HEAD);

	while (host_readb(ptr + FIGHTER_ID) != fighter_id) {

		if (host_readd(ptr + FIGHTER_NEXT) == 0) {
			return;
		}

		ptr = (Bit8u*)(host_readd(ptr + FIGHTER_NEXT));
	}

	host_writeb(ptr + FIGHTER_WSHEET, val);
}

struct dummy {
	char a[35];
};

/**
 * \brief   removes an element from the FIG_LIST
 *
 * \param   fighter_id  the element to remove
 * \param   keep_in_memory whether to save the removed element in FIG_LIST_ELEM, useful for moving element to end of list
 */
void FIG_remove_from_list(signed char fighter_id, signed char keep_in_memory)
{
	Bit8u* p = (Bit8u*)ds_readd(FIG_LIST_HEAD);

	/* NULL check */
	if (!p)	return;

	while (host_readb(p + FIGHTER_ID) != fighter_id) {

		/* if (ptr->next == NULL); */
		if (host_readd(p + FIGHTER_NEXT) == 0) {
			return;
		}

		/* ptr = ptr->next; */
		p = (Bit8u*)(host_readd(p + FIGHTER_NEXT));
	}

	if (!keep_in_memory) {
		ds_writeb(FIG_LIST_ARRAY + fighter_id, 0);
	} else {
//		struct_copy(p_datseg + FIG_LIST_ELEM, p, SIZEOF_FIGHTER);
		*((struct dummy*)(p_datseg + FIG_LIST_ELEM)) = *((struct dummy*)(p));
	}

	/* check if p == HEAD */
	if (p == (Bit8u*)ds_readd(FIG_LIST_HEAD)) {
		/* Set HEAD: head = p->next;*/
		ds_writed(FIG_LIST_HEAD, host_readd(p + FIGHTER_NEXT));
		if (ds_readd(FIG_LIST_HEAD) != 0)
			/* head->prev = NULL */
			host_writed((Bit8u*)ds_readd(FIG_LIST_HEAD) + FIGHTER_PREV, 0);
	} else {
		/* check if p == tail */
		if (host_readd(p + FIGHTER_NEXT) == 0) {
			/* p->prev->next == NULL */
			host_writed((Bit8u*)(host_readd(p + FIGHTER_PREV)) + FIGHTER_NEXT , 0);
		} else {
			/* remove ptr from list
			p->prev->next = p->next;
			p->next->prev = p->prev; */
			host_writed((Bit8u*)(host_readd(p + FIGHTER_PREV)) + FIGHTER_NEXT, host_readd(p + FIGHTER_NEXT));
			host_writed((Bit8u*)(host_readd(p + FIGHTER_NEXT)) + FIGHTER_PREV, host_readd(p + FIGHTER_PREV));
		}
	}

	memset(p, 0, SIZEOF_FIGHTER);

	host_writeb(p + FIGHTER_ID, -1);
}

/**
 * \brief   adds FIG_LIST_ELEM to FIG_LIST
 *
 * \param   fighter_id  id to assign to the new element (-1 = assign a new id)
 * \return              the new element's fighter_id (position in FIG_LIST_ARRAY)
 */
signed char FIG_add_to_list(signed char fighter_id)
{
	Bit8u* p1;
	Bit8u* p2;
	signed short x, y;

	p1 = (Bit8u*)ds_readd(FIG_LIST_BUFFER);
	x = ds_readbs((FIG_LIST_ELEM+FIGHTER_CBX));
	y = ds_readbs((FIG_LIST_ELEM+FIGHTER_CBY));

	/* FIG_list_start == NULL */
	if (ds_readd(FIG_LIST_HEAD) == 0) {

		ds_writed(FIG_LIST_HEAD, ds_readd(FIG_LIST_BUFFER));

//		struct_copy((Bit8u*)ds_readd(FIG_LIST_HEAD), p_datseg + FIG_LIST_ELEM, SIZEOF_FIGHTER);
		*((struct dummy*)((Bit8u*)ds_readd(FIG_LIST_HEAD))) = *((struct dummy*)(p_datseg + FIG_LIST_ELEM));

		if (fighter_id == -1) {
			host_writeb((Bit8u*)ds_readd(FIG_LIST_HEAD) + FIGHTER_ID,
				FIG_set_array());
		}

		host_writed((Bit8u*)ds_readd(FIG_LIST_HEAD) + FIGHTER_PREV, 0);
		host_writed((Bit8u*)ds_readd(FIG_LIST_HEAD) + FIGHTER_NEXT, 0);

#if !defined(__BORLANDC__)
		D1_LOG("\tlist created x = %d, y = %d\n", x, y);
#endif

		return host_readbs((Bit8u*)ds_readd(FIG_LIST_HEAD) + FIGHTER_ID);
	}

	while (host_readbs((Bit8u*)(p1) + FIGHTER_ID) != -1) {
		p1 += SIZEOF_FIGHTER;
	}

//	struct_copy((Bit8u*)(p1), p_datseg + FIG_LIST_ELEM, SIZEOF_FIGHTER);
	*((struct dummy*)((Bit8u*)(p1))) =	*((struct dummy*)(p_datseg + FIG_LIST_ELEM));

	if (fighter_id == -1) {
		host_writeb((Bit8u*)(p1) + FIGHTER_ID, FIG_set_array());
	} else {
		host_writeb((Bit8u*)(p1) + FIGHTER_ID, fighter_id);
	}

	p2 = (Bit8u*)ds_readd(FIG_LIST_HEAD);

	/* The list is filled in the order of rendering, i.e. from rear to front:
	 * (x1,y1) is rendered before (x2,y2) if (x1 < x2) || (x1 == x2 && y1 > y2)
	 * On the same chessboard field, lower z is rendered before larger z.
	 */
	if (ds_readbs((FIG_LIST_ELEM + FIGHTER_Z)) != -1) {
		while ((host_readbs((Bit8u*)(p2) + FIGHTER_CBX) <= x) &&
		(host_readbs((Bit8u*)(p2) + FIGHTER_CBX) != x ||
		host_readbs((Bit8u*)(p2) + FIGHTER_CBY) >= y) &&
		(host_readbs((Bit8u*)(p2) + FIGHTER_CBX) != x ||
		host_readbs((Bit8u*)(p2) + FIGHTER_CBY) != y ||
		host_readbs((Bit8u*)(p2) + FIGHTER_Z) <= ds_readbs((FIG_LIST_ELEM + FIGHTER_Z))))
		{

			/* p2->next != NULL */
			if (host_readd((Bit8u*)(p2) + FIGHTER_NEXT) == 0) {

				/* append to end of the list */

				/* p2->next = p1 */
				host_writed((Bit8u*)(p2) + FIGHTER_NEXT, (Bit32u)p1);
				/* p1->prev = p2 */
				host_writed((Bit8u*)(p1) + FIGHTER_PREV, (Bit32u)p2);
				/* p1->next = NULL */
				host_writed((Bit8u*)(p1) + FIGHTER_NEXT, 0);
#if !defined(__BORLANDC__)
				D1_LOG("\tlist appended x = %d, y = %d\n", x, y);
#endif
				return host_readbs((Bit8u*)(p1) + FIGHTER_ID);
			}

			/* p2 = p2->next */
			p2 = (Bit8u*)host_readd((Bit8u*)(p2) + FIGHTER_NEXT);
		}
	}

	/* p1->prev = p2->prev; */
	host_writed((Bit8u*)(p1) + FIGHTER_PREV, host_readd((Bit8u*)(p2) + FIGHTER_PREV));

	/* if (p2->prev) */
	if (host_readd((Bit8u*)(p2) + FIGHTER_PREV) != 0)
		/* p2->prev->next = p1 */
		host_writed((Bit8u*)(host_readd((Bit8u*)(p2) + FIGHTER_PREV)) + FIGHTER_NEXT, (Bit32u)p1);
	else
		/* FIG_list_start = p1 */
		ds_writed(FIG_LIST_HEAD, (Bit32u)p1);

	/* p2->prev = p1 */
	host_writed((Bit8u*)(p2) + FIGHTER_PREV, (Bit32u)p1);
	/* p1->next = p2 */
	host_writed((Bit8u*)(p1) + FIGHTER_NEXT, (Bit32u)p2);
#if !defined(__BORLANDC__)
	D1_LOG("\tlist insert x = %d, y = %d\n", x, y);
#endif
	return host_readbs((Bit8u*)(p1) + FIGHTER_ID);
}

/**
 * \brief   draws the heroes picture to the fight screen
 *
 * \param   loc         0 = upper left, 1 = lower left
 * \param   hero_pos    position of the hero
 */
void FIG_draw_char_pic(signed short loc, signed short hero_pos)
{
	unsigned char *hero;
	signed short fg_bak, bg_bak;

	hero = get_hero(hero_pos - 1);
	ds_writed(PIC_COPY_SRC, (Bit32u)(hero + HERO_PORTRAIT));

	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	ds_writed(PIC_COPY_DST, (Bit32u)g_renderbuf_ptr);
	ds_writed(VGA_BACKBUFFER, (Bit32u)g_renderbuf_ptr);

	if (loc == 0) {

		do_border(g_renderbuf_ptr, 1, 9, 34, 42, 29);
		ds_writew(PIC_COPY_X1, 2);
		ds_writew(PIC_COPY_Y1, 10);
		ds_writew(PIC_COPY_X2, 33);
		ds_writew(PIC_COPY_Y2, 41);
		GUI_print_string((char*)hero + HERO_NAME2, 1, 1);

		draw_bar(0, 0, host_readw(hero + HERO_LE), host_readw(hero + HERO_LE_ORIG), 1);

		draw_bar(1, 0, host_readw(hero + HERO_AE), host_readw(hero + HERO_AE_ORIG), 1);
	} else {
		do_border(g_renderbuf_ptr, 1, 157, 34, 190, 29);
		ds_writew(PIC_COPY_X1, 2);
		ds_writew(PIC_COPY_Y1, 158);
		ds_writew(PIC_COPY_X2, 33);
		ds_writew(PIC_COPY_Y2, 189);
		GUI_print_string((char*)hero + HERO_NAME2, 1, 193);
	}

	do_pic_copy(0);
	ds_writed(PIC_COPY_DST, ds_readd(VGA_MEMSTART));
	ds_writed(VGA_BACKBUFFER, ds_readd(VGA_MEMSTART));
	set_textcolor(fg_bak, bg_bak);
}

/**
 * \brief   draws a picture of the monster, when on turn
 *
 * \param   loc         0 = left side, 1 = right side
 * \param   id          ID of the enemy
 */
void FIG_draw_enemy_pic(signed short loc, signed short id)
{
	signed short height_width;
	Bit8u *p_enemy;
	signed short fg_bak;
	signed short bg_bak;
	Bit8u* p1;
	struct nvf_desc nvf;

	p1 = (((HugePt)g_buffer8_ptr) - 1288L);

	p_enemy = p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + id * SIZEOF_ENEMY_SHEET;

	if (ds_readbs(GFXTAB_FIGURES_MAIN + host_readbs(p_enemy + ENEMY_SHEET_GFX_ID) * 5) != ds_readws(FIGHT_FIGS_INDEX)) {

		nvf.src = (Bit8u*)(load_fight_figs(ds_readbs(GFXTAB_FIGURES_MAIN + host_readbs(p_enemy + ENEMY_SHEET_GFX_ID) * 5)));
		nvf.dst = (Bit8u*)(p1);
		nvf.no = 1;
		nvf.type = 0;
		nvf.width = (Bit8u*)&height_width;
		nvf.height = (Bit8u*)&height_width;

		process_nvf(&nvf);

		ds_writew(FIGHT_FIGS_INDEX,
			ds_readbs(GFXTAB_FIGURES_MAIN + host_readbs(p_enemy + ENEMY_SHEET_GFX_ID) * 5));
	}

	/* save and set text colors */
	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	/* set gfx address */
	ds_writed(PIC_COPY_DST, (Bit32u)g_renderbuf_ptr);
	ds_writed(VGA_BACKBUFFER, (Bit32u)g_renderbuf_ptr);

	if (loc == 0) {
		do_border(g_renderbuf_ptr, 1, 9, 34, 50, 0x1d);
		ds_writew(PIC_COPY_X1, 2);
		ds_writew(PIC_COPY_Y1, 10);
		ds_writew(PIC_COPY_X2, 33);
		ds_writew(PIC_COPY_Y2, 49);
		ds_writed(PIC_COPY_SRC, (Bit32u)p1);
		do_pic_copy(0);
		GUI_print_string((GUI_name_singular(get_monname(host_readbs(p_enemy)))), 1, 1);
	} else {
		do_border(g_renderbuf_ptr, 1, 149, 34, 190, 0x1d);
		ds_writew(PIC_COPY_X1, 2);
		ds_writew(PIC_COPY_Y1, 150);
		ds_writew(PIC_COPY_X2, 33);
		ds_writew(PIC_COPY_Y2, 189);
		ds_writed(PIC_COPY_SRC, (Bit32u)p1);
		do_pic_copy(0);
		GUI_print_string((GUI_name_singular(get_monname(host_readbs(p_enemy)))), 1, 193);
	}

	ds_writed(PIC_COPY_DST, ds_readd(VGA_MEMSTART));
	ds_writed(VGA_BACKBUFFER, ds_readd(VGA_MEMSTART));

	set_textcolor(fg_bak, bg_bak);
}

#if !defined(__BORLANDC__)
}
#endif
