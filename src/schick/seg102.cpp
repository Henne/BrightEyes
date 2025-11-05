/*
 *      Rewrite of DSA1 v3.02_de functions of seg102 (spells of monsters)
 *      Functions rewritten: 22/22 (complete)
 *      Functions called rewritten 20/20 (complete)
 *      Functions uncalled rewritten 2/2 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg102.cpp
 */

#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg041.h"
#include "seg096.h"
#include "seg102.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static signed short g_monster_spell_ae_cost = -1; // ds:0xaccc

signed int g_spelltest_result;			// ds:0xe5b2
struct enemy_sheet *g_spelltarget_e;		// ds:0xe5b4, Pointer to enemy
struct struct_hero *g_spelltarget;		// ds:0xe5b8
struct struct_hero *g_spelluser;		// ds:0xe5bc
static struct enemy_sheet *g_spelluser_e;	// ds:0xe5c0

void MON_do_spell_damage(const signed int damage)
{
	if (damage > 0) {

		if (g_spelluser_e->target_object_id < 10) {
			/* target is a hero */

			/* set the pointer to the target */
			g_spelltarget = get_hero(g_spelluser_e->target_object_id - 1);

			/* do the damage */
			sub_hero_le(get_spelltarget(), damage);

			/* add a fight message */
			FIG_add_msg(8, damage);

			/* check if someone died */
			if (get_spelltarget()->flags.dead) {
				g_defender_dead = 1;
			}

		} else {
			/* target is a monster */

			/* set the pointer to the target */
			g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

			/* do the damage */
			FIG_damage_enemy(g_spelltarget_e, damage, 1);

			/* add a fight message */
			FIG_add_msg(11, damage);

			/* check if someone died */
			if (g_spelltarget_e->flags.dead) {
				g_defender_dead = 1;
			}
		}
	}
}

/* unused */
signed int MON_get_target_PA(void)
{
	if (g_spelluser_e->target_object_id < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget =  get_hero(g_spelluser_e->target_object_id - 1);

		/* calc and return PA-value */
		return get_spelltarget()->pa_weapon[get_spelltarget()->w_type] - get_spelltarget()->atpa_mod;

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

		/* calc and return PA-value */
		return g_spelltarget_e->pa;
	}
}

/* unused */
signed int MON_get_target_RS(void)
{
	if (g_spelluser_e->target_object_id < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget = get_hero(g_spelluser_e->target_object_id - 1);

		/* return RS-value */
		return get_spelltarget()->rs_bonus1;

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

		/* return PA-value */
		return g_spelltarget_e->rs;
	}
}

signed int MON_get_spell_cost(const signed int mspell_id, const signed int flag)
{
	signed char cost = g_mon_spell_descriptions[mspell_id].ae_cost;

	if (flag != 0) {

		cost = (cost == -1 ? random_interval(5, 10) : cost / 2);
	}

	return cost;
}


/**
 * \brief   skill test for monsters
 *
 * \param   monster     pointer to monster
 * \param   attrib1     no of 1st attribute
 * \param   attrib2     no of 2nd attribute
 * \param   attrib3     no of 3rd attribute
 * \param   handicap    may be positive or negative. The higher the value, the harder the test.
 */
