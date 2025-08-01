/*
 *	Rewrite of DSA1 v3.02_de functions of seg113 (travel events 5 / 10)
 *	Functions rewritten: 22/22 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg113.cpp
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
#include "seg028.h"
#include "seg029.h"
#include "seg032.h"
#include "seg047.h"
#include "seg097.h"
#include "seg098.h"
#include "seg103.h"
#include "seg105.h"
#include "seg109.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* The Tatzelwurm */
void tevent_080(void)
{
	signed short answer;
	Bit8u *hero;

	hero = (Bit8u*)get_first_hero_available_in_group();

	if ((!ds_readb(TEVENT080_FLAG) && (test_skill(hero, TA_WILDNISLEBEN, 4) > 0)) ||
			ds_readb(TEVENT080_FLAG) != 0)
	{
		ds_writeb(TEVENT080_FLAG, 1);
		ds_writed(MAIN_ACTING_HERO, 0);

		TRV_found_replenish_place(0);

		hero = (Bit8u*)ds_readd(MAIN_ACTING_HERO);

		if ((hero && !ds_readb(TEVENT080_TATZELWURM) && test_skill(hero, TA_FAEHRTENSUCHEN, 5) > 0) ||
			ds_readb(TEVENT080_TATZELWURM) == 1)
		{
			ds_writeb(TEVENT080_TATZELWURM, 1);

			sprintf(g_dtp2,
				get_tx2(87),
				(char*)hero + HERO_NAME2);

			do {
				answer = GUI_radio(g_dtp2, 2,
							get_tx2(88),
							get_tx2(89));
			} while (answer == -1);

			if (answer == 1) {

				timewarp(HOURS(1));

				do {
					answer = GUI_radio(get_tx2(90), 2,
								get_tx2(91),
								get_tx2(92));
				} while (answer == -1);

				if (answer == 2) {

					timewarp(HOURS(1));

					do {
						answer = GUI_radio(get_tx2(93), 2,
									get_tx2(94),
									get_tx2(95));
					} while (answer == -1);

					if (answer == 1) {

						GUI_output(get_tx2(96));

						ds_writeb(TEVENT080_TATZELWURM, 2);

						add_hero_ap_all(20);
					}
				}
			}
		}
	}
}

void tevent_081(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 2) > 0 && !ds_readb(TEVENT081_FLAG)) ||
		 ds_readb(TEVENT081_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT081_FLAG, 1);
	}
}

void tevent_082(void)
{
	TRV_cross_a_ford(get_tx(51), 30, 1);
}

void tevent_083(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_PFLANZENKUNDE, 6) > 0 && !ds_readb(TEVENT083_FLAG)) ||
		 ds_readb(TEVENT083_FLAG) != 0)
	{
		ds_writeb(GATHER_HERBS_SPECIAL, 61);
		TRV_found_herb_place(0);
		ds_writeb(GATHER_HERBS_SPECIAL, -1);
		ds_writeb(TEVENT083_FLAG, 1);
	}
}

void tevent_084(void)
{
	signed short answer;

	if (!ds_readb(TEVENT084_FLAG)) {

		load_in_head(44);

		do {

			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
						get_tx2(97), 2,
						get_tx2(98),
						get_tx2(99));
		} while (answer == -1);

		if (answer == 1) {

			if (!do_fight(FIGHTS_F084)) {
				ds_writeb(TEVENT084_FLAG, 1);
			}
		} else {

			if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_KRIEGSKUNST, 5) <= 0)
			{
				g_fig_initiative = 1;

				if (!do_fight(FIGHTS_F084)) {
					ds_writeb(TEVENT084_FLAG, 1);
				}
			}
		}
	}
}

void tevent_085(void)
{
	Bit8u *hero = (Bit8u*)get_first_hero_available_in_group();

	if ((test_skill(hero, TA_WILDNISLEBEN, 4) > 0 && !ds_readb(TEVENT085_FLAG)) ||
		 ds_readb(TEVENT085_FLAG) != 0)
	{
		ds_writeb(TEVENT085_FLAG, 1);

		if ((test_skill(hero, TA_PFLANZENKUNDE, 6) > 0 && !ds_readb(TEVENT085_HERB_FLAG)) ||
			 ds_readb(TEVENT085_HERB_FLAG) != 0)
		{
			ds_writeb(TEVENT085_HERB_FLAG, 1);
			ds_writebs(GATHER_HERBS_SPECIAL, 124);
			TRV_found_camp_place(2);
			ds_writebs(GATHER_HERBS_SPECIAL, -1);
		} else {
			TRV_found_camp_place(0);
		}
	}
}

