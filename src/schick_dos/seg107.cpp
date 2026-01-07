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

static void (*g_use_item_handlers[14])(void) = {
	NULL,                               // USABLE_ITEM_STATS_ID_NONE                  ==  0
	item_invoke_arcano_psychostabilils, // USABLE_ITEM_STATS_ID_ARCANO_PSYCHOSTABILIS ==  1
	item_read_recipe,                   // USABLE_ITEM_STATS_ID_RECIPE                ==  2
	item_read_document,                 // USABLE_ITEM_STATS_ID_DOCUMENT              ==  3
	item_invoke_armatrutz,              // USABLE_ITEM_STATS_ID_ARMATRUTZ             ==  4
	item_invoke_flim_flam,              // USABLE_ITEM_STATS_ID_FLIM_FLAM             ==  5
	item_read_schuldbuch,               // USABLE_ITEM_STATS_ID_SCHULDBUCH            ==  6
	item_read_orkdokument,              // USABLE_ITEM_STATS_ID_ORKDOKUMENT           ==  7
	item_apply_weapon_poison,           // USABLE_ITEM_STATS_ID_WEAPON_POISON         ==  8
	item_use_miasthmaticum,             // USABLE_ITEM_STATS_ID_MIASTHMATICUM         ==  9
	item_use_hylailer_feuer,            // USABLE_ITEM_STATS_ID_HYLAILER_FEUER        == 10
	item_read_spellbook_heptagon,       // USABLE_ITEM_STATS_ID_SPELLBOOK_HEPTAGON    == 11
	item_ignite,                        // USABLE_ITEM_STATS_ID_LIGHTING              == 12
	item_use_beutel                     // USABLE_ITEM_STATS_ID_MAGISCHER_BEUTEL      == 13
}; // ds:0xaeb0

int g_ignite_mode = IGNITE_MODE_SPELL_OR_USE_TINDER; // ds:0xaee8, 0 = spell or use tinder, 1 = use torch, 2 = use lantern

static struct item_stats *g_used_item_desc;	// ds:0xe5c6, pointer to the item description
static signed int g_used_item_id;	// ds:0xe5ca, used_item ID

#ifndef M302de_ORIGINAL_BUGFIX
static signed int g_used_item_inv_slot;	// ds:0xe5cc, used_item position
#else
// required for fix of Original-Bug 55
signed int g_used_item_inv_slot;	// ds:0xe5cc, used_item position
#endif

static struct struct_hero *g_itemuser;	// ds:0xe5ce, pointer to hero

/* Borlandified and identical */
void use_item(const signed int inv_slot, const signed int hero_pos)
{
	void (*func)(void);

	/* set global variables for item usage */
	g_used_item_inv_slot = inv_slot;

	g_itemuser = get_hero(hero_pos);

	g_used_item_id = g_itemuser->inventory[g_used_item_inv_slot].item_id;

	g_used_item_desc = &g_itemsdat[g_used_item_id];

	if (check_hero(g_itemuser)) {

			if (!g_used_item_desc->flags.usable) {

				/* item is not usable */

				if (is_in_int_array(g_used_item_id, g_items_pluralwords))
				{
					/* german grammar, singular and plural are the same */
					sprintf(g_dtp2, get_ttx(792), GUI_name_base_form(g_itemsname[g_used_item_id]));
				} else {
					sprintf(g_dtp2, get_ttx(571), GUI_name_inflect_with_article(
						INFLECT_INDEFINITE_ARTICLE | INFLECT_SINGULAR | INFLECT_1ST_CASE,
						g_used_item_id,
						INFLECT_NAME_TYPE_ITEM
					));
				}

				GUI_output(g_dtp2);

			} else if (g_used_item_desc->flags.herb_potion && !is_in_int_array(g_used_item_id, g_weapon_poisons)) {

				/* don't consume poison */
				consume(g_itemuser, g_itemuser, inv_slot);

			} else if (g_itemuser->inventory[g_used_item_inv_slot].quantity <= 0) {

				/* magic item is used up */
				GUI_output(get_ttx(638));
			} else {
				/* usable item, but not poison, herb, or potion */
				func = g_use_item_handlers[g_usable_items_table[g_used_item_desc->item_type_stats_id].handler_id];
				func();
			}
	}
}

