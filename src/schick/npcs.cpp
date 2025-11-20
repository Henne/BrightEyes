/*
 *	Rewrite of DSA1 v3.02_de functions of seg095 (NPCs)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg095.cpp
 */

#include <string.h>

#include "v302de.h"
#include "common.h"

#include "misc.h"
#include "save_text.h"
#include "resource_loader.h"
#include "playmask.h"
#include "level_up.h"
#include "npcs.h"
#include "gui.h"
#include "game_init.h"

signed int npc_meetings(const signed int type_index)
{
	/* check if an NPC is in the party and if we already had an NPC conversation here */
	if (!get_hero(6)->typus && (type_index != gs_npc_meet_tavern)) {

		gs_npc_meet_tavern = type_index;

		if ((gs_town_id == TOWN_ID_CLANEGH) && (gs_town_typeindex == 48) && !((uint8_t)gs_npc_timers[NPC_NARIELL])) {

			npc_nariell();

		} else if ((gs_town_id == TOWN_ID_PHEXCAER) && (gs_town_typeindex == 40) && !((uint8_t)gs_npc_timers[NPC_HARIKA])) {

			npc_harika();

		} else if ((gs_town_id == TOWN_ID_OVERTHORN) && (gs_town_typeindex == 60) && !((uint8_t)gs_npc_timers[NPC_CURIAN])) {

			npc_curian();

		} else if ((gs_town_id == TOWN_ID_THORWAL) && (gs_town_typeindex == 6) && !((uint8_t)gs_npc_timers[NPC_ARDORA]) && gs_dng01_ardora_freed) {

			npc_ardora();

		} else if ((gs_town_id == TOWN_ID_PREM) && (gs_town_typeindex == 74) && !((uint8_t)gs_npc_timers[NPC_GARSVIK])) {

			npc_garsvik();

		} else if ((gs_town_id == TOWN_ID_MANRIN) && (gs_town_typeindex == 87) && !((uint8_t)gs_npc_timers[NPC_ERWO])) {

			npc_erwo();

		} else {
			return 0;
		}

		return 1;
	} else {
		return 0;
	}
}

void npc_farewell(void)
{
	struct struct_hero *hero_i;
	signed int i;
	signed int tmp;

	/* no NPC there */
	if (get_hero(6)->typus == HERO_TYPE_NONE)
		return;

	/* no NPC in that group */
	if (get_hero(6)->group_id != gs_active_group_id)
		return;

	/* Unconscious or dead NPCs cannot be removed automatically (99 means manual). */
	if (check_hero(get_hero(6)) == 0 && (gs_npc_months < 99))
		return;

	tmp = g_tx_file_index;
	load_tx(ARCHIVE_FILE_NSC_LTX);

	switch (get_hero(6)->npc_id) {

		case NPC_NARIELL: {
			if (gs_npc_months >= 2)
				remove_npc(20, 31, ARCHIVE_FILE_NPCS + 0, get_ttx(753), get_tx(9));
			break;
		}
		case NPC_HARIKA: {
			if (gs_npc_months >= 2) {
				if (gs_npc_months >= 99 ||
					gs_town_id == TOWN_ID_THORWAL ||
					gs_town_id == TOWN_ID_PHEXCAER ||
					gs_town_id == TOWN_ID_PREM ||
					gs_town_id == TOWN_ID_OBERORKEN) {

					remove_npc(22, 31, ARCHIVE_FILE_NPCS + 1, get_ttx(754), get_tx(19));

					hero_i = get_hero(0);
					for (i = 0; i < 6; i++, hero_i++) {
						if ((hero_i->typus) && (hero_i->group_id == gs_active_group_id) && (!hero_i->flags.dead))
						{
							/* Original-Bug 42:
							 * When NPC Harika leaves the party, all non-dead heroes in the same group get up to
							 * three attempts for a talent increase in Schleichen. The idea is probably that the
							 * group members had a chance to learn something from the experienced stray Harika.
							 * However, these "free" attempts are missing at the next level-up.
							 * This does not make sense. */

#ifdef M302de_ORIGINAL_BUGFIX
							char ta_rise_bak = hero_i->saved_talent_increases;
#endif
							/* All non-dead heroes in the same group get a chance to increase TA_SCHLEICHEN */
							inc_talent_novice(hero_i, TA_SCHLEICHEN);
#ifdef M302de_ORIGINAL_BUGFIX
							/* The unwanted reduction is done within the function inc_talent_novice(). We revert it. */
							hero_i->saved_talent_increases = ta_rise_bak;
#endif
						}
					}
				}
			}
			break;
		}
		case NPC_CURIAN: {
			if (gs_npc_months >= 6)
				remove_npc(25, 64, ARCHIVE_FILE_NPCS + 2, get_ttx(755), get_tx(29));
			break;
		}
		case NPC_ARDORA: {
			if (gs_npc_months >= 1)
				remove_npc(21, 31, ARCHIVE_FILE_NPCS + 3, get_ttx(756), get_tx(43));
			break;
		}
		case NPC_GARSVIK: {
			if (gs_npc_months >= 2)
				remove_npc(23, 31, ARCHIVE_FILE_NPCS + 4, get_ttx(757), get_tx(53));
			break;
		}
		case NPC_ERWO: {
			if (gs_npc_months >= 2)
				remove_npc(24, 31, ARCHIVE_FILE_NPCS + 5, get_ttx(758), get_tx(63));
			break;
		}
	}

	if ((tmp != -1) && (tmp != (ARCHIVE_FILE_NPCS - 1)))
		load_tx(tmp);
}

