/*
 *	Rewrite of DSA1 v3.02_de functions of seg031 (tavern helpers)
 *	Functions rewritten: 10/10 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg031.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg007.h"
#include "seg026.h"
#include "seg028.h"
#include "seg030.h"
#include "seg031.h"
#include "seg067.h"
#include "seg073.h"
#include "seg097.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct tlk_option {
	signed short txt, goto_state;
};

static struct struct_informer_tab g_informer_tab[15] = {
	{ 0x02bc, 0x26, 0x00 },
	{ 0x02bd, 0x25, 0x00 },
	{ 0x02be, 0x19, 0x01 },
	{ 0x02bf, 0x1c, 0x00 },
	{ 0x02c0, 0x14, 0x00 },
	{ 0x02c1, 0x2e, 0x01 },
	{ 0x02c2, 0x0d, 0x00 },
	{ 0x02c3, 0x03, 0x00 },
	{ 0x02c4, 0x2b, 0x01 },
	{ 0x02c5, -0x01, 0x00 },
	{ 0x02c6, -0x01, 0x01 },
	{ 0x02c7, -0x01, 0x00 },
	{ 0x02c8, -0x01, 0x00 },
	{ 0x02c9, 0x1f, 0x01 },
	{ 0x02ca, 0x30, 0x00 }
}; // ds:0x5ed6


void do_random_talk(signed short talk_id, signed short informer_id)
{
	signed short optioncount;
	signed short answer = 0;
	signed short txt_id;
	signed short txt_offset;
	signed short opt0_txt;
	signed short opt1_txt;
	signed short opt2_txt;
	signed short txt_id_raw;
	signed short txt_id_rand;
	signed short opt0_rand;
	signed short opt1_rand;
	signed short opt2_rand;
	struct struct_dialog_state *state_ptr;
	struct struct_dialog_state *states_tab;
	struct struct_dialog_partner *partners_tab;
	char *dialog_title;
	char *dst;
	char *fmt;
	signed short shufflepair_1;
	signed short shufflepair_2;
	signed short shufflecount;
	signed short i;
	signed short tmp1;
	signed short tmp2;
	struct tlk_option options[3];

	g_dialog_informer = informer_id;
	g_tlk_id = talk_id;

	load_tlk(talk_id + ARCHIVE_FILE_DIALOGS_TLK);
	g_dialog_state = g_dialog_done = 0;
	partners_tab = &gs_dialog_partners[0];
	states_tab = (struct struct_dialog_state*)partners_tab[informer_id].states_offset;
	txt_offset = partners_tab[informer_id].txt_offset;
	dialog_title = partners_tab[informer_id].title;
	load_in_head(partners_tab[informer_id].head_id);
	dst = (char*)g_dtp2 + 0x400;

	do {
		answer = optioncount = 0;
		state_ptr = &states_tab[g_dialog_state];

		if (g_tlk_id == 13 && g_dialog_state >= 20) {
			txt_id_rand = opt0_rand = opt1_rand = opt2_rand = 0;
		} else {
			txt_id_rand = random_schick(4) - 1;
			opt0_rand = random_schick(4) - 1;
			opt1_rand = random_schick(4) - 1;
			opt2_rand = random_schick(4) - 1;
		}

		if ((txt_id_raw = state_ptr->txt_id) != -1) {
			txt_id_raw = 4 * txt_id_raw + txt_id_rand;
		}
		if (state_ptr->txt_id1) {
			opt0_txt = 4 * state_ptr->txt_id1 + opt0_rand;
			optioncount++;
		}
		if (state_ptr->txt_id2) {
			opt1_txt = 4 * state_ptr->txt_id2 + opt1_rand;
			optioncount++;
		}
		if (state_ptr->txt_id3) {
			opt2_txt = 4 * state_ptr->txt_id3 + opt2_rand;
			optioncount++;
		}

		if (txt_id_raw != -1) {

			txt_id = (4 * state_ptr->txt_id + txt_id_rand) & 0x7fff;
			fmt = get_tx(txt_id + txt_offset);

			if (g_tlk_id == 15) {

				if (g_dialog_state == 13) {
					sprintf(dst, fmt, (char*)waffinfo_herbs());
				} else {
					strcpy(dst, fmt);
				}

			} else if (g_tlk_id == 14) {

				if (g_dialog_state == 11) {
					sprintf(dst, fmt, (char*)waffinfo_general());
				} else {
					strcpy(dst, fmt);
				}

			} else if (g_tlk_id == 16) {

				if (g_dialog_state == 19 || g_dialog_state == 23) {
					sprintf(dst, fmt, (char*)waffinfo_weapons());
				} else {
					strcpy(dst, fmt);
				}

			} else if (g_tlk_id == 1) {

				if (g_dialog_state == 16) {
					sprintf(dst, fmt, load_current_town_gossip());
				} else {
					strcpy(dst, fmt);
				}

			} else {
				strcpy(dst, fmt);
			}

			options[0].txt = opt0_txt + txt_offset;
			options[0].goto_state = state_ptr->state1;
			options[1].txt = opt1_txt + txt_offset;
			options[1].goto_state = state_ptr->state2;
			options[2].txt = opt2_txt + txt_offset;
			options[2].goto_state = state_ptr->state3;

			if (optioncount) {

				shufflecount = random_schick(5);
				for (i = 0; i < shufflecount; i++) {

					shufflepair_1 = random_schick(3) - 1;
					shufflepair_2 = random_schick(3) - 1;

					tmp1 = options[shufflepair_1].txt;
					tmp2 = options[shufflepair_2].txt;

					if (tmp1 != txt_offset && tmp2 != txt_offset) {
						options[shufflepair_1].txt = tmp2;
						options[shufflepair_2].txt = tmp1;
						tmp1 = options[shufflepair_1].goto_state;
						options[shufflepair_1].goto_state = options[shufflepair_2].goto_state;
						options[shufflepair_2].goto_state = tmp1;
					}
				}
			}

			answer = GUI_dialogbox((unsigned char*)g_dtp2, dialog_title, dst, optioncount,
					get_tx(options[0].txt),
					get_tx(options[1].txt),
					get_tx(options[2].txt));

		} else {
			options[0].goto_state = state_ptr->state1;
		}

		g_dialog_next_state = -1;
		if ((state_ptr->txt_id & 0x8000) || (state_ptr->txt_id == -1)) {
			talk_switch();
		}

		g_dialog_state = (g_dialog_next_state == -1 ? options[0].goto_state : g_dialog_next_state);

		if (g_dialog_done == 0) {

			if (optioncount) {

				if (answer == -1) {
					g_dialog_done = 1;
				} else if (answer == 1) {
					g_dialog_state = options[0].goto_state;
				} else if (answer == 2) {
					g_dialog_state = options[1].goto_state;
				} else if (answer == 3) {
					g_dialog_state = options[2].goto_state;
				}
			}

			if (g_dialog_state == 255) {
				g_dialog_done = 1;
			}
		}

	} while (g_dialog_done == 0);

	g_text_file_index = g_current_ani = -1;
	load_tx(g_tx_file_index);
}

/* This function is dead code */
char* get_informer_forename(void)
{
	signed short i;
	char tmp;
	struct struct_informer_tab *p_info;
	char *informer_name;

	p_info = &g_informer_tab[0];

	for (i = 0; i < 15; i++, p_info += 1) {

		if (p_info->town == gs_current_town) {

			i = 0;
			informer_name = get_ttx(p_info->name_id);

			do {
				tmp = *informer_name;
				informer_name++;
				i++;	// TODO: setting i to 0 in a for loop is not a good idea
			} while (tmp != ' ');

			strncpy(g_text_output_buf, get_ttx(p_info->name_id), i);
#ifdef M302de_ORIGINAL_BUGFIX
			break;
#endif
		}
	}

	return g_text_output_buf;
}

