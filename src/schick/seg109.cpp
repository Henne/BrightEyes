/*
 *	Rewrite of DSA1 v3.02_de functions of seg109 (travel events 1 / 10)
 *	Functions rewritten: 30/30 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg109.cpp
 */

#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg032.h"
#include "seg047.h"
#include "seg051.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg110.h"
#include "seg111.h"
#include "seg112.h"
#include "seg113.h"
#include "seg114.h"
#include "seg115.h"
#include "seg116.h"
#include "seg117.h"
#include "seg118.h"

static void (*g_travel_event_handlers[146])(void) = {
	NULL,
	tevent_001, tevent_002, tevent_003, tevent_004, tevent_005,
	tevent_006, tevent_007, tevent_008, tevent_009, tevent_010,
	tevent_011, tevent_012, tevent_013, tevent_014, tevent_015,
	tevent_016, tevent_017,	tevent_018, tevent_019,	tevent_020,
	tevent_021, tevent_022,	tevent_023, tevent_024,	tevent_025,
	tevent_026, tevent_027,	tevent_028, tevent_029,	tevent_030,
	tevent_031, tevent_032,	tevent_033, tevent_034,	tevent_035,
	tevent_036, tevent_037,	tevent_038, tevent_039,	tevent_040,
	tevent_041, tevent_042, tevent_043, tevent_044,	tevent_045,
	tevent_046, tevent_047,	tevent_048, tevent_049,	tevent_050,
	tevent_051, tevent_052,	tevent_053, tevent_054,	tevent_055,
	tevent_056, tevent_057,	tevent_058, tevent_059,	tevent_060,
	tevent_061, tevent_062, tevent_063, tevent_064,	tevent_065,
	tevent_066, tevent_067,	tevent_068, tevent_069,	tevent_070,
	tevent_071, tevent_072,	tevent_073, tevent_074,	tevent_075,
	tevent_076, tevent_077,	tevent_078, tevent_079,	tevent_080,
	tevent_081, tevent_082,	tevent_083, tevent_084,	tevent_085,
	tevent_086, tevent_087,	tevent_088, tevent_089,	tevent_090,
	tevent_091, tevent_092,	tevent_093, tevent_094,	tevent_095,
	tevent_096, tevent_097,	tevent_098, tevent_099,	tevent_100,
	tevent_101, tevent_102,	tevent_103, tevent_104,	tevent_105,
	tevent_106, tevent_107,	tevent_108, tevent_109,	tevent_110,
	tevent_111, tevent_112,	tevent_113, tevent_114,	tevent_115,
	tevent_116, tevent_117,	tevent_118, tevent_119,	tevent_120,
	tevent_121, tevent_122,	tevent_123, tevent_124,	tevent_125,
	tevent_126, tevent_127,	tevent_128, tevent_129,	tevent_130,
	tevent_131, tevent_132,	tevent_133, tevent_134,	tevent_135,
	tevent_136, tevent_137,	tevent_138, tevent_139,	tevent_140,
	tevent_141, tevent_142,	tevent_143, tevent_144,	tevent_145,
}; // ds:0xaeea,

signed char g_travel_event_active = 0; // ds:0xb132

static signed int g_travel_event_tx2 = -1; // ds:0xb133

#ifndef M302de_ORIGINAL_BUGFIX
/* Original-Bug 33:
 * In certain travel events, an axe is needed as a tool.
 * (These are: bridge building Breida <-> Tjoila, bridge building Auplog <-> Varnheim, path blocked by a tree Skjal <-> Ottarje)
 * The list of accepted axes is a bit arbitrary (The common pattern is that all have "...axt" or "...beil" in their name.
 * Other more or less sensible options would be: ITEM_ID_SKRAJA, ITEM_ID_ORKNASE, ITEM_ID_SCHNEIDZAHN, ITEM_ID_ORKNASE__MAGIC, ITEM_ID_HELLEBARDE. */
