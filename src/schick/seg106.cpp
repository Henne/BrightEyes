/*
	Rewrite of DSA1 v3.02_de functions of seg106 (inventory misc)
	Functions rewritten: 8/8 (complete)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__BORLANDC__)
#include <IO.H>
#else
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static const signed int g_hero_startup_items[12][4] = {
	{ ITEM_DOLCH, ITEM_WURFMESSER, ITEM_WURFMESSER, -1 }, /* Gaukler */
	{ ITEM_LANGBOGEN, ITEM_DOLCH, -1, -1 }, /* Jaeger */
	{ ITEM_SCHWERT, ITEM_DOLCH, ITEM_LEDERHARNISCH, -1 }, /* Krieger */
	{ ITEM_RAPIER, ITEM_DOLCH, ITEM_DIETRICHE, -1 }, /* Streuner */
	{ ITEM_SKRAJA, ITEM_SAEBEL, ITEM_SCHNAPSFLASCHE, -1 }, /* Thorwaler */
	{ ITEM_STREITKOLBEN, ITEM_BRECHEISEN, ITEM_HAMMER, -1 }, /* Zwerg */
	{ ITEM_HEXENBESEN, ITEM_EINBEERE, ITEM_EINBEERE, -1 }, /* Hexe */
	{ ITEM_VULKANGLASDOLCH, ITEM_WIRSELKRAUT, ITEM_JORUGAWURZEL, -1 }, /* Druide */
	{ ITEM_ZAUBERSTAB, ITEM_DOLCH, ITEM_SCHREIBZEUG, ITEM_ROBE__GREEN_1 }, /* Magier */
	{ ITEM_LANGBOGEN, ITEM_RAPIER, ITEM_HARFE, -1 }, /* Auelf */
	{ ITEM_ROBBENTOETER, ITEM_SPEER, ITEM_FLOETE, -1 }, /* Firnelf */
	{ ITEM_LANGBOGEN, ITEM_MESSER, ITEM_FLOETE, -1 } /* Waldelf */
}; // ds:0xae48

static const signed int g_hero_startup_items_all[4] = {
	ITEM_WASSERSCHLAUCH, ITEM_PROVIANTPAKET, ITEM_PROVIANTPAKET, ITEM_HOSE
}; // ds:0xaea8

/**
 * \brief   check for a two hand collision
 *
 *          A hero can not equip a two-handed weapon if one hand is in use,
 *          nor can he equip something in his free hand if he has a two-handed weapon
 *          in the other.
 *          If such a collision is detected this function returns 1 else 0.
 *
 * \param   hero        the hero
 * \param   item_id     the item which should be equipped
 * \param   inv_slot    the inventary slot where the item should be placed
 */
/* Borlandified and identical */
signed int two_hand_collision(struct struct_hero* hero, const signed int item_id, const signed int inv_slot)
{
	signed int retval = 0;
	signed int other_inv_slot;
	signed int other_item_id;

	if (inv_slot == HERO_INVENTORY_SLOT_RIGHT_HAND || inv_slot == HERO_INVENTORY_SLOT_LEFT_HAND) {

		other_inv_slot = HERO_INVENTORY_SLOT_RIGHT_HAND;

		if (inv_slot == HERO_INVENTORY_SLOT_RIGHT_HAND) {
			other_inv_slot = HERO_INVENTORY_SLOT_LEFT_HAND;
		}

		/* get the item in the other hand */
		other_item_id = hero->inventory[other_inv_slot].item_id;
		if (other_item_id) {

			/* check if one hand has a two-handed weapon */
			if ((g_itemsdat[item_id].flags.weapon && (g_itemsdat[item_id].subtype == WEAPON_TYPE_ZWEIHAENDER)) ||
			(g_itemsdat[other_item_id].flags.weapon && (g_itemsdat[other_item_id].subtype == WEAPON_TYPE_ZWEIHAENDER))) {
				retval = 1;
			}
		}
	}

	return retval;
}

