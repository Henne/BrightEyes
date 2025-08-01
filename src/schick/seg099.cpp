/*
 *	Rewrite of DSA1 v3.02_de functions of seg099 (spells 1/3)
 *	Spells:		Dispell / Domination / Demonology / Elements /
 *			Movement / Healing / Clairvoyance
 *	Functions rewritten: 39/39 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg099.cpp
 *
 */

#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg039.h"
#include "seg047.h"
#include "seg074.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void spell_beherrschung(void)
{
	g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

	if (!hero_renegade(get_spelltarget())) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
	} else {
		if (get_spelltarget() == get_spelluser()) {
			strcpy(g_dtp2, get_tx(0));
			ds_writew(SPELL_SPECIAL_AECOST, 0);
		} else {
			ds_writew(SPELL_SPECIAL_AECOST, random_interval(2, 5) * 4);

			if (host_readws(get_spelluser() + HERO_AE) < ds_readws(SPELL_SPECIAL_AECOST)) {
				ds_writew(SPELL_SPECIAL_AECOST, -2);
			} else {
				and_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0xdf); /* unset 'renegade' flag */
				sprintf(g_dtp2,	get_tx(1), (char*)get_spelltarget() + HERO_NAME2);
			}
		}
	}
}

void spell_destructibo(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Destructibo\" ist nicht implementiert\n");
#endif
	ds_writew(SPELL_SPECIAL_AECOST, -2);
}

void spell_gardanium(void)
{
	signed short answer;

	/* prepare a question */
	sprintf(g_dtp2, get_tx(2), (char*)(get_spelluser() + HERO_NAME2));

	/* ask and get the answer */
	answer = GUI_input(g_dtp2, 2);

	/* clear the textbuffer */
	*g_dtp2 = '\0';

	if (answer > 0) {

		if (host_readws(get_spelluser() + HERO_AE) >= answer) {
			/* enough AE */

			/* TODO: this adds the AE to a variable no one reads */

			gs_gardanium_used_ae += answer;
			/* set AE costs */
			ds_writew(SPELL_SPECIAL_AECOST, answer);
			/* prepare the message */
			strcpy(g_dtp2, get_tx(7));
		} else {
			/* not enough AE */
			sprintf(g_dtp2, get_ttx(607), (char*)get_spelluser() + HERO_NAME2);
			/* set AE costs */
			ds_writew(SPELL_SPECIAL_AECOST, 0);
		}
	} else {
		ds_writew(SPELL_SPECIAL_AECOST, 0);
	}
}

void spell_illusionen(void)
{
	/* Set pointer to enemy target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	/* check if enemy is an illusion */
	if (enemy_illusion(get_spelltarget_e())) {

		/* AEcosts = enemy level - spelluser_level */
		ds_writew(SPELL_SPECIAL_AECOST,
			(host_readbs(get_spelltarget_e() + ENEMY_SHEET_LEVEL)
			 -host_readbs(get_spelluser() + HERO_LEVEL)) * 2);

		/* AEcost are at least 5 */
		if (ds_readws(SPELL_SPECIAL_AECOST) < 5)
			ds_writew(SPELL_SPECIAL_AECOST, 5);

		/* check if spelluser has enough AE */
		if (host_readws(get_spelluser() + HERO_AE) < ds_readws(SPELL_SPECIAL_AECOST)) {
			/* NO: spell has no effect */
			ds_writew(SPELL_SPECIAL_AECOST, -2);
		} else {
			/* YES: spell has effect */
			ds_writew(SPELL_ILLUSIONEN, 1);
			/* kill enemy */
			or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS1, 1); /* set 'dead' flag */
		}
	} else {
		/* print a failure message */
		sprintf(g_dtp2,	get_tx(3),
			(char*)GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1));

		/* costs 2 AE */
		ds_writew(SPELL_SPECIAL_AECOST, 2);
	}
}

