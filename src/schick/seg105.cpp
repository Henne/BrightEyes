/*
 *      Rewrite of DSA1 v3.02_de functions of seg105 (inventory)
 *      Functions rewritten: 14/14 (complete)
 *      Functions called rewritten 13/13
 *      Functions uncalled rewritten 1/1
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg105.cpp
 *
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg007.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   TODO
 *
 * \param   hero        the hero
 * \param   item        the item which gets unequipped
 * \param   pos         the position of the item
 */
void unequip(Bit8u *hero, unsigned short item, unsigned short pos)
{

	Bit8u *item_p;

	/* unequip of item 0 is not allowed */
	if (item == ITEM_NONE)
		return;

	item_p = get_itemsdat(item);

	/* if item is an armor ? */
	if (item_armor(item_p)) {

		sub_ptr_bs(hero + HERO_RS_BONUS1, g_armors_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].rs);

		add_ptr_bs(hero + HERO_RS_BONUS1, host_readbs(hero + HERO_INVENTORY + INVENTORY_RS_LOST + pos * SIZEOF_INVENTORY));

		sub_ptr_bs(hero + HERO_RS_BE, g_armors_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].be);
	}
	/* if item is a weapon and in the right hand ? */
	if (item_weapon(item_p) && pos == HERO_INVENTORY_SLOT_RIGHT_HAND) {
		host_writebs(hero + HERO_WEAPON_TYPE, 0);
		host_writebs(hero + HERO_AT_MOD, host_writebs(hero + HERO_PA_MOD, 0));
	}
	/* unequip Kraftguertel KK - 5 */
	if (item == ITEM_GIRDLE_MIGHT)
		host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_KK), host_readb(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) - 5);
	/* unequip Helm CH + 1 (cursed) */
	if (item == ITEM_HELMET_CURSED)
		inc_ptr_bs(hero + (HERO_ATTRIB + 3 * ATTRIB_CH));
	/* unequip Silberschmuck TA + 2 */
	if (item == ITEM_SILVER_JEWELRY_MAGIC)
		host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), host_readb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA)) + 2);
	/* unequip Stirnreif or Ring MR - 2 */
	if (item == ITEM_CORONET_BLUE || item == ITEM_RING_RED)
		host_writeb(hero + HERO_MR, host_readb(hero + HERO_MR) - 2);
	/* unequip Totenkopfguertel TA + 4 */
	if (item == ITEM_SKULL_BELT)
		host_writeb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA), host_readb(hero + (HERO_ATTRIB + 3 * ATTRIB_TA)) + 4);
	/* unequip Kristallkugel Gefahrensinn - 2 */
	if (item == ITEM_CRYSTAL_BALL)
		host_writeb(hero + (HERO_TALENTS + TA_GEFAHRENSINN), host_readb(hero + (HERO_TALENTS + TA_GEFAHRENSINN)) - 2);
}


/**
 * \brief   account boni of special items when equipped
 *
 * \param   owner       the owner of the item
 * \param   equipper    the one who equips the item
 * \param   item        the item ID
 * \param   pos_i       the position in the inventory of the owner
 * \param   pos_b       the position in the inventory of the equipper
 */
