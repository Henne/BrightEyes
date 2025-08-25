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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   copies something from ANI.DAT
 *
 * \param   dst         destination
 * \param   no          which record to copy
 * \param   mode        3 for WEAPANI.DAT, anything else is ANI.DAT
 * \return              the number of copied bytes.
 */
#if defined(__BORLANDC__)
static
#endif
signed short copy_ani_stuff(Bit8u *dst, signed short no, signed short mode)
{
	Bit8u *buffer, *src;
	signed char retval;
	signed short i;
	signed short max_no;

	/* ANI.DAT */
	buffer = g_buffer_anidat;

	/* WEAPANI.DAT */
	if (mode == 3)
		buffer = g_buffer_weapanidat;

	max_no = host_readw(buffer);

	/* Sanity check of no */
	if (no < 0)
		return 0;

	if (no > max_no)
		return 0;

	/* set src to the requested data entry */
	src = buffer;
	src += max_no + 2;

	retval = host_readb(buffer + 2);

	for (i = 1; i <= no; i++) {
		src += retval;
		retval = host_readb(buffer + i + 2);
	}

	src++;

	retval = retval - 2;

	/* copy some bytes from ANI.DAT */

	for (i = 0; retval > i; i++) {
		host_writeb(dst, host_readb(src));
		src++;
		dst++;
	}

	/* return the number of copied bytes */

	return retval;
}

