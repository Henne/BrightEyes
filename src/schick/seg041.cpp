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

#if 0
/* FIG_MSG_COUNTER */
static signed short msg_counter;
#endif

/**
 * \brief   check if a range attack can be done
 *
 * \param   hero        the hero who attacks
 * \param   arg         0 = drop one unit, 1 = just check, 2 = check with output
 * \return              0 = no ammo / 1 = have ammo
 */
signed short range_attack_check_ammo(Bit8u *hero, signed short arg)
{
	signed short right_hand;
	signed short left_hand;
	signed short retval;

	retval = 0;

	/* read the item ids from the hands */
	right_hand = host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);
	left_hand = host_readws(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_LEFT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);

	switch (right_hand) {
		case ITEM_SPEAR:		/* Speer */
		case ITEM_FRANCESCA:		/* Wurfbeil */
		case ITEM_THROWING_STAR:	/* Wurfstern */
		case ITEM_THROWING_AXE:		/* Wurfaxt */
		case ITEM_THROWING_KNIFE:	/* Wurfmesser */
		case ITEM_SCHNEIDZAHN:		/* Schneidzahn */
		case ITEM_THROWING_DAGGER_MAGIC:/* Wurfdolch */
			/* Original-Bug: missing throwing weapon: ITEM_SPEAR_MAGIC */
			{
				if (!arg) {

					if (g_fig_dropped_counter < 30) {
						/* potential Original-Bug: Only the item IDs are stored, but not the other item stats. Is this a problem?
						 * For example, magic_revealed for the ITEM_THROWING_DAGGER_MAGIC might get lost.
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
		case ITEM_SHORTBOW: 		/* Kurzbogen */
		case ITEM_LONGBOW:		/* Langbogen */
			{
				if (left_hand != ITEM_ARROWS) { /* Pfeil */
					if (arg != 2) {

						sprintf(g_dtp2, get_tx(8), (char*)hero + HERO_NAME2);
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
		case ITEM_CROSSBOW:		/* Armbrust */
			{
				if (left_hand != ITEM_BOLTS) { /* Bolzen */
					if (arg != 2) {

						sprintf(g_dtp2, get_tx(9), (char*)hero + HERO_NAME2);
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
		case ITEM_SLING:	/* SCHLEUDER	/ SLING */
			{
#ifndef M302de_FEATURE_MOD
				/* sling does not work in the original game.
				 * there is no object with the id 999 */
				if (left_hand != 999) {
					if (arg != 2) {

						sprintf(g_dtp2, get_tx(10), (char*)hero + HERO_NAME2);
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
	memset(p_datseg + FIG_MSG_DATA, 0 , 20);
	g_fig_msg_counter = 0;
}

void FIG_add_msg(unsigned short f_action, unsigned short damage)
{
	ds_writew(FIG_MSG_DATA + 4 * g_fig_msg_counter, f_action);
	ds_writew(FIG_MSG_DATA + 2 + 4 * g_fig_msg_counter, damage);
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
 * \param   flag        impact on 'renegade' flag. 0: not affacted. 1: reset 'renegade' to 0 (monster will be hostile again)
 */
void FIG_damage_enemy(Bit8u *enemy, Bit16s damage, signed short preserve_renegade)
{
	signed short i;

	/* subtract the damage from the enemies LE */
	sub_ptr_ws(enemy + ENEMY_SHEET_LE, damage);

	/* are the enemies LE lower than 0 */
	if (host_readws(enemy + ENEMY_SHEET_LE) <= 0) {
		or_ptr_bs(enemy + ENEMY_SHEET_FLAGS1, 1); /* set 'dead' flag */
		host_writew(enemy + ENEMY_SHEET_LE, 0); /* set LE to 0 */

		if ((g_current_fight_no == FIGHTS_F126_08) && (host_readb(enemy) == 0x38)) {
			/* slaying a special cultist */
			/* set a flag in the status area */
			ds_writeb(DNG09_CULTIST_FLAG, 0);

		} else if ((g_current_fight_no == FIGHTS_F144) &&
				(host_readb(enemy) == 0x48) &&
				!ds_readbs(FINALFIGHT_TUMULT))
		{
			/* slaying the orc champion, ends the fight */
				g_in_fight = 0;

		} else if ((g_current_fight_no == FIGHTS_F064) && (host_readb(enemy) == 0x46)) {

			/* slaying Gorah makes everyone flee except Heshthot */
			for (i = 0; i < 20; i++) {
#if !defined(__BORLANDC__)
				if (ds_readb(ENEMY_SHEETS + ENEMY_SHEET_GFX_ID + i * SIZEOF_ENEMY_SHEET) != 26)
					or_ds_bs((ENEMY_SHEETS + ENEMY_SHEET_FLAGS2) + i * SIZEOF_ENEMY_SHEET, 4); /* set 'scared' flag */
#else
				if ( ((struct enemy_sheets*)((Bit8u*)(MK_FP(datseg, ENEMY_SHEETS))))[i].gfx_id != 0x1a)
					((struct enemy_sheets*)((Bit8u*)(MK_FP(datseg, ENEMY_SHEETS))))[i].flags2.scared = 1;
#endif
			}
		}
	}

	if (!preserve_renegade)
		and_ptr_bs(enemy + ENEMY_SHEET_FLAGS2, 0xfd); /* unset 'renegade' flag */
}

/*
 *	\param attack_hero	0 = the attacked one is a foe; 1 = the attacked one is a hero
 */

signed short FIG_get_hero_weapon_attack_damage(Bit8u* hero, Bit8u* target, signed short attack_hero)
{
	signed short damage;
	signed short damage_mod;
	Bit8u* item_p_rh;
	Bit8u* p_weapontab;
	Bit8u* p_rangedtab;
	signed short target_size;
	signed short right_hand;
	signed short beeline;
	signed short distance;
	signed short x_hero;
	signed short y_hero;
	signed short x_target;
	signed short y_target;
	signed short hero_idx;
	signed char enemy_gfx_id;
	Bit8u* enemy_p;
	signed short weapon_type;

	damage_mod = 0;

	if (attack_hero == 0) {
		enemy_p = target;
	}

	right_hand = host_readw(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);

	item_p_rh = get_itemsdat(right_hand);

	weapon_type = weapon_check(hero);

	if (weapon_type == -1) {
		/* not a weapon or a ranged weapon */
		weapon_type = FIG_get_range_weapon_type(hero);
	}

	/* now depending on the item in the right hand, <weapon_type> is
	 * -1: not a weapon or broken melee weapon (inluding broken WEAPON_TYPE_WAFFENLOS (i.e. ammunition), broken ITEM_MAGIC_WAND and broken ITEM_QUARTERSTAFF, but no other WEAPON_TYPE_SPEER)
	 *  0: non-broken knive weapon
	 *  1: non-broken force weapon (including ITEM_MAGIC_WAND and ITEM_QUARTERSTAFF)
	 *  2: other non-broken meelee weapon, including WEAPON_TYPE_WAFFENLOS (i.e. ammunition), but not WEAPON_TYPE_SPEER
	 *  3: any WEAPON_TYPE_SCHUSSWAFFE, broken or not
	 *  4: any WEAPON_TYPE_WURFWAFFE, broken or not
	 *  5: any weapon of type WEAPON_TYPE_SPEER with the exception of ITEM_QUARTERSTAFF and ITEM_MAGIC_WAND, broken or not
	 */

	if (weapon_type != -1) {

		p_weapontab = p_datseg + WEAPONS_TABLE + host_readbs(item_p_rh + ITEM_STATS_TABLE_INDEX) * SIZEOF_WEAPON_STATS;

		damage = dice_roll(host_readbs(p_weapontab + WEAPON_STATS_DAMAGE_D6), 6, host_readbs(p_weapontab + WEAPON_STATS_DAMAGE_CONSTANT));

		if (host_readbs(p_weapontab + WEAPON_STATS_RANGED_INDEX) != -1) {
			/* weapon does ranged damage */

			hero_idx = get_hero_index(hero);

			FIG_search_obj_on_cb(hero_idx + 1, &x_hero, &y_hero);
			FIG_search_obj_on_cb(host_readbs(hero + HERO_ENEMY_ID), &x_target, &y_target);

#if !defined(__BORLANDC__)
			/* BE-fix */
			x_hero = host_readws((Bit8u*)&x_hero);
			y_hero = host_readws((Bit8u*)&y_hero);
			x_target = host_readws((Bit8u*)&x_target);
			y_target = host_readws((Bit8u*)&y_target);
#endif
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

			p_rangedtab = p_datseg + RANGED_WEAPONS_TABLE + host_readbs(p_weapontab + WEAPON_STATS_RANGED_INDEX) * SIZEOF_RANGED_WEAPON_STATS;

			if (attack_hero != 0) {
				if (host_readbs(target + HERO_TYPE) == HERO_TYPE_DWARF) {
					/* ZWERG / DWARF */
					target_size = 2;
				} else {
					target_size = 3;
				}
			} else {
				/* size of the enemy */
				target_size = host_readbs(target + ENEMY_SHEET_SIZE);
			}

			damage_mod = (test_skill(hero,
					/* Original-Bug: For ITEM_SPEAR and ITEM_SPEAR_MAGIC, a test on TA_SCHUSSWAFFEN will be performed */
						(host_readbs(item_p_rh + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_WURFWAFFE ? TA_WURFWAFFEN : TA_SCHUSSWAFFEN),
						host_readbs(p_rangedtab + RANGED_WEAPON_STATS_BASE_HANDICAP) + 2 * distance - 2 * target_size
					)
				> 0) ?
					ds_readbs(RANGED_WEAPONS_TABLE + SIZEOF_RANGED_WEAPON_STATS * host_readbs(p_weapontab + WEAPON_STATS_RANGED_INDEX) + RANGED_WEAPON_STATS_DAMAGE_MODIFIER + distance)
					: -damage;

			if (damage_mod != 0) { /* test is redundant */
				damage += damage_mod;
			}
			/* If the skill test was not successful, damage == 0 by now. */

		} else {
			/* weapon does melee damage */

			damage_mod = host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK)) - host_readbs(p_weapontab + WEAPON_STATS_DAMAGE_KK_BONUS);
			if (damage_mod > 0) {
				damage += damage_mod;
			}
		}
	} else {
		damage = random_schick(6);
	}

	if (attack_hero == 0) {

		enemy_gfx_id = host_readbs(enemy_p + ENEMY_SHEET_GFX_ID);

		/* magic SABRE gives Damage + 1 to SKELETONS and ZOMBIES */
		if ((right_hand == ITEM_SABER_MAGIC) && (enemy_gfx_id == 0x1c || enemy_gfx_id == 0x22)) {
			damage++;
		} else {
			if (right_hand == ITEM_KUKRIS_DAGGER) {
				/* KUKRIS DAGGER / KUKRISDOLCH */

				/* Interesting */
				damage = 1000;

				/* drop the KUKRISDOLCH and equip a normal DOLCH / DAGGER */
				drop_item(hero, HERO_INVENTORY_SLOT_RIGHT_HAND, 1);
				give_hero_new_item(hero, ITEM_DAGGER, 1 ,1); /* TODO: what if no free knapsack slot? */
				move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, get_item_pos(hero, ITEM_DAGGER), hero);

			} else if (right_hand == ITEM_KUKRIS_MENGBILAR) {
				/* KUKRISMENGBILAR */

				/* Interesting */
				damage = 1000;

				/* drop the KUKRISMENGBILAR and equip a normal MENGBILAR  */
				drop_item(hero, HERO_INVENTORY_SLOT_RIGHT_HAND, 1);
				give_hero_new_item(hero, ITEM_MENGBILAR, 1 ,1); /* TODO: what if no free knapsack slot? */
				move_item(HERO_INVENTORY_SLOT_RIGHT_HAND, get_item_pos(hero, ITEM_MENGBILAR), hero);

			} else if ((right_hand == ITEM_SILVER_MACE) && (enemy_gfx_id == 0x1c)) {
				/* SILVER MACE / SILBERSTREITKOLBEN gives Damage + 4 to SKELETONS */
				damage += 4;
			} else if ((right_hand == ITEM_GRIMRING) && (enemy_gfx_id == 0x18)) {
				/* DAS SCHWERT GRIMRING gives Damage + 5 to ORCS */
				damage += 5;
			}
		}
	}

	damage += host_readbs(hero + HERO_SAFTKRAFT);

	if (damage > 0) {

		if (inventory_poison_expurgicum(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID)) {
			damage += dice_roll(1, 6, 2); /* D6 + 2 damage */
			and_ptr_bs(hero + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_FLAGS), 0xdf); /* unset 'poison_expurgicum' flag */
		}

		if (inventory_poison_vomicum(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY)) {
			damage += dice_roll(1, 20, 5); /* D20 + 5 damage */
			and_ptr_bs(hero + (HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_FLAGS), 0xbf); /* unset 'poison_vomicum' set */
		}

		if (host_readbs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE) != POISON_TYPE_NONE) {

			if (host_readbs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE) == POISON_TYPE_ANGSTGIFT) {
				or_ptr_bs(enemy_p + ENEMY_SHEET_FLAGS2, 0x04); /* set 'scared' flag */
				and_ptr_bs(enemy_p + ENEMY_SHEET_FLAGS2, 0xfd); /* unset 'renegade' flag */
			} else {
				/* the following line is the source for the totally excessive and unbalanced poison damage */

				damage += 10 * ds_readws(POISON_PRICES + 2 * host_readbs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE));
			}

			dec_ptr_bs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES);

			if (!host_readbs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_NR_POISON_CHARGES)) {
				host_writebs(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_POISON_TYPE, POISON_TYPE_NONE);
			}
		}
	}

	if (host_readbs(hero + HERO_ECLIPTIFACTUS) != 0) {
		damage *= 2;
	}

	if ((ds_readbs(TEVENT071_ORCSTATUE) != 0) &&
		(host_readbs(hero + HERO_TYPE) == HERO_TYPE_DWARF) &&
		(attack_hero == 0) &&
		(host_readbs(enemy_p + ENEMY_SHEET_GFX_ID) == 0x18))
	{
		damage++;
	}

	if (attack_hero == 0) {
		damage -= host_readbs(enemy_p + 2);

		if (enemy_petrified(enemy_p)) {
			damage = 0;
		}

		if ((host_readbs(enemy_p + ENEMY_SHEET_MAGIC) != 0) && !inventory_magic(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY)) {
			damage = 0;
		}

		if (host_readws(enemy_p + ENEMY_SHEET_LE) < damage) {
			damage = host_readws(enemy_p + ENEMY_SHEET_LE) + 1;
		}
	} else {
		damage -= host_readbs(target + HERO_RS_BONUS1);

		if (hero_petrified(target)) {
			damage = 0;
		}

		if (host_readws(target + HERO_LE) < damage) {
			damage = host_readws(target + HERO_LE) + 1;
		}
	}

	return damage;
}

/**
 * \brief   calculates attack damage from an enemy
 *
 * \param   attacker    the enemy which attacks
 * \param   attacked    the enemy/hero which gets attacked
 * \param   is_enemy    1 = if attacked is an enemy / 0 = attacked is a hero
 *
 */

signed short FIG_get_enemy_attack_damage(Bit8u *attacker, Bit8u *attacked, signed short is_enemy)
{
	signed short pos;
	Bit8u *hero;

	signed short damage;
	signed short dice;

	dice = host_readw(attacker + ENEMY_SHEET_DAM1);

	if (host_readw(attacker + ENEMY_SHEET_DAM2) != 0 && random_schick(100) < 50)
		dice = host_readw(attacker + ENEMY_SHEET_DAM2);

	damage = dice_template(dice);

	if (!is_enemy) {
		/* the attacked is a hero */
		hero = attacked;

		/* subtract RS */
		damage -= host_readbs(hero + HERO_RS_BONUS1);

		/* armor bonus against skeletons and zombies */
		if (host_readw(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY + INVENTORY_ITEM_ID) == ITEM_CHAIN_MAIL_CURSED && (
			host_readb(attacker + ENEMY_SHEET_GFX_ID) == 0x22 ||
			host_readb(attacker + ENEMY_SHEET_GFX_ID) == 0x1c)) {
				damage -= 3;
		}

		/* get position of Totenkopfguertel/Skullbelt */

		if ( (pos = get_item_pos(hero, ITEM_SKULL_BELT)) != -1 &&
			(host_readb(attacker + ENEMY_SHEET_GFX_ID) == 0x22 ||
			host_readb(attacker + ENEMY_SHEET_GFX_ID) == 0x1c)) {

			/* no damage for the hero who has it */
			damage = 0;

			/* 4% chance to loose this item */
			if (random_schick(100) < 5) {
				drop_item(hero, pos, 1);
				GUI_output(get_tx(11));
			}
		}

		/* no damage if the hero is petrified */
		if (hero_petrified(hero))
			damage = 0;
	} else {
		/* the attacked is an enemy */

		/* subtract RS */
		damage -= host_readbs(attacked + ENEMY_SHEET_RS);

		if (enemy_petrified(attacked))
			damage = 0;

		/* check if the attacked is immune
		 * against non-magicial weapons */
		if (host_readb(attacked + ENEMY_SHEET_MAGIC) != 0)
			damage = 0;
	}

	/* damage bonus */
	damage += host_readbs(attacker + ENEMY_SHEET_SAFTKRAFT);

	/* half damage */
	if (host_readb(attacker + ENEMY_SHEET_BROKEN) != 0)
		damage /= 2;

	return damage;
}

void seg041_8c8(void)
{
	signed short i;

	for (i = 0; i < 8; i++)
		memset(p_datseg + FIG_ANISHEETS + i * 0xf3, -1, 0xf3);

	/* That would be better */
	/* memset(p_datseg + FIG_ANISHEETS, -1, 0xf3 * 8); */
}

/**
 * \brief   check if a hero is equipped with a vaild weapon
 *
 * \param   hero        pointer to hero
 *
 * \return
 *	-1 = not a weapon or a broken weapon or a ranged weapon (the latter including WEAPON_TYPE_SPEER, but not ITEM_MAGIC_WAND or ITEM_QUARTERSTAFF),
 *	0 = non-broken knive weapon,
 *	1 = non-broken force weapon (includes ITEM_MAGIC_WAND and ITEM_QUARTERSTAFF),
 *	2 = any other non-broken melee weapon, including WEAPON_TYPE_WAFFENLOS (i.e. ammunition), but no WEAPON_TYPE_SPEER
 */
signed short weapon_check(Bit8u *hero)
{
	Bit8u *item_p;

	signed short item;
	signed short l_di;

	/* get the ID of the equipped weapon */
	item = host_readw(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID);

	item_p = get_itemsdat(item);

	if (!item_weapon(item_p) ||
		inventory_broken(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY) ||
		(item_weapon(item_p) &&
			((host_readbs(item_p + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_SCHUSSWAFFE) ||
			(host_readbs(item_p + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_WURFWAFFE) ||
			/* TODO: according to original DSA2/3 rules, weapon type SPEER is a melee discipline. */
			(host_readbs(item_p + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_SPEER && item != ITEM_MAGIC_WAND && item != ITEM_QUARTERSTAFF))))
	{
		l_di = -1;
	} else {
		if (is_in_word_array(item, (signed short*)(p_datseg + FORCE_WEAPONS))) {
			l_di = 1;
		} else if (is_in_word_array(item, (signed short*)(p_datseg + KNIVE_WEAPONS))) {
			l_di = 0;
		} else {
			l_di = 2;
		}
	}

	return l_di;
}

#if !defined(__BORLANDC__)
}
#endif
