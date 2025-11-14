/**
 *	Rewrite of DSA1 v3.02_de functions of seg092 (treasures)
 *	Functions rewritten: 22/22 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg092.cpp
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg047.h"
#include "seg076.h"
#include "seg077.h"
#include "seg078.h"
#include "seg079.h"
#include "seg080.h"
#include "seg081.h"
#include "seg082.h"
#include "seg083.h"
#include "seg084.h"
#include "seg085.h"
#include "seg086.h"
#include "seg088.h"
#include "seg089.h"
#include "seg090.h"
#include "seg091.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static struct struct_chest *g_dng_specialchest_index[15] = {
	g_dng01_specialchests,
	g_dng02_specialchests,
	g_dng03_specialchests,
	g_dng04_specialchests,
	NULL,
	g_dng06_specialchests,
	g_dng07_specialchests,
	g_dng08_specialchests,
	g_dng09_specialchests,
	g_dng10_specialchests,
	g_dng11_specialchests,
	g_dng12_specialchests,
	g_dng13_specialchests,
	g_dng14_specialchests,
	g_dng15_specialchests
}; // ds:0x9d84; uint8_t*
static const char g_str_single_space[2] = " "; // ds:0x9dc4

void chest_protected_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(518));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le(get_first_hero_available_in_group(), random_schick(6));
}

void chest_closed(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(519));
}

void chest_protected_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(get_first_hero_available_in_group(), dice_roll(2, 6, 0));
}

void chest_poisoned1(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le((gs_main_acting_hero = get_first_hero_available_in_group()), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(gs_main_acting_hero, POISON_TYPE_SHURINKNOLLENGIFT);

#if !defined(__BORLANDC__)
	gs_main_acting_hero = NULL;
#endif
}

void chest_poisoned2(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le((gs_main_acting_hero = get_first_hero_available_in_group()), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(gs_main_acting_hero, POISON_TYPE_ARAX);

#if !defined(__BORLANDC__)
	gs_main_acting_hero = NULL;
#endif
}

void chest_poisoned3(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le((gs_main_acting_hero = get_first_hero_available_in_group()), dice_roll(1, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned(gs_main_acting_hero, POISON_TYPE_KUKRIS);

#if !defined(__BORLANDC__)
	gs_main_acting_hero = NULL;
#endif
}

void chest_protected_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le(get_first_hero_available_in_group(), dice_roll(4, 6, 0));
}

void chest_petrified(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(776));

	/* save pointer of the first hero */
	gs_main_acting_hero = get_first_hero_available_in_group();

	/* and make him petrified */
	gs_main_acting_hero->flags.petrified = 1;

#if !defined(__BORLANDC__)
	gs_main_acting_hero = NULL;
#endif
}

void chest_ignifax_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le(get_first_hero_available_in_group(), dice_roll(2, 6, 0));
}

void chest_ignifax_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le(get_first_hero_available_in_group(), dice_roll(4, 6, 0));
}

void chest_ignifax_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 3W6 */
	sub_hero_le(get_first_hero_available_in_group(), dice_roll(3, 6, 0));
}

void chest_crossbow_bolts(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(788));

	/* the first hero gets wounded with 2W6+6 */
	sub_hero_le(get_first_hero_available_in_group(), dice_roll(2, 6, 6));
}

void chest_cursed(void)
{
	/* cursed chest on the Totenschiff. 50D, but each good attribute of the group leader is decreased by 1.
	 * can be cured by 'Verwandlung beenden' spell or a Praios/Hesinde miracle */
	signed int i;
	struct struct_hero *hero = get_first_hero_available_in_group();

	if (!hero->flags.transformed) {

		hero->flags.transformed = 1;

		/* decrement each good attribute */
		for (i = 0; i <= 6; i++) {
			hero->attrib[i].current--;
		}

		/* print a message */
		print_msg_with_first_hero(get_ttx(563));
	}
}

void chest_fulminictus(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(564));

	/* the first hero gets wounded with 4W6+5 */
	sub_hero_le(get_first_hero_available_in_group(), dice_roll(4, 6, 5));
}


/**
 * \brief   loot a chest, with standard text messages
 *
 * \param   chest       pointer to the chest
 */
void loot_simple_chest(struct struct_chest *chest)
{
	signed int item_num;
	signed int item_id;
	signed int tw_bak;
	char names[20][30];

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	chest->key = 0;

	do {

		item_num = 0;

		/* write the names of the items in the chest into names[] */
		while ((item_id = chest->content[item_num]) != 0xff) {

			strcpy(names[item_num++], GUI_name_plural(0, g_itemsname[item_id]));
		}

		if (item_num == 0) {

			/* this chest is empty */
			GUI_output(get_ttx(522));
			break;
		} else {

			/* show radio menu with item names */
			item_num = GUI_radio(get_ttx(521), (signed char)item_num,
						names[0], names[1], names[2], names[3],
						names[4], names[5], names[6], names[7],
						names[8], names[9], names[10], names[11],
						names[12], names[13], names[14], names[15],
						names[16], names[17], names[18], names[19]) - 1;

			if (item_num != -2) {

				/* if not pressed ESC */
				if (give_new_item_to_group(chest->content[item_num], 1, 1)) {

					/* got the item in inventory => remove from chest */
					delete_chest_item(chest, item_num);
				} else {
					/* group has not taken the item */
					item_num = -2;
				}
			}
		}

	} while (item_num != -2);

	g_textbox_width = tw_bak;
}

