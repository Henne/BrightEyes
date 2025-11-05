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

static unsigned char *g_hexenknoten_gfx_buf = NULL; // ds:0xacc8;

/* Clairvoyance / Hellsicht */

void spell_eigenschaften(void)
{
	signed int min;
	signed int max;

	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->target_object_id - 10];

	damage_range_template(g_spelltarget_e->dam1, &min, &max);

	/* Remark: For unknown reasons the shown TP-values of enemies are scaled down to 80% */
	min = min * 8 / 10;
	max = max * 8 / 10;

	sprintf(g_dtp2, get_tx(25), GUI_name_singular(get_monname(g_spelltarget_e->mon_id)),
          g_spelltarget_e->level,	/* Level */
          g_spelltarget_e->at,		/* AT */
          g_spelltarget_e->pa,		/* PA */
          g_spelltarget_e->rs,		/* RS */
          g_spelltarget_e->attacks,	/* Attacks */
          (g_spelltarget_e->attacks > 1) ? get_tx(26) : get_tx(27),
          min,				/* TPmin */
          max,				/* TPmax */
          g_spelltarget_e->le,		/* LE */
          g_spelltarget_e->le_orig,	/* LEmax */
          g_spelltarget_e->ae,		/* AE */
          g_spelltarget_e->ae_orig);	/* AEmax */
}

