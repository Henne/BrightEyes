/**
 *	Rewrite of DSA1 v3.02_de functions of seg049 (group management)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg049.cpp
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg028.h"
#include "seg029.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg104.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

int GRP_compare_heroes(const void *p1, const void *p2)
{
	Bit8u *hero1, *hero2;

	hero1 = (Bit8u*)p1;
	hero2 = (Bit8u*)p2;

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) == gs_current_group) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group))
	{
		if (host_readbs(hero1 + HERO_GROUP_POS) < host_readbs(hero2 + HERO_GROUP_POS))
		{
			return -1;
		} else {
			return 1;
		}
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) == gs_current_group) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) != gs_current_group))
	{
		return -1;
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) != gs_current_group) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group))
	{
		return 1;
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) != gs_current_group) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) != gs_current_group))
	{
		if (host_readbs(hero1 + HERO_GROUP_POS) < host_readbs(hero2 + HERO_GROUP_POS))
		{
			return -1;
		} else {
			return 1;
		}
	}

	if (!(host_readbs(hero1 + HERO_TYPE)) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group))
	{
		return 1;
	}

	if (!(host_readbs(hero1 + HERO_TYPE)) &&
		(host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero2 + HERO_GROUP_NO) != gs_current_group))
	{
		return -1;
	}

	if (!(host_readbs(hero1 + HERO_TYPE)) &&
		!(host_readbs(hero2 + HERO_TYPE)))
	{
		return 0;
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) == gs_current_group) &&
		!(host_readbs(hero2 + HERO_TYPE)))
	{
		return -1;
	}

	if ((host_readbs(hero1 + HERO_TYPE) != HERO_TYPE_NONE) &&
		(host_readbs(hero1 + HERO_GROUP_NO) != gs_current_group) &&
		!(host_readbs(hero2 + HERO_TYPE)))
	{
		return 1;
	}

	return 0;
}

void GRP_sort_heroes(void)
{
	signed short i;

	qsort((void*)get_hero(0), 6, SIZEOF_HERO, GRP_compare_heroes);

	for (i = 0; i < 6; i++) {
		host_writeb(get_hero(i) + HERO_GROUP_POS, i + 1);
	}
}

void GRP_save_pos(signed short group)
{
	signed short refresh = 0;

	if (group & 0x8000) {
		refresh = 1;
	}

	group &= 0x7fff;

	GRP_sort_heroes();

	gs_groups_direction[group] = gs_direction;

	gs_groups_x_target[group] = gs_x_target;
	gs_groups_y_target[group] = gs_y_target;

	gs_groups_current_loctype[group] = gs_current_loctype;
	gs_groups_town[group] = gs_current_town;
	gs_groups_dng_index[group] = gs_dungeon_index;
	gs_groups_dng_level[group] = gs_dungeon_level;
	gs_groups_direction_bak[group] = gs_direction_bak;

	gs_groups_x_target_bak[group] = gs_x_target_bak;
	gs_groups_y_target_bak[group] = gs_y_target_bak;

	gs_groups_current_loctype_bak[group] = gs_current_loctype_bak;
	gs_groups_town_bak[group] = gs_current_town_bak;
	gs_groups_dng_index_bak[group] = gs_dungeon_index_bak;
	gs_groups_dng_level_bak[group] = gs_dungeon_level_bak;

	if (!refresh) {
		draw_status_line();
	}
}

void GRP_split(void)
{
	signed short new_group_id;
	signed short not_empty;
	signed short answer;

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 13:
	 * Split group does not work if the active group consists of 2 available heroes and there is an NPC in another group. */
	if (count_heroes_available_in_group() <= (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE ? 2 : 1))
#else
	if (count_heroes_available_in_group_ignore_npc() <= 1)
