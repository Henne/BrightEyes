/*
 *	Rewrite of DSA1 v3.02_de functions of seg101 (spells 3/3)
 *	Spells: Transformation / Transmutation
 *	Functions rewritten: 26/26 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg101.cpp
 *
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg096.h"
#include "seg097.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* Transformation / Verwandlung */
void spell_adler(void)
{
	/* triggers the "spell failed" messages */
	g_spell_special_aecost = -2;

#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Adler, Wolf und Hammerhai\" ist nicht implementiert\n");
#endif
}

void spell_arcano(void)
{
	signed int target;
	signed int slot;

	/* get the spell target */
	target = get_spelluser()->target_id - 1;

	g_spelltarget = get_hero(target);

	/* get a free mod_slot */
	slot = get_free_mod_slot();

	/* MR + 2 for 1 h */
	set_mod_slot(slot, HOURS(1), (Bit8u*)&get_spelltarget()->mr, 2, (signed char)target);

	/* "Die Magieresistenz von %s steigt um 2 Punkte." */
	sprintf(g_dtp2, get_tx(98), get_spelltarget()->alias);
}

void spell_armatrutz(void)
{
	signed int max_bonus;
	signed int pos;
	signed int bonus;
	signed int slot;

	max_bonus = 0;

	/* calc the maximal RS bonus */
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug: you can get one RS point more that you have AE for */
	while (max_bonus * max_bonus < get_spelluser()->ae) {
		max_bonus++;
	}
#else
	while (max_bonus * max_bonus <= get_spelluser()->ae) {
		max_bonus++;
	}
	--max_bonus;
#endif

	/* the maximum bonus is 9 */
	if (max_bonus > 9)
		max_bonus = 9;

	/* ask the user which bonus he wants */
	sprintf(g_dtp2,	get_tx(99), max_bonus);
	bonus = GUI_input(g_dtp2, 1);

	/* fix wrong input */
	if (bonus > max_bonus)
		bonus = max_bonus;

	if (bonus != -1) {

		pos = get_hero_index(get_spelluser());
		g_spell_special_aecost = (bonus * bonus);
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(5), (Bit8u*)&get_spelluser()->rs_bonus1, (signed char)bonus, (signed char)pos);

		/* prepare output message */
		sprintf(g_dtp2,	get_tx(100), get_spelluser()->alias, bonus);

	} else {
		/* spell canceled */
		/* set AE to 0 */
		g_spell_special_aecost = 0;
		/* avoid the textbox output */
		*g_dtp2 = '\0';
	}
}

void spell_inc_ch(void)
{
	signed int target;
	signed int slot;

	/* get the spell target */
	target = get_spelluser()->target_id - 1;

	g_spelltarget = get_hero(target);

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		g_spell_special_aecost = 0;

		/* copy message text */
		strcpy(g_dtp2, get_tx(112));

		return;
	}

	/* check if CH was already increased */
	if (get_spelltarget()->attrib[ATTRIB_CH].current > get_spelltarget()->attrib[ATTRIB_CH].normal) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf(g_dtp2,	get_tx(113), get_spelltarget()->alias, get_ttx(414));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* CH + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), (Bit8u*)&get_spelltarget()->attrib[ATTRIB_CH].current, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf(g_dtp2, get_tx(101), get_spelltarget()->alias, get_ttx(414));
	}
}

void spell_feuerbann(void)
{
	signed int target;
	signed int slot;

	/* check if spell is already activated */
	if (!get_spelluser()->fireban) {

		target = get_hero_index(get_spelluser());

		slot = get_free_mod_slot();

		/* Duration = Level * 12 min */
		set_mod_slot(slot, get_spelluser()->level * MINUTES(5), (Bit8u*)&get_spelluser()->fireban, 1, (signed char)target);

		/* prepare message */
		sprintf(g_dtp2,	get_tx(102), get_spelluser()->alias);
	} else {
		/* set AP costs to 0 */
		g_spell_special_aecost = 0;
	}
}

void spell_inc_ff(void)
{
	signed int target;
	signed int slot;

	/* get the spell target */
	target = get_spelluser()->target_id - 1;

	g_spelltarget = get_hero(target);

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		g_spell_special_aecost = 0;

		/* copy message text */
		strcpy(g_dtp2, get_tx(112));

		return;
	}

	/* check if FF was already increased */
	if (get_spelltarget()->attrib[ATTRIB_FF].current > get_spelltarget()->attrib[ATTRIB_FF].normal) {

		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf(g_dtp2,	get_tx(113), get_spelltarget()->alias, get_ttx(415));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* FF + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), (Bit8u*)&get_spelltarget()->attrib[ATTRIB_FF].current, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf(g_dtp2,	get_tx(101), get_spelltarget()->alias, get_ttx(415));
	}
}