signed int MON_test_attrib3(const struct enemy_sheet *monster, const signed int attrib1, const signed int attrib2, const signed int attrib3, signed char handicap)
/* called only from a single position, in MON_test_skill((struct struct_hero*)..) */
{

#ifndef M302de_FEATURE_MOD
	/* Feature mod 6: The implementation of the skill test logic differs from the original DSA2/3 rules.
	 * It is sometimes called the 'pool' variant, where '3W20 + handicap' is compared to the sum of the attributes.
	 * It is significantly easier than the original rule, where each individuall roll must be at most the corresponding attribute,
	 * where positive handicap must be used up during the process, and negative handicap may be used for compensation. */

	signed int randval;
	signed int attr_sum;

	randval = dice_roll(3, 20, handicap);

	attr_sum = monster->attrib[2 * attrib1 + 1] + monster->attrib[2 * attrib2 + 1] + monster->attrib[2 * attrib3 + 1];

	return attr_sum - randval + 1;
#else
	/* Here, the original DSA2/3 skill test logic is implemented.
	 * WARNING: This makes skill tests and spell casting (on both sides), and thus the game, significantly harder!
	 * Note that we are not implementing the DSA4 rules, where tests with a positive handicap are yet harder. */
	signed int randval;
	signed int i;
	signed int nr_rolls_1 = 0;
	signed int nr_rolls_20 = 0;
	signed int fail = 0;
	signed int attrib[3];

	attrib[0] = monster->attrib[2 * attrib1 + 1];
	attrib[1] = monster->attrib[2 * attrib2 + 1];
	attrib[2] = monster->attrib[2 * attrib3 + 1];

#if !defined(__BORLANDC__)
	D1_INFO(" (%s %d/%s %d/%s %d) ->",
		names_attrib[attrib1], attrib[0],
		names_attrib[attrib2], attrib[1],
		names_attrib[attrib3], attrib[2]);
#endif

	for (i = 0; i < 3; i++) {

		randval = random_schick(20);

#if !defined(__BORLANDC__)
		D1_INFO(" W20 = %d;", randval);
#endif

		if (randval == 20) {
			if (++nr_rolls_20 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> UNGLUECKLICH nicht bestanden\n");
#endif
				return -99;
			}
		}

		if (randval == 1) {
			if (++nr_rolls_1 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> GLUECKLICH bestanden\n");
#endif
				return 99;
			}
		}

		if (!fail) {
			randval -= attrib[i];
			if (handicap <= 0) {
				if (randval > 0) {
					if (randval > -handicap) {
						fail = 1;
#if !defined(__BORLANDC__)
						D1_INFO(" zu hoch!");
#endif
					} else  {
						handicap += randval;
					}
				}
			}
			if (handicap > 0) {
				if (randval > 0) {
					fail = 1;
#if !defined(__BORLANDC__)
					D1_INFO(" zu hoch!");
#endif
				} else {
					handicap += randval;
					if (handicap < 0) {
						handicap = 0;
					}
				}
			}
		}
	}
	if (fail || (handicap > 0)) {
#if !defined(__BORLANDC__)
		D1_INFO(" -> nicht bestanden.\n");
#endif
		return 0;
	} else {
#if !defined(__BORLANDC__)
		D1_INFO(" -> bestanden mit %d.\n",-handicap);
#endif
		return 1 - handicap;
	}
#endif
}


/* called only from a single position, in MON_cast_spell(..) */
signed int MON_test_skill(const struct enemy_sheet *monster, const signed int mspell_id, signed char handicap)
{
	struct mon_spell_description *desc = &g_mon_spell_descriptions[mspell_id];

	/* depends on MR */
	if (desc->vs_mr) {

		/* add MR */
		handicap += (monster->target_object_id >= 10 ?	g_enemy_sheets[monster->target_object_id - 10].mr : get_hero(monster->target_object_id - 1)->mr);
	}

	/* check if the monster spell has a valid ID */
	if ((mspell_id >= 1) && (mspell_id <= 14)) {

#if !defined(__BORLANDC__)
		D1_INFO("Gegnerischer Zauber %s Probe %+d", names_mspell[mspell_id], handicap);
#endif

		/* TODO: balancing problem: enemy spells are always cast with skill value 0 */
		return MON_test_attrib3(monster, desc->attrib1, desc->attrib2, desc->attrib3, handicap);
	}

	return 0;
}

void MON_sub_ae(struct enemy_sheet *monster, signed int ae)
{
	if (!monster->flags.dead) {

		monster->ae -= ae;

		if (monster->ae < 0) {
			monster->ae = 0;
		}
	}
}

