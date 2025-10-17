/**
 *	Rewrite of DSA1 v3.02_de functions of seg036 (Fight Hero KI)
 *	Functions rewritten: 10/10 (complete)
 *	Borlandified and identical
 *
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg036.cpp
 */
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg001.h"
#include "seg002.h"
#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg036.h"
#include "seg038.h"
#include "seg039.h"
#include "seg041.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   copies an ani sequence
 *
 * \param   dst         destination pointer for data
 * \param   ani_no      * \param mode	DAT-File to use 3 = WEAPANI.DAT, else ANI.DAT
 * \return              the length of the sequence in bytes.
 */
#if defined(__BORLANDC__)
static
#endif
signed short KI_copy_ani_sequence(Bit8u *dst, signed short ani_no, signed short mode)
{
	Bit8u *p_datbuffer;
	Bit8u *p_datitem;
	signed char len;

	signed short i;
	signed short ani_max_no;


	/* set the right buffer */
	p_datbuffer = g_buffer_anidat;

	/* This function is never calld with mode == 3 */
	if (mode == 3)
		p_datbuffer = g_buffer_weapanidat;

	/* read how many ani sequences are in the file */
	ani_max_no = *(Bit16s*)p_datbuffer;

	/* check if the desired ani_no is in the range */
	if (ani_no < 0)
		return 0;

	if (ani_no > ani_max_no)
		return 0;

	/* set p_datitem to the first (0) ani sequence */
	p_datitem = p_datbuffer;
	p_datitem += ani_max_no + 2;
	/* set len to the lenght first (0) ani sequence */
	len = p_datbuffer[2];

	/* forward to the desired ani sequence */
	for (i = 1; i <= ani_no; i++) {
		p_datitem += len;
		len = *(p_datbuffer + i + 2);
	}

	p_datitem++;

	len = len - 2;

	/* copy the ani sequenecs to dst */
	for (i = 0; len > i; i++) {
		*dst = *p_datitem;
		p_datitem++;
		dst++;
	}

	return len;
}

void seg036_00ae(struct struct_hero *hero, const signed int hero_pos)
{
	signed short i;
	signed char dir1;
	signed char dir2;
	Bit8s *ptr1;
	signed char dir3;
	Bit16s *ptr2;

	g_fig_anisheets[0][0] = 0;
	g_fig_anisheets[0][242] = hero->sprite_no;

	ptr1 = (Bit8s*)&g_fig_anisheets[0][1];
	ptr2 = g_gfx_ani_index[hero->sprite_no];

	i = 0;

	while (g_fig_move_pathdir[i] != -1) {

		if (hero->viewdir != g_fig_move_pathdir[i]) {

			dir2 = dir1 = -1;
			dir3 = hero->viewdir;
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

					dir2 = hero->viewdir + 4;
					dir1 = -1;
				}
			}

			/* set heroes looking direction */
			hero->viewdir = g_fig_move_pathdir[i];

			ptr1 += KI_copy_ani_sequence((Bit8u*)ptr1, ptr2[dir2], 2);

			if (dir1 != -1) {
				ptr1 += KI_copy_ani_sequence((Bit8u*)ptr1, ptr2[dir1], 2);
			}
		}

		if (g_fig_move_pathdir[i] == g_fig_move_pathdir[i + 1]) {

			ptr1 += KI_copy_ani_sequence((Bit8u*)ptr1, ptr2[(g_fig_move_pathdir[i] + 12)], 2);
			i += 2;
			/* BP - 2 */
			hero->bp_left = hero->bp_left - 2;
		} else {
			ptr1 += KI_copy_ani_sequence((Bit8u*)ptr1, ptr2[(g_fig_move_pathdir[i] + 8)], 2);
			i++;
			/* BP - 1 */
			hero->bp_left--;
		}
	}

	*ptr1 = -1;
	FIG_call_draw_pic();
	FIG_remove_from_list(g_fig_cb_marker_id, 0);
	g_fig_cb_marker_id = -1;
	FIG_set_sheet(hero->fighter_id, 0);
	draw_fight_screen(0);
	memset(&g_fig_anisheets[0], -1, 0xf3);
	FIG_init_list_elem(hero_pos + 1);
}

