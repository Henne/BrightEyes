/**
 *	Rewrite of DSA1 v3.02_de functions of seg074 (automap)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg074.cpp
 */
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg029.h"
#if !defined(__BORLANDC__)
#include "seg066.h"
#else
/* BC: only the same code is produced, when this prototype is known */
signed short get_border_index(unsigned short);
#endif
#include "seg074.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void show_automap(void)
{
	signed short l_si;
	signed short l_di;
	signed short loc_bak;
	signed short done;
	signed short dungeon;
	signed short town;
	signed short tw_bak;

	if (!ds_readb(DUNGEON_LIGHT) ||
		((ds_readbs(DUNGEON_LIGHT) == 1) && (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 6) > 0)))
	{

		g_special_screen = 1;

		dungeon = gs_dungeon_index;
		town = gs_current_town;

		gs_current_town = (gs_dungeon_index = 0);

		tw_bak = g_textbox_width;
		g_textbox_width = 3;

		l_si = (ds_readb(DNG_MAP_SIZE) == 16) ? 0 :
				((gs_x_target - 8 < 0) ? 0 :
				((gs_x_target - 8 > 15) ? 16 : gs_x_target - 8));

		gs_current_town = ((signed char)town);
		gs_dungeon_index = dungeon;

		ds_writew(REQUEST_REFRESH, 1);

		do {

			if (ds_readw(REQUEST_REFRESH) != 0) {

				loc_bak = gs_current_loctype;
				gs_current_loctype = LOCTYPE_UNKN1;

				draw_main_screen();

				gs_current_loctype = loc_bak;

				if (ds_readb(DNG_MAP_SIZE) == 16) {
					draw_loc_icons(1, MENU_ICON_LEAVE);
				} else {
					draw_loc_icons(3, MENU_ICON_SCROLL_LEFT, MENU_ICON_SCROLL_RIGHT, MENU_ICON_LEAVE);
				}

				render_automap(l_si);
				clear_ani_pal();
				draw_automap_to_screen();
				set_ani_pal(p_datseg + PALETTE_FIGHT2);
				ds_writew(REQUEST_REFRESH, 0);
			}

			done = 0;

			handle_gui_input();

			if ((ds_readw(MOUSE2_EVENT) != 0) || (ds_readw(ACTION) == ACTION_ID_PAGE_UP)) {

				if (ds_readb(DNG_MAP_SIZE) == 16) {
					l_di = GUI_radio(get_ttx(612), 1, get_ttx(613)) - 1;
				} else {
					ds_writew(MENU_DEFAULT_SELECT, 2);
					l_di = GUI_radio(get_ttx(612), 3,
								 get_ttx(614),
								 get_ttx(615),
								 get_ttx(613)) - 1;
				}

				if (l_di != -2) {
					ds_writew(ACTION, l_di + ACTION_ID_ICON_1);
				}
			}

			if (ds_readb(DNG_MAP_SIZE) != 16) {

				if ((ds_readws(ACTION) == ACTION_ID_ICON_1) || (ds_readws(ACTION) == ACTION_ID_LEFT)) {

					if (l_si > 0) {
						render_automap(--l_si);
						draw_automap_to_screen();
					}
				}

				if ((ds_readws(ACTION) == ACTION_ID_ICON_2) || (ds_readws(ACTION) == ACTION_ID_RIGHT)) {

					if (l_si < 16) {
						render_automap(++l_si);
						draw_automap_to_screen();
					}
				}
			}

			if (((ds_readws(ACTION) == ACTION_ID_ICON_1) && (ds_readb(DNG_MAP_SIZE) == 16)) ||
				((ds_readws(ACTION) == ACTION_ID_ICON_3) && (ds_readb(DNG_MAP_SIZE) != 16)))
			{
				done = 1;
			}
		} while (done == 0);

		g_textbox_width = tw_bak;
		ds_writew(REQUEST_REFRESH, 1);

		clear_ani_pal();

	} else {
		GUI_output(get_ttx(772));
	}
}

unsigned short is_discovered(signed short x, signed short y)
{
	return ds_readb(AUTOMAP_BUF + (4 * y) + (x >> 3)) & (ds_readb(AUTOMAP_BITMASK + (x & 7)));
}

