/*
 *	Rewrite of DSA1 v3.02_de functions of seg041 (fight)
 *	Functions rewritten: 9/9 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg041.cpp
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg038.h"
#include "seg039.h"
#include "seg041.h"
#include "seg097.h"
#include "seg103.h"
#include "seg105.h"
#include "seg106.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static signed short g_force_weapons[9] = {
	ITEM_KNUEPPEL,
	ITEM_MORGENSTERN,
	ITEM_STREITKOLBEN,
	ITEM_KAMPFSTAB,
	ITEM_PEITSCHE, /* wtf?? */
	ITEM_KRIEGSHAMMER,
	ITEM_HEXENBESEN,
	ITEM_ZAUBERSTAB,
	-1
}; // ds:0x615c, terminated with -1
 
static signed short g_knive_weapons[5] = {
	ITEM_MESSER,
	ITEM_DOLCH,
	ITEM_VULKANGLASDOLCH,
	ITEM_WOLFSMESSER,
	-1
}; // ds:0x616e, terminated with -1


#if 0
static signed short msg_counter;
#endif

/**
 * \brief   check if a range attack can be done
 *
 * \param   hero        the hero who attacks
 * \param   arg         0 = drop one unit, 1 = just check, 2 = check with output
 * \return              0 = no ammo / 1 = have ammo
 */
signed int range_attack_check_ammo(struct struct_hero *hero, const signed int arg)
{
	signed short right_hand;
	signed short left_hand;
	signed short retval;

	retval = 0;

	/* read the item ids from the hands */
	right_hand = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;
	left_hand = hero->inventory[HERO_INVENTORY_SLOT_LEFT_HAND].item_id;

	switch (right_hand) {
		case ITEM_SPEER:		/* Speer */
		case ITEM_WURFBEIL:		/* Wurfbeil */
		case ITEM_WURFSTERN:	/* Wurfstern */
		case ITEM_WURFAXT:		/* Wurfaxt */
		case ITEM_WURFMESSER:	/* Wurfmesser */
		case ITEM_SCHNEIDZAHN:		/* Schneidzahn */
		case ITEM_WURFDOLCH__MAGIC:/* Wurfdolch */
			/* Original-Bug: missing throwing weapon: ITEM_SPEER__MAGIC */
			{
				if (!arg) {

					if (g_fig_dropped_counter < 30) {
						/* potential Original-Bug: Only the item IDs are stored, but not the other item stats. Is this a problem?
						 * For example, magic_revealed for the ITEM_WURFDOLCH__MAGIC might get lost.
						 * Moreover, it would be nice to store the owner, to give it back the hero who used the ranged weapon. */
						g_fig_dropped_weapons[g_fig_dropped_counter] = right_hand;
						g_fig_dropped_counter++;
					}

					drop_item(hero, HERO_INVENTORY_SLOT_RIGHT_HAND, 1);

					if (left_hand == right_hand) {
						move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, HERO_INVENTORY_SLOT_LEFT_HAND, hero);
					}

				}
				retval = 1;
				break;
			}
		case ITEM_KURZBOGEN: 		/* Kurzbogen */
		case ITEM_LANGBOGEN:		/* Langbogen */
			{
				if (left_hand != ITEM_PFEIL) { /* Pfeil */
					if (arg != 2) {

						sprintf(g_dtp2, get_tx(8), hero->alias);
						GUI_output(g_dtp2);
					}

				} else {
					if (!arg) {
						drop_item(hero, HERO_INVENTORY_SLOT_LEFT_HAND, 1);
					}
					retval = 1;
				}
				break;
			}
		case ITEM_ARMBRUST:		/* Armbrust */
			{
				if (left_hand != ITEM_BOLTS) { /* Bolzen */
					if (arg != 2) {

						sprintf(g_dtp2, get_tx(9), hero->alias);
						GUI_output(g_dtp2);
					}
				} else {
					if (!arg) {
						drop_item(hero, HERO_INVENTORY_SLOT_LEFT_HAND, 1);
					}
					retval = 1;
				}
				break;
			}
		case ITEM_SCHLEUDER:	/* SCHLEUDER	/ SLING */
			{
#ifndef M302de_FEATURE_MOD
				/* sling does not work in the original game.
				 * there is no object with the id 999 */
				/* REMARK: That's true for technical reasons!
				 * The maximum value of an unsigned char is 255/0xff */
				if (left_hand != 999) {
					if (arg != 2) {

						sprintf(g_dtp2, get_tx(10), hero->alias);
						GUI_output(g_dtp2);
					}
				} else {
					if (!arg) {
						drop_item(hero, HERO_INVENTORY_SLOT_LEFT_HAND, 1);
					}
					retval = 1;
				}
#else
				/* Feature mod 3:
				 * Make the sling item usable, based on the following decisions:
				 * * No ammunition is needed (rationale: stones don't exist in the original game and are ubiquitos)
				 * * left hand may hold an item (like a shield) (rationale: https://de.wikipedia.org/wiki/Datei:Liber.jpg)
				 */
				retval = 1;
#endif
				break;
			}
	}
	return retval;
}

