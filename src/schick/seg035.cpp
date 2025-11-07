/**
 *	Rewrite of DSA1 v3.02_de functions of seg035 (fightsystem)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg035.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg032.h"
#include "seg034.h"
#include "seg038.h"
#include "seg039.h"
#include "seg047.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   compress monsters
 */
void FIG_tidy_monsters(void)
{
	signed int i;
	signed int j;
	signed int monsters = FIG_count_active_enemies();

	i = 0;
	while (i < 20) {

		/* if the monster is not able to fight anymore ... */
		if (g_enemy_sheets[i].mon_id &&
			(g_enemy_sheets[i].flags.dead || g_enemy_sheets[i].flags.mushroom || g_enemy_sheets[i].flags.petrified ||
			(g_current_fight->monsters[i].round_appear && (monsters == 0))))
		{
			if (i == 19) {
				/* just clear the last one */
				memset(&g_current_fight->monsters[i], 0, sizeof(struct fight_monster));
				break;
			} else {
				/* move the next monsters one position to the front */
				for (j = i; j < 19; j++) {

					g_current_fight->monsters[j] = g_current_fight->monsters[j + 1];

					memset(&g_current_fight->monsters[j + 1], 0, sizeof(struct fight_monster));

					g_enemy_sheets[j] = g_enemy_sheets[j + 1];

					memset(&g_enemy_sheets[j + 1], 0, sizeof(struct enemy_sheet));

					g_enemy_sheets[j + 1].flags.dead = 1;
				}
			}

		} else {
			i++;
		}
	}
}

void FIG_loot_monsters(void)
{
	signed int l_si = 0;
	signed int loot_num;
	signed int item_id;
	signed int item_cnt;
	signed short l4;
	signed short l5;
	signed short l6;
	Bit32s money;
	const signed int autofight_bak = g_autofight;
	signed int tw_bak;

	char *loot_names[31];

	g_autofight = 0;

	for (loot_num = 0; loot_num < 30; loot_num++) {

		loot_names[loot_num] = (char*)(g_buffer9_ptr + 40 * loot_num);
	}

	do {
		loot_num = item_cnt = 0;

		while ((item_id = g_current_fight->loot[loot_num]) && (loot_num < 30) && (item_id != ITEM_KNOCHEN_MIT_RUNE))
			/* Apparently a quick "fix" for an unwanted bone with runes in fight THOR8,
			 * see https://www.crystals-dsa-foren.de/showthread.php?tid=453&pid=172221#pid172221 */
		{
			strcpy(loot_names[loot_num++], GUI_name_plural(0, get_itemname(item_id)));
			item_cnt++;
		}

		if (item_cnt != 0) {

			if (item_cnt > 15) {

				if (l_si == 0) {
					l5 = 15;
					l6 = 16;
				} else {
					l5 = item_cnt;
					l6 = item_cnt - 14;
				}

				strcpy(loot_names[l5], get_ttx(751));

			} else {
				l_si = 0;
				l5 = 0;
				l6 = item_cnt;
			}

			tw_bak = g_textbox_width;
			g_textbox_width = 6;

			l4 = GUI_radio(get_tx(14), l6,
				loot_names[0 + l_si], loot_names[1 + l_si],
				loot_names[2 + l_si], loot_names[3 + l_si],
				loot_names[4 + l_si], loot_names[5 + l_si],
				loot_names[6 + l_si], loot_names[7 + l_si],
				loot_names[8 + l_si], loot_names[9 + l_si],
				loot_names[10 + l_si], loot_names[11 + l_si],
				loot_names[12 + l_si], loot_names[13 + l_si],
				loot_names[14 + l_si], loot_names[15 + l_si]) - 1;

			g_textbox_width = tw_bak;

			if ((l5 != 0) && (l6 - 1 == l4)) {
				if (!l_si) {
					l_si = 15;
				} else {
					l_si = 0;
				}
			}

			if ((l4 != -2) && ((l5 == 0) || ((l5 != 0) && (l6 - 1 != l4)))) {

				if (!get_item(g_current_fight->loot[l4 + l_si], 1, 1))
				{
					l4 = -2;

				} else {
					g_current_fight->loot[l4 + l_si] = 0;

					for (loot_num = l4 + l_si; loot_num < 29; loot_num++) {

						g_current_fight->loot[loot_num] = g_current_fight->loot[loot_num + 1];
						g_current_fight->loot[loot_num + 1] = 0;
					}
				}
			}

		} else {
			l4 = -2;
		}

	} while (l4 != -2);

	money = g_current_fight->ducats * 100;
	money += g_current_fight->silver * 10;
	money += g_current_fight->heller;

	if (money > 0) {

		make_valuta_str(g_text_output_buf, money);

		sprintf((char*)g_dtp2, get_tx(15), g_text_output_buf);
		GUI_output(g_dtp2);

		set_party_money(get_party_money() + money);
	}

	g_autofight = autofight_bak;
}

