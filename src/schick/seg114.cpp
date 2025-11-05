/**
 *	Rewrite of DSA1 v3.02_de functions of seg114 (travel events 6 / 10)
 *	Functions rewritten: 11/11 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg114.cpp
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
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg113.h"
#include "seg114.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* wall of rock */
void tevent_110(void)
{
	signed int i;
	signed int answer;
	struct struct_hero *hero;

	do {
		answer = GUI_radio(get_tx2(79), 2, get_tx2(80),	get_tx2(81));

	} while (answer == -1);

	if (answer == 1)
	{
		/* try to climb */
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
				!hero->flags.dead && test_skill(hero, TA_KLETTERN, 0) <= 0)
			{
				/* skill test failed */

				if (get_first_hero_with_item(ITEM_ROPE) != -1)
				{
					/* one hero in the group has a rope */
					sprintf(g_dtp2,	get_tx2(85), hero->alias);

					sub_hero_le(hero, random_schick(10));

				} else {
					/* all heroes in the group have no ropes */
					sprintf(g_dtp2,	get_tx2(86), hero->alias);

					sub_hero_le(hero, random_schick(16) + 4);
				}

				GUI_output(g_dtp2);
			}

		}

		GUI_output(get_tx2(84));

	} else {

		/* try to walk arround */

		if (test_skill(get_first_hero_available_in_group(), TA_ORIENTIERUNG, 0) > 0)
		{
			/* success */
			timewarp(HOURS(4));
			GUI_output(get_tx2(82));
		} else {
			/* fail */
			timewarp(DAYS(1) + HOURS(6));
			GUI_output(get_tx2(83));
		}
	}
}

/* hunt a rhino */
void tevent_111(void)
{
	signed int counter;
	signed int answer;
	signed int i;
	signed int ret_skill_test2;
	signed int ret_skill_test3;
	signed int unlucky_tests;
	struct struct_hero *hero;

	if ((test_skill(get_first_hero_available_in_group(), TA_FAEHRTENSUCHEN, 1) > 0 && !gs_tevent111_flag) ||
		gs_tevent111_flag == 1)
	{
		gs_tevent111_flag = 1;
		g_event_ani_busy = 1;

		load_ani(33);
		draw_main_screen();
		init_ani(0);

		GUI_output(get_tx2(0));

		hero = get_hero(0);
		for (i = counter = 0; i <= 6; i++, hero++)
		{
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
				!hero->flags.dead && test_skill(hero, TA_SCHLEICHEN, -5) <= 0)
			{
				counter++;
			}
		}

		if (counter)
		{
			do {
				answer = GUI_radio(get_tx2(1), 2, get_tx2(7), get_tx2(8));

			} while (answer == -1);

			if (answer == 1)
			{
				GUI_output(get_tx2(9));

				add_hero_ap_all(7);

				timewarp(HOURS(2));
			} else {
				add_hero_ap_all(5);
			}

		} else {

			hero = get_hero(select_hero_ok_forced(get_tx2(2)));

			if (test_skill(hero, TA_SCHLEICHEN, 0) <= 0)
			{
				/* skill test failed */
				do {
					answer = GUI_radio(get_tx2(1), 2, get_tx2(7), get_tx2(8));

				} while (answer == -1);

				if (answer == 1)
				{
					GUI_output(get_tx2(9));

					add_hero_ap_all(7);

					timewarp(HOURS(2));
				} else {
					add_hero_ap_all(5);
				}

			} else {
				/* skill test succeeded */

				sprintf(g_dtp2,	get_tx2(3), hero->alias);

				GUI_input(g_dtp2, counter = unlucky_tests = 0);

				if ((i = test_skill(hero, TA_SCHUSSWAFFEN, 12)) > 0) counter++;
				if ((ret_skill_test2 = test_skill(hero, TA_SCHUSSWAFFEN, 12)) > 0) counter++;
				if ((ret_skill_test3 = test_skill(hero, TA_SCHUSSWAFFEN, 12)) > 0) counter++;

				if (i == 99) unlucky_tests++;
				if (ret_skill_test2 == 99) unlucky_tests++;
				if (ret_skill_test3 == 99) unlucky_tests++;

				if (counter == 3 || unlucky_tests >= 2)
				{
					sprintf(g_dtp2, get_tx2(4), hero->alias);

					GUI_output(g_dtp2);

					add_hero_ap_all(5);

					add_hero_ap(hero, 20);

					timewarp(HOURS(3));

					get_item(ITEM_FOOD_PACKAGE, 1, 200);

					gs_tevent111_flag = 2;

				} else if (counter == 2 || unlucky_tests == 1 || counter == 1)
				{

					do {
						answer = GUI_radio(get_tx2(5), 2, get_tx2(10), get_tx2(11));

					} while (answer == -1);

					if (answer == 1)
					{
						GUI_output(get_tx2(12));

						add_hero_ap_all(7);

						add_hero_ap(hero, 5);
					} else {

						GUI_output(get_tx2(13));

						add_hero_ap_all(15);

						add_hero_ap(hero, 5);

						timewarp(HOURS(4));

						hero = get_hero(0);
						for (i = 0; i <= 6; i++, hero++)
						{
							if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
								!hero->flags.dead && test_attrib(hero, ATTRIB_GE, 2) <= 0)
							{
								/* attribute test failed */
								sub_hero_le(hero, random_schick(10));
							}
						}

						get_item(ITEM_FOOD_PACKAGE, 1, 180);
						gs_tevent111_flag = 2;
					}

				} else {
					sprintf(g_dtp2, get_tx2(6), hero->alias);

					do {
						answer = GUI_radio(g_dtp2, 2, get_tx2(7),get_tx2(8));

					} while (answer == -1);

					if (answer == 1)
					{
						GUI_output(get_tx2(9));

						add_hero_ap_all(7);

						timewarp(HOURS(2));
					} else {
						add_hero_ap_all(5);
					}
				}
			}
		}

		disable_ani();
		g_request_refresh = 1;
	}

	g_event_ani_busy = 0;
}

