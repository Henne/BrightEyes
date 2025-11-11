/**
 *	Rewrite of DSA1 v3.02_de functions of seg061 (temple, chr-management, miracles)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg061.cpp
 */
#include <stdio.h>
#include <string.h>

#if defined(__BORLANDC__)
#include <DOS.H>
#else
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg047.h"
#include "seg061.h"
#include "seg062.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static int8_t g_god_temples_table_01[2] = { 0x3d, -1 }; // ds:0x6de8, array terminated by -1
static int8_t g_god_temples_table_02[6] = { 0x0b, 0x1c, 0x25, 0x2d, 0x33, -1 }; // ds:0x6dea, array terminated by -1
static int8_t g_god_temples_table_03[11] = { 0x03, 0x0d, 0x0e, 0x21, 0x22, 0x26, 0x29, 0x2c, 0x35, 0x39, -1 }; // ds:0x6df0, array terminated by -1
static int8_t g_god_temples_table_04[16] = { 0x01, 0x08, 0x0a, 0x10, 0x11, 0x14, 0x18, 0x1e, 0x23, 0x2a, 0x2b, 0x2f, 0x30, 0x32, 0x34, -1 }; // ds:0x6dfb, array terminated by -1
static int8_t g_god_temples_table_05[2] = { 0x3e, -1 }; // ds:0x6e0b, array terminated by -1
static int8_t g_god_temples_table_06[2] = { 0x3f, -1 }; // ds:0x6e0d, array terminated by -1
static int8_t g_god_temples_table_07[4] = { 0x12, 0x1a, 0x3a, -1 }; // ds:0x6e0f, array terminated by -1
static int8_t g_god_temples_table_08[2] = { 6, -1 }; // ds:0x6e13, array terminated by -1
static int8_t g_god_temples_table_09[6] = { 0x05, 0x09, 0x13, 0x27, 0x3b, -1 }; // ds:0x6e15, array terminated by -1
static int8_t g_god_temples_table_10[5] = { 0x04, 0x0c, 0x19, 0x3c, -1 }; // ds:0x6e1b, array terminated by -1
static int8_t g_god_temples_table_11[3] = { 0x16, 0x1b, -1 }; // ds:0x6e20, array terminated by -1
static int8_t g_god_temples_table_12[2] = { 0x40, -1 }; // ds:0x6e23, array terminated by -1
static int8_t g_god_temples_table_13[13] = { 0x02, 0x07, 0x0f, 0x15, 0x1d, 0x20, 0x24, 0x28, 0x2e, 0x31, 0x36, 0x38, -1 }; // ds:0x6e25, array terminated by -1
static int8_t g_god_temples_table_14[4] = { 0x17, 0x1f, 0x37, -1 }; // ds:0x6e32, array terminated by -1
static int8_t* g_god_temples_index[15] = {
	NULL,
	g_god_temples_table_01,
	g_god_temples_table_02,
	g_god_temples_table_03,

	g_god_temples_table_04,
	g_god_temples_table_05,
	g_god_temples_table_06,
	g_god_temples_table_07,

	g_god_temples_table_08,
	g_god_temples_table_09,
	g_god_temples_table_10,
	g_god_temples_table_11,

	g_god_temples_table_12,
	g_god_temples_table_13,
	g_god_temples_table_14,
}; // ds:0x6e36; int8_t*
static char g_str_temp_file_wildcard[8] = "TEMP\\%s"; // ds:0x6e72
static char g_str_no_save_in_temple[41] = "IN DIESEM TEMPEL KEIN SPEICHERN M\x99GLICH!"; // ds:0x6e7a


/* REMARK: should be passed as a parameter to asm_miracles() */
signed short g_temple_god; // ds:0xe3f8, id of current temple's god

