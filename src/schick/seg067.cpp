/**
 *	Rewrite of DSA1 v3.02_de functions of seg067 (cityevents, waffinfo)
 *	Functions rewritten: 13/13 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg067.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg031.h"
#include "seg055.h"
#include "seg067.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif
signed char g_c_event_active = 0; // ds:0x7c42, 0 = default, 1 = city event active

/**
 * \brief   select a random city event type
 */
void city_event_switch(void)
{
	/* load STRASSE.LTX */
	load_tx(ARCHIVE_FILE_STRASSE_LTX);

	/* set city flag */
	g_c_event_active = 1;

	switch (random_schick(9)) {
		case 1: city_event_1(); break;
		case 2: city_event_2(); break;
		case 3: city_event_3(); break;
		case 4: city_event_4(); break;
		case 5: city_event_5(); break;
		case 6: city_event_6(); break;
		case 7: city_event_7(); break;
		case 8: city_event_8(); break;
		case 9: city_event_9(); break;
	}

	/* reset city flag */
	g_c_event_active = 0;

	/* load the LTX-file of the current town */
	load_tx(gs_town_id + 77);

	/* update the current position / make the step */
	gs_x_target = gs_x_target_bak;
	gs_y_target = gs_y_target_bak;
}

/**
 * \brief   random city event 1: a hero may loose W20 S to a pickpocket
 */
void city_event_1(void)
{
	signed int randval;
	signed int answer;
	struct struct_hero *hero;

	hero = get_hero(get_random_hero());

	randval = random_schick(20);

	if (test_skill(hero, TA_SINNESSCHAERFE, 2) <= 0) {

		/* TODO:	check the format strings in the english version
		 *		they may be broken.
		 *		Or, at least some changes on the code are neccessary.
		 */
		sprintf(g_dtp2, get_tx(random_schick(4) - 1), hero->alias, randval);

		GUI_dialogbox(hero->pic, hero->alias, g_dtp2, 0);

		randval *= 10;

		hero->money -= randval;

		if (hero->money < 0) {
			hero->money = 0;
		}
	} else {

		sprintf(g_dtp2, get_tx(random_schick(4) + 3), hero->alias, GUI_get_ptr(hero->sex, 1));

		answer = GUI_dialogbox(hero->pic, hero->alias, g_dtp2, 3,
				get_tx(random_schick(4) + 7),
				get_tx(random_schick(4) + 11),
				get_tx(random_schick(4) + 15));

		if (answer == 1) {

			GUI_output(get_tx(random_schick(4) + 19));

		} else {

			randval = random_schick(5) - 1;

			if (randval == 2) {
				GUI_output(get_tx(26));
				GUI_output(get_tx(27));
				GUI_output(get_tx(28));
			} else {
				sprintf(g_dtp2,	get_tx(randval + 24), hero->alias);
				GUI_output(g_dtp2);
			}
		}
	}
}

/**
 * \brief   random city event 2: a hero may loose all money to a pickpocket
 */
void city_event_2(void)
{
	signed int answer;
	struct struct_hero *hero;

	hero = get_hero(get_random_hero());

	if (test_skill(hero, TA_SINNESSCHAERFE, 2) <= 0) {

		/* hero looses all money */
		hero->money = 0;

		sprintf(g_dtp2, get_tx(random_schick(4) + 30), hero->alias);

		GUI_dialogbox(hero->pic, hero->alias, g_dtp2, 0);

	} else {

		sprintf(g_dtp2,	get_tx(random_schick(4) + 34), hero->alias);

		answer = GUI_dialogbox(hero->pic, hero->alias, g_dtp2, 3,
				get_tx(random_schick(4) + 38),
				get_tx(random_schick(4) + 42),
				get_tx(random_schick(4) + 46));

		if (answer == 1) {

			GUI_output(get_tx(random_schick(4) + 50));

		} else {

			sprintf(g_dtp2,	get_tx(random_schick(4) + 54), hero->alias);
			GUI_output(g_dtp2);
		}
	}
}