/* a camp place */
void tevent_112(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent112_flag) ||
		gs_tevent112_flag)
	{
		gs_tevent112_flag = 1;

		if ((test_skill(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 2) > 0 && !gs_tevent112_herb_flag) ||
			gs_tevent112_herb_flag)
		{
			gs_tevent112_herb_flag = 1;

			g_gather_herbs_special = 131;
			TRV_found_camp_place(2);
			g_gather_herbs_special = -1;
		} else {
			TRV_found_camp_place(0);
		}
	}
}

/* FIRUN-temple in the wilderness */
void tevent_113(void)
{
	signed int answer;
	Bit32s est_old;
	Bit32s est_diff;

	do {
		answer = GUI_radio(get_tx2(14), 2, get_tx2(15), get_tx2(16));

	} while (answer == -1);

	if (answer == 1) {

		GUI_output(get_tx2(17));

		/* enter the temple */
		gs_current_typeindex = 58;
		est_old = gs_gods_estimation[GOD_FIRUN];
		gs_current_loctype = LOCTYPE_TEMPLE;
		do_location();
		TRV_load_textfile(-1);

		GUI_output(get_tx2(18));

		/* calculate the difference of the estimation */
		est_diff = gs_gods_estimation[GOD_FIRUN] - est_old;

		g_textbox_width = 3;

		/* the more money you spend, the better the group will sleep */
		g_wildcamp_sleep_quality = (est_diff <= 10 ? 1 : (est_diff <= 50 ? 2 : (est_diff <= 100 ? 3 : 4)));

		gs_current_loctype = LOCTYPE_WILDCAMP;
		do_location();
		gs_current_loctype = LOCTYPE_NONE;

		g_textbox_width = 9;
		g_request_refresh = 2;
	}
}

