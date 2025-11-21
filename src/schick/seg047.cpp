/**
 *	Rewrite of DSA1 v3.02_de functions of seg047 (heroes, group)
 *	Functions rewritten: 18/18 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg047.cpp
 */
#include <stdio.h>
#include <stdlib.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg047.h"
#include "seg097.h"

signed char g_hero_sel_exclude = -1; // ds:0x64a2
static struct int16_t_7 g_seg047_init1 = { 0, 0, 0, 0, 0, 0, 0 }; // ds:0x64a3, filled with zeroes
static struct int16_t_7 g_seg047_init2 = { 0, 0, 0, 0, 0, 0, 0 }; // ds:0x64b1, filled with zeroes
static struct int16_t_7 g_seg047_init3 = { 0, 0, 0, 0, 0, 0, 0 }; // ds:0x64bf, filled with zeroes

/**
 * \brief   get index of the first hero with the highest CH value
 *
 * \return index of the first hero with the highest current CH value.
 * The hero must be alive and in the current group.
 */
signed int get_hero_CH_best(void)
{
	signed int retval;
	struct struct_hero *hero_i;
	signed int i;
	signed int ch_val = -1;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->group_id == gs_active_group_id) &&
			!hero_i->flags.dead && (hero_i->attrib[ATTRIB_CH].current > ch_val))
		{
			/* check if CH value is higher */

			ch_val = hero_i->attrib[ATTRIB_CH].current;
			retval = i;
		}
	}

	return retval;
}

/**
 * \brief   get the index of the first hero with the highest KK value
 *
 * \return index of the hero first hero with the highest current KK value.
 * The hero must be alive and in the current group.
 */
signed int get_hero_KK_best(void)
{
	signed int retval;
	struct struct_hero *hero_i;
	signed int i;
	signed int kk_val = -1;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->group_id == gs_active_group_id) &&
			!hero_i->flags.dead && (hero_i->attrib[ATTRIB_KK].current > kk_val))
		{
			/* check if KK is higher */

			kk_val = hero_i->attrib[ATTRIB_KK].current;
			retval = i;
		}
	}

	return retval;
}

/**
 * \brief   returns the disease ID the hero has
 *
 *          Only the first disease is returned here.
 *
 * \param   hero        the hero which should be checked
 * \return  ID of the first disease the hero has
 */
signed int hero_is_diseased(const struct struct_hero *hero)
{
	signed int disease_id;

	for (disease_id = 1; disease_id <= 7; disease_id++) {

		if (hero->disease[disease_id].status == DISEASE_STATUS_DISEASED) {

			return disease_id;
		}
	}

	return 0;
}

/**
 * \brief   returns the poison ID the hero has
 *
 *          Only the first poison is returned here.
 *
 * \param   hero        the hero which should be checked
 * \return              ID of the first poisoning the hero has
 */
signed int hero_is_poisoned(const struct struct_hero *hero)
{
	signed int poison_id;

	for (poison_id = 1; poison_id <= 9; poison_id++) {

		if (hero->poison[poison_id].status == POISON_STATUS_POISONED) {

			return poison_id;
		}
	}

	return 0;
}

/**
 * \brief   poison a hero
 *
 * \param   hero        the hero which gets poisoned
 * \param   poison      the kind of poison
 */
void hero_gets_poisoned(struct struct_hero *hero, const signed int poison_id)
{
	if (!hero->flags.dead) {
		/* TODO: Original-Bug?: What if the hero is already poisoned? */

		hero->poison[poison_id].status = POISON_STATUS_POISONED;
		hero->poison[poison_id].day_counter = 0;
		hero->poison[poison_id].log_1 = 0;
		hero->poison[poison_id].log_2 = 0;
		hero->poison[poison_id].log_3 = 0;
	}
}

/**
 * \brief   diseases a hero
 *
 * \param   hero        the hero which gets diseased
 * \param   disease_id  ID of the disease
 */
void hero_gets_diseased(struct struct_hero *hero, const signed int disease_id)
{
#ifdef M302de_ORIGINAL_BUGFIX
	/* not a real BUG, but very useless */
	if (hero->typus == HERO_TYPE_NONE)
		return;
#endif

	if (!hero->flags.dead) {
		/* TODO: Original-Bug: If the hero is already diseased, the status bytes are erased,
		 * containing the log about the suffered negative effects.
		 * Hence, these cannot be cured later. */

#if !defined(__BORLANDC__)
		D1_INFO("%s erkrankt an %s\n", hero->alias, get_ttx(disease_id + 0x193));
#endif

		hero->disease[disease_id].status = DISEASE_STATUS_DISEASED;
		hero->disease[disease_id].day_counter = 0;
		hero->disease[disease_id].log_1 = 0;
		hero->disease[disease_id].log_2 = 0;
		hero->disease[disease_id].log_3 = 0;
	}
}