void add_equip_boni(Bit8u *owner, Bit8u *equipper, signed short item, signed short pos_i, signed short pos_b)
{
	Bit8u *item_p;

	if (item) {
		/* calculate pointer to item description */
		item_p = g_itemsdat + item * SIZEOF_ITEM_STATS;

		/* armor and shield */
		if (item_armor(item_p)) {

			/* add RS boni */
			add_ptr_bs(equipper + HERO_RS_BONUS1, g_armors_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].rs);

			/* subtract degraded RS */
			sub_ptr_bs(equipper + HERO_RS_BONUS1, host_readbs(owner + HERO_INVENTORY + INVENTORY_RS_LOST + pos_i * SIZEOF_INVENTORY));

			/* add RS-BE */
			add_ptr_bs(equipper + HERO_RS_BE, g_armors_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].be);

		}

		/* weapon right hand */
		if (item_weapon(item_p) && (pos_b == HERO_INVENTORY_SLOT_RIGHT_HAND)) {

			/* set weapon type */
			host_writeb(equipper + HERO_WEAPON_TYPE, host_readb(item_p + ITEM_STATS_SUBTYPE));

			/* set AT */
			host_writeb(equipper + HERO_AT_MOD, g_weapons_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].at_mod);

			/* set PA */
			host_writeb(equipper + HERO_PA_MOD, g_weapons_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].pa_mod);

		}

		/* Girdle of might / Kraftguertel */
		if (item == ITEM_GIRDLE_MIGHT) {
			/* KK + 5 */
			host_writeb(equipper + (HERO_ATTRIB + 3 * ATTRIB_KK),
				host_readb(equipper + (HERO_ATTRIB + 3 * ATTRIB_KK)) + 5);
		}

		/* Helmet / Helm */
		if (item == ITEM_HELMET_CURSED) {
			/* dec CH */
			dec_ptr_bs(equipper + (HERO_ATTRIB + 3 * ATTRIB_CH));
		}

		/* Silver Jewelry / Silberschmuck (magisch) */
		if (item == ITEM_SILVER_JEWELRY_MAGIC) {
			/* TA - 2 */
			host_writeb(equipper + (HERO_ATTRIB + 3 * ATTRIB_TA),
				host_readbs(equipper + (HERO_ATTRIB + 3 * ATTRIB_TA)) - 2);
		}

		/* Coronet or Ring / Stirnreif oder Ring */
		if (item == ITEM_CORONET_BLUE || item == ITEM_RING_RED) {
			/* MR + 2 */
			host_writeb(equipper + HERO_MR,
				host_readb(equipper + HERO_MR) + 2);
		}

		/* Skull belt / Totenkopfguertel */
		if (item == ITEM_SKULL_BELT) {

			/* TA - 4 */
			host_writeb(equipper + (HERO_ATTRIB + 3 * ATTRIB_TA),
				host_readbs(equipper + (HERO_ATTRIB + 3 * ATTRIB_TA)) - 4);

			if (g_pp20_index == ARCHIVE_FILE_ZUSTA_UK) {
				equip_belt_ani();
			}
		}

		/* Crystal ball / Kristalkugel */
		if (item == ITEM_CRYSTAL_BALL) {

			/* Gefahrensinn + 2 */
			host_writeb(equipper + (HERO_TALENTS + TA_GEFAHRENSINN),
				host_readb(equipper + (HERO_TALENTS + TA_GEFAHRENSINN)) + 2);
		}
	}
}

/**
 * \brief   checks if a hero can use an item
 *
 * \param   hero        the hero
 * \param   item        the item
 * \return              1 if the hero can use the item and 0 if not.
 */
unsigned short can_hero_use_item(Bit8u *hero, unsigned short item)
{

#if !defined(__BORLANDC__)
	/* some new error check */
	if (!host_readbs(hero + HERO_TYPE))
		D1_ERR("Warning: %s() typus == 0\n", __func__);
#endif

	/* calculate the address of the class forbidden items array */
	if (is_in_word_array(item, g_wearable_items_index[host_readbs(hero + HERO_TYPE) - 1]))
		return 0;
	else
		return 1;
}

/**
 * \brief   checks if an item is equipable at a body position
 *
 * \param   item        the item
 * \param   pos         ths position at the body
 * \return              1 if equipping is possible or 0 if not.
 */