/* Phexcaer <-> Skelellen: a swamp */
void tevent_114(void)
{
	signed int i;
	signed int answer;
	signed int done;
	signed int j;
	struct struct_hero *hero;

	done = 0;

	do {

		do {
			answer = GUI_radio(get_tx2(19), 2, get_tx2(20), get_tx2(21));

		} while (answer == -1);

		if (answer == 1)
		{
			do {
				answer = GUI_radio(get_tx2(28), 2, get_tx2(29), get_tx2(30));

			} while (answer == -1);

			if (answer == 1)
			{
				/* turn around */
				gs_trv_return = done = 1;
			}

			if (answer == 2)
			{
				/* go on careful */
				GUI_output(get_tx2(22));
			}

		} else {
			GUI_output(get_tx2(22));
		}

		if (done == 0)
		{
			/* walk through the swamp */
			for (j = 0; j < 2; j++)
			{
				for (i = 0, hero = get_hero(0); i <= 6; i++, hero++)
				{
					if ((hero->typus != HERO_TYPE_NONE) &&
						(hero->group_id == gs_current_group) &&
						!hero->flags.dead &&
						test_attrib(hero, ATTRIB_GE, 4) <= 0)
					{
						/* attrib test failed */
						timewarp(MINUTES(30));

						sprintf(g_dtp2,	get_tx2(23), hero->alias, GUI_get_ptr(hero->sex, 2));
						GUI_output(g_dtp2);

						sub_hero_le(hero, random_schick(8));

						loose_random_item(hero, 50, get_ttx(506));
						loose_random_item(hero, 50, get_ttx(506));
						loose_random_item(hero, 50, get_ttx(506));
					}
				}
			}

			if (!gs_tevent114_olimone_flag)
			{
				/* meet OLIMONE */
				GUI_output(get_tx2(24));

				load_in_head(12);

				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(52), get_tx2(25), 0);
				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(52), get_tx2(26), 0);

				/* get RECIPE FOR POTENT HEALING POTION */
				get_item(ITEM_RECIPE_STRONG_LE_POTION, 1, 1);

				/* TODO: ORIGINAL-BUG: this item could have been not taken => get_hero(-1) => SEGFAULT */
				hero = get_hero(get_first_hero_with_item(ITEM_RECIPE_STRONG_LE_POTION));

				sprintf(g_dtp2,	get_tx2(27), hero->alias);
				GUI_output(g_dtp2);

				timewarp(HOURS(8));

				for (i = 0, hero = get_hero(0); i <= 6; i++, hero++)
				{
					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead)
					{
						add_hero_le(hero, 7);
					}
				}

				gs_tevent114_olimone_flag = done = 1;

			} else {
				GUI_output(get_tx2(64));

				timewarp(HOURS(8));

				for (i = 0, hero = get_hero(0); i <= 6; i++, hero++)
				{
					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead)
					{
						add_hero_le(hero, 4);
					}
				}

				done = 1;
			}
		}

	} while (done == 0);
}

/* a camp place */
void tevent_116(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 6) > 0 && !gs_tevent116_flag) ||
		gs_tevent116_flag)
	{
		gs_tevent116_flag = 1;
		TRV_found_camp_place(1);
	}
}

/* an avalance */
void tevent_117(void)
{
	signed int i;
	struct struct_hero *hero;

	GUI_output(get_tx2(31));
	GUI_output(get_tx2(32));

	for (hero = get_hero(0), i = 0; i <= 6; i++, hero++)
	{
		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
			!hero->flags.dead && test_attrib(hero, ATTRIB_GE, 0) <= 0)
		{
			/* attrib test failed */
			sub_hero_le(hero, random_schick(11) + 1);

			loose_random_item(hero, 15, get_ttx(506));
			loose_random_item(hero, 15, get_ttx(506));
			loose_random_item(hero, 15, get_ttx(506));
		}
	}

	GUI_output(get_tx2(33));
}

/* a herb place */
void tevent_118(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 3) > 0 && !gs_tevent118_flag) ||
		gs_tevent118_flag)
	{
		g_gather_herbs_special = 60;
		TRV_found_herb_place(0);
		g_gather_herbs_special = -1;
		gs_tevent118_flag = 1;
	}
}

/* a camp place */
void tevent_119(void)
{
	if ((test_skill(get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !gs_tevent119_flag) || gs_tevent119_flag)
	{
		gs_tevent119_flag = 1;
		TRV_found_camp_place(0);
	}
}

/* Orvil - Rovik: wolves threaten a sheperd */
void tevent_122(void)
{
	signed int answer;

	if (!gs_tevent122_flag)
	{
		do {
			answer = GUI_radio(get_tx2(34), 2, get_tx2(35),	get_tx2(36));

		} while (answer == -1);

		if (answer == 1)
		{
			/* attack wolves */
			if (!TRV_fight_event(FIGHTS_F122, 122))
			{
				gs_tevent122_flag = 1;

				add_hero_ap_all(10);

				load_in_head(4);

				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(68), get_tx2(37), 0);
				GUI_dialogbox((unsigned char*)g_dtp2, get_tx(68), get_tx2(63), 0);
			}
		} else {
			/* decline to help */
			sub_hero_ap_all(20);
		}
	}
}

