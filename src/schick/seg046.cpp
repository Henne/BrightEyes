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

int8_t g_statuspage_sel_inv_slot_1 = 1; // ds:0x636e
int8_t g_statuspage_sel_inv_slot_2 = 1; // ds:0x636f
int8_t g_statuspage_sel_inv_slot_3 = 7; // ds:0x6370
int8_t g_statuspage_sel_inv_slot_4 = -1; // ds:0x6371
uint8_t g_statuspage_palette[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x06, 0x06, 0x06 },
	{ 0x3f, 0x26, 0x10 },
	{ 0x3f, 0x1e, 0x00 },
	{ 0x33, 0x18, 0x00 },
	{ 0x27, 0x13, 0x00 },
	{ 0x1c, 0x0e, 0x00 },
	{ 0x10, 0x08, 0x00 },
	{ 0x3f, 0x3f, 0x00 },
	{ 0x3c, 0x00, 0x00 },
	{ 0x1b, 0x00, 0x00 },
	{ 0x00, 0x0e, 0x3f },
	{ 0x00, 0x00, 0x25 },
	{ 0x3c, 0x30, 0x21 },
	{ 0x34, 0x28, 0x19 },
	{ 0x2c, 0x1f, 0x12 },
	{ 0x24, 0x17, 0x0a },
	{ 0x3c, 0x3c, 0x3c },
	{ 0x38, 0x38, 0x38 },
	{ 0x34, 0x34, 0x34 },
	{ 0x30, 0x30, 0x30 },
	{ 0x2c, 0x2c, 0x2c },
	{ 0x28, 0x28, 0x28 },
	{ 0x24, 0x24, 0x24 },
	{ 0x20, 0x20, 0x20 },
	{ 0x1c, 0x1c, 0x1c },
	{ 0x18, 0x18, 0x18 },
	{ 0x14, 0x14, 0x14 },
	{ 0x10, 0x10, 0x10 },
	{ 0x0c, 0x0c, 0x0c },
	{ 0x08, 0x08, 0x08 },
	{ 0x3f, 0x3f, 0x3f }
}; // ds:0x6372
const struct struct_point g_invslot_iconxy_table[23] = {
	{ 0x0045, 0x0038 },
	{ 0x003b, 0x0051 },
	{ 0x0092, 0x0051 },
	{ 0x0045, 0x006b },
	{ 0x0088, 0x006b },
	{ 0x0045, 0x008d },
	{ 0x0088, 0x00af },
	{ 0x0010, 0x0038 },
	{ 0x0021, 0x0038 },
	{ 0x0010, 0x0049 },
	{ 0x0021, 0x0049 },
	{ 0x0010, 0x005a },
	{ 0x0021, 0x005a },
	{ 0x0010, 0x006b },
	{ 0x0021, 0x006b },
	{ 0x0010, 0x007c },
	{ 0x0021, 0x007c },
	{ 0x0010, 0x008d },
	{ 0x0021, 0x008d },
	{ 0x0010, 0x009e },
	{ 0x0021, 0x009e },
	{ 0x0010, 0x00af },
	{ 0x0021, 0x00af }
}; // ds:0x63d2
const struct statuspage_line g_statuspage_spellclasses_1_xy[8] = { /* x_name, x_val, y */
	{   5, 104,  62 },
	{   5, 104, 104 },
	{ 112, 209,  62 },
	{ 112, 209, 111 },
	{ 112, 209, 139 },
	{ 217, 315,  62 },
	{ 217, 315, 104 },
	{ 217, 315, 160 }
}; // ds:0x642e
const struct statuspage_line g_statuspage_spellclasses_2_xy[4] = { /* x_name, x_val, y */
	{   5, 104,  62 },
	{   5, 104, 139 },
	{ 112, 209,  62 },
	{ 217, 315,  62 }
}; // ds:0x645e
const struct statuspage_line g_statuspage_talentclasses_xy[7] = { /* x_name, x_val, y */
	{   5, 104,  62 },
	{ 217, 315,  62 },
	{ 217, 315, 139 },
	{   5, 104, 132 },
	{ 112, 209, 132 },
	{ 112, 209,  62 },
	{   5, 104, 181 }
}; // ds:0x6476
static char g_empty_string6[1] = ""; // ds:0x64a0
static char g_empty_string7[1] = ""; // ds:0x64a1


