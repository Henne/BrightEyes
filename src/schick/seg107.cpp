/**
 *	Rewrite of DSA1 v3.02_de functions of seg107 (using items)
 *	Functions rewritten: 14/14 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg107.cpp
 */

#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg101.h"
#include "seg103.h"
#include "seg105.h"
#include "seg107.h"
#include "seg108.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static struct item_stats *g_used_item_desc;	// ds:0xe5c6, pointer to the item description
static signed int g_used_item_id;	// ds:0xe5ca, used_item ID
static signed int g_used_item_pos;	// ds:0xe5cc, used_item position

static struct struct_hero *g_itemuser;	// ds:0xe5ce, pointer to hero

#if defined(__BORLANDC__)
#define get_itemuser() (g_itemuser)
#else
static inline struct struct_hero* get_itemuser() { return g_itemuser; }
#endif

/* Borlandified and identical */
void use_item(signed short item_pos, signed short hero_pos)
{
	void (*func)(void);

	/* set global variables for item usage */
	g_used_item_pos = item_pos;

	g_itemuser = (struct struct_hero*)get_hero(hero_pos);

	g_used_item_id = get_itemuser()->inventory[g_used_item_pos].item_id;

	g_used_item_desc = &g_itemsdat[g_used_item_id];

	if (check_hero((Bit8u*)get_itemuser())) {

			if (!g_used_item_desc->flags.useable) {

				/* item is not usable */

				if (is_in_word_array(g_used_item_id, g_items_pluralwords))
				{
					/* german grammar, singular and plural are the same */
					sprintf(g_dtp2, get_ttx(792), GUI_name_singular(get_itemname(g_used_item_id)));
				} else {
					sprintf(g_dtp2, get_ttx(571), GUI_names_grammar(0, g_used_item_id, 0));
				}

				GUI_output(g_dtp2);

			} else if (g_used_item_desc->flags.herb_potion && !is_in_word_array(g_used_item_id, g_poison_potions)) {

				/* don't consume poison */
				consume(get_itemuser(), get_itemuser(), item_pos);

			} else if (get_itemuser()->inventory[g_used_item_pos].quantity <= 0) {

				/* magic item is used up */
				GUI_output(get_ttx(638));
			} else {
				/* special item */
				func = g_use_special_item_handlers[g_specialitems_table[g_used_item_desc->table_index].handler_id];
				func();
			}
	}
}

