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

static signed char g_prevent_drop_equipped_items = 0; // ds:0xae46
unsigned char g_unkn_067[1] = { 0x00 }; // ds:0xae47

/**
 * \brief   TODO
 *
 * \param   hero		the hero
 * \param   item_id[in]		the item which gets unequipped
 * \param   inv_slot[in]	the inventory slot of the item
 */
void unequip(struct struct_hero *hero, const signed int item_id, const signed int inv_slot)
{
	/* unequip of item 0 is not allowed */
	if (item_id != ITEM_NONE) {

		struct item_stats *item_p = &g_itemsdat[item_id];
	
		/* if item is an armor ? */
		if (item_p->flags.armor) {

			hero->rs_bonus -= g_armors_table[item_p->table_index].rs;

			hero->rs_bonus += hero->inventory[inv_slot].rs_lost;

			hero->rs_be -= g_armors_table[item_p->table_index].be;
		}

		/* if item is a weapon and in the right hand ? */
		if (item_p->flags.weapon && (inv_slot == HERO_INVENTORY_SLOT_RIGHT_HAND)) {

			hero->weapon_type = WEAPON_TYPE_WAFFENLOS;

			hero->weapon_at_mod = hero->weapon_pa_mod = 0;
		}

		/* unequip Kraftguertel KK - 5 */
		if (item_id == ITEM_KRAFTGUERTEL) {

			hero->attrib[ATTRIB_KK].current = hero->attrib[ATTRIB_KK].current - 5;
			/* TODO: Original-Bug: update dependent values like atpa_base */
		}

		/* unequip Helm CH + 1 (cursed) */
		if (item_id == ITEM_HELM__CURSED) {

			hero->attrib[ATTRIB_CH].current++;
		}

		/* unequip Silberschmuck TA + 2 */
		if (item_id == ITEM_SILBERSCHMUCK__MAGIC) {

			hero->attrib[ATTRIB_TA].current = hero->attrib[ATTRIB_TA].current + 2;
		}

		/* unequip Stirnreif or Ring MR - 2 */
		if (item_id == ITEM_STIRNREIF__BLUE || item_id == ITEM_RING__RED) {

			hero->mr = hero->mr - 2;
		}

		/* unequip Totenkopfguertel TA + 4 */
		if (item_id == ITEM_TOTENKOPFGUERTEL) {

			hero->attrib[ATTRIB_TA].current = hero->attrib[ATTRIB_TA].current + 4;
		}

		/* unequip Kristallkugel Gefahrensinn - 2 */
		if (item_id == ITEM_KRISTALLKUGEL) {

			hero->talents[TA_GEFAHRENSINN] = hero->talents[TA_GEFAHRENSINN] - 2;
		}
	}
}


/**
 * \brief   account boni of special items when equipped
 *
 * \param   owner			the owner of the item
 * \param   equipper			the one who equips the item
 * \param   item_id[in]			the item ID
 * \param   inv_slot_owner[in]		owner holds item in this inventory slot
 * \param   inv_slot_equipper[in]	equipper equips item in this inventory slot
 */
