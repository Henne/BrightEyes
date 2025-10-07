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

struct coords {
	signed char x,y;
};

struct dummy {
	struct coords a[4];
};

signed short g_spell_illusionen;	// ds:0xe3a4, 1 = spell has effect
signed short g_defender_dead;		// ds:0xe3a6
signed short g_attacker_dead;		// ds:0xe3a8
signed short g_defender_attacks;	// ds:0xe3aa
signed short g_attacker_attacks_again;	// ds:0xe3ac

/**
 * \brief   executes the fight action of hero
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position in the group (fighter_id = hero_pos + 1)
 */
void FIG_do_hero_action(Bit8u* hero, const signed short hero_pos)
{
	signed short damage;
	struct enemy_sheet *target_monster;
	Bit8u *target_hero;
	Bit8u *p_weapon;
	Bit8u *p_weapon_target;
	signed short weapon_type;
	signed short weapon_type_target;
	signed short two_w_6;
	signed short l5;
	signed short l6;
	signed short randval1;
	signed short randval2;
	signed short atpa;
	signed short l10;
	signed short l11;
	signed short l12;
	signed short l13;
	signed short target_is_hero = 0;
	signed short l15;
	struct struct_fighter *fighter_add;
	signed short width;
	signed short height;
	signed short l16 = 0;
	signed short l17 = 0;
	signed short fighter_id;
	struct viewdir_offsets8s dst = g_viewdir_invoffsets2;
	signed short hero_x;
	signed short hero_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	struct struct_fighter *fighter;
	struct struct_msg tmp;
	signed short fg_bak;
	signed short bg_bak;
	struct nvf_desc nvf;

	update_mouse_cursor();

	if (g_in_fight) {

		FIG_clear_msgs();

		l5 = g_attacker_attacks_again = g_defender_attacks =
			g_spell_illusionen = g_attacker_dead = g_defender_dead = 0;

		weapon_type = weapon_type_target = -1;

		g_fig_actor_grammar.type = 2;
		g_fig_actor_grammar.id = hero_pos;

		if (host_readbs(hero + HERO_ENEMY_ID) >= 10) {

			/* attack monster */
			if (host_readbs(hero + HERO_ENEMY_ID) >= 30) {
				/* tail part of two-fielded enemy */
				sub_ptr_bs(hero + HERO_ENEMY_ID, 20);
				l16 = 1;
				l17 = 1;
			}

			target_monster = &g_enemy_sheets[host_readbs(hero + HERO_ENEMY_ID) - 10];

			/* attacked enemy won't be asleep any more */
			target_monster->flags.asleep = 0;

			g_fig_target_grammar.type = 1;
			g_fig_target_grammar.id = target_monster->mon_id;

			if (!target_monster->mon_id || (target_monster->flags.dead && ((host_readbs(hero + HERO_ACTION_ID) != FIG_ACTION_SPELL) || (host_readbs(hero + HERO_SPELL_ID) != SP_SKELETTARIUS_KRYPTADUFT))))
			{
				refresh_screen_size();
				return;
			}

			if ((is_in_byte_array(target_monster->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) && (l16 == 0))
			{
				FIG_search_obj_on_cb(host_readbs(hero + HERO_ENEMY_ID), &target_x, &target_y);
				FIG_search_obj_on_cb(hero_pos + 1, &hero_x, &hero_y);

#if !defined(__BORLANDC__)
				/* BE-fix */
				target_x = host_readws((Bit8u*)&target_x);
				target_y = host_readws((Bit8u*)&target_y);
				hero_x = host_readws((Bit8u*)&hero_x);
				hero_y = host_readws((Bit8u*)&hero_y);
#endif
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

				if (target_monster->viewdir != dir) {

					fighter_id = get_cb_val(hero_x + dst.a[dir].x, hero_y + dst.a[dir].y);

					if (fighter_id != 0) {

						if ((fighter_id >= 50) ||
							((fighter_id < 10) && !hero_dead(get_hero(fighter_id - 1))) ||
							((fighter_id >= 10) && (fighter_id < 30) && !g_enemy_sheets[fighter_id - 10].flags.dead) ||
							((fighter_id >= 30) && (fighter_id < 50) && !g_enemy_sheets[fighter_id - 30].flags.dead))
						{
							l16 = 1;
						}
					}
				}
			}
		} else {
			if (host_readbs(hero + HERO_ENEMY_ID) > 0) {

				/* hero attacks another hero */
				target_hero = get_hero(host_readbs(hero + HERO_ENEMY_ID) - 1);

				g_fig_target_grammar.type = 2;
				g_fig_target_grammar.id = host_readbs(hero + HERO_ENEMY_ID) - 1;

				if (hero_asleep(target_hero)) {

					/* wake up target hero */

					and_ptr_bs(target_hero + HERO_FLAGS1, 0xfd); /* unset 'asleep' flag */

					fighter = FIG_get_fighter(host_readbs(target_hero + HERO_FIGHTER_ID));

					fighter->nvf_no = host_readbs(target_hero + HERO_VIEWDIR);
					fighter->reload = -1;
					fighter->offsetx = 0;
					fighter->offsety = 0;
				}

				if (hero_dead(target_hero) || !host_readbs(target_hero + HERO_TYPE)) {
					refresh_screen_size();
					return;
				}

				target_is_hero = 1;
			}
		}

		if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_MELEE_ATTACK) {

			/* attack a hero */

			p_weapon = hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY;

			weapon_type = weapon_check((struct struct_hero*)hero);

			if (target_is_hero != 0) {
				p_weapon_target = target_hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY;

				weapon_type_target = weapon_check((struct struct_hero*)target_hero);
			}

			if (weapon_type == -1) {
				/* no valid weapon == bare hands */
				atpa = host_readbs(hero + HERO_AT + WEAPON_TYPE_WAFFENLOS) + host_readbs(hero + HERO_ATTACK_TYPE) - host_readbs(hero + HERO_RS_BE) / 2;
			} else {
				atpa = host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WEAPON_TYPE)) + host_readbs(hero + HERO_ATTACK_TYPE) + host_readbs(hero + HERO_AT_MOD) - host_readbs(hero + HERO_RS_BE) / 2;
			}

			if (host_readbs(hero + HERO_RS_BE) & 1) {
				/* if RS_BE is odd, subtract another point. Changes the rounding behavior of 'host_readbs(hero + HERO_RS_BE) / 2' above to "round up". */
				atpa--;
			}

			/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get a PA-bonus against orcs */
			if (gs_tevent071_orcstatue &&
				(host_readbs(hero + HERO_TYPE) == HERO_TYPE_DWARF) &&
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
					l11 = host_readbs(target_hero + HERO_AT) + host_readbs(target_hero + HERO_ATTACK_TYPE) - host_readbs(hero + HERO_RS_BE) / 2;
				} else {
					l11 = host_readbs(target_hero + HERO_AT + host_readbs(target_hero + HERO_WEAPON_TYPE)) + host_readbs(target_hero + HERO_ATTACK_TYPE) - host_readbs(hero + HERO_RS_BE) / 2;
				}

				if (host_readbs(hero + HERO_RS_BE) & 1) {
					l11--;
				}

				/* spell_dunkelheit is active */
				if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
					l11 -= 4;
				}

				if (g_hero_is_target[host_readbs(hero + HERO_ENEMY_ID) - 1] == 1) {
					atpa += 2;
				}

			} else {
				if ((g_fig_actors_unkn[host_readbs(hero + HERO_ENEMY_ID)] == 1) || (l16 != 0)) {
					atpa += 2;
				}
			}

			randval1 = random_schick(20);

			if ((randval1 == 20) && (random_schick(20) > atpa - 9)) {
				/* critical attack failure */

				damage = 0;
				FIG_add_msg(1, 0);

				two_w_6 = random_interval(2, 12);

				if ((two_w_6 == 2) && (weapon_type != -1) &&
					host_readbs(p_weapon + INVENTORY_BF) != -99)
				{
					/* weapon broken */
					or_ptr_bs(p_weapon + INVENTORY_FLAGS, 1); /* set 'broken' flag */
					FIG_add_msg(6, 0);

				} else if ((two_w_6 >= 3) && (two_w_6 <= 8) && (l16 == 0)) {
					/* defender gets a free attack */

					g_defender_attacks = 1;

					if (target_is_hero != 0) {

						if (random_schick(20) <= l11) {
							damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)target_hero, (struct struct_hero*)hero, 1);
						}

					} else {

						if (random_schick(20) <= target_monster->at) {

							damage = FIG_get_enemy_attack_damage(target_monster, (struct enemy_sheet*)hero, 0);
						}
					}

					if (damage > 0) {

						sub_hero_le((struct struct_hero*)hero, damage);

						FIG_add_msg(8, damage);

						/* swap msg struct */
						tmp = g_fig_target_grammar;
						g_fig_target_grammar = g_fig_actor_grammar;
						g_fig_actor_grammar = tmp;
					}

					if (hero_dead(hero)) {
						g_attacker_dead = 1;
					}

				} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {
					/* attacker hurts himself. 1W6 damage. */

					damage = random_schick(6);

					sub_hero_le((struct struct_hero*)hero, damage);

					FIG_add_msg(8, damage);

					g_fig_target_grammar = g_fig_actor_grammar;

					if (hero_dead(hero)) {
						g_attacker_dead = 1;
					}
				}

			} else if (randval1 <= atpa) {

				if (((target_is_hero == 0) && !g_fig_actors_unkn[host_readbs(hero + HERO_ENEMY_ID)] && (l16 == 0)) ||
					((target_is_hero) && !g_hero_is_target[host_readbs(hero + HERO_ENEMY_ID) - 1]))
				{

					if (target_is_hero != 0) {

						if (weapon_type_target == -1) {
							l10 = host_readbs(target_hero + HERO_PA) - host_readbs(target_hero + HERO_ATTACK_TYPE) - host_readbs(target_hero + HERO_RS_BE) / 2;
						} else {
							l10 = host_readbs(target_hero + HERO_PA + host_readbs(target_hero + HERO_WEAPON_TYPE)) -host_readbs(target_hero + HERO_ATTACK_TYPE) - host_readbs(target_hero + HERO_RS_BE) / 2 + host_readbs(target_hero + HERO_AT_MOD);
						}

						if (host_readbs(target_hero + HERO_ACTION_ID) == FIG_ACTION_GUARD) {
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

							g_defender_attacks = 1;

							if (random_schick(20) <= atpa) {

								if (target_is_hero != 0) {

									damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)target_hero, 1);

									if (damage > 0) {

										sub_hero_le((struct struct_hero*)target_hero, damage);

										FIG_add_msg(8, damage);

										if (hero_dead(target_hero)) {
											g_defender_dead = 1;
										}
									}
								} else {

									damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)target_monster, 0);

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

								sub_hero_le((struct struct_hero*)target_hero, damage);

								FIG_add_msg(8, damage);

								if (hero_dead(target_hero)) {
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

							if ((randval2 == randval1) && (host_readbs(p_weapon + INVENTORY_BF) != -99)) {

								if (host_readbs(p_weapon + INVENTORY_BF) > 3) {

									if (random_schick(12) + host_readbs(p_weapon + INVENTORY_BF) > 15) {

										if ((target_is_hero != 0) && (host_readbs(p_weapon_target + INVENTORY_BF) != -99))
										{
											or_ptr_bs(p_weapon_target + 0x04, 1);
										}

										or_ptr_bs(p_weapon + INVENTORY_FLAGS, 1); /* set 'broken' flag */

										FIG_add_msg(6, 0);

									} else {
										if (target_is_hero != 0) {
											inc_ptr_bs(p_weapon_target + 6);
										}

										inc_ptr_bs(p_weapon + INVENTORY_BF);
									}
								} else {

									if (target_is_hero != 0) {
										inc_ptr_bs(p_weapon_target + 0x06);
									}

									inc_ptr_bs(p_weapon + INVENTORY_BF);
								}
							}
						}
					}
				} else {
					l5 = 1;
				}

				if (l5 != 0) {

					if (target_is_hero != 0) {

						damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)target_hero, 1);

						if (damage > 0) {

							sub_hero_le((struct struct_hero*)target_hero, damage);

							FIG_add_msg(8, damage);

							if (hero_dead(target_hero)) {
								g_defender_dead = 1;
							}
						}
					} else {

						damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)target_monster, 0);

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

			FIG_prepare_hero_fight_ani(0, (struct struct_hero*)hero, weapon_type, 2, hero_pos + 1,
							l17 == 0 ? host_readbs(hero + HERO_ENEMY_ID) : host_readbs(hero + HERO_ENEMY_ID) + 20, 0);

			if (target_is_hero != 0) {

				if (check_hero(target_hero) || (g_defender_dead != 0)) {

					FIG_prepare_hero_fight_ani(1, (struct struct_hero*)target_hero, weapon_type_target,
								100, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
				}
			} else {

				if (l16 == 0) {
					FIG_prepare_enemy_fight_ani(1, target_monster, 100, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
				} else {
					if (g_defender_dead != 0) {
						FIG_prepare_enemy_fight_ani(1, target_monster, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
					}
				}
			}

			g_fig_continue_print = 1;

			draw_fight_screen_pal(0);

			clear_anisheets();

		} else {

			if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK) {

				weapon_type = FIG_get_range_weapon_type(hero);

				if (weapon_type != -1) {

#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 32: throwing weapon in a ranged attack is dropped before the damage logic is done.
					 * As result, all throwing weapons (except throwing stars in a pile of at least 2 units) will be treated
					 * as weaponless fighting for the damage calculation.
					 * This means that they will always hit (without a skill test) and will do only 1D6 damage.
					 * found 2016-04-02 by NRS at https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=146051#pid146051
					 * "Oh mein Gott..." */
					if (!range_attack_check_ammo((struct struct_hero*)hero, 0)) {
						return;
					}
#endif

					if (target_is_hero != 0) {

						/* note that for ranged attacks, the skill test will be done in the following function call. */
						damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)target_hero, 1);
#ifdef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 32:
						 * Fix: move the function call after the damage calculation.
						 */
						if (!range_attack_check_ammo((struct struct_hero*)hero, 0)) {
							return;
						}
#endif

						if (damage > 0) {

							sub_hero_le((struct struct_hero*)target_hero, damage);

							FIG_add_msg(8, damage);

							if (hero_dead(target_hero)) {
								g_defender_dead = 1;
							}
						}
					} else {

						/* note that for ranged attacks, the skill test will be done in the following function call. */
						damage = FIG_get_hero_weapon_attack_damage((struct struct_hero*)hero, (struct struct_hero*)target_monster, 0);
#ifdef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 32:
						 * Fix: move the function call after the damage calculation.
						 */
						if (!range_attack_check_ammo((struct struct_hero*)hero, 0)) {
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

					FIG_prepare_hero_fight_ani(0, (struct struct_hero*)hero, weapon_type,
							15, hero_pos + 1,
							l17 == 0 ? host_readbs(hero + HERO_ENEMY_ID) : host_readbs(hero + HERO_ENEMY_ID) + 20, 0);

					l13 = seg045_01a0(7, l12, hero_pos + 1,
						l17 == 0 ? host_readbs(hero + HERO_ENEMY_ID) : host_readbs(hero + HERO_ENEMY_ID) + 20,
						host_readbs(hero + HERO_VIEWDIR));

					FIG_set_sheet(host_readbs(hero + HERO_FIGHTER_ID), 0);

					draw_fight_screen_pal(0);

					if (FIG_get_range_weapon_type(hero) == -1) {

						fighter = FIG_get_fighter(host_readbs(hero + HERO_FIGHTER_ID));

						fighter->nvf_no = host_readbs(hero + HERO_VIEWDIR);
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
							FIG_prepare_hero_fight_ani(1, (struct struct_hero*)target_hero, -1, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
						} else {
							FIG_prepare_enemy_fight_ani(1, target_monster, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
						}
					}

					FIG_remove_smth();

					draw_fight_screen(0);

					clear_anisheets();
				}

			} else if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) {

				/* cast a spell */

				if (g_current_fight_no == FIGHTS_F144) {
					/* no spells allowed in the final fight */

					sub_hero_le((struct struct_hero*)hero, host_readws(hero + HERO_LE) + 1);

					g_attacker_dead = 1;
				}

				l6 = g_spell_descriptions[host_readbs(hero + HERO_SPELL_ID)].unkn6;

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

					sprintf(g_text_output_buf, g_string_casts_spell, (char*)hero + HERO_NAME2,
						get_ttx(host_readbs(hero + HERO_SPELL_ID) + 0x6a));

					GUI_print_string(g_text_output_buf, 1, 194);

					set_textcolor(fg_bak, bg_bak);

					delay_or_keypress(50);
				}

				if (host_readbs(hero + HERO_ENEMY_ID) != 0) {

					l12 = l13 = 0;

					if (random_schick(100) > 50) {
						l12 = 1;
					}

					if (host_readbs(hero + HERO_ENEMY_ID) < 10) {
						l12 = 2;
					}

					FIG_call_draw_pic();

					if (l15 != -1) {

						seg044_002a(0, (struct struct_hero*)hero, 4, hero_pos + 1,
								l17 == 0 ? host_readbs(hero + HERO_ENEMY_ID) : host_readbs(hero + HERO_ENEMY_ID) + 20, l12, 0);
					}

					if (l15 > 0) {

						if (l6 > 0) {

							if (l6 != 4) {
								seg045_0394(6, (struct struct_hero*)hero, l6);
							}

						} else {

							if (host_readbs(hero + HERO_ENEMY_ID) > 0) {

								if (target_is_hero == 0) {

									seg044_002f(1, target_monster, 99,
											l17 == 0 ? host_readbs(hero + HERO_ENEMY_ID) : host_readbs(hero + HERO_ENEMY_ID) + 20,
											hero_pos + 1, 1);
								} else {

									if (check_hero(target_hero) || (g_defender_dead != 0)) {

										seg044_002a(1, (struct struct_hero*)target_hero, 99, host_readbs(hero + HERO_ENEMY_ID), 0 , -1, 1);
									}
								}
							}
						}

						if ((host_readbs(hero + HERO_SPRITE_NO) != 7) &&
							(host_readbs(hero + HERO_SPRITE_NO) != 18) &&
							(host_readbs(hero + HERO_ENEMY_ID) > 0 ))
						{

							l13 = seg045_01a0(7, l12, hero_pos + 1,
								l17 == 0 ? host_readbs(hero + HERO_ENEMY_ID) : host_readbs(hero + HERO_ENEMY_ID) + 20,
								host_readbs(hero + HERO_VIEWDIR));
						}
					}

					if (l15 != -1) {

						FIG_set_sheet(host_readbs(hero + HERO_FIGHTER_ID), 0);
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
								nvf.no = 26;
								nvf.type = 0;
								nvf.width =(Bit8u*)&width;
								nvf.height = (Bit8u*)&height;

								process_nvf(&nvf);
#if !defined(__BORLANDC__)
								/* BE-fix */
								width = host_readws((Bit8u*)&width);
								height = host_readws((Bit8u*)&height);
#endif

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

								if (is_in_byte_array(target_monster->gfx_id, (Bit8u*)g_two_fielded_sprite_id))
								{
									fighter_add = FIG_get_fighter(target_monster->fighter_id);

									FIG_set_sheet(g_fig_twofielded_table[fighter_add->twofielded], 3);
								}

							} else {

								if (host_readbs(hero + HERO_ENEMY_ID) > 0) {

									FIG_set_sheet(host_readbs(target_hero + HERO_FIGHTER_ID), 1);
								}
							}
						}

						g_fig_continue_print = 1;

						draw_fight_screen(1);

						if (l6 > 0) {
							FIG_make_invisible(g_fig_shot_bolt_id);
						}

						if (g_spell_illusionen) {

							if (host_readbs(hero + HERO_ENEMY_ID) >= 10) {

								FIG_make_invisible(target_monster->fighter_id);

								if (is_in_byte_array(target_monster->gfx_id, (Bit8u*)g_two_fielded_sprite_id))
								{
									fighter_add = FIG_get_fighter(target_monster->fighter_id);

									FIG_make_invisible(g_fig_twofielded_table[fighter_add->twofielded]);
								}
							} else {
								if (host_readbs(hero + HERO_ENEMY_ID) > 0) {

									FIG_make_invisible(host_readbs(target_hero + HERO_FIGHTER_ID));
								}
							}
						}

						if (l13 != 0) {
							FIG_remove_smth();
						}

						if ((l6 > 0) && (l6 != 3) && (l6 != 4)) {
							FIG_remove_smth2();
						}

						FIG_draw_figures();
					}

					FIG_output(g_dtp2);

					clear_anisheets();

				} else {
					FIG_output(g_dtp2);
				}

			} else if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_USE_ITEM) {

				FIG_use_item((struct struct_hero*)hero, target_monster, (struct struct_hero*)target_hero, target_is_hero, hero_pos);
			}
		}
	}

	refresh_screen_size();

	if (l17 != 0) {
		add_ptr_bs(hero + HERO_ENEMY_ID, 20);
	}
}

#if !defined(__BORLANDC__)
}
#endif