/* Borlandified and identical */
void item_arcano(void)
{
	/* RING ID 165 */
	signed short tx_index_bak;

	/* save index of TX_FILE_INDEX */
	tx_index_bak = g_tx_file_index;

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	g_spelluser = get_itemuser();

	/* ask who should be affected */
	((struct struct_hero*)get_spelluser())->enemy_id = select_hero_from_group(get_ttx(637)) + 1;

	if (((struct struct_hero*)get_spelluser())->enemy_id > 0) {
		/* use it */
		spell_arcano();
		/* decrement usage counter */
		get_itemuser()->inventory[g_used_item_pos].quantity--;
	}

	if ((tx_index_bak != -1) && (tx_index_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {
		/* need to reload buffer1 */
		load_tx(tx_index_bak);
	}
}

/* Borlandified and identical */
void item_read_recipe(void)
{
	char *str;

	/* TODO: replace magic numbers */
	switch (g_used_item_id) {
	case 0xa7: str = get_ttx(639); break;
	case 0xa9: str = get_ttx(640); break;
	case 0xca: str = get_ttx(649); break;
	case 0xcb: str = get_ttx(650); break;
	case 0xcc: str = get_ttx(651); break;
	case 0xcd: str = get_ttx(652); break;
	case 0xce: str = get_ttx(653); break;
	case 0xba: str = get_ttx(681); break;
	case 0xf0: str = get_ttx(682); break;
	case 0xf2: str = get_ttx(684); break;
	case 0xf3: str = get_ttx(685); break;
	case 0xf4: str = get_ttx(686); break;
	case 0xf1: str = get_ttx(683); break;
	}

	/* prepare message */
	sprintf(g_dtp2, get_ttx(636), str);

	GUI_output(g_dtp2);
}

/* Borlandified and identical */
void item_read_document(void)
{
	char *str;
	signed short tw_bak;

	/* TODO: replace magic numbers */
	switch (g_used_item_id) {
	case 0xaa: str = get_ttx(641); break;
	case 0xbb: str = get_ttx(645); break;
	case 0xbd: str = get_ttx(646); break;
	case 0xbe: str = get_ttx(647); break;
	case 0xc0: str = get_ttx(648); break;
	case 0xde: str = get_ttx(654); break;
	case 0xe0: str = get_ttx(655); break;
	case 0xeb: str = get_ttx(657); break;
	case 0xf7: str = get_ttx(759); break;
	}

	tw_bak = g_textbox_width;
	g_textbox_width = 7;
	GUI_output(str);
	g_textbox_width = tw_bak;
}

/* Borlandified and identical */
void item_armatrutz(void)
{
	/* ID 171 = ITEM_CORONET_SILVER, 245 = ITEM_CORONET_GREEN */
	signed short tx_index_bak;

	/* save index off buffer1 */
	tx_index_bak = g_tx_file_index;

	/* load SPELLTXT */
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	g_spelluser = get_itemuser();

	/* ask who should be affected */
	((struct struct_hero*)get_spelluser())->enemy_id = select_hero_from_group(get_ttx(637)) + 1;

	if (((struct struct_hero*)get_spelluser())->enemy_id > 0) {
		/* use it */
		spell_armatrutz();
		/* decrement usage counter */
		get_itemuser()->inventory[g_used_item_pos].quantity--;

		GUI_output(g_dtp2);
	}

	if ((tx_index_bak != -1) && (tx_index_bak != 0xde)) {
		/* need to reload buffer1 */
		load_tx(tx_index_bak);
	}
}

/* Borlandified and identical */
void item_flimflam(void)
{
	/* ID 174 = ITEM_AMULET_GREEN */
	signed short tx_index_bak;

	/* save index off buffer1 */
	tx_index_bak = g_tx_file_index;

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	g_spelluser = get_itemuser();

	spell_flimflam();

	/* decrement usage counter */
	get_itemuser()->inventory[g_used_item_pos].quantity--;

	if ((tx_index_bak != -1) && (tx_index_bak != 0xde)) {
		/* need to reload buffer1 */
		load_tx(tx_index_bak);
	}

	GUI_output(g_dtp2);
}

/* Borlandified and identical */
void item_debtbook(void)
{
	/* DEBTBOOK, ID 176 */
	if (gs_debtbook_read_flag) {

		/* mark this event (1 = unread, 0 = read) */
		gs_debtbook_read_flag = 0;

		/* 15 AP */
		add_hero_ap_all(15);

		/* mark informer Hjore as known */
		update_informer_cond(INFORMER_HJORE);
	}

	GUI_output(get_ttx(642));
}

/* Borlandified and identical */
void item_orcdocument(void)
{
	/* ORCDOCUMENT, ID 179 */

	/* Languages + 4, or already read successful */
	if ((test_skill(get_itemuser(), TA_SPRACHEN, 4) > 0) || gs_orcdocument_read_flag) {

		if (!gs_orcdocument_read_flag) {

			add_group_ap(20);
			gs_orcdocument_read_flag = 1;
		}

		GUI_output(get_ttx(644));
	} else {
		GUI_output(get_ttx(643));
	}
}

/* Borlandified and identical */
void item_weapon_poison(void)
{
	/*	WEAPON POISON, EXPURGICUM, VOMICUM
		ID 55-59, 141-144, 166, 168 */

	signed short bottle;

	if ((get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_NONE) &&
		(get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_SHORTBOW) &&
		(get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_LONGBOW) &&
		(get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_CROSSBOW))
		/* TODO: potential Original-Bug: What about sling? */
	{

		switch (g_used_item_id) {
		case ITEM_VOMICUM : {
			/* VOMICUM */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.poison_vomicum = 1;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_VOMICUM), 1);

			bottle = ITEM_FLASK_BRONZE;
			break;
		}
		case ITEM_EXPURGICUM : {
			/* EXPURGICUM */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.poison_expurgicum = 1;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_EXPURGICUM), 1);

			bottle = ITEM_FLASK_BRONZE;
			break;
		}
		case ITEM_SHURIN_POISON: {
			/* SHURIN-BULB POISON / SHURINKNOLLENGIFT */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_SHURINKNOLLENGIFT;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_SHURIN_POISON), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_ARAX_POISON: {
			/* ARAX POISON / ARAXGIFT */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_ARAX;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_ARAX_POISON), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_ANGST_POISON: {
			/* FEAR POISON / ANGSTGIFT */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_ANGSTGIFT;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_ANGST_POISON), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_SLEEP_POISON: {
			/* SLEPPING POISON / SCHALFGIFT */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_SCHLAFGIFT;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_SLEEP_POISON), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_GOLDLEIM: {
			/* GOLDEN GLUE / GOLDLEIM */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_GOLDLEIM;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_GOLDLEIM), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_LOTUS_POISON: {
			/* LOTUS POISON / LOTUSGIFT */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_LOTUSGIFT;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_LOTUS_POISON), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_KUKRIS: {
			/* KUKRIS */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_KUKRIS;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_KUKRIS), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_BANNSTAUB: {
			/* BANE DUST / BANNSTAUB */
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_BANNSTAUB;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_BANNSTAUB), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		case ITEM_KROETEN_POISON: {
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_KROETENSCHEMEL;
			get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges = 5;

			drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_KROETEN_POISON), 1);

			bottle = ITEM_FLASK_GLASS;
			break;
		}
		}

		give_hero_new_item(get_itemuser(), bottle, 1, 1);

		sprintf(g_dtp2, get_ttx(739),
			(char*)(GUI_names_grammar((signed short)0x8000, get_itemuser()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id, 0)));

	} else {
		sprintf(g_dtp2, get_ttx(805), get_itemuser()->alias);
	}

	GUI_output(g_dtp2);
}

