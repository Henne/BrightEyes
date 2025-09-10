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

struct struct_fighter* FIG_get_fighter(signed char fighter_id)
{
	struct struct_fighter *list_i = g_fig_list_head;

	while (list_i->id != fighter_id) {

		if (list_i->next == NULL) {
			return g_fig_list_head;	/* TODO: Should be NULL ? */
		}

		/* set fighter_ptr to the next element */
		list_i = list_i->next;
	}

	return list_i;
}

static signed char FIG_set_array(void)
{
	signed char i = 0;

	/* find first element that is zero */
	while (g_fig_list_array[i]) {

		i++;
	}

	/* make it 1 */
	g_fig_list_array[i] = 1;

	/* return the number of the index */
	return i;
}

void FIG_draw_figures(void)
{
	signed short l1, l2;
	struct struct_fighter *list_i;
	struct struct_rect rect_bak;
	Bit8u* gfx_dst_bak;
	signed short l_si, l_di;

	l1 = 10;
	l2 = 118;

	/* TODO: potential bug: Just backup the pointer dst or the whole structure struct_pic_copy ? */
	gfx_dst_bak = g_pic_copy.dst;
	g_pic_copy.dst = g_renderbuf_ptr;

	/* backup a structure */
	rect_bak = g_pic_copy_rect;

	list_i = g_fig_list_head;

	do {

		if (list_i->visible == 1) {

			l_si = (l1 - list_i->width / 2) + 10 * (list_i->cbx + list_i->cby);
			l_di = (l2 - list_i->height) +	5 * (list_i->cbx - list_i->cby);

			l_si += list_i->offsetx;
			l_di += list_i->offsety;

			g_pic_copy.x1 = l_si;
			g_pic_copy.y1 = l_di;
			g_pic_copy.x2 = l_si + list_i->width - 1;
			g_pic_copy.y2 = l_di + list_i->height - 1;
			g_pic_copy.src = list_i->gfxbuf;

			g_pic_copy_rect.y1 = l_di + list_i->y1;
			if (g_pic_copy_rect.y1 < 0) g_pic_copy_rect.y1 = 0;

			g_pic_copy_rect.x1 = l_si + list_i->x1;
			if (g_pic_copy_rect.x1 < 0) g_pic_copy_rect.x1 = 0;

			g_pic_copy_rect.y2 = l_di + list_i->y2;
			if (g_pic_copy_rect.y2 > (200 - 1)) g_pic_copy_rect.y2 = (200 - 1);

			g_pic_copy_rect.x2 = l_si + list_i->x2;
			if (g_pic_copy_rect.x2 > (320 - 1)) g_pic_copy_rect.x2 = (320 - 1);

			do_pic_copy(2);
		}

	} while (list_i = list_i->next);

	/* restore two structures */
	g_pic_copy_rect = rect_bak;
	g_pic_copy.dst = gfx_dst_bak;
}

/**
 * \brief copy the content of g_renderbuf to g_vga_memstart
 */
