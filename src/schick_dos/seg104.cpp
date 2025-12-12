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
#include "seg104.h"
#include "seg105.h"

static struct struct_recipe g_alchemy_recipes[13] = {
	{
		ITEM_REZEPT_FUER_HEILTRANK,
		ITEM_GLASFLASCHE, ITEM_EINBEERE, ITEM_EINBEERE, ITEM_TARNELE, ITEM_SCHNAPSFLASCHE,
		-1, -1, -1, -1, -1,
		ITEM_HEILTRANK,	5, 2, 8
	},
	{
		ITEM_REZEPT_FUER_STARKEN_HEILTRANK,
		ITEM_BRONZEFLASCHE, ITEM_WIRSELKRAUT, ITEM_WIRSELKRAUT, ITEM_TARNELE, ITEM_SCHNAPSFLASCHE,
		ITEM_ALRAUNE, -1, -1, -1, -1,
		ITEM_STARKER_HEILTRANK, 12, 5, 24
	},
	{
		ITEM_REZEPT_FUER_WUNDERKUR,
		ITEM_GLASFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_WIRSELKRAUT, ITEM_WIRSELKRAUT, ITEM_DONFSTENGEL,
		ITEM_BELMART_BLATT, ITEM_ALRAUNE, ITEM_JORUGAWURZEL, -1, -1,
		ITEM_WUNDERKUR,	25, 10, 3 * 24
	},
	{
		ITEM_REZEPT_FUER_GEGENGIFT,
		ITEM_GLASFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_MENCHALKAKTUS, ITEM_MENCHALKAKTUS, ITEM_ALRAUNE,
		ITEM_SHURINKNOLLE, -1, -1, -1, -1,
		ITEM_GEGENGIFT, 10, 5, 24
	},
	{
		ITEM_REZEPT_FUER_SCHLAFTRUNK,
		ITEM_GLASFLASCHE, ITEM_WEINFLASCHE, ITEM_GULMOND_BLATT, ITEM_TARNELE, -1,
		-1, -1, -1, -1, -1,
		ITEM_SCHLAFTRUNK, 2, 2, 8
	},
	{
		ITEM_REZEPT_FUER_ZAUBERTRANK,
		ITEM_GLASFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_ALRAUNE, ITEM_ALRAUNE, ITEM_KAIRANHALM,
		ITEM_KAIRANHALM, ITEM_THONNYSBLUETE, -1, -1, -1,
		ITEM_ZAUBERTRANK,	7, 8, 2 * 24
	},
	{
		ITEM_REZEPT_FUER_STARKER_ZAUBERTRANK,
		ITEM_GLASFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_ALRAUNE, ITEM_ALRAUNE, ITEM_ALRAUNE,
		ITEM_KAIRANHALM, ITEM_KAIRANHALM, ITEM_KAIRANHALM, ITEM_THONNYSBLUETE, -1,
		ITEM_ZAUBERTRANK_STARK, 15, 12, 5 * 24
	},
	{
		ITEM_REZEPT_FUER_VOMICUM,
		ITEM_BRONZEFLASCHE, ITEM_OEL, ITEM_SHURINKNOLLE, ITEM_ALRAUNE, -1,
		-1, -1, -1, -1, -1,
		ITEM_VOMICUM, 0, 5, 8
	},
	{
		ITEM_REZEPT_FUER_EXPURGICUM,
		ITEM_BRONZEFLASCHE, ITEM_LAKRITZE, ITEM_SHURINKNOLLE, ITEM_SHURINKNOLLE, ITEM_ALRAUNE,
		ITEM_ALRAUNE, -1, -1, -1, -1,
		ITEM_EXPURGICUM, 0, 8, 2 * 24
	},
	{
		ITEM_REZEPT_FUER_MIASTHMATICUM,
		ITEM_GLASFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_KAIRANHALM, ITEM_ILMENBLATT, ITEM_SHURINKNOLLE,
		ITEM_ALRAUNE, -1, -1, -1, -1,
		ITEM_MIASTHMATICUM, 0, 6, 2 * 24
	},
	{
		ITEM_REZEPT_FUER_MUTELIXIER,
		ITEM_GLASFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_FINAGEBAEMCHEN, ITEM_ALRAUNE, ITEM_DONFSTENGEL,
		ITEM_ILMENBLATT, -1, -1, -1, -1,
		ITEM_MU_ELIXIER, 12, 7, 2 * 24
	},
	{
		ITEM_REZEPT_FUER_KRAFTELIXIER,
		ITEM_GLASFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_FINAGEBAEMCHEN, ITEM_ALRAUNE, ITEM_GULMOND_BLATT,
		ITEM_GULMOND_BLATT, -1, -1, -1, -1,
		ITEM_KK_ELIXIER, 15, 8, 2 * 24
	},
	{
		ITEM_REZEPT_FUER_HYLAILER_FEUER,
		ITEM_GLASFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_SCHNAPSFLASCHE, ITEM_LAKRITZE, ITEM_OEL,
		ITEM_OEL, -1, -1, -1, -1,
		ITEM_HYLAILER_FEUER, 0, 5, 8
	}
}; // ds:0xacda