/**
 * \brief   get value of automap field from small (16x16) maps
 *
 * \param   x           x-coordinate
 * \param   y           y-coordinate
 * \return              value of the field at (x,y)
 */
unsigned short get_mapval_small(signed short x, signed short y)
{
	Bit8u *map = p_datseg + DNG_MAP;

	return host_readb(map + 16 * y + x);
}

/**
 * \brief   get value of automap field from small (32x16) maps
 *
 * \param   x           x-coordinate
 * \param   y           y-coordinate
 * \return              value of the field at (x,y)
 */
unsigned short get_mapval_large(signed short x, signed short y)
{
	Bit8u *map = p_datseg + DNG_MAP;

	return host_readb(map + 32 * y + x);
}

/**
 * \brief   checks if the group is in prison
 *
 * \param   group_no    number of the group
 * \return              the value of the "in_prison" flag
 */
signed short is_group_in_prison(signed short group_no)
{
	Bit8u *hero = get_hero(0);
	signed short i;

	for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(hero + HERO_GROUP_NO) == group_no))
		{
			return host_readbs(hero + HERO_JAIL);
		}
	}

	return 0;
}

/**
 * \brief   render the automap in g_renderbuf
 *
 * \param   x_off           x offset for vertical scroll
 */
void render_automap(signed short x_off)
{
	signed short tile_type;
	signed short group_i;
	signed short x;
	signed short y;
	signed short entrance_dir;

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 6;
	g_pic_copy.y2 = 6;

	/* set buffer to 0 */
	memset(g_renderbuf_ptr, 0, 64000);

	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			if (is_discovered(x + x_off, y)) {

				if (gs_dungeon_index != DUNGEONS_NONE) {
					/* in dungeon */

					tile_type = div16(get_mapval_small(x, y));

					draw_automap_square(x, y,
						(tile_type <= DNG_TILE_CORRIDOR)? MAP_TILE_DARK_GREY :
							(((tile_type == DNG_TILE_CLOSED_DOOR) || (tile_type == DNG_TILE_SMASHED_DOOR) || tile_type == DNG_TILE_OPEN_DOOR) ? MAP_TILE_DARK_RED :
							((tile_type == DNG_TILE_STAIR_UP) ? MAP_TILE_LIGHT_BLUE :
							((tile_type == DNG_TILE_STAIR_DOWN) ? MAP_TILE_BLUE :
							((tile_type == DNG_TILE_PIT_IN_CEILING) ? MAP_TILE_BRIGHT_GREEN :
							((tile_type == DNG_TILE_CHEST) ? MAP_TILE_BROWN :
							((tile_type == DNG_TILE_PIT) ? MAP_TILE_DARK_GREEN : MAP_TILE_RED)))))), -1);

				} else {
					/* in a town */

					if (!(tile_type = get_maploc(x + x_off, y))) {
						tile_type = get_border_index((ds_readb(DNG_MAP_SIZE) == 16) ?
										get_mapval_small(x, y) :
										get_mapval_large(x + x_off, y));
					}

					draw_automap_square(x, y,
						(tile_type <= TOWN_TILE_STREET)? MAP_TILE_DARK_GREY :
							((tile_type == TOWN_TILE_WATER) ? MAP_TILE_BLUE :
							((tile_type == TOWN_TILE_GRASS) ? MAP_TILE_GREEN :
							((tile_type == TOWN_TILE_SIGNPOST) ? MAP_TILE_RED :
							((tile_type == TOWN_TILE_TEMPLE) ? MAP_TILE_ORANGE :
							((tile_type == TOWN_TILE_INN_OR_TAVERN) ? MAP_TILE_LIGHT_BLUE :
							((tile_type == TOWN_TILE_MERCHANT) ? MAP_TILE_GREY : /* The lightouse on Runin is also displayed in Gray */
							((tile_type == TOWN_TILE_SMITH) ? MAP_TILE_DARK_GREEN :
							((tile_type == TOWN_TILE_HEALER) ? MAP_TILE_PINK :
							((tile_type == TOWN_TILE_BLACK_FINGER) ? MAP_TILE_DARK_PURPLE :
							(((tile_type >= TOWN_TILE_HOUSE_1) && (tile_type <= TOWN_TILE_HOUSE_4)) ? MAP_TILE_DARK_RED : MAP_TILE_BLACK)))))))))), -1);

					if ((tile_type != TOWN_TILE_STREET) && (tile_type != TOWN_TILE_GRASS) && (tile_type != TOWN_TILE_WATER) && (tile_type != TOWN_TILE_SIGNPOST)) {

						entrance_dir = (ds_readb(DNG_MAP_SIZE) == 16) ?
										get_mapval_small(x, y) :
										get_mapval_large(x + x_off, y);

						entrance_dir &= 3;
						draw_automap_entrance(x, y, entrance_dir);
					}
				}
			}
		}
	}