void spell_verwandlung(void)
{
	signed short i;

	/* set spelltarget */
	g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

	if (hero_petrified(get_spelltarget())) {

		/* set AEcosts */
		ds_writew(SPELL_SPECIAL_AECOST, random_schick(10) * 5);

		/* check if spelluser has enough AE */
		if (host_readws(get_spelluser() + HERO_AE) < ds_readws(SPELL_SPECIAL_AECOST)) {
			/* NO: spell has no effect */
			ds_writew(SPELL_SPECIAL_AECOST, -2);
		} else {
			/* YES: spell has effect */
			/* unset petrified bit */
			and_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0xfb); /* unset 'petrified' flag */
			sprintf(g_dtp2,	get_tx(4), (char*)get_spelltarget() + HERO_NAME2);
		}
	} else {
		if (hero_transformed(get_spelltarget())) {

			and_ptr_bs(get_spelltarget() + HERO_FLAGS2, 0xbf); /* unset 'transformed' flag */

			/* increase attributes */
			for (i = 0; i <= 6; i++)
				inc_ptr_bs(get_spelltarget() + HERO_ATTRIB + i * 3);
			sprintf(g_dtp2,	get_ttx(565), (char*)get_spelltarget() + HERO_NAME2);
		} else {

#ifdef M302de_ORIGINAL_BUGFIX
			/* Broken format string, %S must be %s */
			host_writeb(get_tx(5) + 5, 's');
#endif

			sprintf(g_dtp2,	get_tx(5), (char*)get_spelltarget() + HERO_NAME2);
			ds_writew(SPELL_SPECIAL_AECOST, 0);
		}
	}
}

/**
 * \brief   hero spell 'Band und Fessel'
 *
 * cast on ememy: cannot do anything any more (for the whole fight)
 * cast on hero: every turn of the fight, a MU + 2 probe is executed.
 * 	Success: spell is broken, can act normally again.
 * 	Failure: MU - 2 for 7 hours, cannot do anything in this turn.
 */
void spell_band(void)
{
	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) >= 10) {
		/* cast enemy */

		/* Set pointer to enemy target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

		if (host_readbs(get_spelltarget_e() + ENEMY_SHEET_GFX_ID) == 0x1c) {
			/* does not work on skeletons */
			ds_writew(SPELL_SPECIAL_AECOST, -2);
			return;
		}

		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS1, 0x20); /* set 'tied' flag */

		sprintf(g_dtp2, get_tx(6),
				(Bit8u*)(GUI_names_grammar((signed short)0x8000,
					host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));
	} else {
		/* cast hero */

		/* Set pointer to hero target */
		g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

		if (get_spelltarget() == get_spelluser()) {
			/* don't cast yourself */

			/* set AE costs */
			ds_writew(SPELL_SPECIAL_AECOST, 0);

			/* prepare message */
			strcpy(g_dtp2, get_tx(112));
		} else {
			/* set flag */
			or_ptr_bs(get_spelltarget() + HERO_FLAGS1, 0x80); /* set 'tied' flag */

			/* prepare message */
			sprintf(g_dtp2, get_tx(6), (char*)get_spelltarget() + HERO_NAME2);
		}
	}
}

void spell_bannbaladin(void)
{

	/* Set pointer to enemy target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	if (host_readbs(get_spelltarget_e() + ENEMY_SHEET_IS_ANIMAL) != 0) {
		/* spell does not work on animals */

		ds_writew(SPELL_SPECIAL_AECOST, 0);

		sprintf(g_dtp2,	get_tx(8),
				(Bit8u*)(GUI_names_grammar(0,
					host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));
	} else {

		if (host_readbs(get_spelltarget_e() + ENEMY_SHEET_GFX_ID) == 0x1c) {
			/* spell does not work on skeletons */
			ds_writew(SPELL_SPECIAL_AECOST, -2);
			return;
		}

		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS2, 1); /* set 'tame' flag */

		sprintf(g_dtp2,	get_tx(9),
				(Bit8u*)(GUI_names_grammar((signed short)0x8000,
					host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));
	}
}

void spell_boeser_blick(void)
{
	/* set attacked foe */
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	/* this spell does not work on all kind of skeletons */
	if (host_readb(get_spelltarget_e() + ENEMY_SHEET_GFX_ID) == 0x1c) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
	} else {
		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS2, 2); /* set 'renegade' flag */

		host_writeb(get_spelltarget_e() + ENEMY_SHEET_ATTACKS, 2); /* set number of attacks to 2 */

		/* prepare message */
		sprintf(g_dtp2,	get_tx(10),
			(char*)(GUI_names_grammar((signed short)0x8000, host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));

	}
}

