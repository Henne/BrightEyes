/*
 *	Rewrite of DSA1 v3.02_de functions of seg037 (fight: enemy attack)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg037.cpp
 *
 */
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg038.h"

static struct viewdir_offsets g_fig_viewdir_offsets5 = { { { 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 } } };	// ds:0x5fd8
static struct viewdir_offsets g_fig_viewdir_offsets6 = { { { 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 } } };	// ds:0x5fe8

/**
 * \brief   copies something from ANI.DAT
 *
 * \param   dst         destination
 * \param[in]   ani_num animation seqence to copy
 * \param[in]   mode    3 for WEAPANI.DAT, anything else is ANI.DAT
 * \return              the number of copied bytes.
 */
static signed int copy_ani_sequence(int8_t *dst, const signed int ani_num, const signed int mode)
{
	uint8_t *p_datbuffer;
	int8_t *p_datitem;
	signed char len;
	signed int i;
	signed int ani_max_num;

	/* ANI.DAT */
	p_datbuffer = g_buffer_anidat;

	/* WEAPANI.DAT */
	if (mode == 3)
		p_datbuffer = g_buffer_weapanidat;

	ani_max_num = *(int16_t*)p_datbuffer;

	/* Sanity check of no */
	if (ani_num < 0)
		return 0;

	if (ani_num > ani_max_num)
		return 0;

	/* set p_datitem to the requested data entry */
	p_datitem = (int8_t*)p_datbuffer;
	p_datitem += ani_max_num + 2;

	len = (int8_t)p_datbuffer[2];

	for (i = 1; i <= ani_num; i++) {
		p_datitem += len;
		len = *(int8_t*)(p_datbuffer + i + 2);
	}

	p_datitem++;

	len = len - 2;

	/* copy the ani sequence to dst */
	for (i = 0; len > i; i++) {
		*dst = *p_datitem;
		p_datitem++;
		dst++;
	}

	return len;
}

void prepare_enemy_ani(struct enemy_sheet *enemy, const signed int enemy_id)
{
	signed char dir1;
	signed char dir2;
	signed char dir3;
	int8_t *sheet_ptr;
	struct struct_fighter *fighter;
	int16_t *ani_index_ptr;

	signed int i;

	g_fig_anisheets[1][0] = 0;
	g_fig_anisheets[1][242] = enemy->actor_sprite_id;

	sheet_ptr = &g_fig_anisheets[1][1];
	i = 0;
	ani_index_ptr = g_gfx_ani_index[enemy->actor_sprite_id];

	while (g_fig_move_pathdir[i] != -1) {

		if (enemy->viewdir != g_fig_move_pathdir[i]) {

			dir2 = dir1 = -1;
			dir3 = enemy->viewdir;
			dir2 = dir3;
			dir3++;

			if (dir3 == 4) {
				dir3 = 0;
			}

			if (g_fig_move_pathdir[i] != dir3) {

				dir1 = dir3;
				dir3++;

				if (dir3 == 4) {
					dir3 = 0;
				}

				if (g_fig_move_pathdir[i] != dir3) {

					dir2 = enemy->viewdir + 4;
					dir1 = -1;
				}

			}

			enemy->viewdir = g_fig_move_pathdir[i];

			sheet_ptr += copy_ani_sequence(sheet_ptr, ani_index_ptr[dir2], 1);

			if (dir1 != -1) {

				sheet_ptr += copy_ani_sequence(sheet_ptr, ani_index_ptr[dir1], 1);
			}
		}

		if (g_fig_move_pathdir[i] == g_fig_move_pathdir[i + 1]) {

			sheet_ptr += copy_ani_sequence(sheet_ptr, ani_index_ptr[g_fig_move_pathdir[i] + 0x0c], 1);
			i += 2;
			/* BP - 2 */
			enemy->bp = enemy->bp - 2;

		} else {
			sheet_ptr += copy_ani_sequence(sheet_ptr, ani_index_ptr[g_fig_move_pathdir[i] + 0x08], 1);
			i++;
			/* BP - 1 */
			enemy->bp--;
		}
	}

	/* terminate array */
	*sheet_ptr = -1;
	FIG_call_draw_pic();
	FIG_remove_from_list(g_fig_cb_marker_id, 0);
	g_fig_cb_marker_id = -1;
	FIG_set_sheet(enemy->fighter_id, 1);

	if (is_in_byte_array(enemy->actor_sprite_id, g_double_size_actor_sprite_id_table)) {

		memcpy(&g_fig_anisheets[3], &g_fig_anisheets[1], 243);

		fighter = FIG_get_fighter(enemy->fighter_id);

		FIG_set_sheet(g_fig_double_size_fighter_id_table[fighter->double_size], 3);
	}

	/* draw_fight_screen */
	draw_fight_screen(0);
	memset(&g_fig_anisheets[1], -1, 243);
	memset(&g_fig_anisheets[3], -1, 243);
	FIG_init_list_elem(enemy_id + 10);
}


