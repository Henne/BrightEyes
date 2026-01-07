/**
 *	Rewrite of DSA1 v3.02_de functions of seg042 (fightsystem: hero action)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg042.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg038.h"
#include "seg039.h"
#include "seg041.h"
#include "seg043.h"
#include "seg044.h"
#include "seg045.h"
#include "seg096.h"
#include "seg098.h"

static struct viewdir_offsets8s g_fig_viewdir_inverse_offsets2 = { { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } } }; // ds:0x6178
static const char g_string_casts_spell[14] = "%s ZAUBERT %s"; // ds:0x6180

signed int g_spell_illusionen;		// ds:0xe3a4, 1 = spell has effect
signed int g_defender_dead;		// ds:0xe3a6
signed int g_attacker_dead;		// ds:0xe3a8
signed int g_fig_critical_fail_backfire_1;	// ds:0xe3aa
	/* either: critical attack failure, defender gets a free attack
	 * or: critical defense failure, attacker gets another free attack.
	 * Exception: see g_fig_critical_fail_backfire_2 below. */
signed int g_fig_critical_fail_backfire_2;	// ds:0xe3ac
	/* Used instead of critical attack failure_1 in this special situation:
	 * The attacker is an enemy, the defender does a critical parry failure,
	 * and the attacking enemy gets a free attack.
	 * It is not clear why g_fig_critical_fail_backfire_2 exists next to critical attack failure_1.
	 * Maybe the intention was:
	 * g_fig_critical_fail_backfire_1 for backfire from a critical attack failure,
	 * g_fig_critical_fail_backfire_2 for backfire from a critical parry failure?
	 * In this case, one assignment g_fig_critical_fail_backfire_1 = 1 below should rather be g_fig_critical_fail_backfire_2 = 1.
	 */



/**
 * \brief   executes the fight action of hero
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position in the group (object_id = hero_pos + 1)
 */