void FIG_output(char *str)
{
	if (*str != 0) {
		GUI_output(str);
	}
}

/**
 * \brief   clears the fight messages buffer
 */
void FIG_clear_msgs(void)
{
	memset(g_fig_msg_data, 0 , 20);
	g_fig_msg_counter = 0;
}

void FIG_add_msg(const signed int f_action, const signed int damage)
{
	g_fig_msg_data[g_fig_msg_counter].f_action = f_action;
	g_fig_msg_data[g_fig_msg_counter].damage = damage;
	if (g_fig_msg_counter < 4) {
		g_fig_msg_counter++;
	}
}

/**
 * \brief   damages an enemy
 *
 *          This function has some tweaks, dependent on the fight number.
 *
 * \param   enemy       pointer to the enemy
 * \param   damage      the damage
 * \param   flag        impact on 'renegade' flag. 0: not affected. 1: reset 'renegade' to 0 (monster will be hostile again)
 */
void FIG_damage_enemy(struct enemy_sheet *enemy, Bit16s damage, signed short preserve_renegade)
{
	signed short i;

	/* subtract the damage from the enemies LE */
	enemy->le -= damage;

	/* are the enemies LE lower than 0 */
	if (enemy->le <= 0) {

		enemy->flags.dead = 1;
		enemy->le = 0;

		if ((g_current_fight_no == FIGHTS_F126_08) && (enemy->mon_id == 0x38)) {

			/* slaying a special cultist */
			/* set a flag in the status area */
			gs_dng09_cultist_flag = 0;

		} else if ((g_current_fight_no == FIGHTS_F144) && (enemy->mon_id == 0x48) && !g_finalfight_tumult) {

			/* slaying the orc champion, ends the fight */
			g_in_fight = 0;

		} else if ((g_current_fight_no == FIGHTS_F064) && (enemy->mon_id == 0x46)) {

			/* slaying Gorah makes everyone flee except Heshthot */
			for (i = 0; i < 20; i++) {
				if (g_enemy_sheets[i].gfx_id != 0x1a) {
					g_enemy_sheets[i].flags.scared = 1;
				}
			}
		}
	}

	if (!preserve_renegade)
		enemy->flags.renegade = 0;
}

/*
 *	\param attack_hero	0 = the attacked one is a foe; 1 = the attacked one is a hero
 */