void spell_grosse_gier(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Grosse Gier\" ist nicht implementiert\n");
#endif
	ds_writew(SPELL_SPECIAL_AECOST, -2);
}

void spell_grosse_ver(void)
{
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	/* this spell does not work on all kind of skeletons */
	if (host_readb(get_spelltarget_e() + ENEMY_SHEET_GFX_ID) == 0x1c) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
		return;
	} else {

		/* cost is 8 AP */
		ds_writew(SPELL_SPECIAL_AECOST, 8);
	}

	/* Sub -2 from AT */
	host_writeb(get_spelltarget_e() + ENEMY_SHEET_AT,
		host_readb(get_spelltarget_e() + ENEMY_SHEET_AT) - 2);

	/* Sub -2 from PA */
	host_writeb(get_spelltarget_e() + ENEMY_SHEET_PA,
		host_readb(get_spelltarget_e() + ENEMY_SHEET_PA) - 2);
}

void spell_herrdertiere(void)
{

	/* Set pointer to enemy target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	if (!host_readbs(get_spelltarget_e() + ENEMY_SHEET_IS_ANIMAL)) {
		/* spell does not work on animals */

		ds_writew(SPELL_SPECIAL_AECOST, 0);

		sprintf(g_dtp2,	get_tx(11),
				(Bit8u*)(GUI_names_grammar(0,
					host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));
	} else {

		ds_writew(SPELL_SPECIAL_AECOST, host_readbs(get_spelltarget_e() + ENEMY_SHEET_FIRSTAP));

		if (host_readws(get_spelluser() + HERO_AE) < ds_readws(SPELL_SPECIAL_AECOST)) {
			ds_writew(SPELL_SPECIAL_AECOST, -2);
		} else {

			or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS2, 1); /* set 'tame' flag */

			sprintf(g_dtp2,	get_tx(9),
				(Bit8u*)(GUI_names_grammar((signed short)0x8000,
					host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));
		}
	}
}

void spell_horriphobus(void)
{

	/* Set pointer to enemy target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	/* this spell does not work on all kind of skeletons */
	if (host_readb(get_spelltarget_e() + ENEMY_SHEET_GFX_ID) == 0x1c) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
	} else {
		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS2, 4); /* set 'scared' flag */
		and_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS2, 0xfd); /* unset 'renegade' flag */

		sprintf(g_dtp2,	get_tx(12),
			(Bit8u*)(GUI_names_grammar((signed short)0x8000,
				host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));
	}
}

void spell_magischerraub(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Magischer Raub\" ist nicht implementiert\n");
#endif
}

void spell_respondami(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Respondami\" ist nicht implementiert\n");
#endif
}

void spell_sanftmut(void)
{
	spell_herrdertiere();
}

void spell_somnigravis(void)
{

	if (host_readbs(get_spelluser() + HERO_ENEMY_ID) >= 10) {
		/* cast an enemy */

		/* Set pointer to enemy target */
		ds_writed(SPELLTARGET_E,
			(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

		/* this spell does not work on all kind of skeletons */
		if (host_readb(get_spelltarget_e() + ENEMY_SHEET_GFX_ID) == 0x1c) {
			ds_writew(SPELL_SPECIAL_AECOST, -2);
			return;
		}

		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS1, 2); /* set 'asleep' flag */

		/* prepare message */
		sprintf(g_dtp2,	get_tx(13),
			(Bit8u*)(GUI_names_grammar((signed short)0x8000,
				host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));
		return;
	}

	/* cast hero */

	/* Set pointer to hero target */
	g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

	if (get_spelltarget() == get_spelluser()) {
		/* don't cast yourself */

		/* set AE costs */
		ds_writew(SPELL_SPECIAL_AECOST, 0);

		/* prepare message */
		strcpy(g_dtp2, get_tx(112));
	} else {
		/* set the flag */
		or_ptr_bs(get_spelltarget() + HERO_FLAGS1, 2); /* set 'sleep' flag */

		/* prepare message */
		sprintf(g_dtp2, get_tx(13), (char*)get_spelltarget() + HERO_NAME2);
	}
}

void spell_zwingtanz(void)
{
	/* Set pointer to enemy target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	/* this spell does not work on all kind of skeletons */
	if (host_readb(get_spelltarget_e() + ENEMY_SHEET_GFX_ID) == 0x1c) {
		ds_writew(SPELL_SPECIAL_AECOST, -2);
	} else {

		/* set the flag */
		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS2, 8); /* set 'dancing' flag */

		/* prepare message */
		sprintf(g_dtp2,	get_tx(14),
			(Bit8u*)(GUI_names_grammar((signed short)0x8000,
				host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));
	}
}

/* Demonologie / Demonology */

void spell_furor_blut(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Furor Blut\" ist nicht implementiert\n");
#endif
	ds_writew(SPELL_SPECIAL_AECOST, -2);
}

void spell_geister_bannen(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Geister bannen\" ist nicht implementiert\n");
#endif
}

