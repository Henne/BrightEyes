/**
 *	Rewrite of DSA1 v3.02_de functions of seg090 (dungeon: oberorken mine)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg090.cpp
 *
 */
#include <stdio.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg025.h"
#include "seg075.h"
#include "seg076.h"
#include "seg090.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg103.h"
#include "seg113.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short DNG12_handler(void)
{
	signed short target_pos;
	signed short i;
	signed short tw_bak;
	Bit32s money;
	Bit8u *hero;
	Bit8u *ptr;

	ptr = p_datseg + DNG_MAP;
	tw_bak = g_textbox_width;
	g_textbox_width = 7;
	target_pos = DNG_POS(gs_dungeon_level,gs_x_target,gs_y_target);

	hero = (Bit8u*)get_first_hero_available_in_group();

	if (gs_day_timer % MINUTES(5) == 0) {
		/* TODO: buggy timer for cave in */

		if (ds_readbs(DNG12_TUNNEL1) > 0) dec_ds_bs(DNG12_TUNNEL1);
		if (ds_readbs(DNG12_TUNNEL2) > 0) dec_ds_bs(DNG12_TUNNEL2);
		if (ds_readbs(DNG12_TUNNEL3) > 0) dec_ds_bs(DNG12_TUNNEL3);
		if (ds_readbs(DNG12_TUNNEL4) > 0) dec_ds_bs(DNG12_TUNNEL4);
	}

	if (target_pos == DNG_POS(1,6,8) && target_pos != gs_dng_handled_pos && gs_direction == EAST && ds_readbs(DNG12_WATERTRAP_WATER_RUNS) != 0) {
		/* secret door from water trap */
#if !defined(__BORLANDC__)
		D1_INFO("Geheimtuere\n");
#endif
		if (test_skill(hero, TA_SINNESSCHAERFE, 6) > 0) {

			GUI_output(get_tx(21));

			and_ptr_bs(ptr + MAP_POS(7,8), (DNG_TILE_CORRIDOR << 4) + 0x0f);

			/* turn off water trap */
			ds_writeb(DNG12_WATERTRAP_ACTIVE, 0);
			ds_writeb(DNG12_WATERTRAP_WATER_RUNS, 0);

		} else {
			/* door not detected */
			if (GUI_bool(get_tx(16))) GUI_output(get_tx(17));
		}
	} else {

		if ((	target_pos == DNG_POS(1,1,7) || target_pos == DNG_POS(1,2,7) || target_pos == DNG_POS(1,3,7) ||
			target_pos == DNG_POS(1,4,7) || target_pos == DNG_POS(1,5,7) || target_pos == DNG_POS(1,6,7) ||
			target_pos == DNG_POS(1,1,8) || target_pos == DNG_POS(1,2,8) || target_pos == DNG_POS(1,3,8) ||
			target_pos == DNG_POS(1,4,8) || target_pos == DNG_POS(1,5,8) || target_pos == DNG_POS(1,6,8) ||
			target_pos == DNG_POS(1,1,9) || target_pos == DNG_POS(1,2,9) || target_pos == DNG_POS(1,3,9) ||
			target_pos == DNG_POS(1,4,9) || target_pos == DNG_POS(1,5,9) || target_pos == DNG_POS(1,6,9) )
			&& ds_readbs(DNG12_WATERTRAP_ACTIVE) != 0)
		{
			/* water trap room, activate */
			ds_writeb(DNG12_WATERTRAP_WATER_RUNS, 1);

			if (gs_dng12_watertrap_timer / MINUTES(5) != ds_readws(DNG12_WATERTRAP_BAK))
			{

				ds_writews(DNG12_WATERTRAP_BAK, (signed short)(gs_dng12_watertrap_timer / MINUTES(5)));

				/* warning according to water level */
				GUI_output(gs_dng12_watertrap_timer == MINUTES(0) ? get_tx(20) :
						(gs_dng12_watertrap_timer <= MINUTES(10) ? get_tx(19) : get_tx(18)));

				if (gs_dng12_watertrap_timer == MINUTES(0)) {
					/* time is up, drown party */
					hero = get_hero(0);
					for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

						if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
							host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
							!hero_dead(hero))
						{
							hero_disappear(hero, i, -1);
						}
					}
				} else if (gs_dng12_watertrap_timer <= MINUTES(40)) {
					/* NPC will find secret door */

					if (is_hero_available_in_group(get_hero(6))) {

						/* prepare a message with the name of the NPC */
						sprintf(g_dtp2, get_tx(22), (char*)get_hero(6) + HERO_NAME2);
						GUI_output(g_dtp2);

						while (gs_x_target != 6) {

							gs_x_target += (gs_x_target > 6 ? -1 : 1);

							DNG_update_pos();
						}


						while (gs_y_target != 8) {

							gs_y_target += (gs_y_target > 8 ? -1 : 1);

							DNG_update_pos();
						}

						and_ptr_bs(ptr + MAP_POS(7,8), (DNG_TILE_CORRIDOR << 4) + 0x0f);

						/* turn off water trap */
						ds_writeb(DNG12_WATERTRAP_ACTIVE, 0);
						ds_writeb(DNG12_WATERTRAP_WATER_RUNS, 0);
						DNG_update_pos();
					}
				}
			}
		} else {
			/* clear water */
			ds_writeb(DNG12_WATERTRAP_WATER_RUNS, 0);
			/* reset countdown */
			gs_dng12_watertrap_timer = MINUTES(50) + 5;
		}
	}

	if (target_pos == DNG_POS(0,13,9) && target_pos != gs_dng_handled_pos && !ds_readbs(DNG12_TUNNEL1)) {
		/* 1. tunnel block */
		DNG_clear_corridor(p_datseg + DNG12_TUNNEL1);
	} else if (target_pos == DNG_POS(0,14,8) && target_pos != gs_dng_handled_pos && !ds_readbs(DNG12_TUNNEL2)) {
		/* 2. tunnel block */
		DNG_clear_corridor(p_datseg + DNG12_TUNNEL2);
	} else if (target_pos == DNG_POS(0,13,7) && target_pos != gs_dng_handled_pos && !ds_readbs(DNG12_TUNNEL3)) {
		/* 3. tunnel block */
		DNG_clear_corridor(p_datseg + DNG12_TUNNEL3);
	} else if (target_pos == DNG_POS(0,14,5) && target_pos != gs_dng_handled_pos && !ds_readbs(DNG12_TUNNEL4)) {
		/* 4. tunnel block */
		DNG_clear_corridor(p_datseg + DNG12_TUNNEL4);
	} else if (target_pos == DNG_POS(0,8,4) && target_pos != gs_dng_handled_pos) {

		/* upper Ingerimm idol */
		GUI_output(get_tx(3));

		if (GUI_bool(get_tx(4))) {

			do {
				i = GUI_radio(get_tx(5), 3,
						get_tx(6), get_tx(7), get_tx(8));
			} while (i == -1);

			if (i == 1) {
				/* lift arm */
#if !defined(__BORLANDC__)
				D1_INFO("nichts passiert\n");
#endif
				GUI_output(get_tx(9));
			} else if (i == 2) {
				/* press arm */
#if !defined(__BORLANDC__)
				D1_INFO("Einsturz des Tunnels verhindern (buggy)\n");
#endif

				GUI_output(get_tx(9));

				if (ds_readbs(DNG12_TUNNEL1) > 0) ds_writeb(DNG12_TUNNEL1, -1);
				if (ds_readbs(DNG12_TUNNEL2) > 0) ds_writeb(DNG12_TUNNEL2, -1);
				if (ds_readbs(DNG12_TUNNEL3) > 0) ds_writeb(DNG12_TUNNEL3, -1);
				if (ds_readbs(DNG12_TUNNEL4) > 0) ds_writeb(DNG12_TUNNEL4, -1);
			} else if (i == 3) {
				/* sacrifice gold */

				GUI_output(get_tx(10));

				money = get_party_money();

				if (money >= 100) {
					money -= 100;
					set_party_money(money);
				}
#if !defined(__BORLANDC__)
				D1_INFO("nichts passiert\n");
#endif
			}
		}
	} else if (target_pos == DNG_POS(0,5,3) && target_pos != gs_dng_handled_pos) {
		/* trap door */
		GUI_output(get_tx(11));
		DNG_fallpit(6);
	} else if (target_pos == DNG_POS(0,4,6) && target_pos != gs_dng_handled_pos) {
		/* trap door */
		GUI_output(get_tx(11));
		gs_x_target = (1);
		gs_y_target = (3);
		DNG_inc_level();
	} else if (target_pos == DNG_POS(0,6,13) && target_pos != gs_dng_handled_pos) {
		/* bolt trap */
		print_msg_with_first_hero(get_tx(13));
		sub_hero_le(hero, random_schick(6));
	} else if (target_pos == DNG_POS(1,2,14) && target_pos != gs_dng_handled_pos && !ds_readb(DNG12_INGERIMM_HINT))
	{
		/* lower Ingerimm idol */
#if !defined(__BORLANDC__)
		D1_INFO("Untere Ingerimstatue\n");
#endif
		GUI_output(get_tx(14));

	} else if (target_pos == DNG_POS(1,2,14)) {
#if !defined(__BORLANDC__)
		D1_INFO("Test auf Ingerimm-Opfer\n");
#endif
		if (ds_readb(DNG12_INGERIMM_SACRIFICE) != 0 && !ds_readb(DNG12_INGERIMM_HINT))
		{
			/* hint to secret door */
			ds_writeb(DNG12_INGERIMM_HINT, 1);
			ds_writeb(DNG12_INGERIMM_SACRIFICE, 0);
			GUI_output(get_tx(15));
		}
	} else if (target_pos == DNG_POS(1,3,10) && target_pos != gs_dng_handled_pos) {
#if !defined(__BORLANDC__)
		D1_INFO("In Wasserfalle gefangen\n");
#endif
		if (div16(cast_u16(host_readb(ptr + MAP_POS(3,11)))) == DNG_TILE_OPEN_DOOR) {
			and_ptr_bs(ptr + MAP_POS(3,11), 0xf);
			or_ptr_bs(ptr + MAP_POS(3,11), DNG_TILE_CLOSED_DOOR << 4);
		}
	} else if (target_pos == DNG_POS(1,3,7) && target_pos != gs_dng_handled_pos) {
#if !defined(__BORLANDC__)
		D1_INFO("In Wasserfalle gefangen\n");
#endif
		if (div16(cast_u16(host_readb(ptr + MAP_POS(3,6)))) == 2) {
			and_ptr_bs(ptr + MAP_POS(3,6), 0xf);
			or_ptr_bs(ptr + MAP_POS(3,6), DNG_TILE_CLOSED_DOOR << 4);
		}
	} else if (target_pos == DNG_POS(1,1,8) && target_pos != gs_dng_handled_pos && gs_direction == WEST) {
		/* water source */

		if (GUI_bool(get_tx(16))) {
			GUI_output(get_tx(17));
		}
	} else if (target_pos == DNG_POS(1,3,2) && target_pos != gs_dng_handled_pos && gs_direction == SOUTH) {
#if !defined(__BORLANDC__)
		D1_INFO("Rueckwaerts gehen\n");
#endif
		GUI_output(get_tx(23));
		gs_x_target = (1);
		gs_y_target = (3);
		gs_direction = (WEST);
		DNG_update_pos();
	} else if (target_pos == DNG_POS(1,2,3) && target_pos != gs_dng_handled_pos && gs_direction == EAST) {
#if !defined(__BORLANDC__)
		D1_INFO("Rueckwaerts gehen\n");
#endif
		GUI_output(get_tx(23));
		gs_x_target = (5);
		gs_y_target = (3);
		gs_direction = (EAST);
		DNG_update_pos();
	} else if (target_pos == DNG_POS(1,4,3) && target_pos != gs_dng_handled_pos && gs_direction == WEST) {
#if !defined(__BORLANDC__)
		D1_INFO("Rueckwaerts gehen\n");
#endif
		GUI_output(get_tx(23));
		gs_x_target = (3);
		gs_y_target = (1);
		gs_direction = (NORTH);
		DNG_update_pos();
	} else if (target_pos == DNG_POS(1,11,6) && target_pos != gs_dng_handled_pos) {
#if !defined(__BORLANDC__)
		D1_INFO("Illusionswand und Grube\n");
#endif

		if (GUI_bool(get_tx(24)))
		{
			if (ds_readb(DNG12_OBSTACLE_ACTIVE) != 0)
			{

				/* TODO: Original-Bug: this counter is not in the savegame */
				if (inc_ds_ws(DNG12_OBSTACLE_TRIES) < 3)
				{
					/* the hero must at least fall three times into pit */
					sprintf(g_dtp2,
						get_tx(25),
						(char*)hero + HERO_NAME2,
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));
				} else {
					/* the hero falls again into the pit */
#ifndef M302de_FEATURE_MOD
					sprintf(g_dtp2,
						get_tx(31),
						(char*)hero + HERO_NAME2,
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)),
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)));
#else
					/* Feature Mod 7: The following is a translation of a text block in OBER.DTX
					 * of the English version, which has not been present in the German one.
					 * It's a copy of get_dpt(0x64) plus an additional line.
					 */
					/* TODO: This mod works only for the German version and has to be
							reworked, when BLADE.DAT should also be supported.
					*/
					if (strlen(get_tx(25)) == 219)
					/* TODO: test if this block can really be replaced by the two lines below
					const unsigned char add_line[110] = {	0x40, 0x3c,'I','C','H',' ',
							'G','L','A','U','B','E',',',' ',
							'I','C','H',' ',
							'M','U','S','S',' ',
							'E','S',' ' ,'N','U','R',' ',
							'N','O','C','H',' ','E','I','N',' ',
							'E','I','N','Z','I','G','E','S',' ',
							'M','A','L',' ','V','E','R','S','U','C','H','E','N','!', 0x3e,' ',
							'M','U','R','M','E','L','T',' ',
							'%', 's',' ',
							'A','L','S',' ',
							'%', 's',' ',
							'W','I','E','D','E','R',' ',
							'A','U','F',' ',
							'%','s','E',' ',
							'F', 0x9a, 'S','S','E',' ',
							'K','O','M','M','T','.',
							'\0'
							};

					strcpy(g_text_output_buf, get_tx(25));
					strcat(g_text_output_buf, (const char*)add_line);
					*/

					strcpy(g_text_output_buf, get_tx(25));
					strcat(g_text_output_buf,
						"\x40\x3cICH GLAUBE, ICH MUSS ES NUR NOCH "
						"EIN EINZIGES MAL VERSUCHEN!\x3e MURMELT %s "
						"ALS %s WIEDER AUF %sE F\x9aSSE KOMMT.");

					sprintf(g_dtp2,
						g_text_output_buf,
						(char*)hero + HERO_NAME2,
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)),
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
						(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));