/**
 * \brief   changes the weapon of a hero
 *
 * \param   hero        pointer to the hero with a broken weapon
 * \return              1 if a weapon was found, 0 if the hero fights now with bare hands
 */
signed int KI_change_hero_weapon(struct struct_hero *hero)
{
	signed short pos;
	signed short has_new_weapon = 0;
	struct item_stats *item_p;
	signed short item_id;
	struct struct_fighter *fighter;

	for (pos = HERO_INVENTORY_SLOT_KNAPSACK_1; pos < NR_HERO_INVENTORY_SLOTS; pos++) {

		item_id = hero->inventory[pos].item_id;
		item_p = &g_itemsdat[item_id];

		/* grab the first melee weapon in the knapsack,
		 * and exchange it with the broken weapon. */
		if (item_p->flags.weapon &&
			(item_p->subtype != WEAPON_TYPE_SCHUSSWAFFE) &&
			(item_p->subtype != WEAPON_TYPE_WURFWAFFE) &&
			(item_p->subtype != WEAPON_TYPE_SPEER))
		{
			move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, pos, hero);
			has_new_weapon = 1;
			break;
		}
	}

	if (!has_new_weapon) {

		/* find a free slot, to get rid of the broken weapon */
		for (pos = HERO_INVENTORY_SLOT_KNAPSACK_1; pos < NR_HERO_INVENTORY_SLOTS; pos++) {

			if (hero->inventory[pos].item_id == ITEM_NONE) {

				move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, pos, hero);
				has_new_weapon = 2;
				break;
			}
		}

		/* if nothing helps, put it in the left hand */
		if (!has_new_weapon) {
			move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, HERO_INVENTORY_SLOT_LEFT_HAND, hero);
		}

		has_new_weapon = 0;
	}

	fighter = FIG_get_fighter(hero->fighter_id);
	fighter->nvf_no = hero->viewdir;
	fighter->reload = -1;

	draw_fight_screen_pal(0);

	hero->bp_left = hero->bp_left - 2;

	return has_new_weapon;
}

/**
 * \brief   check if a neighbour can be attacked
 *
 * \param   start_x     X-Coordinate of the hero
 * \param   start_y     Y-Coordinate of the hero
 * \param   offset_x    X-Direction -1/0/+1
 * \param   offset_y    Y-Direction -1/0/+1
 * \param   mode        0 = Attack only Enemies / 1 = Attack heroes and enemies / 2 = Attack only Heros
 * \return              1 if an attack is possible in that mode, else 0.
 */
