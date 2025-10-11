/**
 *	Rewrite of DSA1 v3.02_de functions of seg076 (dungeon: common 2/2)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg076.cpp
 */

#if !defined(__BORLANDC__)
#include <stdlib.h>
#endif

#include <stdio.h>

#include "v302de.h"
#include "common.h"

/* we need an own prototype of div16 */
#define DONT_DEF_DIV16

#include "seg002.h"
#include "seg003.h"
#include "seg007.h"
#include "seg025.h"
#include "seg028.h"
#include "seg032.h"
#include "seg029.h"
#include "seg047.h"
#include "seg049.h"
#include "seg074.h"
#include "seg075.h"
#include "seg076.h"
#include "seg077.h"
#include "seg078.h"
#include "seg079.h"
#include "seg080.h"
#include "seg081.h"
#include "seg082.h"
#include "seg083.h"
#include "seg084.h"
#include "seg085.h"
#include "seg086.h"
#include "seg087.h"
#include "seg088.h"
#include "seg089.h"
#include "seg090.h"
#include "seg091.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"

#if defined(__BORLANDC__)
signed short div16(signed char);
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short g_dng_refresh_direction;	// ds:0xe482
signed short g_dng_refresh_y_target;	// ds:0xe484
signed short g_dng_refresh_x_target;	// ds:0xe486
unsigned char *g_dng_map_ptr;		// ds:0xe488, to DNG_MAP
signed char g_dng_floor_tex;		// ds:0xe48c, changing every timestep
struct dng_gfxtab *g_dng_gfxtab;		// ds:0xe48d, to GFXTAB(_WOOD|_STONE|_MARBLE)
unsigned char g_unkn_090[1];		// ds:0xe491
static signed short g_lockpick_try_counter; // ds:0xe492, {0..4}
unsigned char *g_dungeon_fights_buf;	// ds:0xe494, to buffer of size 630
unsigned char *g_dungeon_stairs_buf;	// ds:0xe498, to buffer of size 80
struct dungeon_door *g_dungeon_doors_buf;	// ds:0xe49c, to buffer of size 225
signed int g_get_extra_loot;		// ds:0xe4a0

/**
 * \brief   door logic
 *
 * \param   action: which menu option (icon) has been triggered?
 * ACTION_ID_ICON_7 = 135: might be 'open door', 'close door', 'smash door'. Can be distinguished by DNG_MENU_MODE
 * ACTION_ID_ICON_8 = 136: 'lockpicks'
 * ACTION_ID_ICON_9 = 137: 'foramen spell'
 */
