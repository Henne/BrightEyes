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

/* get random known informer */
RealPt seg030_0000(signed short arg0)
{
	signed short i;
	signed short counter;

	for (i = counter = 0; i < 15; i++) {
		if ((ds_readb(INFORMER_FLAGS + i) != 0)
			&& (arg0 - 1 != i)
			&& (i != 9)
			&& (i != 11)
			&& (i != 12)) {

			counter++;
		}
	}

	if (counter == 0) {
		return seg030_008d(arg0);
	} else {

		do {
			i = random_schick(15);
		} while (i == 10 || i == 12 || i == 13 || !ds_readb((INFORMER_FLAGS - 1) + i)|| i == arg0);

		return (RealPt)host_readd((Bit8u*)((Bit8u*)ds_readd(TEXT_LTX_INDEX) + (0x291 + i) * 4));
	}
}

/* get random unknown informer */
RealPt seg030_008d(signed short arg0)
{
	signed short i;
	signed short v2;

	for (i = v2 = 0; i < 15; i++) {
		if (!(ds_readb(INFORMER_FLAGS + i))
			&& (arg0 - 1 != i)
			&& (i != 9)
			&& (i != 11)
			&& (i != 12)) {

			v2++;
		}
	}

	if (v2 == 0) {
		return seg030_0000(arg0);
	} else {

		do {
			i = random_schick(15);
		} while (i == 10 || i == 12 || i == 13 || ds_readb((INFORMER_FLAGS - 1) + i)|| i == arg0);
		ds_writeb((INFORMER_FLAGS - 1) + i, 1);

		return (RealPt)host_readd((Bit8u*)((Bit8u*)ds_readd(TEXT_LTX_INDEX) + (0x291 + i) * 4));
	}
}

/* unused in the game */
void print_date(void)
{
	unsigned short textbox_width_bak;

	prepare_date_str();
	textbox_width_bak = ds_readw(TEXTBOX_WIDTH);
	ds_writew(TEXTBOX_WIDTH, 3);

	GUI_input((char*)ds_readd(DTP2), 0);
	ds_writew(TEXTBOX_WIDTH, textbox_width_bak);
}

