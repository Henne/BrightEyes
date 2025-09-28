/**
 *	Rewrite of DSA1 v3.02_de functions of seg119 (disease effect)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg119.cpp
 *
 */

#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg120.h"


#if !defined(__BORLANDC__)
namespace M302de {
#endif

/*
 * Each disease is described by 5 consecutive bytes.
 * The first two bytes have for all diseases the same meaning.
 * Byte 0 means:	-1 = the disease has not been healed,
 *			 0 = not diseased,
 *			 1 = the disease has been healed and the hero gets better.
 *
 * Byte 1 means:	The number of days since the infection or the healing. Depends on Byte 0.
 */

/**
 * \brief   Effects of diseases
 */
void disease_effect(void)
{
	signed short i;
	signed short j;
	struct struct_hero *hero;
	struct struct_hero *hero2;
	Bit8s *disease_ptr;

	g_check_disease = 0;

	for (i = 0; i <= 6; i++) {

		if ((((struct struct_hero*)get_hero(i))->typus != HERO_TYPE_NONE) && !hero_dead(get_hero(i))) {

			hero = (struct struct_hero*)get_hero(i);

			disease_ptr = (Bit8s*)&hero->sick[ILLNESS_TYPE_WUNDFIEBER];

			/* TETANUS / WUNDFIEBER: get worse */
			if (disease_ptr[0] == -1) {

				if (disease_ptr[1] > 13) {
					/* Hero feels better after 13 days */

					disease_ptr[1] = 0;
					disease_ptr[0] = 1;

					sprintf(g_dtp2, get_ttx(561), hero->alias);
					GUI_output(g_dtp2);

				} else {
					/* Strength is fading, but only to 1 */
					sub_hero_le((Bit8u*)hero, dice_roll(2, 6, -(disease_ptr[1] - 1)));

					if (hero->attrib[ATTRIB_KK].current != 0) {

						sprintf(g_dtp2, get_ttx(572), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr[2]++;
						hero->attrib[ATTRIB_KK].current--;
					}
				}
			}

			/* TETANUS / WUNDFIEBER: get better */
			if (disease_ptr[0] == 1) {

				if (!disease_ptr[2]) {
					/* regeneration complete */
					disease_ptr[1] = 0;
					disease_ptr[0] = 0;
				} else {
					/* hero regains the lost strength */
					sprintf(g_dtp2, get_ttx(573), hero->alias);
					GUI_output(g_dtp2);

					disease_ptr[2]--;
					hero->attrib[ATTRIB_KK].current++;
				}
			}


			disease_ptr = (Bit8s*)&hero->sick[ILLNESS_TYPE_DUMPFSCHAEDEL];

			/* NUMBSKULL / DUMPFSCHAEDEL: get worse */
			if (disease_ptr[0] == -1) {

				if (!disease_ptr[4]) {

					disease_ptr[4] = 1;

					for (j = 0; j < 7; j++) {
						hero->at_weapon[j] -= 2;
						hero->pa_weapon[j] -= 2;
					}

					hero->attrib[ATTRIB_GE].current -= 2;
					hero->attrib[ATTRIB_KK].current -= 5;

					sprintf(g_dtp2, get_ttx(574), hero->alias);
					GUI_output(g_dtp2);
				}

				if (disease_ptr[1] > 2) {
					/* after two days, each other hero in the group
					 * can be infectect by a chance of 20 % */

					hero2 = (struct struct_hero*)get_hero(0);

					for (j = 0; j <= 6; j++, hero2++) {
						if ((hero2->typus != HERO_TYPE_NONE) &&
							(hero2->group_no == gs_current_group) &&
							!hero_dead((Bit8u*)hero2) &&
							(hero2 != hero) &&
							(random_schick(100) <= 20))
						{
							hero_gets_diseased(hero2, 2);
						}
					}
				}

				if (random_schick(100) < 5) {

					/* 5% chance of selfhealing */
					disease_ptr[1] = 0;
					disease_ptr[0] = 0;

					if (disease_ptr[4] != 0) {

						disease_ptr[4] = 0;

						for (j = 0; j < 7; j++) {
							hero->at_weapon[j] += 2;
							hero->pa_weapon[j] += 2;
						}

						hero->attrib[ATTRIB_GE].current += 2;
						hero->attrib[ATTRIB_KK].current += 5;
					}

					hero_gets_diseased(hero, 3);
				}

				if (disease_ptr[1] > dice_roll(1, 3, 4)) {
					disease_ptr[0] = 1;
				} else {
					sub_hero_le((Bit8u*)hero, dice_roll(1, 6, -1));
				}
			}

			/* NUMBSKULL / DUMPFSCHAEDEL: get better */
			if (disease_ptr[0] == 1) {

				disease_ptr[1] = 0;
				disease_ptr[0] = 0;

				if (disease_ptr[4] != 0) {

					disease_ptr[4] = 0;

					for (j = 0; j < 7; j++) {
						hero->at_weapon[j] += 2;
						hero->pa_weapon[j] += 2;
					}

					hero->attrib[ATTRIB_GE].current += 2;
					hero->attrib[ATTRIB_KK].current += 5;
				}

				/* regeneration complete */
				sprintf(g_dtp2, get_ttx(575), hero->alias);
				GUI_output(g_dtp2);
			}

			disease_ptr = (Bit8s*)&hero->sick[ILLNESS_TYPE_BLAUE_KEUCHE];

			/* BLUE COUGH / BLAUE KEUCHE: get worse */
			if (disease_ptr[0] == -1) {

				if (!disease_ptr[4]) {

					disease_ptr[4] = 1;

					for (j = 0; j < 7; j++) {
						hero->at_weapon[j] -= 4;
						hero->pa_weapon[j] -= 4;
					}

					disease_ptr[2] = hero->attrib[ATTRIB_KK].current / 2;
					disease_ptr[3] = hero->attrib[ATTRIB_GE].current / 2;


					hero->attrib[ATTRIB_GE].current -= disease_ptr[3];
					hero->attrib[ATTRIB_KK].current -= disease_ptr[2];

					sprintf(g_dtp2, get_ttx(577), hero->alias);
					GUI_output(g_dtp2);

				}

				if ((disease_ptr[1] > 3) && (random_schick(100) <= 25)) {

					hero->attrib[ATTRIB_KK].normal--;
					hero->attrib[ATTRIB_KK].current--;
					hero->le_max -= disease_ptr[1] / 3;
					sub_hero_le((Bit8u*)hero, disease_ptr[1] / 3);
					disease_ptr[0] = 1;

				} else {

					hero2 = (struct struct_hero*)get_hero(0);

					for (j = 0; j <= 6; j++, hero2++) {
						if ((hero2->typus != HERO_TYPE_NONE) &&	(hero2->group_no == gs_current_group) &&
							!hero_dead((Bit8u*)hero2) && (hero2 != hero) &&	(random_schick(100) <= 20))
						{
							hero_gets_diseased(hero2, 3);
						}
					}

					sub_hero_le((Bit8u*)hero, dice_roll(1, 6, 2));
				}
			}

			/* BLUE COUGH / BLAUE KEUCHE: get better */
			if (disease_ptr[0] == 1) {

				/* regeneration complete */
				sprintf(g_dtp2, get_ttx(576), hero->alias);
				GUI_output(g_dtp2);

				disease_ptr[1] = 0;
				disease_ptr[0] = 0;

				if (disease_ptr[4] != 0) {

					disease_ptr[4] = 0;

					for (j = 0; j < 7; j++) {
						hero->at_weapon[j] += 4;
						hero->pa_weapon[j] += 4;
					}

					hero->attrib[ATTRIB_GE].current += disease_ptr[3];
					hero->attrib[ATTRIB_KK].current += disease_ptr[2];
					disease_ptr[2] = disease_ptr[3] = 0;
				}
			}


			disease_ptr = (Bit8s*)&hero->sick[ILLNESS_TYPE_PARALYSE];

			/* PARALYSIS / PARALYSE: get worse */
			if (disease_ptr[0] == -1) {

				if (disease_ptr[1] > dice_roll(1, 3, 4)) {
					disease_ptr[1] = 0;
					disease_ptr[0] = 1;
				} else {
					j = random_schick(6);
					disease_ptr[3] += j;
					hero->attrib[ATTRIB_GE].current -= j;

					j = random_schick(6);
					disease_ptr[2] += j;
					hero->attrib[ATTRIB_KK].current -= j;

					sprintf(g_dtp2, get_ttx(579), hero->alias);
					GUI_output(g_dtp2);
				}
			}

			/* PARALYSIS / PARALYSE: get better */
			if (disease_ptr[0] == 1) {

				if (!disease_ptr[2] && !disease_ptr[3]) {

					disease_ptr[1] = 0;
					disease_ptr[0] = 0;

				} else {

					if (!disease_ptr[2]) {

						sprintf(g_dtp2, get_ttx(573), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr[2]--;
						hero->attrib[ATTRIB_KK].current++;
					}

					if (!disease_ptr[3]) {

						sprintf(g_dtp2, get_ttx(578), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr[3]--;
						hero->attrib[ATTRIB_GE].current++;
					}
				}
			}


			disease_ptr = (Bit8s*)&hero->sick[ILLNESS_TYPE_SCHLACHTENFIEBER];

			/* BATTLEFIELD FEVER / SCHLACHTFELDFIEBER: get worse */
			if (disease_ptr[0] == -1) {

				if (disease_ptr[1] > 7) {

					/* 30 % for elfes, 20% for the all other types */
					if (random_schick(100) <= (hero->typus >= HERO_TYPE_GREEN_ELF ? 30 : 20)) {

						sprintf(g_dtp2, get_ttx(580), hero->alias);
						GUI_output(g_dtp2);

						sub_hero_le((Bit8u*)hero, 1000);

					} else {

						if (disease_ptr[4]) {
							disease_ptr[0] = 1;
						}
					}
				} else {

					if (disease_ptr[1] > 3) {

						if (!disease_ptr[4]) {

							disease_ptr[4] = 1;
							hero->attrib[ATTRIB_KK].current -= 5;

							sprintf(g_dtp2, get_ttx(581), hero->alias, GUI_get_ptr(hero->sex, 0));
							GUI_output(g_dtp2);
						}

						for (j = 0; j <= 6; j++) {

							hero2 = (struct struct_hero*)get_hero(j);

							if ((hero2->typus != HERO_TYPE_NONE) &&	(hero2->group_no == gs_current_group) &&
								!hero_dead((Bit8u*)hero2) && (hero2 != hero) && (random_schick(100) <= 5))
							{
								hero_gets_diseased(hero2, 5);
							}
						}

						sub_hero_le((Bit8u*)hero, dice_roll(hero->typus >= HERO_TYPE_GREEN_ELF ? 2 : 1, 6, disease_ptr[1] - 1));
					}
				}
			}

			/* BATTLEFIELD FEVER / SCHLACHTFELDFIEBER: get better */
			if (disease_ptr[0] == 1) {

				if (disease_ptr[4]) {

					disease_ptr[4] = 0;
					disease_ptr[1] = 0;
					hero->attrib[ATTRIB_KK].current += 5;
				}

				sprintf(g_dtp2, get_ttx(582), hero->alias);
				GUI_output(g_dtp2);
			}

			disease_ptr = (Bit8s*)&hero->sick[ILLNESS_TYPE_FROSTSCHAEDEN];

			/* FROSTBITE / FROSTSCHAEDEN: get worse */
			if (disease_ptr[0] == -1) {

				j = 0;

				if (random_schick(100) <= disease_ptr[1] * 5) {

					hero->attrib[ATTRIB_GE].current--;
					hero->attrib[ATTRIB_GE].normal--;

					j = 1;

					sprintf(g_dtp2, get_ttx(583), hero->alias);
					GUI_output(g_dtp2);
				}

				sub_hero_le((Bit8u*)hero, dice_roll(1, 6, 0));

				if (hero->attrib[ATTRIB_KK].current != 0) {

					disease_ptr[2]++;
					hero->attrib[ATTRIB_KK].current--;
				}

				if (hero->attrib[ATTRIB_GE].current != 0) {

					disease_ptr[3]++;
					hero->attrib[ATTRIB_GE].current--;
				}

				if (j == 0) {

					sprintf(g_dtp2, get_ttx(740), hero->alias);
					GUI_output(g_dtp2);
				}
			}

			/* FROSTBITE / FROSTSCHAEDEN: get better */
			if (disease_ptr[0] == 1) {

				if (!disease_ptr[2] && !disease_ptr[3]) {
					disease_ptr[1] = 0;
					disease_ptr[0] = 0;
				} else {
					if (!disease_ptr[2]) {

						sprintf(g_dtp2, get_ttx(573), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr[2]--;
						hero->attrib[ATTRIB_KK].current++;
					}

					if (!disease_ptr[3]) {

						sprintf(g_dtp2, get_ttx(578), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr[3]--;
						hero->attrib[ATTRIB_GE].current++;
					}
				}
			}

			disease_ptr = (Bit8s*)&hero->sick[ILLNESS_TYPE_TOLLWUT];

			/* RABIES / TOLLWUT: get worse */
			if (disease_ptr[0] == -1) {

				if (disease_ptr[1] > dice_roll(1, 6, 6)) {

					disease_ptr[1] = 0;
					disease_ptr[0] = 1;

				} else {

					hero2 = (struct struct_hero*)get_hero(0);

					for (j = 0; j <= 6; j++, hero2++) {


						if ((hero2->typus != HERO_TYPE_NONE) &&
							(hero2->group_no == gs_current_group) &&
							!hero_dead((Bit8u*)hero2) &&
							(hero2 != hero) &&
							(random_schick(100) <= 10))
						{
							hero_gets_diseased(hero2, 7);
						}
					}

					sub_hero_le((Bit8u*)hero, dice_roll((disease_ptr[1] > 3 ? 3 : disease_ptr[1]), 6, 0));

					disease_ptr[2] += 2;
					hero->attrib[ATTRIB_KK].current -= 2;

					if (disease_ptr[1] > 2) {

						rabies(hero, i);

					} else {

						sprintf(g_dtp2, get_ttx(572), hero->alias);
						GUI_output(g_dtp2);
					}
				}
			}

			/* RABIES / TOLLWUT: get better */
			if (disease_ptr[0] == 1) {

				if (!disease_ptr[2]) {

					disease_ptr[1] = 0;
					disease_ptr[0] = 0;

				} else {
					sprintf(g_dtp2, get_ttx(573), hero->alias);
					GUI_output(g_dtp2);

					disease_ptr[2]--;
					hero->attrib[ATTRIB_KK].current++;
				}
			}

			/* increment day timer for all diseases */
			for (j = 1; j <= 7; j++) {

				if (hero->sick[j][0] != 0) {
					hero->sick[j][1]++;
				}
			}

			/* kill hero if KK <= 0 */
			if (hero->attrib[ATTRIB_KK].current <= 0) {

				hero->attrib[ATTRIB_KK].current = hero->attrib[ATTRIB_KK].normal;

				sub_hero_le((Bit8u*)hero, 5000);
			}

			/* kill hero if GE <= 0 */
			if (hero->attrib[ATTRIB_GE].current <= 0) {

				hero->attrib[ATTRIB_GE].current = hero->attrib[ATTRIB_GE].normal;

				sub_hero_le((Bit8u*)hero, 5000);
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
