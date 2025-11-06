/*
 *	Rewrite of DSA1 v3.02_de functions of seg063 (harbor)
 *	Functions rewritten: 5/5 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg063.cpp
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
#include "seg007.h"
#include "seg008.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg029.h"
#include "seg030.h"
#include "seg049.h"
#include "seg059.h"
#include "seg063.h"
#include "seg064.h"
#include "seg075.h"
#include "seg097.h"
#include "seg119.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static signed short g_passage_type_to_name[7] = {
	/* maps entry PASSAGE_TYPE in SHIP_TABLE -> ptr to name of type of passage (Begleitschutzfahrt, Deckpassage etc.) */
	0x001d, /* HEUER */
	0x001e, /* BEGLEITSCHUTZFAHRT */
	0x001f, /* LUXUSPASSAGE */
	0x0020, /* KOMFORTABLE PASSAGE */
	0x0021, /* KABINENPASSAGE */
	0x0022, /* DECKPASSAGE */
	0x0023  /* MITFAHRGELEGENHEIT */
}; // ds:0x6ec2/*
static const struct ship g_ship_table[8] = {
	{ 0, 1,  0, 120 },
	{ 3, 1, 35, 100 },
	{ 1, 1,  0, (signed char)150 }, //BUG
	{ 2, 1, 45, (signed char)150 }, //BUG
	{ 0, 1,  0,  90 },
	{ 4, 1, 20,  80 },
	{ 5, 0, 10,  60 },
	{ 6, 0,  0,  40 }
}; // ds:0x6ed0
static signed short g_sea_travel_tx_ship[8] = { 0x0024, 0x0025, 0x0026, 0x0026, 0x0024, 0x0027, 0x0028, 0x0029 }; // ds:0x6ef0
struct sea_route g_sea_routes[46] = {
	{ TOWNS_THORWAL    , TOWNS_PREM            , 115,  1, 0, 0, 0, 0 }, //  1
	{ TOWNS_PREM       , TOWNS_HJALSINGOR      , 210,  3, 0, 0, 0, 0 }, //  2
#ifndef M302de_ORIGINAL_BUGFIX
	 /* Original-Bug 25: According to the map, the route Prem <-> Manrin should be the longest ship route by far. Distance 54 is way too short. Maybe the original intention was a value of 310, which after a byte overflow results in 54. */
	{ TOWNS_PREM       , TOWNS_MANRIN          ,  54,  7, 0, 0, 0, 0 }, //  3
#else
	{ TOWNS_PREM       , TOWNS_MANRIN          , 255,  7, 0, 0, 0, 0 }, //  3
	/* set the distance to maximum possible value 255. Still too short, but much better than 54 */
#endif
	{ TOWNS_PREM       , TOWNS_KORD            , 135,  7, 0, 0, 0, 0 }, //  4
	{ TOWNS_KORD       , TOWNS_HJALSINGOR      ,  80,  6, 0, 0, 0, 0 }, //  5
	{ TOWNS_HJALSINGOR , TOWNS_OVERTHORN       , 115,  4, 0, 0, 0, 0 }, //  6
	{ TOWNS_HJALSINGOR , TOWNS_MANRIN          , 110,  5, 0, 0, 0, 0 }, //  7
	{ TOWNS_THORWAL    , TOWNS_VAERMHAG        ,  30,  7, 0, 1, 0, 0 }, //  8
	{ TOWNS_VAERMHAG   , TOWNS_VARNHEIM        ,  37,  5, 0, 1, 0, 0 }, //  9
	{ TOWNS_VARNHEIM   , TOWNS_LJASDAHL        ,  25,  3, 0, 1, 0, 0 }, // 10
	{ TOWNS_VARNHEIM   , TOWNS_OTTARJE         ,  25,  8, 0, 1, 0, 0 }, // 11
	{ TOWNS_LJASDAHL   , TOWNS_OTTARJE         ,  10,  1, 0, 1, 0, 0 }, // 12
	{ TOWNS_SKJAL      , TOWNS_OTTARJE         ,  52,  3, 0, 1, 0, 0 }, // 13
	{ TOWNS_SKJAL      , TOWNS_PREM            ,  28,  4, 0, 1, 0, 0 }, // 14
	{ TOWNS_PREM       , TOWNS_ARYN            ,  28,  6, 0, 1, 0, 0 }, // 15
	{ TOWNS_PREM       , TOWNS_RUNINSHAVEN     ,  35,  7, 0, 1, 0, 0 }, // 16
	{ TOWNS_ARYN       , TOWNS_RUNINSHAVEN     ,  25,  4, 0, 1, 0, 0 }, // 17
	{ TOWNS_RUNINSHAVEN, TOWNS_TREBAN          ,  50,  7, 0, 1, 0, 0 }, // 18
	{ TOWNS_TREBAN     , TOWNS_KORD            ,  40,  2, 0, 1, 0, 0 }, // 19
	{ TOWNS_KORD       , TOWNS_GUDDASUNDEN     ,  65,  2, 0, 1, 0, 0 }, // 20
	{ TOWNS_GUDDASUNDEN, TOWNS_HJALSINGOR      ,  20,  1, 0, 1, 0, 0 }, // 21
	{ TOWNS_HJALSINGOR , TOWNS_ROVIK           ,  46,  2, 0, 1, 0, 0 }, // 22
	{ TOWNS_ROVIK      , TOWNS_OVERTHORN       ,  67,  2, 0, 1, 0, 0 }, // 23
	{ TOWNS_ROVIK      , TOWNS_ORVIL           ,  18,  1, 0, 1, 0, 0 }, // 24
	{ TOWNS_OVERTHORN  , TOWNS_TJANSET         ,  40,  3, 0, 1, 0, 0 }, // 25
	{ TOWNS_OVERTHORN  , TOWNS_VIDSAND         ,  48,  1, 0, 1, 0, 0 }, // 26
	{ TOWNS_TJANSET    , TOWNS_LISKOR          ,  22,  6, 0, 1, 0, 0 }, // 27
	{ TOWNS_LISKOR     , TOWNS_VIDSAND         ,  22,  1, 0, 1, 0, 0 }, // 28
	{ TOWNS_TJANSET    , TOWNS_VIDSAND         ,  36,  2, 0, 1, 0, 0 }, // 29
	{ TOWNS_OVERTHORN  , TOWNS_BRENDHIL        ,  32,  3, 0, 1, 0, 0 }, // 30
	{ TOWNS_BRENDHIL   , TOWNS_MANRIN          ,  57,  5, 0, 1, 0, 0 }, // 31
	{ TOWNS_ROVIK      , TOWNS_MANRIN          ,  83,  7, 0, 1, 0, 0 }, // 32
	{ TOWNS_THORWAL    , TOWNS_MERSKE          ,  30,  7, 0, 1, 0, 0 }, // 33
	{ TOWNS_MERSKE     , TOWNS_EFFERDUN        ,  18,  8, 0, 1, 0, 0 }, // 34
	{ TOWNS_THORWAL    , TOWNS_EFFERDUN        ,  47,  3, 0, 1, 0, 0 }, // 35
	{ TOWNS_SERSKE     , TOWNS_MERSKE          ,  24,  7, 0, 1, 0, 0 }, // 36
	{ TOWNS_SERSKE     , TOWNS_EFFERDUN        ,  41,  5, 0, 1, 0, 0 }, // 37
	{ TOWNS_OVERTHORN  , TOWNS_LISKOR          ,  51,  3, 0, 1, 0, 0 }, // 38
	{ TOWNS_THORWAL    , TOWNS_VARNHEIM        ,  67,  4, 0, 1, 0, 0 }, // 39
	{ TOWNS_OTTARJE    , TOWNS_PREM            ,  62,  4, 0, 1, 0, 0 }, // 40
	{ TOWNS_OTTARJE    , TOWNS_RUNINSHAVEN     ,  73,  7, 0, 1, 0, 0 }, // 41
	{ TOWNS_SKJAL      , TOWNS_RUNINSHAVEN     ,  55,  6, 0, 1, 0, 0 }, // 42
	{ TOWNS_RUNINSHAVEN, TOWNS_LEUCHTTURM_RUNIN,  25,  8, 0, 1, 0, 0 }, // 43
	{ TOWNS_TREBAN     , TOWNS_LEUCHTTURM_RUNIN,  50, 10, 0, 1, 0, 0 }, // 44
	{ TOWNS_MANRIN     , TOWNS_OVERTHORN       ,  60,  5, 0, 1, 0, 0 }, // 45
	{ (unsigned char)-1             , 0x00                  ,   0,  0, 0, 0, 0, 0 } //TODO: towns should be signed
}; // ds:0x6f00
signed char g_travel_by_ship = 0; // ds:0x7070, 0 = on land, 1 = at the ship
static struct Bit16s_7 g_sea_travel_sleepbonus_table1 = { -2, 0, 5, 4, 3, 1, 0 }; // ds:0x7071, { -2, 0, 5, 4, 3, 1, 0 }
static struct Bit16s_7 g_sea_travel_sleepbonus_table2 = { -2, 0, 5, 4, 3, 1, 0 }; // ds:0x707f, { -2, 0, 5, 4, 3, 1, 0 }
static char g_sea_travel_str_t[2] = "T"; // ds:0x708d
static char g_sea_travel_str_en[3] = "EN"; // ds:0x708f
static char g_sea_travel_str_comma[3] = ", "; // ds:0x7092


