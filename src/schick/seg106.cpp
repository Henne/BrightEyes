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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   check for a two hand collision
 *
 *          A hero can not equip a two-handed weapon if one hand is in use,
 *          nor can he equip something in his free hand if he has a two-handed weapon
 *          in the other.
 *          If such a collision is detected this function returns 1 else 0.
 *
 * \param   hero        the hero
 * \param   item        the item which should be equipped
 * \param   pos         the position the item should be placed
 */
/* Borlandified and identical */
signed int two_hand_collision(struct struct_hero* hero, const signed int item_id, const signed int pos)
{
	signed int retval = 0;
	signed int other_pos;
	signed int in_hand;

	if (pos == 3 || pos == 4) {

		other_pos = 3;

		if (pos == 3) {
			other_pos = 4;
		}

		/* get the item in the other hand */
		in_hand = hero->inventory[other_pos].item_id;
		if (in_hand) {

			/* check if one hand has a two-handed weapon */
			if ((g_itemsdat[item_id].flags.weapon && (g_itemsdat[item_id].subtype == WEAPON_TYPE_ZWEIHAENDER)) ||
			(g_itemsdat[in_hand].flags.weapon && (g_itemsdat[in_hand].subtype == WEAPON_TYPE_ZWEIHAENDER))) {
				retval = 1;
			}
		}
	}

	return retval;
}