#endif
				}

				/* try to break through */
				sub_ds_bs(DNG12_OBSTACLE_HP, dice_roll(4, 6, 2));

				if (ds_readbs(DNG12_OBSTACLE_HP) < 0)
				{
					/* obstacle broken */
					ds_writebs(DNG12_OBSTACLE_ACTIVE, 0);

#ifndef M302de_FEATURE_MOD
					sprintf(g_dtp2,
						get_tx(32),
						(char*)hero + HERO_NAME2);
#else
					/* Feature Mod 7: The following is a translation of a text block in OBER.DTX
					 * of the English version, which has not been present in the German one.
					 */
					/* TODO: This mod works only for the German version and has to be
							reworked, when BLADE.DAT should also be supported.
					*/
					/* TODO: Test if this block cann really be replaced by the single line below.
					unsigned char str[] = {	'A','L','S',' ','%','s',' ',
								'M','I','T',' ','D','E','R',' ',
								'B','A','R','R','I','E','R','E',' ',
								'K','O','L','L','I','D','I','E','R','T',' ',
								'B','R','I','C','H','T',' ',
								'S','I','E',' ','I','N',' ',
								'S','T', 0x9a,'C','K','E','.',0x40,

								'D','A','S',' ',
								'G','A','N','Z','E',' ',
								'W','A','R',' ','N','U','R',' ',
								'E','I','N',' ','B','I','L','D','!',0x40,

								'S','C','H','A','D','E',',',' ',
								'D','A','S','S',' ','I','H','R',' ',
								'D','A','S',' ','N','I','C','H','T',' ',
								'F','R', 0x9a,'H','E','R',' ',
								'B','E','M','E','R','K','T',' ',
								'H','A','B','T','.',
								'\0'};

					sprintf(g_dtp2,
						(char*)str,
						(char*)hero + HERO_NAME2);
					*/

					sprintf(g_dtp2,
						"ALS %s MIT DER BARRIERE KOLLIDIERT, BRICHT SIE IN ST\x9aCKE.\x40"
						"DAS GANZE WAR NUR EIN BILD!\x40"
						"SCHADE DASS IHR DAS NICHT FR\x9aHER BEMERKT HABT.\0",
						(char*)hero + HERO_NAME2);
#endif
				}

				GUI_output(g_dtp2);

				sub_hero_le(hero, random_schick(4));
				gs_x_target = (gs_x_target_bak);
				gs_y_target = (gs_y_target_bak);
			} else {
				/* pass */
				GUI_output(get_tx(26));
			}
		} else {
			gs_x_target = (gs_x_target_bak);
			gs_y_target = (gs_y_target_bak);
		}
	} else if (target_pos == DNG_POS(1,14,3) && target_pos != gs_dng_handled_pos && ds_readb(DNG12_SPEARTRAP_ACTIVE) != 0) {
		/* spear trap */

		if (test_skill(hero, TA_SINNESSCHAERFE, 2) > 0) {

			if (GUI_bool(get_tx(28))) {
				if (test_skill(hero, TA_SCHLOESSER, 0) <= 0) {
					/* defusing trap failed */
					sprintf(g_dtp2,
						get_tx(29),
						(char*)hero + HERO_NAME2);
					sub_hero_le(hero , dice_roll(3, 6, 0));
				} else {
					/* trap defused */
					sprintf(g_dtp2,
						get_tx(30),
						(char*)hero + HERO_NAME2,
						(char*)hero + HERO_NAME2);

					ds_writeb(DNG12_SPEARTRAP_ACTIVE, 0);

					add_hero_ap(hero, 10);
				}
				GUI_output(g_dtp2);
			}
		} else {
			/* sprung trap */
			GUI_output(get_tx(27));
			sub_group_le(dice_roll(3, 6, 0));
		}
	} else if (target_pos == DNG_POS(0,13,15) && target_pos != gs_dng_handled_pos) {
		/* exit mine */
		leave_dungeon();
		gs_x_target = (1);
		gs_y_target = (6);
		gs_direction = (NORTH);
	}

	g_textbox_width = tw_bak;
	gs_dng_handled_pos = target_pos;

	return 0;
}

void DNG_clear_corridor(Bit8u *ptr)
{
	/* ask if the corridor should be cleared */
	if (GUI_bool(get_tx(1))) {

		/* clear the corridor */

		GUI_output(get_tx(2));

		timewarp(DAYS(1));

		host_writebs(ptr, -1);
	} else {
		gs_x_target = (gs_x_target_bak);
		gs_y_target = (gs_y_target_bak);
	}
}

void DNG_oberorken_chest(Bit8u* chest)
{
	Bit8u* ptr_bak;

	ptr_bak = (Bit8u*)host_readd((Bit8u*)(chest) + 0x0b);

	host_writed((Bit8u*)(chest) + 0x0b, (Bit32u)(p_datseg + DNG12_CHEST1_CONTENT));

	loot_simple_chest((Bit8u*)(chest));

	host_writed((Bit8u*)(chest) + 0x0b, (Bit32u)ptr_bak);
}

/**
 * \brief   print introductions of dungeon #12 fights
 *
 * \param   fight_id    ID of the fight
 */
void DNG12_fight_intro(signed short fight_id)
{
	if (fight_id == 141) {
		GUI_output(get_tx(12));
	}
}

#if !defined(__BORLANDC__)
}
#endif