signed int FIG_get_hero_weapon_attack_damage(struct struct_hero* hero, struct struct_hero* target, const signed int attack_hero)
{
	signed short damage;
	signed short damage_mod;
	struct item_stats *item_p_rh;
	struct weapon_descr *weapon;
	const struct ranged_weapon_descr *p_rangedtab;
	signed short target_size;
	signed short right_hand;
	signed short beeline;
	signed short distance;
	signed int x_hero;
	signed int y_hero;
	signed int x_target;
	signed int y_target;
	signed short hero_idx;
	signed char enemy_gfx_id;
	struct enemy_sheet* enemy_p;
	signed short weapon_type;

	damage_mod = 0;

	if (attack_hero == 0) {
		enemy_p = (struct enemy_sheet*)target; /* TODO: to attack an enemy enemy_p should be used instead of target */
	}

	right_hand = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;

	item_p_rh = &g_itemsdat[right_hand];

	weapon_type = weapon_check(hero);

	if (weapon_type == -1) {
		/* not a weapon or a ranged weapon */
		weapon_type = FIG_get_range_weapon_type(hero);
	}

	/* now depending on the item in the right hand, <weapon_type> is
	 * -1: not a weapon or broken melee weapon (inluding broken WEAPON_TYPE_WAFFENLOS (i.e. ammunition), broken ITEM_ZAUBERSTAB and broken ITEM_KAMPFSTAB, but no other WEAPON_TYPE_SPEER)
	 *  0: non-broken knive weapon
	 *  1: non-broken force weapon (including ITEM_ZAUBERSTAB and ITEM_KAMPFSTAB)
	 *  2: other non-broken meelee weapon, including WEAPON_TYPE_WAFFENLOS (i.e. ammunition), but not WEAPON_TYPE_SPEER
	 *  3: any WEAPON_TYPE_SCHUSSWAFFE, broken or not
	 *  4: any WEAPON_TYPE_WURFWAFFE, broken or not
	 *  5: any weapon of type WEAPON_TYPE_SPEER with the exception of ITEM_KAMPFSTAB and ITEM_ZAUBERSTAB, broken or not
	 */

	if (weapon_type != -1) {

		weapon = &g_weapons_table[item_p_rh->table_index];

		damage = dice_roll(weapon->damage_d6, 6, weapon->damage_const);

		if (weapon->ranged_index != -1) {

			/* weapon does ranged damage */

			hero_idx = get_hero_index(hero);

			FIG_search_obj_on_cb(hero_idx + 1, &x_hero, &y_hero);
			FIG_search_obj_on_cb(hero->target_object_id, &x_target, &y_target);

			beeline = calc_beeline(x_hero, y_hero, x_target, y_target);

			if (beeline <= 2) {
				distance = 0;
			} else if (beeline <= 4) {
				distance = 1;
			} else if (beeline <= 6) {
				distance = 2;
			} else if (beeline <= 9) {
				distance = 3;
			} else if (beeline <= 15) {
				distance = 4;
			} else if (beeline <= 20) {
				distance = 5;
			} else {
				distance = 6;
			}

			p_rangedtab = &g_ranged_weapons_table[weapon->ranged_index];

			if (attack_hero) {
				if (target->typus == HERO_TYPE_ZWERG) {
					/* ZWERG / DWARF */
					target_size = 2;
				} else {
					target_size = 3;
				}
			} else {
				/* size of the enemy */
				/* TODO: use enemy_p instead of a casted target */
				target_size = ((struct enemy_sheet*)target)->size;
			}

			/* Original-Bug: For ITEM_SPEER and ITEM_SPEER__MAGIC, a test on TA_SCHUSSWAFFEN will be performed */
			damage_mod = (test_skill(hero,
						(item_p_rh->subtype == WEAPON_TYPE_WURFWAFFE ? TA_WURFWAFFEN : TA_SCHUSSWAFFEN),
						p_rangedtab->base_handicap + 2 * distance - 2 * target_size) > 0) ?
					g_ranged_weapons_table[weapon->ranged_index].damage_modifier[distance] : -damage;

			if (damage_mod != 0) { /* test is redundant */
				damage += damage_mod;
			}
			/* If the skill test was not successful, damage == 0 by now. */

		} else {
			/* weapon does melee damage */

			damage_mod = hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod - weapon->damage_kk_bonus;
			if (damage_mod > 0) {
				damage += damage_mod;
			}
		}
	} else {
		damage = random_schick(6);
	}

	if (!attack_hero) {

		enemy_gfx_id = enemy_p->gfx_id;

		if ((right_hand == ITEM_SAEBEL__MAGIC) && (enemy_gfx_id == 0x1c || enemy_gfx_id == 0x22)) {

			/* magic SABRE gives Damage + 1 to SKELETONS and ZOMBIES */
			damage++;

		} else if (right_hand == ITEM_KUKRISDOLCH) {

			/* KUKRIS DAGGER / KUKRISDOLCH */
			/* Interesting */
			damage = 1000;

			/* drop the KUKRISDOLCH and equip a normal DOLCH / DAGGER */
			drop_item(hero, HERO_INVENTORY_SLOT_RIGHT_HAND, 1);
			give_hero_new_item(hero, ITEM_DOLCH, 1, 1); /* TODO: what if no free knapsack slot? */
			move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, get_item_pos(hero, ITEM_DOLCH), hero);

		} else if (right_hand == ITEM_KUKRIS_MENGBILAR) {

			/* KUKRISMENGBILAR */
			/* Interesting */
			damage = 1000;

			/* drop the KUKRISMENGBILAR and equip a normal MENGBILAR  */
			drop_item(hero, HERO_INVENTORY_SLOT_RIGHT_HAND, 1);
			give_hero_new_item(hero, ITEM_MENGBILAR, 1, 1); /* TODO: what if no free knapsack slot? */
			move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, get_item_pos(hero, ITEM_MENGBILAR), hero);

		} else if ((right_hand == ITEM_SILBERSTREITKOLBEN) && (enemy_gfx_id == 0x1c)) {

			/* SILVER MACE / SILBERSTREITKOLBEN gives Damage + 4 to SKELETONS */
			damage += 4;

		} else if ((right_hand == ITEM_GRIMRING) && (enemy_gfx_id == 0x18)) {

			/* DAS SCHWERT GRIMRING gives Damage + 5 to ORCS */
			damage += 5;
		}
	}

	damage += hero->saftkraft;

	if (damage > 0) {

		if (hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.poison_expurgicum) {

			damage += dice_roll(1, 6, 2); /* D6 + 2 damage */
			hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.poison_expurgicum = 0;
		}

		if (hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.poison_vomicum) {

			damage += dice_roll(1, 20, 5); /* D20 + 5 damage */
			hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.poison_vomicum = 0;
		}

		if (hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type != POISON_TYPE_NONE) {

			if (hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type == POISON_TYPE_ANGSTGIFT) {

				enemy_p->flags.scared = 1;
				enemy_p->flags.renegade = 0;

			} else {
				/* the following line is the source for the totally excessive and unbalanced poison damage */
				damage += 10 * g_poison_prices[hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type];
			}

			hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges--;

			if (!hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].nr_poison_charges) {
				hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].poison_type = POISON_TYPE_NONE;
			}
		}
	}

	if (hero->ecliptifactus_timer != 0) {
		damage *= 2;
	}

	if (gs_tevent071_orcstatue && (hero->typus == HERO_TYPE_ZWERG) && (attack_hero == 0) && (enemy_p->gfx_id == 0x18))
	{
		damage++;
	}

	if (!attack_hero) {

		damage -= enemy_p->rs;

		if (enemy_p->flags.petrified) {
			damage = 0;
		}

		if (enemy_p->magic && !hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.magic) {
			damage = 0;
		}

		if (enemy_p->le < damage) {
			damage = enemy_p->le + 1;
		}
	} else {
		damage -= target->rs_bonus1;

		if (target->flags.petrified) {
			damage = 0;
		}

		if (target->le < damage) {
			damage = target->le + 1;
		}
	}

	return damage;
}

