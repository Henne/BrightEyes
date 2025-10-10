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

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

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
	g_spelltarget = (struct struct_hero*)get_hero(get_spelluser()->enemy_id - 1);

	if (!get_spelltarget()->flags.renegade) {
		g_spell_special_aecost = -2;
	} else {
		if (get_spelltarget() == get_spelluser()) {
			strcpy(g_dtp2, get_tx(0));
			g_spell_special_aecost = 0;
		} else {
			g_spell_special_aecost = random_interval(2, 5) * 4;

			if (get_spelluser()->ae < g_spell_special_aecost) {
				g_spell_special_aecost = -2;
			} else {
				get_spelltarget()->flags.renegade = 0;

				sprintf(g_dtp2,	get_tx(1), get_spelltarget()->alias);
			}
		}
	}
}

void spell_destructibo(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Destructibo\" ist nicht implementiert\n");
#endif
	g_spell_special_aecost = -2;
}

void spell_gardanium(void)
{
	signed short answer;

	/* prepare a question */
	sprintf(g_dtp2, get_tx(2), get_spelluser()->alias);

	/* ask and get the answer */
	answer = GUI_input(g_dtp2, 2);

	/* clear the textbuffer */
	*g_dtp2 = '\0';

	if (answer > 0) {

		if (get_spelluser()->ae >= answer) {
			/* enough AE */

			/* TODO: this adds the AE to a variable no one reads */

			gs_gardanium_used_ae += answer;
			/* set AE costs */
			g_spell_special_aecost = answer;
			/* prepare the message */
			strcpy(g_dtp2, get_tx(7));
		} else {
			/* not enough AE */
			sprintf(g_dtp2, get_ttx(607), get_spelluser()->alias);
			/* set AE costs */
			g_spell_special_aecost = 0;
		}
	} else {
		g_spell_special_aecost = 0;
	}
}

void spell_illusionen(void)
{
	/* Set pointer to enemy target */
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

	/* check if enemy is an illusion */
	if (g_spelltarget_e->flags.illusion) {

		/* AEcosts = enemy level - spelluser_level */
		g_spell_special_aecost = (g_spelltarget_e->level - get_spelluser()->level) * 2;

		/* AEcost are at least 5 */
		if (g_spell_special_aecost < 5)
			g_spell_special_aecost = 5;

		/* check if spelluser has enough AE */
		if (get_spelluser()->ae < g_spell_special_aecost) {
			/* NO: spell has no effect */
			g_spell_special_aecost = -2;
		} else {
			/* YES: spell has effect */
			g_spell_illusionen = 1;
			/* set 'dead' flag */
			g_spelltarget_e->flags.dead = 1;
		}
	} else {
		/* print a failure message */
		sprintf(g_dtp2,	get_tx(3), GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));

		/* costs 2 AE */
		g_spell_special_aecost = 2;
	}
}

