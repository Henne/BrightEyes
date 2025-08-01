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
	buffer = (Bit8u*)ds_readd(BUFFER_ANIDAT);

	/* WEAPANI.DAT */
	if (mode == 3)
		buffer = (Bit8u*)ds_readd(BUFFER_WEAPANIDAT);

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

void seg037_00ae(Bit8u *enemy, signed short enemy_no)
{
	signed char b1;
	signed char b2;
	signed char b3;
	Bit8u *p1;
	Bit8u *p2;
	Bit8u *p3;

	signed short i;

	ds_writeb((FIG_ANISHEETS + 0xf3), 0); /* first position of the second FIG_ANISHEET (0xf3 == 243, FIG_ANISHEET is a struct(243)[8]) */
	ds_writeb((FIG_ANISHEETS + 242 + 0xf3), host_readbs(enemy + ENEMY_SHEET_GFX_ID)); /* last position of the second FIG_ANISHEET */
	p1 = p_datseg + (FIG_ANISHEETS + 1 + 0xf3); /* second position of the second FIG_ANISHEET */

	i = 0;
	p3 = (Bit8u*)(ds_readd(GFX_ANI_INDEX + host_readbs(enemy + ENEMY_SHEET_GFX_ID) * 4));

	while (ds_readbs(FIG_MOVE_PATHDIR + i) != -1) {

		if (host_readbs(enemy + ENEMY_SHEET_VIEWDIR) != ds_readbs(FIG_MOVE_PATHDIR + i)) {

			b2 = b1 = -1;
			b3 = host_readbs(enemy + ENEMY_SHEET_VIEWDIR);
			b2 = b3;

			b3++;

			if (b3 == 4) {
				b3 = 0;
			}

			if (ds_readbs(FIG_MOVE_PATHDIR + i) != b3) {

				b1 = b3;
				b3++;

				if (b3 == 4) {
					b3 = 0;
				}

				if (ds_readbs(FIG_MOVE_PATHDIR + i) != b3) {
					b2 = host_readbs(enemy + ENEMY_SHEET_VIEWDIR) + 4;
					b1 = -1;
				}

			}

			host_writeb(enemy + ENEMY_SHEET_VIEWDIR, ds_readbs(FIG_MOVE_PATHDIR + i));

			p1 += copy_ani_stuff(p1, host_readws(p3 + b2 * 2), 1);

			if (b1 != -1) {

				p1 += copy_ani_stuff(p1, host_readws(p3 + b1 * 2), 1);
			}
		}

		if (ds_readbs(FIG_MOVE_PATHDIR + i) == ds_readbs((FIG_MOVE_PATHDIR+1) + i)) {

			p1 += copy_ani_stuff(p1, host_readws(p3 + (ds_readbs(FIG_MOVE_PATHDIR + i) + 0x0c) * 2), 1);
			i += 2;
			/* BP - 2 */
			host_writeb(enemy + ENEMY_SHEET_BP, host_readbs(enemy + ENEMY_SHEET_BP) - 2);

		} else {
			p1 += copy_ani_stuff(p1, host_readws(p3 + (ds_readbs(FIG_MOVE_PATHDIR + i) + 0x08) * 2), 1);
			i++;
			/* BP - 1 */
			dec_ptr_bs(enemy + ENEMY_SHEET_BP);
		}
	}

	/* terminate array */
	host_writeb(p1, -1);

	FIG_call_draw_pic();

	FIG_remove_from_list(ds_readbs(FIG_CB_MAKRER_ID), 0);

	ds_writeb(FIG_CB_MAKRER_ID, -1);

	FIG_set_sheet(host_readbs(enemy + ENEMY_SHEET_FIGHTER_ID), 1);

	if (is_in_byte_array(host_readbs(enemy + ENEMY_SHEET_GFX_ID), p_datseg + TWO_FIELDED_SPRITE_ID)) {

		memcpy(p_datseg + (FIG_ANISHEETS + 3*0xf3), p_datseg + (FIG_ANISHEETS + 0xf3), 0xf3);

		p2 = (Bit8u*)(FIG_get_ptr(host_readbs(enemy + ENEMY_SHEET_FIGHTER_ID)));

		FIG_set_sheet(ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(p2 + FIGHTER_TWOFIELDED)), 3);
	}

	/* draw_fight_screen */
	draw_fight_screen(0);

	memset(p_datseg + (FIG_ANISHEETS + 0xf3), -1, 0xf3); /* set second FIG_ANISHEET to -1 */
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
	signed char cb_val;

	cb_val = get_cb_val(x + dx, y + dy);

	if (mode == 0) {

		if ( ((cb_val > 0) && (cb_val < 10) &&
			(!hero_dead(get_hero(cb_val - 1))) &&
			(!hero_unconscious(get_hero(cb_val - 1)))
			) || (
			(cb_val >= 10) && (cb_val < 30) &&
				(!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + cb_val * SIZEOF_ENEMY_SHEET))  &&
				(enemy_renegade(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + cb_val * SIZEOF_ENEMY_SHEET)))
			)
		{
			return 1;
		} else {
			return 0;
		}

	} else if (mode == 1) {

		/* is a living enemy */
		if ((cb_val >= 10) && (cb_val < 30) && (!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + cb_val * SIZEOF_ENEMY_SHEET)))
		{
			return 1;
		} else {
			return 0;
		}

	} else if (mode == 2) {

		/* is a living, conscious hero */
		if ((cb_val > 0) && (cb_val < 10) && (!hero_dead(get_hero(cb_val - 1))) && (!hero_unconscious(get_hero(cb_val - 1))))
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
				if ( (
					(cb_val > 0) && (cb_val < 10) &&
						!hero_dead(get_hero(cb_val - 1)) &&
						!hero_unconscious(get_hero(cb_val - 1))
					) || (
					(cb_val >= 10) && (cb_val < 30) &&
						!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * cb_val) &&
						enemy_renegade(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * cb_val))
				)
				{
					can_attack = 1;
					done = 1;

				} else

				/* if field is not empty */
				if (cb_val != 0) {

					/* an enemy or another object */
					if ( (
						(cb_val >= 10) && (cb_val < 30) &&
							!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * cb_val)
						) || (
						(cb_val >= 50) &&
							!is_in_word_array(cb_val - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE)))

					)
					{
							done = 1;
					}
				}

			} else if (mode == 1) {
				/* attack foe first */
				if ((cb_val >= 10) && (cb_val < 30) && !enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * cb_val))
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
						((cb_val >= 50) && !is_in_word_array(cb_val - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))))
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

					if ( (
						(cb_val < 10) && !hero_dead(get_hero(cb_val - 1)) &&
						!hero_unconscious(get_hero(cb_val - 1))
						) || (
						(cb_val >= 50) &&
							!is_in_word_array(cb_val - 50, (signed short*)(p_datseg + CB_OBJ_NONOBSTACLE))
						) || (
						(cb_val >= 10) && (cb_val < 30) &&
							!enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * cb_val))
					)
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


