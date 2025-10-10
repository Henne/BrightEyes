/*
 *      Rewrite of DSA1 v3.02_de functions of seg103 (skills)
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
#include "seg104.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct dummy {
	char a[6];
};

signed int LVL_select_skill(struct struct_hero *hero, const signed int show_values)
{
	signed short i;

	signed short answer;
	signed short l1;
	signed short retval = -1;
	/* string on stack "%s~%d" */
	struct dummy format_str = *(struct dummy*)g_select_skill_lvlup;

	if (show_values != 0) {

		strcpy(g_text_output_buf, get_ttx(205));

		if (hero->skill_incs > 1) {
			strcat(g_text_output_buf, get_ttx(393));
		}

		sprintf(g_dtp2, get_ttx(204),
			(hero->skill_incs > 1) ? get_ttx(305) : get_ttx(304),	/* sind / ist */
			hero->skill_incs, g_text_output_buf);
	} else {

		strcpy(g_dtp2, get_ttx(216));
	}

	/* ask for the skill category */
	answer = GUI_radio(g_dtp2, 7, get_ttx(100), get_ttx(101), get_ttx(102), get_ttx(105), get_ttx(103), get_ttx(104), get_ttx(106)) - 1;

	if (answer != -2) {

		l1 = g_skills_index[answer].first;

		if (show_values != 0) {

			for (i = 0; g_skills_index[answer].length > i; i++) {

				sprintf(g_dtp2 + 50 * i, format_str.a, get_ttx(l1 + i + 48), hero->skills[l1 + i]);

				g_radio_name_list[i] = g_dtp2 + 50 * i;
			}
		} else {

			for (i = 0; g_skills_index[answer].length > i; i++) {
				g_radio_name_list[i] = get_ttx(l1 + i + 48);
			}
		}

		retval = GUI_radio(get_ttx(218), g_skills_index[answer].length,
				g_radio_name_list[0], g_radio_name_list[1],
				g_radio_name_list[2], g_radio_name_list[3],
				g_radio_name_list[4], g_radio_name_list[5],
				g_radio_name_list[6], g_radio_name_list[7],
				g_radio_name_list[8], g_radio_name_list[9],
				g_radio_name_list[10], g_radio_name_list[11],
				g_radio_name_list[12], g_radio_name_list[13],
				g_radio_name_list[14], g_radio_name_list[15]);

		if (retval != -1) {
			retval += l1 - 1;
		}
	} else {
		retval = -2;
	}

	return retval;
}

/**
 * \brief   returns hero which seems best for a skill
 *
 * \param   skill_id       skill
 */
