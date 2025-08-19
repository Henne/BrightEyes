/**
 *	Rewrite of DSA1 v3.02_de functions of seg094 (travelmode)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg094.cpp
 */

#include <string.h>

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg025.h"
#include "seg028.h"
#include "seg094.h"
#include "seg097.h"
#include "seg109.h"
#include "seg117.h"
#include "seg119.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void prepare_map_marker(void)
{
	signed short i;
	signed short fd;
	signed short dummy;
	struct nvf_desc nvf;

	/* load OBJECTS.NVF */
	fd = load_archive_file(ARCHIVE_FILE_OBJECTS_NVF);

	read_archive_file(fd, g_buffer6_ptr, 2000);

	close(fd);


	for (i = 0; i < 10; i++) {
		nvf.dst = g_buffer6_ptr + 100 * i + 1000;
		nvf.src = g_buffer6_ptr;
		nvf.no = i;
		nvf.type = 0;
		nvf.width = (Bit8u*)&dummy;
		nvf.height = (Bit8u*)&dummy;

		process_nvf(&nvf);
	}

	set_textbox_positions(gs_current_town);
}

void set_textbox_positions(signed short town_id)
{
	signed short x;
	signed short r_dx;
	signed short y;

	/* zero some global variables */
	g_current_town_over = g_trv_menu_selection = g_selected_town_anix = g_selected_town_aniy = 0;


	x = g_current_town_anix = ds_readws((TOWN_POSITIONS-4) + 4 * town_id);
	y = g_current_town_aniy = ds_readws((TOWN_POSITIONS-4) + 4 * town_id + 2);

	r_dx = (x >= 0 && x <= 159) ? (y >= 0 && y <= 99 ? 3 : 1) : (y >= 0 && y <= 99 ? 2 : 0);

	g_basepos_x = ((!r_dx || r_dx == 2) ? -80 : 80);
	g_basepos_y = ((!r_dx || r_dx == 1) ? -40 : 40);
}

/**
 * \brief   ???
 *
 * \param   route_no    number of the route
 * \param   backwards   0 = travel the route forwards, 1 = travel backwards
 */
