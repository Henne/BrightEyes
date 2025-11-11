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

static struct int8_t_15 g_temple_miracle_bonus = { 0, 2, 15, 10, 20, 5, 10, 1, 15, 3, 15, 5, 10, 0, 1 }; // ds:0x6ea4
static struct int8_t_15 g_temple_miracle_dice = { 0, 9, 9, 10, 17, 6, 10, 10, 18, 10, 19, 8, 15, 0, 10 }; // ds:0x6eb3

void ask_miracle(void)
{
	signed short miracle_dice_roll;
	signed short j;
	struct struct_hero *hero;
	signed short disease;
	signed short fi_bak;
	signed short const_zero; // always zero
	signed short bonus;
	signed short no_spellcaster_in_group;
	signed short i;
	signed short mod_slot;
	signed short item_id;
	struct int8_t_15 god_bonus = g_temple_miracle_bonus;
	//struct dummy15 god_bonus = {{0, 2, 15, 10, 20, 5, 10, 1, 15, 3, 15, 5, 10, 0, 1}};
	struct int8_t_15 god_dice = g_temple_miracle_dice;
	//struct dummy15 god_dice = {{0, 9, 9, 10, 17, 6, 10, 10, 18, 10, 19, 8, 15, 0, 10}};

	const_zero = 0;
	fi_bak = g_text_file_index;

	load_tx2(ARCHIVE_FILE_WONDER_LTX);

	strcpy(g_dtp2, get_tx2(0)); /* "Eure Bitten werden nicht erhoert" */

	/* check gods estimation */
	if (gs_gods_estimation[g_temple_god] >= 100) {

		bonus = (signed short)((god_bonus.a[g_temple_god] * (gs_gods_estimation[g_temple_god] / 100) / 10) - const_zero);

		if (gs_town_id == TOWN_ID_CLANEGH) {
			bonus += 2;
		}

		gs_gods_estimation[g_temple_god] -= 10L;

		if (random_schick(100) <= god_dice.a[g_temple_god] + bonus) {

			miracle_dice_roll = random_schick(god_dice.a[g_temple_god]);

			if (god_dice.a[g_temple_god] == miracle_dice_roll) {
				// For all gods: Highest possible dice roll -> resurrect a hero
				miracle_resurrect(get_tx2(35));
			} else {
				switch (g_temple_god) {
				case GOD_PRAIOS: {
					no_spellcaster_in_group = 1;

					for (i = 0; gs_group_member_counts[gs_active_group_id] > i; i++) {
						if ((get_hero(i))->typus >= HERO_TYPE_HEXE) {
#ifndef M302de_ORIGINAL_BUGFIX
							/* Original-Bug 43:
								The implementation of Praios miracles suggests that Praios only grants miracles to groups without spellcasters.
								This fits the lore that Praios hates magic.
								However, the way is implemented, the check does not make any difference.
								In principle, this bug is irrelevant, since there is no regular Praios temple in the game (but see Original-Bug 24).
							*/
							no_spellcaster_in_group = 1;
#else
							no_spellcaster_in_group = 0;
#endif
						}
					}

					// Praios doesn't like groups with spellcasters.
					if (no_spellcaster_in_group != 0) {

						if (miracle_dice_roll <= 5) {

							/* MU+1 for 1 day */
							if (!gs_ingame_timers[INGAME_TIMER_PRAIOS_MU]) {

								miracle_modify((uint8_t*)&get_hero(0)->attrib[ATTRIB_MU].current - (uint8_t*)get_hero(0), DAYS(1), 1);
								strcpy(g_dtp2, get_tx2(1));

								gs_ingame_timers[INGAME_TIMER_PRAIOS_MU] = DAYS(1);
							}

						} else if (miracle_dice_roll <= 7) {

							/* MR+99 for 3 days */
							if (!gs_ingame_timers[INGAME_TIMER_PRAIOS_MR]) {

								i = get_random_hero();

								if ((i != -1) && !(get_hero(i))->flags.gods_pissed) {

									mod_slot = get_free_mod_slot();
									set_mod_slot(mod_slot, DAYS(3), (uint8_t*)&(get_hero(i))->mr, 99, (signed char)i);

									sprintf(g_dtp2, get_tx2(2), (get_hero(i))->alias);
									gs_ingame_timers[INGAME_TIMER_PRAIOS_MR] = DAYS(3);
								}
							}

						} else if (miracle_dice_roll <= 8) {

							/* remove a transformation or a curse of one hero */
							for (i = 0; i <= 6; i++) {

								hero = get_hero(i);

								if (hero->flags.transformed) {

									hero->flags.transformed = 0;

									for (i = 0; i <= 6; i++) {
										hero->attrib[i].current++;
									}

									sprintf(g_dtp2,	get_ttx(565), hero->alias);
									break;

								} else {
									if (hero->flags.renegade && (hero->group_id == gs_active_group_id) && !hero->flags.gods_pissed)
									{
										hero->flags.renegade = 0;

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
					if (miracle_dice_roll <= 5) {
						if (!gs_ingame_timers[INGAME_TIMER_RONDRA_SWORDS]) {

							miracle_modify((uint8_t*)&get_hero(0)->skills[TA_SCHWERTER] - (uint8_t*)get_hero(0), DAYS(3), 1); /* for 3 days: skill 'Schwerter' + 1 */
							miracle_modify((uint8_t*)&get_hero(0)->at_talent_bonus[WEAPON_TYPE_SCHWERT] - (uint8_t*)get_hero(0), DAYS(3), 1); /* for 3 days: AT + 1 */
							gs_ingame_timers[INGAME_TIMER_RONDRA_SWORDS] = DAYS(3);

							strcpy(g_dtp2, get_tx2(4));
						}
					} else if (miracle_dice_roll <= 7) {
						/* "Rondra breitet ihre Aura ueber euch aus, so dass keine Magie mehr wirken kann. */
						/* spellcasting is blocked (heroes and foes) */
						gs_ingame_timers[INGAME_TIMER_RONDRA_NO_SPELLS] = HOURS(6);

						strcpy(g_dtp2, get_tx2(5));

					} else if (miracle_dice_roll <= 8) {

						if (!gs_ingame_timers[INGAME_TIMER_RONDRA_MAGIC_WEAPON]) {

							miracle_weapon(get_tx2(6), 0);

							gs_ingame_timers[INGAME_TIMER_RONDRA_MAGIC_WEAPON] = DAYS(1);
						}
					}
					break;
				}
				case GOD_EFFERD: {
					if (miracle_dice_roll <= 5) {
						/* "Efferd verleiht euch die Gabe, Wasser zu finden." */
						/* searching for water in a wildcamp will always be successful */
						gs_ingame_timers[INGAME_TIMER_EFFERD_FIND_WATER] = DAYS(3);
						strcpy(g_dtp2, get_tx2(7));
					} else if (miracle_dice_roll <= 8) {
						/* "Efferd gewaehrt euch seinen Schutz auf Wasser." */
						gs_ingame_timers[INGAME_TIMER_EFFERD_SAFE_PASSAGE] = DAYS(3);
						strcpy(g_dtp2, get_tx2(8));
					} else if (miracle_dice_roll <= 9) {
						if (!gs_ingame_timers[INGAME_TIMER_EFFERD_SWIM]) {

							/* Schwimmen +2 for 4 days */
							miracle_modify((uint8_t*)&get_hero(0)->skills[TA_SCHWIMMEN] - (uint8_t*)get_hero(0), DAYS(4), 2);
							strcpy(g_dtp2, get_tx2(9));

							gs_ingame_timers[INGAME_TIMER_EFFERD_SWIM] = DAYS(4);
						}
					}
					break;
				}
				case GOD_TRAVIA: {
					if (miracle_dice_roll <= 10) {
						/* "Die ganze Gruppe wird von Travia goettlich gesaettigt." */
						for (i = 0; i <= 6; i++) {

							hero = get_hero(i);

							if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && !hero->flags.gods_pissed)
							{
								hero->hunger = hero->thirst = 0;
							}
						}

						strcpy(g_dtp2, get_tx2(10));

					} else if (miracle_dice_roll <= 15) {
						miracle_heal_hero(dice_roll(1, 6, 2), get_tx2(11));
					} else if (miracle_dice_roll <= 16) {
						/* "Travia gewaehrt der Gruppe ihren Schutz in der Nacht */
						gs_ingame_timers[INGAME_TIMER_TRAVIA_SAFE_REST] = DAYS(7);
						strcpy(g_dtp2, get_tx2(12));
					}
					break;
				}
				case GOD_BORON: {
					if (miracle_dice_roll <= 3) {
						/* "Boron gewaehrt euch Schutz vor Untoten" */
						/* apparently, does not have an impact anywhere */
						gs_ingame_timers[INGAME_TIMER_BORON_UNDEAD] = DAYS(3);
						strcpy(g_dtp2, get_tx2(13));
					} else if (miracle_dice_roll <= 4) {
						miracle_resurrect(get_tx2(14));
					} else if (miracle_dice_roll <= 5) {
						if (!gs_ingame_timers[INGAME_TIMER_BORON_TA]) {

							miracle_modify((uint8_t*)&get_hero(0)->attrib[ATTRIB_TA].current - (uint8_t*)get_hero(0), DAYS(4), -1);
							strcpy(g_dtp2, get_tx2(15));
							gs_ingame_timers[INGAME_TIMER_BORON_TA] = DAYS(4);
						}
					}
					break;
				}
				case GOD_HESINDE: {
					if (miracle_dice_roll <= 3) {
						if (!gs_ingame_timers[INGAME_TIMER_HESINDE_ANALUES]) {

							miracle_modify((uint8_t*)&get_hero(0)->spells[SP_ANALUES_ARCANSTRUKTUR] - (uint8_t*)get_hero(0), DAYS(4), 1);
							strcpy(g_dtp2, get_tx2(16));
							gs_ingame_timers[INGAME_TIMER_HESINDE_ANALUES] = DAYS(4);
						}
					} else if (miracle_dice_roll <= 6) {
						/* unset transformation or renegade state of the first feasible hero */
						for (i = 0; i <= 6; i++) {

							hero = get_hero(i);

							if (hero->flags.transformed) {

								hero->flags.transformed = 0;

								for (i = 0; i <= 6; i++) {
									hero->attrib[i].current++;
								}

								sprintf(g_dtp2,	get_ttx(565), hero->alias);
								break;
							} else if (hero->flags.renegade && (hero->group_id == gs_active_group_id) && !hero->flags.gods_pissed)
							{
								hero->flags.renegade = 0;

								sprintf(g_dtp2,	get_tx2(17), hero->alias);
								break;
							}
						}
					} else if (miracle_dice_roll <= 7) {

						if (!gs_ingame_timers[INGAME_TIMER_HESINDE_MR]) {

							miracle_modify((uint8_t*)&get_hero(0)->mr - (uint8_t*)get_hero(0), DAYS(3), 5);
							strcpy(g_dtp2, get_tx2(18));
							gs_ingame_timers[INGAME_TIMER_HESINDE_MR] = DAYS(3);
						}
					}
					break;
				}
				case GOD_FIRUN: case GOD_IFIRN: {
					if (miracle_dice_roll <= 5) {
						/* hunting in a wildcamp will always be successful */
						gs_ingame_timers[INGAME_TIMER_FIRUN_HUNT] = DAYS(3);
						strcpy(g_dtp2, get_tx2(19));
					} else if (miracle_dice_roll <= 8) {
						gs_ingame_timers[INGAME_TIMER_FIRUN_HUNT] = DAYS(7);
						strcpy(g_dtp2, get_tx2(19));
					} else if (miracle_dice_roll <= 9) {
						/* "Ihr verspuert keinen Hunger oder Durst mehr." */
						/* sated for 7 days */
						if (!gs_ingame_timers[INGAME_TIMER_FIRUN_SATED]) {

							i = get_random_hero();

							if (i != -1 && !(get_hero(i))->flags.gods_pissed) {

								mod_slot = get_free_mod_slot();
								set_mod_slot(mod_slot, DAYS(7), (uint8_t*)&(get_hero(i))->hunger_timer, 1, (signed char)i);

								(get_hero(i))->hunger = (get_hero(i))->thirst = 0;

								sprintf(g_dtp2, get_tx2(20), (get_hero(i))->alias);
								gs_ingame_timers[INGAME_TIMER_FIRUN_SATED] = DAYS(7);
							}
						}
					}
					break;
				}
				case GOD_TSA: {
					if (miracle_dice_roll <= 10) {
						/* heal 2D6 LE of a hero */
						miracle_heal_hero(dice_roll(2, 6, 0), get_tx2(21));
					} else if (miracle_dice_roll <= 15) {
						/* completely heal all heroes */

						hero = get_hero(0);
						for (i = 0; i <= 6; i++, hero++) {

							if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
								!hero->flags.dead && !hero->flags.gods_pissed)
							{
								/* heal hero completely */
								/* this looks like adding more LE than missing, but the excess LE will be dealt with in add_hero_le */
								add_hero_le(hero, hero->le_max);
							}
						}

						strcpy(g_dtp2, get_tx2(22));

					} else if (miracle_dice_roll <= 18) {
						miracle_resurrect(get_tx2(23));
					}
					break;
				}
				case GOD_PHEX: {
					/* PHEX wants a bit more estimation */
					if (gs_gods_estimation[g_temple_god] > 500L) {

						if (miracle_dice_roll <= 5) {
							if (!gs_ingame_timers[INGAME_TIMER_PHEX_THIEF]) {
								/* Taschendiebstahl +1 for 3 days */
								miracle_modify((uint8_t*)&get_hero(0)->skills[TA_TASCHENDIEBSTAHL] - (uint8_t*)get_hero(0), DAYS(3), 1);
								/* Schloesser knacken +1 for 3 days */
								miracle_modify((uint8_t*)&get_hero(0)->skills[TA_SCHLOESSER] - (uint8_t*)get_hero(0), DAYS(3), 1);
								strcpy(g_dtp2, get_tx2(24));
								gs_ingame_timers[INGAME_TIMER_PHEX_THIEF] = DAYS(3);
							}
						} else if (miracle_dice_roll <= 8) {
							if (!gs_ingame_timers[INGAME_TIMER_PHEX_FEILSCHEN]) {
								/* Feilschen +1 for 3 days */
								miracle_modify((uint8_t*)&get_hero(0)->skills[TA_FEILSCHEN] - (uint8_t*)get_hero(0), DAYS(3), 1);
								strcpy(g_dtp2, get_tx2(25));
								gs_ingame_timers[INGAME_TIMER_PHEX_FEILSCHEN] = DAYS(3);
							}
						} else if (miracle_dice_roll <= 9) {
							if (!gs_ingame_timers[INGAME_TIMER_PHEX_FF]) {
								/* FF +1 for 3 days */
								miracle_modify((uint8_t*)&get_hero(0)->attrib[ATTRIB_FF].current - (uint8_t*)get_hero(0), DAYS(3), 1);
								strcpy(g_dtp2, get_tx2(26));
								gs_ingame_timers[INGAME_TIMER_PHEX_FF] = DAYS(3);
							}
						}
					}
					break;
				}
				case GOD_PERAINE: {
					if (miracle_dice_roll <= 10) {
						miracle_heal_hero(dice_roll(1, 6, 0), get_tx2(27));
					} else if (miracle_dice_roll <= 16) {
						miracle_heal_hero(dice_roll(2, 6, 0), get_tx2(27));
					} else if (miracle_dice_roll <= 18) {

						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);
							disease = hero_is_diseased(hero);

							if (disease != 0 && (hero->group_id == gs_active_group_id) && !hero->flags.gods_pissed)
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
					if (miracle_dice_roll <= 5) {
						/* "Ingerimm segnet alle eure Waffen." */
						/* decrease BF of all weapons of all heroes by 2, but not below 0 */

						for (i = 0; i <= 6; i++) {
							hero = get_hero(i);

							if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
								!hero->flags.dead && !hero->flags.gods_pissed)
							{

								for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {

									if ((item_id = hero->inventory[j].item_id) && g_itemsdat[item_id].flags.weapon)
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

					} else if (miracle_dice_roll <= 6) {
						if (!gs_ingame_timers[INGAME_TIMER_INGERIMM_MAGIC_WEAPON]) {
							miracle_weapon(get_tx2(30), 0);
							gs_ingame_timers[INGAME_TIMER_INGERIMM_MAGIC_WEAPON] = DAYS(1);
						}
					} else if (miracle_dice_roll <= 7) {
						miracle_weapon(get_tx2(31), 1);
					}
					break;
				}
				case GOD_RAHJA: {
					if (miracle_dice_roll <= 8) {
						if (!gs_ingame_timers[INGAME_TIMER_RAHJA_TALENTS]) {
							/* Betören +2 for 7 days */
							miracle_modify((uint8_t*)&get_hero(0)->skills[TA_BETOEREN] - (uint8_t*)get_hero(0), DAYS(7), 2);
							/* Tanzen +2 for 7 days */
							miracle_modify((uint8_t*)&get_hero(0)->skills[TA_TANZEN] - (uint8_t*)get_hero(0), DAYS(7), 2);
							strcpy(g_dtp2, get_tx2(32));
							gs_ingame_timers[INGAME_TIMER_RAHJA_TALENTS] = DAYS(7);
						}
					} else if (miracle_dice_roll <= 13) {
						if (!gs_ingame_timers[INGAME_TIMER_RAHJA_CH]) {
							/* CH +1 for 3 days */
							miracle_modify((uint8_t*)&get_hero(0)->attrib[ATTRIB_CH].current - (uint8_t*)get_hero(0), DAYS(3), 1);
							strcpy(g_dtp2, get_tx2(33));
							gs_ingame_timers[INGAME_TIMER_RAHJA_CH] = DAYS(3);
						}
					} else if (miracle_dice_roll <= 14) {

						if (!gs_ingame_timers[INGAME_TIMER_RAHJA_TALENTS_PERMANENT]) {

							hero = get_hero(0);
							for (i = 0; i <= 6; i++, hero++) {

								if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) &&
									!hero->flags.dead && !hero->flags.gods_pissed)
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