struct struct_hero* get_proper_hero(const signed int skill_id)
/* called from only a single position, namely test_skill((struct struct_hero*)..), and only if game is in 'easy' mode and the tested skill is TA_SINNESSCHAERFE */
{
	signed short i;
	signed short cur;

	signed short max = -1;
	struct struct_hero *hero_i;
	struct struct_hero* retval;

#if !defined(__BORLANDC__)
	retval = 0;
#endif

	hero_i = (struct struct_hero*)get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if ((hero_i->typus != HERO_TYPE_NONE) &&
			(hero_i->group_no == gs_current_group) &&
			/* TODO: potential Original-Bug: What if petrified / unconscious etc.? */
			!hero_i->flags.dead) {

			/* add current and maximum attibute values */
			cur =	hero_i->attrib[g_skill_descriptions[skill_id].attrib1].current +
				hero_i->attrib[g_skill_descriptions[skill_id].attrib1].mod +
				hero_i->attrib[g_skill_descriptions[skill_id].attrib2].current +
				hero_i->attrib[g_skill_descriptions[skill_id].attrib2].mod +
				hero_i->attrib[g_skill_descriptions[skill_id].attrib3].current +
				hero_i->attrib[g_skill_descriptions[skill_id].attrib3].mod +
				hero_i->skills[skill_id];

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
 * \brief   performs a skill test
 *
 * \param   hero        hero which should be tested
 * \param   skill       the skill to test
 * \param   handicap    may be positive or negative. The higher the value, the harder the test.
 */
signed short test_skill(struct struct_hero* hero, const signed int skill_id, signed char handicap)
{
	signed short randval;
	signed short e_skillval;

	/* dont test for melee weapon skills */
	if ((skill_id >= TA_SCHUSSWAFFEN) && (skill_id <= TA_SINNESSCHAERFE)) {

#if !defined(__BORLANDC__)
		D1_INFO("%s Talentprobe %s %+d (TaW %d)", hero->alias, names_skill[skill_id], handicap, hero->skills[skill_id]);
#endif

		/* special test if skill is a range weapon skill */
		if ((skill_id == TA_SCHUSSWAFFEN) || (skill_id == TA_WURFWAFFEN)) {

			/* calculate range weapon base value */
			e_skillval = (	hero->attrib[ATTRIB_KL].current +
					hero->attrib[ATTRIB_KL].mod +
					hero->attrib[ATTRIB_GE].current +
					hero->attrib[ATTRIB_GE].mod +
					hero->attrib[ATTRIB_KK].current +
					hero->attrib[ATTRIB_KK].mod) / 4;

			/* add skill value */
			e_skillval += hero->skills[skill_id];
			/* sub handycap */
			e_skillval -= handicap;

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
			if (randval <= e_skillval) {
				/* test successful */
#if !defined(__BORLANDC__)
				D1_INFO(" (%d) -> bestanden\n", randval);
#endif
				return e_skillval - randval + 1;
			}

			/* test unsuccessful */
#if !defined(__BORLANDC__)
			D1_INFO(" (%d) -> nicht bestanden\n", randval);
#endif
			return 0;
		}

		/* automatically get hero with best senses in beginner mode */
		if ((skill_id == TA_SINNESSCHAERFE) && (g_game_mode == GAME_MODE_BEGINNER)) {

			hero = get_proper_hero(TA_SINNESSCHAERFE);

#if defined(__BORLANDC__)
			/* seems to have been debug stuff with conditional compilation */
			if (0) return 0;
#endif
		}

		/* do the test */
		handicap -= hero->skills[skill_id];

		return test_attrib3(hero, g_skill_descriptions[skill_id].attrib1,
						g_skill_descriptions[skill_id].attrib2,
						g_skill_descriptions[skill_id].attrib3, handicap);
	}

	return 0;
}

struct dummy2 {
	signed char a[6];
};

signed short select_skill(void)
{
	signed short l_si = -1;
	signed short nr_skills = 3;
	/* available skills {TA_HEILEN_GIFT, TA_HEILEN_KRANKHEITEN, TA_HEILEN_WUNDEN, -1, -1, -1} */
	struct dummy2 a = *(struct dummy2*)g_select_skill_defaults;

	/* add skills for special location */
	if (gs_current_loctype == LOCTYPE_TAVERN) {
		a.a[nr_skills] = TA_AKROBATIK;
		nr_skills++;

		if (!g_cheatskill_usable) {
			a.a[nr_skills] = TA_FALSCHSPIEL;
			nr_skills++;
		}

		a.a[nr_skills] = TA_MUSIZIEREN;
		nr_skills++;
	} else if ((gs_current_loctype == LOCTYPE_WILDCAMP) || (gs_current_loctype == LOCTYPE_INN)) {
		a.a[nr_skills] = TA_ALCHIMIE;
		nr_skills++;
	} else if (gs_current_loctype == LOCTYPE_MARKET) {
		a.a[nr_skills] = TA_AKROBATIK;
		nr_skills++;
		a.a[nr_skills] = TA_TASCHENDIEBSTAHL;
		nr_skills++;
	} else if (gs_current_loctype == LOCTYPE_MERCHANT) {
		a.a[nr_skills] = TA_TASCHENDIEBSTAHL;
		nr_skills++;
	}

	l_si = GUI_radio(get_ttx(218), (signed char)nr_skills,
				get_ttx(a.a[0] + 48),
				get_ttx(a.a[1] + 48),
				get_ttx(a.a[2] + 48),
				get_ttx(a.a[3] + 48),
				get_ttx(a.a[4] + 48),
				get_ttx(a.a[5] + 48),
				get_ttx(a.a[6] + 48));

	if (l_si != -1) {
		return a.a[l_si - 1];
	}

	return l_si;
}

signed short use_skill(signed short hero_pos, signed char handicap, signed short skill)
{
	signed short l_si;
	signed short le_damage;

	signed short patient_pos;
	signed short le;
	struct struct_hero *hero;
	struct struct_hero *patient;
	Bit32s money;
	signed short poison_id;
	signed short tx_file_bak;

	l_si = 1;

	hero = (struct struct_hero*)get_hero(hero_pos);

	if (skill != -1) {

		tx_file_bak = g_tx_file_index;

		load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

		switch(skill) {
		case TA_HEILEN_GIFT: {
			g_hero_sel_exclude = (signed char)hero_pos;

			patient_pos = select_hero_from_group(get_ttx(460));

			if (patient_pos != -1) {

				patient = (struct struct_hero*)get_hero(patient_pos);

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

						if (test_skill(hero, TA_HEILEN_GIFT, handicap) > 0) {

							timewarp(MINUTES(20));

							if (test_skill(hero, TA_HEILEN_GIFT, g_poison_prices[poison_id] + handicap) > 0) {
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

									if ((l_si = test_skill(hero, TA_HEILEN_GIFT, le + handicap)) > 0) {

										sprintf(g_dtp2,	get_ttx(691), hero->alias, patient->alias, le);

										add_hero_le(patient, le);

										GUI_output(g_dtp2);
									} else {
										/* skill test failed */
										le_damage = 3;

										if (patient->le <= le_damage) {

											/* don't kill the patient: at least 1 LE should remain */
											le_damage = patient->le - 1;
										}

										sub_hero_le(patient, le_damage);

										sprintf(g_dtp2,	get_ttx(694), patient->alias, le_damage);
										GUI_output(g_dtp2);

										l_si = 0;
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

				patient = (struct struct_hero*)get_hero(patient_pos);

				skill_cure_disease(hero, patient, handicap, 0);
			}
			break;
		}
		case TA_HEILEN_WUNDEN : {
			g_hero_sel_exclude = (signed char)hero_pos;

			patient_pos = select_hero_from_group(get_ttx(460));

			if (patient_pos != -1) {

				patient = (struct struct_hero*)get_hero(patient_pos);

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

						if (test_skill(hero, TA_HEILEN_WUNDEN, handicap) > 0) {

							if (test_skill(hero, TA_HEILEN_WUNDEN, handicap) > 0) {

								l_si = hero->skills[TA_HEILEN_WUNDEN] > 1 ? hero->skills[TA_HEILEN_WUNDEN] : 1;

								add_hero_le(patient, l_si);

								sprintf(g_dtp2, get_ttx(691), hero->alias, patient->alias, l_si);
								GUI_output(g_dtp2);

							} else {
								/* skill test failed */
								le_damage = 3;

								if (patient->le <= le_damage) {

									/* don't kill the patient: at least 1 LE should remain */
									le_damage = patient->le - 1;
								}

								sub_hero_le(patient, le_damage);

								sprintf(g_dtp2,	get_ttx(694), patient->alias, le_damage);
								GUI_output(g_dtp2);

								l_si = 0;

								patient->staffspell_timer = DAYS(1); /* TODO: Why STAFFSPELL ?? BUG! */
								//patient->heal_timer = DAYS(1);
							}
						} else {

							if (random_schick(20) <= 7) {

								/* 35% chance: infected with Wundfieber illness */
								sprintf(g_dtp2, get_ttx(699), hero->alias, patient->alias);

								patient->sick[ILLNESS_TYPE_WUNDFIEBER][0] = -1;
								patient->sick[ILLNESS_TYPE_WUNDFIEBER][1] = 0;

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

				if (test_skill(hero, TA_AKROBATIK, handicap) > 0) {

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

					l_si = -1;
				}
			}
			break;
		}
		case TA_MUSIZIEREN : {

			if (gs_ingame_timers[INGAME_TIMER_MUSIZIEREN] > 0) {

				GUI_output(get_tx(37));

			} else {

				if (test_skill(hero, TA_MUSIZIEREN, handicap) > 0) {

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

					l_si = -1;
				}
			}
			break;
		}
		case TA_FALSCHSPIEL : {

			if (test_skill(hero, TA_FALSCHSPIEL, handicap) > 0) {

				money = random_interval(500, 1000);

				make_valuta_str(g_text_output_buf, money);

				sprintf(g_dtp2, get_tx(38), g_text_output_buf, hero->alias);

				GUI_output(g_dtp2);

				add_party_money(money);

				g_request_refresh = 1;
			} else {
				GUI_output(get_tx(39));

				gs_tav_cheated_flags[gs_current_typeindex] = 1;

				l_si = -1;
			}

			break;
		}
		case TA_TASCHENDIEBSTAHL : {

			if (test_skill(hero, TA_TASCHENDIEBSTAHL, handicap) > 0) {

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

				l_si = -1;
			}

			break;
		}
		case TA_ALCHIMIE : {
			l_si = plan_alchemy(hero);
			break;
		}
		}

		if ((tx_file_bak != -1) && (tx_file_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {
			load_tx(tx_file_bak);
		}
	}

	return l_si;
}

signed short GUI_use_skill(signed short hero_pos, signed char handicap)
{
	signed short skill_id;
	struct struct_hero *hero;

	hero = (struct struct_hero*)get_hero(hero_pos);

	if (!check_hero(hero)) {
		return -1;
	}

	skill_id = select_skill();
	return use_skill(hero_pos, handicap, skill_id);
}

signed short GUI_use_skill2(signed short handicap, char *msg)
{
	signed short hero_pos;
	signed short skill;

	skill = select_skill();

	if (skill != -1) {

		g_skilled_hero_pos = get_skilled_hero_pos(skill);

		hero_pos = select_hero_ok(msg);

		if ((hero_pos != -1) && (((struct struct_hero*)get_hero(hero_pos))->flags.brewing)) {
			GUI_output(get_ttx(730));
			hero_pos = -1;
		}
		if (hero_pos != -1) {
			return use_skill(hero_pos, (signed char)handicap, skill);
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
signed int bargain(struct struct_hero *hero, const signed int items, const Bit32s price,
	const signed int percent, const signed char mod_init)
{

	signed char mod = mod_init;

	/* NPC Harika gets a bonus on bargain */
	if (((struct struct_hero*)get_hero(6))->npc_id == NPC_HARIKA) {
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

	return test_skill(hero, TA_FEILSCHEN, mod);
}

#if !defined(__BORLANDC__)
}
#endif
