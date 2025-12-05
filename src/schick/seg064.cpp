/*
 *	Rewrite of DSA1 v3.02_de functions of seg064 (harbor_helper)
 *	Functions rewritten: 6/6 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg064.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg007.h"
#include "seg028.h"
#include "seg064.h"

static const char g_sea_travel_str_heller[10] = "%d^HELLER"; // ds:0x7096
static const char g_sea_travel_str_nothing[7] = "NICHTS"; // ds:0x70a0

/**
 * \brief   returns a pointer to the name of a ship
 *
 * \param   passage_type           eg 0 = longship, 5 = coastal ship, 6 = cutter,7 = fishing boat
 * \param   nr_ships_created	number of ships which have already been created before (for avoiding duplicate ship names)
 */

char* get_ship_name(const signed char passage_type, const signed int nr_ships_created)
	/* Function is called in a single place only, which is contained in the code of Original-Bug 23 (see below).
	 * Therefore, the function could be removed if M302de_ORIGINAL_BUGFIX is activated.
	 */
{
	signed char done, i;
	signed int name_id;

	done = 0;

	do {
		name_id = passage_type * 10 + random_schick(10) + 0x29;
		done = 1;

		/* check if there is already a ship with the same name */
		for (i = 0; i < nr_ships_created; i++) {
			if (gs_harbor_options[i].ship_name_ptr == get_tx(name_id)) {
				done = 0;
				break;
			}
		}

	} while (!done);

	return get_tx(name_id);
}

/**
 * \brief   prepares the available passages at a harbor
 *
 * \return	number of prepared passages */

signed int prepare_passages(void)
{
	signed int prepared;
	signed int i;
	struct sea_route *ent = &g_sea_routes[0];

	for (i = prepared = 0; i < (SEA_ROUTE_ID__END - 1); ent++, i++) {

		if (!ent->passage_timer && (ent->town_id_1 == gs_town_id || ent->town_id_2 == gs_town_id)) {

			/* ship is leaving today */

			/* prepare a 12-byte entry in gs_harbor_options */
			gs_harbor_options[prepared].route_id = (unsigned char)i;
			gs_harbor_options[prepared].sea_route_ptr = ent;
			gs_harbor_options[prepared].ship_timer = 0;
			gs_harbor_options[prepared].ship_type = ent->ship_type;

#ifndef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 23:
			 * In the function get_ship_name(..), the ship names are created randomly every time the party checks the available ships at a harbor.
			 * In this way, the names of the ships can (and usually do) change when repeatedly checking the available ships. */
			gs_harbor_options[prepared].ship_name_ptr = get_ship_name(ent->ship_type, prepared);
#else
			/* As a fix, we derive the name from the PASSAGE_PRICE_MOD entry of the SEA_ROUTE, which is created
			 * randomly once the new passage of the route is set up, and is kept fixed over the lifetime of the passage.
			 *
			 * In this way, it may however happen that two ships of the same name are located in a harbor
			 * (which has been avoided in the original random assignment code). But this is a rare event and not be a big issue anyway.
			 */
			gs_harbor_options[prepared].ship_name_ptr = get_tx(ent->ship_type * 10 + ent->price_mod % 10 + 0x2a);
#endif

			gs_harbor_options[prepared].destination = ent->town_id_1 == gs_town_id ? ent->town_id_2 : ent->town_id_1;

				/* store the other town of the connection */
			prepared++;
		} else {
			if ((gs_day_timer > HOURS(14)) && (ent->passage_timer == 1)
				&& ((ent->town_id_1 == gs_town_id) || (ent->town_id_2 == gs_town_id)))
		        {
				/* ship is leaving tomorrow and it is later than 14:00 */

				/* prepare a 12-byte entry in gs_harbor_options */
				gs_harbor_options[prepared].route_id = i;
				gs_harbor_options[prepared].sea_route_ptr = ent;
				gs_harbor_options[prepared].ship_timer = 1;
				gs_harbor_options[prepared].ship_type = ent->ship_type;

#ifndef M302de_ORIGINAL_BUGFIX
				/* Original-Bug 23:
				 * see above */
				gs_harbor_options[prepared].ship_name_ptr = get_ship_name(ent->ship_type, prepared);
#else
				gs_harbor_options[prepared].ship_name_ptr = get_tx(ent->ship_type * 10 + ent->price_mod % 10 + 0x2a);
#endif

				gs_harbor_options[prepared].destination = ent->town_id_1 == gs_town_id ? ent->town_id_2 : ent->town_id_1;

				prepared++;
			}
		}
	}
	return prepared;
}

/**
 * \brief   calculates the price and prints it to the text buffer
 *
 * \param   price             base price per unit of distance
 * \param   route_pointer     pointer to the sea route
 * \return                    a pointer to the text buffer.
 */