/* Borlandified and identical */
/* is it pos2 -> pos1 or pos1 -> pos2 ? */
void move_item(signed int pos1, signed int pos2, struct struct_hero *hero)
{
	signed short item1;
	signed short item2;
	signed short v3 = 0;
	signed short temp;
	struct inventory tmp;

	if (!check_hero(hero) || (pos1 == pos2)) {

	} else {

		if ((pos2 > HERO_INVENTORY_SLOT_KNAPSACK_1 - 1) && (pos1 > HERO_INVENTORY_SLOT_KNAPSACK_1 - 1)) {
			/* Both items are in knapsacks */
			v3 = 1;
			item1 = hero->inventory[pos1].item_id;
			item2 = hero->inventory[pos2].item_id;
		} else {
			item1 = hero->inventory[pos1].item_id;
			item2 = hero->inventory[pos2].item_id;

			if ((pos2 < pos1) || ((pos1 < HERO_INVENTORY_SLOT_KNAPSACK_1) && (pos2 < HERO_INVENTORY_SLOT_KNAPSACK_1))) {

				if (pos1 < HERO_INVENTORY_SLOT_KNAPSACK_1) {
					if (item1 != 0)
						v3 = 1;
				} else {
					v3 = 1;
				}

				if (v3 != 0) {
					/* exchange positions */
					temp = pos1;
					pos1 = pos2;
					pos2 = temp;

					/* exchange ids */
					temp = item1;
					item1 = item2;
					item2 = temp;
				}
			}

			v3 = 0;

			if ((item1 == 0) && (item2 == 0)) {
				GUI_output(get_ttx(209));
			} else {
				if (item2 != 0) {
					/* item have the same ids and are stackable */
					if ((item2 == item1) && g_itemsdat[item1].flags.stackable) {
						/* merge them */

						/* add quantity of item at pos2 to item at pos1 */
						hero->inventory[pos1].quantity += hero->inventory[pos2].quantity;

						/* delete item at pos2 */
						memset((Bit8u*)&hero->inventory[pos2], 0, sizeof(inventory));
#ifdef M302de_ORIGINAL_BUGFIX
						/* Decrement the item counter */
						hero->items_num--;
#endif
					} else {
						if (!can_hero_use_item(hero, item2)) {

							sprintf(g_dtp2, get_ttx(221), hero->alias,
								get_ttx((hero->sex != 0 ? 593 : 9) + hero->typus),
								GUI_names_grammar(2, item2, 0));

							GUI_output(g_dtp2);

						} else {
							if (!can_item_at_pos(item2, pos1)) {

								if (is_in_word_array(item2, g_items_pluralwords))

									sprintf(g_dtp2, get_ttx(222),
										GUI_names_grammar(0x4000, item2, 0), get_ttx(557));
								else
									sprintf(g_dtp2, get_ttx(222),
										GUI_names_grammar(0, item2, 0), get_ttx(556));

								GUI_output(g_dtp2);
							} else {
								if (two_hand_collision(hero, item2, pos1)) {

									sprintf(g_dtp2, get_ttx(829), hero->alias);
									GUI_output(g_dtp2);

								} else {
									if (item1 != 0) {
										unequip(hero, item1, pos1);
									}

									add_equip_boni(hero, hero, item2, pos2, pos1);
									v3 = 1;
								}
							}
						}
					}
				} else {
					unequip(hero, item1, pos1);
					v3 = 1;
				}
			}
		}

		if (v3 != 0) {

			/* item have the same ids and are stackable */
			if ((item2 == item1) && g_itemsdat[item1].flags.stackable) {
				/* merge them */

				/* add quantity of item at pos2 to item at pos1 */
				hero->inventory[pos1].quantity += hero->inventory[pos2].quantity;

				/* delete item at pos2 */
				memset(&hero->inventory[pos2], 0, sizeof(inventory));
#ifdef M302de_ORIGINAL_BUGFIX
				/* Decrement the item counter */
				hero->items_num--;
#endif
			} else {

				/* exchange the items */
				tmp = hero->inventory[pos1];
				hero->inventory[pos1] = hero->inventory[pos2];
				hero->inventory[pos2] = tmp;
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
				(Bit8u*)GUI_names_grammar(0x4004, inventory_p->item_id, 0));
		} else {
			/* one item */
			sprintf(g_dtp2, get_tx2(11), get_ttx(304),
				(Bit8u*)GUI_names_grammar(0, inventory_p->item_id, 0));
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
	if (inventory_p->item_id == ITEM_KUKRIS_DAGGER || inventory_p->item_id == ITEM_KUKRIS_MENGBILAR ||
		inventory_p->flags.poison_expurgicum || inventory_p->flags.poison_vomicum ||
		hero->inventory[pos].poison_type != POISON_TYPE_NONE) {

		strcat(g_dtp2, get_ttx(548));
	}

	/* magic wand */
	if (inventory_p->item_id == ITEM_MAGIC_WAND) {
		sprintf(g_text_output_buf, get_tx2(53), hero->staff_level);
		strcat(g_dtp2, g_text_output_buf);
	}

	GUI_output(g_dtp2);
}

/* Borlandified and nearly identical */
void pass_item(struct struct_hero *hero1, const signed int old_pos1, struct struct_hero *hero2, const signed int pos2)
{
	signed short l_di;
	register signed short pos1 = old_pos1;

	signed short item1;
	signed short item2;
	struct item_stats *item1_desc;
	struct item_stats *item2_desc;
	signed short flag;
	signed short desc1_5;
	signed short desc2_5;
	struct inventory tmp;


	item1 = hero1->inventory[pos1].item_id;

	/* check if item1 is an item */
	if (item1 == 0) {

		GUI_output(get_ttx(209));
		return;
	}

	item2 = hero2->inventory[pos2].item_id;

	item1_desc = &g_itemsdat[item1];
	item2_desc = &g_itemsdat[item2];

	if (item1_desc->flags.undropable) {

		sprintf(g_dtp2, get_ttx(454), GUI_names_grammar((signed short)0x8002, item1, 0));

		GUI_output(g_dtp2);
		return;
	}

	if (item2_desc->flags.undropable) {

		sprintf(g_dtp2, get_ttx(454), GUI_names_grammar((signed short)0x8002, item2, 0));
		GUI_output(g_dtp2);
		return;

	}

	/* identical until here */
	if (pos2 < 7) {
		if (!can_hero_use_item(hero2, item1)) {

			sprintf(g_dtp2,	get_ttx(221), hero2->alias, get_ttx((hero2->sex ? 593 : 9) + hero2->typus),
				GUI_names_grammar(2, item1, 0));

			GUI_output(g_dtp2);
			return;

		} else if (!can_item_at_pos(item1, pos2)) {

			if (is_in_word_array(item1, g_items_pluralwords)) {

				sprintf(g_dtp2, get_ttx(222), GUI_names_grammar(0x4000, item1, 0), get_ttx(557));
			} else {
				sprintf(g_dtp2, get_ttx(222), GUI_names_grammar(0, item1, 0), get_ttx(556));
			}

			GUI_output(g_dtp2);
			return;

		} else if (two_hand_collision(hero2, item1, pos2)) {

			sprintf(g_dtp2, get_tx2(67), hero2->alias);
			GUI_output(g_dtp2);
			return;
		}
	}
#if defined(__BORLANDC__)
	/* this assembler code here is only for comparization the disassemblies */
		asm { db 0x9a, 0xff, 0xff, 0x00, 0x00 }
		asm { db 0x9a, 0xff, 0xff, 0x00, 0x00 }
#endif

/* 0x8ff */

	if ((item2 != 0) && (pos1 < 7)) {

		if (!can_hero_use_item(hero1, item2)) {

			sprintf(g_dtp2,	get_ttx(221), hero1->alias,
				get_ttx((hero1->sex ? 593 : 9) + hero1->typus),
				(char*)GUI_names_grammar(2, item2, 0));

#if defined(__BORLANDC__)
			desc1_5 = desc1_5;
#endif

			GUI_output(g_dtp2);
			return;

		} else if (!can_item_at_pos(item2, pos1)) {

			if (is_in_word_array(item2, g_items_pluralwords)) {

				sprintf(g_dtp2, get_ttx(222), GUI_names_grammar(0x4000, item2, 0), get_ttx(557));
			} else {
				sprintf(g_dtp2, get_ttx(222), GUI_names_grammar(0, item2, 0), get_ttx(556));
			}

			GUI_output(g_dtp2);
			return;
		}
	}

/* 0xa14 */

	/* identical from here */
	if (item2 != 0) {

		flag = 1;
		if ((item2 == item1) && item2_desc->flags.stackable) {

			flag = 0;
			l_di = 1;

			if (hero1->inventory[pos1].quantity > 1) {

				sprintf(g_dtp2,	get_ttx(210), hero1->inventory[pos1].quantity,
					(char*)GUI_names_grammar(6, item1, 0), hero2->alias);


				l_di = GUI_input(g_dtp2, 2);
			}

			if (hero1->inventory[pos1].quantity < l_di) {
				l_di = hero1->inventory[pos1].quantity;
			}

			if ((l_di > 0) && (hero2->inventory[pos2].quantity < 99)) {

				if (hero2->inventory[pos2].quantity + l_di > 99) {
					l_di = 99 - hero2->inventory[pos2].quantity;
				}

				while ((hero2->attrib[ATTRIB_KK].current * 100 <= hero2->load + item1_desc->weight * l_di) && (l_di > 0)) {
					l_di--;
				}

				if (l_di > 0) {
					hero2->load += item1_desc->weight * l_di;
					hero2->inventory[pos2].quantity += l_di;
					drop_item(hero1, pos1, l_di);
				} else {
					sprintf(g_dtp2,	get_ttx(779), hero2->alias);
					GUI_output(g_dtp2);
				}
			}
		}

		if (flag != 0) {

			desc1_5 = item1_desc->flags.stackable ?
				hero1->inventory[pos1].quantity * item1_desc->weight :
				item1_desc->weight;

			desc2_5 = item2_desc->flags.stackable ?
				hero2->inventory[pos2].quantity * item2_desc->weight :
				item2_desc->weight;

			if (hero2->attrib[ATTRIB_KK].current * 100 <= hero2->load + desc1_5 - desc2_5) {

				sprintf(g_dtp2,	get_ttx(779), hero2->alias);
				GUI_output(g_dtp2);

			} else {
				if (pos1 < 7) {
					unequip(hero1, item1, pos1);
					add_equip_boni(hero2, hero1, item2, pos2, pos1);
				}

				if (pos2 < 7) {
					unequip(hero2, item2, pos2);
					add_equip_boni(hero1, hero2, item1, pos1, pos2);
				}

				/* exchange two items */
				tmp = hero2->inventory[pos2]; /* struct_copy */

				hero2->load -= desc2_5;

				hero2->inventory[pos2] = hero1->inventory[pos1]; /* struct_copy */

				hero2->load += desc1_5;
				hero1->load -= desc1_5;

				hero1->inventory[pos1] = tmp; /* struct_copy */

				hero1->load += desc2_5;

				/* item counter */
				hero1->items_num--;
				hero2->items_num++;

				/* special items */
				if (item2 == ITEM_SICKLE_MAGIC) {
					hero1->skills[TA_PFLANZENKUNDE] = hero1->skills[TA_PFLANZENKUNDE] + 3;
					hero2->skills[TA_PFLANZENKUNDE] = hero2->skills[TA_PFLANZENKUNDE] - 3;
				}
				if (item2 == ITEM_AMULET_BLUE) {
					hero1->mr = hero1->mr + 5;
					hero2->mr = hero2->mr - 5;
				}
				if (item1 == ITEM_SICKLE_MAGIC) {
					hero1->skills[TA_PFLANZENKUNDE] = hero1->skills[TA_PFLANZENKUNDE] - 3;
					hero2->skills[TA_PFLANZENKUNDE] = hero2->skills[TA_PFLANZENKUNDE] + 3;
				}
				if (item1 == ITEM_AMULET_BLUE) {
					hero1->mr = hero1->mr - 5;
					hero2->mr = hero2->mr + 5;
				}
			}
		}
	} else if (item1_desc->flags.stackable) {

		l_di = 1;

		if (hero1->inventory[pos1].quantity > 1) {

			sprintf(g_dtp2,	get_ttx(210), hero1->inventory[pos1].quantity,
				(char*)GUI_names_grammar(6, item1, 0), hero2->alias);


			l_di = GUI_input(g_dtp2, 2);
		}

		if (hero1->inventory[pos1].quantity < l_di) {
			l_di = hero1->inventory[pos1].quantity;
		}

		while ((hero2->attrib[ATTRIB_KK].current * 100 <= hero2->load + item1_desc->weight * l_di) && (l_di > 0)) {
			l_di--;
		}

		if (l_di > 0) {

			hero2->inventory[pos2] = hero1->inventory[pos1]; /* struct_copy */

			hero2->load += item1_desc->weight * l_di;
			hero2->inventory[pos2].quantity = l_di;
			drop_item(hero1, pos1, l_di);

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
			unequip(hero1, item1, pos1);
		}

		if (pos2 < 7) {
			add_equip_boni(hero1, hero2, item1, pos1, pos2);
		}

		hero2->inventory[pos2] = hero1->inventory[pos1]; /* struct_copy */

		/* adjust weight */
		hero2->load += item1_desc->weight;
		hero1->load -= item1_desc->weight;

		/* adjust item counter */
		hero1->items_num--;
		hero2->items_num++;

		/* clear slot */
		memset(&hero1->inventory[pos1], 0, sizeof(inventory));

		/* special items */
		if (item1 == ITEM_SICKLE_MAGIC) {
			hero1->skills[TA_PFLANZENKUNDE] = hero1->skills[TA_PFLANZENKUNDE] - 3;
			hero2->skills[TA_PFLANZENKUNDE] = hero2->skills[TA_PFLANZENKUNDE] + 3;
		}
		if (item1 == ITEM_AMULET_BLUE) {
			hero1->mr = hero1->mr - 5;
			hero2->mr = hero2->mr + 5;
		}
	}
}

struct items_all {
	signed short a[4];
};

/* Borlandified and identical */
void startup_equipment(struct struct_hero *hero)
{
	signed short i;
	struct items_all all;

	*(struct items_all*)&all = *(struct items_all*)g_hero_startup_items_all;

	for (i = 0; i < 4; i++) {
		give_hero_new_item(hero, all.a[i], 1, 1);
	}

	move_item(HERO_INVENTORY_SLOT_LEGS, HERO_INVENTORY_SLOT_KNAPSACK_3, hero);

	if ((hero->sex != 0) && (hero->typus != HERO_TYPE_WARRIOR) && (hero->typus != HERO_TYPE_MAGE))
       	{
		/* female non-warriors and non-mages get a free shirt */
		give_hero_new_item(hero, ITEM_SHIRT, 1, 1);
		move_item(HERO_INVENTORY_SLOT_BODY, HERO_INVENTORY_SLOT_KNAPSACK_3, hero);
	}

	i = 0;
	while ((g_hero_startup_items[hero->typus - 1][i] != -1) && (i < 4)) {

		give_hero_new_item(hero, g_hero_startup_items[hero->typus - 1][i++], 1, 1);

		if (i == 1) {
			move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, HERO_INVENTORY_SLOT_KNAPSACK_3, hero);
		}
	}

	if (hero->typus == HERO_TYPE_WARRIOR) {
		move_item(HERO_INVENTORY_SLOT_BODY, get_item_pos(hero, ITEM_LEATHER_ARMOR), hero);
	}

	if (hero->typus == HERO_TYPE_MAGE) {
		move_item(HERO_INVENTORY_SLOT_BODY, get_item_pos(hero, ITEM_ROBE_GREEN), hero);
	}

	if ((hero->typus == HERO_TYPE_HUNTER) ||
		(hero->typus == HERO_TYPE_GREEN_ELF) ||
		(hero->typus == HERO_TYPE_SYLVAN_ELF))
	{
		give_hero_new_item(hero, ITEM_ARROWS, 1, 20);
		move_item(HERO_INVENTORY_SLOT_LEFT_HAND, get_item_pos(hero, ITEM_ARROWS), hero);
	}
}

/**
 * \brief   get the maximum time of a burning lightsource
 */
/* Borlandified and identical */
signed short get_max_light_time(void)
{
	struct struct_hero *hero;
	signed short i;
	signed short j;
	signed short retval;

	retval = -1;

	hero = (struct struct_hero*)get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

#ifdef M302de_ORIGINAL_BUGFIX
		if (!hero->typus)
			continue;
#endif

		for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {

			/* search for a burning torch */
			if (hero->inventory[j].item_id == ITEM_TORCH_ON) {

				if (hero->inventory[j].lighting_timer > retval) {
					retval = hero->inventory[j].lighting_timer;
				}

			} else if (hero->inventory[j].item_id == ITEM_LANTERN_ON) {

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
	signed short i;
	signed short handle;

	signed short width;
	signed short height;
	Bit32s nvf_length;
	Bit8u *p_pal;
	struct nvf_desc nvf;

	/* open GUERTEL.NVF */
	handle = load_archive_file(ARCHIVE_FILE_GUERTEL_NVF);

	/* read NVF part 1 */
	nvf_length = read_archive_file(handle, (Bit8u*)g_buffer9_ptr, 64000);
	/* read NVF part 2 */
	nvf_length += read_archive_file(handle, (Bit8u*)(g_buffer9_ptr + 64000L), 64000);

	close(handle);

	/* calculate palette pointer */
	p_pal = (Bit8u*)((g_buffer9_ptr + nvf_length) -0x60L);

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
		nvf.src = (Bit8u*)g_buffer9_ptr;
		nvf.no = i;
		nvf.type = 3;
		nvf.width = &width;
		nvf.height = &height;

		process_nvf(&nvf);

		g_pic_copy.x1 = 160;
		g_pic_copy.y1 = 50;
		g_pic_copy.x2 = width + 159;
		g_pic_copy.y2 = height + 49;
		g_pic_copy.src = g_renderbuf_ptr;

		wait_for_vsync();
		wait_for_vsync();
		wait_for_vsync();
		update_mouse_cursor();
		wait_for_vsync();

		do_pic_copy(0);

		refresh_screen_size();
	}

	g_current_ani = g_dng_area_loaded = g_city_area_loaded = -1;
}

/**
 * \brief   search for a full waterskin
 *
 * \param   hero        pointer to the hero
 * \return              position of a non-empty waterskin
 */
/* Borlandified and identical */
signed short get_full_waterskin_pos(const struct struct_hero *hero)
{
	signed short pos = -1;
	signed short i;

	/* Original-BUG: should start from inventory pos 0 */
	for (i = HERO_INVENTORY_SLOT_KNAPSACK_1; i < NR_HERO_INVENTORY_SLOTS; i++) {

		/* look for a non-empty waterskin */
		if ((hero->inventory[i].item_id == ITEM_WATERSKIN) && !hero->inventory[i].flags.empty)
		{
			pos = i;
			break;
		}
	}

	return pos;
}

#if !defined(__BORLANDC__)
}
#endif