uint8_t g_travel_event_axes[6] = {
	ITEM_ID_KRIEGSBEIL__SPECIAL,
	ITEM_ID_STREITAXT,
	ITEM_ID_WURFAXT,
	ITEM_ID_WURFBEIL,
	ITEM_ID_KRIEGSBEIL,
	0xff
}; // ds:0xb135
#else
uint8_t g_travel_event_axes[6] = {
	ITEM_ID_KRIEGSBEIL__SPECIAL,
	ITEM_ID_STREITAXT,
	ITEM_ID_ORKNASE,
	ITEM_ID_ORKNASE__MAGIC,
	ITEM_ID_KRIEGSBEIL,
	0xff
}; // ds:0xb135
/* rationale:
 * - don't allow throwing axes (ITEM_ID_WURFAXT, ITEM_ID_WURFBEIL, ITEM_ID_SCHNEIDZAHN), as they are pretty light and designed for a completely different purpose.
 * - don't allow ITEM_ID_HELLEBARDE, as it is a polearm with a different purpose. (Note that surprisingliy, it is classified as WEAPON_TYPE_AXT in Schicksalsklinge)
 * - don't allow ITEM_ID_SKRAJA, as it is a small axe with a very short shaft, which won't produce too much momentum.
   (Note that while being an axe, ITEM_ID_SKRAJA is classified as WEAPON_TYPE_HIEBWAFFEN in Schicksalsklinge -- which might be a bug on its own.)
 * - allow all other axes in the game.
 * In this way, we get again a list of 5 possible items.
 * See discussion at https://www.crystals-dsa-foren.de/showthread.php?tid=6036&pid=170066#pid170066
 */
#endif

static const char g_empty_string10[1] = ""; // ds:0xb13b
static const char g_empty_string11[1] = ""; // ds:0xb13c
static const char g_empty_string12[1] = ""; // ds:0xb13d

void TRV_load_textfile(signed int travel_event_id)
{
	load_tx(ARCHIVE_FILE_FEATURE_LTX);

	if (travel_event_id == -1) {
		travel_event_id = g_travel_event_tx2;
	}

	load_tx2( (travel_event_id == 37 || travel_event_id == 47 || travel_event_id == 100) ? ARCHIVE_FILE_FEATURE9_LTX :
			( travel_event_id == 16 ? ARCHIVE_FILE_FEATURE8_LTX :
			( travel_event_id < 30 ? ARCHIVE_FILE_FEATURE1_LTX :
			( travel_event_id < 54 ? ARCHIVE_FILE_FEATURE2_LTX :
			( travel_event_id < 70 ? ARCHIVE_FILE_FEATURE3_LTX :
			( travel_event_id < 90 ? ARCHIVE_FILE_FEATURE4_LTX :
			( travel_event_id < 111 ? ARCHIVE_FILE_FEATURE5_LTX :
			( travel_event_id < 126 ? ARCHIVE_FILE_FEATURE6_LTX :
			( travel_event_id < 143 ? ARCHIVE_FILE_FEATURE7_LTX : ARCHIVE_FILE_FEATURE8_LTX)))))))));

	g_travel_event_tx2 = travel_event_id;
}

void TRV_event(const signed int travel_event_id)
{
	signed int tw_bak;
	signed int x_bak;
	signed int y_bak;
	signed int traveling_bak;
	void (*event_handler)(void);

	TRV_load_textfile(travel_event_id);

	tw_bak = g_textbox_width;
	x_bak = g_basepos_x;
	y_bak = g_basepos_y;
	traveling_bak = gs_show_travel_map;

	g_basepos_x = 0;
	g_basepos_y = 0;
	gs_show_travel_map = 0;
	g_textbox_width = 9;
	g_dialogbox_lock = 1;
	g_travel_event_active = 1;

#if !defined(__BORLANDC__)
	D1_INFO("Reisebegegnung %d\n", travel_event_id);
#endif
	event_handler = g_travel_event_handlers[travel_event_id];
	if (event_handler) {
		event_handler();
	}

	g_travel_event_active = 0;
	gs_show_travel_map = traveling_bak;
	g_basepos_x = x_bak;
	g_basepos_y = y_bak;
	g_textbox_width = tw_bak;
	g_dialogbox_lock = 0;
	load_tx(ARCHIVE_FILE_MAPTEXT_LTX);
	g_wallclock_update = 1;
}