signed short KI_can_attack_neighbour(signed short start_x, signed short start_y,
			signed short offset_x, signed short offset_y,
			signed short mode)
{
	signed char target = get_cb_val(start_x + offset_x, start_y + offset_y);

	if (mode == 1) {
		/* target is hero or enemy */
		if ( ( (target > 0) && (target < 10) &&	!get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious) ||
			((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead &&
				//g_enemy_sheets[target - 10].flags.renegade
				((struct enemy_flags*)(target * sizeof(struct enemy_sheet) + (Bit8u*)g_enemy_sheets - 10 * sizeof(struct enemy_sheet) + 0x31))->renegade
			))
		{
			return 1;
		} else {
			return 0;
		}

	} else if (!mode) {
		/* target is an enemy */
		if ((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead)
		{
			return 1;
		} else {
			return 0;
		}
	} else if (mode == 2) {
		/* target is a hero */
		if ((target > 0) && (target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious) {

			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}

/**
 * \brief   TODO
 *
 * \param   x           x-coordinate of the hero
 * \param   y           y-coordinate of the hero
 * \param   dir         looking direction of the hero
 * \param   renegade    0 = hero is normal, 1 = hero is renegade
 * \return              0 = no target found, fight-id of the target
 */
signed short KI_search_spell_target(signed short x, signed short y,
				signed short dir, signed short renegade)
{
	signed short x_diff;
	signed short y_diff;
	signed char obj_id;
	signed short done;
	signed short will_attack;

	done = 0;
	x_diff = 0;
	y_diff = 0;
	will_attack = 0;

	while (done == 0) {

		/* calculate the offset from direction */
		if (dir == 0) {
			x_diff++;
		} else if (dir == 1) {
			y_diff--;
		} else if (dir == 2) {
			x_diff--;
		} else {
			y_diff++;
		}

		/* check the field is on the chessboard */
		if ((y + y_diff < 0) || (y + y_diff > 23) ||
			(x + x_diff < 0) || (x + x_diff > 23))
		{
			done = 1;
			continue;
		}

		/* get the fight object ID of the object on that field */
		obj_id = get_cb_val(x + x_diff, y + y_diff);

		if (renegade == 1) {

			/* attack only heroes and renegade enemies */
			if ( ((obj_id > 0) && (obj_id < 10) && !(get_hero(obj_id - 1))->flags.dead && !(get_hero(obj_id - 1))->flags.unconscious) ||
				((obj_id >= 10) && (obj_id < 30) && !g_enemy_sheets[obj_id - 10].flags.dead &&
				// g_enemy_sheets[obj_id - 10].flags.renegade
				((struct enemy_flags*)(obj_id * sizeof(struct enemy_sheet) + (Bit8u*)g_enemy_sheets - 10 * sizeof(struct enemy_sheet) + 0x31))->renegade
				))
				{

					will_attack = 1;
					done = 1;

			} else if ( (obj_id != 0) && (((obj_id >= 10) && (obj_id < 30) && !g_enemy_sheets[obj_id - 10].flags.dead) || ((obj_id >= 50) && !is_in_word_array(obj_id - 50, g_cb_obj_nonobstacle))))
				{
					done = 1;
				}

		} else if (renegade == 0) {

			/* attack only enemies */
			if ((obj_id >= 10) && (obj_id < 30) && !g_enemy_sheets[obj_id - 10].flags.dead)
			{
				will_attack = 1;
				done = 1;
			} else if ((obj_id != 0) &&
					 (
#ifdef M302de_ORIGINAL_BUGFIX
						(obj_id > 0) &&
#endif
						 (((obj_id < 10) && !(get_hero(obj_id - 1))->flags.dead &&	!(get_hero(obj_id - 1))->flags.unconscious
						) || (
							(obj_id >= 50) &&
							!is_in_word_array(obj_id - 50, g_cb_obj_nonobstacle)
						))
					)
				)
				{
					done = 1;
				}
		}
	}

	if (will_attack == 0) {
		return 0;
	} else {
		return obj_id;
	}
}

/**
 * \brief   selects a target to be attacked from hero with a spell
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero in the party
 * \param   renegade    0 = hero normal, 1 = hero renegade
 * \param   x           x-coordinate of the hero
 * \param   y           y-coordinate of the hero
 * \return              0 = no target found, 1 = target found (long distance), 2 = target found (short distance)
 */
signed int KI_select_spell_target(struct struct_hero *hero, const signed int hero_pos, const signed int renegade, signed short x, signed short y)
{
	signed short dir;
	signed short count;
	signed short done;
	signed short retval;
	signed short l5;
	signed short target_x;
	signed short target_y;

	retval = 0;
	done = 0;

	while ((done == 0) && (hero->bp_left > 0)) {

		/* reset target fight-id */
		hero->enemy_id = 0;

		if (hero->bp_left >= 3) {

			dir = hero->viewdir;

			count = 0;

			/* try to find a target clockwise from current direction */
			while (!hero->enemy_id && (count < 4)) {

				hero->enemy_id = (signed char)KI_search_spell_target(x, y, dir, renegade);

				count++;

				if (++dir == 4) {
					dir = 0;
				}
			}
		}

		/* check if a target was found */
		if (hero->enemy_id != 0) {

			/* yes */

			FIG_search_obj_on_cb(hero->enemy_id, &target_x, &target_y);

			if (calc_beeline(target_x, target_y, x, y) < 2) {
				retval = 2;
			} else {
				retval = 1;
			}

			done = 1;

		} else {
			/* try a more expensive search */

			if (!hero->flags.tied) {

				if (renegade == 0) {
					l5 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 9);
				} else {
					l5 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 8);
				}

				if (l5 != -1) {
					seg036_00ae(hero, hero_pos);

					FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

					if (hero->bp_left < 3) {

						/* set BP to 0 */
						hero->bp_left = 0;
					}
				} else {
					/* set BP to 0 */
					hero->bp_left = 0;
				}
			} else {
				/* set BP to 0 */
				hero->bp_left = 0;
			}
		}
	}

	return retval;
}

/**
 * \brief   TODO
 *
 * \param   spell       spell index
 * \param   renegade    0 = hero normal, 1 = hero renegade
 * \return              TODO	{-1, 0, 1, 2}
 */
signed short KI_get_spell(signed short spell, signed short renegade)
{
	struct spell_descr *spell_description;
	signed short retval = -1;

	/* make a pointer to the spell description */
	spell_description = &g_spell_descriptions[spell];

	if (renegade == 0) {
		if (spell_description->target_type == 2)
			retval = 1;
		else if ((spell_description->target_type == 1) || (spell_description->target_type == 3))
			retval = 0;
		else
			retval = 2;
	} else {
		if (spell_description->target_type == 3)
			retval = 1;
		else if (spell_description->target_type == 0)
			retval = 2;
	}

	return retval;
}

/**
 * \brief   TODO
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero in the party
 * \param   renegade    0 = hero normal, 1 = hero renegade
 * \param   x           x-coordinate of the hero
 * \param   y           y-coordinate of the hero
 * \return              {0, 1}
 */
signed int seg036_8cf(struct struct_hero *hero, const signed int hero_pos, const signed int renegade, signed short x, signed short y)
{
	signed short l_si;
	signed short count;
	signed short spell;
	signed short done;
	signed short retval;
	signed short spell_mode;
	signed short l5;
	signed short decided;

	struct viewdir_offsets a = g_viewdir_offsets3;

	retval = 0;
	done = 0;

	while ((done == 0) && (hero->bp_left > 0)) {

		decided = 0;

		for (l_si = 0; l_si <= 10; l_si++) {

			/* get a spell from an array */
			spell = g_af_spell_list[l_si];

			if ((g_spell_descriptions[spell].range == 1) && (random_schick(100) < 50))
			{
				decided = 1;
				break;

			} else if (random_schick(100) < 25) {
				decided = 1;
				break;
			}
		}

		if (decided == 0) {

			spell = g_af_spell_list[random_interval(0, 10)];
		}

		/* reset the target of the hero */
		hero->enemy_id = 0;

		if ((spell_mode = KI_get_spell(spell, renegade)) != -1) {

			if (spell_mode == 2) {

				/* set target to hero */
				hero->enemy_id = hero_pos + 1;

				/* set spell */
				hero->spell_id = (signed char)spell;

				retval = 1;
				done = 1;
			} else {

				if (!g_spell_descriptions[spell].range) {

					while ((hero->bp_left != 0) && (done == 0)) {

						l_si = hero->viewdir;

						count = 0;
						while ((!hero->enemy_id) && (count < 4)) {

							if (KI_can_attack_neighbour(x, y, a.a[l_si].x, a.a[l_si].y, spell_mode)) {
								hero->enemy_id = get_cb_val(x + a.a[l_si].x, y + a.a[l_si].y);
							}

							count++;

							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (hero->enemy_id != 0) {

							if (hero->bp_left >= 5) {

								/* enough BP */
								hero->spell_id = (signed char)spell;
								retval = 1;
							} else {
								/* set BP to 0 */
								hero->bp_left = 0;
							}

							done = 1;

						} else if (!hero->flags.tied) {

							if (spell_mode == 0) {
								l5 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 3);
							} else {
								l5 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 1);
							}

							if (l5 != -1) {

								seg036_00ae(hero, hero_pos);
								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

							} else {
								/* set BP to 0 */
								hero->bp_left = 0;
							}

						} else {
							/* set BP to 0 */
							hero->bp_left = 0;
						}
					}
				} else {
					while ((done == 0) && (hero->bp_left > 0)) {

						l_si = hero->viewdir;

						count = 0;

						while (!hero->enemy_id && (count < 4)) {

							hero->enemy_id = (signed char)KI_search_spell_target(x, y, l_si, spell_mode);

							count++;

							if (++l_si == 4) {
								l_si = 0;
							}
						}

						if (hero->enemy_id != 0) {

							if (hero->bp_left >= 5) {

								/* enough BP */
								hero->spell_id = (signed char)spell;
								retval = 1;
							} else {
								/* set BP to 0 */
								hero->bp_left = 0;
							}

							done = 1;

						} else if (!hero->flags.tied) {

							if (spell_mode == 0) {
								l5 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 9);
							} else {
								l5 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 8);
							}

							if (l5 != -1) {

								seg036_00ae(hero, hero_pos);
								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

							} else {
								/* set BP to 0 */
								hero->bp_left = 0;
							}

						} else {
							/* set BP to 0 */
							hero->bp_left = 0;
						}
					}
				}
			}
		}
	}

	return retval;
}