unsigned short can_item_at_pos(unsigned short item, unsigned short pos)
{

	Bit8u *item_p;

	item_p = get_itemsdat(item);

	/* if item is an armor ? */
	if (item_armor(item_p)) {

		/* can be weared on the head */
		if ((pos == HERO_INVENTORY_SLOT_HEAD && host_readb(item_p + ITEM_STATS_SUBTYPE) == ARMOR_TYPE_HEAD) ||
			/* can be weared on the torso */
			(pos == HERO_INVENTORY_SLOT_BODY && host_readb(item_p + ITEM_STATS_SUBTYPE) == ARMOR_TYPE_BODY) ||
			/* can be weared at the feet */
			(pos == HERO_INVENTORY_SLOT_FEET && host_readb(item_p + ITEM_STATS_SUBTYPE) == ARMOR_TYPE_FEET) ||
			/* can be weared at the arms */
			(pos == HERO_INVENTORY_SLOT_ARMS && host_readb(item_p + ITEM_STATS_SUBTYPE) == ARMOR_TYPE_ARMS) ||
			/* can be weared at the legs */
			(pos == HERO_INVENTORY_SLOT_LEGS && host_readb(item_p + ITEM_STATS_SUBTYPE) == ARMOR_TYPE_LEGS) ||
			/* can be weared at the left hand */
			(pos == HERO_INVENTORY_SLOT_LEFT_HAND && host_readb(item_p + ITEM_STATS_SUBTYPE) == ARMOR_TYPE_LEFT_HAND)) {
			return 1;
		} else {
			return 0;
		}
	} else {

		/* coronet (Stirnreif) (3 types) can be weared at the head */
		if ((item == ITEM_CORONET_BLUE || item == ITEM_CORONET_SILVER || item == ITEM_CORONET_GREEN)
			&& (pos == HERO_INVENTORY_SLOT_HEAD))
		{
			return 1;
		}

		/* you can take everything else in the hands, but nowhere else */

		if ((pos != HERO_INVENTORY_SLOT_RIGHT_HAND) && (pos != HERO_INVENTORY_SLOT_LEFT_HAND)) {
			return 0;
		}

	}

	return 1;
}

/**
 * \brief   returns the position of an equipped item
 *
 * \param   hero        the hero
 * \param   item        the item
 * \return              the position of item, if equipped, otherwise -1.
 * Is not used in the game.
 */
signed short has_hero_equipped(Bit8u *hero, unsigned short item)
{
	signed short i;

	for (i = 0; i < HERO_INVENTORY_SLOT_KNAPSACK_1; i++)
		if (host_readw(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + i * SIZEOF_INVENTORY) == item)
			return i;

	return -1;
}

/**
 * \brief   returns the position of a non-full item stack
 *
 * \param   hero        the hero
 * \param   item        the item
 * \return              the postition of a non-full (<99) item stack or -1 if
 * the hero doesn't own this item or has only full stacks of them.
 */
//static
signed short has_hero_stacked(Bit8u *hero, unsigned short item)
{
	signed short i;

	for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++) {
		/* has the hero the item */
		/* is the number of items < 99 */
		if ((host_readw(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + i * SIZEOF_INVENTORY) == item) &&
			(host_readws(hero + HERO_INVENTORY + INVENTORY_QUANTITY + i * SIZEOF_INVENTORY) < 99))
			return i;
	}

	return -1;
}

/**
 * \brief   generates an item and gives it to hero
 *
 *          The mode parameter works that way: If the weight the hero carries
 *          is greater than KK*10 mode 0 and 1 will not give the hero that item.
 *          The difference is, that mode = 1 prints a warning, mode = 0 is quiet.
 *          mode = 2 ignores the carry weight completely.
 *
 * \param   hero        the hero who should get the item
 * \param   item        id of the item
 * \param   mode        0 = quiet / 1 = warn / 2 = ignore
 * \param   quantity	amount of items the hero should get
 */
