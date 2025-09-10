/*
 *	Rewrite of DSA1 v3.02_de functions of seg032 (fight)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg032.cpp
 *
 */
#include <stdlib.h>
#include <string.h>

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg008.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg033.h"
#include "seg034.h"
#include "seg035.h"
#include "seg037.h"
#include "seg038.h"
#include "seg039.h"
#include "seg040.h"
#include "seg042.h"
#include "seg043.h"
#include "seg049.h"
#include "seg097.h"
#include "seg105.h"
#include "seg113.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   sets an object to a chessboard position
 *
 * \param   y           the Y-Coordinate on the chessboard
 * \param   x           the X-Coordinate on the chessboard
 * \param   object      number of the object
 *			0=free, 1-6=heroes, >10=enemies,
 *			>50walls, trees, etc
 *			-3 = flee
 *
 *	This function is called multiple times when an entrant in a fight
 *	is moving. The initial placements of static objects are also done
 *	with this function.
 */
/* The functions in this file need object to be signed short.
 * All other callers use signed short from the header.
 */
void FIG_set_cb_field(signed short y, signed short x, signed short object)
{
	/* check that the object is in the borders */
	if ((y >= 0) && (y <= 24) && (x >= 0) && (x <= 24)) {
		g_chessboard[y * 25 + x] = ((signed char)object);
	}
}

void draw_fight_screen_pal(signed short mode)
{
	FIG_draw_pic();

	/* check for palette update */
	if (g_pp20_index != -1) {

		update_mouse_cursor();

		/* clear framebuffer */
		/* TODO: add update */
		memset((void*)g_vga_memstart, 0, 320 * 200);

		/* set palettes */
		set_palette((Bit8u*)g_palette_fight1, 0x00, 0x20);
		set_palette((Bit8u*)g_palette_fight2, 0x80, 0x14);
		set_palette(g_buffer8_ptr + 0xfa02, 0x60, 0x20);

		g_pp20_index = -1;

		refresh_screen_size();
	}

	draw_fight_screen(mode);
}

/**
 * \brief   chooses the next hero on turn
 *
 *          This is simply done randomly.
 */
signed short FIG_choose_next_hero(void)
{
#if !defined(__BORLANDC__)
	signed short loop_cnt = 0;
	long tries[7] = {0, 0, 0, 0, 0, 0, 0};
#endif

	signed short retval;

	do {
		retval = random_schick(7) - 1;

#if !defined(__BORLANDC__)
		tries[retval]++;

		if (++loop_cnt > 200) {
			D1_ERR("Possible infinite loop in %s()\n", __func__);
			D1_ERR("I'll try to get a possible hero\n");

			retval = -1;

			/*
			 * print random statistic
			 */
			for (int i = 0; i < 7; i++)
				D1_ERR("tries[%d] = %ld\n", i, tries[i]);

			/*
			 * search by hand for a hero and dump the
			 * interesting bits
			 */
			Bit8u *hero = get_hero(0);
			for (int i = 0; i < 7; i++, hero += SIZEOF_HERO) {
				D1_ERR("Hero %d typus = %x group=%x current_group=%x actions=%x\n",
					i, host_readb(hero + HERO_TYPE),
					host_readb(hero + HERO_GROUP_NO),
					gs_current_group,
					host_readb(hero + HERO_ACTIONS));

				if (host_readb(hero + HERO_TYPE) &&
					host_readb(hero + HERO_GROUP_NO) == gs_current_group &&
					host_readb(hero + HERO_ACTIONS))
						retval = i;
			}

			/* exit if we didn't find a hero */
			if (retval == -1) {
				D1_ERR("Sorry, this is an infinite loop.\n");
				D1_ERR("I'll exit\n");
				exit(1);
			}

			return retval;
		}
#endif


	/* search for a hero who has a class, is in the current group and
		something still unknown */
	} while (host_readb(get_hero(retval) + HERO_TYPE) == HERO_TYPE_NONE ||
			host_readb(get_hero(retval) + HERO_GROUP_NO) != gs_current_group ||
			host_readb(get_hero(retval) + HERO_ACTIONS) == 0);

	return retval;
}
/**
 * \brief   chooses the next enemy on turn
 *
 *          This is simply done randomly.
 *
 *	Orig_BUG: I had this loop running infinitely.
 */
signed short FIG_choose_next_enemy(void)
{
	signed short retval;

#if !defined(__BORLANDC__)
	struct enemy_sheet *enemy;
	unsigned short i;
	unsigned short loop_cnt = 0;
	long tries[20] = {	0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0,
				0, 0, 0, 0, 0};
#endif

	do {

		retval = random_schick(g_nr_of_enemies) - 1;

#if !defined(__BORLANDC__)
		tries[retval]++;

		if (++loop_cnt > 200) {
			D1_ERR("Possible infinite loop in %s()\n", __func__);
			D1_ERR("I'll try to get a possible enemy\n");

			retval = -1;

			/*
			 * print random statistic
			 */
			for (i = 0; i < 20; i++)
				D1_ERR("tries[%d] = %ld\n", i, tries[i]);

			/* search by hand for an enemy and dump	the interesting bits */
			enemy = &g_enemy_sheets[0];
			for (i = 0; i < g_nr_of_enemies; i++, enemy++) {

				D1_ERR("Enemy %02d %x %x\n", i, enemy->mon_id, enemy->attacks_left);

				if (enemy->mon_id && enemy->attacks_left)
						retval = i;
			}

			/* exit if we didn't find an enemy */
			if (retval == -1) {
				D1_ERR("Sorry, this is an infinite loop.\n");
				D1_ERR("I'll exit\n");
				exit(1);
			}

			return retval;
		}
		enemy = &g_enemy_sheets[retval];
#endif

	} while (g_enemy_sheets[retval].mon_id == 0 || g_enemy_sheets[retval].attacks_left == 0);

	return retval;
}