void prepare_date_str(void)
{
	unsigned short unused;
	signed short hour;

	unused = 0;

	hour = (signed short)(ds_readd(DAY_TIMER) / HOURS(1));

	if (ds_readbs(DAY_OF_MONTH) < 0) {
		/* Days of the nameless */
		sprintf((char*)ds_readd(DTP2),
			get_ttx(391),
			get_ttx(349 + ds_readbs(DAY_OF_WEEK)),
			ds_readbs(YEAR), hour);
	} else {
		/* Normal day */
		sprintf((char*)ds_readd(DTP2),
			get_ttx(356),
			get_ttx(349 + ds_readbs(DAY_OF_WEEK)),
			ds_readbs(DAY_OF_MONTH),
			get_ttx(21 + ds_readbs(MONTH)),
			get_ttx(551 + get_current_season()),
			ds_readbs(YEAR), hour);
	}

	if (ds_readbs(SPECIAL_DAY) != 0) {
		sprintf((char*)ds_readd(TEXT_OUTPUT_BUF),
			get_ttx(357),
			get_ttx(357 + ds_readbs(SPECIAL_DAY)));

		strcat((char*)ds_readd(DTP2),
			(char*)ds_readd(TEXT_OUTPUT_BUF));
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
	RealPt partners_tab;
	char *dst;
	char *fmt;
	Bit8u *hero;
	signed short shufflepair_1;
	signed short shufflepair_2;
	signed short shuffle_count;
	signed short i;
	signed short tmp1;
	signed short tmp2;

	struct tlk_option options[3];

	answer = 0;

	ds_writews(DIALOG_INFORMER, tlk_informer);
	ds_writews(TLK_ID, talk_id);

	load_informer_tlk(talk_id + ARCHIVE_FILE_DIALOGS_TLK);

	ds_writews(DIALOG_STATE, ds_writews(DIALOG_DONE, 0));

	partners_tab = ((Bit8u*)p_datseg + DIALOG_PARTNERS);
	states_tab = (Bit8u*)(host_readd((Bit8u*)(partners_tab) + 38 * tlk_informer));
	txt_offset = host_readws((Bit8u*)(partners_tab) + 38 * tlk_informer + 4);
	ds_writed(DIALOG_TITLE, (Bit32u)(tlk_informer * 38 + partners_tab + 6));

	load_in_head(host_readws((Bit8u*)(partners_tab) + 38 * tlk_informer + 0x24));
	dst = (char*)ds_readd(DTP2) + 0x400;

	do {
		answer = optioncount = 0;
		state_ptr = 8 * ds_readws(DIALOG_STATE) + states_tab;

		if (host_readbs(state_ptr + 2) != 0) optioncount++;
		if (host_readbs(state_ptr + 3) != 0) optioncount++;
		if (host_readbs(state_ptr + 4) != 0) optioncount++;

		if (host_readws(state_ptr) != -1) {

			txt_id = host_readws(state_ptr) & 0x7fff;

			fmt = get_tx2(txt_id + txt_offset);

			if (ds_readws(TLK_ID) == 11) {
				if (ds_readws(DIALOG_INFORMER) == 2) {

					if (txt_id == 12 || txt_id == 16 || txt_id == 17 ||
						txt_id == 18 || txt_id == 20 || txt_id == 21 ||
						txt_id == 22 || txt_id == 25 || txt_id == 26 ||
						txt_id == 27 || txt_id == 28)
					{
						sprintf(dst, fmt,
							(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 0)),
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 1)));

					} else if (txt_id == 19) {

						sprintf(dst, fmt,
							(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
							(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2);

					} else if (txt_id == 23) {

						sprintf(dst, fmt,
							(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 2)));
					} else if (txt_id == 29) {

						sprintf(dst, fmt,
							(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 0)),
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 1)),
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 2)));

					} else if (txt_id == 30) {

						sprintf(dst, fmt,
							(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 1)),
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 2)));

					} else if (txt_id == 31) {

						sprintf(dst, fmt,
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 3)));

					} else if (txt_id == 32) {

						sprintf(dst, fmt,
							(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
							(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 0)),
							(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2);

					} else {
						strcpy(dst, fmt);
					}
				} else {
					strcpy(dst, fmt);
				}
			} else {

				if (ds_readws(TLK_ID) == 9 && ds_readws(DIALOG_INFORMER) == 1) {

					if (txt_id == 21) {

						sprintf(dst, fmt, get_ttx(658));

					} else if (txt_id == 22) {

						sprintf(dst, fmt, get_ttx(662));

					} else if (txt_id == 23) {

						sprintf(dst, fmt, get_ttx(661));

					} else if (txt_id == 29) {

						hero = get_hero(ds_readb(TIOMAR_DRINKMATE));

						sprintf(dst, fmt,
							(char*)hero + HERO_NAME2,
							(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					} else {

						strcpy(dst, fmt);

					}
				} else if (ds_readws(TLK_ID) == 7 && ds_readws(DIALOG_INFORMER) == 2) {

					if (txt_id == 19) {

						sprintf(dst, fmt, (Bit8u*)(seg030_0000(5)));

					} else if (txt_id == 20) {

						sprintf(dst, fmt, (Bit8u*)(seg030_008d(5)));

					} else {

						strcpy(dst, fmt);

					}

				} else if (ds_readws(TLK_ID) == 6 && ds_readws(DIALOG_INFORMER) == 0) {

					if (txt_id == 35 || txt_id == 36) {

						sprintf(dst, fmt, (Bit8u*)(seg030_008d(1)));

					} else if (txt_id == 34) {

						sprintf(dst, fmt, (Bit8u*)(seg030_0000(1)));

					} else {

						strcpy(dst, fmt);
					}

				} else if (ds_readws(TLK_ID) == 12) {

					if (ds_readws(DIALOG_INFORMER) == 0) {

						hero = (Bit8u*)get_first_hero_available_in_group();

						if (!txt_id || txt_id == 18) {

							sprintf(dst, fmt, (char*)hero + HERO_NAME2);

						} else if (txt_id == 28) {

							sprintf(dst, fmt, get_ttx(663));

						} else if (txt_id == 29) {

							sprintf(dst, fmt, get_ttx(664));

						} else if (txt_id == 30) {

							sprintf(dst, fmt, get_ttx(672));

						} else if (txt_id == 38) {

							sprintf(dst, fmt, get_ttx(235 + ds_readb(SWAFNILD_TP4)));

						} else if (txt_id == 49) {

							sprintf(dst, fmt, get_ttx(235 + ds_readb(SWAFNILD_TP4)));

							ds_writebs(CURRENT_TOWN, ds_readbs(SWAFNILD_TP4));

							ds_writews(X_TARGET_BAK, ds_readbs(SWAFNILD_TP4) == 35 ? 10 : (ds_readbs(SWAFNILD_TP4) == 32 ? 2 : 7));
							ds_writews(Y_TARGET_BAK, ds_readbs(SWAFNILD_TP4) == 35 ? 2 : (ds_readbs(SWAFNILD_TP4) == 32 ? 14 : 3));

						} else if (txt_id == 52) {

							sprintf(dst, fmt,
								get_ttx(235 + ds_readb(SWAFNILD_TP1)),
								get_ttx(235 + ds_readb(SWAFNILD_TP2)),
								get_ttx(235 + ds_readb(SWAFNILD_TP3)));

						} else if (txt_id == 59) {

							sprintf(dst, fmt, get_ttx(235 + (
							    ds_readb(SWAFNILD_DESTINATION) == 1 ?
                                    ds_readb(SWAFNILD_TP1)
                                : (ds_readb(SWAFNILD_DESTINATION) == 2 ?
                                    ds_readb(SWAFNILD_TP2)
                                : ds_readb(SWAFNILD_TP3)))));

						} else {
							strcpy(dst, fmt);
						}

					} else if (ds_readws(DIALOG_INFORMER) == 1) {

						if (!txt_id || txt_id == 3 || txt_id == 4) {

							sprintf(dst, fmt,
								(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
								(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 0)));

						} else if (txt_id == 5) {

							sprintf(dst, fmt,
								(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 0)),
								(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2);

						} else if (txt_id == 6) {

							sprintf(dst, fmt,
								(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
								(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_SEX), 1)),
								(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2);

						} else if (txt_id == 8) {

							sprintf(dst, fmt,
								(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2,
								(Bit8u*)ds_readd(UNICORN_HERO_PTR) + HERO_NAME2);

						} else {
							strcpy(dst, fmt);
						}

					}
				} else if (ds_readws(TLK_ID) == 10 && ds_readws(DIALOG_INFORMER) == 0) {

					if (txt_id == 18) {

						sprintf(dst, fmt, (Bit8u*)(seg030_008d(10)));

					} else if (txt_id == 29) {

						sprintf(dst, fmt, (Bit8u*)(seg030_0000(10)));

					} else {

						strcpy(dst, fmt);
					}

				} else if (ds_readws(TLK_ID) == 17) {


					hero = (Bit8u*)get_first_hero_available_in_group();

					if (!txt_id || txt_id == 2 || txt_id == 25 || txt_id == 31 ||
						txt_id == 32 || txt_id == 60 || txt_id == 78 || txt_id == 87)
					{
						sprintf(dst, fmt, (char*)hero + HERO_NAME2);

					} else if (txt_id == 13 || txt_id == 19 || txt_id == 88 || txt_id == 24) {

						sprintf(dst, fmt,
							(char*)hero + HERO_NAME2,
							(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
							(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)));

					} else if (txt_id == 14 || txt_id == 15 || txt_id == 76) {

						sprintf(dst, fmt,
							(char*)(Bit8u*)ds_readd(RUIN_HERO) + HERO_NAME2,
							(char*)(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(RUIN_HERO) + 0x22), 0)));

					} else if (txt_id == 26 || txt_id == 65) {

						sprintf(dst, fmt,
							(char*)hero + HERO_NAME2,
							(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 0)),
							(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 2)),
							(char*)(GUI_get_ptr(host_readbs(hero + HERO_SEX), 1)));

					} else {

						strcpy(dst, fmt);

					}

				} else if (ds_readws(TLK_ID) == 0) {

					if (txt_id == 40 || txt_id == 41 || txt_id == 43) {

						sprintf(dst, fmt,
							(char*)(Bit8u*)ds_readd(RANDOM_TLK_HERO) + HERO_NAME2,
							(char*)(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(RANDOM_TLK_HERO) + 0x22), 0)),
							(char*)(GUI_get_ptr(host_readbs((Bit8u*)ds_readd(RANDOM_TLK_HERO) + 0x22), 2)));
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

			answer = GUI_dialogbox((unsigned char*)ds_readd(DTP2), (Bit8u*)ds_readd(DIALOG_TITLE), (Bit8u*)dst, optioncount,
					get_tx2(host_readb(state_ptr + 2) + txt_offset),
					get_tx2(host_readb(state_ptr + 3) + txt_offset),
					get_tx2(host_readb(state_ptr + 4) + txt_offset));
		}


		ds_writews(DIALOG_NEXT_STATE, -1);

		if (host_readws(state_ptr) & 0x8000 || host_readws(state_ptr) == -1) {
			talk_switch();
		}

		ds_writew(DIALOG_STATE, ds_readws(DIALOG_NEXT_STATE) == -1 ? host_readb(state_ptr + 5) : ds_readws(DIALOG_NEXT_STATE));

		if (ds_readws(DIALOG_DONE) == 0) {

			/* set the new dialog state */

			if (optioncount != 0 ) {
				if (answer == -1) {
					ds_writew(DIALOG_DONE, 1);
				} else if (answer == 1) {
					ds_writews(DIALOG_STATE, host_readb(state_ptr + 5));
				} else if (answer == 2) {
					ds_writews(DIALOG_STATE, host_readb(state_ptr + 6));
				} else if (answer == 3) {
					ds_writews(DIALOG_STATE, host_readb(state_ptr + 7));
				}
			}

			if (ds_readws(DIALOG_STATE) == 255) {
				ds_writew(DIALOG_DONE, 1);
			}
		}

	} while (ds_readws(DIALOG_DONE) == 0);

	ds_writews(TEXT_FILE_INDEX, -1);
	ds_writews(CURRENT_ANI, -1);
}