void item_myastmatic(void)
{
	/* MYASTMATIC, ID 238 */
	if (!g_in_fight) {
		GUI_output(get_ttx(687));
		return;
	}
}

void item_hylailic(void)
{
	/* HYLAILIC FIRE, ID 239 */
	if (!g_in_fight) {
		GUI_output(get_ttx(687));
		return;
	}
}

void item_magic_book(void)
{
	/* BOOK, ID 246 */

	/* print message */
	GUI_output(get_ttx(749));

	/* Heptagon +2 */
	get_itemuser()->spells[SP_HEPTAGON_UND_KROETENEI] += 2;

	/* drop the book */
	drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_BOOK_HEPTAGON), 1);
}

/* Borlandified and identical */
void item_brenne(void)
{
	/*	LANTERN, TORCH, TINDERBOX, LANTERN
		ID 25, 65, 85, 249 */
	signed short tx_index_bak;
	signed short pos;
	signed short refill_pos;

	/* save index off buffer1 */
	tx_index_bak = g_tx_file_index;

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	if (g_used_item_id == ITEM_LANTERN_ON) {
		/* refill burning lantern */

#ifdef M302de_ORIGINAL_BUGFIX
		if ((struct struct_hero*)get_spelluser() != get_itemuser()) {
			g_spelluser = get_itemuser();
		}
#endif

		/* look for oil at the spelluser() */
		pos = get_item_pos((struct struct_hero*)get_spelluser(), ITEM_OIL);

		if (pos != -1) {
			/* look for the burning lantern at the spelluser() ??? */
			refill_pos = get_item_pos((struct struct_hero*)get_spelluser(), ITEM_LANTERN_ON);

			/* reset the burning time of the lantern */
			get_itemuser()->inventory[refill_pos].lighting_timer = 100;

			/* drop the oil */
			drop_item(get_itemuser(), pos, 1);

			/* give a bronze flask */
			give_hero_new_item(get_itemuser(), ITEM_FLASK_BRONZE, 0, 1);

			/* prepare message */
			sprintf(g_dtp2, get_tx(119), get_itemuser()->alias);
		} else {
			/* prepare message */
			sprintf(g_dtp2, get_tx(120), get_itemuser()->alias);
		}
	} else {

		if (get_item_pos(get_itemuser(), ITEM_TINDERBOX) == -1) {
			/* No tinderbox */
			sprintf(g_dtp2, get_tx(122), get_itemuser()->alias);
		} else {

			if (g_used_item_id == ITEM_TORCH_OFF) {

				g_light_type = LIGHTING_TORCH;

			} else if (g_used_item_id == ITEM_LANTERN_OFF) {

				g_light_type = LIGHTING_LANTERN;

			} else {

				g_light_type = LIGHTING_DARK;
			}

			g_spelluser = get_itemuser();

			spell_brenne();
		}
	}

	if ((tx_index_bak != -1) && (tx_index_bak != 0xde)) {
		/* need to reload buffer1 */
		load_tx(tx_index_bak);
	}

	GUI_output(g_dtp2);
}

/* Borlandified and identical */
void item_bag(void)
{
	/* MAGIC BREADBAG, BAG: ID 184, 221 */


	if ((gs_dungeon_index == DUNGEONS_RUINE_DES_SCHWARZMAGIERS) && (gs_dungeon_level == 0)) {

		/* set ptr to the map */
		Bit8u *ptr = g_dng_map;

		/* remove the wall there */
		ptr[MAP_POS(10,3)] = DNG_TILE_CORRIDOR + 0x01; /* set flag 0, is there a reason? */
	}
#if !defined(__BORLANDC__)
	else {
		D1_INFO("WARNUNG:\tDer BEUTEL wurde nicht im ersten Level der Magierruine geoeffnet!\n");
		D1_INFO("\t\tEventuell kann das Spiel nicht mehr erfolgreich beendet werden.\n");
	}
#endif
	/* TODO: avoid dropping the bag when not in the first level of the mage ruin. (via: 'nothing happens') */

	/* print message */
	GUI_output(get_ttx(775));

	/* drop the BAG */
	drop_item(get_itemuser(), get_item_pos(get_itemuser(), ITEM_BAG), 1);
}

#if !defined(__BORLANDC__)
}
#endif
