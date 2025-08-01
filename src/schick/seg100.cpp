/*
 *	Rewrite of DSA1 v3.02_de functions of seg100 (spells 2/3)
 *	Spells: Clairvoyance / Illusion / Combat / Communication
 *	Functions rewritten: 20/20 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg100.cpp
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg039.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg099.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Clairvoyance / Hellsicht */

void spell_eigenschaften(void)
{
	signed short min;
	signed short max;

	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

	damage_range_template(host_readws(get_spelltarget_e() + ENEMY_SHEET_DAM1),
		(Bit8u*)&min, (Bit8u*)&max);

	/* Remark: For unknown reasons the shown TP-values of enemies are scaled down to 80% */
	min = min * 8 / 10;
	max = max * 8 / 10;

	sprintf(g_dtp2, get_tx(25),
          GUI_name_singular(get_monname(host_readbs(get_spelltarget_e()))),
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_LEVEL),	/* Level */
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_AT),	  /* AT */
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_PA),	  /* PA */
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_RS),		/* RS */
          host_readbs(get_spelltarget_e() + ENEMY_SHEET_ATTACKS),	/* Attacks */
          (host_readbs(get_spelltarget_e() + ENEMY_SHEET_ATTACKS) > 1) ? get_tx(26) : get_tx(27),
          min,							/* TPmin */
          max,							/* TPmax */
          host_readws(get_spelltarget_e() + ENEMY_SHEET_LE),	      /* LE */
          host_readws(get_spelltarget_e() + ENEMY_SHEET_LE_ORIG),	  /* LEmax */
          host_readws(get_spelltarget_e() + ENEMY_SHEET_AE),	      /* AE */
          host_readws(get_spelltarget_e() + ENEMY_SHEET_AE_ORIG));	/* AEmax */
}

void spell_exposami(void)
{
	signed short j;
	signed short id;
	signed short changed;
	int arr[20][2];

	signed short i;
	signed short count;

	count = 0;

	for (i = 0; i < ds_readws(NR_OF_ENEMIES); i++) {

		if (host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + SIZEOF_FIGHT_MONSTER * i + FIGHT_MONSTERS_ROUND_APPEAR) != 0) {

			id = host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + SIZEOF_FIGHT_MONSTER * i + FIGHT_MONSTERS_ID);

			changed = 0;

			for (j = 0; j <= count; j++) {
				if (arr[j][0] == id) {
					/* found an entry with this id */
					arr[j][1]++;
					changed = 1;
					break;
				}
			}

			/* create a new entry */
			if (changed == 0) {
				arr[count][0] = id;
				arr[count][1] = 1;
				count++;
			}
		}
	}


	if (count) {
		/* Intro text */
		strcpy(g_dtp2, get_tx(31));

		for (i = 0; count - 1 > i; i++) {
			sprintf(g_text_output_buf, get_tx(33),		/* "%d %s" */
				arr[i][1],
				(char*)(Bit8u*)GUI_names_grammar(((arr[i][1] > 1) ? 4 : 0) + 0x4000, arr[i][0], 1));
			strcat(g_dtp2, g_text_output_buf);

			if (count - 2 > i) {
				strcat(g_dtp2, get_tx(28));		/* "," */
			}
		}

		if (count > 1) {
			strcat(g_dtp2, get_tx(29));		/* "AND" */
		}

		sprintf(g_text_output_buf, get_tx(33),
			arr[count - 1][1],	/* TODO: this field access produces other code */
			(char*)(Bit8u*)GUI_names_grammar((arr[count - 1][1] > 1 ? 4 : 0) + 0x4000,
								arr[count - 1][0], 1));

		strcat(g_dtp2, g_text_output_buf);

		strcat(g_dtp2, get_tx(30));			/* "." */
	} else {
		/* no more hidden enemies */
		strcpy(g_dtp2, get_tx(32));
	}
}