#ifndef M302de_ORIGINAL_BUGFIX
	/* draw yellow arrow at the position of the active group */

	/* Original-Bug 31: If there is another group on the same map square as the active group, the automap will mark that square by a purple arrow (for the other group) instead of a yellow one (for the active group).
	 * The reason is that the yellow arrow will be overdrawn by the purple ones, which are drawn later. */
	if (((gs_x_target - x_off) >= 0) && ((gs_x_target - x_off) <= 16)) { /* shouldn't this always be true? */

		draw_automap_square(gs_x_target - x_off, gs_y_target,
					MAP_TILE_YELLOW_ARROW, gs_direction);
	}
#endif

	/* draw purple arrows at the positions of other groups */
	for (group_i = 0; group_i < 6; group_i++) {

		if ((gs_current_group != group_i) &&
			(gs_group_member_counts[group_i] > 0) &&
			(gs_groups_dng_level[group_i] == gs_dungeon_level) &&
			(gs_groups_town[group_i] == gs_current_town) &&
			(gs_groups_dng_index[group_i] == gs_dungeon_index) &&
			!is_group_in_prison(group_i) &&
			(gs_groups_x_target[group_i] - x_off >= 0) &&
			(gs_groups_x_target[group_i] - x_off <= 16))
		{
			draw_automap_square(gs_groups_x_target[group_i] - x_off,
					gs_groups_y_target[group_i],
					MAP_TILE_PURPLE_ARROW,
					gs_groups_direction[group_i]);
		}
	}

#ifdef M302de_ORIGINAL_BUGFIX
	/* draw yellow arrow at the position of the active group */

	/* Original-Bug 31: see above.
	 * Fix: Move the code block drawing the yellow arrow after the one drawing the purple arrows. */
	if (((gs_x_target - x_off) >= 0) && ((gs_x_target - x_off) <= 16)) { /* shouldn't this always be true? */

		draw_automap_square(gs_x_target - x_off, gs_y_target,
					MAP_TILE_YELLOW_ARROW, gs_direction);
	}
#endif

	/* In the target selector screen of the Transversalis spell, mark the target with a cross */
	if (((ds_readws(AUTOMAP_SELX) - x_off) >= 0) && ((ds_readws(AUTOMAP_SELX) - x_off) <= 16)) {

		draw_automap_square(ds_readws(AUTOMAP_SELX) - x_off,	ds_readws(AUTOMAP_SELY), MAP_TILE_CROSS, -1);
	}
}

/**
 * \brief   draws a building on the automap
 *
 * \param   x           x-coordinate on the automap
 * \param   y           y-coordiante on the automap
 * \param   color       the color
 * \param   dir         direction of the entrance, -1 for none
 */
