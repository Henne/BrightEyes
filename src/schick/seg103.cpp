/*
 *      Rewrite of DSA1 v3.02_de functions of seg103 (talents)
 *      Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg103.cpp
 *
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg047.h"
#include "seg053.h"
#include "seg097.h"
#include "seg103.h"
#include "seg104.h"

struct dummy {
	char a[6];
};

static char g_select_talent_lvlup[6] = "%s~%d"; // ds:0xacce
static signed char g_select_talent_defaults[6] = { TA_HEILEN_GIFT, TA_HEILEN_KRANKHEITEN, TA_HEILEN_WUNDEN, -1, -1, -1 }; // ds:0xacd4

signed int LVL_select_talent(const struct struct_hero *hero, const signed int show_values)
{
	signed int i;
	signed int answer;
	signed int first_talent;
	signed int retval = -1;
	/* string on stack "%s~%d" */
	struct dummy format_str = *(struct dummy*)g_select_talent_lvlup;

	if (show_values != 0) {

		strcpy(g_text_output_buf, get_ttx(205));

		if (hero->saved_talent_increases > 1) {
			strcat(g_text_output_buf, get_ttx(393));
		}

		sprintf(g_dtp2, get_ttx(204),
			(hero->saved_talent_increases > 1) ? get_ttx(305) : get_ttx(304),	/* sind / ist */
			hero->saved_talent_increases, g_text_output_buf);
	} else {

		strcpy(g_dtp2, get_ttx(216));
	}

	/* ask for the talentclass */
	answer = GUI_radio(g_dtp2, 7, get_ttx(100), get_ttx(101), get_ttx(102), get_ttx(105), get_ttx(103), get_ttx(104), get_ttx(106)) - 1;

	if (answer != -2) {

		first_talent = g_talentclasses[answer].first;

		if (show_values != 0) {

			for (i = 0; g_talentclasses[answer].length > i; i++) {

				sprintf(g_dtp2 + 50 * i, format_str.a, get_ttx(first_talent + i + 48), hero->talents[first_talent + i]);

				g_radio_name_list[i] = g_dtp2 + 50 * i;
			}
		} else {

			for (i = 0; g_talentclasses[answer].length > i; i++) {
				g_radio_name_list[i] = get_ttx(first_talent + i + 48);
			}
		}

		retval = GUI_radio(get_ttx(218), g_talentclasses[answer].length,
				g_radio_name_list[0], g_radio_name_list[1],
				g_radio_name_list[2], g_radio_name_list[3],
				g_radio_name_list[4], g_radio_name_list[5],
				g_radio_name_list[6], g_radio_name_list[7],
				g_radio_name_list[8], g_radio_name_list[9],
				g_radio_name_list[10], g_radio_name_list[11],
				g_radio_name_list[12], g_radio_name_list[13],
				g_radio_name_list[14], g_radio_name_list[15]);

		if (retval != -1) {
			retval += first_talent - 1;
		}

	} else {
		retval = -2;
	}

	return retval;
}

/**
 * \brief   returns hero which seems best for a talent
 *
 * \param   talent_id       talent
 */
struct struct_hero* get_proper_hero(const signed int talent_id)
/* called from only a single position, namely test_talent((struct struct_hero*)..), and only if game is in 'easy' mode and the tested talent is TA_SINNESSCHAERFE */
{
	signed int i;
	signed int cur;

	signed int max = -1;
	struct struct_hero *hero_i;
	struct struct_hero *retval;

#if !defined(__BORLANDC__)
	retval = NULL;
#endif

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if ((hero_i->typus != HERO_TYPE_NONE) &&
			(hero_i->group_id == gs_active_group_id) &&
			/* TODO: potential Original-Bug: What if petrified / unconscious etc.? */
			!hero_i->flags.dead) {

			/* add current and maximum attibute values */
			cur =	hero_i->attrib[g_talent_descriptions[talent_id].attrib1].current +
				hero_i->attrib[g_talent_descriptions[talent_id].attrib1].mod +
				hero_i->attrib[g_talent_descriptions[talent_id].attrib2].current +
				hero_i->attrib[g_talent_descriptions[talent_id].attrib2].mod +
				hero_i->attrib[g_talent_descriptions[talent_id].attrib3].current +
				hero_i->attrib[g_talent_descriptions[talent_id].attrib3].mod +
				hero_i->talents[talent_id];

			if (cur > max) {

				/* if the sum is greater than the current best */
				max = cur;
				retval = hero_i;
			}
		}
	}