signed short give_hero_new_item(Bit8u *hero, signed short item, signed short mode, signed short quantity)
{
	signed short l1;
	signed short retval;
	signed short done;
	Bit8u *item_p;
	signed short si, di;

	si = quantity;

	retval = 0;

	/* check if hero can carry that item */
	if ((mode != 2) && (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 100 <= host_readws(hero + HERO_LOAD))) {

		if (mode != 0) {
			sprintf(g_dtp2,	get_ttx(779), (char*)(hero + HERO_NAME2));
			GUI_output(g_dtp2);
		}

	} else {
		item_p = get_itemsdat(item);

		/* hero has a non-full stack of this item */
		if (item_stackable(item_p) &&
			(l1 = has_hero_stacked(hero, item)) != -1) {


			/* check for space on existing stack */
			if (host_readws(hero + HERO_INVENTORY + INVENTORY_QUANTITY + l1 * SIZEOF_INVENTORY) + si > 99) {
				si = 99 - host_readw(hero + HERO_INVENTORY + INVENTORY_QUANTITY + l1 * SIZEOF_INVENTORY);
			}

			/* add items to stack */
			add_ptr_ws(hero + HERO_INVENTORY + INVENTORY_QUANTITY + l1 * SIZEOF_INVENTORY, si);

			/* add weight */
			add_ptr_ws(hero + HERO_LOAD, (host_readws(item_p + ITEM_STATS_WEIGHT) * si));

			retval = si;
		} else {

			/* Original-Bug: may lead to problems when the item counter is broken */
			if (host_readbs(hero + HERO_NR_INVENTORY_SLOTS_FILLED) < NR_HERO_INVENTORY_SLOTS) {

				done = 0;

				do {
					/* Original-Bug: may lead to problems when the item counter is broken */
					if (host_readbs(hero + HERO_NR_INVENTORY_SLOTS_FILLED) < NR_HERO_INVENTORY_SLOTS) {

						/* look for a free place : tricky */
						di = HERO_INVENTORY_SLOT_KNAPSACK_1 - 1;
						while ((host_readw(hero + (HERO_INVENTORY + INVENTORY_ITEM_ID) + ++di * SIZEOF_INVENTORY) != ITEM_NONE) && (di < NR_HERO_INVENTORY_SLOTS));

						if (di < NR_HERO_INVENTORY_SLOTS) {
							if (si > 99)
								si = 99;
							/* increment item counter */
							inc_ptr_bs(hero + HERO_NR_INVENTORY_SLOTS_FILLED);

							/* write item id */
							host_writew(hero + (HERO_INVENTORY + INVENTORY_ITEM_ID) + di * SIZEOF_INVENTORY, item);


#if 1
							host_writew(hero + HERO_INVENTORY + INVENTORY_QUANTITY + di * SIZEOF_INVENTORY,
								(item_stackable(item_p)) ? si :
								(item_useable(item_p)) ?
										g_specialitems_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].quantity : 0);
#else

							/* write item counter */
							if (item_stackable(item_p))
								/* write stackable items */
								host_writew(hero + HERO_INVENTORY + INVENTORY_QUANTITY + di * SIZEOF_INVENTORY, si);
							else if (item_useable(item_p))
									/* unknown */
									host_writew(hero + HERO_INVENTORY + INVENTORY_QUANTITY + di * SIZEOF_INVENTORY,
										g_specialitems_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].quantity);
								 else
									host_writew(hero + HERO_INVENTORY + INVENTORY_QUANTITY + di * SIZEOF_INVENTORY, 0);
#endif

							/* set magical flag */
							if (host_readb(item_p + ITEM_STATS_MAGIC)) {

								or_ptr_bs(hero + HERO_INVENTORY + INVENTORY_FLAGS + di * SIZEOF_INVENTORY, 0x8); /* set 'magic' flag */

#if !defined(__BORLANDC__)
								D1_INFO("%s hat soeben einen magischen Gegenstand erhalten: %s\n",
									(char*)hero + HERO_NAME2, get_itemname(item));
#endif
							}

							/* set breakfactor */
							if (item_weapon(item_p)) {
								host_writeb(hero + HERO_INVENTORY + INVENTORY_BF + di * SIZEOF_INVENTORY,
									g_weapons_table[host_readbs(item_p + ITEM_STATS_TABLE_INDEX)].bf);
							}

							/* adjust weight */
							if (item_stackable(item_p)) {
								/* add stackable items weight */
								add_ptr_ws(hero + HERO_LOAD, host_readws(item_p + ITEM_STATS_WEIGHT) * si);
								retval = si;
								si = 0;
							} else {
								/* add single item weight */
								add_ptr_ws(hero + HERO_LOAD, host_readws(item_p + ITEM_STATS_WEIGHT));
								si--;
								retval++;
							}

							/* all items are distributed */
							if (si == 0)
								done = 1;

							/* special items */
							if (item == ITEM_SICKLE_MAGIC) {
								host_writeb(hero + (HERO_TALENTS + TA_PFLANZENKUNDE), host_readb(hero + (HERO_TALENTS + TA_PFLANZENKUNDE)) + 3);
							}
							if (item == ITEM_AMULET_BLUE) {
								host_writeb(hero + HERO_MR, host_readb(hero + HERO_MR) + 5);
							}

						} else {
							done = 1;
						}
					} else {
						done = 1;
					}
				} while (done == 0);
			}
		}
	}

	return retval;
}

