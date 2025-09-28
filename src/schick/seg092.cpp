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
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
#include "t_map.h"
#endif

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void chest_protected_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(518));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le((Bit8u*)get_first_hero_available_in_group(), random_schick(6));
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
	sub_hero_le((Bit8u*)get_first_hero_available_in_group(), dice_roll(2, 6, 0));
}

void chest_poisoned1(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le((Bit8u*)(gs_main_acting_hero = get_first_hero_available_in_group()), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned((Bit8u*)gs_main_acting_hero, 1);

#if !defined(__BORLANDC__)
	gs_main_acting_hero = NULL;
#endif
}

void chest_poisoned2(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le((Bit8u*)(gs_main_acting_hero = get_first_hero_available_in_group()), dice_roll(2, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned((Bit8u*)gs_main_acting_hero, 2);

#if !defined(__BORLANDC__)
	gs_main_acting_hero = NULL;
#endif
}

void chest_poisoned3(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 1W6 */
	sub_hero_le((Bit8u*)(gs_main_acting_hero = get_first_hero_available_in_group()), dice_roll(1, 6, 0));

	/* and gets poisoned */
	hero_gets_poisoned((Bit8u*)gs_main_acting_hero, 8);

#if !defined(__BORLANDC__)
	gs_main_acting_hero = NULL;
#endif
}

void chest_protected_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(520));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le((Bit8u*)get_first_hero_available_in_group(), dice_roll(4, 6, 0));
}

void chest_petrified(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(776));

	/* save pointer of the first hero */
	gs_main_acting_hero = get_first_hero_available_in_group();

	/* and make him petrified */
	or_ptr_bs((Bit8u*)gs_main_acting_hero + HERO_FLAGS1, 0x04); /* set 'petrified' flag */

#if !defined(__BORLANDC__)
	gs_main_acting_hero = NULL;
#endif
}

void chest_ignifax_normal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 2W6 */
	sub_hero_le((Bit8u*)get_first_hero_available_in_group(), dice_roll(2, 6, 0));
}

void chest_ignifax_brutal(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 4W6 */
	sub_hero_le((Bit8u*)get_first_hero_available_in_group(), dice_roll(4, 6, 0));
}

void chest_ignifax_heavy(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(777));

	/* the first hero gets wounded with 3W6 */
	sub_hero_le((Bit8u*)get_first_hero_available_in_group(), dice_roll(3, 6, 0));
}

void chest_crossbow_bolts(void)
{
	/* a protected chest */
	print_msg_with_first_hero(get_ttx(788));

	/* the first hero gets wounded with 2W6+6 */
	sub_hero_le((Bit8u*)get_first_hero_available_in_group(), dice_roll(2, 6, 6));
}