void seg037_00ae(struct enemy_sheet *enemy, signed short enemy_no)
{
	signed char b1;
	signed char b2;
	signed char b3;
	Bit8u *p1;
	struct struct_fighter *fighter;
	Bit16s *p3;

	signed short i;

	ds_writeb((FIG_ANISHEETS + 0xf3), 0); /* first position of the second FIG_ANISHEET (0xf3 == 243, FIG_ANISHEET is a struct(243)[8]) */
	ds_writeb((FIG_ANISHEETS + 242 + 0xf3), enemy->gfx_id); /* last position of the second FIG_ANISHEET */
	p1 = p_datseg + (FIG_ANISHEETS + 1 + 0xf3); /* second position of the second FIG_ANISHEET */

	i = 0;
	p3 = g_gfx_ani_index[enemy->gfx_id];

	while (g_fig_move_pathdir[i] != -1) {

		if (enemy->viewdir != g_fig_move_pathdir[i]) {

			b2 = b1 = -1;
			b3 = enemy->viewdir;
			b2 = b3;

			b3++;

			if (b3 == 4) {
				b3 = 0;
			}

			if (g_fig_move_pathdir[i] != b3) {

				b1 = b3;
				b3++;

				if (b3 == 4) {
					b3 = 0;
				}

				if (g_fig_move_pathdir[i] != b3) {
					b2 = enemy->viewdir + 4;
					b1 = -1;
				}

			}

			enemy->viewdir = g_fig_move_pathdir[i];

			p1 += copy_ani_stuff(p1, p3[b2], 1);

			if (b1 != -1) {

				p1 += copy_ani_stuff(p1, p3[b1], 1);
			}
		}

		if (g_fig_move_pathdir[i] == g_fig_move_pathdir[i + 1]) {

			p1 += copy_ani_stuff(p1, p3[g_fig_move_pathdir[i] + 0x0c], 1);
			i += 2;
			/* BP - 2 */
			enemy->bp = enemy->bp - 2;

		} else {
			p1 += copy_ani_stuff(p1, p3[g_fig_move_pathdir[i] + 0x08], 1);
			i++;
			/* BP - 1 */
			enemy->bp--;
		}
	}

	/* terminate array */
	host_writeb(p1, -1);

	FIG_call_draw_pic();

	FIG_remove_from_list(g_fig_cb_marker_id, 0);

	g_fig_cb_marker_id = -1;

	FIG_set_sheet(enemy->fighter_id, 1);

	if (is_in_byte_array(enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {

		memcpy(p_datseg + (FIG_ANISHEETS + 3*0xf3), p_datseg + (FIG_ANISHEETS + 1*0xf3), 0xf3);

		fighter = FIG_get_fighter(enemy->fighter_id);

		FIG_set_sheet(g_fig_twofielded_table[fighter->twofielded], 3);
	}

	/* draw_fight_screen */
	draw_fight_screen(0);

	memset(p_datseg + (FIG_ANISHEETS + 1*0xf3), -1, 0xf3); /* set second FIG_ANISHEET to -1 */
	memset(p_datseg + (FIG_ANISHEETS + 3*0xf3), -1, 0xf3); /* set fourth FIG_ANISHEET to -1 */

	FIG_init_list_elem(enemy_no + 10);
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
unsigned short test_foe_melee_attack(signed short x, signed short y,
		signed short dx, signed short dy, signed short mode)
{
	signed char cb_val = get_cb_val(x + dx, y + dy);

	if (mode == 0) {

		if ( ((cb_val > 0) && (cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unconscious(get_hero(cb_val - 1))) || (
			(cb_val >= 10) && (cb_val < 30) && !g_enemy_sheets[cb_val - 10].flags.dead && g_enemy_sheets[cb_val -10].flags.renegade))
		{
			return 1;
		} else {
			return 0;
		}

	} else if (mode == 1) {

		/* is a living enemy */
		if ((cb_val >= 10) && (cb_val < 30) && !g_enemy_sheets[cb_val - 10].flags.dead)
		{
			return 1;
		} else {
			return 0;
		}

	} else if (mode == 2) {

		/* is a living, conscious hero */
		if ((cb_val > 0) && (cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unconscious(get_hero(cb_val - 1)))
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
 * Original-Bug: range attack of foes is possible with direct contact
 */
signed short test_foe_range_attack(signed short x, signed short y, const signed short dir, signed short mode)
{
	signed short done;
	signed short di;	/* run variables in dir */
	signed char cb_val;
	signed short dy;	/* run variables in dir */
	signed short can_attack;

	done = 0;
	di = 0;
	dy = 0;
	can_attack = 0;

	while (!done) {

		/* go one field further */
		if (dir == 0) {		/* RIGHT-BOTTOM */
			di++;
		} else if (dir == 1) {	/* LEFT-BOTTOM */
			dy--;
		} else if (dir == 2) {	/* LEFT-UP */
			di--;
		} else {		/* RIGHT-UP */
			dy++;
		}

		/* out of chessboard */
		if (y + dy < 0 || y + dy > 23 || x + di < 0 || x + di > 23) {
			done = 1;
		} else {

			/* get value from current field */
			cb_val = get_cb_val(x + di, y + dy);

			if (mode == 0) {
				/* hero or enemy reacheable from enemies position */
				if ( ((cb_val > 0) && (cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unconscious(get_hero(cb_val - 1))) ||
					((cb_val >= 10) && (cb_val < 30) && !g_enemy_sheets[cb_val - 10].flags.dead && g_enemy_sheets[cb_val - 10].flags.renegade))
				{
					can_attack = 1;
					done = 1;

				} else

				/* if field is not empty */
				if (cb_val != 0) {

					/* an enemy or another object */
					if ( ((cb_val >= 10) && (cb_val < 30) && !g_enemy_sheets[cb_val - 10].flags.dead) ||
						((cb_val >= 50) && !is_in_word_array(cb_val - 50, g_cb_obj_nonobstacle)))
					{
							done = 1;
					}
				}

			} else if (mode == 1) {
				/* attack foe first */
				if ((cb_val >= 10) && (cb_val < 30) && !g_enemy_sheets[cb_val - 10].flags.dead)
				{
					can_attack = 1;
					done = 1;
				} else
				/* skip zeros */
				if (cb_val != 0) {

#ifdef M302de_ORIGINAL_BUGFIX
					/* Original-Bugfix: the next if assumes
						that a negative cb_val is a hero -> SEGFAULT*/
					if (cb_val < 0) {
						done = 1;
					} else
#endif

					/* handle heroes or walls */
					if (((cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unconscious(get_hero(cb_val - 1))) ||
						((cb_val >= 50) && !is_in_word_array(cb_val - 50, g_cb_obj_nonobstacle)))
					{
						done = 1;
					}
				}

			} else if (mode == 2) {
				/* attack hero */
				if ((cb_val > 0) && (cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unconscious(get_hero(cb_val - 1)))
				{
					can_attack = 1;
					done = 1;
				} else

				/* skip zeros */
				if (cb_val != 0) {

#ifdef M302de_ORIGINAL_BUGFIX
					if (cb_val < 0) {
						done = 1;
					} else
#endif

					if ( ((cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) && !hero_unconscious(get_hero(cb_val - 1))) ||
						((cb_val >= 50) && !is_in_word_array(cb_val - 50, g_cb_obj_nonobstacle)) ||
						((cb_val >= 10) && (cb_val < 30) && !g_enemy_sheets[cb_val - 10].flags.dead))
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
		return cb_val;
}


signed short get_foe_attack_mode(signed short mspell_id, signed short a2)
{
	signed short retval = 0;
	struct mon_spell_description *desc = &g_mon_spell_descriptions[mspell_id];

	if (a2 == 0) {

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

struct coords {
	signed short x,y;
};

struct dummy {
	struct coords d[4];
};

signed short seg037_0791(struct enemy_sheet* enemy, signed short enemy_no, signed short attack_foe, signed short x, signed short y)
{
	signed short available_spells;
	signed short l2;
	signed short done;
	signed short retval;
	signed short mode;
	signed short l6;
	signed short l7;
	struct dummy diff;
	signed short l_si;
	signed short l_di;

#if !defined(__BORLANDC__)
	diff.d[0].x = 1;
	diff.d[0].y = 0;
	diff.d[1].x = 0;
	diff.d[1].y = -1;
	diff.d[2].x = -1;
	diff.d[2].y = 0;
	diff.d[3].x = 0;
	diff.d[3].y = 1;
#else
	diff = *(struct dummy*)(p_datseg + VIEWDIR_OFFSETS5);
#endif

	retval = 0;

	available_spells = l_si = 0;
	while ((l_si < 5) && (ds_readbs(MON_SPELL_REPERTOIRE + enemy->mag_id * 5 + l_si++) != -1))
	{
		available_spells++;
	}

	done = 0;
	while ((done == 0) && (enemy->bp > 0)) {

		l7 = 0;

		for (l_si = 0; l_si < available_spells; l_si++) {

			l2 = ds_readbs(MON_SPELL_REPERTOIRE + enemy->mag_id * 5 + l_si);

			if (g_mon_spell_descriptions[l2].unkn1 == 1) {

				if (random_schick(100) < 75) {
					l7 = 1;
					break;
				}
			}
		}

		if (l7 == 0) {
			l2 = ds_readbs(MON_SPELL_REPERTOIRE + enemy->mag_id * 5 + random_interval(0, available_spells - 1));
		}

		enemy->enemy_id = 0;

		if ( (mode = get_foe_attack_mode(l2, attack_foe)) > 0) {

			if (mode == 3) {
				enemy->enemy_id = enemy_no + 10;
				enemy->cur_spell = (signed char)l2;
				retval = 1;
				done = 1;
			} else {

				if (!g_mon_spell_descriptions[l2].unkn1) {

					while (enemy->bp && (done == 0)) {

						l_si = enemy->viewdir;
						l_di = 0;

						while (!enemy->enemy_id && (l_di < 4)) {

							if (test_foe_melee_attack(x, y, diff.d[l_si].x, diff.d[l_si].y, mode)) {
								enemy->enemy_id = get_cb_val(x + diff.d[l_si].x, y + diff.d[l_si].y);
							}

							l_di++;
							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (enemy->enemy_id) {

							enemy->cur_spell = (signed char)l2;
							retval = 1;
							done = 1;

						} else if (enemy->bp > 0) {

							if (!enemy->flags.tied) {

								if (mode == 1)
									l6 = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 2);
								else
									l6 = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 0);

								if (l6 != -1) {
									seg037_00ae(enemy, enemy_no);
									FIG_search_obj_on_cb(enemy_no + 10, &x, &y);

#if !defined(__BORLANDC__)
									/* BE-fix */
									x = host_readws((Bit8u*)&x);
									y = host_readws((Bit8u*)&y);
#endif
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

						l_si = enemy->viewdir;
						l_di = 0;

						while (!enemy->enemy_id && (l_di < 4)) {

							enemy->enemy_id = test_foe_range_attack(x, y, l_si, mode);

							l_di++;
							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (enemy->enemy_id) {

							enemy->cur_spell =(signed char)l2;
							retval = 1;
							done = 1;

						} else if (enemy->bp > 0) {

							if (!enemy->flags.tied) {

								if (mode == 1)
									l6 = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 7);
								else
									l6 = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 6);

								if (l6 != -1) {
									seg037_00ae(enemy, enemy_no);
									FIG_search_obj_on_cb(enemy_no + 10, &x, &y);

#if !defined(__BORLANDC__)
									/* BE-fix */
									x = host_readws((Bit8u*)&x);
									y = host_readws((Bit8u*)&y);
#endif
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
		D1_INFO("Zauberspruch %d\n", enemy->cur_spell);
	else
		D1_INFO("Kein Zauberspruch\n");
#endif

	return retval;
}


/* REMARK: range weapon attack */
signed short seg037_0b3e(struct enemy_sheet *enemy, signed short enemy_no, signed short attack_foe, signed short x, signed short y)
{

	signed short cnt;
	signed short done;
	signed short retval;
	signed short l4;
	signed short dir;

	retval = 0;

	done = 0;

	while ((done == 0) && (enemy->bp > 0)) {

		/* reset the attackee ID */
		enemy->enemy_id = 0;

		while ( (done == 0) && (enemy->bp > 0) ) {

			dir = enemy->viewdir;
			cnt = 0;

			/* check clockwise for someone to attack */
			while (!enemy->enemy_id && (cnt < 4)) {

				enemy->enemy_id = test_foe_range_attack(x, y, dir, attack_foe);
				cnt++;
				if (++dir == 4) {
					dir = 0;
				}
			}

			if (enemy->enemy_id) {

				/* found someone to attack */
				retval = 1;
				done = 1;

			} else if (enemy->bp > 0) {

					if (!enemy->flags.tied) {

						if (attack_foe == 0)
							l4 = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 6);
						else
							l4 = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 7);

						if (l4 != -1) {
							seg037_00ae(enemy, enemy_no);
							FIG_search_obj_on_cb(enemy_no + 10, &x, &y);

#if !defined(__BORLANDC__)
							/* BE-fix */
							x = host_readws((Bit8u*)&x);
							y = host_readws((Bit8u*)&y);
#endif
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


void enemy_turn(struct enemy_sheet *enemy, signed short enemy_no, signed short x, signed short y)
{
	signed short target_reachable;
	signed short attack_foe;
	signed short dir;
	signed short l3;
	signed short done;
	signed short l5;
	signed short x_bak;
	signed short y_bak;
	struct dummy diff;
	signed short l_di;

	done = 0;

#if !defined(__BORLANDC__)
	diff.d[0].x = 1;
	diff.d[0].y = 0;
	diff.d[1].x = 0;
	diff.d[1].y = -1;
	diff.d[2].x = -1;
	diff.d[2].y = 0;
	diff.d[3].x = 0;
	diff.d[3].y = 1;
#else
	diff = *(struct dummy*)(p_datseg + VIEWDIR_OFFSETS6);
#endif

	/* check if we are in a special fight */

	if (g_current_fight_no == FIGHTS_F064) {

		/* F064: fight against GORAH */

		if (enemy->mon_id == 0x46) {
			/* GORAH waits the first 5 rounds */

			if (g_fight_round < 5) {
				enemy->bp = 0;
			}
		}

	} else if ((g_current_fight_no == FIGHTS_F099) && (random_interval(8, 12) <= g_fight_round)) {

		/* F099: fight against four HARPIES */

		/* after 8-12 rounds, the enemies flee */
		enemy->flags.scared = 1;

	} else if ((g_current_fight_no == FIGHTS_F122) && (FIG_count_active_enemies() <= 3)) {

		/* F122: fight against 13 WOLVES */

		/* if at most 3 wolves are left, all enemies flee */
		enemy->flags.scared = 1;

	} else if (g_current_fight_no == FIGHTS_F144) {

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

		FIG_init_list_elem(enemy_no + 10);

		draw_fight_screen_pal(0);

		enemy->enemy_id = 0;
		enemy->action_id = 1;

		/* LE threshold reached */
		if (enemy->le_flee >= enemy->le) {

			enemy->flags.scared = 1;
		}

		/* chance of 4% that an illusion enemy disappears */
		if (random_schick(100) < 5) {
#if !defined(__BORLANDC__)
			if (enemy->flags.illusion) {
				D1_INFO("Feind %d verliert seinen Illusionszauber\n", enemy_no);
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
			if ((enemy->mag_id != -1) && (enemy->ae >= 5) && seg037_0791(enemy, enemy_no, attack_foe, x, y))
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
			if ( ((enemy->shots > 0) || (enemy->throws > 0)) && seg037_0b3e(enemy, enemy_no, attack_foe, x, y))
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

			enemy->enemy_id = 0;
			dir = enemy->viewdir;
			l3 = 0;
			while (!enemy->enemy_id && (l3 < 4)) {

				if (test_foe_melee_attack(x, y, diff.d[dir].x, diff.d[dir].y, attack_foe)) {

					l5 = 1;

					if (is_in_byte_array(enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id))
					{

						l_di = get_cb_val(x - diff.d[dir].x, y - diff.d[dir].y);

						if (l_di && (enemy_no + 30 != l_di)) {

							if ((l_di < 0) || (l_di >= 50) || (l_di >= 30) ||
								((l_di > 0) && (l_di < 10) && !hero_dead(get_hero(l_di - 1))) ||
								((l_di < 30) && (l_di >= 10) && !g_enemy_sheets[l_di - 10].flags.dead))
							{
								l5 = 0;
							}
						}
					}

					if (l5 != 0) {
						enemy->enemy_id = get_cb_val(x + diff.d[dir].x, y + diff.d[dir].y);
					}
				}

				l3++;
				if (++dir == 4) {
					dir = 0;
				}
			}
		}

		if (enemy->enemy_id != 0) {

			enemy->action_id = 2;
			enemy->bp = enemy->bp - 3;
			done = 1;

		} else if (enemy->bp > 0) {

			if (!enemy->flags.tied) {

				if (enemy->flags.scared) {
					target_reachable = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 4);
					enemy->bp = 0;
				} else {
					if (enemy->flags.renegade)
						target_reachable = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 2);
					else
						target_reachable = FIG_find_path_to_target((Bit8u*)enemy, enemy_no, x, y, 0);
				}

				if (target_reachable != -1) {

					x_bak = x;
					y_bak = y;
					seg037_00ae(enemy, enemy_no);
					FIG_search_obj_on_cb(enemy_no + 10, &x, &y);

#if !defined(__BORLANDC__)
					/* BE-fix */
					x = host_readws((Bit8u*)&x);
					y = host_readws((Bit8u*)&y);
#endif
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

#if !defined(__BORLANDC__)
}
#endif