/**
 * \brief   checks if Ingerimm accepts this item as sacrifice
 *
 * \param   item        the item
 */
//static
unsigned short item_pleasing_ingerimm(unsigned short item)
{

	Bit8u *p_item;

	p_item = get_itemsdat(item);

	if (item_weapon(p_item) && (host_readb(p_item + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_AXT))
		/* Ingerimm is pleased by either an axe ... */
		return 1;

	if (item_armor(p_item) && (g_armors_table[host_readbs(p_item + ITEM_STATS_TABLE_INDEX)].rs > 1))
		/* or an armor with RS > 1 */
		return 1;

	return 0;
}

/**
 * \brief   tries to drop an item
 *
 * \param   hero        pointer to the hero
 * \param   pos         position of the item to be dropped
 * \param   no          number of stacked items to be dropped / -1 to ask
 * \return              true if the item has been dropped or false if not
 *
 *	TODO: This function can be tuned a bit
 */
unsigned short drop_item(Bit8u *hero, signed short pos, signed short no)
{

	Bit8u *p_item;
	signed short answer;
	unsigned short retval = 0;
	signed short item;

	item = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + pos * SIZEOF_INVENTORY);

	/* check if that item is valid */
	if (item != 0) {

		p_item = get_itemsdat(item);

		if (item_undropable(p_item)) {
			/* this item is not droppable */

			sprintf(g_dtp2,
				get_ttx(454),
				(char*)(GUI_names_grammar((signed short)0x8002, item, 0)));

			GUI_output(g_dtp2);
		} else {
			/* this item is droppable */

			if (item_stackable(p_item)) {
				if (no == -1) {
					sprintf(g_dtp2,
						get_ttx(219),
						(char*)(GUI_names_grammar(6, item, 0)));

					do {
						answer = GUI_input(g_dtp2, 2);
					} while (answer < 0);

					no = answer;
				}

				if (host_readws(hero + HERO_INVENTORY + INVENTORY_QUANTITY + pos * SIZEOF_INVENTORY) > no) {
					/* remove some stacked items */

					/* adjust stack counter */
					sub_ptr_ws(hero + HERO_INVENTORY + INVENTORY_QUANTITY + pos * SIZEOF_INVENTORY, no);
					/* adjust weight */
					sub_ptr_ws(hero + HERO_LOAD, host_readws(p_item + ITEM_STATS_WEIGHT) * no);
				} else {
					/* remove all stacked items */

					/* adjust weight */
					sub_ptr_ws(hero + HERO_LOAD,
						host_readws(p_item + ITEM_STATS_WEIGHT) * host_readws(hero + HERO_INVENTORY + INVENTORY_QUANTITY + pos * SIZEOF_INVENTORY));
					/* decrement item counter */
					dec_ptr_bs(hero + HERO_NR_INVENTORY_SLOTS_FILLED);

					/* clear the inventory pos */
					memset(hero + HERO_INVENTORY + pos * SIZEOF_INVENTORY, 0, SIZEOF_INVENTORY);
				}

				retval = 1;
			} else {
				if (!(no != -1 || GUI_bool(get_ttx(220)))) {
				} else {

					/* check if item is equipped */
					if (pos < 7)
						unequip(hero, item, pos);

					/* decrement item counter */
					dec_ptr_bs(hero + HERO_NR_INVENTORY_SLOTS_FILLED);

					/* subtract item weight */
					sub_ptr_ws(hero + HERO_LOAD, host_readws(p_item + ITEM_STATS_WEIGHT));

					/* check special items */
					/* item: SICHEL Pflanzenkunde -3 */
					if (item == ITEM_SICKLE_MAGIC) {
						host_writeb(hero + (HERO_TALENTS + TA_PFLANZENKUNDE),
							host_readbs(hero + (HERO_TALENTS + TA_PFLANZENKUNDE)) - 3);
					}

					/* item:  AMULETT MR -5 */
					if (item == ITEM_AMULET_BLUE) {
						host_writeb(hero + HERO_MR,
							host_readbs(hero + HERO_MR) - 5);
					}

					/* clear the inventory pos */
					memset(hero + HERO_INVENTORY + pos * SIZEOF_INVENTORY, 0, SIZEOF_INVENTORY);
					retval = 1;
				}
			}
		}

		/* check for the pirate cave on Manrek to bring Efferd a gift */
		if ((item == ITEM_TRIDENT || item == ITEM_NET) && gs_dungeon_index == DUNGEONS_PIRATENHOEHLE &&
			gs_x_target == 9 && gs_y_target == 9)
		{
			gs_dng11_efferd_sacrifice = 1;
		}

		/* check for the mine in Oberorken to bring Ingerimm a gift */
		if (item_pleasing_ingerimm(item) && gs_dungeon_index == DUNGEONS_ZWERGENFESTE &&
			gs_x_target == 2 && gs_y_target == 14 &&
			gs_dungeon_level == 1)
		{
			gs_dng12_ingerimm_sacrifice = 1;
		}
	}

	return retval;
}