#if !defined(__BORLANDC__)
	/* sanity check for Original Bug hunting */
	if (retval == 0)
		D1_ERR("Original-Bug: %s undefinierter Rückgabewert\n",	__func__);
#endif

	return retval;
}

/**
 * \brief   performs a talent test
 *
 * \param   hero        hero which should be tested
 * \param   talent       the talent to test
 * \param   handicap    may be positive or negative. The higher the value, the harder the test.
 */
signed int test_talent(const struct struct_hero* hero, const signed int talent_id, signed char handicap)
{
	signed int randval;
	signed int e_talentval;

	/* dont test for melee weapon talents */
	if ((talent_id >= TA_SCHUSSWAFFEN) && (talent_id <= TA_SINNESSCHAERFE)) {

#if !defined(__BORLANDC__)
		D1_INFO("%s Talentprobe %s %+d (TaW %d)", hero->alias, names_talent[talent_id], handicap, hero->talents[talent_id]);
#endif

		/* special test if talent is a range weapon talent */
		if ((talent_id == TA_SCHUSSWAFFEN) || (talent_id == TA_WURFWAFFEN)) {

			/* calculate range weapon base value */
			e_talentval = (	hero->attrib[ATTRIB_KL].current +
					hero->attrib[ATTRIB_KL].mod +
					hero->attrib[ATTRIB_GE].current +
					hero->attrib[ATTRIB_GE].mod +
					hero->attrib[ATTRIB_KK].current +
					hero->attrib[ATTRIB_KK].mod) / 4;

			/* add talent value */
			e_talentval += hero->talents[talent_id];
			/* sub handycap */
			e_talentval -= handicap;

			randval = random_schick(20);

			/* Unlucky */
			if (randval == 20) {
#if !defined(__BORLANDC__)
				D1_INFO("Ungluecklich\n");
#endif
				return -1;
			}
			/* Lucky */
			if (randval == 1) {
#if !defined(__BORLANDC__)
				D1_INFO("Gluecklich\n");
#endif
				return 99;
			}
			if (randval <= e_talentval) {
				/* test successful */
#if !defined(__BORLANDC__)
				D1_INFO(" (%d) -> bestanden\n", randval);
#endif
				return e_talentval - randval + 1;
			}

			/* test unsuccessful */
#if !defined(__BORLANDC__)
			D1_INFO(" (%d) -> nicht bestanden\n", randval);
#endif
			return 0;
		}

		/* automatically get hero with best senses in beginner mode */
		if ((talent_id == TA_SINNESSCHAERFE) && (g_game_mode == GAME_MODE_BEGINNER)) {

			hero = get_proper_hero(TA_SINNESSCHAERFE);

#if defined(__BORLANDC__)
			/* seems to have been debug stuff with conditional compilation */
			if (0) return 0;
#endif
		}

		/* do the test */
		handicap -= hero->talents[talent_id];

		return test_attrib3(hero, g_talent_descriptions[talent_id].attrib1,
						g_talent_descriptions[talent_id].attrib2,
						g_talent_descriptions[talent_id].attrib3, handicap);
	}

	return 0;
}

struct dummy2 {
	signed char a[6];
};

