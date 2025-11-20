/*
 *	Rewrite of DSA1 v3.02_de functions of seg108 (consume)
 *	Functions rewritten: 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg108.cpp
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"

/**
 * \brief   owner gives consumer an item from his inventory
 *
 * \param   owner       the owner
 * \param   consumer    the consumer
 * \param   inv_slot    the inventory slot of the owner
 */
void consume(struct struct_hero *owner, struct struct_hero *consumer, const signed int inv_slot)
{
	struct item_stats *item_desc;
	signed int item_id;

	signed int bad_elixir_item_id;
	signed int le_diff;

	signed int disease;
	signed int poison;
	signed int tw_bak;
	signed int consumer_idx;

	signed int tmp; /* multiple use: mod_slot, random_le_gain */
	signed int tmp2; /* multiple use: talent_id, flag, attrib_id, etc. */

	/* this hero cannot consume */
	if (check_hero(consumer) == 0) {
		return;
	}

	consumer_idx = get_hero_index(consumer);

	/* get item id */
	item_id = owner->inventory[inv_slot].item_id;

	/* get pointer to ITEMS.DAT */
	item_desc = &g_itemsdat[item_id];

	/* is nutrition */
	if (item_desc->flags.nutrition) {

		if (item_desc->subtype == NUTRITION_TYPE_FOOD) {
			/* eating */

#if !defined(__BORLANDC__)
				int diff = consumer->hunger - item_desc->table_index;
				/* note that, for nutrition, table_index contains the nutrition value */

				D1_INFO("%s isst %s mit Naehrwert %d. Der Hunger sinkt von %d auf %d\n",
					consumer->alias, GUI_name_singular(g_itemsname[item_id]),
					item_desc->table_index,
					consumer->hunger, (diff >= 0) ? diff : 0);
#endif

			/* subtract from hunger value */
			consumer->hunger -= item_desc->table_index;

			/* adjust hunger value */
			if (consumer->hunger < 0) {
				consumer->hunger = 0;
			}

			/* consume quietly */
			if (!g_consume_quiet) {
				GUI_output(get_ttx(207));
			}

			/* drop one unit of that item */
			drop_item(owner, inv_slot, 1);
		} else {
			/* drinking */

			/* check if item is not empty */
			if (!owner->inventory[inv_slot].flags.empty) {

#if !defined(__BORLANDC__)
				int diff = consumer->thirst - item_desc->table_index;
				D1_INFO("%s trinkt aus %s mit Naehrwert %d. Der Durst sinkt von %d auf %d\n",
					consumer->alias, GUI_name_singular(g_itemsname[item_id]), item_desc->table_index,
					consumer->thirst, (diff >= 0) ? diff : 0);
#endif

				/* subtract from thirst value */
				consumer->thirst -= item_desc->table_index;

				/* adjust thirst value */
				if (consumer->thirst < 0) {
					consumer->thirst = 0;
				}

				/* consume quietly */
				if (!g_consume_quiet) {
					GUI_output(get_ttx(208));
				}


				if (item_id == ITEM_WASSERSCHLAUCH) {
					/* water */

					if (owner->inventory[inv_slot].flags.half_empty) {
						owner->inventory[inv_slot].flags.empty = 1;
					} else {
						owner->inventory[inv_slot].flags.half_empty = 1;
					}

				} else if (item_id == ITEM_SCHNAPSFLASCHE || item_id == ITEM_WEINFLASCHE) {
					/* wine or snaps */
					hero_get_drunken(consumer);
					drop_item(owner, inv_slot, 1);
				} else {
					/* everything else: Beer */
					drop_item(owner, inv_slot, 1);

					/* That does not happen */
					if (item_id != ITEM_BIER) {
						/* get an empty glass bottle */
						give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);
					}
				}
			} else {
				/* print empty message */
				GUI_output(get_ttx(429));
			}
		}

		g_request_refresh = 1;

	} else if (item_desc->flags.herb_potion) {

		if (item_desc->subtype == HERB_POTION_TYPE_HERB_AND_POISON) {

			if (is_in_int_array(item_id, g_herbs_uneatable)) {

				GUI_output(get_ttx(499));

			} else if (is_in_int_array(item_id, g_herbs_toxic) || is_in_int_array(item_id, g_weapon_poisons)) {
				/* herbs and poisons */
				GUI_output(get_ttx(500));

			} else {
				/* consume with effects */

				/* drop one entity */
				drop_item(owner, inv_slot, 1);

				/* terminate output string */
				*g_dtp2 = '\0';

				switch (item_id) {
				case ITEM_GULMOND_BLATT: {
					/* Gulmond Blatt */

					/* KK+2 for 12h */
					tmp = get_free_mod_slot();
					set_mod_slot(tmp, HOURS(12), (uint8_t*)(&consumer->attrib[ATTRIB_KK].current), 2, (signed char)consumer_idx);

					/* LE + 2 */
					add_hero_le(consumer, 2);

					/* prepare output */
					strcpy(g_dtp2, get_ttx(501));
					break;
				}
				case ITEM_EINBEERE: {
					/* Vierblaettrige Einbeere */

					tmp = random_schick(6);
					le_diff = consumer->le_max - consumer->le;
					if (tmp > le_diff)
						tmp = le_diff;

					add_hero_le(consumer, tmp);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(502), tmp);

					break;
				}
				case ITEM_BELMART_BLATT: {
					/* Belmart */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison <= 5) {

						/* cure the first poison */
						consumer->poison[poison][1] = 0;
						consumer->poison[poison][0] = 1;
					}

					/* TODO: unknown for 24h */
					tmp = get_free_mod_slot();
					set_mod_slot(tmp, DAYS(1), (uint8_t*)(&consumer->herbs), 1, (signed char)consumer_idx);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(503), consumer->alias);
					break;
				}
				case ITEM_MENCHALKAKTUS: {
					/* Menchalkaktus */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison <= 10) {
						/* cure the first poison */
						consumer->poison[poison][1] = 0;
						consumer->poison[poison][0] = 1;
					}

					tmp = get_free_mod_slot();
					set_mod_slot(tmp, DAYS(1), (uint8_t*)(&consumer->herbs), 2, (signed char)consumer_idx);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(503), consumer->alias);
					break;
				}
				case ITEM_ATMONBLUETE: {
					/* Atmon */
					for (tmp2 = 9; tmp2 < 19; tmp2++) {
						/* All body skills + 2 for 5h */
						tmp = get_free_mod_slot();
						set_mod_slot(tmp, HOURS(5), (uint8_t*)(&consumer->skills[tmp2]), 2, (signed char)consumer_idx);
					}

					/* prepare output */
					sprintf(g_dtp2, get_ttx(504), consumer->alias);
					break;
				}
				case ITEM_WIRSELKRAUT: {
					/* Wirselkraut */
					tmp = 10;
					le_diff = consumer->le_max - consumer->le;
					if (le_diff < tmp)
						tmp = le_diff;

					add_hero_le(consumer, tmp);
					/* prepare output */

					sprintf(g_dtp2, get_ttx(505), tmp);
					break;
				}
				case ITEM_TARNELE: {
					/* Tarnelle */
					consumer->ruhe_koerper = 1;
					break;
				}
				}

				/* print the output */
				GUI_output(g_dtp2);
			}
		} else {

			/* check if item is an elixir */
			tmp2 = is_in_int_array(item_id, g_elixir_potions);
			bad_elixir_item_id = is_in_int_array(item_id, g_bad_elixirs);

			if (tmp2 != 0) {
				/* handle good elixirs */

				/* drop elixir */
				drop_item(owner, inv_slot, 1);

				/* get glassbottle */
				give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

				/* Attribute +5 for 1h */
				tmp = get_free_mod_slot();
				set_mod_slot(tmp, HOURS(1), (uint8_t*)(&consumer->attrib[tmp2 - 1].current), 5, (signed char)consumer_idx);

				/* prepare output */
				sprintf(g_dtp2, get_ttx(509), consumer->alias, get_ttx(411 + tmp2), 5);

				/* print output */
				GUI_output(g_dtp2);

			} else if (bad_elixir_item_id != 0) {
				/* handle bad elixires */

				/* drop elixir */
				drop_item(owner, inv_slot, 1);

				/* get glassbottle */
				give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

				/* Attribute -7 for 1h */
				tmp = get_free_mod_slot();
				set_mod_slot(tmp, HOURS(1), (uint8_t*)(&consumer->attrib[bad_elixir_item_id - 1].current), -7, (signed char)consumer_idx);

				/* prepare output */
				sprintf(g_dtp2, get_ttx(656), consumer->alias, get_ttx(411 + bad_elixir_item_id), 7);

				/* print output */
				GUI_output(g_dtp2);

			} else {
				/* everything else */

				/* drop the item */
				drop_item(owner, inv_slot, 1);

				switch (item_id) {
				case ITEM_HEILTRANK : {
					/* Heiltrank */

					tmp2 = consumer->le_max - consumer->le;
					if (tmp2 > 10)
						tmp2 = 10;

					/* singular POINT/ PUNKT */
					strcpy(g_text_output_buf, get_ttx(392));

					if (tmp2 > 1) {
						/* make plural POINTS/PUNKTE */
						strcat(g_text_output_buf, get_ttx(393));
					}

					/* add LE */
					add_hero_le(consumer, tmp2);

					/* give owner a glassbottle */
					give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(510), consumer->alias, tmp2, g_text_output_buf);
					break;
				}
				case  ITEM_STARKER_HEILTRANK : {
					/* Starker Heiltrank */

					/* 1W20+10 */
					tmp2 = dice_roll(1, 20, 10);
					if (consumer->le_max - consumer->le < tmp2)
						tmp2 = consumer->le_max - consumer->le;

					/* add LE */
					add_hero_le(consumer, tmp2);

					/* give owner a copperbottle */
					give_new_item_to_hero(owner, ITEM_BRONZEFLASCHE, 2, 1);

					/* singular POINT/ PUNKT */
					strcpy(g_text_output_buf, get_ttx(392));

					if (tmp2 > 1) {
						/* make plural POINTS/PUNKTE */
						strcat(g_text_output_buf, get_ttx(393));
					}

					/* prepare output */
					sprintf(g_dtp2, get_ttx(510), consumer->alias, tmp2, g_text_output_buf);
					break;
				}
				case ITEM_WUNDERKUR: {
					/* Wunderkur */

					/* undo starvation damage */
					consumer->le_max += consumer->le_max_malus;
					consumer->le_max_malus = 0;

					/* fill up LE */
					if (consumer->le < consumer->le_max) {
						add_hero_le(consumer, consumer->le_max);
					}

					/* diseases, not all */
					disease = hero_is_diseased(consumer);

					if (disease == 2 || disease == 3) {
						consumer->sick[disease][0] = 1;
						consumer->sick[disease][1] = 0;
					}

					/* poison */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison < 7) {
						consumer->poison[poison][1] = 0;
						consumer->poison[poison][0] = 1;
					}

					/* give owner a glasbottle */
					give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(511), consumer->alias, GUI_get_ptr(consumer->sex, 0));

					break;
				}
				case ITEM_SCHLAFTRUNK: {
					/* Schlaftrunk */

					/* 3 Rounds of sleep */
					consumer->ruhe_koerper = 3;

					/* give owner a glasbottle */
					give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(738), consumer->alias);
					break;
				}
				case ITEM_ZAUBERTRANK: {
					/* Zaubertrank */

					if (consumer->typus >= HERO_TYPE_HEXE) {
						/* Magicuser */

						tmp2 = consumer->ae_max - consumer->ae;

						if (tmp2 > 10)
							tmp2 = 10;

						/* add AE */
						add_hero_ae(consumer, tmp2);

						/* give hero a glassbottle */
						give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

						/* prepare output */
						strcpy(g_text_output_buf, get_ttx(392));

						if (tmp2 > 1) {
							strcat(g_text_output_buf, get_ttx(393));
						}

						sprintf(g_dtp2, get_ttx(512), tmp2, g_text_output_buf, consumer->alias);
					} else {
						/* Not a magicuser */

						strcpy(g_dtp2, get_ttx(804));

						/* give owner a glassbottle */
						give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);
					}

					break;
				}
				case ITEM_ZAUBERTRANK_STARK: {
					/* Zaubertrank (stark) */

					if (consumer->typus >= HERO_TYPE_HEXE) {
						/* Magicuser */

						tmp2 = consumer->ae_max - consumer->ae;

						if (tmp2 > 30)
							tmp2 = 30;

						/* add AE */
						add_hero_ae(consumer, tmp2);

						/* give owner a glassbottle */
						give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

						/* prepare output */
						if (consumer->ae >= consumer->ae_max) {
							sprintf(g_dtp2, get_ttx(513), consumer->alias);
						} else {
							strcpy(g_text_output_buf, get_ttx(392));

							if (tmp2 > 1) {
								strcat(g_text_output_buf, get_ttx(393));
							}

							sprintf(g_dtp2, get_ttx(512), tmp2, g_text_output_buf, consumer->alias);
						}
					} else {
						/* Not a magicuser */
						strcpy(g_dtp2, get_ttx(804));

						/* give owner a glassbottle */
						give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);
					}
					break;
				}
				case ITEM_GEGENGIFT: {
					/* Gegengift */

					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison < 5) {
						consumer->poison[poison][1] = 0;
						consumer->poison[poison][0] = 1;
					}

					/* give owner a glassbottle */
					give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

					sprintf(g_dtp2, get_ttx(467), consumer->alias);
					break;
				}
				case ITEM_ANTIKRANKHEITSELIXIER: {
					/* Antikrankheitselexier */

					disease = hero_is_diseased(consumer);

					consumer->sick[disease][0] = 1;
					consumer->sick[disease][1] = 0;

					/* give owner a glassbottle */
					give_new_item_to_hero(owner, ITEM_GLASFLASCHE, 2, 1);

					sprintf(g_dtp2, get_ttx(467), consumer->alias);
					break;

				}
				}

				/* print output: maybe garbage - string not set to 0 */
				tw_bak = g_textbox_width;
				g_textbox_width = 5;
				GUI_output(g_dtp2);
				g_textbox_width = tw_bak;
			}
		}

		g_request_refresh = 1;
	} else {
		/* this item cannot be consumed */
		GUI_output(get_ttx(206));
	}
}