void spell_inc_ge(void)
{
	signed int target;
	signed int slot;

	/* get the spell target */
	target = get_spelluser()->target_id - 1;

	g_spelltarget = get_hero(target);

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		g_spell_special_aecost = 0;

		/* copy message text */
		strcpy(g_dtp2, get_tx(112));

		return;
	}

	/* check if GE was already increased */
	if (get_spelltarget()->attrib[ATTRIB_GE].current > get_spelltarget()->attrib[ATTRIB_GE].normal) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf(g_dtp2, get_tx(113), get_spelltarget()->alias, get_ttx(416));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* GE + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), (Bit8u*)&get_spelltarget()->attrib[ATTRIB_GE].current, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf(g_dtp2,	get_tx(101), get_spelltarget()->alias, get_ttx(416));
	}
}

void spell_inc_in(void)
{
	signed int target;
	signed int slot;

	/* get the spell target */
	target = get_spelluser()->target_id - 1;

	g_spelltarget = get_hero(target);

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		g_spell_special_aecost = 0;

		/* copy message text */
		strcpy(g_dtp2, get_tx(112));

		return;
	}

	/* check if IN was already increased */
	if (get_spelltarget()->attrib[ATTRIB_IN].current > get_spelltarget()->attrib[ATTRIB_IN].normal) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf(g_dtp2, get_tx(113), get_spelltarget()->alias, get_ttx(417));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* IN + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), (Bit8u*)&get_spelltarget()->attrib[ATTRIB_IN].current, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf(g_dtp2,	get_tx(101), get_spelltarget()->alias, get_ttx(417));
	}
}

void spell_inc_kk(void)
{
	signed int target;
	signed int slot;

	/* get the spell target */
	target = get_spelluser()->target_id - 1;

	g_spelltarget = get_hero(target);

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		g_spell_special_aecost = 0;

		/* copy message text */
		strcpy(g_dtp2, get_tx(112));

		return;
	}

	/* check if KK was already increased */
	if (get_spelltarget()->attrib[ATTRIB_KK].current > get_spelltarget()->attrib[ATTRIB_KK].normal) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf(g_dtp2,	get_tx(113), get_spelltarget()->alias, get_ttx(418));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* IN + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), (Bit8u*)&get_spelltarget()->attrib[ATTRIB_KK].current, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf(g_dtp2,	get_tx(101), get_spelltarget()->alias, get_ttx(418));
	}
}

void spell_inc_kl(void)
{
	signed int target;
	signed int slot;

	/* get the spell target */
	target = get_spelluser()->target_id - 1;

	g_spelltarget = get_hero(target);

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		g_spell_special_aecost = 0;

		/* copy message text */
		strcpy(g_dtp2, get_tx(112));

		return;
	}

	/* check if KL was already increased */
	if (get_spelltarget()->attrib[ATTRIB_KL].current > get_spelltarget()->attrib[ATTRIB_KL].normal) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf(g_dtp2,	get_tx(113), get_spelltarget()->alias, get_ttx(413));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* KL + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), (Bit8u*)&get_spelltarget()->attrib[ATTRIB_KL].current, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf(g_dtp2, get_tx(101), get_spelltarget()->alias, get_ttx(413));
	}
}

void spell_inc_mu(void)
{
	signed int target;
	signed int slot;

	/* get the spell target */
	target = get_spelluser()->target_id - 1;

	g_spelltarget = get_hero(target);

	/* check if the target is the spelluser */
	if (get_spelltarget() == get_spelluser()) {

		/* set AP costs to 0 */
		g_spell_special_aecost = 0;

		/* copy message text */
		strcpy(g_dtp2, get_tx(112));

		return;
	}

	/* check if MU was already increased */
	if (get_spelltarget()->attrib[ATTRIB_MU].current > get_spelltarget()->attrib[ATTRIB_MU].normal) {
		/* "Bei %s ist %s schon magisch gesteigert" */
		sprintf(g_dtp2,	get_tx(113), get_spelltarget()->alias, get_ttx(412));
	} else {
		/* get a free mod_slot */
		slot = get_free_mod_slot();

		/* MU + 2 for 2 hours */
		set_mod_slot(slot, HOURS(2), (Bit8u*)&get_spelltarget()->attrib[ATTRIB_MU].current, 2, (signed char)target);

		/* "Bei %s steigt %s um 2 Punkte" */
		sprintf(g_dtp2, get_tx(101), get_spelltarget()->alias, get_ttx(412));
	}
}

