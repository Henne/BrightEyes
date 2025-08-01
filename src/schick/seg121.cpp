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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void poison_effect(void)
{
	signed short i;
	volatile signed short j;
	Bit8u *hero;
	Bit8u *poison_ptr;

	g_check_poison = 0;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero+=SIZEOF_HERO) {

		if ((host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) && !hero_dead(hero)) {

			poison_ptr = hero + HERO_POISON + POISON_TYPE_SHURINKNOLLENGIFT * SIZEOF_HERO_POISON;

			/* SHURINKNOLLENGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (host_readbs(poison_ptr + 1) >= 6) {

					/* KK */
					if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) != 0) {

						sprintf(g_dtp2, get_ttx(572), (char*)hero + HERO_NAME2);

						GUI_output(g_dtp2);

						inc_ptr_bs(poison_ptr + 2);
						dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
					}

					/* GE */
					if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) != 0) {

						sprintf(g_dtp2, get_ttx(45), (char*)hero + HERO_NAME2);

						GUI_output(g_dtp2);

						inc_ptr_bs(poison_ptr + 3);
						dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE));
					}

					if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) < 0 || host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) < 0) {
						sub_hero_le(hero, 1000);
					}
				}

				/* Loose 1W6 LE */
				sub_hero_le(hero, random_schick(6));
			}

			/* SHURINKNOLLENGIFT: hero regenerates */
			if (host_readbs(poison_ptr) == 1) {

				if (!host_readbs(poison_ptr + 2) && !host_readbs(poison_ptr + 3)) {

					host_writeb(poison_ptr + 1, 0);
					host_writeb(poison_ptr, 0);
				}

				if ((host_readbs(poison_ptr + 2) != 0) && (!host_readbs(poison_ptr + 1) % 12 )) {

						sprintf(g_dtp2, get_ttx(573), (char*)hero + HERO_NAME2);

						GUI_output(g_dtp2);

						dec_ptr_bs(poison_ptr + 2);
						inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
				}

				if ((host_readbs(poison_ptr + 3) != 0) && (!host_readbs(poison_ptr + 1) % 12 )) {

						sprintf(g_dtp2, get_ttx(578), (char*)hero + HERO_NAME2);

						GUI_output(g_dtp2);

						dec_ptr_bs(poison_ptr + 3);
						inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE));
				}
			}


			poison_ptr = hero + (HERO_POISON + POISON_TYPE_ARAX * SIZEOF_HERO_POISON);

			/* ARAXGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if ((host_readbs(poison_ptr + 1) >= 2) && !(host_readbs(poison_ptr + 4))) {

					host_writeb(poison_ptr + 4, 1);

					for (j = 0; j < 7; j++) {
						/* all AT - 2 */
						sub_ptr_bs(hero + HERO_AT + j, 2);
						/* all PA - 2 */
						sub_ptr_bs(hero + HERO_PA + j, 2);
					}

					/* GE - 2 */
					host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) - 2);
					/* KK - 2 */
					host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) - 2);

					sprintf(g_dtp2, get_ttx(37), (char*)hero + HERO_NAME2);

					GUI_output(g_dtp2);

				}

				/* enable self-regeneration */
				if (host_readbs(poison_ptr + 1) >= 120) {
					host_writebs(poison_ptr, 1);
				}
			}

			/* ARAXGIFT: regeneration */
			if ((host_readbs(poison_ptr) == 1) && (host_readbs(poison_ptr + 4))) {

				host_writeb(poison_ptr, 0);
				host_writeb(poison_ptr + 1, 0);
				host_writeb(poison_ptr + 4, 0);

				for (j = 0; j < 7; j++) {
					/* all AT + 2 */
					add_ptr_bs(hero + HERO_AT + j, 2);
					/* all PA + 2 */
					add_ptr_bs(hero + HERO_PA + j, 2);
				}

				/* GE + 2 */
				host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) + 2);
				/* KK + 2 */
				host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + 2);

				sprintf(g_dtp2, get_ttx(38), (char*)hero + HERO_NAME2);

				GUI_output(g_dtp2);
			}


			poison_ptr = hero + (HERO_POISON + POISON_TYPE_ANGSTGIFT * SIZEOF_HERO_POISON);

			/* ANGSTGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if(!host_readbs(poison_ptr + 4)) {

					host_writeb(poison_ptr + 4, 1);

					/* MU - 2 */
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_MU), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU)) - 2);

					/* all negative attributes + 2 */
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_AG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_AG)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_HA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_HA)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_RA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_RA)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_GG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GG)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_TA)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_NG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_NG)) + 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ)) + 2);
					or_ptr_bs(hero + HERO_FLAGS2, 1); /* set 'scared' flag */

					sprintf(g_dtp2, get_ttx(39), (char*)hero + HERO_NAME2);

					GUI_output(g_dtp2);
				}

				/* self regeneration */
				if (host_readbs(poison_ptr + 1) >= 7) {
					host_writeb(poison_ptr, 1);
				}
			}

			/* ANGSTGIFT: regeneration */
			if ((host_readbs(poison_ptr) == 1) && host_readbs(poison_ptr + 4)) {

					host_writeb(poison_ptr, 0);
					host_writeb(poison_ptr + 1, 0);
					host_writeb(poison_ptr + 4, 0);

					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_MU), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_MU)) + 2);

					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_AG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_AG)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_HA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_HA)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_RA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_RA)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_GG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GG)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_TA)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_NG), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_NG)) - 2);
					host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ), host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_JZ)) - 2);
					and_ptr_bs(hero + HERO_FLAGS2, 0xfe); /* unset 'scared' flag */

					sprintf(g_dtp2, get_ttx(40), (char*)hero + HERO_NAME2);

					GUI_output(g_dtp2);
			}


			poison_ptr = hero + (HERO_POISON + POISON_TYPE_SCHLAFGIFT * SIZEOF_HERO_POISON);

			/* SCHLAFGIFT: hero is poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (!host_readbs(poison_ptr + 4)) {

					host_writeb(poison_ptr + 4, 1);
					or_ptr_bs(hero + HERO_FLAGS1, 0x02); /* set 'asleep' flag */

					sprintf(g_dtp2, get_ttx(41), (char*)hero + HERO_NAME2);

					GUI_output(g_dtp2);
				}

				/* self regeneration */
				if (host_readbs(poison_ptr + 1) >= 20) {
					host_writeb(poison_ptr, 1);
				}
			}

			/* SCHLAFGIFT: regeneration */
			if ((host_readbs(poison_ptr) == 1) && (host_readbs(poison_ptr + 4) != 0)) {

				host_writeb(poison_ptr, 0);
				host_writeb(poison_ptr + 1, 0);
				host_writeb(poison_ptr + 4, 0);
				and_ptr_bs(hero + HERO_FLAGS1, 0xfd); /* unset 'asleep' flag */

				sprintf(g_dtp2, get_ttx(42), (char*)hero + HERO_NAME2);

				GUI_output(g_dtp2);
			}


			poison_ptr = hero + (HERO_POISON + POISON_TYPE_GOLDLEIM * SIZEOF_HERO_POISON);

			/* GOLDLEIM: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {
				if (host_readbs(poison_ptr + 1) >= dice_roll(2, 6, 0) * 12) {
					host_writebs(poison_ptr, 0);
				}

				if (!(host_readbs(poison_ptr + 1) % 12)) {
					sub_hero_le(hero, dice_roll(2, 6, -3));
				}
			}

			/* GOLDLEIM: regeneration */
			if (host_readbs(poison_ptr) == 1) {
				host_writeb(poison_ptr, 0);
			}


			poison_ptr = hero + (HERO_POISON + POISON_TYPE_KROETENSCHEMEL * SIZEOF_HERO_POISON);

			/* KROETENSCHEMEL: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {
				if (host_readbs(poison_ptr + 1) >= 48) {
					host_writebs(poison_ptr, 0);
				}

				if (!(host_readbs(poison_ptr + 1) % 12)) {
					sub_hero_le(hero, dice_roll(1, 6, 2));
				}
			}

			/* KROETENSCHEMEL: regeneration */
			if (host_readbs(poison_ptr) == 1) {
				host_writeb(poison_ptr, 0);
			}


			poison_ptr = hero + (HERO_POISON + POISON_TYPE_LOTUSGIFT * SIZEOF_HERO_POISON);

			/* LOTUSGIFT: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {
				if (host_readbs(poison_ptr + 1) >= 24) {
					host_writebs(poison_ptr, 0);
				}

				sub_hero_le(hero, dice_roll(2, 6, 0));
			}

			/* LOTUSGIFT: regeneration */
			if (host_readbs(poison_ptr) == 1) {
				host_writeb(poison_ptr, 0);
			}


			poison_ptr = hero + (HERO_POISON + POISON_TYPE_KUKRIS * SIZEOF_HERO_POISON);

			/* KUKRIS: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {
				if (host_readbs(poison_ptr + 1) >= 3) {
					host_writebs(poison_ptr, 0);
				}

				sub_hero_le(hero, dice_roll(50, 6, 0));
			}

			/* KUKRIS: regeneration */
			if (host_readbs(poison_ptr) == 1) {
				host_writeb(poison_ptr, 0);
			}


			poison_ptr = hero + (HERO_POISON + POISON_TYPE_BANNSTAUB * SIZEOF_HERO_POISON);

			/* BANNSTAUB: hero gets poisoned */
			if (host_readbs(poison_ptr) == -1) {

				if (host_readbs(poison_ptr + 1) >= dice_roll(3, 6, 0)) {
					host_writebs(poison_ptr, 1);
				}

				j = dice_roll(1, 6, 2);

				if (host_readws(hero + HERO_AE) >= j) {

					/* loose 1W6+2 AEmax */
					host_writeb(poison_ptr + 2, host_readbs(poison_ptr + 2) + j);

					sub_ptr_ws(hero + HERO_AE, j);

					sprintf(g_dtp2, get_ttx(43), (char*)hero + HERO_NAME2, j);

					GUI_output(g_dtp2);
				}
			}

			/* BANNSTAUB: regeneration */
			if ((host_readbs(poison_ptr) == 1)) {
				if (!host_readbs(poison_ptr + 2)) {
					host_writeb(poison_ptr, 0);
				} else {

					if (!host_readbs(poison_ptr +1) % 12) {
						/* regenerate one point at a time */
						dec_ptr_bs(poison_ptr + 2);
						inc_ptr_ws(hero + HERO_AE);

						sprintf(g_dtp2, get_ttx(44), (char*)hero + HERO_NAME2);

						GUI_output(g_dtp2);
					}
				}
			}


			for (j = 1; j <= 9; j++) {
				if (host_readbs(hero + HERO_POISON + j * SIZEOF_HERO_POISON) != 0) {
					inc_ptr_bs(hero + (HERO_POISON+1) + j * SIZEOF_HERO_POISON);
				}
			}

			/* KK <= 0 */
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) <= 0) {
				/* reset KK */
				host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_KK)));
				/* die */
				sub_hero_le(hero, 5000);
			}

			/* GE <= 0 */
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) <= 0) {
				/* reset GE */
				host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_GE)));
				/* die */
				sub_hero_le(hero, 5000);
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