/* Borlandified and identical */
void item_invoke_arcano_psychostabilils(void)
{
	/* RING ID 165 */

	/* save index of TX_FILE_INDEX */
	const signed int tx_index_bak = g_tx_file_index;

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	g_spelluser = g_itemuser;

	/* ask who should be affected */
	g_spelluser->target_object_id = select_hero_from_group(get_ttx(637)) + 1;

	if (g_spelluser->target_object_id > 0) {

		/* use it */
		spell_arcano();

		/* decrement usage counter */
		g_itemuser->inventory[g_used_item_inv_slot].quantity--;
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

	switch (g_used_item_id) {
		case ITEM_ID_REZEPT_FUER_EXPURGICUM: str = get_ttx(639); break;
		case ITEM_ID_REZEPT_FUER_VOMICUM: str = get_ttx(640); break;
		case ITEM_ID_REZEPT_FUER_GEGENGIFT: str = get_ttx(649); break;
		case ITEM_ID_REZEPT_FUER_HYLAILER_FEUER: str = get_ttx(650); break;
		case ITEM_ID_REZEPT_FUER_KRAFTELIXIER: str = get_ttx(651); break;
		case ITEM_ID_REZEPT_FUER_MUTELIXIER: str = get_ttx(652); break;
		case ITEM_ID_REZEPT_FUER_ZAUBERTRANK: str = get_ttx(653); break;
		case ITEM_ID_REZEPT_FUER_HEILTRANK: str = get_ttx(681); break;
		case ITEM_ID_REZEPT_FUER_STARKEN_HEILTRANK: str = get_ttx(682); break;
		case ITEM_ID_REZEPT_FUER_SCHLAFTRUNK: str = get_ttx(684); break;
		case ITEM_ID_REZEPT_FUER_STARKER_ZAUBERTRANK: str = get_ttx(685); break;
		case ITEM_ID_REZEPT_FUER_MIASTHMATICUM: str = get_ttx(686); break;
		case ITEM_ID_REZEPT_FUER_WUNDERKUR: str = get_ttx(683); break;
	}

	/* prepare message */
	sprintf(g_dtp2, get_ttx(636), str);

	GUI_output(g_dtp2);
}

/* Borlandified and identical */
void item_read_document(void)
{
	char *str;
	signed int tw_bak;

	switch (g_used_item_id) {

		case ITEM_ID_DOKUMENT__UNICORN: str = get_ttx(641); break;
		case ITEM_ID_SCHREIBEN_VON_JADRA: str = get_ttx(645); break;
		case ITEM_ID_LOBPREISUNGEN: str = get_ttx(646); break;
		case ITEM_ID_MITGLIEDERLISTE: str = get_ttx(647); break;
		case ITEM_ID_SEEKARTE: str = get_ttx(648); break;
		case ITEM_ID_BUCH__PIRATE_ACCOUNTING: str = get_ttx(654); break;
		case ITEM_ID_BUCH__KAISERSPRUECHE_HALS: str = get_ttx(655); break;
		case ITEM_ID_EMPFEHLUNGSSCHREIBEN__HETMAN: str = get_ttx(657); break;
		case ITEM_ID_EMPFEHLUNGSSCHREIBEN__SIEBENSTEIN: str = get_ttx(759); break;
	}

	tw_bak = g_textbox_width;
	g_textbox_width = 7;
	GUI_output(str);
	g_textbox_width = tw_bak;
}

/* Borlandified and identical */
void item_invoke_armatrutz(void)
{
	/* ID 171 = ITEM_ID_SILBERNER_STIRNREIF__1, 245 = ITEM_ID_SILBERNER_STIRNREIF__2 */

	const signed int tx_index_bak = g_tx_file_index;

	/* load SPELLTXT */
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	g_spelluser = g_itemuser;

	/* ask who should be affected */
	g_spelluser->target_object_id = select_hero_from_group(get_ttx(637)) + 1;

	if (get_spelluser()->target_object_id > 0) {
		/* use it */
		spell_armatrutz();
		/* decrement usage counter */
		g_itemuser->inventory[g_used_item_inv_slot].quantity--;

		GUI_output(g_dtp2);
	}

	if ((tx_index_bak != -1) && (tx_index_bak != 0xde)) {
		/* need to reload buffer1 */
		load_tx(tx_index_bak);
	}
}

/* Borlandified and identical */
void item_invoke_flim_flam(void)
{
	/* ID 174 = ITEM_ID_AMULETT__FLIM_FLAM */
	const signed int tx_index_bak = g_tx_file_index;

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	g_spelluser = g_itemuser;

	spell_flim_flam();

	/* decrement usage counter */
	g_itemuser->inventory[g_used_item_inv_slot].quantity--;

	if ((tx_index_bak != -1) && (tx_index_bak != 0xde)) {

		/* need to reload buffer1 */
		load_tx(tx_index_bak);
	}

	GUI_output(g_dtp2);
}

/* Borlandified and identical */
void item_read_schuldbuch(void)
{
	/* ITEM_ID_SCHULDBUCH, ID 176 */

	if (gs_debtbook_read_flag) {

		/* mark this event (1 = unread, 0 = read) */
		gs_debtbook_read_flag = 0;

		/* 15 AP */
		add_hero_ap_all(15);

		/* mark informer Hjore as known */
		update_informer_cond(INFORMER_ID_HJORE);
	}

	GUI_output(get_ttx(642));
}

/* Borlandified and identical */
void item_read_orkdokument(void)
{
	/* ITEM_ID_ORKDOKUMENT, ID 179 */

	/* Languages + 4, or already read successful */
	if ((test_talent(g_itemuser, TA_SPRACHEN, 4) > 0) || gs_orcdocument_read_flag) {

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
void item_apply_weapon_poison(void)
{
	/*	WEAPON POISON, EXPURGICUM, VOMICUM
		ID 55-59, 141-144, 166, 168 */

	signed int bottle_id;

	if ((g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_ID_NONE) &&
		(g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_ID_KURZBOGEN) &&
		(g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_ID_LANGBOGEN) &&
		(g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id != ITEM_ID_ARMBRUST))
		/* TODO: potential Original-Bug: What about sling? */
	{

		switch (g_used_item_id) {
		case ITEM_ID_VOMICUM : {
			/* VOMICUM */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.poison_vomicum = 1;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_VOMICUM), 1);

			bottle_id = ITEM_ID_BRONZEFLASCHE;
			break;
		}
		case ITEM_ID_EXPURGICUM : {
			/* EXPURGICUM */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.poison_expurgicum = 1;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_EXPURGICUM), 1);

			bottle_id = ITEM_ID_BRONZEFLASCHE;
			break;
		}
		case ITEM_ID_SHURINKNOLLEKNOLLENGIFT: {
			/* SHURIN-BULB POISON / SHURINKNOLLENGIFT */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_SHURINKNOLLENGIFT;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_SHURINKNOLLEKNOLLENGIFT), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		case ITEM_ID_ARAXGIFT: {
			/* ARAX POISON / ARAXGIFT */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_ARAX;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_ARAXGIFT), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		case ITEM_ID_ANGSTGIFT: {
			/* FEAR POISON / ANGSTGIFT */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_ANGSTGIFT;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_ANGSTGIFT), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		case ITEM_ID_SCHLAFGIFT: {
			/* SLEPPING POISON / SCHALFGIFT */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_SCHLAFGIFT;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_SCHLAFGIFT), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		case ITEM_ID_GOLDLEIM: {
			/* GOLDEN GLUE / GOLDLEIM */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_GOLDLEIM;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_GOLDLEIM), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		case ITEM_ID_LOTUSGIFT: {
			/* LOTUS POISON / LOTUSGIFT */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_LOTUSGIFT;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_LOTUSGIFT), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		case ITEM_ID_KUKRIS: {
			/* KUKRIS */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_KUKRIS;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_KUKRIS), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		case ITEM_ID_BANNSTAUB: {
			/* BANE DUST / BANNSTAUB */
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_BANNSTAUB;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_BANNSTAUB), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		case ITEM_ID_KROETENSCHEMELGIFT: {
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_id = POISON_ID_KROETENSCHEMEL;
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].num_poison_charges = 5;

			drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_KROETENSCHEMELGIFT), 1);

			bottle_id = ITEM_ID_GLASFLASCHE;
			break;
		}
		}

		give_new_item_to_hero(g_itemuser, bottle_id, 1, 1);

		sprintf(g_dtp2, get_ttx(739), (char*)GUI_name_inflect_with_article(
			INFLECT_DEFINITE_ARTICLE | INFLECT_SINGULAR | INFLECT_1ST_CASE,
			g_itemuser->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id, INFLECT_NAME_TYPE_ITEM
		));

	} else {
		sprintf(g_dtp2, get_ttx(805), g_itemuser->alias);
	}

	GUI_output(g_dtp2);
}