void spell_mutabili(void)
{
	/* triggers the "spell failed" messages */
	g_spell_special_aecost = -2;
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Mutabili\" ist nicht implementiert\n");
#endif
}

void spell_paralue(void)
{
	if (get_spelluser()->target_id >= 10) {
		/* cast at an enemy */

		/* BC-TODO: calculation of ptr could be better */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->target_id - 10];

		/* set 'petrified' flag */
		g_spelltarget_e->flags.petrified = 1;

		sprintf(g_dtp2,	get_tx(103), (char*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	} else {
		/* cast a hero */
		/* TODO: the first check can be removed, cause it would not give a message */
		if (get_spelltarget() != get_spelluser()) {

			/* set the target  */
			g_spelltarget = get_hero(get_spelluser()->target_id - 1);

			/* check again */
			if (get_spelltarget() == get_spelluser()) {

				/* never cast yourself */
				g_spell_special_aecost = 0;

				strcpy(g_dtp2, get_tx(112));
			} else {
				get_spelltarget()->flags.petrified = 1;

				/* prepare message */
				sprintf(g_dtp2, get_tx(103), get_spelltarget()->alias);
			}
		} else {
			/* set AE to 0 */
			g_spell_special_aecost = 0;
		}
	}
}

/**
 * \brief   hero spell 'Salander Mutanderer'
 *
 * Turns the target into a mushroom
 */
void spell_salander(void)
{
	/* TODO: Original-Bug: Strange effect when cast on a two-squared monster */
	signed int ae_cost;

	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->target_id - 10];

	/* read a value from that struct */
	ae_cost = g_spelltarget_e->mr * 3;

	/* set the minimal astral cost to 25 AE */
	if (ae_cost < 25)
		ae_cost = 25;

	if (get_spelluser()->ae >= ae_cost) {

		/* sets 'mushroom' flag */
		g_spelltarget_e->flags.mushroom = 1;

		/* prepare message */
		sprintf(g_dtp2, get_tx(104), (char*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));

		/* set AE cost */
		g_spell_special_aecost = ae_cost;
	} else {
		/* prepare message */
		sprintf(g_dtp2, get_ttx(607), get_spelluser()->alias);

		/* no AE cost */
		g_spell_special_aecost = 0;
	}
}

void spell_see(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"See und Fluss\" ist nicht implementiert\n");
#endif
}

/**
 * \brief   hero spell 'Visibili Vanitar'
 *
 * Makes the spell caster invisible for a certain amount of time. This allows him to skip fights in dungeons.
 */
void spell_visibili(void)
{
	signed int slot;
	signed int pos;
	signed int rounds;

	/* ask the user how many rounds he wants to be invisible */
	sprintf(g_dtp2, get_tx(105), get_spelluser()->alias);
	rounds = GUI_input(g_dtp2, 2);

	/* the spell has also no effect if it is already active */
	if ((rounds <= 0) || (get_spelluser()->invisible != 0)) {

		/* set AE to 0 */
		g_spell_special_aecost = 0;
		/* clear output string */
		*g_dtp2 = '\0';

		return;
	}

	/* check if the hero has enough AE */
	if (rounds * 5 <= get_spelluser()->ae) {

		g_spell_special_aecost = rounds * 5;
		pos = (signed short)get_hero_index(get_spelluser());
		slot = get_free_mod_slot();
		set_mod_slot(slot, (Bit32s)rounds * MINUTES(5), (Bit8u*)&get_spelluser()->invisible, 1, (signed char)pos);

		sprintf(g_dtp2,	get_tx(106), get_spelluser()->alias, GUI_get_ptr(get_spelluser()->sex, 0));
	} else {
		sprintf(g_dtp2,	get_ttx(607), get_spelluser()->alias);

		g_spell_special_aecost = 0;
	}

}

/* Transmutation / Veraenderung */

void spell_abvenenum(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Abvenenum\" ist nicht implementiert\n");
#endif
}

void spell_aeolitus(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Aeolitus\" ist nicht implementiert\n");
#endif
}

/**
 * \brief   hero spell 'Brenne, toter Stoff!'
 *
 * Ignites a lantern (1 oil is consumed) or a torch in the inventory.
 */
