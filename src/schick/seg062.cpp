/**
 *	Rewrite of DSA1 v3.02_de functions of seg062 (temple: miracle)
 *	Functions rewritten: 1/1 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg062.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg047.h"
#include "seg061.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void ask_miracle(void)
{
	signed short l_si;
	signed short j;
	struct struct_hero *hero;
	signed short disease;
	signed short fi_bak;
	signed short l3;
	signed short bonus;
	signed short l5;
	signed short i;
	signed short slot;
	signed short item_id;
	struct Bit8s_15 ga1 = g_temple_miracle_bonus;
	//struct dummy15 ga1 = {{0, 2, 15, 10, 20, 5, 10, 1, 15, 3, 15, 5, 10, 0}};
	struct Bit8s_15 god_dice = g_temple_miracle_dice;
	//struct dummy15 god_dice = {{0, 9, 9, 10, 17, 6, 10, 10, 18, 10, 19, 8, 15, 0, 10}};

	l3 = 0;
	fi_bak = g_text_file_index;

	load_tx2(ARCHIVE_FILE_WONDER_LTX);

	strcpy(g_dtp2, get_tx2(0)); /* "Eure Bitten werden nicht erhoert" */

	/* check gods estimation */
	if (gs_gods_estimation[g_temple_god] >= 100) {

		bonus = (signed short)((ga1.a[g_temple_god] * (gs_gods_estimation[g_temple_god] / 100) / 10) - l3);

		if (gs_current_town == TOWNS_CLANEGH) {
			bonus += 2;
		}

		gs_gods_estimation[g_temple_god] -= 10L;

		if (random_schick(100) <= god_dice.a[g_temple_god] + bonus) {

			l_si = random_schick(god_dice.a[g_temple_god]);

			if (god_dice.a[g_temple_god] == l_si) {
				miracle_resurrect(get_tx2(35));
			} else {
				switch (g_temple_god) {
				case GOD_PRAIOS: {
					l5 = 1;

					for (i = 0; gs_group_member_counts[gs_current_group] > i; i++) {
						if (host_readbs(get_hero(i) + HERO_TYPE) >= HERO_TYPE_WITCH) {
							l5 = 1;
						}
					}

					if (l5 != 0) {

						if (l_si <= 5) {

							/* MU+1 for 1 day */
							if (!gs_ingame_timers[INGAME_TIMER_PRAIOS_MU]) {

								miracle_modify(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_MU) - get_hero(0), DAYS(1), 1);
								strcpy(g_dtp2, get_tx2(1));

								gs_ingame_timers[INGAME_TIMER_PRAIOS_MU] = DAYS(1);
							}

						} else if (l_si <= 7) {

							/* MR+99 for 3 days */
							if (!gs_ingame_timers[INGAME_TIMER_PRAIOS_MR]) {

								i = get_random_hero();

								if ((i != -1) && !hero_gods_pissed(get_hero(i))) {

									slot = get_free_mod_slot();
									set_mod_slot(slot, DAYS(3), get_hero(i) + HERO_MR,
										99, (signed char)i);

									sprintf(g_dtp2, get_tx2(2), (char*)get_hero(i) + HERO_NAME2);
									gs_ingame_timers[INGAME_TIMER_PRAIOS_MR] = DAYS(3);
								}
							}

						} else if (l_si <= 8) {

							/* remove a transformation or a curse of one hero */
							for (i = 0; i <= 6; i++) {

								hero = (struct struct_hero*)get_hero(i);

								if (hero_transformed((Bit8u*)hero)) {

									and_ptr_bs((Bit8u*)hero + HERO_FLAGS2, 0xbf); /* unset 'transformed' flag */

									for (i = 0; i <= 6; i++) {
										hero->attrib[i].current++;
									}

									sprintf(g_dtp2,	get_ttx(565), hero->alias);
									break;

								} else {
									if (hero_renegade((Bit8u*)hero) && (hero->group_no == gs_current_group) && !hero_gods_pissed((Bit8u*)hero))
									{
										and_ptr_bs((Bit8u*)hero + HERO_FLAGS1, 0xdf); /* unset 'renegade' flag */

										sprintf(g_dtp2,	get_tx2(3), hero->alias);
										break;
									}
								}
							}
						}
					}
					break;
				}
				case GOD_RONDRA: {
					if (l_si <= 5) {
						if (!gs_ingame_timers[INGAME_TIMER_RONDRA_SWORDS]) {

							miracle_modify(get_hero(0) + (HERO_TALENTS + TA_SCHWERTER) - get_hero(0), DAYS(3), 1); /* for 3 days: skill 'Schwerter' + 1 */
							miracle_modify(get_hero(0) + (HERO_AT + WEAPON_TYPE_SCHWERT) - get_hero(0), DAYS(3), 1); /* for 3 days: AT + 1 */
							gs_ingame_timers[INGAME_TIMER_RONDRA_SWORDS] = DAYS(3);

							strcpy(g_dtp2, get_tx2(4));
						}
					} else if (l_si <= 7) {
						/* "Rondra breitet ihre Aura ueber euch aus, so dass keine Magie mehr wirken kann. */
						/* spellcasting is blocked (heroes and foes) */
						gs_ingame_timers[INGAME_TIMER_RONDRA_NO_SPELLS] = HOURS(6);

						strcpy(g_dtp2, get_tx2(5));

					} else if (l_si <= 8) {

						if (!gs_ingame_timers[INGAME_TIMER_RONDRA_MAGIC_WEAPON]) {

							miracle_weapon(get_tx2(6), 0);

							gs_ingame_timers[INGAME_TIMER_RONDRA_MAGIC_WEAPON] = DAYS(1);
						}
					}
					break;
				}
				case GOD_EFFERD: {
					if (l_si <= 5) {
						/* "Efferd verleiht euch die Gabe, Wasser zu finden." */
						/* searching for water in a wildcamp will always be successful */
						gs_ingame_timers[INGAME_TIMER_EFFERD_FIND_WATER] = DAYS(3);
						strcpy(g_dtp2, get_tx2(7));
					} else if (l_si <= 8) {
						/* "Efferd gewaehrt euch seinen Schutz auf Wasser." */
						gs_ingame_timers[INGAME_TIMER_EFFERD_SAFE_PASSAGE] = DAYS(3);
						strcpy(g_dtp2, get_tx2(8));
					} else if (l_si <= 9) {
						if (!gs_ingame_timers[INGAME_TIMER_EFFERD_SWIM]) {
							/* Schwimmen +2 for 4 days */
							miracle_modify(get_hero(0) + (HERO_TALENTS + TA_SCHWIMMEN) - get_hero(0), DAYS(4), 2);
							strcpy(g_dtp2, get_tx2(9));

							gs_ingame_timers[INGAME_TIMER_EFFERD_SWIM] = DAYS(4);
						}
					}
					break;
				}
				case GOD_TRAVIA: {
					if (l_si <= 10) {
						/* "Die ganze Gruppe wird von Travia goettlich gesaettigt." */
						for (i = 0; i <= 6; i++) {

							hero = (struct struct_hero*)get_hero(i);

							if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && !hero_gods_pissed((Bit8u*)hero))
							{
								hero->hunger = hero->thirst = 0;
							}
						}

						strcpy(g_dtp2, get_tx2(10));

					} else if (l_si <= 15) {
						miracle_heal_hero(dice_roll(1, 6, 2), get_tx2(11));
					} else if (l_si <= 16) {
						/* "Travia gewaehrt der Gruppe ihren Schutz in der Nacht */
						gs_ingame_timers[INGAME_TIMER_TRAVIA_SAFE_REST] = DAYS(7);
						strcpy(g_dtp2, get_tx2(12));
					}
					break;
				}
				case GOD_BORON: {
					if (l_si <= 3) {
						/* "Boron gewaehrt euch Schutz vor Untoten" */
						/* apparently, does not have an impact anywhere */
						gs_ingame_timers[INGAME_TIMER_BORON_UNDEAD] = DAYS(3);
						strcpy(g_dtp2, get_tx2(13));
					} else if (l_si <= 4) {
						miracle_resurrect(get_tx2(14));
					} else if (l_si <= 5) {
						if (!gs_ingame_timers[INGAME_TIMER_BORON_TA]) {
							miracle_modify(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_TA) - get_hero(0), DAYS(4), -1);
							strcpy(g_dtp2, get_tx2(15));
							gs_ingame_timers[INGAME_TIMER_BORON_TA] = DAYS(4);
						}
					}
					break;
				}
				case GOD_HESINDE: {
					if (l_si <= 3) {
						if (!gs_ingame_timers[INGAME_TIMER_HESINDE_ANALUES]) {
							miracle_modify(get_hero(0) + (HERO_SPELLS + SP_ANALUES_ARCANSTRUKTUR) - get_hero(0), DAYS(4), 1);
							strcpy(g_dtp2, get_tx2(16));
							gs_ingame_timers[INGAME_TIMER_HESINDE_ANALUES] = DAYS(4);
						}
					} else if (l_si <= 6) {
						/* unset transformation or renegade state of the first feasible hero */
						for (i = 0; i <= 6; i++) {

							hero = (struct struct_hero*)get_hero(i);

							if (hero_transformed((Bit8u*)hero)) {

								and_ptr_bs((Bit8u*)hero + HERO_FLAGS2, 0xbf); /* unset 'transformed' flag */

								for (i = 0; i <= 6; i++) {
									hero->attrib[i].current++;
								}

								sprintf(g_dtp2,	get_ttx(565), hero->alias);
								break;
							} else if (hero_renegade((Bit8u*)hero) && (hero->group_no == gs_current_group) && !hero_gods_pissed((Bit8u*)hero))
							{
								and_ptr_bs((Bit8u*)hero + HERO_FLAGS1, 0xdf); /* unset 'renegade' flag */

								sprintf(g_dtp2,	get_tx2(17), hero->alias);
								break;
							}
						}
					} else if (l_si <= 7) {

						if (!gs_ingame_timers[INGAME_TIMER_HESINDE_MR]) {
							miracle_modify(get_hero(0) + HERO_MR - get_hero(0), DAYS(3), 5);
							strcpy(g_dtp2, get_tx2(18));
							gs_ingame_timers[INGAME_TIMER_HESINDE_MR] = DAYS(3);
						}
					}
					break;
				}
				case GOD_FIRUN: case GOD_IFIRN: {
					if (l_si <= 5) {
						/* hunting in a wildcamp will always be successful */
						gs_ingame_timers[INGAME_TIMER_FIRUN_HUNT] = DAYS(3);
						strcpy(g_dtp2, get_tx2(19));
					} else if (l_si <= 8) {
						gs_ingame_timers[INGAME_TIMER_FIRUN_HUNT] = DAYS(7);
						strcpy(g_dtp2, get_tx2(19));
					} else if (l_si <= 9) {
						/* "Ihr verspuert keinen Hunger oder Durst mehr." */
						/* +1 for 7 days */
						if (!gs_ingame_timers[INGAME_TIMER_FIRUN_SATED]) {

							i = get_random_hero();

							if (i != -1 && !hero_gods_pissed(get_hero(i))) {

								slot = get_free_mod_slot();
								set_mod_slot(slot, DAYS(7), get_hero(i) + HERO_HUNGER_TIMER,
									1, (signed char)i);

								host_writebs(get_hero(i) + HERO_HUNGER, host_writebs(get_hero(i) + HERO_THIRST, 0));

								sprintf(g_dtp2, get_tx2(20), (char*)get_hero(i) + HERO_NAME2);
								gs_ingame_timers[INGAME_TIMER_FIRUN_SATED] = DAYS(7);
							}
						}
					}
					break;
				}
				case GOD_TSA: {
					if (l_si <= 10) {
						/* heal 2D6 LE of a hero */
						miracle_heal_hero(dice_roll(2, 6, 0), get_tx2(21));
					} else if (l_si <= 15) {
						/* completely heal all heroes */

						hero = (struct struct_hero*)get_hero(0);
						for (i = 0; i <= 6; i++, hero++) {

							if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) &&
								!hero_dead((Bit8u*)hero) && !hero_gods_pissed((Bit8u*)hero))
							{
								/* heal hero completely */
								/* this looks like adding more LE than missing, but the excess LE will be dealt with in add_hero_le */
								add_hero_le(hero, hero->le_max);
							}
						}

						strcpy(g_dtp2, get_tx2(22));

					} else if (l_si <= 18) {
						miracle_resurrect(get_tx2(23));
					}
					break;
				}
				case GOD_PHEX: {
					/* PHEX wants a bit more estimation */
					if (gs_gods_estimation[g_temple_god] > 500L) {

						if (l_si <= 5) {
							if (!gs_ingame_timers[INGAME_TIMER_PHEX_THIEF]) {
								/* Taschendiebstahl +1 for 3 days */
								miracle_modify(get_hero(0) + (HERO_TALENTS + TA_TASCHENDIEBSTAHL) - get_hero(0), DAYS(3), 1);
								/* Schloesser knacken +1 for 3 days */
								miracle_modify(get_hero(0) + (HERO_TALENTS + TA_SCHLOESSER) - get_hero(0), DAYS(3), 1);
								strcpy(g_dtp2, get_tx2(24));
								gs_ingame_timers[INGAME_TIMER_PHEX_THIEF] = DAYS(3);
							}
						} else if (l_si <= 8) {
							if (!gs_ingame_timers[INGAME_TIMER_PHEX_FEILSCHEN]) {
								/* Feilschen +1 for 3 days */
								miracle_modify(get_hero(0) + (HERO_TALENTS + TA_FEILSCHEN) - get_hero(0), DAYS(3), 1);
								strcpy(g_dtp2, get_tx2(25));
								gs_ingame_timers[INGAME_TIMER_PHEX_FEILSCHEN] = DAYS(3);
							}
						} else if (l_si <= 9) {
							if (!gs_ingame_timers[INGAME_TIMER_PHEX_FF]) {
								/* FF +1 for 3 days */
								miracle_modify(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_FF) - get_hero(0), DAYS(3), 1);
								strcpy(g_dtp2, get_tx2(26));
								gs_ingame_timers[INGAME_TIMER_PHEX_FF] = DAYS(3);
							}
						}
					}
					break;
				}
				case GOD_PERAINE: {
					if (l_si <= 10) {
						miracle_heal_hero(dice_roll(1, 6, 0), get_tx2(27));
					} else if (l_si <= 16) {
						miracle_heal_hero(dice_roll(2, 6, 0), get_tx2(27));
					} else if (l_si <= 18) {

						for (i = 0; i <= 6; i++) {
							hero = (struct struct_hero*)get_hero(i);
							disease = hero_is_diseased(hero);

							if (disease != 0 && (hero->group_no == gs_current_group) && !hero_gods_pissed((Bit8u*)hero))
							{
								hero->sick[disease][0] = 1;
								hero->sick[disease][1] = 0;

								sprintf(g_dtp2,	get_tx2(28), hero->alias);
								break;
							}
						}
					}
					break;
				}
				case GOD_INGERIMM: {
					if (l_si <= 5) {
						/* "Ingerimm segnet alle eure Waffen." */
						/* decrease BF of all weapons of all heroes by 2, but not below 0 */

						for (i = 0; i <= 6; i++) {
							hero = (struct struct_hero*)get_hero(i);

							if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) &&
								!hero_dead((Bit8u*)hero) && !hero_gods_pissed((Bit8u*)hero))
							{

								for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {

									if ((item_id = hero->inventory[j].item_id) && item_weapon(get_itemsdat(item_id)))
									{
										if (hero->inventory[j].bf > 0) {
											hero->inventory[j].bf--;
										}

										if (hero->inventory[j].bf > 0) {
											hero->inventory[j].bf--;
										}
									}
								}
							}
						}

						strcpy(g_dtp2, get_tx2(29));

					} else if (l_si <= 6) {
						if (!gs_ingame_timers[INGAME_TIMER_INGERIMM_MAGIC_WEAPON]) {
							miracle_weapon(get_tx2(30), 0);
							gs_ingame_timers[INGAME_TIMER_INGERIMM_MAGIC_WEAPON] = DAYS(1);
						}
					} else if (l_si <= 7) {
						miracle_weapon(get_tx2(31), 1);
					}
					break;
				}
				case GOD_RAHJA: {
					if (l_si <= 8) {
						if (!gs_ingame_timers[INGAME_TIMER_RAHJA_TALENTS]) {
							/* Betören +2 for 7 days */
							miracle_modify(get_hero(0) + (HERO_TALENTS + TA_BETOEREN) - get_hero(0), DAYS(7), 2);
							/* Tanzen +2 for 7 days */
							miracle_modify(get_hero(0) + (HERO_TALENTS + TA_TANZEN) - get_hero(0), DAYS(7), 2);
							strcpy(g_dtp2, get_tx2(32));
							gs_ingame_timers[INGAME_TIMER_RAHJA_TALENTS] = DAYS(7);
						}
					} else if (l_si <= 13) {
						if (!gs_ingame_timers[INGAME_TIMER_RAHJA_CH]) {
							/* CH +1 for 3 days */
							miracle_modify(get_hero(0) + (HERO_ATTRIB + 3 * ATTRIB_CH) - get_hero(0), DAYS(3), 1);
							strcpy(g_dtp2, get_tx2(33));
							gs_ingame_timers[INGAME_TIMER_RAHJA_CH] = DAYS(3);
						}
					} else if (l_si <= 14) {

						if (!gs_ingame_timers[INGAME_TIMER_RAHJA_TALENTS_PERMANENT]) {

							hero = (struct struct_hero*)get_hero(0);
							for (i = 0; i <= 6; i++, hero++) {

								if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) &&
									!hero_dead((Bit8u*)hero) && !hero_gods_pissed((Bit8u*)hero))
								{
									/* permanent Betören +1 */
									hero->skills[TA_BETOEREN]++;
									/* permanent Tanzen +1 */
									hero->skills[TA_TANZEN]++;
								}
							}

							strcpy(g_dtp2, get_tx2(34));
							gs_ingame_timers[INGAME_TIMER_RAHJA_TALENTS_PERMANENT] = DAYS(7);
						}

					}
					break;
				}
				/* NO miracles for SWAFNIR */
				}
			}
		}
	}

	/* print the output */
	GUI_output(g_dtp2);

	/* restore text file except for WONDER.LTX, TAVERN.TLK and except for dialogs */
	if (fi_bak != -1 && fi_bak != ARCHIVE_FILE_WONDER_LTX
		&& fi_bak != ARCHIVE_FILE_TAVERN_TLK
		&& (fi_bak < 156 || fi_bak > 176)) {
		load_tx2(fi_bak);
	}
}

#if !defined(__BORLANDC__)
}
#endif
