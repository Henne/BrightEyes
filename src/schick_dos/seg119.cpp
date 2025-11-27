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
	signed int i;
	signed int j; /* multi use: hero_pos, disease_id */
	struct struct_hero *hero;
	struct struct_hero *hero2;
	struct hero_affliction *disease_ptr;

	g_check_disease = 0;

	for (i = 0; i <= 6; i++) {

		if ((get_hero(i)->typus != HERO_TYPE_NONE) && !get_hero(i)->flags.dead) {

			hero = get_hero(i);

			disease_ptr = &hero->disease[DISEASE_ID_WUNDFIEBER];

			/* TETANUS / WUNDFIEBER: get worse */
			if (disease_ptr->status == DISEASE_STATUS_DISEASED) {

				if (disease_ptr->time_counter > 13) {
					/* Hero feels better after 13 days */

					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_RECOVER;

					sprintf(g_dtp2, get_ttx(561), hero->alias);
					GUI_output(g_dtp2);

				} else {
					/* LE loss of 2D6 + 1 - (number of days the hero is diseased) */
					/* so the effect is worst at the beginning and gets better over time */
					sub_hero_le(hero, dice_roll(2, 6, -(disease_ptr->time_counter - 1)));

					/* Strength is fading, but only to 1 */
					if (hero->attrib[ATTRIB_KK].current != 0) {

						sprintf(g_dtp2, get_ttx(572), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr->log_1++;
						hero->attrib[ATTRIB_KK].current--;
					}
				}
			}

			/* TETANUS / WUNDFIEBER: get better */
			if (disease_ptr->status == DISEASE_STATUS_RECOVER) {

				if (!disease_ptr->log_1) {
					/* regeneration complete */
					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_HEALTHY;
				} else {
					/* hero regains the lost strength */
					sprintf(g_dtp2, get_ttx(573), hero->alias);
					GUI_output(g_dtp2);

					disease_ptr->log_1--;
					hero->attrib[ATTRIB_KK].current++;
				}
			}


			disease_ptr = &hero->disease[DISEASE_ID_DUMPFSCHAEDEL];

			/* NUMBSKULL / DUMPFSCHAEDEL: get worse */
			if (disease_ptr->status == DISEASE_STATUS_DISEASED) {

				if (!disease_ptr->log_3) {

					disease_ptr->log_3 = 1;

					for (j = 0; j < 7; j++) {
						hero->at_talent_bonus[j] -= 2;
						hero->pa_talent_bonus[j] -= 2;
					}

					hero->attrib[ATTRIB_GE].current -= 2;
					hero->attrib[ATTRIB_KK].current -= 5;

					sprintf(g_dtp2, get_ttx(574), hero->alias);
					GUI_output(g_dtp2);
				}

				if (disease_ptr->time_counter > 2) {
					/* after two days, each other hero in the group
					 * can be infectect by a chance of 20 % */

					hero2 = get_hero(0);

					for (j = 0; j <= 6; j++, hero2++) {
						if ((hero2->typus != HERO_TYPE_NONE) &&
							(hero2->group_id == gs_active_group_id) &&
							!hero2->flags.dead &&
							(hero2 != hero) &&
							(random_schick(100) <= 20))
						{
							hero_gets_diseased(hero2, DISEASE_ID_DUMPFSCHAEDEL);
						}
					}
				}

				if (random_schick(100) < 5) {
					/* 5% chance of worsening to Blaue Keuche */

					/* remove the effect of Dumpfschaedel */
					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_HEALTHY;

					if (disease_ptr->log_3 != 0) {

						disease_ptr->log_3 = 0;

						for (j = 0; j < 7; j++) {
							hero->at_talent_bonus[j] += 2;
							hero->pa_talent_bonus[j] += 2;
						}

						hero->attrib[ATTRIB_GE].current += 2;
						hero->attrib[ATTRIB_KK].current += 5;
					}

					/* infect with the worse Blaue Keuche */
					hero_gets_diseased(hero, DISEASE_ID_BLAUE_KEUCHE);
				}

				if (disease_ptr->time_counter > dice_roll(1, 3, 4)) { /* number of days infected > D3 + 4 */
					// Original-Bug? forgot? disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_RECOVER;
				} else {
					sub_hero_le(hero, dice_roll(1, 6, -1)); /* LE loss of D6 - 1 */
				}
			}

			/* NUMBSKULL / DUMPFSCHAEDEL: get better */
			if (disease_ptr->status == DISEASE_STATUS_RECOVER) {

				disease_ptr->time_counter = 0;
				disease_ptr->status = DISEASE_STATUS_HEALTHY;

				if (disease_ptr->log_3 != 0) {

					disease_ptr->log_3 = 0;

					for (j = 0; j < 7; j++) {
						hero->at_talent_bonus[j] += 2;
						hero->pa_talent_bonus[j] += 2;
					}

					hero->attrib[ATTRIB_GE].current += 2;
					hero->attrib[ATTRIB_KK].current += 5;
				}

				/* regeneration complete */
				sprintf(g_dtp2, get_ttx(575), hero->alias);
				GUI_output(g_dtp2);
			}

			disease_ptr = &hero->disease[DISEASE_ID_BLAUE_KEUCHE];

			/* BLUE COUGH / BLAUE KEUCHE: get worse */
			if (disease_ptr->status == DISEASE_STATUS_DISEASED) {

				if (!disease_ptr->log_3) {

					disease_ptr->log_3 = 1;

					for (j = 0; j < 7; j++) {
						hero->at_talent_bonus[j] -= 4;
						hero->pa_talent_bonus[j] -= 4;
					}

					disease_ptr->log_1 = hero->attrib[ATTRIB_KK].current / 2;
					disease_ptr->log_2 = hero->attrib[ATTRIB_GE].current / 2;


					hero->attrib[ATTRIB_GE].current -= disease_ptr->log_2;
					hero->attrib[ATTRIB_KK].current -= disease_ptr->log_1;

					sprintf(g_dtp2, get_ttx(577), hero->alias);
					GUI_output(g_dtp2);

				}

				if ((disease_ptr->time_counter > 3) && (random_schick(100) <= 25)) {

					hero->attrib[ATTRIB_KK].normal--;
					hero->attrib[ATTRIB_KK].current--;
					hero->le_max -= disease_ptr->time_counter / 3;
					sub_hero_le(hero, disease_ptr->time_counter / 3);
					// Original-Bug? forgot? disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_RECOVER;

				} else {

					hero2 = get_hero(0);

					for (j = 0; j <= 6; j++, hero2++) {
						if ((hero2->typus != HERO_TYPE_NONE) &&	(hero2->group_id == gs_active_group_id) &&
							!hero2->flags.dead && (hero2 != hero) && (random_schick(100) <= 20))
						{
							hero_gets_diseased(hero2, DISEASE_ID_BLAUE_KEUCHE);
						}
					}

					sub_hero_le(hero, dice_roll(1, 6, 2));
				}
			}

			/* BLUE COUGH / BLAUE KEUCHE: get better */
			if (disease_ptr->status == DISEASE_STATUS_RECOVER) {

				/* Original-Bug 48:
				 * When recovering from blaue Keuche, the message
				 * "Das Leben wird wohl nie wieder sein, wie es war!"
				 * ("Life will probably never be the same!")
				 * is shown whether or not a permanent attribute loss actually occurred.
				 */
#ifndef M302de_ORIGINAL_BUGFIX
				/* regeneration complete */
				sprintf(g_dtp2, get_ttx(576), hero->alias);
				// %s HAT DIE KRANKHEIT ÜBERWUNDEN, DOCH DAS LEBEN WIRD WOHL NIE WIEDER SEIN, WIE ES WAR!
				GUI_output(g_dtp2);
#endif

				disease_ptr->time_counter = 0;
				disease_ptr->status = DISEASE_STATUS_HEALTHY;

				if (disease_ptr->log_3 != 0) {
					/* permanent attribute loss happened. */

					disease_ptr->log_3 = 0;

					for (j = 0; j < 7; j++) {
						hero->at_talent_bonus[j] += 4;
						hero->pa_talent_bonus[j] += 4;
					}

					hero->attrib[ATTRIB_GE].current += disease_ptr->log_2;
					hero->attrib[ATTRIB_KK].current += disease_ptr->log_1;
					disease_ptr->log_1 = disease_ptr->log_2 = 0;
#ifdef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 48, see above */
					sprintf(g_dtp2, get_ttx(576), hero->alias);
					// %s HAT DIE KRANKHEIT ÜBERWUNDEN, DOCH DAS LEBEN WIRD WOHL NIE WIEDER SEIN, WIE ES WAR!
				} else {
					/* permanent attribute loss did not happen. */

					sprintf(g_dtp2, "%s HAT DIE KRANKHEIT ""\x9a""BERWUNDEN.", hero->alias);
#endif

				}

#ifdef M302de_ORIGINAL_BUGFIX
				GUI_output(g_dtp2);
#endif
			}


			disease_ptr = &hero->disease[DISEASE_ID_PARALYSE];

			/* PARALYSIS / PARALYSE: get worse */
			if (disease_ptr->status == DISEASE_STATUS_DISEASED) {

				if (disease_ptr->time_counter > dice_roll(1, 3, 4)) {
					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_RECOVER;
				} else {
					j = random_schick(6);
					disease_ptr->log_2 += j;
					hero->attrib[ATTRIB_GE].current -= j;

					j = random_schick(6);
					disease_ptr->log_1 += j;
					hero->attrib[ATTRIB_KK].current -= j;

					sprintf(g_dtp2, get_ttx(579), hero->alias);
					GUI_output(g_dtp2);
				}
			}

			/* PARALYSIS / PARALYSE: get better */
			if (disease_ptr->status == DISEASE_STATUS_RECOVER) {

				if (!disease_ptr->log_1 && !disease_ptr->log_2) {

					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_HEALTHY;

				} else {

					if
#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 44:
					 * Recover from Paralyse and Frostschaden does not work, GE and KK are not recovered.
					 * In the two if statements below, the condition is reversed. */
					(!disease_ptr->log_1)
#else
					(disease_ptr->log_1)
#endif
					{

						sprintf(g_dtp2, get_ttx(573), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr->log_1--;
						hero->attrib[ATTRIB_KK].current++;
					}

					if
#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 44, see above. */
					(!disease_ptr->log_2)
#else
					(disease_ptr->log_2)
#endif
					{

						sprintf(g_dtp2, get_ttx(578), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr->log_2--;
						hero->attrib[ATTRIB_GE].current++;
					}
				}
			}


			disease_ptr = &hero->disease[DISEASE_ID_SCHLACHTENFIEBER];

			/* BATTLEFIELD FEVER / SCHLACHTFELDFIEBER: get worse */
			if (disease_ptr->status == DISEASE_STATUS_DISEASED) {

				if (disease_ptr->time_counter > 7) {

					/* 30 % for elves, 20% for the all other types */
					if (random_schick(100) <= (hero->typus >= HERO_TYPE_AUELF ? 30 : 20)) {

						sprintf(g_dtp2, get_ttx(580), hero->alias);
						GUI_output(g_dtp2);

						sub_hero_le(hero, 1000);

					} else {

						if (disease_ptr->log_3) {
							// Original-Bug? forgot? disease_ptr->time_counter = 0;
							disease_ptr->status = DISEASE_STATUS_RECOVER;
						}
					}
				} else {

					if (disease_ptr->time_counter > 3) {

						if (!disease_ptr->log_3) {

							disease_ptr->log_3 = 1;
							hero->attrib[ATTRIB_KK].current -= 5;

							sprintf(g_dtp2, get_ttx(581), hero->alias, GUI_get_ptr(hero->sex, 0));
							GUI_output(g_dtp2);
						}

						for (j = 0; j <= 6; j++) {

							hero2 = get_hero(j);

							if ((hero2->typus != HERO_TYPE_NONE) &&	(hero2->group_id == gs_active_group_id) &&
								!hero2->flags.dead && (hero2 != hero) && (random_schick(100) <= 5))
							{
								hero_gets_diseased(hero2, DISEASE_ID_SCHLACHTENFIEBER);
							}
						}

						sub_hero_le(hero, dice_roll(hero->typus >= HERO_TYPE_AUELF ? 2 : 1, 6, disease_ptr->time_counter - 1));
					}
				}
			}

			/* BATTLEFIELD FEVER / SCHLACHTFELDFIEBER: get better */
			if (disease_ptr->status == DISEASE_STATUS_RECOVER) {

				/* Original-Bug 45: It is impossible to fully recover from Schlachtenfieber.
				 * There is no way to get the status back to DISEASE_STATUS_HEALTHY. */
#ifndef M302de_ORIGINAL_BUGFIX
				if (disease_ptr->log_3) {

					disease_ptr->log_3 = 0;
					disease_ptr->time_counter = 0;
					hero->attrib[ATTRIB_KK].current += 5;
				}

				sprintf(g_dtp2, get_ttx(582), hero->alias);
				// DIE GELBLICHE FÄRBUNG VON %sS LIPPEN GEHT ZURÜCK.
				GUI_output(g_dtp2);
#else
				if (disease_ptr->log_3) {
					disease_ptr->log_3 = 0;
					hero->attrib[ATTRIB_KK].current += 5;
				} else {
					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_HEALTHY;
					sprintf(g_dtp2, get_ttx(582), hero->alias);
					// DIE GELBLICHE FÄRBUNG VON %sS LIPPEN GEHT ZURÜCK.
					GUI_output(g_dtp2);
				}
#endif
			}

			disease_ptr = &hero->disease[DISEASE_ID_FROSTSCHAEDEN];

			/* FROSTBITE / FROSTSCHAEDEN: get worse */
			if (disease_ptr->status == DISEASE_STATUS_DISEASED) {

				j = 0;

				/* ((number of days * 5) + 1)% chance for *permanent* GE-1 */
				if (random_schick(100) <= disease_ptr->time_counter * 5) {

					hero->attrib[ATTRIB_GE].current--;
					hero->attrib[ATTRIB_GE].normal--;

					j = 1;

					sprintf(g_dtp2, get_ttx(583), hero->alias);
					GUI_output(g_dtp2);
				}

				sub_hero_le(hero, dice_roll(1, 6, 0));

				if (hero->attrib[ATTRIB_KK].current != 0) {

					disease_ptr->log_1++;
					hero->attrib[ATTRIB_KK].current--;
				}

				if (hero->attrib[ATTRIB_GE].current != 0) {

					disease_ptr->log_2++;
					hero->attrib[ATTRIB_GE].current--;
				}

				if (j == 0) {

					sprintf(g_dtp2, get_ttx(740), hero->alias);
					GUI_output(g_dtp2);
				}
			}

			/* FROSTBITE / FROSTSCHAEDEN: get better */
			if (disease_ptr->status == DISEASE_STATUS_RECOVER) {

				if (!disease_ptr->log_1 && !disease_ptr->log_2) {
					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_HEALTHY;
				} else {
					if
#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 44:
					 * Recover from Paralyse and Frostschaeden does not work, GE and KK are not recovered.
					 * In the two if statements below, the condition is reversed. */
					(!disease_ptr->log_1)
#else
					(disease_ptr->log_1)
#endif
					{

						sprintf(g_dtp2, get_ttx(573), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr->log_1--;
						hero->attrib[ATTRIB_KK].current++;
					}

					if
#ifndef M302de_ORIGINAL_BUGFIX
					/* Original-Bug 44, see above. */
					(!disease_ptr->log_2)
#else
					(disease_ptr->log_2)
#endif
					{

						sprintf(g_dtp2, get_ttx(578), hero->alias);
						GUI_output(g_dtp2);

						disease_ptr->log_2--;
						hero->attrib[ATTRIB_GE].current++;
					}
				}
			}

			disease_ptr = &hero->disease[DISEASE_ID_TOLLWUT];

			/* RABIES / TOLLWUT: get worse */
			if (disease_ptr->status == DISEASE_STATUS_DISEASED) {

				if (disease_ptr->time_counter > dice_roll(1, 6, 6)) {

					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_RECOVER;

				} else {

					hero2 = get_hero(0);

					for (j = 0; j <= 6; j++, hero2++) {


						if ((hero2->typus != HERO_TYPE_NONE) &&
							(hero2->group_id == gs_active_group_id) &&
							!hero2->flags.dead &&
							(hero2 != hero) &&
							(random_schick(100) <= 10))
						{
							hero_gets_diseased(hero2, DISEASE_ID_TOLLWUT);
						}
					}

					sub_hero_le(hero, dice_roll((disease_ptr->time_counter > 3 ? 3 : disease_ptr->time_counter), 6, 0));

					disease_ptr->log_1 += 2;
					hero->attrib[ATTRIB_KK].current -= 2;

					if (disease_ptr->time_counter > 2) {

						rabies(hero, i);

					} else {

						sprintf(g_dtp2, get_ttx(572), hero->alias);
						GUI_output(g_dtp2);
					}
				}
			}

			/* RABIES / TOLLWUT: get better */
			if (disease_ptr->status == DISEASE_STATUS_RECOVER) {

				if (!disease_ptr->log_1) {

					disease_ptr->time_counter = 0;
					disease_ptr->status = DISEASE_STATUS_HEALTHY;

				} else {
					sprintf(g_dtp2, get_ttx(573), hero->alias);
					GUI_output(g_dtp2);

					disease_ptr->log_1--;
					hero->attrib[ATTRIB_KK].current++;
				}
			}

			/* increment day timer for all diseases */
			for (j = 1; j <= 7; j++) {

				if (hero->disease[j].status != DISEASE_STATUS_HEALTHY) {
					/* Original-Bug? I think for some of the deseases, an overflow could theoretically happen. */
					hero->disease[j].time_counter++;
				}
			}

			/* kill hero if KK <= 0 */
			if (hero->attrib[ATTRIB_KK].current <= 0) {

				hero->attrib[ATTRIB_KK].current = hero->attrib[ATTRIB_KK].normal;

				sub_hero_le(hero, 5000);
			}

			/* kill hero if GE <= 0 */
			if (hero->attrib[ATTRIB_GE].current <= 0) {

				hero->attrib[ATTRIB_GE].current = hero->attrib[ATTRIB_GE].normal;

				sub_hero_le(hero, 5000);
			}
		}
	}
}
