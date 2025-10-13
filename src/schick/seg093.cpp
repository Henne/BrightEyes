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
	struct trv_start_point *signpost_ptr;
	signed short l6;
	signed short l7;
	signed short bak1;
	signed short tw_bak;
	char *destinations_tab[6];

	bak1 = g_wallclock_update;
	g_route59_flag = g_wallclock_update = gs_travel_detour = 0;
	gs_current_town = gs_current_typeindex;

	update_mouse_cursor();

	for (i = 0; i < 6; i++)
	{
		g_food_message_shown[i] = 0;
	}

	if (g_travel_map_loaded != gs_show_travel_map)
	{
		load_map();
	}

	memmove((void*)g_renderbuf_ptr, (void*)gs_travel_map_ptr, 64000);

	map_effect((Bit8u*)g_renderbuf_ptr);

	wait_for_vsync();

	set_palette(gs_travel_map_ptr + 64000 + 2, 0, 0x20);

	refresh_screen_size();

	prepare_map_marker();

	g_mouse1_event1 = 0;

	signpost_ptr = &g_signposts[0];

	do {
		if (g_request_refresh)
		{
			update_mouse_cursor();

			if (g_travel_map_loaded != gs_show_travel_map)
			{
				load_map();
			}

			wait_for_vsync();

			/* TODO: update window */
			memmove((void*)g_vga_memstart, (void*)gs_travel_map_ptr, 320 * 200);

			wait_for_vsync();

			set_palette(gs_travel_map_ptr + 64000 + 2, 0, 0x20);

			refresh_screen_size();

			g_request_refresh = 0;
		}

		if ((signpost_ptr->town == gs_current_town) && (signpost_ptr->typeindex == gs_current_signpost))
		{
			while (1) {
				handle_input();

				if (g_mouse2_event || g_action == ACTION_ID_PAGE_UP)
				{
					i = 0;
					while ((l_di = signpost_ptr->end_points[i]) != 0xff)
					{
						destinations_tab[i] = get_ttx(235 + (gs_trv_menu_towns[i] = (
							(answer = g_land_routes[l_di - 1].town1_id) != gs_current_town ?
							(signed char) answer : g_land_routes[l_di - 1].town2_id)));

						i++;
					}

					gs_trv_menu_towns[i] = gs_current_town;
					destinations_tab[i] = get_ttx(613);
					i++;

					sprintf(g_text_output_buf, get_ttx(545), get_ttx(235 + gs_current_town));

					tw_bak = g_textbox_width;
					g_textbox_width = 4;

					answer = GUI_radio(g_text_output_buf, i,
								destinations_tab[0], destinations_tab[1], destinations_tab[2],
								destinations_tab[3], destinations_tab[4], destinations_tab[5]) - 1;

					g_textbox_width = tw_bak;

					g_current_town_anix = 0;

					set_and_spin_lock();

					if (i - 1 == answer || answer == -2)
					{
						gs_show_travel_map = 0;
						gs_direction = ((gs_direction + 2) & 3);
						break;
					}

					route_id = signpost_ptr->end_points[answer];
					gs_trv_destination = gs_trv_menu_towns[answer];

					if (!get_current_season() &&
						(route_id == 31 || route_id == 41 || route_id == 47 || route_id == 48 || route_id == 49))
					{
						/* routes not available in winter */
						GUI_input(get_tx(69), 0);
						break;
					}

					g_wallclock_x = g_basepos_x + 120;
					g_wallclock_y = g_basepos_y + 87;

					g_wallclock_update = 1;

					TM_func1(signpost_ptr->end_points[answer],
						(g_land_routes[signpost_ptr->end_points[answer] - 1].town1_id == gs_current_town ? 0 : 1));
					g_wallclock_update = 0;

					if (g_route59_flag)
					{
						TM_func9();
					}

					if (gs_trv_return == 2)
					{
						if (gs_travel_detour && gs_travel_detour != 99)
						{
							DNG_enter_dungeon(gs_travel_detour);
						}
						break;
					}

					TM_enter_target_town();

					if (!gs_travel_detour && g_game_state == GAME_STATE_MAIN)
					{
						gs_current_town = gs_travel_destination_town_id;
						gs_x_target_bak = gs_travel_destination_x;
						gs_y_target_bak = gs_travel_destination_y;
						gs_direction = (gs_travel_destination_viewdir + 2) & 3;

					} else if (g_game_state == GAME_STATE_MAIN && gs_travel_detour != 99)
					{
						DNG_enter_dungeon(gs_travel_detour);
					}

					break;

				} else if (g_mouse1_event1)
				{

					for (i = 0, l4 = -1; i < 52; i++)
					{
						if (is_mouse_in_rect(l_di - 4, answer - 4,
								(l_di = g_town_positions[i].x) + 4,
								(answer = g_town_positions[i].y) + 4))
						{
							l4 = i;
							break;
						}
					}

					if (l4 == -1 && (l_di = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_travelmap)))
					{
						l4 = l_di + 51;
					}

					if (l4 != -1)
					{
						answer = g_current_town_anix;
						g_current_town_anix = 0;
						l6 = g_basepos_x;
						l7 = g_basepos_y;
						g_basepos_y = 0;
						g_basepos_x = (g_mouse_posx >= 0 && g_mouse_posx <= 159 ? 80 : -80);

						set_and_spin_lock();

						GUI_input(get_tx(l4), 0);

						g_basepos_x = l6;
						g_basepos_y = l7;
						g_current_town_anix = answer;
					}

					g_mouse1_event1 = 0;
				}
			}
			break;
		}

		signpost_ptr++;

	} while (signpost_ptr->town != -1);

	g_current_town_anix = g_current_town_aniy = g_selected_town_anix = g_selected_town_aniy = 0;

	i = load_archive_file(ARCHIVE_FILE_COMPASS);
	read_archive_file(i, g_buffer6_ptr, 5000);
	close(i);

	gs_show_travel_map = g_basepos_x = g_basepos_y = g_current_town_over = g_trv_menu_selection = 0;

	if (!gs_travel_detour)
	{
		g_wallclock_update = 0;
		leave_location();

	} else if (gs_travel_detour != 99)
	{
		gs_current_town = TOWNS_NONE;
	}

	if (g_pp20_index == 5)
	{
		memset((void*)g_dtp2, 0, 0xc0);

		memcpy((void*)(g_dtp2 + 0xc0), (void*)(gs_travel_map_ptr + 64000 + 2), 0x60);

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

		do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);
	}

	g_current_ani = g_city_area_loaded = g_pp20_index = -1;
	g_request_refresh = g_fading_state = 1;
	g_wallclock_update = bak1;

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
