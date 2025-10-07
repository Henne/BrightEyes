/**
 *	Rewrite of DSA1 v3.02_de functions of seg104 (alchemy, cure disease, heroes)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg104.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg026.h"
#include "seg047.h"
#include "seg049.h"
#include "seg053.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static signed int g_alchemy_missing_item; // ds:0xe5c4

signed short hero_has_ingrendients(struct struct_hero *hero, const signed short recipe_id)
{
	signed short i = 0;
	signed short retval = 1;
	struct struct_recipe *r_ptr = &g_alchemy_recipes[recipe_id];
	signed short item_pos;

	/* loop over ingrendients */
	while ((r_ptr->ingredients[i] != -1) && retval) {

		item_pos = get_item_pos((Bit8u*)hero, r_ptr->ingredients[i]);

		if (item_pos == -1) {

			/* needed item missing */
			retval = 0;

			/* TODO: g_alchemy_missing_item should not be a globvar */
			g_alchemy_missing_item = r_ptr->ingredients[i];
		} else {
			/* drop all needed items */
			drop_item((struct struct_hero*)hero, item_pos, 1);
		}

		i++;
	}

	i--;

	if (!retval) {
		i--;
	}

	while (i >= 0) {
		/* give all needed items back */
		give_hero_new_item((struct struct_hero*)hero, r_ptr->ingredients[i], 1, 1);
		i--;
	}

	return retval;
}

void hero_use_ingrendients(struct struct_hero *hero, const signed short recipe_id)
{
	signed short i = 0;
	struct struct_recipe *r_ptr = &g_alchemy_recipes[recipe_id];
	signed short item_pos;

	/* loop over ingredients */
	while (r_ptr->ingredients[i] != -1) {

		item_pos = get_item_pos((Bit8u*)hero, r_ptr->ingredients[i]);

		/* drop the needed item */
		drop_item((struct struct_hero*)hero, item_pos, 1);

		/* exchange wine- or brandybottles into glass flask */
		if ((r_ptr->ingredients[i] == ITEM_WINE) || (r_ptr->ingredients[i] == ITEM_BRANDY))
		{
			give_hero_new_item((struct struct_hero*)hero, ITEM_FLASK_GLASS, 1, 1);
		}

		/* exchange oil into bronze flask */
		if (r_ptr->ingredients[i] == ITEM_OIL)
		{
			give_hero_new_item((struct struct_hero*)hero, ITEM_FLASK_BRONZE, 1, 1);
		}

		i++;
	}
}

signed short do_alchemy(struct struct_hero* hero, const signed short recipe_id, const signed short flag_abort)
	/* flag_abort = 0: finalize running brewing process
	 * flag_abort = 1: abort running brewing process */
{
	struct struct_recipe *r_ptr = &g_alchemy_recipes[recipe_id];

	hero_use_ingrendients(hero, recipe_id);

	sub_ae_splash(hero, r_ptr->ae_cost);

	and_ptr_bs((Bit8u*)hero + HERO_FLAGS1, 0xf7); /* unset 'brewing' flag */
	hero->recipe_timer = 0;
	/* set heroes receipe to 0 */
	hero->recipe_id = 0;
	hero->alchemy_inn_id = 0;

	if ((test_skill((struct struct_hero*)(Bit8u*)hero, TA_ALCHIMIE, r_ptr->handicap) > 0) && (flag_abort == 0))
	{
		/* success */

		give_hero_new_item((struct struct_hero*)hero, r_ptr->outcome, 1, 1);

		sprintf(g_dtp2, get_ttx(731), hero->alias, (char*)GUI_names_grammar(1, r_ptr->outcome, 0));
		GUI_output(g_dtp2);

		return 1;
	} else {
		/* failure */
		/* give first ingredient back, which is always the bottle (glass or bronze). */
		give_hero_new_item((struct struct_hero*)hero, r_ptr->ingredients[0], 1, 1);

		sprintf(g_dtp2,	get_ttx(732), hero->alias, (char*)GUI_names_grammar(2, r_ptr->outcome, 0));
		GUI_output(g_dtp2);

		return 0;
	}

}