static signed int g_alchemy_missing_item; // ds:0xe5c4

signed int hero_has_ingrendients(struct struct_hero *hero, const signed int recipe_id)
{
	signed int i = 0;
	signed int retval = 1;
	struct struct_recipe *r_ptr = &g_alchemy_recipes[recipe_id];
	signed int inv_slot;

	/* loop over ingrendients */
	while ((r_ptr->ingredients[i] != -1) && retval) {

		inv_slot = inv_slot_of_item(hero, r_ptr->ingredients[i]);

		if (inv_slot == -1) {

			/* needed item missing */
			retval = 0;

			/* TODO: g_alchemy_missing_item should not be a globvar */
			g_alchemy_missing_item = r_ptr->ingredients[i];
		} else {
			/* drop all needed items */
			drop_item(hero, inv_slot, 1);
		}

		i++;
	}

	i--;

	if (!retval) {
		i--;
	}

	while (i >= 0) {
		/* give all needed items back */
		give_new_item_to_hero(hero, r_ptr->ingredients[i], 1, 1);
		i--;
	}

	return retval;
}

void hero_use_ingrendients(struct struct_hero *hero, const signed int recipe_id)
{
	signed int i = 0;
	struct struct_recipe *r_ptr = &g_alchemy_recipes[recipe_id];
	signed int inv_slot;

	/* loop over ingredients */
	while (r_ptr->ingredients[i] != -1) {

		inv_slot = inv_slot_of_item(hero, r_ptr->ingredients[i]);

		/* drop the needed item */
		drop_item(hero, inv_slot, 1);

		/* exchange wine- or brandybottles into glass flask */
		if ((r_ptr->ingredients[i] == ITEM_WEINFLASCHE) || (r_ptr->ingredients[i] == ITEM_SCHNAPSFLASCHE))
		{
			give_new_item_to_hero(hero, ITEM_GLASFLASCHE, 1, 1);
		}

		/* exchange oil into bronze flask */
		if (r_ptr->ingredients[i] == ITEM_OEL)
		{
			give_new_item_to_hero(hero, ITEM_BRONZEFLASCHE, 1, 1);
		}

		i++;
	}
}

signed int do_alchemy(struct struct_hero* hero, const signed int recipe_id, const signed int flag_abort)
	/* flag_abort = 0: finalize running brewing process
	 * flag_abort = 1: abort running brewing process */
{
	struct struct_recipe *r_ptr = &g_alchemy_recipes[recipe_id];

	hero_use_ingrendients(hero, recipe_id);

	sub_ae_splash(hero, r_ptr->ae_cost);

	hero->flags.brewing = 0;

	hero->recipe_timer = 0;
	/* set heroes receipe to 0 */
	hero->recipe_id = 0;
	hero->alchemy_inn_typeindex = 0;

	if ((test_talent(hero, TA_ALCHIMIE, r_ptr->handicap) > 0) && (flag_abort == 0))
	{
		/* success */

		give_new_item_to_hero(hero, r_ptr->outcome, 1, 1);

		sprintf(g_dtp2, get_ttx(731), hero->alias, (char*)GUI_names_grammar(1, r_ptr->outcome, 0));
		GUI_output(g_dtp2);

		return 1;
	} else {
		/* failure */
		/* give first ingredient back, which is always the bottle (glass or bronze). */
		give_new_item_to_hero(hero, r_ptr->ingredients[0], 1, 1);

		sprintf(g_dtp2,	get_ttx(732), hero->alias, (char*)GUI_names_grammar(2, r_ptr->outcome, 0));
		GUI_output(g_dtp2);

		return 0;
	}

}

