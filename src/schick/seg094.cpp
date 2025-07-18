/**
 *	Rewrite of DSA1 v3.02_de functions of seg094 (travelmode)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg094.cpp
 */

#include <string.h>

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

	read_archive_file(fd, (Bit8u*)ds_readd(BUFFER6_PTR), 2000);

	close(fd);


	for (i = 0; i < 10; i++) {
		nvf.dst = (Bit8u*)ds_readd(BUFFER6_PTR) + 100 * i + 1000;
		nvf.src = (Bit8u*)ds_readd(BUFFER6_PTR);
		nvf.no = i;
		nvf.type = 0;
		nvf.width = (Bit8u*)&dummy;
		nvf.height = (Bit8u*)&dummy;

		process_nvf(&nvf);
	}

	set_textbox_positions(ds_readbs(CURRENT_TOWN));
}

void set_textbox_positions(signed short town_id)
{
	signed short x;
	signed short r_dx;
	signed short y;

	/* zero some global variables */
	ds_writew(CURRENT_TOWN_OVER, ds_writew(TRV_MENU_SELECTION, ds_writew(SELECTED_TOWN_ANIX, ds_writew(SELECTED_TOWN_ANIY, 0))));


	x = ds_writews(CURRENT_TOWN_ANIX, ds_readws((TOWN_POSITIONS-4) + 4 * town_id));
	y = ds_writews(CURRENT_TOWN_ANIY, ds_readws((TOWN_POSITIONS-4) + 4 * town_id + 2));

	r_dx = (x >= 0 && x <= 159) ?
			(y >= 0 && y <= 99 ? 3 : 1) :
			(y >= 0 && y <= 99 ? 2 : 0);

	ds_writew(BASEPOS_X, !r_dx || r_dx == 2 ? -80 : 80);
	ds_writew(BASEPOS_Y, !r_dx || r_dx == 1 ? -40 : 40);
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
	Bit8u *tevent_ptr;
	signed short bak1;
	signed short bak2;
	signed short last_tevent_no;
	signed short answer;

	ds_writeb(TRAVELING, 1);
	last_tevent_no = -1;
	ds_writed(ROUTE_COURSE_PTR, (Bit32u)(F_PADD(F_PADD((Bit8u*)ds_readd(BUFFER9_PTR), host_readws((Bit8u*)ds_readd(BUFFER9_PTR) + 4 * (route_no - 1))), 0xec)));
	fb_start = (Bit8u*)ds_readd(FRAMEBUF_PTR);
#if defined(__BORLANDC__)
	add_ds_fp(ROUTE_COURSE_PTR, 4);
#endif

	memset((void*)(Bit8u*)ds_readd(TRV_TRACK_PIXEL_BAK), 0xaa, 500);
	ds_writed(TRAVEL_ROUTE_PTR, (Bit32u)RealMake(datseg, (LAND_ROUTES - SIZEOF_LAND_ROUTE) + SIZEOF_LAND_ROUTE * route_no));
	ds_writew(TRAVEL_SPEED, 166);
	ds_writew(ROUTE_TOTAL_STEPS, TM_get_track_length((Bit8u*)ds_readd(ROUTE_COURSE_PTR)));
	ds_writew(ROUTE_LENGTH, host_readb((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_DISTANCE) * 100);
	ds_writew(ROUTE_DURATION, ds_readws(ROUTE_LENGTH) / (
        ds_readws(TRAVEL_SPEED) + host_readbs((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_SPEED_MOD) * ds_readws(TRAVEL_SPEED) / 10
    ) * 60);
	ds_writew(ROUTE_TIMEDELTA, ds_readws(ROUTE_DURATION) / ds_readws(ROUTE_TOTAL_STEPS));
	ds_writew(ROUTE_STEPSIZE, ds_readws(ROUTE_LENGTH) / ds_readws(ROUTE_TOTAL_STEPS));

	if (ds_readws(ROUTE_STEPSIZE) == 0)
	{
		ds_writew(ROUTE_STEPSIZE, 1);
	}

#if defined(__BORLANDC__)
	if (backwards)
	{
		while (host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR)) != -1)
		{
			add_ds_fp(ROUTE_COURSE_PTR, 4);
		}
		sub_ds_fp(ROUTE_COURSE_PTR, 4);
	}
#endif

	memset((void*)(p_datseg + ROUTE_TEVENTS), ds_writews(ROUTE_STEPCOUNT, 0), 60);
	memset((void*)(p_datseg + ROUTE_TEVENT_FLAGS), 0, 15);

	ds_writed(TEVENTS_TAB_PTR, (Bit32u)RealMake(datseg, TEVENTS_TAB));
#if defined(__BORLANDC__)
	/* Forward pointer to entries associated with current route. */
	while (host_readb((Bit8u*)ds_readd(TEVENTS_TAB_PTR)) != route_no && host_readbs((Bit8u*)ds_readd(TEVENTS_TAB_PTR)) != -1)
	{
		add_ds_fp(TEVENTS_TAB_PTR, 3);
	}
#endif

	ds_writew(TRV_RETURN, 0);
	ds_writed(ROUTE_COURSE_START, ds_readd(ROUTE_COURSE_PTR));
	ds_writew(ROUTE_DAYPROGRESS, (
	    ds_readws(TRAVEL_SPEED) + host_readbs((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_SPEED_MOD) * ds_readws(TRAVEL_SPEED) / 10
    ) * 18);

	/* random section starts */
	if (ds_readws(QUESTED_MONTHS) > 3)
	{
		if ((ds_writew(ROUTE_INFORMER_FLAG, (random_schick(100) <= 2 ? 1 : 0))) != 0)
		{
			ds_writew(ROUTE_INFORMER_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
		}
	} else {
		ds_writew(ROUTE_INFORMER_FLAG, 0);
	}

	if ((ds_writew(ROUTE_ENCOUNTER_FLAG, (random_schick(100) <= host_readb((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_ENCOUNTERS) ? 1 : 0))) != 0)
	{
		ds_writew(ROUTE_ENCOUNTER_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
	}

	if ((ds_writew(ROUTE_FIGHT_FLAG, (random_schick(100) <= host_readb((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_FIGHTS) / 3 ? 1 : 0))) != 0)
	{
		ds_writew(ROUTE_FIGHT_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
	}

	ds_writew(ROUTE_DAYPROGRESS, 0);
	/* random section ends */

	while (host_readbs((Bit8u*)ds_readd(TEVENTS_TAB_PTR)) != -1 && host_readb((Bit8u*)ds_readd(TEVENTS_TAB_PTR)) == route_no)
	{
		tevent_ptr = p_datseg + ROUTE_TEVENTS + 4 * ds_readws(ROUTE_STEPCOUNT);
		host_writew(tevent_ptr, host_readb((Bit8u*)ds_readd(TEVENTS_TAB_PTR) + 1));
		host_writew(tevent_ptr + 2, host_readb((Bit8u*)ds_readd(TEVENTS_TAB_PTR) + 2));

		if (backwards)
		{
			host_writew(tevent_ptr, host_readb((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_DISTANCE) - host_readws(tevent_ptr));
		}

		mul_ptr_ws(tevent_ptr, 100);
#if defined(__BORLANDC__)
		add_ds_fp(TEVENTS_TAB_PTR, 3);
#endif
		inc_ds_ws(ROUTE_STEPCOUNT);
	}

	ds_writew(ROUTE_STEPCOUNT, ds_writew(ROUTE_PROGRESS, ds_writeb(TRAVEL_DETOUR, 0)));

	while (host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR) + 2 * ds_writew(ROUTE_MOUSEHOVER, 0)) != -1 &&
		!ds_readb(TRAVEL_DETOUR) &&
		ds_readw(GAME_STATE) == GAME_STATE_MAIN)
	{
		if (is_mouse_in_rect(host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR)) - 16,
					host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR) + 2) - 16,
					host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR)) + 16,
					host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR) + 2) + 16))
		{
			update_mouse_cursor();
			ds_writew(ROUTE_MOUSEHOVER, 1);
		}

		if (ds_readws(TRV_RETURN) == 2)
		{
			dec_ds_ws(ROUTE_STEPCOUNT);

			/* restore the pixel from the map */
			*(fb_start + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR) + 2) * 320
				+ host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR))) =
				host_readb((Bit8u*)ds_readd(TRV_TRACK_PIXEL_BAK) + ds_readws(ROUTE_STEPCOUNT));

			*((Bit8u*)ds_readd(TRV_TRACK_PIXEL_BAK) + ds_readws(ROUTE_STEPCOUNT)) = 0xaa;
		} else {
			/* save the old pixel from the map */
			*((Bit8u*)ds_readd(TRV_TRACK_PIXEL_BAK) + ds_readws(ROUTE_STEPCOUNT)) =
				*(fb_start + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR) + 2) * 320 + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR)));

			inc_ds_ws(ROUTE_STEPCOUNT);

			/* write a new pixel */
			*(fb_start + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR) + 2) * 320 + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR))) = 0x1c;
		}

		if (ds_readw(ROUTE_MOUSEHOVER) != 0)
		{
			refresh_screen_size();
		}

		ds_writew(TRV_I, 0);
		while (ds_readws(ROUTE_TIMEDELTA) / 2 > ds_readws(TRV_I))
		{
			handle_input();

			if (ds_readws(TRV_I) % 2)
			{
				wait_for_vsync();
			}

			timewarp(MINUTES(2));

			inc_ds_ws(TRV_I);
		}

		add_ds_ws(ROUTE_PROGRESS, (ds_readws(TRV_RETURN) == 2 ? -ds_readws(ROUTE_STEPSIZE) : ds_readws(ROUTE_STEPSIZE)));
		add_ds_ws(ROUTE_DAYPROGRESS, ds_readws(ROUTE_STEPSIZE));

		if (ds_readws(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP)
		{
			if (!ds_readb(FORCEDMARCH_TIMER))
			{
				answer = GUI_radio(get_ttx(815), 3,
							get_tx(74),
							get_ttx(816),
							get_ttx(613));
			} else {
				answer = GUI_radio(get_ttx(815), 2,
							get_ttx(816),
							get_ttx(613));
			}

			if (answer == 1 && !ds_readb(FORCEDMARCH_TIMER)) /* Gewaltmarsch */
			{
			    /* do forced march for 2 days */
				ds_writew(FORCEDMARCH_LE_COST, random_schick(10));
				ds_writew(TRAVEL_SPEED, ds_readws(ROUTE_STEPCOUNT) + 197);
				ds_writeb(FORCEDMARCH_TIMER, 2);
				ds_writew(ROUTE_DURATION, ds_readws(ROUTE_LENGTH) / (
				    ds_readws(TRAVEL_SPEED) + (host_readbs((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_SPEED_MOD) * ds_readws(TRAVEL_SPEED)) / 10
                ) * 60);
				ds_writew(ROUTE_TIMEDELTA, ds_readws(ROUTE_DURATION) / ds_readws(ROUTE_TOTAL_STEPS));
				shr_ds_ws(FORCEDMARCH_LE_COST, 1);

				hero = get_hero(0);
				for (ds_writew(TRV_I, 0); ds_readws(TRV_I) <= 6; inc_ds_ws(TRV_I), hero += SIZEOF_HERO)
				{
					if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
						!hero_dead(hero))
					{
						sub_hero_le(hero, ds_readws(FORCEDMARCH_LE_COST));
					}
				}

			} else if ((answer == 1 && ds_readb(FORCEDMARCH_TIMER) != 0) ||
					(answer == 2 && !ds_readb(FORCEDMARCH_TIMER))) /* Kampieren */
				// Depending on FORCEDMARCH_TIMER, the "Kampieren" answer is on position 1 or 2.
			{
				ds_writew(WILDCAMP_SLEEP_QUALITY, -3);
				ds_writeb(GOOD_CAMP_PLACE, 99); /* make gather_herbs and replenish_stocks practically impossible */
				ds_writeb(CURRENT_LOCTYPE, LOCTYPE_WILDCAMP);
				do_location();
				ds_writeb(GOOD_CAMP_PLACE, ds_writeb(CURRENT_LOCTYPE, (unsigned char)ds_writew(WILDCAMP_SLEEP_QUALITY, 0)));
				ds_writew(WALLCLOCK_UPDATE, 0);
				ds_writew(REQUEST_REFRESH, 2);
			}
		}

		if (ds_readw(ROUTE_ENCOUNTER_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(ROUTE_ENCOUNTER_TIME) && ds_readws(GAME_STATE) == GAME_STATE_MAIN)
		{
			random_encounter(route_no);
			ds_writew(ROUTE_ENCOUNTER_FLAG, 0);

		} else if (ds_readw(ROUTE_FIGHT_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(ROUTE_FIGHT_TIME) && ds_readws(GAME_STATE) == GAME_STATE_MAIN)
		{
			do_wild8_fight();

		} else if (ds_readw(ROUTE_INFORMER_FLAG) != 0 && ds_readws(ROUTE_DAYPROGRESS) >= ds_readws(ROUTE_INFORMER_TIME) && ds_readws(GAME_STATE) == GAME_STATE_MAIN)
		{
			ds_writew(CURRENT_TYPEINDEX, random_schick(100) <= 50 ? 10 : 12);
			bak1 = ds_readws(BASEPOS_X);
			bak2 = ds_readws(BASEPOS_Y);
			ds_writew(BASEPOS_X, ds_writew(BASEPOS_Y, 0));
			do_informer();
			ds_writew(BASEPOS_X, bak1);
			ds_writew(BASEPOS_Y, bak2);

		}

		if (ds_readws(GAME_STATE) == GAME_STATE_MAIN)
		{
			for (ds_writew(TRV_I, 0); ds_readws(TRV_I) < 15; inc_ds_ws(TRV_I))
			{
				if ((ds_readws(ROUTE_TEVENTS + 4 * ds_readws(TRV_I)) <= ds_readws(ROUTE_PROGRESS) &&
					ds_readws(TRV_RETURN) == 0 &&
					!ds_readb(ROUTE_TEVENT_FLAGS + ds_readws(TRV_I))) ||
					(ds_readws(ROUTE_TEVENTS + 4 * ds_readws(TRV_I)) >= ds_readws(ROUTE_PROGRESS) &&
					ds_readws(TRV_RETURN) == 2 &&
					ds_readbs(ROUTE_TEVENT_FLAGS + ds_readws(TRV_I)) == 2))
				{
					if (ds_readws((ROUTE_TEVENTS + 2) + 4 * ds_readws(TRV_I)) != 0)
					{
						TRV_event(ds_readws((ROUTE_TEVENTS + 2) + 4 * (last_tevent_no = ds_readws(TRV_I))));

						if (!ds_readbs((TEVENTS_REPEATABLE-1) + ds_readws((ROUTE_TEVENTS + 2) + 4 * ds_readws(TRV_I))))
						{
							ds_writeb(ROUTE_TEVENT_FLAGS + ds_readws(TRV_I), 1);
						} else if (ds_readws(TRV_RETURN) == 0)
						{
							ds_writeb(ROUTE_TEVENT_FLAGS + ds_readws(TRV_I), 2);
						} else {
							ds_writeb(ROUTE_TEVENT_FLAGS + ds_readws(TRV_I), 0);
						}

						if (ds_readws(REQUEST_REFRESH) != 0 && !ds_readb(TRAVEL_DETOUR))
						{
							ds_writew(WALLCLOCK_UPDATE, 0);
						}
					}
				}
			}
		}

        /* night camp */
		if (ds_readds(DAY_TIMER) >= HOURS(20) &&
			!ds_readb(TRAVEL_DETOUR) &&
			ds_readws(GAME_STATE) == GAME_STATE_MAIN &&
			2 * ds_readws(ROUTE_STEPSIZE) < ds_readws(ROUTE_PROGRESS) &&
			ds_readws(ROUTE_LENGTH) - 2 * ds_readws(ROUTE_STEPSIZE) > ds_readws(ROUTE_PROGRESS))
		{
			ds_writew(WALLCLOCK_UPDATE, 0);

			if (ds_readws(REQUEST_REFRESH) != 0)
			{
				load_map();
			}

			GUI_input(get_tx(70), 0);

			ds_writeb(CURRENT_LOCTYPE, LOCTYPE_WILDCAMP);
			do_location();
			ds_writeb(CURRENT_LOCTYPE, LOCTYPE_NONE);

			ds_writew(REQUEST_REFRESH, 2);
			ds_writew(WALLCLOCK_UPDATE, 0);

			if (ds_readws(GAME_STATE) == GAME_STATE_MAIN)
			{
			    /* figure out encounters etc. for next day */
				ds_writew(ROUTE_DAYPROGRESS, (
				    ds_readws(TRAVEL_SPEED) + (host_readbs((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_SPEED_MOD) * ds_readws(TRAVEL_SPEED) / 10)
                ) * 18);

				if ((ds_writew(ROUTE_ENCOUNTER_FLAG, random_schick(100) <= host_readb((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_ENCOUNTERS) ? 1 : 0)) != 0)
				{
					ds_writew(ROUTE_ENCOUNTER_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
				}

				if ((ds_writew(ROUTE_FIGHT_FLAG, random_schick(100) <= host_readb((Bit8u*)ds_readd(TRAVEL_ROUTE_PTR) + LAND_ROUTE_FIGHTS) / 3 ? 1 : 0)) != 0)
				{
					ds_writew(ROUTE_FIGHT_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
				}

				if (ds_readws(QUESTED_MONTHS) > 3)
				{
					if ((ds_writew(ROUTE_INFORMER_FLAG, random_schick(100) <= 2 ? 1 : 0)) != 0)
					{
						ds_writew(ROUTE_INFORMER_TIME, random_schick(ds_readws(ROUTE_DAYPROGRESS)));
					}
				} else {
					ds_writew(ROUTE_INFORMER_FLAG, 0);
				}

				ds_writew(ROUTE_DAYPROGRESS, 0);
			}
		}

		if (ds_readw(CHECK_DISEASE) != 0 && !ds_readb(TRAVEL_DETOUR) && ds_readw(GAME_STATE) == GAME_STATE_MAIN)
		{
			disease_effect();
		}

		if (ds_readw(REQUEST_REFRESH) != 0 && !ds_readb(TRAVEL_DETOUR) && ds_readw(GAME_STATE) == GAME_STATE_MAIN)
		{
			update_mouse_cursor();
			load_map();
			/* TODO: update window */
			memmove((void*)((Bit8u*)ds_readd(FRAMEBUF_PTR)), (void*)((Bit8u*)ds_readd(TRAVEL_MAP_PTR)), 320 * 200);

			wait_for_vsync();
			set_palette((Bit8u*)ds_readd(TRAVEL_MAP_PTR) + 64000 + 2, 0, 0x20);

			ds_writeb(PP20_INDEX, 5);
			ds_writew(TRV_I, 0);
			ds_writed(ROUTE_COURSE_PTR2, ds_readd(ROUTE_COURSE_START));

			if (route_no == 59)
			{
				TM_func8(0);
			}

#if defined(__BORLANDC__)
			/* Redraw the track on the map */
			while (host_readb((Bit8u*)ds_readd(TRV_TRACK_PIXEL_BAK) + inc_ds_ws_post(TRV_I)) != 0xaa)
			{
				*(fb_start + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR2) + 2) * 320 + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR2))) =  0x1c;
				add_ds_fp(ROUTE_COURSE_PTR2, 2 * (!backwards ? 2 : -2));
			}
#endif

			refresh_screen_size();

			if (ds_readws(REQUEST_REFRESH) == 2 && route_no != 59)
			{
			    /* Return or continue? */
				if (GUI_radio(get_tx(71), 2, get_tx(72), get_tx(73)) == 2)
				{
					ds_writew(TRV_RETURN, ds_readws(TRV_RETURN) == 0 ? 1 : -1);

					if (last_tevent_no != -1)
					{
						ds_writeb(ROUTE_TEVENT_FLAGS + last_tevent_no, ds_readws(TRV_RETURN) == 1 ? 0 : 2);
					}
				}
			}
			ds_writew(WALLCLOCK_X, ds_readws(BASEPOS_X) + 120);
			ds_writew(WALLCLOCK_Y, ds_readws(BASEPOS_Y) + 87);
			ds_writew(WALLCLOCK_UPDATE, 1);
			ds_writew(REQUEST_REFRESH, 0);
		}

#if defined(__BORLANDC__)
		if (ds_readws(TRV_RETURN) == 1 || ds_readws(TRV_RETURN) == -1)
		{
			ds_writew(TRV_RETURN, ds_readws(TRV_RETURN) == 1 ? 2: 0);

			add_ds_fp(ROUTE_COURSE_PTR, 2 * ((!backwards && ds_readws(TRV_RETURN) == 0) || (backwards && ds_readws(TRV_RETURN) != 0) ? -2 : 2));
		}

		add_ds_fp(ROUTE_COURSE_PTR, 2 * ((!backwards && ds_readws(TRV_RETURN) == 0) || (backwards && ds_readws(TRV_RETURN) != 0) ? 2 : -2));
#endif
	}

	if (ds_readw(GAME_STATE) == GAME_STATE_MAIN && !ds_readb(TRAVEL_DETOUR) && ds_readw(TRV_RETURN) != 2)
	{
		update_mouse_cursor();
#if defined(__BORLANDC__)
		do {
			add_ds_fp(ROUTE_COURSE_PTR, 2 * (!backwards ? -2 : 2));
			dec_ds_ws(ROUTE_STEPCOUNT);
			*(fb_start + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR) + 2) * 320 + host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR))) =
				host_readb((Bit8u*)ds_readd(TRV_TRACK_PIXEL_BAK) + ds_readws(ROUTE_STEPCOUNT));

		} while (host_readws((Bit8u*)ds_readd(ROUTE_COURSE_PTR)) != -1);