/* Borlandified and identical */
/* is it inv_slot_2 -> inv_slot_1 or inv_slot_1 -> inv_slot_2 ? */
/* probably slot_2 -> slot_1 ... */
void move_item(signed int inv_slot_1, signed int inv_slot_2, struct struct_hero *hero)
{
	signed int item_id_1;
	signed int item_id_2;
	signed int v3 = 0;
	signed int temp;
	struct inventory tmp;

	if (!check_hero(hero) || (inv_slot_1 == inv_slot_2)) {

	} else {

		if ((inv_slot_2 > HERO_INVENTORY_SLOT_KNAPSACK_1 - 1) && (inv_slot_1 > HERO_INVENTORY_SLOT_KNAPSACK_1 - 1)) {
			/* Both items are in knapsacks */
			v3 = 1;
			item_id_1 = hero->inventory[inv_slot_1].item_id;
			item_id_2 = hero->inventory[inv_slot_2].item_id;
		} else {
			item_id_1 = hero->inventory[inv_slot_1].item_id;
			item_id_2 = hero->inventory[inv_slot_2].item_id;

			if ((inv_slot_2 < inv_slot_1) || ((inv_slot_1 < HERO_INVENTORY_SLOT_KNAPSACK_1) && (inv_slot_2 < HERO_INVENTORY_SLOT_KNAPSACK_1))) {

				if (inv_slot_1 < HERO_INVENTORY_SLOT_KNAPSACK_1) {
					if (item_id_1 != ITEM_NONE)
						v3 = 1;
				} else {
					v3 = 1;
				}

				if (v3 != 0) {
					/* exchange positions */
					temp = inv_slot_1;
					inv_slot_1 = inv_slot_2;
					inv_slot_2 = temp;

					/* exchange ids */
					temp = item_id_1;
					item_id_1 = item_id_2;
					item_id_2 = temp;
				}
			}

			v3 = 0;

			if ((item_id_1 == 0) && (item_id_2 == 0)) {

				GUI_output(get_ttx(209));

			} else {
				if (item_id_2 != ITEM_NONE) {

					/* item have the same ids and are stackable */
					if ((item_id_2 == item_id_1) && g_itemsdat[item_id_1].flags.stackable) {
						/* merge them */

						/* add quantity of item at inv_slot_2 to item at inv_slot_1 */
						hero->inventory[inv_slot_1].quantity += hero->inventory[inv_slot_2].quantity;
						/* Original-Bug: Might result in a stack size bigger than STACK_SIZE_MAX */

						/* delete item at inv_slot_2 */
						memset((uint8_t*)&hero->inventory[inv_slot_2], 0, sizeof(inventory));
#ifdef M302de_ORIGINAL_BUGFIX
						/* Decrement the item counter */
						hero->num_inv_slots_used--;
#endif
					} else {
						if (!can_hero_use_item(hero, item_id_2)) {

							sprintf(g_dtp2, get_ttx(221), hero->alias,
								get_ttx((hero->sex != 0 ? 593 : 9) + hero->typus),
								GUI_names_grammar(2, item_id_2, 0));

							GUI_output(g_dtp2);

						} else {
							if (!can_hero_equip_item_at_slot(item_id_2, inv_slot_1)) {

								if (is_in_word_array(item_id_2, g_items_pluralwords))

									sprintf(g_dtp2, get_ttx(222),
										GUI_names_grammar(0x4000, item_id_2, 0), get_ttx(557));
								else
									sprintf(g_dtp2, get_ttx(222),
										GUI_names_grammar(0, item_id_2, 0), get_ttx(556));

								GUI_output(g_dtp2);
							} else {
								if (two_hand_collision(hero, item_id_2, inv_slot_1)) {

									sprintf(g_dtp2, get_ttx(829), hero->alias);
									GUI_output(g_dtp2);

								} else {
									if (item_id_1 != 0) {
										unequip(hero, item_id_1, inv_slot_1);
									}

									add_equip_boni(hero, hero, item_id_2, inv_slot_2, inv_slot_1);
									v3 = 1;
								}
							}
						}
					}
				} else {
					unequip(hero, item_id_1, inv_slot_1);
					v3 = 1;
				}
			}
		}

		if (v3 != 0) {

			/* item have the same ids and are stackable */
			if ((item_id_2 == item_id_1) && g_itemsdat[item_id_1].flags.stackable) {
				/* merge them */

				/* add quantity of item at inv_slot_2 to item at inv_slot_1 */
				hero->inventory[inv_slot_1].quantity += hero->inventory[inv_slot_2].quantity;

				/* delete item at inv_slot_2 */
				memset(&hero->inventory[inv_slot_2], 0, sizeof(inventory));
#ifdef M302de_ORIGINAL_BUGFIX
				/* Decrement the item counter */
				hero->num_inv_slots_used--;
#endif
			} else {

				/* exchange the items */
				tmp = hero->inventory[inv_slot_1];
				hero->inventory[inv_slot_1] = hero->inventory[inv_slot_2];
				hero->inventory[inv_slot_2] = tmp;
			}
		}
	}
}