void spell_geister_rufen(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Geister rufen\" ist nicht implementiert\n");
#endif
	ds_writew(SPELL_SPECIAL_AECOST, -2);
}

void spell_heptagon(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Heptagon\" ist nicht implementiert\n");
#endif
	ds_writew(SPELL_SPECIAL_AECOST, -2);
}

void spell_kraehenruf(void)
{
	signed short caster_level;
	signed short pa_value;
	signed short i;
	signed short damage;


	caster_level = host_readbs(get_spelluser() + HERO_LEVEL) + 2;

	pa_value = get_attackee_parade();

	damage = 0;

	for (i = 0; i < caster_level; i++) {

		/* only 40 % chance of success */
		if (random_schick(20) <= 8) {

			if (random_schick(20) > pa_value) {
				damage++;
			}
		}
	}

	FIG_do_spell_damage(damage);
}

void spell_skelettarius(void)
{
	Bit8u *fighter;
	signed short x;
	signed short y;
	signed char unk;

	/* Set pointer to enemy target */
	ds_writed(SPELLTARGET_E,
		(Bit32u)(p_datseg + host_readbs(get_spelluser() + HERO_ENEMY_ID) * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS - 10*SIZEOF_ENEMY_SHEET)));

	/* check if the enemy is dead */
	if (!enemy_dead(get_spelltarget_e())) {

		/* prepare message */
		sprintf(g_dtp2,	get_tx(15),
			(Bit8u*)(GUI_names_grammar((signed short)0x8000,
				host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));

		/* set ae costs */
		ds_writew(SPELL_SPECIAL_AECOST, 0);
	} else {

		/* prepare message */
		sprintf(g_dtp2,	get_tx(16),
			(Bit8u*)(GUI_names_grammar((signed short)0x8000,
				host_readbs(get_spelltarget_e() + ENEMY_SHEET_MON_ID), 1)));

		fighter = (Bit8u*)(FIG_get_ptr(host_readbs(get_spelltarget_e() + ENEMY_SHEET_FIGHTER_ID)));

		x = host_readbs(fighter + FIGHTER_CBX);
		y = host_readbs(fighter + FIGHTER_CBY);

		if (host_readbs(fighter + FIGHTER_TWOFIELDED) != -1) {
			FIG_remove_from_list(ds_readbs(FIG_TWOFIELDED_TABLE + host_readbs(fighter + FIGHTER_TWOFIELDED)), 0);
		}
#ifdef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 1:
		 * If the dead target monster is lying on top of the body of another dead monster,
		 * after the 'Skelettarius' the other body is still displayed, but cannot be selected for 'Skelettarius'.
		 *
		 * Fix: store and restore the FIGHTER_OBJ_ID value. */
		signed char obj_id_bak = host_readbs(fighter + FIGHTER_OBJ_ID);
#endif
#ifdef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 2:
		 * reported at https://www.crystals-dsa-foren.de/showthread.php?tid=5039&pid=148171#pid148171
		 * Every 'Skelettarius' spell adds 1288 (=0x508) bytes at the end of FIGHTOBJ_BUF for the animation of the fighter.
		 * If too many Skelettarius spells are cast, this causes an overflow in FIG_load_enemy_sprites(..)
		 *
		 * Hacky fix by NRS:
		 * modify FIGHTOBJ_BUF_SEEK_PTR to point to the buffer entry of the enemy which is replaced by 'Skelettarius',
		 * such that the buffer space is reused.
		 * restore the pointer later, and adjust FIGHTOBJ_BUF_FREESPACE to the right value
		 * https://www.crystals-dsa-foren.de/showthread.php?tid=5039&pid=148252#pid148252
		 * https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=166097#pid166097
		 * */
                Bit8u* buf_seek_ptr_bak = g_fightobj_buf_seek_ptr; /* backup the entry of FIGHTOBJ_BUF_SEEK_PTR */
		g_fightobj_buf_seek_ptr = (unsigned char*)host_readd(fighter + FIGHTER_GFXBUF);
#endif

		FIG_remove_from_list(host_readbs(get_spelltarget_e() + ENEMY_SHEET_FIGHTER_ID), 0);

		unk = host_readbs(get_spelltarget_e() + ENEMY_SHEET_ATTACKS_LEFT);


		fill_enemy_sheet(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 10, 0x10, 0);

		FIG_load_enemy_sprites(get_spelltarget_e() + ENEMY_SHEET_MON_ID, x, y);
#ifdef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 2:
		 * set FIGHTOBJ_BUF_FREESPACE and FIGHTOBJ_BUF_SEEK_PTR to the correct values as discussed above */
		g_fightobj_buf_freespace += 0x508L;
		g_fightobj_buf_seek_ptr = buf_seek_ptr_bak;
#endif

		/* zombie will fight for the heroes */
		or_ptr_bs(get_spelltarget_e() + ENEMY_SHEET_FLAGS2, 2); /* set 'renegade' flag */
		host_writebs(get_spelltarget_e() + ENEMY_SHEET_ATTACKS_LEFT, unk);
#ifdef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 1:
		 * restore the FIGHTER_OBJ_ID value. */
		fighter = (Bit8u*)(FIG_get_ptr(host_readbs(get_spelltarget_e() + ENEMY_SHEET_FIGHTER_ID)));
		host_writebs(fighter + FIGHTER_OBJ_ID, obj_id_bak);
#endif
	}
}