/**
 * \brief   random city event 3: a beggar asks for 1D
 */
void city_event_3(void)
{
	signed int answer;
	int32_t money;

	load_in_head(43);

	money = get_party_money();

	answer = money >= 100 ? 3 : 2;

	answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(random_schick(4) + 58), answer,
			get_tx(random_schick(4) + 62),
			get_tx(random_schick(4) + 66),
			get_tx(random_schick(4) + 70));

	if (answer == 3) {
		GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(random_schick(4) + 74), 0);
		money -= 100;
		set_party_money(money);
	}
}

/**
 * \brief   random city event 4: a beggar asks for 1D and tells some gossip
 */
void city_event_4(void)
{
	signed int answer;
	int32_t money;

	load_in_head(43);

	money = get_party_money();

	answer = money >= 100 ? 3 : 2;

	answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(random_schick(4) + 58), answer,
			get_tx(random_schick(4) + 78),
			get_tx(random_schick(4) + 82),
			get_tx(random_schick(4) + 86));

	if (answer == 3) {

		money -= 100;
		set_party_money(money);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(random_schick(4) + 90), 0);


		sprintf(g_dtp2 + 0x400,	get_tx(random_schick(4) + 94), (char*)load_current_town_gossip());

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, (char*)(g_dtp2 + 0x400), 0);
	}
}

/**
 * \brief   random city event 5: ??
 */
void city_event_5(void)
{
	signed int randval;
	signed int tw_bak;
	struct struct_hero* hero;

	load_in_head(48);

	tw_bak = g_textbox_width;
	g_textbox_width = 5;

	randval = random_schick(4) - 1;

	if (randval == 1) {

		hero = get_first_hero_available_in_group();

		sprintf(g_dtp2 + 0x400,	get_tx(randval + 99), hero->alias);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, (char*)(g_dtp2 + 0x400), 0);
	} else {
		GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(randval + 99), 0);
	}

	g_textbox_width = tw_bak;
}

/**
 * \brief   random city event 6: meet a merchant
 */
void city_event_6(void)
{
	signed int answer;
	signed int loc_bak;
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 24 */
	signed int type_bak;
#endif

	if (gs_day_timer >= HOURS(8) && gs_day_timer <= HOURS(20)) {

		load_in_head(4);

		sprintf(g_dtp2 + 0x400, get_tx(random_schick(4) + 102), get_hero(get_random_hero())->alias);

		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, (char*)(g_dtp2 + 0x400), 3,
			get_tx(random_schick(4) + 106),
			get_tx(random_schick(4) + 110),
			get_tx(random_schick(4) + 114));

		if (answer == 1 || answer == 2) {
			GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(random_schick(4) + 118), 0);
		} else if (answer == 3) {
			loc_bak = gs_current_loctype;
			gs_current_loctype = LOCTYPE_MERCHANT;
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 24:
	 * When entering a building in Thorwal, Prem, Phexcaer or Oberorken between 8:00 and 20:00 o'clock, the street merchant (random city event) shows up with a chance 1:900. Selecting the third answer in the text box, the shop screen appears. After leaving the street merchant, the entered building is corrupted. For example, an entered temple will be a Praios temple (which otherwise does not exist in the game), or an entered tavern may offer negative food prices.
	 */
			type_bak = gs_current_typeindex;
#endif
			gs_current_typeindex = 93;
			do_merchant();
			gs_current_loctype = loc_bak;
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 24 */
			gs_current_typeindex = type_bak;
#endif
	/* Original-Bug 27: After leaving the street merchant (random city event), the party is rotated by 180 degrees. This doesn't make too much sense, and it is inconsistent to the similar situation of visiting a merchant at a market, where no rotation is performed. If moreover the street merchant happens to appear when the party is about to enter some building, after leaving the street merchant and then leaving the building the party will *not* be rotated. */
#ifdef M302de_ORIGINAL_BUGFIX
	/* fix analogous to to Original-Bug 26.
	 * The rotation is performed in the function leave_location(), which has been called in do_merchant() above.
	 * We fix the bug in a hacky way by simply correcting the rotation afterwards. */
	gs_direction = ((gs_direction + 2) % 4); /* rotate by 180 degrees */
#endif
		}
	}
}