#endif
	{
		/* don't allow to split a group consisting of a single hero or of a single hero + NPC */

		GUI_output(get_ttx(514)); /* Einer alleine kann sich doch nicht aufteilen... Oder soll er sich zerreissen? */
	} else {

		not_empty = 0;
		new_group_id = 0;

		while (gs_group_member_counts[new_group_id] != 0) {
			new_group_id++;
		}

		do {
			g_hero_sel_exclude = 6;
			answer = select_hero_from_group(get_ttx(515));

			if (answer == -1) {
				break;
			} else {

				not_empty = 1;
				host_writeb(get_hero(answer) + HERO_GROUP_NO, (signed char)new_group_id);
				gs_group_member_counts[new_group_id]++;
				gs_group_member_counts[gs_current_group]--;
			}

		}
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 14:
	 * Split group does not allow to select all but one available hero of the active group if there is an NPC in another group. */
		while (count_heroes_available_in_group() > (host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE ? 2 : 1));
#else
		while (count_heroes_available_in_group_ignore_npc() > 1);
#endif

		if (not_empty) {
			GRP_save_pos(new_group_id);
		}
	}
}

void GRP_merge(void)
{
	signed short answer;
	signed short i;

	answer = can_merge_group();

	if (answer == -1) {

		GUI_output(get_ttx(516));
	} else {

		do {

			gs_groups_direction[answer] = gs_groups_x_target[answer]
				= gs_groups_y_target[answer] = gs_groups_town[answer]
				= gs_groups_dng_index[answer] = gs_groups_dng_level[answer]
				= gs_groups_direction_bak[answer] = gs_groups_x_target_bak[answer]
				= gs_groups_y_target_bak[answer] = gs_groups_current_loctype_bak[answer]
				= gs_groups_town_bak[answer] = gs_groups_dng_index_bak[answer]
				= gs_groups_dng_level_bak[answer] = 0;

			gs_group_member_counts[answer] = 0;

			for (i = 0; i <= 6; i++) {

				if ((host_readbs(get_hero(i) + HERO_TYPE) != HERO_TYPE_NONE) &&
					host_readbs(get_hero(i) + HERO_GROUP_NO) == answer)
				{
					host_writeb(get_hero(i) + HERO_GROUP_NO, gs_current_group);
					gs_group_member_counts[gs_current_group]++;
				}
			}

			GRP_sort_heroes();
			answer = can_merge_group();

		} while (answer != -1);

		draw_status_line();
	}
}