//static
void npc_nariell(void)
{
	signed int answer;

	/* load NSC.LTX */
	load_tx(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(20);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(753), get_tx(0),
				3, get_tx(1), get_tx(2), get_tx(3));

	} while (answer == -1);

	/* process the answer */
	if (answer == 1) {

		GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(753), get_tx(4), 0);

	} else {
		do {
			answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(753),
					(answer == 2) ? get_tx(5): get_tx(6),
					2, get_tx(7), get_tx(8));

		} while (answer == -1);

		/* add nariell */
		if (answer == 2)
			add_npc(ARCHIVE_FILE_NPCS + 0);
	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

//static
void npc_harika(void)
{
	signed long money;
	signed int answer;

	/* load NSC.LTX */
	load_tx(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(22);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(754), get_tx(10),
				3, get_tx(11), get_tx(12), get_tx(13));

	} while (answer == -1);

	if (answer == 1) {

		GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(754), get_tx(14), 0);

	} else if (answer == 2) {

		money = get_party_money();

		answer = (money >= 2000) ? 2 : 1;

		do {
			answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(754), get_tx(15),
					answer,	get_tx(17), get_tx(18));

		} while (answer == -1);

		/* hire her for 20D */
		if (answer == 2) {
			/* subtract 20D */
			money -= 2000;
			set_party_money(money);

			/* add her to the party */
			add_npc(ARCHIVE_FILE_NPCS + 1);
		}
	} else {
		money = get_party_money();

		answer = (money >= 1500) ? 2 : 1;

		do {
			answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(754), get_tx(16),
					answer,	get_tx(17), get_tx(18));

		} while (answer == -1);

		/* hire her for 15D */
		if (answer == 2) {
			/* subtract 15D */
			money -= 1500;
			set_party_money(money);

			/* add her to the party */
			add_npc(ARCHIVE_FILE_NPCS + 1);
		}
	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

void npc_curian(void)
{
	signed int answer;

	/* load NSC.LTX */
	load_tx(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(25);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(755), get_tx(20),
				3, get_tx(21), get_tx(22), get_tx(23));

	} while (answer == -1);

	if (answer == 1) {

		GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(755), get_tx(24), 0);

	} else {

		do {
			answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(755),
					(answer == 2) ? get_tx(25): get_tx(26),
					2, get_tx(27), get_tx(28));
		} while (answer == -1);

		/* add NPC */
		if (answer == 2)
			add_npc(ARCHIVE_FILE_NPCS + 2);
	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