void spell_odem_arcanum(void)
{
	signed short pos;
	signed short id;


	pos = select_item_to_drop(get_spelluser());

#ifdef M302de_ORIGINAL_BUGFIX
	/* If the player cancels item selection or has no items select_item_to_drop() returns -1.
	   The original uses the return value to calculate an index, whithout checking for this. */
	if (pos == -1)
	{
		sprintf(g_dtp2, "");
		return;
	}

#endif

	id = host_readws(get_spelluser() + pos * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_ITEM_ID);

	if (id) {

		if (inventory_magic(get_spelluser() + pos * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_ITEM_ID)) {

			sprintf(g_dtp2, get_tx(81), (char*)GUI_names_grammar((signed short)0x8000, id, 0));

			or_ptr_bs(get_spelluser() + pos * SIZEOF_INVENTORY + (HERO_INVENTORY + INVENTORY_FLAGS), 0x80); /* set 'magic_revealed' flag */

		} else {
			sprintf(g_dtp2, get_tx(82), (char*)GUI_names_grammar((signed short)0x8000, id, 0));
		}
	}
}

void spell_penetrizzel(void)
{
	signed short y;
	signed short x;

	for (y = -2;  y <= 2; y++) {
		for (x = -2;  x <= 2; x++) {
			if ((gs_y_target + y >= 0) && (gs_x_target + x >= 0)) {
				if ((ds_readb(DNG_MAP_SIZE) - 1 >= gs_x_target + x) && (gs_y_target + y <= 15)) {
					set_automap_tile(gs_x_target + x, gs_y_target + y);
				}
			}
		}
	}
}

void spell_sensibar(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Sensibar\" ist nicht implementiert\n");
#endif
}

/* Illusion */
void spell_chamaelioni(void)
{
	or_ptr_bs(get_spelluser() + HERO_FLAGS1, 0x10); /* set 'chamaelioni' flag */

	/* prepare the message */
	sprintf(g_dtp2,	get_tx(83), (char*)get_spelluser() + HERO_NAME2,
		(char*)(GUI_get_ptr(host_readbs(get_spelluser() + HERO_SEX), 0)));

}

void spell_duplicatus(void)
{
	/* set the flag for this spell */
	or_ptr_bs(get_spelluser() + HERO_FLAGS2, 0x04); /* set 'duplicatus' flag

	/* prepare the message */
	sprintf(g_dtp2, get_tx(84), (char*)get_spelluser() + HERO_NAME2,
		(char*)GUI_get_ptr(host_readbs(get_spelluser() + HERO_SEX), 0));

}

void spell_harmlos(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Harmlos\" ist nicht implementiert\n");
#endif
}

void spell_hexenknoten(void)
{
	Bit8u *ptr;
	Bit8u *rp;
	signed short x;
	signed short y;
	signed short no;
	signed short height;
	signed short width;
	struct nvf_desc nvf;

	if (g_fightobj_buf_freespace < 0x240L) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
		return;
	}

	ptr = (Bit8u*)FIG_get_ptr(host_readbs(get_spelluser() + HERO_FIGHTER_ID));
	x = host_readbs(ptr + 3);
	y = host_readbs(ptr + 4);

	if (!host_readbs(get_spelluser() + HERO_VIEWDIR)) {
		x++;
	}
	if (host_readbs(get_spelluser() + HERO_VIEWDIR) == 1) {
		y--;
	}
	if (host_readbs(get_spelluser() + HERO_VIEWDIR) == 2) {
		x--;
	}
	if (host_readbs(get_spelluser() + HERO_VIEWDIR) == 3) {
		y++;
	}

	if (get_cb_val(x, y) != 0) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
		return;
	}

	no = 24;

	if (g_hexenknoten_gfx_buf) {
		rp = g_hexenknoten_gfx_buf;
		/* TODO: graphic bug if cast more than once */
	} else {
		rp = g_fightobj_buf_seek_ptr;
		nvf.dst = rp;
		nvf.src = (Bit8u*)ds_readd(SPELLOBJ_NVF_BUF);
		nvf.no = no;
		nvf.type = 0;
		nvf.width = (Bit8u*)&width;
		nvf.height = (Bit8u*)&height;
		process_nvf(&nvf);

#if !defined(__BORLANDC__)
		/* BE-fix */
		width = host_readws((Bit8u*)&width);
		height = host_readws((Bit8u*)&height);
#endif

		g_hexenknoten_gfx_buf = g_fightobj_buf_seek_ptr;
		/* move pointer further */
		g_fightobj_buf_seek_ptr += width * height + 8;
		g_fightobj_buf_freespace -= width * height + 8L;
	}

	ds_writew(FIG_LIST_ELEM, 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO), 127);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), (signed char)x);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), (signed char)y);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETX), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETY), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_HEIGHT), (signed char)height);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_WIDTH), (signed char)width);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_X1), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Y1), 0);
	ds_writebs((FIG_LIST_ELEM+FIGHTER_X2), (signed char)(width) - 1);
	ds_writebs((FIG_LIST_ELEM+FIGHTER_Y2), (signed char)(height) - 1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_RELOAD), 0);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_WSHEET), -1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_SHEET), -1);
	ds_writed((FIG_LIST_ELEM+FIGHTER_GFXBUF), (Bit32u)rp);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_Z), 50);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_VISIBLE), 1);
	ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), -1);

	FIG_add_to_list(-1);

	place_obj_on_cb(x, y, 127, 127, 0);
}


