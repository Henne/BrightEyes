/**
 *	Rewrite of DSA1 v3.02_de functions of seg034 (fight system)
 *	Functions rewritten: 6/6 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg034.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg036.h"
#include "seg038.h"
#include "seg039.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   TODO
 *
 * \param   x_hero      x-coordinate of the hero
 * \param   y_hero      y-coordinate of the hero
 * \param   x           x-coordinate of the last position
 * \param   y           y-coordinate of the last position
 * \param   x_diff      x-coordinate difference {-1, 0, 1}
 * \param   y_diff      y-coordinate difference {-1, 0, 1}
 * \param   max_range   maximal range for the weapon
 * \return              TODO {0, 1}
 **/
signed short seg034_000(signed short x_hero, signed short y_hero,
			signed short x, signed short y,
			signed short x_diff, signed short y_diff,
			signed short max_range)
{
	signed short fighter_id_target;
	signed short fighter_id;
	signed short beeline;

	fighter_id = get_cb_val(x, y);
	fighter_id_target = get_cb_val(x + x_diff, y + y_diff);

	beeline = calc_beeline(x + x_diff, y + y_diff, x_hero, y_hero);

	if ((fighter_id != 0) && (calc_beeline(x, y, x_hero, y_hero) < beeline) && (beeline <= max_range)) {

		if ((x_hero == x) && (y_hero == y)) {

			if ((fighter_id_target < 50) || ((fighter_id_target >= 50) && is_in_word_array(fighter_id_target - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))))
			{
				return 1;
			} else {
				return 0;
			}

		} else if (((fighter_id >= 50) ||
				((fighter_id >= 10) && (fighter_id < 30) && test_bit0(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + 49) + SIZEOF_ENEMY_SHEET * fighter_id)) ||
				((fighter_id >= 30) && (fighter_id < 50) && test_bit0(p_datseg + ((ENEMY_SHEETS - 30*SIZEOF_ENEMY_SHEET) + 49) + SIZEOF_ENEMY_SHEET * fighter_id)) ||
				((fighter_id < 10) && hero_dead(get_hero(fighter_id - 1))))
				&&
				((fighter_id_target >= 0) &&
				 ((fighter_id_target < 50) || ((fighter_id_target >= 50) && is_in_word_array(fighter_id_target - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))))))
			{

				if (((((x_diff == 1) || (x_diff == -1)) && (y_hero != y))) ||
					((((y_diff == 1) || (y_diff == -1)) && (x_hero != x))))
				{
					return 0;
				} else {
					return 1;
				}

			} else {
				return 0;
			}
	}

	if (x_diff == 1) {
		if ((fighter_id_target >= 0) &&
			((fighter_id_target < 50) || ((fighter_id_target >= 50) && is_in_word_array(fighter_id_target - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))))
			&& ((x < 23) && (y == y_hero) && (calc_beeline(x_hero, y_hero, x + 1, y) <= max_range)))
		{
			return 1;
		} else {
			return 0;
		}

		return 0;
	}

	if (x_diff == -1) {
		if ((fighter_id_target >= 0) &&
			((fighter_id_target < 50) || ((fighter_id_target >= 50) && is_in_word_array(fighter_id_target - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))))
			&& ((x > 0) && (y == y_hero) && (calc_beeline(x_hero, y_hero, x - 1, y) <= max_range)))
		{
			return 1;
		} else {
			return 0;
		}

		return 0;

	}

	if (y_diff == 1) {
		if ((fighter_id_target >= 0) &&
			((fighter_id_target < 50) || ((fighter_id_target >= 50) && is_in_word_array(fighter_id_target - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))))
			&& ((y < 23) && (x == x_hero) && (calc_beeline(x_hero, y_hero, x, y + 1) <= max_range)))
		{
			return 1;
		} else {
			return 0;
		}

		return 0;

	}

	if (y_diff == -1) {
		if ((fighter_id_target >= 0) &&
			((fighter_id_target < 50) || ((fighter_id_target >= 50) && is_in_word_array(fighter_id_target - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))))
			&& ((y > 0) && (x == x_hero) && (calc_beeline(x_hero, y_hero, x, y - 1) <= max_range)))
		{
			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}

