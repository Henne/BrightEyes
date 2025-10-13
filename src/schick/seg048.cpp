/*
 *      Rewrite of DSA1 v3.02_de functions of seg048 (status menu)
 *      Functions rewritten: 3/3 (complete)
 *
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg046.h"
#include "seg047.h"
#include "seg048.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg106.h"
#include "seg107.h"
#include "seg108.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static struct mouse_cursor *g_current_cursor_bak; // ds:0xe3ae

/* Borlandified and identical */
void reset_item_selector(void)
{
	if (g_status_page_mode < 3) {

		/* remove the previous border */
		do_border(g_vga_memstart,
				g_invslot_borderxy_table[g_statuspage_selitem3_no].x,
				g_invslot_borderxy_table[g_statuspage_selitem3_no].y,
				g_invslot_borderxy_table[g_statuspage_selitem3_no].x + 17,
				g_invslot_borderxy_table[g_statuspage_selitem3_no].y + 17,
				30);

		/* mark the actual selected item */
		g_statuspage_selitem1_no = (g_statuspage_selitem3_no = (7));

		/* set the new red border */
		do_border(g_vga_memstart,
				g_invslot_borderxy_table[g_statuspage_selitem3_no].x,
				g_invslot_borderxy_table[g_statuspage_selitem3_no].y,
				g_invslot_borderxy_table[g_statuspage_selitem3_no].x + 17,
				g_invslot_borderxy_table[g_statuspage_selitem3_no].y + 17,
				9);

		g_statuspage_selitem4_no = -1;
		g_statuspage_selitem2_no = 23;
	}

	g_current_cursor = g_current_cursor_bak = &g_default_mouse_cursor;
}

