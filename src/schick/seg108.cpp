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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   owner gives consumer an item from his inventory
 *
 * \param   owner       the owner
 * \param   consumer    the consumer
 * \param   pos         the position in the inventory of the owner
 */
void consume(struct struct_hero *owner, struct struct_hero *consumer, const signed short pos)
{
	struct item_stats *item_p;
	signed short item;

	signed short id_bad_elex;
	signed short le_diff;

	signed short disease;
	signed short poison;
	signed short tw_bak;
	signed short consumer_idx;

	signed short l_di, l_si;

	/* this hero cannot consume */
	if (check_hero((Bit8u*)consumer) == 0) {
		return;
	}

	consumer_idx = get_hero_index(consumer);

	/* get item id */
	item = owner->inventory[pos].item_id;

	/* get pointer to ITEMS.DAT */
	item_p = &g_itemsdat[item];

	/* is food */
	if (item_p->flags.food) {

		if (item_p->subtype == 1) {
			/* eating */

#if !defined(__BORLANDC__)
				int diff = consumer->hunger - item_p->table_index;

				D1_INFO("%s isst %s mit Naehrwert %d. Der Hunger sinkt von %d auf %d\n",
					consumer->alias, GUI_name_singular(get_itemname(item)),	item_p->table_index,
					consumer->hunger, (diff >= 0) ? diff : 0);
#endif

			/* subtract from hunger value */
			consumer->hunger -= item_p->table_index;

			/* adjust hunger value */
			if (consumer->hunger < 0) {
				consumer->hunger = 0;
			}

			/* consume quietly */
			if (!g_consume_quiet) {
				GUI_output(get_ttx(207));
			}

			/* drop one unit of that item */
			drop_item(owner, pos, 1);
		} else {
			/* drinking */

			/* check if item is not empty */
			if (!owner->inventory[pos].flags.empty) {

#if !defined(__BORLANDC__)
				int diff = consumer->thirst - item_p->table_index;
				D1_INFO("%s trinkt aus %s mit Naehrwert %d. Der Durst sinkt von %d auf %d\n",
					consumer->alias, GUI_name_singular(get_itemname(item)), item_p->table_index,
					consumer->thirst, (diff >= 0) ? diff : 0);
#endif

				/* subtract from thirst value */
				consumer->thirst -= item_p->table_index;

				/* adjust thirst value */
				if (consumer->thirst < 0) {
					consumer->thirst = 0;
				}

				/* consume quietly */
				if (!g_consume_quiet) {
					GUI_output(get_ttx(208));
				}


				if (item == ITEM_WATERSKIN) {
					/* water */

					if (owner->inventory[pos].flags.half_empty) {
						owner->inventory[pos].flags.empty = 1;
					} else {
						owner->inventory[pos].flags.half_empty = 1;
					}

				} else if (item == ITEM_BRANDY || item == ITEM_WINE) {
					/* wine or snaps */
					hero_get_drunken(consumer);
					drop_item(owner, pos, 1);
				} else {
					/* everything else: Beer */
					drop_item(owner, pos, 1);

					/* That does not happen */
					if (item != ITEM_BEER) {
						/* get an empty glass bottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);
					}
				}
			} else {
				/* print empty message */
				GUI_output(get_ttx(429));
			}
		}

		g_request_refresh = 1;

	} else if (item_p->flags.herb_potion) {

		if (item_p->subtype == 0) {

			if (is_in_word_array(item, g_herbs_uneatable)) {
				GUI_output(get_ttx(499));
			} else if (is_in_word_array(item, g_herbs_toxic) || is_in_word_array(item, g_poison_potions)) {
				/* herbs and poisons */
				GUI_output(get_ttx(500));
			} else {
				/* consume with effects */

				/* drop one entity */
				drop_item(owner, pos, 1);

				/* terminate output string */
				*g_dtp2 = '\0';

				switch (item) {
				case ITEM_GULMOND_LEAF: {
					/* Gulmond Blatt */

					/* KK+2 for 12h */
					l_di = get_free_mod_slot();
					set_mod_slot(l_di, HOURS(12), (Bit8u*)(&consumer->attrib[ATTRIB_KK].current), 2, (signed char)consumer_idx);

					/* LE + 2 */
					add_hero_le(consumer, 2);

					/* prepare output */
					strcpy(g_dtp2, get_ttx(501));
					break;
				}
				case ITEM_EINBEERE: {
					/* Vierblaettrige Einbeere */

					l_di = random_schick(6);
					le_diff = consumer->le_max - consumer->le;
					if (l_di > le_diff)
						l_di = le_diff;

					add_hero_le(consumer, l_di);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(502), l_di);

					break;
				}
				case ITEM_BELMART: {
					/* Belmart */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison <= 5) {

						/* cure the first poison */
						consumer->poison[poison][1] = 0;
						consumer->poison[poison][0] = 1;
					}

					/* TODO: unknown for 24h */
					l_di = get_free_mod_slot();
					set_mod_slot(l_di, DAYS(1), (Bit8u*)(&consumer->herbs), 1, (signed char)consumer_idx);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(503), consumer->alias);
					break;
				}
				case ITEM_MENCHAL: {
					/* Menchalkaktus */
					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison <= 10) {
						/* cure the first poison */
						consumer->poison[poison][1] = 0;
						consumer->poison[poison][0] = 1;
					}

					l_di = get_free_mod_slot();
					set_mod_slot(l_di, DAYS(1), (Bit8u*)(&consumer->herbs), 2, (signed char)consumer_idx);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(503), consumer->alias);
					break;
				}
				case ITEM_ATMONBLUETE: {
					/* Atmon */
					for (l_si = 9; l_si < 19; l_si++) {
						/* All body skills + 2 for 5h */
						l_di = get_free_mod_slot();
						set_mod_slot(l_di, HOURS(5), (Bit8u*)(&consumer->skills[l_si]), 2, (signed char)consumer_idx);
					}

					/* prepare output */
					sprintf(g_dtp2, get_ttx(504), consumer->alias);
					break;
				}
				case ITEM_WHIRLWEED: {
					/* Wirselkraut */
					l_di = 10;
					le_diff = consumer->le_max - consumer->le;
					if (le_diff < l_di)
						l_di = le_diff;

					add_hero_le(consumer, l_di);
					/* prepare output */

					sprintf(g_dtp2, get_ttx(505), l_di);
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

			/* check if item is an elexire */
			l_si = is_in_word_array(item, g_elixir_potions);
			id_bad_elex = is_in_word_array(item, g_bad_elixirs);

			if (l_si != 0) {
				/* handle good elexires */

				/* drop elexire */
				drop_item(owner, pos, 1);

				/* get glassbottle */
				give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

				/* Attribute +5 for 1h */
				l_di = get_free_mod_slot();
				set_mod_slot(l_di, HOURS(1), (Bit8u*)(&consumer->attrib[l_si - 1].current), 5, (signed char)consumer_idx);

				/* prepare output */
				sprintf(g_dtp2, get_ttx(509), consumer->alias, get_ttx(411 + l_si), 5);

				/* print output */
				GUI_output(g_dtp2);

			} else if (id_bad_elex != 0) {
				/* handle bad elexires */

				/* drop elexire */
				drop_item(owner, pos, 1);

				/* get glassbottle */
				give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

				/* Attribute -7 for 1h */
				l_di = get_free_mod_slot();
				set_mod_slot(l_di, HOURS(1), (Bit8u*)(&consumer->attrib[id_bad_elex - 1].current), -7, (signed char)consumer_idx);

				/* prepare output */
				sprintf(g_dtp2, get_ttx(656), consumer->alias, get_ttx(411 + id_bad_elex), 7);

				/* print output */
				GUI_output(g_dtp2);

			} else {
				/* everything else */

				/* drop the item */
				drop_item(owner, pos, 1);

				switch (item) {
				case 0x91 : {
					/* Heiltrank */

					l_si = consumer->le_max - consumer->le;
					if (l_si > 10)
						l_si = 10;

					/* singular POINT/ PUNKT */
					strcpy(g_text_output_buf, get_ttx(392));

					if (l_si > 1) {
						/* make plural POINTS/PUNKTE */
						strcat(g_text_output_buf, get_ttx(393));
					}

					/* add LE */
					add_hero_le(consumer, l_si);

					/* give owner a glassbottle */
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(510), consumer->alias, l_si, g_text_output_buf);
					break;
				}
				case 0x92 : {
					/* Starker Heiltrank */

					/* 1W20+10 */
					l_si = dice_roll(1, 20, 10);
					if (consumer->le_max - consumer->le < l_si)
						l_si = consumer->le_max - consumer->le;

					/* add LE */
					add_hero_le(consumer, l_si);

					/* give owner a copperbottle */
					give_hero_new_item(owner, ITEM_FLASK_BRONZE, 2, 1);

					/* singular POINT/ PUNKT */
					strcpy(g_text_output_buf, get_ttx(392));

					if (l_si > 1) {
						/* make plural POINTS/PUNKTE */
						strcat(g_text_output_buf, get_ttx(393));
					}

					/* prepare output */
					sprintf(g_dtp2, get_ttx(510), consumer->alias, l_si, g_text_output_buf);
					break;
				}
				case 0xec: {
					/* Wunderkur */

					/* undo starvation damage */
					consumer->le_max += consumer->le_malus;
					consumer->le_malus = 0;

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
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(511), consumer->alias, GUI_get_ptr(consumer->sex, 0));

					break;
				}
				case 0xed: {
					/* Schlaftrunk */

					/* 3 Rounds of sleep */
					consumer->ruhe_koerper = 3;

					/* give owner a glasbottle */
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

					/* prepare output */
					sprintf(g_dtp2, get_ttx(738), consumer->alias);
					break;
				}
				case 0x9a: {
					/* Zaubertrank */

					if (consumer->typus >= HERO_TYPE_WITCH) {
						/* Magicuser */

						l_si = consumer->ae_max - consumer->ae;

						if (l_si > 10)
							l_si = 10;

						/* add AE */
						add_hero_ae(consumer, l_si);

						/* give hero a glassbottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

						/* prepare output */
						strcpy(g_text_output_buf, get_ttx(392));

						if (l_si > 1) {
							strcat(g_text_output_buf, get_ttx(393));
						}

						sprintf(g_dtp2, get_ttx(512), l_si, g_text_output_buf, consumer->alias);
					} else {
						/* Not a magicuser */

						strcpy(g_dtp2, get_ttx(804));

						/* give owner a glassbottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);
					}

					break;
				}
				case 0x9b: {
					/* Zaubertrank (stark) */

					if (consumer->typus >= HERO_TYPE_WITCH) {
						/* Magicuser */

						l_si = consumer->ae_max - consumer->ae;

						if (l_si > 30)
							l_si = 30;

						/* add AE */
						add_hero_ae(consumer, l_si);

						/* give owner a glassbottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

						/* prepare output */
						if (consumer->ae >= consumer->ae_max) {
							sprintf(g_dtp2, get_ttx(513), consumer->alias);
						} else {
							strcpy(g_text_output_buf, get_ttx(392));

							if (l_si > 1) {
								strcat(g_text_output_buf, get_ttx(393));
							}

							sprintf(g_dtp2, get_ttx(512), l_si, g_text_output_buf, consumer->alias);
						}
					} else {
						/* Not a magicuser */
						strcpy(g_dtp2, get_ttx(804));

						/* give owner a glassbottle */
						give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);
					}
					break;
				}
				case 0xb4: {
					/* Gegengift */

					poison = hero_is_poisoned(consumer);

					if (poison != 0 && poison < 5) {
						consumer->poison[poison][1] = 0;
						consumer->poison[poison][0] = 1;
					}

					/* give owner a glassbottle */
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

					sprintf(g_dtp2, get_ttx(467), consumer->alias);
					break;
				}
				case 0xdf: {
					/* Antikrankheitselexier */

					disease = hero_is_diseased(consumer);

					consumer->sick[disease][0] = 1;
					consumer->sick[disease][1] = 0;

					/* give owner a glassbottle */
					give_hero_new_item(owner, ITEM_FLASK_GLASS, 2, 1);

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

#if !defined(__BORLANDC__)
}
#endif