/**
 * \brief   calculates attack damage from an enemy
 *
 * \param   attacker    the enemy which attacks
 * \param   target    the enemy/hero which gets attacked
 * \param   is_enemy    1 = if target is an enemy / 0 = target is a hero
 *
 */

signed short FIG_get_enemy_attack_damage(struct enemy_sheet *attacker, struct enemy_sheet *target, signed short is_enemy)
{
	signed short pos;
	struct struct_hero *hero;

	signed short damage;
	signed short dice;

	dice = attacker->dam1;

	if ((attacker->dam2 != 0) && (random_schick(100) < 50)) {
		dice = attacker->dam2;
	}

	damage = dice_template(dice);

	if (!is_enemy) {

		/* the target is a hero */
		hero = (struct struct_hero*)target;

		/* subtract RS */
		damage -= hero->rs_bonus1;

		/* armor bonus against skeletons and zombies */
		if (hero->inventory[HERO_INVENTORY_SLOT_BODY].item_id == ITEM_KETTENHEMD__CURSED && ((attacker->gfx_id == 0x22) || (attacker->gfx_id == 0x1c)))
		{
			damage -= 3;
		}

		/* get position of Totenkopfguertel/Skullbelt */

		if ( ((pos = get_item_pos(hero, ITEM_TOTENKOPFGUERTEL)) != -1) && ((attacker->gfx_id == 0x22) || (attacker->gfx_id == 0x1c))) {

			/* no damage for the hero who has it */
			damage = 0;

			/* 4% chance to loose this item */
			if (random_schick(100) < 5) {
				drop_item(hero, pos, 1);
				GUI_output(get_tx(11));
			}
		}

		/* no damage if the hero is petrified */
		if (hero->flags.petrified)
			damage = 0;
	} else {
		/* the target is an enemy */

		/* subtract RS */
		damage -= target->rs;

		if (target->flags.petrified)
			damage = 0;

		/* check if the target is immune
		 * against non-magicial weapons */
		if (target->magic)
			damage = 0;
	}

	/* damage bonus */
	damage += attacker->saftkraft;

	/* half damage */
	if (attacker->weapon_broken)
		damage /= 2;

	return damage;
}