/* a bridge */
void tevent_123(void)
{
	signed int i;
	signed int counter;
	signed int answer;
	signed int done;
	signed int attrib_result;
	signed int skill_result;
	struct struct_hero *hero;

	load_in_head(54);

	do {
		done = 0;

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(38), 3, get_tx2(39), get_tx2(40), get_tx2(41));

		} while (answer == -1);

		if (answer == 1)
		{
			/* go over the bridge */
			hero = get_hero(0);
			for (i = counter = 0; i <= 6; i++, hero++)
			{
				if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) &&
					!hero->flags.dead && test_attrib(hero, ATTRIB_HA, 0) > 0)
				{
					/* attrib test failed */
					counter++;

					sprintf(g_dtp2 + 0x400,	get_tx2(42), hero->alias);

					GUI_dialog_na(0, (char*)(g_dtp2 + 0x400));
				}
			}

			if (!counter)
			{
				GUI_dialog_na(0, get_tx2(43));
				done = 1;
			}

		} else if (answer == 2)
		{
			/* turn arround */
			gs_trv_return = done = 1;

		} else {
			/* fight acrophobia */
			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(44), 3,
							get_tx2(45), get_tx2(46), get_tx2(47));

			} while (answer == -1);

			if (answer == 1)
			{
				hero = get_hero(0);
				for (i = counter = 0; i <= 6; i++, hero++)
				{
					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead)
					{
						attrib_result = test_attrib(hero, ATTRIB_HA, 4);
						skill_result = test_skill(hero, TA_KLETTERN, 0);

						if (attrib_result == 99 && skill_result == -1)
						{
							sprintf(g_dtp2 + 0x400, get_tx2(48), hero->alias);

							GUI_dialog_na(0, (char*)g_dtp2 + 0x400);

							hero_disappear(hero, i, -1);

							counter++;

						} else if (attrib_result > 0 || skill_result <= 0)
						{
							sprintf(g_dtp2 + 0x400,	get_tx2(42), hero->alias);

							GUI_dialog_na(0, (char*)g_dtp2 + 0x400);

							counter++;
						}
					}
				}

				if (!counter)
				{
					GUI_dialog_na(0, get_tx2(43));
					done = 1;
				}

			} else if (answer == 2)
			{
				/* cast a spell */
				hero = get_hero(select_hero_ok_forced(get_ttx(317)));

				if (test_spell(hero, SP_MU_STEIGERN, 0) > 0)
				{
					/* success */
					sub_ae_splash(hero, get_spell_cost(SP_MU_STEIGERN, 0));

					GUI_dialog_na(0, get_tx2(43));

					done = 1;
				} else {
					/* failed */
					sub_ae_splash(hero, get_spell_cost(SP_MU_STEIGERN, 1));

					hero = get_hero(0);
					for (i = counter = 0; i <= 6; i++, hero++)
					{
						if ((hero->typus != HERO_TYPE_NONE) &&
							(hero->group_id == gs_current_group) &&
							!hero->flags.dead)
						{
							sprintf(g_dtp2 + 0x400, get_tx2(42), hero->alias);

							GUI_dialog_na(0, (char*)g_dtp2 + 0x400);
						}
					}
				}
			} else {
				hero = get_hero(0);
				for (i = counter = 0; i <= 6; i++, hero++)
				{
					if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_current_group) && !hero->flags.dead)
					{
						attrib_result = test_attrib(hero, ATTRIB_HA, 2);

						if (attrib_result == 99)
						{
							/* unlucky */
							sprintf(g_dtp2 + 0x400,	get_tx2(48), hero->alias);

							GUI_dialog_na(0, (char*)g_dtp2 + 0x400);

							hero_disappear(hero, i, -1);

							counter++;

						} else if (attrib_result > 0)
						{
							/* succeeded */
							sprintf(g_dtp2 + 0x400,	get_tx2(42), hero->alias);

							GUI_dialog_na(0, (char*)g_dtp2 + 0x400);

							counter++;
						}
					}
				}

				if (!counter)
				{
					GUI_dialog_na(0, get_tx2(43));
					done = 1;
				}
			}
		}


	} while (done == 0);
}

#if !defined(__BORLANDC__)
}
#endif