signed int plan_alchemy(struct struct_hero *hero)
{
	signed int retval;
	signed int inv_slot;
	signed int recipes;
	signed int answer;
	signed int decision;
	signed int l5;
	signed int i;
	signed char recipe_index;
	struct struct_hero *hero_p;
	signed int tw_bak;
	signed char array[13];


	retval = 1;
	recipes = 0;
	inv_slot = inv_slot_of_item(hero, ITEM_ALCHIMIESET);

	if (inv_slot == -1) {

		/* no alchemy kit */
		GUI_output(get_tx(42));
		retval = 0;
	} else {

		/* count all recipes and prepare the menu */
		for (i = 0; i <= 12; i++) {
			if (inv_slot_of_item(hero, g_alchemy_recipes[i].item_id) != -1) {

				strcpy(g_dtp2 + recipes * 50, GUI_name_singular(g_itemsname[g_alchemy_recipes[i].outcome]));

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

						if ((gs_town_loc_type == LOCTYPE_INN) && (g_sleep_quality == -1)) {

							/* no room booked => brewing not possible */
							GUI_output(get_ttx(346));

							return 0;
						}

						if ((g_alchemy_recipes[recipe_index].duration > 8) && (gs_town_loc_type != LOCTYPE_INN)) {

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
								((hero == get_hero(6)) || (count_heroes_available_in_group_ignore_npc() > 1)) && /* still allow to single out the NPC if he is the brewing hero */
#endif
								(gs_town_loc_type != LOCTYPE_WILDCAMP) &&
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

								if (gs_town_loc_type != LOCTYPE_WILDCAMP) {

									hero_p = get_hero(0);
									for (i = 0; i <= 6; i++, hero_p++) {
										if ((hero_p->typus != HERO_TYPE_NONE) && (hero_p->group_id == gs_active_group_id))
										{
											GRP_hero_sleep(hero_p, g_sleep_quality);
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

								hero->group_id = (signed char)l5;
								gs_group_member_counts[l5]++;
								gs_group_member_counts[gs_active_group_id]--;

								/* time in days, rounded down */
								hero->recipe_timer = g_alchemy_recipes[recipe_index].duration / 24;

								hero->recipe_id = recipe_index;
								hero->alchemy_inn_typeindex = gs_town_typeindex;

								hero->flags.brewing = 1;

								GRP_save_pos(l5);
							} else {
								/* decision == 3, abort brewing process */
								retval = 0;
							}
						}
					}
				} else {
					/* not all ingrendients */
					sprintf(g_dtp2, get_tx(49), GUI_name_singular(g_itemsname[g_alchemy_missing_item]));
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

signed int has_herb_for_disease(const struct struct_hero *hero, const signed int disease_id)
{
	signed int retval = 0;

	switch (disease_id) {

		case DISEASE_ID_WUNDFIEBER:
		case DISEASE_ID_BLAUE_KEUCHE:
			retval = 99;
			break;

		case DISEASE_ID_DUMPFSCHAEDEL:
			/* any single one of the following herbs is sufficient */
			if (inv_slot_of_item(hero, ITEM_BELMART_BLATT)          != -1) retval = ITEM_BELMART_BLATT;
			else if (inv_slot_of_item(hero, ITEM_WIRSELKRAUT)   != -1) retval = ITEM_WIRSELKRAUT;
			else if (inv_slot_of_item(hero, ITEM_EINBEERE)    != -1) retval = ITEM_EINBEERE;
			else if (inv_slot_of_item(hero, ITEM_TARNELE)     != -1) retval = ITEM_TARNELE;
			else if (inv_slot_of_item(hero, ITEM_DONFSTENGEL) != -1) retval = ITEM_DONFSTENGEL;
			else if (inv_slot_of_item(hero, ITEM_FINAGEBAEMCHEN) != -1) retval = ITEM_FINAGEBAEMCHEN;
			else if (inv_slot_of_item(hero, ITEM_MENCHALKAKTUS)     != -1) retval = ITEM_MENCHALKAKTUS;
			else if (inv_slot_of_item(hero, ITEM_OLGINWURZEL)  != -1) retval = ITEM_OLGINWURZEL;
			else if (inv_slot_of_item(hero, ITEM_JORUGAWURZEL) != -1) retval = ITEM_JORUGAWURZEL;
			break;

		case DISEASE_ID_PARALYSE:
			if (inv_slot_of_item(hero, ITEM_DONFSTENGEL) != -1)
				retval = ITEM_DONFSTENGEL;
			break;

		case DISEASE_ID_SCHLACHTENFIEBER:
			if ( (inv_slot_of_item(hero, ITEM_JORUGAWURZEL) != -1) && (inv_slot_of_item(hero, ITEM_GULMOND_BLATT) != -1)) retval = 999;
			break;

		case DISEASE_ID_FROSTSCHAEDEN:
			if (inv_slot_of_item(hero, ITEM_WIRSELKRAUT) != -1) retval = ITEM_WIRSELKRAUT;
			break;

		case DISEASE_ID_TOLLWUT:
			if (inv_slot_of_item(hero, ITEM_JORUGAWURZEL) != -1) retval = ITEM_JORUGAWURZEL;
			break;
	}

	return retval;
}

signed int talent_cure_disease(struct struct_hero *healer, struct struct_hero *patient, const signed int handicap, const signed int flag)
{
	signed int disease_id;
	signed int retval;

	signed int damage;
	signed int tx_file_bak;
	signed int herb;

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

			/* recently tried to cure with talent */
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

			if ((flag != 0) || (test_talent(healer, TA_HEILEN_KRANKHEITEN, (signed char)handicap) > 0)) {

				if (((retval = test_talent(healer, TA_HEILEN_KRANKHEITEN, g_diseases_healer_price[disease_id] + handicap)) > 0) &&
					(disease_id != DISEASE_ID_WUNDFIEBER) && (disease_id != DISEASE_ID_BLAUE_KEUCHE))
				{

					add_hero_le(patient, retval);

					sprintf(g_dtp2,	get_ttx(695), healer->alias, patient->alias, GUI_get_ptr(patient->sex, 3), retval);
					GUI_output(g_dtp2);

					/* cure the disease */
					patient->disease[disease_id].status = DISEASE_STATUS_RECOVER;
					patient->disease[disease_id].time_counter = 0;

					if (herb == 999) {
						/* drop JORUGA & GULMOND LEAF */
						drop_item(healer, inv_slot_of_item(healer, ITEM_JORUGAWURZEL), 1);
						drop_item(healer, inv_slot_of_item(healer, ITEM_GULMOND_BLATT), 1);
					} else {
						/* drop the herb */
						drop_item(healer, inv_slot_of_item(healer, herb), 1);
					}

					retval = 1;
				} else {
					/* talent test failed */
					damage = 3;

					if (patient->le <= damage) {
						/* don't kill the patient: at least 1 LE should remain */
						damage = patient->le - 1;
					}

					sub_hero_le(patient, damage);

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

	signed int weight;
	signed int w_max;
	signed int i;

	w_max = 0;
	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id))
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

signed int get_hero_weight(const struct struct_hero *hero)
{
	return hero->weight + hero->load;
}

signed int get_talented_hero_pos(const signed int talent_id)
{
	signed int i;
	signed int cur;

	signed int max;
	signed int pos;
	struct struct_hero *hero;

	max = -100;

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id))
		{

			cur =	hero->attrib[g_talent_descriptions[talent_id].attrib1].current +
				hero->attrib[g_talent_descriptions[talent_id].attrib1].mod +
				hero->attrib[g_talent_descriptions[talent_id].attrib2].current +
				hero->attrib[g_talent_descriptions[talent_id].attrib2].mod +
				hero->attrib[g_talent_descriptions[talent_id].attrib3].current +
				hero->attrib[g_talent_descriptions[talent_id].attrib3].mod +
				hero->talents[talent_id];

			if (cur > max) {
				max = cur;
				pos = i;
			}
		}
	}

	return pos;
}