void clear_anisheets(void)
{
	signed short i;

	for (i = 0; i < 8; i++)
		memset(&g_fig_anisheets[i], -1, 0xf3);

	/* That would be better */
	/* memset(&g_fig_anisheets[0], -1, 0xf3 * 8); */
}

/**
 * \brief   check if a hero is equipped with a vaild weapon
 *
 * \param   hero        pointer to hero
 *
 * \return
 *	-1 = not a weapon or a broken weapon or a ranged weapon (the latter including WEAPON_TYPE_SPEER, but not ITEM_ZAUBERSTAB or ITEM_KAMPFSTAB),
 *	0 = non-broken knive weapon,
 *	1 = non-broken force weapon (includes ITEM_ZAUBERSTAB and ITEM_KAMPFSTAB),
 *	2 = any other non-broken melee weapon, including WEAPON_TYPE_WAFFENLOS (i.e. ammunition), but no WEAPON_TYPE_SPEER
 */
signed int weapon_check(struct struct_hero *hero)
{
	struct item_stats *item_p;

	signed int item_id;
	signed int retval;

	/* get the ID of the equipped weapon */
	item_id = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;

	item_p = &g_itemsdat[item_id];

	if (!item_p->flags.weapon || hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].flags.broken ||
		(item_p->flags.weapon &&
			((item_p->subtype == WEAPON_TYPE_SCHUSSWAFFE) ||
			(item_p->subtype == WEAPON_TYPE_WURFWAFFE) ||
			/* TODO: according to original DSA2/3 rules, weapon type SPEER is a melee discipline. */
			(item_p->subtype == WEAPON_TYPE_SPEER &&
			(item_id != ITEM_ZAUBERSTAB) && (item_id != ITEM_KAMPFSTAB)))))
	{
		retval = -1;
	} else {
		if (is_in_word_array(item_id, g_force_weapons)) {

			retval = 1;

		} else if (is_in_word_array(item_id, g_knive_weapons)) {

			retval = 0;

		} else {

			retval = 2;
		}
	}

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
