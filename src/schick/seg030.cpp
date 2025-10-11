/*
 *	Rewrite of DSA1 v3.02_de functions of seg030 (tlk_logic, date_printer)
 *	Functions rewritten: 6/6 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg030.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg028.h"
#include "seg030.h"
#include "seg032.h"
#include "seg054.h"
#include "seg055.h"
#include "seg058.h"
#include "seg066.h"
#include "seg072.h"
#include "seg096.h"
#include "seg097.h"
#include "seg116.h"
#include "seg117.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

char *g_dialog_title;		// ds:0xe308
signed int g_dialog_informer;	// ds:0xe30c
signed int g_dialog_next_state; // ds:0xe30e
signed int g_dialog_done;	// ds:0xe310
signed int g_dialog_state;	// ds:0xe312
signed int g_tlk_id;		// ds:0xe314

#if !defined(__BORLANDC__)
struct struct_hero *gs_random_tlk_hero;	/* REMARK: scope has changed drastically! */
#endif

/* NOTE: here's a inconvenience in start counting from 0 (computer science) and from 1 (math) */

/**
 * \brief  get random known informer
 * \param informer_math number of the informer starting from 1 => (INFORMER_JURGE == 1, ...)
 * \return pointer to the name of an unknown informer
 * */
char* get_random_known_informer_name(const int informer_math)
{
	signed short i;
	signed short counter;

	for (i = counter = 0; i < 15; i++) {

		if ((gs_informer_flags[i] != 0) && (informer_math - 1 != i)
			&& (i != INFORMER_OLVIR) && (i != INFORMER_TREBORN) && (i != INFORMER_UNICORN)) {

			counter++;
		}
	}

	if (counter == 0) {
		return get_random_unknown_informer_name(informer_math);
	} else {

		do {
			i = random_schick(15);
			/* TODO: i is in {1, 15} but {0, 14} would be more readable */

		} while ((i == (INFORMER_OLVIR+1)) || (i == (INFORMER_TREBORN+1)) || (i == (INFORMER_UNICORN+1))
				|| !gs_informer_flags[i - 1] || (i == informer_math));

		return get_ttx(656 + 1 + i);
	}
}

/* get random unknown informer */
char* get_random_unknown_informer_name(const int informer_math)
{
	signed short i;
	signed short counter;

	for (i = counter = 0; i < 15; i++) {

		if (!gs_informer_flags[i] && (informer_math - 1 != i)
			&& (i != INFORMER_OLVIR) && (i != INFORMER_TREBORN) && (i != INFORMER_UNICORN)) {

			counter++;
		}
	}

	if (counter == 0) {
		return get_random_known_informer_name(informer_math);
	} else {

		do {
			i = random_schick(15);
			/* TODO: i is in {1, 15} but {0, 14} would be more readable */

		} while ((i == (INFORMER_OLVIR+1)) || (i == (INFORMER_TREBORN+1)) || (i == (INFORMER_UNICORN+1))
			       || gs_informer_flags[i - 1] || i == informer_math);

		gs_informer_flags[i - 1] = 1;

		return get_ttx(656 + 1 + i);
	}
}

/* unused in the game */
void print_date(void)
{
	signed short tw_bak;

	prepare_date_str();
	tw_bak = g_textbox_width;
	g_textbox_width = 3;
	GUI_input(g_dtp2, 0);
	g_textbox_width = tw_bak;
}

void prepare_date_str(void)
{
	unsigned short unused;
	signed short hour;

	unused = 0;

	hour = (signed short)(gs_day_timer / HOURS(1));

	if (gs_day_of_month < 0) {

		/* Days of the nameless */
		sprintf(g_dtp2, get_ttx(391), get_ttx(349 + gs_day_of_week), gs_year, hour);
	} else {
		/* Normal day */
		sprintf(g_dtp2, get_ttx(356), get_ttx(349 + gs_day_of_week), gs_day_of_month,
			get_ttx(21 + gs_month), get_ttx(551 + get_current_season()), gs_year, hour);
	}

	if (gs_special_day) {

		sprintf(g_text_output_buf, get_ttx(357), get_ttx(357 + gs_special_day));
		strcat(g_dtp2, (const char*)g_text_output_buf);
	}
}

struct tlk_option {
	signed short txt, goto_state;
};