/**
 * \brief   select a target for the fight action of the current actor
 *
 * \param   px          x-coordinate of actor
 * \param   py          y-coordinate of actor
 * \param   max_range   maximal range for the weapon
 * \return              fight-id of the target
 */
signed char FIG_cb_select_target(Bit8u *px, Bit8u *py, const signed short max_range)
{
	signed short x_diff;
	signed short y_diff;
	signed short x;
	signed short y;
	signed short fighter_id;
	signed short x_screen;
	signed short y_screen;
	signed short from_kbd;
	/* TODO: the next two variables are constants */
	signed short cb_base_x = 9;
	signed short cb_base_y = 116;

	ds_writew(MOUSE1_EVENT1, ds_writew(MOUSE2_EVENT, 0));

	update_mouse_cursor();

	ds_writew(MOUSE_POSX_BAK, ds_writew(MOUSE_POSX, x_screen = cb_base_x + 10 * (host_readws(px) + host_readws(py))));

	ds_writew(MOUSE_POSY_BAK, ds_writew(MOUSE_POSY, y_screen = cb_base_y + 5 * (host_readws(px) - host_readws(py))));

	mouse_move_cursor(ds_readws(MOUSE_POSX), ds_readws(MOUSE_POSY));

	refresh_screen_size();

	x = host_readws(px);
	y = host_readws(py);

	if (ds_readbs(FIG_CB_SELECTOR_ID) != -1) {
		FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 0);
		ds_writebs(FIG_CB_SELECTOR_ID, -1);
	}

	ds_writew(FIG_LIST_ELEM, 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), host_readbs(px));
	ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), host_readbs(py));
	ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETX), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETY), 4);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_HEIGHT), 11);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_WIDTH), 22);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_X1), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Y1), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_X2), 21);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Y2), 10);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_IS_ENEMY), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_RELOAD), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_WSHEET), -1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_SHEET), -1);
	ds_writed((FIG_LIST_ELEM+FIGHTER_GFXBUF), ds_readd(FIG_CB_SELECTOR_BUF));
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Z), 1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_VISIBLE), 1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), -1);

	ds_writeb(FIG_CB_SELECTOR_ID, FIG_add_to_list(-1));

	draw_fight_screen_pal(0);

	do {
		handle_input();

		if ((ds_readws(ACTION) == ACTION_ID_ESC) ||
			(ds_readws(MOUSE2_EVENT) != 0)) {

			/* cancel */

			ds_writew(MOUSE2_EVENT, 0);

			FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 0);

			ds_writeb(FIG_CB_SELECTOR_ID, -1);

			draw_fight_screen_pal(0);

			return -99;
		}

		from_kbd = 0;

		if ((ds_readws(ACTION) == ACTION_ID_UP) ||
			(ds_readws(ACTION) == ACTION_ID_DOWN) ||
			(ds_readws(ACTION) == ACTION_ID_RIGHT) ||
			(ds_readws(ACTION) == ACTION_ID_LEFT))
		{
			from_kbd = 1;
		} else {

			x_diff = ds_readws(MOUSE_POSX) - x_screen;
			y_diff = ds_readws(MOUSE_POSY) - y_screen;

			if (((y_diff > 0) && (x_diff <= -10)) ||
				((x_diff < 0) && (y_diff >= 5)))
			{
				ds_writew(ACTION, ACTION_ID_DOWN);

			} else if (((y_diff < 0) && (x_diff >= 10)) ||
					((x_diff > 0) && (y_diff <= -5)))
			{
				ds_writew(ACTION, ACTION_ID_UP);

			} else if (((y_diff > 0) && (x_diff >= 10)) ||
					((x_diff > 0) && (y_diff >= 5)))
			{
				ds_writew(ACTION, ACTION_ID_RIGHT);

			} else if (((y_diff < 0) && (x_diff <= -10)) ||
					((x_diff < 0) && (y_diff <= -5)))
			{
				ds_writew(ACTION, ACTION_ID_LEFT);
			}
		}

		if (ds_readws(MOUSE1_EVENT1) != 0) {
			ds_writew(ACTION, ACTION_ID_RETURN);
			ds_writew(MOUSE1_EVENT1, 0);
		}

		if (ds_readws(ACTION) == ACTION_ID_RIGHT) {

			if (seg034_000(x, y, host_readws(px), host_readws(py), 1, 0, max_range)) {
				inc_ptr_ws(px);
			}
		} else if (ds_readws(ACTION) == ACTION_ID_LEFT) {
			if (seg034_000(x, y, host_readws(px), host_readws(py), -1, 0, max_range)) {
				dec_ptr_ws(px);
			}
		} else if (ds_readws(ACTION) == ACTION_ID_UP) {
			if (seg034_000(x, y, host_readws(px), host_readws(py), 0, 1, max_range)) {
				inc_ptr_ws(py);
			}
		} else if (ds_readws(ACTION) == ACTION_ID_DOWN) {
			if (seg034_000(x, y, host_readws(px), host_readws(py), 0, -1, max_range)) {
				dec_ptr_ws(py);
			}
		}

		if ((ds_readbs((FIG_LIST_ELEM+FIGHTER_CBX)) != host_readws(px)) || (ds_readbs((FIG_LIST_ELEM+FIGHTER_CBY)) != host_readws(py))) {

			update_mouse_cursor();

			x_screen = cb_base_x + 10 * (host_readws(px) + host_readws(py));
			y_screen = cb_base_y + 5 * (host_readws(px) - host_readws(py));

			if (from_kbd != 0) {
				ds_writew(MOUSE_POSX_BAK, ds_writew(MOUSE_POSX, x_screen));
				ds_writew(MOUSE_POSY_BAK, ds_writew(MOUSE_POSY, y_screen));

				mouse_move_cursor(ds_readws(MOUSE_POSX), ds_readws(MOUSE_POSY));
			}

			refresh_screen_size();
			FIG_call_draw_pic();
			fighter_id = get_cb_val(host_readws(px), host_readws(py));

			if ((fighter_id > 0) && (fighter_id < 50)) {

				if (fighter_id < 10) {
					FIG_draw_char_pic(1, fighter_id);
				} else {

					if (fighter_id >= 30) {
						FIG_draw_enemy_pic(1, fighter_id - 20);
					} else {
						FIG_draw_enemy_pic(1, fighter_id);
					}
				}
			}

			FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 1);

			ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), (signed char)host_readws(px));
			ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), (signed char)host_readws(py));

			FIG_add_to_list(ds_readbs(FIG_CB_SELECTOR_ID));
			FIG_draw_figures();
			FIG_set_gfx();
		}

	} while (ds_readws(ACTION) != ACTION_ID_RETURN);

	FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 0);

	ds_writeb(FIG_CB_SELECTOR_ID, -1);

	draw_fight_screen_pal(0);

	return get_cb_val(host_readws(px), host_readws(py));
}