/**
 * \brief   ?
 *
 * \return              a value between 0 and 15
 */
signed short get_town_lookup_entry(void)
{
	struct struct_informer_tab *p_info;
	signed short i;

	p_info = &g_informer_tab[0];
	for (i = 0; i < 15; i++, p_info++) {
		if (p_info->town == gs_current_town) {
			return i;
		}
	}

	return 15;
}

/**
 * \brief   gives a hint where a informer lives
 *
 *          Game Info: You can ask in some towns where informers live.
 *          This function returns a pointer to the answer or an empty string.
 *
 * \return              a pointer to the string.
 */
char* get_informer_hint(void)
{
	signed short i;
	struct struct_informer_tab *p_info;

	p_info = &g_informer_tab[0];
	for (i = 0; i < 15; i++, p_info++) {
		if (p_info->town == gs_current_town) {
			return get_ttx(i + 715);
		}
	}

	return get_ttx(725);
}

/**
 * \brief   get the name on the current informer
 *
 * \return  a pointer to the name of the informer
 */
char* get_informer_name(void)
{
	return get_ttx(g_informer_tab[gs_current_informer - 1].name_id);
}

/**
 * \brief   get the name of the informer in this town
 *
 * \return a pointer to the name of the informer
 */
char* get_informer_name2(void)
{
	return get_ttx(g_informer_tab[get_town_lookup_entry()].name_id);
}