void draw_automap_square(signed short x, signed short y, signed short color, signed short dir)
{
	signed short i;
	unsigned short offset_y;
	Bit8u* p_img_tile;
	signed char tile[50];

	offset_y = y;
	offset_y <<= 3;
	offset_y *= 320;

	p_img_tile = ((Bit8u*)g_renderbuf_ptr) + offset_y + 8 * x + 0xca8;

	for (i = 0; i < 49; i++) {
		tile[i] = (signed char)color;
	}

	if ((color == 4) || (color == 16)) {

		if (dir == 0) {

			memcpy(g_text_output_buf, p_datseg + AUTOMAP_TILE_ARROWUP, 49);

		} else if (dir == 2) {

			for (i = 0; i < 49; i++) {
				host_writeb((Bit8u*)g_text_output_buf + i, ds_readb(AUTOMAP_TILE_ARROWUP + (48 - i)));
			}

		} else if (dir == 1) {

			memcpy(g_text_output_buf, (char*)(p_datseg + AUTOMAP_TILE_ARROWRIGHT), 49);

		} else {

			for (i = 0; i < 49; i++) {
				host_writeb((Bit8u*)g_text_output_buf + i, ds_readb(AUTOMAP_TILE_ARROWRIGHT + (48 - i)));
			}
		}

		for (i = 0; i < 49; i++) {
			if (!host_readbs((Bit8u*)g_text_output_buf + i)) {
				tile[i] = 0;
			}
		}
	}

	if (color == 7) {

		for (i = 0; i < 49; i++) {
			if (!ds_readb(AUTOMAP_TILE_CROSS + i)) {
				tile[i] = 0;
			} else {
				tile[i] = (signed char)color;
			}
		}
	}

	g_pic_copy.dst = p_img_tile;
	g_pic_copy.src = (Bit8u*)tile;

	do_pic_copy(0);
}

/**
 * \brief   draws the entrance of a automap building
 *
 * \param   x           x-coordinate of the building
 * \param   y           y-coordinate of the building
 * \param   dir         direction of the entrance, 0 = NORTH, 1 = EAST,...
 */
void draw_automap_entrance(signed short x, signed short y, signed short dir)
{
	unsigned short offset_y = y;
	signed short d = dir;
	signed short skipsize;
	Bit8u *p_img_tile;

	offset_y <<= 3;
	offset_y *= 320;

	p_img_tile = ((Bit8u*)g_renderbuf_ptr) + offset_y + 8 * x + 0xca8;

	if (!d) {
		p_img_tile += 2;
		skipsize = 1;
	} else if (d == 1) {
		p_img_tile += 646;
		skipsize = 320;
	} else if (d == 2) {
		p_img_tile += 1922;
		skipsize = 1;
	} else {
		p_img_tile += 640;
		skipsize = 320;
	}

	/* set 3 pixel to black */
	host_writeb(p_img_tile, 0);
	host_writeb(p_img_tile + skipsize, 0);
	host_writeb(p_img_tile + skipsize + skipsize, 0);
}

struct coords {
	signed short x, y;
};

struct dummy {
	struct coords a[2];
};

void draw_automap_to_screen(void)
{
	struct dummy bak;

	/* save screen coordinates */
	bak = *(struct dummy*)(p_datseg + PIC_COPY_DS_RECT);

	/* set the screen coordinates */
	ds_writew((PIC_COPY_DS_RECT + 2), ds_writew(PIC_COPY_DS_RECT, 0));
	ds_writew((PIC_COPY_DS_RECT + 6), ds_readw(ANI_POSX) + 208);
	ds_writew((PIC_COPY_DS_RECT + 4), ds_readw(ANI_POSY) + 135);

	g_pic_copy.src = g_renderbuf_ptr;

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 134;

	g_pic_copy.dst = g_vga_memstart + ds_readws(ANI_POSX) + 320 * ds_readws(ANI_POSY);

	update_mouse_cursor();

	ds_writew(PIC_COPY_FLAG, 1);
	do_pic_copy(1);
	ds_writew(PIC_COPY_FLAG, 0);

	refresh_screen_size();

	g_pic_copy.dst = g_vga_memstart;

	/* restore screen coordinates */
	*(struct dummy*)(p_datseg + PIC_COPY_DS_RECT) = bak;
}

/**
 * \brief   select a teleport destination
 *
 * \return              costs for teleport in AE
 */