/* Elemente / Elements */

void spell_elementare(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Elementare\" ist nicht implementiert\n");
#endif
	ds_writew(SPELL_SPECIAL_AECOST, -2);
}

void spell_nihilatio(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Nihilatio\" ist nicht implementiert\n");
#endif
}

void spell_solidirid(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Solidirid\" ist nicht implementiert\n");
#endif
}


/* Bewegung / Movement */

void spell_axxeleratus(void)
{
	signed short hero_pos;
	signed short slot;

	hero_pos = host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1;

	/* Set pointer to hero target */
	g_spelltarget = get_hero(hero_pos);

	if (!host_readbs(get_spelltarget() + HERO_AXXELERATUS)) {

		/* AT + 2 for 1 minute (= 10 rounds of fighting) */
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(1),
			get_spelltarget() + HERO_AT + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE),
			2, (signed char)hero_pos);

		/* PA + 2 for 1 minute (= 10 rounds of fighting) */
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(1),
			get_spelltarget() + HERO_PA + host_readbs(get_spelltarget() + HERO_WEAPON_TYPE),
			2, (signed char)hero_pos);

		/* set Axxeleratus flag (yields +4 BP, second action phase per round of fighting) for 1 minute (= 10 rounds of fighting) */
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(1),
			get_spelltarget() + HERO_AXXELERATUS,
			1, (signed char)hero_pos);

		/* prepare message */
		sprintf(g_dtp2,	get_tx(17), (char*)get_spelltarget() + HERO_NAME2);

	} else {
		/* axxeleratus already active */
		ds_writew(SPELL_SPECIAL_AECOST, -2);
	}
}