/**
 * \brief   the hero may get a disease
 *
 * \param   hero        the hero which may gets diseased
 * \param   disease_id  ID of the disease
 * \param   probability the probability to get diseased in percent
 */
void hero_disease_test(struct struct_hero *hero, const signed int disease_id, const signed int probability)
{
#ifdef M302de_ORIGINAL_BUGFIX
	/* not a real BUG, but very useless */
	if (hero->typus == HERO_TYPE_NONE) {
		D1_ERR("WARNING: called %s with an invalid hero\n", __func__);
		return;
	}
#endif

	/* check the probability and if hero is diseased*/
	if ((random_schick(100) <= probability) && (hero->disease[disease_id].status != DISEASE_STATUS_DISEASED)) {

		hero_gets_diseased(hero, disease_id);
	}
}

/**
 *	check_hero_KK_unused
 *
 *	This function is not used in the game!
 */
signed int check_hero_KK_unused(const signed int val)
{
	return get_hero(0)->attrib[ATTRIB_KK].current + get_hero(0)->attrib[ATTRIB_KK].mod >= val ? 1 : 0;
}

/**
 * \brief check_heroes_KK
 * \param   val         value to compare KK with
 *
 *	This function, like hero_check_KK_unused, is buggy!
 *	It does not check if the first slot is a valid hero.
 */
signed int check_heroes_KK(const signed int val)
{
	struct struct_hero *hero;
	signed int sum;

	hero = get_hero(0);

	/* Orig-BUG: not checked if hero is valid */
	sum = hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod;

	hero = get_hero(1);

	/* check class, group and dead status of hero in slot 2*/
	if (hero->typus && (hero->group_id == gs_active_group_id) && !hero->flags.dead) {
		sum += hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod;
	}

#if !defined(__BORLANDC__)
	D1_INFO("Pruefe KK der ersten beiden Helden (%d) >= %d: ", sum, val);
	D1_INFO("%s\n", sum >= val ? "gelungen" : "mislungen");
#endif

	return (sum >= val) ? 1 : 0;
}

/**
 * \brief   makes a valuta string
 *
 * \param   dst         the destination
 * \param   money       the money in Heller
 *
 *	This funcion is buggy.
 */
void make_valuta_str(char *dst, int32_t money) {

	/* Orig-BUG: d can overflow  on D > 65536*/
	signed int d = 0;
	signed int s = 0;

	/*	These loops are not very performant.
		They take longer the more money you have.
		Here is a much better solution.
	*/

	/*
	d = money / 100;
	money -= d * 100;

	s = money / 10;
	money -= s * 10;
	*/
	while (money / 100) {
		d++;
		money -= 100;
	}

	while (money / 10) {
		s++;
		money -= 10;
	}

	sprintf(dst, get_ttx(748), d, s, (signed int)money);
}

/**
 * \brief   recalculates the AT PA values
 */
void update_atpa(struct struct_hero *hero)
{
	div_t erg;
	signed int diff;
	signed int i;

	/* ATPA base = (IN + KK + GE) / 5 rounded */
	erg = div(hero->attrib[ATTRIB_IN].normal + hero->attrib[ATTRIB_KK].normal + hero->attrib[ATTRIB_GE].normal, 5);
	/* Original-Bug:
	 * According to DSA 3 rules, AT basis value is (MU + KK + GE) / 5
	 * (PA basis (IN + KK + GE) / 5 is correct */

	/* round up */
	if (erg.rem >= 3) {
		erg.quot++;
	}

	/* calculate difference */
	diff = erg.quot - hero->atpa_base;

	if (diff != 0) {

		/* update atpa base value */
		hero->atpa_base = erg.quot;

		/* prepare message */
		sprintf(g_dtp2,	get_ttx(8), hero->atpa_base);

		/* print message */
		GUI_output(g_dtp2);

		for (i = 0; i < 7; i++) {
			/* add diff to AT value */
			hero->at_talent_bonus[i] = hero->at_talent_bonus[i] + diff;
			/* add diff to PA value */
			hero->pa_talent_bonus[i] = hero->pa_talent_bonus[i] + diff;
		}
	}
}

/**
 * \brief   selects a hero to enter / get deleted
 *
 * \param   ptr         pointer
 * \param   entries     how many heroes
 * \param   mode        1 = enter / -1 = delete
 * \return              the number of the selected hero.
 * Used only in temples.
 */
