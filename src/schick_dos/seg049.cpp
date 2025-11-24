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
#include "seg049.h"
#include "seg096.h"
#include "seg097.h"
#include "seg104.h"

int GRP_compare_heroes(const void *p1, const void *p2)
{
	struct struct_hero *hero1 = (struct struct_hero*)p1;
	struct struct_hero *hero2 = (struct struct_hero*)p2;

	if ((hero1->typus != HERO_TYPE_NONE) &&
		(hero1->group_id == gs_active_group_id) &&
		(hero2->typus != HERO_TYPE_NONE) &&
		(hero2->group_id == gs_active_group_id))
	{
		if (hero1->slot_pos < hero2->slot_pos)
		{
			return -1;
		} else {
			return 1;
		}
	}

	if ((hero1->typus != HERO_TYPE_NONE) &&
		(hero1->group_id == gs_active_group_id) &&
		(hero2->typus != HERO_TYPE_NONE) &&
		(hero2->group_id != gs_active_group_id))
	{
		return -1;
	}

	if ((hero1->typus != HERO_TYPE_NONE) &&
		(hero1->group_id != gs_active_group_id) &&
		(hero2->typus != HERO_TYPE_NONE) &&
		(hero2->group_id == gs_active_group_id))
	{
		return 1;
	}

	if ((hero1->typus != HERO_TYPE_NONE) &&
		(hero1->group_id != gs_active_group_id) &&
		(hero2->typus != HERO_TYPE_NONE) &&
		(hero2->group_id != gs_active_group_id))
	{
		if (hero1->slot_pos < hero2->slot_pos)
		{
			return -1;
		} else {
			return 1;
		}
	}

	if (!(hero1->typus) &&
		(hero2->typus != HERO_TYPE_NONE) &&
		(hero2->group_id == gs_active_group_id))
	{
		return 1;
	}

	if (!(hero1->typus) &&
		(hero2->typus != HERO_TYPE_NONE) &&
		(hero2->group_id != gs_active_group_id))
	{
		return -1;
	}

	if (!(hero1->typus) && !(hero2->typus))
	{
		return 0;
	}

	if ((hero1->typus != HERO_TYPE_NONE) &&
		(hero1->group_id == gs_active_group_id) &&
		!(hero2->typus))
	{
		return -1;
	}

	if ((hero1->typus != HERO_TYPE_NONE) &&
		(hero1->group_id != gs_active_group_id) &&
		!(hero2->typus))
	{
		return 1;
	}

	return 0;
}

void GRP_sort_heroes(void)
{
	signed int i;

	qsort((void*)get_hero(0), 6, sizeof(struct struct_hero), GRP_compare_heroes);

	for (i = 0; i < 6; i++) {
		get_hero(i)->slot_pos = i + 1;
	}
}

void GRP_save_pos(signed int group)
{
	signed int refresh = 0;

	if (group & 0x8000) {
		refresh = 1;
	}

	group &= 0x7fff;

	GRP_sort_heroes();

	gs_groups_direction[group] = gs_direction;

	gs_groups_x_target[group] = gs_x_target;
	gs_groups_y_target[group] = gs_y_target;

	gs_town_groups_loctype[group] = gs_town_loc_type;
	gs_groups_town_id[group] = gs_town_id;
	gs_groups_dungeon_id[group] = gs_dungeon_id;
	gs_groups_dng_level[group] = gs_dungeon_level;
	gs_groups_direction_bak[group] = gs_direction_bak;

	gs_groups_x_target_bak[group] = gs_x_target_bak;
	gs_groups_y_target_bak[group] = gs_y_target_bak;

	gs_town_groups_loctype_bak[group] = gs_town_loc_type_bak;
	gs_groups_town_id_bak[group] = gs_town_id_bak;
	gs_groups_dungeon_id_bak[group] = gs_dungeon_id_bak;
	gs_groups_dng_level_bak[group] = gs_dungeon_level_bak;

	if (!refresh) {
		draw_status_line();
	}
}

