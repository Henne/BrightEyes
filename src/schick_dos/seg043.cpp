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
	struct inventory *p_weapon;
	signed int two_w_6;
	signed int weapon_type;
	signed int defender_gets_hit;
	signed int randval;
	signed int randval2;
	signed int attacker_at;
	signed int defender_at;
	signed int defender_pa;
	signed char target_is_hero;
	signed int l11;
	signed int l12;
	signed int l13;
	signed int l14;
	signed int l15;
	struct struct_fighter *fighter;
	signed int l17 = 0;
	signed int fighter_id;
	signed int hero_x;
	signed int hero_y;
	signed int target_x;
	signed int target_y;
	signed int dir;
	struct viewdir_offsets8s dst = g_fig_viewdir_inverse_offsets3;
	struct struct_msg tmp;

	call_mouse_bg();

	if (g_in_fight) {

		FIG_clear_msgs();

		defender_gets_hit = g_fig_critical_fail_backfire_2 =
			g_fig_critical_fail_backfire_1 = g_attacker_dead = g_defender_dead = 0;

		g_fig_actor_grammar.type = 1;
		g_fig_actor_grammar.id = p_enemy->monster_id;

		if (p_enemy->target_object_id < 10) {

			/* enemy attacks hero */
			hero = get_hero(p_enemy->target_object_id - 1);

			g_fig_target_grammar.type = 2;
			g_fig_target_grammar.id = p_enemy->target_object_id - 1;

			if (hero->flags.dead || !hero->typus) {
				return;
			}

			target_is_hero = 1;
		} else {
			/* enemy attacks enemy */

			target_enemy = &g_enemy_sheets[p_enemy->target_object_id - 10];

			g_fig_target_grammar.type = 1;
			g_fig_target_grammar.id = target_enemy->monster_id;

			if (target_enemy->flags.dead || !target_enemy->monster_id) {
				return;
			}

			target_is_hero = 0;

			if ((is_in_byte_array(target_enemy->gfx_id, g_double_size_gfx_id_table)) && (l17 == 0))
			{
				FIG_search_obj_on_cb(p_enemy->target_object_id, &target_x, &target_y);
				FIG_search_obj_on_cb(enemy_id + 10, &hero_x, &hero_y);

				if (hero_x == target_x) {

					if (target_y < hero_y) {
						dir = 3;
					} else {
						dir = 1;
					}
				} else {
					if (target_x < hero_x) {
						dir = 0;
					} else {
						dir = 2;
					}
				}

				if (target_enemy->viewdir != dir) {

					fighter_id = get_cb_val(hero_x + dst.offset[dir].x, hero_y + dst.offset[dir].y);

					if (fighter_id != 0) {

						if ((fighter_id >= 50) ||
							((fighter_id < 10) && !get_hero(fighter_id - 1)->flags.dead) ||
							((fighter_id >= 10) && (fighter_id < 30) && !g_enemy_sheets[fighter_id - 10].flags.dead) ||
							((fighter_id >= 30) && (fighter_id < 50) && !g_enemy_sheets[fighter_id - 30].flags.dead))
						{
							l17 = 1;
						}
					}
				}

			}
		}

		if (p_enemy->action_id == FIG_ACTION_MELEE_ATTACK) {

			if (p_enemy->target_object_id < 10) {

				/* attack a hero */

				p_weapon = &hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND];

				weapon_type = weapon_check(hero);

				if (weapon_type == -1) {
					/* no valid weapon == bare hands */
					defender_at = hero->at_talent_bonus[WEAPON_TYPE_WAFFENLOS] + hero->fight_atpa_mod;
					defender_pa = hero->pa_talent_bonus[WEAPON_TYPE_WAFFENLOS] - hero->fight_atpa_mod;
				} else {
					defender_at = hero->at_talent_bonus[hero->weapon_type] + hero->fight_atpa_mod + hero->weapon_at_mod;
					defender_pa = hero->pa_talent_bonus[hero->weapon_type] - hero->fight_atpa_mod + hero->weapon_pa_mod;
				}

				/* guarding heroes get a PA-bonus of 3 */
				if (hero->action_id == FIG_ACTION_GUARD) {
					defender_pa += 3;
				}

				/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get a PA-bonus against orcs */
				if (gs_tevent071_orcstatue && (hero->typus == HERO_TYPE_ZWERG) && (p_enemy->gfx_id == 24))
				{
					defender_pa++;
				}

				/* subtract RS handicap */
				defender_at -= hero->rs_be / 2;
				defender_pa -= hero->rs_be / 2;

			} else {
				/* target is an enemy */
				defender_at = target_enemy->at;
				defender_pa = target_enemy->pa;
			}

			/* spell_dunkelheit() is active => AT-4, PA-4*/
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				defender_at -= 4;
				defender_pa -= 4;
			}

			attacker_at = p_enemy->at;
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				attacker_at -= 4;
			}

			if (target_is_hero) {

				/* target hero has already parried another atack => AT+2 */
				if (g_fig_hero_parry_action_used[p_enemy->target_object_id - 1] == 1) {
					attacker_at += 2;
				}

				/* 'Chamaelioni' spell is active on the target hero => AT-5 */
				if (hero->flags.chamaelioni == 1) {
					attacker_at -= 5;
				}

				/* 'Duplicatus' spell is active on the target hero => AT/2 */
				if (hero->flags.duplicatus == 1) {
					attacker_at /= 2;
				}
			} else {
				/* target enemy has already parried another atack => AT+2 */
				if (g_fig_enemy_parry_action_used[p_enemy->target_object_id] == 1) {
					attacker_at += 2;
				}
			}

			randval = random_schick(20);

			if ((randval == 20) && (random_schick(20) > attacker_at - 9)) {
				/* critical attack failure */
#if !defined(__BORLANDC__)
				D1_INFO("Critical attack failure...");
#endif

				if (!target_is_hero || check_hero(hero)) {
					/* if enemy has been attacked, or if hero has been attacked and that hero
					 * is not asleep, dead, petrified, unconscious, renegade or fleeing */
					FIG_add_msg(3, 0);

					two_w_6 = random_interval(2, 12);
#if !defined(__BORLANDC__)
					D1_INFO("rolled %d ...", two_w_6);
#endif

					if ((two_w_6 >= 3) && (two_w_6 <= 8)) {
						/* defender gets a free attack,
						 * which cannot be parried and which cannot fail critically */
#if !defined(__BORLANDC__)
						D1_INFO("defender gets a free attack.\n");
#endif

						g_fig_critical_fail_backfire_1 = 1;

						if (random_schick(20) <= defender_at) {

							if (target_is_hero != 0) {
								damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)p_enemy, 0);
							} else {
								damage = FIG_get_enemy_attack_damage(target_enemy, p_enemy, 1);
							}

							if (damage > 0) {

								FIG_damage_enemy(p_enemy, damage, 1);

								FIG_add_msg(11, damage);

								/* swap msg struct */
								tmp = g_fig_target_grammar;
								g_fig_target_grammar = g_fig_actor_grammar;
								g_fig_actor_grammar = tmp;

								if (p_enemy->flags.dead) {
									g_attacker_dead = 1;
								}
							}
						}
					} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {
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
					} else if (two_w_6 == 12) {
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

				if (randval <= attacker_at) {

					/* check if parry is allowed */
					if ((target_is_hero && !g_fig_hero_parry_action_used[p_enemy->target_object_id - 1] && check_hero(hero)) ||
						(!target_is_hero && !g_fig_enemy_parry_action_used[p_enemy->target_object_id]))
					{

						randval2 = random_schick(20);

						if ((randval2 == 20) && (random_schick(20) > defender_pa - 7)) {
							/* critical parry failure */
#if !defined(__BORLANDC__)
							D1_INFO("Critical parry failure...");
#endif

							FIG_add_msg(2, 0);

							two_w_6 = random_interval(2, 12);
#if !defined(__BORLANDC__)
							D1_INFO("rolled %d ...", two_w_6);
#endif

							if ((two_w_6 >= 3) && (two_w_6 <= 8)) {
								/* attacker gets an additional attack,
								 * which cannot be parried and which cannot fail critically */
#if !defined(__BORLANDC__)
								D1_INFO("attacker gets a free attack.\n");
#endif

								g_fig_critical_fail_backfire_2 = 1;

								if (random_schick(20) <= attacker_at) {

									if (target_is_hero != 0) {

										damage = FIG_get_enemy_attack_damage(p_enemy, (struct enemy_sheet*)hero, 0);

										if (damage > 0) {

											/* HESHTHOT */
											if (p_enemy->monster_id != MONSTER_ID_HESHTOT__3) {
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

							} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {
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
							if (randval2 > defender_pa) {
								/* defense was not successful */
								defender_gets_hit = 1;
							} else {

								FIG_add_msg(5, 0);

								if ((randval2 == randval) && (target_is_hero != 0) && (p_weapon->bf != -99)) {
									/* if both random values agree and hero got attacked and his weapon is not unbreakable */
									/* now if 1W12 + BF is >= 16, weapon is broken. Otherwise, BF is increased by 1. */
#if !defined(__BORLANDC__)
									D1_INFO("weapon of defender gets damaged...");
#endif

									if (p_weapon->bf > 3) {

										if ((random_schick(12) + p_weapon->bf) > 15) {
#if !defined(__BORLANDC__)
											D1_INFO("broken.\n");
#endif

											p_weapon->flags.broken = 1; /* set 'broken' flag */

											FIG_add_msg(6, 0);
										} else {
#if !defined(__BORLANDC__)
											D1_INFO("BF increased from %d -> %d.\n", p_weapon->bf, p_weapon->bf + 1);
#endif
											p_weapon->bf++;
										}
									} else {
#if !defined(__BORLANDC__)
										D1_INFO("BF increased from %d -> %d.\n", p_weapon->bf, p_weapon->bf + 1);
#endif
										p_weapon->bf++;
									}
								}
							}
						}
					} else {
						defender_gets_hit = 1;
					}

					if (defender_gets_hit != 0) {

						if (target_is_hero != 0) {

							damage = FIG_get_enemy_attack_damage(p_enemy, (struct enemy_sheet*)hero, 0);

							if (damage > 0) {

								if (p_enemy->monster_id != MONSTER_ID_HESHTOT__3) {
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

					FANI_prepare_fight_hero_ani(0, hero, weapon_type, FIG_ACTION_PARRY, p_enemy->target_object_id, enemy_id + 10, 1);
				}

			} else if (l17 == 0) {
				FANI_prepare_fight_enemy_ani(0, target_enemy, FIG_ACTION_PARRY, p_enemy->target_object_id, enemy_id + 10, 1);
			} else if (g_defender_dead != 0) {
				FANI_prepare_fight_enemy_ani(0, target_enemy, FIG_ACTION_NONE, p_enemy->target_object_id, enemy_id + 10, 1);
			}

			FANI_prepare_fight_enemy_ani(1, p_enemy, FIG_ACTION_MELEE_ATTACK, enemy_id + 10, p_enemy->target_object_id, 0);
			g_fig_continue_print = 1;
			draw_fight_screen_pal(0);
			clear_anisheets();

		} else {

			if (p_enemy->action_id == FIG_ACTION_RANGE_ATTACK) {

				if (p_enemy->shots > 0) {
					l15 = 3;
					p_enemy->shots--;
				} else {
					l15 = 4;
					p_enemy->throws--;
				}

				if (target_is_hero != 0) {

					/* attack hero */

					damage = dice_template(l15 == 3 ? p_enemy->shot_dam : p_enemy->throw_dam);

					damage = (damage * 8) / 10;

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

					damage = dice_template(l15 == 3 ? p_enemy->shot_dam : p_enemy->throw_dam) - target_enemy->rs;

					if (damage > 0) {

						FIG_damage_enemy(target_enemy, damage, 1);

						FIG_add_msg(11, damage);

						if (target_enemy->flags.dead) {
							g_defender_dead = 1;
						}
					}
				}

				clear_anisheets();

				l11 = l15;
				l12 = 0;

				FIG_call_draw_pic();

				FANI_prepare_fight_enemy_ani(0, p_enemy, FIG_ACTION_RANGE_ATTACK, enemy_id + 10, p_enemy->target_object_id, 0);

				l12 = FANI_prepare_shotbolt_ani(7, l11, enemy_id + 10, p_enemy->target_object_id, p_enemy->viewdir);

				FIG_set_sheet(p_enemy->fighter_id, 0);

				draw_fight_screen_pal(0);

				if (l12 != 0) {

					FIG_set_sheet(g_fig_shot_bolt_id, 7);

					draw_fight_screen((l12 == 0) && (g_defender_dead == 0) ? 0 : 1);

					FIG_make_invisible(g_fig_shot_bolt_id);
				}

				g_fig_continue_print = 1;

				if (g_defender_dead != 0) {

					if (target_is_hero != 0) {

						FANI_prepare_fight_hero_ani(1, hero, -1, FIG_ACTION_NONE, p_enemy->target_object_id, enemy_id + 10, 1);
					} else {

						FANI_prepare_fight_enemy_ani(1, target_enemy, FIG_ACTION_NONE, p_enemy->target_object_id, enemy_id + 10, 1);
					}
				}

				FANI_remove_shotbolt();
				draw_fight_screen(0);
				clear_anisheets();

			} else if (p_enemy->action_id == FIG_ACTION_SPELL) {

				/* spellcast */

				l14 = g_mon_spell_descriptions[p_enemy->mspell_id].ani_id;

				*g_dtp2 = '\0';

				l13 = MON_cast_spell(p_enemy, 0);

				clear_anisheets();

				if (p_enemy->target_object_id) {

					l11 = l12 = 0;

					if (random_schick(100) > 50) {
						l11 = 1;
					}

					if (p_enemy->target_object_id < 10) {
						l11 = 2;
					}

					FIG_call_draw_pic();

					if (l13 != -1) {

						FANI_prepare_spell_enemy(0, p_enemy, 4, enemy_id + 10, p_enemy->target_object_id, 0);
					}

					if (l13 > 0) {

						if (l14 > 0) {

							FANI_prepare_enemy_spell_ani(6, p_enemy, l14);

						} else {

							if (p_enemy->target_object_id > 0) {

								if (!target_is_hero) {

									FANI_prepare_spell_enemy(1, target_enemy, 99, p_enemy->target_object_id, enemy_id + 10, 1);
								} else {

									if (check_hero(hero) || (g_defender_dead != 0)) {

										FANI_prepare_spell_hero(1, hero, 99, p_enemy->target_object_id, 0, -1, 1);
									}
								}
							}
						}

						if ((p_enemy->gfx_id != 0x12) && (p_enemy->gfx_id != 7) && (p_enemy->target_object_id > 0)) {

							l12 = FANI_prepare_shotbolt_ani(7, l11, enemy_id + 10, p_enemy->target_object_id, p_enemy->viewdir);
						}

					}
					if (l13 != -1) {

						FIG_set_sheet(p_enemy->fighter_id, 0);

						draw_fight_screen_pal(1);
					}

					if (l13 > 0) {

						if (l12 != 0) {

							FIG_set_sheet(g_fig_shot_bolt_id, 7);

							draw_fight_screen(1);

							FIG_make_invisible(g_fig_shot_bolt_id);
						}

						if (l14 > 0) {
							FIG_set_sheet(g_fig_spellgfx_id, 6);
						}

						if (!target_is_hero) {

							FIG_set_sheet(target_enemy->fighter_id, 1);


							if (is_in_byte_array(target_enemy->gfx_id, g_double_size_gfx_id_table)) {

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

						if (l14 > 0) {
							FIG_make_invisible(g_fig_shot_bolt_id);
						}

						if (g_mspell_awake_flag) {

							g_mspell_awake_flag = 0;

							FIG_remove_from_list(target_enemy->fighter_id, 1);

							g_fig_list_elem.figure = g_gfxtab_figures_main[target_enemy->gfx_id][0];
							g_fig_list_elem.nvf_no = target_enemy->viewdir;
							g_fig_list_elem.offsetx = g_gfxtab_offsets_main[target_enemy->gfx_id][target_enemy->viewdir].x;
							g_fig_list_elem.offsety = g_gfxtab_offsets_main[target_enemy->gfx_id][target_enemy->viewdir].y;

							if (is_in_byte_array(target_enemy->gfx_id, g_double_size_gfx_id_table)) {

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

								if (is_in_byte_array(target_enemy->gfx_id, g_double_size_gfx_id_table)) {

									fighter = FIG_get_fighter(target_enemy->fighter_id);

									FIG_make_invisible(g_fig_double_size_fighter_id_table[fighter->double_size]);
								}
							} else {

								if (p_enemy->target_object_id > 0) {

									FIG_make_invisible(hero->fighter_id);
								}
							}
						}

						if (l12 != 0) {
							FANI_remove_shotbolt();
						}

						if (l14 > 0) {
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

	if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id == ITEM_MIASTHMATICUM) {
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

	} else if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id == ITEM_HYLAILER_FEUER) {

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

		FANI_prepare_fight_hero_ani(0, hero, -1, usecase == 1 ? FIG_ACTION_UNKNOWN3 : FIG_ACTION_UNKNOWN4, hero_pos + 1, hero->target_object_id, 0);

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
				FANI_prepare_fight_hero_ani(1, target_hero, -1, FIG_ACTION_NONE, hero->target_object_id, hero_pos + 1, 1);
			} else {
				FANI_prepare_fight_enemy_ani(1, target_enemy, FIG_ACTION_NONE, hero->target_object_id, hero_pos + 1, 1);
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
