/*
 *	Rewrite of DSA1 v3.02_de functions of seg046 (Status)
 *	Functions rewritten: 4/4 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg046.cpp
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg027.h"
#include "seg046.h"
#include "seg047.h"
#include "seg096.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   prints spellname and value
 *
 * \param   hero        the hero the spell is from
 * \param   spell       spellnumber
 * \param   fsig        the first spell in the spellgroup
 * \param   x1          the leftmost x coordinate
 * \param   x2          the rightmost x coordinate
 * \param   gy          the upper y coordinate of this spellgroup
 */
/* Borlandified and identical */
void status_show_spell(Bit8u *hero, unsigned short spell, unsigned short fsig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	unsigned short group;
	char str[10];

	group = spell - fsig;

	/* print spellname */
	GUI_print_string(get_ttx(spell + 0x6a), x1, gy + group * 7);

	/* convert value to string */
	my_itoa(host_readbs(hero + HERO_SPELLS + spell), str, 10);

	/* print value */
	GUI_print_string(str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), gy + group * 7);
}

/**
 * \brief   prints skillname and value
 *
 * \param   hero        the hero the skill is from
 * \param   talen       skillnumber
 * \param   ftig        the first skill in the skillgroup
 * \param   x1          the leftmost x coordinate
 * \param   x2          the rightmost x coordinate
 * \param   gy          the upper y coordinate of this skillgroup
 */
/* Borlandified and identical */
void status_show_skill(Bit8u *hero, unsigned short skill, unsigned short ftig,
			unsigned short x1, unsigned short x2, unsigned short gy) {
	unsigned short group;
	char str[10];

	group = skill - ftig;

	/* print skillname */
	GUI_print_string(get_ttx(skill + 0x30), x1, gy + group * 7);

	/* convert value to string */
	my_itoa(host_readbs(hero + HERO_TALENTS + skill) , str, 10);

	/* print value */
	GUI_print_string(str, x2 - GUI_get_space_for_string((Bit8u*)str, 0), gy + group * 7);
}

/**
 * \brief   shows all skills and their values
 *
 * \param   hero        the hero whose skills should be shown
 */
/* Borlandified and identical */
void status_show_skills(Bit8u *hero) {

	signed short skill_category, skill_no;

	set_textcolor(0xff, 2);

	/* print skill category names */
	GUI_print_string(get_ttx(100), GUI_get_first_pos_centered(get_ttx(100), 5, 100, 0), 55);

	GUI_print_string(get_ttx(104), GUI_get_first_pos_centered(get_ttx(104), 110, 100, 0), 55);

	GUI_print_string(get_ttx(101), GUI_get_first_pos_centered(get_ttx(101), 215, 100, 0), 55);

	GUI_print_string(get_ttx(105), GUI_get_first_pos_centered(get_ttx(105), 5, 100, 0), 125);

	GUI_print_string(get_ttx(103), GUI_get_first_pos_centered(get_ttx(103), 110, 100, 0), 125);

	GUI_print_string(get_ttx(102), GUI_get_first_pos_centered(get_ttx(102), 215, 100, 0), 132);

	GUI_print_string(get_ttx(106), GUI_get_first_pos_centered(get_ttx(106), 5, 100, 0), 174);

	set_textcolor(0, 2);

	for (skill_category = 0; skill_category < 7; skill_category++) {
		skill_no = ds_readbs(SKILLS_INDEX + skill_category * 2);
		while (ds_readbs(SKILLS_INDEX + skill_category * 2) + ds_readbs((SKILLS_INDEX + 1) + skill_category * 2) > skill_no) {
			status_show_skill(hero, skill_no,
				ds_readbs(SKILLS_INDEX + skill_category * 2),
				ds_readw(STATUSPAGE_SKILLS_XY + skill_category * 6),
				ds_readw((STATUSPAGE_SKILLS_XY + 2) + skill_category * 6),
				ds_readw((STATUSPAGE_SKILLS_XY + 4) + skill_category * 6));

			skill_no++;
		}
	}
}