void tevent_086(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	load_in_head(58);

	do {
		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
					get_tx2(100), 2,
					get_tx2(101), get_tx2(102));
	} while (answer == -1);

	if (answer == 1) {

		hero = get_hero(0);

		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group)
			{
				sub_hero_le(hero, 2);
			}
		}

		GUI_dialog_na(0, get_tx2(103));

		if (!ds_readb(TEVENT086_FLAG)) {

			add_hero_ap_all(ds_writeb(TEVENT086_FLAG, 15));
		}

	} else {

		GUI_dialog_na(0, get_tx2(104));

		if (!ds_readb(TEVENT086_FLAG)) {

			add_hero_ap_all(ds_writeb(TEVENT086_FLAG, 5));
		}
	}
}

void tevent_088(void)
{
	if (((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 3) > 0) && (!ds_readb(TEVENT088_FLAG))) ||
		 ds_readb(TEVENT088_FLAG) != 0)
	{
		TRV_found_camp_place(0);
		ds_writeb(TEVENT088_FLAG, 1);
	}
}

void tevent_089(void)
{
	TRV_cross_a_ford(get_tx(51), 40, 2);
}

void tevent_097(void)
{
	if (TRV_enter_hut_question()) {

		gs_current_loctype = LOCTYPE_WILDCAMP;
		do_location();
		gs_current_loctype = LOCTYPE_NONE;
	}
}

/**
 * \brief   travelevent 98: a gap
 *
 *          disappeared heroes can be found in the Efferd temple in Liskor
 */
void tevent_098(void)
{
	signed short answer;
	signed short repeat;
	signed short i;
	signed short hero_pos;
	Bit8u *hero;

	/* TODO: this may not be correct */
	i = get_first_hero_with_item(ITEM_ROPE) != -1 || get_first_hero_with_item(ITEM_ROPE_LADDER) != -1 ? 3 : 2; /* TODO: STAFFSPELL? */

	do {
		answer = GUI_radio(get_tx2(27), (signed char)i,
				get_tx2(28),
				get_tx2(29),
				get_tx2(30));
	} while (answer == -1);

	do {
		repeat = 0;

		if (answer == 1) {

			hero = get_hero(0);

			for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

				if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
					host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
					!hero_dead(hero))
				{

					if (test_skill(hero, TA_AKROBATIK, 0) > 0) {

						sprintf(g_dtp2,
							get_tx2(31),
							(char*)hero + HERO_NAME2);

						GUI_output(g_dtp2);

					} else {

						sprintf(g_dtp2,
							get_tx2(32),
							(char*)hero + HERO_NAME2);

						GUI_output(g_dtp2);

						hero_disappear(hero, i, 33);
					}
				}
			}

		} else if (answer == 2) {

			GUI_output(get_tx2(33));

			/* Original-Bug: hero not initialized */
#ifdef M302de_ORIGINAL_BUGFIX
			/* Original-Bugfix: take the leader of the group */
			hero = (Bit8u*)get_first_hero_available_in_group();
#endif
			hero_disease_test(hero, 2, 20 - (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))));

			loose_random_item(hero, 1, get_ttx(506));

			gs_current_loctype = LOCTYPE_WILDCAMP;
			do_location();
			gs_current_loctype = LOCTYPE_NONE;

			TRV_load_textfile(-1);
		} else {

			hero = get_hero(hero_pos = select_hero_ok_forced(get_tx2(34)));

			if (test_skill(hero, TA_AKROBATIK, 0) > 0) {

				sprintf(g_dtp2,
					get_tx2(35),
					(char*)hero + HERO_NAME2);

				GUI_output(g_dtp2);

				hero = get_hero(0);
				for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

					if (i != hero_pos &&
						host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
						host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
						!hero_dead(hero))
					{
						if (test_attrib(hero, ATTRIB_GE, -4) > 0) {

							sprintf(g_dtp2,
								get_tx2(36),
								(char*)hero + HERO_NAME2);

							GUI_output(g_dtp2);

						} else {

							sprintf(g_dtp2,
								get_tx2(32),
								(char*)hero + HERO_NAME2);

							GUI_output(g_dtp2);

							hero_disappear(hero, i, 33);
						}
					}
				}
			} else {

				sprintf(g_dtp2,
					get_tx2(32),
					(char*)hero + HERO_NAME2);

				GUI_output(g_dtp2);

				hero_disappear(hero, i, 33);

				do {
					answer = GUI_radio(get_tx2(37), 3,
								get_tx2(28),
								get_tx2(29),
								get_tx2(30));
				} while (answer == -1);

				repeat = 1;
			}
		}

	} while (repeat);
}