/**
 * \brief   return the number of active enemies
 */
signed short FIG_count_active_enemies(void)
{
	struct enemy_sheet *enemy;
	signed short i;
	signed short retval = 0;

	for (i = 0; i < 20; i++) {

		enemy = &g_enemy_sheets[i];

		if (enemy->mon_id &&
			!enemy->flags.dead &&
			!enemy->flags.petrified &&
			!enemy->flags.tied &&
			!enemy->flags.mushroom &&
			!enemy->flags.busy &&
			!enemy->round_appear)
		{
			retval++;
		}
	}

	return retval;
}

/**
 * \brief   checks if an enemy is active
 *
 * \param   enemy       pointer to the enemy sheet
 * \return              1 if enemy can act or 0 if not.
 */
//static
signed short FIG_is_enemy_active(struct enemy_sheet *enemy)
{
	if (enemy->flags.asleep ||
		enemy->flags.dead ||
		enemy->flags.petrified ||
		enemy->flags.dancing ||
		enemy->flags.mushroom ||
		enemy->flags.busy ||
		enemy->flags.tame ||
		(enemy->round_appear > 0))
	{
		return 0;
	}

	return 1;
}

/**
 * \brief   returns the first active hero
 *
 * \return              the index of the firsta active hero.
 */
signed short FIG_get_first_active_hero(void)
{
	Bit8u *hero_i;
	signed short i;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		if ((host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readb(hero_i + HERO_GROUP_NO) == gs_current_group) &&
			!hero_dead(hero_i) &&
			!hero_petrified(hero_i) &&
			!hero_renegade(hero_i) &&
			!hero_scared(hero_i) &&
			!hero_unconscious(hero_i))
		{
			return i;
		}
	}

	return -1;
}

/**
 * \brief   1 if all (active) heroes in group escaped from the fight
 *
 * \return              1 if FIG_get_first_active_hero() returns -1
 *                      and at least one hero in the group is not dead and has
 *                      something at offset HERO_ACTION_ID set (maybe sleeping).
 */
//static
unsigned short FIG_all_heroes_escaped(void)
{
	Bit8u *hero_i;
	signed short i;

	if (FIG_get_first_active_hero() == -1) {
		hero_i = get_hero(0);
		for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
			if ((host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readb(hero_i + HERO_GROUP_NO) == gs_current_group) &&
				!hero_dead(hero_i) &&
				(host_readb(hero_i + HERO_ACTION_ID) == FIG_ACTION_FLEE))
			{
				return 1;
			}
		}

		return 0;
	} else {
		return 0;
	}
}

//static
unsigned short FIG_fight_continues(void)
{

	if (FIG_all_heroes_escaped()) {
		g_fig_all_heroes_withdrawn = 1;
		return 0;
	}

	if (FIG_get_first_active_hero() == -1) {
		g_game_state = (GAME_STATE_DEAD);
		return 0;
	}

	if (FIG_count_active_enemies() == 0)
		return 0;

	return 1;
}