void do_temple(void)
{
	signed short l_si;
	signed short l_di;
	signed short input;
	signed char done = 0;
	int32_t money;
	int32_t donation;
	signed short game_state;

	g_intemple = g_intemple2 = 0;
	g_request_refresh = 1;

	draw_loc_icons(9, MENU_ICON_HIRE_HERO, MENU_ICON_DISMISS_HERO, MENU_ICON_DELETE_HERO, MENU_ICON_LOAD_GAME, MENU_ICON_SAVE_GAME, MENU_ICON_QUIT_GAME, MENU_ICON_PRAY, MENU_ICON_SACRIFICE, MENU_ICON_LEAVE);

	while (!done) {

		if (g_request_refresh != 0) {

			/* search which god owns this temple */
			g_temple_god = 1;
			for (l_si = 1; l_si < 15; l_si++) {
				if (is_in_byte_array(gs_current_typeindex, g_god_temples_index[l_si]))
				{
					g_temple_god = l_si;
					break;
				}
			}

			draw_main_screen();
			load_ani(3);
			load_tempicon(g_temple_god - 1);
			init_ani(0);
			set_audio_track(ARCHIVE_FILE_TEMPLE_XMI);

			/* draw temple icon */
			g_pic_copy.x1 = g_pic_copy.y1 = 0;
			g_pic_copy.x2 = 40;
			g_pic_copy.y2 = 22;
			g_pic_copy.dst = g_vga_memstart + 88 * 320 + 99;
			g_pic_copy.src = g_buffer8_ptr + 7000;

			call_mouse_bg();
			do_pic_copy(0);
			call_mouse();
			g_pic_copy.dst = g_vga_memstart;

			/* location string */
			sprintf(g_dtp2, get_ttx(235),
				get_ttx(g_temple_god + 21),	/* name of the god */
				(char*)(gs_current_typeindex != 58 ? get_ttx(gs_town_id + 235): get_ttx(622)));

			GUI_print_loc_line(g_dtp2);

			g_request_refresh = 0;
		}

		handle_gui_input();

		/* input window */
		if (g_mouse_rightclick_event || g_action == ACTION_ID_PAGE_UP) {

			l_di = GUI_radio(get_ttx(225), 9,
						get_ttx(226), get_ttx(227), get_ttx(293),
						get_ttx(228), get_ttx(229), get_ttx(230),
						get_ttx(620), get_ttx(296), get_ttx(231)) - 1;

			if (l_di != -2) {
				g_action = (l_di + ACTION_ID_ICON_1);
			}
		}

		if (g_action == ACTION_ID_ICON_9) {
			/* leave temple */
			if (!gs_group_member_counts[gs_current_group]) {
				GUI_output(get_ttx(232));
			} else {
				done = 1;
			}
		} else if (g_action == ACTION_ID_ICON_1) {
			/* add character */
			char_add(gs_current_typeindex);
			draw_status_line();
		} else if (g_action == ACTION_ID_ICON_2) {
			/* let go character */
			char_letgo(gs_current_typeindex);
			draw_status_line();
		} else if (g_action == ACTION_ID_ICON_3) {
			/* erase character */
			char_erase();
		} else if (g_action == ACTION_ID_ICON_4) {
			/* load game */
			if (gs_current_typeindex != 58) {

				do {
					game_state = load_game_state();
				} while (game_state == -1);

				/* location string */
				sprintf(g_dtp2,	get_ttx(235), get_ttx(g_temple_god + 21), get_ttx(gs_town_id + 235));
				GUI_print_loc_line(g_dtp2);

				draw_status_line();

				if (gs_current_loctype != LOCTYPE_TEMPLE) {
					done = 1;
				}
			} else {
				GUI_output(get_ttx(817));
			}
		} else if (g_action == ACTION_ID_ICON_5) {

			/* save game */
			if (gs_current_typeindex != 58) {

				if (!gs_group_member_counts[gs_current_group]) {
					GUI_output(get_ttx(232));
				} else {
					save_game_state();
				}
			} else {
				GUI_output(g_str_no_save_in_temple);
			}
		}

		if (g_action == ACTION_ID_ICON_6) {

			/* quit game */
			if (GUI_bool(get_ttx(299))) {

				done = 1;
				g_game_state = (GAME_STATE_QUIT);
			}
		}

		if (g_action == ACTION_ID_ICON_7) {

			/* ask for a miracle */
			if (!gs_group_member_counts[gs_current_group]) {
				GUI_output(get_ttx(232));
			} else {
				ask_miracle();
			}
		}

		if (g_action == ACTION_ID_ICON_8) {

			/* make a donation */
			if (!gs_group_member_counts[gs_current_group]) {
				GUI_output(get_ttx(232));
			} else {

				money = get_party_money();

				if (!money) {
					GUI_output(get_ttx(401));
				} else {

					make_valuta_str(g_text_output_buf, money);

					sprintf(g_dtp2, get_ttx(297), g_text_output_buf);

					input = GUI_input(g_dtp2, 3);

					donation = input;

					if (donation > 0) {

						if (10 * donation >= money) {
							/* donate all money */
							gs_gods_estimation[g_temple_god] += money / 10L;
							money = 0;
						} else {
							gs_gods_estimation[g_temple_god] += donation;
							money -= 10 * donation;
						}

						set_party_money(money);
						GUI_output(get_ttx(298));
					}
				}
			}
		}
	}

	copy_palette();
	leave_location();

	g_intemple = g_intemple2 = 1;
}