#endif

		if (route_no == 59)
		{
			TM_func8(1);
		}

		refresh_screen_size();
	}

	ds_writeb(TRAVELING, 0);
}

/* continue travel after arrival while still on map */
signed short TM_unused1(RealPt signpost_ptr, signed short old_route_no)
{
	signed short route_no1;
	signed short route_id;
	signed short answer;
	signed short town_i;
	signed short route_no2;
	signed short town;
	signed short old_route_id;
	Bit8u *destinations_tab[7];

	old_route_id = host_readb((Bit8u*)(host_readd((Bit8u*)(signpost_ptr) + SIGNPOST_LAND_ROUTES)) + old_route_no) - 1;
	ds_writeb(CURRENT_TOWN, (signed char)(town = ds_readws(TRV_DESTINATION)));
	signpost_ptr = ((Bit8u*)p_datseg + SIGNPOSTS);

	do {
		if (host_readb((Bit8u*)(signpost_ptr) + SIGNPOST_TOWN) == town)
		{
			route_no1 = 0;
			while (host_readbs((Bit8u*)(host_readd((Bit8u*)(signpost_ptr) + SIGNPOST_LAND_ROUTES)) + route_no1) != -1)
			{
				if (host_readb((Bit8u*)(host_readd((Bit8u*)(signpost_ptr) + SIGNPOST_LAND_ROUTES)) + route_no1) - 1 == old_route_id &&
					(route_no1 || host_readb((Bit8u*)(host_readd((Bit8u*)(signpost_ptr) + SIGNPOST_LAND_ROUTES)) + (route_no1 + 1)) != 255))
				{
					town_i = route_no2 = 0;
					while ((route_id = host_readb((Bit8u*)(host_readd((Bit8u*)(signpost_ptr) + SIGNPOST_LAND_ROUTES)) + route_no2)) != 255)
					{
						if (route_no2 != route_no1)
						{
							destinations_tab[town_i++] = get_ttx(235 + ds_writebs(TRV_MENU_TOWNS + town_i,
                                ((answer = ds_readb((LAND_ROUTES - SIZEOF_LAND_ROUTE + LAND_ROUTE_TOWN_1) + SIZEOF_LAND_ROUTE * route_id)) != ds_readbs(CURRENT_TOWN) ?
                                    (unsigned char)answer : ds_readb((LAND_ROUTES - SIZEOF_LAND_ROUTE + LAND_ROUTE_TOWN_2) + SIZEOF_LAND_ROUTE * route_id))
                            ));
						}
						route_no2++;
					}

					ds_writeb(TRV_MENU_TOWNS + town_i, (signed char)town);
					destinations_tab[town_i] = get_ttx(547);
					town_i++;
					ds_writed(TM_UNUSED1_PTR, (Bit32u)signpost_ptr);

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

					ds_writew(TRV_DESTINATION, ds_readbs((TRV_MENU_TOWNS - 1) + answer));
					return answer;
				}

				route_no1++;
			}
		}

		signpost_ptr += SIZEOF_SIGNPOST;

	} while (host_readbs((Bit8u*)(signpost_ptr)) != -1);

	return -1;
}

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
	ds_writew(TRAVEL_DESTINATION_TOWN_ID, ds_readw(TRV_DESTINATION));
	signpost_id = 1;

	if (signpost_id)
	{
		signpost_ptr = p_datseg + SIGNPOSTS;
		signpost_id = 0;
		do {
			if (host_readb(signpost_ptr) == ds_readw(TRAVEL_DESTINATION_TOWN_ID))
			{
				tmp = 0;

				do {
					tmp2 = host_readb((Bit8u*)(host_readd(signpost_ptr + 2)) + tmp) - 1;

					if (ds_readbs(LAND_ROUTES + 9 * tmp2) == ds_readbs(CURRENT_TOWN) || ds_readbs((LAND_ROUTES + 1) + 9 * tmp2) == ds_readbs(CURRENT_TOWN))
					{
						signpost_id = host_readb(signpost_ptr + 1);
						break;
					}

					tmp++;

				} while (host_readb((Bit8u*)(host_readd(signpost_ptr + 2)) + tmp) != 255);
			}

			signpost_ptr += 6;

		} while (!signpost_id && host_readb(signpost_ptr) != 255);

		if (signpost_id)
		{
			/* set the target town as current town */
			tmp2 = ds_readbs(CURRENT_TOWN);
			ds_writeb(CURRENT_TOWN, (signed char)ds_readws(TRAVEL_DESTINATION_TOWN_ID));

			/* load the map */
			call_load_area(1);

			locations_list_ptr = p_datseg + LOCATIONS_LIST;
			while (host_readb(locations_list_ptr + LOCATION_LOCTYPE) != LOCTYPE_SIGNPOST || host_readb(locations_list_ptr + LOCATION_TYPEINDEX) != signpost_id)
			{
				locations_list_ptr += SIZEOF_LOCATION;
			}

			tmp = host_readws(locations_list_ptr + LOCATION_LOCDATA);
			ds_writew(TRAVEL_DESTINATION_X, (tmp >> 8) & 0xff);
			ds_writew(TRAVEL_DESTINATION_Y, tmp & 0xf);
			ds_writew(TRAVEL_DESTINATION_VIEWDIR, TM_enter_target_town_viewdir(host_readws(locations_list_ptr)));

			ds_writeb(CURRENT_TOWN, (signed char)tmp2);

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

	retval = (ds_readws(TRAVEL_DESTINATION_X) < x ? EAST :
			(ds_readws(TRAVEL_DESTINATION_X) > x ? WEST :
			(ds_readws(TRAVEL_DESTINATION_Y) < y ? SOUTH : NORTH)));

	return retval;
}

void TM_draw_track(signed short a1, signed short length, signed short direction, signed short restore)
{
	signed short i;
	Bit8u *ptr;
	RealPt fb_start;

	fb_start = (Bit8u*)ds_readd(FRAMEBUF_PTR);
	ptr = (Bit8u*)(F_PADD(F_PADD((Bit8u*)ds_readd(BUFFER9_PTR), host_readws((Bit8u*)ds_readd(BUFFER9_PTR) + 4 * (a1 - 1))), 0xec));
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
			ds_writeb(TRV_DETOUR_PIXEL_BAK + i,
				*(fb_start + host_readws(ptr + 2) * 320 + host_readws(ptr)));

			/* write a new one */
			*(fb_start + host_readws(ptr + 2) * 320 + host_readws(ptr)) = 0x1c;

			/* move the pointer */
			ptr += 2 * ((!direction ? 2 : -2));
		} else {
			/* move the pointer */
			ptr += 2 * ((!direction ? 2 : -2));

			/* restore the pixel from the map */
			*(fb_start + host_readws(ptr + 2) * 320 + host_readws(ptr)) = ds_readb(TRV_DETOUR_PIXEL_BAK + i);
		}
	}
}

void TM_unused2(void)
{

}

void TM_func8(signed short a1)
{
	if (!(ds_readb(ROUTE59_FLAG) & 1))
	{
		if (ds_readbs(CURRENT_TOWN) == TOWNS_PEILINEN)
		{
			TM_draw_track(11, 9, 0, a1);
		} else {
			TM_draw_track(11, 17, 1, a1);
		}
	} else {
		if (ds_readbs(CURRENT_TOWN) == TOWNS_KRAVIK)
		{
			TM_draw_track(14, 8, 0, a1);
		} else {
			TM_draw_track(14, 17, 1, a1);
		}
	}
}

void TM_func9(void)
{
	TM_func1(59, ds_readb(ROUTE59_FLAG) & 1);

	TRV_event(145);
}

#if !defined(__BORLANDC__)
}
#endif