void FIG_do_round(void)
{
	signed short i;
	signed short nr_hero_action_phases_left_in_round; /* total number over all heroes */
	signed short nr_enemy_action_phases_left_in_round; /* total number over all enemies */
	signed short actor_id;
	signed short x_coord;
	signed short y_coord;
	signed short nr_action_phases_left_in_turn; /* number of action phases left in the turn of an actor */
	signed char is_enemies_turn; /* 0: enemies' turn; 1: heroes' turn */
	Bit8u* hero;
	struct enemy_sheet *enemy;
	signed short x;
	signed short y;
	struct struct_fighter *fighter_ptr;

	/* A round is the phase of a fight where all heroes and enemies get their number of BP (Bewegungspunkte; depending on load, enemy type etc.) and use them to perform a series of actions.
	 * Actions are performed in action phases, An action phase consists of one or more actions by the active actor, where the actions 'melee attack', 'ranged attack',
	 * 'cast spell', 'use item', 'guard' (and others?) terminate the phase and 'move', 'change weapon', 'change item' etc. do not.
	 * The total number of action phases per round of an actor is usually 1, but can be 2 for certain types of enmies or with active Axxeleratus or Boeser Blick spells.
	 *
	 * A round is divided into turns, which alternate between heroes' and enemies' turns.
	 * In a turn, a number of action phases is performed by actors on the active side.
	 * This number of action phases depends on the relation between the number of remaining action phases of the heroes and the enemies.
	 * Phase by phase the actor on the active side is picked randomly among the actors which have action phases left. */

	if (!FIG_fight_continues()) {
		/* this fight is over */
		g_in_fight = 0;
	}

#if !defined(__BORLANDC__)
	D1_INFO("Kampfrunde %d beginnt\n", g_fight_round);
#endif

	nr_hero_action_phases_left_in_round = 0;

	/* initialize heroes' #action phases and BP */
	for (i = 0; i <= 6; (g_hero_is_target[i] = 0), i++) {

		hero = get_hero(i);

		if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
			(host_readbs(hero + HERO_ACTION_ID) != FIG_ACTION_FLEE))
		{
			/* set #action phases to 1 */
			host_writeb(hero + HERO_ACTIONS, 1);

			/* give this hero 8 BP */
			host_writeb(hero + HERO_BP_LEFT, 8);

			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 50 <= host_readws(hero + HERO_LOAD)) {
				/* load at least 50% of carrying capacity: give BP malus -1 */
				dec_ptr_bs(hero + HERO_BP_LEFT);
			}

			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 75 <= host_readws(hero + HERO_LOAD)) {
				/* load at least 75% of carrying capacity: give additional BP malus -2 */
				sub_ptr_bs(hero + HERO_BP_LEFT, 2);
			}

			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 100 <= host_readws(hero + HERO_LOAD)) {
				/* load at least 100% of carrying capacity: give additional give BP malus -2 */
				sub_ptr_bs(hero + HERO_BP_LEFT, 2);

			}

			host_writew(hero + HERO_ESCAPE_POSITION, 0);

			nr_hero_action_phases_left_in_round++;

			if (host_readbs(hero + HERO_AXXELERATUS) != 0) {
				/* Axxeleratus => BP + 4 ... */
				add_ptr_bs(hero + HERO_BP_LEFT, 4);

				/* ... and one extra action phase */
				inc_ptr_bs(hero + HERO_ACTIONS);

				nr_hero_action_phases_left_in_round++;
			}

			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 110 <= host_readws(hero + HERO_LOAD)) {
				/* load at least 110% of carrying capacity: set BP to 1 */
				host_writeb(hero + HERO_BP_LEFT, 1);
			}
		}
	}

	nr_enemy_action_phases_left_in_round = 0;

	for (i = 0; i < g_nr_of_enemies; i++) {

		/* set #phases */
		g_enemy_sheets[i].attacks_left = g_enemy_sheets[i].attacks;

		nr_enemy_action_phases_left_in_round += g_enemy_sheets[i].attacks;

		/* set BP */
		g_enemy_sheets[i].bp = g_enemy_sheets[i].bp_orig;

		g_fig_actors_unkn[i + 10] = 0;
	}

	nr_action_phases_left_in_turn = 0;

	/* the variable is set up 'the wrong way round',
	 * as it will be flipped in the first run */
	is_enemies_turn = (g_fig_initiative == 2 ? 1 : (g_fig_initiative == 1 ? 0 : random_interval(0, 1)));

	while ((g_in_fight) && (nr_hero_action_phases_left_in_round + nr_enemy_action_phases_left_in_round > 0)) {

		if (g_autofight == 2) {
			g_autofight = 0;
		}

		/* decide if heroes or enemies are next */
		if (nr_action_phases_left_in_turn == 0) {

			/* flip turn */
			is_enemies_turn ^= 1;

			if (!is_enemies_turn) {
				/* heroes' turn */

				/* this might be an Original-Bug:
				 * The code block here is similar, but not equivalent to to the corresponding block for the enemies' turn below.
				 * I'd expect first to check nr_hero_action_phases_left_in_round == 0 -> switch turn to enemies
				 * and then check nr_hero_action_phases_left_in_round <= nr_enemy_action_phases_left_in_round
				 * as below in the corresponding lines for the enemies. */
				if (nr_hero_action_phases_left_in_round <= nr_enemy_action_phases_left_in_round) {
					nr_action_phases_left_in_turn = 1;
				} else if (nr_hero_action_phases_left_in_round == 0) {
					is_enemies_turn = 1;
				} else if (nr_enemy_action_phases_left_in_round != 0) {
					nr_action_phases_left_in_turn = nr_hero_action_phases_left_in_round / nr_enemy_action_phases_left_in_round;
				} else {
					nr_action_phases_left_in_turn = nr_hero_action_phases_left_in_round;
				}
			}

			if (is_enemies_turn == 1) {
				/* enemies' turn */

				if (nr_enemy_action_phases_left_in_round == 0) {
					is_enemies_turn = 0;
					nr_action_phases_left_in_turn = 1;
				} else if (nr_enemy_action_phases_left_in_round <= nr_hero_action_phases_left_in_round) {
					nr_action_phases_left_in_turn = 1;
				} else {
					nr_action_phases_left_in_turn = (nr_hero_action_phases_left_in_round ? nr_enemy_action_phases_left_in_round / nr_hero_action_phases_left_in_round : nr_enemy_action_phases_left_in_round);
				}
			}
		}

		if (is_enemies_turn == 0) {

			/* heroes on turn */
			actor_id = FIG_choose_next_hero();

			hero = get_hero(actor_id);

			dec_ptr_bs(hero + HERO_ACTIONS);

			if (hero_asleep(hero) && !hero_dead(hero)) {

				/* hero asleep and is not dead: 74% chance of waking up */

				if (random_schick(100) < 75) {

					/* awake him (or her) */

					and_ptr_bs(hero + HERO_FLAGS1, 0xfd); /* unset 'sleep' flag */

					fighter_ptr = FIG_get_fighter(host_readbs(hero + HERO_FIGHTER_ID));

					fighter_ptr->nvf_no = host_readbs(hero + HERO_VIEWDIR);
					fighter_ptr->reload = -1;
					fighter_ptr->offsetx = 0;
					fighter_ptr->offsety = 0;
				}
			}

			if (FIG_search_obj_on_cb(actor_id + 1, &x_coord, &y_coord) &&
				check_hero(hero))
			{

#if !defined(__BORLANDC__)
				/* BE-fix */
				x_coord = host_readws((Bit8u*)&x_coord);
				y_coord = host_readws((Bit8u*)&y_coord);
#endif

				if (host_readbs(hero + HERO_BLIND) != 0) {
					dec_ptr_bs(hero + HERO_BLIND);
				} else {
					if (host_readbs(hero + HERO_ECLIPTIFACTUS) != 0) {
						dec_ptr_bs(hero + HERO_ECLIPTIFACTUS);
					}

					/* save the fighter_id of this hero */
					g_fig_char_pic = actor_id + 1;

					/* select a fight action */
					FIG_menu(hero, actor_id, x_coord, y_coord);

					if ((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_MELEE_ATTACK) ||
						(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) ||
						(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_USE_ITEM) ||
						(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK))
					{

						FIG_do_hero_action(hero, actor_id);

						if (host_readbs(hero + HERO_ENEMY_ID) >= 10) {
							/* hero did attack some enemy (by weapon/spell/item etc.) */

							/* if the tail of a two-squares enemy has been attacked,
							 * replace HERO_ENEMY_ID by the main id of that enemy */
							if (host_readbs(hero + HERO_ENEMY_ID) >= 30) {
								sub_ptr_bs(hero + HERO_ENEMY_ID, 20);
							}

							/* TODO: seems that (hero->enemy_id) gives better results than (hero->enemy_id - 10) */
							if (g_enemy_sheets[host_readbs(hero + HERO_ENEMY_ID)].flags.dead)
							{
								/* attacked enemy is dead */
								if (is_in_byte_array(g_enemy_sheets[host_readbs(hero + HERO_ENEMY_ID) - 10].gfx_id, (Bit8u*)g_two_fielded_sprite_id))
								{
									/* attacked dead enemy is two-squares */
									/* goal: remove tail part */

									FIG_search_obj_on_cb(host_readbs(hero + HERO_ENEMY_ID) + 20, &x, &y);
									/* (x,y) are the coordinates of the tail of the enemy. redundant as fighter_ptr + FIGHTER_CBX, fighter_ptr + FIGHTER_CBY could have been used later. */

#if !defined(__BORLANDC__)
									/* BE-fix */
									x = host_readws((Bit8u*)&x);
									y = host_readws((Bit8u*)&y);
#endif


									fighter_ptr = FIG_get_fighter(g_enemy_sheets[host_readbs(hero + HERO_ENEMY_ID) - 10].fighter_id);
									/* intermediate: fighter_ptr points to the FIGHTER entry of the enemy */

									fighter_ptr = FIG_get_fighter(g_fig_twofielded_table[fighter_ptr->twofielded]);
									/* fighter_ptr now points the FIGHTER entry of the tail part of the enemy */
									/* should be true: (host_readbs(fighter_ptr + FIGHTER_CBX) == x) and (host_readbs(fighter_ptr + FIGHTER_CBY) == y) */

									/* Probably, the following if-then-else-condition is not necessary as the condition is always true. */
									if (fighter_ptr->obj_id >= 0) {
										/* if the id of a cb_entry has been saved in FIGHTER_OBJ_ID (meaning that the tail part is standing on it),
										 * restore that to the cb */
										/* TODO: passing of the 3rd parameter is different */
										FIG_set_cb_field(y, x, ((unsigned char)fighter_ptr->obj_id));
									} else {
										/* otherwise, set the square in the cb to 0 (free) */
										FIG_set_cb_field(fighter_ptr->cby, fighter_ptr->cbx, 0);
									}
								}
							}
						}

						herokeeping();
					}

					/* set fighter_id of the hero to 0 */
					g_fig_char_pic = 0;
				}
			}

			if (!FIG_fight_continues()) {
				/* this fight is over */
				g_in_fight = 0;
			}

			nr_hero_action_phases_left_in_round--;

		} else {
			/* enemies on turn */

			actor_id = FIG_choose_next_enemy();

			enemy = &g_enemy_sheets[actor_id];

			enemy->attacks_left--;

			if (FIG_search_obj_on_cb(actor_id + 10, &x_coord, &y_coord) && FIG_is_enemy_active(enemy))
			{
#if !defined(__BORLANDC__)
				/* BE-fix */
				x_coord = host_readws((Bit8u*)&x_coord);
				y_coord = host_readws((Bit8u*)&y_coord);
#endif
				if (enemy->blind) {
					enemy->blind--;
				} else {

					g_fig_enemy_pic = actor_id + 10;

					enemy->action_id = 1;

					enemy_turn(enemy, actor_id, x_coord, y_coord);

					if ((enemy->action_id == FIG_ACTION_MELEE_ATTACK) || (enemy->action_id == FIG_ACTION_SPELL) ||
						(enemy->action_id == FIG_ACTION_USE_ITEM) || (enemy->action_id == FIG_ACTION_RANGE_ATTACK))
					{

						FIG_do_enemy_action(enemy, actor_id);

						if (enemy->enemy_id >= 10) {
							/* enemy attacks another enemy (by weapon/spell etc.) */

							/* if the tail of a two-squares enemy has been attacked,
							 * replace enemy->enemy_id by the main id of that enemy */
							if (enemy->enemy_id >= 30) {
								enemy->enemy_id -= 20;
							}

							if (g_enemy_sheets[enemy->enemy_id - 10].flags.dead) /* check 'dead' flag */
							{
								/* attacked enemy is dead */
								if (is_in_byte_array(g_enemy_sheets[enemy->enemy_id - 10].gfx_id, (Bit8u*)g_two_fielded_sprite_id))
								{
									/* attacked dead enemy is two-squares */
									/* goal: remove tail part */
									FIG_search_obj_on_cb(enemy->enemy_id + 20, &x, &y);
									/* (x,y) are the coordinates of the tail of the enemy. redundant as fighter_ptr + FIGHTER_CBX, fighter_ptr + FIGHTER_CBY could have been used later. */

#if !defined(__BORLANDC__)
									/* BE-fix */
									x = host_readws((Bit8u*)&x);
									y = host_readws((Bit8u*)&y);
#endif

									fighter_ptr = FIG_get_fighter(g_enemy_sheets[enemy->enemy_id - 10].fighter_id);
									/* intermediate: fighter_ptr points to the FIGHTER entry of the killed enemy */

									fighter_ptr = FIG_get_fighter(g_fig_twofielded_table[fighter_ptr->twofielded]);
									/* fighter_ptr now points the FIGHTER entry of the tail part of the killed enemy */
									/* should be true: (host_readbs(fighter_ptr + FIGHTER_CBX) == x) and (host_readbs(fighter_ptr + FIGHTER_CBY) == y) */

									/* Probably, the following if-then-else-condition is not necessary as the condition is always true. */
									if (fighter_ptr->obj_id >= 0) {
										/* if the id of a cb_entry has been saved in FIGHTER_OBJ_ID (meaning that the tail part is standing on it),
										 * restore that to the cb */
										FIG_set_cb_field(y, x, fighter_ptr->obj_id);
									} else {
										/* otherwise, set the square in the cb to 0 (free) */
										FIG_set_cb_field(fighter_ptr->cby, fighter_ptr->cbx, 0);
									}
								}
							}
						}
#ifdef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 3:
						 * Tails of dead/disappeared two-squared enemies must be removed separately.
						 * Otherwise, those tails are an obstacle for movement (by another bug) or can be used
						 * as a target of the 'Skelettarius' spell, which may result in weired situations, see
						 * https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=166089#pid166089
						 *
						 * The tails are removed correctly (by some code above) if the enemies have been
						 * killed "regularly" by a hero (melee/ranged attack/spell/item) or by a (renegade) enemy.
						 * However, it has been forgotten for escaped enemies, as well as enemies which killed
						 * themselves by a critical melee attack failure. (Hopefully, this list is complete.)
						 *
						 * The following adds the missing code for self-killed enemies.
						 * The case of escaped enemies is dealt with in seg005.cpp
						 * It cannot be treated here as the FIGHTER entry of the tail is
						 * removed in seg005.cpp, which is needed to restore the object under the tail. */

						if (enemy->flags.dead) { /* check 'dead' flag */

							/* attacking enemy is dead because of critical attack failure */
							if (is_in_byte_array(enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {
								/* attacking dead enemy is two-squares */
								/* goal: remove tail part */

								fighter_ptr = FIG_get_fighter(enemy->fighter_id);
								/* intermediate: fighter_ptr points to the FIGHTER entry of the enemy */

								fighter_ptr = FIG_get_fighter(g_fig_twofielded_table[fighter_ptr->twofielded]);
								/* fighter_ptr now points the FIGHTER entry of the tail part of the enemy */
								/* should be true: (host_readbs(fighter_ptr + FIGHTER_CBX) == x) and (host_readbs(fighter_ptr + FIGHTER_CBY) == y) */

								/* restore the cb_entry stored at FIGHTER_OBJ_ID (meaning that the tail part is standing on it). */
								FIG_set_cb_field(fighter_ptr->cby, fighter_ptr->cbx, fighter_ptr->obj_id);
							}
						}
#endif

						herokeeping();
					}

					g_fig_enemy_pic = 0;
				}
			}

			if (!FIG_fight_continues()) {
				g_in_fight = 0;
			}

			nr_enemy_action_phases_left_in_round--;
		}

		nr_action_phases_left_in_turn--;

		if (g_fig_cb_marker_id != -1) {

			FIG_remove_from_list(g_fig_cb_marker_id, 0);
			g_fig_cb_marker_id = -1;
		}

		if (g_fig_cb_selector_id[0] != -1) {

			FIG_remove_from_list(g_fig_cb_selector_id[0], 0);
			g_fig_cb_selector_id[0] = -1;
		}

	}

#if !defined(__BORLANDC__)
	D1_INFO("Kampfrunde %d endet\n", g_fight_round);
#endif
}


/**
 * \brief   loads some special textures for the ghost ship
 */
void FIG_load_ship_sprites(void)
{
	signed short l_si;
	signed short l_di;
	signed short i;
	signed short width;
	signed short height;
	signed short const1 = 10;
	signed short const2 = 118;
	signed short l3;
	signed short l4;
	Bit8u *ptr;
	struct nvf_desc nvf;

	for (i = 0; i < 24; i++) {

		for (l_di = 0; l_di < 24; l_di++) {

			l_si =*(g_scenario_buf + 0x15 + 25 * i + l_di);

			if ((l_si >= 108) && (l_si <= 111)) {

				l_si -= 50;

			if (g_figobj_gfxbuf_table[l_si]) {

				/* this sprite has already been buffered */

				ptr = g_figobj_gfxbuf_table[l_si];

			} else {
				/* this sprite has not been used yet */

				ptr = g_fightobj_buf_seek_ptr;

				nvf.dst = ptr;
				nvf.src = g_fightobj_buf;
				nvf.no = l_si;
				nvf.type = 0;
				nvf.width = (Bit8u*)&width;
				nvf.height = (Bit8u*)&height;

				process_nvf(&nvf);
#if !defined(__BORLANDC__)
				/* BE-fix */
				width = host_readws((Bit8u*)&width);
				height = host_readws((Bit8u*)&height);
#endif

				/* buffer this picture */
				g_figobj_gfxbuf_table[l_si] = g_fightobj_buf_seek_ptr;
				g_figobj_gfxwidth_table[l_si] = width;
				g_figobj_gfxheight_table[l_si] = height;

				/* adjust the pointer */
				g_fightobj_buf_seek_ptr +=  width * height + 8;

				/* adjust the counter */
				g_fightobj_buf_freespace -= width * height + 8L;

				/* check for memory allocation error */
				if (g_fightobj_buf_freespace < 0L) {
					GUI_input((char*)g_str_error_on_obj_malloc, 0);
				}
			}


			/* calculate screen coordinates */
			l3 = const1 - g_figobj_gfxwidth_table[l_si] / 2 + 10 * (l_di + i);
			l4 = const2 - g_figobj_gfxheight_table[l_si] + 5 * (l_di - i);

			l3 += g_gfxtab_obj_offset_x[l_si];
			l4 += g_gfxtab_obj_offset_y[l_si];

			/* set screen coordinates */
			g_pic_copy.x1 = l3;
			g_pic_copy.y1 = l4;
			g_pic_copy.x2 = l3 + g_figobj_gfxwidth_table[l_si] - 1;
			g_pic_copy.y2 = l4 + g_figobj_gfxheight_table[l_si] - 1;
			g_pic_copy.src = g_figobj_gfxbuf_table[l_si];
			g_pic_copy.dst = g_buffer8_ptr;

			do_pic_copy(2);

			g_pic_copy.dst = g_vga_memstart;

			}
		}
	}
}

/**
 * \brief   the heroes encounter a fight
 *
 * \param   fight_id    id of the fight
 * \return              0 = heroes are in Hygellik's ruin and cursed -> no fight (?); 1 = no enemies -> no fight; 2 = ?; 3 = sneaked around -> no fight;
 */
signed short do_fight(signed short fight_id)
{
	signed short i;

	signed short fd;
	signed short j;
	signed short new_escape_position_found;
	signed short group_nr;
	signed short group_size;
	signed short retval = 0;
	Bit8u *hero;
	Bit8u *ptr;
	signed short nr_escape_positions;
	signed short x_target_bak;
	signed short y_target_bak;
	signed short dungeon_level_bak;
	signed short direction_bak;
	signed short tw_bak;
	signed short escape_positions[6];

	if ((gs_group_member_counts[gs_current_group] == 1) && (host_readbs(get_hero(0) + HERO_INVISIBLE) != 0))
	{
		/* group consists of a single hero with an active Visibili spell */
		/* TODO: potential Original-Bug: what about groups with >= 2 heroes where all have an active Visibili? */
		return 3;
	}

	if (gs_dng15_took_cursed_money) {
		return 0;
	}

	if (!count_fight_enemies(fight_id)) {
		return 1;
	}

	g_timers_disabled = 1;

	g_current_fight_no = fight_id;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;

	/* set some pointers */
	g_scenario_buf = (signed char*)(((HugePt)g_buffer8_ptr) + 64100L);
	g_monster_dat_buf = (struct enemy_sheet*)(((HugePt)g_scenario_buf) + 621L);
	g_current_fight = (struct fight*)(((HugePt)g_monster_dat_buf) + 3476L);

	read_fight_lst(fight_id);

	load_scenario(g_current_fight->scenario_id);

	if (!g_current_fight->intro_seen) {

		GUI_print_fight_intro_msg(fight_id);

		g_current_fight->intro_seen = 1;
	}

	if (g_max_enemies > 0) {

		/* reduce number of enemies to MAX_ENEMIES */
		memset(&g_current_fight->monsters[g_max_enemies], 0, sizeof(struct fight_monster) * (20 - g_max_enemies));
		g_max_enemies = 0;
	}

	/* state that we are in a fight */
	g_in_fight = 1;

	/* set some vars to 0 */
	g_autofight = g_fight_round = g_fig_all_heroes_withdrawn = 0;
	/* set some vars to -1 */
	g_fig_figure1 = g_fig_figure2 = -1;
	g_fight_figs_index = -1;

	g_request_refresh = 1;

	g_action_table_primary = &g_action_table_menu[0];

	g_wallclock_update = 0;

	/* open MONSTER.DAT */
	fd = load_archive_file(ARCHIVE_FILE_MONSTER_DAT);
	read_archive_file(fd, (Bit8u*)g_monster_dat_buf, 3476);
	close(fd);

	/* clear all dropped weapons */
	g_fig_dropped_counter = 0;
	for (i = 0; i < 30; i++) {
		g_fig_dropped_weapons[i] = 0;
	}

	load_tx(ARCHIVE_FILE_FIGHTTXT_LTX);

	/* open OBJECTS.NVF */
	fd = load_archive_file(ARCHIVE_FILE_OBJECTS_NVF);
	read_archive_file(fd, g_objects_nvf_buf, 3000);
	close(fd);

	FIG_chessboard_init();

	FIG_preload_gfx();

	/* open FIGHTOBJ.NVF */
	fd = load_archive_file(ARCHIVE_FILE_FIGHTOBJ_NVF);
	read_archive_file(fd, g_fightobj_buf, 16919);
	close(fd);

	set_var_to_zero();
	update_mouse_cursor();

	if (g_scenario_buf[0x14] > 3) {

		load_fightbg(g_scenario_buf[0x14] + 197);

	} else {

		load_fightbg(g_scenario_buf[0x14] + 1);

	}

	FIG_load_ship_sprites();

	FIG_draw_scenario();

	/* open WEAPONS.NVF */
	fd = load_archive_file(ARCHIVE_FILE_WEAPONS_NVF);
	read_archive_file(fd, g_weapons_nvf_buf, 6483);
	close(fd);

	/* open SPELLOBJ.NVF */
	fd = load_archive_file(ARCHIVE_FILE_SPELLOBJ_NVF);
	read_archive_file(fd, g_spellobj_nvf_buf, 3935);
	close(fd);

	FIG_init_enemies();
	FIG_init_heroes();

	set_audio_track(ARCHIVE_FILE_COMBAT_XMI);

	/* the fight happens in this loop */
	while (g_in_fight) {

		if (g_request_refresh != 0) {
			draw_fight_screen_pal(0);
			g_request_refresh = 0;
		}

		/* TODO: isnt that bogus? */
		if (g_in_fight) {

			/* fight a round */
			FIG_do_round();
			/* increment round counter */
			g_fight_round++;
			timewarp(SECONDS(6));

			if (g_in_fight) {
				FIG_latecomers();
			}

			if ((fight_id == 138) && (g_fight_round >= 10)) {
				/* This fight ends after 9 rounds */
				g_in_fight = 0;
			}
		}
	}

	refresh_screen_size();

	if (g_have_mouse == 2) {

		while (g_mouse_refresh_flag < 0) {
			refresh_screen_size();
		}
	}

	/* aftermath */

	if (g_game_state != GAME_STATE_FIGQUIT) {

		hero = get_hero(0);
		for (i = 0; i <=6; i++, hero += SIZEOF_HERO) {

			if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE)
				&& (host_readbs(hero + HERO_GROUP_NO) == gs_current_group))
			{

				and_ptr_bs(hero + HERO_FLAGS1, 0x7f); /* unset 'unconscious' flag */
				and_ptr_bs(hero + HERO_FLAGS1, 0xfd); /* unset 'sleeps' flag */
				and_ptr_bs(hero + HERO_FLAGS1, 0xef); /* unset 'chamaelioni' flag */
				and_ptr_bs(hero + HERO_FLAGS2, 0xfb); /* unset 'duplicatus' flag */
				and_ptr_bs(hero + HERO_FLAGS2, 0xfe); /* unset 'scared' flag */

				host_writebs(hero + HERO_BLIND, 0); /* unset blindness (set counter to 0) */
				host_writebs(hero + HERO_ECLIPTIFACTUS, 0); /* unset 'Ecliptifactus' (set counter to 0) */
				host_writebs(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);
			}
		}

		if (g_game_state != GAME_STATE_MAIN) {
			/* GAME_STATE is neither GAME_STATE_FIGQUIT nor GAME_STATE_MAIN. Does that mean that the fight is lost?? */

			if ((fight_id != 192) && count_heroes_available()) {

				g_game_state = (GAME_STATE_MAIN);

				if (g_traveling) {

					gs_travel_detour = (99);
					ptr = get_hero(0);
					for (j = 0; j <=6; j++, ptr += SIZEOF_HERO) {

						if ((host_readbs(ptr + HERO_TYPE) != HERO_TYPE_NONE)
							&& (host_readbs(ptr + HERO_GROUP_NO) == gs_current_group))
						{
							hero_disappear(ptr, j, -2);
						}
					}
				}

				GRP_switch_to_next(2);

				if (gs_current_town != TOWNS_NONE) {
					g_fading_state = 3;
				}
			}

			retval = 2;
		}

		if (g_fig_all_heroes_withdrawn) {
			retval = 1;
		}

		if (retval == 0) {
			/* the heroes won the fight => loot */

			/* give automatically dropped items to the heroes */
			i = 0;
			while (g_fig_dropped_weapons[i]) {
				get_item(g_fig_dropped_weapons[i++], 0, 1);
			}

			FIG_loot_monsters();

			FIG_split_ap();

			if ((g_max_enemies != 0) && !g_fig_discard) {

				for (i = 0; i < 20; i++) {
					g_enemy_sheets[i].flags.dead = 1;
				}
			}
		}

		if ((retval != 2) && !g_fig_discard) {

			FIG_tidy_monsters();
			write_fight_lst();
		}

		if ((retval == 1) && (gs_dungeon_index != DUNGEONS_NONE)) {
			/* heroes escaped and fight was in a dungeon => distribute escaped heroes and split group */

			nr_escape_positions = 0;

			for (i = 0; gs_group_member_counts[gs_current_group] > i; i++) {

				hero = get_hero(i);

				if (host_readws(hero + HERO_ESCAPE_POSITION) != 0) {

					new_escape_position_found = 0;

					for (j = 0; j < nr_escape_positions; j++) {
						if (escape_positions[j] == host_readws(hero + HERO_ESCAPE_POSITION)) {
							new_escape_position_found = 1;
						}
					}

					if (new_escape_position_found == 0) {
						escape_positions[nr_escape_positions++] = host_readws(hero + HERO_ESCAPE_POSITION);
					}
				}
			}

			if (nr_escape_positions > 0) {

				for (i = 0; nr_escape_positions - 1 > i; i++) {

					group_nr = 0;
					while (gs_group_member_counts[group_nr] != 0) {
						group_nr++;
					}

					group_size = gs_group_member_counts[gs_current_group];
					x_target_bak = gs_x_target;
					y_target_bak = gs_y_target;
					direction_bak = gs_direction;
					dungeon_level_bak = gs_dungeon_level;

					gs_x_target = ((escape_positions[i] >> 8) & 0x0f); /* bits 8..11 */
					gs_y_target = (escape_positions[i] & 0x0f); /* bits 0..3 */
					gs_direction = ((escape_positions[i] & 0xf0) >> 4); /* bits 4..7 */
					gs_dungeon_level = (escape_positions[i] >> 12); /* bits 12..15 */

					for (j = 0; j < group_size; j++) {

						hero = get_hero(j);

						if (escape_positions[i] == host_readws(hero + HERO_ESCAPE_POSITION)) {

							host_writeb(hero + HERO_GROUP_NO, (signed char)group_nr);
							host_writew(hero + HERO_ESCAPE_POSITION, 0);
							gs_group_member_counts[group_nr]++;
							gs_group_member_counts[gs_current_group]--;
						}
					}

					GRP_save_pos(group_nr | 0x8000);
					gs_x_target = x_target_bak;
					gs_y_target = y_target_bak;
					gs_direction = (signed char)direction_bak;
					gs_dungeon_level = dungeon_level_bak;
				}

				group_size = gs_group_member_counts[gs_current_group];

				for (j = 0; j < group_size; j++) {
					host_writews(get_hero(j) + HERO_ESCAPE_POSITION, 0);
				}

				gs_x_target = ((escape_positions[i] >> 8) & 0x0f);
				gs_y_target = (escape_positions[i] & 0x0f);
				gs_direction = ((escape_positions[i] & 0xf0) >> 4);

				gs_dungeon_level_bak = gs_dungeon_level;
				gs_dungeon_level = (escape_positions[i] >> 12);

				if (gs_dungeon_level != gs_dungeon_level_bak) {
					load_area_description(1);
				}
			}
		}

	} else {
		/* GAME_STATE == GAME_STATE_FIGQUIT */
		gs_travel_detour = (99);
		retval = 4;
	}

	g_fig_initiative = g_always_zero4 = 0;
	g_fig_discard = 0;
	g_max_enemies = 0;
	g_in_fight = 0;
	g_request_refresh = 1;
	g_current_ani = -1;
	g_area_prepared = -1;
	g_timers_disabled = 0;
	g_autofight = 0;
	g_check_party = 1;
	g_textbox_width = tw_bak;
	g_pp20_index = (ARCHIVE_FILE_DNGS + 12);

	update_mouse_cursor();

	/* clear the screen */
	/* TODO: add update */
	memset((void*)(g_vga_memstart), 0, 320 * 200);

	refresh_colors();

	g_gui_buffer_unkn = g_renderbuf_ptr;

	if (!gs_show_travel_map) {
		seg028_0555(gs_dungeon_index != DUNGEONS_NONE ? 0 : 1);
	}

	load_wallclock_nvf();
	refresh_screen_size();

	if ((gs_current_town != TOWNS_NONE) && !gs_show_travel_map) {
		g_fading_state = 3;
	}


	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