void char_add(signed short temple_id)
{
	signed short l_si;
	signed short l_di;
	signed short i;
	uint8_t *ptr;
	struct struct_hero *hero;

	ptr = g_renderbuf_ptr + 50000;
	l_di = copy_chr_names(ptr, temple_id);

	if (gs_total_hero_counter == 7 || (gs_total_hero_counter == 6 && !get_hero(6)->typus))
	{
		GUI_output(get_ttx(288));

	} else {

		do {

			if (!l_di) {
				GUI_output(get_ttx(290));
				l_si = -1;
			} else {

				l_si = menu_enter_delete(ptr, l_di, 1);

				if (l_si != -1) {

					hero = get_hero(0);

					for (i = 0; i < 6; i++, hero++) {

						if (!hero->typus) {

							prepare_chr_name(g_dtp2, (char*)(ptr + 32 * l_si));

							if (read_chr_temp(g_dtp2, i, gs_current_group)) {
								gs_total_hero_counter++;
								gs_group_member_counts[gs_current_group]++;
								hero->slot_pos = i + 1;
								write_chr_temp(i);
							}
							break;
						}
					}

					draw_main_screen();
					init_ani(2);

					/* location string */
					sprintf(g_dtp2, get_ttx(235), get_ttx(g_temple_god + 21), get_ttx(gs_town_id + 235));

					GUI_print_loc_line(g_dtp2);
				}

				l_di = copy_chr_names(ptr, temple_id);
			}

		} while ((l_si != -1) && (gs_total_hero_counter < (get_hero(6)->typus ? 7 : 6)));
	}
}

void char_letgo(signed short temple_id)
{
	signed short hero_pos;
	struct struct_hero *hero;

	if (!gs_total_hero_counter || !gs_group_member_counts[gs_current_group]) {

		GUI_output(get_ttx(232));

	} else {

		do {
			hero_pos = select_hero_from_group(get_ttx(618));

			if (hero_pos != -1) {

				if (hero_pos == 6) {

					/* let go an NPC */
					gs_npc_months = 99;
					npc_farewell();

				} else {

					/* let go a hero */
					hero = get_hero(hero_pos);
					gs_total_hero_counter--;
					gs_group_member_counts[gs_current_group]--;

					hero->temple_id = (signed char)temple_id;
					hero->slot_pos = 0;

					write_chr_temp(hero_pos);

					memset(hero, 0, sizeof(struct struct_hero));

					draw_main_screen();
					init_ani(2);

					/* location string */
					sprintf(g_dtp2,	get_ttx(235), get_ttx(g_temple_god + 21), get_ttx(gs_town_id + 235));
					GUI_print_loc_line(g_dtp2);
				}
			}

		} while ((hero_pos != -1) && (gs_group_member_counts[gs_current_group] > (get_hero(6)->typus ? 1 : 0)));
	}
}

signed short char_erase(void)
{
	signed short l_si;
	signed short l_di;
	signed short unlink_ret;
	uint8_t *ptr;

	if (g_renderbuf_in_use_flag) {
		ptr = g_buffer9_ptr + 30000L;
	} else {
		ptr = g_renderbuf_ptr + 50000;
	}

	l_di = copy_chr_names(ptr, -1);

	do {
		if (!l_di) {
			l_si = -1;
		} else {
			l_si = menu_enter_delete(ptr, l_di, -1);

			if (l_si != -1) {

				strcpy(g_dtp2, (char*)ptr + 32 * l_si);

				sprintf(g_text_output_buf, get_ttx(295), g_dtp2);

				if (GUI_bool(g_text_output_buf)) {

					prepare_chr_name(g_text_output_buf, g_dtp2);

					unlink_ret = unlink(g_text_output_buf);

					if (unlink_ret) {

						GUI_output(get_ttx(294));

						return 0;
					}

					sprintf(g_dtp2, g_str_temp_file_wildcard, g_text_output_buf);
					unlink(g_dtp2);
				}

				l_di = copy_chr_names(ptr, -1);

			} else {
				return 0;
			}
		}

	} while (l_si != -1);

	return 1;
}