void spell_verwandlung(void)
{
	signed short i;

	/* set spelltarget */
	g_spelltarget = (struct struct_hero*)get_hero(get_spelluser()->enemy_id - 1);

	if (get_spelltarget()->flags.petrified) {

		/* set AEcosts */
		g_spell_special_aecost = random_schick(10) * 5;

		/* check if spelluser has enough AE */
		if (get_spelluser()->ae < g_spell_special_aecost) {
			/* NO: spell has no effect */
			g_spell_special_aecost = -2;
		} else {
			/* YES: spell has effect */
			/* unset petrified bit */
			get_spelltarget()->flags.petrified = 0;

			sprintf(g_dtp2,	get_tx(4), get_spelltarget()->alias);
		}
	} else {
		if (get_spelltarget()->flags.transformed) {

			get_spelltarget()->flags.transformed = 0;

			/* increase attributes */
			for (i = 0; i <= 6; i++) {
				get_spelltarget()->attrib[i].current++;
			}
			sprintf(g_dtp2,	get_ttx(565), get_spelltarget()->alias);
		} else {

#ifdef M302de_ORIGINAL_BUGFIX
			/* Broken format string, %S must be %s */
			get_tx(5)[5] = 's';
#endif

			sprintf(g_dtp2,	get_tx(5), get_spelltarget()->alias);
			g_spell_special_aecost = 0;
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
	if (get_spelluser()->enemy_id >= 10) {
		/* cast enemy */

		/* Set pointer to enemy target */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

		if (g_spelltarget_e->gfx_id == 0x1c) {
			/* does not work on skeletons */
			g_spell_special_aecost = -2;
			return;
		}

		/* set 'tied' flag */
		g_spelltarget_e->flags.tied = 1;

		sprintf(g_dtp2, get_tx(6), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	} else {
		/* cast hero */

		/* Set pointer to hero target */
		g_spelltarget = (struct struct_hero*)get_hero(get_spelluser()->enemy_id - 1);

		if (get_spelltarget() == get_spelluser()) {

			/* don't cast yourself */

			/* set AE costs */
			g_spell_special_aecost = 0;

			/* prepare message */
			strcpy(g_dtp2, get_tx(112));
		} else {
			/* set flag */
			get_spelltarget()->flags.tied = 1;

			/* prepare message */
			sprintf(g_dtp2, get_tx(6), get_spelltarget()->alias);
		}
	}
}

void spell_bannbaladin(void)
{
	/* Set pointer to enemy target */
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

	if (g_spelltarget_e->is_animal) {

		/* spell does not work on animals */
		g_spell_special_aecost = 0;

		sprintf(g_dtp2,	get_tx(8), (Bit8u*)GUI_names_grammar(0, g_spelltarget_e->mon_id, 1));
	} else {

		if (g_spelltarget_e->gfx_id == 0x1c) {
			/* spell does not work on skeletons */
			g_spell_special_aecost = -2;
			return;
		}

		/* set 'tame' flag */
		g_spelltarget_e->flags.tame = 1;

		sprintf(g_dtp2,	get_tx(9), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	}
}

void spell_boeser_blick(void)
{
	/* set attacked foe */
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

	/* this spell does not work on all kind of skeletons */
	if (g_spelltarget_e->gfx_id == 0x1c) {

		g_spell_special_aecost = -2;

	} else {

		/* set 'renegade' flag */
		g_spelltarget_e->flags.renegade = 1;

		/* set number of attacks to 2 */
		g_spelltarget_e->attacks = 2;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(10), (char*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	}
}

void spell_grosse_gier(void)
{
#if !defined(__BORLANDC__)
	D1_INFO("Zauberspruch \"Grosse Gier\" ist nicht implementiert\n");
#endif
	g_spell_special_aecost = -2;
}

void spell_grosse_ver(void)
{
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

	/* this spell does not work on all kind of skeletons */
	if (g_spelltarget_e->gfx_id == 0x1c) {
		g_spell_special_aecost = -2;
		return;
	} else {

		/* cost is 8 AP */
		g_spell_special_aecost = 8;
	}

	/* Sub -2 from AT */
	g_spelltarget_e->at = g_spelltarget_e->at - 2;

	/* Sub -2 from PA */
	g_spelltarget_e->pa = g_spelltarget_e->pa - 2;
}

void spell_herrdertiere(void)
{

	/* Set pointer to enemy target */
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

	if (!g_spelltarget_e->is_animal) {
		/* spell does not work on animals */

		g_spell_special_aecost = 0;

		sprintf(g_dtp2,	get_tx(11), (Bit8u*)GUI_names_grammar(0, g_spelltarget_e->mon_id, 1));
	} else {

		g_spell_special_aecost = (g_spelltarget_e->first_ap);

		if (get_spelluser()->ae < g_spell_special_aecost) {
			g_spell_special_aecost = -2;
		} else {

			/* set 'tame' flag */
			g_spelltarget_e->flags.tame = 1;

			sprintf(g_dtp2,	get_tx(9), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
		}
	}
}

void spell_horriphobus(void)
{
	/* Set pointer to enemy target */
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

	/* this spell does not work on all kind of skeletons */
	if (g_spelltarget_e->gfx_id == 0x1c) {

		g_spell_special_aecost = -2;

	} else {
		/* set 'scared' flag */
		g_spelltarget_e->flags.scared = 1;
		/* unset 'renegade' flag */
		g_spelltarget_e->flags.renegade = 0;

		sprintf(g_dtp2,	get_tx(12), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
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

	if (get_spelluser()->enemy_id >= 10) {
		/* cast an enemy */

		/* Set pointer to enemy target */
		g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

		/* this spell does not work on all kind of skeletons */
		if (g_spelltarget_e->gfx_id == 0x1c) {
			g_spell_special_aecost = -2;
			return;
		}

		/* set 'asleep' flag */
		g_spelltarget_e->flags.asleep = 1;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(13), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
		return;
	}

	/* cast hero */

	/* Set pointer to hero target */
	g_spelltarget = (struct struct_hero*)get_hero(get_spelluser()->enemy_id - 1);

	if (get_spelltarget() == get_spelluser()) {

		/* don't cast yourself */

		/* set AE costs */
		g_spell_special_aecost = 0;

		/* prepare message */
		strcpy(g_dtp2, get_tx(112));
	} else {
		/* set the flag */
		get_spelltarget()->flags.asleep = 1;

		/* prepare message */
		sprintf(g_dtp2, get_tx(13), get_spelltarget()->alias);
	}
}

void spell_zwingtanz(void)
{
	/* Set pointer to enemy target */
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

	/* this spell does not work on all kind of skeletons */
	if (g_spelltarget_e->gfx_id == 0x1c) {
		g_spell_special_aecost = -2;
	} else {

		/* set 'dancing' flag */
		g_spelltarget_e->flags.dancing = 1;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(14), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));
	}
}

/* Demonologie / Demonology */

void spell_furor_blut(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Furor Blut\" ist nicht implementiert\n");
#endif
	g_spell_special_aecost = -2;
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
	g_spell_special_aecost = -2;
}

void spell_heptagon(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Heptagon\" ist nicht implementiert\n");
#endif
	g_spell_special_aecost = (-2);
}

void spell_kraehenruf(void)
{
	signed short caster_level;
	signed short pa_value;
	signed short i;
	signed short damage;


	caster_level = get_spelluser()->level + 2;

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
	struct struct_fighter *fighter;
	signed short x;
	signed short y;
	signed char unk;

	/* Set pointer to enemy target */
	g_spelltarget_e = &g_enemy_sheets[get_spelluser()->enemy_id - 10];

	/* check if the enemy is dead */
	if (!g_spelltarget_e->flags.dead) {

		/* prepare message */
		sprintf(g_dtp2,	get_tx(15), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));

		/* set ae costs */
		g_spell_special_aecost = 0;
	} else {

		/* prepare message */
		sprintf(g_dtp2,	get_tx(16), (Bit8u*)GUI_names_grammar((signed short)0x8000, g_spelltarget_e->mon_id, 1));

		fighter = FIG_get_fighter(g_spelltarget_e->fighter_id);

		x = fighter->cbx;
		y = fighter->cby;

		if (fighter->twofielded != -1) {
			FIG_remove_from_list(g_fig_twofielded_table[fighter->twofielded], 0);
		}
#ifdef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 1:
		 * If the dead target monster is lying on top of the body of another dead monster,
		 * after the 'Skelettarius' the other body is still displayed, but cannot be selected for 'Skelettarius'.
		 *
		 * Fix: store and restore the FIGHTER_OBJ_ID value. */
		signed char obj_id_bak = fighter->obj_id;
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
		g_fightobj_buf_seek_ptr = fighter->gfxbuf;
#endif

		FIG_remove_from_list(g_spelltarget_e->fighter_id, 0);

		unk = g_spelltarget_e->attacks_left;

		fill_enemy_sheet(get_spelluser()->enemy_id - 10, 0x10, 0);

		FIG_load_enemy_sprites(g_spelltarget_e, x, y);
#ifdef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 2:
		 * set FIGHTOBJ_BUF_FREESPACE and FIGHTOBJ_BUF_SEEK_PTR to the correct values as discussed above */
		g_fightobj_buf_freespace += 0x508L;
		g_fightobj_buf_seek_ptr = buf_seek_ptr_bak;
#endif

		/* zombie will fight for the heroes */
		/* set 'renegade' flag */
		g_spelltarget_e->flags.renegade = 1;
		g_spelltarget_e->attacks_left = unk;

#ifdef M302de_ORIGINAL_BUGFIX
		/* Original-Bug 1:
		 * restore the FIGHTER_OBJ_ID value. */
		fighter = FIG_get_fighter(g_spelltarget_e->fighter_id);
		fighter->obj_id = obj_id_bak;
#endif
	}
}

/* Elemente / Elements */

void spell_elementare(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Elementare\" ist nicht implementiert\n");
#endif
	g_spell_special_aecost = (-2);
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

	hero_pos = get_spelluser()->enemy_id - 1;

	/* Set pointer to hero target */
	g_spelltarget = (struct struct_hero*)get_hero(hero_pos);

	if (!get_spelltarget()->axxeleratus) {

		/* AT + 2 for 1 minute (= 10 rounds of fighting) */
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(1),
			(Bit8u*)&get_spelltarget()->at_weapon[get_spelltarget()->w_type],
			2, (signed char)hero_pos);

		/* PA + 2 for 1 minute (= 10 rounds of fighting) */
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(1),
			(Bit8u*)&get_spelltarget()->pa_weapon[get_spelltarget()->w_type],
			2, (signed char)hero_pos);

		/* set Axxeleratus flag (yields +4 BP, second action phase per round of fighting) for 1 minute (= 10 rounds of fighting) */
		slot = get_free_mod_slot();
		set_mod_slot(slot, MINUTES(1), (Bit8u*)&get_spelltarget()->axxeleratus, 1, (signed char)hero_pos);

		/* prepare message */
		sprintf(g_dtp2,	get_tx(17), get_spelltarget()->alias);

	} else {
		/* axxeleratus already active */
		g_spell_special_aecost = -2;
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
	(g_dng_extra_action != DNG_MENU_MODE_UNLOCK_DOOR)
#else
	(gs_dungeon_index == DUNGEONS_NONE || (g_dng_extra_action != DNG_MENU_MODE_UNLOCK_DOOR && g_dng_extra_action != DNG_MENU_MODE_OPEN_DOOR))
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

	*(g_dng_map_ptr + MAP_POS(x,y)) &= 0x0f; /* clear higher 4 bits */
	*(g_dng_map_ptr + MAP_POS(x,y)) |= (DNG_TILE_OPEN_DOOR << 4);
	g_steptarget_front = *(g_dng_map_ptr + MAP_POS(x,y));

	DNG_open_door();

	add_hero_ap(get_spelluser(), 1); /* hero gets 1 AP for successful lock pick */

	g_new_menu_icons[6] = g_new_menu_icons[7] = g_new_menu_icons[8] = MENU_ICON_NONE;
	g_redraw_menuicons = 1;
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
		g_spell_special_aecost = (select_teleport_dest());

		if (g_spell_special_aecost > 0) {
			/* check AE */
			if (get_spelluser()->ae < g_spell_special_aecost) {
				/* abort */
				g_spell_special_aecost = (-2);
			} else {
				/* play sound */
				play_voc(ARCHIVE_FILE_FX16_VOC);

				/* set new coordinates */
				gs_x_target = g_automap_selx;
				gs_y_target = g_automap_sely;
			}
		}
	}

	g_automap_selx = g_automap_sely = -1;
}