void TM_func1(signed short route_no, signed short backwards)
{
	Bit8u* fb_start;
	Bit8u *hero;
	struct struct_route_tevent *tevent_ptr;
	signed short bak1;
	signed short bak2;
	signed short last_tevent_no;
	signed short answer;

	ds_writeb(TRAVELING, 1);
	last_tevent_no = -1;
	gs_route_course_ptr = (Bit8u*)((g_buffer9_ptr + host_readws((Bit8u*)g_buffer9_ptr + 4 * (route_no - 1))) + 0xecL);
	fb_start = g_vga_memstart;
	gs_route_course_ptr += 4;

	memset((void*)g_trv_track_pixel_bak, 0xaa, 500);
	/* TODO: move this pointer out of the game state, verify if that works correctly.
	 * 		Can be replaced by a locvar! */
	gs_travel_route_ptr = &g_land_routes[route_no - 1];
	gs_travel_speed = 166;
	gs_route_total_steps = (TM_get_track_length(gs_route_course_ptr));
	gs_route_length = (gs_travel_route_ptr->distance * 100);
	gs_route_duration = (gs_route_length / (gs_travel_speed + gs_travel_route_ptr->speed_mod * gs_travel_speed / 10) * 60);
	gs_route_timedelta = (gs_route_duration / gs_route_total_steps);
	gs_route_stepsize = gs_route_length / gs_route_total_steps;

	if (gs_route_stepsize == 0)
	{
		gs_route_stepsize = 1;
	}

	if (backwards)
	{
		while (host_readws(gs_route_course_ptr) != -1)
		{
			gs_route_course_ptr += 4;
		}
		gs_route_course_ptr -= 4;
	}

	memset((void*)gs_route_tevents, (gs_route_stepcount = 0), 15 * sizeof(struct_route_tevent));
	memset(g_route_tevent_flags, 0, 15);

	/* TODO: move this pointer out of the game state, verify if that works correctly.
	 * 		Can be replaced by a locvar! */
	gs_tevents_tab_ptr = &g_tevents_tab[0];
	/* Forward pointer to entries associated with current route. */
	while (((unsigned char)gs_tevents_tab_ptr->route_id != route_no) && (gs_tevents_tab_ptr->route_id != -1))
	{
		gs_tevents_tab_ptr++;
	}

	gs_trv_return = 0;
	gs_route_course_start = gs_route_course_ptr;
	gs_route_dayprogress = (gs_travel_speed + gs_travel_route_ptr->speed_mod * gs_travel_speed / 10) * 18;

	/* random section starts */
	if (gs_quested_months > 3)
	{
		if ((gs_route_informer_flag = (random_schick(100) <= 2) ? 1 : 0))
		{
			gs_route_informer_time = random_schick(gs_route_dayprogress);
		}
	} else {
		gs_route_informer_flag = 0;
	}

	if ((gs_route_encounter_flag = random_schick(100) <= gs_travel_route_ptr->encounters ? 1 : 0))
	{
		gs_route_encounter_time = random_schick(gs_route_dayprogress);
	}

	if ((gs_route_fight_flag = (random_schick(100) <= gs_travel_route_ptr->fights / 3 ? 1 : 0)))
	{
		gs_route_fight_time = random_schick(gs_route_dayprogress);
	}

	gs_route_dayprogress = 0;
	/* random section ends */

	while ((gs_tevents_tab_ptr->route_id != -1) && ((unsigned char)gs_tevents_tab_ptr->route_id == route_no))
	{
		tevent_ptr = &gs_route_tevents[gs_route_stepcount];
		tevent_ptr->place = gs_tevents_tab_ptr->place;
		tevent_ptr->tevent_id = gs_tevents_tab_ptr->tevent_id;

		if (backwards)
		{
			tevent_ptr->place = gs_travel_route_ptr->distance - tevent_ptr->place;
		}

		tevent_ptr->place *= 100;
		gs_tevents_tab_ptr++;
		gs_route_stepcount++;
	}

	gs_route_stepcount = gs_route_progress = gs_travel_detour = 0;

	while (host_readws(gs_route_course_ptr + 2 * (gs_route_mousehover = 0)) != -1 &&
		!gs_travel_detour &&
		g_game_state == GAME_STATE_MAIN)
	{
		if (is_mouse_in_rect(host_readws(gs_route_course_ptr) - 16,
					host_readws(gs_route_course_ptr + 2) - 16,
					host_readws(gs_route_course_ptr) + 16,
					host_readws(gs_route_course_ptr + 2) + 16))
		{
			update_mouse_cursor();
			gs_route_mousehover = 1;
		}

		if (gs_trv_return == 2)
		{
			gs_route_stepcount--;

			/* restore the pixel from the map */
			*(fb_start + host_readws(gs_route_course_ptr + 2) * 320
				+ host_readws(gs_route_course_ptr)) =
				g_trv_track_pixel_bak[gs_route_stepcount];

			g_trv_track_pixel_bak[gs_route_stepcount] = 0xaa;
		} else {
			/* save the old pixel from the map */
			g_trv_track_pixel_bak[gs_route_stepcount] =
				*(fb_start + host_readws(gs_route_course_ptr + 2) * 320 + host_readws(gs_route_course_ptr));

			gs_route_stepcount++;

			/* write a new pixel */
			*(fb_start + host_readws(gs_route_course_ptr + 2) * 320 + host_readws(gs_route_course_ptr)) = 0x1c;
		}

		if (gs_route_mousehover) {
			refresh_screen_size();
		}

		gs_trv_i = 0;
		while (gs_route_timedelta / 2 > gs_trv_i)
		{
			handle_input();

			if (gs_trv_i % 2)
			{
				wait_for_vsync();
			}

			timewarp(MINUTES(2));

			gs_trv_i++;
		}

		gs_route_progress += (gs_trv_return == 2 ? -gs_route_stepsize : gs_route_stepsize);
		gs_route_dayprogress += gs_route_stepsize;

		if (g_mouse2_event || g_action == ACTION_ID_PAGE_UP)
		{
			if (!gs_forcedmarch_timer)
			{
				answer = GUI_radio(get_ttx(815), 3, get_tx(74), get_ttx(816), get_ttx(613));
			} else {
				answer = GUI_radio(get_ttx(815), 2, get_ttx(816), get_ttx(613));
			}

			if (answer == 1 && !gs_forcedmarch_timer) /* Gewaltmarsch */
			{
				/* do forced march for 2 days */
				gs_forcedmarch_le_cost = random_schick(10);
				gs_travel_speed = gs_route_stepcount + 197;
				gs_forcedmarch_timer = 2;
				gs_route_duration = (gs_route_length / (gs_travel_speed + (gs_travel_route_ptr->speed_mod * gs_travel_speed) / 10) * 60);
				gs_route_timedelta = (gs_route_duration / gs_route_total_steps);
				/* Remark: gs_forcedmarch_le_cost = gs_forcedmarch_le_cost / 2; */
				gs_forcedmarch_le_cost >>= 1;

				hero = get_hero(0);
				for (gs_trv_i = 0; gs_trv_i <= 6; gs_trv_i++, hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
						!hero_dead(hero))
					{
						sub_hero_le(hero, gs_forcedmarch_le_cost);
					}
				}

			} else if ((answer == 1 && gs_forcedmarch_timer != 0) ||
					(answer == 2 && !gs_forcedmarch_timer)) /* Kampieren */
				// Depending on gs_forcedmarch_timer, the "Kampieren" answer is on position 1 or 2.
			{
				/* make gather_herbs and replenish_stocks practically impossible */
				g_wildcamp_sleep_quality = -3;
				g_good_camp_place = 99;
				gs_current_loctype = LOCTYPE_WILDCAMP;

				do_location();

				g_good_camp_place = gs_current_loctype = g_wildcamp_sleep_quality = 0;
				g_wallclock_update = 0;
				g_request_refresh = 2;
			}
		}

		if (gs_route_encounter_flag && gs_route_dayprogress >= gs_route_encounter_time && g_game_state == GAME_STATE_MAIN)
		{
			random_encounter(route_no);
			gs_route_encounter_flag = 0;

		} else if (gs_route_fight_flag && gs_route_dayprogress >= gs_route_fight_time && g_game_state == GAME_STATE_MAIN)
		{
			do_wild8_fight();

		} else if (gs_route_informer_flag && gs_route_dayprogress >= gs_route_informer_time && g_game_state == GAME_STATE_MAIN)
		{
			gs_current_typeindex = (random_schick(100) <= 50 ? 10 : 12);
			bak1 = g_basepos_x;
			bak2 = g_basepos_y;
			g_basepos_x = g_basepos_y = 0;
			do_informer();
			g_basepos_x = bak1;
			g_basepos_y = bak2;

		}

		if (g_game_state == GAME_STATE_MAIN)
		{
			for (gs_trv_i = 0; gs_trv_i < 15; gs_trv_i++)
			{
				if (((gs_route_tevents[gs_trv_i].place <= gs_route_progress) &&
					(gs_trv_return == 0) &&
					!g_route_tevent_flags[gs_trv_i]) ||
					((gs_route_tevents[gs_trv_i].place >= gs_route_progress) &&
					(gs_trv_return == 2) &&
					(g_route_tevent_flags[gs_trv_i] == 2)))
				{
					if (gs_route_tevents[gs_trv_i].tevent_id)
					{
						TRV_event(gs_route_tevents[(last_tevent_no = gs_trv_i)].tevent_id);

						if (!ds_readbs((TEVENTS_REPEATABLE-1) + gs_route_tevents[gs_trv_i].tevent_id))
						{
							g_route_tevent_flags[gs_trv_i] = 1;

						} else if (gs_trv_return == 0)
						{
							g_route_tevent_flags[gs_trv_i] = 2;
						} else {
							g_route_tevent_flags[gs_trv_i] = 0;
						}

						if (g_request_refresh != 0 && !gs_travel_detour)
						{
							g_wallclock_update = 0;
						}
					}
				}
			}
		}

	        /* night camp */
		if (gs_day_timer >= HOURS(20) && !gs_travel_detour && g_game_state == GAME_STATE_MAIN &&
			2 * gs_route_stepsize < gs_route_progress &&
			gs_route_length - 2 * gs_route_stepsize > gs_route_progress)
		{
			g_wallclock_update = 0;

			if (g_request_refresh != 0)
			{
				load_map();
			}

			GUI_input(get_tx(70), 0);

			gs_current_loctype = LOCTYPE_WILDCAMP;
			do_location();
			gs_current_loctype = LOCTYPE_NONE;

			g_request_refresh = 2;
			g_wallclock_update = 0;

			if (g_game_state == GAME_STATE_MAIN)
			{
				/* figure out encounters etc. for next day */
				gs_route_dayprogress = ((gs_travel_speed + (gs_travel_route_ptr->speed_mod * gs_travel_speed / 10)) * 18);

				if ((gs_route_encounter_flag = (random_schick(100) <= gs_travel_route_ptr->encounters ? 1 : 0)))
				{
					gs_route_encounter_time = random_schick(gs_route_dayprogress);
				}

				if ((gs_route_fight_flag = random_schick(100) <= gs_travel_route_ptr->fights / 3 ? 1 : 0) != 0)
				{
					gs_route_fight_time = random_schick(gs_route_dayprogress);
				}

				if (gs_quested_months > 3)
				{
					if ((gs_route_informer_flag = random_schick(100) <= 2 ? 1 : 0) != 0)
					{
						gs_route_informer_time = random_schick(gs_route_dayprogress);
					}
				} else {
					gs_route_informer_flag = 0;
				}

				gs_route_dayprogress = 0;
			}
		}

		if (g_check_disease && !gs_travel_detour && g_game_state == GAME_STATE_MAIN)
		{
			disease_effect();
		}

		if (g_request_refresh != 0 && !gs_travel_detour && g_game_state == GAME_STATE_MAIN)
		{
			update_mouse_cursor();
			load_map();
			/* TODO: update window */
			memmove((void*)g_vga_memstart, (void*)gs_travel_map_ptr, 320 * 200);

			wait_for_vsync();
			set_palette(gs_travel_map_ptr + 64000 + 2, 0, 0x20);

			g_pp20_index = 5;
			gs_trv_i = 0;
			gs_route_course_ptr2 = gs_route_course_start;

			if (route_no == 59)
			{
				TM_func8(0);
			}

			/* Redraw the track on the map */
			while (g_trv_track_pixel_bak[gs_trv_i++] != 0xaa)
			{
				*(fb_start + host_readws(gs_route_course_ptr2 + 2) * 320 + host_readws(gs_route_course_ptr2)) =  0x1c;
				gs_route_course_ptr2 += 2 * (!backwards ? 2 : -2);
			}

			refresh_screen_size();

			if (g_request_refresh == 2 && route_no != 59)
			{
				/* Return or continue? */
				if (GUI_radio(get_tx(71), 2, get_tx(72), get_tx(73)) == 2)
				{
					gs_trv_return = (gs_trv_return == 0 ? 1 : -1);

					if (last_tevent_no != -1)
					{
						g_route_tevent_flags[last_tevent_no] = (gs_trv_return == 1 ? 0 : 2);
					}
				}
			}
			g_wallclock_x = g_basepos_x + 120;
			g_wallclock_y = g_basepos_y + 87;
			g_wallclock_update = 1;
			g_request_refresh = 0;
		}

		if (gs_trv_return == 1 || gs_trv_return == -1)
		{
			gs_trv_return = (gs_trv_return == 1 ? 2: 0);

			gs_route_course_ptr += 2 * ((!backwards && gs_trv_return == 0) || (backwards && gs_trv_return != 0) ? -2 : 2);
		}

		gs_route_course_ptr += 2 * ((!backwards && gs_trv_return == 0) || (backwards && gs_trv_return != 0) ? 2 : -2);
	}

	if (g_game_state == GAME_STATE_MAIN && !gs_travel_detour && gs_trv_return != 2)
	{
		update_mouse_cursor();

		do {
			gs_route_course_ptr += 2 * (!backwards ? -2 : 2);
			gs_route_stepcount--;
			*(fb_start + host_readws(gs_route_course_ptr + 2) * 320 + host_readws(gs_route_course_ptr)) =
				g_trv_track_pixel_bak[gs_route_stepcount];

		} while (host_readws(gs_route_course_ptr) != -1);

		if (route_no == 59)
		{
			TM_func8(1);
		}

		refresh_screen_size();
	}

	ds_writeb(TRAVELING, 0);
}