signed short get_foe_attack_mode(signed short a1, signed short a2)
	/* a1: ID of monster spell */
{
	signed short retval = 0;
	Bit8u *ptr = p_datseg + MON_SPELL_DESCRIPTIONS + a1 * SIZEOF_MON_SPELL_DESCRIPTIONS;

	if (a2 == 0) {

		if ((host_readbs(ptr + MON_SPELL_DESCRIPTIONS_MODE) == 3) || (host_readbs(ptr + MON_SPELL_DESCRIPTIONS_MODE) == 2)) {
			retval = 2;
		} else {
			if (host_readbs(ptr + MON_SPELL_DESCRIPTIONS_MODE) == 1) {
				retval = 1;
			} else {
				retval = 3;
			}
		}

	} else {
		if (host_readbs(ptr + MON_SPELL_DESCRIPTIONS_MODE) == 3) {
			retval = 1;
		} else if (host_readbs(ptr + MON_SPELL_DESCRIPTIONS_MODE) == 0) {
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

signed short seg037_0791(Bit8u* enemy, signed short enemy_no, signed short attack_foe, signed short x, signed short y)
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
	while ((l_si < 5) && (ds_readbs(MON_SPELL_REPERTOIRE + host_readbs(enemy + ENEMY_SHEET_MAG_ID) * 5 + l_si++) != -1))
	{
		available_spells++;
	}

	done = 0;
	while ((done == 0) && (host_readbs(enemy + ENEMY_SHEET_BP) > 0)) {

		l7 = 0;

		for (l_si = 0; l_si < available_spells; l_si++) {

			l2 = ds_readbs(MON_SPELL_REPERTOIRE + host_readbs(enemy + ENEMY_SHEET_MAG_ID) * 5 + l_si);

			if (ds_readbs((MON_SPELL_DESCRIPTIONS_UNKN1 + MON_SPELL_DESCRIPTIONS) + l2 * SIZEOF_MON_SPELL_DESCRIPTIONS) == 1) {

				if (random_schick(100) < 75) {
					l7 = 1;
					break;
				}
			}
		}

		if (l7 == 0) {
			l2 = ds_readbs(MON_SPELL_REPERTOIRE + host_readbs(enemy + ENEMY_SHEET_MAG_ID) * 5 + random_interval(0, available_spells - 1));
		}

		host_writeb(enemy + ENEMY_SHEET_ENEMY_ID, 0);

		if ( (mode = get_foe_attack_mode(l2, attack_foe)) > 0) {

			if (mode == 3) {
				host_writeb(enemy + ENEMY_SHEET_ENEMY_ID, enemy_no + 10);
				host_writeb(enemy + ENEMY_SHEET_CUR_SPELL, (signed char)l2);
				retval = 1;
				done = 1;
			} else {

				if (!ds_readbs((MON_SPELL_DESCRIPTIONS + MON_SPELL_DESCRIPTIONS_UNKN1) + l2 * SIZEOF_MON_SPELL_DESCRIPTIONS)) {

					while ((host_readbs(enemy + ENEMY_SHEET_BP) != 0) && (done == 0)) {

						l_si = host_readbs(enemy + ENEMY_SHEET_VIEWDIR);
						l_di = 0;

						while (!host_readbs(enemy + ENEMY_SHEET_ENEMY_ID) && (l_di < 4)) {

							if (test_foe_melee_attack(x, y, diff.d[l_si].x, diff.d[l_si].y, mode)) {
								host_writeb(enemy + ENEMY_SHEET_ENEMY_ID, get_cb_val(x + diff.d[l_si].x, y + diff.d[l_si].y));
							}

							l_di++;
							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (host_readbs(enemy + ENEMY_SHEET_ENEMY_ID)) {
							host_writeb(enemy + ENEMY_SHEET_CUR_SPELL, (signed char)l2);
							retval = 1;
							done = 1;
						} else if (host_readbs(enemy + ENEMY_SHEET_BP) > 0) {
							if (!enemy_tied(enemy)) {

								if (mode == 1)
									l6 = FIG_find_path_to_target(enemy, enemy_no, x, y, 2);
								else
									l6 = FIG_find_path_to_target(enemy, enemy_no, x, y, 0);

								if (l6 != -1) {
									seg037_00ae(enemy, enemy_no);
									FIG_search_obj_on_cb(enemy_no + 10, &x, &y);

#if !defined(__BORLANDC__)
									/* BE-fix */
									x = host_readws((Bit8u*)&x);
									y = host_readws((Bit8u*)&y);
#endif
									if (host_readbs(enemy + ENEMY_SHEET_BP) < 3) {
										host_writeb(enemy + ENEMY_SHEET_BP, 0);
									}
								} else {
									host_writeb(enemy + ENEMY_SHEET_BP, 0);
								}
							} else {
								host_writeb(enemy + ENEMY_SHEET_BP, 0);
							}
						}
					}
				} else {

					while ((done == 0) && (host_readbs(enemy + ENEMY_SHEET_BP) > 0)) {

						l_si = host_readbs(enemy + ENEMY_SHEET_VIEWDIR);
						l_di = 0;

						while (!host_readbs(enemy + ENEMY_SHEET_ENEMY_ID) && (l_di < 4)) {

							host_writeb(enemy + ENEMY_SHEET_ENEMY_ID, (signed char)test_foe_range_attack(x, y, l_si, mode));

							l_di++;
							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (host_readbs(enemy + ENEMY_SHEET_ENEMY_ID)) {
							host_writeb(enemy + ENEMY_SHEET_CUR_SPELL, (signed char)l2);
							retval = 1;
							done = 1;
						} else if (host_readbs(enemy + ENEMY_SHEET_BP) > 0) {
							if (!enemy_tied(enemy)) {

								if (mode == 1)
									l6 = FIG_find_path_to_target(enemy, enemy_no, x, y, 7);
								else
									l6 = FIG_find_path_to_target(enemy, enemy_no, x, y, 6);

								if (l6 != -1) {
									seg037_00ae(enemy, enemy_no);
									FIG_search_obj_on_cb(enemy_no + 10, &x, &y);

#if !defined(__BORLANDC__)
									/* BE-fix */
									x = host_readws((Bit8u*)&x);
									y = host_readws((Bit8u*)&y);
#endif
									if (host_readbs(enemy + ENEMY_SHEET_BP) < 5) {
										host_writeb(enemy + ENEMY_SHEET_BP, 0);
									}
								} else {
									host_writeb(enemy + ENEMY_SHEET_BP, 0);
								}
							} else {
								host_writeb(enemy + ENEMY_SHEET_BP, 0);
							}
						}
					}
				}
			}
		}
	}

#if !defined(__BORLANDC__)
	if (retval)
		D1_INFO("Zauberspruch %d\n", host_readbs(enemy + ENEMY_SHEET_CUR_SPELL));
	else
		D1_INFO("Kein Zauberspruch\n");
#endif

	return retval;
}


signed short seg037_0b3e(Bit8u* enemy, signed short enemy_no, signed short attack_foe, signed short x, signed short y)
{

	signed short cnt;
	signed short done;
	signed short retval;
	signed short l4;
	signed short dir;

	retval = 0;

	done = 0;

	while ((done == 0) && (host_readbs(enemy + ENEMY_SHEET_BP) > 0)) {

		/* reset the attackee ID */
		host_writeb(enemy + ENEMY_SHEET_ENEMY_ID, 0);

		while ( (done == 0) && (host_readbs(enemy + ENEMY_SHEET_BP) > 0)) {

			dir = host_readbs(enemy + ENEMY_SHEET_VIEWDIR);
			cnt = 0;

			/* check clockwise for someone to attack */
			while ( !host_readbs(enemy + ENEMY_SHEET_ENEMY_ID) && (cnt < 4)) {

				host_writeb(enemy + ENEMY_SHEET_ENEMY_ID,
					(signed char)test_foe_range_attack(x, y, dir, attack_foe));
				cnt++;
				if (++dir == 4) {
					dir = 0;
				}
			}

			if (host_readbs(enemy + ENEMY_SHEET_ENEMY_ID) != 0) {
				/* found someone to attack */
				retval = 1;
				done = 1;
			} else if (host_readbs(enemy + ENEMY_SHEET_BP) > 0) {

					if (!enemy_tied(enemy)) {
						if (attack_foe == 0)
							l4 = FIG_find_path_to_target(enemy, enemy_no, x, y, 6);
						else
							l4 = FIG_find_path_to_target(enemy, enemy_no, x, y, 7);

						if (l4 != -1) {
							seg037_00ae(enemy, enemy_no);
							FIG_search_obj_on_cb(enemy_no + 10, &x, &y);

#if !defined(__BORLANDC__)
							/* BE-fix */
							x = host_readws((Bit8u*)&x);
							y = host_readws((Bit8u*)&y);
#endif
							if (host_readbs(enemy + ENEMY_SHEET_BP) < 3) {
								host_writeb(enemy + ENEMY_SHEET_BP, 0);
							}
						} else {
							host_writeb(enemy + ENEMY_SHEET_BP, 0);
						}
					} else {
						host_writeb(enemy + ENEMY_SHEET_BP, 0);
					}
			}
		}
	}

	return retval;
}


void enemy_turn(Bit8u *enemy, signed short enemy_no, signed short x, signed short y)
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

		if (host_readbs(enemy) == 0x46) {
			/* GORAH waits the first 5 rounds */

			if (g_fight_round < 5) {
				host_writeb(enemy + ENEMY_SHEET_BP, 0);
			}
		}

	} else if ((g_current_fight_no == FIGHTS_F099) &&
		/* F099: fight against four HARPIES */
			(random_interval(8, 12) <= g_fight_round)) {

			/* after 8-12 rounds, the enemies flee */
			or_ptr_bs(enemy + ENEMY_SHEET_FLAGS2, 4); /* set 'scared' flag */

	} else if ((g_current_fight_no == FIGHTS_F122) && /* 13 wolves */
		/* F122: fight against 13 WOLVES */
			(FIG_count_active_enemies() <= 3)) {

			/* if at most 3 wolves are left, the enemies flee */
			or_ptr_bs(enemy + ENEMY_SHEET_FLAGS2, 4); /* set 'scared' flag */

	} else if (g_current_fight_no == FIGHTS_F144) {
		/* F144: final fight */

		if (enemy_tied(enemy)) {
			host_writeb(enemy + ENEMY_SHEET_BP, 0);
		}
	}

	while ( (done == 0) && (host_readbs(enemy + ENEMY_SHEET_BP) > 0)) {

		if (ds_readbs(FIG_CB_MAKRER_ID) != -1) {
			FIG_remove_from_list(ds_readbs(FIG_CB_MAKRER_ID), 0);
			ds_writebs(FIG_CB_MAKRER_ID, -1);
		}

		FIG_init_list_elem(enemy_no + 10);

		draw_fight_screen_pal(0);

		host_writeb(enemy + ENEMY_SHEET_ENEMY_ID, 0);
		host_writeb(enemy + ENEMY_SHEET_ACTION_ID, 1);

		/* should I flee */
		if (host_readbs(enemy + ENEMY_SHEET_LE_FLEE) >= host_readws(enemy + ENEMY_SHEET_LE)) {
#if !defined(__BORLANDC__)
			D1_INFO("Feind %d flieht\n", enemy_no);
#endif
			or_ptr_bs(enemy + ENEMY_SHEET_FLAGS2, 4); /* set 'scared' flag */
		}

		/* chance of 4% that an illusion enemy disappears */
		if (random_schick(100) < 5) {
#if !defined(__BORLANDC__)
			if (enemy_illusion(enemy)) {
				D1_INFO("Feind %d verliert seinen Illusionszauber\n", enemy_no);
			}
#endif
			/* Original-Bug? Why unset 'tied' and not 'illusion'?? */
			and_ptr_bs(enemy + ENEMY_SHEET_FLAGS1, 0xdf); /* unset 'tied' flag */
		}

		if (!enemy_scared(enemy)) {

			attack_foe = 0;

			if (enemy_renegade(enemy)) {
				attack_foe = 1;
			}

			/* enemy can cast spells and has AE >= 5 left */
			if ((host_readbs(enemy + ENEMY_SHEET_MAG_ID) != -1) && (host_readws(enemy + ENEMY_SHEET_AE) >= 5) &&
				(seg037_0791(enemy, enemy_no, attack_foe, x, y)))
			{
#if !defined(__BORLANDC__)
				D1_INFO("Feind %d zaubert\n", enemy_no);
#endif
				host_writeb(enemy + ENEMY_SHEET_ACTION_ID, 4);

				/* adjust BP */
				host_writeb(enemy + ENEMY_SHEET_BP, host_readbs(enemy + ENEMY_SHEET_BP) -5);

				if (host_readbs(enemy + ENEMY_SHEET_BP) < 0) {
					host_writeb(enemy + ENEMY_SHEET_BP, 0);
				}
				return;
			}

			/* enemy has range weapons */
			if ( ((host_readbs(enemy + ENEMY_SHEET_SHOTS) > 0) || (host_readbs(enemy + ENEMY_SHEET_THROWS) > 0)) &&
				seg037_0b3e(enemy, enemy_no, attack_foe, x, y))
			{
#if !defined(__BORLANDC__)
				D1_INFO("Feind %d greift mit Fernkampfwaffe an\n", enemy_no);
#endif
				host_writeb(enemy + ENEMY_SHEET_ACTION_ID, 15);

				/* adjust BP */
				host_writeb(enemy + ENEMY_SHEET_BP, host_readbs(enemy + ENEMY_SHEET_BP) -3);

				if (host_readbs(enemy + ENEMY_SHEET_BP) < 0) {
					host_writeb(enemy + ENEMY_SHEET_BP, 0);
				}
				return;
			}

			host_writeb(enemy + ENEMY_SHEET_ENEMY_ID, 0);
			dir = host_readbs(enemy + ENEMY_SHEET_VIEWDIR);
			l3 = 0;
			while (!(host_readbs(enemy + ENEMY_SHEET_ENEMY_ID)) && (l3 < 4)) {

				if (test_foe_melee_attack(x, y, diff.d[dir].x, diff.d[dir].y, attack_foe)) {

					l5 = 1;

					if (is_in_byte_array(host_readbs(enemy + ENEMY_SHEET_GFX_ID), p_datseg + TWO_FIELDED_SPRITE_ID))
					{

						l_di = get_cb_val(x - diff.d[dir].x, y - diff.d[dir].y);

						if (l_di && (enemy_no + 30 != l_di)) {

							if ((l_di < 0) || (l_di >= 50) || (l_di >= 30) ||
								((l_di > 0) && (l_di < 10) && !hero_dead(get_hero(l_di - 1))) ||
								((l_di < 30) && (l_di >= 10) && !(enemy_dead(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * l_di))))
							{
								l5 = 0;
							}
						}
					}

					if (l5 != 0) {
						host_writeb(enemy + ENEMY_SHEET_ENEMY_ID, get_cb_val(x + diff.d[dir].x, y + diff.d[dir].y));
					}
				}

				l3++;
				if (++dir == 4) {
					dir = 0;
				}
			}
		}

		if (host_readbs(enemy + ENEMY_SHEET_ENEMY_ID) != 0) {
			host_writeb(enemy + ENEMY_SHEET_ACTION_ID, 2);
			host_writeb(enemy + ENEMY_SHEET_BP, host_readbs(enemy + ENEMY_SHEET_BP) -3);
			done = 1;
		} else if (host_readbs(enemy + ENEMY_SHEET_BP) > 0) {

			if (!enemy_tied(enemy)) {

				if (enemy_scared(enemy)) {
					target_reachable = FIG_find_path_to_target(enemy, enemy_no, x, y, 4);
					host_writeb(enemy + ENEMY_SHEET_BP, 0);
				} else {
					if (enemy_renegade(enemy))
						target_reachable = FIG_find_path_to_target(enemy, enemy_no, x, y, 2);
					else
						target_reachable = FIG_find_path_to_target(enemy, enemy_no, x, y, 0);
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
						host_writeb(enemy + ENEMY_SHEET_BP, 0);
					}

					if (host_readbs(enemy + ENEMY_SHEET_BP) < 3) {
						host_writeb(enemy + ENEMY_SHEET_BP, 0);
					}

				} else {
					host_writeb(enemy + ENEMY_SHEET_BP, 0);
				}
			} else {
				host_writeb(enemy + ENEMY_SHEET_BP, 0);
			}

			host_writeb(enemy + ENEMY_SHEET_ACTION_ID, 1);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