void chest_cursed(void)
{
	/* cursed chest on the Totenschiff. 50D, but each good attribute of the group leader is decreased by 1.
	 * can be cured by 'Verwandlung beenden' spell or a Praios/Hesinde miracle */
	signed short i;
	Bit8u *hero;

	/* get the group leader */
	hero = (Bit8u*)get_first_hero_available_in_group();

	if (!hero_transformed(hero)) {

		or_ptr_bs(hero + HERO_FLAGS2, 0x40); /* set 'transformed' flag */

		/* decrement each good attribute */
		for (i = 0; i <= 6; i++) {
			dec_ptr_bs(hero + HERO_ATTRIB + 3 * i);
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
	sub_hero_le((Bit8u*)get_first_hero_available_in_group(), dice_roll(4, 6, 5));
}


/**
 * \brief   loot a chest, with standard text messages
 *
 * \param   chest       pointer to the chest
 */
void loot_simple_chest(struct struct_chest *chest)
{
	signed short item_no;
	signed short item_id;
	signed short tw_bak;
	char names[20][30];

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	((struct struct_chest*)chest)->key = 0;

	do {

		item_no = 0;

		/* write the names of the items in the chest into names[] */
		while((item_id = ((struct struct_chest*)chest)->content[item_no]) != 0xff) {

			strcpy(names[item_no++], GUI_name_plural(0, get_itemname(item_id)));
		}

		if (item_no == 0) {

			/* this chest is empty */
			GUI_output(get_ttx(522));
			break;
		} else {

			/* show radio menu with item names */
			item_no = GUI_radio(get_ttx(521), (signed char)item_no,
						names[0], names[1], names[2], names[3],
						names[4], names[5], names[6], names[7],
						names[8], names[9], names[10], names[11],
						names[12], names[13], names[14], names[15],
						names[16], names[17], names[18], names[19]) - 1;

			if (item_no != -2) {

				/* if not pressed ESC */
				if (get_item(((struct struct_chest*)chest)->content[item_no], 1, 1)) {

					/* got the item in inventory => remove from chest */
					delete_chest_item((struct struct_chest*)chest, item_no);
				} else {
					/* group has not taken the item */
					item_no = -2;
				}
			}
		}

	} while (item_no != -2);

	g_textbox_width = tw_bak;
}

/**
 * \brief   deletes an item from a chest
 *
 * \param   chest       pointer to the chest
 * \param   item_no     the number of the item to be deleted
 */
void delete_chest_item(struct struct_chest *chest, signed short item_no)
{
	signed char tmp;

	do {
		chest->content[item_no] = tmp = chest->content[item_no + 1];

		item_no++;

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
	signed short item_no;
	signed short item_id;
	signed short tw_bak;
	char names[20][20];

	chest->key = 0;

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	do {
		item_no = 0;

		/* write the names of the items in the chest into names[] */
		while ((item_id = chest->content[item_no]) != 0xff) {

			strcpy(names[item_no++], GUI_name_plural(0, get_itemname(item_id)));
		}

		if (item_no == 0) {
			/* this chest is empty */
			GUI_output(text_empty);
			break;
		} else {

			/* show radio menu with item names */
			item_no = GUI_radio(text_non_empty, (signed char)item_no,
						names[0], names[1], names[2], names[3],
						names[4], names[5], names[6], names[7],
						names[8], names[9], names[10], names[11],
						names[12], names[13], names[14], names[15],
						names[16], names[17], names[18], names[19]) - 1;

			if (item_no != -2) {

				/* if not pressed ESC */
				if (get_item(chest->content[item_no], 1, 1)) {

					/* got the item in inventory => remove from chest */
					delete_chest_item(chest, item_no);
				} else {
					/* group has not taken the item */
					item_no = -2;
				}
			}
		}

	} while (item_no != -2);

	g_textbox_width = tw_bak;
}

/**
 * \brief   check if a hero has lockpicks
 *
 * \param   hero        pointer to the hero
 * \return              -1 = no lockpicks, -2 = all lockpicks are broken, else position of the lockpicks
 */
signed short hero_has_lockpicks(Bit8u *hero)
{
	signed short retval = -1;
	signed short i;

	/* in each inventory slot... */
	for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++) {

		/* ... check for lockpicks ... */
		if (host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * i) == ITEM_PICKLOCKS) {

			/* ... which are not broken */
			if (!inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * i)) {
				return i;
			} else {
				retval = -2;
			}
		}
	}

	return retval;
}

/* handle special chest */
void seg092_06b4(signed short a1)
{
	signed short x;
	signed short y;
	signed short pos;
	signed short l4;
	struct struct_chest *chest_ptr;
	Bit8u *ptr;

	chest_ptr = g_dng_specialchest_index[gs_dungeon_index - 1];
	ptr = g_dng_map;
	g_get_extra_loot = 0;
	x = gs_x_target;
	y = gs_y_target;

	if (a1 != 0) {
		switch (gs_direction) {
			case NORTH: y--; break;
			case EAST: x++; break;
			case SOUTH: y++; break;
			case WEST: x--; break;
		}
	}

	l4 = ptr[MAP_POS(x,y)] & 0x02;
	pos = DNG_POS(gs_dungeon_level, x, y);

	play_voc(ARCHIVE_FILE_FX13_VOC);

	do {

		if (chest_ptr->pos == pos) {

			if (l4 != 0 && chest_ptr->loot) {

				chest_ptr->loot((Bit8u*)chest_ptr);

			} else if (chest_ptr->mod) {

				chest_ptr->open(chest_ptr);

			} else if (chest_ptr->open) {

				chest_ptr->open(chest_ptr);

			} else if (chest_ptr->loot) {

				chest_ptr->loot((Bit8u*)chest_ptr);
				g_get_extra_loot = 1;

			} else if (chest_ptr->money) {

				g_get_extra_loot = 1;
			}

			break;
		}

		chest_ptr++;

	} while (chest_ptr->pos != -1);

	if (l4 == 0 && g_get_extra_loot) {

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
			get_item(ITEM_FOOD_PACKAGE, 1, chest_ptr->food);
		}
	}
}