/**
 * \brief   give the group the AP after a fight
 */
void FIG_split_ap(void)
{
	signed int i;
	signed int ap;
	signed int known_ap;
	signed int autofight_bak;

	ap = 0;
	autofight_bak = g_autofight;
	g_autofight = 0;

	/* calculate ap from all monsters in that fight */
	for (i = 0; i < 20; i++) {

		if (g_enemy_sheets[i].mon_id) {

			if (gs_known_monsters[g_enemy_sheets[i].mon_id]) {

				/* monster is already known */
				known_ap = g_enemy_sheets[i].first_ap / 10;
				ap += (known_ap == 0 ? 1 : known_ap);
			} else {
				/* first time bonus */
				ap += g_enemy_sheets[i].first_ap;
			}
		}
	}

	/* mark each type of monster from that fight in the game state */
	for (i = 0; i < 20; i++) {

		if (g_enemy_sheets[i].mon_id) {

			gs_known_monsters[g_enemy_sheets[i].mon_id] = 1;
		}
	}

	if (count_heroes_available_in_group() > ap) {
		/* every hero gets at least 1 AP */

		ap = count_heroes_available_in_group();

	} else {
		/* every hero should get the same AP */
		while ((ap % count_heroes_available_in_group())) {
			ap++;
		}
	}

	/* prepare output */
	sprintf(g_dtp2, get_tx(33), ap);
	GUI_output(g_dtp2);

	/* give AP to the group */
	add_group_ap(ap);

	g_autofight = autofight_bak;
}

/**
 * \brief   test if a range attack is possible
 *
 * \param   hero        pointer to the hero
 * \param   hero_pos    position of the hero
 * \return              1 = range attack is possible, 0 = ... not possible
 */
signed short check_hero_range_attack(const struct struct_hero *hero, const signed int hero_pos)
{
	signed int i;
	signed int retval;	/* retval */
	signed int target_x;
	signed int target_y;
	signed int hero_x;
	signed int hero_y;

	retval = 1;

	/* get position of the target */
	FIG_search_obj_on_cb(hero->target_object_id, &target_x, &target_y);

	/* get position of the hero */
	FIG_search_obj_on_cb(hero_pos + 1, &hero_x, &hero_y);

	/* check that the range attack is in the same line */
	if ((hero_x != target_x) && (hero_y != target_y)) {
		GUI_output(get_tx(45));
		retval = 0;
	}

	if (calc_beeline(hero_x, hero_y, target_x, target_y) != 1) {

		/* exchange coordinates if needed */
		if (hero_x > target_x) {
			i = hero_x;
			hero_x = target_x;
			target_x = i;
		}

		/* exchange coordinates if needed */
		if (hero_y > target_y) {
			i = hero_y;
			hero_y = target_y;
			target_y = i;
		}

		if (hero_x != target_x) {

			for (i = hero_x; i < target_x; i++) {

				if (!FIG_check_hero_attack(hero_x, hero_y, i, hero_y, 1, 0, 99)) {
					GUI_output(get_tx(45));
					retval = 0;
					break;
				}
			}
		} else {
			for (i = hero_y; i < target_y; i++) {

				if (!FIG_check_hero_attack(hero_x, hero_y, hero_x, i, 0, 1, 99)) {
					GUI_output(get_tx(45));
					retval = 0;
					break;
				}
			}
		}
	} else {

		if (hero->action_id == FIG_ACTION_RANGE_ATTACK) {
			GUI_output(get_ttx(508));
			retval = 0;
		}
	}

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