struct coord {
	signed short x;
	signed short y;
};

struct dummy {
	struct coord a[4];
};

/* determine free position (*px,*py) for new enemy to appear on chessboard
 * if the desired position (x,y) is occupied, a free position is determined as
 * close as possible to this position.
 * mode is 1 if the enemy has a two-fielded sprite (such as wolves)
 */
void seg034_718(signed short x, signed short y, Bit8u *px, Bit8u *py, signed short dir, signed short mode)
{
	signed short new_dir;
	signed short dist;
	signed short new_x;
	signed short new_y;
	signed char done;

#if !defined(__BORLANDC__)
        struct dummy a;
        a.a[0].x = 1;
        a.a[0].y = 0;
        a.a[1].x = 0;
        a.a[1].y = -1;
        a.a[2].x = -1;
        a.a[2].y = 0;
        a.a[3].x = 0;
        a.a[3].y = 1;
#else
        struct dummy a = *(struct dummy*)(p_datseg + VIEWDIR_OFFSETS1);
#endif

	done = 0;

	host_writew(px, x);
	host_writew(py, y);

	if (get_cb_val(x, y) == 0) {

		if (mode == 0) {
			return;
		}

		if (get_cb_val(x - a.a[dir].x, y - a.a[dir].y) == 0) {
			return;
		}
	}

	dist = 1;

	while (!done) {

		for (new_dir = 0; new_dir < 4; new_dir++) {

			new_x = host_readws(px) + a.a[new_dir].x * dist;
			new_y = host_readws(py) + a.a[new_dir].y * dist;

			if ((new_x >= 0) && (new_x < 24) && (new_y >= 0) && (new_y < 24) &&
				!get_cb_val(new_x, new_y))
			{

				if ((mode == 0) ||
					(!get_cb_val(new_x - a.a[dir].x, new_y - a.a[dir].y)))
				{

					done = 1;
					host_writew(px, new_x);
					host_writew(py, new_y);
					break;
				}
			}
		}

		dist++;
	}
}