void spell_exposami(void)
{
	signed int j;
	signed int id;
	signed int changed;
	int arr[20][2];

	signed int i;
	signed int count;

	count = 0;

	for (i = 0; i < g_nr_of_enemies; i++) {

		if (g_current_fight->monsters[i].round_appear) {

			id = g_current_fight->monsters[i].id;

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
	signed int inv_pos;
	signed int item_id;

	inv_pos = select_item_to_drop(get_spelluser());

#ifdef M302de_ORIGINAL_BUGFIX
	/* If the player cancels item selection or has no items select_item_to_drop() returns -1.
	   The original uses the return value to calculate an index, whithout checking for this. */
	if (inv_pos == -1)
	{
		sprintf(g_dtp2, "");
		return;
	}

#endif

	item_id = get_spelluser()->inventory[inv_pos].item_id;

	if (item_id) {

		if (get_spelluser()->inventory[inv_pos].flags.magic) {

			sprintf(g_dtp2, get_tx(81), GUI_names_grammar((signed int)0x8000, item_id, 0));

			get_spelluser()->inventory[inv_pos].flags.magic_revealed = 1;

		} else {
			sprintf(g_dtp2, get_tx(82), GUI_names_grammar((signed int)0x8000, item_id, 0));
		}
	}
}

void spell_penetrizzel(void)
{
	signed int y;
	signed int x;

	for (y = -2;  y <= 2; y++) {
		for (x = -2;  x <= 2; x++) {
			if ((gs_y_target + y >= 0) && (gs_x_target + x >= 0)) {
				if ((g_dng_map_size - 1 >= gs_x_target + x) && (gs_y_target + y <= 15)) {
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
	get_spelluser()->flags.chamaelioni = 1;

	/* prepare the message */
	sprintf(g_dtp2,	get_tx(83), get_spelluser()->alias,
		(GUI_get_ptr(get_spelluser()->sex, 0)));
}

void spell_duplicatus(void)
{
	get_spelluser()->flags.duplicatus = 1;

	/* prepare the message */
	sprintf(g_dtp2, get_tx(84), get_spelluser()->alias,
		GUI_get_ptr(get_spelluser()->sex, 0));
}

void spell_harmlos(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Harmlos\" ist nicht implementiert\n");
#endif
}

void spell_hexenknoten(void)
{
	struct struct_fighter *fighter;
	Bit8u *rp;
	signed int x;
	signed int y;
	signed int no;
	signed int height;
	signed int width;
	struct nvf_desc nvf;

	if (g_fightobj_buf_freespace < 0x240L) {
		g_spell_special_aecost = -2;
		return;
	}

	fighter = FIG_get_fighter(get_spelluser()->fighter_id);
	x = fighter->cbx;
	y = fighter->cby;

	if (!get_spelluser()->viewdir) {
		x++;
	}
	if (get_spelluser()->viewdir == 1) {
		y--;
	}
	if (get_spelluser()->viewdir == 2) {
		x--;
	}
	if (get_spelluser()->viewdir == 3) {
		y++;
	}

	if (get_cb_val(x, y) != 0) {
		g_spell_special_aecost = (-2);
		return;
	}

	no = 24;

	if (g_hexenknoten_gfx_buf) {
		rp = g_hexenknoten_gfx_buf;
		/* TODO: graphic bug if cast more than once */
	} else {
		rp = g_fightobj_buf_seek_ptr;
		nvf.dst = rp;
		nvf.src = g_spellobj_nvf_buf;
		nvf.no = no;
		nvf.type = 0;
		nvf.width = &width;
		nvf.height = &height;
		process_nvf(&nvf);

		g_hexenknoten_gfx_buf = g_fightobj_buf_seek_ptr;
		/* move pointer further */
		g_fightobj_buf_seek_ptr += width * height + 8;
		g_fightobj_buf_freespace -= width * height + 8L;
	}

	g_fig_list_elem.figure = 0;
	g_fig_list_elem.nvf_no = 127;
	g_fig_list_elem.cbx = x;
	g_fig_list_elem.cby = y;
	g_fig_list_elem.offsetx = 0;
	g_fig_list_elem.offsety = 0;
	g_fig_list_elem.height = height;
	g_fig_list_elem.width = width;
	g_fig_list_elem.x1 = 0;
	g_fig_list_elem.y1 = 0;
	g_fig_list_elem.x2 = width - 1;
	g_fig_list_elem.y2 = height - 1;
	g_fig_list_elem.reload = 0;
	g_fig_list_elem.wsheet = -1;
	g_fig_list_elem.sheet = -1;
	g_fig_list_elem.gfxbuf = rp;
	g_fig_list_elem.z = 50;
	g_fig_list_elem.visible = 1;
	g_fig_list_elem.double_size = -1;

	FIG_add_to_list(-1);

	place_obj_on_cb(x, y, 127, 127, 0); /* BAE-TODO: not linked correctly: 2xBit16, 3xBit8 */
}


/* Combat / Kampf */

void spell_blitz(void)
{

	if (get_spelluser()->target_object_id < 10) {
		/* cast a hero */

		/* set the spell target */
		g_spelltarget = get_hero(get_spelluser()->target_object_id - 1);

		if (get_spelltarget() == get_spelluser()) {

			g_spell_special_aecost = 0;

			strcpy(g_dtp2, get_tx(112));
		} else {
			/* set the rounds counter */
			get_spelltarget()->blind_timer = 3;

			/* prepare the message */
			sprintf(g_dtp2, get_tx(86), get_spelltarget()->alias);
		}
	} else {
		/* cast an enemy */

		/* set a pointer to the enemy */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->target_object_id - 10];

		/* set the rounds counter */
		g_spelltarget_e->blind = 3;

		/* prepare the message */
		sprintf(g_dtp2, get_tx(85), GUI_names_grammar((signed int)0x8000, g_spelltarget_e->mon_id, 1));
	}
}

void spell_ecliptifactus(void)
{
	signed int rounds;
	signed int ae;

	/* ask how many rounds */
	rounds = GUI_input(get_tx(87), 1);

	if (rounds != -1) {

		/* calculate the AE costs */
		ae = rounds * 2 + 5;

		if (get_spelluser()->ae >= ae) {
			/* set AP costs */
			g_spell_special_aecost = (ae);
			/* enable the spell */
			get_spelluser()->ecliptifactus_timer = (signed char)rounds + 1;
			/* prepare the message */
			sprintf(g_dtp2, get_tx(88), get_spelluser()->alias,
				GUI_get_ptr(get_spelluser()->sex, 3), rounds);
		} else {
			/* prepare the message */
			sprintf(g_dtp2, get_ttx(607), get_spelluser()->alias);
			/* set costs to 0 */
			g_spell_special_aecost = 0;
		}
	} else {
		g_spell_special_aecost = -2;
	}
}

void spell_eisenrost(void)
{
	signed int id;

	if (get_spelluser()->target_object_id < 10) {
		/* target is a hero */

		/* set the spell target */
		g_spelltarget = get_hero(get_spelluser()->target_object_id - 1);

		if (get_spelltarget() == get_spelluser()) {

			g_spell_special_aecost = 0;

			strcpy(g_dtp2, get_tx(112));
		} else {
			/* get weapon id of the target */
			id = get_spelltarget()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;

			if (!id) {
				/* no weapon in hand */
				g_spell_special_aecost = (-2);
			} else {
				/* check if weapon is already broken */
				if (get_spelltarget()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.broken) {

					strcpy(g_dtp2, get_tx(90));

				} else {

					if (get_spelltarget()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].bf > 0) {

						get_spelltarget()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.broken = 1;

						sprintf(g_dtp2, get_tx(92), GUI_names_grammar((signed int)0x8000, id, 0),
							(char*)get_spelltarget()->alias);
					} else {
						g_spell_special_aecost = -2;
					}
				}
			}
		}
	} else {
		/* target is an enemy */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->target_object_id - 10];

		/* check if target is an animal */
		if (g_spelltarget_e->is_animal)
		{
			sprintf(g_dtp2, get_tx(89));
		} else {
			/* check if weapon is already broken */
			if (g_spelltarget_e->weapon_broken) {
				strcpy(g_dtp2, get_tx(90));
			} else {

				/* set weapon broken */
				g_spelltarget_e->weapon_broken = 1;

				/* prepare message */
				sprintf(g_dtp2, get_tx(91), GUI_names_grammar((signed int)0x8000, g_spelltarget_e->mon_id, 1));
			}
		}
	}
}

void spell_fulminictus(void)
{
	signed int damage;

	if ((get_spelluser()->target_object_id < 10) &&
		get_hero(get_spelluser()->target_object_id - 1) == get_spelluser()) {

		/* do not attack yourself */

		/* set costs to 0 */
		g_spell_special_aecost = 0;

		/* prepare message */
		strcpy(g_dtp2, get_tx(112));
	} else {
		/* roll 3W6+0 damage */
		damage = dice_roll(3, 6, 0);

		/* add level to damage */
		damage += get_spelluser()->level;

		/* reduce damage if the spellcaster has not enough AE */
		if (get_spelluser()->ae < damage) {

			damage = get_spelluser()->ae;
		}

		/* do the damage */
		FIG_do_spell_damage(damage);

		/* set costs to damage AE */
		g_spell_special_aecost = (damage);
	}
}

void spell_ignifaxius(void)
{
	signed int rs_malus;
	signed int hero_pos;
	signed int slot;
	signed int mummy = 0;
	struct inventory *p_armor;
	signed int damage;
	signed int level;

	if (get_spelluser()->target_object_id < 10) {

		if (get_hero(get_spelluser()->target_object_id - 1) == get_spelluser()) {

			/* don't attack yourself */
			g_spell_special_aecost = 0;

			/* prepare message */
			strcpy(g_dtp2, get_tx(112));
			return;;
		}
	}

	/* get spell level... */
	if ((g_autofight == 0) && (get_spelluser()->npc_id == 0)) {
		/* ... manual mode */

		/* prepare question of spell level */
		sprintf(g_dtp2, get_tx(93), get_spelluser()->level + 1);

		level = GUI_input(g_dtp2, 2);

		if (level <= 0) {
			/* abort */
			/* terminate string */
			*g_dtp2 = '\0';
			g_spell_special_aecost = 0;
			return;
		}

		/* adjust wrong input */
		if ((get_spelluser()->level + 1) < level) {
			level = get_spelluser()->level + 1;
		}

	} else {
		/* ... autofight mode */
		/* always cast at maximum possible spell level */
		level = get_spelluser()->level + 1;
	}

	/* calculate: damage = level * W6 */
	damage = dice_roll(level, 6, 0);

	/* damage must not exceed AE of the spelluser */
	if (get_spelluser()->ae < damage) {
		damage = get_spelluser()->ae;
	}

	/* damage doubles if the target is a mummy */
	if ((get_spelluser()->target_object_id >= 10) &&
			(g_enemy_sheets[get_spelluser()->target_object_id - 10].gfx_id == 0x1e))
	{
		damage *= 2;
		mummy = 1;
	}

	/* do the damage to the target */
	FIG_do_spell_damage(damage);

	rs_malus = damage / 10;

	if (get_spelluser()->target_object_id < 10) {

		/* target is a hero */
		hero_pos = get_spelluser()->target_object_id - 1;

		/* set the spell target */
		g_spelltarget = get_hero(hero_pos);

		/* get a pointer to the armor */
		p_armor = (struct inventory*)&(get_spelltarget()->inventory[HERO_INVENTORY_SLOT_BODY]);

		if ((p_armor->item_id != ITEM_NONE) && (rs_malus != 0)) {

			/* adjust rs_malus such that the RS of the worn body armor won't be negative */
			if ((p_armor->rs_lost + rs_malus) > g_armors_table[g_itemsdat[p_armor->item_id].table_index].rs) {
				rs_malus = g_armors_table[g_itemsdat[p_armor->item_id].table_index].rs - p_armor->rs_lost;
			}

			/* add rs_malus to the armor */
			p_armor->rs_lost = p_armor->rs_lost + rs_malus;
			/* subtract rs_malus from RS1 */
			get_spelltarget()->rs_bonus1 = get_spelltarget()->rs_bonus1 - rs_malus;
		}

		/* get an AT/PA-Malus of -level / 2 for the current weapon and one hour */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1),
			(Bit8u*)&get_spelltarget()->at_weapon[get_spelltarget()->w_type],
			-level / 2, (signed char)hero_pos);

		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1),
			(Bit8u*)&get_spelltarget()->pa_weapon[get_spelltarget()->w_type],
			-level / 2, (signed char)hero_pos);

	} else {
		/* target is an enemy */

		/* set a pointer to the enemy */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->target_object_id - 10];

		g_spelltarget_e->rs = g_spelltarget_e->rs - rs_malus;
		g_spelltarget_e->at -= level / 2;
		g_spelltarget_e->pa -= level / 2;

	}

	/* terminate output string */
	*g_dtp2 = '\0';

	if (mummy != 0) {
		/* halve damage to get the correct AE costs */
		damage /= 2;
	}

	/* set spell costs */
	g_spell_special_aecost = damage;
}