void spell_foramen(void)
{
	signed short x;
	signed short y;

	if
#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 28:
	 * Free Foramen spell (from the spellcast menu) can open closed dors only if the 'open door' submenu has been activated before (showing the three symbols smash, pick and spell). */
	(ds_readws(DNG_MENU_MODE) != DNG_MENU_MODE_UNLOCK_DOOR)
#else
	(gs_dungeon_index == DUNGEONS_NONE || (ds_readws(DNG_MENU_MODE) != DNG_MENU_MODE_UNLOCK_DOOR && ds_readws(DNG_MENU_MODE) != DNG_MENU_MODE_OPEN_DOOR))
#endif
	{
		/* check if the party is in front of a closed door in a dungeon */
		return;
	}

	x = gs_x_target;
	y = gs_y_target;

	switch (gs_direction) {
		case 0:	y--; break;
		case 1:	x++; break;
		case 2: y++; break;
		case 3: x--; break;
	}

	and_ptr_bs((Bit8u*)ds_readd(DNG_MAP_PTR) + y * 16 + x, 0x0f); /* clear higher 4 bits */
	or_ptr_bs((Bit8u*)ds_readd(DNG_MAP_PTR) + y * 16 + x, DNG_TILE_OPEN_DOOR << 4);
	ds_writeb(STEPTARGET_FRONT, host_readbs((Bit8u*)ds_readd(DNG_MAP_PTR) + y * 16 + x));

	DNG_open_door();

	add_hero_ap(get_spelluser(), 1); /* hero gets 1 AP for successful lock pick */

	ds_writebs((NEW_MENU_ICONS + 6), ds_writebs((NEW_MENU_ICONS + 7), ds_writebs((NEW_MENU_ICONS + 8), MENU_ICON_NONE)));
	ds_writew(REDRAW_MENUICONS, 1);
}

void spell_motoricus(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Motoricus\" ist nicht implementiert\n");
#endif
}

void spell_spurlos(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Spurlos\" ist nicht implementiert\n");
#endif
}

void spell_transversalis(void)
{
	if (!gs_dungeon_index && !gs_current_town) {
		/* cannot be used outside of a dungeon or a town */

		/* prepare message */
		strcpy(g_dtp2, get_tx(18));

	} else {
		/* set spell costs */
		ds_writew(SPELL_SPECIAL_AECOST, select_teleport_dest());

		if (ds_readws(SPELL_SPECIAL_AECOST) > 0) {
			/* check AE */
			if (host_readws(get_spelluser() + HERO_AE) < ds_readws(SPELL_SPECIAL_AECOST)) {
				/* abort */
				ds_writew(SPELL_SPECIAL_AECOST, -2);
			} else {
				/* play sound */
				play_voc(ARCHIVE_FILE_FX16_VOC);

				/* set new coordinates */
				gs_x_target = (ds_readws(AUTOMAP_SELX));
				gs_y_target = (ds_readws(AUTOMAP_SELY));
			}
		}
	}

	ds_writew(AUTOMAP_SELX, ds_writew(AUTOMAP_SELY, -1));
}

void spell_ueber_eis(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Ueber Eis\" ist nicht implementiert\n");
#endif
	ds_writew(SPELL_SPECIAL_AECOST, -2);
}


/* Heilung / Healing */

void spell_balsam(void)
{

	signed short le_to_heal;

	/* Set pointer to hero target */
	g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

	ds_writew(SPELL_SPECIAL_AECOST, 0);

	if ((g_autofight != 0) ||
		((host_readbs(get_spelluser() + HERO_NPC_ID) != 0) && g_in_fight))
	{
		/* automatic */
		le_to_heal = (host_readws(get_spelltarget() + HERO_LE_ORIG) - host_readws(get_spelltarget() + HERO_LE)) / 2;
	} else {
		/* prepare message */
		sprintf(g_dtp2,	get_tx(19), (char*)get_spelluser() + HERO_NAME2, (char*)get_spelltarget() + HERO_NAME2);

		/* ask question */
		le_to_heal = GUI_input(g_dtp2, 2);

		/* terminate string */
		*g_dtp2 = '\0';
	}

	if (le_to_heal != -1) {

		if (le_to_heal < 7) {
			/* AE-cost is at least 7 */
			ds_writew(SPELL_SPECIAL_AECOST, 7);
			/* TODO: potential ORIGINAL-BUG: What if le_to_heal is bigger than the missing LE? */
		} else {
			if (host_readws(get_spelltarget() + HERO_LE_ORIG) - host_readws(get_spelltarget() + HERO_LE) < le_to_heal) {
				/* spellcaster wants to heal more LE than wiche are missing */
				ds_writew(SPELL_SPECIAL_AECOST, host_readws(get_spelltarget() + HERO_LE_ORIG) - host_readws(get_spelltarget() + HERO_LE));
				le_to_heal = ds_readws(SPELL_SPECIAL_AECOST);
				/* reduce le and AE_COST to the amount of LE which is actually missing */
				/* This is in accordance with DSA3 rules. The spell costs are at least 7 AE, unless the spellcaster does not have 7 AE any more. */
			} else {
				ds_writew(SPELL_SPECIAL_AECOST, le_to_heal);
			}
		}

		if (host_readws(get_spelluser() + HERO_AE) < ds_readws(SPELL_SPECIAL_AECOST)) {
			/* not enough AE */
			ds_writew(SPELL_SPECIAL_AECOST, host_readws(get_spelluser() + HERO_AE));
			le_to_heal = ds_readws(SPELL_SPECIAL_AECOST);
			/* reduce AE-costs and 'le_to_heal' to the available AE */
		}

		add_hero_le(get_spelltarget(), le_to_heal);
	}
}