/**
 * \brief   makes a hero disappear
 *
 * \param   hero        the hero
 * \param   pos         the position of the hero
 * \param   temple_id	value = -2, -1: hero disappears completely; value >= 0: hero can be found in the temple with that id.
 */
void hero_disappear(Bit8u *hero, unsigned short pos, signed short temple_id)
{

	/* decrement the number of heroes */
	gs_total_hero_counter--;

	/* load a new savegame if no hero is present */
	/* TODO: potential Original-Bug: What if only the NPC is left? */
	if (!gs_total_hero_counter) {
		ds_writew(GAME_STATE, GAME_STATE_DEAD);
	}

	/* decrement group counter */
	gs_group_member_counts[gs_current_group]--;

	/* write temple_id to character sheet */
	host_writeb(hero + HERO_TEMPLE_ID, (signed char)temple_id);

	/* reset position in group */
	host_writeb(hero + HERO_GROUP_POS, 0);

	if (pos == 6) {
		/* NPC */
		save_npc(ARCHIVE_FILE_NPCS + host_readbs(get_hero(6) + HERO_NPC_ID));

		/* reset NPC timer */
		ds_writebs((NPC_TIMERS + 1) + host_readbs(get_hero(6) + HERO_NPC_ID), -1);
	} else {
		/* Regular Hero */
		write_chr_temp(pos);
	}

	/* set typus to 0 */
	host_writeb(hero + HERO_TYPE, 0);

	if (temple_id != -2) {
		draw_main_screen();
		init_ani(2);
		ds_writew(REQUEST_REFRESH, 1);
	}

	/* set flag to check all heroes */
	g_check_party = 1;
}

/* fight with 4 harpyes */
void tevent_099(void)
{
	signed short answer;

	if (!ds_readb(TEVENT099_FLAG)) {

		do {
			answer = GUI_radio(get_tx2(38), 2,
						get_tx2(39),
						get_tx2(40));
		} while (answer == -1);

		/* Original-Bug: The 2nd option "try to flee (or avoid the fight)" is not a try.
		 *		 To trigger the code you have to flee from the fight mode,
		 *		 then the skill test is done, and if this one fails you reenter this fight.
		*/


		if (answer == 1) {

			if (!do_fight(FIGHTS_F099)) {
				ds_writeb(TEVENT099_FLAG, 1);
#ifdef M302de_ORIGINAL_BUGFIX
			}
#endif
		} else {

			if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_KRIEGSKUNST, 4) <= 0) {

				g_fig_initiative = 1;

				if (!do_fight(FIGHTS_F099)) {
					ds_writeb(TEVENT099_FLAG, 1);
				}
			}
#ifndef M302de_ORIGINAL_BUGFIX
			}
#endif
		}
	}
}

/* fight with 2-6 harpyes */
void tevent_101(void)
{
	signed short answer;
	signed short mod;

	if (!ds_readb(TEVENT101_FLAG)) {

		sprintf(g_dtp2,
			get_tx2(41),
			(mod = random_schick(4) + 2));


		do {
			answer = GUI_radio(g_dtp2, 2,
						get_tx2(42),
						get_tx2(43));
		} while (answer == -1);

		g_max_enemies = (mod);

		/* Original-Bugfix: see description in tevent_099() */
		if (answer == 1) {

			if (!do_fight(FIGHTS_F101)) {
				ds_writeb(TEVENT101_FLAG, 1);
#ifdef M302de_ORIGINAL_BUGFIX
			}
#endif
		} else {

			if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_KRIEGSKUNST, (signed char)mod) <= 0) {

				g_fig_initiative = 1;

				if (!do_fight(FIGHTS_F101)) {
					ds_writeb(TEVENT101_FLAG, 1);
				}
			}
#ifndef M302de_ORIGINAL_BUGFIX
			}