void DNG_door(signed short action)
	/* is called from a single code position. DNG_MENU_MODE is one of 1 = DNG_MENU_MODE_OPEN_DOOR, 3 = DNG_MENU_MODE_CLOSE_DOOR, 5 = DNG_MENU_MODE_UNLOCK_DOOR */
{
	signed short x;
	signed short y;
	signed short pos;
	signed short l4;
	signed short spell_result;
	struct dungeon_door *ptr_doors;
	struct struct_hero *hero;
	signed short hero_pos;
	signed short lockpick_pos;
	signed short lockpick_result;

	ptr_doors = g_dungeon_doors_buf;
	x = gs_x_target;
	y = gs_y_target;

	switch (gs_direction)
	{
		case NORTH: y--; break;
		case EAST:  x++; break;
		case SOUTH: y++; break;
		case WEST:  x--; break;
	}

	pos = DNG_POS(gs_dungeon_level, x, y);

	do {

		if (ptr_doors->pos == pos)
		{
#if !defined(__BORLANDC__)
			D1_INFO("Tuer: KK notwendig %d, SCHLOESSER mod = %d, FORAMEN mod = %d\n",
				ptr_doors->smash_handicap, ptr_doors->lockpick_handicap, ptr_doors->foramen_handicap);
#endif

			if (action == ACTION_ID_ICON_7)
			{
				/* 7th icon/menu option has been triggered. Might be 'open door', 'close door', 'smash door'. distinguish based on DNG_MENU_MODE */
				if (g_dng_extra_action != DNG_MENU_MODE_UNLOCK_DOOR) {
					/* either 'open door' or 'close door' */

					if (gs_dungeon_index == DUNGEONS_HYGGELIKS_RUINE && pos == DNG_POS(1,9,3) && gs_dng15_unknown_flag)
					{
						GUI_output(get_tx(18));

					} else if (!(gs_dungeon_index == DUNGEONS_PIRATENHOEHLE && pos == DNG_POS(0,12,8) && gs_dng11_lever_flag != 4) &&
							!(gs_dungeon_index == DUNGEONS_DASPOTASCHATZ && pos == DNG_POS(0,11,1) && gs_dng06_pitdoor_flag) &&
							!(gs_dungeon_index == DUNGEONS_ZWERGENFESTE && gs_dng12_watertrap_water_runs))
					{
						/* for this door works an IRON KEY [first type] */
						/* BEWARE: there are two types of IRON KEYs in the game */
						if (gs_dungeon_index == DUNGEONS_PIRATENHOEHLE && pos == DNG_POS(0,12,8) && get_first_hero_with_item(ITEM_KEY_IRON_1) != -1)
						{
							*(g_dng_map_ptr + MAP_POS(x, y)) |= 0x02;
							/* effect: ......1. i.e. door is unlocked */
						}

						if (div16(*(g_dng_map_ptr + MAP_POS(x,y))) == DNG_TILE_CLOSED_DOOR) /* if 0001.... door is closed */
						{
							/* ASSERT */
							/*
							if (g_dng_extra_action != DNG_MENU_MODE_OPEN_DOOR) {
								D1_INFO("FEHLER: DNG_MENU_MODE sollte DNG_MENU_MODE_OPEN_DOOR sein, stimmt aber nicht.\n");
							}
							*/
							l4 = *(g_dng_map_ptr + MAP_POS(x,y)) & 0x02; /* read bit 1: is door unlocked? */

							if (l4 != 0 || !ptr_doors->smash_handicap)
							{
								/* door closed and unlocked -> open it */

								/* clear higher 4 bits */
								*(g_dng_map_ptr + MAP_POS(x,y)) &= 0x0f;
								*(g_dng_map_ptr + MAP_POS(x,y)) |= DNG_TILE_OPEN_DOOR << 4;
								g_steptarget_front = *(g_dng_map_ptr + MAP_POS(x,y));
								DNG_open_door();

								g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;
								g_redraw_menuicons = 1;
								g_dng_extra_action = DNG_MENU_MODE_CLOSE_DOOR;
							} else {
								/* door closed and locked -> show icons for different opening methods */
								g_new_menu_icons[6] = MENU_ICON_SMASH_DOOR;
								g_new_menu_icons[7] = MENU_ICON_PICK_LOCK;
								g_new_menu_icons[8] = MENU_ICON_MAGIC;
								g_redraw_menuicons = 1;
								g_dng_extra_action = DNG_MENU_MODE_UNLOCK_DOOR;
							}

						} else if (div16(*(g_dng_map_ptr + MAP_POS(x,y))) == DNG_TILE_OPEN_DOOR) /* 0010.... i.e. door is open */
						{
							/* ASSERT */
							/*
							if (g_dng_extra_action != DNG_MENU_MODE_CLOSE_DOOR) {
								D1_INFO("FEHLER: DNG_MENU_MODE sollte DNG_MENU_MODE_CLOSE_DOOR sein, stimmt aber nicht.\n");
							}
							*/

							/* the door is open -> close it */
							DNG_close_door();

							/* clear higher 4 bits */
							*(g_dng_map_ptr + MAP_POS(x, y)) &= 0x0f;
							/* +0x02: set bit 1 'unlocked' */
							*(g_dng_map_ptr + MAP_POS(x, y)) |= (DNG_TILE_CLOSED_DOOR << 4) + 0x02;

							g_steptarget_front = *(g_dng_map_ptr + MAP_POS(x,y));
							g_dng_extra_action = DNG_MENU_MODE_OPEN_DOOR;
						}
					}
				} else {
					/* smash door */
					play_voc(ARCHIVE_FILE_FX14_VOC);

					if (check_heroes_KK(ptr_doors->smash_handicap))
					{
						/* clear higher 4 bits */
						*(g_dng_map_ptr + MAP_POS(x,y)) &= 0x0f;
						*(g_dng_map_ptr + MAP_POS(x,y)) |= DNG_TILE_SMASHED_DOOR << 4;

						g_steptarget_front = *(g_dng_map_ptr + MAP_POS(x,y));
						g_dng_refresh_direction = -1;
					}

					sub_hero_le(get_first_hero_available_in_group(), 1);

					if (get_second_hero_available_in_group())
					{
						sub_hero_le(get_second_hero_available_in_group(), 1);
					}
				}
			}

			if (action == ACTION_ID_ICON_8)
			{
				/* use lockpicks */

				hero = get_first_hero_available_in_group();

				if ((lockpick_pos = hero_has_lockpicks(hero)) != -1)
				{
					if (lockpick_pos != -2)
					{
						lockpick_result = test_skill(hero, TA_SCHLOESSER, ptr_doors->lockpick_handicap);

						play_voc(ARCHIVE_FILE_FX11_VOC);

						g_lockpick_try_counter++;
						/* Original-Bug: Why is this a global variable? Should be related to the door. */

						if (lockpick_result == -99 || g_lockpick_try_counter > 3)
						{
							/* LOCKPICK breaks if the test was unlucky,
								or when tried three times without moving */
							print_msg_with_first_hero(get_ttx(533));

							hero->inventory[lockpick_pos].flags.broken = 1;

							g_lockpick_try_counter = 0;

						} else if (lockpick_result <= 0) {

							/* just failed */
							print_msg_with_first_hero(get_ttx(532));

						} else {
							/* success => the door opens */

							/* clear higher 4 bits */
							*(g_dng_map_ptr + MAP_POS(x,y)) &= 0x0f;

							/* note that the 'unlocked' flag ......1. is not explicitly set. It will be set if the party closes the door. */
							*(g_dng_map_ptr + MAP_POS(x,y)) |= DNG_TILE_OPEN_DOOR << 4;
							g_steptarget_front = *(g_dng_map_ptr + MAP_POS(x,y));
							DNG_open_door();

							add_hero_ap(hero, 1L); /* hero gets 1 AP for successful lock pick */

							g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;
							g_redraw_menuicons = 1;
						}

					} else {
						/* all LOCKPICKS are broken */
						print_msg_with_first_hero(get_ttx(531));
					}
				} else {
					/* the leader has no LOCKPICKS */
					print_msg_with_first_hero(get_ttx(530));
				}
			}

			if (action == ACTION_ID_ICON_9)
			{
				/* use magic */
				hero_pos = select_hero_ok(get_ttx(317));

				if (hero_pos != -1)
				{
					hero = get_hero(hero_pos);

					if (hero->typus < HERO_TYPE_WITCH)
					{
						/* not a spellcaster */
						GUI_output(get_ttx(330));

					} else {
						spell_result = test_spell(hero, SP_FORAMEN_FORAMINOR, ptr_doors->foramen_handicap);

						if (spell_result == -99)
						{
							/* unlucky => just print a message */
							sprintf(g_dtp2, get_ttx(607), hero->alias);
							GUI_output(g_dtp2);

						} else if (spell_result <= 0)
						{
							/* failed => half AE costs */
							sub_ae_splash(hero, get_spell_cost(SP_FORAMEN_FORAMINOR, 1));

							play_voc(ARCHIVE_FILE_FX17_VOC);
						} else {
							play_voc(ARCHIVE_FILE_FX17_VOC);

							sub_ae_splash(hero, get_spell_cost(SP_FORAMEN_FORAMINOR, 0));

							/* success => the door opens */
							*(g_dng_map_ptr + MAP_POS(x,y)) = 0x0f; /* clear higher 4 bits */
							*(g_dng_map_ptr + MAP_POS(x,y)) |= (DNG_TILE_OPEN_DOOR << 4);
							/* note that the 'unlocked' flag ......1. is not explicitly set. It will be set if the party closes the door. */
							g_steptarget_front = *(g_dng_map_ptr + MAP_POS(x,y));
							DNG_open_door();

							add_hero_ap(hero, 1L); /* hero gets 1 AP for successful lock pick */

							g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;
							g_redraw_menuicons = 1;
						}
					}
				}
			}
		}

	} while ((ptr_doors++)->pos != -1);
}

