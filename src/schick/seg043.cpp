/**
 *	Rewrite of DSA1 v3.02_de functions of seg043 (fightsystem: enemy action, use item)
 *	Functions rewritten: 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg043.cpp
 */
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg005.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg038.h"
#include "seg041.h"
#include "seg044.h"
#include "seg045.h"
#include "seg097.h"
#include "seg102.h"
#include "seg105.h"
#include "seg107.h"

signed int g_mspell_awake_flag = 0; // ds:0x618e
static struct viewdir_offsets8s g_fig_viewdir_inverse_offsets3 = { { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } } }; // ds:0x6190

/**
 * \brief   execute the fight action of an enemy
 *
 * \param   enemy     pointer to the enemy sheet
 * \param   enemy_id  id of the enemy
 */
void FIG_do_enemy_action(struct enemy_sheet* p_enemy, const signed int enemy_id)
{
	signed int damage;
	struct struct_hero *hero;
	struct enemy_sheet *target_enemy;
	struct inventory *p_weapon_target_hero;
	signed int critical_failure_roll_2d6;
	signed int weapon_gfx_id;
	signed int attacker_hits_target;
	signed int at_roll_d20;
	signed int pa_roll_d20;
	signed int attacker_at_val;
	signed int target_at_val;
	signed int target_pa_val;
	signed char target_is_hero;
	signed int projectile_gfx_id;
	signed int ranged_attack_nonadjacent_flag; /* for ranged or spell attack: 0: at an adjacent square; 1: at a square at distance >= 2 */
	signed int spell_test_result;
	signed int spellcast_ani_id;
	signed int weapon_gfx_id_ranged;
	struct struct_fighter *fighter;

	signed int target_cannot_parry = 0;
	/* The target cannot parry if it is a double-size enemy,
	 * which is attacked either at the tail part or at a flank (i.e., not at the front) of the head part */

	signed int object_id;
	signed int attacker_x;
	signed int attacker_y;
	signed int target_x;
	signed int target_y;
	signed int viewdir;
	struct viewdir_offsets8s inverse_offset = g_fig_viewdir_inverse_offsets3;
	struct actor_class grammar_tmp;

	call_mouse_bg();

	if (g_in_fight) {

		FIG_clear_msgs();

		attacker_hits_target = g_fig_critical_fail_backfire_2 = g_fig_critical_fail_backfire_1 = g_attacker_dead = g_defender_dead = 0;

		g_fig_actor_grammar.actor_class_type = ACTOR_CLASS_TYPE_MONSTER;
		g_fig_actor_grammar.actor_class_id = p_enemy->monster_id;

		if (p_enemy->target_object_id < 10) {

			/* enemy attacks hero */
			hero = get_hero(p_enemy->target_object_id - 1);

			g_fig_target_grammar.actor_class_type = ACTOR_CLASS_TYPE_HERO;
			g_fig_target_grammar.actor_class_id = p_enemy->target_object_id - 1;

			if (hero->flags.dead || !hero->typus) {
				return;
			}

			target_is_hero = 1;
		} else {
			/* enemy attacks enemy */

			target_enemy = &g_enemy_sheets[p_enemy->target_object_id - 10];

			g_fig_target_grammar.actor_class_type = ACTOR_CLASS_TYPE_MONSTER;
			g_fig_target_grammar.actor_class_id = target_enemy->monster_id;

			if (target_enemy->flags.dead || !target_enemy->monster_id) {
				return;
			}

			target_is_hero = 0;

			if ((is_in_byte_array(target_enemy->actor_sprite_id, g_double_size_actor_sprite_id_table)) && (target_cannot_parry == 0))
			{
				FIG_search_obj_on_cb(p_enemy->target_object_id, &target_x, &target_y);
				FIG_search_obj_on_cb(enemy_id + 10, &attacker_x, &attacker_y);

				if (attacker_x == target_x) {

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

				if (target_enemy->viewdir != viewdir) {

					object_id = get_cb_val(attacker_x + inverse_offset.offset[viewdir].x, attacker_y + inverse_offset.offset[viewdir].y);

					if (object_id != 0) {

						if (
							(object_id >= 50)
							|| ((object_id < 10) && !get_hero(object_id - 1)->flags.dead)
							|| ((object_id >= 10) && (object_id < 30) && !g_enemy_sheets[object_id - 10].flags.dead)
							|| ((object_id >= 30) && (object_id < 50) && !g_enemy_sheets[object_id - 30].flags.dead)
						) {
							target_cannot_parry = 1;
						}
					}
				}

			}
		}

		if (p_enemy->action_id == FIG_ACTION_MELEE_ATTACK) {

			if (p_enemy->target_object_id < 10) {

				/* target is a hero */

				p_weapon_target_hero = &hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND];

				weapon_gfx_id = FIG_weapon_gfx_id_melee(hero);

				if (weapon_gfx_id == WEAPON_GFX_ID_NONE) {
					/* no valid weapon == bare hands */
					target_at_val = hero->at_talent_bonus[WEAPON_TYPE_WAFFENLOS] + hero->fight_atpa_mod;
					target_pa_val = hero->pa_talent_bonus[WEAPON_TYPE_WAFFENLOS] - hero->fight_atpa_mod;
				} else {
					target_at_val = hero->at_talent_bonus[hero->weapon_type] + hero->fight_atpa_mod + hero->weapon_at_mod;
					target_pa_val = hero->pa_talent_bonus[hero->weapon_type] - hero->fight_atpa_mod + hero->weapon_pa_mod;
				}

				/* guarding heroes get a PA-bonus of 3 */
				if (hero->action_id == FIG_ACTION_GUARD) {
					target_pa_val += 3;
				}

				/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get a PA-bonus against orcs */
				if (gs_tevent071_orcstatue && (hero->typus == HERO_TYPE_ZWERG) && (p_enemy->actor_sprite_id == ACTOR_SPRITE_ID_ORK))
				{
					target_pa_val++;
				}

				/* subtract RS handicap */
				target_at_val -= hero->rs_be / 2;
				target_pa_val -= hero->rs_be / 2;

			} else {
				/* target is an enemy */
				target_at_val = target_enemy->at;
				target_pa_val = target_enemy->pa;
			}

			/* spell_dunkelheit() is active => AT-4, PA-4*/
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				target_at_val -= 4;
				target_pa_val -= 4;
			}

			attacker_at_val = p_enemy->at;
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				attacker_at_val -= 4;
			}

			if (target_is_hero) {

				/* target hero has already parried another atack => AT+2 */
				if (g_fig_hero_parry_action_used[p_enemy->target_object_id - 1] == 1) {
					attacker_at_val += 2;
				}

				/* 'Chamaelioni' spell is active on the target hero => AT-5 */
				if (hero->flags.chamaelioni == 1) {
					attacker_at_val -= 5;
				}

				/* 'Duplicatus' spell is active on the target hero => AT/2 */
				if (hero->flags.duplicatus == 1) {
					attacker_at_val /= 2;
				}
			} else {
				/* target enemy has already parried another atack => AT+2 */
				if (g_fig_enemy_parry_action_used[p_enemy->target_object_id] == 1) {
					attacker_at_val += 2;
				}
			}

			at_roll_d20 = random_schick(20);

			if ((at_roll_d20 == 20) && (random_schick(20) > attacker_at_val - 9)) {
				/* critical attack failure */
#if !defined(__BORLANDC__)
				D1_INFO("Critical attack failure...");
#endif

				if (!target_is_hero || check_hero(hero)) {
					/* if enemy has been attacked, or if hero has been attacked and that hero
					 * is not asleep, dead, petrified, unconscious, renegade or fleeing */
					FIG_add_msg(3, 0);

					critical_failure_roll_2d6 = random_interval(2, 12);
#if !defined(__BORLANDC__)
					D1_INFO("rolled %d ...", critical_failure_roll_2d6);
#endif

					if ((critical_failure_roll_2d6 >= 3) && (critical_failure_roll_2d6 <= 8)) {
						/* defender gets a free attack,
						 * which cannot be parried and which cannot fail critically */
#if !defined(__BORLANDC__)
						D1_INFO("defender gets a free attack.\n");
#endif

						g_fig_critical_fail_backfire_1 = 1;

						if (random_schick(20) <= target_at_val) {

							if (target_is_hero != 0) {
								damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)p_enemy, 0);
							} else {
								damage = FIG_get_enemy_attack_damage(target_enemy, p_enemy, 1);
							}

							if (damage > 0) {

								FIG_damage_enemy(p_enemy, damage, 1);

								FIG_add_msg(11, damage);

								/* swap msg struct */
								grammar_tmp = g_fig_target_grammar;
								g_fig_target_grammar = g_fig_actor_grammar;
								g_fig_actor_grammar = grammar_tmp;

								if (p_enemy->flags.dead) {
									g_attacker_dead = 1;
								}
							}
						}
					} else if ((critical_failure_roll_2d6 >= 9) && (critical_failure_roll_2d6 <= 11)) {
						/* attacker gets 1W6 damage */
#if !defined(__BORLANDC__)
						D1_INFO("1D6 damage.\n");
#endif

						damage = random_schick(6);

						FIG_damage_enemy(p_enemy, damage, 1);

						FIG_add_msg(11, damage);

						g_fig_target_grammar = g_fig_actor_grammar;

						if (p_enemy->flags.dead) {
							g_attacker_dead = 1;
						}
					} else if (critical_failure_roll_2d6 == 12) {
#if !defined(__BORLANDC__)
						D1_INFO("nothing happens.\n");
#endif
					}
#if !defined(__BORLANDC__)
					else {
						D1_INFO("nothing happens.\n");
					}
#endif
				}
			} else {

				if (at_roll_d20 <= attacker_at_val) {

					/* check if parry is allowed */
					if ((target_is_hero && !g_fig_hero_parry_action_used[p_enemy->target_object_id - 1] && check_hero(hero)) ||
						(!target_is_hero && !g_fig_enemy_parry_action_used[p_enemy->target_object_id]))
					{

						pa_roll_d20 = random_schick(20);

						if ((pa_roll_d20 == 20) && (random_schick(20) > target_pa_val - 7)) {
							/* critical parry failure */
#if !defined(__BORLANDC__)
							D1_INFO("Critical parry failure...");
#endif

							FIG_add_msg(2, 0);

							critical_failure_roll_2d6 = random_interval(2, 12);
#if !defined(__BORLANDC__)
							D1_INFO("rolled %d ...", critical_failure_roll_2d6);
#endif

							if ((critical_failure_roll_2d6 >= 3) && (critical_failure_roll_2d6 <= 8)) {
								/* attacker gets an additional attack,
								 * which cannot be parried and which cannot fail critically */
#if !defined(__BORLANDC__)
								D1_INFO("attacker gets a free attack.\n");
#endif

								g_fig_critical_fail_backfire_2 = 1;

								if (random_schick(20) <= attacker_at_val) {

									if (target_is_hero != 0) {

										damage = FIG_get_enemy_attack_damage(p_enemy, (struct enemy_sheet*)hero, 0);

										if (damage > 0) {

											/* HESHTHOT */
											if (p_enemy->monster_id != MONSTER_ID_HESHTOT__WEAK) {
												sub_hero_le(hero, damage);
											}

											FIG_add_msg(8, damage);

											if (hero->flags.dead) {
												g_defender_dead = 1;
											}
										}
									} else {
										damage = FIG_get_enemy_attack_damage(p_enemy, target_enemy, 1);

										if (damage > 0) {

											FIG_damage_enemy(target_enemy, damage, 1);

											FIG_add_msg(11, damage);

											if (target_enemy->flags.dead) {
												g_defender_dead = 1;
											}
										}
									}
								}

							} else if ((critical_failure_roll_2d6 >= 9) && (critical_failure_roll_2d6 <= 11)) {
								/* defender gegs 1W6 damage */
#if !defined(__BORLANDC__)
								D1_INFO("1D6 damage.\n");
#endif

								damage = random_schick(6);

								if (target_is_hero != 0) {

									sub_hero_le(hero, damage);

									FIG_add_msg(8, damage);

									if (hero->flags.dead) {
										g_defender_dead = 1;
									}

								} else {

									FIG_damage_enemy(target_enemy, damage, 1);

									FIG_add_msg(11, damage);

									if (target_enemy->flags.dead) {
										g_defender_dead = 1;
									}
								}
							}
#if !defined(__BORLANDC__)
							else {
								D1_INFO("nothing happens.\n");
							}
#endif
						} else {
							if (pa_roll_d20 > target_pa_val) {
								/* defense was not successful */
								attacker_hits_target = 1;
							} else {

								FIG_add_msg(5, 0);

								if ((pa_roll_d20 == at_roll_d20) && (target_is_hero != 0) && (p_weapon_target_hero->bf != -99)) {
									/* if both random values agree and hero got attacked and his weapon is not unbreakable */
									/* now if 1W12 + BF is >= 16, weapon is broken. Otherwise, BF is increased by 1. */
#if !defined(__BORLANDC__)
									D1_INFO("weapon of defender gets damaged...");
#endif

									if (p_weapon_target_hero->bf > 3) {

										if ((random_schick(12) + p_weapon_target_hero->bf) > 15) {
#if !defined(__BORLANDC__)
											D1_INFO("broken.\n");
#endif

											p_weapon_target_hero->flags.broken = 1; /* set 'broken' flag */

											FIG_add_msg(6, 0);
										} else {
#if !defined(__BORLANDC__)
											D1_INFO("BF increased from %d -> %d.\n", p_weapon_target_hero->bf, p_weapon_target_hero->bf + 1);
#endif
											p_weapon_target_hero->bf++;
										}
									} else {
#if !defined(__BORLANDC__)
										D1_INFO("BF increased from %d -> %d.\n", p_weapon_target_hero->bf, p_weapon_target_hero->bf + 1);
#endif
										p_weapon_target_hero->bf++;
									}
								}
							}
						}
					} else {
						attacker_hits_target = 1;
					}

					if (attacker_hits_target != 0) {

						if (target_is_hero != 0) {

							damage = FIG_get_enemy_attack_damage(p_enemy, (struct enemy_sheet*)hero, 0);

							if (damage > 0) {

								if (p_enemy->monster_id != MONSTER_ID_HESHTOT__WEAK) {
									sub_hero_le(hero, damage);
								}

								FIG_add_msg(8, damage);

								if (hero->flags.dead) {
									g_defender_dead = 1;
								}
							}
						} else {

							damage = FIG_get_enemy_attack_damage(p_enemy, target_enemy, 1);

							if (damage > 0) {

								FIG_damage_enemy(target_enemy, damage, 1);

								FIG_add_msg(11, damage);

								if (target_enemy->flags.dead) {
									g_defender_dead = 1;
								}
							}
						}
					}
				}
			}

			clear_anisheets();

			if (target_is_hero != 0) {

				if (check_hero(hero) || (g_defender_dead != 0)) {

					FANI_prepare_fight_hero_ani(
						0,
						hero,
						weapon_gfx_id,
						FIG_ACTION_PARRY,
						p_enemy->target_object_id,
						enemy_id + 10,
						1
					);
				}

			} else if (target_cannot_parry == 0) {
				FANI_prepare_fight_enemy_ani(
					0,
					target_enemy,
					FIG_ACTION_PARRY,
					p_enemy->target_object_id,
					enemy_id + 10,
					1
				);
			} else if (g_defender_dead != 0) {
				FANI_prepare_fight_enemy_ani(
					0,
					target_enemy,
					FIG_ACTION_NONE,
					p_enemy->target_object_id,
					enemy_id + 10,
					1
				);
			}

			FANI_prepare_fight_enemy_ani(
				1,
				p_enemy,
				FIG_ACTION_MELEE_ATTACK,
				enemy_id + 10,
				p_enemy->target_object_id,
				0
			);
			g_fig_continue_print = 1;
			draw_fight_screen_pal(0);
			clear_anisheets();

		} else {

			if (p_enemy->action_id == FIG_ACTION_RANGE_ATTACK) {

				if (p_enemy->shots > 0) {
					weapon_gfx_id_ranged = WEAPON_GFX_ID_RANGED_MISSILE;
					p_enemy->shots--;
				} else {
					weapon_gfx_id_ranged = WEAPON_GFX_ID_RANGED_THROW;
					p_enemy->throws--;
				}

				if (target_is_hero != 0) {

					/* attack hero */

					damage = dice_template(weapon_gfx_id_ranged == WEAPON_GFX_ID_RANGED_MISSILE ? p_enemy->shot_dam : p_enemy->throw_dam);

					/* Feature mod 1: avoid the a posteriori weakening of enemies. */
#ifndef M302de_FEATURE_MOD
					damage = (damage * 8) / 10;
#endif

					/* RS */
					damage -= hero->rs_bonus;

					if (damage > 0) {

						sub_hero_le(hero, damage);

						FIG_add_msg(8, damage);

						if (hero->flags.dead) {
							g_defender_dead = 1;
						}
					}

				} else {
					/* target is an enemy */

					damage = dice_template(weapon_gfx_id_ranged == 3 ? p_enemy->shot_dam : p_enemy->throw_dam) - target_enemy->rs;

					if (damage > 0) {

						FIG_damage_enemy(target_enemy, damage, 1);

						FIG_add_msg(11, damage);

						if (target_enemy->flags.dead) {
							g_defender_dead = 1;
						}
					}
				}

				clear_anisheets();

				projectile_gfx_id = weapon_gfx_id_ranged;
				ranged_attack_nonadjacent_flag = 0;

				FIG_call_draw_pic();

				FANI_prepare_fight_enemy_ani(
					0,
					p_enemy,
					FIG_ACTION_RANGE_ATTACK,
					enemy_id + 10,
					p_enemy->target_object_id,
					0
				);

				ranged_attack_nonadjacent_flag = FANI_prepare_projectile_ani(
					7,
					projectile_gfx_id,
					enemy_id + 10,
					p_enemy->target_object_id,
					p_enemy->viewdir
				);

				FIG_set_sheet(p_enemy->fighter_id, 0);

				draw_fight_screen_pal(0);

				if (ranged_attack_nonadjacent_flag != 0) {

					FIG_set_sheet(g_fig_projectile_id, 7);

					draw_fight_screen((ranged_attack_nonadjacent_flag == 0) && (g_defender_dead == 0) ? 0 : 1);

					FIG_make_invisible(g_fig_projectile_id);
				}

				g_fig_continue_print = 1;

				if (g_defender_dead != 0) {

					if (target_is_hero != 0) {

						FANI_prepare_fight_hero_ani(
							1,
							hero,
							-1,
							FIG_ACTION_NONE,
							p_enemy->target_object_id,
							enemy_id + 10,
							1
						);
					} else {

						FANI_prepare_fight_enemy_ani(
							1,
							target_enemy,
							FIG_ACTION_NONE,
							p_enemy->target_object_id,
							enemy_id + 10,
							1
						);
					}
				}

				FANI_remove_projectile();
				draw_fight_screen(0);
				clear_anisheets();

			} else if (p_enemy->action_id == FIG_ACTION_SPELL) {

				/* spellcast */

				spellcast_ani_id = g_mon_spell_descriptions[p_enemy->mspell_id].ani_id;

				*g_dtp2 = '\0';

				spell_test_result = MON_cast_spell(p_enemy, 0);

				clear_anisheets();

				if (p_enemy->target_object_id) {

					projectile_gfx_id = ranged_attack_nonadjacent_flag = PROJECTILE_GFX_ID_SPELLCAST_ORB; // == 0

					if (random_schick(100) > 50) {
						projectile_gfx_id = PROJECTILE_GFX_ID_SPELLCAST_BOLT;
					}

					if (p_enemy->target_object_id < 10) {
						projectile_gfx_id = PROJECTILE_GFX_ID_SPELLCAST_STAR;
					}

					FIG_call_draw_pic();

					if (spell_test_result != -1) {

						FANI_prepare_spell_enemy(
							0,
							p_enemy,
							4,
							enemy_id + 10,
							p_enemy->target_object_id,
							0
						);
					}

					if (spell_test_result > 0) {

						if (spellcast_ani_id > 0) {

							FANI_prepare_enemy_spell_ani(6, p_enemy, spellcast_ani_id);

						} else {

							if (p_enemy->target_object_id > 0) {

								if (!target_is_hero) {

									FANI_prepare_spell_enemy(
										1,
										target_enemy,
										99,
										p_enemy->target_object_id,
										enemy_id + 10,
										1
									);
								} else {

									if (check_hero(hero) || (g_defender_dead != 0)) {

										FANI_prepare_spell_hero(
											1,
											hero,
											99,
											p_enemy->target_object_id,
											0,
											-1,
											1
										);
									}
								}
							}
						}

						if (
							(p_enemy->actor_sprite_id != ACTOR_SPRITE_ID_HEXE__FEMALE)
							&& (p_enemy->actor_sprite_id != ACTOR_SPRITE_ID_HEXE__MALE)
							&& (p_enemy->target_object_id > 0)
						) {

							ranged_attack_nonadjacent_flag = FANI_prepare_projectile_ani(
								7,
								projectile_gfx_id,
								enemy_id + 10,
								p_enemy->target_object_id,
								p_enemy->viewdir
							);
						}

					}
					if (spell_test_result != -1) {

						FIG_set_sheet(p_enemy->fighter_id, 0);

						draw_fight_screen_pal(1);
					}

					if (spell_test_result > 0) {

						if (ranged_attack_nonadjacent_flag != 0) {

							FIG_set_sheet(g_fig_projectile_id, 7);

							draw_fight_screen(1);

							FIG_make_invisible(g_fig_projectile_id);
						}

						if (spellcast_ani_id > 0) {
							FIG_set_sheet(g_fig_spellgfx_id, 6);
						}

						if (!target_is_hero) {

							FIG_set_sheet(target_enemy->fighter_id, 1);


							if (is_in_byte_array(target_enemy->actor_sprite_id, g_double_size_actor_sprite_id_table)) {

								fighter = FIG_get_fighter(target_enemy->fighter_id);

								FIG_set_sheet(g_fig_double_size_fighter_id_table[fighter->double_size], 3);
							}
						} else {

							if (p_enemy->target_object_id > 0) {

								FIG_set_sheet(hero->fighter_id, 1);
							}
						}

						g_fig_continue_print = 1;

						draw_fight_screen(1);

						if (spellcast_ani_id > 0) {
							FIG_make_invisible(g_fig_projectile_id);
						}

						if (g_mspell_awake_flag) {

							g_mspell_awake_flag = 0;

							FIG_remove_from_list(target_enemy->fighter_id, 1);

							g_fig_list_elem.figure = g_gfxtab_figures_main[target_enemy->actor_sprite_id][0];
							g_fig_list_elem.nvf_no = target_enemy->viewdir;
							g_fig_list_elem.offsetx = g_gfxtab_offsets_main[target_enemy->actor_sprite_id][target_enemy->viewdir].x;
							g_fig_list_elem.offsety = g_gfxtab_offsets_main[target_enemy->actor_sprite_id][target_enemy->viewdir].y;

							if (is_in_byte_array(target_enemy->actor_sprite_id, g_double_size_actor_sprite_id_table)) {

								g_fig_list_elem.x1 = g_gfxtab_double_size_x1[target_enemy->viewdir];
								g_fig_list_elem.x2 = g_gfxtab_double_size_x2[target_enemy->viewdir];
							} else {
								g_fig_list_elem.x1 = 0;
								g_fig_list_elem.x2 = 31;
								g_fig_list_elem.double_size = -1;
							}

							g_fig_list_elem.y1 = 0;
							g_fig_list_elem.y2 = 39;
							g_fig_list_elem.height = 40;
							g_fig_list_elem.width = 32;
							g_fig_list_elem.is_enemy = 1;
							g_fig_list_elem.reload = -1;
							g_fig_list_elem.wsheet = -1;
							g_fig_list_elem.sheet = -1;

							FIG_add_to_list(target_enemy->fighter_id);
						}

						if (g_spell_illusionen) {

							if (p_enemy->target_object_id >= 10) {

								FIG_make_invisible(target_enemy->fighter_id);

								if (is_in_byte_array(target_enemy->actor_sprite_id, g_double_size_actor_sprite_id_table)) {

									fighter = FIG_get_fighter(target_enemy->fighter_id);

									FIG_make_invisible(g_fig_double_size_fighter_id_table[fighter->double_size]);
								}
							} else {

								if (p_enemy->target_object_id > 0) {

									FIG_make_invisible(hero->fighter_id);
								}
							}
						}

						if (ranged_attack_nonadjacent_flag != 0) {
							FANI_remove_projectile();
						}

						if (spellcast_ani_id > 0) {
							FANI_remove_spell();
						}

						FIG_draw_figures();
					}


					FIG_output(g_dtp2);

					clear_anisheets();
				} else {
					FIG_output(g_dtp2);
				}
			}
		}

	}

	call_mouse();
}