char* print_passage_price(signed int price, const struct sea_route *route)
{
	unsigned int di;

	if (price != 0) {
		/* calc price per distance */
		di = ROUNDED_DIVISION(route->price_mod * price, 10);
		/* multiply with distance */
		di = di * route->distance;
		price = ROUNDED_DIVISION(di,100);

		/* generate a price string "%d^HELLER" */
		sprintf(g_text_output_buf, g_sea_travel_str_heller, price);
	} else {
		/* "NICHTS" */
		strcpy(g_text_output_buf, g_sea_travel_str_nothing);
	}

	gs_sea_travel_passage_price = price;

	return g_text_output_buf;
}

/**
 * \brief   calculates the traveling time of a ship passage in hours
 *
 * \param   distance     the distance [unit: km]
 * \param   base_speed   base speed of the ship [unit: km per day]
 * \return               the traveling time in hours
 */
signed int get_passage_travel_hours(const signed int distance, signed int base_speed)
{
	int32_t tmp;

	/* convert base_speed to unit [100m per hour] */
	base_speed = ROUNDED_DIVISION(base_speed * 10, 24);
	/* lowest possible value: (40 * 10 + 11) / 24 = 17 (for Fischerboot) */
	/* highest possible value: (150 * 10 + 11) / 24 = 62 (for Schnellsegler) */

	/* reminder: WEATHER1 = random([1..6]), WEATHER2 = random([1..7]) */
	/* adjust speed to between 80.5% (when WEATHER1 = WEATHER2 = 1) and 247% (when WEATHER1 = 6 and WEATHER2 = 7). */
	/* unit: [100m per hour] */
	gs_sea_travel_passage_speed2 = ROUNDED_DIVISION(base_speed * (gs_weather2 + 6) * (gs_weather1 * 15 + 100), 1000L);
		/* possible values for gs_sea_travel_passage_speed2:
		 * ## high seas routes ##
		 * Schnellsegler (base_speed 150): {* 50, 56, 57, 63, 64^^2, 69, 71, 72, 73, 76, 78, 79, 81^^2, 82, 86, 87, 89^^2, 90, 93, 94, 97, 98, 99^^2, 105, 106, 108^^2, 109, 117, 118, 119^^2, 129, 130^^2, 141^^2, 153 *}
		 * Langschiff (base_speed 120): {* 40, 45, 46, 51, 52^^2, 56, 57, 58^^2, 61, 63, 64, 65^^2, 66, 69, 70, 71, 72^^2, 75, 76, 78, 79, 80^^2, 84, 85, 87^^2, 88, 94, 95, 96^^2, 104^^2, 105, 114^^2, 123 *}
		 * Karracke (base_speed 100): {* 34, 38, 39, 43^^2, 44, 47, 48, 49^^2, 51, 53, 54, 55^^2, 56, 58, 59, 60^^2, 61, 63, 64, 66^^2, 67^^2, 71, 72, 73^^2, 74, 79, 80, 81^^2, 87, 88^^2, 96^^2, 104 *}
		 * ## costal routes ##
		 * Langschiff (base_speed 90): {* 30, 34^^2, 38^^3, 41, 43^^3, 45, 47^^2, 48^^2, 49, 51, 52, 53^^2, 54, 55, 56, 58^^2, 59^^2, 63^^2, 64, 65^^2, 70^^2, 71^^2, 77^^2, 78, 84^^2, 91 *}
		 * Kuestensegler (base_speed 80): {* 27, 30^^2, 33, 34^^2, 37, 38^^2, 39, 40, 42^^2, 43^^2, 44, 46^^2, 47, 48^^2, 49, 50, 51, 52, 53^^2, 56^^2, 57, 58^^2, 62, 63^^2, 64, 69^^3, 75^^2, 82 *}
		 * Kutter (base_speed 60): {* 20, 23^^2, 25, 26^^2, 28, 29^^3, 31, 32^^3, 33^^2, 34, 35, 36^^3, 37, 38, 39^^2, 40^^2, 42, 43^^2, 44^^2, 47^^2, 48^^2, 52^^3, 57^^2, 62 *}
		 * Fischerboot (base_speed 40): {* 14, 15, 16, 17, 18^^2, 19, 20^^3, 21, 22^^4, 23^^2, 24^^3, 25^^2, 26, 27^^4, 29^^2, 30^^3, 32^^2, 33^^2, 35, 36^^2, 39^^2, 42 *} */

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 35:
	 * In the predicted traveling times of sea passages, only very specific values show up.
	 * For example, the predicted number of hours for Prem-Hjalsingor will be one of 14, 15, 16, 17, 19, 21, 23, 26, 30, 35, 42, 52, 70, 105, 210.
	 * The reason is that a bad conversion severly reduces the computational precision. */

	tmp = ROUNDED_DIVISION(gs_sea_travel_passage_speed2, 10); /* the speed of the ship [unit: km per hour] */

	/* possible values for tmp at this point:
	 * ## high seas routes ##
	 * Schnellsegler (base_speed 150): {* 5, 6^^5, 7^^4, 8^^6, 9^^7, 10^^5, 11^^4, 12^^4, 13^^3, 14^^2, 15 *}
	 * Langschiff (base_speed 120): {* 4^^2, 5^^4, 6^^9, 7^^7, 8^^7, 9^^5, 10^^5, 11^^2, 12 *}
	 * Karracke (base_speed 100): {* 3, 4^^5, 5^^9, 6^^8, 7^^9, 8^^4, 9^^3, 10^^3 *}
	 * ## costal routes ##
	 * Langschiff (base_speed 90): {* 3^^3, 4^^8, 5^^11, 6^^10, 7^^4, 8^^5, 9 *}
	 * Kuestensegler (base_speed 80): {* 3^^6, 4^^10, 5^^11, 6^^9, 7^^5, 8 *}
	 * Kutter (base_speed 60): {* 2^^4, 3^^14, 4^^14, 5^^7, 6^^3 *}
	 * Fischerboot (base_speed 40): {* 1^^2, 2^^20, 3^^15, 4^^5 *} */

	if (tmp == 0)
		/* should not happen, see computed values above */
		tmp = 1;

	tmp = distance / tmp; /* now 'tmp' is the number of traveling hours */
#else
	/* first multiply, then divide for higher precision */
	tmp = ROUNDED_DIVISION(10 * distance, gs_sea_travel_passage_speed2);
#endif

	return tmp;
}

