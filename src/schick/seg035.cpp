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

struct dummy5 {
	char a[5];
};

struct dummy62 {
	char a[62];
};

/**
 * \brief   compress monsters
 */
void FIG_tidy_monsters(void)
{
	signed short i;
	signed short j;
	signed short monsters = FIG_count_active_enemies();

	i = 0;
	while (i < 20) {

		/* if the monster is not able to fight anymore ... */
		if ((ds_readbs(ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * i + ENEMY_SHEET_MON_ID) != 0) &&
			(enemy_dead(((Bit8u*)p_datseg + (ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * i))) ||
			enemy_mushroom(((Bit8u*)p_datseg + (ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * i))) ||
			enemy_petrified(((Bit8u*)p_datseg + (ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * i))) ||
			((host_readbs((Bit8u*)ds_readd(CURRENT_FIGHT) + SIZEOF_FIGHT_MONSTER * i + FIGHT_MONSTERS_ROUND_APPEAR) != 0) && (monsters == 0))))
		{

			if (i == 19) {
				/* just clear the last one */
				memset((Bit8u*)ds_readd(CURRENT_FIGHT) + SIZEOF_FIGHT_MONSTER * i + FIGHT_MONSTERS_ID, 0, 5);
				break;
			} else {
				/* move the next monsters one position to the front */
				for (j = i; j < 19; j++) {

					*(struct dummy5*)((Bit8u*)ds_readd(CURRENT_FIGHT) + SIZEOF_FIGHT_MONSTER * j + FIGHT_MONSTERS_ID) =
						*(struct dummy5*)((Bit8u*)ds_readd(CURRENT_FIGHT) + SIZEOF_FIGHT_MONSTER * (j + 1) + FIGHT_MONSTERS_ID);

					memset((Bit8u*)ds_readd(CURRENT_FIGHT) + SIZEOF_FIGHT_MONSTER * (j + 1) + FIGHT_MONSTERS_ID, 0, SIZEOF_FIGHT_MONSTER);

					*(struct dummy62*)(p_datseg + ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * j) =
						*(struct dummy62*)(p_datseg + ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * (j + 1));

					memset(p_datseg + ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * (j + 1), 0, SIZEOF_ENEMY_SHEET);

					or_ds_bs((ENEMY_SHEETS + ENEMY_SHEET_FLAGS1 + SIZEOF_ENEMY_SHEET) + SIZEOF_ENEMY_SHEET * j, 1); /* set 'dead' flag */
				}
			}
		} else {
			i++;
		}
	}
}