#endif
		}
	}
}

void tevent_102(void)
{
	if (TRV_enter_hut_question())
	{
		gs_current_loctype = LOCTYPE_WILDCAMP;
		do_location();
		gs_current_loctype = LOCTYPE_NONE;
	}
}

void tevent_103(void)
{
	signed short answer;
	Bit8u *hero;

	/* Original-Bug: save the address instead the return value of a function as a hero */
#ifdef M302de_ORIGINAL_BUGFIX
	hero = (Bit8u*)get_first_hero_available_in_group();
#else
	hero = (Bit8u*)(get_first_hero_available_in_group);
#endif

	do {
		answer = GUI_radio(get_tx2(44), 2,
					get_tx2(45),
					get_tx2(46));
	} while (answer == -1);

	if (answer == 1) {

		if (test_skill(hero, TA_ORIENTIERUNG, 2) > 0) {

			timewarp(HOURS(4));

			GUI_output(get_tx2(47));
		} else {

			timewarp(HOURS(6));

			GUI_output(get_tx2(49));

			answer = -1;
		}
	} else {

		if (test_skill(hero, TA_ORIENTIERUNG, 3) > 0) {

			timewarp(HOURS(6));

			GUI_output(get_tx2(48));
		} else {

			timewarp(HOURS(8));

			GUI_output(get_tx2(50));

			answer = -1;
		}
	}

	if (answer == -1) {

		if (test_skill(hero, TA_ORIENTIERUNG, 4) > 0) {

			timewarp(HOURS(4));

			GUI_output(get_tx2(51));
		} else {

			timewarp(HOURS(6));

			GUI_output(get_tx2(52));

			gs_current_loctype = LOCTYPE_WILDCAMP;
			do_location();
			gs_current_loctype = LOCTYPE_NONE;

			TRV_load_textfile(-1);

			GUI_output(get_tx2(53));
		}
	}
}

void tevent_104(void)
{
	signed short l_si;
	signed short done;
	signed short i;
	signed short nr_heroes;
	Bit8u *hero;
	signed short spell_result;

	done = 0;

	load_in_head(57);

	do {

		GUI_dialog_na(0, get_tx2(54));

		hero = get_hero(0);

		for (i = l_si = nr_heroes = 0; i <= 6; i++, hero += SIZEOF_HERO)
		{
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero))
			{
				nr_heroes++;

				/* test for HA+0 */

				if (test_attrib(hero, ATTRIB_HA, 0) > 0)
				{

					timewarp(MINUTES(30));

					sprintf(g_dtp2 + 0x400,
						get_tx2(55),
						(char*)hero + HERO_NAME2);

					GUI_dialog_na(0, (char*)((char*)(g_dtp2 + 0x400)));

					l_si++;
				}
			}
		}

		if (!l_si) {

			/* everything is fine */

			GUI_dialog_na(0, get_tx2(63));
			done = 1;

		} else if (l_si == nr_heroes) {

			/* all heroes have failed the test */

			do {
				l_si = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
							get_tx2(56), 2,
							get_tx2(57), get_tx2(58));
			} while (l_si == -1);

			if (l_si == 2) {
				/* make a camp */

				gs_current_loctype = LOCTYPE_WILDCAMP;
				do_location();
				gs_current_loctype = LOCTYPE_NONE;

				TRV_load_textfile(-1);
#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bug: The dialog picture is lost when a campfight occurs. */
				load_in_head(57);
#endif

			} else {
				/* turn around */

				ds_writew(TRV_RETURN, done = 1);
			}

		} else {

			/* some heroes, but not all, have failed the test */

			nr_heroes = 0;

			do {

				do {
					l_si = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
								(nr_heroes == 0 ? get_tx2(59) : get_tx2(87)), 3,
								get_tx2(60), get_tx2(61), get_tx2(62));
				} while (l_si == -1);

				if (l_si == 1) {

					timewarp(HOURS(2));

					GUI_dialog_na(0, get_tx2(63));

					done = 1;

				} else if (l_si == 2) {

					hero = get_hero(select_hero_ok_forced(get_ttx(317)));

					if (host_readbs(hero + HERO_TYPE) < HERO_TYPE_WITCH) {
						/* hero is not a spell user */
						GUI_output(get_ttx(330));
					} else {

						spell_result = test_spell(hero, SP_BANNBALADIN, 0);

						if (spell_result > 0) {

							sub_ae_splash(hero, get_spell_cost(SP_BANNBALADIN, 0));

							GUI_output(get_tx2(16));

							done = 1;

						} else if (spell_result != -99) {

							sub_ae_splash(hero, get_spell_cost(SP_BANNBALADIN, 1));

							nr_heroes = 1;

						} else {

							sprintf(g_dtp2,
								get_ttx(607),
								(char*)hero + HERO_NAME2);

							GUI_output(g_dtp2);
						}

						timewarp(MINUTES(30));
					}
				} else {

					timewarp(HOURS(4));

					GUI_dialog_na(0, get_tx2(63));

					done = 1;
				}

			} while (!done);
		}

	} while (!done);
}