void spell_hexenspeichel(void)
{
	signed short le;

	/* Set pointer to hero target */
	g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

	/* set costs to 0 */
	ds_writew(SPELL_SPECIAL_AECOST, 0);

	if (get_spelltarget() == get_spelluser()) {
		/* spell cannot be used to heal yourself */

		/* prepare message */
		sprintf(g_dtp2,	get_tx(20), (char*)get_spelluser() + HERO_NAME2);
		return;
	}

	/* prepare question */
	sprintf(g_dtp2, get_tx(19), (char*)get_spelluser() + HERO_NAME2, (char*)get_spelltarget() + HERO_NAME2);

	le = GUI_input(g_dtp2, 2);

	*g_dtp2 = '\0';

	if (le != -1) {

		if (host_readws(get_spelltarget() + HERO_LE_ORIG) - host_readws(get_spelltarget() + HERO_LE) < le * 2) {
			ds_writew(SPELL_SPECIAL_AECOST,
				(host_readws(get_spelltarget() + HERO_LE_ORIG) - host_readws(get_spelltarget() + HERO_LE)) * 2);
		} else {
			if (host_readws(get_spelluser() + HERO_AE ) < ds_readws(SPELL_SPECIAL_AECOST)) {
				ds_writew(SPELL_SPECIAL_AECOST, host_readws(get_spelluser() + HERO_AE));
			} else {
				ds_writew(SPELL_SPECIAL_AECOST, le * 2);
			}
		}

		add_hero_le(get_spelltarget(), ds_readws(SPELL_SPECIAL_AECOST) / 2);
	}
}

void spell_klarum_purum(void)
{
	signed short poison;

	/* Set pointer to hero target */
	g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

	poison = hero_is_poisoned(get_spelltarget());

	if (!poison) {

		/* prepare message */
		sprintf(g_dtp2, get_tx(21), (char*)get_spelltarget() + HERO_NAME2);
		ds_writew(SPELL_SPECIAL_AECOST, 0);
		return;
	}

	/* AE-cost = poison cost */
	ds_writew(SPELL_SPECIAL_AECOST, ds_readws(POISON_PRICES + poison * 2));

	if (host_readws(get_spelluser() + HERO_AE) < ds_readws(SPELL_SPECIAL_AECOST)) {
		/* not enough AE */
		ds_writew(SPELL_SPECIAL_AECOST, -2);
	} else {
		/* reset poison */
		host_writeb(get_spelltarget() + poison * 5 + 0xd7, 0);
		host_writeb(get_spelltarget() + poison * 5 + 0xd6, 1);

		/* prepare message */
		sprintf(g_dtp2,	get_tx(22), (char*)get_spelltarget() + HERO_NAME2);
	}
}

void spell_ruhe_koerper(void)
{
	/* Set pointer to hero target */
	g_spelltarget = get_hero(host_readbs(get_spelluser() + HERO_ENEMY_ID) - 1);

	/* set the flag */
	host_writeb(get_spelltarget() + HERO_RUHE_KOERPER, 1);

	/* prepare message */
	sprintf(g_dtp2, get_tx(51), (char*)get_spelltarget() + HERO_NAME2);
}