/* nearly identical, same length */
void status_menu(signed short hero_pos)
{
	signed short l_di;
	signed short flag1;
	signed short flag2;
	signed short width;
	signed short height;
	signed short file_bak;
	struct struct_hero *hero2;
	struct struct_hero *hero1;
	signed short flag3;
	signed short l1;
	signed short tw_bak;
	signed short flag4;
	struct nvf_desc nvf;

	flag1 = 0;
	flag2 = 1;
	flag3 = 0;
	flag4 = 0;
	g_mouse1_doubleclick = 0;

	hero1 = hero2 = get_hero(hero_pos);

	if (!hero2->typus || !g_statusmenu_allowed) {
		return;
	}

	/* disable timers */
	g_timers_disabled++;

	/* set textbox width */
	tw_bak = g_textbox_width;
	g_textbox_width = 3;

	set_audio_track(ARCHIVE_FILE_SUMMARY_XMI);

	file_bak = g_text_file_index;

	load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);

	load_ggsts_nvf();

	g_request_refresh = 1;
	g_action = 0;
	g_status_page_mode = 1;

	while (flag1 == 0) {

		if (g_request_refresh != 0 || flag2 != 0) {

			g_status_page_hero = hero_pos;

			update_mouse_cursor();

			status_show(hero_pos);

			g_status_page_hunger = g_status_page_thirst = -1;

			update_status_bars();

			if (g_status_page_mode < 3) {

				if (hero1 == hero2) {
					/* set the new red border */
					do_border(g_vga_memstart,
						g_invslot_borderxy_table[g_statuspage_selitem3_no].x,
						g_invslot_borderxy_table[g_statuspage_selitem3_no].y,
						g_invslot_borderxy_table[g_statuspage_selitem3_no].x + 17,
						g_invslot_borderxy_table[g_statuspage_selitem3_no].y + 17,
						9);
				}

				if (hero1->inventory[g_statuspage_selitem3_no].item_id != ITEM_NONE) {

					sprintf(g_dtp2, g_extraspace_separated_strings,
						(char*)GUI_name_singular(get_itemname(hero1->inventory[g_statuspage_selitem3_no].item_id)),
						!is_in_word_array(hero1->inventory[g_statuspage_selitem3_no].item_id,
							g_wearable_items_index[hero2->typus - 1]) ? g_empty_string8 : get_tx2(66));

					if (g_itemsdat[hero1->inventory[g_statuspage_selitem3_no].item_id].flags.weapon) {
						strcat(g_dtp2, get_ttx(48 + g_itemsdat[hero1->inventory[g_statuspage_selitem3_no].item_id].subtype));
					}

					GUI_print_string(g_dtp2, 16, 192);
				}

				if (g_statuspage_selitem4_no != -1) {
					/* set the new ??? border */
					do_border(g_vga_memstart,
						g_invslot_borderxy_table[g_statuspage_selitem4_no].x,
						g_invslot_borderxy_table[g_statuspage_selitem4_no].y,
						g_invslot_borderxy_table[g_statuspage_selitem4_no].x + 17,
						g_invslot_borderxy_table[g_statuspage_selitem4_no].y + 17,
						8);
				}
			}

			refresh_screen_size();

			g_request_refresh = flag2 = 0;
		}

		handle_input();

		/* RIGHT_KEY */
		if ((g_action == ACTION_ID_RIGHT) && (gs_group_member_counts[gs_current_group] > 1)) {

			/* set hero_pos to the next possible hero */
			do {
				hero_pos++;

				if (hero_pos > 6) hero_pos = 0;

			} while (!((get_hero(hero_pos))->typus) ||
					((get_hero(hero_pos))->group_no != gs_current_group) ||
					(((get_hero(hero_pos))->typus < HERO_TYPE_WITCH) && (g_status_page_mode > 3)));


			if (g_statuspage_selitem4_no != -1) {

				if (flag4 == 0) {
					hero1 = hero2;
				}

				hero2 = get_hero(hero_pos);
				flag4 = 1;
			} else {
				flag4 = 0;

				hero1 = hero2 = get_hero(hero_pos);

				reset_item_selector();
			}

			g_request_refresh = 1;
		}

		/* LEFT_KEY */
		if ((g_action == ACTION_ID_LEFT) && (gs_group_member_counts[gs_current_group] > 1)) {

			/* set hero_pos to the next possible hero */
			do {
				hero_pos--;

				if (hero_pos < 0) hero_pos = 6;

			} while (!((get_hero(hero_pos))->typus) ||
					((get_hero(hero_pos))->group_no != gs_current_group) ||
					(((get_hero(hero_pos))->typus < HERO_TYPE_WITCH) && (g_status_page_mode > 3)));


			if (g_statuspage_selitem4_no != -1) {

				if (flag4 == 0) {
					hero1 = hero2;
				}

				hero2 = get_hero(hero_pos);
				flag4 = 1;
			} else {
				flag4 = 0;

				hero1 = hero2 = get_hero(hero_pos);

				reset_item_selector();
			}

			g_request_refresh = 1;
		}

		if (g_status_page_mode < 3) {

			/* UP_KEY */
			if (g_action == ACTION_ID_UP) {

				if (g_statuspage_selitem4_no != -1) {

					if (!g_statuspage_selitem4_no) {
						g_statuspage_selitem4_no = 24;
					} else {
						g_statuspage_selitem4_no--;
					}
				} else {
					if (g_statuspage_selitem3_no == 0) {
						g_statuspage_selitem3_no = 22;
					} else {
						g_statuspage_selitem3_no--;
					}
				}
			}

			/* DOWN_KEY */
			if (g_action == ACTION_ID_DOWN) {

				if (g_statuspage_selitem4_no != -1) {

					if (hero1 != hero2) {

						if (g_statuspage_selitem4_no == 24) {
							g_statuspage_selitem4_no = 0;
						} else {
							g_statuspage_selitem4_no++;
						}
					} else {
						if (g_statuspage_selitem4_no == 24) {
							g_statuspage_selitem4_no = 0;
						} else {
							g_statuspage_selitem4_no++;
						}
					}
				} else {
					if (g_statuspage_selitem3_no == 22) {
						g_statuspage_selitem3_no = 0;
					} else {
						g_statuspage_selitem3_no++;
					}
				}
			}

			if (g_action >= 128 && g_action <= 152)
			{
				if (g_statuspage_selitem4_no != -1) {
					g_statuspage_selitem4_no = g_action + 128;
					g_action = ACTION_ID_RETURN;
				} else if (g_action <= 150) {
					g_statuspage_selitem3_no = g_action + 128;
					g_action = ACTION_ID_RETURN;
				}
			}

			if (g_statuspage_selitem1_no != g_statuspage_selitem3_no && hero1 == hero2) {

				/* set the new ??? border */
				do_border(g_vga_memstart,
					g_invslot_borderxy_table[g_statuspage_selitem1_no].x,
					g_invslot_borderxy_table[g_statuspage_selitem1_no].y,
					g_invslot_borderxy_table[g_statuspage_selitem1_no].x + 17,
					g_invslot_borderxy_table[g_statuspage_selitem1_no].y + 17,
					30);
				/* set the new ??? border */
				do_border(g_vga_memstart,
					g_invslot_borderxy_table[g_statuspage_selitem3_no].x,
					g_invslot_borderxy_table[g_statuspage_selitem3_no].y,
					g_invslot_borderxy_table[g_statuspage_selitem3_no].x + 17,
					g_invslot_borderxy_table[g_statuspage_selitem3_no].y + 17,
					9);

				g_statuspage_selitem1_no = g_statuspage_selitem3_no;

				memset(g_dtp2, ' ', 60);
				*(g_dtp2 + 60) = '\0';
				GUI_print_string(g_dtp2, 16, 192);

				if (hero2->inventory[g_statuspage_selitem3_no].item_id) {

					sprintf(g_dtp2, g_extraspace_separated_strings2,
						(char*)GUI_name_singular(get_itemname(hero2->inventory[g_statuspage_selitem3_no].item_id)),
						!is_in_word_array(
						    hero2->inventory[g_statuspage_selitem3_no].item_id,
						    g_wearable_items_index[hero2->typus - 1]) ? g_empty_string9 : get_tx2(66));

					if (g_itemsdat[hero1->inventory[g_statuspage_selitem3_no].item_id].flags.weapon) {

						strcat(g_dtp2, get_ttx(48 + g_itemsdat[hero1->inventory[g_statuspage_selitem3_no].item_id].subtype));
					}

					GUI_print_string(g_dtp2, 16, 192);
				}
			}

			if (g_statuspage_selitem2_no != g_statuspage_selitem4_no && g_statuspage_selitem4_no != -1) {

				/* set the new ??? border */
				do_border(g_vga_memstart,
					g_invslot_borderxy_table[g_statuspage_selitem2_no].x,
					g_invslot_borderxy_table[g_statuspage_selitem2_no].y,
					g_invslot_borderxy_table[g_statuspage_selitem2_no].x + 17,
					g_invslot_borderxy_table[g_statuspage_selitem2_no].y + 17,
					30);

				if (hero1 == hero2) {

					/* set the new ??? border */
					do_border(g_vga_memstart,
						g_invslot_borderxy_table[g_statuspage_selitem3_no].x,
						g_invslot_borderxy_table[g_statuspage_selitem3_no].y,
						g_invslot_borderxy_table[g_statuspage_selitem3_no].x + 17,
						g_invslot_borderxy_table[g_statuspage_selitem3_no].y + 17,
						9);
				}


				/* set the new ??? border */
				do_border(g_vga_memstart,
					g_invslot_borderxy_table[g_statuspage_selitem4_no].x,
					g_invslot_borderxy_table[g_statuspage_selitem4_no].y,
					g_invslot_borderxy_table[g_statuspage_selitem4_no].x + 17,
					g_invslot_borderxy_table[g_statuspage_selitem4_no].y + 17,
					8);

				g_statuspage_selitem2_no = g_statuspage_selitem4_no;
			}

			if (g_action == ACTION_ID_RETURN) {
				if (g_statuspage_selitem4_no != -1) {

					if (flag4 != 0) {

						if (g_statuspage_selitem4_no < 23) {
							pass_item(hero1, g_statuspage_selitem3_no, hero2, g_statuspage_selitem4_no);
							g_request_refresh = 1;
						} else if (g_statuspage_selitem4_no == 23) { /* eye icon */
							print_item_description(hero1, g_statuspage_selitem3_no);
						} else if (g_statuspage_selitem4_no == 24) { /* mouth icon */
							consume(hero1, hero2, g_statuspage_selitem3_no);
						}

						/* set the new ??? border */
						do_border(g_vga_memstart,
							g_invslot_borderxy_table[g_statuspage_selitem4_no].x,
							g_invslot_borderxy_table[g_statuspage_selitem4_no].y,
							g_invslot_borderxy_table[g_statuspage_selitem4_no].x + 17,
							g_invslot_borderxy_table[g_statuspage_selitem4_no].y + 17,
							30);

						flag4 = 0;
						hero1 = hero2;

					} else {
						if (g_statuspage_selitem4_no < 23) {
							move_item(g_statuspage_selitem3_no, g_statuspage_selitem4_no, hero2);
							g_request_refresh = 1;
						} else if (g_statuspage_selitem4_no == HERO_INVENTORY_SLOT_EYE) { /* eye icon */
							print_item_description(hero2, g_statuspage_selitem3_no);
						} else if (g_statuspage_selitem4_no == HERO_INVENTORY_SLOT_MOUTH) { /* mouth icon */
							consume(hero2, hero2, g_statuspage_selitem3_no);
						}

						/* set the new ??? border */
						do_border(g_vga_memstart,
							g_invslot_borderxy_table[g_statuspage_selitem4_no].x,
							g_invslot_borderxy_table[g_statuspage_selitem4_no].y,
							g_invslot_borderxy_table[g_statuspage_selitem4_no].x + 17,
							g_invslot_borderxy_table[g_statuspage_selitem4_no].y + 17,
							30);
					}

					reset_item_selector();
					flag2 = 1;
				} else {

					g_statuspage_selitem4_no = HERO_INVENTORY_SLOT_EYE;

					/* set the new ??? border */
					do_border(g_vga_memstart,
						g_invslot_borderxy_table[g_statuspage_selitem4_no].x,
						g_invslot_borderxy_table[g_statuspage_selitem4_no].y,
						g_invslot_borderxy_table[g_statuspage_selitem4_no].x + 17,
						g_invslot_borderxy_table[g_statuspage_selitem4_no].y + 17,
						8);

					if (hero2->inventory[g_statuspage_selitem3_no].item_id) {

						nvf.dst = g_icon;
						nvf.src = g_buffer10_ptr;
						nvf.type = 0;
						nvf.width = &width;
						nvf.height = &height;
						nvf.no = g_itemsdat[hero2->inventory[g_statuspage_selitem3_no].item_id].gfx;
						process_nvf(&nvf);

						make_ggst_cursor(g_icon);

						g_current_cursor = &g_ggst_cursor;
					}
				}
			}
		}

		/* check if the hero is diseased and print a message */
		if (g_status_page_mode == 1 &&
			g_action == ACTION_ID_240 &&
			hero_is_diseased(hero2))
		{
			sprintf(g_dtp2, get_tx2(25 + hero_is_diseased(hero2)), hero2->alias,
				GUI_get_ptr(hero2->sex, 1));

			GUI_output(g_dtp2);

		}

		if (g_mouse2_event || g_action == ACTION_ID_PAGE_UP) {

			g_current_cursor_bak = g_current_cursor;
			g_current_cursor = &g_default_mouse_cursor;

			switch (g_status_page_mode) {
			case 1: {
				/* from start-page */

				if (g_game_mode == GAME_MODE_ADVANCED) {
					g_radio_name_list[0] = get_tx2(19);
					g_radio_name_list[1] = get_tx2(20);
					g_radio_name_list[2] = get_tx2(21);

					flag3 = hero_is_diseased(hero2);

					if (flag3 != 0) {
						g_radio_name_list[3] = get_tx2(25);
						g_radio_name_list[4] = get_tx2(15);
						l1 = 10;
					} else {
						g_radio_name_list[3] = get_tx2(15);
						l1 = 9;
					}
				} else {

					if ((flag3 = hero_is_diseased(hero2))) {
						g_radio_name_list[0] = get_tx2(25);
						g_radio_name_list[1] = get_tx2(15);
						l1 = 7;
					} else {
						g_radio_name_list[0] = get_tx2(15);
						l1 = 6;
					}
				}

				l_di = GUI_radio(NULL, (signed char)l1,
						get_tx2(69), get_tx2(16), get_tx2(17), get_ttx(212), get_ttx(213),
						g_radio_name_list[0], g_radio_name_list[1], g_radio_name_list[2],
						g_radio_name_list[3], g_radio_name_list[4]);

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* change name */
						if (hero2->npc_id != 0) {
							GUI_output(get_tx2(71));
						} else {
							GUI_input(get_tx2(70), 15);
							strcpy(hero2->alias, g_text_input_buf);
							g_request_refresh = 1;
						}
						break;
					}
					case 2: {
						/* use item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							use_item(g_statuspage_selitem3_no, hero_pos);
							reset_item_selector();
							g_request_refresh = 1;
						}
						break;
					}
					case 3: {
						/* drop item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							drop_item(hero2, g_statuspage_selitem3_no, -1);
							reset_item_selector();
							g_request_refresh = 1;
						}
						break;
					}
					case 4: {
						/* use skill */
						g_cheatskill_usable = 1;
						GUI_use_skill(hero_pos, 0);
						g_cheatskill_usable = 0;

						if (g_tx_file_index == 19) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 5: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							reset_item_selector();
						}
						g_request_refresh = 1;
						break;
					}
					case 6: {
						/* show ATPA values */
						if (l1 == 6) {
							flag1 = 1;
							reset_item_selector();
						} else if (l1 == 7) {

							sprintf(g_dtp2, get_tx2(25 + flag3),
								hero2->alias, GUI_get_ptr(hero2->sex, 1));

								GUI_output(g_dtp2);
						} else {
							g_status_page_mode = 2;
							g_request_refresh = 1;
						}
						break;
					}
					case 7: {
						/* show skills */
						if (l1 == 7) {
							flag1 = 1;
							reset_item_selector();
						} else {
							reset_item_selector();
							g_status_page_mode = 3;
							g_request_refresh = 1;
						}
						break;
					}
					case 8: {
						/* show spells */
						if (hero2->typus < HERO_TYPE_WITCH) {
							/* not a spellcaster */
							GUI_output(get_ttx(215));
						} else {
							reset_item_selector();
							g_status_page_mode = 4;
							g_request_refresh = 1;
						}
						break;
					}
					case 9: {
						if (flag3) {

							sprintf(g_dtp2, get_tx2(25 + flag3),
								hero2->alias, GUI_get_ptr(hero2->sex, 1));

							GUI_output(g_dtp2);
							break;
						}
					}
					case 10: {
						flag1 = 1;
						reset_item_selector();
						break;
					}
					}
				}

				break;
			}
			case 2: {
				/* from ATPA-page */

				l_di = GUI_radio((char*)0, 8,
						get_tx2(16),
						get_tx2(17),
						get_ttx(212),
						get_ttx(213),
						get_tx2(24),
						get_tx2(20),
						get_tx2(21),
						get_tx2(15));

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* use item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							use_item(g_statuspage_selitem3_no, hero_pos);
							reset_item_selector();
						}
						break;
					}
					case 2: {
						/* drop item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							drop_item(hero2, g_statuspage_selitem3_no, -1);
							reset_item_selector();
							g_request_refresh = 1;
						}
						break;
					}
					case 3: {
						/* use skill */
						g_cheatskill_usable = 1;
						GUI_use_skill(hero_pos, 0);
						g_cheatskill_usable = 0;

						if (g_tx_file_index == 19) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 4: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							reset_item_selector();
							g_request_refresh = 1;
						}
						break;
					}
					case 5: {
						/* TODO: different code is generated here */
						g_status_page_mode = 1;
						g_request_refresh = 1;
						break;
					}
					case 6: {
						reset_item_selector();
						g_status_page_mode = 3;
						g_request_refresh = 1;
						break;
					}
					case 7: {
						if (hero2->typus < HERO_TYPE_WITCH) {
							/* not a spellcaster */
							GUI_output(get_ttx(215));
						} else {
							reset_item_selector();
							g_status_page_mode = 4;
							g_request_refresh = 1;
						}
						break;
					}
					case 8: {
						flag1 = 1;
						reset_item_selector();
						break;
					}
					}
				}
				break;
			}
			case 3: {
				/* from skills-page */
				l_di = GUI_radio((char*)NULL, 6, get_ttx(212), get_tx2(24), get_tx2(19), get_tx2(21), get_ttx(213), get_tx2(15));

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* use skill */
						g_cheatskill_usable = 1;
						GUI_use_skill(hero_pos, 0);
						g_cheatskill_usable = 0;

						if (g_tx_file_index == 19) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 2: {
						g_status_page_mode = 1;
						g_request_refresh = 1;
						break;
					}
					case 3: {
						g_status_page_mode = 2;
						g_request_refresh = 1;
						break;
					}
					case 4: {
						if (hero2->typus < HERO_TYPE_WITCH) {
							/* not a spellcaster */
							GUI_output(get_ttx(215));
						} else {
							g_status_page_mode = 4;
							g_request_refresh = 1;
						}
						break;
					}
					case 5: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							g_request_refresh = 1;
						}
						break;
					}
					case 6: {
						flag1 = 1;
						break;
					}
					}
				}
				break;
			}
			case 4:
			case 5:{
				/* from spells-page */
				l_di = GUI_radio((char*)0, 6,
						get_ttx(213),    // Zauber sprechen
						get_tx2(24),     // Grundwerte
						get_tx2(19),     // AT PA Werte
						get_tx2(20),     // Talente
						get_ttx(214),    // Mehr Zauber
						get_tx2(15));    // Zurueck

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							g_request_refresh = 1;
						}
						break;
					}
					case 2: {
						g_status_page_mode = 1;
						g_request_refresh = 1;
						break;
					}
					case 3: {
						g_status_page_mode = 2;
						g_request_refresh = 1;
						break;
					}
					case 4: {
						g_status_page_mode = 3;
						g_request_refresh = 1;
						break;
					}
					case 5: {
						if (g_status_page_mode == 4)
							g_status_page_mode = 5;
						else
							g_status_page_mode = 4;
						g_request_refresh = 1;
						break;
					}
					case 6: {
						flag1 = 1;
						break;
					}
					}
				}
				break;
			}
			}

			g_current_cursor = g_current_cursor_bak;
		}
	}

	/* restore text file except for CHARTEXT.LTX, TAVERN.TLK and except for dialogs */
	if (file_bak != -1 &&
		file_bak != ARCHIVE_FILE_CHARTEXT_LTX &&
		file_bak != ARCHIVE_FILE_TAVERN_TLK &&
		(file_bak < 156 || file_bak > 176))
	{
		load_tx2(file_bak);
	}

	g_request_refresh = 1;
	g_textbox_width = tw_bak;
	g_timers_disabled--;

	if (gs_current_town != TOWNS_NONE) {
		g_fading_state = 3;
	}
}

/* Borlandified and identical */
void status_select_hero(void)
{

	if (!g_statusmenu_allowed || !gs_group_member_counts[gs_current_group]) {
		/* Yes, it was written that way! */
	} else {
		signed short hero_pos = select_hero_from_group(get_ttx(301));

		if (hero_pos != -1) {
			status_menu(hero_pos);
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
