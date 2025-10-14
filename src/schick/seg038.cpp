/*
 *      Rewrite of DSA1 v3.02_de functions of seg038 (Fight)
 *      Functions rewritten: 6/6
 *
 *      Functions called rewritten 5/5
 *      Functions uncalled rewritten 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg038.cpp
 */
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg006.h"
#include "seg007.h"
#include "seg038.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   searches an object on the chessboard
 *
 * \param   obj         number of the object
 * \param   px          pointer where x should be stored
 * \param   py          pointer where y should be stored
 * \return              0 if the object was not found. If the object was
 * found it returns 1 and stores the coordinates at the pointers.
 */
signed int FIG_search_obj_on_cb(const signed short obj, signed short *px, signed short *py)
{
	/* This is the Endian save version */
	for (*px = 0; *px < 25; (*px)++) {
		for (*py = 0; *py < 24; (*py)++) {
			if (get_cb_val(*px, *py) == obj) {
				return 1;
			}
		}
	}

	return 0;
}

void FIG_init_list_elem(signed short obj)
{

	signed short x, y;

	FIG_search_obj_on_cb(obj, &x, &y);

	/* This initializes the global FIGHTER structure g_fig_list_elem */
	g_fig_list_elem.figure = 0;
	g_fig_list_elem.nvf_no = 0;
	g_fig_list_elem.cbx = (signed char)x;
	g_fig_list_elem.cby = (signed char)y;
	g_fig_list_elem.offsetx = 0;
	g_fig_list_elem.offsety = 4;
	g_fig_list_elem.height = 11;
	g_fig_list_elem.width = 22;
	g_fig_list_elem.x1 = 0;
	g_fig_list_elem.y1 = 0;
	g_fig_list_elem.x2 = 21;
	g_fig_list_elem.y2 = 10;

	g_fig_list_elem.is_enemy = 0;
	g_fig_list_elem.reload = 0;
	g_fig_list_elem.wsheet = -1;
	g_fig_list_elem.sheet = -1;

	g_fig_list_elem.gfxbuf = g_fig_cb_marker_buf;
	g_fig_list_elem.z = 0;
	g_fig_list_elem.visible = 1;
	g_fig_list_elem.twofielded = -1;

	g_fig_cb_marker_id = FIG_add_to_list(-1);
}

#if defined(__BORLANDC__)
void FIG_unused(const signed short a1, const signed short a2, signed short *p1, signed short *p2)
{
	signed short loc1 = 10;
	signed short loc2 = 118;

	*p2 = ((loc2 - a2) + ((a1 - loc1) / 2)) / 10;
	*p1 = (a1 - loc1) / 10 - *p2;
}
#endif

static signed char *g_chessboard_cpy; // ds:0xe356

/**
 * \brief  Computes path via backtracking on the distance table
 *
 * \param   dist_table_ptr    pointer to the distance table
 * \param   target_x          x-coordinate of the target
 * \param   target_y          y_coordinate of the target
 * \param   dist              distance to the target
 * \param   bp_avail          left BP of the actor
 * \param   mode              mode (see FIG_find_path_to_target)
 * \param   two_squares       actor occupies two squares on the map (wolves, dogs, lions)
 */