void GRP_split(void)
{
	signed int new_group_id;
	signed int not_empty;
	signed int answer;

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 13:
	 * Split group does not work if the active group consists of 2 available heroes and there is an NPC in another group. */
	if (count_heroes_available_in_group() <= (get_hero(6)->typus != HERO_TYPE_NONE ? 2 : 1))
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
				get_hero(answer)->group_id = new_group_id;
				gs_group_member_counts[new_group_id]++;
				gs_group_member_counts[gs_active_group_id]--;
			}

		}
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 14:
	 * Split group does not allow to select all but one available hero of the active group if there is an NPC in another group. */
		while (count_heroes_available_in_group() > (get_hero(6)->typus != HERO_TYPE_NONE ? 2 : 1));
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
	signed int answer;
	signed int i;

	answer = can_merge_group();

	if (answer == -1) {

		GUI_output(get_ttx(516));
	} else {

		do {

			gs_groups_direction[answer] = gs_groups_x_target[answer]
				= gs_groups_y_target[answer] = gs_groups_town_id[answer]
				= gs_groups_dungeon_id[answer] = gs_groups_dng_level[answer]
				= gs_groups_direction_bak[answer] = gs_groups_x_target_bak[answer]
				= gs_groups_y_target_bak[answer] = gs_town_groups_loctype_bak[answer]
				= gs_groups_town_id_bak[answer] = gs_groups_dungeon_id_bak[answer]
				= gs_groups_dng_level_bak[answer] = 0;

			gs_group_member_counts[answer] = 0;

			for (i = 0; i <= 6; i++) {

				if ((get_hero(i)->typus != HERO_TYPE_NONE) && (get_hero(i)->group_id == answer))
				{
					get_hero(i)->group_id = gs_active_group_id;
					gs_group_member_counts[gs_active_group_id]++;
				}
			}

			GRP_sort_heroes();
			answer = can_merge_group();

		} while (answer != -1);

		draw_status_line();
	}
}

