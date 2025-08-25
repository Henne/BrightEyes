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

void MON_do_spell_damage(signed short damage)
{
	if (damage > 0) {

		if (g_spelluser_e->enemy_id < 10) {
			/* target is a hero */

			/* set the pointer to the target */
			g_spelltarget = get_hero(g_spelluser_e->enemy_id - 1);

			/* do the damage */
			sub_hero_le(get_spelltarget(), damage);

			/* add a fight message */
			FIG_add_msg(8, damage);

			/* check if someone died */
			if (hero_dead(get_spelltarget())) {
				g_defender_dead = 1;
			}

		} else {
			/* target is a monster */

			/* set the pointer to the target */
			g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

			/* do the damage */
			FIG_damage_enemy(g_spelltarget_e, damage, 1);

			/* add a fight message */
			FIG_add_msg(11, damage);

			/* check if someone died */
			if (g_spelltarget_e->flags1.dead) {
				g_defender_dead = 1;
			}
		}
	}
}

/* unused */
signed short MON_get_target_PA(void)
{
	if (g_spelluser_e->enemy_id < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget =  get_hero(g_spelluser_e->enemy_id - 1);

		/* calc and return PA-value */
		return host_readbs(get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE))
			- host_readbs(get_spelltarget() + HERO_ATTACK_TYPE);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

		/* calc and return PA-value */
		return g_spelltarget_e->pa;
	}
}

/* unused */
signed short MON_get_target_RS(void)
{
	if (g_spelluser_e->enemy_id < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget = get_hero(g_spelluser_e->enemy_id - 1);

		/* return RS-value */
		return host_readbs(get_spelltarget() + HERO_RS_BONUS1);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

		/* return PA-value */
		return g_spelltarget_e->rs;
	}
}