/* Combat / Kampf */

void spell_blitz(void)
{

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {
		/* cast a hero */

		/* set the spell target */
		g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

		if (get_spelltarget() == get_spelluser()) {

			ds_writew(SPELL_SPECIAL_AECOST, 0);

			strcpy(g_dtp2, get_tx(112));
		} else {
			/* set the rounds counter */
			host_writeb(get_spelltarget() + HERO_BLIND, 3);

			/* prepare the message */
			sprintf(g_dtp2, get_tx(86), (char*)get_spelltarget() + HERO_NAME2);
		}
	} else {
		/* cast an enemy */

		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* set the rounds counter */
		host_writeb(get_spelltarget_e() + ENEMY_SHEET_BLIND, 3);

		/* prepare the message */
		sprintf(g_dtp2, get_tx(85),
			(char*)GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1));
	}
}

void spell_ecliptifactus(void)
{
	signed short rounds;
	signed short ae;

	/* ask how many rounds */
	rounds = GUI_input(get_tx(87), 1);

	if (rounds != -1) {

		/* calculate the AE costs */
		ae = rounds * 2 + 5;

		if (host_readws(get_spelluser() + HERO_AE) >= ae) {
			/* set AP costs */
			ds_writew(SPELL_SPECIAL_AECOST, ae);
			/* enable the spell */
			host_writeb(get_spelluser() + HERO_ECLIPTIFACTUS, (signed char)rounds + 1);
			/* prepare the message */
			sprintf(g_dtp2, get_tx(88), (char*)(get_spelluser() + HERO_NAME2),
				(char*)GUI_get_ptr(host_readbs(get_spelluser() + HERO_SEX), 3), rounds);
		} else {
			/* prepare the message */
			sprintf(g_dtp2, get_ttx(607), (char*)get_spelluser() + HERO_NAME2);
			/* set costs to 0 */
			ds_writew(SPELL_SPECIAL_AECOST, 0);
		}
	} else {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
	}
}

void spell_eisenrost(void)
{
	signed short id;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {
		/* target is a hero */

		/* set the spell target */
		g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

		if (get_spelltarget() == get_spelluser()) {

			ds_writew(SPELL_SPECIAL_AECOST, 0);

			strcpy(g_dtp2, get_tx(112));
		} else {
			/* get weapon id of the target */
			id = host_readws(get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);

			if (!id) {
				/* no weapon in hand */
				ds_writew(SPELL_SPECIAL_AECOST, -2);
			} else {
				/* check if weapon is already broken */
				if (inventory_broken(get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY)) {

					strcpy(g_dtp2, get_tx(90));

				} else {

					if (host_readbs(get_spelltarget() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_BF)) > 0) {
						or_ptr_bs(get_spelltarget() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_FLAGS), 0x01); /* set 'broken' flag */

						sprintf(g_dtp2, get_tx(92),
							(char*)GUI_names_grammar((signed short)0x8000, id, 0),
							(char*)(get_spelltarget() + HERO_NAME2));
					} else {
						ds_writew(SPELL_SPECIAL_AECOST, -2);
					}
				}
			}
		}
	} else {
		/* target is an enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		/* check if target is an animal */
		if (host_readbs(get_spelltarget_e() + ENEMY_SHEET_IS_ANIMAL) != 0)
		{
			sprintf(g_dtp2, get_tx(89));
		} else {
			/* check if weapon is already broken */
			if (host_readbs(get_spelltarget_e() + ENEMY_SHEET_BROKEN) != 0) {
				strcpy(g_dtp2, get_tx(90));
			} else {

				/* set weapon broken */
				host_writeb(get_spelltarget_e() + ENEMY_SHEET_BROKEN, 1);

				/* prepare message */
				sprintf(g_dtp2, get_tx(91),
					(char*)GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e()), 1));
			}
		}
	}
}

