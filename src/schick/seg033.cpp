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
void FIG_menu(Bit8u *hero, signed short hero_pos, signed short x, signed short y)
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
	Bit8u *p_itemsdat;
	Bit8u *p_weapontab;
	Bit8u *spell_description;
	signed short damage_lo;
	signed short damage_hi;
	signed short weapon_id;
	signed short radio_i;
	signed short slot_no;
	Bit8u *ptr;
	signed short tw_bak;
	signed short slots[16];

	l1 = 1;
	done = 0;

	while ((done == 0) && (host_readbs(hero + HERO_BP_LEFT) > 0)) {

		if (ds_readbs(FIG_CB_MAKRER_ID) != -1) {
			FIG_remove_from_list(ds_readbs(FIG_CB_MAKRER_ID), 0);
			ds_writeb(FIG_CB_MAKRER_ID, -1);
		}

		FIG_init_list_elem(hero_pos + 1);
		draw_fight_screen_pal(0);

		if ((hero_scared(hero)) || (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_FLEE)) {

			and_ptr_bs(hero + HERO_FLAGS1, 0x7f); /* unset 'tied' flag (why??) */
			and_ptr_bs(hero + HERO_FLAGS1, 0xfb); /* unset 'petrified' flag (why???) */

			if (FIG_find_path_to_target(hero, hero_pos, x, y, 5) != -1) {
				seg036_00ae(hero, hero_pos); /* probably: execute hero movement based on path saved in 'FIG_MOVE_PATHDIR'. */
			}
			done = 1;

		} else if (hero_renegade(hero) || (host_readbs(hero + HERO_NPC_ID) > 0)|| (g_autofight != 0)) {

			host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);

			if (((g_current_fight_no != FIGHTS_F144) || (g_finalfight_tumult)) &&
				(host_readbs(hero + HERO_BP_LEFT) >= 3))
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
				sprintf(g_dtp2, get_tx(18), host_readbs(hero + HERO_BP_LEFT));

				txt_tabpos_bak = ds_readws(TXT_TABPOS1);
				ds_writews(TXT_TABPOS1, g_basepos_x + 204);

				refresh_screen_size();

				weapon_id = host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);

				//if (!item_weapon(get_itemsdat(weapon_id)) || (item_weapon(get_itemsdat(weapon_id)) && test_bit0(hero + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_FLAGS)))) { /* test 'broken' flag */
				if (!item_weapon(get_itemsdat(weapon_id)) || (item_weapon(get_itemsdat(weapon_id)) && inventory_broken(hero + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY)))) { /* test 'broken' flag */
					/* no weapon or weapon broken, use red color for "change weapon" */
					sprintf(g_text_output_buf, (char*)g_red_string1, get_tx(24));
				} else {
					/* good weapon, no special color */
					strcpy(g_text_output_buf, get_tx(24));
				}

				if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 110 <= host_readws(hero + HERO_LOAD)) {
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
				ds_writew(TXT_TABPOS1, txt_tabpos_bak);
				g_basepos_x = 0;
				g_basepos_y = 0;

			} while (selected == -1);

			l1 = 0;

			if (selected == FIG_ACTION_MOVE) {
				/* MOVE / BEWEGEN */

				if (hero_tied(hero)) {
					/* Probe: MU + 2 */
					if (test_attrib(hero, ATTRIB_MU, 2) > 0) {
						/* Success */
						and_ptr_bs(hero + HERO_FLAGS1, 0x7f); /* unset 'tied' flag */

					} else if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU)) > 4) {
						/* Failure */
						/* MU - 2 for 7 hours */
						slot_no = get_free_mod_slot();
						set_mod_slot(slot_no, HOURS(7), hero + (HERO_ATTRIB + 3 * ATTRIB_MU), -2, (signed char)hero_pos);
					}
				}

				if (!hero_tied(hero)) {

					host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);

					if (host_readbs(hero + HERO_BP_LEFT) > 0) {
						/* let the player select a move destination */

						refresh_screen_size();
#if !defined(__BORLANDC__)
						/* BE-fix */
						x = host_readws((Bit8u*)&x);
						y = host_readws((Bit8u*)&y);
#endif
						FIG_move_hero(hero, hero_pos, (Bit8u*)&x, (Bit8u*)&y);

#if !defined(__BORLANDC__)
						/* BE-fix */
						x = host_readws((Bit8u*)&x);
						y = host_readws((Bit8u*)&y);
#endif
						update_mouse_cursor();

						/* Moving destroys an active 'Chamaelioni' spell */
						and_ptr_bs(hero + HERO_FLAGS1, 0xef); /* unset 'chamaelioni' flag.  (???) */
						/* TODO: What if the target square agreed with the starting square (such that no movement has happened? */

					} else {
						/* no BP left */
						sprintf(g_dtp2, get_tx(17), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);
					}

				} else {
					sprintf(g_dtp2,	get_tx(7), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);
				}
			} else if (selected == FIG_ACTION_MELEE_ATTACK) {
				/* ATTACK / ANGRIFF */

				if (host_readbs(hero + HERO_BP_LEFT) >= 3) {

					target_x = x;
					target_y = y;
					target_id = -5;
					if ((range_weapon = FIG_get_range_weapon_type(hero)) == -1) {
						/* not a range weapon */
						refresh_screen_size();
#if !defined(__BORLANDC__)
						/* BE-fix */
						target_x = host_readws((Bit8u*)&target_x);
						target_y = host_readws((Bit8u*)&target_y);
#endif
						target_id = FIG_cb_select_target((Bit8u*)&target_x, (Bit8u*)&target_y, 1);
#if !defined(__BORLANDC__)
						/* BE-fix */
						target_x = host_readws((Bit8u*)&target_x);
						target_y = host_readws((Bit8u*)&target_y);
#endif
						update_mouse_cursor();
					} else {
						if (range_attack_check_ammo(hero, 1)) {
							/* a range weapon */
							refresh_screen_size();
#if !defined(__BORLANDC__)
							/* BE-fix */
							target_x = host_readws((Bit8u*)&target_x);
							target_y = host_readws((Bit8u*)&target_y);
#endif
							target_id = FIG_cb_select_target((Bit8u*)&target_x, (Bit8u*)&target_y, 99);
#if !defined(__BORLANDC__)
							/* BE-fix */
							target_x = host_readws((Bit8u*)&target_x);
							target_y = host_readws((Bit8u*)&target_y);
#endif
							update_mouse_cursor();
						}
					}

					if (target_id != -5) {

						if ((target_id <= 0) || (target_id >= 50)) {
							GUI_output(get_tx(28));
						} else if (target_id == (hero_pos + 1)) {
							GUI_output(get_tx(3));
						} else if (((target_id < 10) && hero_dead(get_hero(target_id - 1))) ||
								((target_id >= 10) && (target_id < 30) &&
										/* mushroom or dead */
										(test_bit0(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_FLAGS1) + SIZEOF_ENEMY_SHEET * target_id) || /* check 'dead' flag */
										test_bit6(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_FLAGS1) + SIZEOF_ENEMY_SHEET * target_id))) || /* check 'mushroom' flag */
								((target_id >= 30) &&
										/* mushroom or dead */
										(test_bit0(p_datseg + ((ENEMY_SHEETS - 30*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_FLAGS1) + SIZEOF_ENEMY_SHEET * target_id) || /* check 'dead' flag */
										test_bit6(p_datseg + ((ENEMY_SHEETS - 30*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_FLAGS1) + SIZEOF_ENEMY_SHEET * target_id)))) /* check 'mushroom' flag */
						{
							GUI_output(get_tx(29));

							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);

							/* set target id to 0 */
							host_writeb(hero + HERO_ENEMY_ID, 0);

						} else if ((range_weapon != -1) && (calc_beeline(x, y, target_x, target_y) < 2)) {

							GUI_output(get_ttx(508));

							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);

							/* set target id to 0 */
							host_writeb(hero + HERO_ENEMY_ID, 0);

						} else {
							host_writeb(hero + HERO_ATTACK_TYPE, 0);

							if (range_weapon == -1) {

								selected = -1;

								do {
									refresh_screen_size();

									selected = GUI_radio(get_ttx(584), 3,
											get_ttx(585),
											get_ttx(586),
											get_ttx(587));
									update_mouse_cursor();

								} while (selected == -1);

								host_writeb(hero + HERO_ATTACK_TYPE, selected == 1 ? 2 : (selected == 2) ? 0 : -2);
							}

							/* set target id */
							host_writeb(hero + HERO_ENEMY_ID, target_id);
							/* set BP to 0 */
							host_writeb(hero + HERO_BP_LEFT, 0);
							host_writeb(hero + HERO_ACTION_ID, (range_weapon > 0 ? FIG_ACTION_RANGE_ATTACK : FIG_ACTION_MELEE_ATTACK));
							done = 1;
						}
					}
				} else {
					/* no BP left */
					sprintf(g_dtp2,	get_tx(17), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);
				}
			} else if (selected == FIG_ACTION_GUARD) {
				/* GUARD / SICHERN */

				if (host_readbs(hero + HERO_BP_LEFT) >= 3) {

					/* set BP to 0 */
					host_writeb(hero + HERO_BP_LEFT, 0);

					host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_GUARD);

					done = 1;
				} else {
					/* no BP left */
					sprintf(g_dtp2, get_tx(17), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);
				}

			} else if (selected == FIG_ACTION_SPELL) {
				/* CAST SPELL / ZAUBERN */

				if (host_readbs(hero + HERO_TYPE) < HERO_TYPE_WITCH) {
					/* not a magic user */
					GUI_output(get_ttx(215));
				} else {

					if (host_readbs(hero + HERO_BP_LEFT) >= 5) {

						refresh_screen_size();
						host_writeb(hero + HERO_SPELL_ID, select_spell(hero, 0));
						update_mouse_cursor();

						if (host_readbs(hero + HERO_SPELL_ID) > 0) {

							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);
							host_writeb(hero + HERO_ENEMY_ID, 0);

							spell_description = p_datseg + SPELL_DESCRIPTIONS + SIZEOF_SPELL_DESCRIPTIONS * host_readbs(hero + HERO_SPELL_ID);

							if (host_readbs(spell_description + SPELL_DESCRIPTIONS_WHERE_TO_USE) == -1) {

								/* not a combat spell */
								GUI_output(get_ttx(592));

							} else {

								if (host_readbs(spell_description + SPELL_DESCRIPTIONS_TARGET_TYPE) != 0) {

									target_x = x;
									target_y = y;
									weapon_id = 1;
									if (host_readbs(spell_description + SPELL_DESCRIPTIONS_RANGE) > 0) {
										weapon_id = 99;
									}
									refresh_screen_size();
#if !defined(__BORLANDC__)
									/* BE-fix */
									target_x = host_readws((Bit8u*)&target_x);
									target_y = host_readws((Bit8u*)&target_y);
#endif
									target_id = FIG_cb_select_target((Bit8u*)&target_x, (Bit8u*)&target_y, weapon_id);
#if !defined(__BORLANDC__)
									/* BE-fix */
									target_x = host_readws((Bit8u*)&target_x);
									target_y = host_readws((Bit8u*)&target_y);
#endif
									update_mouse_cursor();

									if (target_id != -99) {

										if (target_id < 0) {
											GUI_output(get_tx(28));

										} else if ((target_id == 0) &&
											(host_readbs(spell_description + SPELL_DESCRIPTIONS_TARGET_TYPE) != 4))
										{
											GUI_output(get_tx(4));

										} else if ((target_id < 10) &&
											(host_readbs(spell_description + SPELL_DESCRIPTIONS_TARGET_TYPE) != 2) &&
											(host_readbs(spell_description + SPELL_DESCRIPTIONS_TARGET_TYPE) != 3))
										{
											GUI_output(get_tx(5));
										} else if ((target_id >= 10) &&
											(target_id < 50) &&
											(host_readbs(spell_description + SPELL_DESCRIPTIONS_TARGET_TYPE) != 1) &&
											(host_readbs(spell_description + SPELL_DESCRIPTIONS_TARGET_TYPE) != 3))
										{
											GUI_output(get_tx(6));

										} else {
											host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_SPELL);
											host_writeb(hero + HERO_ENEMY_ID, target_id);
											done = 1;
										}
									}
								} else {
									host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_SPELL);
									host_writeb(hero + HERO_ENEMY_ID, 0);
									done = 1;
								}
							}

						}

						if ((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) &&
							(get_spell_cost(host_readbs(hero + HERO_SPELL_ID), 1) > host_readws(hero + HERO_AE)))
						{
							/* not enough AE */
							GUI_output(get_ttx(337));
							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_MOVE);
							done = 0;
						}
					} else {
						/* no BP left */
						sprintf(g_dtp2, get_tx(17), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);
					}
				}
			} else if (selected == FIG_ACTION_USE_ITEM) {
				/* USE ITEM / GGST. BENUTZEN */

					if (host_readbs(hero + HERO_BP_LEFT) >= 3) {

						if (is_in_word_array(host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID), g_attack_items))
						{
							target_x = x;
							target_y = y;
							refresh_screen_size();
#if !defined(__BORLANDC__)
							/* BE-fix */
							target_x = host_readws((Bit8u*)&target_x);
							target_y = host_readws((Bit8u*)&target_y);
#endif
							target_id = FIG_cb_select_target((Bit8u*)&target_x, (Bit8u*)&target_y, 99);
#if !defined(__BORLANDC__)
							/* BE-fix */
							target_x = host_readws((Bit8u*)&target_x);
							target_y = host_readws((Bit8u*)&target_y);
#endif
							update_mouse_cursor();
							host_writeb(hero + HERO_ENEMY_ID, target_id);
						} else {
							host_writeb(hero + HERO_ENEMY_ID, 0);
						}

						if ((host_readbs(hero + HERO_ENEMY_ID) < 0) || host_readbs(hero + HERO_ENEMY_ID) >= 50) {
							host_writeb(hero + HERO_ENEMY_ID, 0);
							GUI_output(get_tx(28));
						} else {
							/* set BP to 0 */
							host_writeb(hero + HERO_BP_LEFT, 0);

							host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_USE_ITEM);
							done = 1;
						}
					} else {
						/* no BP left */
						sprintf(g_dtp2,	get_tx(17), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);
					}
			} else if (selected == FIG_ACTION_EXCHANGE_ITEM) {
				/* EXCHANGE ITEM / GGST. WECHSELN */
					if (host_readbs(hero + HERO_BP_LEFT) >= 2) {

						radio_i = 0;

						for (slot_no = HERO_INVENTORY_SLOT_KNAPSACK_1; slot_no < NR_HERO_INVENTORY_SLOTS; slot_no++) {

							weapon_id = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * slot_no);

							if (weapon_id != 0) {

								slots[radio_i] = slot_no;

								g_radio_name_list[radio_i] = (g_dtp2 + 30 * radio_i);

								strcpy(g_radio_name_list[radio_i], GUI_name_singular(get_itemname(weapon_id)));

								radio_i++;
							}
						}

						if (radio_i == 0) {
							sprintf(g_dtp2,	get_ttx(750), (char*)hero + HERO_NAME2);
							GUI_output(g_dtp2);
						} else {
							if (host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID) == ITEM_NONE) {
								sprintf(g_text_output_buf, get_tx(60), (char*)hero + HERO_NAME2);
							} else {
								sprintf(g_text_output_buf, get_tx(31),
									(char*)hero + HERO_NAME2,
									(char*)GUI_names_grammar((signed short)0x8002, host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID), 0));
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
								sub_ptr_bs(hero + HERO_BP_LEFT, 2);
								move_item(HERO_INVENTORY_SLOT_LEFT_HAND, slots[selected -1], hero);
							}
						}
					} else {
						/* no BP left */
						sprintf(g_dtp2,	get_tx(17), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);
					}
			} else if (selected == FIG_ACTION_EXCHANGE_WEAPON) {
				/* CHANGE WEAPON / WAFFE WECHSELN */
					if (host_readbs(hero + HERO_BP_LEFT) >= 2) {

						radio_i = 0;

						for (slot_no = HERO_INVENTORY_SLOT_KNAPSACK_1; slot_no < NR_HERO_INVENTORY_SLOTS; slot_no++) {

							weapon_id = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * slot_no);

							if (item_weapon(get_itemsdat(weapon_id))) {

								slots[radio_i] = slot_no;

								g_radio_name_list[radio_i] = (g_dtp2 + 40 * radio_i);

								sprintf(g_radio_name_list[radio_i],
									g_space_separated_strings, /* "%s %s" */
									GUI_name_singular(get_itemname(weapon_id)),
									inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * slot_no) ? get_ttx(478) : g_empty_string3);

								radio_i++;
							}
						}

						if (radio_i == 0) {
							sprintf(g_dtp2,	get_tx(1), (char*)hero + HERO_NAME2);
							GUI_output(g_dtp2);
						} else {
							sprintf(g_text_output_buf, get_tx(2), (char*)hero + HERO_NAME2,
								(char*)GUI_names_grammar((signed short)0x8002, host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID), 0));

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
								sub_ptr_bs(hero + HERO_BP_LEFT, 2);

								rwt1 = FIG_get_range_weapon_type(hero);

								move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, slots[selected -1], hero);

								rwt2 = FIG_get_range_weapon_type(hero);

								if (rwt1 != rwt2) {

									ptr = (Bit8u*)(FIG_get_ptr(host_readbs(hero + HERO_FIGHTER_ID)));

									if (rwt2 != -1) {
										host_writeb(ptr + 2,
										ds_readbs((NVFTAB_FIGURES_RANGEWEAPON - 12) + 12 * host_readbs(hero + HERO_SPRITE_NO) + 4 * rwt2 + host_readbs(hero + HERO_VIEWDIR)));
									} else {
										host_writeb(ptr + 2, host_readbs(hero + HERO_VIEWDIR));
									}

									host_writeb(ptr + 0xd, -1);

									draw_fight_screen_pal(0);
								}
							}
						}
					} else {
						/* no BP left */
						sprintf(g_dtp2, get_tx(17), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);
					}
			} else if (selected == FIG_ACTION_CHECK_VALUES) {
				/* CHECK VALUES / WERTE PRUEFEN */

				rwt1 = weapon_check(hero);

				if (rwt1 == -1) {

					rwt1 = FIG_get_range_weapon_type(hero);

					if (rwt1 == -1) {

						/* calculate AT and PA values for "BARE HANDS" */
						at = host_readbs(hero + HERO_AT) - host_readbs(hero + HERO_RS_BE) / 2;
						if (host_readbs(hero + HERO_RS_BE) & 1) {
							at--;
						}

						pa = host_readbs(hero + HERO_PA) - host_readbs(hero + HERO_RS_BE) / 2;

						/* set damage bounds: 1W6 */
						damage_lo = 1;
						damage_hi = 6;

					} else {

						/* calculate AT and PA values for range weapons */
						at = host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WEAPON_TYPE))
							- host_readbs(hero + HERO_RS_BE) / 2
							+ host_readbs(hero + HERO_AT_MOD);

						if (host_readbs(hero + HERO_RS_BE) & 1) {
							at--;
						}
						pa = host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WEAPON_TYPE))
							- host_readbs(hero + HERO_RS_BE) / 2
							+ host_readbs(hero + HERO_PA_MOD);


						p_itemsdat = get_itemsdat(host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID));
						p_weapontab = p_datseg + WEAPONS_TABLE + SIZEOF_WEAPON_STATS * host_readbs(p_itemsdat + ITEM_STATS_TABLE_INDEX);

						calc_damage_range(host_readbs(p_weapontab + WEAPON_STATS_DAMAGE_D6), 6, host_readbs(p_weapontab + WEAPON_STATS_DAMAGE_CONSTANT),
							(Bit8u*)&damage_lo, (Bit8u*)&damage_hi);

					}
				} else {
					/* calculate AT and PA values for melee weapons */
					at = host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WEAPON_TYPE))
						- host_readbs(hero + HERO_RS_BE) / 2
						+ host_readbs(hero + HERO_AT_MOD);

					if (host_readbs(hero + HERO_RS_BE) & 1) {
						at--;
					}
					pa = host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WEAPON_TYPE))
						- host_readbs(hero + HERO_RS_BE) / 2
						+ host_readbs(hero + HERO_PA_MOD);


					p_itemsdat = get_itemsdat(host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID));
					p_weapontab = p_datseg + WEAPONS_TABLE + SIZEOF_WEAPON_STATS * host_readbs(p_itemsdat + ITEM_STATS_TABLE_INDEX);

					calc_damage_range(host_readbs(p_weapontab + WEAPON_STATS_DAMAGE_D6), 6, host_readbs(p_weapontab + WEAPON_STATS_DAMAGE_CONSTANT),
						(Bit8u*)&damage_lo, (Bit8u*)&damage_hi);

					/* "THE SWORD GRIMRING" gets a damage bonus + 5 in the final fight */
					if ((host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID) == ITEM_GRIMRING) && (g_current_fight_no == FIGHTS_F144)) {
						damage_lo += 5;
						damage_hi += 5;
					}

					weapon_id = host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK))
							+ host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))
							- host_readbs(p_weapontab +  WEAPON_STATS_DAMAGE_KK_BONUS);

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

				sprintf(g_dtp2,	get_tx(0), (char*)hero + HERO_NAME2,
					/* typus */
					get_ttx((host_readbs(hero + HERO_SEX) != 0 ? 0x251 : 9) + host_readbs(hero + HERO_TYPE)),
					/* level */
					host_readbs(hero + HERO_LEVEL),
					/* AT and PA value */
					at, pa,
					/* RS */
					host_readbs(hero + HERO_RS_BONUS1),
					/* weapon name */
					(GUI_name_singular(get_itemname(host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID)))),
					/* damage bounds */
					damage_lo, damage_hi,
					/* LE */
					host_readws(hero + HERO_LE), host_readws(hero + HERO_LE_ORIG),
					/* AE */
					host_readws(hero + HERO_AE), host_readws(hero + HERO_AE_ORIG),
					/* poison */
					hero_is_poisoned(hero) ? get_tx(36) : (char*)g_empty_string4,
					/* renegade */
					hero_renegade(hero) == 1 ? get_tx(38) : (char*)g_empty_string5);

				GUI_output(g_dtp2);

			} else if (selected == FIG_ACTION_WAIT) {
				/* WAIT / ABWARTEN */

				done = 1;
				host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
				/* set BP to 0 */
				host_writeb(hero + HERO_BP_LEFT, 0);
				/* set target id to 0 */
				host_writeb(hero + HERO_ENEMY_ID, 0);

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
				if (host_readbs(hero + HERO_BP_LEFT) >= 1) {

					radio_i = 0;

					for (slot_no = HERO_INVENTORY_SLOT_KNAPSACK_1; slot_no < NR_HERO_INVENTORY_SLOTS; slot_no++) {

						weapon_id = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * slot_no);

						if (weapon_id != 0) {

							slots[radio_i] = slot_no;

							g_radio_name_list[radio_i] = (g_dtp2 + 30 * radio_i);

							strcpy(g_radio_name_list[radio_i], GUI_name_singular(get_itemname(weapon_id)));

							radio_i++;
						}
					}

					if (radio_i == 0) {
						sprintf(g_dtp2,	get_ttx(750), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);
					} else {
						sprintf(g_text_output_buf, get_tx(47), (char*)hero + HERO_NAME2);

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
							dec_ptr_bs(hero + HERO_BP_LEFT);
						}
					}
				} else {
					/* no BP left */
					sprintf(g_dtp2, get_tx(17), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);
				}

			} else if (selected == FIG_ACTION_QUIT_AND_LOAD) {
				/* QUIT AND RELOAD / VERLASSEN / LADEN */

				refresh_screen_size();

				if (GUI_bool(get_tx(52))) {
					done = 1;
					g_in_fight = 0;
					ds_writew(GAME_STATE, GAME_STATE_FIGQUIT);
				}

				update_mouse_cursor();

			} else if (selected == FIG_ACTION_REPEAT_OPTION) {
				/* REPEAT OPTIONS / ALTE OPTION */

				done = 1;

				/* check last action and target_id */
				if (((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) ||
					(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_MELEE_ATTACK) ||
					(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK)) && (host_readbs(hero + HERO_ENEMY_ID) > 0))
				{

					/* TODO: check fighter_id upper bound */
					if (((host_readbs(hero + HERO_ENEMY_ID) >= 10)
						&& (test_bit0(p_datseg + ((ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_FLAGS1) + SIZEOF_ENEMY_SHEET * host_readbs(hero + HERO_ENEMY_ID)))) || /* check 'dead' flag */
						((host_readbs(hero + HERO_ENEMY_ID) < 10)
						&& (hero_dead(get_hero(host_readbs(hero + HERO_ENEMY_ID) - 1)))))
					{

						GUI_output(get_tx(29));
						host_writebs(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
						host_writebs(hero + HERO_ENEMY_ID, 0);
						done = 0;

					/* TODO: check fighter_id upper bound */
					} else if (((host_readbs(hero + HERO_ENEMY_ID) >= 10)
						&& (test_bit2(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_FLAGS2 + SIZEOF_ENEMY_SHEET * host_readbs(hero + HERO_ENEMY_ID)))) || /* check 'scared' flag */
						((host_readbs(hero + HERO_ENEMY_ID) < 10)
						&& (hero_scared(get_hero(host_readbs(hero + HERO_ENEMY_ID) - 1)))))
					{

						/* GUI_output(get_tx(29)); */
						host_writebs(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
						host_writebs(hero + HERO_ENEMY_ID, 0);
						done = 0;
					} else if (((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) ||
							(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK)) &&
							!check_hero_range_attack(hero, hero_pos))
					{
						/* GUI_output(get_tx(29)); */
						host_writebs(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
						host_writebs(hero + HERO_ENEMY_ID, 0);
						done = 0;
					}
				}
			}
		}
	}

	if ((g_current_fight_no == FIGHTS_F144) && /* final fight vs. Orkchampion */
		(get_hero_index((Bit8u*)ds_readd(MAIN_ACTING_HERO)) != hero_pos) &&
		((host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_MELEE_ATTACK) || (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK) ||
		(host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_SPELL) || (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_USE_ITEM)))
	{
		for (slot_no = 0; slot_no < 20; slot_no++) {
			and_ds_bs((ENEMY_SHEETS + ENEMY_SHEET_FLAGS1) + SIZEOF_ENEMY_SHEET * slot_no, (signed char)0xdf); /* unset 'tied' flag */
		}

		g_finalfight_tumult = 1;
	}
}

#if !defined(__BORLANDC__)
}
#endif