signed short MON_cast_spell(struct enemy_sheet* monster, signed char handicap)
{
	signed int mspell_id;
	signed int retval;
	signed int cost;
	void (*func)(void);
	signed int tx_file_bak;

	mspell_id = monster->cur_spell;

	if (mspell_id > 0) {

		cost = MON_get_spell_cost(mspell_id, 0);

		/* check AE */
		if (monster->ae < cost) {
			return -1;
		}

		g_spelltest_result = MON_test_skill(monster, mspell_id, handicap);

		if ((g_spelltest_result <= 0) || (gs_ingame_timers[INGAME_TIMER_RONDRA_NO_SPELLS] > 0)) {

			/* spell failed */
			MON_sub_ae(monster, MON_get_spell_cost(mspell_id, 1));

			return 0;

		} else {

			g_spelluser_e = monster;

			g_monster_spell_ae_cost = -1;

			/* terminate output string */
			*g_dtp2 = '\0';

			tx_file_bak = g_tx_file_index;

			load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

			func = g_mon_spellhandlers[mspell_id];
			func();

			if ((tx_file_bak != -1) && (tx_file_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {

				load_tx(tx_file_bak);
			}

			retval = 1;

			if (!g_monster_spell_ae_cost) {

				retval = -1;

			} else if (g_monster_spell_ae_cost == -2) {

				MON_sub_ae(monster, MON_get_spell_cost(mspell_id, 1));

				retval = 0;

			} else if (g_monster_spell_ae_cost != -1) {

				MON_sub_ae(monster, g_monster_spell_ae_cost);

			} else {
				MON_sub_ae(monster, cost);
			}

			return retval;
		}
	} else {
		return 0;
	}
}

/**
 * \brief can undo 'petrified' and 'mushroom'-effects on enemies
 */
void mspell_verwandlung(void)
{
	/* set pointer to monster target */
	g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

	if (g_spelltarget_e->flags.petrified) {

		/* set the spellcosts */
		g_monster_spell_ae_cost = 5 * random_schick(10);

		if (g_spelluser_e->ae < g_monster_spell_ae_cost) {
			/* if not enough AE, all AE will be consumed, without further effect */
			g_monster_spell_ae_cost = g_spelluser_e->ae;
		} else {
			/* unset 'petrified' flag */
			g_spelltarget_e->flags.petrified = 0;

			/* prepare message */
			sprintf(g_dtp2,	get_tx(114), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
		}

	} else if (g_spelltarget_e->flags.mushroom) {

		/* set the spellcosts */
		g_monster_spell_ae_cost = 5 * random_schick(10);

		if (g_spelluser_e->ae < g_monster_spell_ae_cost) {
			/* if not enough AE, all AE will be consumed, without further effect */
			g_monster_spell_ae_cost = g_spelluser_e->ae;
		} else {
			/* unset 'mushroom' flag */
			g_spelltarget_e->flags.mushroom = 0;

			g_mspell_awake_flag = 1;
		}
	} else {
		g_monster_spell_ae_cost = 2;
	}
}

void mspell_bannbaladin(void)
{
	/* set pointer to hero target */
	g_spelltarget = get_hero(g_spelluser_e->target_object_id - 1);

	/* set the flag */
	get_spelltarget()->flags.tame = 1;

	/* prepare message */
	sprintf(g_dtp2, get_tx(115), get_spelltarget()->alias);
}

void mspell_boeser_blick(void)
{
	/* set pointer to hero target */
	g_spelltarget = get_hero(g_spelluser_e->target_object_id - 1);

	/* set the flag */
	get_spelltarget()->flags.renegade = 1;

	/* prepare message */
	sprintf(g_dtp2, get_tx(116), get_spelltarget()->alias);
}

void mspell_horriphobus(void)
{
	/* set pointer to hero target */
	g_spelltarget = get_hero(g_spelluser_e->target_object_id - 1);

	/* set the flag */
	get_spelltarget()->flags.scared = 1; /* set 'scared' flag */
	get_spelltarget()->flags.renegade = 0; /* unset 'renegade' flag */

	/* prepare message */
	sprintf(g_dtp2, get_tx(117), get_spelltarget()->alias);
}

void mspell_axxeleratus(void)
{
	/* set pointer to monster target */
	g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

	/* #Attacks + 1 */
	g_spelltarget_e->attacks++;

	/* AT + 1 */
	g_spelltarget_e->at += 1;

	/* PA + 1 */
	g_spelltarget_e->pa += 1;

	/* BP * 2 */
	g_spelltarget_e->bp = 2 * g_spelltarget_e->bp;

}

void mspell_balsam(void)
{
	signed int le;

	/* set pointer to monster target */
	g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug:
	 * very strange behavior.
	 * If LE loss is less then 14, 7 LE will be healed, price AE = LE.
	 * If not enouh AE available, number of healed LE is lowered accordingly.
	 *
	 * Problem 1: For LE loss < 7, LE may be healed above max.
	 * Problem 2: For LE loss >=14, no effect. No healing.
	 *
	 * Hard to guess what the intended behavior was. */

	g_monster_spell_ae_cost = 0;

	le = (g_spelltarget_e->le_orig - g_spelltarget_e->le) / 2; /* half of the missing LE */

	if (le) {
		if (le < 7) {
			/* AE costs are at least 7 */
			g_monster_spell_ae_cost = 7;
		}
		if (g_spelluser_e->ae < g_monster_spell_ae_cost) {
			/* not enough AE: heal only that many LE as the spellcaster has AE available */
			g_monster_spell_ae_cost = g_spelluser_e->ae;
		}
		g_spelltarget_e->le += g_monster_spell_ae_cost;
	}
#else
	/* Fix:
	 * Let's do it this way:
	 * Heal (missing LE)/2 LE, but at least 7, as long as it won't be healed above max.
	 * If not enouh AE available, number of healed LE is lowered accordingly. */
	le = g_spelltarget_e->le_orig - g_spelltarget_e->le; /* missing LE */
	if (le >= 16) {
		le /= 2;
	} else if (le > 7) {
		le = 7;
	}
	if (g_spelluser_e->ae < le) {
		le = g_spelluser_e->ae;
	}

	g_monster_spell_ae_cost = le;
	g_spelltarget_e->le += le;
#endif
}

void mspell_blitz(void)
{
	if (g_spelluser_e->target_object_id < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget = get_hero(g_spelluser_e->target_object_id - 1);

		/* set blitz timer to 3 rounds */
		get_spelltarget()->blind_timer = 3;

		/* prepare message */
		sprintf(g_dtp2, get_tx(86), get_spelltarget()->alias);
	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

		/* set blitz timer to 3 rounds */
		g_spelltarget_e->blind = 3;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(85), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	}
}

void mspell_eisenrost(void)
{
	signed int item_id;

	if (g_spelluser_e->target_object_id < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget = get_hero(g_spelluser_e->target_object_id - 1);

		item_id = get_spelltarget()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;

		if (!item_id) {

			/* target hero has no weapon */
			g_monster_spell_ae_cost = 2;

		} else if (!get_spelltarget()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.broken) {

			if (get_spelltarget()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].bf > 0) {

				get_spelltarget()->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.broken = 1;

				/* prepare message */
				sprintf(g_dtp2,	get_tx(92), (Bit8u*)GUI_names_grammar((signed short)0x8000, item_id, 0), get_spelltarget()->alias);

			} else {
				g_monster_spell_ae_cost = -2;
			}
		}

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

		/* if weapon is not broken */
		if (!g_spelltarget_e->weapon_broken) {

			/* set weapon broken */
			g_spelltarget_e->weapon_broken = 1;

			/* prepare message */
			sprintf(g_dtp2,	get_tx(91), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
		}
	}
}

void mspell_fulminictus(void)
{
	signed int damage;

	/* roll 3W6 */
	damage = dice_roll(3, 6, 0);

	/* add the level of the spelluser */
	damage += g_spelluser_e->level;

	/* adjust damage */
	if (g_spelluser_e->ae < damage) {
		damage = g_spelluser_e->ae;
	}

	/* do the damage */
	MON_do_spell_damage(damage);

	/* set the cost */
	g_monster_spell_ae_cost = damage;
}

void mspell_ignifaxius(void)
{
	signed int damage;
	signed int level;
	signed int rs_malus;
	signed int hero_pos;
	signed int slot;
	struct inventory *p_armor;

	/* get the level of the spelluser */
	level = g_spelluser_e->level;

	/* roll the damage: damage = level * W6 */
	damage = dice_roll(level, 6, 0);

	/* damage must not exceed AE of the spelluser */
	if (g_spelluser_e->ae < damage) {
		damage = g_spelluser_e->ae;
	}

	/* do the damage */
	MON_do_spell_damage(damage);

	/* calc RS malus */
	rs_malus = damage / 10;

	if (g_spelluser_e->target_object_id < 10) {
		/* target is a hero */

		/* get the position of the target hero */
		hero_pos = g_spelluser_e->target_object_id - 1;

		/* set the pointer to the target */
		g_spelltarget = get_hero(hero_pos);

		/* pointer to the armor of the target hero */
		p_armor = (struct inventory*)&get_spelltarget()->inventory[HERO_INVENTORY_SLOT_BODY];

		if ((p_armor->item_id != ITEM_NONE) && (rs_malus != 0)) {

			/* adjust rs_malus such that the RS of the worn body armor won't be negative */
			if ((p_armor->rs_lost + rs_malus) > g_armors_table[g_itemsdat[p_armor->item_id].table_index].rs)
			{
				rs_malus = g_armors_table[g_itemsdat[p_armor->item_id].table_index].rs - p_armor->rs_lost;
			}

			p_armor->rs_lost = p_armor->rs_lost + rs_malus;

			get_spelltarget()->rs_bonus1 = get_spelltarget()->rs_bonus1 - rs_malus;
		}

		/* AT - level / 2 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), (Bit8u*)&get_spelltarget()->at_weapon[get_spelltarget()->w_type], -level / 2, (signed char)hero_pos);

		/* PA - level / 2 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), (Bit8u*)&get_spelltarget()->pa_weapon[get_spelltarget()->w_type], -level / 2, (signed char)hero_pos);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

		/* subtract RS malus */
		g_spelltarget_e->rs = g_spelltarget_e->rs - rs_malus;

		/* AT - level / 2 */
		g_spelltarget_e->at = g_spelltarget_e->at - level / 2;

		/* PA - level / 2 */
		g_spelltarget_e->pa = g_spelltarget_e->pa - level / 2;
	}

	/* terminate output string */
	*g_dtp2 = '\0';
	g_monster_spell_ae_cost = damage;
}

void mspell_plumbumbarum(void)
{
	signed int slot;
	signed int hero_pos;

	if (g_spelluser_e->target_object_id < 10) {
		/* target is a hero */

		/* get the position of the target hero */
		hero_pos = g_spelluser_e->target_object_id - 1;

		/* set the pointer to the target */
		g_spelltarget = get_hero(hero_pos);

		/* AT - 3 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), (Bit8u*)&get_spelltarget()->at_weapon[get_spelltarget()->w_type], -3, (signed char)hero_pos);

		/* prepare message */
		sprintf(g_dtp2, get_tx(94), get_spelltarget()->alias);
	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

		/* AT - 3 */
		g_spelltarget_e->at = g_spelltarget_e->at - 3;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(95), (Bit8u*)GUI_names_grammar((signed short)0x8001, g_spelltarget_e->mon_id, 1));
	}
}

void mspell_saft_kraft(void)
{
	/* set the pointer to the target */
	g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

	/* AT + 5 */
	g_spelltarget_e->at = g_spelltarget_e->at + 5;

	/* PA - 5 */
	g_spelltarget_e->pa = g_spelltarget_e->pa - 5;

	/* damage + 5 */
	g_spelltarget_e->saftkraft = g_spelltarget_e->saftkraft + 5;

	/* set spellcost */
	g_monster_spell_ae_cost = random_schick(20);
}

void mspell_armatrutz(void)
{
	signed int i;
	signed int rs_bonus;

	i = 0;
	while ((i * i) < g_spelluser_e->ae) {
		i++;
	}

	rs_bonus = random_interval(1, i);

	/* set spellcost */
	g_monster_spell_ae_cost = rs_bonus * rs_bonus;

	/* RS + rs_bonus */
	g_spelluser_e->rs = g_spelluser_e->rs + rs_bonus;
}

void mspell_paralue(void)
{
	if (g_spelluser_e->target_object_id >= 10) {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->target_object_id - 10];

		/* set 'petrified' flag */
		g_spelltarget_e->flags.petrified = 1;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(103), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	} else {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget = get_hero(g_spelluser_e->target_object_id - 1);

		/* set the flag */
		get_spelltarget()->flags.petrified = 1;

		/* prepare message */
		sprintf(g_dtp2, get_tx(103), get_spelltarget()->alias);
	}
}

#if !defined(__BORLANDC__)
}
#endif