void GRP_switch_to_next(signed short mode)
{
	signed short i;
	signed short state;

	signed short group;
	signed short done = 0;
	signed short dng_level;

	group = gs_current_group;

	do {
		/* select next group */
		group++;

		if (group == 6) {
			group = 0;
		}

		if (gs_group_member_counts[group] != 0) {

			state = 0;

			for (i = 0; i < 6; i++) {

				if ((host_readbs(get_hero(i) + HERO_TYPE) != HERO_TYPE_NONE) &&
					(host_readbs(get_hero(i) + HERO_GROUP_NO) == group) &&
					check_hero(get_hero(i)))
				{
					if (host_readbs(get_hero(i) + HERO_JAIL) != 0) {
						/* hero is in prison */
						state = 2;
					} else {
						/* hero is free */
						state = 1;
					}
				}
			}

			if (!state || state == 2) {

				if (mode == 0) {

					if (!state) {
						GUI_output(get_ttx(36));
					} else {
						if ((state == 2) && GUI_bool(get_ttx(773))) {
							timewarp_until_time_of_day(HOURS(6));
							done = 1;
						}
					}
				}

			} else {

				if (state || mode != 2) {
					done = 1;
				}
			}
		}

	} while (done == 0);

	if (gs_current_group != group) {

		if ( ((gs_current_town != TOWNS_NONE) && !gs_groups_town[group]) ||
			(!gs_current_town && (gs_groups_town[group] != TOWNS_NONE)))
		{
			set_palette(g_palette_allblack2, 0x00, 0x20);
			set_palette(g_palette_allblack2, 0x80, 0x20);
			set_palette(g_palette_allblack2, 0xa0, 0x20);
		}

		if (gs_groups_dng_index[group] && (gs_groups_dng_index[group] != gs_dungeon_index))
		{
			g_dng_area_loaded = -1;
			g_area_prepared = -1;
		}

		if (gs_groups_town[group] && (gs_groups_town[group] != gs_current_town))
		{
			g_city_area_loaded = -1;
			g_area_prepared = -1;
		}

		/* save positions from the old group */
		gs_groups_direction[gs_current_group] = gs_direction;
		gs_groups_x_target[gs_current_group] = gs_x_target;
		gs_groups_y_target[gs_current_group] = gs_y_target;
		gs_groups_current_loctype[gs_current_group] = gs_current_loctype;
		gs_groups_town[gs_current_group] = gs_current_town;
		gs_groups_dng_index[gs_current_group] = gs_dungeon_index;
		gs_groups_dng_level[gs_current_group] = gs_dungeon_level;
		gs_groups_direction_bak[gs_current_group] = gs_direction_bak;
		gs_groups_x_target_bak[gs_current_group] = gs_x_target_bak;
		gs_groups_y_target_bak[gs_current_group] = gs_y_target_bak;
		gs_groups_current_loctype_bak[gs_current_group] = gs_current_loctype_bak;
		gs_groups_town_bak[gs_current_group] = gs_current_town_bak;
		gs_groups_dng_index_bak[gs_current_group] = gs_dungeon_index_bak;
		gs_groups_dng_level_bak[gs_current_group] = gs_dungeon_level_bak;

		/* set positions for the new group */
		gs_current_group = group;
		gs_direction = gs_groups_direction[group];
		gs_x_target = gs_groups_x_target[group];
		gs_y_target = gs_groups_y_target[group];
		gs_current_loctype = gs_groups_current_loctype[group];
		gs_current_town = gs_groups_town[group];
		gs_dungeon_index = gs_groups_dng_index[group];
		dng_level = gs_dungeon_level;
		gs_dungeon_level = gs_groups_dng_level[group];

		if (dng_level != gs_dungeon_level) {
			load_area_description(1);
		}

		gs_direction_bak = (gs_groups_direction_bak[group]);
		gs_x_target_bak = (gs_groups_x_target_bak[group]);
		gs_y_target_bak = (gs_groups_y_target_bak[group]);
		gs_current_loctype_bak = gs_groups_current_loctype_bak[group];
		gs_current_town_bak = gs_groups_town_bak[group];
		gs_dungeon_index_bak = gs_groups_dng_index_bak[group];
		gs_dungeon_level_bak = gs_groups_dng_level_bak[group];

		GRP_sort_heroes();

		for (group = 0; group <= 6; group++) {
			gs_food_message[group] = gs_unconscious_message[group] = 0;
		}

		ds_writew(REQUEST_REFRESH, 1);
	}
}

struct dummy {
	char a[SIZEOF_HERO];
};

void GRP_swap_heroes(void)
{
	signed short hero1_no;
	signed short hero2_no;
	signed char l2;
	signed char l3;
	signed char l4;
	signed char l5;
	signed char i;
	signed char tmp[SIZEOF_HERO];

	if ((ds_readw(HEROSWAP_ALLOWED) == 0) || !gs_group_member_counts[gs_current_group]) {
		return;
	}

	hero1_no = select_hero_from_group(get_ttx(302));

	if ((hero1_no != -1) && (hero1_no < 6)) {

		hero2_no = select_hero_from_group(get_ttx(303));

		if ((hero2_no != -1) && (hero1_no != hero2_no) && (hero2_no < 6)) {

			for (i = 0; i < 3; i++) {

				if (ds_readbs(WILDCAMP_GUARDS + i) == hero1_no) {
					ds_writebs(WILDCAMP_GUARDS + i, (signed char)hero2_no);
				}
			}

			/* save hero1 in tmp */
			*((struct dummy*)&tmp) = *((struct dummy*)get_hero(hero1_no));

			l2 = ds_readbs(WILDCAMP_GUARDSTATUS + hero1_no);
			l3 = ds_readbs(WILDCAMP_MAGICSTATUS + hero1_no);
			l4 = ds_readbs(WILDCAMP_REPLSTATUS + hero1_no);
			l5 = ds_readbs(WILDCAMP_HERBSTATUS + hero1_no);

			*((struct dummy*)get_hero(hero1_no)) = *((struct dummy*)get_hero(hero2_no));

			ds_writebs(WILDCAMP_GUARDSTATUS + hero1_no, ds_readbs(WILDCAMP_GUARDSTATUS + hero2_no));
			ds_writebs(WILDCAMP_MAGICSTATUS + hero1_no, ds_readbs(WILDCAMP_MAGICSTATUS + hero2_no));
			ds_writebs(WILDCAMP_REPLSTATUS + hero1_no, ds_readbs(WILDCAMP_REPLSTATUS + hero2_no));
			ds_writebs(WILDCAMP_HERBSTATUS + hero1_no, ds_readbs(WILDCAMP_HERBSTATUS + hero2_no));

			*((struct dummy*)get_hero(hero2_no)) = *((struct dummy*)&tmp);

			ds_writebs(WILDCAMP_GUARDSTATUS + hero2_no, l2);
			ds_writebs(WILDCAMP_MAGICSTATUS + hero2_no, l3);
			ds_writebs(WILDCAMP_REPLSTATUS + hero2_no, l4);
			ds_writebs(WILDCAMP_HERBSTATUS + hero2_no, l5);

			if (host_readbs(get_hero(hero1_no) + HERO_TYPE)) {
				host_writebs(get_hero(hero1_no) + HERO_ACTION_ID, FIG_ACTION_UNKNOWN2);
			}

			if (host_readbs(get_hero(hero2_no) + HERO_TYPE)) {
				host_writebs(get_hero(hero2_no) + HERO_ACTION_ID, FIG_ACTION_UNKNOWN2);
			}

			host_writeb(get_hero(hero1_no) + HERO_GROUP_POS, hero1_no + 1);
			host_writeb(get_hero(hero2_no) + HERO_GROUP_POS, hero2_no + 1);
		}
	}

	draw_status_line();
}