/**
 * \brief   deletes an item from a chest
 *
 * \param   chest       pointer to the chest
 * \param   item_num    the number of the item to be deleted
 */
void delete_chest_item(struct struct_chest *chest, signed int item_num)
{
	signed char tmp;

	do {
		chest->content[item_num] = tmp = chest->content[item_num + 1];

		item_num++;

	} while (tmp != -1);
}

/**
 * \brief   loot a chest, with different text messages
 *
 * \param   chest       pointer to the chest
 * \param   text_non_empty shown text if chest is not empty
 * \param   text_empty  shown text if chest is empty
 */
void loot_chest(struct struct_chest *chest, char *text_non_empty, char *text_empty)
{
	signed int pos;
	signed int item_id;
	signed int tw_bak;
	char names[20][20];

	chest->key = 0;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	do {
		pos = 0;

		/* write the names of the items in the chest into names[] */
		while ((item_id = chest->content[pos]) != 0xff) {

			strcpy(names[pos++], GUI_name_plural(0, g_itemsname[item_id]));
		}

		if (pos == 0) {
			/* this chest is empty */
			GUI_output(text_empty);
			break;
		} else {

			/* show radio menu with item names */
			pos = GUI_radio(text_non_empty, (signed char)pos,
						names[0], names[1], names[2], names[3],
						names[4], names[5], names[6], names[7],
						names[8], names[9], names[10], names[11],
						names[12], names[13], names[14], names[15],
						names[16], names[17], names[18], names[19]) - 1;

			if (pos != -2) {

				/* if not pressed ESC */
				if (give_new_item_to_group(chest->content[pos], 1, 1)) {

					/* got the item in inventory => remove from chest */
					delete_chest_item(chest, pos);
				} else {
					/* group has not taken the item */
					pos = -2;
				}
			}
		}

	} while (pos != -2);

	g_textbox_width = tw_bak;
}

/**
 * \brief   check if a hero has lockpicks
 *
 * \param   hero        pointer to the hero
 * \return              -1 = no lockpicks, -2 = all lockpicks are broken, else position of the lockpicks
 */
signed int hero_has_lockpicks(const struct struct_hero *hero)
{
	signed int retval = -1;
	signed int inv_slot;

	/* in each inventory slot... */
	for (inv_slot = 0; inv_slot < NR_HERO_INVENTORY_SLOTS; inv_slot++) {

		/* ... check for lockpicks ... */
		if (hero->inventory[inv_slot].item_id == ITEM_DIETRICHE) {

			/* ... which are not broken */
			if (!hero->inventory[inv_slot].flags.broken) {
				return inv_slot;
			} else {
				retval = -2;
			}
		}
	}

	return retval;
}

/*
 * \brief handle special chests
 * \param[in] check_dir {0 = no, 1 = yes}
 **/
void loot_special_chest(const signed int check_dir)
{
	signed int x;
	signed int y;
	signed int pos;
	signed int mapflag;
	struct struct_chest *chest_ptr = g_dng_specialchest_index[gs_dungeon_id - 1];
	uint8_t *ptr = g_dng_map;

	g_get_extra_loot = 0;
	x = gs_x_target;
	y = gs_y_target;

	if (check_dir != 0) {
		switch (gs_direction) {
			case NORTH: y--; break;
			case EAST: x++; break;
			case SOUTH: y++; break;
			case WEST: x--; break;
		}
	}

	mapflag = *(ptr + MAP_POS(x,y)) & 0x02;
	pos = DNG_POS(gs_dungeon_level, x, y);

	play_voc(ARCHIVE_FILE_FX13_VOC);

	do {

		if (chest_ptr->pos == pos) {

			if (mapflag != 0 && chest_ptr->loot) {

				chest_ptr->loot(chest_ptr);

			} else if (chest_ptr->mod) {

				chest_ptr->open(chest_ptr);

			} else if (chest_ptr->open) {

				chest_ptr->open(chest_ptr);

			} else if (chest_ptr->loot) {

				chest_ptr->loot(chest_ptr);
				g_get_extra_loot = 1;

			} else if (chest_ptr->money) {

				g_get_extra_loot = 1;
			}

			break;
		}

	} while ((chest_ptr++)->pos != -1);

	if ((mapflag == 0) && g_get_extra_loot) {

		if (chest_ptr->ap) {

			/* There are AP in the chest */
			add_hero_ap_all(chest_ptr->ap);
		}

		if (chest_ptr->money) {

			/* There is money in the chest */
			make_valuta_str(g_text_output_buf, chest_ptr->money);

			sprintf(g_dtp2, get_ttx(793), g_text_output_buf);
			GUI_output(g_dtp2);

			set_party_money(get_party_money() + chest_ptr->money);
		}

		if (chest_ptr->food) {
			/* There are FOOD PACKAGES in the chest */
			give_new_item_to_group(ITEM_PROVIANTPAKET, 1, chest_ptr->food);
		}
	}
}