void print_msg_with_first_hero(char *msg)
{
	struct struct_hero *hero = get_first_hero_available_in_group();

	sprintf(g_text_output_buf, msg, hero->alias);

	GUI_input(g_text_output_buf, 0);
	
	return;
}

/**
 * \brief   fallpit logic for the current group in dungeons
 *
 * \param   max_damage  maximum damage if a hero drops in the fallpit
 */
void DNG_fallpit_test(signed short max_damage)
{
	signed short i;
	struct struct_hero *hero;

	play_voc(ARCHIVE_FILE_FX18_VOC);

	*(g_dng_map_ptr + MAP_POS(gs_x_target, gs_y_target)) &= 0x0f; /* clear higher 4 bits */
	*(g_dng_map_ptr + MAP_POS(gs_x_target, gs_y_target)) |= (DNG_TILE_PIT << 4);

	if (gs_dungeon_light != 0)
	{
		/* light is on */
		GUI_output(get_ttx(517));

		/* drop one level down */
		DNG_inc_level();

		*(g_dng_map_ptr + MAP_POS(gs_x_target, gs_y_target)) &= 0x0f; /* clear higher 4 bits */
		*(g_dng_map_ptr + MAP_POS(gs_x_target, gs_y_target)) |= (DNG_TILE_PIT_IN_CEILING << 4);
		/* effect: 0101.... */

		/* damage the heroes */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			/* TODO: need to check if the hero is dead ? */
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group))
			{
				sub_hero_le(hero, random_schick(max_damage));
			}
		}
	} else {
		/* light is off */
		if (DNG_fallpit(max_damage))
		{
			/* drop one level down */
			gs_dungeon_level++;
			load_area_description(1);

			*(g_dng_map_ptr + MAP_POS(gs_x_target, gs_y_target)) &= 0x0f; /* clear higher 4 bits */
			*(g_dng_map_ptr + MAP_POS(gs_x_target, gs_y_target)) |= (DNG_TILE_PIT_IN_CEILING << 4);

			/* move one level up. */
			gs_dungeon_level--;

			gs_x_target = gs_x_target_bak;
			gs_y_target = gs_y_target_bak;

			load_area_description(1);

			DNG_update_pos();
		} else {
			*(g_dng_map_ptr + MAP_POS(gs_x_target, gs_y_target)) &= 0x0f; /* clear higher 4 bits */
			*(g_dng_map_ptr + MAP_POS(gs_x_target, gs_y_target)) |= (DNG_TILE_PIT_IN_CEILING << 4);
			/* effect: 0101.... */
		}
	}
}

