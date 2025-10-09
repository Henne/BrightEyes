/**
 *	Rewrite of DSA1 v3.02_de functions of seg033 (fightsystem: menu)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg033.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg032.h"
#include "seg034.h"
#include "seg035.h"
#include "seg036.h"
#include "seg038.h"
#include "seg039.h"
#include "seg041.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg105.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif
/**
 * \brief   combat menu
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero
 * \param   x           x-coordinate on the chessboard
 * \param   y           y-coordinate on the chessboard
 */
void FIG_menu(struct struct_hero *hero, signed short hero_pos, signed short x, signed short y)
{
	signed short selected;
	signed short l1;
	signed short txt_tabpos_bak;
	signed short done;
	signed short target_x;
	signed short target_y;
	signed short range_weapon;
	signed char target_id;
	signed short rwt1;
	signed short rwt2;
	signed char at;
	signed char pa;
	struct item_stats *p_itemsdat;
	struct weapon_descr *weapon;
	struct spell_descr *spell_description;
	signed short damage_lo;
	signed short damage_hi;
	signed short weapon_id;
	signed short radio_i;
	signed short slot_no;
	struct struct_fighter *ptr;
	signed short tw_bak;
	signed short slots[16];

	l1 = 1;
	done = 0;

	while ((done == 0) && (hero->bp_left > 0)) {

		if (g_fig_cb_marker_id != -1) {
			FIG_remove_from_list(g_fig_cb_marker_id, 0);
			g_fig_cb_marker_id = -1;
		}

		FIG_init_list_elem(hero_pos + 1);
		draw_fight_screen_pal(0);

		if (hero->flags.scared || (hero->action_id == FIG_ACTION_FLEE)) {

			hero->flags.tied = 0; /* unset 'tied' flag (why??) */
			hero->flags.petrified = 0; /* unset 'petrified' flag (why???) */

			if (FIG_find_path_to_target((Bit8u*)hero, hero_pos, x, y, 5) != -1) {
				seg036_00ae(hero, hero_pos); /* probably: execute hero movement based on path saved in g_fig_move_pathdir. */
			}
			done = 1;

		} else if (hero->flags.renegade || (hero->npc_id > 0)|| (g_autofight != 0)) {

			hero->action_id = FIG_ACTION_WAIT;

			if (((g_current_fight_no != FIGHTS_F144) || (g_finalfight_tumult)) &&
				(hero->bp_left >= 3))
			{
				KI_hero(hero, hero_pos, x, y);
			}
			done = 1;
		} else {

			selected = -1;
			do {
				g_basepos_x = ((x + y < 15) ? 90 : -90);
				g_basepos_y = ((x + y < 15) ? -30 : 30);

				/* prepare question with BP */
				sprintf(g_dtp2, get_tx(18), hero->bp_left);

				txt_tabpos_bak = g_txt_tabpos[0];
				g_txt_tabpos[0] = g_basepos_x + 204;

				refresh_screen_size();

				weapon_id = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;

				if (!g_itemsdat[weapon_id].flags.weapon || (g_itemsdat[weapon_id].flags.weapon && hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.broken)) {
					/* test 'broken' flag */
					/* no weapon or weapon broken, use red color for "change weapon" */
					sprintf(g_text_output_buf, (char*)g_red_string1, get_tx(24));
				} else {
					/* good weapon, no special color */
					strcpy(g_text_output_buf, get_tx(24));
				}

				if (hero->attrib[ATTRIB_KK].current * 110 <= hero->load) {
					/* too much weight, use red color for "drop item" */
					sprintf(g_text_output_buf + 50, (char*)g_red_string2, get_tx(46));
				} else {
					/* weight ok, no special color */
					strcpy(g_text_output_buf + 50, get_tx(46));
				}

				/* show radion menu */
				selected = GUI_radio(g_dtp2, l1 != 0 ? 13 : 12,
							get_tx(19),
							get_tx(20),
							get_tx(21),
							get_tx(22),
							get_tx(23),
							g_text_output_buf + 50,
							g_text_output_buf,
							get_tx(16),
							get_tx(26),
							get_tx(25),
							get_tx(34),
							get_tx(51),
							get_tx(27));

				update_mouse_cursor();
				g_txt_tabpos[0] = txt_tabpos_bak;
				g_basepos_x = 0;
				g_basepos_y = 0;

			} while (selected == -1);

			l1 = 0;

			if (selected == FIG_ACTION_MOVE) {
				/* MOVE / BEWEGEN */

				if (hero->flags.tied) {

					/* Probe: MU + 2 */
					if (test_attrib(hero, ATTRIB_MU, 2) > 0) {

						/* Success */
						hero->flags.tied = 0;

					} else if (hero->attrib[ATTRIB_MU].current > 4) {
						/* Failure */
						/* MU - 2 for 7 hours */
						slot_no = get_free_mod_slot();
						set_mod_slot(slot_no, HOURS(7), (Bit8u*)&hero->attrib[ATTRIB_MU].current, -2, (signed char)hero_pos);
					}
				}

				if (!hero->flags.tied) {

					hero->action_id = FIG_ACTION_MOVE;

					if (hero->bp_left > 0) {
						/* let the player select a move destination */

						refresh_screen_size();

						FIG_move_hero(hero, hero_pos, &x, &y);

						update_mouse_cursor();

						/* Moving destroys an active 'Chamaelioni' spell */
						hero->flags.chamaelioni = 0; /* unset 'chamaelioni' flag.  (???) */
						/* TODO: What if the target square agreed with the starting square (such that no movement has happened? */

					} else {
						/* no BP left */
						sprintf(g_dtp2, get_tx(17), hero->alias);
						GUI_output(g_dtp2);
					}

				} else {
					sprintf(g_dtp2,	get_tx(7), hero->alias);
					GUI_output(g_dtp2);
				}
			} else if (selected == FIG_ACTION_MELEE_ATTACK) {
				/* ATTACK / ANGRIFF */

				if (hero->bp_left >= 3) {

					target_x = x;
					target_y = y;
					target_id = -5;
					if ((range_weapon = FIG_get_range_weapon_type(hero)) == -1) {
						/* not a range weapon */
						refresh_screen_size();

						target_id = FIG_cb_select_target(&target_x, &target_y, 1);

						update_mouse_cursor();
					} else {
						if (range_attack_check_ammo(hero, 1)) {

							/* a range weapon */
							refresh_screen_size();

							target_id = FIG_cb_select_target(&target_x, &target_y, 99);

							update_mouse_cursor();
						}
					}

					if (target_id != -5) {

						if ((target_id <= 0) || (target_id >= 50)) {
							GUI_output(get_tx(28));
						} else if (target_id == (hero_pos + 1)) {
							GUI_output(get_tx(3));
						} else if (((target_id < 10) && hero_dead(get_hero(target_id - 1))) ||
								//((target_id >= 10) && (target_id < 30) && (g_enemy_sheets[target_id - 10].flags.dead || g_enemy_sheets[target_id - 10].flags.dead)) ||
								((target_id >= 10) && (target_id < 30) && (g_enemy_sheets[target_id].flags.dead || g_enemy_sheets[target_id].flags.mushroom)) ||
								/* TODO: check target_id < 50 */
								//((target_id >= 30) && (g_enemy_sheets[target_id - 30].flags.dead || g_enemy_sheets[target_id - 30].flags.mushroom)))
								((target_id >= 30) && (g_enemy_sheets[target_id].flags.dead || g_enemy_sheets[target_id].flags.mushroom)))
						{
							GUI_output(get_tx(29));

							hero->action_id = FIG_ACTION_MOVE;

							/* set target id to 0 */
							hero->enemy_id = 0;

						} else if ((range_weapon != -1) && (calc_beeline(x, y, target_x, target_y) < 2)) {

							GUI_output(get_ttx(508));

							hero->action_id = FIG_ACTION_MOVE;

							/* set target id to 0 */
							hero->enemy_id = 0;

						} else {
							hero->atpa_mod = 0;

							if (range_weapon == -1) {

								selected = -1;

								do {
									refresh_screen_size();

									selected = GUI_radio(get_ttx(584), 3, get_ttx(585), get_ttx(586), get_ttx(587));

									update_mouse_cursor();

								} while (selected == -1);

								hero->atpa_mod = selected == 1 ? 2 : (selected == 2) ? 0 : -2;
							}

							/* set target id */
							hero->enemy_id = target_id;
							/* set BP to 0 */
							hero->bp_left = 0;
							hero->action_id = (range_weapon > 0 ? FIG_ACTION_RANGE_ATTACK : FIG_ACTION_MELEE_ATTACK);
							done = 1;
						}
					}
				} else {
					/* no BP left */
					sprintf(g_dtp2,	get_tx(17), hero->alias);
					GUI_output(g_dtp2);
				}
			} else if (selected == FIG_ACTION_GUARD) {
				/* GUARD / SICHERN */

				if (hero->bp_left >= 3) {

					/* set BP to 0 */
					hero->bp_left = 0;

					hero->action_id = FIG_ACTION_GUARD;

					done = 1;
				} else {
					/* no BP left */
					sprintf(g_dtp2, get_tx(17), hero->alias);
					GUI_output(g_dtp2);
				}

			} else if (selected == FIG_ACTION_SPELL) {
				/* CAST SPELL / ZAUBERN */

				if (hero->typus < HERO_TYPE_WITCH) {
					/* not a magic user */
					GUI_output(get_ttx(215));
				} else {

					if (hero->bp_left >= 5) {

						refresh_screen_size();
						hero->spell_id = select_spell(hero, 0);
						update_mouse_cursor();

						if (hero->spell_id > 0) {

							hero->action_id = FIG_ACTION_MOVE;
							hero->enemy_id = 0;

							spell_description = &g_spell_descriptions[hero->spell_id];

							if (spell_description->where_to_use == -1) {

								/* not a combat spell */
								GUI_output(get_ttx(592));

							} else {

								if (spell_description->target_type) {

									target_x = x;
									target_y = y;
									weapon_id = 1;

									if (spell_description->range > 0) {
										weapon_id = 99;
									}

									refresh_screen_size();

									target_id = FIG_cb_select_target(&target_x, &target_y, weapon_id);

									update_mouse_cursor();

									if (target_id != -99) {

										if (target_id < 0) {
											GUI_output(get_tx(28));

										} else if ((target_id == 0) && (spell_description->target_type != 4))
										{
											GUI_output(get_tx(4));

										} else if ((target_id < 10) &&
											(spell_description->target_type != 2) &&
											(spell_description->target_type != 3))
										{
											GUI_output(get_tx(5));

										} else if ((target_id >= 10) &&	(target_id < 50) &&
											(spell_description->target_type != 1) &&
											(spell_description->target_type != 3))
										{
											GUI_output(get_tx(6));

										} else {
											hero->action_id = FIG_ACTION_SPELL;
											hero->enemy_id = target_id;
											done = 1;
										}
									}
								} else {
									hero->action_id = FIG_ACTION_SPELL;
									hero->enemy_id = 0;
									done = 1;
								}
							}
						}

						if ((hero->action_id == FIG_ACTION_SPELL) &&
							(get_spell_cost(hero->spell_id, 1) > hero->ae))
						{
							/* not enough AE */
							GUI_output(get_ttx(337));
							hero->action_id = FIG_ACTION_MOVE;
							done = 0;
						}
					} else {
						/* no BP left */
						sprintf(g_dtp2, get_tx(17), hero->alias);
						GUI_output(g_dtp2);
					}
				}
			} else if (selected == FIG_ACTION_USE_ITEM) {
				/* USE ITEM / GGST. BENUTZEN */

					if (hero->bp_left >= 3) {

						if (is_in_word_array(hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id, g_attack_items))
						{
							target_x = x;
							target_y = y;
							refresh_screen_size();

							target_id = FIG_cb_select_target(&target_x, &target_y, 99);

							update_mouse_cursor();
							hero->enemy_id = target_id;
						} else {
							hero->enemy_id = 0;
						}

						if ((hero->enemy_id < 0) || hero->enemy_id >= 50) {
							hero->enemy_id = 0;
							GUI_output(get_tx(28));
						} else {
							/* set BP to 0 */
							hero->bp_left = 0;

							hero->action_id = FIG_ACTION_USE_ITEM;
							done = 1;
						}
					} else {
						/* no BP left */
						sprintf(g_dtp2,	get_tx(17), hero->alias);
						GUI_output(g_dtp2);
					}
			} else if (selected == FIG_ACTION_EXCHANGE_ITEM) {
				/* EXCHANGE ITEM / GGST. WECHSELN */
					if (hero->bp_left >= 2) {

						radio_i = 0;

						for (slot_no = HERO_INVENTORY_SLOT_KNAPSACK_1; slot_no < NR_HERO_INVENTORY_SLOTS; slot_no++) {

							weapon_id = hero->inventory[slot_no].item_id;

							if (weapon_id != 0) {

								slots[radio_i] = slot_no;

								g_radio_name_list[radio_i] = (g_dtp2 + 30 * radio_i);

								strcpy(g_radio_name_list[radio_i], GUI_name_singular(get_itemname(weapon_id)));

								radio_i++;
							}
						}

						if (radio_i == 0) {
							sprintf(g_dtp2,	get_ttx(750), hero->alias);
							GUI_output(g_dtp2);
						} else {
							if (hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id == ITEM_NONE) {
								sprintf(g_text_output_buf, get_tx(60), hero->alias);
							} else {
								sprintf(g_text_output_buf, get_tx(31), hero->alias,
									GUI_names_grammar((signed short)0x8002, hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id, 0));
							}

							refresh_screen_size();
							tw_bak = g_textbox_width;
							g_textbox_width = 6;

							selected = GUI_radio(g_text_output_buf, radio_i,
									g_radio_name_list[0], g_radio_name_list[1],
									g_radio_name_list[2], g_radio_name_list[3],
									g_radio_name_list[4], g_radio_name_list[5],
									g_radio_name_list[6], g_radio_name_list[7],
									g_radio_name_list[8], g_radio_name_list[9],
									g_radio_name_list[10], g_radio_name_list[11],
									g_radio_name_list[12], g_radio_name_list[13],
									g_radio_name_list[14], g_radio_name_list[15]);

							g_textbox_width = tw_bak;

							update_mouse_cursor();

							if (selected != -1) {

								/* subtract 2 BP */
								hero->bp_left -= 2;
								move_item(HERO_INVENTORY_SLOT_LEFT_HAND, slots[selected -1], hero);
							}
						}
					} else {
						/* no BP left */
						sprintf(g_dtp2,	get_tx(17), hero->alias);
						GUI_output(g_dtp2);
					}

			} else if (selected == FIG_ACTION_EXCHANGE_WEAPON) {
				/* CHANGE WEAPON / WAFFE WECHSELN */
					if (hero->bp_left >= 2) {

						radio_i = 0;

						for (slot_no = HERO_INVENTORY_SLOT_KNAPSACK_1; slot_no < NR_HERO_INVENTORY_SLOTS; slot_no++) {

							weapon_id = hero->inventory[slot_no].item_id;

							if (g_itemsdat[weapon_id].flags.weapon) {

								slots[radio_i] = slot_no;

								g_radio_name_list[radio_i] = (g_dtp2 + 40 * radio_i);

								sprintf(g_radio_name_list[radio_i],
									g_space_separated_strings, /* "%s %s" */
									GUI_name_singular(get_itemname(weapon_id)),
									hero->inventory[slot_no].flags.broken ? get_ttx(478) : g_empty_string3);

								radio_i++;
							}
						}

						if (radio_i == 0) {
							sprintf(g_dtp2,	get_tx(1), hero->alias);
							GUI_output(g_dtp2);
						} else {
							sprintf(g_text_output_buf, get_tx(2), hero->alias,
								GUI_names_grammar((signed short)0x8002, hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id, 0));

							refresh_screen_size();
							tw_bak = g_textbox_width;
							g_textbox_width = 6;

							selected = GUI_radio(g_text_output_buf, radio_i,
									g_radio_name_list[0], g_radio_name_list[1],
									g_radio_name_list[2], g_radio_name_list[3],
									g_radio_name_list[4], g_radio_name_list[5],
									g_radio_name_list[6], g_radio_name_list[7],
									g_radio_name_list[8], g_radio_name_list[9],
									g_radio_name_list[10], g_radio_name_list[11],
									g_radio_name_list[12], g_radio_name_list[13],
									g_radio_name_list[14], g_radio_name_list[15]);

							g_textbox_width = tw_bak;

							update_mouse_cursor();

							if (selected != -1) {

								/* subtract 2 BP */
								hero->bp_left -= 2;

								rwt1 = FIG_get_range_weapon_type(hero);

								move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, slots[selected - 1], hero);

								rwt2 = FIG_get_range_weapon_type(hero);

								if (rwt1 != rwt2) {

									ptr = FIG_get_fighter(hero->fighter_id);

									if (rwt2 != -1) {
										ptr->nvf_no = g_nvftab_figures_rangeweapon[hero->sprite_no - 1][rwt2][hero->viewdir];
									} else {
										ptr->nvf_no = hero->viewdir;
									}

									ptr->reload = -1;

									draw_fight_screen_pal(0);
								}
							}
						}
					} else {
						/* no BP left */
						sprintf(g_dtp2, get_tx(17), hero->alias);
						GUI_output(g_dtp2);
					}
			} else if (selected == FIG_ACTION_CHECK_VALUES) {
				/* CHECK VALUES / WERTE PRUEFEN */

				rwt1 = weapon_check(hero);

				if (rwt1 == -1) {

					rwt1 = FIG_get_range_weapon_type(hero);

					if (rwt1 == -1) {

						/* calculate AT and PA values for "BARE HANDS" */
						at = hero->at_weapon[0] - hero->rs_be / 2;
						if (hero->rs_be & 1) {
							at--;
						}

						pa = hero->pa_weapon[0] - hero->rs_be / 2;

						/* set damage bounds: 1W6 */
						damage_lo = 1;
						damage_hi = 6;

					} else {

						/* calculate AT and PA values for range weapons */
						at = hero->at_weapon[hero->w_type] - hero->rs_be / 2 + hero->w_at_mod;

						if (hero->rs_be & 1) {
							at--;
						}
						pa = hero->pa_weapon[hero->w_type] - hero->rs_be / 2 + hero->w_pa_mod;


						p_itemsdat = &g_itemsdat[hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id];
						weapon = &g_weapons_table[p_itemsdat->table_index];

						calc_damage_range(weapon->damage_d6, 6, weapon->damage_const, &damage_lo, &damage_hi);

					}
				} else {
					/* calculate AT and PA values for melee weapons */
					at = hero->at_weapon[hero->w_type] - hero->rs_be / 2 + hero->w_at_mod;

					if (hero->rs_be & 1) {
						at--;
					}
					pa = hero->pa_weapon[hero->w_type] - hero->rs_be / 2 + hero->w_pa_mod;


					p_itemsdat = &g_itemsdat[hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id];
					weapon = &g_weapons_table[p_itemsdat->table_index];

					calc_damage_range(weapon->damage_d6, 6, weapon->damage_const, &damage_lo, &damage_hi);

					/* "THE SWORD GRIMRING" gets a damage bonus + 5 in the final fight */
					if ((hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id == ITEM_GRIMRING) && (g_current_fight_no == FIGHTS_F144)) {
						damage_lo += 5;
						damage_hi += 5;
					}

					weapon_id = hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod - weapon->damage_kk_bonus;

					if (weapon_id > 0) {
						damage_lo += weapon_id;
						damage_hi += weapon_id;
					}
				}

				if (at < 0) {
					at = 0;
				}

				if (pa < 0) {
					pa = 0;
				}

				sprintf(g_dtp2,	get_tx(0), hero->alias,
					/* typus */
					get_ttx((hero->sex != 0 ? 0x251 : 9) + hero->typus),
					/* level */
					hero->level,
					/* AT and PA value */
					at, pa,
					/* RS */
					hero->rs_bonus1,
					/* weapon name */
					GUI_name_singular(get_itemname(hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id)),
					/* damage bounds */
					damage_lo, damage_hi,
					/* LE */
					hero->le, hero->le_max,
					/* AE */
					hero->ae, hero->ae_max,
					/* poison */
					hero_is_poisoned(hero) ? get_tx(36) : (char*)g_empty_string4,
					/* renegade */
					hero->flags.renegade == 1 ? get_tx(38) : (char*)g_empty_string5);

				GUI_output(g_dtp2);

			} else if (selected == FIG_ACTION_WAIT) {
				/* WAIT / ABWARTEN */

				done = 1;
				hero->action_id = FIG_ACTION_WAIT;
				/* set BP to 0 */
				hero->bp_left = 0;
				/* set target id to 0 */
				hero->enemy_id = 0;

			} else if (selected == FIG_ACTION_COMPUTER_FIGHT) {
				/* COMPUTER FIGHT / COMPUTERKAMPF */

				if (g_current_fight_no != FIGHTS_F144) {

					refresh_screen_size();

					/* use magic in autofight */
					g_autofight_magic = GUI_bool(get_tx(53));

					update_mouse_cursor();

					g_autofight = 1;
				}

			} else if (selected == FIG_ACTION_DROP_ITEM) {

				/* DROP ITEM / GGST. WEGWERFEN */
				if (hero->bp_left >= 1) {

					radio_i = 0;

					for (slot_no = HERO_INVENTORY_SLOT_KNAPSACK_1; slot_no < NR_HERO_INVENTORY_SLOTS; slot_no++) {

						weapon_id = hero->inventory[slot_no].item_id;

						if (weapon_id != 0) {

							slots[radio_i] = slot_no;

							g_radio_name_list[radio_i] = (g_dtp2 + 30 * radio_i);

							strcpy(g_radio_name_list[radio_i], GUI_name_singular(get_itemname(weapon_id)));

							radio_i++;
						}
					}

					if (radio_i == 0) {
						sprintf(g_dtp2,	get_ttx(750), hero->alias);
						GUI_output(g_dtp2);
					} else {
						sprintf(g_text_output_buf, get_tx(47), hero->alias);

						refresh_screen_size();
						tw_bak = g_textbox_width;
						g_textbox_width = 6;

						selected = GUI_radio(g_text_output_buf, radio_i,
								g_radio_name_list[0], g_radio_name_list[1],
								g_radio_name_list[2], g_radio_name_list[3],
								g_radio_name_list[4], g_radio_name_list[5],
								g_radio_name_list[6], g_radio_name_list[7],
								g_radio_name_list[8], g_radio_name_list[9],
								g_radio_name_list[10], g_radio_name_list[11],
								g_radio_name_list[12], g_radio_name_list[13],
								g_radio_name_list[14], g_radio_name_list[15]);

						g_textbox_width = tw_bak;

						update_mouse_cursor();

						if (selected != -1) {

							drop_item(hero, slots[selected -1], -1);

							/* subtract 1 BP */
							hero->bp_left--;
						}
					}
				} else {
					/* no BP left */
					sprintf(g_dtp2, get_tx(17), hero->alias);
					GUI_output(g_dtp2);
				}

			} else if (selected == FIG_ACTION_QUIT_AND_LOAD) {
				/* QUIT AND RELOAD / VERLASSEN / LADEN */

				refresh_screen_size();

				if (GUI_bool(get_tx(52))) {
					done = 1;
					g_in_fight = 0;
					g_game_state = GAME_STATE_FIGQUIT;
				}

				update_mouse_cursor();

			} else if (selected == FIG_ACTION_REPEAT_OPTION) {
				/* REPEAT OPTIONS / ALTE OPTION */

				done = 1;

				/* check last action and target_id */
				if (((hero->action_id == FIG_ACTION_SPELL) || (hero->action_id == FIG_ACTION_MELEE_ATTACK) ||
					(hero->action_id == FIG_ACTION_RANGE_ATTACK)) && (hero->enemy_id > 0))
				{

					/* TODO: check fighter_id upper bound */
					//if (((hero->enemy_id >= 10) && g_enemy_sheets[hero->enemy_id - 10].flags.dead) || /* check 'dead' flag */
					if (((hero->enemy_id >= 10) && g_enemy_sheets[hero->enemy_id].flags.dead) || /* check 'dead' flag */
						((hero->enemy_id < 10) && hero_dead((Bit8u*)get_hero(hero->enemy_id - 1))))
					{

						GUI_output(get_tx(29));
						hero->action_id = FIG_ACTION_WAIT;
						hero->enemy_id = 0;
						done = 0;

					/* TODO: check fighter_id upper bound */
					} else if (((hero->enemy_id >= 10) && ((struct enemy_sheet*)(&g_enemy_sheets[hero->enemy_id] - 10))->flags.scared) || /* check 'scared' flag */
						((hero->enemy_id < 10) && (((struct struct_hero*)get_hero(hero->enemy_id - 1))->flags.scared)))
					{

						/* GUI_output(get_tx(29)); */
						hero->action_id = FIG_ACTION_WAIT;
						hero->enemy_id = 0;
						done = 0;

					} else if (((hero->action_id == FIG_ACTION_SPELL) ||	(hero->action_id == FIG_ACTION_RANGE_ATTACK)) && !check_hero_range_attack(hero, hero_pos))
					{
						/* GUI_output(get_tx(29)); */
						hero->action_id = FIG_ACTION_WAIT;
						hero->enemy_id = 0;
						done = 0;
					}
				}
			}
		}
	}

	/* final fight vs. Orkchampion */
	if ((g_current_fight_no == FIGHTS_F144) &&
		(get_hero_index(gs_main_acting_hero) != hero_pos) &&
		((hero->action_id == FIG_ACTION_MELEE_ATTACK) ||
			(hero->action_id == FIG_ACTION_RANGE_ATTACK) ||
			(hero->action_id == FIG_ACTION_SPELL) ||
			(hero->action_id == FIG_ACTION_USE_ITEM)))
	{

		for (slot_no = 0; slot_no < 20; slot_no++) {
			g_enemy_sheets[slot_no].flags.tied = 0;
		}

		g_finalfight_tumult = 1;
	}
}

#if !defined(__BORLANDC__)
}
#endif
