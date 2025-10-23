/*
 *	Rewrite of DSA1 v3.02_de functions of seg098 (Magic)
 *	Functions rewritten: 12/12 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg098.cpp
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg026.h"
#include "seg029.h"
#include "seg041.h"
#include "seg047.h"
#include "seg097.h"
#include "seg098.h"
#include "seg099.h"
#include "seg100.h"
#include "seg101.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short g_spell_special_aecost = -1; // ds:0xac0e
static struct Bit16s_5 g_ani_heal_picstars = { 0, 1, 2, 1, 0 };	// ds:0xac10
static struct c_str_6 g_spell_select_str_keyval = { "%s~%d" };		// ds:0xac1a
static struct c_str_5 g_spell_select_str_key = { "\xf2%s\xf0"};	// ds:0xac20
static struct c_str_3 g_spell_select_str_key_color = { "%s" };		// ds:0xac25
static struct c_str_8 g_spell_select_str_keyval_color = { "\xf2%s~%d\xf0" }; // ds:0xac28
static struct Bit8s_12 g_spell_select_ones = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; // ds:0xac30

/**
 * \brief   show some stars on the picture of a healed hero
 *
 * \param   hero        the hero who heals
 */
void magic_heal_ani(const struct struct_hero *hero)
{
	signed int target_no;
	struct Bit16s_5 a = g_ani_heal_picstars;
	//signed short a[5] = { 0, 1, 2, 1, 0 };

	struct struct_hero *target;
	signed int handle;
	signed int i;

	/* load SPSTAR.NVF */
	handle = load_archive_file(ARCHIVE_FILE_SPSTAR_NVF);
	read_archive_file(handle, g_buffer8_ptr, 0x400);
	read_archive_file(handle, g_buffer8_ptr + 0x400, 0x400);
	read_archive_file(handle, g_buffer8_ptr + 0x800, 0x400);
	close(handle);

	target_no = hero->enemy_id - 1;
	target = get_hero(target_no);

	g_pic_copy.v1 = 0;
	g_pic_copy.v2 = 0;
	g_pic_copy.v3 = 31;
	g_pic_copy.v4 = 31;

	for (i = 0; i < 5; i++) {

		/* copy hero picture into buffer */
		g_pic_copy.x1 = 0;
		g_pic_copy.y1 = 0;
		g_pic_copy.x2 = 31;
		g_pic_copy.y2 = 31;
		g_pic_copy.dst = g_renderbuf_ptr;
		g_pic_copy.src = target->pic;
		do_pic_copy(0);

		/* copy stars over it */
		g_pic_copy.src = g_buffer8_ptr + (a.a[i] * 1024);
		do_pic_copy(2);

		/* copy buffer content to screen */
		g_pic_copy.x1 = g_hero_pic_posx[target_no];
		g_pic_copy.y1 = 157;
		g_pic_copy.x2 = g_hero_pic_posx[target_no] + 31;
		g_pic_copy.y2 = 188;
		g_pic_copy.src = g_renderbuf_ptr;
		g_pic_copy.dst = g_vga_memstart;
		do_pic_copy(3);

		delay_or_keypress(10);
	}

	draw_status_line();
}

/**
 * \brief   account physical spell damage in fight
 *
 * \param   le          LE someone looses
 */
void FIG_do_spell_damage(signed short le)
{

	if (le <= 0)
		return;

	if (get_spelluser()->enemy_id < 10) {
		/* attack hero */

		/* set pointer */
		g_spelltarget = get_hero(get_spelluser()->enemy_id - 1);

		/* ensure the spelluser does not attack himself */
		if (get_spelltarget() != get_spelluser()) {

			/* do the damage */
			sub_hero_le(get_spelltarget(), le);

			/* add a message (red star with le) */
			FIG_add_msg(0x08, le);

			/* set a variable if the hero died */
			if (get_spelltarget()->flags.dead) {
				g_defender_dead = 1;
			}
		}

	} else {
		/* attack enemy */

		/* set a pointer to the enemy */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

		/* do the damage */
		FIG_damage_enemy(g_spelltarget_e, le, 0);

		/* add a message (green star with le) */
		FIG_add_msg(0x0b, le);

		/* set a variable if the enemy died */
		if (g_spelltarget_e->flags.dead) {
			g_defender_dead = 1;
		}
	}
}