/**
 * \brief   random city event 7: some harmless events
 */
void city_event_7(void)
{
	signed int randval;
	struct struct_hero *hero;

	randval = random_schick(4) - 1;
	hero = get_hero(get_random_hero());

	if (!randval) {

		sprintf(g_dtp2 + 0x400, get_tx(123), hero->alias);

		GUI_dialogbox(hero->pic, hero->alias, (char*)(g_dtp2 + 0x400), 0);

	} else if (randval == 1) {

		load_in_head(12);

		sprintf(g_dtp2 + 0x400, get_tx(124), hero->alias);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, (char*)(g_dtp2 + 0x400), 0);

		sprintf(g_dtp2 + 0x400, get_tx(125), GUI_get_ptr(hero->sex, 3));

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, (char*)(g_dtp2 + 0x400), 0);

	} else if (randval == 2) {

		load_in_head(47);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(126), 0);

		sprintf(g_dtp2 + 0x400, get_tx(127), hero->alias);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, (char*)(g_dtp2 + 0x400), 0);

		sprintf(g_dtp2 + 0x400, get_tx(128), hero->alias);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, (char*)(g_dtp2 + 0x400), 0);
	} else {
		GUI_output(get_tx(129));
	}
}

/**
 * \brief   random city event 8: some harmless events
 */
void city_event_8(void)
{
	signed int randval;
	struct struct_hero *hero;

	randval = random_schick(4) - 1;
	hero = get_hero(get_random_hero());

	if (!randval) {

		GUI_output(get_tx(130));

	} else if (randval == 1) {

		load_in_head(12);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(131), 0);

		GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx(132), 0);

	} else if (randval == 3) {

		GUI_output(get_tx(133));

	} else {

		sprintf(g_dtp2, get_tx(134), hero->alias);
		GUI_output(g_dtp2);
	}
}

/**
 * \brief   random city event 9: some harmless events
 */
void city_event_9(void)
{
	signed int randval = random_schick(4) - 1;

	if (!randval) {

		GUI_output(get_tx(135));

	} else if (randval == 1) {

		GUI_output(get_tx(136));

	} else if (randval == 2) {

		GUI_output(get_tx(137));

	} else {

		GUI_output(get_tx(138));
		GUI_output(get_tx(139));
	}
}

/**
 * \brief   return a pointer to a weapon related information
 */
char* waffinfo_weapons(void)
{
	signed int randval;
	char **ptr;

	load_ltx(ARCHIVE_FILE_WAFFINFO_LTX);

	ptr = (char**)g_buffer9_ptr3;

	randval = random_schick(19) - 1;

	return ptr[randval];
}

/**
 * \brief   return a pointer to a herb related information
 */
char* waffinfo_herbs(void)
{
	signed int randval;
	char **ptr;

	load_ltx(ARCHIVE_FILE_WAFFINFO_LTX);

	ptr = ((char**)g_buffer9_ptr3) + 19;

	randval = random_schick(40) - 1;

	return ptr[randval];
}

/**
 * \brief   return a pointer to a general information
 */
char* waffinfo_general(void)
{
	signed int randval;
	char **ptr;

	load_ltx(ARCHIVE_FILE_WAFFINFO_LTX);

	ptr = ((char**)g_buffer9_ptr3) + 59;

	randval = random_schick(67) - 1;

	return ptr[randval];
}


#if !defined(__BORLANDC__)
}
#endif