/**
 * \brief   count the other heroes in the current group
 *
 * \param   hero_pos    position of the calling hero
 * \return              number of heroes in the group - 1
 */
signed short KI_count_heroes(signed short hero_pos)
{
	signed short cnt = 0;
	signed short i;

	/* for each hero in this group */
	for (i = 0; gs_group_member_counts[gs_current_group] > i; i++) {

		if ((i != hero_pos) && check_hero(get_hero(i))) {
			cnt++;
		}
	}

	return cnt;
}

/**
 * \brief   TODO
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero in the party
 * \param   x           x-coordinate of the hero
 * \param   y           y-coordinate of the hero
 */
void KI_hero(struct struct_hero *hero, const signed int hero_pos, signed short x, signed short y)
{
	signed short l_di;
	signed short l1;
	signed short done;
	signed short l4;
	signed short l5;
	signed short x_bak;
	signed short y_bak;
	signed short l8;
	signed short hero_x;
	signed short hero_y;

	struct viewdir_offsets a = g_viewdir_offsets4;

	done = 0;
	l5 = 1;

	hero->action_id = FIG_ACTION_MOVE;

	if (hero->npc_id > 0) {

		if (hero->npc_id == NPC_NARIELL) {

			/* equip LONGBOW and ARROWS in the first round,
			 * if the hero has them in the inventory */
			if ((g_fight_round == 0) &&
				(hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_LONGBOW) &&
				(get_item_pos(hero, ITEM_ARROWS) != -1) &&
				(get_item_pos(hero, ITEM_LONGBOW) != -1))
			{
				move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, get_item_pos(hero, ITEM_LONGBOW), hero);

				if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id != ITEM_ARROWS) {
					move_item(HERO_INVENTORY_SLOT_LEFT_HAND, get_item_pos(hero, ITEM_ARROWS), hero);
				}
			}

		} else if (hero->npc_id == NPC_HARIKA) {

			if (hero->le <= 12) {

				/* equip LONGBOW and ARROWS in the first round,
				 * if the hero has them in the inventory */
				if ((g_fight_round == 0) &&
					(hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_LONGBOW) &&
					(get_item_pos(hero, ITEM_ARROWS) != -1) &&
					(get_item_pos(hero, ITEM_LONGBOW) != -1))
				{
					move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, get_item_pos(hero, ITEM_LONGBOW), hero);

					if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id != ITEM_ARROWS) {
						move_item(HERO_INVENTORY_SLOT_LEFT_HAND, get_item_pos(hero, ITEM_ARROWS), hero);
					}

				} else if (FIG_get_range_weapon_type(hero) == -1) {
					hero->action_id = FIG_ACTION_FLEE;
				}
			}

		} else if (hero->npc_id == NPC_CURIAN) {

			if ((hero->le < 10) && (hero->ae < 10))
			{
				hero->action_id = FIG_ACTION_FLEE;
			}

		} else if (hero->npc_id == NPC_ARDORA) {

			if (hero->le < 8)
			{
				hero->action_id = FIG_ACTION_FLEE;
			}

		} else if (hero->npc_id == NPC_GARSVIK) {

			if (!KI_count_heroes(hero_pos)) {
				hero->action_id = FIG_ACTION_FLEE;
			}

		} else if (hero->npc_id == NPC_ERWO) {

			if (hero->le < 15)
			{
				hero->action_id = FIG_ACTION_FLEE;
			}

		}

		if (FIG_get_first_active_hero() == 6) {

			hero->action_id = FIG_ACTION_FLEE;
		}

		if (hero->action_id == FIG_ACTION_FLEE) {

			FIG_search_obj_on_cb(hero_pos + 1, &hero_x, &hero_y);

			if ((hero_x - 1 >= 0) && (hero_x + 1 <= 25) &&
				(hero_y -1 >= 0) && (hero_y + 1 <= 25) &&
				(get_cb_val(hero_x, hero_y + 1) != 0) &&
				(get_cb_val(hero_x, hero_y - 1) != 0) &&
				(get_cb_val(hero_x + 1, hero_y) != 0) &&
				(get_cb_val(hero_x - 1, hero_y) != 0))
			{
				hero->action_id = FIG_ACTION_MOVE;
			}
		}
	}

	if (hero->bp_left == 1) {
		/* set BP to 0 */
		hero->bp_left = 0;
	}

	while ((done == 0) && (hero->bp_left > 0)) {

		seg001_02c4();

		if ((hero->action_id == FIG_ACTION_FLEE) && (hero->bp_left > 0)) {

			if (!hero->flags.tied) {

				l4 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 5);

				if (l4 != -1) {

					x_bak = x;
					y_bak = y;

					seg036_00ae(hero, hero_pos);

					hero->enemy_id = 0;

					if (FIG_search_obj_on_cb(hero_pos + 1, &x, &y)) {

						hero->action_id = FIG_ACTION_MOVE;

						if ((x_bak == x) && (y_bak == y)) {

							hero->bp_left = 0;
						}
					} else {
						hero->bp_left = 0;
					}

					if (hero->bp_left < 3) {
						hero->bp_left = 0;
					}

				} else {
					hero->bp_left = 0;
				}
			} else {
				hero->bp_left = 0;
			}

		} else {

			if ((hero->typus >= HERO_TYPE_WITCH) &&		/* magic user */
				(hero->ae > 10) &&	/* AE > 10 */
				(l5 != 0) &&
				(g_current_fight_no != FIGHTS_F144) &&	/* not in the final fight */
				g_autofight_magic) /* magic activated in auto fight */
			{
				if (seg036_8cf(hero, hero_pos, hero->flags.renegade, x, y))
				{
					hero->action_id = FIG_ACTION_SPELL;
					hero->bp_left = 0;

				} else {
					l5 = 0;
				}
			}

			if ((hero->action_id == FIG_ACTION_MOVE) && (hero->bp_left > 0)) {

				if (FIG_get_range_weapon_type(hero) != -1) {

					if (range_attack_check_ammo(hero, 2)) {

						l8 = KI_select_spell_target(hero, hero_pos, hero->flags.renegade, x, y);

						if (l8 != 0) {
							if (l8 == 2) {
								if (!KI_change_hero_weapon(hero)) {
									done = 1;
								}
							} else {
								hero->action_id = FIG_ACTION_RANGE_ATTACK;
							}
						}

						/* set BP to 0 */
						hero->bp_left = 0;
					} else {
						if (!KI_change_hero_weapon(hero)) {
							done = 1;
						}
					}
				} else {
					hero->enemy_id = 0;

					if (hero->bp_left >= 3) {

						l_di = hero->viewdir;
						l1 = 0;

						while (!hero->enemy_id && (l1 < 4)) {

							if (KI_can_attack_neighbour(x, y, a.a[l_di].x, a.a[l_di].y, hero->flags.renegade))
							{
								hero->enemy_id = get_cb_val(x + a.a[l_di].x, y + a.a[l_di].y);
							}

							l1++;
							if (++l_di == 4) {
								l_di = 0;
							}
						}
					}

					if (hero->enemy_id != 0) {

						hero->action_id = FIG_ACTION_MELEE_ATTACK;
						hero->bp_left = 0;

					} else {

						if (!hero->flags.tied) {

							if (!hero->flags.renegade) {
								l4 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 3);
							} else {
								l4 = FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 1);
							}

							if (l4 != -1) {
								x_bak = x;
								y_bak = y;

								seg036_00ae(hero, hero_pos);

								hero->action_id = FIG_ACTION_MOVE;
								hero->enemy_id = 0;

								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

								if ((x_bak == x) && (y_bak == y)) {
									hero->bp_left = 0;
								}

								if (hero->bp_left < 3) {
									hero->bp_left = 0;
								}
							} else {
								hero->bp_left = 0;
							}
						} else {
							hero->bp_left = 0;
						}
					}
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