signed int select_talent(void)
{
	signed int retval = -1;
	signed int nr_talents = 3;

	/* available talents {TA_HEILEN_GIFT, TA_HEILEN_KRANKHEITEN, TA_HEILEN_WUNDEN, -1, -1, -1} */
	struct dummy2 a = *(struct dummy2*)g_select_talent_defaults;

	/* add talents for special location */
	if (gs_town_loc_type == LOCTYPE_TAVERN) {
		a.a[nr_talents] = TA_AKROBATIK;
		nr_talents++;

		if (!g_forbid_ta_falschspiel) {
			a.a[nr_talents] = TA_FALSCHSPIEL;
			nr_talents++;
		}

		a.a[nr_talents] = TA_MUSIZIEREN;
		nr_talents++;
	} else if ((gs_town_loc_type == LOCTYPE_WILDCAMP) || (gs_town_loc_type == LOCTYPE_INN)) {
		a.a[nr_talents] = TA_ALCHIMIE;
		nr_talents++;
	} else if (gs_town_loc_type == LOCTYPE_MARKET) {
		a.a[nr_talents] = TA_AKROBATIK;
		nr_talents++;
		a.a[nr_talents] = TA_TASCHENDIEBSTAHL;
		nr_talents++;
	} else if (gs_town_loc_type == LOCTYPE_MERCHANT) {
		a.a[nr_talents] = TA_TASCHENDIEBSTAHL;
		nr_talents++;
	}

	retval = GUI_radio(get_ttx(218), (signed char)nr_talents,
				get_ttx(a.a[0] + 48),
				get_ttx(a.a[1] + 48),
				get_ttx(a.a[2] + 48),
				get_ttx(a.a[3] + 48),
				get_ttx(a.a[4] + 48),
				get_ttx(a.a[5] + 48),
				get_ttx(a.a[6] + 48));

	if (retval != -1) {
		return a.a[retval - 1];
	}

	return retval;
}