/**
 * \brief   gives one item to the party
 *
 * \param   id          ID of the item
 * \param   unused      unused parameter
 * \param   no          number of items
 * \return              the number of given items.
 */
signed short get_item(signed short id, signed short unused, signed short no)
{
	signed short i;
	signed short retval = 0;
	signed short v6;
	signed short done = 0;
	signed short dropper;
	signed short vc;
	struct struct_hero *hero_i;
	signed short autofight_bak;

	/* Special stacked items */
	if (id == ITEM_200_ARROWS) { id = ITEM_ARROWS; no = 200;} else
	if (id == ITEM_50_BOLTS) { id = ITEM_BOLTS; no = 50;} else
	if (id == ITEM_20_CLIMBING_HOOKS) { id = ITEM_CLIMBING_HOOKS; no = 20;}

	do {
		hero_i = (struct struct_hero*)get_hero(0);
		for (i = 0; i <= 6; i++, hero_i++) {
			if ((hero_i->typus) && (hero_i->group_no == gs_current_group))
			{

				while ((no > 0) && (v6 = give_hero_new_item((Bit8u*)hero_i, id, 0, no)) > 0) {
					no -= v6;
					retval += v6;
				}
			}
		}

		if (no > 0) {
			autofight_bak = g_autofight;
			g_autofight = 0;

			sprintf(g_dtp2,	get_ttx(549), GUI_names_grammar(((no > 1) ? 4 : 0) + 2, id, 0));

			if (GUI_bool(g_dtp2)) {

				dropper = select_hero_ok(get_ttx(550));

				if (dropper != -1) {
					hero_i = (struct struct_hero*)get_hero(dropper);
					g_prevent_drop_equipped_items = 1;
					vc = select_item_to_drop((Bit8u*)hero_i);
					g_prevent_drop_equipped_items = 0;

					if (vc != -1) {
						drop_item((Bit8u*)hero_i, vc, -1);
					}
				}
			} else {
				done = 1;
			}
			g_autofight = autofight_bak;
		} else {
			done = 1;
		}
	} while (done == 0);

	return retval;
}

/**
 * \brief   returns how many items of one type the hero has
 *
 * \param   hero        the hero
 * \param   item        the item
 */
signed short hero_count_item(Bit8u *hero, unsigned short item) {

	signed short i;
	unsigned short ret = 0;

	for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++)
		if (host_readw(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + i * SIZEOF_INVENTORY) == item)
			ret++;

	return ret;
}

/**
 * \brief   count the number of items of one type the current group has
 *
 * \param   item        the item-ID
 * \return              the number of items
 */