//static
void npc_ardora(void)
{
	signed int answer;

	/* load NSC.LTX */
	load_tx(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(21);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(756), get_tx(30), 3,
					get_tx(31), get_tx(32),	get_tx(33));

	} while (answer == -1);

	if (answer == 1) {
		do {
			answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(756), get_tx(34), 2,
					get_tx(35), get_tx(36));

		} while (answer == -1);

		if (answer == 2) {

			GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(756), get_tx(37), 0);

		} else {
			do {
				answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(756), get_tx(39), 2,
					get_tx(40), get_tx(41));

			} while (answer == -1);

			if (answer == 1) {

				add_npc(ARCHIVE_FILE_NPCS + 3);

			} else {

				GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(756), get_tx(42), 0);
			}
		}
	} else {
		if (answer == 2) {

			GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(756), get_tx(38), 0);
		}

		do {
			answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(756), get_tx(39), 2,
					get_tx(40), get_tx(41));

		} while (answer == -1);

		if (answer == 1) {

			add_npc(ARCHIVE_FILE_NPCS + 3);

		} else {
			GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(756), get_tx(42), 0);
		}
	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

//static
void npc_garsvik(void)
{
	signed int answer;

	/* load NSC.LTX */
	load_tx(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(23);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(757), get_tx(44), 3,
				get_tx(45), get_tx(46),	get_tx(47));

	} while (answer == -1);

	if (answer == 1) {

		GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(757), get_tx(48), 0);

	} else {
		do {
			answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(757),
					(answer == 2) ? get_tx(49) : get_tx(50),
					2, get_tx(51), get_tx(52));

		} while (answer == -1);

		/* add NPC */
		if (answer == 2)
			add_npc(ARCHIVE_FILE_NPCS + 4);
	}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

//static
void npc_erwo(void)
{
	signed int answer;

	/* load NSC.LTX */
	load_tx(ARCHIVE_FILE_NSC_LTX);

	/* load head */
	load_in_head(24);

	/* show dialog window */
	do {
		answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(758), get_tx(54),
				3, get_tx(55), get_tx(56), get_tx(57));

	} while (answer == -1);

	if (answer == 1) {

		GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(758), get_tx(58), 0);

	} else {
		do {
			answer = GUI_dialogbox((uint8_t*)g_dtp2, get_ttx(758),
					(answer == 2) ? get_tx(59): get_tx(60),
					2, get_tx(61), get_tx(62));

		} while (answer == -1);

		/* add NPC */
		if (answer == 2)
			add_npc(ARCHIVE_FILE_NPCS + 5);

		}

	load_tlk(ARCHIVE_FILE_TAVERN_TLK);
}

void remove_npc(const signed int head_index, const signed char days,
		const signed int index, char *name, char *text)
{

	if (g_fading_state != 0)
		refresh_colors();

	/* reset NPCs groups position */
	/* TODO: this is bogus, since memset() will come */
	get_hero(6)->slot_pos = 0;

	/* save the NPC */
	save_npc(index);

	/* print farewell message if the NPC has and can */
        if (text && check_hero(get_hero(6))) {

		load_in_head(head_index);
		GUI_dialogbox((uint8_t*)g_dtp2, name, text, 0);
	}

	/* clear the NPC from memory */
	memset((void*)get_hero(6), 0, sizeof(struct struct_hero));

	/* dec group counter */
	gs_group_member_counts[gs_active_group_id]--;

	/* dec global hero counter */
	gs_total_hero_counter--;

	g_refresh_status_line = 1;

	/* TODO:	check_hero() will now, after memset() return 0,
			so the parameter days is useless */
	if (check_hero(get_hero(6)))
		gs_npc_timers[index - ARCHIVE_FILE_NPCS + 1] = days;
	else
		gs_npc_timers[index - ARCHIVE_FILE_NPCS + 1] = -1;

}

void add_npc(const signed int index)
{
	load_npc(index);

	/* overwrite the picture of the NPC with one from IN_HEAD.NVF */
	memcpy(get_hero(6)->pic, g_dtp2, 0x400);

	/* increment heroes in that group */
	gs_group_member_counts[gs_active_group_id]++;

	/* increment heroes */
	gs_total_hero_counter++;

	/* reset the months the NPC is in the group */
	gs_npc_months = 0;

	/* set a number to decide between the NPCs (1-6) */
	get_hero(6)->npc_id = index - ARCHIVE_FILE_NPCS + 1;

	/* set the group the NPC contains in */
	get_hero(6)->group_id = gs_active_group_id;

	draw_status_line();
}
