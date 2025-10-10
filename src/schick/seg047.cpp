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
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   get index of the first hero with the highest CH value
 *
 * \return index of the first hero with the highest current CH value.
 * The hero must be alive and in the current group.
 */
signed short get_hero_CH_best(void)
{
	signed short retval;
	struct struct_hero *hero_i;
	signed short i;
	signed short ch_val = -1;

	hero_i = (struct struct_hero*)get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->group_no == gs_current_group) &&
			(!hero_i->flags.dead) &&	(hero_i->attrib[ATTRIB_CH].current > ch_val))
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
signed short get_hero_KK_best(void)
{
	signed short retval;
	struct struct_hero *hero_i;
	signed short i;
	signed short kk_val = -1;

	hero_i = (struct struct_hero*)get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->group_no == gs_current_group) &&
			(!hero_i->flags.dead) &&	(hero_i->attrib[ATTRIB_KK].current > kk_val))
		{
			/* check if KK is higher */

			kk_val = hero_i->attrib[ATTRIB_KK].current;
			retval = i;
		}
	}

	return retval;
}

/**
 * \brief   returns the disease number the hero has
 *
 *          Only the first disease is returned here.
 *
 * \param   hero        the hero which should be checked
 * \return              number of the first disease the hero has
 */
signed short hero_is_diseased(struct struct_hero *hero)
{
	signed int i;

	for (i = 1; i <= 7; i++) {

		if (hero->sick[i][0] == -1) {

			return i;
		}
	}

	return 0;
}

/**
 * \brief   returns the poison number the hero has
 *
 *          Only the first poison is returned here.
 *
 * \param   hero        the hero which should be checked
 * \return              number of the first poisoning the hero has
 */
