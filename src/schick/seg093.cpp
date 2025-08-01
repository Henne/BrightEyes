/**
 *	Rewrite of DSA1 v3.02_de functions of seg093 (travelmode)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg093.cpp
 */

#include <stdio.h>
#include <string.h>

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg025.h"
#include "seg028.h"
#include "seg075.h"
#include "seg094.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short do_travel_mode(void)
{
	signed short l_di;
	signed short i;
	signed short answer;
	signed short l4;
	signed short route_id;
	Bit8u *signpost_ptr;
	signed short l6;
	signed short l7;
	signed short bak1;
	signed short tw_bak;
	char *destinations_tab[6];

	bak1 = g_wallclock_update;
	ds_writeb(ROUTE59_FLAG, (unsigned char)(g_wallclock_update = (unsigned short)ds_writeb(TRAVEL_DETOUR, 0)));
	ds_writeb(CURRENT_TOWN, gs_current_typeindex);

	update_mouse_cursor();

	for (i = 0; i < 6; i++)
	{
		g_food_message_shown[i] = 0;
	}

	if (ds_readb(TRAVEL_MAP_LOADED) != ds_readb(SHOW_TRAVEL_MAP))
	{
		load_map();
	}

	memmove((void*)g_renderbuf_ptr, (void*)(Bit8u*)ds_readd(TRAVEL_MAP_PTR), 64000);

	map_effect((Bit8u*)g_renderbuf_ptr);

	wait_for_vsync();

	set_palette((Bit8u*)ds_readd(TRAVEL_MAP_PTR) + 64000 + 2, 0, 0x20);

	refresh_screen_size();

	prepare_map_marker();

	ds_writew(MOUSE1_EVENT1, 0);

	signpost_ptr = p_datseg + SIGNPOSTS;

	do {
		if (ds_readws(REQUEST_REFRESH) != 0)
		{
			update_mouse_cursor();

			if (ds_readb(TRAVEL_MAP_LOADED) != ds_readb(SHOW_TRAVEL_MAP))
			{
				load_map();
			}

			wait_for_vsync();

			/* TODO: update window */
			memmove((void*)((Bit8u*)ds_readd(FRAMEBUF_PTR)), (void*)((Bit8u*)ds_readd(TRAVEL_MAP_PTR)), 320 * 200);

			wait_for_vsync();

			set_palette((Bit8u*)ds_readd(TRAVEL_MAP_PTR) + 64000 + 2, 0, 0x20);

			refresh_screen_size();

			ds_writew(REQUEST_REFRESH, 0);
		}

		if (host_readbs(signpost_ptr + SIGNPOST_TOWN) == ds_readbs(CURRENT_TOWN) && host_readb(signpost_ptr + SIGNPOST_TYPEINDEX) == ds_readw(CURRENT_SIGNPOST))
		{
			while (1) {
				handle_input();

				if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP)
				{
					i = 0;
					while ((l_di = host_readb((Bit8u*)(host_readd(signpost_ptr + SIGNPOST_LAND_ROUTES)) + i)) != 255)
					{
						destinations_tab[i] = get_ttx(235 + ds_writebs(TRV_MENU_TOWNS + i,
						    (answer = ds_readb((LAND_ROUTES - SIZEOF_LAND_ROUTE + LAND_ROUTE_TOWN_1) + SIZEOF_LAND_ROUTE * l_di)) != ds_readbs(CURRENT_TOWN) ?
						    (signed char) answer : ds_readbs((LAND_ROUTES - SIZEOF_LAND_ROUTE + LAND_ROUTE_TOWN_2) + SIZEOF_LAND_ROUTE * l_di)));

						i++;
					}

					ds_writeb(TRV_MENU_TOWNS + i, ds_readbs(CURRENT_TOWN));
					destinations_tab[i] = get_ttx(613);
					i++;

					sprintf((char*)g_text_output_buf,
						get_ttx(545),
						get_ttx(235 + ds_readbs(CURRENT_TOWN)));

					tw_bak = g_textbox_width;
					g_textbox_width = 4;

					answer = GUI_radio(g_text_output_buf, (signed char)i,
								destinations_tab[0],
								destinations_tab[1],
								destinations_tab[2],
								destinations_tab[3],
								destinations_tab[4],
								destinations_tab[5]) - 1;

					g_textbox_width = tw_bak;

					ds_writew(CURRENT_TOWN_ANIX, 0);

					set_and_spin_lock();

					if (i - 1 == answer || answer == -2)
					{
						ds_writeb(SHOW_TRAVEL_MAP, 0);
						ds_writeb(DIRECTION, (ds_readb(DIRECTION) + 2) & 3);
						break;
					}

					route_id = host_readb((Bit8u*)(host_readd(signpost_ptr + SIGNPOST_LAND_ROUTES)) + answer);
					ds_writew(TRV_DESTINATION, ds_readbs(TRV_MENU_TOWNS + answer));

					if (!get_current_season() &&
						(route_id == 31 || route_id == 41 || route_id == 47 || route_id == 48 || route_id == 49))
					{
						/* routes not available in winter */
						GUI_input(get_tx(69), 0);
						break;
					}

					ds_writew(WALLCLOCK_X, ds_readws(BASEPOS_X) + 120);
					ds_writew(WALLCLOCK_Y, ds_readws(BASEPOS_Y) + 87);

					g_wallclock_update = 1;

					TM_func1(host_readb((Bit8u*)(host_readd(signpost_ptr + SIGNPOST_LAND_ROUTES)) + answer),
						(ds_readbs((LAND_ROUTES - SIZEOF_LAND_ROUTE) + SIZEOF_LAND_ROUTE * host_readb((Bit8u*)(host_readd(signpost_ptr + SIGNPOST_LAND_ROUTES)) + answer)) == ds_readbs(CURRENT_TOWN) ? 0 : 1));
					g_wallclock_update = 0;

					if (ds_readb(ROUTE59_FLAG) != 0)
					{
						TM_func9();
					}

					if (ds_readw(TRV_RETURN) == 2)
					{
						if (ds_readb(TRAVEL_DETOUR) != 0 && ds_readb(TRAVEL_DETOUR) != 99)
						{
							DNG_enter_dungeon(ds_readb(TRAVEL_DETOUR));
						}
						break;
					}

					TM_enter_target_town();

					if (!ds_readb(TRAVEL_DETOUR) && ds_readw(GAME_STATE) == GAME_STATE_MAIN)
					{
						ds_writeb(CURRENT_TOWN, (signed char)ds_readw(TRAVEL_DESTINATION_TOWN_ID));
						ds_writew(X_TARGET_BAK, ds_readw(TRAVEL_DESTINATION_X));
						ds_writew(Y_TARGET_BAK, ds_readw(TRAVEL_DESTINATION_Y));
						ds_writeb(DIRECTION, (ds_readb(TRAVEL_DESTINATION_VIEWDIR) + 2) & 3);

					} else if (ds_readw(GAME_STATE) == GAME_STATE_MAIN && ds_readb(TRAVEL_DETOUR) != 99)
					{
						DNG_enter_dungeon(ds_readb(TRAVEL_DETOUR));
					}

					break;

				} else if (ds_readw(MOUSE1_EVENT1) != 0 )
				{

					for (i = 0, l4 = -1; i < 52; i++)
					{
						if (is_mouse_in_rect(l_di - 4,
									answer - 4,
							    (l_di = ds_readws(TOWN_POSITIONS + 4 * i)) + 4,
							    (answer = ds_readws((TOWN_POSITIONS + 2) + 4 * i)) + 4))
						{
							l4 = i;
							break;
						}
					}

					if (l4 == -1 && (l_di = get_mouse_action(ds_readws(MOUSE_POSX), ds_readws(MOUSE_POSY), p_datseg + ACTION_TABLE_TRAVELMAP)))
					{
						l4 = l_di + 51;
					}

					if (l4 != -1)
					{
						answer = ds_readws(CURRENT_TOWN_ANIX);
						ds_writew(CURRENT_TOWN_ANIX, 0);
						l6 = ds_readws(BASEPOS_X);
						l7 = ds_readws(BASEPOS_Y);
						ds_writew(BASEPOS_Y, 0);
						ds_writew(BASEPOS_X, (ds_readws(MOUSE_POSX) >= 0 && ds_readws(MOUSE_POSX) <= 159 ? 80 : -80));

						set_and_spin_lock();

						GUI_input(get_tx(l4), 0);

						ds_writew(BASEPOS_X, l6);
						ds_writew(BASEPOS_Y, l7);
						ds_writew(CURRENT_TOWN_ANIX, answer);
					}

					ds_writew(MOUSE1_EVENT1, 0);
				}
			}
			break;
		}

		signpost_ptr += SIZEOF_SIGNPOST;

	} while (host_readb(signpost_ptr) != 255);

	ds_writew(CURRENT_TOWN_ANIX, ds_writew(CURRENT_TOWN_ANIY, ds_writew(SELECTED_TOWN_ANIX, ds_writew(SELECTED_TOWN_ANIY, 0))));

	i = load_archive_file(ARCHIVE_FILE_COMPASS);
	read_archive_file(i, g_buffer6_ptr, 5000);
	close(i);

	ds_writeb(SHOW_TRAVEL_MAP, (signed char)ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, ds_writew(CURRENT_TOWN_OVER, ds_writew(TRV_MENU_SELECTION, 0)))));

	if (!ds_readb(TRAVEL_DETOUR))
	{
		g_wallclock_update = 0;
		leave_location();

	} else if (ds_readb(TRAVEL_DETOUR) != 99)
	{
		ds_writeb(CURRENT_TOWN, TOWNS_NONE);
	}

	if (ds_readb(PP20_INDEX) == 5)
	{
		memset((void*)g_dtp2, 0, 0xc0);

		memcpy((void*)(g_dtp2 + 0xc0), (void*)((Bit8u*)ds_readd(TRAVEL_MAP_PTR) + 64000 + 2), 0x60);

		memcpy((void*)(g_dtp2 + 0x120),	(void*)g_palette_special, 0x60);

		for (i = 0; i < 64; i++)
		{
			pal_fade((Bit8u*)g_dtp2 + 0xc0, (Bit8u*)g_dtp2);
			pal_fade((Bit8u*)g_dtp2 + 0x120, (Bit8u*)g_dtp2 + 0x60);
			wait_for_vsync();
			set_palette((Bit8u*)g_dtp2 + 0xc0, 0x00, 0x20);
			set_palette((Bit8u*)g_dtp2 + 0x120, 0xe0, 0x20);
		}

		wait_for_vsync();
		set_palette((Bit8u*)g_dtp2, 0x80, 0x40);
		set_palette((Bit8u*)g_dtp2, 0x00, 0x20);

		do_fill_rect((Bit8u*)ds_readd(FRAMEBUF_PTR), 0, 0, 319, 199, 0);
	}

	g_current_ani = ds_writebs(CITY_AREA_LOADED, ds_writebs(PP20_INDEX, -1));
	ds_writew(REQUEST_REFRESH, (unsigned short)(g_fading_state = 1));
	g_wallclock_update = bak1;

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