signed int menu_enter_delete(uint8_t* ptr, const signed int entries, const signed int mode)
{
	signed int i;
	signed int answer = 0;
	signed int i_max;
	signed int i_min = 0;

	while (answer != -1) {

		i_max = (i_min + 10 < entries) ? 10 : entries - i_min;

		/* fill a pointer array with the pointer to the names */
		for (i = 0; i < i_max; i++) {
			g_radio_name_list[i] = (char*)((i + i_min) * 32 + ptr + 0x10);
		}

		i = i_max;
		if (entries > 10) {
			g_radio_name_list[i] = get_ttx(291);
			i++;
		}

		answer = GUI_radio( (mode == -1) ? get_ttx(567) : get_ttx(292), (signed char)i,
				g_radio_name_list[0], g_radio_name_list[1],
				g_radio_name_list[2], g_radio_name_list[3],
				g_radio_name_list[4], g_radio_name_list[5],
				g_radio_name_list[6], g_radio_name_list[7],
				g_radio_name_list[8], g_radio_name_list[9],
				g_radio_name_list[10], g_radio_name_list[11]);

		if ((entries > 10) && (answer == i)) {
			i_min += 10;

			if (i_min > entries) {
				i_min = 0;
			}

		} else {

			if (answer != -1) {
				return i_min + answer - 1;
			}
		}
	}

	return -1;
}

/**
 * \brief   menu for selecting a hero
 *
 * \param   title       titlestring for the menu
 * \return              index of the hero or -1 (ESC).
 * Remark: The available heroes must be in the group only.
 */
signed int select_hero_from_group(char *title)
{
	signed int i;
	signed int answer;
	struct int16_t_7 dst = g_seg047_init1;
	/* Hack for
	signed int dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/

	signed int cnt;
	signed int tw_bak;
	signed int bak_2;
	signed int bak_3;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;
	cnt = 0;

	for (i = 0; i <= 6; i++) {

		hero = get_hero(i);

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && (g_hero_sel_exclude != i)) {
			/* TODO: find out what that means */

			/* save pointer to the name of the hero */
			g_radio_name_list[cnt] = hero->alias;
			dst.a[cnt] = i;
			cnt++;
		}
	}


	g_hero_sel_exclude = -1;

	if (cnt != 0) {

		bak_2 = g_basepos_x;
		bak_3 = g_basepos_y;

		g_basepos_x = g_basepos_y = 0;

		answer = GUI_radio(title, (signed char)cnt,
				g_radio_name_list[0], g_radio_name_list[1],
				g_radio_name_list[2], g_radio_name_list[3],
				g_radio_name_list[4], g_radio_name_list[5],
				g_radio_name_list[6]) - 1;

		g_basepos_x = bak_2;
		g_basepos_y = bak_3;
		g_textbox_width = tw_bak;

		if (answer != -2)
			return dst.a[answer];
		else
			return -1;
	}

	g_textbox_width = tw_bak;

	return -1;
}

/**
 * \brief   menu for selecting a hero
 *
 * \param   title       titlestring for the menu
 * \return              index of the hero or -1 (ESC).
 * Remark: The available heroes must be in the group and pass check_hero().
 */
signed int select_hero_ok(char *title)
{
	signed int i;
	signed int answer;
	struct int16_t_7 dst = g_seg047_init2;
	/* Hack for
	signed int dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/

	signed int cnt;
	signed int tw_bak;
	signed int bak_2;
	signed int bak_3;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;
	cnt = 0;

	for (hero = get_hero(0), i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && check_hero(hero) && (g_hero_sel_exclude != i)) {

			/* save pointer to the name of the hero */
			g_radio_name_list[cnt] = hero->alias;
			dst.a[cnt] = i;
			cnt++;
		}
	}

	g_hero_sel_exclude = -1;

	if (cnt != 0) {
		bak_2 = g_basepos_x;
		bak_3 = g_basepos_y;

		g_basepos_x = g_basepos_y = 0;

		answer = GUI_radio(title, (signed char)cnt,
				g_radio_name_list[0], g_radio_name_list[1],
				g_radio_name_list[2], g_radio_name_list[3],
				g_radio_name_list[4], g_radio_name_list[5],
				g_radio_name_list[6]) - 1;

		g_basepos_x = bak_2;
		g_basepos_y = bak_3;
		g_textbox_width = tw_bak;
		g_talented_hero_pos = -1;

		if (answer != -2)
			return dst.a[answer];
		else
			return -1;
	}

	g_textbox_width = tw_bak;
	g_talented_hero_pos = -1;

	return -1;
}

/**
 * \brief   menu for selecting a hero
 *
 * \param   title       titlestring for the menu
 * \return              index of the hero or -1 (ESC).
 * Remark: The available heroes must be in the group, pass check_hero() and
 *		you are forced to select a hero.
 */