/**
 * \brief   executes a fight and load a textfile
 *
 * \param   fight_id    ID of the fight
 * \param   travel_event ID of the travel event
 * \return              return value of the fight
 */
signed int TRV_fight_event(const signed int fight_id, const signed int travel_event_id)
{
	const signed int retval = do_fight(fight_id);

	TRV_load_textfile(travel_event_id);

	return retval;
}

void TRV_found_herb_place(const signed int a0)
{
	signed int answer;
	signed int randval;
	const struct struct_hero *hero = get_first_hero_available_in_group();

	randval = random_schick(5) + 2;

	sprintf(g_dtp2,	get_tx(0), get_tx(randval), hero->alias, (a0 != 0 ? get_tx(42) : g_empty_string10));

	do {
		answer = GUI_radio(g_dtp2, 2, get_tx(1), get_tx(2));

	} while (answer == -1);

	if (answer == 1) {
		TRV_inside_herb_place();
	}
}

void TRV_inside_herb_place(void)
{
	signed int hero_pos;

	load_ani(1);
	draw_main_screen();
	init_ani(0);

	hero_pos = select_hero_ok(get_ttx(326));

	if (hero_pos != -1) {

		const signed int hours = GUI_input(get_ttx(327), 1);

		if (hours > 0) {

			const signed int screen_bak = g_special_screen;

			g_special_screen = 1;

			gather_herbs(get_hero(hero_pos), hours - 1, -3);

			g_special_screen = screen_bak;
		}
	}

	disable_ani();
	g_request_refresh = 1;
}

signed int TRV_found_camp_place(const signed int a0)
{
	signed int answer;
	const signed int randval = random_schick(5) + 10;

	sprintf(g_dtp2, get_tx(8), get_tx(randval),
		(a0 == 1 ? get_tx(42) : (a0 == 2 ? get_tx(45) : g_empty_string11)));

	do {
		answer = GUI_radio(g_dtp2, 2, get_tx(9), get_tx(10));

	} while (answer == -1);

	if (answer == 1) {

		g_wildcamp_sleep_quality = g_replenish_stocks_mod = g_gather_herbs_mod = 0;

		if (a0 == 1) {

			g_replenish_stocks_mod = -3;

		} else if (a0 == 2) {

			g_gather_herbs_mod = -3;
		}

		g_good_camp_place = 1;
		gs_town_loc_type = LOCTYPE_WILDCAMP;

		do_location();

		gs_town_loc_type = g_good_camp_place = LOCTYPE_NONE; /* == 0 */

		TRV_load_textfile(-1);

		g_wildcamp_sleep_quality = g_replenish_stocks_mod = g_gather_herbs_mod = 0;
		g_request_refresh = 2;

		return 1;
	}

	return 0;
}