/* Borlandified and identical */
void talk_switch(void)
{
	signed short state;

	state = ds_readws(DIALOG_STATE);

	if (ds_readws(TLK_ID) == 3) {
		/* DASPOTA1.TLK */

		if (ds_readws(DIALOG_INFORMER) == 0) {
			if ((state == 1 || state == 2 || state == 3) && !ds_readb((DASPOTA_FIGHTFLAGS + 1))) {

				ds_writew(FIG_DISCARD, 1);

				if (!do_fight(FIGHTS_DASP1A)) {
					if (GUI_bool(get_tx(22))) {

						ds_writew(FIG_DISCARD, 0);

						if (!do_fight(FIGHTS_DASP1B)) {
							ds_writeb((DASPOTA_FIGHTFLAGS + 1), 1);
						}
					}
				}

				ds_writew(FIG_DISCARD, 0);
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 1) {

			if (state == 1 && !ds_readb((DASPOTA_FIGHTFLAGS + 2))) {

				if (!do_fight(FIGHTS_DASP2)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 2), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 2) {

			if ((state == 4 || state == 6) && !ds_readb((DASPOTA_FIGHTFLAGS + 3))) {

				if (!do_fight(FIGHTS_DASP3)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 3), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 3) {

			if (state == 4 && !ds_readb((DASPOTA_FIGHTFLAGS + 4))) {

				if (!do_fight(FIGHTS_DASP4)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 4), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 4) {

			if ((state == 1 || state == 4 || state == 7) && !ds_readb((DASPOTA_FIGHTFLAGS + 5))) {

				if (!do_fight(FIGHTS_DASP5)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 5), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 5) {

			if ((state == 4 || state == 5) && !ds_readb((DASPOTA_FIGHTFLAGS + 6))) {

				if (!do_fight(FIGHTS_DASP6A)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 6), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 6) {

			if ((state == 1 || state == 4 || state == 7) && !ds_readb((DASPOTA_FIGHTFLAGS + 7))) {

				if (!do_fight(FIGHTS_DASP7)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 7), 1);
				}
			}
		}

	} else if (ds_readws(TLK_ID) == 4) {
		/* DASPOTA2.TLK */

		if (ds_readws(DIALOG_INFORMER) == 0) {

			if (state == 4 && !ds_readb((DASPOTA_FIGHTFLAGS + 8))) {

				if (!do_fight(FIGHTS_DASP8)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 8), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 1) {

			if (state == 1 && !ds_readb((DASPOTA_FIGHTFLAGS + 9))) {

				if (!do_fight(FIGHTS_DASP9)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 9), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 2) {

			if (state == 1 && !ds_readb((DASPOTA_FIGHTFLAGS + 10))) {

				if (!do_fight(FIGHTS_DASP10)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 10), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 3) {

			if (state == 4 && !ds_readb((DASPOTA_FIGHTFLAGS + 11))) {

				if (!do_fight(FIGHTS_DASP11)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 11), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 4) {

			if (state == 1 && !ds_readb((DASPOTA_FIGHTFLAGS + 12))) {

				if (!do_fight(FIGHTS_DASP12A)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 12), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 5) {

			if (state == 5 && !ds_readb((DASPOTA_FIGHTFLAGS + 13))) {

				if (!do_fight(FIGHTS_DASP13)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 13), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 6) {

			if (state == 1 && !ds_readb((DASPOTA_FIGHTFLAGS + 14))) {

				if (!do_fight(FIGHTS_DASP14)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 14), 1);
				}
			}
		}

	} else if (ds_readws(TLK_ID) == 5) {
		/* DASPOTA3.TLK */

		if (ds_readws(DIALOG_INFORMER) == 0) {

			if ((state == 4 || state == 6) && !ds_readb((DASPOTA_FIGHTFLAGS + 16))) {

				if (!do_fight(FIGHTS_DASP16)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 16), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 1) {

			if ((state == 4 || state == 5 || state == 6) && !ds_readb((DASPOTA_FIGHTFLAGS + 17))) {

				if (!do_fight(FIGHTS_DASP17)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 17), 1);
				}
			}
		}

		if (ds_readws(DIALOG_INFORMER) == 2) {

			if ((state == 6 || state == 7) && !ds_readb((DASPOTA_FIGHTFLAGS + 18))) {

				if (!do_fight(FIGHTS_DASP18)) {
					ds_writeb((DASPOTA_FIGHTFLAGS + 18), 1);
				}
			}
		}

	} else if (ds_readws(TLK_ID) == 6) {
		/* INFO1.TLK */
		INF_jurge_hjore(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 7) {
		/* INFO2.TLK */
		INF_yasma_umbrik_isleif(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 8) {
		/* INFO3.TLK */
		INF_ragna_beorn_algrid(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 9) {
		/* INFO4.TLK */
		INF_eliane_tiomar(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 10) {
		/* INFO5.TLK */
		INF_olvir_asgrimm(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 11) {
		/* INFO6.TLK */
		INF_treborn_unicorn(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 12) {
		/* INFO7.TLK */
		INF_swafnild_unicorn(ds_readws(DIALOG_INFORMER), state);

	} else if (ds_readws(TLK_ID) == 13) {
		/* SCHMIED.TLK */
		TLK_schmied(state);

	} else if (ds_readws(TLK_ID) == 14) {
		/* GHANDEL.TLK */
		TLK_ghandel(state);

	} else if (ds_readws(TLK_ID) == 15) {
		/* KHANDEL.TLK */
		TLK_khandel(state);

	} else if (ds_readws(TLK_ID) == 16) {
		/* WHANDEL.TLK */
		TLK_whandel(state);

	} else if (ds_readws(TLK_ID) == 1) {
		/* HERBERG.TLK */
		TLK_herberg(state);

	} else if (ds_readws(TLK_ID) == 0) {
		/* F092.TLK */
		TLK_old_woman(state);

	} else if (ds_readws(TLK_ID) == 17) {
		/* F115.TLK */
		TLK_way_to_ruin(state);

	} else if (ds_readws(TLK_ID) == 18) {
		/* EREMIT.TLK */
		TLK_eremit(state);
	}
}

#if !defined(__BORLANDC__)
}
#endif