signed short DNG_step(void)
{
	signed short l_si;
	signed short l_di;
	signed short (*dungeon_handler)(void);
	signed short retval;
	signed short tw_bak;
	signed short old_value;
	signed short x;
	signed short y;
	signed short pos;

	g_new_menu_icons[0] = MENU_ICON_SPLIT_GROUP;
	old_value = g_new_menu_icons[1];
	g_new_menu_icons[1] = (g_can_merge_group == -1 ? MENU_ICON_MERGE_GROUP_GRAYED : MENU_ICON_MERGE_GROUP);

	if (g_new_menu_icons[1] != old_value)
	{
		g_redraw_menuicons = 1;
	}

	g_new_menu_icons[2] = MENU_ICON_SWITCH_GROUP;
	g_new_menu_icons[3] = MENU_ICON_INFO;
	g_new_menu_icons[4] = MENU_ICON_MAP;
	g_new_menu_icons[5] = MENU_ICON_MAGIC;

	if (g_dng_extra_action == DNG_MENU_MODE_PLAIN && g_new_menu_icons[6] != MENU_ICON_CAMP)
	{
		g_new_menu_icons[6] = MENU_ICON_CAMP;
		g_redraw_menuicons = 1;
	}

	if (g_request_refresh != 0)
	{
		draw_main_screen();
		GUI_print_loc_line(get_tx(0));
		g_request_refresh = g_redraw_menuicons = 0;
		g_dng_refresh_x_target = -1;
	}

	if (g_redraw_menuicons && g_pp20_index == 0)
	{
		draw_icons();
		g_redraw_menuicons = 0;
	}

	if (gs_direction != g_dng_refresh_direction || gs_x_target != g_dng_refresh_x_target || gs_y_target != g_dng_refresh_y_target)
	{
		DNG_update_pos();
		set_automap_tiles(gs_x_target, gs_y_target);
		DNG_fight();
	}

	/* TODO: potential bug: g_dng_level_changed is set to 1, but never back to 0 */
	if ((gs_x_target != gs_x_target_bak) || (gs_y_target != gs_y_target_bak) || g_dng_level_changed)
	{
		g_can_merge_group = can_merge_group();
		g_lockpick_try_counter = 0;
	}

	gs_x_target_bak = (gs_x_target);
	gs_y_target_bak = (gs_y_target);
	gs_direction_bak = (gs_direction);

	handle_gui_input();

	if (g_mouse2_event || g_action == ACTION_ID_PAGE_UP)
	{
		tw_bak = g_textbox_width;
		g_textbox_width = 3;

		for (l_di = retval = 0; l_di < 9; l_di++)
		{
			if (g_new_menu_icons[l_di] != MENU_ICON_NONE)
			{
				retval++;
			}
		}

		l_di = GUI_radio(get_ttx(534), (signed char)retval,
					get_ttx(535),
					get_ttx(536),
					get_ttx(537),
					get_ttx(538),
					get_ttx(539),
					get_ttx(213),
					g_dng_extra_action == DNG_MENU_MODE_PLAIN ? get_ttx(306) :(
						g_dng_extra_action == DNG_MENU_MODE_OPEN_DOOR ? get_ttx(540) :(
						g_dng_extra_action == DNG_MENU_MODE_CLOSE_DOOR ? get_ttx(787) :(
						g_dng_extra_action == DNG_MENU_MODE_UNLOCK_DOOR ? get_ttx(542) :(
						g_dng_extra_action == 4 ? get_ttx(555) : get_ttx(541))))),
					get_ttx(543),
					get_ttx(544)) - 1;

		if (l_di != -2)
		{
			g_action = l_di + ACTION_ID_ICON_1;
		}

		g_textbox_width = tw_bak;
	}

	l_di = 0;

	if (g_action == ACTION_ID_ICON_1)
	{
		GRP_split();
		g_can_merge_group = can_merge_group();

	} else if (g_action == ACTION_ID_ICON_2)
	{
		/* merge groups or reach hands through the mirror */
		pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

		if ((gs_dungeon_index == DUNGEONS_HYGGELIKS_RUINE && pos == DNG_POS(1,8,1)) || pos == DNG_POS(1,8,5))
		{
			GUI_output(get_tx(33));

			gs_dng15_reached_hands = 1;
		} else {
			GRP_merge();
#ifdef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 19: After merging groups in a dungeon, the icon "merge groups" is not grayed.
			 * Compare to the corresponding code in city_step() in seg066.cpp, where the following line is present. */
			g_can_merge_group = -1;
#endif
			/* TODO: if a "dark" group was merged with a "lighted" group, make group "lighted". */
		}

	} else if (g_action == ACTION_ID_ICON_3)
	{
		GRP_switch_to_next(0);

	} else if (g_action == ACTION_ID_ICON_4)
	{
		game_options();

	} else if (g_action == ACTION_ID_ICON_5)
	{
		show_automap();

	} else if (g_action == ACTION_ID_ICON_6)
	{
		if (select_magic_user() > 0)
		{
			g_dng_refresh_direction = -1;
		}

	} else if (g_action == ACTION_ID_ICON_7 && g_dng_extra_action == DNG_MENU_MODE_PLAIN)
	{
		gs_current_loctype = LOCTYPE_CITYCAMP;
		g_citycamp_city = 0; /* CITYCAMP takes place in dungeon */
		l_di = 1;

	} else if (g_action == ACTION_ID_LEFT)
	{
		update_direction(3);
		g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;

	} else if (g_action == ACTION_ID_RIGHT)
	{
		update_direction(1);
		g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;

	} else if (g_action == ACTION_ID_UP)
	{
		if ((l_si = div16(g_steptarget_front)) == DNG_TILE_SEMIPERMEABLE_WALL)
		{
			l_si = 1 << gs_direction;

			if (g_steptarget_front & l_si & 0x0f)
				/* can only be entered if flag no. <direction> is set. */
			{
				DNG_timestep(1);
			}

		} else if ((l_si = div16(g_steptarget_front)) != DNG_TILE_WALL &&
				l_si != DNG_TILE_CLOSED_DOOR && /* closed door */
				l_si != DNG_TILE_REMOVABLE_WALL &&
				l_si != DNG_TILE_CHEST &&
				l_si != DNG_TILE_BLOCKER)
		{
			DNG_timestep(1);
		} else {
			no_way();
		}

	} else if (g_action == ACTION_ID_DOWN)
	{
		if ((l_si = div16(g_steptarget_back)) != DNG_TILE_WALL &&
				l_si != DNG_TILE_CLOSED_DOOR &&
				l_si != DNG_TILE_REMOVABLE_WALL &&
				l_si != DNG_TILE_CHEST &&
				l_si != DNG_TILE_BLOCKER)
		{
			DNG_timestep(-1);
		} else {
			no_way();
		}

	} else if (g_action >= ACTION_ID_ICON_7 &&
			g_action <= ACTION_ID_ICON_9 &&
			g_new_menu_icons[g_action - ACTION_ID_ICON_1] != -1)
	{
		if (g_dng_extra_action == DNG_MENU_MODE_OPEN_DOOR || g_dng_extra_action == DNG_MENU_MODE_CLOSE_DOOR || g_dng_extra_action == DNG_MENU_MODE_UNLOCK_DOOR)
		{
			DNG_door(g_action);
		} else if (g_action == ACTION_ID_ICON_7 && g_dng_extra_action == DNG_MENU_MODE_OPEN_CHEST)
		{
			seg092_06b4(1);

			if (g_get_extra_loot)
			{
				x = gs_x_target;
				y = gs_y_target;

				switch (gs_direction)
				{
					case NORTH: y--; break;
					case EAST:  x++; break;
					case SOUTH: y++; break;
					case WEST:  x--; break;
				}

				*(g_dng_map_ptr + MAP_POS(x,y)) |= (0x02);
				/* set bit 1 'unlocked' */
			}
		} else if (g_action == ACTION_ID_ICON_7 && (!gs_dng15_lever_south || !gs_dng15_lever_north))
		{
			DNG15_riddle();
		}
	}

	if ((gs_dungeon_index != DUNGEONS_NONE) && (g_dng_area_loaded != -1) && !l_di)
	{
		DNG_see_stairs();
		DNG_see_door();
		DNG_see_chest();
		DNG_see_lever();

		dungeon_handler = g_dng_handlers[gs_dungeon_index - 1];

		retval = dungeon_handler();
	}

	return 0;
}