signed short plan_alchemy(struct struct_hero *hero)
{
	signed short retval;
	signed short item_pos;
	signed short recipes;
	signed short answer;
	signed short decision;
	signed short l5;
	signed short i;
	signed char recipe_index;
	struct struct_hero *hero_p;
	signed short tw_bak;
	signed char array[13];


	retval = 1;
	recipes = 0;
	item_pos = get_item_pos((Bit8u*)hero, ITEM_ALCHEMY_KIT);

	if (item_pos == -1) {

		/* no alchemy kit */
		GUI_output(get_tx(42));
		retval = 0;
	} else {

		/* count all recipes and prepare the menu */
		for (i = 0; i <= 12; i++) {
			if (get_item_pos((Bit8u*)hero, g_alchemy_recipes[i].item_id) != -1) {

				strcpy(g_dtp2 + recipes * 50, GUI_name_singular(get_itemname(g_alchemy_recipes[i].outcome)));

				g_radio_name_list[recipes] = g_dtp2 + recipes * 50;
				array[recipes] = (signed char)i;
				recipes++;
			}
		}

		if (recipes != 0) {

			/* ask which recipe should be used */
			tw_bak = g_textbox_width;
			g_textbox_width = 7;

			answer = GUI_radio(get_tx(43), (signed char)recipes,
						g_radio_name_list[0], g_radio_name_list[1],
						g_radio_name_list[2], g_radio_name_list[3],
						g_radio_name_list[4], g_radio_name_list[5],
						g_radio_name_list[6], g_radio_name_list[7],
						g_radio_name_list[8], g_radio_name_list[9],
						g_radio_name_list[10], g_radio_name_list[11],
						g_radio_name_list[12], g_radio_name_list[13]);

			g_textbox_width = tw_bak;

			if (answer != -1) {

				recipe_index = array[answer -1];

				if (hero_has_ingrendients(hero, recipe_index)) {

					if (g_alchemy_recipes[recipe_index].ae_cost > hero->ae) {

						/* AE not sufficient => brewing not possible */
						sprintf(g_dtp2,	get_ttx(607), hero->alias);
						GUI_output(g_dtp2);

						retval = 0;
					} else {
						/* AE sufficient */

						if ((gs_current_loctype == LOCTYPE_INN) && (g_sleep_quality == -1)) {

							/* no room booked => brewing not possible */
							GUI_output(get_ttx(346));

							return 0;
						}

						if ((g_alchemy_recipes[recipe_index].duration > 8) && (gs_current_loctype != LOCTYPE_INN)) {

							/* recipes with durations > 8 hours have to be done in a inn. */
							sprintf(g_dtp2,	get_tx(44), g_alchemy_recipes[recipe_index].duration);
							GUI_output(g_dtp2);

							retval = 0;
						} else {
							if (
#ifndef M302de_ORIGINAL_BUGFIX
								/* Original-Bug 12: If the total number of heroes is >= 2, and the current group consists only of a single available hero (+possibly the non-brewing NPC),
								 * the option to separate that hero from the group is shown, resulting in an active group which is empty or contains only the NPC and dead/petrified/unconscious/renegade heroes
								 * See https://www.crystals-dsa-foren.de/showthread.php?tid=98&pid=166399#pid166399 and the following posts. */
								(gs_total_hero_counter > 1) &&
#else
								((hero == (struct struct_hero*)get_hero(6)) || (count_heroes_available_in_group_ignore_npc() > 1)) && /* still allow to single out the NPC if he is the brewing hero */
#endif
								(gs_current_loctype != LOCTYPE_WILDCAMP) &&
								(g_alchemy_recipes[recipe_index].duration > 8)
							) {

								sprintf(g_dtp2,	get_tx(45), g_alchemy_recipes[recipe_index].duration);

								sprintf(g_text_output_buf, get_tx(47), hero->alias);

								g_textbox_width = 7;

								do {
									decision = GUI_radio(g_dtp2, 3,
											get_tx(46), /* Die Zeit einfach verstreichen lassen */
											g_text_output_buf, /* <Held> von der Gruppe trennen */
											get_tx(48)); /* Lieber doch nicht brauen */
								} while (decision == -1);

								g_textbox_width = 3;

							} else {
								decision = 1;
							}

							if (decision == 1) {
								/* rest of group waits */
								timewarp(HOURS(g_alchemy_recipes[recipe_index].duration));

								if (gs_current_loctype != LOCTYPE_WILDCAMP) {

									hero_p = (struct struct_hero*)get_hero(0);
									for (i = 0; i <= 6; i++, hero_p++) {
										if ((hero_p->typus != HERO_TYPE_NONE) && (hero_p->group_no == gs_current_group))
										{
											GRP_hero_sleep((Bit8u*)hero_p, g_sleep_quality);
										}
									}
								} else {
									hero->staffspell_timer = DAYS(1); /* TODO: Why STAFFSPELL ?? */
								}

								retval = do_alchemy(hero, recipe_index, 0);

								g_sleep_quality = -1;

							} else if (decision == 2) {
								/* split group */

								/* find an empty group */
								/* Original-Bug: only 6 groups, but 7 heroes might cause an 'out of boundary' here */
								for (l5 = 0; gs_group_member_counts[l5] != 0; l5++);

								hero->group_no = (signed char)l5;
								gs_group_member_counts[l5]++;
								gs_group_member_counts[gs_current_group]--;

								/* time in days, rounded down */
								hero->recipe_timer = g_alchemy_recipes[recipe_index].duration / 24;

								hero->recipe_id = recipe_index;
								hero->alchemy_inn_id = gs_current_typeindex;
								or_ptr_bs((Bit8u*)hero + HERO_FLAGS1, 8); /* set 'brewing' flag */

								GRP_save_pos(l5);
							} else {
								/* decision == 3, abort brewing process */
								retval = 0;
							}
						}
					}
				} else {
					/* not all ingrendients */
					sprintf(g_dtp2, get_tx(49), GUI_name_singular(get_itemname(g_alchemy_missing_item)));
					GUI_output(g_dtp2);
				}
			}
		} else {
			/* no recipes */
			sprintf(g_dtp2, get_tx(50), hero->alias);
			GUI_output(g_dtp2);
		}
	}

	return retval;
}