void FIG_find_path_to_target_backtrack(Bit8u *dist_table_ptr, signed short target_x, signed short target_y,
			signed short dist, signed char bp_avail,
			signed short mode, signed short two_squares, signed short actor_id)
{
	signed short i;
	signed short dist_duplicate; /* duplicates the dist variable. apparently redundant */
	signed short backtrack_x;
	signed short backtrack_y;
	signed short cb_or_dist_entry; /* used for both a chessboard entry and as a distance table entry */
	signed short dist_bak;
	signed short target_is_escape_square = 0;
	signed short tail_x;
	signed short tail_y;
	signed short dir;
	signed short success;
	signed short lowest_nr_dir_changes;
	signed short nr_dir_changes;
#ifndef M302de_ORIGINAL_BUGFIX
	/* potential Original-Bug:
	 * best_dir is not initialized and may stay so in case that FIG_find_path_to_target_backtrack is called with equal target and hero/enemy position.
	 * It's not clear however if this does indeed happen.
	 * See https://www.crystals-dsa-foren.de/showthread.php?tid=5383&pid=155007#pid155007
	 */
	signed short best_dir;
#else
	signed short best_dir = 0;
#endif
	Bit8s *path_cur;
	signed short x_bak;
	signed short y_bak;
	signed short target_out_of_reach; /* will be set to 1 if the target is out of reach with avail_bp steps. Redundant, as this could simply be tested by (avail_bp < dist). */
	struct viewdir_offsets inverse_coordinate_offset = g_viewdir_invoffsets1;

	Bit8s *path_table[4];

	target_out_of_reach = 0;
	lowest_nr_dir_changes = 99;

	memset(g_text_output_buf, 0, 80);
	path_table[0] = (Bit8s*)g_text_output_buf;
	path_table[1] = (Bit8s*)g_text_output_buf + 20;
	path_table[2] = (Bit8s*)g_text_output_buf + 40;
	path_table[3] = (Bit8s*)g_text_output_buf + 60;

	cb_or_dist_entry = get_cb_val(target_x, target_y); /* possibly reads out of the boundary of the chessboard. not critical, as the following condition is always true for coordinates (target_x, target_y) out of the map. */

	if ((cb_or_dist_entry < 0) || (target_y < 0) || (target_y > 23) || (target_x < 0) || (target_x > 23)) {
		target_is_escape_square = 1;
	}

	dist_bak = dist;
	x_bak = target_x;
	y_bak = target_y;

	/* the following appears to be a simplistic way to produce paths with few (but not necessarily the fewest possible number of) direction changes.
	 * the value i in the following loop is the "preferred" direction. The backtracking always tries this direction first. */
	for (i = 0; i < 4; i++) {

		dist = dist_bak;
		target_x = x_bak;
		target_y = y_bak;
		dir = i; /* start with the preferred direction */

		path_cur = path_table[i];
		dist_duplicate = dist;

		path_cur[dist_duplicate] = -1;
		dist_duplicate--;
		dist--;

		while (dist >= 0) {

			success = 0;

			while (success == 0) {

				backtrack_y = target_y + inverse_coordinate_offset.a[dir].y;
				backtrack_x = target_x + inverse_coordinate_offset.a[dir].x;
				tail_y = backtrack_y + inverse_coordinate_offset.a[dir].y;
				tail_x = backtrack_x + inverse_coordinate_offset.a[dir].x;

				if ((backtrack_y < 24) && (backtrack_y >= 0) && (backtrack_x < 24) && (backtrack_x >= 0))
				{
					cb_or_dist_entry = host_readbs(dist_table_ptr + (25 * backtrack_y) + backtrack_x);

					if ((cb_or_dist_entry == dist)	&&
						((!two_squares) ||
						/* Original-Bug
						 * A fight with two-squares enemies may freeze in an infinite loop here.
						 * The following check of the tail-condition (space for the tail part of a two-squares monster) is executed for every single backtracking step.
						 * However, in the function FIG_find_path_to_target this check is not applied for the last step to the target of a fleeing two-squares monster.
						 * Fix: don't apply the check in the last step, i.e. the first step of the backtracking.
						 * (another, maybe better fix would be to add the missing tests of the tail-condition in FIG_find_path_to_target)
						 * See discussion at https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=165957#pid165957
						 */
#ifdef M302de_ORIGINAL_BUGFIX
						(dist == dist_bak-1) ||
#endif
						(
							two_squares && /* this check is redundant, as we had (!two_squares) || ... before */
							(!*(g_chessboard_cpy + (tail_y * 25) + tail_x) || /* square is empty */
							(*(g_chessboard_cpy + (tail_y * 25) + tail_x) == (actor_id + 10)) || /* head of active enemy is on square */
							(*(g_chessboard_cpy + (tail_y * 25) + tail_x) == (actor_id + 30))) && /* tail of active enemy is on square */
							(tail_y < 24) && (tail_y >= 0) && (tail_x < 24) && (tail_x >= 0))))
					{
						target_y = backtrack_y;
						target_x = backtrack_x;

						if (bp_avail <= dist) {
							/* if the old square (target_x, target_y) (before updating to (backtrack_x, backtrack_y)) cannot be reached with the available BP, write -1 to the path */
							path_cur[dist_duplicate] = -1;
							dist_duplicate--;
							target_out_of_reach = 1;
						} else {
							/* otherwise, write the found direction to the path */
							path_cur[dist_duplicate] = (signed char)dir;
							dist_duplicate--;
						}

						dist--;
						success = 1;
					}
				}

				if (success == 0) {

					dir++;

					if  (dir == 4) {
						dir = 0;
					}
				}
			}
		}

		if ((bp_avail >= dist_bak) && (target_is_escape_square == 0) && (mode < 6))
		/* if mode < 6, i.e. the mode is melee attack or flee, and the target is an escape square which can be reached with the available BP.
		 * As the target square is always an escape square for the flee mode, this should be logically equivalent to (bp_avail >= dist_bak) && (mode < 4) */
		{
			/* remove the last step from the path (as for melee attack, the target should not be entered) */
			path_cur[dist_bak - 1] = -1;
		}

		if (*path_cur != -1) {

			nr_dir_changes = FIG_count_direction_changes_of_path(path_cur);

			if (nr_dir_changes < lowest_nr_dir_changes) {

				best_dir = i;
				lowest_nr_dir_changes = nr_dir_changes;

				if (nr_dir_changes == 0) {
					break;
				}
			}
		}
	}

	/* TODO: g_fig_move_pathdir is only 10 bytes, but should be enlarged */
	memcpy(g_fig_move_pathdir, path_table[best_dir], 20);

	/* In the way the path has been created, it is terminated by at least one symbol -1.
	 * In the case that the path doesnt reach the target (because of insufficient bp_avail),
	 * after the (first) end marker -1 another marker -2 is appended. */
	if (target_out_of_reach) {

		i = 0;

		while (g_fig_move_pathdir[i++] != -1);
		g_fig_move_pathdir[i] = -2;
	}
}