void TRV_found_replenish_place(const signed int a0)
{
	signed int hero_pos;
	signed int answer;

	sprintf(g_dtp2,	get_tx(16), get_tx(random_schick(5) + 11), (a0 != 0 ? get_tx(45) : g_empty_string12));

	do {
		answer = GUI_radio(g_dtp2, 2, get_tx(21), get_tx(22));

	} while (answer == -1);

	if (answer == 1) {

		load_ani(1);
		draw_main_screen();
		init_ani(0);

		/* Original-Bug: This code assumes that the leader of the group will replenish the stocks,
		 *		 which may not be the case, since replenish_stocks() asks who will do it.
		 *		 To be prepared, clear these arrays for all heroes.
		*/
#ifdef M302de_ORIGINAL_BUGFIX
		for (hero_pos = 0; hero_pos <= 7; hero_pos++) {

			g_wildcamp_replstatus[hero_pos] =
				g_wildcamp_herbstatus[hero_pos] =
				g_wildcamp_magicstatus[hero_pos] =
				g_wildcamp_guardstatus[hero_pos] = 0;
		}
#else
		hero_pos = get_hero_index(get_first_hero_available_in_group());

		g_wildcamp_replstatus[hero_pos] =
			g_wildcamp_herbstatus[hero_pos] =
			g_wildcamp_magicstatus[hero_pos] =
			g_wildcamp_guardstatus[hero_pos] = 0;
#endif

		/* Original-Bug: the second argument is is the counter of replenish tries, not the position of the leader.
		 * Trigger:	Leave the first two hero slots empty and call this function. */
#ifdef M302de_ORIGINAL_BUGFIX
		replenish_stocks(-3, 0);
#else
		replenish_stocks(-3, hero_pos);
#endif

		timewarp(HOURS(1));

		disable_ani();

		g_request_refresh = 1;
	}
}

void TRV_found_inn(const signed int town_id, const signed int type)
/* strange: why does an inn in the countryside need a town_id?
 * This feels like a dirty hack... */
{
	load_ani(12);
	draw_main_screen();
	init_ani(0);

	if (GUI_bool(get_tx(23))) {

		gs_town_locdata = town_id;
		gs_town_typeindex = type;
		gs_town_loc_type = LOCTYPE_INN;

		do_location();

		gs_town_loc_type = LOCTYPE_NONE;

		TRV_load_textfile(-1);
	}

	disable_ani();
	g_request_refresh = 1;
}

/**
 * \brief   question if you want to rest in a hut
 *
 * \return              the answer 0 = no, 1 = yes
 */
signed int TRV_enter_hut_question(void)
{
	signed int answer;

	load_ani(8);
	draw_main_screen();
	init_ani(0);

	answer = GUI_bool(get_tx(24));

	disable_ani();
	g_request_refresh = 1;

	return answer;
}

signed int TRV_follow_trail_question(void)
{
	signed int answer;
	const struct struct_hero *hero = get_first_hero_available_in_group();

	sprintf(g_dtp2, get_tx(25), hero->alias);

	do {
		answer = GUI_radio(g_dtp2, 2, get_tx(26), get_tx(27));

	} while (answer == -1);

	return answer - 1;
}

signed int TRV_cross_a_ford(char *msg, const signed int time, const signed int handicap)
{
	signed int answer;
	signed int done = 0;

	g_event_ani_busy = 1;

	load_ani(7);
	draw_main_screen();
	init_ani(0);

	do {
		sprintf(g_dtp2,	get_tx(34), (char*)msg);

		do {
			answer = GUI_radio(g_dtp2, 2, get_tx(35), get_tx(36));

		} while (answer == -1);

		if (answer == 1) {

			done = 1;
			TRV_ford_test(handicap, time);

		} else {
			answer = GUI_bool(get_tx(39));

			if (answer == 1) {
#ifndef M302de_ORIGINAL_BUGFIX
				/* Original-Bug 53:
				 * If the party was already traveling backward, the new direction should be forward. */
				done = gs_journey_direction = JOURNEY_DIRECTION_CHANGE_TO_BACKWARD; // this is value 1
#else
				gs_journey_direction = (gs_journey_direction == JOURNEY_DIRECTION_FORWARD ? JOURNEY_DIRECTION_CHANGE_TO_BACKWARD : JOURNEY_DIRECTION_CHANGE_TO_FORWARD);
				done = 1;
#endif
			}
		}

	} while (!done);

	disable_ani();
	g_event_ani_busy = 0;
	g_request_refresh = 1;

	return 1;
}

void TRV_ford_test(const signed int handicap, const signed int time)
{
	signed int i;
	struct struct_hero *hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.dead)
		{
			/* Original-Bug 47: When crossing a ford, the result of the GE test is mis-evaluated, resulting in these success rates:
			 * For (GE - handicap) <= 19: 95% success.
			 * For (GE - handicap) >= 20: 100% success.
			 * The intended behaviour probably was a usual GE-test with the given handicap. */
#ifndef M302de_ORIGINAL_BUGFIX
			if (test_attrib(hero, ATTRIB_GE, handicap) == 0)
#else
			if (test_attrib(hero, ATTRIB_GE, handicap) < 0)
#endif
			{
				/* test failed */
				timewarp(MINUTES(time));

				sprintf(g_dtp2, get_tx(37), hero->alias);

				hero_disease_test(hero, DISEASE_ID_DUMPFSCHAEDEL, 20 - (hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod));

				loose_random_item(hero, 1, get_ttx(506));
			} else {
				/* test succeded */
				timewarp(MINUTES(10));

				sprintf(g_dtp2,	get_tx(38), hero->alias);
			}

			GUI_output(g_dtp2);
		}
	}
}

signed int TRV_ferry(char *msg, signed int price)
{
	signed int done = 0;
	signed int answer;
	const signed int heroes_num = count_heroes_in_group();
	uint32_t p_money;

	do {

		sprintf(g_dtp2, get_tx(28), (char*)msg, price, 5 * price);

		do {
			answer = GUI_radio(g_dtp2, 3, get_tx(29), get_tx(30), get_tx(31));

		} while (answer == -1);

		if (answer == 1) {

			/* Original-Bug: If party doesn't have enough money, the price will stay at *5*heroes_num (typically *30) afterwards... */
			price *= 5 * heroes_num;
			p_money = get_party_money();

			if ((uint32_t)price > p_money) {
				GUI_output(get_ttx(401));
			} else {
				done = 1;
				p_money -= price;
				set_party_money(p_money);
				timewarp(MINUTES(30));
			}

		} else if (answer == 2) {

			price *= heroes_num;
			p_money = get_party_money();

			if ((uint32_t)price > p_money) {
				GUI_output(get_ttx(401));
			} else {
				p_money -= price;
				set_party_money(p_money);

				sprintf(g_dtp2,	get_tx(32), (done = random_schick(6)));

				GUI_output(g_dtp2);

				timewarp(HOURS(done));

				done = 1;
			}
		} else {

			if (GUI_bool(get_tx(33))) {
				/* Original-Bug? What if the ferry was entered in backward direction, is this possible? */
				gs_journey_direction = done = JOURNEY_DIRECTION_CHANGE_TO_BACKWARD; // this is value 1
			}
		}

	} while (!done);

	return 1;
}

/* Vaermhag <-> Varnheim: inn "Golfblick" */
void tevent_001(void)
{
	TRV_found_inn(TOWN_ID_DASPOTA, 65);
}

/* Varnheim <-> Daspota: hunter */
void tevent_002(void)
{
	signed int answer;

	/* load dialog head */
	load_in_head(10);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(0), 3,
				get_tx2(1), get_tx2(2), get_tx2(3));

	} while (answer == -1);

	/* talk to the hunter for 15 min */
	if (answer == 1 || answer == 2) {

		GUI_dialog_na(0, answer == 1 ? get_tx2(4) : get_tx2(5));

		timewarp(MINUTES(15));
	}
}

void tevent_003(void)
{
	TRV_ferry(get_tx(69), 5);
}

void tevent_004(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 2) > 0 && !gs_tevent004_flag) ||
		gs_tevent004_flag)
	{
		gs_tevent004_flag = 1;

		if (!TRV_follow_trail_question()) {
			TRV_hunt_generic(29, 6, -2, 4, 10, 8, 7, 5, 7, 80, 0);
		}
	}
}

void TRV_hunt_generic(const signed int ani_id, const signed int city_index,
		const signed int handicap_sneak_1, const signed int handicap_sneak_2, const signed int handicap_ranged_weapon,
		const signed int ap_all1, const signed int ap_hero, const signed int ap_all2,
		const signed int ap_all3, const signed int foods1, const signed int foods2)
{
	signed int failed_sneaks_num;
	signed int i;
	signed int l4;
	signed int answer;
	struct struct_hero *hero;

	g_event_ani_busy = 1;

	load_ani(ani_id);
	draw_main_screen();
	init_ani(0);

	GUI_output(get_tx2(city_index));

	hero = get_hero(0);
	for (i = failed_sneaks_num = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
			!hero->flags.dead && (test_talent(hero, TA_SCHLEICHEN, (signed char)handicap_sneak_1) <= 0))
		{
			failed_sneaks_num++;
		}
	}

	if (failed_sneaks_num) {

		do {
			answer = GUI_radio(get_tx2(city_index + 1), 2, get_tx2(city_index + 7), get_tx2(city_index + 8));

		} while (answer == -1);

	} else {

		i = select_hero_ok_forced(get_tx2(city_index + 2));

		hero = get_hero(i);

		if (test_talent(hero, TA_SCHLEICHEN, (signed char)handicap_sneak_2) <= 0) {

			do {
				answer = GUI_radio(get_tx2(city_index + 1), 2, get_tx2(city_index + 7), get_tx2(city_index + 8));

			} while (answer == -1);

		} else {

			sprintf(g_dtp2,	get_tx2(city_index + 3), hero->alias);
			GUI_input(g_dtp2, failed_sneaks_num = 0);

			if ((i = test_talent(hero, TA_SCHUSSWAFFEN, (signed char)handicap_ranged_weapon)) > 0) {
				failed_sneaks_num++;
			}

			if ((l4 = test_talent(hero, TA_SCHUSSWAFFEN, (signed char)handicap_ranged_weapon)) > 0) {
				failed_sneaks_num++;
			}

			if (failed_sneaks_num == 2 || i == 99 || l4 == 99) {

				sprintf(g_dtp2, get_tx2(city_index + 4), hero->alias);
				GUI_output(g_dtp2);

				add_hero_ap_all(ap_all1);
				add_hero_ap(hero, ap_hero);

				timewarp(HOURS(1));

				give_new_item_to_group(ITEM_ID_PROVIANTPAKET, 1, foods1);

				answer = 0;
			} else {

				sprintf(g_dtp2,	get_tx2(failed_sneaks_num == 1 ? city_index + 5 : city_index + 6), hero->alias);

				do {
					answer = GUI_radio(g_dtp2, 2, get_tx2(city_index + 7), get_tx2(city_index + 8));

				} while (answer == -1);
			}
		}
	}

	if (answer == 2) {

		add_hero_ap_all(ap_all2);

		if (foods2 != 0) {
			give_new_item_to_group(ITEM_ID_PROVIANTPAKET, 1, foods2);
		}

	} else if (answer == 1) {

		GUI_output(get_tx2(city_index + 9));

		timewarp(HOURS(2));

		add_hero_ap_all(ap_all3);
	}

	disable_ani();
	g_event_ani_busy = 0;
	g_request_refresh = 1;
}

void tevent_005(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 0) > 0 && !gs_tevent005_flag) ||
		gs_tevent005_flag)
	{
		TRV_found_camp_place(1);

		gs_tevent005_flag = 1;
	}
}

void tevent_006(void)
	/* Tjeula <-> Faehrstation Tjeula: ferry */
{
	TRV_ferry(get_tx(69), 4);
}

void tevent_007(void)
/* Tjeula <-> Breida: bridge building */
{
	const signed int season = get_current_season();

	if (season == SEASON_AUTUMN || season == SEASON_SPRING) {
		TRV_barrier(16);
	}
}