void tevent_105(void)
{
	if (TRV_enter_hut_question()) {

		gs_current_loctype = LOCTYPE_WILDCAMP;
		do_location();
		gs_current_loctype = LOCTYPE_NONE;
	}
}

void tevent_106(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 4) > 0 && !ds_readb(TEVENT106_FLAG)) ||
		 ds_readb(TEVENT106_FLAG) != 0)
	{
		ds_writeb(TEVENT106_FLAG, 1);
		TRV_found_camp_place(1);
	}
}

void tevent_107(void)
{
	signed short i;
	signed short answer;
	Bit8u *hero;

	load_ani(1);
	draw_main_screen();
	init_ani(0);

	do {
		answer = GUI_radio(get_tx2(64), 2, get_tx2(65), get_tx2(66));

	} while (answer == -1);

	if (answer == 1) {

		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {
			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero) &&
				test_skill(hero, TA_KLETTERN, 1) <= 0)
			{

				if (get_first_hero_with_item(ITEM_ROPE) != -1) { /* TODO: ROPE_LADDER? STAFFSPELL? */

					sprintf(g_dtp2, get_tx2(70), (char*)hero + HERO_NAME2);

					sub_hero_le(hero, random_schick(11) + 1);

				} else {

					sprintf(g_dtp2,
						get_tx2(71),
						(char*)hero + HERO_NAME2);

					sub_hero_le(hero, random_schick(11) + 4);
				}

				GUI_output(g_dtp2);
			}
		}

		GUI_output(get_tx2(69));

	} else {

		if (test_skill((Bit8u*)get_first_hero_available_in_group(), TA_ORIENTIERUNG, 1) > 0) {

			timewarp(HOURS(3));

			GUI_output(get_tx2(67));

		} else {

			timewarp(HOURS(12));

			GUI_output(get_tx2(68));

		}
	}

	set_var_to_zero();
	ds_writew(REQUEST_REFRESH, 1);
}

/* Phexcaer <-> Skelellen: entrance to the dungeon 'orc cave' */
void tevent_108(void)
{
	signed short answer;

	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_SINNESSCHAERFE, 3) > 0) && !ds_readb(TEVENT108_FLAG))
	{
		ds_writeb(TEVENT108_FLAG, 1);

		GUI_dialog_na(53, get_tx2(72));

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
						get_tx2(73), 2,
						get_tx2(74), get_tx2(75));
		} while (answer == -1);

		if (answer == 1) {
			ds_writeb(TRAVEL_DETOUR, DUNGEONS_ORKBEHAUSUNG);
		}

	} else if (ds_readb(TEVENT108_FLAG) != 0) {

		load_in_head(53);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
						get_tx2(76), 2,
						get_tx2(77), get_tx2(78));
		} while (answer == -1);

		if (answer == 1) {
			ds_writeb(TRAVEL_DETOUR, DUNGEONS_ORKBEHAUSUNG);
		}
	}
}

void tevent_109(void)
{
	if ((test_skill((Bit8u*)get_first_hero_available_in_group(), TA_WILDNISLEBEN, 6) > 0 && !ds_readb(TEVENT109_FLAG)) ||
		 ds_readb(TEVENT109_FLAG) != 0)
	{
		ds_writeb(TEVENT109_FLAG, 1);
		TRV_found_camp_place(0);
	}
}

#if !defined(__BORLANDC__)
}
#endif
