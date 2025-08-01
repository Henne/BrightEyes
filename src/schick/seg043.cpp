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

struct msg {
	signed short pos, type;
};

/**
 * \brief   execute the fight action of a monster
 *
 * \param   monster     pointer to a monster datasheet
 * \param   monster_pos position of the monster (fighter_id = monster_pos + 10)
 */
void FIG_do_enemy_action(Bit8u* monster, signed short monster_pos)
{
	signed short damage;

	Bit8u *hero;
	Bit8u *mon;
	Bit8u *p_weapon;
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
	Bit8u *l16;
	signed short l17 = 0;
	signed short fighter_id;
	signed short hero_x;
	signed short hero_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	struct dummy dst = *(struct dummy*)(p_datseg + VIEWDIR_INVOFFSETS3);
	struct msg tmp;

	update_mouse_cursor();

	if (g_in_fight) {

		FIG_clear_msgs();

		/* set defender_gets_hit, ATTACKER_ATTACKS_AGAIN, DEFENDER_ATTACKS, ATTACKER_DEAD, DEFENDER_DEAD all to 0 */
		defender_gets_hit = ds_writews(ATTACKER_ATTACKS_AGAIN, ds_writews(DEFENDER_ATTACKS, ds_writews(ATTACKER_DEAD, ds_writews(DEFENDER_DEAD, 0))));

		ds_writew(FIG_ACTOR_GRAMMAR_TYPE, 1);
		ds_writew(FIG_ACTOR_GRAMMAR_ID, host_readbs((Bit8u*)(monster)));

		if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) < 10) {

			/* monster attacks hero */
			hero = get_hero(host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) - 1);

			ds_writew(FIG_TARGET_GRAMMAR_TYPE, 2);
			ds_writew(FIG_TARGET_GRAMMAR_ID, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) - 1);

			if (hero_dead(hero) || !host_readbs(hero + HERO_TYPE)) {
				return;
			}

			target_is_hero = 1;
		} else {
			/* monster attacks monster */

			mon = p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID);

			ds_writew(FIG_TARGET_GRAMMAR_TYPE, 1);
			ds_writew(FIG_TARGET_GRAMMAR_ID, host_readbs(mon));

			if (enemy_dead(mon) || !host_readbs(mon)) {
				return;
			}

			target_is_hero = 0;

			if ((is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) &&
				(l17 == 0))
			{
				FIG_search_obj_on_cb(host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), &target_x, &target_y);
				FIG_search_obj_on_cb(monster_pos + 10, &hero_x, &hero_y);

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

				if (host_readbs(mon + ENEMY_SHEET_VIEWDIR) != dir) {

					fighter_id = get_cb_val(hero_x + dst.a[dir].x, hero_y + dst.a[dir].y);

					if (fighter_id != 0) {

						if ((fighter_id >= 50) ||
							((fighter_id < 10) && !hero_dead(get_hero(fighter_id - 1))) ||
							((fighter_id >= 10) && (fighter_id < 30) && !enemy_dead((p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)) + (SIZEOF_ENEMY_SHEET * fighter_id))) ||
							((fighter_id >= 30) && (fighter_id < 50) && !enemy_dead((p_datseg + (ENEMY_SHEETS - 30*SIZEOF_ENEMY_SHEET)) + (SIZEOF_ENEMY_SHEET * fighter_id))))
						{
							l17 = 1;
						}
					}
				}

			}
		}

		if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ACTION_ID) == FIG_ACTION_MELEE_ATTACK) {

			if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) < 10) {

				/* attack a hero */

				p_weapon = hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY;

				weapon_type = weapon_check(hero);

				if (weapon_type == -1) {
					/* no valid weapon == bare hands */
					defender_at = host_readbs(hero + HERO_AT) + host_readbs(hero + HERO_ATTACK_TYPE);
					defender_pa = host_readbs(hero + HERO_PA) - host_readbs(hero + HERO_ATTACK_TYPE);
				} else {
					defender_at = host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WEAPON_TYPE)) + host_readbs(hero + HERO_ATTACK_TYPE) + host_readbs(hero + HERO_AT_MOD);
					defender_pa = host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WEAPON_TYPE)) - host_readbs(hero + HERO_ATTACK_TYPE) + host_readbs(hero + HERO_PA_MOD);
				}

				/* guarding heroes get a PA-bonus of 3 */
				if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_GUARD) {
					defender_pa += 3;
				}

				/* after destroying the orc statuette between Oberorken and Felsteyn, dwarfs get a PA-bonus against orcs */
				if ((ds_readbs(TEVENT071_ORCSTATUE) != 0) &&
					(host_readbs(hero + HERO_TYPE) == HERO_TYPE_DWARF) &&
					(host_readbs((Bit8u*)(monster) + ENEMY_SHEET_GFX_ID) == 24))
				{
					defender_pa++;
				}

				/* subtract RS handycap */
				defender_at -= host_readbs(hero + HERO_RS_BE) / 2;
				defender_pa -= host_readbs(hero + HERO_RS_BE) / 2;

			} else {
				/* attack a monster */
				defender_at = host_readbs(mon + ENEMY_SHEET_AT);
				defender_pa = host_readbs(mon + ENEMY_SHEET_PA);
			}

			/* spell_dunkelheit() is active => AT-4, PA-4*/
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				defender_at -= 4;
				defender_pa -= 4;
			}

			attacker_at = host_readbs((Bit8u*)(monster) + ENEMY_SHEET_AT);
			if (gs_ingame_timers[INGAME_TIMER_DARKNESS]) {
				attacker_at -= 4;
			}

			if (target_is_hero != 0) {

				/* TODO */
				if (ds_readbs((HERO_IS_TARGET-1) + host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID)) == 1) {
					attacker_at += 2;
				}

				/* 'Chamaelioni' spell is active on the target hero => AT-5 */
				if (hero_chamaelioni(hero) == 1) {
					attacker_at -= 5;
				}

				/* 'Duplicatus' spell is active on the target hero => AT/2 */
				if (hero_duplicatus(hero) == 1) {
					attacker_at /= 2;
				}
			} else {
				/* TODO */
				if (ds_readbs(FIG_ACTORS_UNKN + host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID)) == 1) {
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

						ds_writew(DEFENDER_ATTACKS, 1);

						if (random_schick(20) <= defender_at) {

							if (target_is_hero != 0) {
								damage = FIG_get_hero_weapon_attack_damage(hero, (Bit8u*)(monster), 0);
							} else {
								damage = FIG_get_enemy_attack_damage(mon, (Bit8u*)(monster), 1);
							}

							if (damage > 0) {

								FIG_damage_enemy((Bit8u*)(monster), damage, 1);

								FIG_add_msg(11, damage);

								/* swap msg struct */
								tmp = *(struct msg*)(p_datseg + FIG_TARGET_GRAMMAR_TYPE);
								*(struct msg*)(p_datseg + FIG_TARGET_GRAMMAR_TYPE) = *(struct msg*)(p_datseg + FIG_ACTOR_GRAMMAR_TYPE);
								*(struct msg*)(p_datseg + FIG_ACTOR_GRAMMAR_TYPE) = tmp;

								if (enemy_dead((Bit8u*)(monster))) {
									ds_writew(ATTACKER_DEAD, 1);
								}
							}
						}
					} else if ((two_w_6 >= 9) && (two_w_6 <= 11)) {
						/* attacker gets 1W6 damage */
#if !defined(__BORLANDC__)
						D1_INFO("1D6 damage.\n");
#endif

						damage = random_schick(6);

						FIG_damage_enemy((Bit8u*)(monster), damage, 1);

						FIG_add_msg(11, damage);

						*(struct msg*)(p_datseg + FIG_TARGET_GRAMMAR_TYPE) = *(struct msg*)(p_datseg + FIG_ACTOR_GRAMMAR_TYPE);

						if (enemy_dead((Bit8u*)(monster))) {
							ds_writew(ATTACKER_DEAD, 1);
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

					if (((target_is_hero != 0) && !ds_readbs((HERO_IS_TARGET-1) + host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID)) && check_hero(hero)) ||
						(!target_is_hero && (!ds_readbs(FIG_ACTORS_UNKN + host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID)))))
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

								ds_writew(ATTACKER_ATTACKS_AGAIN, 1);

								if (random_schick(20) <= attacker_at) {

									if (target_is_hero != 0) {
										damage = FIG_get_enemy_attack_damage((Bit8u*)(monster), hero, 0);

										if (damage > 0) {

											/* HESHTHOT */
											if (host_readbs((Bit8u*)(monster)) != 77) {
												sub_hero_le(hero, damage);
											}

											FIG_add_msg(8, damage);

											if (hero_dead(hero)) {
												ds_writew(DEFENDER_DEAD, 1);
											}
										}
									} else {
										damage = FIG_get_enemy_attack_damage((Bit8u*)(monster), mon, 1);

										if (damage > 0) {

											FIG_damage_enemy(mon, damage, 1);

											FIG_add_msg(11, damage);

											if (enemy_dead(mon)) {
												ds_writew(DEFENDER_DEAD, 1);
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

									if (hero_dead(hero)) {
										ds_writew(DEFENDER_DEAD, 1);
									}

								} else {

									FIG_damage_enemy(mon, damage, 1);

									FIG_add_msg(11, damage);

									if (enemy_dead(mon)) {
										ds_writew(DEFENDER_DEAD, 1);
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

								if ((randval2 == randval) && (target_is_hero != 0) && (host_readbs(p_weapon + INVENTORY_BF) != -99)) {
									/* if both random values agree and hero got attacked and his weapon is not unbreakable */
									/* now if 1W12 + BF is >= 16, weapon is broken. Otherwise, BF is increased by 1. */
#if !defined(__BORLANDC__)
									D1_INFO("weapon of defender gets damaged...");
#endif

									if (host_readbs(p_weapon + INVENTORY_BF) > 3) {

										if (random_schick(12) + host_readbs(p_weapon + INVENTORY_BF) > 15) {
#if !defined(__BORLANDC__)
											D1_INFO("broken.\n");
#endif

											or_ptr_bs(p_weapon + INVENTORY_FLAGS, 1); /* set 'broken' flag */

											FIG_add_msg(6, 0);
										} else {
#if !defined(__BORLANDC__)
											D1_INFO("BF increased from %d -> %d.\n",host_readbs(p_weapon + INVENTORY_BF),host_readbs(p_weapon + INVENTORY_BF) + 1);
#endif
											inc_ptr_bs(p_weapon + INVENTORY_BF);
										}
									} else {
#if !defined(__BORLANDC__)
										D1_INFO("BF increased %d -> %d.\n",host_readbs(p_weapon + INVENTORY_BF),host_readbs(p_weapon + INVENTORY_BF) + 1);
#endif
										inc_ptr_bs(p_weapon + INVENTORY_BF);
									}
								}
							}
						}
					} else {
						defender_gets_hit = 1;
					}

					if (defender_gets_hit != 0) {

						if (target_is_hero != 0) {

							damage = FIG_get_enemy_attack_damage((Bit8u*)(monster), hero, 0);

							if (damage > 0) {

								if (host_readbs((Bit8u*)(monster)) != 77) {
									sub_hero_le(hero, damage);
								}

								FIG_add_msg(8, damage);

								if (hero_dead(hero)) {
									ds_writew(DEFENDER_DEAD, 1);
								}
							}
						} else {

							damage = FIG_get_enemy_attack_damage((Bit8u*)(monster), mon, 1);

							if (damage > 0) {

								FIG_damage_enemy(mon, damage, 1);

								FIG_add_msg(11, damage);

								if (enemy_dead(mon)) {
									ds_writew(DEFENDER_DEAD, 1);
								}
							}
						}
					}
				}
			}

			seg041_8c8();

			if (target_is_hero != 0) {

				if (check_hero(hero) || (ds_readws(DEFENDER_DEAD) != 0)) {

					FIG_prepare_hero_fight_ani(0, hero, weapon_type, 100, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
				}

			} else if (l17 == 0) {
					FIG_prepare_enemy_fight_ani(0, mon, 100, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
			} else if (ds_readws(DEFENDER_DEAD) != 0) {
					FIG_prepare_enemy_fight_ani(0, mon, 0, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
			}

			FIG_prepare_enemy_fight_ani(1, (Bit8u*)(monster), 2, monster_pos + 10, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), 0);
			g_fig_continue_print = 1;
			draw_fight_screen_pal(0);
			seg041_8c8();

		} else {

			if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ACTION_ID) == FIG_ACTION_RANGE_ATTACK) {

				if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_SHOTS) > 0) {
					l15 = 3;
					dec_ptr_bs((Bit8u*)(monster) + ENEMY_SHEET_SHOTS);
				} else {
					l15 = 4;
					dec_ptr_bs((Bit8u*)(monster) + ENEMY_SHEET_THROWS);
				}

				if (target_is_hero != 0) {

					/* attack hero */

					damage = dice_template(l15 == 3 ? host_readws((Bit8u*)(monster) + ENEMY_SHEET_SHOT_DAM) : host_readws((Bit8u*)(monster) + ENEMY_SHEET_THROW_DAM));

					damage = (damage * 8) / 10;

					/* RS */
					damage -= host_readbs(hero + HERO_RS_BONUS1);

					if (damage > 0) {

						sub_hero_le(hero, damage);

						FIG_add_msg(8, damage);

						if (hero_dead(hero)) {
							ds_writew(DEFENDER_DEAD, 1);
						}
					}

				} else {

					/* attack monster */

					damage = dice_template(l15 == 3 ? host_readws((Bit8u*)(monster) + ENEMY_SHEET_SHOT_DAM) : host_readws((Bit8u*)(monster) + ENEMY_SHEET_THROW_DAM))
							- host_readbs(mon + ENEMY_SHEET_RS);

					if (damage > 0) {

						FIG_damage_enemy(mon, damage, 1);

						FIG_add_msg(11, damage);

						if (enemy_dead(mon)) {
							ds_writew(DEFENDER_DEAD, 1);
						}
					}
				}

				seg041_8c8();

				l11 = l15;
				l12 = 0;

				FIG_call_draw_pic();

				FIG_prepare_enemy_fight_ani(0, (Bit8u*)(monster), 15, monster_pos + 10, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), 0);

				l12 = seg045_01a0(7, l11, monster_pos + 10, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), host_readbs((Bit8u*)(monster) + ENEMY_SHEET_VIEWDIR));

				FIG_set_sheet(host_readbs((Bit8u*)(monster) + ENEMY_SHEET_FIGHTER_ID), 0);

				draw_fight_screen_pal(0);

				if (l12 != 0) {

					FIG_set_sheet(ds_readbs(FIG_SHOT_BOLT_ID), 7);

					draw_fight_screen((l12 == 0) && (ds_readws(DEFENDER_DEAD) == 0) ? 0 : 1);

					FIG_make_invisible(ds_readbs(FIG_SHOT_BOLT_ID));
				}

				g_fig_continue_print = 1;

				if (ds_readws(DEFENDER_DEAD) != 0) {

					if (target_is_hero != 0) {

						FIG_prepare_hero_fight_ani(1, hero, -1, 0, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
					} else {

						FIG_prepare_enemy_fight_ani(1, mon, 0, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
					}
				}

				FIG_remove_smth();
				draw_fight_screen(0);
				seg041_8c8();

			} else if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ACTION_ID) == FIG_ACTION_SPELL) {

				/* spellcast */

				l14 = ds_readbs((MON_SPELL_DESCRIPTIONS + MON_SPELL_DESCRIPTIONS_ANI_ID) + SIZEOF_MON_SPELL_DESCRIPTIONS * host_readbs((Bit8u*)(monster) + ENEMY_SHEET_CUR_SPELL));

				*g_dtp2 = '\0';

				l13 = MON_cast_spell(monster, 0);

				seg041_8c8();

				if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) != 0) {

					l11 = l12 = 0;

					if (random_schick(100) > 50) {
						l11 = 1;
					}

					if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) < 10) {
						l11 = 2;
					}

					FIG_call_draw_pic();

					if (l13 != -1) {

						seg044_002f(0, (Bit8u*)(monster), 4, monster_pos + 10, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), 0);
					}

					if (l13 > 0) {

						if (l14 > 0) {

							seg045_041b(6, (Bit8u*)(monster), l14);

						} else {

							if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) > 0) {

								if (!target_is_hero) {

									seg044_002f(1, mon, 99, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), monster_pos + 10, 1);
								} else {

									if (check_hero(hero) || (ds_readws(DEFENDER_DEAD) != 0)) {

										seg044_002a(1, hero, 99, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), 0, -1, 1);
									}
								}
							}
						}

						if ((host_readbs((Bit8u*)(monster) + ENEMY_SHEET_GFX_ID) != 0x12) &&
							(host_readbs((Bit8u*)(monster) + ENEMY_SHEET_GFX_ID) != 7) &&
							(host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) > 0))
						{
							l12 = seg045_01a0(7, l11, monster_pos + 10, host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID), host_readbs((Bit8u*)(monster) + ENEMY_SHEET_VIEWDIR));
						}

					}
					if (l13 != -1) {

						FIG_set_sheet(host_readbs((Bit8u*)(monster) + ENEMY_SHEET_FIGHTER_ID), 0);

						draw_fight_screen_pal(1);
					}

					if (l13 > 0) {

						if (l12 != 0) {

							FIG_set_sheet(ds_readbs(FIG_SHOT_BOLT_ID), 7);

							draw_fight_screen(1);

							FIG_make_invisible(ds_readbs(FIG_SHOT_BOLT_ID));
						}

						if (l14 > 0) {
							FIG_set_sheet(ds_readbs(FIG_SPELLGFX_ID), 6);
						}

						if (!target_is_hero) {

							FIG_set_sheet(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID), 1);


							if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {

								l16 = (Bit8u*)(FIG_get_ptr(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID)));

								FIG_set_sheet(ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(l16 + FIGHTER_TWOFIELDED)), 3);
							}
						} else {

							if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) > 0) {

								FIG_set_sheet(host_readbs(hero + HERO_FIGHTER_ID), 1);
							}
						}

						g_fig_continue_print = 1;

						draw_fight_screen(1);

						if (l14 > 0) {
							FIG_make_invisible(ds_readbs(FIG_SHOT_BOLT_ID));
						}

						if (ds_readw(MSPELL_AWAKE_FLAG) != 0) {

							ds_writew(MSPELL_AWAKE_FLAG, 0);

							FIG_remove_from_list(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID), 1);

							ds_writew((FIG_LIST_ELEM + 0x00), ds_readbs(GFXTAB_FIGURES_MAIN + 5 * host_readbs(mon + ENEMY_SHEET_GFX_ID)));
							ds_writeb((FIG_LIST_ELEM + FIGHTER_NVF_NO), host_readbs(mon + ENEMY_SHEET_VIEWDIR));
							ds_writeb((FIG_LIST_ELEM + FIGHTER_OFFSETX), ds_readbs(GFXTAB_OFFSETS_MAIN + 10 * host_readbs(mon + ENEMY_SHEET_GFX_ID) + 2 * host_readbs(mon + ENEMY_SHEET_VIEWDIR)));
							ds_writeb((FIG_LIST_ELEM + FIGHTER_OFFSETY), ds_readbs((GFXTAB_OFFSETS_MAIN + 1) + 10 * host_readbs(mon + ENEMY_SHEET_GFX_ID) + 2 * host_readbs(mon + ENEMY_SHEET_VIEWDIR)));

							if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {
								ds_writeb((FIG_LIST_ELEM + FIGHTER_X1), ds_readbs(GFXTAB_TWOFIELDED_X1 + host_readbs(mon + ENEMY_SHEET_VIEWDIR)));
								ds_writeb((FIG_LIST_ELEM + FIGHTER_X2), ds_readbs(GFXTAB_TWOFIELDED_X2 + host_readbs(mon + ENEMY_SHEET_VIEWDIR)));
							} else {
								ds_writeb((FIG_LIST_ELEM + FIGHTER_X1), 0);
								ds_writeb((FIG_LIST_ELEM + FIGHTER_X2), 31);
								ds_writeb((FIG_LIST_ELEM + FIGHTER_TWOFIELDED), -1);
							}

							ds_writeb((FIG_LIST_ELEM + FIGHTER_Y1), 0);
							ds_writeb((FIG_LIST_ELEM + FIGHTER_Y2), 39);
							ds_writeb((FIG_LIST_ELEM + FIGHTER_HEIGHT), 40);
							ds_writeb((FIG_LIST_ELEM + FIGHTER_WIDTH), 32);
							ds_writeb((FIG_LIST_ELEM + FIGHTER_IS_ENEMY), 1);
							ds_writeb((FIG_LIST_ELEM + FIGHTER_RELOAD), -1);
							ds_writeb((FIG_LIST_ELEM + FIGHTER_WSHEET), -1);
							ds_writeb((FIG_LIST_ELEM + FIGHTER_SHEET), -1);

							FIG_add_to_list(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID));
						}

						if (ds_readws(SPELL_ILLUSIONEN) != 0) {

							if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) >= 10) {

								FIG_make_invisible(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID));

								if (is_in_byte_array(host_readbs(mon + 1), p_datseg + TWO_FIELDED_SPRITE_ID)) {

									l16 = (Bit8u*)(FIG_get_ptr(host_readbs(mon + ENEMY_SHEET_FIGHTER_ID)));

									FIG_make_invisible(ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(l16 + FIGHTER_TWOFIELDED)));
								}
							} else {

								if (host_readbs((Bit8u*)(monster) + ENEMY_SHEET_ENEMY_ID) > 0) {

									FIG_make_invisible(host_readbs(hero + HERO_FIGHTER_ID));
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

					seg041_8c8();
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
void FIG_use_item(Bit8u *hero, Bit8u *target_monster, Bit8u *target_hero, signed short flag, signed short hero_pos)
{
	signed short damage;

	signed short l3;
	signed short hylailic = 0;
	signed short usecase;
	signed short item_id = host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);
	Bit8u *p_item = get_itemsdat(item_id);

	if (item_herb_potion(p_item)) {
		usecase = 1;
	} else if (!item_useable(p_item) || (host_readws(hero + (HERO_INVENTORY + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY + INVENTORY_QUANTITY)) == 0)) {
		usecase = 0;
	} else {
		usecase = 2;
	}

	*g_dtp2 = '\0';

	if (host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID) == ITEM_MIASTHMATICUM) {
		/* MIASTHMATIC */

		/* 1W6 + 4 */
		damage = dice_roll(1, 6, 4);

		if (host_readbs(hero + HERO_ENEMY_ID) >= 10) {

			strcpy(g_dtp2, get_tx(37));

			FIG_damage_enemy(target_monster, damage, 0);

			FIG_add_msg(11, damage);

			if (enemy_dead(target_monster)) {
				ds_writew(DEFENDER_DEAD, 1);
			}
		} else {

			if (flag != 0) {

				strcpy(g_dtp2, get_tx(37));

				sub_hero_le(target_hero, damage);

				FIG_add_msg(8, damage);

				if (hero_dead(target_hero)) {
					ds_writew(DEFENDER_DEAD, 1);
				}
			}
		}

		/* drop the item in the left hand */
		drop_item(hero, HERO_INVENTORY_SLOT_LEFT_HAND, 1);

	} else if (host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID) == ITEM_HYLAILIC_FIRE) {

		/* HYLAILIC FIRE */

		if (host_readbs(hero + HERO_ENEMY_ID) >= 10) {

			/* .. used on a monster */

			FIG_damage_enemy(target_monster, 20, 0);

			FIG_add_msg(11, 20);

			if (enemy_dead(target_monster)) {
				ds_writew(DEFENDER_DEAD, 1);
			}
		} else {
			/* .. used on another hero */
			if (flag != 0) {

				sub_hero_le(target_hero, 20);

				FIG_add_msg(8, 20);

				if (hero_dead(target_hero)) {
					ds_writew(DEFENDER_DEAD, 1);
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

		seg041_8c8();

		FIG_prepare_hero_fight_ani(0, hero, -1, usecase == 1 ? FIG_ACTION_UNKNOWN3 : FIG_ACTION_UNKNOWN4, hero_pos + 1, host_readbs(hero + HERO_ENEMY_ID), 0);

		l3 = 0;

		if (hylailic != 0) {
			seg045_0394(6, hero, 2);
		} else {
			g_fig_continue_print = 1;
		}

		draw_fight_screen_pal(0);

		if (hylailic != 0) {

			FIG_set_sheet(ds_readbs(FIG_SPELLGFX_ID), 6);

			l3 = 1;

			g_fig_continue_print = 1;

			draw_fight_screen(1);
		}

		if (l3 != 0) {
			FIG_remove_smth2();
		}

		if (ds_readws(DEFENDER_DEAD) != 0) {

			if (flag != 0) {
				FIG_prepare_hero_fight_ani(1, target_hero, -1, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
			} else {
				FIG_prepare_enemy_fight_ani(1, target_monster, 0, host_readbs(hero + HERO_ENEMY_ID), hero_pos + 1, 1);
			}

		}

		if ((l3 != 0) || (ds_readws(DEFENDER_DEAD) != 0)) {
			draw_fight_screen(0);
		}

		seg041_8c8();
	}

	if (*g_dtp2) {
		/* show output string if needed */
		GUI_output(g_dtp2);
	}
}

#if !defined(__BORLANDC__)
}
#endif