#ifdef M302de_ORIGINAL_BUGFIX
/**
 * \brief   a helper for an Original Bugfix
 *
 *          This makes changes to the max LE visible by changing the format string.
 *
 * \param   fmt         format string
 */
static void set_status_string(char *fmt)
{
	char *fp;

	fp = strstr(fmt, "%d");

	fp[1] = 's';
}

/**
 * \brief   a helper for an Original Bugfix
 *
 *          This makes changes to the max LE visible by changing the format string.
 *
 * \param   fmt         format string
 */
static void reset_status_string(char *fmt)
{
	char *fp;

	fp = strstr(fmt, "%s");

	fp[1] = 'd';
}
#endif

/**
 * \brief   shows the status screen of a hero
 *
 * \param   index       index of the hero
 */
void status_show(Bit16u index)
{
#ifdef M302de_ORIGINAL_BUGFIX
	char le_fix[10];
#endif
	unsigned char *hero;
	Bit16u txt_tabpos1_bak, txt_tabpos2_bak, txt_tabpos3_bak, txt_tabpos4_bak;
	Bit8s val;
	signed short width;
	signed short height;
	signed short at;
	signed short pa;
	signed short l1;
	signed short bp;
	signed short i;
	signed short j;

	struct nvf_desc nvf;

	txt_tabpos1_bak = ds_readw(TXT_TABPOS1);
	txt_tabpos2_bak = ds_readw(TXT_TABPOS2);
	txt_tabpos3_bak = ds_readw(TXT_TABPOS3);
	txt_tabpos4_bak = ds_readw(TXT_TABPOS4);

	hero = get_hero(index);

	set_var_to_zero();
	update_mouse_cursor();

	if (ds_readb(PP20_INDEX) != ARCHIVE_FILE_ZUSTA_UK) {
		ds_writew(UPDATE_STATUSLINE, 0);
		ds_writeb(PP20_INDEX, ARCHIVE_FILE_ZUSTA_UK);
		do_fill_rect((Bit8u*)ds_readd(VGA_MEMSTART), 0, 0, 319, 199, 0);
		wait_for_vsync();
		set_palette(p_datseg + STATUSPAGE_PALETTE, 0, 0x20);
	}

	ds_writed(ACTION_TABLE_PRIMARY, (Bit32u)(p_datseg + ACTION_TABLE_STATUS));
	ds_writed(ACTION_TABLE_SECONDARY, 0);
	ds_writed(VGA_BACKBUFFER, (Bit32u)g_renderbuf_ptr);
	set_textcolor(0, 2);

	/* load and draw the background */
	load_pp20(ARCHIVE_FILE_ZUSTA_UK);

	/* draw the picture of the hero */
	ds_writew(PIC_COPY_X1, 16);
	ds_writew(PIC_COPY_Y1, 9);
	ds_writew(PIC_COPY_X2, 47);
	ds_writew(PIC_COPY_Y2, 40);
	ds_writed(PIC_COPY_DST, (Bit32u)g_renderbuf_ptr);
	/* the source must be passed here as Bit8u* */
	ds_writed(PIC_COPY_SRC, (Bit32u)(hero + HERO_PORTRAIT));
	do_pic_copy(0);

	ds_writed(PIC_COPY_DST, ds_readd(VGA_MEMSTART));

	/* print inventory and silouette values */
	if (ds_readws(STATUS_PAGE_MODE) < 3) {

		nvf.src = g_buffer10_ptr;
		nvf.type = 0;

		nvf.width = (Bit8u*)&width;
		nvf.height = (Bit8u*)&height;

		for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++) {

			if (host_readw(hero + i * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_ITEM_ID) == ITEM_NONE)
				continue;

			nvf.dst = g_icon;
			/* set no */
			nvf.no = host_readw(get_itemsdat(host_readw(hero + i * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_ITEM_ID)));

			process_nvf(&nvf);

			/* draw the item icon */
			ds_writew(PIC_COPY_X1, ds_readw(INVSLOT_ICONXY_TABLE + i * 4));
			ds_writew(PIC_COPY_Y1, ds_readw(INVSLOT_ICONXY_TABLE + i * 4 + 2));
			ds_writew(PIC_COPY_X2, ds_readw(INVSLOT_ICONXY_TABLE + i * 4) + 15);
			ds_writew(PIC_COPY_Y2, ds_readw(INVSLOT_ICONXY_TABLE + i * 4 + 2) + 15);
			ds_writed(PIC_COPY_DST, (Bit32u)g_renderbuf_ptr);
			ds_writed(PIC_COPY_SRC, (Bit32u)g_icon);
			do_pic_copy(0);

			ds_writed(PIC_COPY_DST, ds_readd(VGA_MEMSTART));

			/* check if stackable */
			if (item_stackable(get_itemsdat(host_readw(hero + i * SIZEOF_INVENTORY + HERO_INVENTORY)))) {

				set_textcolor(0xff, 0);
				my_itoa(host_readw(hero + i * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_QUANTITY),
					g_dtp2, 10);

				GUI_print_string(g_dtp2,
					ds_readw(INVSLOT_ICONXY_TABLE + i * 4) + 16 - GUI_get_space_for_string(g_dtp2, 0),
					ds_readw(INVSLOT_ICONXY_TABLE + i * 4 + 2) + 9);

				set_textcolor(0, 2);
			}
		}

		/* print height */
		sprintf((char*)g_dtp2, get_tx2(33), host_readb(hero + HERO_HEIGHT));
		GUI_print_string(g_dtp2, 158, 116);

		/* print weight */
		sprintf((char*)g_dtp2, get_tx2(34), host_readw(hero + HERO_WEIGHT));
		GUI_print_string(g_dtp2, 59, 179);

	} else {
		do_fill_rect(g_renderbuf_ptr, 0, 50, 319, 191, 2);
	}

	ds_writew(TXT_TABPOS1, 0x5f);

	/* print name */
	set_textcolor(0xff, 2);
	GUI_print_string((char*)hero + HERO_NAME2, 59, 9);

	/* print typus */
	set_textcolor(0, 2);

	GUI_print_string(get_ttx(((host_readb(hero + HERO_SEX)) ? 0x251 : 0x9) + host_readbs(hero + HERO_TYPE)), 59, 16);


	/* show AP */
	sprintf((char*)g_dtp2, get_ttx(619), host_readd(hero + HERO_AP));
	GUI_print_string(g_dtp2, 59, 26);

	/* print level */
	sprintf((char*)g_dtp2, get_tx2(7), host_readbs(hero + HERO_LEVEL));
	GUI_print_string(g_dtp2, 59, 33);

	/* print money */
	make_valuta_str((char*)g_text_output_buf, host_readd(hero + HERO_MONEY));

	sprintf((char*)g_dtp2, get_ttx(300), g_text_output_buf);
	GUI_print_string(g_dtp2, 59, 43);

	/* dead, unconscious or drunk */
	if (hero_dead(hero))
		/* print if dead */
		GUI_print_string(get_tx2(0), 155, 9);
	else if (hero_unconscious(hero))
		/* print if uncounscious */
		GUI_print_string(get_tx2(6), 155, 9);
	else if (host_readb(hero + HERO_DRUNK))
		/* print if drunk */
		GUI_print_string(get_tx2(54), 155, 9);

	/* print asleep */
	if (hero_asleep(hero))
		GUI_print_string(get_tx2(1), 155, 16);

	/* print petrified */
	if (hero_petrified(hero))
		GUI_print_string(get_tx2(2), 155, 23);

	/* print diseased */
	if (hero_is_diseased(hero))
		GUI_print_string(get_tx2(4), 155, 30);

	/* print poison */
	if (hero_is_poisoned(hero))
		GUI_print_string(get_tx2(3), 155, 37);

	/* print hunger */
	GUI_print_string(get_tx2(8), 220, 36);
	/* print thirst */
	GUI_print_string(get_tx2(9), 220, 43);


	/* print page content */
	switch (ds_readw(STATUS_PAGE_MODE)) {
		/* base values */
		case 1: {

			/* print god */
			ds_writew(TXT_TABPOS1, 265);

			sprintf((char*)g_dtp2, get_tx2(10), get_ttx(host_readbs(hero + HERO_GOD) + 21));
			GUI_print_string(g_dtp2, 200, 55);

			/* show attributes */
			ds_writew(TXT_TABPOS1, 220);
			ds_writew(TXT_TABPOS2, 265);
			ds_writew(TXT_TABPOS3, 285);

			for (i = 0; i <= 13; i++) {

				val = host_readbs(hero + i * 3 + HERO_ATTRIB) + host_readbs(hero + i * 3 + HERO_ATTRIB_MOD);

				sprintf((char*)g_text_output_buf + i * 10,
					get_tx2(51),
					host_readbs(hero + i * 3 + 0x34) != val ?
						get_tx2(49) :
						(char*)p_datseg + EMPTY_STRING6,
					val,
					host_readbs(hero + i * 3 + 0x34) != val ?
						get_tx2(50) :
						(char*)p_datseg + EMPTY_STRING7,
					host_readbs(hero + i * 3 + 0x34));

			}
			sprintf((char*)g_dtp2,
				get_tx2(12),
				g_text_output_buf,
				g_text_output_buf + 70,
				g_text_output_buf + 10,
				g_text_output_buf + 80,
				g_text_output_buf + 20,
				g_text_output_buf + 90,
				g_text_output_buf + 30,
				g_text_output_buf + 100,
				g_text_output_buf + 40,
				g_text_output_buf + 110,
				g_text_output_buf + 50,
				g_text_output_buf + 120,
				g_text_output_buf + 60,
				g_text_output_buf + 130);

			GUI_print_string(g_dtp2, 200, 70);

			/* calculate BP */
			bp = 8;
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 50 <= host_readws(hero + HERO_LOAD))
				bp--;
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 75 <= host_readws(hero + HERO_LOAD))
				bp -= 2;
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 100 <= host_readws(hero + HERO_LOAD))
				bp -= 2;