void spell_tiere_heilen(void)
{

#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Tiere heilen\" ist nicht implementiert und dient nur zum AE ausgeben.\n");
#endif
	signed short ae;

	/* set AE costs to 0 */
	ds_writew(SPELL_SPECIAL_AECOST, 0);

	/* prepare message */
	sprintf(g_dtp2, get_tx(23), (char*)get_spelluser() + HERO_NAME2);

	/* ask how many AE should be spent */
	ae = GUI_input(g_dtp2, 2);

	/* terminate string */
	*g_dtp2 = '\0';

	if (ae != -1) {
		/* set AE costs to AE */
		ds_writew(SPELL_SPECIAL_AECOST, ae);
	}
}



/* Hellsicht / Clarvoyance */

void spell_adleraug(void)
{
	signed short slot;
	signed short hero_pos;

	hero_pos = get_hero_index(get_spelluser());

	slot = get_free_mod_slot();

	/* Perception / Sinnesschaerfe + 7 */
	set_mod_slot(slot, MINUTES(6), get_spelluser() + (HERO_TALENTS + TA_SINNESSCHAERFE), 7, (signed char)hero_pos);

	/* prepare message */
	sprintf(g_dtp2,	get_tx(24), (char*)get_spelluser() + HERO_NAME2);
}

char* spell_analues(void)
{
	signed short item_id;
	signed short i;
	signed short item_pos;

	/* set analyzation capabilities */
	if (gs_in_academy == 99) {
		ds_writew(SPELLTEST_RESULT, 99);
	}

	item_pos = select_item_to_drop(get_spelluser());

#ifdef M302de_ORIGINAL_BUGFIX
	/* If the player cancels item selection or has no items select_item_to_drop() returns -1.
	The original uses the return value to calculate an index, whithout checking for this. */
	if (item_pos == -1) item_id = 0;
	else item_id = host_readws(get_spelluser() + SIZEOF_INVENTORY * item_pos + HERO_INVENTORY + INVENTORY_ITEM_ID);
#else
	item_id = host_readws(get_spelluser() + SIZEOF_INVENTORY * item_pos + HERO_INVENTORY + INVENTORY_ITEM_ID);
#endif

	strcpy(g_text_output_buf, get_tx(52));

	if (item_id) {

		for (i = 0; ds_readws(ANALUES_ITEMS + i * SIZEOF_ANALUES_ITEM + ANALUES_ITEM_ID) != -1; i++) {

			if (ds_readws(ANALUES_ITEMS + i * SIZEOF_ANALUES_ITEM + ANALUES_ITEM_ID) == item_id) {

				/* check if the spellcaster is able to analyze this item */

				/* The following is pretty hacky. The Analues spell was already performed, before calling this function,
				 * without knowing the exact handicap. The handicap is considered only now, via comparing to the
				 * SPELLTEST_RESULT global variable.
				 * This a posteriori adjustment is only possible since not the original DSA 2/3 spell test logic is used,
				 * but a lighter "pool version".
				 * Also, it results in the Original-Bug that, in the case that the +0 spell test was successful, but with handicap it is not,
				 * the full AE cost has to be payed. (Normally, you get a 1/2 reduction for a failed attempt.) */
				if (ds_readws((ANALUES_ITEMS + ANALUES_ITEM_HANDICAP) + i * SIZEOF_ANALUES_ITEM) <= ds_readws(SPELLTEST_RESULT)) {

					/* copy the matching result string */
					strcpy(g_text_output_buf, get_tx(ds_readbs((ANALUES_ITEMS + ANALUES_ITEM_DTP) + i * SIZEOF_ANALUES_ITEM)));

					or_ptr_bs(get_spelluser() + item_pos * SIZEOF_INVENTORY + (HERO_INVENTORY + INVENTORY_FLAGS), 0x80); /* set 'magic_revealed' flag */
					break;
				} else {
					/* nothing found string */
					strcpy(g_text_output_buf, get_tx(55));
					break;
				}
			}
		}
	}

	if (gs_in_academy != 99) {

		/* prepare message */
		sprintf(g_dtp2, get_tx(53), (char*)get_spelluser() + HERO_NAME2, g_text_output_buf);
	}

	return g_text_output_buf;
}

#if !defined(__BORLANDC__)
}
#endif