void spell_plumbumbarum(void)
{
	signed int slot;
	signed int hero_pos;

	if (get_spelluser()->target_object_id < 10) {

		/* target is a hero */

		hero_pos = get_spelluser()->target_object_id - 1;

		/* set the spell target */
		g_spelltarget = get_hero(hero_pos);

		if (get_spelltarget() == get_spelluser()) {

			/* don't attack yourself */
			g_spell_special_aecost = 0;

			/* prepare message */
			strcpy(g_dtp2, get_tx(112));

		} else {

			/* give a short AT-malus of -3 to the current weapon of the target */
			slot = get_free_mod_slot();
			set_mod_slot(slot, 0x2d,
				(Bit8u*)&get_spelltarget()->at_weapon[get_spelltarget()->w_type],
				-3, (signed char)hero_pos);

			/* prepare the message */
			sprintf(g_dtp2, get_tx(94), get_spelltarget()->alias);
		}

		return;

	}

	/* target is an enemy */

	/* set a pointer to the enemy */
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->target_object_id - 10];

	/* AT-malus of -3 (permanent) */
	g_spelltarget_e->at -= 3;

	/* prepare the message */
	sprintf(g_dtp2, get_tx(95), GUI_names_grammar((signed int)0x8001, g_spelltarget_e->mon_id, 1));
}