signed short select_teleport_dest(void)
{
	signed short l_si;
	signed short l_di;

	signed short answer;
	signed short done;
	signed short dungeon;
	signed short town;
	signed short ae_costs;
	signed short tw_bak;

	draw_main_screen();

	dungeon = gs_dungeon_index;
	town = gs_current_town;
	gs_current_town = ((gs_dungeon_index = 0));

	l_si = ((ds_readb(DNG_MAP_SIZE) == 16) ? 0 :
			((gs_x_target - 8 < 0) ? 0 :
			((gs_x_target - 8 > 15) ? 16 : gs_x_target - 8)));

	ds_writew(AUTOMAP_SELX, gs_x_target);
	ds_writew(AUTOMAP_SELY, gs_y_target);
	gs_dungeon_index = dungeon;
	gs_current_town = ((signed char)town);
	tw_bak = g_textbox_width;
	g_textbox_width = 3;

	render_automap(l_si);

	clear_ani_pal();

	draw_automap_to_screen();

	set_ani_pal(p_datseg + PALETTE_FIGHT2);

	if (ds_readb(DNG_MAP_SIZE) == 16) {
		draw_loc_icons(1, MENU_ICON_MAGIC);
	} else {
		draw_loc_icons(3, MENU_ICON_SCROLL_LEFT, MENU_ICON_SCROLL_RIGHT, MENU_ICON_MAGIC);
	}

	done = 0;

	do {
		handle_input();

		if ((ds_readw(MOUSE2_EVENT) != 0) || (ds_readw(ACTION) == ACTION_ID_PAGE_UP)) {

			if (ds_readb(DNG_MAP_SIZE) == 16) {
				answer = GUI_radio(get_ttx(616), 1, get_ttx(617)) - 1;
			} else {
				answer = GUI_radio(get_ttx(616), 3,
							 get_ttx(614),
							 get_ttx(615),
							 get_ttx(617)) - 1;
			}

			if (answer != -2) {
				ds_writew(ACTION, answer + ACTION_ID_ICON_1);
			}
		}

		if ((ds_readw(ACTION) == ACTION_ID_LEFT) &&
			(ds_readws(AUTOMAP_SELX) > 0) &&
			is_discovered(ds_readws(AUTOMAP_SELX) - 1, ds_readws(AUTOMAP_SELY)))
		{
			dec_ds_ws(AUTOMAP_SELX);
			render_automap(l_si);
			draw_automap_to_screen();

		} else if ((ds_readw(ACTION) == ACTION_ID_UP) &&
			(ds_readws(AUTOMAP_SELY) > 0) &&
			is_discovered(ds_readws(AUTOMAP_SELX), ds_readws(AUTOMAP_SELY) - 1))
		{
			dec_ds_ws(AUTOMAP_SELY);
			render_automap(l_si);
			draw_automap_to_screen();

		} else if ((ds_readw(ACTION) == ACTION_ID_RIGHT) &&
			(ds_readb(DNG_MAP_SIZE) - 1 > ds_readws(AUTOMAP_SELX)) &&
			is_discovered(ds_readws(AUTOMAP_SELX) + 1, ds_readws(AUTOMAP_SELY)))
		{
			inc_ds_ws(AUTOMAP_SELX);
			render_automap(l_si);
			draw_automap_to_screen();

		} else if ((ds_readw(ACTION) == ACTION_ID_DOWN) &&
			(ds_readws(AUTOMAP_SELY) < 16) &&
			is_discovered(ds_readws(AUTOMAP_SELX), ds_readws(AUTOMAP_SELY) + 1))
		{
			inc_ds_ws(AUTOMAP_SELY);
			render_automap(l_si);
			draw_automap_to_screen();
		}

		if (ds_readb(DNG_MAP_SIZE) != 16) {

			if ((ds_readw(ACTION) == ACTION_ID_ICON_1) && (l_si > 0)) {
				render_automap(--l_si);
				draw_automap_to_screen();
			}

			if ((ds_readw(ACTION) == ACTION_ID_ICON_2) && (l_si < 16)) {
				render_automap(++l_si);
				draw_automap_to_screen();
			}
		}

		if (((ds_readw(ACTION) == ACTION_ID_ICON_1) && (ds_readb(DNG_MAP_SIZE) == 16)) ||
			((ds_readw(ACTION) == ACTION_ID_ICON_3) && (ds_readb(DNG_MAP_SIZE) != 16)))
		{
			done = 1;
		}


	} while (done == 0);

	l_di = (ds_readb(DNG_MAP_SIZE) == 16) ?
		get_mapval_small(ds_readws(AUTOMAP_SELX), ds_readws(AUTOMAP_SELY)) :
		get_mapval_large(ds_readws(AUTOMAP_SELX), ds_readws(AUTOMAP_SELY));

	if (gs_current_town != TOWNS_NONE) {
		l_di = get_border_index(l_di);
	} else {
		l_di = div16(l_di);
	}

	ae_costs = 0;

	if ((ds_readws(AUTOMAP_SELX) == gs_x_target) &&
		(ds_readws(AUTOMAP_SELY) == gs_y_target))
	{
		ae_costs = 0;
		host_writeb((Bit8u*)g_dtp2, 0);

	} else if (((gs_dungeon_index != 0) && (l_di == 15)) ||
			((gs_current_town != TOWNS_NONE) && (((l_di >= 2) && (l_di <= 5)) ||
			(l_di == 6))))
	{
		strcpy(g_dtp2, get_ttx(611));
		ae_costs = -2;
	} else {
		host_writeb((Bit8u*)g_dtp2, 0);
		ae_costs = 15;
	}

	g_textbox_width = tw_bak;
	ds_writew(REQUEST_REFRESH, 1);

	return ae_costs;
}