#ifdef M302de_ORIGINAL_BUGFIX
			if (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) * 110 <= host_readws(hero + HERO_LOAD)) {
				/* Original-Bug 6: status screen shows wrong number of BP if the load is at least 110% of the carrying capacity.
				   (3 BP shown, but in fact only 1 BP, see function FIG_do_round in seg032.cpp) */
				bp = 1;
			}
#endif

			if (ds_readw(GAME_MODE) == GAME_MODE_ADVANCED) {
				/* advanded mode */

#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bugfix: show permanent damage red */
				char le_fix[10];
				set_status_string(get_tx2(13));

				if (host_readb(hero + HERO_LE_MOD)) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, host_readw(hero + HERO_LE_ORIG), 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", host_readw(hero + HERO_LE_ORIG));
				}

				sprintf((char*)g_dtp2,
					get_tx2(13),
					host_readw(hero + HERO_LE), le_fix,			/* LE */
					host_readw(hero + HERO_AE), host_readw(hero + HERO_AE_ORIG),	/* AE */
					host_readbs(hero + HERO_MR),			/* MR */
					host_readbs(hero + HERO_RS_BONUS1) + host_readbs(hero + HERO_RS_BONUS2), /* RS */
					host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readw(hero + HERO_LE) +
						host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK)),		/* Ausdauer*/
					host_readw(hero + HERO_LOAD),				/* Last */
					bp);							/* BP */
				reset_status_string(get_tx2(13));
				/* Original-Bugfix end */