/**
 * \brief   add monsters to the fight, which appear in later rounds
 */
void FIG_latecomers(void)
{
	signed short i;
	signed short x;
	signed short y;
	signed short l4;
	Bit8u *p_mon;		/* pointer to a monster sheet */
	Bit8u *p2;
	Bit8u *p3;

#if !defined(__BORLANDC__)
        struct dummy a;
        a.a[0].x = 1;
        a.a[0].y = 0;
        a.a[1].x = 0;
        a.a[1].y = -1;
        a.a[2].x = -1;
        a.a[2].y = 0;
        a.a[3].x = 0;
        a.a[3].y = 1;
#else
        struct dummy a = *(struct dummy*)(p_datseg + VIEWDIR_OFFSETS2);
#endif

	/* for all enemies in this fight */
	for (i = 0; i < ds_readws(NR_OF_ENEMIES); i++) {

		p_mon = p_datseg + ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * i;

		/* if monster has not appeared */
		if (host_readbs(p_mon + ENEMY_SHEET_ROUND_APPEAR) > 0) {

			/* decrement counter */
			dec_ptr_bs(p_mon + ENEMY_SHEET_ROUND_APPEAR);

			if (!host_readbs(p_mon + ENEMY_SHEET_ROUND_APPEAR)) {
				/* let monster enter the fight */

				if (!enemy_scared(p_mon)) {

					if (is_in_byte_array(host_readbs(p_mon + ENEMY_SHEET_GFX_ID), p_datseg + TWO_FIELDED_SPRITE_ID)) {

						seg034_718(	host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_X + SIZEOF_FIGHT_MONSTER * i),
								host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_Y + SIZEOF_FIGHT_MONSTER * i),
								(Bit8u*)&x, (Bit8u*)&y,
								host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_VIEWDIR + SIZEOF_FIGHT_MONSTER * i),
								1);

						p2 = (Bit8u*)(FIG_get_ptr(host_readbs(p_mon + ENEMY_SHEET_FIGHTER_ID)));

						host_writebs(p2 + 3, (signed char)x);
						host_writebs(p2 + 4, (signed char)y);

						l4 = ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(p2 + FIGHTER_TWOFIELDED));

						p3 = (Bit8u*)(FIG_get_ptr((signed char)l4));

						host_writeb(p3 + 3,
								x - a.a[host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_VIEWDIR + SIZEOF_FIGHT_MONSTER * i)].x);
						host_writeb(p3 + 4,
								y - a.a[host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_VIEWDIR + SIZEOF_FIGHT_MONSTER * i)].y);

						FIG_remove_from_list(host_readbs(p_mon + ENEMY_SHEET_FIGHTER_ID), 1);

						FIG_add_to_list(host_readbs(p_mon + ENEMY_SHEET_FIGHTER_ID));

						FIG_remove_from_list((signed char)l4, 1);

						FIG_add_to_list((signed char)l4);
					} else {
						seg034_718(	host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_X + SIZEOF_FIGHT_MONSTER * i),
								host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_Y + SIZEOF_FIGHT_MONSTER * i),
								(Bit8u*)&x, (Bit8u*)&y,
								host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_VIEWDIR + SIZEOF_FIGHT_MONSTER * i),
								0);

						p2 = (Bit8u*)(FIG_get_ptr(host_readbs(p_mon + ENEMY_SHEET_FIGHTER_ID)));

						host_writebs(p2 + 3, (signed char)x);
						host_writebs(p2 + 4, (signed char)y);

						FIG_remove_from_list(host_readbs(p_mon + ENEMY_SHEET_FIGHTER_ID), 1);

						FIG_add_to_list(host_readbs(p_mon + ENEMY_SHEET_FIGHTER_ID));
					}

					place_obj_on_cb(x, y, i + 10, host_readbs(p_mon + ENEMY_SHEET_GFX_ID),
						(signed short)host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_MONSTERS_VIEWDIR + SIZEOF_FIGHT_MONSTER * i));

					FIG_make_visible(host_readbs(p_mon + ENEMY_SHEET_FIGHTER_ID));

				} else {
					/* scared enemies entering a fight are marked as dead. does this ever happen? */
					or_ptr_bs(p_mon + ENEMY_SHEET_FLAGS1, 1); /* set 'dead' flag */
				}
			}
		}
	}
}