signed short hero_is_poisoned(struct struct_hero *hero)
{
	signed int i;

	for (i = 1; i <= 9; i++) {

		if (hero->poison[i][0] == -1) {

			return i;
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
void hero_gets_poisoned(struct struct_hero *hero, const signed short poison)
{
	if (!hero->flags.dead) {

		hero->poison[poison][0] = -1;
		hero->poison[poison][1] = 0;
		hero->poison[poison][2] = 0;
		hero->poison[poison][3] = 0;
		hero->poison[poison][4] = 0;
	}
}

/**
 * \brief   diseases a hero
 *
 * \param   hero        the hero which gets diseased
 * \param   disease     the kind of disease
 */
void hero_gets_diseased(struct struct_hero *hero, const signed short disease)
{
#ifdef M302de_ORIGINAL_BUGFIX
	/* not a real BUG, but very useless */
	if (hero->typus == HERO_TYPE_NONE)
		return;
#endif

	if (!hero->flags.dead) {

#if !defined(__BORLANDC__)
		D1_INFO("%s erkrankt an %s\n", hero->alias, get_ttx(disease + 0x193));
#endif

		hero->sick[disease][0] = -1;
		hero->sick[disease][1] = 0;
		hero->sick[disease][2] = 0;
		hero->sick[disease][3] = 0;
		hero->sick[disease][4] = 0;
	}
}

/**
 * \brief   the hero may get a disease
 *
 * \param   hero        the hero which may gets diseased
 * \param   disease     the kind of disease
 * \param   probability the probability to get diseased in percent
 */
void hero_disease_test(struct struct_hero *hero, const signed short disease, const signed short probability) {

#ifdef M302de_ORIGINAL_BUGFIX
	/* not a real BUG, but very useless */
	if (hero->typus == HERO_TYPE_NONE) {
		D1_ERR("WARNING: called %s with an invalid hero\n", __func__);
		return;
	}
#endif

	/* check the probability and if hero is diseased*/
	if ((random_schick(100) <= probability) && (hero->sick[disease][0] != -1)) {

		hero_gets_diseased(hero, disease);
	}
}

/**
 *	check_hero_KK_unused
 *
 *	This function is not used in the game!
 */
signed short check_hero_KK_unused(const signed short val)
{
	return ((struct struct_hero*)get_hero(0))->attrib[ATTRIB_KK].current + ((struct struct_hero*)get_hero(0))->attrib[ATTRIB_KK].mod >= val ? 1 : 0;
}

/**
 *	check_heroes_KK
 * \param   val         value to compare KK with
 *
 *	This function, like hero_check_KK_unused, is buggy!
 *	It does not check if the first slot is a valid hero.
 */
signed short check_heroes_KK(const signed short val)
{
	struct struct_hero *hero;
	signed short sum;

	hero = (struct struct_hero*)get_hero(0);

	/* Orig-BUG: not checked if hero is valid */
	sum = hero->attrib[ATTRIB_KK].current + hero->attrib[ATTRIB_KK].mod;

	hero = (struct struct_hero*)get_hero(1);

	/* check class, group and dead status of hero in slot 2*/
	if (hero->typus && (hero->group_no == gs_current_group) && !hero->flags.dead) {
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
void make_valuta_str(char *dst, Bit32s money) {
	/* Orig-BUG: d can overflow  on D > 65536*/
	unsigned short d = 0;
	unsigned short s = 0;

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

	sprintf(dst, get_ttx(748), d, s, (short)money);
}

/**
 * \brief   recalculates the AT PA values
 */
void update_atpa(struct struct_hero *hero)
{
	div_t erg;
	signed short diff;
	signed short i;

	/* ATPA base = (IN + KK + GE) / 5 rounded */
	erg = div(hero->attrib[ATTRIB_IN].normal + hero->attrib[ATTRIB_KK].normal + hero->attrib[ATTRIB_GE].normal, 5);
	/* Original-Bug:
	 * According to DSA 3 rules, AT basis value is (MU + KK + GE) / 5
	 * (PA basis (IN + KK + GE)/5 is correct */

	/* round up */
	if (erg.rem >= 3)
		erg.quot++;

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
			hero->at_weapon[i] = hero->at_weapon[i] + diff;
			/* add diff to PA value */
			hero->pa_weapon[i] = hero->pa_weapon[i] + diff;
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
signed short menu_enter_delete(Bit8u* ptr, signed short entries, signed short mode)
{
	signed short i;
	signed short answer;
	signed short i_max;
	signed short i_min;

	answer = 0;
	i_min = 0;

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
signed short select_hero_from_group(char *title)
{
	signed short i;
	signed short answer;
	struct Bit16s_7 dst = g_seg047_init1;
	/* Hack for
	signed short dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/

	signed short cnt;
	signed short tw_bak;
	signed short bak_2;
	signed short bak_3;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;
	cnt = 0;

	for (i = 0; i <= 6; i++) {

		hero = (struct struct_hero*)get_hero(i);

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && (g_hero_sel_exclude != i)) {
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
signed short select_hero_ok(char *title)
{
	signed short i;
	signed short answer;
	struct Bit16s_7 dst = g_seg047_init2;
	/* Hack for
	signed short dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/

	signed short cnt;
	signed short tw_bak;
	signed short bak_2;
	signed short bak_3;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;
	cnt = 0;

	for (hero = (struct struct_hero*)get_hero(0), i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && check_hero(hero) && (g_hero_sel_exclude != i)) {

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
		g_skilled_hero_pos = -1;

		if (answer != -2)
			return dst.a[answer];
		else
			return -1;
	}

	g_textbox_width = tw_bak;
	g_skilled_hero_pos = -1;

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
signed short select_hero_ok_forced(char *title)
{
	signed short i;
	signed short answer;
	struct Bit16s_7 dst = g_seg047_init3;
	/* Hack for
	signed short dst[7] = {0, 0, 0, 0, 0, 0, 0};
	*/

	signed short cnt;
	signed short tw_bak;
	signed short bak_2;
	signed short bak_3;
	struct struct_hero *hero;

	tw_bak = g_textbox_width;
	g_textbox_width = 3;
	cnt = 0;

	for (hero = (struct struct_hero*)get_hero(0), i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && check_hero(hero) && (g_hero_sel_exclude != i)) {

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

		g_skilled_hero_pos = -1;
		g_textbox_width = tw_bak;

		return dst.a[answer];
	} else {

		g_skilled_hero_pos = -1;
		return 0;
	}
}

/**
 * \brief   counts the heroes in the current group
 *
 * \return  number of living heroes in the current group
 */
signed short count_heroes_in_group(void)
{
	struct struct_hero *hero_i;
	signed short i;
	signed short retval;

	retval = 0;

	for (hero_i = (struct struct_hero*)get_hero(0), i = 0; i <= 6; i++, hero_i++) {

		/* Check class, group and dead */
		if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->group_no == gs_current_group) && !hero_i->flags.dead) {

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

#if !defined(__BORLANDC__)
}
#endif