#else

				/* Original Behavior: print max LE in black */
				sprintf((char*)g_dtp2,
					get_tx2(13),
					host_readw(hero + HERO_LE), host_readw(hero + HERO_LE_ORIG),	/* LE */
					host_readw(hero + HERO_AE), host_readw(hero + HERO_AE_ORIG),	/* AE */
					host_readbs(hero + HERO_MR),			/* MR */
					host_readbs(hero + HERO_RS_BONUS1) + host_readbs(hero + HERO_RS_BONUS2), /* RS */
					host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK)) + (host_readws(hero + HERO_LE) +
						host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK))),		/* Ausdauer*/
					host_readw(hero + HERO_LOAD),				/* Last */
					bp);							/* BP */
#endif


				GUI_print_string(g_dtp2, 200, 130);
			} else {
				/* novice mode */

				/* calculate AT base value */
				if (host_readb(hero + HERO_RS_BE) & 1) {
					l1 = -1;
				} else {
					l1 = 0;
				}

				at = (host_readbs(hero + HERO_WEAPON_TYPE) < 7) ?
					/* melee weapons */
					(host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WEAPON_TYPE)) +
						host_readbs(hero + HERO_AT_MOD) -
						(host_readbs(hero + HERO_RS_BE) / 2)) :
					/* range weapons */
					(host_readbs(hero + HERO_AT) -
						(host_readbs(hero + HERO_RS_BE) / 2));
				at += l1;

				/* calculate PA base value */
				pa = (host_readbs(hero + HERO_WEAPON_TYPE) < 7) ?
					/* melee weapons */
					(host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WEAPON_TYPE)) +
						host_readbs(hero + HERO_PA_MOD) -
						(host_readbs(hero + HERO_RS_BE) / 2)) :
					/* range weapons */
					 (host_readbs(hero + HERO_PA) -
						(host_readbs(hero + HERO_RS_BE) / 2));

				if (at < 0)
					at = 0;
				if (pa < 0)
					pa = 0;