signed int use_talent(const signed int hero_pos, signed char handicap, const signed int talent_id)
{
	signed int retval = 1;
	signed int le_damage;

	signed int patient_pos;
	signed int le;
	struct struct_hero *hero = get_hero(hero_pos);
	struct struct_hero *patient;
	int32_t money;
	signed int poison_id;
	signed int tx_file_bak;

	if (talent_id != -1) {

		tx_file_bak = g_tx_file_index;

		load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

		switch (talent_id) {

		case TA_HEILEN_GIFT: {

			g_hero_sel_exclude = (signed char)hero_pos;

			patient_pos = select_hero_from_group(get_ttx(460));

			if (patient_pos != -1) {

				patient = get_hero(patient_pos);

				if (is_hero_healable(patient)) {

					poison_id = hero_is_poisoned(patient);

					if (poison_id == 0) {

						/* patient is not poisoned */
						sprintf(g_dtp2,	get_ttx(463), patient->alias);
						GUI_output(g_dtp2);

					} else if (patient->heal_timer > 0) {

						/* patient timer is not zero */
						sprintf(g_dtp2,	get_ttx(697), patient->alias);
						GUI_output(g_dtp2);

					} else {
						/* set patient timer */
						patient->heal_timer = HOURS(4); /* 4 hours */

						if (test_talent(hero, TA_HEILEN_GIFT, handicap) > 0) {

							timewarp(MINUTES(20));

							if (test_talent(hero, TA_HEILEN_GIFT, g_poison_prices[poison_id] + handicap) > 0) {
								/* success */
								sprintf(g_dtp2, get_ttx(690), hero->alias, patient->alias);
								GUI_output(g_dtp2);

								patient->poison[poison_id][1] = 0;
								patient->poison[poison_id][0] = 1;

								sprintf(g_dtp2,	get_ttx(692), hero->alias, patient->alias);

								if (GUI_bool(g_dtp2)) {

									do {
										le = GUI_input(get_ttx(693), 2);

									} while (le <= 0);

									if ((retval = test_talent(hero, TA_HEILEN_GIFT, le + handicap)) > 0) {

										sprintf(g_dtp2,	get_ttx(691), hero->alias, patient->alias, le);

										add_hero_le(patient, le);

										GUI_output(g_dtp2);
									} else {
										/* talent test failed */
										le_damage = 3;

										if (patient->le <= le_damage) {

											/* don't kill the patient: at least 1 LE should remain */
											le_damage = patient->le - 1;
										}

										sub_hero_le(patient, le_damage);

										sprintf(g_dtp2,	get_ttx(694), patient->alias, le_damage);
										GUI_output(g_dtp2);

										retval = 0;
									}
								}
							} else {
								/* healing failed */
								sprintf(g_dtp2,	get_ttx(689), hero->alias, patient->alias);
								GUI_output(g_dtp2);
							}
						} else {
							/* recognizing the poison failed */
							sprintf(g_dtp2,	get_ttx(688), hero->alias, patient->alias);
							GUI_output(g_dtp2);
						}
					}
				}
			}
			break;
		}
		case  TA_HEILEN_KRANKHEITEN : {

			g_hero_sel_exclude = (signed char)hero_pos;

			patient_pos = select_hero_from_group(get_ttx(460));

			if (patient_pos != -1) {

				patient = get_hero(patient_pos);

				talent_cure_disease(hero, patient, handicap, 0);
			}
			break;
		}
		case TA_HEILEN_WUNDEN : {

			g_hero_sel_exclude = (signed char)hero_pos;

			patient_pos = select_hero_from_group(get_ttx(460));

			if (patient_pos != -1) {

				patient = get_hero(patient_pos);

				if (is_hero_healable(patient)) {

					if (patient->le >= patient->le_max) {

						/* no need to heal */
						sprintf(g_dtp2, get_ttx(461), patient->alias);
						GUI_output(g_dtp2);

					} else if (patient->heal_timer > 0) {

						/* timer is still running */
						sprintf(g_dtp2, get_ttx(697), patient->alias);
						GUI_output(g_dtp2);

					} else {
						patient->heal_timer = DAYS(1);

						if (test_talent(hero, TA_HEILEN_WUNDEN, handicap) > 0) {

							if (test_talent(hero, TA_HEILEN_WUNDEN, handicap) > 0) {

								retval = hero->talents[TA_HEILEN_WUNDEN] > 1 ? hero->talents[TA_HEILEN_WUNDEN] : 1;

								add_hero_le(patient, retval);

								sprintf(g_dtp2, get_ttx(691), hero->alias, patient->alias, retval);
								GUI_output(g_dtp2);

							} else {
								/* talent test failed */
								le_damage = 3;

								if (patient->le <= le_damage) {

									/* don't kill the patient: at least 1 LE should remain */
									le_damage = patient->le - 1;
								}

								sub_hero_le(patient, le_damage);

								sprintf(g_dtp2,	get_ttx(694), patient->alias, le_damage);
								GUI_output(g_dtp2);

								retval = 0;

								patient->staffspell_timer = DAYS(1); /* TODO: Why STAFFSPELL ?? BUG! */
								//patient->heal_timer = DAYS(1);
							}
						} else {

							if (random_schick(20) <= 7) {
								/* TODO Original-Bug? What if already diseased or recovering? */

								/* 35% chance: infected with Wundfieber illness */
								sprintf(g_dtp2, get_ttx(699), hero->alias, patient->alias);

								patient->disease[ILLNESS_TYPE_WUNDFIEBER][0] = DISEASE_STATUS_DISEASED;
								patient->disease[ILLNESS_TYPE_WUNDFIEBER][1] = 0;

							} else {
								/* 65% chance: just failed, no infection */
								sprintf(g_dtp2,	get_ttx(698), hero->alias, patient->alias);
							}

							GUI_output(g_dtp2);
						}
					}
				}
			}
			break;
		}
		case TA_AKROBATIK : {

			if (gs_ingame_timers[INGAME_TIMER_AKROBATIK] > 0) {

				GUI_output(get_tx(34));

			} else {

				if (test_talent(hero, TA_AKROBATIK, handicap) > 0) {

					money = random_interval(10, 200);

					make_valuta_str(g_text_output_buf, money);

					sprintf(g_dtp2, get_tx(35), hero->alias, g_text_output_buf);

					GUI_output(g_dtp2);

					add_party_money(money);

					gs_ingame_timers[INGAME_TIMER_AKROBATIK] = HOURS(8);

					g_request_refresh = 1;
				} else {
					GUI_output(get_tx(36));

					gs_ingame_timers[INGAME_TIMER_AKROBATIK] = HOURS(8);

					retval = -1;
				}
			}
			break;
		}
		case TA_MUSIZIEREN : {

			if (gs_ingame_timers[INGAME_TIMER_MUSIZIEREN] > 0) {

				GUI_output(get_tx(37));

			} else {

				if (test_talent(hero, TA_MUSIZIEREN, handicap) > 0) {

					money = random_interval(100, 300);

					make_valuta_str(g_text_output_buf, money);

					sprintf(g_dtp2, get_tx(35), hero->alias, g_text_output_buf);

					GUI_output(g_dtp2);

					add_party_money(money);

					gs_ingame_timers[INGAME_TIMER_MUSIZIEREN] = HOURS(8);

					g_request_refresh = 1;
				} else {
					GUI_output(get_tx(36));

					gs_ingame_timers[INGAME_TIMER_MUSIZIEREN] = HOURS(8);

					retval = -1;
				}
			}
			break;
		}
		case TA_FALSCHSPIEL : {

			if (test_talent(hero, TA_FALSCHSPIEL, handicap) > 0) {

				money = random_interval(500, 1000);

				make_valuta_str(g_text_output_buf, money);

				sprintf(g_dtp2, get_tx(38), g_text_output_buf, hero->alias);

				GUI_output(g_dtp2);

				add_party_money(money);

				g_request_refresh = 1;
			} else {
				GUI_output(get_tx(39));

				gs_tav_cheated_flags[gs_town_typeindex] = 1;

				retval = -1;
			}

			break;
		}
		case TA_TASCHENDIEBSTAHL : {

			if (test_talent(hero, TA_TASCHENDIEBSTAHL, handicap) > 0) {

				money = random_interval(500, 1000);

				make_valuta_str(g_text_output_buf, money);

				sprintf(g_dtp2, get_tx(40), g_text_output_buf, hero->alias);

				GUI_output(g_dtp2);

				add_party_money(money);

				g_request_refresh = 1;
			} else {
				sprintf(g_dtp2, get_tx(41), hero->alias);

				GUI_output(g_dtp2);

				set_party_money(0);

				g_request_refresh = 1;

				retval = -1;
			}

			break;
		}
		case TA_ALCHIMIE : {
			retval = plan_alchemy(hero);
			break;
		}
		}

		if ((tx_file_bak != -1) && (tx_file_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {
			load_tx(tx_file_bak);
		}
	}

	return retval;
}

signed int GUI_use_talent(const signed int hero_pos, const signed char handicap)
{
	signed int talent_id;
	const struct struct_hero *hero = get_hero(hero_pos);

	if (!check_hero(hero)) {
		return -1;
	}

	talent_id = select_talent();
	return use_talent(hero_pos, handicap, talent_id);
}

signed int GUI_use_talent2(const signed int handicap, char *msg)
{
	signed int hero_pos;
	const signed int talent_id = select_talent();

	if (talent_id != -1) {

		g_talented_hero_pos = get_talented_hero_pos(talent_id);

		hero_pos = select_hero_ok(msg);

		if ((hero_pos != -1) && get_hero(hero_pos)->flags.brewing) {

			GUI_output(get_ttx(730));
			hero_pos = -1;
		}
		if (hero_pos != -1) {
			return use_talent(hero_pos, (signed char)handicap, talent_id);
		}
	}

	return 1;
}

/**
 * \brief   does a bargain throw
 *
 * \param   hero        the hero who bargain
 * \param   items       the number of different goods
 * \param   price       the total price
 * \param   percent     how many percent the player wants to get
 * \param   mod_init    initial value for the modificator
 * \return              the result of the throw. A value greater than zero
 *	means success, below or zero means failed.
 */
signed int bargain(const struct struct_hero *hero, const signed int items, const int32_t price,
	const signed int percent, const signed char mod_init)
{

	signed char mod = mod_init;

	/* NPC Harika gets a bonus on bargain */
	if (get_hero(6)->npc_id == NPC_HARIKA) {
		mod -= 2;
	}

	/* the more different items you buy, the easier the bargain */
	mod += (items == 1) ? 2 :
			((items == 2) ?  1 :
			((items < 5)  ?  0 :
			((items < 9)  ? -1 : -2)));

	/* the higher the price, the easier the bargain */
	mod += (price < 100) ? 2 :
			((price < 500)  ?  1 :
			((price < 1000) ?  0 :
			((price < 2000) ? -1 : -2 )));

	/* the lower the percent, the easier the bargain */
	mod += percent / 5 + 1;

	return test_talent(hero, TA_FEILSCHEN, mod);
}