void spell_ueber_eis(void)
{
#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Ueber Eis\" ist nicht implementiert\n");
#endif
	g_spell_special_aecost = (-2);
}


/* Heilung / Healing */

void spell_balsam(void)
{

	signed short le_to_heal;

	/* Set pointer to hero target */
	g_spelltarget = (struct struct_hero*)get_hero(get_spelluser()->enemy_id - 1);

	g_spell_special_aecost = 0;

	if ((g_autofight != 0) ||
		((get_spelluser()->npc_id != 0) && g_in_fight))
	{
		/* automatic */
		le_to_heal = (get_spelltarget()->le_max - get_spelltarget()->le) / 2;
	} else {
		/* prepare message */
		sprintf(g_dtp2,	get_tx(19), get_spelluser()->alias, get_spelltarget()->alias);

		/* ask question */
		le_to_heal = GUI_input(g_dtp2, 2);

		/* terminate string */
		*g_dtp2 = '\0';
	}

	if (le_to_heal != -1) {

		if (le_to_heal < 7) {
			/* AE-cost is at least 7 */
			g_spell_special_aecost = 7;
			/* TODO: potential ORIGINAL-BUG: What if le_to_heal is bigger than the missing LE? */
		} else {
			if (get_spelltarget()->le_max - get_spelltarget()->le < le_to_heal) {
				/* spellcaster wants to heal more LE than wiche are missing */
				g_spell_special_aecost = get_spelltarget()->le_max - get_spelltarget()->le;
				le_to_heal = g_spell_special_aecost;
				/* reduce le and AE_COST to the amount of LE which is actually missing */
				/* This is in accordance with DSA3 rules. The spell costs are at least 7 AE, unless the spellcaster does not have 7 AE any more. */
			} else {
				g_spell_special_aecost = le_to_heal;
			}
		}

		if (get_spelluser()->ae < g_spell_special_aecost) {
			/* not enough AE */
			g_spell_special_aecost = (get_spelluser()->ae);
			le_to_heal = g_spell_special_aecost;
			/* reduce AE-costs and 'le_to_heal' to the available AE */
		}

		add_hero_le(get_spelltarget(), le_to_heal);
	}
}