/* Borlandified and identical */
void print_item_description(struct struct_hero *hero, const signed int pos)
{
	/* create pointer to the item in the inventory */
	struct inventory *inventory_p = &hero->inventory[pos];

	if (inventory_p->item_id != ITEM_NONE) {

		/* normal item */

		if (((inventory_p->quantity > 1) && g_itemsdat[inventory_p->item_id].flags.stackable) ||
			is_in_word_array(inventory_p->item_id, g_items_pluralwords)) {

			/* more than one item or special */
			sprintf(g_dtp2, get_tx2(72), get_ttx(305),
				(uint8_t*)GUI_names_grammar(0x4004, inventory_p->item_id, 0));
		} else {
			/* one item */
			sprintf(g_dtp2, get_tx2(11), get_ttx(304),
				(uint8_t*)GUI_names_grammar(0, inventory_p->item_id, 0));
		}
	} else {
		/* no item */
		strcpy(g_dtp2, get_ttx(209));
	}


	/* broken */
	if (inventory_p->flags.broken) {
		strcat(g_dtp2, get_ttx(478));
	}

	/* magic */
	if (inventory_p->flags.magic && inventory_p->flags.magic_revealed) {
		strcat(g_dtp2, get_ttx(479));
	}

	/* RS degraded */
	if (inventory_p->rs_lost != 0) {
		strcat(g_dtp2, get_ttx(480));
	}

	/* poisoned */
	if (inventory_p->item_id == ITEM_KUKRISDOLCH || inventory_p->item_id == ITEM_KUKRIS_MENGBILAR ||
		inventory_p->flags.poison_expurgicum || inventory_p->flags.poison_vomicum ||
		hero->inventory[pos].poison_type != POISON_TYPE_NONE) {

		strcat(g_dtp2, get_ttx(548));
	}

	/* magic wand */
	if (inventory_p->item_id == ITEM_ZAUBERSTAB) {
		sprintf(g_text_output_buf, get_tx2(53), hero->staff_level);
		strcat(g_dtp2, g_text_output_buf);
	}

	GUI_output(g_dtp2);
}

