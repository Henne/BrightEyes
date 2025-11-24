/**
 *	Rewrite of DSA1 v3.02_de functions of seg112 (travel events 4 / 10)
 *	Functions rewritten: 13/13 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg112.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg026.h"
#include "seg047.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg112.h"

static uint8_t g_tevent067_chest[10] = {
	ITEM_BASTARDSCHWERT, 1,
	ITEM_SCHNAPSFLASCHE, 1,
	ITEM_REZEPT_FUER_VOMICUM, 1,
	ITEM_BOLZEN, 2,
	0xff, 0
}; // ds:0xb16a

void tevent_067(void)
{
	signed int i;
	signed int answer;
	signed int count;
	struct struct_hero *hero;

	if ((test_talent(get_first_hero_available_in_group(), TA_SINNESSCHAERFE, 6) > 0 && !gs_tevent067_flag) ||
		gs_tevent067_flag)
	{
		GUI_output(get_tx2(90));

		do {
			answer = GUI_radio(get_tx2(91), 3, get_tx2(92),	get_tx2(93), get_tx2(94));

		} while (answer == -1);

		if (answer == 2) {

			gs_town_loc_type = LOCTYPE_WILDCAMP;
			do_location();
			gs_town_loc_type = LOCTYPE_NONE;

		} else if (answer == 3) {

			hero = get_hero(0);
			for (i = count = 0; i <= 6; i++, hero++) {

				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
					!hero->flags.dead && test_attrib(hero, ATTRIB_GE, 0) > 0)
				{
					count++;
				}
			}

			if (!gs_tevent067_flag && count >= 3) {

				loot_multi_chest(g_tevent067_chest, get_tx2(96));

				GUI_output(get_tx2(97));

				hero = get_first_hero_available_in_group(); //TODO: no effect

				add_party_money(7L);

				gs_town_loc_type = LOCTYPE_WILDCAMP;
				do_location();
				gs_town_loc_type = LOCTYPE_NONE;
			} else {

				GUI_output(get_tx2(95));

				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero++) {

					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id))
					{
						sub_hero_le(hero, random_schick(8));
					}
				}

				gs_town_loc_type = LOCTYPE_WILDCAMP;
				do_location();
				gs_town_loc_type = LOCTYPE_NONE;

				TRV_load_textfile(-1);
			}

			gs_tevent067_flag = 1;
		}
	} else {

		GUI_output(get_tx2(90));

		do {
			answer = GUI_radio(get_tx2(82), 2, get_tx2(92), get_tx2(93));

		} while (answer == -1);

		if (answer == 2) {
			gs_town_loc_type = LOCTYPE_WILDCAMP;
			do_location();
			gs_town_loc_type = LOCTYPE_NONE;
		}
	}
}

void tevent_068(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Reiseevent %d ist nicht implementiert\n", 68);
#endif
}

void tevent_069(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 5) > 0 && !gs_tevent069_flag) ||
		gs_tevent069_flag)
	{
		TRV_found_camp_place(0);
		gs_tevent069_flag = 1;
	}
}

void tevent_070(void)
{
	signed int answer;

	if (test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 || gs_tevent070_flag)
	{
		gs_tevent070_flag = 1;

		if (test_talent(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 4) > 0 || gs_tevent070_herb_flag)
		{
			gs_tevent070_herb_flag = 1;

			g_gather_herbs_special = (int8_t)ITEM_JORUGAWURZEL; /* REMARK: item_id > 127 */

			answer = TRV_found_camp_place(2);

			g_gather_herbs_special = -1;

		} else {
			answer = TRV_found_camp_place(0);
		}

		if ((answer && !gs_tevent070_trail_flag && test_talent(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 0) > 0) ||
			gs_tevent070_trail_flag != 0) {

			gs_tevent070_trail_flag = 1;

			if (!TRV_follow_trail_question()) {

				do {
					answer = GUI_radio(get_tx2(0), 2, get_tx2(1), get_tx2(2));

				} while (answer == -1);

				if (answer == 1) {
					TRV_hunt_generic(25, 3, -1, 3, 8, 3, 7, 2, 4, 35, 0);
				}
			}
		}
	}
}