static signed char g_sea_travel_sleep_quality; // ds:0xe3fa

void passages_init(void)
{
	signed short si;
	signed short i;
	struct sea_route *route = &g_sea_routes[0];


	for (i = 0; i < NR_SEA_ROUTES; route++, i++) {

		route->passage_timer = (unsigned char)random_interval(0, route->frequency);
		route->price_mod = (unsigned char)random_interval(70, 130);

		si = random_schick(100);

		if (!route->coastal_route) {
			/* high seas route */
			route->ship_type = si <= 50 ? SHIP_TYPE_LANGSCHIFF_HIGH_SEAS : (si <= 80 ? SHIP_TYPE_KARRACKE : (si <= 95 ? SHIP_TYPE_SCHNELLSEGLER : SHIP_TYPE_SCHNELLSEGLER_LUXURY));

		} else {

			/* costal route */
			route->ship_type = si <= 10 ? SHIP_TYPE_LANGSCHIFF_COSTAL : (si <= 40 ? SHIP_TYPE_KUESTENSEGLER : (si <= 80 ? SHIP_TYPE_KUTTER : SHIP_TYPE_FISCHERBOOT));
		}

#if !defined(__BORLANDC__)
	D1_LOG("%16s - %16s: %d %d %d %d %d %d\n",
		get_ttx(route->town1 + 0xeb),
		get_ttx(route->town2 + 0xeb),
		route->distance,
		route->frequency,
		route->passage_timer,
		route->coastal_route,
		route->ship_type,
		route->price_mod);
#endif
	}
}