/**
 * \brief   prints spell name and value
 *
 * \param   hero              the hero the spell is from
 * \param   spell_id          spell ID
 * \param   first_spell_id    ID of the the first spell in the same spellclass
 * \param   x1                the leftmost x coordinate
 * \param   x2                the rightmost x coordinate
 * \param   gy                the upper y coordinate of this spellclass
 */
/* Borlandified and identical */
void status_show_spell(const struct struct_hero *hero, const signed int spell_id, const signed int first_spell_id,
			const signed int x1, const signed int x2, const signed int gy)
{
	const signed int pos_in_spellclass = spell_id - first_spell_id;
	char str[10];


	/* print spellname */
	GUI_print_string(get_ttx(spell_id + 0x6a), x1, gy + pos_in_spellclass * 7);

	/* convert value to string */
	my_itoa(hero->spells[spell_id], str, 10);

	/* print value */
	GUI_print_string(str, x2 - GUI_get_space_for_string(str, 0), gy + pos_in_spellclass * 7);
}

/**
 * \brief   prints talent name and value
 *
 * \param   hero              the hero the talent is from
 * \param   talent_id          talent ID
 * \param   first_talent_id    ID of the first talent in the same talentclass
 * \param   x1                the leftmost x coordinate
 * \param   x2                the rightmost x coordinate
 * \param   gy                the upper y coordinate of this talentclass
 */
/* Borlandified and identical */
void status_show_talent(const struct struct_hero *hero, const signed int talent_id, const signed int first_talent_id,
			const signed int x1, const signed int x2, const signed int gy)
{
	const signed int pos_in_talentclass = talent_id - first_talent_id;
	char str[10];

	/* print talentname */
	GUI_print_string(get_ttx(talent_id + 0x30), x1, gy + pos_in_talentclass * 7);

	/* convert value to string */
	my_itoa(hero->talents[talent_id] , str, 10);

	/* print value */
	GUI_print_string(str, x2 - GUI_get_space_for_string(str, 0), gy + pos_in_talentclass * 7);
}

/**
 * \brief   shows all talents and their values
 *
 * \param   hero        the hero whose talents should be shown
 */