#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bugfix: show permanent damage in red */
				set_status_string(get_tx2(52));

				if (host_readb(hero + HERO_LE_MOD)) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, host_readw(hero + HERO_LE_ORIG), 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", host_readw(hero + HERO_LE_ORIG));
				}


				sprintf((char*)g_dtp2,
					get_tx2(52),
					host_readw(hero + HERO_LE), le_fix,			/* LE */
					host_readw(hero + HERO_AE), host_readw(hero + HERO_AE_ORIG),	/* AE */
					at, pa,							/* AT PA */
					host_readbs(hero + HERO_MR),			/* MR */
					host_readbs(hero + HERO_RS_BONUS1) + host_readbs(hero + HERO_RS_BONUS2),	/* RS */
					host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) + host_readw(hero + HERO_LE) +
						host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK)),		/* Ausdauer */
					host_readw(hero + HERO_LOAD),				/* Last */
					bp);							/* BP */

				reset_status_string(get_tx2(52));
				/* Original-Bugfix end */
#else
				sprintf((char*)g_dtp2,
					get_tx2(52),
					host_readw(hero + HERO_LE), host_readw(hero + HERO_LE_ORIG),	/* LE */
					host_readw(hero + HERO_AE), host_readw(hero + HERO_AE_ORIG),	/* AE */
					at, pa,							/* AT PA */
					host_readbs(hero + HERO_MR),			/* MR */
					host_readbs(hero + HERO_RS_BONUS1) + host_readbs(hero + HERO_RS_BONUS2),	/* RS */
					host_readws(hero + HERO_LE) + host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) +
						host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK)),		/* Ausdauer */
					host_readw(hero + HERO_LOAD),				/* Last */
					bp);							/* BP */