//static
signed short FIG_count_direction_changes_of_path(signed char *path_ptr)
{

	signed short i = 0;
	signed short count = 0;

	if (path_ptr[0] == -1) {
		return 99;
	}

	i++;
	while (path_ptr[i] != -1) {
		if (path_ptr[i - 1] != path_ptr[i]) {
			count++;
		}

		i++;
	}

	return count;
 }

 /**
  * \brief  Find target for actor and compute path
  *
  * Searches for a target (depending on given mode) for a actor and computes a path to it. The computed path is written at g_fig_move_pathdir in the data segment.
  * The path is a sequence of the following symbols:
  * 0: right / 1: down / 2: left / 3: up
  * -1: end marker of the path. The length of the path (without end marker) is at most the number of available BP of the actor.
  * -2: the end marker of the path will be followed by -2, if the available BP of the actor are not sufficient to reach the target.
  *
  * A return value of 1 means that there is a path of length <50 from the actor to a target (depending on mode), independently of the available BP.
  *
  * \param   actor_ptr      pointer to a actor (hero or enemy, depending on mode)
  * \param   actor_id       id of actor. hero: range 1..9 or; monster: range 10..29.
  * \param   x_in           x-coordinate of actor
  * \param   y_in           y-coordinate of actor
  * \param   mode           0: enemy to hero melee / 1: hero to hero melee / 2: enemy to enemy melee / 3: hero to enemy melee / 4: enemy is fleeing / 5: hero is fleeing / 6: enemy to hero ranged /  7: enemy to enemy ranged / 8: hero to hero ranged / 9: hero to enemy ranged / 10: hero movement (target marker 124 on the chess board)
  * \return                 1: reachable target found, path written / -1: no reachable target found.
  */