signed short has_herb_for_disease(struct struct_hero *hero, const signed short disease_id)
{
	signed short retval = 0;

	switch (disease_id) {

		case ILLNESS_TYPE_WUNDFIEBER:
		case ILLNESS_TYPE_BLAUE_KEUCHE:
			retval = 99;
			break;

		case ILLNESS_TYPE_DUMPFSCHAEDEL:
			/* any single one of the following herbs is sufficient */
			if (get_item_pos((Bit8u*)hero, ITEM_BELMART)          != -1) retval = ITEM_BELMART;
			else if (get_item_pos((Bit8u*)hero, ITEM_WHIRLWEED)   != -1) retval = ITEM_WHIRLWEED;
			else if (get_item_pos((Bit8u*)hero, ITEM_EINBEERE)    != -1) retval = ITEM_EINBEERE;
			else if (get_item_pos((Bit8u*)hero, ITEM_TARNELE)     != -1) retval = ITEM_TARNELE;
			else if (get_item_pos((Bit8u*)hero, ITEM_DONF_SPRING) != -1) retval = ITEM_DONF_SPRING;
			else if (get_item_pos((Bit8u*)hero, ITEM_FINAGE_TREE) != -1) retval = ITEM_FINAGE_TREE;
			else if (get_item_pos((Bit8u*)hero, ITEM_MENCHAL)     != -1) retval = ITEM_MENCHAL;
			else if (get_item_pos((Bit8u*)hero, ITEM_OLGIN_ROOT)  != -1) retval = ITEM_OLGIN_ROOT;
			else if (get_item_pos((Bit8u*)hero, ITEM_JORUGA_ROOT) != -1) retval = ITEM_JORUGA_ROOT;
			break;

		case ILLNESS_TYPE_PARALYSE:
			if (get_item_pos((Bit8u*)hero, ITEM_DONF_SPRING) != -1)
				retval = ITEM_DONF_SPRING;
			break;

		case ILLNESS_TYPE_SCHLACHTENFIEBER:
			if ( (get_item_pos((Bit8u*)hero, ITEM_JORUGA_ROOT) != -1) && (get_item_pos((Bit8u*)hero, ITEM_GULMOND_LEAF) != -1)) retval = 999;
			break;

		case ILLNESS_TYPE_FROSTSCHAEDEN:
			if (get_item_pos((Bit8u*)hero, ITEM_WHIRLWEED) != -1) retval = ITEM_WHIRLWEED;
			break;

		case ILLNESS_TYPE_TOLLWUT:
			if (get_item_pos((Bit8u*)hero, ITEM_JORUGA_ROOT) != -1) retval = ITEM_JORUGA_ROOT;
			break;
	}

	return retval;
}

