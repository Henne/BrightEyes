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

signed char g_traveling = 0; // ds:0xa842
static const signed char g_tevents_repeatable[145] = { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }; // ds:0xa843


unsigned char g_route59_flag;			// ds:0xe4a2, {0, 1 = from Kravik, 2 = from Peilinen, 3 = from Skelellen, 4 = from Rovamund}
signed int g_trv_menu_selection;		// ds:0xe4a3
signed int g_current_town_over;			// ds:0xe4a5
signed int g_current_town_overy;		// ds:0xe4a7
signed int g_current_town_overx;		// ds:0xe4a9
signed int g_selected_town_aniy;		// ds:0xe4ab
signed int g_selected_town_anix;		// ds:0xe4ad
signed int g_current_town_aniy;			// ds:0xe4af
signed int g_current_town_anix;			// ds:0xe4b1
unsigned char g_unkn_091[1];			// ds:0xe4b3
static unsigned char g_trv_detour_pixel_bak[20]; // ds:0xe4b4
unsigned char g_good_camp_place;		// ds:0xe4c8
static unsigned char g_route_tevent_flags[15];	// ds:0xe4c9

void prepare_map_marker(void)
{
	signed int i;
	signed int handle;
	signed int dummy;
	struct nvf_extract_desc nvf;

	/* load OBJECTS.NVF */
	handle = load_archive_file(ARCHIVE_FILE_OBJECTS_NVF);

	read_archive_file(handle, g_buffer6_ptr, 2000);

	close(handle);

	for (i = 0; i < 10; i++) {
		nvf.dst = g_buffer6_ptr + 100 * i + 1000;
		nvf.src = g_buffer6_ptr;
		nvf.image_num = i;
		nvf.compression_type = 0;
		nvf.width = &dummy;
		nvf.height = &dummy;

		process_nvf_extraction(&nvf);
	}

	set_textbox_positions(gs_town_id);
}

void set_textbox_positions(const signed int town_id)
{
	signed int x;
	signed int r_dx;
	signed int y;

	/* zero some global variables */
	g_current_town_over = g_trv_menu_selection = g_selected_town_anix = g_selected_town_aniy = 0;


	x = g_current_town_anix = g_town_positions[town_id - 1].x;
	y = g_current_town_aniy = *(signed int*)((uint8_t*)g_town_positions + 4 * (town_id - 1) + 2);

	r_dx = (x >= 0 && x <= 159) ? (y >= 0 && y <= 99 ? 3 : 1) : (y >= 0 && y <= 99 ? 2 : 0);

	g_basepos_x = ((!r_dx || r_dx == 2) ? -80 : 80);
	g_basepos_y = ((!r_dx || r_dx == 1) ? -40 : 40);
}

/**
 * \brief   ???
 *
 * \param   route_id    number of the route
 * \param   backwards   0 = travel the route forwards, 1 = travel backwards
 */