struct stair_struct {
	signed short pos;
	signed char target_x;
	signed char target_y;
};

void DNG_see_stairs(void)
{
	signed short target_pos;
	stair_struct *stair_ptr;
	stair_ptr = (stair_struct*)g_dungeon_stairs_buf;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

#if !defined(__BORLANDC__)
	if (sizeof(stair_struct) != 4)
	{
		D1_INFO("sizeof(stair_struct) = %d\n", sizeof(stair_struct));
		exit(-1);
	}
#endif

	do {
		if (stair_ptr->pos == target_pos)
		{
			/* found the current stairs */
			gs_x_target = (stair_ptr->target_x & 0x0f);
			gs_y_target = (stair_ptr->target_y & 0x0f);
			gs_direction = (stair_ptr->target_y >> 4);

			if (stair_ptr->target_x & 0x80)
			{
				/* downstairs */
				if (stair_ptr->target_x & 0x40)
				{
					gs_dungeon_level++;
				}

				DNG_inc_level();

			} else {
				/* upstairs */
				if (stair_ptr->target_x & 0x40)
				{
					gs_dungeon_level--;
				}

				DNG_dec_level();
			}

			set_automap_tiles(gs_x_target, gs_y_target);

			break;
		}

	} while ((stair_ptr++)->pos != -1);
}