#if defined(__BORLANDC__)
/* continue travel after arrival while still on map */
signed short TM_unused1(Bit8u* signpost_ptr, signed short old_route_no)
{
	signed short route_no1;
	signed short route_id;
	signed short answer;
	signed short town_i;
	signed short route_no2;
	signed short town;
	signed short old_route_id;
	char *destinations_tab[7];

	old_route_id = host_readb((Bit8u*)(host_readd((Bit8u*)(signpost_ptr) + SIGNPOST_LAND_ROUTES)) + old_route_no) - 1;
	gs_current_town = town = gs_trv_destination;
	signpost_ptr = ((Bit8u*)p_datseg + SIGNPOSTS);

	do {
		if (host_readb(signpost_ptr + SIGNPOST_TOWN) == town)
		{
			route_no1 = 0;
			while (host_readbs((Bit8u*)(host_readd(signpost_ptr + SIGNPOST_LAND_ROUTES)) + route_no1) != -1)
			{
				if (host_readb((Bit8u*)(host_readd(signpost_ptr + SIGNPOST_LAND_ROUTES)) + route_no1) - 1 == old_route_id &&
					(route_no1 || host_readb((Bit8u*)(host_readd(signpost_ptr + SIGNPOST_LAND_ROUTES)) + (route_no1 + 1)) != 255))
				{
					town_i = route_no2 = 0;
					while ((route_id = host_readb((Bit8u*)(host_readd(signpost_ptr + SIGNPOST_LAND_ROUTES)) + route_no2)) != 255)
					{
						if (route_no2 != route_no1)
						{
							destinations_tab[town_i++] = get_ttx(235 + (gs_trv_menu_towns[town_i] =
								((answer = g_land_routes[route_id - 1].town1_id) != gs_current_town ?
									(signed char)answer : g_land_routes[route_id - 1].town2_id)
                            ));
						}
						route_no2++;
					}

					gs_trv_menu_towns[town_i] = (signed char)town;
					destinations_tab[town_i] = get_ttx(547);
					town_i++;

#if defined(__BORLANDC__)
					gs_tm_unused1_ptr = signpost_ptr;
#endif
					set_textbox_positions(town);
					answer = GUI_radio(get_ttx(546), (signed char)town_i,
								destinations_tab[0],
								destinations_tab[1],
								destinations_tab[2],
								destinations_tab[3],
								destinations_tab[4],
								destinations_tab[5]);

					if (answer == -1)
					{
						answer = town_i;
					}

					gs_trv_destination = gs_trv_menu_towns[answer - 1];
					return answer;
				}

				route_no1++;
			}
		}

		signpost_ptr += SIZEOF_SIGNPOST;

	} while (host_readbs((Bit8u*)(signpost_ptr)) != -1);

	return -1;
}
#endif

