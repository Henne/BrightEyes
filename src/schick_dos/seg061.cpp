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

static int8_t g_praios_temples_typeindex[2] = { 0x3d, -1 }; // ds:0x6de8, array terminated by -1
static int8_t g_rondra_temples_typeindex[6] = { 0x0b, 0x1c, 0x25, 0x2d, 0x33, -1 }; // ds:0x6dea, array terminated by -1
static int8_t g_efferd_temples_typeindex[11] = { 0x03, 0x0d, 0x0e, 0x21, 0x22, 0x26, 0x29, 0x2c, 0x35, 0x39, -1 }; // ds:0x6df0, array terminated by -1
static int8_t g_travia_temples_typeindex[16] = { 0x01, 0x08, 0x0a, 0x10, 0x11, 0x14, 0x18, 0x1e, 0x23, 0x2a, 0x2b, 0x2f, 0x30, 0x32, 0x34, -1 }; // ds:0x6dfb, array terminated by -1
static int8_t g_boron_temples_typeindex[2] = { 0x3e, -1 }; // ds:0x6e0b, array terminated by -1
static int8_t g_hesinde_temples_typeindex[2] = { 0x3f, -1 }; // ds:0x6e0d, array terminated by -1
static int8_t g_firun_temples_typeindex[4] = { 0x12, 0x1a, 0x3a, -1 }; // ds:0x6e0f, array terminated by -1
static int8_t g_tsa_temples_typeindex[2] = { 6, -1 }; // ds:0x6e13, array terminated by -1
static int8_t g_phex_temples_typeindex[6] = { 0x05, 0x09, 0x13, 0x27, 0x3b, -1 }; // ds:0x6e15, array terminated by -1
static int8_t g_peraine_temples_typeindex[5] = { 0x04, 0x0c, 0x19, 0x3c, -1 }; // ds:0x6e1b, array terminated by -1
static int8_t g_ingerimm_temples_typeindex[3] = { 0x16, 0x1b, -1 }; // ds:0x6e20, array terminated by -1
static int8_t g_rahja_temples_typeindex[2] = { 0x40, -1 }; // ds:0x6e23, array terminated by -1
static int8_t g_swafnir_temples_typeindex[13] = { 0x02, 0x07, 0x0f, 0x15, 0x1d, 0x20, 0x24, 0x28, 0x2e, 0x31, 0x36, 0x38, -1 }; // ds:0x6e25, array terminated by -1
static int8_t g_ifirn_temples_typeindex[4] = { 0x17, 0x1f, 0x37, -1 }; // ds:0x6e32, array terminated by -1

static int8_t* g_god_temples_typeindex[15] = {
	NULL,
	g_praios_temples_typeindex,
	g_rondra_temples_typeindex,
	g_efferd_temples_typeindex,

	g_travia_temples_typeindex,
	g_boron_temples_typeindex,
	g_hesinde_temples_typeindex,
	g_firun_temples_typeindex,

	g_tsa_temples_typeindex,
	g_phex_temples_typeindex,
	g_peraine_temples_typeindex,
	g_ingerimm_temples_typeindex,

	g_rahja_temples_typeindex,
	g_swafnir_temples_typeindex,
	g_ifirn_temples_typeindex,
}; // ds:0x6e36; int8_t*

static char g_str_temp_file_wildcard[8] = "TEMP\\%s"; // ds:0x6e72
static char g_str_no_save_in_temple[41] = "IN DIESEM TEMPEL KEIN SPEICHERN M\x99GLICH!"; // ds:0x6e7a


/* REMARK: should be passed as a parameter to asm_miracles() */
signed int g_temple_god; // ds:0xe3f8, id of current temple's god