void GRP_move_hero(signed short src_pos)
{

	signed short dst_pos;
	Bit8u *src;
	Bit8u *dst;
	signed char src_guardstatus;
	signed char src_magicstatus;
	signed char src_replstatus;
	signed char src_herbstatus;
	signed char i;

	/* don't move NPC */
	if (src_pos != 6) {

		ds_writew(MOUSE_POSY_MIN, 157);
		ds_writew(MOUSE_POSY_MAX, 157);
		ds_writew(MOUSE_POSX_MIN, 1);
		ds_writew(MOUSE_POSX_MAX, 260);

		update_mouse_cursor();

		ds_writew(MOUSE_LOCKED, 1);
		ds_writew(MOUSE_POSY, 157);

		ds_writew(MOUSE_POSX, g_hero_pic_posx[src_pos]);
		g_pic_copy.x1 = ds_readw(MOUSE_POSX);
		g_pic_copy.y1 = ds_readw(MOUSE_POSY);
		g_pic_copy.x2 = ds_readw(MOUSE_POSX) + 31;
		g_pic_copy.y2 = ds_readw(MOUSE_POSY) + 31;
		g_pic_copy.src = g_icon;

		do_save_rect();

		ds_writew(MOUSE_LOCKED, 0);

		refresh_screen_size();

		ds_writew(MOUSE_LOCKED, 1);

		g_pic_copy.x1 = g_hero_pic_posx[src_pos];
		g_pic_copy.y1 = 157;
		g_pic_copy.x2 = g_hero_pic_posx[src_pos] + 31;
		g_pic_copy.y2 = 188;
		g_pic_copy.src = g_renderbuf_ptr;

		do_save_rect();

		ds_writew(MOUSE_LOCKED, 0);

		update_mouse_cursor();

		ds_writew(MOUSE_POSX_BAK, ds_readw(MOUSE_POSX));
		ds_writew(MOUSE_POSY_BAK, ds_readw(MOUSE_POSY));

		while (ds_readw(MOUSE1_EVENT2) == 0) {
#if !defined(__BORLANDC__)
			/* call DOSBOX to handle mouse ISR */
			wait_for_vsync();
#endif
			if (ds_readw(MOUSE_MOVED) != 0) {

				ds_writew(MOUSE_LOCKED, 1);

				wait_for_vsync();

				g_pic_copy.x1 = ds_readw(MOUSE_POSX_BAK);
				g_pic_copy.y1 = ds_readw(MOUSE_POSY_BAK);
				g_pic_copy.x2 = ds_readw(MOUSE_POSX_BAK) + 31;
				g_pic_copy.y2 = ds_readw(MOUSE_POSY_BAK) + 31;
				g_pic_copy.src = g_icon;

				do_pic_copy(0);

				g_pic_copy.x1 = ds_readw(MOUSE_POSX);
				g_pic_copy.y1 = ds_readw(MOUSE_POSY);
				g_pic_copy.x2 = ds_readw(MOUSE_POSX) + 31;
				g_pic_copy.y2 = ds_readw(MOUSE_POSY) + 31;
				g_pic_copy.src = g_icon;

				do_save_rect();

				g_pic_copy.src = (g_renderbuf_ptr);

				do_pic_copy(0);

				ds_writew(MOUSE_POSX_BAK, ds_readw(MOUSE_POSX));
				ds_writew(MOUSE_POSY_BAK, ds_readw(MOUSE_POSY));
				ds_writew(MOUSE_MOVED, 0);
				ds_writew(MOUSE_LOCKED, 0);
			}
		}

		g_pic_copy.x1 = ds_readw(MOUSE_POSX_BAK);
		g_pic_copy.y1 = ds_readw(MOUSE_POSY_BAK);
		g_pic_copy.x2 = ds_readw(MOUSE_POSX_BAK) + 31;
		g_pic_copy.y2 = ds_readw(MOUSE_POSY_BAK) + 31;
		g_pic_copy.src = g_icon;

		do_pic_copy(0);

		dst_pos = 6;
		while (g_hero_pic_posx[--dst_pos] > ds_readws(MOUSE_POSX))
		{
			;
		}

		if (dst_pos < 0) {
			dst_pos = 0;
		}

		if ((src_pos != dst_pos) && (dst_pos != 6)) {

			dst = get_hero(dst_pos);
			src = get_hero(src_pos);

			if (!host_readbs(dst + 0x21) || (host_readbs(dst + 0x87) == gs_current_group)) {

				for (i = 0; i < 3; i++) {
					if (ds_readbs(WILDCAMP_GUARDS + i) == src_pos) {
						ds_writebs(WILDCAMP_GUARDS + i, (signed char)dst_pos);
					}
				}

				memcpy(g_renderbuf_ptr, src, SIZEOF_HERO);

				src_guardstatus = ds_readbs(WILDCAMP_GUARDSTATUS + src_pos);
				src_magicstatus = ds_readbs(WILDCAMP_MAGICSTATUS + src_pos);
				src_replstatus = ds_readbs(WILDCAMP_REPLSTATUS + src_pos);
				src_herbstatus = ds_readbs(WILDCAMP_HERBSTATUS + src_pos);

				*((struct dummy*)src) = *((struct dummy*)dst);

				ds_writeb(WILDCAMP_GUARDSTATUS + src_pos, ds_readbs(WILDCAMP_GUARDSTATUS + dst_pos));
				ds_writeb(WILDCAMP_MAGICSTATUS + src_pos, ds_readbs(WILDCAMP_MAGICSTATUS + dst_pos));
				ds_writeb(WILDCAMP_REPLSTATUS + src_pos, ds_readbs(WILDCAMP_REPLSTATUS + dst_pos));
				ds_writeb(WILDCAMP_HERBSTATUS + src_pos, ds_readbs(WILDCAMP_HERBSTATUS + dst_pos));

				memcpy(dst, g_renderbuf_ptr, SIZEOF_HERO);

				ds_writeb(WILDCAMP_GUARDSTATUS + dst_pos, src_guardstatus);
				ds_writeb(WILDCAMP_MAGICSTATUS + dst_pos, src_magicstatus);
				ds_writeb(WILDCAMP_REPLSTATUS + dst_pos, src_replstatus);
				ds_writeb(WILDCAMP_HERBSTATUS + dst_pos, src_herbstatus);

				host_writeb(src + 0x84, 100);
				host_writeb(dst + 0x84, 100);
				host_writeb(dst + 0x8a, src_pos + 1);
				host_writeb(src + 0x8a, dst_pos + 1);
			}
		}

		ds_writew(MOUSE1_EVENT2, 0);
		ds_writew(MOUSE_POSY_MIN, 0);
		ds_writew(MOUSE_POSY_MAX, 199);
		ds_writew(MOUSE_POSX_MIN, 0);
		ds_writew(MOUSE_POSX_MAX, 319);

		refresh_screen_size();
		draw_status_line();
	}
}