void FIG_loot_monsters(void)
{
	signed short l_si;
	signed short l_di; /* loot number */
	signed short l1;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;
	Bit32s money;
	signed short autofight_bak;
	signed short tw_bak;

	char *a[31];

	l_si = 0;

	autofight_bak = g_autofight;
	g_autofight = 0;

	for (l_di = 0; l_di < 30; l_di++) {

			a[l_di] = (char*)(F_PADD((Bit8u*)g_buffer9_ptr, 40 * l_di));
	}

	do {

		l_di = l3 = 0;

		while (((l1 = host_readws((Bit8u*)ds_readd(CURRENT_FIGHT) + 2 * l_di + FIGHT_LOOT)) != 0) &&
			(l_di < 30) && (l1 != ITEM_BONE_WITH_RUNE))
			/* Apparently a quick "fix" for an unwanted bone with runes in fight THOR8,
			 * see https://www.crystals-dsa-foren.de/showthread.php?tid=453&pid=172221#pid172221 */
		{
			strcpy(a[l_di++], GUI_name_plural(0, get_itemname(l1)));
			l3++;
		}

		if (l3 != 0) {

			if (l3 > 15) {

				if (l_si == 0) {
					l5 = 15;
					l6 = 16;
				} else {
					l5 = l3;
					l6 = l3 - 14;
				}

				strcpy(a[l5], get_ttx(751));
			} else {
				l_si = 0;
				l5 = 0;
				l6 = l3;
			}

			tw_bak = g_textbox_width;
			g_textbox_width = 6;

			l4 = GUI_radio(get_tx(14), l6,
						a[0 + l_si], a[1 + l_si], a[2 + l_si], a[3 + l_si],
						a[4 + l_si], a[5 + l_si], a[6 + l_si], a[7 + l_si],
						a[8 + l_si], a[9 + l_si], a[10 + l_si], a[11 + l_si],
						a[12 + l_si], a[13 + l_si], a[14 + l_si], a[15 + l_si]) - 1;

			g_textbox_width = tw_bak;

			if ((l5 != 0) && (l6 - 1 == l4)) {
				if (!l_si) {
					l_si = 15;
				} else {
					l_si = 0;
				}
			}

			if ((l4 != -2) && ((l5 == 0) || ((l5 != 0) && (l6 - 1 != l4)))) {

				if (!get_item(host_readws((Bit8u*)ds_readd(CURRENT_FIGHT) + 2 * (l4 + l_si) + FIGHT_LOOT), 1, 1))
				{
					l4 = -2;
				} else {
					host_writew((Bit8u*)ds_readd(CURRENT_FIGHT) + 2 * (l4 + l_si) + FIGHT_LOOT, 0);

					for (l_di = l4 + l_si; l_di < 29; l_di++) {

						host_writew((Bit8u*)ds_readd(CURRENT_FIGHT) + 2 * (l_di) + FIGHT_LOOT,
							host_readws((Bit8u*)ds_readd(CURRENT_FIGHT) + 2 * (l_di + 1) + FIGHT_LOOT));

						host_writew((Bit8u*)ds_readd(CURRENT_FIGHT) + 2 * (l_di + 1) + FIGHT_LOOT, 0);
					}
				}
			}

		} else {
			l4 = -2;
		}
	} while (l4 != -2);

	money = host_readws((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_DUCATS) * 100;
	money += host_readws((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_SILVER) * 10;
	money += host_readws((Bit8u*)ds_readd(CURRENT_FIGHT) + FIGHT_HELLER);

	if (money > 0) {

		make_valuta_str(g_text_output_buf, money);

		sprintf((char*)(g_dtp2),
			get_tx(15),
			g_text_output_buf);
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
	signed short l_si;
	signed short ap;
	signed short known_ap;
	signed short autofight_bak;

	ap = 0;
	autofight_bak = g_autofight;
	g_autofight = 0;

	/* calculate ap from all monsters in that fight */
	for (l_si = 0; l_si < 20; l_si++) {

		if (ds_readbs(ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * l_si + ENEMY_SHEET_MON_ID) != 0) {

			if (ds_readbs(KNOWN_MONSTERS + ds_readbs(ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * l_si)) != 0) {

				/* monster is already known */
				known_ap = ds_readbs((ENEMY_SHEETS + ENEMY_SHEET_FIRSTAP) + SIZEOF_ENEMY_SHEET * l_si) / 10;
				ap += (known_ap == 0 ? 1 : known_ap);
			} else {
				/* first time bonus */
				ap += ds_readbs((ENEMY_SHEETS + ENEMY_SHEET_FIRSTAP) + SIZEOF_ENEMY_SHEET * l_si);
			}
		}
	}

	/* mark each monster type from that fight */
	for (l_si = 0; l_si < 20; l_si++) {

		if (ds_readbs(ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * l_si + ENEMY_SHEET_MON_ID) != 0) {

			ds_writeb(KNOWN_MONSTERS + ds_readbs(ENEMY_SHEETS + SIZEOF_ENEMY_SHEET * l_si), 1);
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
signed short check_hero_range_attack(Bit8u *hero, signed short hero_pos)
{
	signed short i;
	signed short retval;	/* retval */
	signed short target_x;
	signed short target_y;
	signed short hero_x;
	signed short hero_y;

	retval = 1;

	/* get position of the target */
	FIG_search_obj_on_cb(host_readbs(hero + HERO_ENEMY_ID), &target_x, &target_y);

	/* get position of the hero */
	FIG_search_obj_on_cb(hero_pos + 1, &hero_x, &hero_y);

#if !defined(__BORLANDC__)
	/* BE-fix */
	target_x = host_readws((Bit8u*)&target_x);
	target_y = host_readws((Bit8u*)&target_y);
	hero_x = host_readws((Bit8u*)&hero_x);
	hero_y = host_readws((Bit8u*)&hero_y);
#endif
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

				if (!seg034_000(hero_x, hero_y, i, hero_y, 1, 0, 99)) {
					GUI_output(get_tx(45));
					retval = 0;
					break;
				}
			}
		} else {
			for (i = hero_y; i < target_y; i++) {

				if (!seg034_000(hero_x, hero_y, hero_x, i, 0, 1, 99)) {
					GUI_output(get_tx(45));
					retval = 0;
					break;
				}
			}
		}
	} else {

		if (host_readbs(hero + HERO_ACTION_ID) == FIG_ACTION_RANGE_ATTACK) {
			GUI_output(get_ttx(508));
			retval = 0;
		}
	}

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