signed short skill_cure_disease(struct struct_hero *healer, struct struct_hero *patient, const signed short handycap, const signed short flag)
{
	signed short disease_id;
	signed short retval;

	signed short damage;
	signed short tx_file_bak;
	signed short herb;

	retval = 0;

	if (flag) {
		tx_file_bak = g_tx_file_index;
		load_tx(ARCHIVE_FILE_SPELLTXT_LTX);
	}

	if (is_hero_healable(patient)) {

		disease_id = hero_is_diseased(patient);

		if (!disease_id) {

			/* not diseased */
			sprintf(g_dtp2,	get_ttx(462), patient->alias);
			GUI_output(g_dtp2);

		} else if (patient->heal_timer > 0) {

			/* recently tried to cure with skill */
			sprintf(g_dtp2,	get_ttx(697), patient->alias);
			GUI_output(g_dtp2);

		} else if (!(herb = has_herb_for_disease(healer, disease_id))) {

			/* not the needed herbs for healing this disease */
			sprintf(g_dtp2, get_tx(118), healer->alias);
			GUI_output(g_dtp2);

		} else {
			timewarp(MINUTES(5));

			/* set timer */
			patient->heal_timer = HOURS(4);

			if ((flag != 0) || (test_skill((struct struct_hero*)(Bit8u*)healer, TA_HEILEN_KRANKHEITEN, (signed char)handycap) > 0)) {

				if (((retval = test_skill((struct struct_hero*)(Bit8u*)healer, TA_HEILEN_KRANKHEITEN, g_disease_prices[disease_id] + handycap)) > 0) &&
					(disease_id != ILLNESS_TYPE_WUNDFIEBER) && (disease_id != ILLNESS_TYPE_BLAUE_KEUCHE))
				{

					add_hero_le(patient, retval);

					sprintf(g_dtp2,	get_ttx(695), healer->alias, patient->alias, GUI_get_ptr(patient->sex, 3), retval);
					GUI_output(g_dtp2);

					/* cure the disease */
					patient->sick[disease_id][0] = 1;
					patient->sick[disease_id][1] = 0;

					if (herb == 999) {
						/* drop JORUGA & GULMOND LEAF */
						drop_item((struct struct_hero*)healer, get_item_pos((Bit8u*)healer, ITEM_JORUGA_ROOT), 1);
						drop_item((struct struct_hero*)healer, get_item_pos((Bit8u*)healer, ITEM_GULMOND_LEAF), 1);
					} else {
						/* drop the herb */
						drop_item((struct struct_hero*)healer, get_item_pos((Bit8u*)healer, herb), 1);
					}

					retval = 1;
				} else {
					/* skill test failed */
					damage = 3;

					if (patient->le <= damage) {
						/* don't kill the patient: at least 1 LE should remain */
						damage = patient->le - 1;
					}

					sub_hero_le((struct struct_hero*)patient, damage);

					sprintf(g_dtp2,	get_ttx(694), patient->alias, damage);
					GUI_output(g_dtp2);
				}

			} else {
				/* failed to heal */
				sprintf(g_dtp2,	get_ttx(696), healer->alias, patient->alias);
				GUI_output(g_dtp2);
			}
		}


		if ((flag != 0) && (tx_file_bak != -1) && (tx_file_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {
			load_tx(tx_file_bak);
		}
	}

	return retval;
}

struct struct_hero* get_heaviest_hero(void)
{
	struct struct_hero *hero;
	struct struct_hero *retval;

	signed short weight;
	signed short w_max;
	signed short i;

	w_max = 0;
	hero = (struct struct_hero*)get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group))
		{
			weight = hero->weight + hero->load;

			if (weight > w_max) {
				w_max = weight;
				retval = hero;
			}
		}
	}

	return retval;
}

signed short get_hero_weight(struct struct_hero *hero)
{
	return hero->weight + hero->load;
}

signed short get_skilled_hero_pos(const signed short skill_id)
{
	signed short i;
	signed short cur;

	signed short max;
	signed short pos;
	struct struct_hero *hero;

	max = -100;

	hero = (struct struct_hero*)get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group))
		{

			cur =	hero->attrib[g_skill_descriptions[skill_id].attrib1].current +
				hero->attrib[g_skill_descriptions[skill_id].attrib1].mod +
				hero->attrib[g_skill_descriptions[skill_id].attrib2].current +
				hero->attrib[g_skill_descriptions[skill_id].attrib2].mod +
				hero->attrib[g_skill_descriptions[skill_id].attrib3].current +
				hero->attrib[g_skill_descriptions[skill_id].attrib3].mod +
				hero->skills[skill_id];

			if (cur > max) {
				max = cur;
				pos = i;
			}
		}
	}

	return pos;
}

#if !defined(__BORLANDC__)
}
#endif