void spell_hexenspeichel(void)
{
	signed short le;

	/* Set pointer to hero target */
	g_spelltarget = (struct struct_hero*)get_hero(get_spelluser()->enemy_id - 1);

	/* set costs to 0 */
	g_spell_special_aecost = 0;

	if (get_spelltarget() == get_spelluser()) {
		/* spell cannot be used to heal yourself */

		/* prepare message */
		sprintf(g_dtp2,	get_tx(20), get_spelluser()->alias);
		return;
	}

	/* prepare question */
	sprintf(g_dtp2, get_tx(19), get_spelluser()->alias, get_spelltarget()->alias);

	le = GUI_input(g_dtp2, 2);

	*g_dtp2 = '\0';

	if (le != -1) {

		if (get_spelltarget()->le_max - get_spelltarget()->le < le * 2) {

			g_spell_special_aecost = (get_spelltarget()->le_max - get_spelltarget()->le) * 2;

		} else {
			if (get_spelluser()->ae < g_spell_special_aecost) {
				g_spell_special_aecost = get_spelluser()->ae;
			} else {
				g_spell_special_aecost = le * 2;
			}
		}

		add_hero_le(get_spelltarget(), g_spell_special_aecost / 2);
	}
}

void spell_klarum_purum(void)
{
	signed short poison_id;

	/* Set pointer to hero target */
	g_spelltarget = (struct struct_hero*)get_hero(get_spelluser()->enemy_id - 1);

	poison_id = hero_is_poisoned(get_spelltarget());

	if (!poison_id) {

		/* prepare message */
		sprintf(g_dtp2, get_tx(21), get_spelltarget()->alias);
		g_spell_special_aecost = 0;
		return;
	}

	/* AE-cost = poison cost */
	g_spell_special_aecost = g_poison_prices[poison_id];

	if (get_spelluser()->ae < g_spell_special_aecost) {
		/* not enough AE */
		g_spell_special_aecost = -2;
	} else {
		/* reset poison */
		get_spelltarget()->poison[poison_id][1] = 0;
		get_spelltarget()->poison[poison_id][0] = 1;

		/* prepare message */
		sprintf(g_dtp2,	get_tx(22), get_spelltarget()->alias);
	}
}