void add_equip_boni(struct struct_hero *owner, struct struct_hero *equipper, const signed int item_id, const signed int inv_slot_owner, const signed int inv_slot_equipper)
{
	if (item_id != ITEM_NONE) {

		/* calculate pointer to item description */
		struct item_stats *item_p = &g_itemsdat[item_id];

		/* armor and shield */
		if (item_p->flags.armor) {

			/* add RS boni */
			equipper->rs_bonus += g_armors_table[item_p->table_index].rs;

			/* subtract degraded RS */
			equipper->rs_bonus -= owner->inventory[inv_slot_owner].rs_lost;

			/* add RS-BE */
			equipper->rs_be += g_armors_table[item_p->table_index].be;

		}

		/* weapon right hand */
		if (item_p->flags.weapon && (inv_slot_equipper == HERO_INVENTORY_SLOT_RIGHT_HAND)) {

			/* set weapon type */
			equipper->weapon_type = item_p->subtype;

			/* set AT */
			equipper->weapon_at_mod = g_weapons_table[item_p->table_index].at_mod;

			/* set PA */
			equipper->weapon_pa_mod = g_weapons_table[item_p->table_index].pa_mod;
		}

		/* Girdle of might / Kraftguertel */
		if (item_id == ITEM_KRAFTGUERTEL) {

			equipper->attrib[ATTRIB_KK].current = equipper->attrib[ATTRIB_KK].current + 5;
			/* TODO: Original-Bug: update dependent values like atpa_base */
		}

		/* Helmet / Helm */
		if (item_id == ITEM_HELM__CURSED) {

			equipper->attrib[ATTRIB_CH].current--;
		}

		/* Silver Jewelry / Silberschmuck (magisch) */
		if (item_id == ITEM_SILBERSCHMUCK__MAGIC) {

			equipper->attrib[ATTRIB_TA].current = equipper->attrib[ATTRIB_TA].current - 2;
		}

		/* Coronet or Ring / Stirnreif oder Ring */
		if (item_id == ITEM_STIRNREIF__BLUE || item_id == ITEM_RING__RED) {

			equipper->mr = equipper->mr + 2;
		}

		/* Skull belt / Totenkopfguertel */
		if (item_id == ITEM_TOTENKOPFGUERTEL) {

			/* TA - 4 */
			equipper->attrib[ATTRIB_TA].current = equipper->attrib[ATTRIB_TA].current - 4;

			if (g_pp20_index == ARCHIVE_FILE_ZUSTA_UK) {
				equip_belt_ani();
			}
		}

		/* Crystal ball / Kristalkugel */
		if (item_id == ITEM_KRISTALLKUGEL) {

			equipper->talents[TA_GEFAHRENSINN] = equipper->talents[TA_GEFAHRENSINN] + 2;
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
signed int can_hero_use_item(const struct struct_hero *hero, const signed int item_id)
{

#if !defined(__BORLANDC__)
	/* some new error check */
	if (!hero->typus) {
		D1_ERR("Warning: %s() typus == 0\n", __func__);
	}
#endif

	/* calculate the address of the class forbidden items array */
	if (is_in_int_array(item_id, g_forbidden_item_ids_table[hero->typus - 1])) {

		return 0;
	} else {
		return 1;
	}
}

/**
 * \brief   checks if an item is equipable at a body slot
 *
 * \param   item_id[in]	the item ID
 * \param   inv_slot[in] inventory slot to check for equippability
 * \return  0: not possible / 1: possible
 */
signed int can_hero_equip_item_at_slot(const signed int item_id, const signed int inv_slot)
{
	// assert(inv_slot) < HERO_INVENTORY_SLOT_KNAPSACK_1 // only for inv_slot at the body
	struct item_stats *item_p = &g_itemsdat[item_id];

	/* if item is an armor ? */
	if (item_p->flags.armor) {

		/* can be worn on the head */
		if ((inv_slot == HERO_INVENTORY_SLOT_HEAD && item_p->subtype == ARMOR_TYPE_HEAD) ||
			/* can be worn on the torso */
			(inv_slot == HERO_INVENTORY_SLOT_BODY && item_p->subtype == ARMOR_TYPE_BODY) ||
			/* can be worn at the feet */
			(inv_slot == HERO_INVENTORY_SLOT_FEET && item_p->subtype == ARMOR_TYPE_FEET) ||
			/* can be worn at the arms */
			(inv_slot == HERO_INVENTORY_SLOT_ARMS && item_p->subtype == ARMOR_TYPE_ARMS) ||
			/* can be worn at the legs */
			(inv_slot == HERO_INVENTORY_SLOT_LEGS && item_p->subtype == ARMOR_TYPE_LEGS) ||
			/* can be worn at the left hand */
			(inv_slot == HERO_INVENTORY_SLOT_LEFT_HAND && item_p->subtype == ARMOR_TYPE_LEFT_HAND)) {
			return 1;
		} else {
			return 0;
		}
	} else {

		/* coronet (Stirnreif) (3 types) can be worn at the head */
		if ((item_id == ITEM_STIRNREIF__BLUE || item_id == ITEM_SILBERNER_STIRNREIF || item_id == ITEM_STIRNREIF__GREEN)
			&& (inv_slot == HERO_INVENTORY_SLOT_HEAD))
		{
			return 1;
		}

		/* you can take everything else in the hands, but nowhere else */

		if ((inv_slot != HERO_INVENTORY_SLOT_RIGHT_HAND) && (inv_slot != HERO_INVENTORY_SLOT_LEFT_HAND)) {
			return 0;
		}
	}

	return 1;
}

/**
 * \brief   returns the position of an equipped item
 *
 * \param   hero	the hero
 * \param   item_id[in]	the item ID
 * \return  the position of item, if equipped, otherwise -1.
 * Is not used in the game.
 */
signed int where_has_hero_equipped_item(struct struct_hero *hero, const signed int item_id)
{
	signed int i;

	for (i = 0; i < HERO_INVENTORY_SLOT_KNAPSACK_1; i++) {

		if (hero->inventory[i].item_id == item_id) {

			return i;
		}
	}

	return -1;
}

/**
 * \brief   returns the inventory slot holding non-full stack of a given item
 *
 * \param   hero        the hero
 * \param   item        the item
 * \return              -1 if hero doesn't own this item or has only full stacks of it
 * 			otherwise: inventory slot holding the first incomplete stack of this item
 */
//static
signed int where_has_hero_incomplete_stack_of_item(struct struct_hero *hero, const signed int item_id)
{
	// assert(g_itemsdat[item_id]->flags.stackable)
	signed int inv_slot;

	for (inv_slot = 0; inv_slot < NR_HERO_INVENTORY_SLOTS; inv_slot++) {

		/* has the hero the item */
		/* is the number of items < 99 */
		/* REMARK: should check if item is stackable */
		if ((hero->inventory[inv_slot].item_id == item_id) && (hero->inventory[inv_slot].quantity < STACK_SIZE_MAX)) {

			return inv_slot;
		}
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
 * \param   hero		the hero who should get the item
 * \param   item_id[in]		the item ID
 * \param   mode[in]		0 = quiet / 1 = warn / 2 = ignore
 * \param   quantity[in]	amount of items the hero should get
 *
 * \return	quantity of items the hero has taken
 */
signed int give_new_item_to_hero(struct struct_hero *hero, const signed int item_id, const signed int mode, const signed int quantity)
{
	signed int inv_slot;
	signed int retval;
	signed int done;
	struct item_stats *item_p;
	signed int quantity_left, inv_slot_2;

	quantity_left = quantity;

	retval = 0;

	/* check if hero can carry that item.
	 *
	 * The following check doesn't take into account the weight of the new item, which is a bit inconsistent.
	 * Might be a future FEATURE_MOD to fix this. Then, one has to consider also taking a partial stack, if the full stack is too heavy. */
	if ((mode != 2) && (hero->attrib[ATTRIB_KK].current * 100 <= hero->load)) {

		if (mode != 0) {

			sprintf(g_dtp2,	get_ttx(779), hero->alias);
			GUI_output(g_dtp2);
		}

	} else {
		item_p = &g_itemsdat[item_id];

		/* hero has a non-full stack of this item */
		if (item_p->flags.stackable && ((inv_slot = where_has_hero_incomplete_stack_of_item(hero, item_id)) != -1)) {


			/* check for remaining capacity of existing stack */
			if (hero->inventory[inv_slot].quantity + quantity_left > STACK_SIZE_MAX) {

				quantity_left = STACK_SIZE_MAX - hero->inventory[inv_slot].quantity;
				/* Throw away all items which do not fit into the non-full stack. */
				/* Potential FEATURE_MOD: Instead, consider opening a new stack instead if there are items left. */
			}

			/* add items to stack */
			hero->inventory[inv_slot].quantity += quantity_left;

			/* add weight */
			hero->load += item_p->weight * quantity_left;

			retval = quantity_left;
		} else {

			/* Original-Bug: may lead to problems when the slot counter is broken */

			/* The following check is pretty weak.
			 * If the condition is true, we know that there is a free inventory slot.
			 * But we need a free inventory slot **in the knapsack**, and that is not guaranteed. */
			if (hero->num_filled_inv_slots < NR_HERO_INVENTORY_SLOTS) {

				done = 0;

				do {
					/* Original-Bug: may lead to problems when the slot counter is broken */
					if (hero->num_filled_inv_slots < NR_HERO_INVENTORY_SLOTS) {

						/* look for a free place : tricky */
						inv_slot_2 = HERO_INVENTORY_SLOT_KNAPSACK_1 - 1;
						while ((hero->inventory[++inv_slot_2].item_id != ITEM_NONE) && (inv_slot_2 < NR_HERO_INVENTORY_SLOTS));

						if (inv_slot_2 < NR_HERO_INVENTORY_SLOTS) {
							/* inv_slot_2 is free */

							/* the following is also done for non-stackable items.
							 * The condition might be always false, so probably not a problem.
							 * (Or is there a situation where more than 99 new non-stackable identical items are found?)
							 */
							if (quantity_left > STACK_SIZE_MAX) {
								quantity_left = STACK_SIZE_MAX;
								/* Throw away all items which do not fit into a new stack. */
								/* Potential FEATURE_MOD: Instead, consider creating more than one stack if needed. */
							}

							/* increment slot counter */
							hero->num_filled_inv_slots++;

							/* write item id */
							hero->inventory[inv_slot_2].item_id = item_id;

							hero->inventory[inv_slot_2].quantity =
								(item_p->flags.stackable ? quantity_left : (item_p->flags.usable ?
								g_usable_items_table[item_p->table_index].quantity : 0));

							/* set magical flag */
							if (item_p->magic) {

								hero->inventory[inv_slot_2].flags.magic = 1;

#if !defined(__BORLANDC__)
								D1_INFO("%s hat soeben einen magischen Gegenstand erhalten: %s\n",
									hero->alias, g_itemsname[item_id]);
#endif
							}

							/* set breakfactor */
							if (item_p->flags.weapon) {
								hero->inventory[inv_slot_2].bf = g_weapons_table[item_p->table_index].bf;
							}

							/* adjust weight */
							if (item_p->flags.stackable) {

								/* add stackable items weight */
								hero->load += item_p->weight * quantity_left;
								retval = quantity_left;
								quantity_left = 0;
							} else {
								/* add single item weight */
								hero->load += item_p->weight;
								quantity_left--;
								retval++;
							}

							/* all items are distributed */
							if (quantity_left == 0)
								done = 1;

							/* Apply effects for items which have an effect as soon as they are in the inventory. */
							if (item_id == ITEM_SICHEL__MAGIC) {
								hero->talents[TA_PFLANZENKUNDE] = hero->talents[TA_PFLANZENKUNDE] + 3;
							}

							if (item_id == ITEM_AMULETT__BLUE) {

								hero->mr = hero->mr + 5;
							}

						} else {
							// all inventory slots are full (info from loop reaching the last knapsack slot).
							done = 1;
						}
					} else {
						// all inventory slots are full (info from slot counter)
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
 * \param   item_id[in]	the item ID
 */
//static
signed int is_item_pleasing_ingerimm(const signed int item_id)
{
	struct item_stats *item_p = &g_itemsdat[item_id];

	if (item_p->flags.weapon && (item_p->subtype == WEAPON_TYPE_AXT))
		/* Ingerimm is pleased by either an axe ... */
		return 1;

	if (item_p->flags.armor && (g_armors_table[item_p->table_index].rs > 1))
		/* or an armor with RS > 1 */
		return 1;

	return 0;
}

/**
 * \brief   tries to drop an item
 *
 * \param   hero        pointer to the hero
 * \param   inv_slot    position of the item to be dropped
 * \param   quantity    if item is stackable: number of stacked items to be dropped / -1 to ask
 * 			if item is not stackable: -1: don't drop anything / otherwise (including quantity==0): drop a single item
 * \return              true if the item has been dropped or false if not
 *
 *	TODO: This function can be tuned a bit
 */
signed int drop_item(struct struct_hero *hero, const signed int inv_slot, signed int quantity)
{
	struct item_stats *p_item;
	signed int answer;
	signed int retval = 0;
	const signed int item_id = hero->inventory[inv_slot].item_id;

	/* check if that item_id is valid */
	if (item_id != ITEM_NONE) {

		p_item = &g_itemsdat[item_id];

		if (p_item->flags.undropable) {

			/* this item is not droppable */
			sprintf(g_dtp2,get_ttx(454), (char*)GUI_name_inflect_with_article(
				INFLECT_DEFINITE_ARTICLE | INFLECT_SINGULAR | INFLECT_4TH_CASE,
				item_id,
				INFLECT_NAME_TYPE_ITEM
			));
			GUI_output(g_dtp2);

		} else {

			/* this item is droppable */
			if (p_item->flags.stackable) {

				if (quantity == -1) {
					sprintf(g_dtp2,	get_ttx(219), (char*)GUI_name_inflect_with_article(
						INFLECT_INDEFINITE_ARTICLE | INFLECT_PLURAL | INFLECT_4TH_CASE,
						item_id,
						INFLECT_NAME_TYPE_ITEM
					));

					do {
						answer = GUI_input(g_dtp2, 2);

					} while (answer < 0);

					quantity = answer;
				}

				if (hero->inventory[inv_slot].quantity > quantity) {

					/* remove part of the stack */

					/* adjust stack counter */
					hero->inventory[inv_slot].quantity -= quantity;

					/* adjust weight */
					hero->load -= p_item->weight * quantity;
				} else {
					/* remove full stack */

					/* adjust weight */
					hero->load -= p_item->weight * hero->inventory[inv_slot].quantity;

					/* decrement slot counter */
					hero->num_filled_inv_slots--;

					/* clear the inv_slot */
					memset(&hero->inventory[inv_slot], 0, sizeof(inventory));
				}

				retval = 1;
			} else {
				/* item is not stackable */
				if (!(quantity != -1 || GUI_bool(get_ttx(220)))) {
				} else {

					/* check if item is equipped */
					if (inv_slot < HERO_INVENTORY_SLOT_KNAPSACK_1) {
						unequip(hero, item_id, inv_slot);
					}

					/* decrement slot counter */
					hero->num_filled_inv_slots--;

					/* subtract item weight */
					hero->load -= p_item->weight;

					/* check special items */
					/* item: SICHEL Pflanzenkunde -3 */
					if (item_id == ITEM_SICHEL__MAGIC) {

						hero->talents[TA_PFLANZENKUNDE] = hero->talents[TA_PFLANZENKUNDE] - 3;
					}

					/* item:  AMULETT MR -5 */
					if (item_id == ITEM_AMULETT__BLUE) {

						hero->mr = hero->mr - 5;
					}

					/* clear the inventory slot */
					memset(&hero->inventory[inv_slot], 0, sizeof(inventory));
					retval = 1;
				}
			}
		}

		/* check for the pirate cave on Manrek to bring Efferd a gift */
		if ((item_id == ITEM_DREIZACK || item_id == ITEM_NETZ) &&
			(gs_dungeon_id == DUNGEON_ID_PIRATENHOEHLE) && (gs_x_target == 9) && (gs_y_target == 9))
			/* no check of dungeon level needed: pirate cafe has only a single level */
		{
			gs_dng11_efferd_sacrifice = 1;
		}

		/* check for the mine in Oberorken to bring Ingerimm a gift */
		if (is_item_pleasing_ingerimm(item_id) &&
			(gs_dungeon_id == DUNGEON_ID_ZWERGENFESTE) && (gs_x_target == 2) && (gs_y_target == 14) && (gs_dungeon_level == 1))
		{
			gs_dng12_ingerimm_sacrifice = 1;
		}
	}

	return retval;
}

/**
 * \brief   gives one item to the active group
 *
 * \param   item_id	the item ID
 * \param   dummy	unused parameter
 * \param   quantity	number of items
 * \return  the number of given items
 */
signed int give_new_item_to_group(signed int item_id, const signed int dummy, signed int quantity)
{
	signed int i;
	signed int retval = 0;
	signed int quant_hero;
	signed int done = 0;
	signed int dropper_pos;
	signed int vc;
	struct struct_hero *hero_i;
	signed int autofight_bak;

	/* Special stacked items */
	if (item_id == ITEM_200_PFEILE) { item_id = ITEM_PFEIL; quantity = 200; } else
	if (item_id == ITEM_50_BOLZEN) { item_id = ITEM_BOLZEN; quantity = 50; } else
	if (item_id == ITEM_20_KLETTERHAKEN) { item_id = ITEM_KLETTERHAKEN; quantity = 20;}

	do {
		hero_i = get_hero(0);
		for (i = 0; i <= 6; i++, hero_i++) {

			if ((hero_i->typus) && (hero_i->group_id == gs_active_group_id))
			{
				while ((quantity > 0) && (quant_hero = give_new_item_to_hero(hero_i, item_id, 0, quantity)) > 0) {
					quantity -= quant_hero;
					retval += quant_hero;
				}
			}
		}

		if (quantity > 0) {

			autofight_bak = g_autofight;
			g_autofight = 0;

			sprintf(g_dtp2,	get_ttx(549), GUI_name_inflect_with_article(
				(quantity > 1 ? INFLECT_PLURAL : INFLECT_SINGULAR) + (INFLECT_INDEFINITE_ARTICLE | INFLECT_4TH_CASE),
				item_id,
				INFLECT_NAME_TYPE_ITEM
			));

			if (GUI_bool(g_dtp2)) {

				dropper_pos = select_hero_ok(get_ttx(550));

				if (dropper_pos != -1) {
					hero_i = get_hero(dropper_pos);
					g_prevent_drop_equipped_items = 1;
					vc = select_item_to_drop(hero_i);
					g_prevent_drop_equipped_items = 0;

					if (vc != -1) {
						drop_item(hero_i, vc, -1);
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
 * \brief   returns how many items of one type the hero has, ignoring stack sizes.
 *
 * \param   hero        the hero
 * \param   item[in]	the item ID
 */
signed int hero_count_item(struct struct_hero *hero, const signed int item_id)
{
	signed int inv_slot;
	signed int ret = 0;

	for (inv_slot = 0; inv_slot < NR_HERO_INVENTORY_SLOTS; inv_slot++) {

		if (hero->inventory[inv_slot].item_id == item_id) {
			ret++;
		}
	}

	return ret;
}

/**
 * \brief   count the number of items of one type the current group has, ignoring stack sizes.
 *
 * \param   item_id[in]	the item-ID
 * \return  the number of items
 */
signed int group_count_item(const signed int item_id)
{
	struct struct_hero *hero_i;
	signed int hero_pos;
	signed int ret = 0;

	hero_i = get_hero(0);
	for (hero_pos = 0; hero_pos <= 6; hero_pos++, hero_i++) {

		if (hero_i->typus && (hero_i->group_id == gs_active_group_id)) {

			ret += hero_count_item(hero_i, item_id);
		}
	}

	return ret;
}

/**
 * \brief   a hero will loose an item
 *
 * \param   hero        the hero
 * \param   chance      probability to loose as a percentage value
 * \param   text        the displayed text
 */
void loose_random_item(struct struct_hero *hero, const signed int chance, char *text)
	/* TODO / FEATURE_MOD: rethink plausibility.
	 * For example, in many situations, suddenly loosing your equipped body armor does not make any sense.
	 */
{
	struct item_stats *p_item;
	signed int item_id;
	signed int inv_slot;

	if (random_schick(100) > chance)
		return;

	/* Original-Bug: infinite loop if the hero has no item */
	do {
		inv_slot = random_schick(NR_HERO_INVENTORY_SLOTS) - 1;

		item_id = hero->inventory[inv_slot].item_id;

		p_item = &g_itemsdat[item_id];

		/* No item to drop */
		if (item_id != 0 && !p_item->flags.undropable) {

			/* drop 1 item */
			drop_item(hero, inv_slot, 1);

			sprintf(g_text_output_buf, text, hero->alias, (uint8_t*)GUI_name_inflect_with_article(
				INFLECT_INDEFINITE_ARTICLE | INFLECT_SINGULAR | INFLECT_1ST_CASE,
				item_id,
				INFLECT_NAME_TYPE_ITEM
			));
			GUI_output(g_text_output_buf);

			return;
		}

	} while (1);
}

signed int select_item_to_drop(struct struct_hero *hero)
{
	signed int i;
	signed int answer = 0;
	signed int item_cnt = 0;
	signed int item_id;
	signed int va;
	signed int tw_bak, bak2, bak3;
	char *ptr;
	signed int str[23];
	signed int di;

	/* check if we drop equipped items or not */
	i = g_prevent_drop_equipped_items ? HERO_INVENTORY_SLOT_KNAPSACK_1 : 0;
	for (; i < NR_HERO_INVENTORY_SLOTS; i++) {

		if ((item_id = hero->inventory[i].item_id)) {
			str[item_cnt] = i;
			g_radio_name_list[item_cnt] = (g_dtp2 + item_cnt * 30);
			strcpy(g_radio_name_list[item_cnt], GUI_name_base_form(g_itemsname[item_id]));
			item_cnt++;
		}
	}

	if (item_cnt == 0) {
		sprintf(g_dtp2, get_ttx(750), hero->alias);
		GUI_output(g_dtp2);
		return -1;
	}

	di = 0;
	while (answer != -1) {

		va = -1;
		if (item_cnt > 12) {
			if (!di) {
				i = 13;
				va = i - 1;
				ptr = g_radio_name_list[va];
				g_radio_name_list[va] = get_ttx(751);
			} else {
				i = item_cnt + 1;
				va = i - 1;
				ptr = g_radio_name_list[va];
				g_radio_name_list[va] = get_ttx(751);
				i -= di;
			}
		} else {
			i = item_cnt;
		}

		tw_bak = g_textbox_width;
		bak2 = g_basepos_x;
		bak3 = g_basepos_y;
		g_textbox_width = 6;
		g_basepos_x = g_basepos_y = 0;

		answer = GUI_radio(get_ttx(752), (signed char)i,
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
		if ((item_cnt > 12) && (answer == i)) {
			di += 12;
			if (di > item_cnt) {
				di = 0;
			}
		} else {
			if (answer != -1) {
				return str[di + answer - 1];
			}
		}
	}

	return -1;
}