signed short FIG_find_path_to_target(Bit8u *actor_ptr, signed short actor_id, signed short x_in, signed short y_in, signed short mode)
 {
	signed short nr_targets_reached;
	signed short i;
	signed short target_reached = 0;
	signed short dist = 0;
	signed short new_squares_reached;
	signed short new_x;
	signed short new_y;
	signed short x;
	signed short y;
	signed short tail_x;
	signed short tail_y;
	signed short dir;
	signed short nr_dir_changes;
	signed short lowest_nr_dir_changes;
	signed short best_target;
	signed char cb_or_dist_entry; /* used for both a chessboard entry and as a distance table entry */
	signed char cb_entry;
	Bit8u *dist_table_ptr;
	struct struct_hero *hero_ptr;
	struct enemy_sheet *enemy_ptr;
	signed short done;
	signed short ranged_dist;
	Bit8u *actor_enemy_ptr; /* not needed, in principal. is only used for tests with NOT_NULL at a few places to determine wether the actor is an enemy. could also be done based on mode. */
	signed char two_squares;
	signed short target_reached_x[10];
	signed short target_reached_y[10];
	signed short unused[10]; /* array gets only written, but never read */

	struct viewdir_offsets coordinate_offset = g_viewdir_offsets7;

	actor_enemy_ptr = NULL;
	two_squares = 0;

	if ((mode == 0) || (mode == 2) || (mode == 4) || (mode == 6) || (mode == 7)) /* actor is an enemy */
	{
		actor_enemy_ptr = actor_ptr;
		if (is_in_byte_array(((struct enemy_sheet*)actor_enemy_ptr)->gfx_id, (Bit8u*)g_two_fielded_sprite_id))
		{
			two_squares = 1;
		}
	}

	dist_table_ptr = (Bit8u*)g_dtp2;
	g_chessboard_cpy = (signed char*)(g_dtp2 + 600);
	new_squares_reached = 1;
	memset(dist_table_ptr, -1, 600);
	*(dist_table_ptr + (y_in * 25) + x_in) = 0;
	memcpy(g_chessboard_cpy, g_chessboard, 600);

	for (i = 0; i < 10; i++) {
		target_reached_x[i] = target_reached_y[i] = unused[i] = 0;
	}

	/* make squares with dead heroes/enemies walkable */
	for (y = 0; y < 24; y++) {
		for (x = 0; x < 24; x++) {

			cb_or_dist_entry = *(g_chessboard_cpy + (y * 25) + x);

			if (cb_or_dist_entry > 0) {
				if ((cb_or_dist_entry < 10) && ((get_hero(cb_or_dist_entry - 1))->flags.dead || (get_hero(cb_or_dist_entry - 1))->flags.unconscious))
				{
					/* cb_or_dist_entry is a dead or unsonscious hero */
					*(g_chessboard_cpy + (y * 25) + x) = 0;

				} else if ((cb_or_dist_entry >= 10) && (cb_or_dist_entry < 30) && g_enemy_sheets[cb_or_dist_entry - 10].flags.dead) {
					/* test 'dead' flag */
					/* cb_or_dist_entry is a dead enemy. tail parts of two-squares enemies are not considered. */
					*(g_chessboard_cpy + (y * 25) + x) = 0;
					/* Original-Bug: The tail parts of dead two-squares enemies have been forgotten,
					 * meaning that they are not walkable.
					 * The fact that they are invisible does not make things better.
					 * However, in the target selection of the hero movement, tail parts of two-squares monsters are allowed.
					 * The weird outcome is that dead tail-parts can be entered, but not crossed by heroes.
					 * For enemies, squares with dead tail-parts are blocked completely. */
#ifdef M302de_ORIGINAL_BUGFIX
					/* make dead tail-parts walkable */
				} else if ((cb_or_dist_entry >= 30) && (cb_or_dist_entry < 50) && g_enemy_sheets[cb_or_dist_entry - 30].flags.dead) {
					/* test 'dead' flag */
					*(g_chessboard_cpy + (y * 25) + x) = 0;
#endif
				}
			}
		}
	}

	if ((mode == 6) || (mode == 8)) { /* ranged attack to some hero. mark possible base squares for launching the attack. */

		for (i = 0; i <= 6; i++) { /* loop over all heroes */
			if ((mode != 8) || (i != actor_id)) { /* hero should not attack himself */

				hero_ptr = get_hero(i);

				if ((hero_ptr->typus != HERO_TYPE_NONE) &&
					(hero_ptr->group_no == gs_current_group) &&
					!hero_ptr->flags.dead)
				{
					/* hero_ptr points to an actual alive hero in the current group */
					FIG_search_obj_on_cb(i + 1, &x, &y);

					for (dir = 0; dir < 4; dir++) {

						done = 0;
						ranged_dist = 1;

						while (done == 0) {

							new_y = y + ranged_dist * coordinate_offset.a[dir].y;
							new_x = x + ranged_dist * coordinate_offset.a[dir].x;


							if ((new_y < 0) || (new_y > 23) || (new_x < 0) ||
#ifndef M302de_ORIGINAL_BUGFIX
								/* Original-Bug: new_y > 23 gets checked twice, new_x > 23 is missing */
								(new_y > 23)
#else
								(new_x > 23)
#endif
							) {
								/* search passed the border of the map */
								done = 1;
							} else {
								if (!*(g_chessboard_cpy + (25 * new_y) + new_x))
								{
									/* sqare empty and may be used as base square for launching the ranged attack. set target marker */
									*(g_chessboard_cpy + (25 * new_y) + new_x) = 9;
									/* DANGER: The id 9 is in the range [1..9] which is typically considered as a hero.
									 * However, as there are at most 7 heroes, this should not be a problem, hopefully. */
								} else {
									/* square blocked */
									done = 1;
								}
							}
							ranged_dist++;
						}
					}
				}
			}
		}
	}

	if ((mode == 7) || (mode == 9)) { /* ranged attack to some enemy. mark possible base squares for launching the attack */

		for (i = 0; i < 20; i++) { /* loop over all enemies */

			if ((mode != 7) || (i != actor_id)) { /* enemy shoud not attack himself */

				enemy_ptr = &g_enemy_sheets[i];

				if (enemy_ptr->mon_id && !enemy_ptr->flags.dead) {

					FIG_search_obj_on_cb(i + 10, &x, &y);

					for (dir = 0; dir < 4; dir++) {
						done = 0;
						ranged_dist = 1;

						while (done == 0) {
							new_y = y + ranged_dist * coordinate_offset.a[dir].y;
							new_x = x + ranged_dist * coordinate_offset.a[dir].x;

							if ((new_y < 0) || (new_y > 23) || (new_x < 0) ||
#ifndef M302de_ORIGINAL_BUGFIX
								/* Original-Bug: new_y > 23 gets checked twice, new_x > 23 is missing */
								(new_y > 23)
#else
								(new_x > 23)
#endif
							) {
								/* search passed the border of the map */
								done =1;
							} else {
								if (!*(g_chessboard_cpy + (25 * new_y) + new_x))
								{
									/* sqare empty and may be used as base square for launching the ranged attack. set target marker */
									*(g_chessboard_cpy + (25 * new_y) + new_x) = 49;
									/* DANGER: The id 49 is in the range [30..49] which is typically considered as the tail of a two-squares enemy.
									 * However, presumably there is no fight involving the last id 49 in this range.
									 * So hopefully, there is no collision. */
								} else {
									/* square blocked */
									done = 1;
								}
							}

							ranged_dist++;
						}
					}
				}
			}
		}
	}

	/* breadth-first search to fill the distance table
	 * search stops if for the last distance, a target was found, no new reachable squares have been found, or if the new distance will be as large as 50.
	 * Moreover, the search will stop with a break if for the current distance, 10 targets have been found. */
	while ( (target_reached == 0) && (new_squares_reached != 0) && (dist < 50)) {
		new_squares_reached = 0;
		dist++;
		nr_targets_reached = 0;

		for (y = 0; y < 24; y++) {

			if (nr_targets_reached == 10) {
				break;
			}

			for (x = 0; x < 24; x++) {

				if (nr_targets_reached == 10) {
					break;
				}

				if (host_readbs(dist_table_ptr + (y * 25) + x) == (dist - 1)) {

					for (i = 0; i < 4; i++) {

						new_y = y + coordinate_offset.a[i].y;
						new_x = x + coordinate_offset.a[i].x;
						tail_y = y - coordinate_offset.a[i].y; /* for movement of two-squares actors */
						tail_x = x - coordinate_offset.a[i].x; /* for movement of two-squares actors */

						if ((new_y < 24) && (new_y >= 0) && (new_x < 24) && (new_x >= 0)) {

							cb_or_dist_entry = host_readbs(dist_table_ptr + (new_y * 25) + new_x);
							cb_entry = *(g_chessboard_cpy + (new_y * 25) + new_x);

							if (cb_or_dist_entry < 0) { /* square has not been reached before */

								if (cb_entry == 0) { /* square is empty */

									if (!actor_enemy_ptr ||
										(actor_enemy_ptr && (!two_squares ||
													((two_squares != 0) &&
														((!*(g_chessboard_cpy + (tail_y * 25) + tail_x)) || /* square is empty */
														(*(g_chessboard_cpy + (tail_y * 25) + tail_x))  == (actor_id + 10) || /* head of active enemy is on square */
														(*(g_chessboard_cpy + (tail_y * 25) + tail_x)) == (actor_id + 30)) && /* tail of active enemy is on square */
														((tail_y < 24) && (tail_y >= 0) && (tail_x < 24) && (tail_x >= 0))))))
									{
										*(dist_table_ptr + (new_y * 25) + new_x) = (signed char)dist;
										new_squares_reached = 1;
									}
								} else {
									if (cb_entry < 0) { /* escape square */

										if ((mode == 4) || (mode == 5)) { /* actor is fleeing */
											/* it is not tested if there is space for the tail of a two-squares monster! */
											unused[nr_targets_reached] = 1;
											target_reached_x[nr_targets_reached] = new_x;
											target_reached_y[nr_targets_reached] = new_y;
											nr_targets_reached++;
											if (nr_targets_reached == 10) {
												break;
											}
										} else {
											*(dist_table_ptr + (new_y * 25) + new_x) = 100; /* for all other modes: square is blocked */
										}
									} else if (cb_entry == 124) { /* target marker for hero movement (implies mode == 10 (hero movement)) */
										/* test of the tail-condition not needed here, as the active actor is a hero */
										unused[nr_targets_reached] = 1;
										target_reached_x[nr_targets_reached] = new_x;
										target_reached_y[nr_targets_reached] = new_y;
										nr_targets_reached++;
										if (nr_targets_reached == 10) {
											break;
										}
									} else if (cb_entry >= 50) { /* square is blocked */
										*(dist_table_ptr + (new_y * 25) + new_x) = 100;
									} else if (cb_entry < 10) {

										if (cb_entry == 9) { /* target marker for ranged attack to some hero (implies mode == 6 or mode == 8) */
											/* test of the tail-condition not needed here, as two-squares enemies don't have ranged attacks */
											unused[nr_targets_reached] = 1;
											target_reached_x[nr_targets_reached] = new_x;
											target_reached_y[nr_targets_reached] = new_y;
											nr_targets_reached++;
											if (nr_targets_reached == 10) {
												break;
											}
										} else { /* some hero on square */
											if (((mode == 0) || (mode == 1)) /* melee attack */
												&& (!actor_enemy_ptr || (actor_enemy_ptr && (!two_squares ||
															((two_squares != 0) &&
																((!*(g_chessboard_cpy + (tail_y * 25) + tail_x)) ||
																(*(g_chessboard_cpy + (tail_y * 25) + tail_x))  == (actor_id + 10) ||
																(*(g_chessboard_cpy + (tail_y * 25) + tail_x)) == (actor_id + 30)) &&
																((tail_y < 24) && (tail_y >= 0) && (tail_x < 24) && (tail_x >= 0)))))))
											{
												unused[nr_targets_reached] = 1;
												target_reached_x[nr_targets_reached] = new_x;
												target_reached_y[nr_targets_reached] = new_y;
												nr_targets_reached++;
												if (nr_targets_reached == 10) {
													break;
												}
											} /* if a two-squares monster cannot attack, because there is no space for the tail, the entry in dist_table stays at -1.
											     In this way, later the hero may be considered again as a target from a different direction. */
										}
									} else if (cb_entry < 50) {
										if (cb_entry == 49) { /* target marker for ranged attack to enemy (implies mode == 7 or mode == 9) */
											/* test of the tail-condition not needed here, as two-squares enemies don't have ranged attacks */
											unused[nr_targets_reached] = 1;
											target_reached_x[nr_targets_reached] = new_x;
											target_reached_y[nr_targets_reached] = new_y;
											nr_targets_reached++;
											if (nr_targets_reached == 10) {
												break;
											}
										} else { /* enemy on square */
											if (((mode == 2) || (mode == 3)) && /* melee attack */
												(cb_entry < 30) && (!actor_enemy_ptr || (actor_enemy_ptr && (!two_squares ||
															((two_squares != 0) &&
																((!*(g_chessboard_cpy + (tail_y * 25) + tail_x)) ||
																(*(g_chessboard_cpy + (tail_y * 25) + tail_x)) == (actor_id + 10) ||
																(*(g_chessboard_cpy + (tail_y * 25) + tail_x)) == (actor_id + 30)) &&
																((tail_y < 24) && (tail_y >= 0) && (tail_x < 24) && (tail_x >= 0)))))))
											{
												unused[nr_targets_reached] = 1;
												target_reached_x[nr_targets_reached] = new_x;
												target_reached_y[nr_targets_reached] = new_y;
												nr_targets_reached++;
												if (nr_targets_reached == 10) {
													break;
												}
											} /* if a two-squares monster cannot attack, because there is no space for the tail, the entry in dist_table stays at -1.
											     In this way, later the enemy may be considered again as a target from a different direction. */

										}
									}
								}
							}
						} else {
							if (((mode == 4) || (mode == 5)) && /* actor is fleeing */
								((g_scenario_buf[0x14] > 3) ||
									((g_scenario_buf[0x14] <= 3) && ((new_x > 23) || (new_y > 23) || (new_y < 0)))))
							{
								/* it is not tested if there is space for the tail of a two-squares monster! */
								unused[nr_targets_reached] = 1;
								target_reached_x[nr_targets_reached] = new_x;
								target_reached_y[nr_targets_reached] = new_y;
								nr_targets_reached++;
								if (nr_targets_reached == 10) {
									break;
								}
							}
						}
					}
				}
			}

		}

		if (nr_targets_reached) {
			target_reached = 1;
			best_target = 0;
			lowest_nr_dir_changes = 99;

			/* find target whose path has the lowest number of direction changes */
			for (i = 0; i < nr_targets_reached; i++) {

				if ((mode == 0) || (mode == 2) || (mode == 4) || (mode == 6) || (mode == 7)) { /* actor is an enemy */
					FIG_find_path_to_target_backtrack(dist_table_ptr, target_reached_x[i], target_reached_y[i], dist, ((struct enemy_sheet*)actor_ptr)->bp, mode, two_squares, actor_id);
				} else { /* actor is a hero */
					FIG_find_path_to_target_backtrack(dist_table_ptr, target_reached_x[i], target_reached_y[i], dist, ((struct struct_hero*)actor_ptr)->bp_left, mode, two_squares, actor_id);
				}

				nr_dir_changes = FIG_count_direction_changes_of_path(g_fig_move_pathdir);

				if ((nr_dir_changes == 0)) {
					best_target = i;
					break;
				}

				if (nr_dir_changes < lowest_nr_dir_changes) {
					lowest_nr_dir_changes = nr_dir_changes;
					best_target = i;
				}
			}

			if ((mode == 0) || (mode == 2) || (mode == 4) || (mode == 6) || (mode == 7)) { /* actor is an enemy */
				FIG_find_path_to_target_backtrack(dist_table_ptr, target_reached_x[best_target], target_reached_y[best_target], dist, ((struct enemy_sheet*)actor_ptr)->bp, mode, two_squares, actor_id);
			} else { /* actor is a hero */
				FIG_find_path_to_target_backtrack(dist_table_ptr, target_reached_x[best_target], target_reached_y[best_target], dist, ((struct struct_hero*)actor_ptr)->bp_left, mode, two_squares, actor_id);
			}
		}
	}


	if (target_reached != 0) {
		return 1;
	} else {
		return -1;
	}
}

#if !defined(__BORLANDC__)
}
#endif