void DNG_see_door(void)
{
	signed short l_si;

	if ((l_si = div16(g_steptarget_front)) == 1 || l_si == 2)
	{
		/* standing direct in front of a door with view to it */
		if (g_new_menu_icons[6] != MENU_ICON_OPEN_CLOSE_DOOR && g_new_menu_icons[6] != MENU_ICON_SMASH_DOOR)
		{
			g_new_menu_icons[6] = MENU_ICON_OPEN_CLOSE_DOOR;
			g_redraw_menuicons = 1;
		}

		if (g_new_menu_icons[6] != MENU_ICON_SMASH_DOOR)
		{
			g_dng_extra_action = (l_si == 1 ? DNG_MENU_MODE_OPEN_DOOR : DNG_MENU_MODE_CLOSE_DOOR);
		}

	} else {
		if (g_new_menu_icons[6] != MENU_ICON_NONE &&
			(g_dng_extra_action == DNG_MENU_MODE_OPEN_DOOR || g_dng_extra_action == DNG_MENU_MODE_CLOSE_DOOR || g_dng_extra_action == DNG_MENU_MODE_UNLOCK_DOOR))
		{
			/* standing two fields before a door with view to it */
			g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;
			g_redraw_menuicons = 1;
			g_dng_extra_action = DNG_MENU_MODE_PLAIN;
		}
	}
}

