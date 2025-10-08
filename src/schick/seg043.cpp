/**
 *	Rewrite of DSA1 v3.02_de functions of seg043 (fightsystem: monster action, use item)
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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct coords {
	signed char x, y;
};

struct dummy {
	struct coords a[4];
};

/**
 * \brief   execute the fight action of a monster
 *
 * \param   monster     pointer to a monster datasheet
 * \param   monster_pos position of the monster (fighter_id = monster_pos + 10)
 */
void FIG_do_enemy_action(struct enemy_sheet* monster, signed short monster_pos)
{
	signed short damage;

	struct struct_hero *hero;
	struct enemy_sheet *target_enemy;
	struct inventory *p_weapon;
	signed short two_w_6;
	signed short weapon_type;
	signed short defender_gets_hit;
	signed short randval;
	signed short randval2;
	signed short attacker_at;
	signed short defender_at;
	signed short defender_pa;
	signed char target_is_hero;
	signed short l11;
	signed short l12;
	signed short l13;
	signed short l14;
	signed short l15;
	struct struct_fighter *fighter;
	signed short l17 = 0;
	signed short fighter_id;
	signed short hero_x;
	signed short hero_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	struct viewdir_offsets8s dst = g_viewdir_invoffsets3;
	struct struct_msg tmp;

	update_mouse_cursor();

	if (g_in_fight) {

		FIG_clear_msgs();

		defender_gets_hit = g_attacker_attacks_again =
			g_defender_attacks = g_attacker_dead = g_defender_dead = 0;

		g_fig_actor_grammar.type = 1;
		g_fig_actor_grammar.id = monster->mon_id;

		if (monster->enemy_id < 10) {

			/* monster attacks hero */
			hero = (struct struct_hero*)get_hero(monster->enemy_id - 1);

			g_fig_target_grammar.type = 2;
			g_fig_target_grammar.id = monster->enemy_id - 1;

			if (hero_dead((Bit8u*)hero) || !hero->typus) {
				return;
			}

			target_is_hero = 1;
		} else {
			/* monster attacks monster */

			target_enemy = &g_enemy_sheets[monster->enemy_id - 10];

			g_fig_target_grammar.type = 1;
			g_fig_target_grammar.id = target_enemy->mon_id;

			if (target_enemy->flags.dead || !target_enemy->mon_id) {
				return;
			}

			target_is_hero = 0;

			if ((is_in_byte_array(target_enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) && (l17 == 0))
			{
				FIG_search_obj_on_cb(monster->enemy_id, &target_x, &target_y);
				FIG_search_obj_on_cb(monster_pos + 10, &hero_x, &hero_y);

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

					fighter_id = get_cb_val(hero_x + dst.a[dir].x, hero_y + dst.a[dir].y);

					if (fighter_id != 0) {

						if ((fighter_id >= 50) ||
							((fighter_id < 10) && !hero_dead((Bit8u*)get_hero(fighter_id - 1))) ||
							((fighter_id >= 10) && (fighter_id < 30) && !g_enemy_sheets[fighter_id - 10].flags.dead) ||
							((fighter_id >= 30) && (fighter_id < 50) && !g_enemy_sheets[fighter_id - 30].flags.dead))
						{
							l17 = 1;
						}
					}
				}

			}
		}

		if (monster->action_id == FIG_ACTION_MELEE_ATTACK) {

			if (monster->enemy_id < 10) {

				/* attack a hero */

				p_weapon = &hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND];

				weapon_type = weapon_check(hero);

				if (weapon_type == -1) {
					/* no valid weapon == bare hands */
					defender_at = hero->at_weapon[WEAPON_TYPE_WAFFENLOS] + hero->atpa_mod;
					defender_pa = hero->pa_weapon[WEAPON_TYPE_WAFFENLOS] - hero->atpa_mod;
				} else {
					defender_at = hero->at_weapon[hero->w_type] + hero->atpa_mod + hero->w_at_mod;
					defender_pa = hero->pa_weapon[hero->w_type] - hero->atpa_mod + hero->w_pa_mod;
				}

				/* guarding heroes get a PA-bonus of 3 */
				if (hero->action_id == FIG_ACTION_GUARD) {
					defender_pa += 3;
				}

				/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get a PA-bonus against orcs */
				if (gs_tevent071_orcstatue && (hero->typus == HERO_TYPE_DWARF) && (monster->gfx_id == 24))
				{
					defender_pa++;
				}

				/* subtract RS handycap */
				defender_at -= hero->rs_be / 2;
				defender_pa -= hero->rs_be / 2;

			} else {
				/* attack a monster */
				defender_at = target_enemy->at;
				defender_pa = target_enemy->pa;
			}

			/* spell_dunkelheit() is active => AT-4, PA-4*/
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				defender_at -= 4;
				defender_pa -= 4;
			}

			attacker_at = monster->at;
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				attacker_at -= 4;
			}

			if (target_is_hero) {

				/* TODO */
				if (g_hero_is_target[monster->enemy_id - 1] == 1) {
					attacker_at += 2;
				}

				/* 'Chamaelioni' spell is active on the target hero => AT-5 */
				if (hero_chamaelioni((Bit8u*)hero) == 1) {
					attacker_at -= 5;
				}

				/* 'Duplicatus' spell is active on the target hero => AT/2 */
				if (hero_duplicatus((Bit8u*)hero) == 1) {
					attacker_at /= 2;
				}
			} else {
				/* TODO */
				if (g_fig_actors_unkn[monster->enemy_id] == 1) {
					attacker_at += 2;
				}
			}

			randval = random_schick(20);

			if ((randval == 20) && (random_schick(20) > attacker_at - 9)) {
				/* critical attack failure */
#if !defined(__BORLANDC__)
				D1_INFO("Critical attack failure...");
#endif

				if (!target_is_hero || check_hero((Bit8u*)hero)) {
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

						g_defender_attacks = 1;

						if (random_schick(20) <= defender_at) {

							if (target_is_hero != 0) {
								damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)monster, 0);
							} else {
								damage = FIG_get_enemy_attack_damage(target_enemy, monster, 1);
							}

							if (damage > 0) {

								FIG_damage_enemy(monster, damage, 1);

								FIG_add_msg(11, damage);

								/* swap msg struct */
								tmp = g_fig_target_grammar;
								g_fig_target_grammar = g_fig_actor_grammar;
								g_fig_actor_grammar = tmp;

								if (monster->flags.dead) {
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

						FIG_damage_enemy(monster, damage, 1);

						FIG_add_msg(11, damage);

						g_fig_target_grammar = g_fig_actor_grammar;

						if (monster->flags.dead) {
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

					if ((target_is_hero && !g_hero_is_target[monster->enemy_id - 1] && check_hero((Bit8u*)hero)) ||
						(!target_is_hero && !g_fig_actors_unkn[monster->enemy_id]))
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

								g_attacker_attacks_again = 1;

								if (random_schick(20) <= attacker_at) {

									if (target_is_hero != 0) {

										damage = FIG_get_enemy_attack_damage(monster, (struct enemy_sheet*)hero, 0);

										if (damage > 0) {

											/* HESHTHOT */
											if (monster->mon_id != 0x4d) {
												sub_hero_le(hero, damage);
											}

											FIG_add_msg(8, damage);

											if (hero_dead((Bit8u*)hero)) {
												g_defender_dead = 1;
											}
										}
									} else {
										damage = FIG_get_enemy_attack_damage(monster, target_enemy, 1);

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

									if (hero_dead((Bit8u*)hero)) {
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

							damage = FIG_get_enemy_attack_damage(monster, (struct enemy_sheet*)hero, 0);

							if (damage > 0) {

								if (monster->mon_id != 0x4d) {
									sub_hero_le(hero, damage);
								}

								FIG_add_msg(8, damage);

								if (hero_dead((Bit8u*)hero)) {
									g_defender_dead = 1;
								}
							}
						} else {

							damage = FIG_get_enemy_attack_damage(monster, target_enemy, 1);

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

				if (check_hero((Bit8u*)hero) || (g_defender_dead != 0)) {

					FIG_prepare_hero_fight_ani(0, hero, weapon_type, 100, monster->enemy_id, monster_pos + 10, 1);
				}

			} else if (l17 == 0) {
				FIG_prepare_enemy_fight_ani(0, target_enemy, 100, monster->enemy_id, monster_pos + 10, 1);
			} else if (g_defender_dead != 0) {
				FIG_prepare_enemy_fight_ani(0, target_enemy, 0, monster->enemy_id, monster_pos + 10, 1);
			}

			FIG_prepare_enemy_fight_ani(1, monster, 2, monster_pos + 10, monster->enemy_id, 0);
			g_fig_continue_print = 1;
			draw_fight_screen_pal(0);
			clear_anisheets();

		} else {

			if (monster->action_id == FIG_ACTION_RANGE_ATTACK) {

				if (monster->shots > 0) {
					l15 = 3;
					monster->shots--;
				} else {
					l15 = 4;
					monster->throws--;
				}

				if (target_is_hero != 0) {

					/* attack hero */

					damage = dice_template(l15 == 3 ? monster->shot_dam : monster->throw_dam);

					damage = (damage * 8) / 10;

					/* RS */
					damage -= hero->rs_bonus1;

					if (damage > 0) {

						sub_hero_le(hero, damage);

						FIG_add_msg(8, damage);

						if (hero_dead((Bit8u*)hero)) {
							g_defender_dead = 1;
						}
					}

				} else {

					/* attack monster */
					damage = dice_template(l15 == 3 ? monster->shot_dam : monster->throw_dam) - target_enemy->rs;

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

				FIG_prepare_enemy_fight_ani(0, monster, 15, monster_pos + 10, monster->enemy_id, 0);

				l12 = seg045_01a0(7, l11, monster_pos + 10, monster->enemy_id, monster->viewdir);

				FIG_set_sheet(monster->fighter_id, 0);

				draw_fight_screen_pal(0);

				if (l12 != 0) {

					FIG_set_sheet(g_fig_shot_bolt_id, 7);

					draw_fight_screen((l12 == 0) && (g_defender_dead == 0) ? 0 : 1);

					FIG_make_invisible(g_fig_shot_bolt_id);
				}

				g_fig_continue_print = 1;

				if (g_defender_dead != 0) {

					if (target_is_hero != 0) {

						FIG_prepare_hero_fight_ani(1, (struct struct_hero*)hero, -1, 0, monster->enemy_id, monster_pos + 10, 1);
					} else {

						FIG_prepare_enemy_fight_ani(1, target_enemy, 0, monster->enemy_id, monster_pos + 10, 1);
					}
				}

				FIG_remove_smth();
				draw_fight_screen(0);
				clear_anisheets();

			} else if (monster->action_id == FIG_ACTION_SPELL) {

				/* spellcast */

				l14 = g_mon_spell_descriptions[monster->cur_spell].ani_id;

				*g_dtp2 = '\0';

				l13 = MON_cast_spell(monster, 0);

				clear_anisheets();

				if (monster->enemy_id) {

					l11 = l12 = 0;

					if (random_schick(100) > 50) {
						l11 = 1;
					}

					if (monster->enemy_id < 10) {
						l11 = 2;
					}

					FIG_call_draw_pic();

					if (l13 != -1) {

						seg044_002f(0, monster, 4, monster_pos + 10, monster->enemy_id, 0);
					}

					if (l13 > 0) {

						if (l14 > 0) {

							seg045_041b(6, monster, l14);

						} else {

							if (monster->enemy_id > 0) {

								if (!target_is_hero) {

									seg044_002f(1, target_enemy, 99, monster->enemy_id, monster_pos + 10, 1);
								} else {

									if (check_hero((Bit8u*)hero) || (g_defender_dead != 0)) {

										seg044_002a(1, hero, 99, monster->enemy_id, 0, -1, 1);
									}
								}
							}
						}

						if ((monster->gfx_id != 0x12) && (monster->gfx_id != 7) && (monster->enemy_id > 0)) {

							l12 = seg045_01a0(7, l11, monster_pos + 10, monster->enemy_id, monster->viewdir);
						}

					}
					if (l13 != -1) {

						FIG_set_sheet(monster->fighter_id, 0);

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


							if (is_in_byte_array(target_enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {

								fighter = FIG_get_fighter(target_enemy->fighter_id);

								FIG_set_sheet(g_fig_twofielded_table[fighter->twofielded], 3);
							}
						} else {

							if (monster->enemy_id > 0) {

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

							if (is_in_byte_array(target_enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {

								g_fig_list_elem.x1 = g_gfxtab_twofielded_x1[target_enemy->viewdir];
								g_fig_list_elem.x2 = g_gfxtab_twofielded_x2[target_enemy->viewdir];
							} else {
								g_fig_list_elem.x1 = 0;
								g_fig_list_elem.x2 = 31;
								g_fig_list_elem.twofielded = -1;
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

							if (monster->enemy_id >= 10) {

								FIG_make_invisible(target_enemy->fighter_id);

								if (is_in_byte_array(target_enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {

									fighter = FIG_get_fighter(target_enemy->fighter_id);

									FIG_make_invisible(g_fig_twofielded_table[fighter->twofielded]);
								}
							} else {

								if (monster->enemy_id > 0) {

									FIG_make_invisible(hero->fighter_id);
								}
							}
						}

						if (l12 != 0) {
							FIG_remove_smth();
						}

						if (l14 > 0) {
							FIG_remove_smth2();
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

	refresh_screen_size();
}

/**
 * \brief   *
 *
 * \param   hero        pointer to the hero who uses the item
 * \param   target_monster pointer to the monster
 * \param   target_hero pointer to the hero
 * \param   flag        bool value, used when a hero attacks a hero
 * \param   hero_pos    position of the hero
 */
void FIG_use_item(struct struct_hero *hero, struct enemy_sheet *target_monster, struct struct_hero *target_hero, signed short flag, signed short hero_pos)
{
	signed short damage;

	signed short l3;
	signed short hylailic = 0;
	signed short usecase;
	signed short item_id = hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id;
	struct item_stats *p_item = (struct item_stats*)get_itemsdat(item_id);

	if (item_herb_potion((Bit8u*)p_item)) {
		usecase = 1;
	} else if (!item_useable((Bit8u*)p_item) || (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].quantity == 0)) {
		usecase = 0;
	} else {
		usecase = 2;
	}

	*g_dtp2 = '\0';

	if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id == ITEM_MIASTHMATICUM) {
		/* MIASTHMATIC */

		/* 1W6 + 4 */
		damage = dice_roll(1, 6, 4);

		if (hero->enemy_id >= 10) {

			strcpy(g_dtp2, get_tx(37));

			FIG_damage_enemy(target_monster, damage, 0);

			FIG_add_msg(11, damage);

			if (target_monster->flags.dead) {
				g_defender_dead = 1;
			}
		} else {

			if (flag != 0) {

				strcpy(g_dtp2, get_tx(37));

				sub_hero_le(target_hero, damage);

				FIG_add_msg(8, damage);

				if (hero_dead((Bit8u*)target_hero)) {
					g_defender_dead = 1;
				}
			}
		}

		/* drop the item in the left hand */
		drop_item(hero, HERO_INVENTORY_SLOT_LEFT_HAND, 1);

	} else if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id == ITEM_HYLAILIC_FIRE) {

		/* HYLAILIC FIRE */

		if (hero->enemy_id >= 10) {

			/* .. used on a monster */

			FIG_damage_enemy(target_monster, 20, 0);

			FIG_add_msg(11, 20);

			if (target_monster->flags.dead) {
				g_defender_dead = 1;
			}
		} else {
			/* .. used on another hero */
			if (flag != 0) {

				sub_hero_le(target_hero, 20);

				FIG_add_msg(8, 20);

				if (hero_dead((Bit8u*)target_hero)) {
					g_defender_dead = 1;
				}
			}
		}

		/* drop the item in the left hand */
		drop_item(hero, HERO_INVENTORY_SLOT_LEFT_HAND, 1);

		hylailic = 1;
	} else {
		/* use item in the regular way */

		use_item(4, hero_pos);

		*g_dtp2 = '\0';
	}

	if (usecase > 0) {

		clear_anisheets();

		FIG_prepare_hero_fight_ani(0, (struct struct_hero*)hero, -1, usecase == 1 ? FIG_ACTION_UNKNOWN3 : FIG_ACTION_UNKNOWN4, hero_pos + 1, hero->enemy_id, 0);

		l3 = 0;

		if (hylailic != 0) {
			seg045_0394(6, hero, 2);
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
			FIG_remove_smth2();
		}

		if (g_defender_dead != 0) {

			if (flag != 0) {
				FIG_prepare_hero_fight_ani(1, (struct struct_hero*)target_hero, -1, 0, hero->enemy_id, hero_pos + 1, 1);
			} else {
				FIG_prepare_enemy_fight_ani(1, target_monster, 0, hero->enemy_id, hero_pos + 1, 1);
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

#if !defined(__BORLANDC__)
}
#endif