/**
 * \brief   MIRACLE heal one hero
 *
 * \param   le_in       healable LE maximum
 * \param   str         a format-string for the output
 */
void miracle_heal_hero(signed short le_in, char *str)
{
	signed short i;
	signed short le;
	signed short hero_pos;
	signed short le_diff;
	struct struct_hero *hero;

	le = 0;
	hero_pos = -1;

	/* search for the hero with the largest LE-difference */
	for (i = 0; i <= 6; i++) {

		hero = get_hero(i);

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
			!hero->flags.dead && !hero->flags.gods_pissed && !hero->flags.dead &&
			((le_diff = hero->le_max - hero->le) > le))
		{
			le = le_diff;
			hero_pos = i;
		}
	}

	if (hero_pos != -1) {

		/* adjust le_in if the maximum is not reached */
		if (le_in > le) {
			le_in = le;
		}

		add_hero_le(get_hero(hero_pos), le_in);

		/* prepare a message */
		strcpy(g_text_output_buf, get_ttx(392));

		if (le_in > 1) {
			strcat(g_text_output_buf, get_ttx(393));
		}

		sprintf(g_dtp2, (char*)str, get_hero(hero_pos)->alias, le_in, g_text_output_buf);
	}
}

void miracle_resurrect(char *str)
{
	signed short i;

	for (i = 0; i <= 6; i++) {

		struct struct_hero *hero = get_hero(i);

		if (hero->flags.dead && (hero->group_id == gs_current_group) && !hero->flags.gods_pissed)
		{
			/* resurrect from the dead */
			hero->flags.dead = 0;

			/* add 7 LE */
			add_hero_le(hero, 7);

			/* update_ the status line */
			draw_status_line();

			/* prepare a message */
			sprintf(g_dtp2, (char*)str, hero->alias);

			break;
		}
	}
}

/**
 * \brief   MIRACLE modify all living group members
 *
 * \param   offset      offset in the datasheet
 * \param   timer_value how long should the modification last
 * \param   mod         modification value
 */
void miracle_modify(unsigned short offset, int32_t timer_value, signed short mod)
{
	int i;
	int slot;
	HugePt ptr;
	struct struct_hero *hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
			!hero->flags.dead && !hero->flags.gods_pissed)
		{
			slot = get_free_mod_slot();
			ptr = (uint8_t*)hero;
			ptr += offset;

			set_mod_slot(slot, timer_value, (uint8_t*)ptr, (signed char)mod, (signed char)i);
		}
	}
}

/**
 * \brief   MIRACLE repair or magicise weapon
 *
 * \param   str         format string for output
 * \param   mode        0 = magic, != 0 repair
 */
void miracle_weapon(char *str, signed short mode)
{
	int i;
	int j;
	int done;
	int item_id;

	for (j = done = 0; (j <= 6) && (!done); j++) {

		struct struct_hero *hero = get_hero(j);

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead && !hero->flags.gods_pissed)
		{
			for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++)
			{

				if ((item_id = hero->inventory[i].item_id) && g_itemsdat[item_id].flags.weapon)
				{

					if (mode == 0) {

						/* if weapon is neither broken nor magic magic, make it magic and magic_revealed */

						if (!hero->inventory[i].flags.broken &&	!hero->inventory[i].flags.magic)
						{
							hero->inventory[i].flags.magic = 1;
							hero->inventory[i].flags.magic_revealed = 1;

							sprintf(g_dtp2, (char*)str, GUI_names_grammar((signed short)0x8000, item_id, 0), hero->alias);

							done = 1;
							break;
						}
					} else {

						/* repair a broken weapon */
						if (hero->inventory[i].flags.broken)
						{
							hero->inventory[i].flags.broken = 0;

							sprintf(g_dtp2, (char*)str, GUI_names_grammar((signed short)0x8000, item_id, 0), hero->alias);

							done = 1;
							break;
						}
					}
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