/**
 * \brief   loads a gossip message from the current town
 *
 * \return              a pointer to the message.
 */
char* load_current_town_gossip(void)
{
	signed int gossip_id;
	char **ptr;

	/* load TOWN.LTX */
	load_ltx(gs_current_town + ARCHIVE_FILE_CITY_LTX);

	/* mark some buffers invalid */
	g_area_prepared = g_current_ani = -1;

	/* get the pointer to the ltx buffer */
	ptr = (char**)g_buffer9_ptr3;

	/* get some gossip */
	gossip_id = get_tavern_gossip();

	/* return the pointer to the gossip (pointers are stored in the first 1000 bytes) */
	return ptr[gossip_id];
}

char* get_random_tavern_message(void)
{
	const signed short randval = random_schick(20) - 1;
	char *ptr = get_tx(randval + 147);

	if (!randval || randval == 19) {

		sprintf(g_text_output_buf, ptr, load_current_town_gossip());

		return g_text_output_buf;

	} else if (randval == 3) {

		sprintf(g_text_output_buf, ptr, get_ttx(gs_current_town + 235));

		return g_text_output_buf;

	} else {
		return ptr;
	}
}

/* 0x70b */
/**
 * \brief   drink while sitting in a tavern
 *
 * \param   amount      how much amount fluid you get
 *
 * \todo    This function is only called while sitting in a tavern.
 *          When you enter a tavern and order food another function is called.
 *          Also this function is called in taverns with amount = 10,
 *          and in the thorwalian dungeon with 100.
 */
void drink_while_drinking(signed short amount)
{
	struct struct_hero *hero;
	signed short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && !hero->flags.dead) {

			/* sub fluid amount */
			hero->thirst = hero->thirst - amount;

			/* adjust food counter */
			if (hero->thirst < 0) {
				hero->thirst = 0;
			}
		}
	}
}

/**
 * \brief   eat food while sitting in a tavern
 *
 * \param   amount      how much food you get
 *
 * \todo    This function is only called while sitting in a tavern.
 *          When you enter a tavern and order food another function is called.
 *          Also this function is called only at one play with amount = 100,
 *          so there is space for tuning.
 */
void eat_while_drinking(signed short amount)
{
	struct struct_hero *hero;
	signed short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && !hero->flags.dead) {

			/* sub food amount */
			hero->hunger = hero->hunger - amount;

			/* adjust food counter */
			if (hero->hunger < 0) {
				hero->hunger = 0;
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif
