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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void TRV_load_textfile(signed short travel_event)
{
	load_tx(ARCHIVE_FILE_FEATURE_LTX);

	if (travel_event == -1) {
		travel_event = ds_readws(TRAVEL_EVENT_TX2);
	}

	load_tx2( (travel_event == 37 || travel_event == 47 || travel_event == 100) ? ARCHIVE_FILE_FEATURE9_LTX :
			( travel_event == 16 ? ARCHIVE_FILE_FEATURE8_LTX :
			( travel_event < 30 ? ARCHIVE_FILE_FEATURE1_LTX :
			( travel_event < 54 ? ARCHIVE_FILE_FEATURE2_LTX :
			( travel_event < 70 ? ARCHIVE_FILE_FEATURE3_LTX :
			( travel_event < 90 ? ARCHIVE_FILE_FEATURE4_LTX :
			( travel_event < 111 ? ARCHIVE_FILE_FEATURE5_LTX :
			( travel_event < 126 ? ARCHIVE_FILE_FEATURE6_LTX :
			( travel_event < 143 ? ARCHIVE_FILE_FEATURE7_LTX : ARCHIVE_FILE_FEATURE8_LTX)))))))));

	ds_writews(TRAVEL_EVENT_TX2, travel_event);
}

void TRV_event(signed short travel_event)
{
	signed short tw_bak;
	signed short bak1;
	signed short bak2;
	signed short traveling_bak;
	void (*event_handler)(void);

	TRV_load_textfile(travel_event);

	tw_bak = ds_readws(TEXTBOX_WIDTH);
	bak1 = ds_readws(BASEPOS_X);
	bak2 = ds_readws(BASEPOS_Y);
	traveling_bak = ds_readb(SHOW_TRAVEL_MAP);
	ds_writews(BASEPOS_X, 0);
	ds_writews(BASEPOS_Y, 0);
	ds_writeb(SHOW_TRAVEL_MAP, 0);
	ds_writews(TEXTBOX_WIDTH, 9);
	ds_writeb(DIALOGBOX_LOCK, 1);
	ds_writeb(TRAVEL_EVENT_ACTIVE, 1);

#if !defined(__BORLANDC__)
	D1_INFO("Reisebegegnung %d\n", travel_event);
#endif
	event_handler = (void (*)(void))g_travel_event_handlers[travel_event];
	if (event_handler) event_handler();

	ds_writeb(TRAVEL_EVENT_ACTIVE, 0);
	ds_writeb(SHOW_TRAVEL_MAP, (signed char)traveling_bak);
	ds_writews(BASEPOS_X, bak1);
	ds_writews(BASEPOS_Y, bak2);
	ds_writews(TEXTBOX_WIDTH, tw_bak);
	ds_writeb(DIALOGBOX_LOCK, 0);
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
signed short TRV_fight_event(signed short fight_id, signed short travel_event)
{
	signed short retval;

	retval = do_fight(fight_id);
	TRV_load_textfile(travel_event);

	return retval;
}

void TRV_found_herb_place(signed short a0)
{
	signed short answer;
	signed short randval;
	Bit8u *hero;

	hero = (Bit8u*)get_first_hero_available_in_group();

	randval = random_schick(5) + 2;

	sprintf((char*)ds_readd(DTP2),
		get_tx(0),
		get_tx(randval),
		(char*)hero + HERO_NAME2,
		(a0 != 0 ? get_tx(42) : (char*)p_datseg + EMPTY_STRING10));

	do {
		answer = GUI_radio((char*)ds_readd(DTP2), 2,
				get_tx(1),
				get_tx(2));

	} while (answer == -1);

	if (answer == 1) {
		TRV_inside_herb_place();
	}
}

void TRV_inside_herb_place(void)
{
	signed short hero_pos;
	signed short hours;
	signed short bak;

	load_ani(1);
	draw_main_screen();
	init_ani(0);
	hero_pos = select_hero_ok(get_ttx(326));

	if (hero_pos != -1) {

		hours = GUI_input(get_ttx(327), 1);

		if (hours > 0) {
			bak = ds_readbs(SPECIAL_SCREEN);
			ds_writebs(SPECIAL_SCREEN, 1);

			gather_herbs(get_hero(hero_pos), hours - 1, -3);

			ds_writebs(SPECIAL_SCREEN, (signed char)bak);
		}
	}

	set_var_to_zero();
	ds_writew(REQUEST_REFRESH, 1);
}

signed short TRV_found_camp_place(signed short a0)
{
	signed short answer;
	signed short randval;

	randval = random_schick(5) + 10;
	sprintf((char*)ds_readd(DTP2),
		get_tx(8),
		get_tx(randval),
		(a0 == 1 ? get_tx(42) : (a0 == 2 ? get_tx(45) : (char*)p_datseg + EMPTY_STRING11)));
	do {
		answer = GUI_radio((char*)ds_readd(DTP2), 2,
					get_tx(9),
					get_tx(10));
	} while (answer == -1);

	if (answer == 1) {

		ds_writew(WILDCAMP_SLEEP_QUALITY, ds_writews(REPLENISH_STOCKS_MOD, ds_writews(GATHER_HERBS_MOD, 0)));

		if (a0 == 1) {
			ds_writews(REPLENISH_STOCKS_MOD, -3);
		} else if (a0 == 2) {
			ds_writews(GATHER_HERBS_MOD, -3);
		}

		ds_writeb(GOOD_CAMP_PLACE, 1);
		ds_writeb(CURRENT_LOCTYPE, LOCTYPE_WILDCAMP);

		do_location();

		ds_writeb(CURRENT_LOCTYPE, ds_writeb(GOOD_CAMP_PLACE, 0));

		TRV_load_textfile(-1);

		ds_writew(WILDCAMP_SLEEP_QUALITY, ds_writews(REPLENISH_STOCKS_MOD, ds_writews(GATHER_HERBS_MOD, 0)));
		ds_writew(REQUEST_REFRESH, 2);

		return 1;
	}

	return 0;
}

void TRV_found_replenish_place(signed short a0)
{
	signed short hero_pos;
	signed short answer;

	sprintf((char*)ds_readd(DTP2),
		get_tx(16),
		get_tx(random_schick(5) + 11),
		(a0 != 0 ? get_tx(45) : (char*)p_datseg + EMPTY_STRING12));
	do {
		answer = GUI_radio((char*)ds_readd(DTP2), 2,
					get_tx(21),
					get_tx(22));
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
			ds_writeb(WILDCAMP_REPLSTATUS + hero_pos,
				ds_writeb(WILDCAMP_HERBSTATUS + hero_pos,
				ds_writeb(WILDCAMP_MAGICSTATUS + hero_pos,
				ds_writeb(WILDCAMP_GUARDSTATUS + hero_pos, 0))));
		}
#else
		hero_pos = get_hero_index((Bit8u*)get_first_hero_available_in_group());

		ds_writeb(WILDCAMP_REPLSTATUS + hero_pos, ds_writeb(WILDCAMP_HERBSTATUS + hero_pos, ds_writeb(WILDCAMP_MAGICSTATUS + hero_pos, ds_writeb(WILDCAMP_GUARDSTATUS + hero_pos, 0))));
#endif

		/* Original-Bug: the second argument is is the counter of replenish tries, not the position of the leader.
		 * Trigger:	Leave the first two hero slots empty and call this function. */
#ifdef M302de_ORIGINAL_BUGFIX
		replenish_stocks(-3, 0);
#else
		replenish_stocks(-3, hero_pos);
#endif

		timewarp(HOURS(1));

		set_var_to_zero();

		ds_writew(REQUEST_REFRESH, 1);
	}
}

void TRV_found_inn(signed short town_id, signed short type)
/* strange: why does an inn in the countryside need a town_id?
 * This feels like a dirty hack... */
{
	load_ani(12);
	draw_main_screen();
	init_ani(0);

	if (GUI_bool(get_tx(23))) {
		ds_writew(CURRENT_LOCDATA, town_id);
		ds_writew(CURRENT_TYPEINDEX, type);
		ds_writeb(CURRENT_LOCTYPE, LOCTYPE_INN);

		do_location();

		ds_writeb(CURRENT_LOCTYPE, LOCTYPE_NONE);

		TRV_load_textfile(-1);
	}

	set_var_to_zero();
	ds_writew(REQUEST_REFRESH, 1);
}

/**
 * \brief   question if you want to rest in a hut
 *
 * \return              the answer 0 = no, 1 = yes
 */
signed short TRV_enter_hut_question(void)
{
	signed short answer;

	load_ani(8);
	draw_main_screen();
	init_ani(0);

	answer = GUI_bool(get_tx(24));

	set_var_to_zero();
	ds_writew(REQUEST_REFRESH, 1);

	return answer;
}

signed short TRV_follow_trail_question(void)
{
	signed short answer;
	Bit8u *hero;

	hero = (Bit8u*)get_first_hero_available_in_group();

	sprintf((char*)ds_readd(DTP2),
		get_tx(25),
		(char*)hero + HERO_NAME2);

	do {
		answer = GUI_radio((char*)ds_readd(DTP2), 2,
				get_tx(26),
				get_tx(27));

	} while (answer == -1);

	return answer - 1;
}

signed short TRV_cross_a_ford(char *msg, signed short time, signed short mod)
{
	signed short answer;
	signed short done;

	done = 0;
	g_event_ani_busy = 1;

	load_ani(7);
	draw_main_screen();
	init_ani(0);

	do {
		sprintf((char*)ds_readd(DTP2),
			get_tx(34),
			(char*)msg);

		do {
			answer = GUI_radio((char*)ds_readd(DTP2), 2,
						get_tx(35),
						get_tx(36));
		} while (answer == -1);

		if (answer == 1) {
			done = 1;
			TRV_ford_test(mod, time);
		} else {
			answer = GUI_bool(get_tx(39));

			if (answer == 1) {
				done = ds_writew(TRV_RETURN, 1);
			}
		}

	} while (!done);

	set_var_to_zero();
	g_event_ani_busy = 0;
	ds_writew(REQUEST_REFRESH, 1);
	return 1;
}

void TRV_ford_test(signed short mod, signed short time)
{
	signed short i;
	Bit8u *hero;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP) &&
			!hero_dead(hero))
		{
			/* Original-Bugfix: tests fail if their result is lower or equal than zero */
#ifdef M302de_ORIGINAL_BUGFIX
			if (test_attrib(hero, ATTRIB_GE, mod) < 0)
#else
			if (test_attrib(hero, ATTRIB_GE, mod) == 0)
#endif
			{
				/* test failed */
				timewarp(MINUTES(time));

				sprintf((char*)ds_readd(DTP2),
					get_tx(37),
					(char*)hero + HERO_NAME2);

				hero_disease_test(hero, 2, 20 - (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))));

				loose_random_item(hero, 1, get_ttx(506));
			} else {
				/* test succeded */
				timewarp(MINUTES(10));

				sprintf((char*)ds_readd(DTP2),
					get_tx(38),
					(char*)hero + HERO_NAME2);
			}

			GUI_output((char*)ds_readd(DTP2));
		}
	}
}

signed short TRV_ferry(char *msg, signed short price)
{
	signed short done;
	signed short answer;
	signed short nr_heroes;
	Bit32u p_money;

	done = 0;
	nr_heroes = count_heroes_in_group();

	do {

		sprintf((char*)ds_readd(DTP2),
			get_tx(28),
			(char*)msg,
			price, 5 * price);

		do {
			answer = GUI_radio((char*)ds_readd(DTP2), 3,
						get_tx(29),
						get_tx(30),
						get_tx(31));
		} while (answer == -1);

		if (answer == 1) {

			/* Original-Bug: If party doesn't have enough money, the price will stay at *5*nr_heroes (typically *30) afterwards... */
			price *= 5 * nr_heroes;
			p_money = get_party_money();

			if ((Bit32u)price > p_money) {
				GUI_output(get_ttx(401));
			} else {
				done = 1;
				p_money -= price;
				set_party_money(p_money);
				timewarp(MINUTES(30));
			}

		} else if (answer == 2) {

			price *= nr_heroes;
			p_money = get_party_money();

			if ((Bit32u)price > p_money) {
				GUI_output(get_ttx(401));
			} else {
				p_money -= price;
				set_party_money(p_money);

				sprintf((char*)ds_readd(DTP2),
					get_tx(32),
					done = random_schick(6));

				GUI_output((char*)ds_readd(DTP2));

				timewarp(HOURS(done));

				done = 1;
			}
		} else {

			if (GUI_bool(get_tx(33))) {
				ds_writew(TRV_RETURN, done = 1);
			}
		}

	} while (!done);

	return 1;
}

/* Vaermhag <-> Varnheim: inn "Golfblick" */
void tevent_001(void)
{
	TRV_found_inn(TOWNS_DASPOTA, 65);
}

/* Varnheim <-> Daspota: hunter */
void tevent_002(void)
{
	signed short answer;

	/* load dialog head */
	load_in_head(10);

	/* show dialogbox */
	do {
		answer = GUI_dialogbox((unsigned char*)ds_readd(DTP2),
				NULL,
				get_tx2(0),
				3,
				get_tx2(1),
				get_tx2(2),
				get_tx2(3));

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
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 2) > 0 && !ds_readb(TEVENT004_FLAG)) ||
		ds_readb(TEVENT004_FLAG) != 0)
	{
		ds_writeb(TEVENT004_FLAG, 1);
		if (!TRV_follow_trail_question()) {
			TRV_hunt_generic(29, 6, -2, 4, 10, 8, 7, 5, 7, 80, 0);
		}
	}
}

void TRV_hunt_generic(signed short ani_id, signed short city_index, signed short mod1, signed short mod2,
			signed short mod3, signed short ap_all1, signed short ap_hero, signed short ap_all2,
			signed short ap_all3, signed short foods1, signed short foods2)
{
	signed short l_di;
	signed short i;
	signed short l4;
	signed short answer;
	Bit8u *hero;

	g_event_ani_busy = 1;

	load_ani(ani_id);
	draw_main_screen();
	init_ani(0);

	GUI_output(get_tx2(city_index));

	hero = get_hero(0);
	for (i = l_di = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP)) &&
			!hero_dead(hero) &&
			test_skill(hero, TA_SCHLEICHEN, (signed char)mod1) <= 0)
		{
			l_di++;
		}
	}

	if (l_di) {

		do {
			answer = GUI_radio(get_tx2(city_index + 1), 2,
						get_tx2(city_index + 7),
						get_tx2(city_index + 8));
		} while (answer == -1);

	} else {

		i = select_hero_ok_forced(get_tx2(city_index + 2));

		hero = get_hero(i);

		if (test_skill(hero, TA_SCHLEICHEN, (signed char)mod2) <= 0) {

			do {
				answer = GUI_radio(get_tx2(city_index + 1), 2,
							get_tx2(city_index + 7),
							get_tx2(city_index + 8));
			} while (answer == -1);
		} else {

			sprintf((char*)ds_readd(DTP2),
				get_tx2(city_index + 3),
				(char*)hero + HERO_NAME2);

			GUI_input((char*)ds_readd(DTP2), l_di = 0);

			if ((i = test_skill(hero, TA_SCHUSSWAFFEN, (signed char)mod3)) > 0) {
				l_di++;
			}

			if ((l4 = test_skill(hero, TA_SCHUSSWAFFEN, (signed char)mod3)) > 0) {
				l_di++;
			}

			if (l_di == 2 || i == 99 || l4 == 99) {

				sprintf((char*)ds_readd(DTP2),
					get_tx2(city_index + 4),
					(char*)hero + HERO_NAME2);

				GUI_output((char*)ds_readd(DTP2));

				add_hero_ap_all(ap_all1);
				add_hero_ap(hero, ap_hero);

				timewarp(HOURS(1));

				get_item(ITEM_FOOD_PACKAGE, 1, foods1);

				answer = 0;
			} else {

				sprintf((char*)ds_readd(DTP2),
					get_tx2(l_di == 1 ? city_index + 5 : city_index + 6),
					(char*)hero + HERO_NAME2);

				do {
					answer = GUI_radio((char*)ds_readd(DTP2), 2,
								get_tx2(city_index + 7),
								get_tx2(city_index + 8));
				} while (answer == -1);
			}
		}
	}

	if (answer == 2) {

		add_hero_ap_all(ap_all2);

		if (foods2 != 0) {
			get_item(ITEM_FOOD_PACKAGE, 1, foods2);
		}

	} else if (answer == 1) {

		GUI_output(get_tx2(city_index + 9));

		timewarp(HOURS(2));

		add_hero_ap_all(ap_all3);
	}

	set_var_to_zero();
	g_event_ani_busy = 0;
	ds_writew(REQUEST_REFRESH, 1);
}

void tevent_005(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 0) > 0 && !ds_readb(TEVENT005_FLAG)) ||
		ds_readb(TEVENT005_FLAG) != 0)
	{
		TRV_found_camp_place(1);
		ds_writeb(TEVENT005_FLAG, 1);
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
	signed short season = get_current_season();

	if (season == SEASON_AUTUMN || season == SEASON_SPRING) {
		TRV_barrier(16);
	}
}

void TRV_barrier(signed short text_start)
{
	signed short i;
	signed short l_di;
	signed short answer;
	signed short done;
	Bit8u *hero;

	done = 0;

	load_in_head(54);

	do {

		do {
			answer = GUI_dialogbox((unsigned char*)ds_readd(DTP2), NULL,
						get_tx2(text_start + 0), 2,
						get_tx2(text_start + 1),
						get_tx2(text_start + 2));
		} while (answer == -1);

		if (answer == 1) {

			timewarp(HOURS(2));

			GUI_dialog_na(0, get_tx2(text_start + 3));

			done = 1;
		} else {

			/* count ropes and rope ladders in the active group */
			hero = get_hero(0);
			for (i = l_di = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == ds_readbs(CURRENT_GROUP))
				{
					l_di+=hero_count_item(hero, ITEM_ROPE);
					l_di+=hero_count_item(hero, ITEM_ROPE_LADDER);
				}
			}

			if (l_di >= 2) {
				/* at least 2 ropes or rope ladders: enough */

				i = 0;
				l_di = 1;

				do {
					/* look for a suitable axe */
					if (get_first_hero_with_item(ds_readb(TRAVEL_EVENT_AXES + i)) != -1)
					{
						l_di = 0;
					}

					i++;

				} while (l_di && ds_readbs(TRAVEL_EVENT_AXES + i) != -1);

				if
					(l_di || get_first_hero_with_item(ITEM_HAMMER) == -1)
				{

					GUI_dialog_na(0, get_tx2(text_start + 4));

				} else {
					hero = get_hero(0);

					for (i = l_di = 0; i <= 6; i++, hero += SIZEOF_HERO)
					{
						if (test_skill(hero, TA_WILDNISLEBEN, 0) > 0) l_di++;
					}

					add_hero_ap_all(10);

					i = get_item_pos(hero = get_hero(get_first_hero_with_item(ITEM_ROPE)), ITEM_ROPE);
					if (i == -1) {
						i = get_item_pos(hero = get_hero(get_first_hero_with_item(ITEM_ROPE_LADDER)), ITEM_ROPE_LADDER);
					}
					drop_item(hero, i, 1);

					i = get_item_pos(hero = get_hero(get_first_hero_with_item(ITEM_ROPE)), ITEM_ROPE);
					if (i == -1) {
						i = get_item_pos(hero = get_hero(get_first_hero_with_item(ITEM_ROPE_LADDER)), ITEM_ROPE_LADDER);
					}
					drop_item(hero, i, 1);

					if (l_di >= 3) {

						GUI_dialog_na(0, get_tx2(text_start + 6));
					} else {

						hero = get_hero(select_hero_ok_forced(get_tx2(text_start + 5)));

						sprintf((char*)ds_readd(DTP2) + 0x400,
								get_tx2(text_start + 7),
								(char*)hero + HERO_NAME2,
								(GUI_get_ptr(host_readbs(hero + HERO_SEX), 3)),
								(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));
						GUI_dialog_na(0, (char*)ds_readd(DTP2) + 0x400);

						hero_disease_test(hero, 2, 30);

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
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT008_FLAG)) ||
		ds_readb(TEVENT008_FLAG) != 0)
	{
		TRV_found_replenish_place(0);
		ds_writeb(TEVENT008_FLAG, 1);
	}
}

void tevent_009(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 4) > 0 && !ds_readb(TEVENT009_FLAG)) ||
		ds_readb(TEVENT009_FLAG) != 0)
	{
		ds_writeb(GATHER_HERBS_SPECIAL, 60);
		TRV_found_herb_place(0);
		ds_writeb(GATHER_HERBS_SPECIAL, 255);
		ds_writeb(TEVENT009_FLAG, 1);
	}
}

void tevent_010(void)
{
	TRV_found_inn(TOWNS_VARNHEIM, 66);
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

#if !defined(__BORLANDC__)
}
#endif