/* Borlandified and nearly identical */
void pass_item(struct struct_hero *hero1, const signed int old_pos1, struct struct_hero *hero2, const signed int pos2)
{
	signed int stackable_quant;
	register signed int pos1 = old_pos1;

	const signed int item_id1 = hero1->inventory[pos1].item_id;
	signed int item_id2;
	struct item_stats *item1_desc;
	struct item_stats *item2_desc;
	signed int flag;
	signed int item_weight1;
	signed int item_weight2;
	struct inventory tmp;

	/* check if item_id1 is an item */
	if (item_id1 == 0) {

		GUI_output(get_ttx(209));
		return;
	}

	item_id2 = hero2->inventory[pos2].item_id;

	item1_desc = &g_itemsdat[item_id1];
	item2_desc = &g_itemsdat[item_id2];

	if (item1_desc->flags.undropable) {

		sprintf(g_dtp2, get_ttx(454), GUI_names_grammar((signed int)0x8002, item_id1, 0));

		GUI_output(g_dtp2);
		return;
	}

	if (item2_desc->flags.undropable) {

		sprintf(g_dtp2, get_ttx(454), GUI_names_grammar((signed int)0x8002, item_id2, 0));
		GUI_output(g_dtp2);
		return;

	}

	/* identical until here */
	if (pos2 < 7) {

		if (!can_hero_use_item(hero2, item_id1)) {

			sprintf(g_dtp2,	get_ttx(221), hero2->alias, get_ttx((hero2->sex ? 593 : 9) + hero2->typus),
				GUI_names_grammar(2, item_id1, 0));

#if !defined(__BORLANDC__)
			GUI_output(g_dtp2);
			return;
#else
			asm {
				jmp lab04
			}
#endif

		} else if (!can_hero_equip_item_at_slot(item_id1, pos2)) {

			if (is_in_word_array(item_id1, g_items_pluralwords)) {

				sprintf(g_dtp2, get_ttx(222), GUI_names_grammar(0x4000, item_id1, 0), get_ttx(557));
			} else {
				sprintf(g_dtp2, get_ttx(222), GUI_names_grammar(0, item_id1, 0), get_ttx(556));
			}

#if !defined(__BORLANDC__)
			GUI_output(g_dtp2);
			return;
#else
			asm {
				jmp lab04
			}
#endif

		} else if (two_hand_collision(hero2, item_id1, pos2)) {

			sprintf(g_dtp2, get_tx2(67), hero2->alias);

#if !defined(__BORLANDC__)
			GUI_output(g_dtp2);
			return;
#else
			asm {
				jmp lab04
			}
#endif
		}
	}

/* 0x8ff */

	if ((item_id2 != 0) && (pos1 < 7)) {

		if (!can_hero_use_item(hero1, item_id2)) {

#if !defined(__BORLANDC__)
			sprintf(g_dtp2,	get_ttx(221), hero1->alias,
				get_ttx((hero1->sex ? 593 : 9) + hero1->typus),
				(char*)GUI_names_grammar(2, item_id2, 0));


			GUI_output(g_dtp2);
			return;
#else
			/* REMARK: cant imagine that it was written that way. */
			asm {
				push 0
				push word [item_id2]
				push 2
				call far ptr GUI_names_grammar
				add sp, 0x06
				push dx
				push ax

				les bx, [hero1]
				mov al, es:[bx+0x21]
				cbw
				push ax
				les bx, [hero1]
				cmp byte ptr es:[bx+0x22], 0
				jz lab01
				mov ax, 593
				jmp short lab02
			}
lab01:
			asm {
				mov ax, 9
			}
lab02:
			asm {
				pop dx
				db 0x03, 0xc2 /* add ax, dx */
				shl ax, 2
				les bx, [g_text_ltx_index]
				db 0x03, 0xd8 /* add bx, ax */
				push word ptr es:[bx+2]
				push word ptr es:[bx]

				mov ax, word ptr [hero1]
				add ax, 0x10
				push word ptr[hero1 + 2]
				push ax

				les bx, [g_text_ltx_index]
				push word ptr es:[bx+0x376]
				push word ptr es:[bx+0x374]

				push word ptr [g_dtp2 + 2]
				push word ptr [g_dtp2]

				call far ptr sprintf
				add sp, 0x14
				jmp lab04
			}

#endif

		} else if (!can_hero_equip_item_at_slot(item_id2, pos1)) {

			if (is_in_word_array(item_id2, g_items_pluralwords)) {

				sprintf(g_dtp2, get_ttx(222), GUI_names_grammar(0x4000, item_id2, 0), get_ttx(557));
			} else {
				sprintf(g_dtp2, get_ttx(222), GUI_names_grammar(0, item_id2, 0), get_ttx(556));

			}
#if defined (__BORLANDC__)
lab04:
#endif
			GUI_output(g_dtp2);
			return;
		}
	}

/* 0xa14 */

	/* identical from here */
	if (item_id2 != ITEM_NONE) {

		flag = 1;
		if ((item_id2 == item_id1) && item2_desc->flags.stackable) {

			flag = 0;
			stackable_quant = 1;

			if (hero1->inventory[pos1].quantity > 1) {

				sprintf(g_dtp2,	get_ttx(210), hero1->inventory[pos1].quantity,
					(char*)GUI_names_grammar(6, item_id1, 0), hero2->alias);


				stackable_quant = GUI_input(g_dtp2, 2);
			}

			if (hero1->inventory[pos1].quantity < stackable_quant) {
				stackable_quant = hero1->inventory[pos1].quantity;
			}

			if ((stackable_quant > 0) && (hero2->inventory[pos2].quantity < STACK_SIZE_MAX)) {

				if (hero2->inventory[pos2].quantity + stackable_quant > STACK_SIZE_MAX) {
					stackable_quant = STACK_SIZE_MAX - hero2->inventory[pos2].quantity;
				}

				while ((hero2->attrib[ATTRIB_KK].current * 100 <= hero2->load + item1_desc->weight * stackable_quant) && (stackable_quant > 0)) {
					stackable_quant--;
				}

				if (stackable_quant > 0) {
					hero2->load += item1_desc->weight * stackable_quant;
					hero2->inventory[pos2].quantity += stackable_quant;
					drop_item(hero1, pos1, stackable_quant);
				} else {
					sprintf(g_dtp2,	get_ttx(779), hero2->alias);
					GUI_output(g_dtp2);
				}
			}
		}

		if (flag != 0) {

			item_weight1 = item1_desc->flags.stackable ?
				hero1->inventory[pos1].quantity * item1_desc->weight :
				item1_desc->weight;

			item_weight2 = item2_desc->flags.stackable ?
				hero2->inventory[pos2].quantity * item2_desc->weight :
				item2_desc->weight;

			if (hero2->attrib[ATTRIB_KK].current * 100 <= hero2->load + item_weight1 - item_weight2) {

				sprintf(g_dtp2,	get_ttx(779), hero2->alias);
				GUI_output(g_dtp2);

			} else {
				if (pos1 < 7) {
					unequip(hero1, item_id1, pos1);
					add_equip_boni(hero2, hero1, item_id2, pos2, pos1);
				}

				if (pos2 < 7) {
					unequip(hero2, item_id2, pos2);
					add_equip_boni(hero1, hero2, item_id1, pos1, pos2);
				}

				/* exchange two items */
				tmp = hero2->inventory[pos2]; /* struct_copy */

				hero2->load -= item_weight2;

				hero2->inventory[pos2] = hero1->inventory[pos1]; /* struct_copy */

				hero2->load += item_weight1;
				hero1->load -= item_weight1;

				hero1->inventory[pos1] = tmp; /* struct_copy */

				hero1->load += item_weight2;

				/* item counter */
				hero1->num_inv_slots_used--;
				hero2->num_inv_slots_used++;

				/* special items */
				if (item_id2 == ITEM_SICHEL__MAGIC) {
					hero1->skills[TA_PFLANZENKUNDE] = hero1->skills[TA_PFLANZENKUNDE] + 3;
					hero2->skills[TA_PFLANZENKUNDE] = hero2->skills[TA_PFLANZENKUNDE] - 3;
				}
				if (item_id2 == ITEM_AMULETT__BLUE) {
					hero1->mr = hero1->mr + 5;
					hero2->mr = hero2->mr - 5;
				}
				if (item_id1 == ITEM_SICHEL__MAGIC) {
					hero1->skills[TA_PFLANZENKUNDE] = hero1->skills[TA_PFLANZENKUNDE] - 3;
					hero2->skills[TA_PFLANZENKUNDE] = hero2->skills[TA_PFLANZENKUNDE] + 3;
				}
				if (item_id1 == ITEM_AMULETT__BLUE) {
					hero1->mr = hero1->mr - 5;
					hero2->mr = hero2->mr + 5;
				}
			}
		}

	} else if (item1_desc->flags.stackable) {

		stackable_quant = 1;

		if (hero1->inventory[pos1].quantity > 1) {

			sprintf(g_dtp2,	get_ttx(210), hero1->inventory[pos1].quantity,
				(char*)GUI_names_grammar(6, item_id1, 0), hero2->alias);


			stackable_quant = GUI_input(g_dtp2, 2);
		}

		if (hero1->inventory[pos1].quantity < stackable_quant) {
			stackable_quant = hero1->inventory[pos1].quantity;
		}

		while ((hero2->attrib[ATTRIB_KK].current * 100 <= hero2->load + item1_desc->weight * stackable_quant) && (stackable_quant > 0)) {
			stackable_quant--;
		}

		if (stackable_quant > 0) {

			hero2->inventory[pos2] = hero1->inventory[pos1]; /* struct_copy */

			hero2->load += item1_desc->weight * stackable_quant;
			hero2->inventory[pos2].quantity = stackable_quant;
			drop_item(hero1, pos1, stackable_quant);

		} else {
			sprintf(g_dtp2,	get_ttx(779), hero2->alias);
			GUI_output(g_dtp2);
		}

	} else if (hero2->attrib[ATTRIB_KK].current * 100 <= hero2->load + item1_desc->weight) {

		sprintf(g_dtp2,	get_ttx(779), hero2->alias);
		GUI_output(g_dtp2);

	} else {

		/* do the change */

		if (pos1 < 7) {
			unequip(hero1, item_id1, pos1);
		}

		if (pos2 < 7) {
			add_equip_boni(hero1, hero2, item_id1, pos1, pos2);
		}

		hero2->inventory[pos2] = hero1->inventory[pos1]; /* struct_copy */

		/* adjust weight */
		hero2->load += item1_desc->weight;
		hero1->load -= item1_desc->weight;

		/* adjust item counter */
		hero1->num_inv_slots_used--;
		hero2->num_inv_slots_used++;

		/* clear slot */
		memset(&hero1->inventory[pos1], 0, sizeof(inventory));

		/* special items */
		if (item_id1 == ITEM_SICHEL__MAGIC) {
			hero1->skills[TA_PFLANZENKUNDE] = hero1->skills[TA_PFLANZENKUNDE] - 3;
			hero2->skills[TA_PFLANZENKUNDE] = hero2->skills[TA_PFLANZENKUNDE] + 3;
		}
		if (item_id1 == ITEM_AMULETT__BLUE) {
			hero1->mr = hero1->mr - 5;
			hero2->mr = hero2->mr + 5;
		}
	}
}