void do_temple(void)
{
	signed int god_id;
	signed int answer;
	signed int input;
	signed char done = 0;
	int32_t money;
	int32_t donation;
	signed int game_state;

	g_intemple = g_intemple2 = 0;
	g_request_refresh = 1;

	draw_loc_icons(9, MENU_ICON_HIRE_HERO, MENU_ICON_DISMISS_HERO, MENU_ICON_DELETE_HERO, MENU_ICON_LOAD_GAME, MENU_ICON_SAVE_GAME, MENU_ICON_QUIT_GAME, MENU_ICON_PRAY, MENU_ICON_SACRIFICE, MENU_ICON_LEAVE);

	while (!done) {

		if (g_request_refresh != 0) {

			/* search which god owns this temple */
			g_temple_god = 1;
			for (god_id = 1; god_id < GOD_ID__END; god_id++) {
				if (is_in_byte_array(gs_town_typeindex, g_god_temples_typeindex[god_id]))
				{
					g_temple_god = god_id;
					break;
				}
			}

			draw_main_screen();
			load_ani(3);
			load_temple_logo(g_temple_god - 1);
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
				(char*)(gs_town_typeindex != 58 ? get_ttx(gs_town_id + 235): get_ttx(622)));

			GUI_print_loc_line(g_dtp2);

			g_request_refresh = 0;
		}

		handle_gui_input();

		/* input window */
		if (g_mouse_rightclick_event || g_action == ACTION_ID_PAGE_UP) {

			answer = GUI_radio(get_ttx(225), 9,
						get_ttx(226), get_ttx(227), get_ttx(293),
						get_ttx(228), get_ttx(229), get_ttx(230),
						get_ttx(620), get_ttx(296), get_ttx(231)) - 1;

			if (answer != -2) {
				g_action = answer + ACTION_ID_ICON_1;
			}
		}

		if (g_action == ACTION_ID_ICON_9) {
			/* leave temple */
			if (!gs_group_member_counts[gs_active_group_id]) {
				GUI_output(get_ttx(232));
			} else {
				done = 1;
			}
		} else if (g_action == ACTION_ID_ICON_1) {
			/* add character */
			char_add(gs_town_typeindex);
			draw_status_line();
		} else if (g_action == ACTION_ID_ICON_2) {
			/* let go character */
			char_letgo(gs_town_typeindex);
			draw_status_line();
		} else if (g_action == ACTION_ID_ICON_3) {
			/* erase character */
			char_erase();
		} else if (g_action == ACTION_ID_ICON_4) {
			/* load game */
			if (gs_town_typeindex != 58) {

				do {
					game_state = load_game_state();
				} while (game_state == -1);

				/* location string */
				sprintf(g_dtp2,	get_ttx(235), get_ttx(g_temple_god + 21), get_ttx(gs_town_id + 235));
				GUI_print_loc_line(g_dtp2);

				draw_status_line();

				if (gs_town_loc_type != LOCTYPE_TEMPLE) {
					done = 1;
				}
			} else {
				GUI_output(get_ttx(817));
			}
		} else if (g_action == ACTION_ID_ICON_5) {

			/* save game */
			if (gs_town_typeindex != 58) {

				if (!gs_group_member_counts[gs_active_group_id]) {
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
				g_game_state = GAME_STATE_QUIT;
			}
		}

		if (g_action == ACTION_ID_ICON_7) {

			/* ask for a miracle */
			if (!gs_group_member_counts[gs_active_group_id]) {
				GUI_output(get_ttx(232));
			} else {
				ask_miracle();
			}
		}

		if (g_action == ACTION_ID_ICON_8) {

			/* make a donation */
			if (!gs_group_member_counts[gs_active_group_id]) {
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

void char_add(const signed int temple_id)
{
	signed int position;
	signed int entries;
	signed int i;
	char *ptr;

	ptr = (char*)(g_renderbuf_ptr + 50000);
	entries = copy_chr_names(ptr, temple_id);

	if ((gs_total_hero_counter == 7) || (gs_total_hero_counter == 6 && !get_hero(6)->typus))
	{
		GUI_output(get_ttx(288));

	} else {

		do {

			if (!entries) {
				GUI_output(get_ttx(290));
				position = -1;
			} else {

				position = menu_enter_delete(ptr, entries, 1);

				if (position != -1) {

					struct struct_hero *hero = get_hero(0);

					for (i = 0; i < 6; i++, hero++) {

						if (!hero->typus) {

							prepare_chr_name(g_dtp2, ptr + 32 * position);

							if (read_chr_temp(g_dtp2, i, gs_active_group_id)) {
								gs_total_hero_counter++;
								gs_group_member_counts[gs_active_group_id]++;
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

				entries = copy_chr_names(ptr, temple_id);
			}

		} while ((position != -1) && (gs_total_hero_counter < (get_hero(6)->typus ? 7 : 6)));
	}
}

void char_letgo(const signed int temple_id)
{
	if (!gs_total_hero_counter || !gs_group_member_counts[gs_active_group_id]) {

		GUI_output(get_ttx(232));

	} else {

		signed int hero_pos;

		do {
			hero_pos = select_hero_from_group(get_ttx(618));

			if (hero_pos != -1) {

				if (hero_pos == 6) {

					/* let go an NPC */
					gs_npc_months = 99;
					npc_farewell();

				} else {

					/* let go a hero */
					struct struct_hero *hero = get_hero(hero_pos);
					gs_total_hero_counter--;
					gs_group_member_counts[gs_active_group_id]--;

					hero->temple_id = temple_id;
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

		} while ((hero_pos != -1) && (gs_group_member_counts[gs_active_group_id] > (get_hero(6)->typus ? 1 : 0)));
	}
}

signed int char_erase(void)
{
	signed int position;
	signed int entries;
	signed int unlink_ret;
	char *ptr;

	if (g_renderbuf_in_use_flag) {
		ptr = (char*)(g_buffer9_ptr + 30000L);
	} else {
		ptr = (char*)(g_renderbuf_ptr + 50000);
	}

	entries = copy_chr_names(ptr, -1);

	do {
		if (!entries) {
			position = -1;
		} else {
			position = menu_enter_delete(ptr, entries, -1);

			if (position != -1) {

				strcpy(g_dtp2, ptr + 32 * position);

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

				entries = copy_chr_names(ptr, -1);

			} else {
				return 0;
			}
		}

	} while (position != -1);

	return 1;
}

/**
 * \brief   MIRACLE heal one hero
 *
 * \param   le_in       healable LE maximum
 * \param   str         a format-string for the output
 */
void miracle_heal_hero(signed int le_in, const char *str)
{
	signed int i;
	signed int le = 0;
	signed int hero_pos = -1;

	/* search for the hero with the largest LE-difference */
	for (i = 0; i <= 6; i++) {

		signed int le_diff;
		const struct struct_hero *hero = get_hero(i);

		/* REMARK: remove one check for dead, do difference calculation before the if */
		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
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

		sprintf(g_dtp2, str, get_hero(hero_pos)->alias, le_in, g_text_output_buf);
	}
}

void miracle_resurrect(const char *str)
{
	signed int i;

	for (i = 0; i <= 6; i++) {

		struct struct_hero *hero = get_hero(i);

		if (hero->flags.dead && (hero->group_id == gs_active_group_id) && !hero->flags.gods_pissed)
		{
			/* resurrect from the dead */
			hero->flags.dead = 0;

			/* add 7 LE */
			add_hero_le(hero, 7);

			/* update_ the status line */
			draw_status_line();

			/* prepare a message */
			sprintf(g_dtp2, str, hero->alias);

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
void miracle_modify(const unsigned int offset, const int32_t timer_value, const signed int mod)
{
	signed int i;
	signed int mod_slot;
	HugePt ptr;
	struct struct_hero *hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
			!hero->flags.dead && !hero->flags.gods_pissed)
		{
			mod_slot = get_free_mod_slot();
			ptr = (uint8_t*)hero;
			ptr += offset;

			set_mod_slot(mod_slot, timer_value, (uint8_t*)ptr, (signed char)mod, (signed char)i);
		}
	}
}

/**
 * \brief   MIRACLE repair or magicise weapon
 *
 * \param   str         format string for output
 * \param   mode        0 = magic, != 0 repair
 */
void miracle_weapon(const char *str, const signed int mode)
{
	signed int i;
	signed int j;
	signed int done;
	signed int item_id;

	for (j = done = 0; (j <= 6) && (!done); j++) {

		struct struct_hero *hero = get_hero(j);

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead && !hero->flags.gods_pissed)
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

							sprintf(g_dtp2, str, GUI_name_inflect_with_article(
								INFLECT_DEFINITE_ARTICLE | INFLECT_SINGULAR | INFLECT_1ST_CASE,
								item_id,
								INFLECT_NAME_TYPE_ITEM
							), hero->alias);

							done = 1;
							break;
						}
					} else {

						/* repair a broken weapon */
						if (hero->inventory[i].flags.broken)
						{
							hero->inventory[i].flags.broken = 0;

							sprintf(g_dtp2, str, GUI_name_inflect_with_article(
								INFLECT_DEFINITE_ARTICLE | INFLECT_SINGULAR | INFLECT_1ST_CASE,
								item_id,
								INFLECT_NAME_TYPE_ITEM),
							hero->alias);

							done = 1;
							break;
						}
					}
				}
			}
		}
	}
}
