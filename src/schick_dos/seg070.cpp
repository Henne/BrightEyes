/**
 *	Rewrite of DSA1 v3.02_de functions of seg070 (phexcaer: buildings 1/2)
 *	Functions rewritten: 7/7 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg070.cpp
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg027.h"
#include "seg032.h"
#include "seg047.h"
#include "seg053.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

/**
 * \brief   carriage station
 */
void PHX_fuhrhaus(void)
{
	signed int answer;

	load_in_head(42);

	do {
		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(0), 3,
				get_tx2(1), get_tx2(2), get_tx2(3));

	} while (answer == -1);

	if (answer == 1) {

		GUI_dialog_na(0, get_tx2(4));

	} else if (answer == 2) {

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(5), 3,
					get_tx2(6), get_tx2(7), get_tx2(8));

		} while (answer == -1);

		GUI_dialog_na(0, answer == 1 ? get_tx2(10): (answer == 2 ? get_tx2(11) : get_tx2(12)));

		if (answer == 3) {

			/* You are now able to talk to Alrik */
			gs_alrik_derondan = 1;
		}

		GUI_dialog_na(0, get_tx2(13));

	} else {

		GUI_dialog_na(0, get_tx2(9));
	}
}

/**
 * \brief   stadthaus of Phexcaer
 */
void PHX_stadthaus(void)
{
	signed int answer;
	int32_t money;

	do {
		answer = GUI_radio(get_tx2(14), 3, get_tx2(15), get_tx2(16), get_tx2(17));

	} while (answer == -1);

	if (answer == 1) {

		GUI_output(get_tx2(18));

	} else {

#ifdef M302de_ORIGINAL_BUGFIX
		signed int answer2;
#endif
		money = get_party_money();

		/* reuse answer as number of options */
		answer = money >= 1000 ? 3 : 1;

#ifdef M302de_ORIGINAL_BUGFIX

		do {
			answer2 = GUI_radio(get_tx2(19), (signed char)answer, get_tx2(21), get_tx2(20),	get_tx2(22));

		} while (answer2 == -1);

		answer = answer2;

#else
		/* Original-Bug: pressing ESC calls GUI_radio() again with -1 options */
		do {
			answer = GUI_radio(get_tx2(19), answer, get_tx2(21), get_tx2(20), get_tx2(22));

		} while (answer == -1);
#endif

		if (answer == 1) {

			GUI_output(get_tx2(23));

		} else {

			/* the Party looses 10D */
			money -= 1000;

			set_party_money(money);

			GUI_output(get_tx2(24));
		}
	}
}

/**
 * \brief   spielhaus of Phexcaer
 */
void PHX_spielhaus(void)
{
	signed int answer;
	signed int counter;
	signed int pos;
	int32_t money;
	struct struct_hero *hero;

	do {
		answer = GUI_radio(get_tx2(25), 3, get_tx2(26), get_tx2(27), get_tx2(28));

	} while (answer == -1);

	if (answer == 1) {

		GUI_output(get_tx2(29));

		hero = get_hero(0);

		for (pos = counter = answer = 0; pos <= 6; pos++, hero++) {

			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
				!hero->flags.dead && (test_talent(hero, TA_FALSCHSPIEL, 3) > 0))
			{
				/* Gambling +3 */
				counter++;
			}
		}

		pos = random_schick(20);

		money = get_party_money();

#ifdef M302de_ORIGINAL_BUGFIX
		if ((count_heroes_in_group() >> 1) >= counter) {
#else
		if ((count_heroes_in_group() >> 1) < counter) {
#endif

			/* you loose: at least the half of your group have not passed the test */

			sprintf(g_dtp2,	get_tx2(30), pos);

			money -= pos * 100;

			if (money < 0) {
				money = 0;
			}
		} else {

			/* you win */
			sprintf(g_dtp2,	get_tx2(31), pos);

			money += pos * 100;
		}

		set_party_money(money);

		GUI_output(g_dtp2);

	} else if (answer == 2) {

		GUI_output(get_tx2(32));

		/* You are now able to talk to Alrik */
		gs_alrik_derondan = 1;

	} else {

		money = get_party_money();

		pos = money >= 500 ? 3 : 2;

		do {
			answer = GUI_radio(get_tx2(33), (signed char)pos, get_tx2(34), get_tx2(36), get_tx2(35));

		} while (answer == -1);

		GUI_output(answer == 3 ? get_tx2(38): get_tx2(37));

		if (answer == 3) {

			money -= 500;

			if (money < 0) {
				money = 0;
			}
		}

		set_party_money(money);
	}
}

/**
 * \brief   the villa of Ektor Gremob
 */
void PHX_villa_gremob(void)
{
	GUI_output(!gs_gremob_invited ? get_tx2(40) : get_tx2(39));

	if (gs_gremob_invited) {
		do_fight(FIGHT_ID_PHEX24);
	}
}

/**
 * \brief   the brothel
 */