void GRP_hero_sleep(Bit8u *hero, signed short quality)
{
	signed short le_regen;
	signed short ae_regen;
	signed short diff;
	signed short tmp;

	if (!hero_dead(hero) &&
		(host_readd(hero + HERO_STAFFSPELL_TIMER) == 0) &&
		(host_readbs(hero + HERO_RECIPE_TIMER) == 0))
	{

		if ((ds_readbs(TRAVEL_BY_SHIP) != 0) && (random_schick(100) < 10)) {
			/* chance of motion sickness is 9% */

			sprintf(g_dtp2,
				get_ttx(796),
				hero + HERO_NAME2);

			GUI_output(g_dtp2);

		} else {

			if (!hero_is_diseased(hero) && !hero_is_poisoned(hero)) {

				if (host_readbs(hero + HERO_RUHE_KOERPER) > 0) {
					dec_ptr_bs(hero + HERO_RUHE_KOERPER);
				}

				if (!hero_brewing(hero)) {

					le_regen = random_schick(6) + quality;
					ae_regen = random_schick(6) + quality;

					/* Original-Bug: next operator should be <= */
					if (le_regen < 0) {
						le_regen = 1;
					}

					/* Original-Bug: next operator should be <= */
					if (ae_regen < 0) {
						ae_regen = 1;
					}

					/* swap LE and AE */
					if ((host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH) && (le_regen < ae_regen)) {
						tmp = ae_regen;
						ae_regen = le_regen;
						le_regen = tmp;
					}

					if (host_readbs(hero + HERO_RUHE_KOERPER) > 0) {
						le_regen += le_regen;
						ae_regen += ae_regen;
					}

					/* Do LE */

					diff = host_readws(hero + HERO_LE_ORIG) - host_readws(hero + HERO_LE);

					if ((diff != 0) && le_regen) {

						if (diff < le_regen) {
							le_regen = diff;
						}

						add_hero_le(hero, le_regen);

						strcpy(g_text_output_buf, get_ttx(392));

						if (le_regen > 1) {
							strcat(g_text_output_buf, get_ttx(393));
						}

						sprintf(g_dtp2, get_ttx(319), hero + HERO_NAME2, le_regen, g_text_output_buf);

						if (ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
							GUI_print_loc_line(g_dtp2);
							delay_or_keypress(200);
						} else {
							if (ds_readbs(TRAVEL_BY_SHIP) != 0) {
								GUI_output(g_dtp2);
							}
						}

					}

					if (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_WITCH) {

						diff = host_readws(hero + HERO_AE_ORIG) - host_readws(hero + HERO_AE);

						if ((diff != 0) && ae_regen) {

							if (diff < ae_regen) {
								ae_regen = diff;
							}

							add_hero_ae(hero, ae_regen);

							strcpy(g_text_output_buf, get_ttx(392));

							if (ae_regen > 1) {
								strcat(g_text_output_buf, get_ttx(393));
							}

							sprintf(g_dtp2, get_ttx(320), hero + HERO_NAME2,	ae_regen, g_text_output_buf);

							if (ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
								GUI_print_loc_line(g_dtp2);
								delay_or_keypress(200);
							} else {
								if (ds_readbs(TRAVEL_BY_SHIP) != 0) {
									GUI_output(g_dtp2);
								}
							}
						}
					}
				} else {
					/* hero brewing, HERO_STAFFSPELL_TIMER == 0, HERO_RECIPE_TIMER == 0, not dead, not deseased, not poisoned */
					/* TODO: potential Original-Bug: why is 'not poisoned', 'not deseased' and 'HERO_STAFFSPELL_TIMER == 0' needed to complete the brewing process? */
					do_alchemy(hero, host_readbs(hero + HERO_RECIPE_ID), 0);
				}
			} else {
				sprintf(g_dtp2,
					get_ttx(558),
					hero + HERO_NAME2);

				GUI_output(g_dtp2);
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