signed short MON_get_spell_cost(signed short mspell_id, signed short flag)
{
	signed char cost;

	cost = g_mon_spell_descriptions[mspell_id].ae_cost;

	if (flag != 0) {

		cost = (cost == -1) ? random_interval(5, 10) : cost / 2;
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
signed short MON_test_attrib3(struct enemy_sheet *monster, signed short attrib1, signed short attrib2, signed short attrib3, signed char handicap)
/* called only from a single position, in MON_test_skill(..) */
{
#ifndef M302de_FEATURE_MOD
	/* Feature mod 6: The implementation of the skill test logic differs from the original DSA2/3 rules.
	 * It is sometimes called the 'pool' variant, where '3W20 + handicap' is compared to the sum of the attributes.
	 * It is significantly easier than the original rule, where each individuall roll must be at most the corresponding attribute,
	 * where positive handicap must be used up during the process, and negative handicap may be used for compensation. */

	signed short randval;
	signed short attr_sum;

	randval = dice_roll(3, 20, handicap);

	attr_sum = monster->attrib[2 * attrib1] + monster->attrib[2 * attrib2] + monster->attrib[2 * attrib3];

	return attr_sum - randval + 1;
#else
	/* Here, the original DSA2/3 skill test logic is implemented.
	 * WARNING: This makes skill tests and spell casting (on both sides), and thus the game, significantly harder!
	 * Note that we are not implementing the DSA4 rules, where tests with a positive handicap are yet harder. */
	signed short i;
	signed short tmp;
	signed short nr_rolls_1 = 0;
	signed short nr_rolls_20 = 0;
	signed short fail = 0;
	signed char attrib [3];

	attrib[0] = monster->attrib[2 * attrib1];
	attrib[1] = monster->attrib[2 * attrib2];
	attrib[2] = monster->attrib[2 * attrib3];

#if !defined(__BORLANDC__)
	D1_INFO(" (%s %d/%s %d/%s %d) ->",
		names_attrib[attrib1],
		attrib[0],
		names_attrib[attrib2],
		attrib[1],
		names_attrib[attrib3],
		attrib[2]
	);
#endif

	for (i = 0; i < 3; i++) {

		tmp = random_schick(20);

#if !defined(__BORLANDC__)
		D1_INFO(" W20 = %d;", tmp);
#endif

		if (tmp == 20) {
			if (++nr_rolls_20 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> UNGLUECKLICH nicht bestanden\n");
#endif
				return -99;
			}
		}

		if (tmp == 1) {
			if (++nr_rolls_1 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> GLUECKLICH bestanden\n");
#endif
				return 99;
			}
		}

		if (!fail) {
			tmp -= attrib[i];
			if (handicap <= 0) {
				if (tmp > 0) {
					if (tmp > -handicap) {
						fail = 1;
#if !defined(__BORLANDC__)
						D1_INFO(" zu hoch!");
#endif
					} else  {
						handicap += tmp;
					}
				}
			}
			if (handicap > 0) {
				if (tmp > 0) {
					fail = 1;
#if !defined(__BORLANDC__)
					D1_INFO(" zu hoch!");
#endif
				} else {
					handicap += tmp;
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
signed short MON_test_skill(struct enemy_sheet *monster, signed short mspell_id, signed char handicap)
{
	struct mon_spell_description *desc;

	desc = &g_mon_spell_descriptions[mspell_id];

	/* depends on MR */
	if (desc->vs_mr) {

		/* add MR */
		handicap += (monster->enemy_id >= 10 ?	g_enemy_sheets[monster->enemy_id].mr : host_readbs(get_hero(monster->enemy_id - 1) + HERO_MR));
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

void MON_sub_ae(struct enemy_sheet *monster, signed short ae)
{
	if (!monster->flags1.dead) {

		monster->ae -= ae;

		if (monster->ae < 0) {
			monster->ae = 0;
		}
	}
}

signed short MON_cast_spell(struct enemy_sheet* monster, signed char handicap)
{
	signed short l_si;
	signed short l_di;
	signed short cost;
	void (*func)(void);
	signed short tx_file_bak;

	l_si = monster->cur_spell;

	if (l_si > 0) {

		cost = MON_get_spell_cost(l_si, 0);

		/* check AE */
		if (monster->ae < cost) {
			return -1;
		}

		g_spelltest_result = MON_test_skill(monster, l_si, handicap);

		if ((g_spelltest_result <= 0) || (gs_ingame_timers[INGAME_TIMER_RONDRA_NO_SPELLS] > 0)) {

			/* spell failed */
			MON_sub_ae(monster, MON_get_spell_cost(l_si, 1));

			return 0;

		} else {

			g_spelluser_e = monster;

			g_monster_spell_ae_cost = -1;

			/* terminate output string */
			*g_dtp2 = '\0';

			tx_file_bak = g_tx_file_index;

			load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

			func = g_mon_spellhandlers[l_si];
			func();

			if ((tx_file_bak != -1) && (tx_file_bak != ARCHIVE_FILE_SPELLTXT_LTX)) {
				load_tx(tx_file_bak);
			}

			l_di = 1;

			if (!g_monster_spell_ae_cost) {
				l_di = -1;
			} else if (g_monster_spell_ae_cost == -2) {
				MON_sub_ae(monster, MON_get_spell_cost(l_si, 1));
				l_di = 0;
			} else if (g_monster_spell_ae_cost != -1) {
				MON_sub_ae(monster, g_monster_spell_ae_cost);
			} else {
				MON_sub_ae(monster, cost);
			}

			return l_di;
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
	g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

	if (g_spelltarget_e->flags1.petrified) {

		/* set the spellcosts */
		g_monster_spell_ae_cost = 5 * random_schick(10);

		if (g_spelluser_e->ae < g_monster_spell_ae_cost) {
			/* if not enough AE, all AE will be consumed, without further effect */
			g_monster_spell_ae_cost = g_spelluser_e->ae;
		} else {
			/* unset 'petrified' flag */
			g_spelltarget_e->flags1.petrified = 0;

			/* prepare message */
			sprintf(g_dtp2,	get_tx(114), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
		}

	} else if (g_spelltarget_e->flags1.mushroom) {

		/* set the spellcosts */
		g_monster_spell_ae_cost = 5 * random_schick(10);

		if (g_spelluser_e->ae < g_monster_spell_ae_cost) {
			/* if not enough AE, all AE will be consumed, without further effect */
			g_monster_spell_ae_cost = g_spelluser_e->ae;
		} else {
			/* unset 'mushroom' flag */
			g_spelltarget_e->flags1.mushroom = 0;

			ds_writew(MSPELL_AWAKE_FLAG, 1);
		}
	} else {
		g_monster_spell_ae_cost = 2;
	}
}

void mspell_bannbaladin(void)
{
	/* set pointer to hero target */
	g_spelltarget = get_hero(g_spelluser_e->enemy_id - 1);

	/* set the flag */
	or_ptr_bs(get_spelltarget() + HERO_FLAGS2, 0x08); /* set 'tame' flag */

	/* prepare message */
	sprintf(g_dtp2, get_tx(115), get_spelltarget() + HERO_NAME2);
}

void mspell_boeser_blick(void)
{
	/* set pointer to hero target */
	g_spelltarget = get_hero(g_spelluser_e->enemy_id - 1);

	/* set the flag */
	or_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0x20); /* set 'renegade' flag */

	/* prepare message */
	sprintf(g_dtp2, get_tx(116), get_spelltarget() + HERO_NAME2);
}

void mspell_horriphobus(void)
{
	/* set pointer to hero target */
	g_spelltarget = get_hero(g_spelluser_e->enemy_id - 1);

	/* set the flag */
	or_ptr_bs(get_spelltarget() + HERO_FLAGS2, 0x01); /* set 'scared' flag */
	and_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0xdf); /* unset 'renegade' flag */

	/* prepare message */
	sprintf(g_dtp2, get_tx(117), get_spelltarget() + HERO_NAME2);
}

void mspell_axxeleratus(void)
{
	/* set pointer to monster target */
	g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

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
	signed short le;

	/* set pointer to monster target */
	g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

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
	if (g_spelluser_e->enemy_id < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget = get_hero(g_spelluser_e->enemy_id - 1);

		/* set blitz timer to 3 rounds */
		host_writeb(get_spelltarget() + HERO_BLIND, 3);

		/* prepare message */
		sprintf(g_dtp2, get_tx(86), get_spelltarget() + HERO_NAME2);
	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

		/* set blitz timer to 3 rounds */
		g_spelltarget_e->blind = 3;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(85), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	}
}

void mspell_eisenrost(void)
{
	signed short id;

	if (g_spelluser_e->enemy_id < 10) {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget = get_hero(g_spelluser_e->enemy_id - 1);

		id = host_readws(get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);

		if (!id) {
			/* target hero has no weapon */
			g_monster_spell_ae_cost = 2;

		} else if (!inventory_broken(get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY)) {

			if (host_readbs(get_spelltarget() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_BF)) > 0) {

				or_ptr_bs(get_spelltarget() + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_FLAGS), 1); /* set 'broken' flag */

				/* prepare message */
				sprintf(g_dtp2,	get_tx(92), (Bit8u*)GUI_names_grammar((signed short)0x8000, id, 0), get_spelltarget() + HERO_NAME2);

			} else {
				g_monster_spell_ae_cost = -2;
			}
		}

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

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
	signed short damage;

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
	signed short damage;
	signed short level;
	signed short rs_malus;
	signed short hero_pos;
	signed short slot;
	Bit8u *p_armor;

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

	if (g_spelluser_e->enemy_id < 10) {
		/* target is a hero */

		/* get the position of the target hero */
		hero_pos = g_spelluser_e->enemy_id - 1;

		/* set the pointer to the target */
		g_spelltarget = get_hero(hero_pos);

		/* pointer to the armor of the target hero */
		p_armor = get_spelltarget() + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY;

		if ((host_readws(p_armor + INVENTORY_ITEM_ID) != ITEM_NONE) && (rs_malus != 0)) {

			/* adjust rs_malus such that the RS of the worn body armor won't be negative */
			if ((host_readbs(p_armor + INVENTORY_RS_LOST) + rs_malus) > ds_readbs(ARMORS_TABLE + ARMOR_STATS_RS + SIZEOF_ARMOR_STATS * host_readbs(ITEM_STATS_TABLE_INDEX + get_itemsdat(host_readws(p_armor + INVENTORY_ITEM_ID)))))
			{
				rs_malus = ds_readbs(ARMORS_TABLE + ARMOR_STATS_RS + SIZEOF_ARMOR_STATS * host_readbs(ITEM_STATS_TABLE_INDEX + get_itemsdat(host_readws(p_armor + INVENTORY_ITEM_ID))))
						- host_readbs(p_armor + INVENTORY_RS_LOST);
			}

			host_writeb(p_armor + INVENTORY_RS_LOST, host_readbs(p_armor + INVENTORY_RS_LOST) + rs_malus);
			host_writeb(get_spelltarget() + HERO_RS_BONUS1, host_readbs(get_spelltarget() + HERO_RS_BONUS1) - rs_malus);
		}

		/* AT - level / 2 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE), -level / 2, (signed char)hero_pos);

		/* PA - level / 2 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE), -level / 2, (signed char)hero_pos);

	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

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
	signed short slot;
	signed short hero_pos;

	if (g_spelluser_e->enemy_id < 10) {
		/* target is a hero */

		/* get the position of the target hero */
		hero_pos = g_spelluser_e->enemy_id - 1;

		/* set the pointer to the target */
		g_spelltarget = get_hero(hero_pos);

		/* AT - 3 */
		slot = get_free_mod_slot();
		set_mod_slot(slot, HOURS(1), get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE), -3, (signed char)hero_pos);

		/* prepare message */
		sprintf(g_dtp2, get_tx(94), get_spelltarget() + HERO_NAME2);
	} else {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

		/* AT - 3 */
		g_spelltarget_e->at = g_spelltarget_e->at - 3;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(95), (Bit8u*)GUI_names_grammar((signed short)0x8001, g_spelltarget_e->mon_id, 1));
	}
}

void mspell_saft_kraft(void)
{
	/* set the pointer to the target */
	g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

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
	signed short i;
	signed short rs_bonus;

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
	if (g_spelluser_e->enemy_id >= 10) {
		/* target is a monster */

		/* set the pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[g_spelluser_e->enemy_id - 10];

		/* set 'petrified' flag */
		g_spelltarget_e->flags1.petrified = 1;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(103), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	} else {
		/* target is a hero */

		/* set the pointer to the target */
		g_spelltarget = get_hero(g_spelluser_e->enemy_id - 1);

		/* set the flag */
		or_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0x04); /* set 'petrified' flag */

		/* prepare message */
		sprintf(g_dtp2, get_tx(103), get_spelltarget() + HERO_NAME2);
	}
}

#if !defined(__BORLANDC__)
}
#endif