/**
 * \brief   checks if range attack is possible
 *
 * \param   x           x - coordinate of attacker
 * \param   y           y - coordinate of attacker
 * \param   dx          delta to x (looking direction)
 * \param   dy          delta to y (looking direction)
 * \param   mode        0 = common, 1 = attack enemies only, 2 = attack heroes only
 * \return              0 if theres nothing to attack else 1
 */
signed int FIG_enemy_can_attack_neighbour(const signed int x, const signed int y, const signed int dx, const signed int dy, const signed int mode)
{
	const signed char target = get_cb_val(x + dx, y + dy);

	if (mode == 0) {

		if ( ((target > 0) && (target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious) ||
			(
			(target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead &&
			//g_enemy_sheets[target - 10].flags.renegade
				((struct enemy_flags*)(target * sizeof(enemy_sheet) + (uint8_t*)g_enemy_sheets - 10 * sizeof(enemy_sheet) + 0x31))->renegade
			))
		{
			return 1;
		} else {
			return 0;
		}

	} else if (mode == 1) {

		/* is a living enemy */
		if ((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead)
		{
			return 1;
		} else {
			return 0;
		}

	} else if (mode == 2) {

		/* is a living, conscious hero */
		if ((target > 0) && (target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious)
		{
			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}

/**
 * \brief   checks if range attack is possible
 *
 * \param   x           x - coordinate of attacker
 * \param   y           y - coordinate of attacker
 * \param   dir         looking direction
 * \param   mode        0 = common, 1 = attack enemies only, 2 = attack heroes only
 * \return              0 if theres nothing to attack in that direction
 *                      or the ID of the attackee.
 */
/*
 * Original-Bug: range attack of enemies is possible with direct contact
 */
signed int FIG_search_range_target(const signed int x, const signed int y, const signed int dir, const signed int mode)
{
	signed int done;
	signed int x_diff;	/* run variables in dir */
	signed char target;
	signed int y_diff;	/* run variables in dir */
	signed int can_attack;

	done = 0;
	x_diff = 0;
	y_diff = 0;
	can_attack = 0;

	while (!done) {

		/* go one field further */
		if (dir == 0) {		/* RIGHT-BOTTOM */
			x_diff++;
		} else if (dir == 1) {	/* LEFT-BOTTOM */
			y_diff--;
		} else if (dir == 2) {	/* LEFT-UP */
			x_diff--;
		} else {		/* RIGHT-UP */
			y_diff++;
		}

		/* out of chessboard */
		if (y + y_diff < 0 || y + y_diff > 23 || x + x_diff < 0 || x + x_diff > 23) {
			done = 1;
		} else {

			/* get value from current field */
			target = get_cb_val(x + x_diff, y + y_diff);

			if (mode == 0) {
				/* hero or enemy reacheable from enemies position */
				if ( ((target > 0) && (target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious) ||
					((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead &&
					// g_enemy_sheets[target - 10].flags.renegade
					((struct enemy_flags*)(target * sizeof(enemy_sheet) + (uint8_t*)g_enemy_sheets - 10 * sizeof(enemy_sheet) + 0x31))->renegade
					))
				{
					can_attack = 1;
					done = 1;

				} else

				/* if field is not empty */
				if (target != 0) {

					/* an enemy or another object */
					if ( ((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead) ||
						((target >= 50) && !is_in_int_array(target - 50, g_cb_obj_nonobstacle)))
					{
							done = 1;
					}
				}

			} else if (mode == 1) {
				/* attack foe first */
				if ((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead)
				{
					can_attack = 1;
					done = 1;
				} else
				/* skip zeros */
				if (target != 0) {

#ifdef M302de_ORIGINAL_BUGFIX
					/* Original-Bugfix: the next if assumes
						that a negative target is a hero -> SEGFAULT*/
					if (target < 0) {
						done = 1;
					} else
#endif

					/* handle heroes or walls */
					if (((target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious) ||
						((target >= 50) && !is_in_int_array(target - 50, g_cb_obj_nonobstacle)))
					{
						done = 1;
					}
				}

			} else if (mode == 2) {
				/* attack hero */
				if ((target > 0) && (target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious)
				{
					can_attack = 1;
					done = 1;
				} else

				/* skip zeros */
				if (target != 0) {

#ifdef M302de_ORIGINAL_BUGFIX
					if (target < 0) {
						done = 1;
					} else
#endif

					if ( ((target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious) ||
						((target >= 50) && !is_in_int_array(target - 50, g_cb_obj_nonobstacle)) ||
						((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead))
					{
						done = 1;
					}
				}

			}
		}
	}

	if (can_attack == 0)
		return 0;
	else
		return target;
}


signed int FIG_get_mspell(const signed int mspell_id, const signed int mode)
{
	signed int retval = 0;
	struct mon_spell_description *desc = &g_mon_spell_descriptions[mspell_id];

	if (mode == 0) {

		if ((desc->mode == 3) || (desc->mode == 2)) {
			retval = 2;
		} else {
			if (desc->mode == 1) {
				retval = 1;
			} else {
				retval = 3;
			}
		}

	} else {
		if (desc->mode == 3) {
			retval = 1;
		} else if (desc->mode == 0) {
			retval = 3;
		}
	}

	return retval;
}

signed int FIG_select_mspell(struct enemy_sheet* enemy, const signed int enemy_id, const signed int attack_foe, signed int x, signed int y)
{
	signed int available_spells;
	signed int mspell_id;
	signed int done;
	signed int retval;
	signed int mode;
	signed int target_found;
	signed int decided;
	struct viewdir_offsets diff = g_fig_viewdir_offsets5;

	signed int i;
	signed int cnt;

	retval = 0;

	available_spells = i = 0;
	while ((i < 5) && (g_mon_spellbooks[enemy->spellbook_id][i++] != -1))
	{
		available_spells++;
	}

	done = 0;
	while ((done == 0) && (enemy->bp > 0)) {

		decided = 0;

		for (i = 0; i < available_spells; i++) {

			mspell_id = g_mon_spellbooks[enemy->spellbook_id][i];

			if (g_mon_spell_descriptions[mspell_id].unkn1 == 1) {

				if (random_schick(100) < 75) {
					decided = 1;
					break;
				}
			}
		}

		if (decided == 0) {
			mspell_id = g_mon_spellbooks[enemy->spellbook_id][random_interval(0, available_spells - 1)];
		}

		enemy->target_object_id = 0;

		if ( (mode = FIG_get_mspell(mspell_id, attack_foe)) > 0) {

			if (mode == 3) {
				enemy->target_object_id = enemy_id + 10;
 				enemy->mspell_id = mspell_id;
				retval = 1;
				done = 1;
			} else {

				if (!g_mon_spell_descriptions[mspell_id].unkn1) {

					while (enemy->bp && (done == 0)) {

						i = enemy->viewdir;
						cnt = 0;

						while (!enemy->target_object_id && (cnt < 4)) {

							if (FIG_enemy_can_attack_neighbour(x, y, diff.offset[i].x, diff.offset[i].y, mode)) {
								enemy->target_object_id = get_cb_val(x + diff.offset[i].x, y + diff.offset[i].y);
							}

							cnt++;
							if (++i == 4) {
								i = 0;
							}
						}

						if (enemy->target_object_id) {

							enemy->mspell_id = mspell_id;
							retval = 1;
							done = 1;

						} else if (enemy->bp > 0) {

							if (!enemy->flags.tied) {

								if (mode == 1)
									target_found = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 2);
								else
									target_found = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 0);

								if (target_found != -1) {
									prepare_enemy_ani(enemy, enemy_id);
									FIG_search_obj_on_cb(enemy_id + 10, &x, &y);

									if (enemy->bp < 3) {
										enemy->bp = 0;
									}
								} else {
									enemy->bp = 0;
								}
							} else {
								enemy->bp = 0;
							}
						}
					}
				} else {

					while ((done == 0) && (enemy->bp > 0)) {

						i = enemy->viewdir;
						cnt = 0;

						while (!enemy->target_object_id && (cnt < 4)) {

							enemy->target_object_id = FIG_search_range_target(x, y, i, mode);

							cnt++;
							if (++i == 4) {
								i = 0;
							}
						}

						if (enemy->target_object_id) {

							enemy->mspell_id = mspell_id;
							retval = 1;
							done = 1;

						} else if (enemy->bp > 0) {

							if (!enemy->flags.tied) {

								if (mode == 1)
									target_found = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 7);
								else
									target_found = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 6);

								if (target_found != -1) {
									prepare_enemy_ani(enemy, enemy_id);
									FIG_search_obj_on_cb(enemy_id + 10, &x, &y);

									if (enemy->bp < 5) {
										enemy->bp = 0;
									}
								} else {
									enemy->bp = 0;
								}
							} else {
								enemy->bp = 0;
							}
						}
					}
				}
			}
		}
	}

#if !defined(__BORLANDC__)
	if (retval)
		D1_INFO("Zauberspruch %d\n", enemy->mspell_id);
	else
		D1_INFO("Kein Zauberspruch\n");
#endif

	return retval;
}


signed int FIG_enemy_range_attack(struct enemy_sheet *enemy, const signed int enemy_id, const signed int attack_foe, signed int x, signed int y)
{

	signed int cnt;
	signed int done;
	signed int retval;
	signed int target_found;
	signed int dir;

	retval = 0;

	done = 0;

	while ((done == 0) && (enemy->bp > 0)) {

		/* reset the attackee ID */
		enemy->target_object_id = 0;

		while ( (done == 0) && (enemy->bp > 0) ) {

			dir = enemy->viewdir;
			cnt = 0;

			/* check clockwise for someone to attack */
			while (!enemy->target_object_id && (cnt < 4)) {

				enemy->target_object_id = FIG_search_range_target(x, y, dir, attack_foe);
				cnt++;
				if (++dir == 4) {
					dir = 0;
				}
			}

			if (enemy->target_object_id) {

				/* found someone to attack */
				retval = 1;
				done = 1;

			} else if (enemy->bp > 0) {

					if (!enemy->flags.tied) {

						if (attack_foe == 0)
							target_found = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 6);
						else
							target_found = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 7);

						if (target_found != -1) {
							prepare_enemy_ani(enemy, enemy_id);
							FIG_search_obj_on_cb(enemy_id + 10, &x, &y);

							if (enemy->bp < 3) {
								enemy->bp = 0;
							}
						} else {
							enemy->bp = 0;
						}
					} else {
						enemy->bp = 0;
					}
			}
		}
	}

	return retval;
}


void FIG_enemy_turn(struct enemy_sheet *enemy, const signed int enemy_id, signed int x, signed int y)
{
	signed int target_reachable;
	signed int attack_foe;
	signed int i;
	signed int cnt;
	signed int done = 0;
	signed int flag;
	signed int x_bak;
	signed int y_bak;
	signed int target;

	struct viewdir_offsets diff = g_fig_viewdir_offsets6;

	/* check if we are in a special fight */

	if (g_current_fight_id == FIGHT_ID_F064) {

		/* F064: fight against GORAH */

		if (enemy->monster_id == MONSTER_ID_GORAH) {
			/* GORAH waits the first 5 rounds */

			if (g_fight_round < 5) {
				enemy->bp = 0;
			}
		}

	} else if ((g_current_fight_id == FIGHT_ID_F099) && (random_interval(8, 12) <= g_fight_round)) {

		/* F099: fight against four HARPIES */

		/* after 8-12 rounds, the enemies flee */
		enemy->flags.scared = 1;

	} else if ((g_current_fight_id == FIGHT_ID_F122) && (FIG_count_active_enemies() <= 3)) {

		/* F122: fight against 13 WOLVES */

		/* if at most 3 wolves are left, all enemies flee */
		enemy->flags.scared = 1;

	} else if (g_current_fight_id == FIGHT_ID_F144) {

		/* F144: final fight */

		if (enemy->flags.tied) {
			enemy->bp = 0;
		}
	}

	while ( (done == 0) && (enemy->bp > 0)) {

		if (g_fig_cb_marker_id != -1) {
			FIG_remove_from_list(g_fig_cb_marker_id, 0);
			g_fig_cb_marker_id = -1;
		}

		FIG_init_list_elem(enemy_id + 10);

		draw_fight_screen_pal(0);

		enemy->target_object_id = 0;
		enemy->action_id = 1;

		/* LE threshold reached */
		if (enemy->le_flee >= enemy->le) {

			enemy->flags.scared = 1;
		}

		/* chance of 4% that an illusion enemy disappears */
		if (random_schick(100) < 5) {
#if !defined(__BORLANDC__)
			if (enemy->flags.illusion) {
				D1_INFO("Feind %d verliert seinen Illusionszauber\n", enemy_id);
			}
#endif
			/* Original-Bug? Why unset 'tied' and not 'illusion'?? */
			enemy->flags.tied = 0;
		}

		if (!enemy->flags.scared) {

			attack_foe = 0;

			if (enemy->flags.renegade) {
				attack_foe = 1;
			}

			/* enemy can cast spells and has AE >= 5 left */
			if ((enemy->spellbook_id != -1) && (enemy->ae >= 5) && FIG_select_mspell(enemy, enemy_id, attack_foe, x, y))
			{
				/* REMARK: enemy can still cast a spell with less than 5 BP. */
				enemy->action_id = FIG_ACTION_SPELL;

				/* adjust BP */
				enemy->bp = enemy->bp - 5;

				if (enemy->bp < 0) {
					enemy->bp = 0;
				}
				return;
			}

			/* enemy has range weapons */
			if ( ((enemy->shots > 0) || (enemy->throws > 0)) && FIG_enemy_range_attack(enemy, enemy_id, attack_foe, x, y))
			{
				/* REMARK: enemy can still attack with less than 3 BP. */
				enemy->action_id = FIG_ACTION_RANGE_ATTACK;

				/* adjust BP */
				enemy->bp = enemy->bp - 3;

				if (enemy->bp < 0) {
					enemy->bp = 0;
				}
				return;
			}

			enemy->target_object_id = 0;
			i = enemy->viewdir;
			cnt = 0;
			while (!enemy->target_object_id && (cnt < 4)) {

				if (FIG_enemy_can_attack_neighbour(x, y, diff.offset[i].x, diff.offset[i].y, attack_foe)) {

					flag = 1;

					if (is_in_byte_array(enemy->actor_sprite_id, g_double_size_actor_sprite_id_table))
					{

						target = get_cb_val(x - diff.offset[i].x, y - diff.offset[i].y);

						if (target && (enemy_id + 30 != target)) {

							if ((target < 0) || (target >= 50) || (target >= 30) ||
								((target > 0) && (target < 10) && !get_hero(target - 1)->flags.dead) ||
								((target < 30) && (target >= 10) && !g_enemy_sheets[target - 10].flags.dead))
							{
								flag = 0;
							}
						}
					}

					if (flag != 0) {
						enemy->target_object_id = get_cb_val(x + diff.offset[i].x, y + diff.offset[i].y);
					}
				}

				cnt++;
				if (++i == 4) {
					i = 0;
				}
			}
		}

		if (enemy->target_object_id != 0) {

			enemy->action_id = 2;
			enemy->bp = enemy->bp - 3;
			done = 1;

		} else if (enemy->bp > 0) {

			if (!enemy->flags.tied) {

				if (enemy->flags.scared) {
					target_reachable = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 4);
					enemy->bp = 0;
				} else {
					if (enemy->flags.renegade)
						target_reachable = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 2);
					else
						target_reachable = FIG_find_path_to_target((uint8_t*)enemy, enemy_id, x, y, 0);
				}

				if (target_reachable != -1) {

					x_bak = x;
					y_bak = y;
					prepare_enemy_ani(enemy, enemy_id);
					FIG_search_obj_on_cb(enemy_id + 10, &x, &y);

					if ((x_bak == x) && (y_bak == y)) {
						enemy->bp = 0;
					}

					if (enemy->bp < 3) {
						enemy->bp = 0;
					}

				} else {
					enemy->bp = 0;
				}
			} else {
				enemy->bp = 0;
			}

			enemy->action_id = 1;
		}
	}
}