void spell_fulminictus(void)
{
	signed short damage;

	if ((host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) &&
		get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) == get_spelluser()) {

		/* do not attack yourself */

		/* set costs to 0 */
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* prepare message */
		strcpy(g_dtp2, get_tx(112));
	} else {
		/* roll 3W6+0 damage */
		damage = dice_roll(3, 6, 0);

		/* add level to damage */
		damage += host_readbs(get_spelluser() + HERO_LEVEL);

		/* reduce damage if the spellcaster has not enough AE */
		if (host_readws(get_spelluser() + HERO_AE) < damage) {

			damage = host_readws(get_spelluser() + HERO_AE);
		}

		/* do the damage */
		FIG_do_spell_damage(damage);

		/* set costs to damage AE */
		ds_writew(SPELL_SPECIAL_AECOST, damage);
	}
}

void spell_ignifaxius(void)
{
	signed short rs_malus;
	signed short hero_pos;
	signed short slot;
	signed short mummy = 0;
	Bit8u *p_armor;
	signed short damage;
	signed short level;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {

		if (get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1) == get_spelluser()) {

			/* don't attack yourself */
			ds_writew(SPELL_SPECIAL_AECOST, 0);

			/* prepare message */
			strcpy(g_dtp2, get_tx(112));
			return;;
		}
	}

	/* get spell level... */
	if ((g_autofight == 0) && (host_readbs(get_spelluser() + HERO_NPC_ID) == 0)) {
		/* ... manual mode */

		/* prepare question of spell level */
		sprintf(g_dtp2, get_tx(93), host_readbs(get_spelluser() + HERO_LEVEL) + 1);

		level = GUI_input(g_dtp2, 2);

		if (level <= 0) {
			/* abort */
			/* terminate string */
			*g_dtp2 = '\0';
			ds_writew(SPELL_SPECIAL_AECOST, 0);
			return;
		}

		/* adjust wrong input */
		if ((host_readbs(get_spelluser() + HERO_LEVEL) + 1) < level) {
			level = host_readbs(get_spelluser() + HERO_LEVEL) + 1;
		}

	} else {
		/* ... autofight mode */
		/* always cast at maximum possible spell level */
		level = host_readbs(get_spelluser() + HERO_LEVEL) + 1;
	}

	/* calculate: damage = level * W6 */
	damage = dice_roll(level, 6, 0);

	/* damage must not exceed AE of the spelluser */
	if (host_readws(get_spelluser() + HERO_AE) < damage) {
		damage = host_readws(get_spelluser() + HERO_AE);
	}

	/* damage doubles if the target is a mummy */
	if ((host_readbs(get_spelluser() + HERO_ENEMY_ID) >= 10) &&
		(host_readbs(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + ENEMY_SHEET_GFX_ID) == 0x1e))
	{
		damage *= 2;
		mummy = 1;
	}

	/* do the damage to the target */
	FIG_do_spell_damage(damage);

	rs_malus = damage / 10;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {

		/* target is a hero */
		hero_pos = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

		/* set the spell target */
		g_spelltarget = get_hero(hero_pos);

		/* get a pointer to the armor */
		p_armor = get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY;

		if ((host_readws(p_armor + INVENTORY_ITEM_ID) != ITEM_NONE) && (rs_malus != 0)) {

			/* adjust rs_malus such that the RS of the worn body armor won't be negative */
			if ((host_readbs(p_armor + INVENTORY_RS_LOST) + rs_malus) > ds_readbs(ARMORS_TABLE + ARMOR_STATS_RS + host_readbs(get_itemsdat(host_readws(p_armor + INVENTORY_ITEM_ID)) + ITEM_STATS_TABLE_INDEX) * SIZEOF_ARMOR_STATS)) {
				rs_malus = ds_readbs(ARMORS_TABLE + ARMOR_STATS_RS + host_readbs(get_itemsdat(host_readws(p_armor + INVENTORY_ITEM_ID)) + ITEM_STATS_TABLE_INDEX) * SIZEOF_ARMOR_STATS) - host_readbs(p_armor + INVENTORY_RS_LOST);
			}

			/* add rs_malus to the armor */
			host_writebs(p_armor + INVENTORY_RS_LOST,
				host_readbs(p_armor + INVENTORY_RS_LOST) + rs_malus);
			/* subtract rs_malus from RS1 */
			host_writebs(get_spelltarget() + HERO_RS_BONUS1,
				host_readbs(get_spelltarget() + HERO_RS_BONUS1) - rs_malus);
		}

		/* get an AT/PA-Malus of -level / 2 for the current weapon and one hour */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1),
			get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE),
			-level / 2, (signed char)hero_pos);

		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1),
			get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE),
			-level / 2, (signed char)hero_pos);

	} else {
		/* target is an enemy */

		/* set a pointer to the enemy */
		ds_writed(SPELLTARGET_E,
			(Bit32u)(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

		host_writebs(get_spelltarget_e() + ENEMY_SHEET_RS,
			host_readbs(get_spelltarget_e() + ENEMY_SHEET_RS) - rs_malus);
		sub_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_AT, level / 2);
		sub_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_PA, level / 2);

	}

	/* terminate output string */
	*g_dtp2 = '\0';

	if (mummy != 0) {
		/* halve damage to get the correct AE costs */
		damage /= 2;
	}

	/* set spell costs */
	ds_writew(SPELL_SPECIAL_AECOST, damage);
}