void use_lockpicks_on_chest(struct struct_chest* chest_ptr)
{
	signed int inv_slot;
	signed int test_val;
	struct struct_hero *hero = get_first_hero_available_in_group();

	if ((inv_slot = hero_has_lockpicks(hero)) != -1) {

		if (inv_slot != -2) {

			test_val = test_skill(hero, TA_SCHLOESSER, chest_ptr->mod);

			if (test_val == -99) {

				/* unlucky, your lockpicks break... */

				print_msg_with_first_hero(get_ttx(533));
				hero->inventory[inv_slot].flags.broken = 1;

				/* ... and you trigger the trap */
				if (chest_ptr->trap) {
					chest_ptr->trap();
				}

			} else if (test_val <= 0) {

				/* trigger the trap */
				if (chest_ptr->trap) {
					chest_ptr->trap();
				}

			} else {
				/* success */

				add_hero_ap(hero, 1);


				if (chest_ptr->loot) {

					chest_ptr->loot(chest_ptr);

					if (chest_ptr->trap == chest_protected_heavy) {
						add_hero_ap(hero, 5);
					}
				}

				g_get_extra_loot = 1;
			}

		} else {
			print_msg_with_first_hero(get_ttx(531));
		}
	} else {
		print_msg_with_first_hero(get_ttx(530));
	}
}

void use_key_on_chest(struct struct_chest* chest)
{
	signed int key_pos;
	struct struct_hero *hero = get_first_hero_available_in_group();

	/* the leader of the group must have the key */
	if ((key_pos = inv_slot_of_item(hero, chest->key)) != -1) {

		if (!hero->inventory[key_pos].flags.broken) {

			chest->loot(chest);

			g_get_extra_loot = 1;
		}

	} else {

		chest->trap();
	}
}

/**
 * \brief loot a chest with stacked items (item_id, quantity)
 * \param content pointer to content, format: see below.
 * \param msg header of the radio box
 *
 * \note: These type of chests have a content of the following format:
 *  ( (item_id_1, quantity_1), (item_id_2, quantity_2), ... , (item_id_n, quantity_n), 0xff).
 *  These informations are stored in an array of type uint8_t[2*n+1]
 *  and are contained in the game state.
 */
void loot_multi_chest(uint8_t *content, char *msg)
{
	unsigned int quantity;
	signed int pos;
	signed int i;
	signed int tw_bak;
	char temp_str[10];
	signed int len;
	char names[20][25];

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	do {
		pos = 0;
		while ((i = content[pos + pos]) != 0xff) {

			names[pos][0] = '\0';

			if ((quantity = content[pos + pos + 1]) > 1)
			{
				my_itoa(quantity, names[pos], 10);

				strcat(names[pos], g_str_single_space);
			}

			strcat(names[pos++], GUI_name_plural( ((int16_t)(quantity > 1 ? (uint16_t)1 : (uint16_t)0)) ? 4 : 0, g_itemsname[i]));
		}

		if (pos != 0) {

			pos = GUI_radio(msg, (signed char)pos,
				names[0], names[1], names[2], names[3],
				names[4], names[5], names[6], names[7],
				names[8], names[9], names[10], names[11],
				names[12], names[13], names[14], names[15],
				names[16], names[17], names[18], names[19]) - 1;

			if (pos != -2) {

				pos += pos;

				my_itoa(content[pos + 1], temp_str, 10);

				len = strlen(temp_str);

				do {
					i = (quantity = content[pos + 1]) > 1 ? GUI_input(get_ttx(593), len) : quantity;

				} while (i < 0);

				if (i > quantity) {
					i = quantity;
				}

				if (i != 0) {

					if (content[pos] == ITEM_DUKATEN) {

						add_party_money(i * 100L);

					} else {
						i = give_new_item_to_group(content[pos], 1, i);
					}

					if (i == quantity) {
						// full quantity of item taken away.
						// To fill the gap, move all subseqent list entries up by one position.

						do {
							content[pos] = (unsigned char)(i = content[pos + 2]);
							content[pos + 1] = content[pos + 3];
							pos += 2;

						} while (i != 0xff);

					} else if (i != 0) {
						content[pos + 1] -= i;
					} else {
						pos = -2;
					}
				}
			}


		} else {
			pos = -2;
		}

	} while (pos != -2);

	g_textbox_width = tw_bak;
}

#if !defined(__BORLANDC__)
}
#endif