void item_use_miasthmaticum(void)
{
	/* ITEM_ID_MIASTHMATICUM, ID 238 */

	if (!g_in_fight) {
		GUI_output(get_ttx(687));
		return;
	}
}

void item_use_hylailer_feuer(void)
{
	/* ITEM_ID_HYLAILER_FEUER, ID 239 */

	if (!g_in_fight) {
		GUI_output(get_ttx(687));
		return;
	}
}

void item_read_spellbook_heptagon(void)
{
	/* ITEM_ID_BUCH__HEPTAGON, ID 246 */

	/* print message */
	GUI_output(get_ttx(749));

	/* Heptagon +2 */
	g_itemuser->spells[SP_HEPTAGON_UND_KROETENEI] += 2;

	/* drop the book */
	drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_BUCH__HEPTAGON), 1);
}

/* Borlandified and identical */
void item_ignite(void)
{
	/* ITEM_ID_LATERNE__UNLIT, ITEM_ID_FACKEL__UNLIT, ITEM_ID_ZUNDERKAESTCHEN, ITEM_ID_LATERNE__LIT; ID 25, 65, 85, 249 */
	signed int tx_index_bak = g_tx_file_index;
	signed int inv_slot;
	signed int refill_inv_slot;

	/* load SPELLTXT*/
	load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

	if (g_used_item_id == ITEM_ID_LATERNE__LIT) {

		/* refill burning lantern */

#ifdef M302de_ORIGINAL_BUGFIX
		if (get_spelluser() != g_itemuser) {
			g_spelluser = g_itemuser;
		}
#endif

		/* look for oil at the spelluser() */
		inv_slot = inv_slot_of_item(get_spelluser(), ITEM_ID_OEL);

		if (inv_slot != -1) {
			/* look for the burning lantern at the spelluser() ??? */
			refill_inv_slot = inv_slot_of_item(get_spelluser(), ITEM_ID_LATERNE__LIT);

			/* reset the burning time of the lantern */
			g_itemuser->inventory[refill_inv_slot].lighting_timer = 100;

			/* drop the oil */
			drop_item(g_itemuser, inv_slot, 1);

			/* give a bronze flask */
			give_new_item_to_hero(g_itemuser, ITEM_ID_BRONZEFLASCHE, 0, 1);

			sprintf(g_dtp2, get_tx(119), g_itemuser->alias);
			/* "hero ignites the lantern" */
		} else {
			sprintf(g_dtp2, get_tx(120), g_itemuser->alias);
			/* "hero does not have oil" */
		}
	} else {
		/* In all other cases, ITEM_ID_ZUNDERKAESTCHEN is needed */

		if (inv_slot_of_item(g_itemuser, ITEM_ID_ZUNDERKAESTCHEN) == -1) {
			sprintf(g_dtp2, get_tx(122), g_itemuser->alias);
			/* "hero does not have tinder" */
		} else {

			if (g_used_item_id == ITEM_ID_FACKEL__UNLIT) {

				g_ignite_mode = IGNITE_MODE_USE_TORCH;

			} else if (g_used_item_id == ITEM_ID_LATERNE__UNLIT) {

				g_ignite_mode = IGNITE_MODE_USE_LANTERN;

			} else {
				// assert(g_used_item_id == ITEM_ID_ZUNDERKAESTCHEN)
				g_ignite_mode = IGNITE_MODE_SPELL_OR_USE_TINDER;
			}

			g_spelluser = g_itemuser;

			spell_brenne();
		}
	}

#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 54:
	 * Use a torch to ignite it. After that, it is not possible to ignite a
	 * lantern with the spell "Brenne toter Stoff".
         * (For exampe, if the hero casting that spell has a a torch, but not a
         * lantern, there will be the message "<hero> hat weder eine Fackel noch
         * eine Laterne dabei".)
         * The bug exists also the other way round: Use a lantern to ignite it.
         * After that, it is not possible to ignite a torch with the spell "Brenne
         * toter Stoff".

	 * Use a torch to ignite it. After that, it is not possible to ignite a lantern with the spell "Brenne toter Stoff".
	 * Or the other way round: Use a lantern to ignite it. After that, it is not possible to ignite a torch with the spell "Brenne toter Stoff".
	 */

	// Fix: reset g_ignite_mode.
	 g_ignite_mode = IGNITE_MODE_SPELL_OR_USE_TINDER;
#endif

	if ((tx_index_bak != -1) && (tx_index_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {
		/* need to reload buffer1 */
		load_tx(tx_index_bak);
	}

	GUI_output(g_dtp2);
}

/* Borlandified and identical */
void item_use_beutel(void)
{
	/* ITEM_ID_MAGISCHER_BROTBEUTEL, ITEM_ID_BEUTEL; ID 184, 221 */

#ifndef M302de_FEATURE_MOD
	if ((gs_dungeon_id == DUNGEON_ID_RUINE_DES_SCHWARZMAGIERS) && (gs_dungeon_level == 0)) {

		/* set ptr to the map */
		uint8_t *ptr = g_dng_map;

		/* remove the wall there */
		ptr[MAP_POS(10,3)] = DNG_TILE_CORRIDOR + 0x01; /* set flag 0, is there a reason? */
	}

	GUI_output(get_ttx(775));
	/* bag releases a dust cloud, which forms a frame ... */

	/* drop the BAG */
	drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_BEUTEL), 1);
	/* Beware: The bag is dropped wether or not the wall was removed!
	 * Hence, using it at the wrong place will leave the dungeon "Ruine des Schwarzmagiers"
	 * in a state which cannot be accessed deeper by regular means. */
#else
	/* Feature mod 9:
	 * Avoid the loss of the magic bag if there was no effect,
	 * i.e. when it was used outside the first level of Ruine des Schwarzmagiers.
	 * Moreover, give a better message in this case. */
	if ((gs_dungeon_id == DUNGEON_ID_RUINE_DES_SCHWARZMAGIERS) && (gs_dungeon_level == 0)) {

		/* set ptr to the map */
		uint8_t *ptr = g_dng_map;

		/* remove the wall there */
		ptr[MAP_POS(10,3)] = DNG_TILE_CORRIDOR + 0x01; /* set flag 0, is there a reason? */

		GUI_output(get_ttx(775));
		/* bag releases a dust cloud, which forms a frame ... */

		drop_item(g_itemuser, inv_slot_of_item(g_itemuser, ITEM_ID_BEUTEL), 1);
	} else {
		sprintf(g_dtp2, "DER BEUTEL ENTH\x8e""LT ETWAS STAUB.\x40""%s WARTET EIN WENIG, DOCH NICHTS PASSIERT.",g_itemuser->alias);
		GUI_output(g_dtp2);
	}
#endif
}