/**
 * \brief   get location type at given position on map
 *
 * \param   x           x-coordinate of the building
 * \param   y           y-coordinate of the building
 * \return              type of location
 */
signed short get_maploc(signed short x, signed short y)
{
	Bit8u *locations_list_ptr;
	unsigned short pos_xy = TOWN_POS(x,y);

	// Wow. Original game has these hard-coded manipulation of the data.
	if (gs_current_town == TOWNS_THORWAL) {

		if (pos_xy == TOWN_POS(4,13)) {
			// schwarzer Finger
			return TOWN_TILE_BLACK_FINGER;
		} else if (pos_xy == TOWN_POS(5,2)) {
			// town exit to Vaermhag, which is located in a building (not a signpost).
			return TOWN_TILE_SIGNPOST;
		} else if ((pos_xy == TOWN_POS(5,1))
		        || (pos_xy == TOWN_POS(5,4) || (pos_xy == TOWN_POS(3,6)))
		        || (pos_xy == TOWN_POS(13,8) || (pos_xy == TOWN_POS(20,11)))
		        || (pos_xy == TOWN_POS(5,5) || (pos_xy == TOWN_POS(3,10))))
		{
			return TOWN_TILE_INN_OR_TAVERN;
		}
	} else if (gs_current_town == TOWNS_PREM) {
		if (pos_xy == TOWN_POS(28,9)) {
			// Inn "Zur Trutz". Why is this a special case here?
			return TOWN_TILE_INN_OR_TAVERN;
		}
	} else if (gs_current_town == TOWNS_GUDDASUNDEN) {
		if (pos_xy == TOWN_POS(1,14)) {
			// Harbor, which is located in a building (not a signpost).
			return TOWN_TILE_SIGNPOST;
		}
	}

	locations_list_ptr = p_datseg + LOCATIONS_LIST;

	do {

		if (host_readws(locations_list_ptr) + LOCATION_XY == pos_xy) {
			if (host_readbs(locations_list_ptr + LOCATION_LOCTYPE) == LOCTYPE_TEMPLE) {
				return TOWN_TILE_TEMPLE;
			}
			if (host_readbs(locations_list_ptr + LOCATION_LOCTYPE) == LOCTYPE_MERCHANT) {
				return TOWN_TILE_MERCHANT;
			}
			if (host_readbs(locations_list_ptr + LOCATION_LOCTYPE) == LOCTYPE_SMITH) {
				return TOWN_TILE_SMITH;
			}
			if ((host_readbs(locations_list_ptr + LOCATION_LOCTYPE) == LOCTYPE_TAVERN) || (host_readbs(locations_list_ptr + LOCATION_LOCTYPE) == LOCTYPE_INN)) {
				return TOWN_TILE_INN_OR_TAVERN;
			}
			if (host_readbs(locations_list_ptr + LOCATION_LOCTYPE) == LOCTYPE_HEALER) {
				return TOWN_TILE_HEALER;
			}
		}

		locations_list_ptr += SIZEOF_LOCATION;
	} while (host_readws(locations_list_ptr) != -1);

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