void spell_brenne(void)
{
	signed int oil_pos;
	signed int answer;

	signed int torch_pos;
	signed int lantern_pos;

	torch_pos = -1;
	lantern_pos = -1;

	g_spell_special_aecost = 0;

	if (g_light_type == LIGHTING_TORCH) {
		torch_pos = get_item_pos(get_spelluser(), ITEM_TORCH_OFF);
	} else {
		if (g_light_type == LIGHTING_LANTERN) {
		} else {
			torch_pos = get_item_pos(get_spelluser(), ITEM_TORCH_OFF);
		}

		lantern_pos = get_item_pos(get_spelluser(), ITEM_LANTERN_OFF);
	}

	if (torch_pos != -1) {

		if (lantern_pos != -1) {

			/* lantern and torch are available, must decide */
			sprintf(g_dtp2,	get_tx(107), get_spelluser()->alias);

			answer = GUI_radio(g_dtp2, 2,
					(char*)GUI_names_grammar(0x4000, ITEM_TORCH_OFF, 0),
					(char*)GUI_names_grammar(0x4000, ITEM_LANTERN_OFF, 0));

			if (answer == -1) {

				/* abort */
				torch_pos = lantern_pos = -1;

			} else if (answer == 1) {

				lantern_pos = -1;

			} else {

				torch_pos = -1;
			}
		}
	}

	if (torch_pos != -1) {

		/* change torch to burning torch */
		get_spelluser()->inventory[torch_pos].item_id = ITEM_TORCH_ON;

		/* set timer to 10 */
		get_spelluser()->inventory[torch_pos].lighting_timer = 10;

		/* set AP cost */
		g_spell_special_aecost = random_schick(20);

		/* prepare message */
		sprintf(g_dtp2,	get_tx(108), get_spelluser()->alias);

	} else if (lantern_pos != -1) {

		/* get position of oil */
		oil_pos = get_item_pos(get_spelluser(), ITEM_OIL);

		if (oil_pos != -1) {

			/* change lantern to burning lantern */
			get_spelluser()->inventory[lantern_pos].item_id = ITEM_LANTERN_ON;

			/* set counter to 100 */
			get_spelluser()->inventory[lantern_pos].lighting_timer = 100;

			/* drop one oil flask */
			drop_item(get_spelluser(), oil_pos, 1);

			/* give bronze flask */
			give_hero_new_item(get_spelluser(), ITEM_FLASK_BRONZE, 0, 1);

			/* set AP cost */
			g_spell_special_aecost = random_schick(20);

			/* prepare message */
			sprintf(g_dtp2,	get_tx(119), get_spelluser()->alias);
		} else {
			/* prepare message */
			sprintf(g_dtp2, get_tx(120), get_spelluser()->alias);
		}
	} else {
		/* neither torch nor lantern */

		/* prepare message */
		sprintf(g_dtp2, get_tx(121), get_spelluser()->alias);
	}
}

void spell_claudibus(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Claudibus\" ist nicht implementiert\n");
#endif
}

void spell_dunkelheit(void)
{
	/* set dunkelheit duration (level + 3) * hours */
	gs_ingame_timers[INGAME_TIMER_DARKNESS] = (Bit32s)(get_spelluser()->level + 3) * HOURS(1);

	/* copy message text */
	strcpy(g_dtp2, get_tx(109));
}

void spell_erstarre(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Erstarre\" ist nicht implementiert\n");
#endif
}

void spell_flimflam(void)
{
	/* set flim flam duration (level + 3) hours */
	gs_ingame_timers[INGAME_TIMER_FLIM_FLAM] = (Bit32s)(get_spelluser()->level + 3) * HOURS(1);

	/* copy message text */
	strcpy(g_dtp2, get_tx(110));
}

void spell_schmelze(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Schmelze\" ist nicht implementiert\n");
#endif
}

void spell_silentium(void)
{
	signed int i;
	signed int slot;
	struct struct_hero *hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && !(hero->flags.dead))
		{
			/* get a free mod_slot */
			slot = get_free_mod_slot();

			/* skill stealth + 10 for 5 minutes */
			set_mod_slot(slot, MINUTES(5), (Bit8u*)&hero->skills[TA_SCHLEICHEN], 10, (signed char)i);
		}
	}

	/* set AP cost */
	g_spell_special_aecost = 5;

	/* copy message text */
	strcpy(g_dtp2, get_tx(111));
}

void spell_sturmgebr(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Sturmgebruell\" ist nicht implementiert\n");
#endif
}

#if !defined(__BORLANDC__)
}
#endif