struct items_all {
	signed int a[4];
};

/* Borlandified and identical */
void startup_equipment(struct struct_hero *hero)
{
	signed int i;
	struct items_all all;

	*(struct items_all*)&all = *(struct items_all*)g_hero_startup_items_all; /* struct copy */

	for (i = 0; i < 4; i++) {
		give_new_item_to_hero(hero, all.a[i], 1, 1);
	}

	move_item(HERO_INVENTORY_SLOT_LEGS, HERO_INVENTORY_SLOT_KNAPSACK_3, hero);

	if ((hero->sex != 0) && (hero->typus != HERO_TYPE_KRIEGER) && (hero->typus != HERO_TYPE_MAGIER))
       	{
		/* female non-warriors and non-mages get a free shirt */
		give_new_item_to_hero(hero, ITEM_HEMD, 1, 1);
		move_item(HERO_INVENTORY_SLOT_BODY, HERO_INVENTORY_SLOT_KNAPSACK_3, hero);
	}

	i = 0;
	while ((g_hero_startup_items[hero->typus - 1][i] != -1) && (i < 4)) {

		give_new_item_to_hero(hero, g_hero_startup_items[hero->typus - 1][i++], 1, 1);

		if (i == 1) {
			move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, HERO_INVENTORY_SLOT_KNAPSACK_3, hero);
		}
	}

	if (hero->typus == HERO_TYPE_KRIEGER) {
		move_item(HERO_INVENTORY_SLOT_BODY, inv_slot_of_item(hero, ITEM_LEDERHARNISCH), hero);
	}

	if (hero->typus == HERO_TYPE_MAGIER) {
		move_item(HERO_INVENTORY_SLOT_BODY, inv_slot_of_item(hero, ITEM_ROBE__GREEN_1), hero);
	}

	if ((hero->typus == HERO_TYPE_JAEGER) ||
		(hero->typus == HERO_TYPE_AUELF) ||
		(hero->typus == HERO_TYPE_WALDELF))
	{
		give_new_item_to_hero(hero, ITEM_PFEIL, 1, 20);
		move_item(HERO_INVENTORY_SLOT_LEFT_HAND, inv_slot_of_item(hero, ITEM_PFEIL), hero);
	}
}