signed int select_hero_ok_forced(char *title)
{
	signed int i;
	signed int answer;
	struct int16_t_7 dst = g_seg047_init3;
	/* Hack for
	signed int dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/

	signed int cnt;
	signed int tw_bak;
	signed int bak_2;
	signed int bak_3;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;
	cnt = 0;

	for (hero = get_hero(0), i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_id == gs_active_group_id) && check_hero(hero) && (g_hero_sel_exclude != i)) {

			/* save pointer to the name of the hero */
			g_radio_name_list[cnt] = hero->alias;
			dst.a[cnt] = i;
			cnt++;
		}
	}

	g_hero_sel_exclude = -1;

	if (cnt != 0) {
		do {
			bak_2 = g_basepos_x;
			bak_3 = g_basepos_y;

			g_basepos_x = g_basepos_y = 0;

			answer = GUI_radio(title, (signed char)cnt,
					g_radio_name_list[0], g_radio_name_list[1],
					g_radio_name_list[2], g_radio_name_list[3],
					g_radio_name_list[4], g_radio_name_list[5],
					g_radio_name_list[6]) - 1;

			g_basepos_x = bak_2;
			g_basepos_y = bak_3;

		} while (answer == -2);

		g_talented_hero_pos = -1;
		g_textbox_width = tw_bak;

		return dst.a[answer];
	} else {

		g_talented_hero_pos = -1;
		return 0;
	}
}

/**
 * \brief   counts the heroes in the current group
 *
 * \return  number of living heroes in the current group
 */
signed int count_heroes_in_group(void)
{
	struct struct_hero *hero_i;
	signed int i;
	signed int retval = 0;

	for (hero_i = get_hero(0), i = 0; i <= 6; i++, hero_i++) {

		/* Check class, group and dead */
		if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->group_id == gs_active_group_id) && !hero_i->flags.dead) {

			retval++;
		}
	}

	return retval;
}

/**
 * \brief   let the hero feel the result of too much alcohol
 *
 * \param   hero        the hero
 */
void hero_get_drunken(struct struct_hero *hero)
{
	if (!hero->drunk) {

		/* set the hero drunken */
		hero->drunk = 1;

		/* change good attributes */
		hero->attrib[ATTRIB_MU].current += 1;
		hero->attrib[ATTRIB_KL].current -= 1;
		hero->attrib[ATTRIB_CH].current -= 1;
		hero->attrib[ATTRIB_FF].current -= 1;
		hero->attrib[ATTRIB_GE].current -= 1;
		hero->attrib[ATTRIB_IN].current += 1;
		hero->attrib[ATTRIB_KK].current += 1;

		/* Reset bad attributes */
		hero->attrib[ATTRIB_AG].current += 1;
		hero->attrib[ATTRIB_HA].current -= 1;
		hero->attrib[ATTRIB_RA].current -= 1;
		hero->attrib[ATTRIB_GG].current += 1;
		hero->attrib[ATTRIB_TA].current -= 1;
		hero->attrib[ATTRIB_NG].current += 1;
		hero->attrib[ATTRIB_JZ].current += 1;

		/* do a burp FX2.VOC */
		if (g_pp20_index == ARCHIVE_FILE_ZUSTA_UK) {
			g_request_refresh = 1;
		}

		play_voc_delay(ARCHIVE_FILE_FX2_VOC);
	}
}

/**
 * \brief   makes a drunken hero sober
 *
 * \param   hero        pointer to the hero
 */
void hero_get_sober(struct struct_hero *hero)
{
	/* This is checked twice */
	/* Is hero drunken ? */
	if (hero->drunk == 0)
		return;

#if !defined(__BORLANDC__)
	D1_INFO("%s ist wieder nuechtern\n", hero->alias);
#endif

	/* set hero sober */
	hero->drunk = 0;

	/* Reset good attributes */
	hero->attrib[ATTRIB_MU].current -= 1;
	hero->attrib[ATTRIB_KL].current += 1;
	hero->attrib[ATTRIB_CH].current += 1;
	hero->attrib[ATTRIB_FF].current += 1;
	hero->attrib[ATTRIB_GE].current += 1;
	hero->attrib[ATTRIB_IN].current -= 1;
	hero->attrib[ATTRIB_KK].current -= 1;

	/* Reset bad attributes */
	hero->attrib[ATTRIB_AG].current -= 1;
	hero->attrib[ATTRIB_HA].current += 1;
	hero->attrib[ATTRIB_RA].current += 1;
	hero->attrib[ATTRIB_GG].current -= 1;
	hero->attrib[ATTRIB_TA].current += 1;
	hero->attrib[ATTRIB_NG].current -= 1;
	hero->attrib[ATTRIB_JZ].current -= 1;

	if (g_pp20_index == ARCHIVE_FILE_ZUSTA_UK)
		g_request_refresh = 1;
}