/* Orc-Monolith */
void tevent_071(void)
{
	signed int answer;
	signed int have_raft = 0;
	signed int i;
	struct struct_hero *hero;

	/* Perception + 8, Sinnesschaerfe + 8 */
	if ((test_talent((hero = get_first_hero_available_in_group()), TA_SINNESSCHAERFE, 8) > 0) && !gs_tevent071_flag)
	{
		gs_tevent071_flag = 1;

		/* Track + 4, Faehrtensuche + 4 */
		if (test_talent(hero, TA_FAEHRTENSUCHEN, 4) > 0) {

			sprintf(g_dtp2,	get_tx2(13), hero->alias, GUI_get_ptr(hero->sex, 0), GUI_get_ptr(hero->sex, 3));

			do {
				answer = GUI_radio(g_dtp2, 2, get_tx2(14), get_tx2(15));

			} while (answer == -1);

			if (answer == 2) {

				/* investigate */

				do {
					answer = GUI_radio(get_tx2(16), 3, get_tx2(17), get_tx2(18), get_tx2(19));

				} while (answer == -1);

				if (answer == 1) {

					/* just swim */
					TRV_swimm(2, 5);

				} else if (answer == 2) {

					/* build a raft before swimming */
					have_raft = 1;
					timewarp(HOURS(1));
					TRV_swimm(-1, 0);
				}

				if (answer == 1 || answer == 2) {

					sprintf(g_dtp2,	get_tx2(22), hero->alias);

					do {
						answer = GUI_radio(g_dtp2, 2, get_tx2(23), get_tx2(24));

					} while (answer == -1);

					if (answer == 1) {
						/* destroy monolith */

						GUI_output(get_tx2(25));

						/* REWARD: get gods estimation + 500 for each god */
						for (i = 1; i <= 14; i++) {
							gs_gods_estimation[i] += 500L;
						}

						/* mark the statuette as destroyed => has effects in fights */
						gs_tevent071_orcstatue = 1;

						add_hero_ap_all(10);

					} else {
						/* leave monolith */

						/* PUNISHMENT: set gods estimation to 0 for each god */
						for (i = 1; i <= 14; i++) {
							gs_gods_estimation[i] = 0L;
						}
					}

					i = (!have_raft ? 1 : 2);

					do {
						answer = GUI_radio(get_tx2(26), (signed char)i, get_tx2(27), get_tx2(28));

					} while (answer == -1);

					if (answer == 1) {
						TRV_swimm(2, 5);
					} else {
						timewarp(HOURS(1));
						TRV_swimm(-1, 0);
					}
				}
			}
		}
	}
}

/**
 * \brief   all members of the current group try to swim
 *
 * \param   mod         modificator for the swim test
 * \param   percent     probability to loose an item
 */
void TRV_swimm(const signed int mod, const signed int percent)
{
	signed int i;
	struct struct_hero *hero;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
			!hero->flags.dead)
		{

			if (test_talent(hero, TA_SCHWIMMEN, (signed char)mod) <= 0) {

				/* test failed */
				hero_disease_test(hero, DISEASE_ID_DUMPFSCHAEDEL, 20 - (hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod));

				loose_random_item(hero, percent, get_ttx(506));

				sub_hero_le(hero, random_schick(5));

				sprintf(g_dtp2,	get_tx2(21), hero->alias);
			} else {
				sprintf(g_dtp2,	get_tx2(20), hero->alias);
			}

			GUI_output(g_dtp2);
		}
	}
}

/* a herb place you need to swim to */
void tevent_unused01(void)
{
	signed int answer;
	signed int has_raft;
	signed int options;
	struct struct_hero *hero;

	has_raft = 0;

	hero = get_first_hero_available_in_group();

	if ((test_talent(hero, TA_SINNESSCHAERFE, 8) > 0 && !gs_teventu01_flag) || gs_teventu01_flag)
	{
		gs_teventu01_flag = 1;

		sprintf(g_dtp2,	get_tx2(29), hero->alias, GUI_get_ptr(hero->sex, 0), GUI_get_ptr(hero->sex, 3));

		do {
			answer = GUI_radio(g_dtp2, 2, get_tx2(30), get_tx2(31));

		} while (answer == -1);

		if (answer == 2) {

			do {
				answer = GUI_radio(get_tx2(32), 3, get_tx2(33), get_tx2(34), get_tx2(35));

			} while (answer == -1);

			if (answer == 1) {

				TRV_swimm(2, 5);

			} else if (answer == 2) {

				has_raft = 1;
				timewarp(HOURS(1));
				TRV_swimm(-1, 0);
			}

			if (answer == 1 || answer == 2) {

				GUI_output(get_tx2(38));

				g_gather_herbs_special = ITEM_GULMOND_BLATT;

				TRV_inside_herb_place();

				g_gather_herbs_special = -1;

				options = (!has_raft ? 1 : 2);

				do {
					answer = GUI_radio(get_tx2(39), (signed char)options, get_tx2(40), get_tx2(41));

				} while (answer == -1);

				if (answer == 1) {

					TRV_swimm(2, 5);

				} else {

					timewarp(HOURS(1));
					TRV_swimm(-1, 0);
				}
			}
		}
	}
}

