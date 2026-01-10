/**
 *	Rewrite of DSA1 v3.02_de functions of seg036 (Auto FIGht Hero)
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

static const signed char g_af_spell_list[11] = {
	SP_FULMINICTUS_DONNERKEIL,
	SP_HORRIPHOBUS,
	SP_IGNIFAXIUS_FLAMMENSTRAHL,
	SP_BALSAM_SALABUNDE,
	SP_BOESER_BLICK,
	SP_AXXELERATUS_BLITZGESCHWIND,
	SP_DUPLICATUS_DOPPELPEIN,
	SP_BLITZ_DICH_FIND,
	SP_PLUMBUMBARUM_UND_NARRETEI,
	SP_SAFT_KRAFT_MONSTERMACHT,
	SP_PARALUE_PARALEIN
}; // ds:0x5fac

static struct viewdir_offsets g_fig_viewdir_offsets3 = { { { 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 } } }; // ds:0x5fb7
static struct viewdir_offsets g_fig_viewdir_offsets4 = { { { 1, 0 }, { 0, -1 }, { -1, 0 }, { 0, 1 } } }; // ds:0x5fc7

/**
 * \brief   copies an ani sequence
 *
 * \param dst         destination pointer for data
 * \param ani_num
 * \param mode	DAT-File to use 3 = WEAPANI.DAT, else ANI.DAT
 * \return   the length of the sequence in bytes.
 */
static signed int AFIG_copy_ani_sequence(int8_t *dst, const signed int ani_num, const signed int mode)
{
	uint8_t *p_datbuffer;
	uint8_t *p_datitem;
	signed char len;
	signed int i;
	signed int ani_max_num;

	/* set the right buffer */
	p_datbuffer = g_buffer_anidat;

	/* This function is never called with mode == 3 */
	if (mode == 3)
		p_datbuffer = g_buffer_weapanidat;

	/* read how many ani sequences are in the file */
	ani_max_num = *(int16_t*)p_datbuffer;

	/* check if the desired ani_no is in the range */
	if (ani_num < 0)
		return 0;

	if (ani_num > ani_max_num)
		return 0;

	/* set p_datitem to the first (0) ani sequence */
	p_datitem = p_datbuffer;
	p_datitem += ani_max_num + 2;
	/* set len to the lenght first (0) ani sequence */
	len = p_datbuffer[2];

	/* forward to the desired ani sequence */
	for (i = 1; i <= ani_num; i++) {
		p_datitem += len;
		len = *(p_datbuffer + i + 2);
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

void FIG_prepare_hero_ani(struct struct_hero *hero, const signed int hero_pos)
{
	signed int i;
	signed char dir1;
	signed char dir2;
	int8_t *sheet_ptr;
	signed char dir3;
	int16_t *ani_index_ptr;

	g_fig_anisheets[0][0] = 0;
	g_fig_anisheets[0][242] = hero->actor_sprite_id;

	sheet_ptr = &g_fig_anisheets[0][1];
	ani_index_ptr = g_gfx_ani_index[hero->actor_sprite_id];

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

			sheet_ptr += AFIG_copy_ani_sequence(sheet_ptr, ani_index_ptr[dir2], 2);

			if (dir1 != -1) {
				sheet_ptr += AFIG_copy_ani_sequence(sheet_ptr, ani_index_ptr[dir1], 2);
			}
		}

		if (g_fig_move_pathdir[i] == g_fig_move_pathdir[i + 1]) {

			sheet_ptr += AFIG_copy_ani_sequence(sheet_ptr, ani_index_ptr[(g_fig_move_pathdir[i] + 12)], 2);
			i += 2;
			/* BP - 2 */
			hero->fight_bp_left = hero->fight_bp_left - 2;
		} else {
			sheet_ptr += AFIG_copy_ani_sequence(sheet_ptr, ani_index_ptr[(g_fig_move_pathdir[i] + 8)], 2);
			i++;
			/* BP - 1 */
			hero->fight_bp_left--;
		}
	}

	*sheet_ptr = -1;
	FIG_call_draw_pic();
	FIG_remove_from_list(g_fig_cb_marker_id, 0);
	g_fig_cb_marker_id = -1;
	FIG_set_sheet(hero->fighter_id, 0);

	draw_fight_screen(0);
	memset(&g_fig_anisheets[0], -1, 243);
	FIG_init_list_elem(hero_pos + 1);
}