void use_lockpicks_on_chest(struct struct_chest* chest_ptr)
{
	signed short l_si;
	signed short l_di;
	Bit8u *hero;

	hero = (Bit8u*)get_first_hero_available_in_group();

	if ((l_si = hero_has_lockpicks(hero)) != -1) {

		if (l_si != -2) {

			l_di = test_skill(hero, TA_SCHLOESSER, ((struct struct_chest*)chest_ptr)->mod);

			if (l_di == -99) {

				/* unlucky, your lockpicks break... */

				print_msg_with_first_hero(get_ttx(533));
				or_ptr_bs(hero + (HERO_INVENTORY + INVENTORY_FLAGS) + SIZEOF_INVENTORY * l_si, 1); /* set 'broken' flag */

				/* ... and you trigger the trap */
				if (((struct struct_chest*)chest_ptr)->trap) {
					((struct struct_chest*)chest_ptr)->trap();
				}

			} else if (l_di <= 0) {

				/* trigger the trap */
				if (((struct struct_chest*)chest_ptr)->trap) {
					((struct struct_chest*)chest_ptr)->trap();
				}

			} else {
				/* success */

				add_hero_ap((struct struct_hero*)hero, 1);


				if (((struct struct_chest*)chest_ptr)->loot) {

					((struct struct_chest*)chest_ptr)->loot((Bit8u*)chest_ptr);

					if (((struct struct_chest*)chest_ptr)->trap == chest_protected_heavy) {
						add_hero_ap((struct struct_hero*)hero, 5);
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
	signed short key_pos;
	Bit8u *hero = (Bit8u*)get_first_hero_available_in_group();

	/* the leader of the group must have the key */
	if ((key_pos = get_item_pos(hero, ((struct struct_chest*)chest)->key)) != -1) {

		if (!inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * key_pos)) {

			((struct struct_chest*)chest)->loot((Bit8u*)chest);

			g_get_extra_loot = 1;
		}

	} else {

		((struct struct_chest*)chest)->trap();
	}
}

/**
 * \brief loot a chest with stacked items (item_id, no of items)
 * \param chest pointer to the chest
 * \param msg header of the radio box
 *
 * \note: These type of chests have a content of the following format:
 *  ( (item_1, no_1), (item_2, no_2), ... , (item_n, no_n), 0xff).
 *  These informations are stored in an array of type Bit8u[2*n+1]
 *  and are contained in the game state.
 */
void loot_multi_chest(Bit8u *content, char *msg)
{
	unsigned short item_cnt;
	signed short item_no;
	signed short i;
	signed short tw_bak;
	char temp_str[10];
	signed short len;
	char names[20][25];

	tw_bak = g_textbox_width;
	g_textbox_width = 7;

	do {
		item_no = 0;
		while ((i = content[(item_no + item_no)]) != 0xff) {

			names[item_no][0] = '\0';

			if ((item_cnt = content[item_no + item_no + 1]) > 1)
			{
				my_itoa(item_cnt, names[item_no], 10);

				strcat(names[item_no], g_str_single_space);
			}

			strcat(names[item_no++], GUI_name_plural( ((signed short)(item_cnt > 1 ? (unsigned short)1 : (unsigned short)0)) ? 4 : 0, get_itemname(i)));
		}

		if (item_no != 0) {

			item_no = GUI_radio(msg, (signed char)item_no,
				names[0], names[1], names[2], names[3],
				names[4], names[5], names[6], names[7],
				names[8], names[9], names[10], names[11],
				names[12], names[13], names[14], names[15],
				names[16], names[17], names[18], names[19]) - 1;

			if (item_no != -2) {

				item_no += item_no;

				my_itoa(content[item_no + 1], temp_str, 10);

				len = strlen(temp_str);

				do {
					i = (item_cnt = content[item_no + 1]) > 1 ? GUI_input(get_ttx(593), len) : item_cnt;

				} while (i < 0);

				if (i > item_cnt) {
					i = item_cnt;
				}

				if (i != 0) {

					if (content[item_no] == ITEM_DUCATS) {

						add_party_money(i * 100L);

					} else {
						i = get_item(content[item_no], 1, i);
					}

					if (i == item_cnt) {

						do {
							content[item_no] = (unsigned char)(i = content[item_no + 2]);
							content[item_no + 1] = content[item_no + 3];
							item_no += 2;

						} while (i != 255);

					} else if (i != 0) {
						content[item_no + 1] -= i;
					} else {
						item_no = -2;
					}
				}
			}


		} else {
			item_no = -2;
		}

	} while (item_no != -2);

	g_textbox_width = tw_bak;
}

#if !defined(__BORLANDC__)
}
#endif