void TRV_barrier(const signed int text_start)
{
	signed int i;
	signed int tools_num;
	signed int answer;
	signed int done = 0;
	struct struct_hero *hero;

	load_in_head(54);

	do {

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(text_start + 0), 2,
						get_tx2(text_start + 1), get_tx2(text_start + 2));

		} while (answer == -1);

		if (answer == 1) {

			timewarp(HOURS(2));

			GUI_dialog_na(0, get_tx2(text_start + 3));

			done = 1;
		} else {

			/* count ropes and rope ladders in the active group */
			hero = get_hero(0);
			for (i = tools_num = 0; i <= 6; i++, hero++) {

				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id))
				{
					tools_num += hero_count_item(hero, ITEM_ID_SEIL);
					tools_num += hero_count_item(hero, ITEM_ID_STRICKLEITER);
				}
			}

			if (tools_num >= 2) {
				/* at least 2 ropes or rope ladders: enough */

				i = 0;
				tools_num = 1;

				do {
					/* look for a suitable axe */
					if (get_first_hero_with_item(g_travel_event_axes[i]) != -1)
					{
						tools_num = 0;
					}

					i++;

				} while (tools_num && g_travel_event_axes[i] != 0xff);

				if (tools_num || get_first_hero_with_item(ITEM_ID_HAMMER) == -1) {

					GUI_dialog_na(0, get_tx2(text_start + 4));

				} else {

					hero = get_hero(0);

					for (i = tools_num = 0; i <= 6; i++, hero++)
					{
						if (test_talent(hero, TA_WILDNISLEBEN, 0) > 0) {
							tools_num++;
						}
					}

					add_hero_ap_all(10);

					i = inv_slot_of_item((hero = get_hero(get_first_hero_with_item(ITEM_ID_SEIL))), ITEM_ID_SEIL);
					if (i == -1) {
						i = inv_slot_of_item((hero = get_hero(get_first_hero_with_item(ITEM_ID_STRICKLEITER))), ITEM_ID_STRICKLEITER);
					}
					drop_item(hero, i, 1);

					i = inv_slot_of_item((hero = get_hero(get_first_hero_with_item(ITEM_ID_SEIL))), ITEM_ID_SEIL);
					if (i == -1) {
						i = inv_slot_of_item((hero = get_hero(get_first_hero_with_item(ITEM_ID_STRICKLEITER))), ITEM_ID_STRICKLEITER);
					}
					drop_item(hero, i, 1);

					if (tools_num >= 3) {

						GUI_dialog_na(0, get_tx2(text_start + 6));

					} else {

						hero = get_hero(select_hero_ok_forced(get_tx2(text_start + 5)));

						sprintf(g_dtp2 + 0x400,	get_tx2(text_start + 7), hero->alias,
								GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_3RD), GUI_get_personal_pronoun(hero->sex, GRAMMAR_CASE_4TH));
						GUI_dialog_na(0, (char*)(g_dtp2 + 0x400));

						hero_disease_test(hero, DISEASE_ID_DUMPFSCHAEDEL, 30);

						loose_random_item(hero, 30, get_ttx(506));
					}
					done = 1;
				}
			} else {
				/* not enough */
				GUI_dialog_na(0, get_tx2(text_start + 4));
			}
		}

	} while (done == 0);
}

/* Tjeula <-> Breida: kleiner Bachlauf */
void tevent_008(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent008_flag) ||
		gs_tevent008_flag)
	{
		TRV_found_replenish_place(0);
		gs_tevent008_flag = 1;
	}
}

void tevent_009(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 4) > 0 && !gs_tevent009_flag) ||	gs_tevent009_flag)
	{
		g_gather_herbs_special = ITEM_ID_EINBEERE;

		TRV_found_herb_place(0);

		g_gather_herbs_special = -1;

		gs_tevent009_flag = 1;
	}
}

void tevent_010(void)
{
	TRV_found_inn(TOWN_ID_VARNHEIM, 66);
}

void tevent_012(void)
{
	TRV_cross_a_ford(get_tx(53), 30, 0);
}

void tevent_018(void)
{
	TRV_cross_a_ford(get_tx(53), 20, 0);
}

void tevent_019(void)
{
	TRV_cross_a_ford(get_tx(70), 20, 0);
}

void tevent_023(void)
{
	TRV_ferry(get_tx(69), 3);
}

void tevent_092(void)
{
	do_talk(0, 0);
}