void DNG_see_chest(void)
{
	if (div16(g_steptarget_front) == 8)
	{
		/* standing direct in front of a treasure chest with view to it */
		if (g_new_menu_icons[6] != MENU_ICON_OPEN_CHEST)
		{
			g_new_menu_icons[6] = MENU_ICON_OPEN_CHEST;
			g_redraw_menuicons = 1;
			g_dng_extra_action = DNG_MENU_MODE_OPEN_CHEST;
		}

	} else {
		if (g_new_menu_icons[6] != MENU_ICON_NONE && g_dng_extra_action == DNG_MENU_MODE_OPEN_CHEST)
		{
			/* standing two fields before a treasure chest with view to it */
			g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;
			g_redraw_menuicons = 1;
			g_dng_extra_action = DNG_MENU_MODE_PLAIN;
		}
	}
}

void do_dungeon(void)
{
	signed short tw_bak;

	if ((g_dng_area_loaded != gs_dungeon_index) || g_area_prepared || g_dng_init_flag)
	{
		g_dng_map_ptr = g_dng_map;

		seg028_0555(0);

		set_audio_track(ARCHIVE_FILE_DUNGEON_XMI);
#if defined(__BORLANDC__)
		g_dng_gfxtab = (struct dng_gfxtab*)(MK_FP(_DS, (!gs_dungeon_gfx_style ? FP_OFF(&g_dng_gfxtab_wood) : (gs_dungeon_gfx_style == 1 ? FP_OFF(&g_dng_gfxtab_marble) : FP_OFF(&g_dng_gfxtab_stone)))));
#else
		g_dng_gfxtab = (!gs_dungeon_gfx_style ? g_dng_gfxtab_wood : (gs_dungeon_gfx_style == 1 ? g_dng_gfxtab_marble : g_dng_gfxtab_stone));
#endif

		g_dng_init_flag = 0;
		g_request_refresh = 1;
	}

	g_current_ani = -1;

	gs_dungeon_index_bak = gs_dungeon_index;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	DNG_step();

	g_textbox_width = tw_bak;
}

struct fight_struct {
	signed short pos;
	signed short fight_id;
	signed short flee_north;
	signed short flee_east;
	signed short flee_south;
	signed short flee_west;
	signed short ap;
};

void DNG_fight(void)
{
	signed short target_pos;
	struct fight_struct *fight_ptr;

	fight_ptr = (struct fight_struct*)g_dungeon_fights_buf;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

#if !defined(__BORLANDC__)
	if (sizeof(fight_struct) != 14)
	{
		D1_INFO("sizeof(fight_struct) = %d\n", sizeof(fight_struct));
		exit(-1);
	}
#endif
	do {
		if (fight_ptr->pos == target_pos)
		{
			/* set positions of heroes which escape from the fight */
			g_fig_flee_position[NORTH] = fight_ptr->flee_north;
			g_fig_flee_position[EAST] = fight_ptr->flee_east;
			g_fig_flee_position[SOUTH] = fight_ptr->flee_south;
			g_fig_flee_position[WEST] = fight_ptr->flee_west;

			/* execute the fight */
			if (!do_fight(fight_ptr->fight_id))
			{
				add_hero_ap_all(fight_ptr->ap);
			}

			/* play the music for the DUNGEON again */
			set_audio_track(ARCHIVE_FILE_DUNGEON_XMI);
		}

	} while ((++fight_ptr)->pos != -1);
}