/**
 * \brief   changes the weapon of a hero
 *
 * \param   hero        pointer to the hero with a broken weapon
 * \return              1 if a weapon was found, 0 if the hero fights now with bare hands
 */
signed int AFIG_change_hero_weapon(struct struct_hero *hero)
{
	signed int pos;
	signed int has_new_weapon = 0;
	struct item_stats *item_p;
	signed int item_id;
	struct struct_fighter *fighter;

	for (pos = HERO_INVENTORY_SLOT_KNAPSACK_1; pos < NR_HERO_INVENTORY_SLOTS; pos++) {

		item_id = hero->inventory[pos].item_id;
		item_p = &g_itemsdat[item_id];

		/* grab the first melee weapon in the knapsack,
		 * and exchange it with the broken weapon. */
		if (item_p->flags.weapon &&
			(item_p->item_subtype_id != WEAPON_TYPE_SCHUSSWAFFE) &&
			(item_p->item_subtype_id != WEAPON_TYPE_WURFWAFFE) &&
			(item_p->item_subtype_id != WEAPON_TYPE_SPEER)) // Original-Bug: WEAPON_TYPE_SPEER is a melee weapon category.
		{
			move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, pos, hero);
			has_new_weapon = 1;
			break;
		}
	}

	if (!has_new_weapon) {

		/* find a free slot, to get rid of the broken weapon */
		for (pos = HERO_INVENTORY_SLOT_KNAPSACK_1; pos < NR_HERO_INVENTORY_SLOTS; pos++) {

			if (hero->inventory[pos].item_id == ITEM_ID_NONE) {

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

	hero->fight_bp_left = hero->fight_bp_left - 2;

	return has_new_weapon;
}

/**
 * \brief   check if a neighbour can be attacked
 *
 * \param   start_x     X-Coordinate of the hero
 * \param   start_y     Y-Coordinate of the hero
 * \param   offset_x    X-Direction -1/0/+1
 * \param   offset_y    Y-Direction -1/0/+1
 * \param   mode        0 = Attack only enemies / 1 = Attack heroes and enemies / 2 = Attack only Heros
 * \return              1 if an attack is possible in that mode, else 0.
 */
signed int AFIG_can_attack_neighbour(const signed int start_x, const signed int start_y,
			const signed int offset_x, const signed int offset_y, const signed int mode)
{
	const signed char target = get_cb_val(start_x + offset_x, start_y + offset_y);

	if (mode == 1) {

		/* target is hero or enemy */
		if ( ( (target > 0) && (target < 10) &&	!get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious) ||
			((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead &&
				//g_enemy_sheets[target - 10].flags.renegade
				((struct enemy_flags*)(target * sizeof(struct enemy_sheet) + (uint8_t*)g_enemy_sheets - 10 * sizeof(struct enemy_sheet) + 0x31))->renegade
			))
		{
			return 1;
		} else {
			return 0;
		}

	} else if (!mode) {

		/* target is an enemy */
		if ((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead) {
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
signed int AFIG_search_spell_target(const signed int x, const signed int y, const signed int viewdir, const signed int renegade)
{
	signed int x_diff;
	signed int y_diff;
	signed char target;
	signed int done;
	signed int will_attack;

	done = 0;
	x_diff = 0;
	y_diff = 0;
	will_attack = 0;

	while (done == 0) {
		/* calculate the offset from direction */
		if (viewdir == FIG_VIEWDIR_RIGHT) {
			x_diff++;
		} else if (viewdir == FIG_VIEWDIR_DOWN) {
			y_diff--;
		} else if (viewdir == FIG_VIEWDIR_LEFT) {
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
		target = get_cb_val(x + x_diff, y + y_diff);

		if (renegade == 1) {

			/* attack only heroes and renegade enemies */
			if ( ((target > 0) && (target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious) ||
				((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead &&
				// g_enemy_sheets[target - 10].flags.renegade
				((struct enemy_flags*)(target * sizeof(struct enemy_sheet) + (uint8_t*)g_enemy_sheets - 10 * sizeof(struct enemy_sheet) + 0x31))->renegade
				))
				{

					will_attack = 1;
					done = 1;

			} else if ( (target != 0) && (((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead) || ((target >= 50) && !is_in_int_array(target - 50, g_cb_obj_nonobstacle))))
				{
					done = 1;
				}

		} else if (renegade == 0) {

			/* attack only enemies */
			if ((target >= 10) && (target < 30) && !g_enemy_sheets[target - 10].flags.dead)
			{
				will_attack = 1;
				done = 1;
			} else if ((target != 0) &&
					 (
#ifdef M302de_ORIGINAL_BUGFIX
						(target > 0) &&
#endif
						 (((target < 10) && !get_hero(target - 1)->flags.dead && !get_hero(target - 1)->flags.unconscious
						) || ( (target >= 50) && !is_in_int_array(target - 50, g_cb_obj_nonobstacle)
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
		return target;
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
signed int AFIG_select_range_target(struct struct_hero *hero, const signed int hero_pos, const signed int renegade, signed int x, signed int y)
{
	signed int dir;
	signed int count;
	signed int done;
	signed int retval;
	signed int target_found;
	signed int target_x;
	signed int target_y;

	retval = 0;
	done = 0;

	while ((done == 0) && (hero->fight_bp_left > 0)) {

		/* reset target fight-id */
		hero->target_object_id = 0;

		/* REMARK: spells usually require 5 BP */
		if (hero->fight_bp_left >= 3) {

			dir = hero->viewdir;

			count = 0;

			/* try to find a target clockwise from current direction */
			while (!hero->target_object_id && (count < 4)) {

				hero->target_object_id = AFIG_search_spell_target(x, y, dir, renegade);

				count++;

				if (++dir == 4) {
					dir = 0;
				}
			}
		}

		/* check if a target was found */
		if (hero->target_object_id != 0) {

			/* yes */

			FIG_search_obj_on_cb(hero->target_object_id, &target_x, &target_y);

			if (manhattan_distance(target_x, target_y, x, y) < 2) {
				retval = 2;
			} else {
				retval = 1;
			}

			done = 1;

		} else {
			/* try a more expensive search */

			if (!hero->flags.tied) {

				if (renegade == 0) {
					target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 9);
				} else {
					target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 8);
				}

				if (target_found != -1) {

					FIG_prepare_hero_ani(hero, hero_pos);

					FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

					/* REMARK: spells usually require 5 BP */
					if (hero->fight_bp_left < 3) {

						/* set BP to 0 */
						hero->fight_bp_left = 0;
					}
				} else {
					/* set BP to 0 */
					hero->fight_bp_left = 0;
				}
			} else {
				/* set BP to 0 */
				hero->fight_bp_left = 0;
			}
		}
	}

	return retval;
}

/**
 * \brief   TODO
 *
 * \param   spell_id       spell index
 * \param   renegade    0 = hero normal, 1 = hero renegade
 * \return              TODO	{-1, 0, 1, 2}
 */
signed int AFIG_get_spell(const signed int spell_id, const signed int renegade)
{
	struct spell_descr *spell_description;
	signed int retval = -1;

	/* make a pointer to the spell description */
	spell_description = &g_spell_descriptions[spell_id];

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
signed int AFIG_select_autospell(struct struct_hero *hero, const signed int hero_pos, const signed int renegade, signed int x, signed int y)
{
	signed int i;
	signed int count;
	signed int spell_id;
	signed int done;
	signed int retval;
	signed int spell_mode;
	signed int target_found;
	signed int decided;

	struct viewdir_offsets a = g_fig_viewdir_offsets3;

	retval = 0;
	done = 0;

	while ((done == 0) && (hero->fight_bp_left > 0)) {

		decided = 0;

		for (i = 0; i <= 10; i++) {

			/* get a spell from an array */
			spell_id = g_af_spell_list[i];

			if ((g_spell_descriptions[spell_id].range == 1) && (random_schick(100) < 50))
			{
				decided = 1;
				break;

			} else if (random_schick(100) < 25) {
				decided = 1;
				break;
			}
		}

		if (decided == 0) {

			spell_id = g_af_spell_list[random_interval(0, 10)];
		}

		/* reset the target of the hero */
		hero->target_object_id = 0;

		if ((spell_mode = AFIG_get_spell(spell_id, renegade)) != -1) {

			if (spell_mode == 2) {

				/* set target to hero */
				hero->target_object_id = hero_pos + 1;

				/* set spell */
				hero->spell_id = spell_id;

				retval = 1;
				done = 1;
			} else {

				if (!g_spell_descriptions[spell_id].range) {

					while ((hero->fight_bp_left != 0) && (done == 0)) {

						i = hero->viewdir;

						count = 0;
						while ((!hero->target_object_id) && (count < 4)) {

							if (AFIG_can_attack_neighbour(x, y, a.offset[i].x, a.offset[i].y, spell_mode)) {
								hero->target_object_id = get_cb_val(x + a.offset[i].x, y + a.offset[i].y);
							}

							count++;

							if (++i == 4) {
								i = 0;
							}
						}

						if (hero->target_object_id != 0) {

							if (hero->fight_bp_left >= 5) {

								/* enough BP */
								hero->spell_id = spell_id;
								retval = 1;
							} else {
								/* set BP to 0 */
								hero->fight_bp_left = 0;
							}

							done = 1;

						} else if (!hero->flags.tied) {

							if (spell_mode == 0) {
								target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 3);
							} else {
								target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 1);
							}

							if (target_found != -1) {

								FIG_prepare_hero_ani(hero, hero_pos);
								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

							} else {
								/* set BP to 0 */
								hero->fight_bp_left = 0;
							}

						} else {
							/* set BP to 0 */
							hero->fight_bp_left = 0;
						}
					}
				} else {
					while ((done == 0) && (hero->fight_bp_left > 0)) {

						i = hero->viewdir;

						count = 0;

						while (!hero->target_object_id && (count < 4)) {

							hero->target_object_id = AFIG_search_spell_target(x, y, i, spell_mode);

							count++;

							if (++i == 4) {
								i = 0;
							}
						}

						if (hero->target_object_id != 0) {

							if (hero->fight_bp_left >= 5) {

								/* enough BP */
								hero->spell_id = spell_id;
								retval = 1;
							} else {
								/* set BP to 0 */
								hero->fight_bp_left = 0;
							}

							done = 1;

						} else if (!hero->flags.tied) {

							if (spell_mode == 0) {
								target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 9);
							} else {
								target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 8);
							}

							if (target_found != -1) {

								FIG_prepare_hero_ani(hero, hero_pos);
								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

							} else {
								/* set BP to 0 */
								hero->fight_bp_left = 0;
							}

						} else {
							/* set BP to 0 */
							hero->fight_bp_left = 0;
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
signed int AFIG_count_heroes(const signed int hero_pos)
{
	signed int cnt = 0;
	signed int i;

	/* for each hero in this group */
	for (i = 0; gs_group_member_counts[gs_active_group_id] > i; i++) {

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
void AFIG_hero_turn(struct struct_hero *hero, const signed int hero_pos, signed int x, signed int y)
{
	signed int i;
	signed int cnt;
	signed int done;
	signed int target_found;
	signed int try_autospell;
	signed int x_bak;
	signed int y_bak;
	signed int range_type;
	signed int hero_x;
	signed int hero_y;

	struct viewdir_offsets a = g_fig_viewdir_offsets4;

	done = 0;
	try_autospell = 1;

	hero->action_id = FIG_ACTION_MOVE;

	if (hero->npc_id > 0) {

		if (hero->npc_id == NPC_NARIELL) {

			/* equip LONGBOW and ARROWS in the first round,
			 * if the hero has them in the inventory */
			if ((g_fight_round == 0) &&
				(hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_ID_LANGBOGEN) &&
				(inv_slot_of_item(hero, ITEM_ID_PFEIL) != -1) &&
				(inv_slot_of_item(hero, ITEM_ID_LANGBOGEN) != -1))
			{
				move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, inv_slot_of_item(hero, ITEM_ID_LANGBOGEN), hero);

				if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id != ITEM_ID_PFEIL) {
					move_item(HERO_INVENTORY_SLOT_LEFT_HAND, inv_slot_of_item(hero, ITEM_ID_PFEIL), hero);
				}
			}

		} else if (hero->npc_id == NPC_HARIKA) {

			if (hero->le <= 12) {

				/* equip LONGBOW and ARROWS in the first round,
				 * if the hero has them in the inventory */
				if ((g_fight_round == 0) &&
					(hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_ID_LANGBOGEN) &&
					(inv_slot_of_item(hero, ITEM_ID_PFEIL) != -1) &&
					(inv_slot_of_item(hero, ITEM_ID_LANGBOGEN) != -1))
				{
					move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, inv_slot_of_item(hero, ITEM_ID_LANGBOGEN), hero);

					if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id != ITEM_ID_PFEIL) {
						move_item(HERO_INVENTORY_SLOT_LEFT_HAND, inv_slot_of_item(hero, ITEM_ID_PFEIL), hero);
					}

				} else if (FIG_weapon_gfx_id_ranged(hero) == WEAPON_GFX_ID_NONE) {
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

			if (!AFIG_count_heroes(hero_pos)) {
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

	if (hero->fight_bp_left == 1) {
		/* set BP to 0 */
		hero->fight_bp_left = 0;
	}

	while ((done == 0) && (hero->fight_bp_left > 0)) {

		/* REMARK: should be done in the timer */
		CD_enable_repeat();

		if ((hero->action_id == FIG_ACTION_FLEE) && (hero->fight_bp_left > 0)) {

			if (!hero->flags.tied) {

				target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 5);

				if (target_found != -1) {

					x_bak = x;
					y_bak = y;

					FIG_prepare_hero_ani(hero, hero_pos);

					hero->target_object_id = 0;

					if (FIG_search_obj_on_cb(hero_pos + 1, &x, &y)) {

						hero->action_id = FIG_ACTION_MOVE;

						if ((x_bak == x) && (y_bak == y)) {

							hero->fight_bp_left = 0;
						}
					} else {
						hero->fight_bp_left = 0;
					}

					if (hero->fight_bp_left < 3) {
						hero->fight_bp_left = 0;
					}

				} else {
					hero->fight_bp_left = 0;
				}
			} else {
				hero->fight_bp_left = 0;
			}

		} else {

			/* REMARK: hero->bp should be >= 5 */
			if ((hero->typus >= HERO_TYPE_HEXE) &&		/* spellcaster */
				(hero->ae > 10) &&	/* AE > 10 */
				(try_autospell != 0) &&
				(g_current_fight_id != FIGHT_ID_F144) &&	/* not in the final fight */
				g_autofight_magic) /* magic activated in auto fight */
			{
				if (AFIG_select_autospell(hero, hero_pos, hero->flags.renegade, x, y))
				{
					hero->action_id = FIG_ACTION_SPELL;
					hero->fight_bp_left = 0;

				} else {
					try_autospell = 0;
				}
			}

			if ((hero->action_id == FIG_ACTION_MOVE) && (hero->fight_bp_left > 0)) {

				if (FIG_weapon_gfx_id_ranged(hero) != WEAPON_GFX_ID_NONE) {

					if (range_attack_check_ammo(hero, 2)) {

						range_type = AFIG_select_range_target(hero, hero_pos, hero->flags.renegade, x, y);

						if (range_type != 0) {
							if (range_type == 2) {
								if (!AFIG_change_hero_weapon(hero)) {
									done = 1;
								}
							} else {
								hero->action_id = FIG_ACTION_RANGE_ATTACK;
							}
						}

						/* set BP to 0 */
						hero->fight_bp_left = 0;
					} else {
						if (!AFIG_change_hero_weapon(hero)) {
							done = 1;
						}
					}
				} else {
					hero->target_object_id = 0;

					if (hero->fight_bp_left >= 3) {

						i = hero->viewdir;
						cnt = 0;

						while (!hero->target_object_id && (cnt < 4)) {

							if (AFIG_can_attack_neighbour(x, y, a.offset[i].x, a.offset[i].y, hero->flags.renegade))
							{
								hero->target_object_id = get_cb_val(x + a.offset[i].x, y + a.offset[i].y);
							}

							cnt++;
							if (++i == 4) {
								i = 0;
							}
						}
					}

					if (hero->target_object_id != 0) {

						hero->action_id = FIG_ACTION_MELEE_ATTACK;
						hero->fight_bp_left = 0;

					} else {

						if (!hero->flags.tied) {

							if (!hero->flags.renegade) {
								target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 3);
							} else {
								target_found = FIG_find_path_to_target((uint8_t*)hero, hero_pos, x, y, 1);
							}

							if (target_found != -1) {
								x_bak = x;
								y_bak = y;

								FIG_prepare_hero_ani(hero, hero_pos);

								hero->action_id = FIG_ACTION_MOVE;
								hero->target_object_id = 0;

								FIG_search_obj_on_cb(hero_pos + 1, &x, &y);

								if ((x_bak == x) && (y_bak == y)) {
									hero->fight_bp_left = 0;
								}

								if (hero->fight_bp_left < 3) {
									hero->fight_bp_left = 0;
								}
							} else {
								hero->fight_bp_left = 0;
							}
						} else {
							hero->fight_bp_left = 0;
						}
					}
				}
			}
		}
	}
}
