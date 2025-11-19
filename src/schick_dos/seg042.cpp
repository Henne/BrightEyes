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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

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
 * \param   hero_pos    position in the group (fighter_id = hero_pos + 1)
 */
void FIG_do_hero_action(struct struct_hero* hero, const signed int hero_pos)
{
	signed int damage;
	struct enemy_sheet *target_monster;
	struct struct_hero *target_hero;
	struct inventory *p_weapon;
	struct inventory *p_weapon_target;
	signed int weapon_type;
	signed int weapon_type_target;
	signed int two_w_6;
	signed int l5;
	signed int l6;
	signed int randval1;
	signed int randval2;
	signed int atpa;
	signed int l10;
	signed int l11;
	signed int l12;
	signed int l13;
	signed int target_is_hero = 0;
	signed int l15;
	struct struct_fighter *fighter_add;
	signed int width;
	signed int height;
	signed int l16 = 0;
	signed int l17 = 0;
	signed int fighter_id;
	struct viewdir_offsets8s dst = g_fig_viewdir_inverse_offsets2;
	signed int hero_x;
	signed int hero_y;
	signed int target_x;
	signed int target_y;
	signed int viewdir;
	struct struct_fighter *fighter;
	struct struct_msg tmp;
	signed int fg_bak;
	signed int bg_bak;
	struct nvf_extract_desc nvf;

	call_mouse_bg();

	if (g_in_fight) {

		FIG_clear_msgs();

		l5 = g_fig_critical_fail_backfire_2 = g_fig_critical_fail_backfire_1 =
			g_spell_illusionen = g_attacker_dead = g_defender_dead = 0;

		weapon_type = weapon_type_target = -1;

		g_fig_actor_grammar.type = 2;
		g_fig_actor_grammar.id = hero_pos;

		if (hero->target_object_id >= 10) {

			/* attack monster */
			if (hero->target_object_id >= 30) {
				/* tail part of double-size enemy */
				hero->target_object_id -= 20;
				l16 = 1;
				l17 = 1;
			}

			target_monster = &g_enemy_sheets[hero->target_object_id - 10];

			/* attacked enemy won't be asleep any more */
			target_monster->flags.asleep = 0;

			g_fig_target_grammar.type = 1;
			g_fig_target_grammar.id = target_monster->mon_id;

			if (!target_monster->mon_id || (target_monster->flags.dead && ((hero->action_id != FIG_ACTION_SPELL) || (hero->spell_id != SP_SKELETTARIUS_KRYPTADUFT))))
			{
				call_mouse();
				return;
			}

			if ((is_in_byte_array(target_monster->gfx_id, g_double_size_gfx_id_table)) && (l16 == 0))
			{
				FIG_search_obj_on_cb(hero->target_object_id, &target_x, &target_y);
				FIG_search_obj_on_cb(hero_pos + 1, &hero_x, &hero_y);

				/* In the following, viewdir is pointing from the target to the attacker */
				if (hero_x == target_x) {
					if (target_y < hero_y) {
						viewdir = FIG_VIEWDIR_UP;
					} else {
						viewdir = FIG_VIEWDIR_DOWN;
					}
				} else {
					if (target_x < hero_x) {
						viewdir = FIG_VIEWDIR_RIGHT;
					} else {
						viewdir = FIG_VIEWDIR_LEFT;
					}
				}

				if (target_monster->viewdir != viewdir) {

					fighter_id = get_cb_val(hero_x + dst.offset[viewdir].x, hero_y + dst.offset[viewdir].y);

					if (fighter_id != 0) {

						if ((fighter_id >= 50) ||
							((fighter_id < 10) && !get_hero(fighter_id - 1)->flags.dead) ||
							((fighter_id >= 10) && (fighter_id < 30) && !g_enemy_sheets[fighter_id - 10].flags.dead) ||
							((fighter_id >= 30) && (fighter_id < 50) && !g_enemy_sheets[fighter_id - 30].flags.dead))
						{
							l16 = 1;
						}
					}
				}
			}
		} else {
			if (hero->target_object_id > 0) {

				/* hero attacks another hero */
				target_hero = get_hero(hero->target_object_id - 1);

				g_fig_target_grammar.type = 2;
				g_fig_target_grammar.id = hero->target_object_id - 1;

				if (target_hero->flags.asleep) {

					/* wake up target hero */
					target_hero->flags.asleep = 0;

					fighter = FIG_get_fighter(target_hero->fighter_id);

					fighter->nvf_no = target_hero->viewdir;
					fighter->reload = -1;
					fighter->offsetx = 0;
					fighter->offsety = 0;
				}

				if (target_hero->flags.dead || !target_hero->typus) {
					call_mouse();
					return;
				}

				target_is_hero = 1;
			}
		}

		if (hero->action_id == FIG_ACTION_MELEE_ATTACK) {

			p_weapon = &hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND];

			weapon_type = weapon_check(hero);

			if (target_is_hero != 0) {
				p_weapon_target = &target_hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND];

				weapon_type_target = weapon_check(target_hero);
			}

			if (weapon_type == -1) {
				/* no valid weapon == bare hands */
				atpa = hero->at_talent_bonus[WEAPON_TYPE_WAFFENLOS] + hero->fight_atpa_mod - hero->rs_be / 2;
			} else {
				atpa = hero->at_talent_bonus[hero->weapon_type] + hero->fight_atpa_mod + hero->weapon_at_mod - hero->rs_be / 2;
			}

			if (hero->rs_be & 1) {
				/* if RS_BE is odd, subtract another point. Changes the rounding behavior of 'hero->rs_be / 2' above to "round up". */
				atpa--;
			}

			/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get an AT-bonus against orcs */
			if (gs_tevent071_orcstatue &&
				(hero->typus == HERO_TYPE_ZWERG) &&
				!target_is_hero &&
				(target_monster->gfx_id == 24))
			{
				atpa++;
			}

			/* spell_dunkelheit is active */
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				atpa -= 4;
			}

			if (target_is_hero) {

				if (weapon_type_target == -1) {
					l11 = target_hero->at_talent_bonus[WEAPON_TYPE_WAFFENLOS] + target_hero->fight_atpa_mod - hero->rs_be / 2;
				} else {
					l11 = target_hero->at_talent_bonus[target_hero->weapon_type] + target_hero->fight_atpa_mod - hero->rs_be / 2;
				}

				if (hero->rs_be & 1) {
					l11--;
				}

				/* spell_dunkelheit is active */
				if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
					l11 -= 4;
				}

				if (g_hero_is_target[hero->target_object_id - 1] == 1) {
					atpa += 2;
				}

			} else {
				if ((g_fig_actors_unkn[hero->target_object_id] == 1) || (l16 != 0)) {
					atpa += 2;
				}
			}

			randval1 = random_schick(20);

			if ((randval1 == 20) && (random_schick(20) > atpa - 9)) {
				/* critical attack failure */

				damage = 0;
				FIG_add_msg(1, 0);

				two_w_6 = random_interval(2, 12);

				if ((two_w_6 == 2) && (weapon_type != -1) && (p_weapon->bf != -99))
				{
					/* weapon broken */
					p_weapon->flags.broken = 1;
					FIG_add_msg(6, 0);

				} else if ((two_w_6 >= 3) && (two_w_6 <= 8) && (l16 == 0)) {
					/* defender gets a free attack */

					g_fig_critical_fail_backfire_1 = 1;

					if (target_is_hero != 0) {

						if (random_schick(20) <= l11) {
							damage = FIG_get_hero_weapon_attack_damage(target_hero, hero, 1);
						}

					} else {

						if (random_schick(20) <= target_monster->at) {

							damage = FIG_get_enemy_attack_damage(target_monster, (struct enemy_sheet*)hero, 0);
						}
					}

					if (damage > 0) {

						sub_hero_le(hero, damage);

						FIG_add_msg(8, damage);

						/* swap msg struct */
						tmp = g_fig_target_grammar;
						g_fig_target_grammar = g_fig_actor_grammar;
						g_fig_actor_grammar = tmp;
					}

					if (hero->flags.dead) {
						g_attacker_dead = 1;
					}

				} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {
					/* attacker hurts himself. 1W6 damage. */

					damage = random_schick(6);

					sub_hero_le(hero, damage);

					FIG_add_msg(8, damage);

					g_fig_target_grammar = g_fig_actor_grammar;

					if (hero->flags.dead) {
						g_attacker_dead = 1;
					}
				}

			} else if (randval1 <= atpa) {

				if (((target_is_hero == 0) && !g_fig_actors_unkn[hero->target_object_id] && (l16 == 0)) ||
					((target_is_hero) && !g_hero_is_target[hero->target_object_id - 1]))
				{

					if (target_is_hero != 0) {

						if (weapon_type_target == -1) {
							l10 = target_hero->pa_talent_bonus[WEAPON_TYPE_WAFFENLOS] - target_hero->fight_atpa_mod - target_hero->rs_be / 2;
						} else {
							l10 = target_hero->pa_talent_bonus[target_hero->weapon_type] - target_hero->fight_atpa_mod - target_hero->rs_be / 2 + target_hero->weapon_at_mod;
						}

						if (target_hero->action_id == FIG_ACTION_GUARD) {
							l10 += 3;
						}
					} else {

						l10 = target_monster->pa;

						if (target_monster->blind) {
							l10 -= 5;
						}

						if (target_monster->flags.tied) { /* check 'tied' flag */
							l10 -= 2;
						} else if (target_monster->flags.dancing) { /* check 'dancing' flag */
							l10 -= 3;
						}

						if (target_monster->flags.petrified || /* check 'petrified' flag */
							target_monster->flags.busy || /* check 'busy' flag */
							target_monster->flags.mushroom || /* check 'mushroom' flag */
							target_monster->flags.tame || /* check 'tame' flag */
							target_monster->flags.renegade) /* check 'renegade' flag */
						{
							l10 = 0;
						}
					}

					/* spell_dunkelheit is active */
					if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
						l10 -= 4;
					}

					if (l10 < 0) {
						l10 = 0;
					}

					randval2 = random_schick(20);

					if ((randval2 == 20) && (random_schick(20) > l10 - 7)) {
						/* critical defense failure */

						damage = 0;

						FIG_add_msg(4, 0);

						two_w_6 = random_interval(2, 12);

						if ((two_w_6 >= 3) && (two_w_6 <= 8)) {

							g_fig_critical_fail_backfire_1 = 1;
							/* Maybe the intention was:
							 * g_fig_critical_fail_backfire_2 = 1
							 * See comment at the declaration of g_fig_critical_fail_backfire_2. */

							if (random_schick(20) <= atpa) {

								if (target_is_hero != 0) {

									damage = FIG_get_hero_weapon_attack_damage(hero, target_hero, 1);

									if (damage > 0) {

										sub_hero_le(target_hero, damage);

										FIG_add_msg(8, damage);

										if (target_hero->flags.dead) {
											g_defender_dead = 1;
										}
									}
								} else {

									damage = FIG_get_hero_weapon_attack_damage(hero, (struct struct_hero*)target_monster, 0);

									if (damage > 0) {

										FIG_damage_enemy(target_monster, damage, 0);

										FIG_add_msg(11, damage);

										if (target_monster->flags.dead) {
											g_defender_dead = 1;
										}
									}
								}
							}
						} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {

							damage = random_schick(6);

							if (target_is_hero != 0) {

								sub_hero_le(target_hero, damage);

								FIG_add_msg(8, damage);

								if (target_hero->flags.dead) {
									g_defender_dead = 1;
								}
							} else {
								FIG_damage_enemy(target_monster, damage, 1);

								FIG_add_msg(11, damage);

								if (target_monster->flags.dead) {
									g_defender_dead = 1;
								}
							}
						}
					} else {

						if (randval2 > l10) {
							l5 = 1;
						} else {
							FIG_add_msg(5, 0);

							if ((randval2 == randval1) && (p_weapon->bf != -99)) {

								if (p_weapon->bf > 3) {

									if ((random_schick(12) + p_weapon->bf) > 15) {

										if ((target_is_hero != 0) && (p_weapon_target->bf != -99))
										{
											p_weapon_target->flags.broken = 1;
										}

										p_weapon->flags.broken = 1; /* set 'broken' flag */

										FIG_add_msg(6, 0);

									} else {
										if (target_is_hero != 0) {
											p_weapon_target->bf++;
										}

										p_weapon->bf++;
									}
								} else {

									if (target_is_hero != 0) {
										p_weapon_target->bf++;
									}

									p_weapon->bf++;
								}
							}
						}
					}
				} else {
					l5 = 1;
				}

				if (l5 != 0) {

					if (target_is_hero != 0) {

						damage = FIG_get_hero_weapon_attack_damage(hero, target_hero, 1);

						if (damage > 0) {

							sub_hero_le(target_hero, damage);

							FIG_add_msg(8, damage);

							if (target_hero->flags.dead) {
								g_defender_dead = 1;
							}
						}
					} else {

						damage = FIG_get_hero_weapon_attack_damage(hero, (struct struct_hero*)target_monster, 0);

						if (damage > 0 ) {

							FIG_damage_enemy(target_monster, damage, 0);

							FIG_add_msg(11, damage);

							if (target_monster->flags.dead) {
								g_defender_dead = 1;
							}
						}
					}
				}
			}

			clear_anisheets();

			FANI_prepare_fight_hero_ani(0, hero, weapon_type, 2, hero_pos + 1,
							l17 == 0 ? hero->target_object_id : hero->target_object_id + 20, 0);

			if (target_is_hero != 0) {

				if (check_hero(target_hero) || (g_defender_dead != 0)) {

					FANI_prepare_fight_hero_ani(1, target_hero, weapon_type_target,
								FIG_ACTION_PARRY, hero->target_object_id, hero_pos + 1, 1);
				}
			} else {

				if (l16 == 0) {
					FANI_prepare_fight_enemy_ani(1, target_monster, FIG_ACTION_PARRY, hero->target_object_id, hero_pos + 1, 1);
				} else {
					if (g_defender_dead != 0) {
						FANI_prepare_fight_enemy_ani(1, target_monster, 0, hero->target_object_id, hero_pos + 1, 1);
					}
				}
			}

			g_fig_continue_print = 1;

			draw_fight_screen_pal(0);

			clear_anisheets();

		} else {

			if (hero->action_id == FIG_ACTION_RANGE_ATTACK) {

				weapon_type = FIG_get_range_weapon_type(hero);

				if (weapon_type != -1) {

#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 32: throwing weapon in a ranged attack is dropped before the damage logic is done.
					 * As result, all throwing weapons (except throwing stars in a pile of at least 2 units) will be treated
					 * as weaponless fighting for the damage calculation.
					 * This means that they will always hit (without a skill test) and will do only 1D6 damage.
					 * found 2016-04-02 by NRS at https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=146051#pid146051
					 * "Oh mein Gott..." */
					if (!range_attack_check_ammo(hero, 0)) {
						return;
					}
#endif

					if (target_is_hero != 0) {

						/* note that for ranged attacks, the skill test will be done in the following function call. */
						damage = FIG_get_hero_weapon_attack_damage(hero, target_hero, 1);
#ifdef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 32:
						 * Fix: move the function call after the damage calculation.
						 */
						if (!range_attack_check_ammo(hero, 0)) {
							return;
						}
#endif

						if (damage > 0) {

							sub_hero_le(target_hero, damage);

							FIG_add_msg(8, damage);

							if (target_hero->flags.dead) {
								g_defender_dead = 1;
							}
						}
					} else {

						/* note that for ranged attacks, the skill test will be done in the following function call. */
						damage = FIG_get_hero_weapon_attack_damage(hero, (struct struct_hero*)target_monster, 0);
#ifdef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 32:
						 * Fix: move the function call after the damage calculation.
						 */
						if (!range_attack_check_ammo(hero, 0)) {
							return;
						}
#endif

						if (damage > 0 ) {

							FIG_damage_enemy(target_monster, damage, 0);

							FIG_add_msg(11, damage);

							if (target_monster->flags.dead) {
								g_defender_dead = 1;
							}
						}
					}

					clear_anisheets();

					l12 = weapon_type;
					l13 = 0;

					FIG_call_draw_pic();

					FANI_prepare_fight_hero_ani(0, hero, weapon_type,
							15, hero_pos + 1,
							l17 == 0 ? hero->target_object_id : hero->target_object_id + 20, 0);

					l13 = FANI_prepare_shotbolt_ani(7, l12, hero_pos + 1,
						l17 == 0 ? hero->target_object_id : hero->target_object_id + 20,
						hero->viewdir);

					FIG_set_sheet(hero->fighter_id, 0);

					draw_fight_screen_pal(0);

					if (FIG_get_range_weapon_type(hero) == -1) {

						fighter = FIG_get_fighter(hero->fighter_id);

						fighter->nvf_no = hero->viewdir;
						fighter->reload = -1;
					}

					if (l13 != 0) {

						FIG_set_sheet(g_fig_shot_bolt_id, 7);

						draw_fight_screen(l13 == 0 && g_defender_dead == 0 ? 0 : 1);

						FIG_make_invisible(g_fig_shot_bolt_id);
					}

					g_fig_continue_print = 1;

					if (g_defender_dead != 0) {

						if (target_is_hero != 0) {
							FANI_prepare_fight_hero_ani(1, target_hero, -1, 0, hero->target_object_id, hero_pos + 1, 1);
						} else {
							FANI_prepare_fight_enemy_ani(1, target_monster, 0, hero->target_object_id, hero_pos + 1, 1);
						}
					}

					FANI_remove_shotbolt();

					draw_fight_screen(0);

					clear_anisheets();
				}

			} else if (hero->action_id == FIG_ACTION_SPELL) {

				/* cast a spell */

				if (g_current_fight_no == FIGHTS_F144) {
					/* no spells allowed in the final fight */

					sub_hero_le(hero, hero->le + 1);

					g_attacker_dead = 1;
				}

				l6 = g_spell_descriptions[hero->spell_id].ani;

				*g_dtp2 = '\0';

				l15 = use_spell(hero, 0, 0);

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

					l12 = l13 = 0;

					if (random_schick(100) > 50) {
						l12 = 1;
					}

					if (hero->target_object_id < 10) {
						l12 = 2;
					}

					FIG_call_draw_pic();

					if (l15 != -1) {

						FANI_prepare_spell_hero(0, hero, 4, hero_pos + 1,
								l17 == 0 ? hero->target_object_id : hero->target_object_id + 20, l12, 0);
					}

					if (l15 > 0) {

						if (l6 > 0) {

							if (l6 != 4) {
								FANI_prepare_hero_spell_ani(6, hero, l6);
							}

						} else {

							if (hero->target_object_id > 0) {

								if (target_is_hero == 0) {

									FANI_prepare_spell_enemy(1, target_monster, 99,
											l17 == 0 ? hero->target_object_id : hero->target_object_id + 20,
											hero_pos + 1, 1);
								} else {

									if (check_hero(target_hero) || (g_defender_dead != 0)) {

										FANI_prepare_spell_hero(1, target_hero, 99, hero->target_object_id, 0 , -1, 1);
									}
								}
							}
						}

						if ((hero->sprite_id != 7) &&
							(hero->sprite_id != 18) &&
							(hero->target_object_id > 0 ))
						{

							l13 = FANI_prepare_shotbolt_ani(7, l12, hero_pos + 1,
								l17 == 0 ? hero->target_object_id : hero->target_object_id + 20,
								hero->viewdir);
						}
					}

					if (l15 != -1) {

						FIG_set_sheet(hero->fighter_id, 0);
						draw_fight_screen_pal(1);
					}

					if (l15 > 0) {

						if (l13 != 0) {

							FIG_set_sheet(g_fig_shot_bolt_id, 7);

							draw_fight_screen(1);

							FIG_make_invisible(g_fig_shot_bolt_id);
						}

						if (l6 > 0) {

							if (l6 != 4) {
								FIG_set_sheet(g_fig_spellgfx_id, 6);
							} else {

								FIG_call_draw_pic();

								FIG_remove_from_list(target_monster->fighter_id, 1);


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

								FIG_add_to_list(target_monster->fighter_id);

							}
						} else {

							if (target_is_hero == 0) {

								FIG_set_sheet(target_monster->fighter_id, 1);

								if (is_in_byte_array(target_monster->gfx_id, g_double_size_gfx_id_table))
								{
									fighter_add = FIG_get_fighter(target_monster->fighter_id);

									FIG_set_sheet(g_fig_double_size_fighter_id_table[fighter_add->double_size], 3);
								}

							} else {

								if (hero->target_object_id > 0) {

									FIG_set_sheet(target_hero->fighter_id, 1);
								}
							}
						}

						g_fig_continue_print = 1;

						draw_fight_screen(1);

						if (l6 > 0) {
							FIG_make_invisible(g_fig_shot_bolt_id);
						}

						if (g_spell_illusionen) {

							if (hero->target_object_id >= 10) {

								FIG_make_invisible(target_monster->fighter_id);

								if (is_in_byte_array(target_monster->gfx_id, g_double_size_gfx_id_table))
								{
									fighter_add = FIG_get_fighter(target_monster->fighter_id);

									FIG_make_invisible(g_fig_double_size_fighter_id_table[fighter_add->double_size]);
								}
							} else {
								if (hero->target_object_id > 0) {

									FIG_make_invisible(target_hero->fighter_id);
								}
							}
						}

						if (l13 != 0) {
							FANI_remove_shotbolt();
						}

						if ((l6 > 0) && (l6 != 3) && (l6 != 4)) {
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

				FIG_use_item(hero, target_monster, target_hero, target_is_hero, hero_pos);
			}
		}
	}

	call_mouse();

	if (l17 != 0) {
		hero->target_object_id += 20;
	}
}

#if !defined(__BORLANDC__)
}
#endif