void TM_func1(const signed int route_id, const signed int backwards)
{
	uint8_t* fb_start;
	struct struct_hero *hero;
	struct struct_route_tevent *tevent_ptr;
	signed int bak1;
	signed int bak2;
	signed int last_tevent_no;
	signed int answer;

	g_traveling = 1;

	last_tevent_no = -1;
	gs_route_course_ptr = (int16_t*)((g_buffer9_ptr + *(int16_t*)((uint8_t*)g_buffer9_ptr + 4 * (route_id - 1))) + 0xecL);
	fb_start = g_vga_memstart;
	gs_route_course_ptr += 2;

	memset((void*)g_trv_track_pixel_bak, 0xaa, 500);
	/* TODO: move this pointer out of the game state, verify if that works correctly.
	 * 		Can be replaced by a locvar! */
	gs_travel_route_ptr = &g_land_routes[route_id - 1];
	gs_travel_speed = 166;
	gs_route_total_steps = TM_get_track_length((struct struct_point*)gs_route_course_ptr);
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
		while (gs_route_course_ptr[0] != -1)
		{
			gs_route_course_ptr += 2;
		}
		gs_route_course_ptr -= 2;
	}

	memset((void*)gs_route_tevents, (gs_route_stepcount = 0), 15 * sizeof(struct_route_tevent));
	memset(g_route_tevent_flags, 0, 15);

	/* TODO: move this pointer out of the game state, verify if that works correctly.
	 * 		Can be replaced by a locvar! */
	gs_tevents_tab_ptr = &g_tevents_tab[0];
	/* Forward pointer to entries associated with current route. */
	while (((unsigned char)gs_tevents_tab_ptr->route_id != route_id) && (gs_tevents_tab_ptr->route_id != -1))
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

	while ((gs_tevents_tab_ptr->route_id != -1) && ((unsigned char)gs_tevents_tab_ptr->route_id == route_id))
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

	while (gs_route_course_ptr[(gs_route_mousehover = 0)] != -1 &&
		!gs_travel_detour &&
		g_game_state == GAME_STATE_MAIN)
	{
		if (is_mouse_in_rect(gs_route_course_ptr[0] - 16, gs_route_course_ptr[1] - 16,
					gs_route_course_ptr[0] + 16, gs_route_course_ptr[1] + 16))
		{
			call_mouse_bg();
			gs_route_mousehover = 1;
		}

		if (gs_trv_return == 2)
		{
			gs_route_stepcount--;

			/* restore the pixel from the map */
			*(fb_start + gs_route_course_ptr[1] * 320 + gs_route_course_ptr[0]) =
				g_trv_track_pixel_bak[gs_route_stepcount];

			g_trv_track_pixel_bak[gs_route_stepcount] = 0xaa;
		} else {
			/* save the old pixel from the map */
			g_trv_track_pixel_bak[gs_route_stepcount] =
				*(fb_start + gs_route_course_ptr[1] * 320 + gs_route_course_ptr[0]);

			gs_route_stepcount++;

			/* write a new pixel */
			*(fb_start + gs_route_course_ptr[1] * 320 + gs_route_course_ptr[0]) = 0x1c;
		}

		if (gs_route_mousehover) {
			call_mouse();
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

		if (g_mouse_rightclick_event || g_action == ACTION_ID_PAGE_UP)
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
				for (gs_trv_i = 0; gs_trv_i <= 6; gs_trv_i++, hero++)
				{
					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
						!hero->flags.dead)
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
			random_encounter(route_id);
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

						if (!g_tevents_repeatable[gs_route_tevents[gs_trv_i].tevent_id - 1])
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
			call_mouse_bg();
			load_map();
			/* TODO: update window */
			memmove((void*)g_vga_memstart, (void*)gs_travel_map_ptr, 320 * 200);

			wait_for_vsync();
			set_palette(gs_travel_map_ptr + 64000 + 2, 0, 0x20);

			g_pp20_index = 5;
			gs_trv_i = 0;
			gs_route_course_ptr2 = gs_route_course_start;

			if (route_id == 59)
			{
				TM_func8(0);
			}

			/* Redraw the track on the map */
			while (g_trv_track_pixel_bak[gs_trv_i++] != 0xaa)
			{
				*(fb_start + gs_route_course_ptr2[1] * 320 + gs_route_course_ptr2[0]) =  0x1c;
				gs_route_course_ptr2 += (!backwards ? 2 : -2);
			}

			call_mouse();

			if (g_request_refresh == 2 && route_id != 59)
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

			gs_route_course_ptr += ((!backwards && gs_trv_return == 0) || (backwards && gs_trv_return != 0) ? -2 : 2);
		}

		gs_route_course_ptr += ((!backwards && gs_trv_return == 0) || (backwards && gs_trv_return != 0) ? 2 : -2);
	}

	if (g_game_state == GAME_STATE_MAIN && !gs_travel_detour && gs_trv_return != 2)
	{
		call_mouse_bg();

		do {
			gs_route_course_ptr += (!backwards ? -2 : 2);
			gs_route_stepcount--;
			*(fb_start + gs_route_course_ptr[1] * 320 + gs_route_course_ptr[0]) =
				g_trv_track_pixel_bak[gs_route_stepcount];

		} while (gs_route_course_ptr[0] != -1);

		if (route_id == 59)
		{
			TM_func8(1);
		}

		call_mouse();
	}

	g_traveling = 0;
}

#if defined(__BORLANDC__)
/* continue travel after arrival while still on map */
signed int TM_unused1(struct trv_start_point *signpost_ptr, const signed int old_route_id)
{
	signed int route_id1;
	signed int route_id;
	signed int answer;
	signed int town_i;
	signed int route_id2;
	signed int town_id;
	signed int old_route_id2;
	char *destinations_tab[7];

	old_route_id2 = signpost_ptr->linked_travel_routes[old_route_id] - 1;
	gs_town_id = town_id = gs_trv_destination;
	signpost_ptr = &g_signposts[0];

	do {
		if ((unsigned char)signpost_ptr->town_id == town_id)
		{
			route_id1 = 0;
			while (signpost_ptr->linked_travel_routes[route_id1] != (signed char)-1)
			{
				if (signpost_ptr->linked_travel_routes[route_id1] - 1 == old_route_id2 &&
					(route_id1 || signpost_ptr->linked_travel_routes[route_id1 + 1] != (signed char)-1))
				{
					town_i = route_id2 = 0;

					while ((route_id = signpost_ptr->linked_travel_routes[route_id2]) != 0xff)
					{
						if (route_id2 != route_id1)
						{
							destinations_tab[town_i++] = get_ttx(235 + (gs_trv_menu_towns[town_i] =
								((answer = g_land_routes[route_id - 1].town_id_1) != gs_town_id ?
									(signed char)answer : g_land_routes[route_id - 1].town_id_2)));
						}
						route_id2++;
					}

					gs_trv_menu_towns[town_i] = town_id;
					destinations_tab[town_i] = get_ttx(547);
					town_i++;

					gs_tm_unused1_ptr = signpost_ptr;

					set_textbox_positions(town_id);

					answer = GUI_radio(get_ttx(546), (signed char)town_i,
								destinations_tab[0], destinations_tab[1],
								destinations_tab[2], destinations_tab[3],
								destinations_tab[4], destinations_tab[5]);

					if (answer == -1)
					{
						answer = town_i;
					}

					gs_trv_destination = gs_trv_menu_towns[answer - 1];
					return answer;
				}

				route_id1++;
			}
		}

		signpost_ptr++;

	} while (signpost_ptr->town_id != -1);

	return -1;
}
#endif