/**
 * \brief   calculates the PA value of one who is attacked
 *
 *          This function is only used by the spell Kraehenruf.
 */
signed short get_attackee_parade(void)
{
	/* check if enemy or hero is attacked */
	if (get_spelluser()->enemy_id < 10) {

		/* attacked a hero */
		g_spelltarget = get_hero(get_spelluser()->enemy_id - 1);

		/* calculate PA  */

		/* PA = PA-Current-Weapon - AT-Modificator - 1/2 * RS-BE */

		return get_spelltarget()->pa_weapon[get_spelltarget()->w_type]
			- get_spelltarget()->atpa_mod
			- get_spelltarget()->rs_be / 2;
	} else {

		/* attacked an enemy */

		/* set a global pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

		return g_spelltarget_e->pa;
	}
}

/**
 * \brief   calculates the RS value of one who is attacked
 *
 *          This function is unused.
 */
signed short get_attackee_rs(void)
{
	/* check if enemy or hero is attacked */
	if (get_spelluser()->enemy_id < 10) {

		/* attacked a hero */

		g_spelltarget = get_hero(get_spelluser()->enemy_id - 1);

		return get_spelltarget()->rs_bonus1; /* why not also HERO_RS_BONUS2? Anyway, function is unused... */

	} else {

		/* attacked an enemy */

		/* set a global pointer to the target */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

		return g_spelltarget_e->rs;
	}
}

/**
 * \brief   get the amount of AE-Points needed for a spell
 *
 * \param   spell_id	the spell ID
 * \param   half_cost	spellcaster needs only half AE
 */
signed short get_spell_cost(signed short spell_id, signed short half_cost)
{
	signed char ret = g_spell_descriptions[spell_id].ae_cost;

	if (half_cost != 0) {

		if (ret == -1) {
			ret = (signed char)random_interval(5, 10);
		} else {
			ret /= 2;
		}

		if (!ret) {
			/* spell cost is at least 1 */
			ret = 1;
		}
	}

	return ret;
}

/**
 * \brief   use magic menu, meditation and staffspell logic
 *
 * \param   hero        pointer to the hero
 * \return              {0, 1, 2}
 */
signed int use_magic(struct struct_hero *hero)
{
	signed int ae;
	signed int retval;
	signed int answer;
	signed int thonny_pos;

	retval = 0;

	answer = GUI_radio(get_ttx(329), 3, get_ttx(311), get_ttx(312), get_ttx(213));

	if (answer != -1) {

		switch(answer) {

		case 1: {
			/* Meditation */

			if (hero->typus != HERO_TYPE_MAGE) {
				/* not a mage, need thonnys */


				if ((thonny_pos = get_item_pos(hero, ITEM_THONNYS)) == -1) {
					GUI_output(get_ttx(790));
					return 0;
				}
			} else {
				/* a mage */
				thonny_pos = -1;
			}

			/* Asks how many AE should be generated */
			ae = GUI_input(get_ttx(333), 2);

			if (ae != -1) {
				retval = 2;

				if (thonny_pos != -1) {
					/* drop a thonny */
					drop_item(hero, thonny_pos, 1);
				}

				/* cap the converted AE such that the hero has at most HERO_AE_ORIG in the end. */
				if ((hero->ae_max - hero->ae)  < ae) {
					ae = hero->ae_max - hero->ae;
				}

				/* spend one AE point */
				sub_ae_splash(hero, 1);

#if !defined(__BORLANDC__)
				D1_INFO("%s Meditationsprobe +0 ", hero->alias);
#endif
				if (test_attrib3(hero, ATTRIB_MU, ATTRIB_CH, ATTRIB_KK, 0) > 0) {
					/* Success */

					/* cap the converted AE such that at least 5 LE remain .*/
					if (hero->le <= ae + 8) {
						ae = hero->le - 8;
					}

					sub_hero_le(hero, ae + 3);
					add_hero_ae(hero, ae);
				} else {
					/* Failed, print only a message */
					sprintf(g_dtp2, get_ttx(795), hero->alias);
					GUI_output(g_dtp2);
				}
			}
			break;
		}
		case 2: {
			/* Staffspell */

			if (hero->typus != HERO_TYPE_MAGE) {
				/* only for mages */
				GUI_output(get_ttx(403));
				return 0;
			}

			if (hero->staff_level == 7) {
				/* Original-Bug: This never happens.
				 * The highest possible staff spell is the 4th one, since the 5th staff spell has handicap 99.
				 * The check should ask for '== 4' instead. */
				GUI_output(get_ttx(335));
			} else {

				if (g_staffspell_descriptions[hero->staff_level].ae_cost <= hero->ae) {

					/* check AE */

					retval = 1;

#if !defined(__BORLANDC__)
					D1_INFO("%s Probe fuer Stabzauber Nr. %d (%+d)", hero->alias, hero->staff_level + 1,
						       	g_staffspell_descriptions[hero->staff_level].handicap);
#endif
					if (test_attrib3(hero,
							g_staffspell_descriptions[hero->staff_level].attrib1,
							g_staffspell_descriptions[hero->staff_level].attrib2,
#ifndef M302de_ORIGINAL_BUGFIX
						/* Original-Bug 17: the first attribute is tested twice, the second one is left out */
							g_staffspell_descriptions[hero->staff_level].attrib2,
#else
							g_staffspell_descriptions[hero->staff_level].attrib3,
#endif
							g_staffspell_descriptions[hero->staff_level].handicap) > 0)
					{
						/* Success */

						/* print a message */
						sprintf(g_dtp2,	get_ttx(339), hero->staff_level + 1);
						GUI_output(g_dtp2);

						sub_ae_splash(hero, g_staffspell_descriptions[hero->staff_level].ae_cost);

						/* REMARK: ae_max or ae */
						hero->ae_max -= g_staffspell_descriptions[hero->staff_level].ae_mod;

						/* Staffspell level +1 */
						hero->staff_level++;

						/* set the timer */
						hero->staffspell_timer = HOURS(12);

						timewarp(HOURS(5));
					} else {
						/* Failed */
						GUI_output(get_ttx(338));

						/* only half of the AE costs */
						sub_ae_splash(hero, g_staffspell_descriptions[hero->staff_level].ae_cost / 2);

						timewarp(HOURS(2));
					}
				} else {
					/* not enough AE */
					GUI_output(get_ttx(337));
				}
			}

			break;
		}
		case 3: {
			/* Cast Spell */
			use_spell(hero, 1, 0);
			break;
		}
		}
	}

	return retval;
}

/**
 * \brief   check if a spellclass can be used
 *
 * \param   hero        pointer to the hero
 * \param   spellclass_id the ID of the spellclass
 * \return              0 = can't be used, 1 = can be used
 */
signed int can_use_spellclass(const struct struct_hero *hero, const signed int spellclass_id)
{
	signed int i;
	const signed int first_spell = g_spells_index[spellclass_id].first;

	for (i = 0; g_spells_index[spellclass_id].length > i; i++) {

		if ((hero->spells[first_spell + i] >= -5) &&
			((g_in_fight && (g_spell_descriptions[first_spell + i].where_to_use == 1)) ||
			(!g_in_fight && (g_spell_descriptions[first_spell + i].where_to_use != 1))))
		{
			return 1;
		}

	}

	return 0;
}

signed int select_spell(struct struct_hero *hero, signed int show_vals)
{
	signed int l_di;
	signed int answer1;
	signed int first_spell;
	signed int retval = -1;
	struct c_str_6 str_val = g_spell_select_str_keyval;
	struct c_str_5 col_str = g_spell_select_str_key;
	struct c_str_3 str = g_spell_select_str_key_color;
	struct c_str_8 col_str_val = g_spell_select_str_keyval_color;
	struct Bit8s_12 ones = g_spell_select_ones;

	if ((show_vals == 0) && (g_game_mode == GAME_MODE_ADVANCED)) {
		show_vals = 2;
	}

	/* only for magic users */
	if (hero->typus < HERO_TYPE_WITCH) {
		GUI_output(get_ttx(330));
		return -2;
	}

	if (show_vals == 1) {

		strcpy(g_text_output_buf, get_ttx(205));

		if (hero->spell_incs > 1) {
			strcat(g_text_output_buf, get_ttx(393));
		}

		sprintf(g_dtp2,	get_ttx(204),
			(hero->spell_incs > 1) ? get_ttx(305) : get_ttx(304),
			hero->spell_incs, g_text_output_buf);

		answer1 = GUI_radio(g_dtp2, 12,
					get_ttx(192), get_ttx(193),
					get_ttx(194), get_ttx(195),
					get_ttx(196), get_ttx(197),
					get_ttx(198), get_ttx(199),
					get_ttx(100), get_ttx(201),
					get_ttx(202), get_ttx(203)) - 1;
	} else {

		strcpy(g_dtp2, get_ttx(216));

		for (l_di = 0; l_di < 12; l_di++) {

			g_radio_name_list[l_di] = g_dtp2 + 50 * (l_di + 1);

			ones.a[l_di] = (signed char)can_use_spellclass(hero, l_di);

			sprintf(g_radio_name_list[l_di], (ones.a[l_di] != 0) ? str.a : col_str.a, get_ttx(l_di + 192));
		}

		answer1 = GUI_radio(g_dtp2, 12,
				g_radio_name_list[0], g_radio_name_list[1],
				g_radio_name_list[2], g_radio_name_list[3],
				g_radio_name_list[4], g_radio_name_list[5],
				g_radio_name_list[6], g_radio_name_list[7],
				g_radio_name_list[8], g_radio_name_list[9],
				g_radio_name_list[10], g_radio_name_list[11]) - 1;
	}

	if (answer1 != -2) {

		if (!ones.a[answer1]) {

			/* this cant use any spells of this class */

			sprintf(g_dtp2, get_ttx(559), hero->alias);
			GUI_output(g_dtp2);

			retval = -2;
		} else {

			first_spell = g_spells_index[answer1].first;

			for (l_di = 0; l_di < g_spells_index[answer1].length; l_di++) {

				g_radio_name_list[l_di] = g_dtp2 + 50 * l_di;

				if (show_vals == 1) {

					sprintf(g_radio_name_list[l_di], (char*)str_val.a,
						get_ttx(first_spell + l_di + 106),
						hero->spells[first_spell + l_di]);
				} else if (
					((g_in_fight && (g_spell_descriptions[first_spell + l_di].where_to_use == 1)) ||
					(!g_in_fight && (g_spell_descriptions[first_spell + l_di].where_to_use != 1))) &&
					(hero->spells[first_spell + l_di] >= -5))
				{

					if (show_vals == 2) {
						sprintf(g_radio_name_list[l_di], (char*)str_val.a,
							get_ttx(first_spell + l_di + 106),
							hero->spells[first_spell + l_di]);
					} else {
						sprintf(g_radio_name_list[l_di], (char*)str.a, get_ttx(first_spell + l_di + 106));
					}
				} else if (show_vals == 2) {

					sprintf(g_radio_name_list[l_di], (char*)col_str_val.a,
						get_ttx(first_spell + l_di + 106),
						hero->spells[first_spell + l_di]);
				} else {
					sprintf(g_radio_name_list[l_di], (char*)col_str.a, get_ttx(first_spell + l_di + 106));
				}
			}

			retval = GUI_radio(get_ttx(217), g_spells_index[answer1].length,
					g_radio_name_list[0], g_radio_name_list[1],
					g_radio_name_list[2], g_radio_name_list[3],
					g_radio_name_list[4], g_radio_name_list[5],
					g_radio_name_list[6], g_radio_name_list[7],
					g_radio_name_list[8], g_radio_name_list[9],
					g_radio_name_list[10], g_radio_name_list[11],
					g_radio_name_list[12], g_radio_name_list[13],
					g_radio_name_list[14], g_radio_name_list[15]);

			if (retval != -1) {

				if ((hero->spells[retval + first_spell - 1] < -5) && (show_vals == 0))
				{
					sprintf(g_dtp2,	get_ttx(560), hero->alias);
					GUI_output(g_dtp2);
					retval = -1;

				} else {
					retval += first_spell -1;
				}
			}
		}

		if (retval > 0) {
			if (!g_in_fight && (g_spell_descriptions[retval].where_to_use == 1) && (show_vals == 0))
			{
				GUI_output(get_ttx(591));
				retval = -2;

			} else {
				if (g_in_fight && (g_spell_descriptions[retval].where_to_use == -1))
				{
					GUI_output(get_ttx(592));
					retval = -2;
				}
			}
		}
	} else {
		retval = -2;
	}

	return retval;
}

/**
 * \brief   makes a spell test. no AE deduction in this function.
 */
signed short test_spell(struct struct_hero *hero, signed short spell_no, signed char handicap)
{
	signed int retval;
	struct spell_descr *spell_desc;

	/* check if class is magic user */
	if ((hero->typus < HERO_TYPE_WITCH) || (check_hero(hero) == 0)) {
		return 0;
	}

	/* check if spell skill >= -5 */
	if (hero->spells[spell_no] < -5)
		return 0;

	/* check if hero has enough AE */
	if (get_spell_cost(spell_no, 0) > hero->ae)
		return -99;

	spell_desc = &g_spell_descriptions[spell_no];

	if (spell_desc->fight) {

		if (hero->enemy_id >= 10) {

			handicap += g_enemy_sheets[hero->enemy_id - 10].mr;

			//if (g_enemy_sheets[hero->enemy_id - 10].flags.mushroom) {	/* BAE-TODO: different code */
			if (((struct enemy_flags)g_enemy_sheets[hero->enemy_id - 10].flags).mushroom) { /* BAE-TODO: good */
				return 0;
			}
		} else {
			handicap += get_hero(hero->enemy_id - 1)->mr;
		}
	}

	if ((spell_no >= 1) && (spell_no <= 85)) {

#if !defined(__BORLANDC__)
		D1_INFO("%s Zauberprobe %s %+d (TaW %d)", hero->alias, names_spell[spell_no], handicap, hero->spells[spell_no]);
#endif

		handicap -= hero->spells[spell_no];

		retval = test_attrib3(hero, spell_desc->attrib1, spell_desc->attrib2, spell_desc->attrib3, handicap);

		if (retval == -99) {
			retval = -1;
		}

		return retval;
	}

	return 0;
}

/**
 * \brief   makes a spell test for all magic users in the current group
 */
signed short test_spell_group(signed short spell, signed char handicap)
{

	struct struct_hero *hero_i = get_hero(0);
	signed int i;

	for (i = 0; i <= 6; i++, hero_i++) {

		/* Check class is magicuser */
		if ((hero_i->typus >= HERO_TYPE_WITCH) &&
			/* Check class  BOGUS */
			(hero_i->typus != HERO_TYPE_NONE) &&
			/* Check in group */
			(hero_i->group_no == gs_current_group) &&
			/* Check if dead */
			!hero_i->flags.dead)
			/* Original-Bug: what if petrified, sleeping, unconcious etc. */
		{

			if (test_spell(hero_i, spell, handicap) > 0) {
				return 1;
			}
		}
	}

	return 0;
}

signed short select_magic_user(void)
{
	signed int answer;

	/* select the hero who shoulds cast a spell */
	answer = select_hero_ok(get_ttx(317));

	if (answer != -1) {
		/* valid answer => cast spell */
		return use_spell(get_hero(answer), 1, 0);
	}

	/* abort with error message */
	return -2;
}

/**
 * \brief   casts a spell
 *
 * \param   hero        	the hero who casts the spell
 * \param   selection_menu	1: select spell from menu / 0: spell is preselected in HERO_SPELL_ID
 * \param   handicap		modifier for the spell cast
 */
signed int use_spell(struct struct_hero* hero, const signed int selection_menu, signed char handicap)
{
	signed int retval = 1;
	signed int spell_id;
	signed int ae_cost;
	signed int tw_bak;
	struct spell_descr *spell_description;
	void (*func)(void);
	signed int l4;
#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 29: see below */
	signed short x;
	signed short y;
	signed int pos;
	struct dungeon_door *ptr_doors;
#endif

	if (!check_hero(hero) && !hero->flags.renegade) {

		return 0;
	}

	tw_bak = g_textbox_width;
	g_textbox_width = 3;

	if (selection_menu == 1) {

		spell_id = select_spell(hero, 0);

		if (spell_id > SP_NONE) {

			/* pointer to the spell description */
			spell_description = &g_spell_descriptions[spell_id];

			/* reset the spelltarget of the hero */
			hero->enemy_id = 0;

			if (spell_description->target_type && (spell_description->target_type != 4)) {

				if (spell_description->target_type == 1) {

					GUI_output(get_ttx(234));

				} else {

					hero->enemy_id = select_hero_from_group(get_ttx(47)) + 1;

					if (hero->enemy_id <= 0) {
						spell_id = -1;
					}
				}
			}
		}

	} else {
		spell_id = hero->spell_id;
	}

	if (spell_id > 0) {

		/* pointer to the spell description */
		spell_description = &g_spell_descriptions[spell_id];

		if (!g_in_fight && (spell_description->where_to_use == 1)) {

			/* only in fight spell */
			GUI_output(get_ttx(591));
			retval = 0;

		} else if (g_in_fight && (spell_description->where_to_use == -1)) {

			/* only in fight spell */
			GUI_output(get_ttx(592));
			retval = 0;
		}

		if (retval) {

#ifdef M302de_ORIGINAL_BUGFIX
			/* Original-Bug 29: door-specific spell handicap is not considered in a free Foramen spell (from the spellcast menu). */
			if (spell_id == SP_FORAMEN_FORAMINOR && gs_dungeon_index != DUNGEONS_NONE && (g_dng_extra_action == DNG_MENU_MODE_OPEN_DOOR || g_dng_extra_action == DNG_MENU_MODE_UNLOCK_DOOR)) {
				x = gs_x_target;
				y = gs_y_target;
				ptr_doors = g_dungeon_doors_buf;

				switch (gs_direction)
				{
					case NORTH: y--; break;
					case EAST:  x++; break;
					case SOUTH: y++; break;
					case WEST:  x--; break;
				}

				pos = DNG_POS(gs_dungeon_level, x, y);

				if ((*(g_dng_map_ptr + MAP_POS(x,y)) & 0x02) == 0) {

					/* flag 1 'unlocked' is not set -> door is locked  */
					while (ptr_doors->pos != pos) {
						/* ASSERT */
						/*
						if (ptr_doors->pos == -1) {
						D1_INFO("In free call of Foramen spell: door not found. This should not happen.\n");
						 */

						ptr_doors++;
					}
					handicap += ptr_doors->foramen_handicap;
				}
			}
#endif

			g_spelltest_result = test_spell(hero, spell_id, handicap);

			if (g_spelltest_result == -99) {

				/* prepare output */
				sprintf(g_dtp2, get_ttx(607), hero->alias);

				if (!g_in_fight) {
					GUI_output(g_dtp2);
				}

				retval = -1;

			} else if ((g_spelltest_result <= 0) || (gs_ingame_timers[INGAME_TIMER_RONDRA_NO_SPELLS] > 0)) {

				strcpy(g_dtp2, get_ttx(606));

				sub_ae_splash(hero, get_spell_cost(spell_id, 1)); /* spell failed -> half AE cost */

				if (!g_in_fight) {
					GUI_output(g_dtp2);
				}

				retval = 0;
			} else {
				/* set global spelluser variable */
				g_spelluser = hero;

				ae_cost = get_spell_cost(spell_id, 0); /* spell successful -> full AE cost */
				g_spell_special_aecost = -1;

				*g_dtp2 = '\0';

				l4 = g_tx_file_index;

				load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

				func = g_spell_handlers[spell_id];
				func();

				if ((l4 != -1) && (l4 != 222)) {
					load_tx(l4);
				}

				retval = 1;

				if (g_spell_special_aecost == 0) {

					retval = -1;

					if (!(*g_dtp2)) {
						strcpy(g_dtp2, get_ttx(606));
					}

				} else if (g_spell_special_aecost == -2) {

					strcpy(g_dtp2, get_ttx(606));
					sub_ae_splash(hero, get_spell_cost(spell_id, 1));
					retval = 0;

				} else if (g_spell_special_aecost != -1) {
					sub_ae_splash(hero, g_spell_special_aecost);
				} else {
					sub_ae_splash(hero, ae_cost);
				}

				if (!g_in_fight) {

					GUI_output(g_dtp2);

					if (retval > 0) {
						play_voc(ARCHIVE_FILE_FX17_VOC);

						if ((hero->enemy_id < 10) && (hero->enemy_id > 0) && (g_pp20_index == ARCHIVE_FILE_PLAYM_UK))
						{
							magic_heal_ani(hero);
						}
					}
				}
			}
		} else {
			retval = 0;
		}
	} else {
		retval = -1;
	}

	g_textbox_width = tw_bak;

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
