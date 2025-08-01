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
	unsigned char *hero;
	Bit8u *hero2;
	Bit8u *disease_ptr;

	g_check_disease = 0;

	for (i = 0; i <= 6; i++) {

		if ((host_readbs(get_hero(i) + HERO_TYPE) != HERO_TYPE_NONE) && !hero_dead(get_hero(i))) {

			hero = get_hero(i);

			disease_ptr = hero + (HERO_ILLNESS + ILLNESS_TYPE_WUNDFIEBER * SIZEOF_HERO_ILLNESS);

			/* TETANUS / WUNDFIEBER: get worse */
			if (host_readbs(disease_ptr) == -1) {

				if (host_readbs(disease_ptr + 1) > 13) {
					/* Hero feels better after 13 days */

					host_writeb(disease_ptr + 1, 0);
					host_writeb(disease_ptr, 1);

					sprintf(g_dtp2, get_ttx(561), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);

				} else {
					/* Strength is fading, but only to 1 */
					sub_hero_le(hero, dice_roll(2, 6, -(host_readbs(disease_ptr + 1) - 1)));

					if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) != 0) {

						sprintf(g_dtp2, get_ttx(572), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);

						inc_ptr_bs(disease_ptr + 2);
						dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
					}
				}
			}

			/* TETANUS / WUNDFIEBER: get better */
			if (host_readbs(disease_ptr) == 1) {

				if (!host_readbs(disease_ptr + 2)) {
					/* regeneration complete */
					host_writeb(disease_ptr + 1, 0);
					host_writeb(disease_ptr, 0);
				} else {
					/* hero regains the lost strength */
					sprintf(g_dtp2, get_ttx(573), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);

					dec_ptr_bs(disease_ptr + 2);
					inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
				}
			}


			disease_ptr = hero + (HERO_ILLNESS + ILLNESS_TYPE_DUMPFSCHAEDEL * SIZEOF_HERO_ILLNESS);

			/* NUMBSKULL / DUMPFSCHAEDEL: get worse */
			if (host_readbs(disease_ptr) == -1) {

				if (!host_readbs(disease_ptr + 4)) {

					host_writeb(disease_ptr + 4, 1);

					for (j = 0; j < 7; j++) {
						sub_ptr_bs(hero + j + 0x68, 2);
						sub_ptr_bs(hero + j + 0x6f, 2);
					}

					sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), 2);
					sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), 5);

					sprintf(g_dtp2, get_ttx(574), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);

				}

				if (host_readbs(disease_ptr + 1) > 2) {
					/* after two days, each other hero in the group
					 * can be infectect by a chance of 20 % */

					hero2 = get_hero(0);

					for (j = 0; j <= 6; j++, hero2 += SIZEOF_HERO) {
						if ((host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
							(host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group) &&
							!hero_dead(hero2) &&
							(hero2 != hero) &&
							(random_schick(100) <= 20))
						{
							hero_gets_diseased(hero2, 2);
						}
					}
				}

				if (random_schick(100) < 5) {

					/* 5% chance of selfhealing */
					host_writebs(disease_ptr + 1, 0);
					host_writebs(disease_ptr, 0);

					if (host_readb(disease_ptr + 4) != 0) {

						host_writebs(disease_ptr + 4, 0);

						for (j = 0; j < 7; j++) {
							add_ptr_bs(hero + j + 0x68, 2);
							add_ptr_bs(hero + j + 0x6f, 2);
						}

						add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), 2);
						add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), 5);
					}

					hero_gets_diseased(hero, 3);
				}

				if (host_readbs(disease_ptr + 1) > dice_roll(1, 3, 4)) {
					host_writeb(disease_ptr, 1);
				} else {
					sub_hero_le(hero, dice_roll(1, 6, -1));
				}
			}

			/* NUMBSKULL / DUMPFSCHAEDEL: get better */
			if (host_readbs(disease_ptr) == 1) {

				host_writebs(disease_ptr + 1, 0);
				host_writebs(disease_ptr, 0);

				if (host_readb(disease_ptr + 4) != 0) {

					host_writebs(disease_ptr + 4, 0);

					for (j = 0; j < 7; j++) {
						add_ptr_bs(hero + j + 0x68, 2);
						add_ptr_bs(hero + j + 0x6f, 2);
					}

					add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), 2);
					add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), 5);
				}

				/* regeneration complete */
				sprintf(g_dtp2, get_ttx(575), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);
			}

			disease_ptr = hero + (HERO_ILLNESS + ILLNESS_TYPE_BLAUE_KEUCHE * SIZEOF_HERO_ILLNESS);

			/* BLUE COUGH / BLAUE KEUCHE: get worse */
			if (host_readbs(disease_ptr) == -1) {

				if (!host_readbs(disease_ptr + 4)) {

					host_writeb(disease_ptr + 4, 1);

					for (j = 0; j < 7; j++) {
						sub_ptr_bs(hero + j + 0x68, 4);
						sub_ptr_bs(hero + j + 0x6f, 4);
					}

					host_writebs(disease_ptr + 2, host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) / 2);
					host_writebs(disease_ptr + 3, host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) / 2);


					sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(disease_ptr + 3));
					sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(disease_ptr + 2));

					sprintf(g_dtp2, get_ttx(577), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);

				}

				if ((host_readbs(disease_ptr + 1) > 3) && (random_schick(100) <= 25)) {

					dec_ptr_bs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_KK));
					dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
					sub_ptr_ws(hero + HERO_LE_ORIG, host_readbs(disease_ptr + 1) / 3);
					sub_hero_le(hero, host_readbs(disease_ptr + 1) / 3);
					host_writebs(disease_ptr, 1);

				} else {

					hero2 = get_hero(0);

					for (j = 0; j <= 6; j++, hero2 += SIZEOF_HERO) {
						if ((host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
							(host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group) &&
							!hero_dead(hero2) &&
							(hero2 != hero) &&
							(random_schick(100) <= 20))
						{
							hero_gets_diseased(hero2, 3);
						}
					}

					sub_hero_le(hero, dice_roll(1, 6, 2));
				}
			}

			/* BLUE COUGH / BLAUE KEUCHE: get better */
			if (host_readbs(disease_ptr) == 1) {

				/* regeneration complete */
				sprintf(g_dtp2, get_ttx(576), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);

				host_writebs(disease_ptr + 1, 0);
				host_writebs(disease_ptr, 0);

				if (host_readb(disease_ptr + 4) != 0) {

					host_writebs(disease_ptr + 4, 0);

					for (j = 0; j < 7; j++) {
						add_ptr_bs(hero + j + 0x68, 4);
						add_ptr_bs(hero + j + 0x6f, 4);
					}

					add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(disease_ptr + 3));
					add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(disease_ptr + 2));
					host_writebs(disease_ptr + 2, host_writebs(disease_ptr + 3, 0));
				}
			}


			disease_ptr = hero + (HERO_ILLNESS + ILLNESS_TYPE_PARALYSE * SIZEOF_HERO_ILLNESS);

			/* PARALYSIS / PARALYSE: get worse */
			if (host_readbs(disease_ptr) == -1) {

				if (host_readbs(disease_ptr + 1) > dice_roll(1, 3, 4)) {
					host_writebs(disease_ptr + 1, 0);
					host_writebs(disease_ptr, 1);
				} else {
					j = random_schick(6);
					add_ptr_bs(disease_ptr + 3, j);
					sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), j);

					j = random_schick(6);
					add_ptr_bs(disease_ptr + 2, j);
					sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), j);

					sprintf(g_dtp2, get_ttx(579), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);
				}
			}

			/* PARALYSIS / PARALYSE: get better */
			if (host_readbs(disease_ptr) == 1) {

				if (!host_readbs(disease_ptr + 2) && !host_readbs(disease_ptr + 3)) {
					host_writebs(disease_ptr + 1, 0);
					host_writebs(disease_ptr, 0);
				} else {

					if (!host_readbs(disease_ptr + 2)) {

						sprintf(g_dtp2, get_ttx(573), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);

						dec_ptr_bs(disease_ptr + 2);
						inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
					}

					if (!host_readbs(disease_ptr + 3)) {

						sprintf(g_dtp2, get_ttx(578), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);

						dec_ptr_bs(disease_ptr + 3);
						inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE));
					}
				}
			}


			disease_ptr = hero + (HERO_ILLNESS + ILLNESS_TYPE_SCHLACHTENFIEBER * SIZEOF_HERO_ILLNESS);

			/* BATTLEFIELD FEVER / SCHLACHTFELDFIEBER: get worse */
			if (host_readbs(disease_ptr) == -1) {

				if (host_readbs(disease_ptr + 1) > 7) {

					/* 30 % for elfes, 20% for the all other types */
					if (random_schick(100) <= (host_readbs(hero + HERO_TYPE) >= HERO_TYPE_GREEN_ELF ? 30 : 20)) {

						sprintf(g_dtp2, get_ttx(580), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);

						sub_hero_le(hero, 1000);

					} else {

						if (host_readbs(disease_ptr + 4) != 0) {
							host_writeb(disease_ptr, 1);
						}
					}
				} else {

					if (host_readbs(disease_ptr + 1) > 3) {

						if (!host_readbs(disease_ptr + 4)) {

							host_writeb(disease_ptr + 4, 1);
							sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), 5);

							sprintf(g_dtp2, get_ttx(581), (char*)hero + HERO_NAME2,
								(char*)GUI_get_ptr(host_readbs(hero + HERO_SEX), 0));
							GUI_output(g_dtp2);
						}

						for (j = 0; j <= 6; j++) {

							hero2 = get_hero(j);

							if ((host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
								(host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group) &&
								!hero_dead(hero2) &&
								(hero2 != hero) &&
								(random_schick(100) <= 5))
							{
								hero_gets_diseased(hero2, 5);
							}
						}

						sub_hero_le(hero, dice_roll((host_readbs(hero + HERO_TYPE) >= HERO_TYPE_GREEN_ELF ? 2 : 1),
										6, host_readbs(disease_ptr + 1) - 1));
					}
				}
			}

			/* BATTLEFIELD FEVER / SCHLACHTFELDFIEBER: get better */
			if (host_readbs(disease_ptr) == 1) {

				if (host_readbs(disease_ptr + 4) != 0) {

					host_writeb(disease_ptr + 4, 0);
					host_writeb(disease_ptr + 1, 0);
					add_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), 5);
				}

				sprintf(g_dtp2, get_ttx(582), (char*)hero + HERO_NAME2);
				GUI_output(g_dtp2);
			}

			disease_ptr = hero + (HERO_ILLNESS + ILLNESS_TYPE_FROSTSCHAEDEN * SIZEOF_HERO_ILLNESS);

			/* FROSTBITE / FROSTSCHAEDEN: get worse */
			if (host_readbs(disease_ptr) == -1) {

				j = 0;

				if (random_schick(100) <= host_readbs(disease_ptr + 1) * 5) {

					dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE));
					dec_ptr_bs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_GE));

					j = 1;

					sprintf(g_dtp2, get_ttx(583), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);
				}

				sub_hero_le(hero, dice_roll(1, 6, 0));

				if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) != 0) {

					inc_ptr_bs(disease_ptr + 2);
					dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
				}

				if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) != 0) {

					inc_ptr_bs(disease_ptr + 3);
					dec_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE));
				}

				if (j == 0) {

					sprintf(g_dtp2, get_ttx(740), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);
				}
			}

			/* FROSTBITE / FROSTSCHAEDEN: get better */
			if (host_readbs(disease_ptr) == 1) {

				if (!host_readbs(disease_ptr + 2) && !host_readbs(disease_ptr + 3)) {
					host_writebs(disease_ptr + 1, 0);
					host_writebs(disease_ptr, 0);
				} else {
					if (!host_readbs(disease_ptr + 2)) {

						sprintf(g_dtp2, get_ttx(573), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);

						dec_ptr_bs(disease_ptr + 2);
						inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
					}

					if (!host_readbs(disease_ptr + 3)) {

						sprintf(g_dtp2, get_ttx(578), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);

						dec_ptr_bs(disease_ptr + 3);
						inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE));
					}
				}
			}

			disease_ptr = hero + (HERO_ILLNESS + ILLNESS_TYPE_TOLLWUT * SIZEOF_HERO_ILLNESS);

			/* RABIES / TOLLWUT: get worse */
			if (host_readbs(disease_ptr) == -1) {

				if (host_readbs(disease_ptr + 1) > dice_roll(1, 6, 6)) {

					host_writeb(disease_ptr + 1, 0);
					host_writeb(disease_ptr, 1);

				} else {

					hero2 = get_hero(0);

					for (j = 0; j <= 6; j++, hero2 += SIZEOF_HERO) {


						if ((host_readbs(hero2 + HERO_TYPE) != HERO_TYPE_NONE) &&
							(host_readbs(hero2 + HERO_GROUP_NO) == gs_current_group) &&
							!hero_dead(hero2) &&
							(hero2 != hero) &&
							(random_schick(100) <= 10))
						{
							hero_gets_diseased(hero2, 7);
						}
					}

					sub_hero_le(hero, dice_roll((host_readbs(disease_ptr + 1) > 3 ? 3 : host_readbs(disease_ptr + 1)),
									6, 0));

					add_ptr_bs(disease_ptr + 2, 2);
					sub_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), 2);

					if (host_readbs(disease_ptr + 1) > 2) {

						rabies(hero, i);

					} else {

						sprintf(g_dtp2, get_ttx(572), (char*)hero + HERO_NAME2);
						GUI_output(g_dtp2);
					}
				}
			}

			/* RABIES / TOLLWUT: get better */
			if (host_readbs(disease_ptr) == 1) {

				if (!host_readbs(disease_ptr + 2)) {

					host_writeb(disease_ptr + 1, 0);
					host_writeb(disease_ptr, 0);

				} else {
					sprintf(g_dtp2, get_ttx(573), (char*)hero + HERO_NAME2);
					GUI_output(g_dtp2);

					dec_ptr_bs(disease_ptr + 2);
					inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK));
				}
			}

			/* increment day timer for all diseases */
			for (j = 1; j <= 7; j++) {

				if (host_readbs(hero + 5 * j + 0xae) != 0) {
					inc_ptr_bs(hero + 5 * j + 0xaf);
				}
			}

			/* kill hero if KK <= 0 */
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) <= 0) {

				host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_KK)));

				sub_hero_le(hero, 5000);
			}

			/* kill hero if GE <= 0 */
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) <= 0) {

				host_writebs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE), host_readbs(hero + (HERO_ATTRIB_ORIG + 3 * ATTRIB_GE)));

				sub_hero_le(hero, 5000);
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