/**
 * \brief   get the maximum time of a burning lightsource
 */
/* Borlandified and identical */
signed int get_max_light_time(void)
{
	struct struct_hero *hero;
	signed int i;
	signed int j;
	signed int retval = -1;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

#ifdef M302de_ORIGINAL_BUGFIX
		if (!hero->typus)
			continue;
#endif

		for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {

			/* search for a burning torch */
			if (hero->inventory[j].item_id == ITEM_FACKEL__LIT) {

				if (hero->inventory[j].lighting_timer > retval) {
					retval = hero->inventory[j].lighting_timer;
				}

			} else if (hero->inventory[j].item_id == ITEM_LATERNE__LIT) {

				/* search for a burning lantern */

				if (hero->inventory[j].lighting_timer / 10 > retval) {
					retval = hero->inventory[j].lighting_timer / 10;
				}
			}
		}
	}

	return retval;
}

/**
 * \brief   shows an animation of a man equipping a belt
 *
 *          This is only executed when equipping a skullbelt.
 */
/* Borlandified and identical */
void equip_belt_ani(void)
{
	signed int i;
	signed int handle;

	signed int width;
	signed int height;
	int32_t nvf_length;
	uint8_t *p_pal;
	struct nvf_extract_desc nvf;

	/* open GUERTEL.NVF */
	handle = load_archive_file(ARCHIVE_FILE_GUERTEL_NVF);

	/* read NVF part 1 */
	nvf_length = read_archive_file(handle, (uint8_t*)g_buffer9_ptr, 64000);
	/* read NVF part 2 */
	nvf_length += read_archive_file(handle, (uint8_t*)(g_buffer9_ptr + 64000L), 64000);

	close(handle);

	/* calculate palette pointer */
	p_pal = (uint8_t*)((g_buffer9_ptr + nvf_length) -0x60L);

	wait_for_vsync();

	set_palette(p_pal, 0x80, 0x20);

	do_border(g_vga_memstart, 209, 79, 215, 89, 9);

	do_fill_rect(g_vga_memstart, 209, 79, 215, 89, 0);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect(g_vga_memstart, 189, 69, 235, 99, 0);

	do_border(g_vga_memstart, 189, 69, 235, 99, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect(g_vga_memstart, 169, 59, 255, 109, 0);

	do_border(g_vga_memstart, 169, 59, 255, 109, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect(g_vga_memstart, 164, 54, 260, 114, 0);

	do_border(g_vga_memstart, 164, 54, 260, 114, 9);

	wait_for_vsync();
	wait_for_vsync();
	wait_for_vsync();

	do_fill_rect(g_vga_memstart, 159, 49, 263, 117, 0);

	do_border(g_vga_memstart, 159, 49, 263, 117, 9);

	for (i = 0; i < 12; i++) {

		nvf.dst = g_renderbuf_ptr;
		nvf.src = (uint8_t*)g_buffer9_ptr;
		nvf.image_num = i;
		nvf.compression_type = 3;
		nvf.width = &width;
		nvf.height = &height;

		process_nvf_extraction(&nvf);

		g_pic_copy.x1 = 160;
		g_pic_copy.y1 = 50;
		g_pic_copy.x2 = width + 159;
		g_pic_copy.y2 = height + 49;
		g_pic_copy.src = g_renderbuf_ptr;

		wait_for_vsync();
		wait_for_vsync();
		wait_for_vsync();
		call_mouse_bg();
		wait_for_vsync();

		do_pic_copy(0);

		call_mouse();
	}

	g_current_ani = g_dng_loaded_dungeon_id = g_town_loaded_town_id = -1;
}

/**
 * \brief   search for a full waterskin
 *
 * \param   hero        pointer to the hero
 * \return              position of a non-empty waterskin
 */
/* Borlandified and identical */
signed int get_full_waterskin_pos(const struct struct_hero *hero)
{
	signed int inv_pos = -1;
	signed int i;

	/* Original-BUG: should start from inventory pos 0 */
	for (i = HERO_INVENTORY_SLOT_KNAPSACK_1; i < NR_HERO_INVENTORY_SLOTS; i++) {

		/* look for a non-empty waterskin */
		if ((hero->inventory[i].item_id == ITEM_WASSERSCHLAUCH) && !hero->inventory[i].flags.empty)
		{
			inv_pos = i;
			break;
		}
	}

	return inv_pos;
}

#if !defined(__BORLANDC__)
}
#endif