void do_harbor(void)
{
	signed short i; /* diverse usage */
	signed short done;
	signed short answer;
	struct harbor_option_obsolete *psg_ptr;
	Bit32s p_money;
	struct struct_hero *hero;
	signed char flag;
	Bit32s money;

	done = 0;
	struct Bit16s_7 a = g_sea_travel_sleepbonus_table1;
	//struct dummy7 a = { { -2, 0, 5, 4, 3, 1, 0 } };

	flag = 1;

	gs_current_signpost = gs_current_typeindex;

	draw_loc_icons(4, MENU_ICON_BOOK_SHIP_PASSAGE, MENU_ICON_HARBOR_MASTER, MENU_ICON_BOARD_SHIP, MENU_ICON_LEAVE);
	g_request_refresh = 1;
	gs_travel_detour = 0;

	do {
		if (g_request_refresh) {

			draw_main_screen();
			disable_ani();
			load_ani(6);
			init_ani(0);
			g_request_refresh = 0;

			load_tx(ARCHIVE_FILE_HAFEN_LTX);

			if (flag != 0) {

				flag = 0;

				if ((gs_quested_months > 2) && !gs_treasure_maps[6] &&
					gs_informer_flags[INFORMER_SWAFNILD] && (random_schick(100) <= gs_quested_months + 4))
				{
					/* meet SWAFNILD */
					do_talk(12, 0);

					if (g_swafnild_traveled) {

						g_swafnild_traveled = 0;

						leave_location();

						return;
					}
				}
			}
		}

		handle_gui_input();

		if (g_mouse_rightclick_event || g_action == ACTION_ID_PAGE_UP) {

			answer = GUI_radio(get_tx(9), 4,get_tx(10), get_tx(11), get_tx(12), get_tx(13)) - 1;

			if (answer != -2) {
				g_action = answer + ACTION_ID_ICON_1;
			}
		}

		if (g_action == ACTION_ID_ICON_1) {
			/* book passage */

			answer = prepare_passages();

			if (answer == 0) {
				GUI_output(get_tx(15));
			} else {

				/* select a destination */
				answer = GUI_radio(get_tx(14), (signed char)answer,
						get_ttx(gs_harbor_options[0].destination + 235),
						get_ttx(gs_harbor_options[1].destination + 235),
						get_ttx(gs_harbor_options[2].destination + 235),
						get_ttx(gs_harbor_options[3].destination + 235),
						get_ttx(gs_harbor_options[4].destination + 235),
						get_ttx(gs_harbor_options[5].destination + 235),
						get_ttx(gs_harbor_options[6].destination + 235),
						get_ttx(gs_harbor_options[7].destination + 235),
						get_ttx(gs_harbor_options[8].destination + 235),
						get_ttx(gs_harbor_options[9].destination + 235)) - 1;

				if (answer != -2) {

					psg_ptr = &gs_harbor_options[answer];

					sprintf(g_dtp2,	get_tx(16),

						get_tx(g_sea_travel_tx_ship[psg_ptr->ship_type]), /* Fischerboot, Schnellsegler etc. */
						psg_ptr->ship_name_ptr,
						(!psg_ptr->ship_timer ? get_tx(5) : get_tx(6)), /* today or tomorrow */

						get_tx(g_passage_type_to_name[g_ship_table[psg_ptr->ship_type].passage_type]), /* Kabinenpassage etc. */
						get_ttx(psg_ptr->destination + 235),
#ifdef __BORLANDC__
						get_passage_travel_hours(psg_ptr->sea_route_ptr->distance, g_ship_table[psg_ptr->ship_type].base_speed),
#else
						/* when compiled with gcc, occasionally passage times of 0 hours do show up. (which does not happen in the original game!!)
						 * I observed that within the function get_passage_travel_hours(..), computations with negative numbers might happen and lead to this bug.
						 * The following line fixes this. However, it will lead to incompatible binaries when compiled with the original 1992 BCC compiler
						 * This incompatibility of the behavior gcc vs. BCC is a bit scary.
						 * A better understanding is urgently needed... */
						/* REMARK: The base speed should be an unsigned char,
						 * since a speed of 150 would result in a negative speed, which ssomehow happened.
						 * Changing just this type to unsigned would hurt the binary aequivalency.
						 * */
						get_passage_travel_hours(psg_ptr->sea_route_ptr->distance, g_ship_table[psg_ptr->ship_type].base_speed),
#endif
						print_passage_price(g_ship_table[psg_ptr->ship_type].base_price, psg_ptr->sea_route_ptr));

					i = g_textbox_width;
					g_textbox_width = 5;

					answer = GUI_bool(g_dtp2);

					g_textbox_width = i;

					if (answer != 0) {

						money = get_party_money();

						if (gs_sea_travel_passage_price > money) {
							/* party does not have enough money to pay the ship passage */

							GUI_output(get_ttx(401));

						} else {

							g_sea_travel_sleep_quality = g_ship_table[psg_ptr->ship_type].passage_type;
							money -= gs_sea_travel_passage_price;
							set_party_money(money);

							gs_sea_travel_psgbooked_timer = psg_ptr->ship_timer;
							gs_sea_travel_psgbooked_flag = 0xaa;

							gs_sea_travel_passage_speed1 = gs_sea_travel_passage_speed2; /* speed in [100m per hour] */
							/* Now ..._SPEED1 is the lower byte of ..._SPEED2 */
							/* not clear why two variables ..._SPEED1 and ..._SPEED2 are used. */
							/* In my opinion, a single variable would be enough (and then there would not be the need to copy the value around) */

							gs_current_sea_route_id = psg_ptr->route_id;

							GUI_output(psg_ptr->ship_timer ? get_tx(18) : get_tx(17));
							/* ship leaving tomorrow or today */
						}
					}
				}
			}

		} else if (g_action == ACTION_ID_ICON_2) {
			/* Hafenmeister */

			if (gs_day_timer <= HOURS(6) || gs_day_timer >= HOURS(21)) {

				GUI_output(get_ttx(568));

			} else {

				load_in_head(11);

				do {
					answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx(4),
									get_tx(0), 3,
									get_tx(1), /* Ankommende Schiffe erfragen */
									get_tx(2), /* Saemtliche Routen erfragen */
									get_tx(3)  /* Den Geldhai verlassen */
					);
				} while (answer == -1);

				p_money = get_party_money();

				if (p_money < 10L) {

					GUI_output(get_tx(8));

				} else if (answer == 1 || answer == 2) {
					/* answer == 1: Ankommende Schiffe erfragen (next 2 days)
					 * answer == 2: Saemtliche Routern erfragen */

					p_money -= 10L;
					set_party_money(p_money);

					i = answer;
					answer = get_next_passages(answer);
					/* answer now: number of passages reported by Hafenmeister */

					if (answer != 0) {

						sprintf(g_dtp2,	(i == 1 ? get_tx(28) : get_tx(22)),
							(answer == 1 ? g_sea_travel_str_t : g_sea_travel_str_en),
							(answer == 1 ? get_tx(23) : get_tx(24)));

						i = 0;

						do {

							strcat(g_dtp2, get_ttx(gs_harbor_options[i++].destination + 235));

							if (--answer) {

								strcat(g_dtp2, (answer >= 2 ? g_sea_travel_str_comma : get_tx(7)));
							}

						} while (answer);

						strcat(g_dtp2, get_tx(25));

						GUI_output(g_dtp2);

					} else {

						GUI_output(get_tx(26));
						/* "In den naechsten Tagen wird wohl kein Schiff kommen [...]" */
					}
				}
			}

		} else if (g_action == ACTION_ID_ICON_3) {
			/* enter booked ship */

			if (gs_sea_travel_psgbooked_flag != 0xaa) {
				/* no ship booked... */

				GUI_output(get_tx(19));

			} else if (gs_sea_travel_psgbooked_timer) {

				/* REMARK: cast is important, since var is Bit8u */
				GUI_output((signed char)gs_sea_travel_psgbooked_timer == -1 ?
					get_tx(27) : /* SEA_TRAVEL_PSGBOOKED_TIMER == -1 -> "Zu spaet! Das Schiff, fuer das ihr gebucht wart ist leider ohne euch losgefahren!" */
					get_tx(20)   /* SEA_TRAVEL_PSGBOOKED_TIMER == +1 -> "Die Matrosen lassen euch das Schiff noch nicht besteigen. Kommt morgen wieder..." */
				);

			} else {

				disable_ani();

				GUI_output(get_tx(21));
				/* "Ihr begebt euch an Bord. <Macht's euch gemuetlich!> brummelt der Kapitaen. <Wir legen bald ab!>" */

				g_travel_herokeeping = 1;

				timewarp_until_time_of_day(HOURS(9));

				/* g_check_disease is set to 1 each midnight in timers_daily() */
				if (g_check_disease) {
					/* effectively, this branch is reached if the ship was entered before midnight. */

					disease_effect();
					/* in this call, g_check_disease is reset to 0 */

					hero = get_hero(0);
					for (i = 0; i <= 6; i++, hero++) {

						if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group))
						{
							GRP_hero_sleep(hero, a.a[g_sea_travel_sleep_quality]);

							hero->hunger = hero->thirst = 0;
							/* on the ship, food and drinks are served. */
						}
					}
				}

				g_travel_herokeeping = 0;

				for (i = 0; i < 6; i++) {
					g_food_message_shown[i] = 0;
				}

				load_map();

				g_current_ani = -1;
				g_wallclock_update = 0;

				memmove(g_renderbuf_ptr, gs_travel_map_ptr, 64000);
				map_effect(g_renderbuf_ptr);

				wait_for_vsync();

				set_palette(gs_travel_map_ptr + 64002, 0, 0x20);

				mod_clock_pos(gs_current_town);

				set_audio_track(ARCHIVE_FILE_TERMS_XMI);

				g_wallclock_x = g_basepos_x + 120;
				g_wallclock_y = g_basepos_y + 87;
				g_wallclock_update = 1;

				sea_travel(gs_current_sea_route_id, (g_sea_routes[gs_current_sea_route_id].town1 == gs_current_town ? 0 : 1));
				passage_arrival();

				g_wallclock_update = g_basepos_x = g_basepos_y = gs_sea_travel_psgbooked_flag = 0;
				g_current_ani = g_city_area_loaded = g_pp20_index = -1;
				g_request_refresh = 1;
				gs_show_travel_map = 0;

				if (!gs_travel_detour) {

					gs_current_town = (signed char)gs_travel_destination_town_id;
					gs_x_target_bak = gs_travel_destination_x;
					gs_y_target_bak = gs_travel_destination_y;
					gs_direction = ((gs_travel_destination_viewdir + 2) & 3);

				} else {
					done = 1;
					DNG_enter_dungeon(DUNGEONS_TOTENSCHIFF);
				}
			}

		} else if (g_action == ACTION_ID_ICON_4) {
			done = 1;
		}

	} while (!done);

	i = load_archive_file(ARCHIVE_FILE_COMPASS);
	read_archive_file(i, g_buffer6_ptr, 5000);
	close(i);

	disable_ani();

	if (!gs_travel_detour) {

		copy_palette();
		leave_location();

	} else {

		gs_current_town = TOWNS_NONE;
	}
}

