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

/* Borlandified and identical */
void reset_item_selector(void)
{
	if (ds_readws(STATUS_PAGE_MODE) < 3) {

		/* remove the previous border */
		do_border(g_vga_memstart,
				ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
				ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
				ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
				ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
				30);

		/* mark the actual selected item */
		ds_writeb(STATUSPAGE_SELITEM1_NO, ds_writeb(STATUSPAGE_SELITEM3_NO, 7));

		/* set the new red border */
		do_border(g_vga_memstart,
				ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
				ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
				ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
				ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
				9);

		ds_writebs(STATUSPAGE_SELITEM4_NO, -1);
		ds_writebs(STATUSPAGE_SELITEM2_NO, 23);
	}

	ds_writed(CURRENT_CURSOR, ds_writed(CURRENT_CURSOR_BAK, (Bit32u)(p_datseg + DEFAULT_MOUSE_CURSOR)));
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
	Bit8u *hero2;
	Bit8u *hero1;
	signed short flag3;
	signed short l1;
	signed short tw_bak;
	signed short flag4;
	struct nvf_desc nvf;

	flag1 = 0;
	flag2 = 1;
	flag3 = 0;
	flag4 = 0;
	ds_writew(MOUSE1_DOUBLECLICK, 0);

	hero1 = hero2 = get_hero(hero_pos);

	if (!host_readbs(hero2 + HERO_TYPE) || ds_readw(STATUSMENU_ALLOWED) == 0) {
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

	ds_writew(REQUEST_REFRESH, 1);
	ds_writew(ACTION, 0);
	ds_writew(STATUS_PAGE_MODE, 1);

	while (flag1 == 0) {

		if (ds_readw(REQUEST_REFRESH) != 0 || flag2 != 0) {

			ds_writew(STATUS_PAGE_HERO, hero_pos);

			update_mouse_cursor();

			status_show(hero_pos);

			g_status_page_hunger = g_status_page_thirst = -1;

			update_status_bars();

			if (ds_readws(STATUS_PAGE_MODE) < 3) {

				if (hero1 == hero2) {
					/* set the new red border */
					do_border(g_vga_memstart,
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
						9);
				}

				if (host_readws(hero1 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO)) != ITEM_NONE) {

					sprintf((char*)g_dtp2,
						/* "%s %s " */
						(char*)(p_datseg + EXTRASPACE_SEPARATED_STRINGS),
						(char*)GUI_name_singular(get_itemname(host_readws(hero1 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO)))),
						!is_in_word_array(
						    host_readws(hero1 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						    g_wearable_items_index[host_readbs(hero2 + HERO_TYPE) - 1]) ? ((char*)p_datseg + EMPTY_STRING8) : get_tx2(66));

					if (item_weapon(get_itemsdat(host_readws(hero1 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO))))) {
						strcat((char*)g_dtp2,
							get_ttx(48 + host_readbs(get_itemsdat(host_readws(hero1 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO))) + 3)));
					}

					GUI_print_string(g_dtp2, 16, 192);
				}

				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {
					/* set the new ??? border */
					do_border(g_vga_memstart,
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
						8);
				}
			}

			refresh_screen_size();

			ds_writew(REQUEST_REFRESH, flag2 = 0);
		}

		handle_input();

		/* RIGHT_KEY */
		if ((ds_readws(ACTION) == ACTION_ID_RIGHT) && (gs_group_member_counts[gs_current_group] > 1)) {

			/* set hero_pos to the next possible hero */
			do {
				hero_pos++;

				if (hero_pos > 6) hero_pos = 0;

			} while (!host_readbs(get_hero(hero_pos) + HERO_TYPE) ||
					host_readbs(get_hero(hero_pos) + HERO_GROUP_NO) != gs_current_group ||
					(host_readbs(get_hero(hero_pos) + HERO_TYPE) < HERO_TYPE_WITCH && ds_readws(STATUS_PAGE_MODE) > 3));


			if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

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

			ds_writew(REQUEST_REFRESH, 1);
		}

		/* LEFT_KEY */
		if ((ds_readws(ACTION) == ACTION_ID_LEFT) && (gs_group_member_counts[gs_current_group] > 1)) {

			/* set hero_pos to the next possible hero */
			do {
				hero_pos--;

				if (hero_pos < 0) hero_pos = 6;
			} while (!host_readbs(get_hero(hero_pos) + HERO_TYPE) ||
					host_readbs(get_hero(hero_pos) + HERO_GROUP_NO) != gs_current_group ||
					(host_readbs(get_hero(hero_pos) + HERO_TYPE) < HERO_TYPE_WITCH && ds_readws(STATUS_PAGE_MODE) > 3));


			if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

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

			ds_writew(REQUEST_REFRESH, 1);
		}

		if (ds_readws(STATUS_PAGE_MODE) < 3) {

			/* UP_KEY */
			if (ds_readws(ACTION) == ACTION_ID_UP) {

				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

					if (!ds_readbs(STATUSPAGE_SELITEM4_NO)) {
						ds_writeb(STATUSPAGE_SELITEM4_NO, 24);
					} else {
						dec_ds_bs_post(STATUSPAGE_SELITEM4_NO);
					}
				} else {
					if (ds_readbs(STATUSPAGE_SELITEM3_NO) == 0) {
						ds_writeb(STATUSPAGE_SELITEM3_NO, 22);
					} else {
						dec_ds_bs_post(STATUSPAGE_SELITEM3_NO);
					}
				}
			}

			/* DOWN_KEY */
			if (ds_readws(ACTION) == ACTION_ID_DOWN) {

				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

					if (hero1 != hero2) {

						if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 24) {
							ds_writeb(STATUSPAGE_SELITEM4_NO, 0);
						} else {
							inc_ds_bs_post(STATUSPAGE_SELITEM4_NO);
						}
					} else {
						if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 24) {
							ds_writeb(STATUSPAGE_SELITEM4_NO, 0);
						} else {
							inc_ds_bs_post(STATUSPAGE_SELITEM4_NO);
						}
					}
				} else {
					if (ds_readbs(STATUSPAGE_SELITEM3_NO) == 22) {
						ds_writeb(STATUSPAGE_SELITEM3_NO, 0);
					} else {
						inc_ds_bs_post(STATUSPAGE_SELITEM3_NO);
					}
				}
			}

			if (ds_readws(ACTION) >= 128 && ds_readws(ACTION) <= 152)
			{
				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {
					ds_writeb(STATUSPAGE_SELITEM4_NO, ds_readws(ACTION) + 128);
					ds_writew(ACTION, ACTION_ID_RETURN);
				} else if (ds_readws(ACTION) <= 150) {
					ds_writeb(STATUSPAGE_SELITEM3_NO, ds_readws(ACTION) + 128);
					ds_writew(ACTION, ACTION_ID_RETURN);
				}
			}

			if (ds_readbs(STATUSPAGE_SELITEM1_NO) != ds_readbs(STATUSPAGE_SELITEM3_NO) && hero1 == hero2) {

				/* set the new ??? border */
				do_border(g_vga_memstart,
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM1_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM1_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM1_NO)) + 17,
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM1_NO)) + 17,
					30);
				/* set the new ??? border */
				do_border(g_vga_memstart,
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
					9);

				ds_writeb(STATUSPAGE_SELITEM1_NO, ds_readbs(STATUSPAGE_SELITEM3_NO));

				memset(g_dtp2, ' ', 60);
				host_writeb((Bit8u*)g_dtp2 + 60, 0);
				GUI_print_string(g_dtp2, 16, 192);

				if (host_readws(hero2 + HERO_INVENTORY + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO))) {
					sprintf((char*)g_dtp2,
						/* "%s %s " */
						(char*)(p_datseg + EXTRASPACE_SEPARATED_STRINGS2),
						(char*)GUI_name_singular(get_itemname(host_readws(hero2 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO)))),
						!is_in_word_array(
						    host_readws(hero2 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						    g_wearable_items_index[host_readbs(hero2 + HERO_TYPE) - 1]) ? ((char*)p_datseg + EMPTY_STRING9) : get_tx2(66));

					if (item_weapon(get_itemsdat(host_readws(hero1 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO))))) {
						strcat((char*)g_dtp2,
							get_ttx(48 + host_readbs(get_itemsdat(host_readws(hero1 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO))) + 3)));
					}

					GUI_print_string(g_dtp2, 16, 192);
				}
			}

			if (ds_readbs(STATUSPAGE_SELITEM2_NO) != ds_readbs(STATUSPAGE_SELITEM4_NO) && ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

				/* set the new ??? border */
				do_border(g_vga_memstart,
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM2_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM2_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM2_NO)) + 17,
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM2_NO)) + 17,
					30);

				if (hero1 == hero2) {

					/* set the new ??? border */
					do_border(g_vga_memstart,
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM3_NO)) + 17,
						9);
				}


				/* set the new ??? border */
				do_border(g_vga_memstart,
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
					ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
					ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
					8);

				ds_writeb(STATUSPAGE_SELITEM2_NO, ds_readbs(STATUSPAGE_SELITEM4_NO));
			}

			if (ds_readws(ACTION) == ACTION_ID_RETURN) {
				if (ds_readbs(STATUSPAGE_SELITEM4_NO) != -1) {

					if (flag4 != 0) {

						if (ds_readbs(STATUSPAGE_SELITEM4_NO) < 23) {
							pass_item(hero1, ds_readbs(STATUSPAGE_SELITEM3_NO), hero2, ds_readbs(STATUSPAGE_SELITEM4_NO));
							ds_writew(REQUEST_REFRESH, 1);
						} else if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 23) { /* eye icon */
							print_item_description(hero1, ds_readbs(STATUSPAGE_SELITEM3_NO));
						} else if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 24) { /* mouth icon */
							consume(hero1, hero2, ds_readbs(STATUSPAGE_SELITEM3_NO));
						}

						/* set the new ??? border */
						do_border(g_vga_memstart,
							ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
							ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
							ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
							ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
							30);

						flag4 = 0;
						hero1 = hero2;

					} else {
						if (ds_readbs(STATUSPAGE_SELITEM4_NO) < 23) {
							move_item(ds_readbs(STATUSPAGE_SELITEM3_NO), ds_readbs(STATUSPAGE_SELITEM4_NO), hero2);
							ds_writew(REQUEST_REFRESH, 1);
						} else if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 23) { /* eye icon */
							print_item_description(hero2, ds_readbs(STATUSPAGE_SELITEM3_NO));
						} else if (ds_readbs(STATUSPAGE_SELITEM4_NO) == 24) { /* mouth icon */
							consume(hero2, hero2, ds_readbs(STATUSPAGE_SELITEM3_NO));
						}

						/* set the new ??? border */
						do_border(g_vga_memstart,
							ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
							ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
							ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
							ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
							30);
					}

					reset_item_selector();
					flag2 = 1;
				} else {

					ds_writeb(STATUSPAGE_SELITEM4_NO, 23);

					/* set the new ??? border */
					do_border(g_vga_memstart,
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)),
						ds_readws(INVSLOT_BORDERXY_TABLE + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
						ds_readws(INVSLOT_BORDERXY_TABLE + 2 + 4 * ds_readbs(STATUSPAGE_SELITEM4_NO)) + 17,
						8);

					if (host_readws(hero2 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO))) {

						nvf.dst = g_icon;
						nvf.src = g_buffer10_ptr;
						nvf.type = 0;
						nvf.width = (Bit8u*)&width;
						nvf.height = (Bit8u*)&height;
						nvf.no = host_readws(get_itemsdat(host_readws(hero2 + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * ds_readbs(STATUSPAGE_SELITEM3_NO))));
						process_nvf(&nvf);

						make_ggst_cursor(g_icon);

						ds_writed(CURRENT_CURSOR, (Bit32u)(p_datseg + GGST_CURSOR));
					}
				}
			}
		}

		/* check if the hero is diseased and print a message */
		if (ds_readws(STATUS_PAGE_MODE) == 1 &&
			ds_readws(ACTION) == ACTION_ID_240 &&
			hero_is_diseased(hero2))
		{
			sprintf((char*)g_dtp2, get_tx2(25 + hero_is_diseased(hero2)), (char*)(hero2 + HERO_NAME2),
				GUI_get_ptr(host_readbs(hero2 + HERO_SEX), 1));

			GUI_output((char*)g_dtp2);

		}

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

			ds_writed(CURRENT_CURSOR_BAK, ds_readd(CURRENT_CURSOR));
			ds_writed(CURRENT_CURSOR, (Bit32u)(p_datseg + DEFAULT_MOUSE_CURSOR));

			switch (ds_readws(STATUS_PAGE_MODE)) {
			case 1: {
				/* from start-page */

				if (ds_readws(GAME_MODE) == GAME_MODE_ADVANCED) {
					ds_writed((RADIO_NAME_LIST + 4 * 0), (Bit32u)get_tx2(19));
					ds_writed((RADIO_NAME_LIST + 4 * 1), (Bit32u)get_tx2(20));
					ds_writed((RADIO_NAME_LIST + 4 * 2), (Bit32u)get_tx2(21));

					flag3 = hero_is_diseased(hero2);

					if (flag3 != 0) {
						ds_writed((RADIO_NAME_LIST + 4 * 3), (Bit32u)get_tx2(25));
						ds_writed((RADIO_NAME_LIST + 4 * 4), (Bit32u)get_tx2(15));
						l1 = 10;
					} else {
						ds_writed((RADIO_NAME_LIST + 4 * 3), (Bit32u)get_tx2(15));
						l1 = 9;
					}
				} else {

					if ((flag3 = hero_is_diseased(hero2))) {
						ds_writed((RADIO_NAME_LIST + 4 * 0), (Bit32u)get_tx2(25));
						ds_writed((RADIO_NAME_LIST + 4 * 1), (Bit32u)get_tx2(15));
						l1 = 7;
					} else {
						ds_writed((RADIO_NAME_LIST + 4 * 0), (Bit32u)get_tx2(15));
						l1 = 6;
					}
				}

				l_di = GUI_radio(NULL, (signed char)l1,
						get_tx2(69),
						get_tx2(16),
						get_tx2(17),
						get_ttx(212),
						get_ttx(213),
						(char*)ds_readd((RADIO_NAME_LIST + 4 * 0)),
						(char*)ds_readd((RADIO_NAME_LIST + 4 * 1)),
						(char*)ds_readd((RADIO_NAME_LIST + 4 * 2)),
						(char*)ds_readd((RADIO_NAME_LIST + 4 * 3)),
						(char*)ds_readd((RADIO_NAME_LIST + 4 * 4)));

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* change name */
						if (host_readbs(hero2 + HERO_NPC_ID) != 0) {
							GUI_output(get_tx2(71));
						} else {
							GUI_input(get_tx2(70), 15);
							strcpy((char*)hero2 + HERO_NAME2, g_text_input_buf);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 2: {
						/* use item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							use_item(ds_readbs(STATUSPAGE_SELITEM3_NO), hero_pos);
							reset_item_selector();
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 3: {
						/* drop item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							drop_item(hero2, ds_readbs(STATUSPAGE_SELITEM3_NO), -1);
							reset_item_selector();
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 4: {
						/* use skill */
						ds_writew(CHEATSKILL_USABLE, 1);
						GUI_use_skill(hero_pos, 0);
						ds_writew(CHEATSKILL_USABLE, 0);

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
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 6: {
						/* show ATPA values */
						if (l1 == 6) {
							flag1 = 1;
							reset_item_selector();
						} else if (l1 == 7) {

							sprintf((char*)g_dtp2, get_tx2(25 + flag3),
								(char*)(hero2 + HERO_NAME2), GUI_get_ptr(host_readbs(hero2 + HERO_SEX), 1));

								GUI_output((char*)g_dtp2);
						} else {
							ds_writew(STATUS_PAGE_MODE, 2);
							ds_writew(REQUEST_REFRESH, 1);
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
							ds_writew(STATUS_PAGE_MODE, 3);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 8: {
						/* show spells */
						if (host_readbs(hero2 + HERO_TYPE) < HERO_TYPE_WITCH) {
							/* not a spellcaster */
							GUI_output(get_ttx(215));
						} else {
							reset_item_selector();
							ds_writew(STATUS_PAGE_MODE, 4);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 9: {
						if (flag3) {

							sprintf((char*)g_dtp2, get_tx2(25 + flag3),
								(char*)(hero2 + HERO_NAME2), GUI_get_ptr(host_readbs(hero2 + HERO_SEX), 1));

							GUI_output((char*)g_dtp2);
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
							use_item(ds_readbs(STATUSPAGE_SELITEM3_NO), hero_pos);
							reset_item_selector();
						}
						break;
					}
					case 2: {
						/* drop item */
						if (hero1 != hero2) {
							GUI_output(get_tx2(68));
						} else {
							drop_item(hero2, ds_readbs(STATUSPAGE_SELITEM3_NO), -1);
							reset_item_selector();
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 3: {
						/* use skill */
						ds_writew(CHEATSKILL_USABLE, 1);
						GUI_use_skill(hero_pos, 0);
						ds_writew(CHEATSKILL_USABLE, 0);

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
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 5: {
						/* TODO: different code is generated here */
						ds_writew(STATUS_PAGE_MODE, 1);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 6: {
						reset_item_selector();
						ds_writew(STATUS_PAGE_MODE, 3);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 7: {
						if (host_readbs(hero2 + HERO_TYPE) < HERO_TYPE_WITCH) {
							/* not a spellcaster */
							GUI_output(get_ttx(215));
						} else {
							reset_item_selector();
							ds_writew(STATUS_PAGE_MODE, 4);
							ds_writew(REQUEST_REFRESH, 1);
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
				l_di = GUI_radio((char*)0, 6,
						get_ttx(212),
						get_tx2(24),
						get_tx2(19),
						get_tx2(21),
						get_ttx(213),
						get_tx2(15));

				if (l_di != -1) {
					switch (l_di) {
					case 1: {
						/* use skill */
						ds_writew(CHEATSKILL_USABLE, 1);
						GUI_use_skill(hero_pos, 0);
						ds_writew(CHEATSKILL_USABLE, 0);

						if (g_tx_file_index == 19) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
						}
						break;
					}
					case 2: {
						ds_writew(STATUS_PAGE_MODE, 1);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 3: {
						ds_writew(STATUS_PAGE_MODE, 2);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 4: {
						if (host_readbs(hero2 + HERO_TYPE) < HERO_TYPE_WITCH) {
							/* not a spellcaster */
							GUI_output(get_ttx(215));
						} else {
							ds_writew(STATUS_PAGE_MODE, 4);
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 5: {
						/* cast spell */
						if (select_magic_user() != -2) {
							load_tx2(ARCHIVE_FILE_CHARTEXT_LTX);
							ds_writew(REQUEST_REFRESH, 1);
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
							ds_writew(REQUEST_REFRESH, 1);
						}
						break;
					}
					case 2: {
						ds_writew(STATUS_PAGE_MODE, 1);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 3: {
						ds_writew(STATUS_PAGE_MODE, 2);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 4: {
						ds_writew(STATUS_PAGE_MODE, 3);
						ds_writew(REQUEST_REFRESH, 1);
						break;
					}
					case 5: {
						if (ds_readws(STATUS_PAGE_MODE) == 4)
							ds_writew(STATUS_PAGE_MODE, 5);
						else
							ds_writew(STATUS_PAGE_MODE, 4);
						ds_writew(REQUEST_REFRESH, 1);
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

			ds_writed(CURRENT_CURSOR, ds_readd(CURRENT_CURSOR_BAK));
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

	ds_writew(REQUEST_REFRESH, 1);
	g_textbox_width = tw_bak;
	g_timers_disabled--;

	if (gs_current_town != TOWNS_NONE) {
		g_fading_state = 3;
	}
}

/* Borlandified and identical */
void status_select_hero(void)
{

	if (ds_readws(STATUSMENU_ALLOWED) == 0 || !gs_group_member_counts[gs_current_group]) {
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