/* Borlandified and identical */
void status_show_talents(const struct struct_hero *hero)
{
	signed int talentclass_id;
	signed int talent_id;

	set_textcolor(0xff, 2);

	/* print talentclass names */
	GUI_print_string(get_ttx(100), GUI_get_first_pos_centered(get_ttx(100), 5, 100, 0), 55);

	GUI_print_string(get_ttx(104), GUI_get_first_pos_centered(get_ttx(104), 110, 100, 0), 55);

	GUI_print_string(get_ttx(101), GUI_get_first_pos_centered(get_ttx(101), 215, 100, 0), 55);

	GUI_print_string(get_ttx(105), GUI_get_first_pos_centered(get_ttx(105), 5, 100, 0), 125);

	GUI_print_string(get_ttx(103), GUI_get_first_pos_centered(get_ttx(103), 110, 100, 0), 125);

	GUI_print_string(get_ttx(102), GUI_get_first_pos_centered(get_ttx(102), 215, 100, 0), 132);

	GUI_print_string(get_ttx(106), GUI_get_first_pos_centered(get_ttx(106), 5, 100, 0), 174);

	set_textcolor(0, 2);

	for (talentclass_id = 0; talentclass_id < 7; talentclass_id++) {

		talent_id = g_talentclasses[talentclass_id].first;

		while (g_talentclasses[talentclass_id].first + g_talentclasses[talentclass_id].length > talent_id) {

			status_show_talent(hero, talent_id, g_talentclasses[talentclass_id].first,
					g_statuspage_talentclasses_xy[talentclass_id].x_name,
					g_statuspage_talentclasses_xy[talentclass_id].x_val,
					g_statuspage_talentclasses_xy[talentclass_id].y);

			talent_id++;
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
	char *fp = strstr(fmt, "%d");

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
	char *fp = strstr(fmt, "%s");

	fp[1] = 'd';
}
#endif

/**
 * \brief   shows the status screen of a hero
 *
 * \param   index       index of the hero
 */
void status_show(const signed int index)
{
#ifdef M302de_ORIGINAL_BUGFIX
	char le_fix[10];
#endif
	struct struct_hero *hero;
	int16_t txt_tabpos1_bak, txt_tabpos2_bak, txt_tabpos3_bak, txt_tabpos4_bak;
	int8_t val;
	signed int width;
	signed int height;
	signed int at;
	signed int pa;
	signed int l1;
	signed int bp;
	signed int i;
	signed int j;

	struct nvf_extract_desc nvf;

	txt_tabpos1_bak = g_txt_tabpos[0];
	txt_tabpos2_bak = g_txt_tabpos[1];
	txt_tabpos3_bak = g_txt_tabpos[2];
	txt_tabpos4_bak = g_txt_tabpos[3];

	hero = get_hero(index);

	disable_ani();
	call_mouse_bg();

	if (g_pp20_index != ARCHIVE_FILE_ZUSTA_UK) {
		g_update_statusline = 0;
		g_pp20_index = ARCHIVE_FILE_ZUSTA_UK;
		do_fill_rect(g_vga_memstart, 0, 0, 319, 199, 0);
		wait_for_vsync();
		set_palette(&g_statuspage_palette[0][0], 0, 0x20);
	}

	g_action_table_primary = &g_action_table_status[0];
	g_action_table_secondary = NULL;

	g_vga_backbuffer = g_renderbuf_ptr;
	set_textcolor(0, 2);

	/* load and draw the background */
	load_pp20(ARCHIVE_FILE_ZUSTA_UK);

	/* draw the picture of the hero */
	g_pic_copy.x1 = 16;
	g_pic_copy.y1 = 9;
	g_pic_copy.x2 = 47;
	g_pic_copy.y2 = 40;
	g_pic_copy.dst = g_renderbuf_ptr;
	/* the source must be passed here as uint8_t* */
	g_pic_copy.src = hero->pic;
	do_pic_copy(0);

	g_pic_copy.dst = g_vga_memstart;

	/* print inventory and silouette values */
	if (g_status_page_mode < 3) {

		nvf.src = g_buffer10_ptr;
		nvf.compression_type = 0;

		nvf.width = &width;
		nvf.height = &height;

		for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++) {

			if (hero->inventory[i].item_id == ITEM_NONE)
				continue;

			nvf.dst = g_icon;
			/* set no */
			nvf.image_num = g_itemsdat[hero->inventory[i].item_id].gfx;

			process_nvf_extraction(&nvf);

			/* draw the item icon */
			g_pic_copy.x1 = g_invslot_iconxy_table[i].x;
			g_pic_copy.y1 = g_invslot_iconxy_table[i].y;
			g_pic_copy.x2 = g_invslot_iconxy_table[i].x + 15;
			g_pic_copy.y2 = g_invslot_iconxy_table[i].y + 15;
			g_pic_copy.dst = g_renderbuf_ptr;
			g_pic_copy.src = g_icon;
			do_pic_copy(0);

			g_pic_copy.dst = g_vga_memstart;

			/* check if stackable */
			if (g_itemsdat[hero->inventory[i].item_id].flags.stackable) {


				set_textcolor(0xff, 0);
				my_itoa(hero->inventory[i].quantity, g_dtp2, 10);

				GUI_print_string(g_dtp2,
					g_invslot_iconxy_table[i].x + 16 - GUI_get_space_for_string(g_dtp2, 0),
					g_invslot_iconxy_table[i].y + 9);

				set_textcolor(0, 2);
			}
		}

		/* print height */
		sprintf(g_dtp2, get_tx2(33), hero->height);
		GUI_print_string(g_dtp2, 158, 116);

		/* print weight */
		sprintf(g_dtp2, get_tx2(34), hero->weight);
		GUI_print_string(g_dtp2, 59, 179);

	} else {
		do_fill_rect(g_renderbuf_ptr, 0, 50, 319, 191, 2);
	}

	g_txt_tabpos[0] = 0x5f;

	/* print name */
	set_textcolor(0xff, 2);
	GUI_print_string(hero->alias, 59, 9);

	/* print typus */
	set_textcolor(0, 2);
	GUI_print_string(get_ttx((hero->sex ? 0x251 : 0x9) + hero->typus), 59, 16);


	/* show AP */
	sprintf(g_dtp2, get_ttx(619), hero->ap);
	GUI_print_string(g_dtp2, 59, 26);

	/* print level */
	sprintf(g_dtp2, get_tx2(7), hero->level);
	GUI_print_string(g_dtp2, 59, 33);

	/* print money */
	make_valuta_str(g_text_output_buf, hero->money);

	sprintf(g_dtp2, get_ttx(300), g_text_output_buf);
	GUI_print_string(g_dtp2, 59, 43);

	/* dead, unconscious or drunk */
	if (hero->flags.dead) {

		/* print if dead */
		GUI_print_string(get_tx2(0), 155, 9);

	} else if (hero->flags.unconscious) {

		/* print if uncounscious */
		GUI_print_string(get_tx2(6), 155, 9);

	} else if (hero->drunk) {

		/* print if drunk */
		GUI_print_string(get_tx2(54), 155, 9);
	}

	/* print asleep */
	if (hero->flags.asleep)
		GUI_print_string(get_tx2(1), 155, 16);

	/* print petrified */
	if (hero->flags.petrified)
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
	switch (g_status_page_mode) {
		/* base values */
		case 1: {

			/* print god */
			g_txt_tabpos[0] = 265;

			sprintf(g_dtp2, get_tx2(10), get_ttx(hero->god + 21));
			GUI_print_string(g_dtp2, 200, 55);

			/* show attributes */
			g_txt_tabpos[0] = 220;
			g_txt_tabpos[1] = 265;
			g_txt_tabpos[2] = 285;

			for (i = 0; i <= 13; i++) {

				val = hero->attrib[i].current + hero->attrib[i].mod;

				sprintf(g_text_output_buf + i * 10, get_tx2(51),
					hero->attrib[i].normal != val ? get_tx2(49) : g_empty_string6,
					val,
					hero->attrib[i].normal != val ? get_tx2(50) : g_empty_string7,
					hero->attrib[i].normal);

			}
			sprintf(g_dtp2,	get_tx2(12),
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
			if (hero->attrib[ATTRIB_KK].current * 50 <= hero->load) {
				bp--;
			}

			if (hero->attrib[ATTRIB_KK].current * 75 <= hero->load) {
				bp -= 2;
			}

			if (hero->attrib[ATTRIB_KK].current * 100 <= hero->load) {
				bp -= 2;
			}

#ifdef M302de_ORIGINAL_BUGFIX
			if (hero->attrib[ATTRIB_KK].current * 110 <= hero->load) {
				/* Original-Bug 6: status screen shows wrong number of BP if the load is at least 110% of the carrying capacity.
				   (3 BP shown, but in fact only 1 BP, see function FIG_do_round in seg032.cpp) */
				bp = 1;
			}
#endif

			if (g_game_mode == GAME_MODE_ADVANCED) {
				/* advanded mode */

#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bugfix: show permanent damage red */
				char le_fix[10];
				set_status_string(get_tx2(13));

				if (hero->le_max_malus) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, hero->le_max, 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", hero->le_max);
				}

				sprintf(g_dtp2,	get_tx2(13),
					hero->le, le_fix,			/* LE */
					hero->ae, hero->ae_max,			/* AE */
					hero->mr,				/* MR */
					hero->rs_bonus + hero->rs_bonus_dummy,	/* RS */
					hero->attrib[ATTRIB_KK].current + hero->le +
						hero->attrib[ATTRIB_KK].mod,	/* Ausdauer*/
					hero->load,				/* Last */
					bp);					/* BP */

				reset_status_string(get_tx2(13));
				/* Original-Bugfix end */
#else

				/* Original Behavior: print max LE in black */
				sprintf(g_dtp2,
					get_tx2(13),
					hero->le, hero->le_max,			/* LE */
					hero->ae, hero->ae_max,			/* AE */
					hero->mr,				/* MR */
					hero->rs_bonus + hero->rs_bonus_dummy,	/* RS */
					hero->le +
						hero->attrib[ATTRIB_KK].current +
						hero->attrib[ATTRIB_KK].mod,	/* Ausdauer*/
					hero->load,				/* Last */
					bp);					/* BP */
#endif


				GUI_print_string(g_dtp2, 200, 130);
			} else {
				/* novice mode */

				/* calculate AT base value */
				if (hero->rs_be & 1) {
					l1 = -1;
				} else {
					l1 = 0;
				}

				at = (hero->weapon_type < 7) ?
					/* melee weapons */
					(hero->at_talent_bonus[hero->weapon_type] + hero->weapon_at_mod - hero->rs_be / 2) :
					/* range weapons */
					hero->at_talent_bonus[0] - hero->rs_be / 2;
				at += l1;

				/* calculate PA base value */
				pa = (hero->weapon_type < 7) ?
					/* melee weapons */
					(hero->pa_talent_bonus[hero->weapon_type] + hero->weapon_pa_mod - hero->rs_be / 2) :
					/* range weapons */
					 (hero->pa_talent_bonus[0] - hero->rs_be / 2);

				if (at < 0)
					at = 0;
				if (pa < 0)
					pa = 0;
#ifdef M302de_ORIGINAL_BUGFIX
				/* Original-Bugfix: show permanent damage in red */
				set_status_string(get_tx2(52));

				if (hero->le_max_malus) {
					/* print max LE in red if hero has permanent damage */
					sprintf(le_fix, "%c%d%c", 0xf1, hero->le_max, 0xf0);
				} else {
					/* print max LE in black if hero has no permanent damage */
					sprintf(le_fix, "%d", hero->le_max);
				}


				sprintf(g_dtp2, get_tx2(52),
					hero->le, le_fix,			/* LE */
					hero->ae, hero->ae_max,			/* AE */
					at, pa,					/* AT PA */
					hero->mr,				/* MR */
					hero->rs_bonus + hero->rs_bonus_dummy,	/* RS */
					hero->le + hero->attrib[ATTRIB_KK].current +
						hero->attrib[ATTRIB_KK].mod,	/* Ausdauer */
					hero->load,				/* Last */
					bp);					/* BP */

				reset_status_string(get_tx2(52));
				/* Original-Bugfix end */
#else
				sprintf(g_dtp2,	get_tx2(52),
					hero->le, hero->le_max,			/* LE */
					hero->ae, hero->ae_max,			/* AE */
					at, pa,					/* AT PA */
					hero->mr,				/* MR */
					hero->rs_bonus + hero->rs_bonus_dummy,	/* RS */
					hero->le + hero->attrib[ATTRIB_KK].current +
				       		hero->attrib[ATTRIB_KK].mod,	/* Ausdauer */
					hero->load,				/* Last */
					bp);					/* BP */
#endif

				GUI_print_string(g_dtp2, 200, 124);
			}

			break;
		}
		/* AT PA values */
		case 2: {
			g_txt_tabpos[0] = 275;
			g_txt_tabpos[1] = 295;

			/* Fernkampfwaffen-Basiswert: (KL + GE + KK)/4 */
			j = (	hero->attrib[ATTRIB_KL].current +
				hero->attrib[ATTRIB_KL].mod +
				hero->attrib[ATTRIB_GE].current +
				hero->attrib[ATTRIB_GE].mod +
				hero->attrib[ATTRIB_KK].current +
				hero->attrib[ATTRIB_KK].mod) / 4;

			if (hero->rs_be & 1)
				l1 = -1;
			else
				l1 = 0;

			at = (hero->weapon_type < 7) ?
				hero->at_talent_bonus[hero->weapon_type] +	hero->weapon_at_mod - hero->rs_be / 2 :
				hero->at_talent_bonus[0] - hero->rs_be / 2;

			at += l1;

			pa =  (hero->weapon_type < 7) ?
				hero->pa_talent_bonus[hero->weapon_type] +	hero->weapon_pa_mod - hero->rs_be / 2 :
				hero->pa_talent_bonus[0] - hero->rs_be / 2;

			if (at < 0)
				at = 0;

			if (pa < 0)
				pa = 0;

			sprintf(g_dtp2,	get_tx2(5),
				hero->atpa_base,
				get_ttx(48),
				hero->at_talent_bonus[0] - hero->rs_be / 2,
				hero->pa_talent_bonus[0] - hero->rs_be / 2,
				get_ttx(49),

				hero->at_talent_bonus[1] - hero->rs_be / 2,
				hero->pa_talent_bonus[1] - hero->rs_be / 2,
				get_ttx(50),

				hero->at_talent_bonus[2] - hero->rs_be / 2,
				hero->pa_talent_bonus[2] - hero->rs_be / 2,
				get_ttx(51),

				hero->at_talent_bonus[3] - hero->rs_be / 2,
				hero->pa_talent_bonus[3] - hero->rs_be / 2,
				get_ttx(52),

				hero->at_talent_bonus[4] - hero->rs_be / 2,
				hero->pa_talent_bonus[4] - hero->rs_be / 2,
				get_ttx(53),

				hero->at_talent_bonus[5] - hero->rs_be / 2,
				hero->pa_talent_bonus[5] - hero->rs_be / 2,
				get_ttx(54),

				hero->at_talent_bonus[6] - hero->rs_be / 2,
				hero->pa_talent_bonus[6] - hero->rs_be / 2,
				at,
				pa,

				get_ttx(55),
				hero->talents[TA_SCHUSSWAFFEN] + j,

				get_ttx(56),
				hero->talents[TA_WURFWAFFEN] + j);

			GUI_print_string(g_dtp2, 200, 60);
			break;
		}
		/* talents */
		case 3: {
			status_show_talents(hero);
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

				i = g_spellclasses_1[j].first;

				while (g_spellclasses_1[j].first + g_spellclasses_1[j].length > i) {

					status_show_spell(hero, i,
						g_spellclasses_1[j].first,
						g_statuspage_spellclasses_1_xy[j].x_name,
						g_statuspage_spellclasses_1_xy[j].x_val,
						g_statuspage_spellclasses_1_xy[j].y);

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

				i = g_spellclasses_2[j].first;

				while (g_spellclasses_2[j].first + g_spellclasses_2[j].length > i) {

					status_show_spell(hero,
						i,
						g_spellclasses_2[j].first,
						g_statuspage_spellclasses_2_xy[j].x_name,
						g_statuspage_spellclasses_2_xy[j].x_val,
						g_statuspage_spellclasses_2_xy[j].y);
					i++;
				}
			}
			break;
		}
	}

	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 319;
	g_pic_copy.y2 = 199;
	g_pic_copy.src = g_renderbuf_ptr;
	do_pic_copy(0);

	g_update_statusline = 1;

	if (g_status_page_mode >= 3) {
		do_v_line(g_vga_memstart, 107, 54, 195, 0);
		do_v_line(g_vga_memstart, 212, 54, 195, 0);
	}

	g_txt_tabpos[0] = txt_tabpos1_bak;
	g_txt_tabpos[1] = txt_tabpos2_bak;
	g_txt_tabpos[2] = txt_tabpos3_bak;
	g_txt_tabpos[3] = txt_tabpos4_bak;

	g_vga_backbuffer = g_vga_memstart;

	call_mouse();
}