/**
 * \brief   * \param town_id	ID of the town
 */
void mod_clock_pos(signed short town_id)
{
	signed short val;
	signed short map_x;
	signed short map_y;

	map_x = g_town_positions[town_id - 1].x;
	//map_y = g_town_positions[town_id - 1].y;
	map_y = *(signed short*)((Bit8u*)g_town_positions - sizeof(signed short) + 2 * sizeof(signed short) * town_id);

	val = map_x >= 0 && map_x <= 159 ?
		(map_y >= 0 && map_y <= 99 ? 3 : 1) :
		(map_y >= 0 && map_y <= 99 ? 2 : 0);

	g_basepos_x = ((!val || val == 2) ? -80 : 80);
	g_basepos_y = ((!val || val == 1) ? -40 : 40);
}

void sea_travel(signed short passage_id, signed short reverse)
{
	signed short i;
	struct struct_hero *hero;
	Bit8u *ptr;
	Bit32s off;

	struct Bit16s_7 a = g_sea_travel_sleepbonus_table2;
	//struct dummy7 a = { { -2, 0, 5, 4, 3, 1, 0 } };

	g_traveling = 1;

	gs_sea_travel_courses = (Bit8u*)(passage_id < 7 ? (g_buffer9_ptr + 7600L) : (g_buffer9_ptr + 11400L));

	/* high seas routes have id 0..6, costal routes id 7..44 */

	/* if high seas route, write 7 (total number of high seas routes)
	 * if costal route, write 38 (total number of costal routes) */
	gs_route_mousehover = (passage_id < 7 ? 7 : 38);

	/* convert costal route ids to range 0..37 */
	gs_sea_travel_passage_id = passage_id < 7 ? passage_id : passage_id - 7;

	off = ((Bit32s*)gs_sea_travel_courses)[gs_sea_travel_passage_id];
	gs_route_course_ptr = (Bit16s*)(gs_sea_travel_courses + off + 4 * gs_route_mousehover);
	ptr = g_vga_memstart;

	gs_route_course_ptr += 2;

	memset(g_trv_track_pixel_bak, 0xaa, 500);
	gs_travel_speed = 10 * gs_sea_travel_passage_speed1; /* speed [unit: 10m per hour] */
	gs_route_total_steps = get_srout_len((struct struct_point*)gs_route_course_ptr); /* a step for each pixel on the map. */
	gs_route_length = 100 * g_sea_routes[passage_id].distance; /* length of sea route [unit: 10m] */
	gs_route_duration = gs_route_length / gs_travel_speed * 60; /* duration [unit: minutes] */
	gs_route_timedelta = gs_route_duration / gs_route_total_steps; /* duration of each step [unit: minutes] */
	gs_route_stepsize = gs_route_length / gs_route_total_steps; /* length of a single step [unit: 10m] */

	if (gs_route_stepsize == 0) {
		gs_route_stepsize = 1;
	}
#if !defined(__BORLANDC__)
	D1_INFO("Schiffspassage gestartet. Entfernung: %d0 Schritt. Geschwindigkeit: %d0 Schritt/h. Dauer (lt. Hafen): %d min. Dauer (real): %d min.\n", gs_route_length, gs_travel_speed, gs_route_duration,gs_route_total_steps*2*(gs_route_timedelta/2));
	D1_INFO_VERBOSE("#Pixel = %d, Entfernung/Pixel: %d0 Schritt, Dauer/Pixel: %d min\n", gs_route_total_steps, gs_route_stepsize, gs_route_timedelta);
#endif

	if (reverse) {
		/* for reverse direction, point gs_route_course_ptr to end of route */

		while (gs_route_course_ptr[0] != -1) {
			gs_route_course_ptr += 2;
		}

		gs_route_course_ptr -= 2;
	}

	gs_route_course_start = gs_route_course_ptr;

	gs_route_dayprogress = (18 * (gs_travel_speed + gs_travel_speed / 10));
	/* this is 19.8h * gs_travel_speed, which is the distance [unit: 10m] the ship travels in 19.8 h.
	 * It is used as upper bound for the position of the random encounters. */

	if (passage_id <= 6 && gs_quest_deadship && !gs_quest_deadship_done) {
		/* only on high seas routes */

		if ((gs_passage_deadship_flag = random_schick(100) <= 20 ? 1 : 0)) {
			gs_passage_deadship_position = random_schick(gs_route_dayprogress);
#if !defined(__BORLANDC__)
			D1_INFO("Totenschiff wurde bei %o0 Schritt aktiviert!\n", gs_passage_deadship_position);
#endif
		}
	} else {
		gs_passage_deadship_flag = 0;
	}

	if ((gs_passage_octopus_flag = random_schick(100) <= 5 ? 1 : 0)) {

		gs_passage_octopus_position = random_schick(gs_route_dayprogress);
#if !defined(__BORLANDC__)
		D1_INFO("Krakenmolch wurde bei %o0 Schritt aktiviert!\n", gs_passage_octopus_position);
#endif
	}

	if ((gs_passage_pirates_flag = random_schick(100) <= 10 ? 1 : 0)) {

		gs_passage_pirates_position = random_schick(gs_route_dayprogress);
#if !defined(__BORLANDC__)
		D1_INFO("Piratenangriff wurde bei %o0 Schritt aktiviert!\n", gs_passage_pirates_position);
#endif
	}

	gs_route_stepcount = gs_route_progress = gs_route_dayprogress = gs_travel_detour = 0;
	g_travel_herokeeping = 1;

	while (gs_route_course_ptr[(gs_route_mousehover = 0)] != -1 && !gs_travel_detour)
	{

		if (is_mouse_in_rect(gs_route_course_ptr[0] - 16, gs_route_course_ptr[1] - 16,
					gs_route_course_ptr[0] + 16, gs_route_course_ptr[1] + 16))
		{
			call_mouse_bg();
			gs_route_mousehover = 1;
		}

		g_trv_track_pixel_bak[gs_route_stepcount] =
			*(ptr + gs_route_course_ptr[1] * 320 + gs_route_course_ptr[0]);

		gs_route_stepcount++;

		*(ptr + gs_route_course_ptr[1] * 320 + gs_route_course_ptr[0]) = 0x1f;

		if (gs_route_mousehover) {
			call_mouse();
		}

		/* the following loop will be executed Floor(gs_route_timedelta / 2) times.
		 * therefore, 2 * Floor(gs_route_timedelta / 2) minutes ingame times will pass. */
		for (gs_trv_i = 0; gs_route_timedelta / 2 > gs_trv_i; gs_trv_i++) {

			handle_input();

			wait_for_vsync();

			timewarp(MINUTES(2));
		}

		gs_route_progress += gs_route_stepsize;
		gs_route_dayprogress += gs_route_stepsize;

#if !defined(__BORLANDC__)
		D1_LOG("%d0 Schritt zurueckgelegt.\n",gs_route_dayprogress);
#endif

		if (gs_passage_deadship_flag != 0 && gs_route_dayprogress >= gs_passage_deadship_position && !gs_quest_deadship_done) {

			/* within the call prolog_ghostship(), the party can decide if they enter the Totenschiff.
			 * In that case, gs_travel_detour is set to DUNGEONS_TOTENSCHIFF (instead of 0) */
			prolog_ghostship();
			gs_passage_deadship_flag = 0;

		} else if (gs_passage_octopus_flag != 0 && gs_route_dayprogress >= gs_passage_octopus_position && !gs_ingame_timers[INGAME_TIMER_EFFERD_SAFE_PASSAGE]) {

			octopus_attack_wrapper();
			gs_passage_octopus_flag = 0;

		} else if
#ifndef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 34:
			 * There is an Efferd miracle with the text "Efferd gewaehrt euch seinen Schutz auf Wasser.".
			 * For sea traveling, it prevents octopus encounters. However, pirate encounters are still possible, which feels wrong. */
			(gs_passage_pirates_flag != 0 && gs_route_dayprogress >= gs_passage_pirates_position)
#else
			(gs_passage_pirates_flag != 0 && gs_route_dayprogress >= gs_passage_pirates_position && !gs_ingame_timers[INGAME_TIMER_EFFERD_SAFE_PASSAGE])
#endif
		{
			pirates_attack_wrapper();
			gs_passage_pirates_flag = 0;
		}

		/* This looks dirty.
		 * g_check_disease is set to 1 each midnight in timers_daily()
		 * Usually, the disease_effect() function is called within game_loop(). But the game does not jump back to that function at this point,
		 * which is probably the reason to replicate the disease_effect() call here.
		 * Why the dependence on g_check_party? */
		if (g_check_disease && !g_check_party) {

			disease_effect();

			g_travel_by_ship = 1;

			hero = get_hero(0);
			for (i = 0; i <= 6; i++, hero++) {

				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group))
				{
					GRP_hero_sleep(hero, a.a[g_sea_travel_sleep_quality]);
					hero->hunger = hero->thirst = 0;
				}
			}

			g_travel_by_ship = 0;
		}

		if (g_request_refresh && !gs_travel_detour) {

			call_mouse_bg();

			load_map();

			/* TODO: update window */
			memmove((void*)g_vga_memstart, (void*)gs_travel_map_ptr, 320 * 200);

			wait_for_vsync();

			set_palette(gs_travel_map_ptr + 64002, 0, 0x20);

			set_audio_track(ARCHIVE_FILE_TERMS_XMI);

			gs_trv_i = 0;

			for (gs_route_course_ptr2 = gs_route_course_start;
					g_trv_track_pixel_bak[gs_trv_i++] != 0xaa;
					gs_route_course_ptr2 += (!reverse ? 2 : -2))
			{
				*(ptr + gs_route_course_ptr2[1] * 320 + gs_route_course_ptr2[0]) = 0x1f;
			}

			call_mouse();

			g_wallclock_x = g_basepos_x + 120;
			g_wallclock_y = g_basepos_y + 87;
			g_wallclock_update = 1;
			g_request_refresh = 0;
		}

		gs_route_course_ptr += (!reverse ? 2 : -2);
	}

	g_travel_herokeeping = 0;

	if (!gs_travel_detour) {

		call_mouse_bg();

		do {
			if (!reverse) {
				gs_route_course_ptr -= 2;
			} else {
				gs_route_course_ptr += 2;
			}
			gs_route_stepcount--;

			*(ptr + gs_route_course_ptr[1] * 320 + gs_route_course_ptr[0]) =
				g_trv_track_pixel_bak[gs_route_stepcount];

		} while (gs_route_course_ptr[0] != -1);

		call_mouse();
	}

	g_traveling = 0;
}

signed int get_srout_len(struct struct_point *ptr)
{
	signed int i = 0;

	while (ptr->x != -1) {
		ptr++;
		i++;
	}

	return i;
}

#if !defined(__BORLANDC__)
}
#endif