signed short TM_get_track_length(Bit8u *track)
{
	signed short length;

	length = 0;

	while (host_readws(track) != -1)
	{
		track += 4;
		length++;
	}
	return length;
}

signed short TM_enter_target_town(void)
{
	signed short tmp;
	signed short signpost_id;
	signed short tmp2;
	Bit8u *signpost_ptr;
	Bit8u *locations_list_ptr;

	signpost_id = 0;
	gs_travel_destination_town_id = gs_trv_destination;
	signpost_id = 1;

	if (signpost_id)
	{
		signpost_ptr = p_datseg + SIGNPOSTS;
		signpost_id = 0;
		do {
			if (host_readb(signpost_ptr) == gs_travel_destination_town_id)
			{
				tmp = 0;

				do {
					tmp2 = host_readb((Bit8u*)host_readd(signpost_ptr + 2) + tmp) - 1;

					if ((g_land_routes[tmp2].town1_id == gs_current_town) || (g_land_routes[tmp2].town2_id == gs_current_town))
					{
						signpost_id = host_readb(signpost_ptr + 1);
						break;
					}

					tmp++;

				} while (host_readb((Bit8u*)host_readd(signpost_ptr + 2) + tmp) != 255);
			}

			signpost_ptr += 6;

		} while (!signpost_id && host_readb(signpost_ptr) != 255);

		if (signpost_id)
		{
			/* set the target town as current town */
			tmp2 = gs_current_town;
			gs_current_town = (signed char)gs_travel_destination_town_id;

			/* load the map */
			call_load_area(1);

			locations_list_ptr = p_datseg + LOCATIONS_LIST;
			while (host_readb(locations_list_ptr + LOCATION_LOCTYPE) != LOCTYPE_SIGNPOST || host_readb(locations_list_ptr + LOCATION_TYPEINDEX) != signpost_id)
			{
				locations_list_ptr += SIZEOF_LOCATION;
			}

			tmp = host_readws(locations_list_ptr + LOCATION_LOCDATA);
			gs_travel_destination_x = (tmp >> 8) & 0xff;
			gs_travel_destination_y = tmp & 0x0f;
			gs_travel_destination_viewdir = TM_enter_target_town_viewdir(host_readws(locations_list_ptr));

			gs_current_town = (signed char)tmp2;

			/* load the map */
			call_load_area(1);
		}
	}

	return 0;
}