signed short group_count_item(signed short item)
{

	struct struct_hero *hero_i;
	signed short i;
	signed short ret = 0;

	hero_i = (struct struct_hero*)get_hero(0);
	for (i = 0; i <= 6; i++, hero_i++) {
		/* check class */
		if (hero_i->typus && (hero_i->group_no == gs_current_group)) {

			ret += hero_count_item((Bit8u*)hero_i, item);
		}
	}

	return ret;
}

/**
 * \brief   a hero will loose an item
 *
 * \param   hero        the hero
 * \param   percent     probability to loose
 * \param   text        the displayed text
 */
void loose_random_item(struct struct_hero *hero, const signed int percent, char *text)
{
	Bit8u *p_item;
	signed int item_id;
	signed int pos;

	if (random_schick(100) > percent)
		return;

	/* Original-Bug: infinite loop if the hero has no item */
	do {
		pos = random_schick(NR_HERO_INVENTORY_SLOTS) - 1;

		item_id = host_readw((Bit8u*)hero + HERO_INVENTORY + INVENTORY_ITEM_ID + pos * SIZEOF_INVENTORY);

		p_item = get_itemsdat(item_id);

		/* No item to drop */
		if (item_id != 0 && !item_undropable(p_item)) {

			/* drop 1 item */
			drop_item((Bit8u*)hero, pos, 1);

			sprintf(g_text_output_buf, text, hero->alias, (Bit8u*)GUI_names_grammar(0, item_id, 0));
			GUI_output(g_text_output_buf);

			return;
		}
	} while (1);
}

signed short select_item_to_drop(Bit8u *hero)
{
	signed short i;
	signed short v4 = 0;
	signed short v6 = 0;
	signed short item;
	signed short va;
	signed short tw_bak, bak2, bak3;
	char *ptr;
	signed short str[23];
	signed short di;

	/* check if we drop equipped items or not */
	i = g_prevent_drop_equipped_items ? HERO_INVENTORY_SLOT_KNAPSACK_1 : 0;
	for (; i < NR_HERO_INVENTORY_SLOTS; i++) {
		if ((item = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + i * SIZEOF_INVENTORY))) {
			str[v6] = i;
			g_radio_name_list[v6] = (g_dtp2 + v6 * 30);
			strcpy(g_radio_name_list[v6], GUI_name_singular(get_itemname(item)));
			v6++;
		}
	}

	if (v6 == 0) {
		sprintf(g_dtp2, get_ttx(750), (char*)(hero + HERO_NAME2));
		GUI_output(g_dtp2);
		return -1;
	}
	di = 0;
	while (v4 != -1) {
		va = -1;
		if (v6 > 12) {
			if (!di) {
				i = 13;
				va = i - 1;
				ptr = g_radio_name_list[va];
				g_radio_name_list[va] = get_ttx(751);
			} else {
				i = v6 + 1;
				va = i - 1;
				ptr = g_radio_name_list[va];
				g_radio_name_list[va] = get_ttx(751);
				i -= di;
			}
		} else {
			i = v6;
		}
		tw_bak = g_textbox_width;
		bak2 = g_basepos_x;
		bak3 = g_basepos_y;
		g_textbox_width = 6;
		g_basepos_x = g_basepos_y = 0;

		v4 = GUI_radio(get_ttx(752), (signed char)i,
				g_radio_name_list[di + 0], g_radio_name_list[di + 1],
				g_radio_name_list[di + 2], g_radio_name_list[di + 3],
				g_radio_name_list[di + 4], g_radio_name_list[di + 5],
				g_radio_name_list[di + 6], g_radio_name_list[di + 7],
				g_radio_name_list[di + 8], g_radio_name_list[di + 9],
				g_radio_name_list[di + 10], g_radio_name_list[di + 11],
				g_radio_name_list[di + 12]);

		g_textbox_width = tw_bak;
		g_basepos_x = bak2;
		g_basepos_y = bak3;

		if (va != -1) {
			g_radio_name_list[va] = ptr;
		}
		if ((v6 > 12) && (v4 == i)) {
			di += 12;
			if (di > v6) {
				di = 0;
			}
		} else {
			if (v4 != -1) {
				return str[di + v4 - 1];
			}
		}
	}

	return -1;
}

#if !defined(__BORLANDC__)
}
#endif
