/**
 *	Rewrite of DSA1 v3.02_de functions of seg121 (poison effect)
 *	Functions rewritten: 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg121.cpp
 */

#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg097.h"

void poison_effect(void)
{
	signed int hero_pos;
	volatile signed int j; /* multi use: weapon_type, hero_pos, poison_id */
	struct struct_hero *hero;
	struct hero_affliction *poison_ptr;

	g_check_poison = 0;

	hero = get_hero(0);

	for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && !hero->flags.dead) {


			poison_ptr = &hero->poison[POISON_ID_SHURINKNOLLENGIFT];

			/* SHURINKNOLLENGIFT: hero is poisoned */
			/* After half an hour, every 5 minutes KK-1, GE-1, LE - 1D6. */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				if (poison_ptr->time_counter >= 6) {

					/* KK */
					if (hero->attrib[ATTRIB_KK].current) {

						sprintf(g_dtp2, get_ttx(572), hero->alias);

						GUI_output(g_dtp2);

						poison_ptr->log_1++;
						hero->attrib[ATTRIB_KK].current--;
					}

					/* GE */
					if (hero->attrib[ATTRIB_GE].current) {

						sprintf(g_dtp2, get_ttx(45), hero->alias);

						GUI_output(g_dtp2);

						poison_ptr->log_2++;
						hero->attrib[ATTRIB_GE].current--;
					}

					/* The following is redundant, there is a check for <= 0 at the end of this function. */
					if ((hero->attrib[ATTRIB_KK].current < 0) || (hero->attrib[ATTRIB_GE].current < 0)) {
						sub_hero_le(hero, 1000);
					}
				}

				/* Loose D6 LE */
				sub_hero_le(hero, random_schick(6));
			}

			/* SHURINKNOLLENGIFT: hero regenerates */
			if (poison_ptr->status == POISON_STATUS_RECOVER) {

				if (!poison_ptr->log_1 && !poison_ptr->log_2) {

					poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_HEALTHY;
				}

				/* every hour, regenerate 1 KK */
				if ((poison_ptr->log_1 != 0) && (!poison_ptr->time_counter % 12 )) {

					sprintf(g_dtp2, get_ttx(573), hero->alias);

					GUI_output(g_dtp2);

					poison_ptr->log_1--;
					hero->attrib[ATTRIB_KK].current++;
				}

				/* every hour, regenerate 1 GE */
				if ((poison_ptr->log_2 != 0) && (!poison_ptr->time_counter % 12 )) {

					sprintf(g_dtp2, get_ttx(578), hero->alias);

					GUI_output(g_dtp2);

					poison_ptr->log_2--;
					hero->attrib[ATTRIB_GE].current++;
				}
			}


			poison_ptr = &hero->poison[POISON_ID_ARAX];

			/* ARAXGIFT: hero is poisoned */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				/* once after 10 minutes: AT-2 and PA-2 for all weapon talents, moreover GE-2 and KK-2 */
				if ((poison_ptr->time_counter >= 2) && !poison_ptr->log_3) {

					poison_ptr->log_3 = 1;

					for (j = 0; j < 7; j++) {
						/* all AT - 2 */
						hero->at_talent_bonus[j] -= 2;
						/* all PA - 2 */
						hero->pa_talent_bonus[j] -= 2;
					}

					/* GE - 2 */
					hero->attrib[ATTRIB_GE].current = hero->attrib[ATTRIB_GE].current - 2;
					/* KK - 2 */
					hero->attrib[ATTRIB_KK].current = hero->attrib[ATTRIB_KK].current - 2;

					sprintf(g_dtp2, get_ttx(37), hero->alias);

					GUI_output(g_dtp2);

				}

				/* after 10 hours: regenerate */
				if (poison_ptr->time_counter >= 120) {
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_RECOVER;
				}
			}

			/* ARAXGIFT: regeneration */
			if ((poison_ptr->status == POISON_STATUS_RECOVER) && poison_ptr->log_3) {

				poison_ptr->status = POISON_STATUS_HEALTHY;
				poison_ptr->time_counter = 0;
				poison_ptr->log_3 = 0;

				for (j = 0; j < 7; j++) {
					/* all AT + 2 */
					hero->at_talent_bonus[j] += 2;
					/* all PA + 2 */
					hero->pa_talent_bonus[j] += 2;
				}

				/* GE + 2 */
				hero->attrib[ATTRIB_GE].current = hero->attrib[ATTRIB_GE].current + 2;
				/* KK + 2 */
				hero->attrib[ATTRIB_KK].current = hero->attrib[ATTRIB_KK].current + 2;

				sprintf(g_dtp2, get_ttx(38), hero->alias);

				GUI_output(g_dtp2);
			}


			poison_ptr = &hero->poison[POISON_ID_ANGSTGIFT];

			/* ANGSTGIFT: hero is poisoned */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				if (!poison_ptr->log_3) {

					poison_ptr->log_3 = 1;

					/* MU - 2 */
					hero->attrib[ATTRIB_MU].current = hero->attrib[ATTRIB_MU].current - 2;

					/* all negative attributes + 2 */
					hero->attrib[ATTRIB_AG].current = hero->attrib[ATTRIB_AG].current + 2;
					hero->attrib[ATTRIB_HA].current = hero->attrib[ATTRIB_HA].current + 2;
					hero->attrib[ATTRIB_RA].current = hero->attrib[ATTRIB_RA].current + 2;
					hero->attrib[ATTRIB_GG].current = hero->attrib[ATTRIB_GG].current + 2;
					hero->attrib[ATTRIB_TA].current = hero->attrib[ATTRIB_TA].current + 2;
					hero->attrib[ATTRIB_NG].current = hero->attrib[ATTRIB_NG].current + 2;
					hero->attrib[ATTRIB_JZ].current = hero->attrib[ATTRIB_JZ].current + 2;
					hero->flags.scared = 1;

					sprintf(g_dtp2, get_ttx(39), hero->alias);

					GUI_output(g_dtp2);
				}

				/* self regeneration */
				if (poison_ptr->time_counter >= 7) {
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_RECOVER;
				}
			}

			/* ANGSTGIFT: regeneration */
			if ((poison_ptr->status == POISON_STATUS_RECOVER) && poison_ptr->log_3) {

					poison_ptr->status = POISON_STATUS_HEALTHY;
					poison_ptr->time_counter = 0;
					poison_ptr->log_3 = 0;

					/* MU + 2 */
					hero->attrib[ATTRIB_MU].current = hero->attrib[ATTRIB_MU].current + 2;

					/* all negative attributes - 2 */
					hero->attrib[ATTRIB_AG].current = hero->attrib[ATTRIB_AG].current - 2;
					hero->attrib[ATTRIB_HA].current = hero->attrib[ATTRIB_HA].current - 2;
					hero->attrib[ATTRIB_RA].current = hero->attrib[ATTRIB_RA].current - 2;
					hero->attrib[ATTRIB_GG].current = hero->attrib[ATTRIB_GG].current - 2;
					hero->attrib[ATTRIB_TA].current = hero->attrib[ATTRIB_TA].current - 2;
					hero->attrib[ATTRIB_NG].current = hero->attrib[ATTRIB_NG].current - 2;
					hero->attrib[ATTRIB_JZ].current = hero->attrib[ATTRIB_JZ].current - 2;

					hero->flags.scared = 0;

					sprintf(g_dtp2, get_ttx(40), hero->alias);

					GUI_output(g_dtp2);
			}


			poison_ptr = &hero->poison[POISON_ID_SCHLAFGIFT];

			/* SCHLAFGIFT: hero is poisoned */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				if (!poison_ptr->log_3) {

					poison_ptr->log_3 = 1;

					hero->flags.asleep = 1;

					sprintf(g_dtp2, get_ttx(41), hero->alias);

					GUI_output(g_dtp2);
				}

				/* self regeneration */
				if (poison_ptr->time_counter >= 20) {
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_RECOVER;
				}
			}

			/* SCHLAFGIFT: regeneration */
			if ((poison_ptr->status == POISON_STATUS_RECOVER) && poison_ptr->log_3) {

				poison_ptr->status = POISON_STATUS_HEALTHY;
				poison_ptr->time_counter = 0;
				poison_ptr->log_3 = 0;

				hero->flags.asleep = 0;

				sprintf(g_dtp2, get_ttx(42), hero->alias);

				GUI_output(g_dtp2);
			}


			poison_ptr = &hero->poison[POISON_ID_GOLDLEIM];

			/* GOLDLEIM: hero gets poisoned */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				if (poison_ptr->time_counter >= dice_roll(2, 6, 0) * 12) {
					/* for Goldleim, no recovering phase is needed. */
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_HEALTHY;
				}

				/* every hour, an LE loss of 2 D6 - 3 */
				if (!(poison_ptr->time_counter % 12)) {
					sub_hero_le(hero, dice_roll(2, 6, -3));
				}
			}

			/* GOLDLEIM: regeneration */
			if (poison_ptr->status == POISON_STATUS_RECOVER) {
				// Original-Bug? forgot? poison_ptr->time_counter = 0;
				poison_ptr->status = POISON_STATUS_HEALTHY;
			}


			poison_ptr = &hero->poison[POISON_ID_KROETENSCHEMEL];

			/* KROETENSCHEMEL: hero gets poisoned */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				if (poison_ptr->time_counter >= 48) {
					/* for Krötenschemel, no recovering phase is needed. */
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_HEALTHY;
				}

				/* every hour, an LE loss of D6 + 2 */
				if (!(poison_ptr->time_counter % 12)) {
					sub_hero_le(hero, dice_roll(1, 6, 2));
				}
			}

			/* KROETENSCHEMEL: regeneration */
			if (poison_ptr->status == POISON_STATUS_RECOVER) {
				// Original-Bug? forgot? poison_ptr->time_counter = 0;
				poison_ptr->status = POISON_STATUS_HEALTHY;
			}


			poison_ptr = &hero->poison[POISON_ID_LOTUSGIFT];

			/* LOTUSGIFT: hero gets poisoned */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				if (poison_ptr->time_counter >= 24) {
					/* for Lotusgift, no recovering phase is needed. */
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_HEALTHY;
				}

				sub_hero_le(hero, dice_roll(2, 6, 0));
			}

			/* LOTUSGIFT: regeneration */
			if (poison_ptr->status == POISON_STATUS_RECOVER) {
				// Original-Bug? forgot? poison_ptr->time_counter = 0;
				poison_ptr->status = POISON_STATUS_HEALTHY;
			}


			poison_ptr = &hero->poison[POISON_ID_KUKRIS];

			/* KUKRIS: hero gets poisoned */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				if (poison_ptr->time_counter >= 3) {
					/* for Kukris, no recovering phase is needed. */
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_HEALTHY;
				}

				sub_hero_le(hero, dice_roll(50, 6, 0));
			}

			/* KUKRIS: regeneration */
			if (poison_ptr->status == POISON_STATUS_RECOVER) {
				// Original-Bug? forgot? poison_ptr->time_counter = 0;
				poison_ptr->status = POISON_STATUS_HEALTHY;
			}


			poison_ptr = &hero->poison[POISON_ID_BANNSTAUB];

			/* BANNSTAUB: hero gets poisoned */
			if (poison_ptr->status == POISON_STATUS_POISONED) {

				if (poison_ptr->time_counter >= dice_roll(3, 6, 0)) {
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_RECOVER;
				}

				j = dice_roll(1, 6, 2);

				if (hero->ae >= j) {

					/* loose D6 + 2 AE */
					poison_ptr->log_1 = poison_ptr->log_1 + j;

					hero->ae -= j;

					sprintf(g_dtp2, get_ttx(43), hero->alias, j);

					GUI_output(g_dtp2);
				}
			}

			/* BANNSTAUB: regeneration */
			if (poison_ptr->status == POISON_STATUS_RECOVER) {

				if (!poison_ptr->log_1) {
					// Original-Bug? forgot? poison_ptr->time_counter = 0;
					poison_ptr->status = POISON_STATUS_HEALTHY;
				} else {

					/* every hour, regenerate one AE */
					if (!poison_ptr->time_counter % 12) {

						poison_ptr->log_1--;
						hero->ae++;

						sprintf(g_dtp2, get_ttx(44), hero->alias);

						GUI_output(g_dtp2);
					}
				}
			}


			for (j = 1; j <= 9; j++) {

				if (hero->poison[j].status != POISON_STATUS_HEALTHY) {
					/* Original-Bug? For Goldleim, an overflow could theoretically happen. */
					hero->poison[j].time_counter++;
				}
			}

			/* KK <= 0 */
			if (hero->attrib[ATTRIB_KK].current <= 0) {

				/* reset KK */
				hero->attrib[ATTRIB_KK].current = hero->attrib[ATTRIB_KK].normal;
				/* die */
				sub_hero_le(hero, 5000);
			}

			/* GE <= 0 */
			if (hero->attrib[ATTRIB_GE].current <= 0) {

				/* reset GE */
				hero->attrib[ATTRIB_GE].current = hero->attrib[ATTRIB_GE].normal;
				/* die */
				sub_hero_le(hero, 5000);
			}
		}
	}
}