void FIG_set_gfx(void)
{
	/* TODO: potential bug: Just backup pointer dst or the whole struct_pic_copy ? */
	Bit8u* dst_bak = g_pic_copy.dst;

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 199;
	g_pic_copy.src = g_renderbuf_ptr;
	g_pic_copy.dst = g_vga_memstart;
	update_mouse_cursor();
	do_pic_copy(0);
	refresh_screen_size();

	g_pic_copy.dst = dst_bak;
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

struct enemy_sheet* FIG_get_enemy_sheet(const signed short fighter_id)
{
	signed short i;

	for (i = 0; i < 20; i++) {
		if (fighter_id == g_enemy_sheets[i].fighter_id)
			return &g_enemy_sheets[i];
	}

	return NULL;
}

void FIG_set_sheet(signed char fighter_id, signed char val)
{
	struct struct_fighter *list_i = g_fig_list_head;

	while (list_i->id != fighter_id) {

		/* check for end of list */
		if (list_i->next == NULL) {
			return;
		}

		/* set ptr to ptr->next */
		list_i = list_i->next;
	}

	list_i->sheet = val;

	/* set presence flag */
	list_i->visible = 1;
}

/**
 * \brief   hides an element from the chessboard without removing it
 * \param   fighter_id  identificates the element to hide
 */
void FIG_make_invisible(signed char fighter_id)
{
	struct struct_fighter *list_i;
	struct struct_fighter *ptr2;

	list_i = g_fig_list_head;

	while (list_i->id != fighter_id) {

		if (list_i->next == NULL) {
			return;
		}

		list_i = list_i->next;
	}

	list_i->visible = 0;

	if (list_i->twofielded != -1) {

		ptr2 = g_fig_list_head;

		while (g_fig_twofielded_table[list_i->twofielded] != ptr2->id) {
			ptr2 = ptr2->next;
		}
		ptr2->visible = 0;
	}
}

/**
 * \brief   unhides an element from the chessboard
 *
 * \param   fighter_id  identificates the element to unhide
 */
void FIG_make_visible(signed short fighter_id)
{
	struct struct_fighter *list_i;
	struct struct_fighter *ptr2;

	list_i = g_fig_list_head;

	while (list_i->id != (signed char)fighter_id) {

		if (list_i->next == NULL){
			return;
		}

		list_i = list_i->next;
	}

	list_i->visible = 1;

	if (list_i->twofielded != -1) {

		ptr2 = g_fig_list_head;

		while (g_fig_twofielded_table[list_i->twofielded] != ptr2->id) {

			ptr2 = ptr2->next;
		}

		ptr2->visible = 1;
	}
}

void FIG_set_weapon_sheet(signed char fighter_id, signed char val)
{
	struct struct_fighter *list_i = g_fig_list_head;

	while (list_i->id != fighter_id) {

		if (list_i->next == NULL) {
			return;
		}

		list_i = list_i->next;
	}

	list_i->wsheet = val;
}

/**
 * \brief   removes an element from the FIG_LIST
 *
 * \param   fighter_id  the element to remove
 * \param   keep_in_memory whether to save the removed element in g_fig_list_elem, useful for moving element to end of list
 */
void FIG_remove_from_list(signed char fighter_id, signed char keep_in_memory)
{
	struct struct_fighter *list_i = g_fig_list_head;

	if (!list_i) return;

	while (list_i->id != fighter_id) {

		if (list_i->next == NULL) {
			return;
		}

		/* ptr = ptr->next; */
		list_i = list_i->next;
	}

	if (!keep_in_memory) {
		g_fig_list_array[fighter_id] = 0;
	} else {
		g_fig_list_elem = *list_i;
	}

	/* check if p == HEAD */
	if (list_i == g_fig_list_head) {
		/* Set HEAD: head = p->next;*/
		g_fig_list_head = list_i->next;

		if (g_fig_list_head) {
			g_fig_list_head->prev = NULL;
		}
	} else {
		/* check if p == tail */
		if (list_i->next == NULL) {
			/* list_i->prev->next = NULL */
			list_i->prev->next = NULL;
		} else {
			/* remove ptr from list */
			list_i->prev->next = list_i->next;
			list_i->next->prev = list_i->prev;
		}
	}

	memset(list_i, 0, sizeof(struct struct_fighter));

	list_i->id = -1;
}

/**
 * \brief   adds g_fig_list_elem to FIG_LIST
 *
 * \param   fighter_id  id to assign to the new element (-1 = assign a new id)
 * \return  the new element's fighter_id (position in g_fig_list_array)
 */
signed char FIG_add_to_list(signed char fighter_id)
{
	struct struct_fighter* p1;
	struct struct_fighter* p2;
	signed short x, y;

	p1 = g_fig_list_buffer;
	x = g_fig_list_elem.cbx;
	y = g_fig_list_elem.cby;

	if (g_fig_list_head == NULL) {

		g_fig_list_head = g_fig_list_buffer;

		*g_fig_list_head = g_fig_list_elem;

		if (fighter_id == -1) {
			g_fig_list_head->id = FIG_set_array();
		}

		g_fig_list_head->prev = NULL;
		g_fig_list_head->next = NULL;

#if !defined(__BORLANDC__)
		D1_LOG("\tlist created x = %d, y = %d\n", x, y);
#endif

		return g_fig_list_head->id;
	}

	while (p1->id != -1) {
		p1++;
	}

	*p1 = g_fig_list_elem;

	if (fighter_id == -1) {
		p1->id = FIG_set_array();
	} else {
		p1->id = fighter_id;
	}

	p2 = g_fig_list_head;

	/* The list is filled in the order of rendering, i.e. from rear to front:
	 * (x1,y1) is rendered before (x2,y2) if (x1 < x2) || (x1 == x2 && y1 > y2)
	 * On the same chessboard field, lower z is rendered before larger z.
	 */
	if (g_fig_list_elem.z != -1) {

		while ((p2->cbx <= x) && (p2->cbx != x || p2->cby >= y) &&
		((p2->cbx != x) || (p2->cby != y) || (p2->z <= g_fig_list_elem.z)))
		{
			if (p2->next == 0) {

				/* append to end of the list */

				p2->next = p1;
				p1->prev = p2;
				p1->next = NULL;

#if !defined(__BORLANDC__)
				D1_LOG("\tlist appended x = %d, y = %d\n", x, y);
#endif
				return p1->id;
			}

			p2 = p2->next;
		}
	}

	p1->prev = p2->prev;

	if (p2->prev != 0)
		p2->prev->next = p1;
	else
		g_fig_list_head = p1;

	p2->prev = p1;
	p1->next = p2;

#if !defined(__BORLANDC__)
	D1_LOG("\tlist insert x = %d, y = %d\n", x, y);
#endif

	return p1->id;
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
	g_pic_copy.src = hero + HERO_PORTRAIT;

	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	g_pic_copy.dst = g_renderbuf_ptr;
	g_vga_backbuffer = g_renderbuf_ptr;

	if (loc == 0) {

		do_border(g_renderbuf_ptr, 1, 9, 34, 42, 29);
		g_pic_copy.x1 = 2;
		g_pic_copy.y1 = 10;
		g_pic_copy.x2 = 33;
		g_pic_copy.y2 = 41;
		GUI_print_string((char*)hero + HERO_NAME2, 1, 1);

		draw_bar(0, 0, host_readw(hero + HERO_LE), host_readw(hero + HERO_LE_ORIG), 1);

		draw_bar(1, 0, host_readw(hero + HERO_AE), host_readw(hero + HERO_AE_ORIG), 1);
	} else {
		do_border(g_renderbuf_ptr, 1, 157, 34, 190, 29);
		g_pic_copy.x1 = 2;
		g_pic_copy.y1 = 158;
		g_pic_copy.x2 = 33;
		g_pic_copy.y2 = 189;
		GUI_print_string((char*)hero + HERO_NAME2, 1, 193);
	}

	do_pic_copy(0);

	g_pic_copy.dst = g_vga_memstart;
	g_vga_backbuffer = g_vga_memstart;

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
	struct enemy_sheet *p_enemy;
	signed short fg_bak;
	signed short bg_bak;
	Bit8u* p1;
	struct nvf_desc nvf;

	p1 = ((HugePt)g_buffer8_ptr) - 1288L;

	p_enemy = &g_enemy_sheets[id - 10];

	if (g_gfxtab_figures_main[p_enemy->gfx_id][0] != g_fight_figs_index) {

		nvf.src = (Bit8u*)load_fight_figs(g_gfxtab_figures_main[p_enemy->gfx_id][0]);
		nvf.dst = p1;
		nvf.no = 1;
		nvf.type = 0;
		nvf.width = (Bit8u*)&height_width;
		nvf.height = (Bit8u*)&height_width;

		process_nvf(&nvf);

		g_fight_figs_index = g_gfxtab_figures_main[p_enemy->gfx_id][0];
	}

	/* save and set text colors */
	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0);

	/* set gfx pointers */
	g_pic_copy.dst = g_renderbuf_ptr;
	g_vga_backbuffer = g_renderbuf_ptr;

	if (loc == 0) {
		do_border(g_renderbuf_ptr, 1, 9, 34, 50, 0x1d);
		g_pic_copy.x1 = 2;
		g_pic_copy.y1 = 10;
		g_pic_copy.x2 = 33;
		g_pic_copy.y2 = 49;
		g_pic_copy.src = p1;
		do_pic_copy(0);
		GUI_print_string(GUI_name_singular(get_monname(p_enemy->mon_id)), 1, 1);
	} else {
		do_border(g_renderbuf_ptr, 1, 149, 34, 190, 0x1d);
		g_pic_copy.x1 = 2;
		g_pic_copy.y1 = 150;
		g_pic_copy.x2 = 33;
		g_pic_copy.y2 = 189;
		g_pic_copy.src = p1;
		do_pic_copy(0);
		GUI_print_string(GUI_name_singular(get_monname(p_enemy->mon_id)), 1, 193);
	}

	g_pic_copy.dst = g_vga_memstart;
	g_vga_backbuffer = g_vga_memstart;

	set_textcolor(fg_bak, bg_bak);
}

#if !defined(__BORLANDC__)
}
#endif