void spell_plumbumbarum(void)
{
	signed short slot;
	signed short hero_pos;

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) < 10) {

		/* target is a hero */

		hero_pos = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

		/* set the spell target */
		g_spelltarget = get_hero(hero_pos);

		if (get_spelltarget() == get_spelluser()) {

			/* don't attack yourself */
			ds_writew(SPELL_SPECIAL_AECOST, 0);

			/* prepare message */
			strcpy(g_dtp2, get_tx(112));

		} else {

			/* give a short AT-malus of -3 to the current weapon of the target */
			slot = get_free_mod_slot();
			set_mod_slot(slot, 0x2d,
				get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE),
				-3, (signed char)hero_pos);

			/* prepare the message */
			sprintf(g_dtp2, get_tx(94), (char*)get_spelltarget() + HERO_NAME2);
			}

		return;

	}

	/* target is an enemy */

	/* set a pointer to the enemy */
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET) + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET));

	/* AT-malus of -3 (permanent) */
	sub_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_AT, 3);

	/* prepare the message */
	sprintf(g_dtp2, get_tx(95),
		(char*)GUI_names_grammar((signed short)0x8001, host_readbs(get_spelltarget_e()), 1));
}

void spell_radau(void)
{
	spell_kraehenruf();
}

void spell_saft_kraft(void)
{
	signed short target;
	signed short slot;
	signed short rounds;

	rounds = random_schick(20);

	/* get the index of the hero on whom the spell is cast */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	/* set a pointer to the target */
	g_spelltarget = get_hero(target);

	/* +5 on AT of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE),
		5, (signed char)target);

	/* -5 on PA of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE),
		-5, (signed char)target);

	/* +5 extra damage */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		get_spelltarget() + HERO_SAFTKRAFT, 5, (signed char)target);

	/* set ae costs */
	ds_writew(SPELL_SPECIAL_AECOST, rounds);

#ifdef M302de_ORIGINAL_BUGFIX
	char *p = get_tx(96);

	if (p[10] == 'L' && p[11] == 'E') {
		/* change "VERWANDLET" into "VERWANDELT" */
		p[10] = 'E';
		p[11] = 'L';
	}
#endif

	/* prepare message */
	sprintf(g_dtp2, get_tx(96), (char*)get_spelltarget() + HERO_NAME2);

}

void spell_scharfes_auge(void)
{
	signed short target;
	signed short slot;

	/* get the index of the hero on whom the spell is cast */
	target = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	/* set a pointer to the target */
	g_spelltarget = get_hero(target);

	/* all range skills are boosted + 3 */

	slot = get_free_mod_slot();

	set_mod_slot(slot, 3 * 9L, get_spelltarget() + (HERO_TALENTS + TA_WURFWAFFEN), 3, (signed char)target); /* TA_WURFWAFFEN */

	slot = get_free_mod_slot();

	set_mod_slot(slot, 3 * 9L, get_spelltarget() + (HERO_TALENTS + TA_SCHUSSWAFFEN), 3, (signed char)target); /* TA_SCHUSSWAFFEN */

	sprintf(g_dtp2, get_tx(97), (char*)get_spelltarget() + HERO_NAME2);
}


/* Communication / Verstaendigung */
void spell_hexenblick(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Hexenblick\" ist nicht implementiert\n");
#endif
}

void spell_necropathia(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Necropathia\" ist nicht implementiert\n");
#endif
}

#if !defined(__BORLANDC__)
}
#endif