/**
 * \brief   the group finds a barrel of water
 *
 * \param   unit_ptr    pointer to the number of water units
 */
void DNG_waterbarrel(Bit8u *unit_ptr)
{
	signed short item_pos;
	signed short l_di;
	signed short answer;
	signed short units_needed;
	signed short hero_refilled;
	signed short hero_refilled_counter;
	signed short done;
	struct struct_hero *hero;

	done = 0;

	/* TODO: check the value of unit_ptr first and skip if unit_ptr <= 0*/

	do {
		sprintf(g_dtp2,	get_ttx(781), *unit_ptr);

		answer = GUI_radio(g_dtp2, 3, get_ttx(782), get_ttx(783), get_ttx(784));

		if (answer == 1) {

			/* drink */
			hero = get_hero(0);
			for (l_di = 0; l_di <= 6; l_di++, hero++) {

				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) &&
					!hero->flags.dead)
				{
					/* 1 unit of water <=> 10 Points of thirst */

					units_needed = (hero->thirst + 9) / 10;
					/* +9 means: round up */

					if (*unit_ptr <= units_needed)
					{
						/* not enough units in the barrel for this hero */
						hero->thirst -= *unit_ptr * 10;

						if (hero->thirst < 0)
						{
							hero->thirst = 0;
						}

						*unit_ptr = 0;

						GUI_output(get_ttx(785));

						break;
					} else {
						/* this hero quenches his/her thirst completely */
						*unit_ptr -= units_needed;
						hero->thirst = 0;
					}
				}
			}

		} else	if (answer == 2) {

			/* replenish WATERSKINS */
			hero = get_hero(0);
			for (hero_refilled_counter = l_di = 0; l_di <= 6; l_di++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) &&
					!hero->flags.dead)
				{
					for (item_pos = hero_refilled = 0; item_pos < NR_HERO_INVENTORY_SLOTS; item_pos++)
					{
						if (hero->inventory[item_pos].item_id == ITEM_WATERSKIN)
						{
							units_needed = 0;

							if (hero->inventory[item_pos].flags.half_empty) {

								units_needed = 1;

							} else if (hero->inventory[item_pos].flags.empty) {

								units_needed = 2;
							}

							if (units_needed != 0)
							{
								hero_refilled = 1;

								/* refill waterskin */
								hero->inventory[item_pos].flags.half_empty = hero->inventory[item_pos].flags.empty = 0;

								if (*unit_ptr <= units_needed) {

									/* empty the barrel completely */
									GUI_output(get_ttx(785));

									*unit_ptr = 0;

								} else {
									/* remove units from the barrel */
									*unit_ptr -= units_needed;
								}
							}
						}
					}

					if (hero_refilled) {
						hero_refilled_counter++;
					}
				}
			}

			/* print a message if no hero used the barrel */
			if (!hero_refilled_counter) {
				GUI_output(get_ttx(786));
			}

			done = 1;
		} else {
			/* stay away or pushed ESC */
			done = 1;
		}

	} while (!done);
}

void DNG_see_lever(void)
{
	signed short target_pos;

	target_pos = DNG_POS(gs_dungeon_level, gs_x_target, gs_y_target);

	if (gs_dungeon_index == DUNGEONS_HYGGELIKS_RUINE &&
		(target_pos == DNG_POS(1,8,1) || target_pos == DNG_POS(1,8,5)) &&
		(!gs_dng15_lever_south || !gs_dng15_lever_north))
	{
		if (g_new_menu_icons[6] == MENU_ICON_NONE)
		{
			g_new_menu_icons[6] = MENU_ICON_MOVE_LEVER;
			g_redraw_menuicons = 1;
			g_dng_extra_action = DNG_MENU_MODE_LEVER;
		}

	} else if (g_new_menu_icons[6] != MENU_ICON_NONE && g_dng_extra_action == DNG_MENU_MODE_LEVER)
	{
			g_new_menu_icons[6] = MENU_ICON_NONE;
			g_redraw_menuicons = 1;
			g_dng_extra_action = DNG_MENU_MODE_PLAIN;
	}
}

#if !defined(__BORLANDC__)
}
#endif