void tevent_072(void)
{
	if ((test_talent(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0 && !gs_tevent072_flag) ||
		gs_tevent072_flag != 0)
	{
		TRV_found_camp_place(1);
		gs_tevent072_flag = 1;
	}
}

/* a corpse with the unicorn letter */
void tevent_073(void)
{
	signed int answer;

	if (!gs_tevent073_flag) {

		load_in_head(55);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(42), 2, get_tx2(43), get_tx2(44));

		} while (answer == -1);

		if (answer == 1) {

			GUI_dialog_na(0, get_tx2(45));

			loot_multi_chest(gs_tevent073_corpse, get_tx2(105));

			gs_tevent073_flag = 1;
		}
	}
}

/* brigants */
void tevent_074(void)
{
	signed int answer;
	signed int i;
	int32_t p_money;
	struct struct_hero *hero;

	if (!gs_tevent074_flag) {

		gs_tevent074_flag = (1);

		load_in_head(49);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx(49), get_tx2(46), 3,
						get_tx2(47), get_tx2(48), get_tx2(49));

		} while (answer == -1);

		if (answer == 1) {

			/* fight */
			gs_tevent074_fight_flag = TRV_fight_event(FIGHTS_F074, 74);

		} else if (answer == 2) {

			/* try to flee */

			if (test_talent(get_first_hero_available_in_group(), TA_KRIEGSKUNST, 2) <= 0)
			{
				/* failed, so fight */
				g_fig_initiative = 1;
				gs_tevent074_fight_flag = TRV_fight_event(FIGHTS_F074, 74);

			} else {

				/* remember the position of the last hero in the group */
				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero++) {

					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
						!hero->flags.dead)
					{
						answer = i;
					}
				}

				hero = get_hero(answer);

				sprintf((char*)g_dtp2 + 0x400, get_tx2(54), hero->alias);
				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(49), (char*)(g_dtp2 + 0x400), 0);

				/* this hero gets a damage of 2W6+4 */
				sub_hero_le(hero, dice_roll(2, 6, 4));
			}
		} else {

			/* try to make a deal */
			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx(49), get_tx2(50), 3,
							get_tx2(51), get_tx2(52), get_tx2(53));

			} while (answer == -1);

			if (answer == 1) {

				p_money = get_party_money();

				if (p_money < 1500) {
					/* not enough money, so fight */
					gs_tevent074_fight_flag = TRV_fight_event(FIGHTS_F074, 74);
				} else {
					/* pay 1500 */
					p_money -= 1500;
					set_party_money(p_money);
				}

			} else if (answer == 2) {

				/* fight */
				gs_tevent074_fight_flag = TRV_fight_event(FIGHTS_F074, 74);

			} else {

				/* try to be Charismatic */
				answer = count_heroes_in_group();

				if (test_attrib(get_first_hero_available_in_group(), ATTRIB_CH, 14 - answer) <= 0)
				{
					/* fight */
					g_fig_initiative = 1;
					gs_tevent074_fight_flag = TRV_fight_event(FIGHTS_F074, 74);
				}
			}
		}
	}
}