void GRP_switch_to_next(const signed int mode)
{
	signed int i;
	signed int state;

	signed int group;
	signed int done = 0;
	signed int dng_level;

	group = gs_active_group_id;

	do {
		/* select next group */
		group++;

		if (group == 6) {
			group = 0;
		}

		if (gs_group_member_counts[group] != 0) {

			state = 0;

			for (i = 0; i < 6; i++) {

				if ((get_hero(i)->typus != HERO_TYPE_NONE) && (get_hero(i)->group_id == group) && check_hero(get_hero(i)))
				{
					if (get_hero(i)->jail) {
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

	if (gs_active_group_id != group) {

		if ( ((gs_town_id != TOWN_ID_NONE) && !gs_groups_town_id[group]) ||
			(!gs_town_id && (gs_groups_town_id[group] != TOWN_ID_NONE)))
		{
			set_palette(g_palette_allblack2, 0x00, 0x20);
			set_palette(g_palette_allblack2, 0x80, 0x20);
			set_palette(g_palette_allblack2, 0xa0, 0x20);
		}

		if (gs_groups_dungeon_id[group] && (gs_groups_dungeon_id[group] != gs_dungeon_id))
		{
			g_dng_loaded_dungeon_id = -1;
			g_area_prepared = AREA_TYPE_NONE;
		}

		if (gs_groups_town_id[group] && (gs_groups_town_id[group] != gs_town_id))
		{
			g_town_loaded_town_id = -1;
			g_area_prepared = AREA_TYPE_NONE;
		}

		/* save positions from the old group */
		gs_groups_direction[gs_active_group_id] = gs_direction;
		gs_groups_x_target[gs_active_group_id] = gs_x_target;
		gs_groups_y_target[gs_active_group_id] = gs_y_target;
		gs_town_groups_loctype[gs_active_group_id] = gs_town_loc_type;
		gs_groups_town_id[gs_active_group_id] = gs_town_id;
		gs_groups_dungeon_id[gs_active_group_id] = gs_dungeon_id;
		gs_groups_dng_level[gs_active_group_id] = gs_dungeon_level;
		gs_groups_direction_bak[gs_active_group_id] = gs_direction_bak;
		gs_groups_x_target_bak[gs_active_group_id] = gs_x_target_bak;
		gs_groups_y_target_bak[gs_active_group_id] = gs_y_target_bak;
		gs_town_groups_loctype_bak[gs_active_group_id] = gs_town_loc_type_bak;
		gs_groups_town_id_bak[gs_active_group_id] = gs_town_id_bak;
		gs_groups_dungeon_id_bak[gs_active_group_id] = gs_dungeon_id_bak;
		gs_groups_dng_level_bak[gs_active_group_id] = gs_dungeon_level_bak;

		/* set positions for the new group */
		gs_active_group_id = group;
		gs_direction = gs_groups_direction[group];
		gs_x_target = gs_groups_x_target[group];
		gs_y_target = gs_groups_y_target[group];
		gs_town_loc_type = gs_town_groups_loctype[group];
		gs_town_id = gs_groups_town_id[group];
		gs_dungeon_id = gs_groups_dungeon_id[group];
		dng_level = gs_dungeon_level;
		gs_dungeon_level = gs_groups_dng_level[group];

		if (dng_level != gs_dungeon_level) {
			load_area_description(1);
		}

		gs_direction_bak = gs_groups_direction_bak[group];
		gs_x_target_bak = gs_groups_x_target_bak[group];
		gs_y_target_bak = gs_groups_y_target_bak[group];
		gs_town_loc_type_bak = gs_town_groups_loctype_bak[group];
		gs_town_id_bak = gs_groups_town_id_bak[group];
		gs_dungeon_id_bak = gs_groups_dungeon_id_bak[group];
		gs_dungeon_level_bak = gs_groups_dng_level_bak[group];

		GRP_sort_heroes();

		for (group = 0; group <= 6; group++) {
			gs_food_message[group] = gs_unconscious_message[group] = 0;
		}

		g_request_refresh = 1;
	}
}

void GRP_swap_heroes(void)
{
	signed int hero_pos_1;
	signed int hero_pos_2;
	signed char l2;
	signed char l3;
	signed char l4;
	signed char l5;
	signed char i;
	struct struct_hero tmp;

	if (!g_heroswap_allowed || !gs_group_member_counts[gs_active_group_id]) {
		return;
	}

	hero_pos_1 = select_hero_from_group(get_ttx(302));

	if ((hero_pos_1 != -1) && (hero_pos_1 < 6)) {

		hero_pos_2 = select_hero_from_group(get_ttx(303));

		if ((hero_pos_2 != -1) && (hero_pos_1 != hero_pos_2) && (hero_pos_2 < 6)) {

			for (i = 0; i < 3; i++) {

				if (g_wildcamp_guards[i] == hero_pos_1) {
					g_wildcamp_guards[i] = hero_pos_2;
				}
			}

			/* save hero1 in tmp */
			tmp = *get_hero(hero_pos_1);	/* struct_copy */

			l2 = g_wildcamp_guardstatus[hero_pos_1];
			l3 = g_wildcamp_magicstatus[hero_pos_1];
			l4 = g_wildcamp_replstatus[hero_pos_1];
			l5 = g_wildcamp_herbstatus[hero_pos_1];

			*(get_hero(hero_pos_1)) = *(get_hero(hero_pos_2)); /* struct_copy */

			g_wildcamp_guardstatus[hero_pos_1] = g_wildcamp_guardstatus[hero_pos_2];
			g_wildcamp_magicstatus[hero_pos_1] = g_wildcamp_magicstatus[hero_pos_2];
			g_wildcamp_replstatus[hero_pos_1] = g_wildcamp_replstatus[hero_pos_2];
			g_wildcamp_herbstatus[hero_pos_1] = g_wildcamp_herbstatus[hero_pos_2];

			*(get_hero(hero_pos_2)) = tmp; /* struct_copy */

			g_wildcamp_guardstatus[hero_pos_2] = l2;
			g_wildcamp_magicstatus[hero_pos_2] = l3;
			g_wildcamp_replstatus[hero_pos_2] = l4;
			g_wildcamp_herbstatus[hero_pos_2] = l5;

			if (get_hero(hero_pos_1)->typus) {
				get_hero(hero_pos_1)->action_id = FIG_ACTION_PARRY;
			}

			if (get_hero(hero_pos_2)->typus) {
				get_hero(hero_pos_2)->action_id = FIG_ACTION_PARRY;
			}

			get_hero(hero_pos_1)->slot_pos = hero_pos_1 + 1;
			get_hero(hero_pos_2)->slot_pos = hero_pos_2 + 1;
		}
	}

	draw_status_line();
}

void GRP_move_hero(const signed int src_pos)
{
	signed int dst_pos;
	struct struct_hero *src;
	struct struct_hero *dst;
	signed char src_guardstatus;
	signed char src_magicstatus;
	signed char src_replstatus;
	signed char src_herbstatus;
	signed char i;

	/* don't move NPC */
	if (src_pos != 6) {

		g_mouse_posy_min = 157;
		g_mouse_posy_max = 157;
		g_mouse_posx_min = 1;
		g_mouse_posx_max = 260;

		call_mouse_bg();

		g_mouse_locked = 1;
		g_mouse_posy = 157;

		g_mouse_posx = g_hero_pic_posx[src_pos];
		g_pic_copy.x1 = g_mouse_posx;
		g_pic_copy.y1 = g_mouse_posy;
		g_pic_copy.x2 = g_mouse_posx + 31;
		g_pic_copy.y2 = g_mouse_posy + 31;
		g_pic_copy.src = g_icon;

		do_save_rect();

		g_mouse_locked = 0;

		call_mouse();

		g_mouse_locked = 1;

		g_pic_copy.x1 = g_hero_pic_posx[src_pos];
		g_pic_copy.y1 = 157;
		g_pic_copy.x2 = g_hero_pic_posx[src_pos] + 31;
		g_pic_copy.y2 = 188;
		g_pic_copy.src = g_renderbuf_ptr;

		do_save_rect();

		g_mouse_locked = 0;

		call_mouse_bg();

		g_mouse_posx_bak = g_mouse_posx;
		g_mouse_posy_bak = g_mouse_posy;

		while (!g_mouse1_event2) {
#if !defined(__BORLANDC__)
			/* call DOSBOX to handle mouse ISR */
			wait_for_vsync();
#endif
			if (g_mouse_moved) {

				g_mouse_locked = 1;

				wait_for_vsync();

				g_pic_copy.x1 = g_mouse_posx_bak;
				g_pic_copy.y1 = g_mouse_posy_bak;
				g_pic_copy.x2 = g_mouse_posx_bak + 31;
				g_pic_copy.y2 = g_mouse_posy_bak + 31;
				g_pic_copy.src = g_icon;

				do_pic_copy(0);

				g_pic_copy.x1 = g_mouse_posx;
				g_pic_copy.y1 = g_mouse_posy;
				g_pic_copy.x2 = g_mouse_posx + 31;
				g_pic_copy.y2 = g_mouse_posy + 31;
				g_pic_copy.src = g_icon;

				do_save_rect();

				g_pic_copy.src = g_renderbuf_ptr;

				do_pic_copy(0);

				g_mouse_posx_bak = g_mouse_posx;
				g_mouse_posy_bak = g_mouse_posy;
				g_mouse_moved = 0;
				g_mouse_locked = 0;
			}
		}

		g_pic_copy.x1 = g_mouse_posx_bak;
		g_pic_copy.y1 = g_mouse_posy_bak;
		g_pic_copy.x2 = g_mouse_posx_bak + 31;
		g_pic_copy.y2 = g_mouse_posy_bak + 31;
		g_pic_copy.src = g_icon;

		do_pic_copy(0);

		dst_pos = 6;
		while (g_hero_pic_posx[--dst_pos] > g_mouse_posx)
		{
			;
		}

		if (dst_pos < 0) {
			dst_pos = 0;
		}

		if ((src_pos != dst_pos) && (dst_pos != 6)) {

			dst = get_hero(dst_pos);
			src = get_hero(src_pos);

			if (!dst->typus || (dst->group_id == gs_active_group_id)) {

				for (i = 0; i < 3; i++) {
					if (g_wildcamp_guards[i] == src_pos) {
						g_wildcamp_guards[i] = dst_pos;
					}
				}

				memcpy(g_renderbuf_ptr, src, sizeof(struct struct_hero));

				src_guardstatus = g_wildcamp_guardstatus[src_pos];
				src_magicstatus = g_wildcamp_magicstatus[src_pos];
				src_replstatus = g_wildcamp_replstatus[src_pos];
				src_herbstatus = g_wildcamp_herbstatus[src_pos];

				*src = *dst; /* struct_copy */

				g_wildcamp_guardstatus[src_pos] = g_wildcamp_guardstatus[dst_pos];
				g_wildcamp_magicstatus[src_pos] = g_wildcamp_magicstatus[dst_pos];
				g_wildcamp_replstatus[src_pos] = g_wildcamp_replstatus[dst_pos];
				g_wildcamp_herbstatus[src_pos] = g_wildcamp_herbstatus[dst_pos];

				memcpy(dst, g_renderbuf_ptr, sizeof(struct struct_hero));

				g_wildcamp_guardstatus[dst_pos] = src_guardstatus;
				g_wildcamp_magicstatus[dst_pos] = src_magicstatus;
				g_wildcamp_replstatus[dst_pos] = src_replstatus;
				g_wildcamp_herbstatus[dst_pos] = src_herbstatus;

				src->action_id = 100;
				dst->action_id = 100;
				dst->slot_pos = src_pos + 1;
				src->slot_pos = dst_pos + 1;
			}
		}

		g_mouse1_event2 = 0;
		g_mouse_posy_min = 0;
		g_mouse_posy_max = 200 - 1;
		g_mouse_posx_min = 0;
		g_mouse_posx_max = 320 - 1;

		call_mouse();
		draw_status_line();
	}
}

void GRP_hero_sleep(struct struct_hero *hero, const signed int quality)
{
	signed int le_regen;
	signed int ae_regen;
	signed int diff;
	signed int tmp;

	if (!hero->flags.dead && (hero->staffspell_timer == 0) && (hero->recipe_timer == 0))
	{

		if (g_travel_by_ship && (random_schick(100) < 10)) {

			/* 9% chance of becoming sea-sick */

			sprintf(g_dtp2,	get_ttx(796), hero->alias);
			GUI_output(g_dtp2);

		} else {

			if (!hero_is_diseased(hero) && !hero_is_poisoned(hero)) {

				if (hero->ruhe_koerper > 0) {
					hero->ruhe_koerper--;
				}

				if (!hero->flags.brewing) {

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
					if ((hero->typus >= HERO_TYPE_HEXE) && (le_regen < ae_regen)) {
						tmp = ae_regen;
						ae_regen = le_regen;
						le_regen = tmp;
					}

					if (hero->ruhe_koerper > 0) {
						le_regen += le_regen;
						ae_regen += ae_regen;
					}

					/* Do LE */

					diff = hero->le_max - hero->le;

					if ((diff != 0) && le_regen) {

						if (diff < le_regen) {
							le_regen = diff;
						}

						add_hero_le(hero, le_regen);

						strcpy(g_text_output_buf, get_ttx(392));

						if (le_regen > 1) {
							strcat(g_text_output_buf, get_ttx(393));
						}

						sprintf(g_dtp2, get_ttx(319), hero->alias, le_regen, g_text_output_buf);

						if (g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {

							GUI_print_loc_line(g_dtp2);
							vsync_or_key(200);

						} else {

							if (g_travel_by_ship) {
								GUI_output(g_dtp2);
							}
						}

					}

					if (hero->typus >= HERO_TYPE_HEXE) {

						diff = hero->ae_max - hero->ae;

						if ((diff != 0) && ae_regen) {

							if (diff < ae_regen) {
								ae_regen = diff;
							}

							add_hero_ae(hero, ae_regen);

							strcpy(g_text_output_buf, get_ttx(392));

							if (ae_regen > 1) {
								strcat(g_text_output_buf, get_ttx(393));
							}

							sprintf(g_dtp2, get_ttx(320), hero->alias, ae_regen, g_text_output_buf);

							if (g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {

								GUI_print_loc_line(g_dtp2);
								vsync_or_key(200);

							} else {
								if (g_travel_by_ship) {
									GUI_output(g_dtp2);
								}
							}
						}
					}
				} else {
					/* hero brewing, HERO_STAFFSPELL_TIMER == 0, HERO_RECIPE_TIMER == 0, not dead, not diseased, not poisoned */
					/* TODO: potential Original-Bug: why is 'not poisoned', 'not diseased' and 'HERO_STAFFSPELL_TIMER == 0' needed to complete the brewing process? */
					do_alchemy(hero, hero->recipe_id, 0);
				}
			} else {
				sprintf(g_dtp2,	get_ttx(558), hero->alias);
				GUI_output(g_dtp2);
			}
		}
	}
}