void do_talk(signed short talk_id, signed short tlk_informer)
{
	signed short txt_id;
	signed short txt_offset;
	signed short answer;
	signed short optioncount;
	Bit8u *state_ptr;
	Bit8u *states_tab;
	struct struct_dialog_partner *partners_tab;
	char *dst;
	char *fmt;
	struct struct_hero *hero;
	signed short shufflepair_1;
	signed short shufflepair_2;
	signed short shuffle_count;
	signed short i;
	signed short tmp1;
	signed short tmp2;

	struct tlk_option options[3];

	answer = 0;

	g_dialog_informer = tlk_informer;
	g_tlk_id = talk_id;

	load_informer_tlk(talk_id + ARCHIVE_FILE_DIALOGS_TLK);

	g_dialog_state = g_dialog_done = 0;

	partners_tab = &gs_dialog_partners[0];
	states_tab = (Bit8u*)partners_tab[tlk_informer].states_offset;
	txt_offset = partners_tab[tlk_informer].txt_offset;
	g_dialog_title = (char*)partners_tab[tlk_informer].title;

	load_in_head(partners_tab[tlk_informer].head_id);
	dst = (char*)(g_dtp2 + 0x400);

	do {
		answer = optioncount = 0;
		state_ptr = 8 * g_dialog_state + states_tab;

		if (host_readb(state_ptr + 2)) optioncount++;
		if (host_readb(state_ptr + 3)) optioncount++;
		if (host_readb(state_ptr + 4)) optioncount++;

		if (host_readws(state_ptr) != -1) {

			txt_id = host_readws(state_ptr) & 0x7fff;

			fmt = get_tx2(txt_id + txt_offset);

			if (g_tlk_id == 11) {
				if (g_dialog_informer == 2) {

					if (txt_id == 12 || txt_id == 16 || txt_id == 17 ||
						txt_id == 18 || txt_id == 20 || txt_id == 21 ||
						txt_id == 22 || txt_id == 25 || txt_id == 26 ||
						txt_id == 27 || txt_id == 28)
					{
						sprintf(dst, fmt,
							((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
							GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 0),
							GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 1));

					} else if (txt_id == 19) {

						sprintf(dst, fmt,
							((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
							((struct struct_hero*)gs_unicorn_hero_ptr)->alias);

					} else if (txt_id == 23) {

						sprintf(dst, fmt,
							((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
							(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 2)));
					} else if (txt_id == 29) {

						sprintf(dst, fmt,
							((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
							(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 0)),
							(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 1)),
							(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 2)));

					} else if (txt_id == 30) {

						sprintf(dst, fmt,
							((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
							(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 1)),
							(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 2)));

					} else if (txt_id == 31) {

						sprintf(dst, fmt,
							(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 3)));

					} else if (txt_id == 32) {

						sprintf(dst, fmt,
							((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
							(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 0)),
							((struct struct_hero*)gs_unicorn_hero_ptr)->alias);

					} else {
						strcpy(dst, fmt);
					}
				} else {
					strcpy(dst, fmt);
				}
			} else {

				if (g_tlk_id == 9 && g_dialog_informer == 1) {

					if (txt_id == 21) {

						sprintf(dst, fmt, get_ttx(658));

					} else if (txt_id == 22) {

						sprintf(dst, fmt, get_ttx(662));

					} else if (txt_id == 23) {

						sprintf(dst, fmt, get_ttx(661));

					} else if (txt_id == 29) {

						hero = get_hero(gs_tiomar_drinkmate);

						sprintf(dst, fmt, hero->alias, GUI_get_ptr(hero->sex, 0));

					} else {

						strcpy(dst, fmt);

					}
				} else if (g_tlk_id == 7 && g_dialog_informer == 2) {

					if (txt_id == 19) {

						sprintf(dst, fmt, get_random_known_informer_name(INFORMER_ISLEIF+1));

					} else if (txt_id == 20) {

						sprintf(dst, fmt, get_random_known_informer_name(INFORMER_ISLEIF+1));

					} else {

						strcpy(dst, fmt);

					}

				} else if (g_tlk_id == 6 && g_dialog_informer == 0) {

					if (txt_id == 35 || txt_id == 36) {

						sprintf(dst, fmt, get_random_unknown_informer_name(INFORMER_JURGE+1));

					} else if (txt_id == 34) {

						sprintf(dst, fmt, get_random_known_informer_name(INFORMER_JURGE+1));

					} else {

						strcpy(dst, fmt);
					}

				} else if (g_tlk_id == 12) {

					if (g_dialog_informer == 0) {

						hero = get_first_hero_available_in_group();

						if (!txt_id || txt_id == 18) {

							sprintf(dst, fmt, hero->alias);

						} else if (txt_id == 28) {

							sprintf(dst, fmt, get_ttx(663));

						} else if (txt_id == 29) {

							sprintf(dst, fmt, get_ttx(664));

						} else if (txt_id == 30) {

							sprintf(dst, fmt, get_ttx(672));

						} else if (txt_id == 38) {

							sprintf(dst, fmt, get_ttx(235 + gs_swafnild_tp4));

						} else if (txt_id == 49) {

							sprintf(dst, fmt, get_ttx(235 + gs_swafnild_tp4));

							gs_current_town = gs_swafnild_tp4;

							gs_x_target_bak = (gs_swafnild_tp4 == 35 ? 10 : (gs_swafnild_tp4 == 32 ? 2 : 7));
							gs_y_target_bak = (gs_swafnild_tp4 == 35 ? 2 : (gs_swafnild_tp4 == 32 ? 14 : 3));

						} else if (txt_id == 52) {

							sprintf(dst, fmt,
								get_ttx(235 + gs_swafnild_tp1),
								get_ttx(235 + gs_swafnild_tp2),
								get_ttx(235 + gs_swafnild_tp3));

						} else if (txt_id == 59) {

							sprintf(dst, fmt, get_ttx(235 + (
							    gs_swafnild_destination == 1 ? gs_swafnild_tp1 :
							    (gs_swafnild_destination == 2 ? gs_swafnild_tp2 : gs_swafnild_tp3))));

						} else {
							strcpy(dst, fmt);
						}

					} else if (g_dialog_informer == 1) {

						if (!txt_id || txt_id == 3 || txt_id == 4) {

							sprintf(dst, fmt,
								((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
								(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 0)));

						} else if (txt_id == 5) {

							sprintf(dst, fmt,
								(GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 0)),
								((struct struct_hero*)gs_unicorn_hero_ptr)->alias);

						} else if (txt_id == 6) {

							sprintf(dst, fmt,
								((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
								GUI_get_ptr(((struct struct_hero*)gs_unicorn_hero_ptr)->sex, 1),
								((struct struct_hero*)gs_unicorn_hero_ptr)->alias);

						} else if (txt_id == 8) {

							sprintf(dst, fmt,
								((struct struct_hero*)gs_unicorn_hero_ptr)->alias,
								((struct struct_hero*)gs_unicorn_hero_ptr)->alias);

						} else {
							strcpy(dst, fmt);
						}

					}
				} else if (g_tlk_id == 10 && g_dialog_informer == 0) {

					if (txt_id == 18) {

						sprintf(dst, fmt, get_random_unknown_informer_name(INFORMER_OLVIR+1));

					} else if (txt_id == 29) {

						sprintf(dst, fmt, get_random_known_informer_name(INFORMER_OLVIR+1));

					} else {

						strcpy(dst, fmt);
					}

				} else if (g_tlk_id == 17) {


					hero = get_first_hero_available_in_group();

					if (!txt_id || txt_id == 2 || txt_id == 25 || txt_id == 31 ||
						txt_id == 32 || txt_id == 60 || txt_id == 78 || txt_id == 87)
					{
						sprintf(dst, fmt, hero->alias);

					} else if (txt_id == 13 || txt_id == 19 || txt_id == 88 || txt_id == 24) {

						sprintf(dst, fmt, hero->alias,
							GUI_get_ptr(hero->sex, 0),
							GUI_get_ptr(hero->sex, 0));

					} else if (txt_id == 14 || txt_id == 15 || txt_id == 76) {

						sprintf(dst, fmt, gs_ruin_hero->alias,
							GUI_get_ptr(gs_ruin_hero->sex, 0));

					} else if (txt_id == 26 || txt_id == 65) {

						sprintf(dst, fmt, hero->alias,
							GUI_get_ptr(hero->sex, 0),
							GUI_get_ptr(hero->sex, 2),
							GUI_get_ptr(hero->sex, 1));

					} else {

						strcpy(dst, fmt);

					}

				} else if (g_tlk_id == 0) {

					if (txt_id == 40 || txt_id == 41 || txt_id == 43) {
#if !defined(__BORLANDC__)
						gs_random_tlk_hero = get_hero(get_random_hero());
#endif

						sprintf(dst, fmt, gs_random_tlk_hero->alias,
							GUI_get_ptr(gs_random_tlk_hero->sex, 0),
							GUI_get_ptr(gs_random_tlk_hero->sex, 2));
					} else {

						strcpy(dst, fmt);
					}
				} else {
					strcpy(dst, fmt);
				}
			}

			options[0].txt = host_readb(state_ptr + 2) + txt_offset;
			options[0].goto_state = host_readb(state_ptr + 5);
			options[1].txt = host_readb(state_ptr + 3) + txt_offset;
			options[1].goto_state = host_readb(state_ptr + 6);
			options[2].txt = host_readb(state_ptr + 4) + txt_offset;
			options[2].goto_state = host_readb(state_ptr + 7);
			shuffle_count = random_schick(5);

			/* shuffle options by pairwise interchanging */
			for (i = 0; i < shuffle_count; i++) {
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

			answer = GUI_dialogbox((unsigned char*)g_dtp2, g_dialog_title, (char*)dst, optioncount,
					get_tx2(host_readb(state_ptr + 2) + txt_offset),
					get_tx2(host_readb(state_ptr + 3) + txt_offset),
					get_tx2(host_readb(state_ptr + 4) + txt_offset));
		}


		g_dialog_next_state = -1;

		if (host_readws(state_ptr) & 0x8000 || host_readws(state_ptr) == -1) {
			talk_switch();
		}

		g_dialog_state = (g_dialog_next_state == -1 ? host_readb(state_ptr + 5) : g_dialog_next_state);

		if (g_dialog_done == 0) {

			/* set the new dialog state */

			if (optioncount != 0 ) {
				if (answer == -1) {
					g_dialog_done = 1;
				} else if (answer == 1) {
					g_dialog_state = (host_readb(state_ptr + 5));
				} else if (answer == 2) {
					g_dialog_state = (host_readb(state_ptr + 6));
				} else if (answer == 3) {
					g_dialog_state = (host_readb(state_ptr + 7));
				}
			}

			if (g_dialog_state == 255) {
				g_dialog_done = 1;
			}
		}

	} while (g_dialog_done == 0);

	g_text_file_index = -1;
	g_current_ani = -1;
}

/* Borlandified and identical */
void talk_switch(void)
{
	signed short state;

	state = g_dialog_state;

	if (g_tlk_id == 3) {
		/* DASPOTA1.TLK */

		if (g_dialog_informer == 0) {
			if ((state == 1 || state == 2 || state == 3) && !gs_daspota_fightflags[1]) {

				g_fig_discard = 1;

				if (!do_fight(FIGHTS_DASP1A)) {
					if (GUI_bool(get_tx(22))) {

						g_fig_discard = 0;

						if (!do_fight(FIGHTS_DASP1B)) {
							gs_daspota_fightflags[1] = 1;
						}
					}
				}

				g_fig_discard = 0;
			}
		}

		if (g_dialog_informer == 1) {

			if (state == 1 && !gs_daspota_fightflags[2]) {

				if (!do_fight(FIGHTS_DASP2)) {
					gs_daspota_fightflags[2] = 1;
				}
			}
		}

		if (g_dialog_informer == 2) {

			if ((state == 4 || state == 6) && !gs_daspota_fightflags[3]) {

				if (!do_fight(FIGHTS_DASP3)) {
					gs_daspota_fightflags[3] = 1;
				}
			}
		}

		if (g_dialog_informer == 3) {

			if (state == 4 && !gs_daspota_fightflags[4]) {

				if (!do_fight(FIGHTS_DASP4)) {
					gs_daspota_fightflags[4] = 1;
				}
			}
		}

		if (g_dialog_informer == 4) {

			if ((state == 1 || state == 4 || state == 7) && !gs_daspota_fightflags[5]) {

				if (!do_fight(FIGHTS_DASP5)) {
					gs_daspota_fightflags[5] = 1;
				}
			}
		}

		if (g_dialog_informer == 5) {

			if ((state == 4 || state == 5) && !gs_daspota_fightflags[6]) {

				if (!do_fight(FIGHTS_DASP6A)) {
					gs_daspota_fightflags[6] = 1;
				}
			}
		}

		if (g_dialog_informer == 6) {

			if ((state == 1 || state == 4 || state == 7) && !gs_daspota_fightflags[7]) {

				if (!do_fight(FIGHTS_DASP7)) {
					gs_daspota_fightflags[7] = 1;
				}
			}
		}

	} else if (g_tlk_id == 4) {
		/* DASPOTA2.TLK */

		if (g_dialog_informer == 0) {

			if (state == 4 && !gs_daspota_fightflags[8]) {

				if (!do_fight(FIGHTS_DASP8)) {
					gs_daspota_fightflags[8] = 1;
				}
			}
		}

		if (g_dialog_informer == 1) {

			if (state == 1 && !gs_daspota_fightflags[9]) {

				if (!do_fight(FIGHTS_DASP9)) {
					gs_daspota_fightflags[9] = 1;
				}
			}
		}

		if (g_dialog_informer == 2) {

			if (state == 1 && !gs_daspota_fightflags[10]) {

				if (!do_fight(FIGHTS_DASP10)) {
					gs_daspota_fightflags[10] = 1;
				}
			}
		}

		if (g_dialog_informer == 3) {

			if (state == 4 && !gs_daspota_fightflags[11]) {

				if (!do_fight(FIGHTS_DASP11)) {
					gs_daspota_fightflags[11] = 1;
				}
			}
		}

		if (g_dialog_informer == 4) {

			if (state == 1 && !gs_daspota_fightflags[12]) {

				if (!do_fight(FIGHTS_DASP12A)) {
					gs_daspota_fightflags[12] = 1;
				}
			}
		}

		if (g_dialog_informer == 5) {

			if (state == 5 && !gs_daspota_fightflags[13]) {

				if (!do_fight(FIGHTS_DASP13)) {
					gs_daspota_fightflags[13] = 1;
				}
			}
		}

		if (g_dialog_informer == 6) {

			if (state == 1 && !gs_daspota_fightflags[14]) {

				if (!do_fight(FIGHTS_DASP14)) {
					gs_daspota_fightflags[14] = 1;
				}
			}
		}

	} else if (g_tlk_id == 5) {
		/* DASPOTA3.TLK */

		if (g_dialog_informer == 0) {

			if ((state == 4 || state == 6) && !gs_daspota_fightflags[16]) {

				if (!do_fight(FIGHTS_DASP16)) {
					gs_daspota_fightflags[16] = 1;
				}
			}
		}

		if (g_dialog_informer == 1) {

			if ((state == 4 || state == 5 || state == 6) && !gs_daspota_fightflags[17]) {

				if (!do_fight(FIGHTS_DASP17)) {
					gs_daspota_fightflags[17] = 1;
				}
			}
		}

		if (g_dialog_informer == 2) {

			if ((state == 6 || state == 7) && !gs_daspota_fightflags[18]) {

				if (!do_fight(FIGHTS_DASP18)) {
					gs_daspota_fightflags[18] = 1;
				}
			}
		}

	} else if (g_tlk_id == 6) {
		/* INFO1.TLK */
		INF_jurge_hjore(g_dialog_informer, state);

	} else if (g_tlk_id == 7) {
		/* INFO2.TLK */
		INF_yasma_umbrik_isleif(g_dialog_informer, state);

	} else if (g_tlk_id == 8) {
		/* INFO3.TLK */
		INF_ragna_beorn_algrid(g_dialog_informer, state);

	} else if (g_tlk_id == 9) {
		/* INFO4.TLK */
		INF_eliane_tiomar(g_dialog_informer, state);

	} else if (g_tlk_id == 10) {
		/* INFO5.TLK */
		INF_olvir_asgrimm(g_dialog_informer, state);

	} else if (g_tlk_id == 11) {
		/* INFO6.TLK */
		INF_treborn_unicorn(g_dialog_informer, state);

	} else if (g_tlk_id == 12) {
		/* INFO7.TLK */
		INF_swafnild_unicorn(g_dialog_informer, state);

	} else if (g_tlk_id == 13) {
		/* SCHMIED.TLK */
		TLK_schmied(state);

	} else if (g_tlk_id == 14) {
		/* GHANDEL.TLK */
		TLK_ghandel(state);

	} else if (g_tlk_id == 15) {
		/* KHANDEL.TLK */
		TLK_khandel(state);

	} else if (g_tlk_id == 16) {
		/* WHANDEL.TLK */
		TLK_whandel(state);

	} else if (g_tlk_id == 1) {
		/* HERBERG.TLK */
		TLK_herberg(state);

	} else if (g_tlk_id == 0) {
		/* F092.TLK */
		TLK_old_woman(state);

	} else if (g_tlk_id == 17) {
		/* F115.TLK */
		TLK_way_to_ruin(state);

	} else if (g_tlk_id == 18) {
		/* EREMIT.TLK */
		TLK_eremit(state);
	}
}

#if !defined(__BORLANDC__)
}
#endif