void tevent_075(void)
{
	signed int i;
	signed int answer;
	signed int ret;
	struct struct_hero *hero;

	ret = -1;

	if (!gs_tevent075_flag) {

		do {
			answer = GUI_radio(get_tx2(55), 2, get_tx2(56), get_tx2(57));

		} while (answer == -1);

		if (answer == 1) {

			do {
				answer = GUI_radio(get_tx2(58), 3, get_tx2(59),	get_tx2(60), get_tx2(61));

			} while (answer == -1);

			if (answer == 1) {

				g_fig_initiative = 2;

				ret = TRV_fight_event(FIGHTS_F075_A, 75);

			} else if (answer == 2) {

				i = FIGHTS_F075_B;

				if (!gs_tevent074_fight_flag && !gs_tevent077_fight_flag) {

					i = FIGHTS_F075_A;

				} else if (gs_tevent074_fight_flag && gs_tevent077_fight_flag) {

					i = FIGHTS_F075_C;
				}

				ret = TRV_fight_event(i, 75);

			} else {

				hero = get_hero(0);
				for (i = answer = 0; i <= 6; i++, hero++) {

					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
						!hero->flags.dead && test_talent(hero, TA_SCHLEICHEN, 0) <= 0)
					{
						answer++;
					}
				}

				if (answer > 1) {

					GUI_output(get_tx2(62));

					g_fig_initiative = 1;

					i = FIGHTS_F075_B;

					if (!gs_tevent074_fight_flag && !gs_tevent077_fight_flag) {

						i = FIGHTS_F075_A;

					} else if (gs_tevent074_fight_flag != 0 && gs_tevent077_fight_flag != 0) {

						i = FIGHTS_F075_C;
					}

					ret = TRV_fight_event(i, 75);

				} else {
					return;
				}
			}
		} else {
			return;
		}

		if (ret == 0) {
			gs_tevent075_flag = 1;
		}
	}
}

/* Felsteyn <-> Orkanger: entrance to the dungeon "goblin cave" */
void tevent_076(void)
{
	signed int answer;

	if (!gs_tevent076_flag) {

		if (test_talent(get_first_hero_available_in_group(), TA_SINNESSCHAERFE, 5) > 0)
		{
			gs_tevent076_flag = (1);

			GUI_dialog_na(53, get_tx2(63));

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(64), 2,
								get_tx2(65), get_tx2(66));

			} while (answer == -1);

			if (answer == 1) {
				gs_travel_detour = DUNGEON_ID_GOBLINHOEHLE;
			}
		}

	} else {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(67), 2,
							get_tx2(68), get_tx2(69));

		} while (answer == -1);

		if (answer == 1) {
			gs_travel_detour = DUNGEON_ID_GOBLINHOEHLE;
		}
	}
}

/* brigants */
void tevent_077(void)
{
	signed int answer;
	signed int i;
	int32_t p_money;
	struct struct_hero *hero;

	if (!gs_tevent077_flag) {

		gs_tevent077_flag = 1;

		load_in_head(4);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx(50), get_tx2(70), 3,
						get_tx2(71), get_tx2(72), get_tx2(73));

		} while (answer == -1);

		if (answer == 1) {

			/* fight */
			gs_tevent077_fight_flag = TRV_fight_event(186, 77);

		} else if (answer == 2) {

			if (test_talent(get_first_hero_available_in_group(), TA_KRIEGSKUNST, 6) <= 0) {

				/* test failed, so fight */
				g_fig_initiative = 1;
				gs_tevent077_fight_flag = TRV_fight_event(FIGHTS_F077, 77);

			} else {

				/* remember the last hero */
				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero++) {

					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
						!hero->flags.dead)
					{
						answer = i;
					}
				}

				hero = get_hero(answer);

				sprintf((char*)(g_dtp2 + 0x400), get_tx2(54), hero->alias);
				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(50), (char*)(g_dtp2 + 0x400), 0);

				/* the last hero looses between 6 and 16 LE */
				sub_hero_le(hero, random_schick(11) + 5);
			}

		} else {

			/* try to make a deal */
			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx(50), get_tx2(74), 3,
							get_tx2(75), get_tx2(76), get_tx2(77));

			} while (answer == -1);

			if (answer == 1) {

				p_money = get_party_money();

				if (p_money < 1500) {
					/* not enough money, so fight */
					gs_tevent077_fight_flag = TRV_fight_event(FIGHTS_F077, 77);
				} else {
					/* pay 1500 */
					p_money -= 1500;
					set_party_money(p_money);
				}

			} else if (answer == 2) {

				/* fight */
				gs_tevent077_fight_flag = TRV_fight_event(FIGHTS_F077, 77);

			} else {

				/* try to be Charismatic */
				answer = count_heroes_in_group();

				if (test_attrib(get_first_hero_available_in_group(), ATTRIB_CH, 14 - answer) <= 0)
				{
					/* fight */
					g_fig_initiative = 1;
					gs_tevent077_fight_flag = TRV_fight_event(FIGHTS_F077, 77);
				}
			}
		}
	}
}