void PHX_bordell(void)
{
	signed int answer;

	load_ani(24);

	init_ani(0);

	do {
		answer = GUI_radio(get_tx2(41), 3, get_tx2(42), get_tx2(43), get_tx2(44));

	} while (answer == -1);

	if (answer == 1) {

		do {
			answer = GUI_radio(get_tx2(45), 3, get_tx2(46), get_tx2(47), get_tx2(48));

		} while (answer == -1);

		GUI_input(answer == 3 ? get_tx2(51) : get_tx2(52), 0);

		if ((answer == 1) || (answer == 2)) {
			gs_harlot_date = 1;
		}

	} else if (answer == 2) {

		/* increase RAHJA's estimation */
		gs_gods_estimation[GOD_ID_RAHJA] += 30L;

		timewarp_until_time_of_day(HOURS(7));

		GUI_input(get_tx2(49), 0);

	} else if (answer == 3) {	/* this check is bogus */

		GUI_input(get_tx2(50), 0);
	}

	disable_ani();
}

/**
 * \brief   the pharmacy
 */
void PHX_apotheke(void)
{
	signed int answer;
	signed int options;
	int32_t money;

	load_ani(23);

	init_ani(0);

	do {
		answer = GUI_radio(get_tx2(53), 3, get_tx2(54),	get_tx2(55), get_tx2(56));

	} while (answer == -1);

	if (answer == 1) {

		money = get_party_money();

		options = money >= 1000 ? 3 : 2;


		do {
			answer = GUI_radio(get_tx2(57), (signed char)options, get_tx2(62), get_tx2(61), get_tx2(60));

		} while (answer == -1);

		if (answer == 3) {

			money -= 1000;

			set_party_money(money);

			give_new_item_to_group(ITEM_ID_WIRSELKRAUT, 1, 1);
		} else {
			GUI_input(get_tx2(69), 0);
		}

	} else if (answer == 2) {

		money = get_party_money();

		answer = money >= 500 ? 3 : 2;

		do {
			answer = GUI_radio(get_tx2(58), (signed char)answer, get_tx2(63), get_tx2(65), get_tx2(64));

		} while (answer == -1);


		if (answer == 3) {

			money -= 500;

			set_party_money(money);

			GUI_input(get_tx2(71), 0);
		} else {

			GUI_input(answer == 1 ? get_tx2(70) : get_tx2(69), 0);

		}
	} else {

		do {
			answer = GUI_radio(get_tx2(59), 3, get_tx2(66), get_tx2(67), get_tx2(68));

		} while (answer == -1);

		if (answer == 1) {

			GUI_input(get_tx2(69), 0);

		} else if (answer == 2) {

			do {
				answer = GUI_radio(get_tx2(72), 3, get_tx2(55), get_tx2(73), get_tx2(74));

			} while (answer == -1);

			if (answer == 1) {
				money = get_party_money();

				answer = money >= 500 ? 3 : 2;

				do {
					answer = GUI_radio(get_tx2(58), (signed char)answer, get_tx2(63), get_tx2(65), get_tx2(64));

				} while (answer == -1);

				if (answer == 3) {

					money -= 500;

					set_party_money(money);

					GUI_input(get_tx2(71), 0);
				} else {
					GUI_input(answer == 1 ? get_tx2(70) : get_tx2(69), 0);
				}
			} else {
				GUI_input(get_tx2(75), 0);
			}

		} else {

			money = get_party_money();

			answer = money >= 1000 ? 3 : 2;

			do {
				answer = GUI_radio(get_tx2(57), (signed char)answer, get_tx2(62), get_tx2(61), get_tx2(60));

			} while (answer == -1);

			if (answer == 3) {

				money -= 1000;

				set_party_money(money);

				give_new_item_to_group(ITEM_ID_WIRSELKRAUT, 1, 1);
			} else {
				GUI_input(get_tx2(69), 0);
			}
		}
	}

	disable_ani();
}

/**
 * \brief   the healer
 */
void PHX_healer(void)
{
	signed int answer;
	int32_t money;

	answer = random_schick(100);

	if (answer <= 70) {

		GUI_output(get_tx2(76));

	} else if (answer <= 85) {

		GUI_output(get_tx2(77));

	} else {

		load_ani(23);

		init_ani(0);

		do {
			answer = GUI_radio(get_tx2(78), 3, get_tx2(79), get_tx2(80), get_tx2(81));

		} while (answer == -1);

		if (answer == 1) {

			GUI_output(get_tx2(82));

			/* enter the healer */
			gs_town_typeindex = 13;
			gs_town_locdata = (15);

			do_healer();

		} else if (answer == 3) {

			GUI_output(get_tx2(84));

		} else {
			/* answer == 2 */
			do {
				answer = GUI_radio(get_tx2(83), 3, get_tx2(85), get_tx2(86), get_tx2(87));

			} while (answer == -1);

			if (answer == 1) {

				do {
					answer = GUI_radio(get_tx2(88), 3, get_tx2(89), get_tx2(90), get_tx2(91));

				} while (answer == -1);

			} else if (answer == 2) {

				money = get_party_money();

				if (money >= 200) {

					GUI_output(get_tx2(92));

					money -= 200;

					set_party_money(money);
				} else {
					GUI_output(get_tx2(99));
				}
			} else {

				money = get_party_money();

				answer = money >= 500 ? 3 : (money >= 10 ? 2 : 1);

				do {
					answer = GUI_radio(get_tx2(93), (signed char)answer, get_tx2(96), get_tx2(94), get_tx2(95));

				} while (answer == -1);

				if (answer == 2) {

					money -= 10;

					set_party_money(money);

					GUI_output(get_tx2(97));

				} else if (answer == 3) {

					money -= 500;

					set_party_money(money);

					GUI_output(get_tx2(98));
				}
			}
		}
	}

	disable_ani();
}