signed int TM_get_track_length(struct struct_point *track)
{
	signed int length = 0;

	while (track->x != -1)
	{
		track++;
		length++;
	}

	return length;
}

signed int TM_enter_target_town(void)
{
	signed int tmp;
	signed int signpost_id;
	signed int tmp2;
	struct trv_start_point *signpost_ptr;
	struct location *locations_tab_ptr;

	signpost_id = 0;
	gs_travel_destination_town_id = gs_trv_destination;
	signpost_id = 1;

	if (signpost_id)
	{
		signpost_ptr = &g_signposts[0];
		signpost_id = 0;
		do {
			if ((unsigned char)signpost_ptr->town_id == gs_travel_destination_town_id)
			{
				tmp = 0;

				do {
					tmp2 = signpost_ptr->linked_travel_routes[tmp] - 1;

					if ((g_land_routes[tmp2].town_id_1 == gs_town_id) || (g_land_routes[tmp2].town_id_2 == gs_town_id))
					{
						signpost_id = signpost_ptr->typeindex;
						break;
					}

					tmp++;

				} while (signpost_ptr->linked_travel_routes[tmp] != 0xff);
			}

			signpost_ptr++;

		} while (!signpost_id && signpost_ptr->town_id != -1);

		if (signpost_id)
		{
			/* set the target town as current town */
			tmp2 = gs_town_id;
			gs_town_id = gs_travel_destination_town_id;

			/* load the map */
			call_load_area(1);

			locations_tab_ptr = &g_locations_tab[0];
			while (locations_tab_ptr->loctype != LOCTYPE_SIGNPOST || locations_tab_ptr->typeindex != signpost_id)
			{
				locations_tab_ptr++;
			}

			tmp = locations_tab_ptr->locdata;
			gs_travel_destination_x = (tmp >> 8) & 0xff;
			gs_travel_destination_y = tmp & 0x0f;
			gs_travel_destination_viewdir = TM_enter_target_town_viewdir(locations_tab_ptr->pos);

			gs_town_id = tmp2;

			/* load the map */
			call_load_area(1);
		}
	}

	return 0;
}

signed int TM_enter_target_town_viewdir(const signed int coordinates)
{
	const signed int x = (coordinates >> 8) & 0xff;
	const signed int y = coordinates & 0xf;
	const signed int retval = (gs_travel_destination_x < x ? EAST :
				(gs_travel_destination_x > x ? WEST :
				(gs_travel_destination_y < y ? SOUTH : NORTH)));

	return retval;
}

void TM_draw_track(const signed int route_id, const signed int length, const signed int direction, const signed int restore)
{
	signed int i;
	int16_t *ptr;
	uint8_t* fb_start;

	fb_start = g_vga_memstart;
	ptr = (int16_t*)((g_buffer9_ptr + *(int16_t*)((uint8_t*)g_buffer9_ptr + 4 * (route_id - 1))) + 0xecL);
	ptr += 2;

	if (direction)
	{
		/* move ptr to the last valid value */
		while (ptr[0] != -1)
		{
			ptr += 2;
		}

		ptr -= 2;
	}

	for (i = 0; i < length; i++)
	{
		if (restore == 0)
		{
			/* save the old pixel from the map */
			g_trv_detour_pixel_bak[i] = *(fb_start + ptr[1] * 320 + ptr[0]);

			/* write a new one */
			*(fb_start + ptr[1] * 320 + ptr[0]) = 0x1c;

			/* move the pointer */
			ptr += (!direction ? 2 : -2);
		} else {
			/* move the pointer */
			ptr += (!direction ? 2 : -2);

			/* restore the pixel from the map */
			*(fb_start + ptr[1] * 320 + ptr[0]) = g_trv_detour_pixel_bak[i];
		}
	}
}

#if defined(__BORLANDC__)
void TM_unused2(void)
{

}
#endif

void TM_func8(const signed int restore)
{
	if (!(g_route59_flag & 1))
	{
		if (gs_town_id == TOWN_ID_PEILINEN)
		{
			TM_draw_track(11, 9, 0, restore);
		} else {
			TM_draw_track(11, 17, 1, restore);
		}
	} else {
		if (gs_town_id == TOWN_ID_KRAVIK)
		{
			TM_draw_track(14, 8, 0, restore);
		} else {
			TM_draw_track(14, 17, 1, restore);
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