#endif

				GUI_print_string(g_dtp2, 200, 124);
			}

			break;
		}
		/* AT PA values */
		case 2: {
			ds_writew(TXT_TABPOS1, 275);
			ds_writew(TXT_TABPOS2, 295);

			/* Fernkampfwaffen-Basiswert: (KL + GE + KK)/4 */
			j = (host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KL)) +
				host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KL)) +
				host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_GE)) +
				host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_GE)) +
				host_readbs(hero + (HERO_ATTRIB + 3 * ATTRIB_KK)) +
				host_readbs(hero + (HERO_ATTRIB_MOD + 3 * ATTRIB_KK))) / 4;

			if (host_readb(hero + HERO_RS_BE) & 1)
				l1 = -1;
			else
				l1 = 0;

			at = (host_readbs(hero + HERO_WEAPON_TYPE) < 7) ?
				(host_readbs(hero + HERO_AT + host_readbs(hero + HERO_WEAPON_TYPE)) +
					host_readbs(hero + HERO_AT_MOD) -
					host_readbs(hero + HERO_RS_BE) / 2) :
				host_readbs(hero + HERO_AT) - host_readbs(hero + HERO_RS_BE) / 2;

			at += l1;

			pa =  (host_readbs(hero + HERO_WEAPON_TYPE) < 7) ?
				host_readbs(hero + HERO_PA + host_readbs(hero + HERO_WEAPON_TYPE)) +
					host_readbs(hero + HERO_PA_MOD) -
					host_readbs(hero + HERO_RS_BE) / 2 :
				host_readbs(hero + HERO_PA) - host_readbs(hero + HERO_RS_BE) / 2;

			if (at < 0)
				at = 0;

			if (pa < 0)
				pa = 0;

			sprintf((char*)g_dtp2,
				get_tx2(5),
				host_readbs(hero + HERO_ATPA_BASIS),
				get_ttx(48),
				host_readbs(hero + HERO_AT) - host_readbs(hero + HERO_RS_BE) / 2,
				host_readbs(hero + HERO_PA) - host_readbs(hero + HERO_RS_BE) / 2,
				get_ttx(49),

				host_readbs(hero + (HERO_AT + 1)) - host_readbs(hero + HERO_RS_BE) / 2,
				host_readbs(hero + (HERO_PA + 1)) - host_readbs(hero + HERO_RS_BE) / 2,
				get_ttx(50),

				host_readbs(hero + (HERO_AT + 2)) - host_readbs(hero + HERO_RS_BE) / 2,
				host_readbs(hero + (HERO_PA + 2)) - host_readbs(hero + HERO_RS_BE) / 2,
				get_ttx(51),

				host_readbs(hero + (HERO_AT + 3)) - host_readbs(hero + HERO_RS_BE) / 2,
				host_readbs(hero + (HERO_PA + 3)) - host_readbs(hero + HERO_RS_BE) / 2,
				get_ttx(52),

				host_readbs(hero + (HERO_AT + 4)) - host_readbs(hero + HERO_RS_BE) / 2,
				host_readbs(hero + (HERO_PA + 4)) - host_readbs(hero + HERO_RS_BE) / 2,
				get_ttx(53),

				host_readbs(hero + (HERO_AT + 5)) - host_readbs(hero + HERO_RS_BE) / 2,
				host_readbs(hero + (HERO_PA + 5)) - host_readbs(hero + HERO_RS_BE) / 2,
				get_ttx(54),

				host_readbs(hero + (HERO_AT + 6)) - host_readbs(hero + HERO_RS_BE) / 2,
				host_readbs(hero + (HERO_PA + 6)) - host_readbs(hero + HERO_RS_BE) / 2,
				at,
				pa,

				get_ttx(55),
				host_readbs(hero + (HERO_TALENTS + TA_SCHUSSWAFFEN)) + j,

				get_ttx(56),
				host_readbs(hero + (HERO_TALENTS + TA_WURFWAFFEN)) + j);

			GUI_print_string(g_dtp2, 200, 60);
			break;
		}
		/* skills */
		case 3: {
			status_show_skills(hero);
			break;
		}
		/* spells */
		case 4: {
			/* print headers */
			set_textcolor(0xff, 2);

			GUI_print_string(get_ttx(192), GUI_get_first_pos_centered(get_ttx(192), 5, 100, 0), 55);

			GUI_print_string(get_ttx(194), GUI_get_first_pos_centered(get_ttx(194), 110, 100, 0), 55);

			GUI_print_string(get_ttx(197), GUI_get_first_pos_centered(get_ttx(197), 215, 100, 0), 55);

			GUI_print_string(get_ttx(193), GUI_get_first_pos_centered(get_ttx(193), 5, 100, 0), 97);

			GUI_print_string(get_ttx(195), GUI_get_first_pos_centered(get_ttx(195), 110, 100, 0), 104);

			GUI_print_string(get_ttx(198), GUI_get_first_pos_centered(get_ttx(198), 215, 100, 0), 97);

			GUI_print_string(get_ttx(196), GUI_get_first_pos_centered(get_ttx(196), 110, 100, 0), 132);

			GUI_print_string(get_ttx(199), GUI_get_first_pos_centered(get_ttx(199), 215, 100, 0), 153);

			/* print values */
			set_textcolor(0, 2);

			for (j = 0; j < 8; j++) {

				i = ds_readbs(SPELLS_INDEX + j * 2);

				while (ds_readbs(SPELLS_INDEX + j * 2) + ds_readbs(SPELLS_INDEX + 1 + j * 2) > i) {

					status_show_spell(hero,
						i,
						ds_readbs(SPELLS_INDEX + j * 2),
						ds_readws(STATUSPAGE_SPELLS_XY + 0 + j * 6),
						ds_readws(STATUSPAGE_SPELLS_XY + 2 + j * 6),
						ds_readws(STATUSPAGE_SPELLS_XY + 4 + j * 6));
					i++;
				}
			}
			break;
		}
		/* more spells */
		case 5: {
			/* print headers */
			set_textcolor(0xff, 2);

			GUI_print_string(get_ttx(100), GUI_get_first_pos_centered(get_ttx(100), 5, 100, 0), 55);

			GUI_print_string(get_ttx(201), GUI_get_first_pos_centered(get_ttx(201), 5, 100, 0), 129);

			GUI_print_string(get_ttx(202), GUI_get_first_pos_centered(get_ttx(202), 110, 100, 0), 55);

			GUI_print_string(get_ttx(203), GUI_get_first_pos_centered(get_ttx(203), 215, 100, 0), 55);

			/* show values */
			set_textcolor(0, 2);

			for (j = 0; j < 4; j++) {

				i = ds_readbs(SPELLS_INDEX2 + j * 2);

				while (ds_readbs(SPELLS_INDEX2 + j * 2) + ds_readbs(SPELLS_INDEX2 + 1 + j * 2) > i) {

					status_show_spell(hero,
						i,
						ds_readbs(SPELLS_INDEX2 + j * 2),
						ds_readws(STATUSPAGE_SPELLS2_XY + 0 + j * 6),
						ds_readws(STATUSPAGE_SPELLS2_XY + 2 + j * 6),
						ds_readws(STATUSPAGE_SPELLS2_XY + 4 + j * 6));
					i++;
				}
			}
			break;
		}
	}

	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 319);
	ds_writew(PIC_COPY_Y2, 199);
	ds_writed(PIC_COPY_SRC, (Bit32u)g_renderbuf_ptr);
	do_pic_copy(0);

	ds_writew(UPDATE_STATUSLINE, 1);

	if (ds_readws(STATUS_PAGE_MODE) >= 3) {
		do_v_line((Bit8u*)ds_readd(VGA_MEMSTART), 107, 54, 195, 0);
		do_v_line((Bit8u*)ds_readd(VGA_MEMSTART), 212, 54, 195, 0);
	}

	ds_writew(TXT_TABPOS1, txt_tabpos1_bak);
	ds_writew(TXT_TABPOS2, txt_tabpos2_bak);
	ds_writew(TXT_TABPOS3, txt_tabpos3_bak);
	ds_writew(TXT_TABPOS4, txt_tabpos4_bak);

	ds_writed(VGA_BACKBUFFER, ds_readd(VGA_MEMSTART));

	refresh_screen_size();
}

#if !defined(__BORLANDC__)
}
#endif