/**
 * \brief   get destination harbors. called from Hafenmeister option at a harbor
 *
 * \param   type        1 = passages next days "Ankommende Routen erfragen" / 2 = all passages
 * \return  number of prepared passages.
 */
signed int get_next_passages(const signed int type)
{
	sea_route *entry = &g_sea_routes[0];
	signed int option;
	signed int i;

	for (i = option = 0; i < (SEA_ROUTE_ID__END - 1); entry++, i++) {

		if (type == 1) {

			/* check passages in the next two days */
			if (entry->passage_timer == 1 || entry->passage_timer == 2) {

				/* compare town */
				if (entry->town_id_1 == gs_town_id || entry->town_id_2 == gs_town_id) {
					gs_harbor_options[option].destination = entry->town_id_1 == gs_town_id ? entry->town_id_2 : entry->town_id_1;

					option++;
				}
			}
		} else {

			/* compare town */
			if (entry->town_id_1 == gs_town_id || entry->town_id_2 == gs_town_id) {

				gs_harbor_options[option].destination = entry->town_id_1 == gs_town_id ? entry->town_id_2: entry->town_id_1;

				option++;
			}
		}
	}

	return option;
}

signed int passage_arrival(void)
{
	signed int tmp;
	struct trv_start_point *harbor_ptr;
	struct location *locations_tab_ptr;
	sea_route *p_sea_route;
	signed int si;
	signed int harbor_typeindex;

	harbor_typeindex = 0;
	harbor_ptr = &g_harbors[0];

	p_sea_route = &g_sea_routes[gs_current_sea_route_id];

	/* write the id of the destination town to the global variable gs_travel_destination_town_id
	   Code is a bit unorthodox: Within the if-condition, the id of TOWN_1 is written to gs_travel_destination_town_id
	   Then the condition is evaluated: If this is the initial town, then gs_travel_destination_town_id is overwritten by the id of TOWN_2,
	   which in this case must be the destination town. */
	if ((gs_travel_destination_town_id = p_sea_route->town_id_1) == gs_town_id) {
		gs_travel_destination_town_id = p_sea_route->town_id_2;
	}

	/* find the harbor of the destination town. */

	do {
		if ((unsigned char)harbor_ptr->town_id == gs_travel_destination_town_id) {
			si = 0;
			do {
				/* tmp ranges over the IDs of the linked sea routes, diminished by 1. */
				tmp = harbor_ptr->linked_travel_routes[si] - 1;
				if (g_sea_routes[tmp].town_id_1 == gs_town_id || g_sea_routes[tmp].town_id_2 == gs_town_id) {
					//harbor_typeindex = (unsigned char)harbor_ptr->typeindex;
					harbor_typeindex = harbor_ptr->typeindex;
					break;
				}

				si++;

			} while (harbor_ptr->linked_travel_routes[si] != 0xff);
		}

		harbor_ptr++;

	} while ((harbor_typeindex == 0) && (harbor_ptr->town_id != -1));

	if (harbor_typeindex) {

		/* save the old town in tmp */
		tmp = gs_town_id;
		/* set the new town in current_town */
		gs_town_id = gs_travel_destination_town_id;

		/* load the area of the new town */
		call_load_area(1);

		/* search for the harbor in the locations list */
		locations_tab_ptr = &g_locations_tab[0];
		while ((locations_tab_ptr->loctype != LOCTYPE_HARBOR) || (locations_tab_ptr->typeindex != harbor_typeindex)) {
			locations_tab_ptr++;
		}

		/* set the position of the party */
		si = locations_tab_ptr->locdata;

		gs_travel_destination_x = (si >> 8) & 0xff;		/* = (si / 256) % 255 */
		gs_travel_destination_y = si & 0x0f;			/* = si % 15 */
		gs_travel_destination_viewdir = (si >> 4) & 0x0f;	/* = (si / 16) % 15 */

		/* restore the old town area / TODO: a bit bogus */
		gs_town_id = tmp;
		call_load_area(1);
	}

	return 0;
}