signed short TM_enter_target_town_viewdir(signed short coordinates)
{
	signed short x;
	signed short y;
	signed short retval;

	x = (coordinates >> 8) & 0xff;
	y = coordinates & 0xf;

	retval = (gs_travel_destination_x < x ? EAST :
			(gs_travel_destination_x > x ? WEST :
			(gs_travel_destination_y < y ? SOUTH : NORTH)));

	return retval;
}

void TM_draw_track(signed short a1, signed short length, signed short direction, signed short restore)
{
	signed short i;
	Bit8u *ptr;
	Bit8u* fb_start;

	fb_start = g_vga_memstart;
	ptr = (Bit8u*)((g_buffer9_ptr + host_readws((Bit8u*)g_buffer9_ptr + 4 * (a1 - 1))) + 0xecL);
	ptr += 4;

	if (direction)
	{
		/* move ptr to the last valid value */
		while (host_readws(ptr) != -1)
		{
			ptr += 4;
		}

		ptr -= 4;
	}

	for (i = 0; i < length; i++)
	{
		if (restore == 0)
		{
			/* save the old pixel from the map */
			g_trv_detour_pixel_bak[i] =
				*(fb_start + host_readws(ptr + 2) * 320 + host_readws(ptr));

			/* write a new one */
			*(fb_start + host_readws(ptr + 2) * 320 + host_readws(ptr)) = 0x1c;

			/* move the pointer */
			ptr += 2 * (!direction ? 2 : -2);
		} else {
			/* move the pointer */
			ptr += 2 * (!direction ? 2 : -2);

			/* restore the pixel from the map */
			*(fb_start + host_readws(ptr + 2) * 320 + host_readws(ptr)) = g_trv_detour_pixel_bak[i];
		}
	}
}

#if defined(__BORLANDC__)
void TM_unused2(void)
{

}
#endif

void TM_func8(signed short a1)
{
	if (!(g_route59_flag & 1))
	{
		if (gs_current_town == TOWNS_PEILINEN)
		{
			TM_draw_track(11, 9, 0, a1);
		} else {
			TM_draw_track(11, 17, 1, a1);
		}
	} else {
		if (gs_current_town == TOWNS_KRAVIK)
		{
			TM_draw_track(14, 8, 0, a1);
		} else {
			TM_draw_track(14, 17, 1, a1);
		}
	}
}

void TM_func9(void)
{
	TM_func1(59, g_route59_flag & 1);

	TRV_event(145);
}

#if !defined(__BORLANDC__)
}
#endif