void FIG_do_hero_action(struct struct_hero* hero, const signed int hero_pos)
{
	signed int damage;
	struct enemy_sheet *p_target_enemy;
	struct struct_hero *p_target_hero;
	struct inventory *p_weapon_attacker;
	struct inventory *p_weapon_target;
	signed int attacker_weapon_gfx_id;
	signed int target_weapon_gfx_id;
	signed int critical_failure_roll_2d6;
	signed int attacker_hits_target;
	signed int spellcast_ani_id;
	signed int at_roll_d20;
	signed int pa_roll_d20;
	signed int atpa;
	signed int target_pa_val; /* of attacked enemy or hero */
	signed int target_hero_at_val;
	signed int projectile_gfx_id; /* some ani related value */
	signed int ranged_attack_nonadjacent_flag; /* for ranged or spell attack: 0: at an adjacent square; 1: at a square at distance >= 2 */
	signed int target_is_hero = 0;
	signed int spell_test_result;
	struct struct_fighter *fighter_add;
	signed int width;
	signed int height;

	signed int target_cannot_parry = 0;
	/* The target cannot parry if it is a double-size enemy,
	 * which is attacked either at the tail part or at a flank (i.e., not at the front) of the head part */

	signed int target_object_id_was_modified = 0;
	/* This happens when the target is the tail part of a double-size enemy.
	 * In this case, the object_id entry will be adjusted to the head part. */
	
	signed int object_id;
	struct viewdir_offsets8s inverse_offset = g_fig_viewdir_inverse_offsets2;
	signed int attacker_x;
	signed int attacker_y;
	signed int target_x;
	signed int target_y;
	signed int viewdir;
	struct struct_fighter *fighter;
	struct actor_class grammar_tmp;
	signed int fg_bak;
	signed int bg_bak;
	struct nvf_extract_desc nvf;

	call_mouse_bg();

	if (g_in_fight) {

		FIG_clear_msgs();

		attacker_hits_target = g_fig_critical_fail_backfire_2 = g_fig_critical_fail_backfire_1 =
			g_spell_illusionen = g_attacker_dead = g_defender_dead = 0;

		attacker_weapon_gfx_id = target_weapon_gfx_id = WEAPON_GFX_ID_NONE;

		g_fig_actor_grammar.actor_class_type = ACTOR_CLASS_TYPE_HERO;
		g_fig_actor_grammar.actor_class_id = hero_pos;

		if (hero->target_object_id >= 10) {

			/* attack enemy */
			if (hero->target_object_id >= 30) {
				/* tail part of double-size enemy */
				hero->target_object_id -= 20;
				/* set target_object_id to the head part of the double-size enemy */
				target_cannot_parry = 1;
				target_object_id_was_modified = 1;
			}

			p_target_enemy = &g_enemy_sheets[hero->target_object_id - 10];

			/* attacked enemy won't be asleep any more */
			p_target_enemy->flags.asleep = 0;

			g_fig_target_grammar.actor_class_type = ACTOR_CLASS_TYPE_MONSTER;
			g_fig_target_grammar.actor_class_id = p_target_enemy->monster_id;

			if (
				(!p_target_enemy->monster_id)
				|| (
					p_target_enemy->flags.dead
					&& ((hero->action_id != FIG_ACTION_SPELL) || (hero->spell_id != SP_SKELETTARIUS_KRYPTADUFT))
				)
			) {
				call_mouse();
				return;
			}

			if ((is_in_byte_array(p_target_enemy->actor_sprite_id, g_double_size_actor_sprite_id_table)) && (target_cannot_parry == 0))
					/* attack head part of double-size enemy */
			{
				FIG_search_obj_on_cb(hero->target_object_id, &target_x, &target_y);
				FIG_search_obj_on_cb(hero_pos + 1, &attacker_x, &attacker_y);

				/* In the following, viewdir is pointing from the target to the attacker */
				if (attacker_x == target_x) {

					/* set dir to the direction from the attacker towards the enemy */
					if (target_y < attacker_y) {
						viewdir = FIG_VIEWDIR_UP;
					} else {
						viewdir = FIG_VIEWDIR_DOWN;
					}
				} else {
					if (target_x < attacker_x) {
						viewdir = FIG_VIEWDIR_RIGHT;
					} else {
						viewdir = FIG_VIEWDIR_LEFT;
					}
				}

				if (p_target_enemy->viewdir != viewdir) {

					/* target enemy is not looking towards the attacker */

					object_id = get_cb_val(attacker_x + inverse_offset.offset[viewdir].x, attacker_y + inverse_offset.offset[viewdir].y);
					/* object_id now refers to the target */

					/* the following if looks pretty redundant.
					 * object_id refers to the target, which in this branch is the head of a double-size enemy.
					 * So, automatically, 10 <= object_id < 30, and we only have to check g_enemy_sheets[object_id - 10].flags.dead
					 */
					if (object_id != 0) {

						if (
							(object_id >= 50)
							|| ((object_id < 10) && !get_hero(object_id - 1)->flags.dead)
							|| ((object_id >= 10) && (object_id < 30) && !g_enemy_sheets[object_id - 10].flags.dead)
							|| ((object_id >= 30) && (object_id < 50) && !g_enemy_sheets[object_id - 30].flags.dead))
						{
							/* the attacker attacks the head, but the enemy's viewdir is not pointing to the attacker. */
							target_cannot_parry = 1;
						}
					}
				}
			}
		} else {
			if (hero->target_object_id > 0) {

				/* hero attacks another hero */
				p_target_hero = get_hero(hero->target_object_id - 1);

				g_fig_target_grammar.actor_class_type = ACTOR_CLASS_TYPE_HERO;
				g_fig_target_grammar.actor_class_id = hero->target_object_id - 1;

				if (p_target_hero->flags.asleep) {

					/* wake up target hero */
					p_target_hero->flags.asleep = 0;

					fighter = FIG_get_fighter(p_target_hero->fighter_id);

					fighter->nvf_no = p_target_hero->viewdir;
					fighter->reload = -1;
					fighter->offsetx = 0;
					fighter->offsety = 0;
				}

				if (p_target_hero->flags.dead || !p_target_hero->typus) {
					call_mouse();
					return;
				}

				target_is_hero = 1;
			}
		}

		if (hero->action_id == FIG_ACTION_MELEE_ATTACK) {

			p_weapon_attacker = &hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND];

			attacker_weapon_gfx_id = FIG_weapon_gfx_id_melee(hero);

			if (target_is_hero != 0) {
				p_weapon_target = &p_target_hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND];

				target_weapon_gfx_id = FIG_weapon_gfx_id_melee(p_target_hero);
			}

			if (attacker_weapon_gfx_id == WEAPON_GFX_ID_NONE) {
				/* no valid weapon == bare hands */
				atpa = hero->at_talent_bonus[WEAPON_TYPE_WAFFENLOS] + hero->fight_atpa_mod - hero->rs_be / 2;
			} else {
				atpa = hero->at_talent_bonus[hero->weapon_type] + hero->fight_atpa_mod + hero->weapon_at_mod - hero->rs_be / 2;
			}

			if (hero->rs_be & 1) {
				/* if RS_BE is odd, subtract another point. Changes the rounding behavior of 'hero->rs_be / 2' above to "round up". */
				atpa--;
			}

			/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get a AT+1 bonus against orcs */
			if (gs_tevent071_orcstatue &&
				(hero->typus == HERO_TYPE_ZWERG) &&
				!target_is_hero &&
				(p_target_enemy->actor_sprite_id == ACTOR_SPRITE_ID_ORK))
			{
				atpa++;
			}

			/* spell_dunkelheit is active -> AT-4 */
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				atpa -= 4;
			}

			if (target_is_hero) {

				if (target_weapon_gfx_id == WEAPON_GFX_ID_NONE) {
					/* Original-Bug? Why is target_hero mixed with hero??
					 * Probably, the intention was
					 * target_hero_at_val = p_target_hero->at_talent_bonus[WEAPON_TYPE_WAFFENLOS] + p_target_hero->fight_atpa_mod - p_target_hero->rs_be / 2;
					 */
					target_hero_at_val = p_target_hero->at_talent_bonus[WEAPON_TYPE_WAFFENLOS] + p_target_hero->fight_atpa_mod - hero->rs_be / 2;
				} else {
					/* Probably, the intention was
					 * target_hero_at_val = p_target_hero->at_talent_bonus[p_target_hero->weapon_type] + p_target_hero->fight_atpa_mod - p_target_hero->rs_be / 2;
					 */
					target_hero_at_val = p_target_hero->at_talent_bonus[p_target_hero->weapon_type] + p_target_hero->fight_atpa_mod - hero->rs_be / 2;
				}

				/* Probably, the intention was
				 * if (target_hero->rs_be & 1) {
				 */
				if (hero->rs_be & 1) {
					/* if rs_be is odd, subtract another point. Changes the rounding behavior of 'target_hero->rs_be / 2' above to "round up". */
					target_hero_at_val--;
				}

				/* spell_dunkelheit is active -> AT-4 */
				if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
					target_hero_at_val -= 4;
				}

				/* target hero has already parried another atack => AT+2 */
				if (g_fig_hero_parry_action_used[hero->target_object_id - 1] == 1) {
					atpa += 2;
				}

			} else {
				/* target is enemy */
				if ((g_fig_enemy_parry_action_used[hero->target_object_id] == 1) || (target_cannot_parry != 0)) {
					/* target enemy has already parried another atack or cannot parry => AT+2 */
					atpa += 2;
				}
			}

			at_roll_d20 = random_schick(20);

			if ((at_roll_d20 == 20) && (random_schick(20) > atpa - 9)) {
				/* critical attack failure */

				damage = 0;
				FIG_add_msg(1, 0);

				critical_failure_roll_2d6 = random_interval(2, 12);

				if (
					(critical_failure_roll_2d6 == 2)
					&& (attacker_weapon_gfx_id != WEAPON_GFX_ID_NONE)
					&& (p_weapon_attacker->bf != -99)
				) {
					/* weapon broken */
					p_weapon_attacker->flags.broken = 1;
					FIG_add_msg(6, 0);

				} else if ((critical_failure_roll_2d6 >= 3) && (critical_failure_roll_2d6 <= 8) && (target_cannot_parry == 0)) {
					/* defender gets a free attack */

					g_fig_critical_fail_backfire_1 = 1;

					if (target_is_hero != 0) {

						if (random_schick(20) <= target_hero_at_val) {
							damage = FIG_get_hero_weapon_attack_damage(p_target_hero, hero, 1);
						}

					} else {

						if (random_schick(20) <= p_target_enemy->at) {

							damage = FIG_get_enemy_attack_damage(p_target_enemy, (struct enemy_sheet*)hero, 0);
						}
					}

					if (damage > 0) {

						sub_hero_le(hero, damage);

						FIG_add_msg(8, damage);

						/* swap msg struct */
						grammar_tmp = g_fig_target_grammar;
						g_fig_target_grammar = g_fig_actor_grammar;
						g_fig_actor_grammar = grammar_tmp;
					}

					if (hero->flags.dead) {
						g_attacker_dead = 1;
					}

				} else if ((critical_failure_roll_2d6 >= 9) && (critical_failure_roll_2d6 <= 11)) {
					/* attacker hurts himself. 1W6 damage. */

					damage = random_schick(6);

					sub_hero_le(hero, damage);

					FIG_add_msg(8, damage);

					g_fig_target_grammar = g_fig_actor_grammar;

					if (hero->flags.dead) {
						g_attacker_dead = 1;
					}
				}

				// critical_failure_roll_2d6 == 12: luck in misfortune. No further effect.

			} else if (at_roll_d20 <= atpa) {
				/* attack succeeded */

				/* check if parry is allowed */
				if (((target_is_hero == 0) && !g_fig_enemy_parry_action_used[hero->target_object_id] && (target_cannot_parry == 0)) ||
					((target_is_hero) && !g_fig_hero_parry_action_used[hero->target_object_id - 1]))
				{

					if (target_is_hero != 0) {

						if (target_weapon_gfx_id == WEAPON_GFX_ID_NONE) {
							target_pa_val = p_target_hero->pa_talent_bonus[WEAPON_TYPE_WAFFENLOS] - p_target_hero->fight_atpa_mod - p_target_hero->rs_be / 2;
						} else {
							target_pa_val = p_target_hero->pa_talent_bonus[p_target_hero->weapon_type] - p_target_hero->fight_atpa_mod - p_target_hero->rs_be / 2 + p_target_hero->weapon_at_mod;
						}

						if (p_target_hero->action_id == FIG_ACTION_GUARD) {
							target_pa_val += 3;
						}
					} else {

						target_pa_val = p_target_enemy->pa;

						if (p_target_enemy->blind) {
							target_pa_val -= 5;
						}

						if (p_target_enemy->flags.tied) { /* check 'tied' flag */
							target_pa_val -= 2;
						} else if (p_target_enemy->flags.dancing) { /* check 'dancing' flag */
							target_pa_val -= 3;
						}

						if (p_target_enemy->flags.petrified || /* check 'petrified' flag */
							p_target_enemy->flags.busy || /* check 'busy' flag */
							p_target_enemy->flags.mushroom || /* check 'mushroom' flag */
							p_target_enemy->flags.tame || /* check 'tame' flag */
							p_target_enemy->flags.renegade) /* check 'renegade' flag */
						{
							target_pa_val = 0;
						}
					}

					/* spell_dunkelheit is active */
					if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
						target_pa_val -= 4;
					}

					if (target_pa_val < 0) {
						target_pa_val = 0;
					}

					pa_roll_d20 = random_schick(20);

					if ((pa_roll_d20 == 20) && (random_schick(20) > target_pa_val - 7)) {
						/* critical defense failure */

						damage = 0;

						FIG_add_msg(4, 0);

						critical_failure_roll_2d6 = random_interval(2, 12);

						if ((critical_failure_roll_2d6 >= 3) && (critical_failure_roll_2d6 <= 8)) {

							g_fig_critical_fail_backfire_1 = 1;
							/* Maybe the intention was:
							 * g_fig_critical_fail_backfire_2 = 1
							 * See comment at the declaration of g_fig_critical_fail_backfire_2. */

							if (random_schick(20) <= atpa) {

								if (target_is_hero != 0) {

									damage = FIG_get_hero_weapon_attack_damage(hero, p_target_hero, 1);

									if (damage > 0) {

										sub_hero_le(p_target_hero, damage);

										FIG_add_msg(8, damage);

										if (p_target_hero->flags.dead) {
											g_defender_dead = 1;
										}
									}
								} else {

									damage = FIG_get_hero_weapon_attack_damage(hero, (struct struct_hero*)p_target_enemy, 0);

									if (damage > 0) {

										FIG_damage_enemy(p_target_enemy, damage, 0);

										FIG_add_msg(11, damage);

										if (p_target_enemy->flags.dead) {
											g_defender_dead = 1;
										}
									}
								}
							}
						} else if ((critical_failure_roll_2d6 >= 9) && (critical_failure_roll_2d6 <= 11)) {

							damage = random_schick(6);

							if (target_is_hero != 0) {

								sub_hero_le(p_target_hero, damage);

								FIG_add_msg(8, damage);

								if (p_target_hero->flags.dead) {
									g_defender_dead = 1;
								}
							} else {
								FIG_damage_enemy(p_target_enemy, damage, 1);

								FIG_add_msg(11, damage);

								if (p_target_enemy->flags.dead) {
									g_defender_dead = 1;
								}
							}
						}
					} else {

						if (pa_roll_d20 > target_pa_val) {
							/* parry failed, but not critical */
							attacker_hits_target = 1;
						} else {
							/* parry succeeded */
							FIG_add_msg(5, 0);

							if ((pa_roll_d20 == at_roll_d20) && (p_weapon_attacker->bf != -99)) {
							/* both attacker and defender damage their weapon.
							 *
							 * Original-Bug? The following logic is quite strange.
							 *
							 * If the attacker has an indestructible weapon (bf == 99),
							 * neither weapon is damaged (i.e., also the defender is saved)
							 *
							 * Usually, the attacker weapon and the weapon of a defending hero will damaged,
							 * and if the bf of attacking weapon is >= 4, there is a chance that *both* are broken instead.
							 *
							 * In all above situations, the weapon of a defending *enemy* will never be damaged,
							 * only defending *heroes* are affected.
							 */

								if (p_weapon_attacker->bf > 3) {

									if ((random_schick(12) + p_weapon_attacker->bf) > 15) {

										if ((target_is_hero != 0) && (p_weapon_target->bf != -99))
										{
											p_weapon_target->flags.broken = 1;
										}

										p_weapon_attacker->flags.broken = 1; /* set 'broken' flag */

										FIG_add_msg(6, 0);

									} else {
										if (target_is_hero != 0) {
											p_weapon_target->bf++;
										}

										p_weapon_attacker->bf++;
									}
								} else {

									if (target_is_hero != 0) {
										p_weapon_target->bf++;
									}

									p_weapon_attacker->bf++;
								}
							}
						}
					}
				} else {
					attacker_hits_target = 1;
				}

				if (attacker_hits_target != 0) {

					if (target_is_hero != 0) {

						damage = FIG_get_hero_weapon_attack_damage(hero, p_target_hero, 1);

						if (damage > 0) {

							sub_hero_le(p_target_hero, damage);

							FIG_add_msg(8, damage);

							if (p_target_hero->flags.dead) {
								g_defender_dead = 1;
							}
						}
					} else {

						damage = FIG_get_hero_weapon_attack_damage(hero, (struct struct_hero*)p_target_enemy, 0);

						if (damage > 0 ) {

							FIG_damage_enemy(p_target_enemy, damage, 0);

							FIG_add_msg(11, damage);

							if (p_target_enemy->flags.dead) {
								g_defender_dead = 1;
							}
						}
					}
				}
			}

			clear_anisheets();

			FANI_prepare_fight_hero_ani(
				0,
				hero,
				attacker_weapon_gfx_id,
				FIG_ACTION_MELEE_ATTACK,
				hero_pos + 1,
				target_object_id_was_modified == 0 ? hero->target_object_id : hero->target_object_id + 20,
				0
			);

			if (target_is_hero != 0) {
				/* target is hero */

				if (check_hero(p_target_hero) || (g_defender_dead != 0)) {

					FANI_prepare_fight_hero_ani(
						1,
						p_target_hero,
						target_weapon_gfx_id,
						FIG_ACTION_PARRY,
						hero->target_object_id,
						hero_pos + 1,
						1
					);
				}
			} else {
				/* target is enemy */

				if (target_cannot_parry == 0) {
					FANI_prepare_fight_enemy_ani(
						1,
						p_target_enemy,
						FIG_ACTION_PARRY,
						hero->target_object_id,
						hero_pos + 1,
						1
					);
				} else {
					/* flank or rear is attacked -> no parry animation */
					if (g_defender_dead != 0) {
						FANI_prepare_fight_enemy_ani(
							1,
							p_target_enemy,
							FIG_ACTION_NONE,
							hero->target_object_id,
							hero_pos + 1,
							1
						);
					}
				}
			}

			g_fig_continue_print = 1;

			draw_fight_screen_pal(0);

			clear_anisheets();

		} else {

			if (hero->action_id == FIG_ACTION_RANGE_ATTACK) {

				attacker_weapon_gfx_id = FIG_weapon_gfx_id_ranged(hero);

				if (attacker_weapon_gfx_id != WEAPON_GFX_ID_NONE) {

#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 32: throwing weapon in a ranged attack is dropped before the damage logic is done.
					 * As result, all throwing weapons (except throwing stars in a pile of at least 2 units) will be treated
					 * as weaponless fighting for the damage calculation.
					 * This means that they will always hit (without a talent test) and will do only 1D6 damage.
					 * found 2016-04-02 by NRS at https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=146051#pid146051
					 * "Oh mein Gott..." */
					if (!range_attack_check_ammo(hero, 0)) {
						return;
					}
#endif

					if (target_is_hero != 0) {

						/* note that for ranged attacks, the talent test will be done in the following function call. */
						damage = FIG_get_hero_weapon_attack_damage(hero, p_target_hero, 1);
#ifdef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 32:
						 * Fix: move the function call after the damage calculation.
						 */
						if (!range_attack_check_ammo(hero, 0)) {
							return;
						}
#endif

						if (damage > 0) {

							sub_hero_le(p_target_hero, damage);

							FIG_add_msg(8, damage);

							if (p_target_hero->flags.dead) {
								g_defender_dead = 1;
							}
						}
					} else {

						/* note that for ranged attacks, the talent test will be done in the following function call. */
						damage = FIG_get_hero_weapon_attack_damage(hero, (struct struct_hero*)p_target_enemy, 0);
#ifdef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 32:
						 * Fix: move the function call after the damage calculation.
						 */
						if (!range_attack_check_ammo(hero, 0)) {
							return;
						}
#endif

						if (damage > 0 ) {

							FIG_damage_enemy(p_target_enemy, damage, 0);

							FIG_add_msg(11, damage);

							if (p_target_enemy->flags.dead) {
								g_defender_dead = 1;
							}
						}
					}

					clear_anisheets();

					projectile_gfx_id = attacker_weapon_gfx_id;
					ranged_attack_nonadjacent_flag = 0;

					FIG_call_draw_pic();

					FANI_prepare_fight_hero_ani(
						0,
						hero,
						attacker_weapon_gfx_id,
						FIG_ACTION_RANGE_ATTACK,
						hero_pos + 1,
						target_object_id_was_modified == 0 ? hero->target_object_id : hero->target_object_id + 20,
						0
					);

					ranged_attack_nonadjacent_flag = FANI_prepare_projectile_ani(
						7,
						projectile_gfx_id,
						hero_pos + 1,
						target_object_id_was_modified == 0 ? hero->target_object_id : hero->target_object_id + 20,
						hero->viewdir
					);

					FIG_set_sheet(hero->fighter_id, 0);

					draw_fight_screen_pal(0);

					if (FIG_weapon_gfx_id_ranged(hero) == WEAPON_GFX_ID_NONE) {

						fighter = FIG_get_fighter(hero->fighter_id);

						fighter->nvf_no = hero->viewdir;
						fighter->reload = -1;
					}

					if (ranged_attack_nonadjacent_flag != 0) {

						FIG_set_sheet(g_fig_projectile_id, 7);

						draw_fight_screen(ranged_attack_nonadjacent_flag == 0 && g_defender_dead == 0 ? 0 : 1);

						FIG_make_invisible(g_fig_projectile_id);
					}

					g_fig_continue_print = 1;

					if (g_defender_dead != 0) {

						if (target_is_hero != 0) {
							/* target is hero */
							FANI_prepare_fight_hero_ani(
								1,
								p_target_hero,
								-1,
								FIG_ACTION_NONE,
								hero->target_object_id,
								hero_pos + 1,
								1
							);
						} else {
							/* target is enemy */
							FANI_prepare_fight_enemy_ani(
								1,
								p_target_enemy,
								FIG_ACTION_NONE,
								hero->target_object_id,
								hero_pos + 1,
								1
							);
						}
					}

					FANI_remove_projectile();

					draw_fight_screen(0);

					clear_anisheets();
				}

			} else if (hero->action_id == FIG_ACTION_SPELL) {

				/* cast a spell */

				if (g_current_fight_id == FIGHT_ID_F144) {
					/* No spells allowed in the final fight -> instant death */

					sub_hero_le(hero, hero->le + 1);

					g_attacker_dead = 1;
				}

				spellcast_ani_id = g_spell_descriptions[hero->spell_id].ani;

				*g_dtp2 = '\0';

				spell_test_result = use_spell(hero, 0, 0);

				clear_anisheets();

				if (g_autofight != 0) {
					g_pic_copy.x1 = g_pic_copy.v1 = 0;
					g_pic_copy.y1 = g_pic_copy.v2 = 194;
					g_pic_copy.x2 = 318;
					g_pic_copy.y2 = 199;
					g_pic_copy.src = g_buffer8_ptr;

					do_pic_copy(3);

					get_textcolor(&fg_bak, &bg_bak);
					set_textcolor(0xff, 0x00);

					sprintf(g_text_output_buf, g_string_casts_spell, hero->alias,
						get_ttx(hero->spell_id + 0x6a));

					GUI_print_string(g_text_output_buf, 1, 194);

					set_textcolor(fg_bak, bg_bak);

					vsync_or_key(50);
				}

				if (hero->target_object_id != 0) {

					projectile_gfx_id = ranged_attack_nonadjacent_flag = PROJECTILE_GFX_ID_SPELLCAST_ORB; // == 0

					if (random_schick(100) > 50) {
						projectile_gfx_id = PROJECTILE_GFX_ID_SPELLCAST_BOLT;
					}

					if (hero->target_object_id < 10) {
						/* target is a hero */
						projectile_gfx_id = PROJECTILE_GFX_ID_SPELLCAST_STAR;
					}

					FIG_call_draw_pic();

					if (spell_test_result != -1) {

						FANI_prepare_spell_hero(
							0,
							hero,
							4,
							hero_pos + 1,
							target_object_id_was_modified == 0 ? hero->target_object_id : hero->target_object_id + 20,
							projectile_gfx_id,
							0
						);
					}

					if (spell_test_result > 0) {

						if (spellcast_ani_id > 0) {

							if (spellcast_ani_id != 4) {
								FANI_prepare_hero_spell_ani(6, hero, spellcast_ani_id);
							}

						} else {

							if (hero->target_object_id > 0) {

								if (target_is_hero == 0) {

									FANI_prepare_spell_enemy(
										1,
										p_target_enemy,
										99,
										target_object_id_was_modified == 0 ? hero->target_object_id : hero->target_object_id + 20,
										hero_pos + 1,
										1
									);
								} else {

									if (check_hero(p_target_hero) || (g_defender_dead != 0)) {

										FANI_prepare_spell_hero(
											1,
											p_target_hero,
											99,
											hero->target_object_id,
											0,
											-1,
											1
										);
									}
								}
							}
						}

						if (
							(hero->actor_sprite_id != ACTOR_SPRITE_ID_HEXE__MALE)
							&& (hero->actor_sprite_id != ACTOR_SPRITE_ID_HEXE__FEMALE)
							&& (hero->target_object_id > 0)
						) {

							ranged_attack_nonadjacent_flag = FANI_prepare_projectile_ani(
								7,
								projectile_gfx_id,
								hero_pos + 1,
								target_object_id_was_modified == 0 ? hero->target_object_id : hero->target_object_id + 20,
								hero->viewdir
							);
						}
					}

					if (spell_test_result != -1) {

						FIG_set_sheet(hero->fighter_id, 0);
						draw_fight_screen_pal(1);
					}

					if (spell_test_result > 0) {

						if (ranged_attack_nonadjacent_flag != 0) {

							FIG_set_sheet(g_fig_projectile_id, 7);

							draw_fight_screen(1);

							FIG_make_invisible(g_fig_projectile_id);
						}

						if (spellcast_ani_id > 0) {

							if (spellcast_ani_id != 4) {
								FIG_set_sheet(g_fig_spellgfx_id, 6);
							} else {

								FIG_call_draw_pic();

								FIG_remove_from_list(p_target_enemy->fighter_id, 1);


								nvf.dst = g_fig_list_elem.gfxbuf;
								nvf.src = g_spellobj_nvf_buf;
								nvf.image_num = 26;
								nvf.compression_type = 0;
								nvf.width = &width;
								nvf.height = &height;

								process_nvf_extraction(&nvf);

								g_fig_list_elem.offsetx = 0;
								g_fig_list_elem.offsety = 0;
								g_fig_list_elem.height = (signed char)height;
								g_fig_list_elem.width = (signed char)width;
								g_fig_list_elem.x1 = 0;
								g_fig_list_elem.y1 = 0;
								g_fig_list_elem.x2 = (signed char)(width - 1);
								g_fig_list_elem.y2 = (signed char)(height - 1);
								g_fig_list_elem.reload = 0;

								FIG_add_to_list(p_target_enemy->fighter_id);

							}
						} else {

							if (target_is_hero == 0) {

								FIG_set_sheet(p_target_enemy->fighter_id, 1);

								if (is_in_byte_array(p_target_enemy->actor_sprite_id, g_double_size_actor_sprite_id_table))
								{
									fighter_add = FIG_get_fighter(p_target_enemy->fighter_id);

									FIG_set_sheet(g_fig_double_size_fighter_id_table[fighter_add->double_size], 3);
								}

							} else {

								if (hero->target_object_id > 0) {

									FIG_set_sheet(p_target_hero->fighter_id, 1);
								}
							}
						}

						g_fig_continue_print = 1;

						draw_fight_screen(1);

						if (spellcast_ani_id > 0) {
							FIG_make_invisible(g_fig_projectile_id);
						}

						if (g_spell_illusionen) {

							if (hero->target_object_id >= 10) {

								FIG_make_invisible(p_target_enemy->fighter_id);

								if (is_in_byte_array(p_target_enemy->actor_sprite_id, g_double_size_actor_sprite_id_table))
								{
									fighter_add = FIG_get_fighter(p_target_enemy->fighter_id);

									FIG_make_invisible(g_fig_double_size_fighter_id_table[fighter_add->double_size]);
								}
							} else {
								if (hero->target_object_id > 0) {

									FIG_make_invisible(p_target_hero->fighter_id);
								}
							}
						}

						if (ranged_attack_nonadjacent_flag != 0) {
							FANI_remove_projectile();
						}

						if ((spellcast_ani_id > 0) && (spellcast_ani_id != 3) && (spellcast_ani_id != 4)) {
							FANI_remove_spell();
						}

						FIG_draw_figures();
					}

					FIG_output(g_dtp2);

					clear_anisheets();

				} else {
					FIG_output(g_dtp2);
				}

			} else if (hero->action_id == FIG_ACTION_USE_ITEM) {

				FIG_use_item(hero, p_target_enemy, p_target_hero, target_is_hero, hero_pos);
			}
		}
	}

	call_mouse();

	if (target_object_id_was_modified != 0) {
		/* restore target_object_id */
		hero->target_object_id += 20;
	}
}