/**
 * \brief   *
 *
 * \param   hero        pointer to the hero who uses the item
 * \param   target_enemy pointer to the enemy
 * \param   target_hero pointer to the hero
 * \param   flag        bool value, used when a hero attacks a hero
 * \param   hero_pos    position of the hero
 */
void FIG_use_item(struct struct_hero *hero, struct enemy_sheet *target_enemy, struct struct_hero *target_hero, const signed int flag, const signed int hero_pos)
{
	signed int damage;

	signed int l3;
	signed int hylailic = 0;
	signed int usecase;
	signed int item_id = hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id;
	struct item_stats *p_item = &g_itemsdat[item_id];

	if (p_item->flags.herb_potion) {
		usecase = 1;
	} else if (!p_item->flags.usable || (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].quantity == 0)) {
		usecase = 0;
	} else {
		usecase = 2;
	}

	*g_dtp2 = '\0';

	if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id == ITEM_ID_MIASTHMATICUM) {
		/* MIASTHMATIC */

		/* 1W6 + 4 */
		damage = dice_roll(1, 6, 4);

		if (hero->target_object_id >= 10) {

			strcpy(g_dtp2, get_tx(37));

			FIG_damage_enemy(target_enemy, damage, 0);

			FIG_add_msg(11, damage);

			if (target_enemy->flags.dead) {
				g_defender_dead = 1;
			}
		} else {

			if (flag != 0) {

				strcpy(g_dtp2, get_tx(37));

				sub_hero_le(target_hero, damage);

				FIG_add_msg(8, damage);

				if (target_hero->flags.dead) {
					g_defender_dead = 1;
				}
			}
		}

		/* drop the item in the left hand */
		drop_item(hero, HERO_INVENTORY_SLOT_LEFT_HAND, 1);

	} else if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id == ITEM_ID_HYLAILER_FEUER) {

		/* HYLAILIC FIRE */

		if (hero->target_object_id >= 10) {

			/* .. used on an enemy */

			FIG_damage_enemy(target_enemy, 20, 0);

			FIG_add_msg(11, 20);

			if (target_enemy->flags.dead) {
				g_defender_dead = 1;
			}
		} else {
			/* .. used on another hero */
			if (flag != 0) {

				sub_hero_le(target_hero, 20);

				FIG_add_msg(8, 20);

				if (target_hero->flags.dead) {
					g_defender_dead = 1;
				}
			}
		}

		/* drop the item in the left hand */
		drop_item(hero, HERO_INVENTORY_SLOT_LEFT_HAND, 1);

		hylailic = 1;
	} else {
		/* use item in the regular way */

		use_item(HERO_INVENTORY_SLOT_LEFT_HAND, hero_pos);

		*g_dtp2 = '\0';
	}

	if (usecase > 0) {

		clear_anisheets();

		FANI_prepare_fight_hero_ani(
			0,
			hero,
			-1,
			usecase == 1 ? FIG_ACTION_UNKNOWN3 : FIG_ACTION_UNKNOWN4,
			hero_pos + 1,
			hero->target_object_id,
			0
		);

		l3 = 0;

		if (hylailic != 0) {
			FANI_prepare_hero_spell_ani(6, hero, 2);
		} else {
			g_fig_continue_print = 1;
		}

		draw_fight_screen_pal(0);

		if (hylailic != 0) {

			FIG_set_sheet(g_fig_spellgfx_id, 6);

			l3 = 1;

			g_fig_continue_print = 1;

			draw_fight_screen(1);
		}

		if (l3 != 0) {
			FANI_remove_spell();
		}

		if (g_defender_dead != 0) {

			if (flag != 0) {
				FANI_prepare_fight_hero_ani(
					1,
					target_hero,
					-1,
					FIG_ACTION_NONE,
					hero->target_object_id,
					hero_pos + 1,
					1
				);
			} else {
				FANI_prepare_fight_enemy_ani(
					1,
					target_enemy,
					FIG_ACTION_NONE,
					hero->target_object_id,
					hero_pos + 1,
					1
				);
			}

		}

		if ((l3 != 0) || (g_defender_dead != 0)) {
			draw_fight_screen(0);
		}

		clear_anisheets();
	}

	if (*g_dtp2) {
		/* show output string if needed */
		GUI_output(g_dtp2);
	}
}