void spell_ruhe_koerper(void)
{
	/* Set pointer to hero target */
	g_spelltarget = (struct struct_hero*)get_hero(get_spelluser()->enemy_id - 1);

	/* set the flag */
	get_spelltarget()->ruhe_koerper = 1;

	/* prepare message */
	sprintf(g_dtp2, get_tx(51), get_spelltarget()->alias);
}

void spell_tiere_heilen(void)
{

#if !defined(__BORLANDC__)
        D1_INFO("Zauberspruch \"Tiere heilen\" ist nicht implementiert und dient nur zum AE ausgeben.\n");
#endif
	signed short ae;

	/* set AE costs to 0 */
	g_spell_special_aecost = 0;

	/* prepare message */
	sprintf(g_dtp2, get_tx(23), get_spelluser()->alias);

	/* ask how many AE should be spent */
	ae = GUI_input(g_dtp2, 2);

	/* terminate string */
	*g_dtp2 = '\0';

	if (ae != -1) {
		/* set AE costs to AE */
		g_spell_special_aecost = ae;
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
	set_mod_slot(slot, MINUTES(6), (Bit8u*)&get_spelluser()->skills[TA_SINNESSCHAERFE], 7, (signed char)hero_pos);

	/* prepare message */
	sprintf(g_dtp2,	get_tx(24), get_spelluser()->alias);
}

char* spell_analues(void)
{
	signed short item_id;
	signed short i;
	signed short item_pos;

	/* set analisation capabilities */
	if (gs_in_academy == 99) {
		g_spelltest_result = 99;
	}

	item_pos = select_item_to_drop(get_spelluser());

#ifdef M302de_ORIGINAL_BUGFIX
	/* If the player cancels item selection or has no items select_item_to_drop() returns -1.
	The original uses the return value to calculate an index, whithout checking for this. */
	if (item_pos == -1) item_id = 0;
	else item_id = get_spelluser()->inventory[item_pos].item_id;
#else
	item_id = get_spelluser()->inventory[item_pos].item_id;
#endif

	strcpy(g_text_output_buf, get_tx(52));

	if (item_id) {

		for (i = 0; g_analues_items[i].item_id != -1; i++) {

			if (g_analues_items[i].item_id == item_id) {

				/* check if the spellcaster is able to analyze this item */

				/* The following is pretty hacky. The Analues spell was already performed, before calling this function,
				 * without knowing the exact handicap. The handicap is considered only now, via comparing to the
				 * g_spelltest_result global variable.
				 * This a posteriori adjustment is only possible since not the original DSA 2/3 spell test logic is used,
				 * but a lighter "pool version".
				 * Also, it results in the Original-Bug that, in the case that the +0 spell test was successful, but with handicap it is not,
				 * the full AE cost has to be payed. (Normally, you get a 1/2 reduction for a failed attempt.) */
				if (g_analues_items[i].handicap <= g_spelltest_result) {

					/* copy the matching result string */
					strcpy(g_text_output_buf, get_tx(g_analues_items[i].text_id));

					get_spelluser()->inventory[item_pos].flags.magic_revealed = 1;
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
		sprintf(g_dtp2, get_tx(53), get_spelluser()->alias, g_text_output_buf);
	}

	return g_text_output_buf;
}

#if !defined(__BORLANDC__)
}
#endif