void spell_radau(void)
{
	spell_kraehenruf();
}

void spell_saft_kraft(void)
{
	signed int target;
	signed int slot;
	signed int rounds;

	rounds = random_schick(20);

	/* get the index of the hero on whom the spell is cast */
	target = get_spelluser()->target_object_id - 1;

	/* set a pointer to the target */
	g_spelltarget = get_hero(target);

	/* +5 on AT of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		(Bit8u*)&get_spelltarget()->at_weapon[get_spelltarget()->w_type],
		5, (signed char)target);

	/* -5 on PA of the current weapon */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L,
		(Bit8u*)&get_spelltarget()->pa_weapon[get_spelltarget()->w_type],
		-5, (signed char)target);

	/* +5 extra damage */
	slot = get_free_mod_slot();

	set_mod_slot(slot, rounds * 9L, (Bit8u*)&get_spelltarget()->saftkraft, 5, (signed char)target);

	/* set ae costs */
	g_spell_special_aecost = rounds;

#ifdef M302de_ORIGINAL_BUGFIX
	char *p = get_tx(96);

	if (p[10] == 'L' && p[11] == 'E') {
		/* change "VERWANDLET" into "VERWANDELT" */
		p[10] = 'E';
		p[11] = 'L';
	}
#endif

	/* prepare message */
	sprintf(g_dtp2, get_tx(96), get_spelltarget()->alias);

}

void spell_scharfes_auge(void)
{
	signed int target;
	signed int slot;

	/* get the index of the hero on whom the spell is cast */
	target = get_spelluser()->target_object_id - 1;

	/* set a pointer to the target */
	g_spelltarget = get_hero(target);

	/* all range skills are boosted + 3 */

	slot = get_free_mod_slot();

	set_mod_slot(slot, 3 * 9L, (Bit8u*)&get_spelltarget()->skills[TA_WURFWAFFEN], 3, (signed char)target); /* TA_WURFWAFFEN */

	slot = get_free_mod_slot();

	set_mod_slot(slot, 3 * 9L, (Bit8u*)&get_spelltarget()->skills[TA_SCHUSSWAFFEN], 3, (signed char)target); /* TA_SCHUSSWAFFEN */

	sprintf(g_dtp2, get_tx(97), get_spelltarget()->alias);
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