signed short FIG_move_pathlen(void)
{
	signed short i = 0;

	/* count everything till the end marker -1 of the path */
	while (ds_readbs(FIG_MOVE_PATHDIR + i) != -1) {
		i++;
	}

	/* if the end marker -1 is followed by a -2, the available BP are not sufficient */
	if (ds_readbs((FIG_MOVE_PATHDIR + 1) + i) == -2) {
		return 99;
	}

	return i;
}

/**
 * \brief   Moves a hero on the chessboard
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero in the group
 * \param   px          pointer to the x-coordinate on the chessboard
 * \param   py          pointer to the y-coordinate on the chessboard
 */
void FIG_move_hero(Bit8u *hero, signed short hero_pos, Bit8u *px, Bit8u *py)
{
	signed short problem;
	signed short path_end;
	signed short sel_x;
	signed short sel_y;
	signed short target_reachable;
	signed short curr_x;
	signed short curr_y;
	signed short fg_bak;
	signed short bg_bak;
	signed short x_screen;
	signed short y_screen;
	signed short from_kbd;
	signed short base_x = 9;
	signed short base_y = 116;
	signed char cb_entry_bak;
	signed char bp_cost;
	signed char cb_entry_bak_escape;
	signed short escape_dir; /* 0: no escape; 1: left; 2: down; 3: up. not compatible with the directions in the path! */
	signed short mouse_cb_x;
	signed short mouse_cb_y;

	update_mouse_cursor();

	ds_writew(MOUSE_POSX_BAK, ds_writew(MOUSE_POSX, x_screen = base_x + 10 * (host_readws(px) + host_readws(py))));

	ds_writew(MOUSE_POSY_BAK, ds_writew(MOUSE_POSY, y_screen = base_y + 5 * (host_readws(px) - host_readws(py))));

	mouse_move_cursor(ds_readws(MOUSE_POSX), ds_readws(MOUSE_POSY));

	refresh_screen_size();

	ds_writew(MOUSE1_EVENT1, ds_writew(MOUSE2_EVENT, 0));

	sel_x = host_readws(px);
	sel_y = host_readws(py);
	curr_x = sel_x;
	curr_y = sel_y;

	if (ds_readbs(FIG_CB_SELECTOR_ID) != -1) {
		FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 0);
		ds_writebs(FIG_CB_SELECTOR_ID, -1);
	}

	ds_writew(FIG_LIST_ELEM, 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), (signed char)sel_x);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), (signed char)sel_y);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETX), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETY), 4);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_X1), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Y1), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_X2), 21);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Y2), 10);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_HEIGHT), 11);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_WIDTH), 22);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_IS_ENEMY), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_RELOAD), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_WSHEET), -1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_SHEET), -1);
	ds_writed((FIG_LIST_ELEM+FIGHTER_GFXBUF), ds_readd(FIG_CB_SELECTOR_BUF));
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Z), 1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_VISIBLE), 1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), -1);

	ds_writeb(FIG_CB_SELECTOR_ID, FIG_add_to_list(-1));

	draw_fight_screen_pal(0);

	do {
		handle_input();

		curr_x = sel_x;
		curr_y = sel_y;
		from_kbd = 0;

		if ((ds_readws(ACTION) == ACTION_ID_UP) ||
			(ds_readws(ACTION) == ACTION_ID_DOWN) ||
			(ds_readws(ACTION) == ACTION_ID_RIGHT) ||
			(ds_readws(ACTION) == ACTION_ID_LEFT))
		{
			from_kbd = 1;
		} else {

			mouse_cb_x = ((ds_readws(MOUSE_POSX) - base_x) / 10 + (ds_readws(MOUSE_POSY) - base_y) / 5) / 2;
			mouse_cb_y = -((ds_readws(MOUSE_POSY) - base_y) / 5 - mouse_cb_x);

			if ((mouse_cb_x != sel_x) || (mouse_cb_y != sel_y)) {

				if ((mouse_cb_x >= -1) && (mouse_cb_x <= 24) && (mouse_cb_y >= -1) && (mouse_cb_y <= 24)) {
					ds_writew(ACTION, ACTION_ID_VOID);
				}
			}
		}

		if (ds_readws(MOUSE1_EVENT1) != 0) {
			ds_writew(MOUSE1_EVENT1, 0);
			ds_writew(ACTION, ACTION_ID_RETURN);
		}

		if ((ds_readws(ACTION) == ACTION_ID_RIGHT) && (sel_x < 23)) {
			sel_x++;
		} else if ((ds_readws(ACTION) == ACTION_ID_LEFT) && (sel_x >= 0)) {
			sel_x--;
		} else if ((ds_readws(ACTION) == ACTION_ID_UP) && (sel_y <= 23)) {
			sel_y++;
		} else if ((ds_readws(ACTION) == ACTION_ID_DOWN) && (sel_y >= 0)) {
			sel_y--;
		} else if (ds_readws(ACTION) == ACTION_ID_VOID) {
			sel_x = mouse_cb_x;
			sel_y = mouse_cb_y;
		}

		/* treatment of the two visible corners of the map */
		if ((sel_x < 0) && (sel_y < 0)) {
			sel_x = 0;
		}
		if ((sel_x < 0) && (sel_y > 23)) {
			sel_x = 0;
		}

		if ((sel_x != curr_x) || (sel_y != curr_y)) {

			update_mouse_cursor();

			x_screen = base_x + 10 * (sel_x + sel_y);
			y_screen = base_y + 5 * (sel_x - sel_y);

			if (from_kbd != 0) {
				ds_writew(MOUSE_POSX_BAK, ds_writew(MOUSE_POSX, x_screen));

				ds_writew(MOUSE_POSY_BAK, ds_writew(MOUSE_POSY, y_screen));

				mouse_move_cursor(ds_readws(MOUSE_POSX), ds_readws(MOUSE_POSY));
			}

			refresh_screen_size();

			FIG_call_draw_pic();

			FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 0);

			ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), (signed char)sel_x);
			ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), (signed char)sel_y);
			ds_writed((FIG_LIST_ELEM+FIGHTER_GFXBUF), ds_readd(FIG_CB_SELECTOR_BUF));
			ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), -1);
			ds_writeb(FIG_CB_SELECTOR_ID, FIG_add_to_list(-1));

			FIG_draw_figures();
			FIG_set_gfx();

			problem = 0;
			bp_cost = 0;

			if ((host_readws(px) != sel_x) || (host_readws(py) != sel_y)) { /* selected square is different from active hero position */

				escape_dir = 0;

				/* (sel_x > 23) is missing as the right border of the map is not visible, so it cannot be used for escape (I guess...) */
				if ((sel_x < 0) || (sel_y < 0) || (sel_y > 23)) {
					/* active hero wants to escape over the border of the map */

					/* reset (sel_x, sel_y) to the square at the border of the map. remember the direction of the escape */
					if (sel_x < 0) {
						sel_x = 0;
						escape_dir = 1;
					} else if (sel_y < 0) {
						sel_y = 0;
						escape_dir = 2;
					} else {
						sel_y = 23;
						escape_dir = 3;
					}

					if ((get_cb_val(sel_x, sel_y) != 0) &&
						((host_readws(px) != sel_x) || (host_readws(py) != sel_y)))
					{
						/* the reset square (sel_x, sel_y) is blocked and different from the position of the active hero */

						problem = 3; /* target square blocked */

						/* again reset (sel_x, sel_y) to the square beyond the border */
						if (escape_dir == 1) {
							sel_x = -1;
						} else if (escape_dir == 2) {
							sel_y = -1;
						} else {
							/*  escape_dir == 3 */
							sel_y = 24;
						}

					} else {
						cb_entry_bak_escape = get_cb_val(sel_x, sel_y);
					}
				} else {
					cb_entry_bak = get_cb_val(sel_x, sel_y);
				}

				if (problem != 3) {

					if ((escape_dir != 0) && (host_readws(px) == sel_x) && (host_readws(py) == sel_y))
					{
						/* active hero wants to escape over the border of the map and is already at the border. */
						ds_writeb(FIG_MOVE_PATHDIR, -1);
						bp_cost = 0;
					} else {
						FIG_set_cb_field(sel_y, sel_x, 124); /* target marker for FIG_find_path_to_target. The original content of this square has been backuped before in 'cb_entry_bak' or 'cb_entry_bak_escape'. */
						target_reachable = FIG_find_path_to_target(hero, hero_pos, host_readws(px), host_readws(py), 10);
						/* target_reachable = 1: there is a path of length < 50 to the target square; target_reachable = -1: there is no such path */
						bp_cost = (signed char)FIG_move_pathlen();
#if !defined(__BORLANDC__)
						D1_INFO("x: %d, y: %d, target id: %d, reachable: %d, distance: %d\n", escape_dir?cb_entry_bak_escape:sel_x, sel_y, cb_entry_bak, target_reachable, bp_cost);
#endif
					}

					if (escape_dir != 0) {
						/* restore the original entry of the target square, which has been overwritten by the target marker. */
						FIG_set_cb_field(sel_y, sel_x, cb_entry_bak_escape);

						path_end = 0;
						while (ds_readbs(FIG_MOVE_PATHDIR + path_end) != -1) {
							path_end++;
						}

						/* add the last escape step to the path */
						if (escape_dir == 1) {
							sel_x = -1;
							if (host_readbs(hero + HERO_BP_LEFT) > bp_cost) {
								ds_writeb((FIG_MOVE_PATHDIR + 0) + path_end, 2);
								ds_writeb((FIG_MOVE_PATHDIR + 1) + path_end, -1);
							}
						} else if (escape_dir == 2) {
							sel_y = -1;
							if (bp_cost < (host_readbs(hero + HERO_BP_LEFT) - 1)) {
								ds_writeb((FIG_MOVE_PATHDIR + 0) + path_end, 1);
								ds_writeb((FIG_MOVE_PATHDIR + 1) + path_end, -1);
							}
						} else {
							sel_y = 24;
							if (bp_cost < (host_readbs(hero + HERO_BP_LEFT) - 1)) {
								ds_writeb((FIG_MOVE_PATHDIR + 0) + path_end, 3);
								ds_writeb((FIG_MOVE_PATHDIR + 1) + path_end, -1);
							}
						}

						bp_cost++;
					} else {
						/* restore the original entry of the target square, which has been overwritten by the target marker. */
						FIG_set_cb_field(sel_y, sel_x, cb_entry_bak);
					}

					if (cb_entry_bak >= 50) {
						problem = 3; /* target square blocked */
#ifndef M302de_ORIGINAL_BUGFIX
					} else if (cb_entry_bak >= 10) {
						/* target square contains a monster (including the tail of a two-squares monster) */
						if (!test_bit0(p_datseg + (ENEMY_SHEETS + ENEMY_SHEET_FLAGS1) + SIZEOF_ENEMY_SHEET * (cb_entry_bak - 10 - (cb_entry_bak >= 30 ? 20 : 0)))) /* check 'dead' flag */
						{
							/* monster is not dead */
							problem = 3;
						}
						/* Original-Bug:
						 * If the target square contains a dead monster,
						 * the checks for target_reachable and bp_cost below are not executed because of the "nested if" structure of the code.
						 * This results in the incorrect message "ZIEL: 99 BP" if the target is pointing at a dead monster
						 * which is not reachable within the available BP or not reachable at all. */
					} else if (cb_entry_bak > 0) {
						/* target square contains a hero */
						if (!hero_dead(get_hero(cb_entry_bak - 1)) &&
							!hero_unconscious(get_hero(cb_entry_bak - 1)) &&
							(cb_entry_bak != hero_pos + 1))
						{
							/* hero is not dead, not unconscious, and not the active hero */
							problem = 3;
						}
						/* Original-Bug:
						 * If the target square contains a dead or unconscious hero different from the active hero,
						 * the checks for target_reachable and bp_cost below are not executed because of the "nested if" structure of the code.
						 * This results in the incorrect message "ZIEL: 99 BP" if the target is pointing at a dead/unconscious hero
						 * which is not reachable within the available BP or not reachable at all. */
#else
						/* Bug fix:
						 * flatten the nested if branches. */
					} else if ((cb_entry_bak >= 10) && (!test_bit0(p_datseg + (ENEMY_SHEETS + ENEMY_SHEET_FLAGS1) + SIZEOF_ENEMY_SHEET * (cb_entry_bak - 10 - (cb_entry_bak >= 30 ? 20 : 0))))) { /* check 'dead' flag */
						/* target square contains a non-dead monster (including the tail of a two-squares monster) */
						problem = 3;
					} else if ((cb_entry_bak > 0) && (cb_entry_bak < 10) && !hero_dead(get_hero(cb_entry_bak - 1)) && !hero_unconscious(get_hero(cb_entry_bak - 1)) && (cb_entry_bak != hero_pos + 1)) {
						/* target square contains a non-dead and non-unconscious hero different from the active hero */
						problem = 3;
#endif
					} else if (target_reachable == -1) {
						problem = 4;
					} else if (host_readbs(hero + HERO_BP_LEFT) < bp_cost) {
						problem = 2;
					} else if ((sel_x > 23) || (sel_x < 0) || (sel_y > 23) || (sel_y < 0) || (get_cb_val(sel_x, sel_y) < 0)) {
						if ((host_readbs((Bit8u*)ds_readd(SCENARIO_BUF) + 0x14) > 3) || (sel_x >= 0))
						{
							problem = 1;
						}
					}
				}
			}

			get_textcolor(&fg_bak, &bg_bak);
			set_textcolor(255, 0);

			if (!problem) {
				sprintf(g_dtp2,
					get_tx(12), bp_cost); /* Target: %d BP */
			}


			GUI_print_string(problem == 1 ? get_tx(13) : /* Target: escape */
						(problem == 3 ? get_tx(30) : /* Target: blocked */
						(problem == 2 ? get_tx(32) : /* Target: too far */
						(problem == 4 ? get_tx(48) : /* Target: no way */
						g_dtp2))), /* Target: %d BP */
					 5, 190);

			set_textcolor(fg_bak, bg_bak);
		}

		if ((ds_readws(MOUSE2_EVENT) != 0) || (ds_readws(ACTION) == ACTION_ID_ESC)) {
			ds_writew(MOUSE2_EVENT, 0);
			ds_writew(ACTION, ACTION_ID_RETURN);
			problem = 5;
		}

	} while (ds_readws(ACTION) != ACTION_ID_RETURN);

	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(255, 0);
	GUI_print_string((char*)p_datseg + STRING_14SPACES, 5, 190);
	set_textcolor(fg_bak, bg_bak);

	if ((host_readws(px) != sel_x) || (host_readws(py) != sel_y)) {

		if (!problem || (problem == 1) || (problem == 2)) {

			if (problem == 1) {

				if (GUI_bool(get_tx(35))) {
					host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_FLEE);
					problem = 0;
				} else {
					FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 0);
					ds_writeb(FIG_CB_SELECTOR_ID, -1);
				}
			}

			if (!problem || (problem == 2)) {

				FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 0);
				ds_writeb(FIG_CB_SELECTOR_ID, -1);

				seg036_00ae(hero, hero_pos);

				if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_FLEE) {
					host_writeb(hero + HERO_BP_LEFT, 0);
				} else {
					FIG_search_obj_on_cb(hero_pos + 1, (signed short*)px, (signed short*)py);
				}
			}

		} else if (problem == 3) {
			GUI_output(get_tx(49));
		} else if (problem == 4) {
			GUI_output(get_tx(50));
		}
	}

	if (ds_readbs(FIG_CB_SELECTOR_ID) != -1) {
		FIG_remove_from_list(ds_readbs(FIG_CB_SELECTOR_ID), 0);
		ds_writebs(FIG_CB_SELECTOR_ID, -1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
