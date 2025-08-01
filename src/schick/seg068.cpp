/*
 *	Rewrite of DSA1 v3.02_de functions of seg068 (special buildings: Thorwal 1/2)
 *	Functions rewritten: 13/13 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg058.cpp
 */
#include <stdio.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg007.h"
#include "seg026.h"
#include "seg047.h"
#include "seg055.h"
#include "seg058.h"
#include "seg068.h"
#include "seg075.h"
#include "seg096.h"
#include "seg097.h"
#include "seg099.h"
#include "seg105.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void THO_eisenhof(void)
{
	signed short answer;
	Bit32s money;

	do {
		answer = GUI_radio(get_tx2(47), 3, get_tx2(48),	get_tx2(49), get_tx2(50));

	} while (answer == -1);

	if (answer == 1) {

		gs_current_typeindex = 41;
		do_smith();

	} else if (answer == 2) {

		GUI_input(get_tx2(51), 0);

		money = get_party_money();
		money -= 10;
		set_party_money(money);

		/* test for CH+0 */
		if (test_attrib((Bit8u*)get_first_hero_available_in_group(), ATTRIB_CH, 0) > 0) {

			GUI_input(get_tx2(52), 0);

			sprintf((char*)g_dtp2, get_tx(random_schick(26) + 55));
			GUI_input((char*)g_dtp2, 0);
		} else {
			GUI_input(get_tx2(53), 0);
		}
	}
}

void THO_imman(void)
{

	unsigned short tmp;

	tmp = get_current_season();

	if ((tmp == 1 || tmp == 3) && (gs_day_of_week == 5)) {
		/* ask to visit the game */
		if (GUI_bool(get_tx2(55)) != 0) {

			tmp = random_schick(4) + 0x38;
			sprintf((char*)g_dtp2, get_tx2(56),
				/* winner */
				get_tx2(tmp),
				/* looser */
				get_tx2(random_schick(7) + 0x3c),
				/* winner */
				get_tx2(tmp),
				/* winners points */
				random_interval(15, 30),
				/* loosers points */
				random_schick(14));

			GUI_input((char*)g_dtp2, 0);
		}
	} else {
		/* no imman game at the moment */
		GUI_input(get_tx2(54), 0);
	}

}

void THO_botschaft(void)
{
	int closed = 0;

	/* At the 6th month in year 17 Hal the embassy is closed and another message is shown */
	/* Reason:
	 * Diplomatische Probleme infolge des Bruchs des Garether Vertrags? (Die Thorwaler besetzen im Hesinde Salzerhaven) *
	 * https://www.crystals-dsa-foren.de/showthread.php?tid=700&pid=99706#pid99706 */

	if ((gs_year > 17) || ((gs_year == 17) && (gs_month > 5))) {

		closed = 1;
	}

	GUI_input(!closed ? get_tx2(68) : get_tx2(69), 0);
}

void THO_bank(void)
{
	signed short answer;
	signed short done;
	signed short l3;
	Bit32s p_money;

	done = 0;

	if (gs_bank_deposit <= -1000) {

		GUI_input(get_tx2(77), 0);

		if (!gs_debt_days) {
			gs_debt_days = 7;
		}
	}

	load_in_head(16);

	do {

		sprintf((char*)g_text_output_buf, get_tx2(72), gs_bank_deposit);

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(81), (char*)g_text_output_buf, 3,
						get_tx2(73), get_tx2(74), get_tx2(80));
		} while (answer == -1);


		if (answer == 2) {

			answer = GUI_input(get_tx2(75), 3);

			if (answer <= 0) {

				GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(81), get_tx2(79), 0);

			} else {

				if (gs_days_to_cens
					|| ((gs_bank_deposit > 0) && (gs_bank_deposit + 200 < answer))
					|| ((gs_bank_deposit <= 0) && (answer > 200))) {

					GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(81), get_tx2(76), 0);

				} else {

					if (gs_bank_deposit < answer) {

						if (gs_bank_deposit > 0) {
							answer -= gs_bank_deposit;
							p_money = get_party_money();
							p_money += 10 * gs_bank_deposit;
							set_party_money(p_money);
							gs_bank_deposit = 0;
						}

						gs_monthly_credit += answer;

						if (gs_monthly_credit > 200) {

							GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(81), get_tx2(76), 0);

							l3 = gs_monthly_credit - 200;
							answer -= l3;
							gs_monthly_credit = 200;

							if (answer < 0) {
								answer = 0;
							}
						}

						if (gs_monthly_credit >= 200) {
							gs_days_to_cens = 30;
						}
					}

					gs_bank_deposit -= answer;
					p_money = get_party_money();
					p_money += 10 * answer;
					set_party_money(p_money);
				}
			}

		} else if (answer == 1) {

			if (gs_bank_deposit >= 30000) {

				/* prevent overflow  at 32767 */
				GUI_output((char*)(p_datseg + STR_BANK_DEPOSIT_TO_BIG));

			} else {

				answer = GUI_input(get_tx2(75), 3);
				p_money = get_party_money();

				if (answer * 10 > p_money) {

					GUI_output(get_ttx(401));

				} else {

					if (answer <= 0) {

						GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(81), get_tx2(79), 0);

					} else {

						p_money -= 10 * answer;
						set_party_money(p_money);
						gs_days_to_cens = 0;
						gs_bank_deposit += answer;

						if (gs_monthly_credit) {

							gs_monthly_credit -= answer;

							if (gs_monthly_credit < 0) {

								gs_monthly_credit = 0;
							}
						}

						if (gs_bank_deposit > -1000) {
							gs_debt_days = 0;
						}
					}
				}
			}
		} else {
			done = 1;
		}

	} while (!done);
}

void THO_arsenal(void)
{
	/* ARSENAL / ZEUGHAUS */
	signed short answer;
	signed short options;
	signed short tw_bak;
	Bit32s p_money;

	if (gs_day_timer < HOURS(8) || gs_day_timer > HOURS(19)) {

		GUI_output(get_ttx(482));

	} else if (gs_merchant_kicked_flags[gs_current_typeindex]) {

			talk_merchant();

	} else if (gs_arsenal_money) {

		load_in_head(13);

		/* only show two options when the group has "LETTER FROM JADRA" or "LETTER OF INTRODUCTION" */
		options = get_first_hero_with_item(ITEM_WRITING_OF_JARDA) != -1 || get_first_hero_with_item(ITEM_WRITING_OF_HETMAN) != -1 ? 2 : 1;

		do {
			answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(0), options, get_tx2(2), get_tx2(1));

		} while (answer == -1);

		if (answer == 2) {

			if (gs_arsenal_money == -1) {

				/* calculate the maximal shopping price [10-60] D */
				if ((gs_arsenal_money = 15 * gs_subvention) < 10) {
					/* at least 10D */
					gs_arsenal_money = 10;
				}
			}

			sprintf((char*)g_dtp2 + 0x400, get_tx2(3), gs_arsenal_money);

			gs_arsenal_money *= 100;
			GUI_dialog_na(0, (char*)(g_dtp2 + 0x400));
			p_money = get_party_money();
			set_party_money(gs_arsenal_money);

			gs_current_typeindex = 92;
			tw_bak = g_textbox_width;
			g_textbox_width = 3;

			do_merchant();

			gs_arsenal_money = (signed short)get_party_money();
			gs_arsenal_money /= 100;
			g_textbox_width = tw_bak;
			set_party_money(p_money);


		} else {
			GUI_dialog_na(0, get_tx2(4));
			gs_need_letter = 1;
		}

	} else {

		GUI_output((char*)(p_datseg + STR_OBVIOUSLY_CLOSED));
	}
}

void THO_magistracy(void)
{
	signed short answer;

	if (!gs_got_main_quest) {

		GUI_output(get_tx2(5));
		GUI_output(get_tx2(6));
		GUI_output(get_tx2(7));

	} else {

		do {
			answer = GUI_radio(get_tx2(5), 3, get_tx2(8), get_tx2(9), get_tx2(10));

		} while (answer == -1);

		if (answer == 1) {

			if (!gs_got_letter_jad) {

				gs_got_letter_jad = 1;

				GUI_output(get_tx2(14));

				/* get "LETTER FROM JADRA" */
				get_item(ITEM_WRITING_OF_JARDA, 1, 1);

			} else {
				GUI_output(get_tx2(15));
			}

		} else if (answer == 2) {

			GUI_output(get_tx2(12));

		} else {
			GUI_output(get_tx2(13));
		}
	}
}

void THO_mueller(void)
{
	if (GUI_bool(get_tx2(16))) {

		/* first visit ? */
		GUI_output(!gs_visited_miller ? get_tx2(17) : get_tx2(18));

		/* mark the miller as visited */
		gs_visited_miller = 1;
	}
}

void THO_black_finger(void)
{
	if (GUI_radio(get_tx2(19), 2, get_tx2(20), get_tx2(21)) == 1) {

		GUI_output(get_tx2(22));
	}
}

/* static */
void dramosch_says(char *msg)
{
	GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(48), msg, 0);
}

void THO_ugdalf(void)
{
	signed short answer;
	signed short randval;

	load_in_head(gs_quest_ugdalf == 0 ? 0 : 14);

	if (gs_quest_ugdalf == 0) {

		/* talk to the guards */
		randval = random_schick(10) - 1;

		answer = GUI_dialogbox((unsigned char*)g_dtp2, NULL,
					get_tx2(23), 3,
					get_tx2(randval + 38),
					get_tx2(24),
					get_tx2(25));

		if (answer == 1) {

			GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(27), 0);

		} else if (answer == 2) {

			/* talk to DRAMOSCH */
			GUI_dialogbox((unsigned char*)g_dtp2, NULL, get_tx2(28), 0);

			load_in_head(14);

			dramosch_says(get_tx2(29));

			do {
				answer = GUI_dialogbox((unsigned char*)g_dtp2, get_tx2(48), get_tx2(30), 2, get_tx2(31), get_tx2(32));

			} while (answer == -1);

			if (answer == 1) {
				/* take the quest */
				dramosch_says(get_tx2(33));
				gs_quest_ugdalf = 1;

			} else {

				dramosch_says(get_tx2(34));
			}
		}
	} else if (gs_quest_ugdalf == 1 || !ds_readb(DNG14_UGDALF_DONE)) {

		dramosch_says(get_tx2(35));

		/* enter the dungeon */
		DNG_enter_dungeon(DUNGEONS_ZWINGFESTE);

		gs_x_target_bak = (ds_readw(X_TARGET));
		gs_y_target_bak = (ds_readw(Y_TARGET));

		if (gs_quest_ugdalf == 1) {
			add_party_money(2000L);

		/* Original-Bug:	Everytime the heroes enter the dungeon they get 20D.
					Why this fix works is not seen that easy.
					As long as ds_readb(DNG14_UGDALF_DONE) is 0 this block is executed.
		 */
#ifdef M302de_ORIGINAL_BUGFIX
			gs_quest_ugdalf = 2;
#endif
		}

	} else if (gs_quest_ugdalf == 3) {

		/* talk with DRAMOSCH for 8 h */
		dramosch_says(get_tx2(36));
		timewarp(HOURS(8));

		/* mark this quest as done */
		gs_quest_ugdalf = 4;

		/* get the reward */
		add_hero_ap_all(25);
		add_party_money(5000);
	} else {

		dramosch_says(get_tx2(37));

		sprintf((char*)g_dtp2 + 0x400, get_tx(random_schick(26) + 55));

		dramosch_says((char*)(g_dtp2 + 0x400));

		/* enter the dungeon */
		DNG_enter_dungeon(DUNGEONS_ZWINGFESTE);

		gs_x_target_bak = (ds_readw(X_TARGET));
		gs_y_target_bak = (ds_readw(Y_TARGET));
	}
}

/* should be static */
void academy_analues(void)
{
	signed short buffer1_bak;
	signed short hero_pos;

	GUI_input(get_tx2(62), 0);

	/* change behavior of analues spell */
	gs_in_academy = 99;

	/* select a hero (does not need to be a magic user here) */
	hero_pos = select_hero_ok(get_ttx(794));

	if (hero_pos != -1) {

		g_spelluser = get_hero(hero_pos);

		buffer1_bak = g_tx_file_index;

		load_tx(ARCHIVE_FILE_SPELLTXT_LTX);

		sprintf((char*)g_dtp2, get_tx2(64), (char*)spell_analues());

		if (buffer1_bak != -1 && buffer1_bak != 222) {

			load_tx(buffer1_bak);
		}

		GUI_input((char*)g_dtp2, 0);

		gs_academy_daily_ident = 1;
	}

	/* change behaviour of analues spell */
	gs_in_academy = 0;
}

void THO_academy(void)
{
	signed short answer;
	signed short item_id;
	signed short item_pos;
	signed short cursed_hero_pos;
	Bit32s p_money;
	Bit8u *hero;

	/* find the position of the first cursed (=renegade) hero */
	hero = get_hero(0);
	for (item_pos = cursed_hero_pos = 0; item_pos <= 6; item_pos++, hero += SIZEOF_HERO) {

		if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
			hero_renegade(hero))
		{
			cursed_hero_pos = item_pos;
			break;
		}
	}

	do {
		answer = GUI_radio(get_tx2(49), 3, get_tx2(50), get_tx2(51), get_tx2(52));

	} while (answer == -1);

	if (answer == 1) {

		/* remove curse */

		if (cursed_hero_pos == 0) {

			GUI_input(get_tx2(67), 0);

		} else if (gs_academy_daily_curse != 0) {

			GUI_input(get_tx2(65), 0);

		} else {

			sprintf((char*)g_dtp2, get_tx2(53), (char*)hero + HERO_NAME2);

			do {
				answer = GUI_radio((char*)g_dtp2, 2, get_tx2(68), get_tx2(69));

			} while (answer == -1);

			if (answer == 1) {

				item_id = academy_get_equal_item(2000);

				if (item_id >= 0) {

					sprintf((char*)g_dtp2, get_tx2(56),
						(char*)GUI_names_grammar((signed short)0x8002, item_id, 0));

					do {
						answer = GUI_radio((char*)g_dtp2, 4, get_tx2(57), get_tx2(58), get_tx2(59), get_tx2(60));

					} while (answer == -1);

					if (answer == 1 || answer == 3) {

						GUI_input(get_tx2(61), 0);

					} else {

						hero = get_hero(get_first_hero_with_item(item_id));
						item_pos = get_item_pos(hero, item_id);

						if (drop_item(hero, item_pos, 1)) {

							GUI_input(get_tx2(62), 0);
							GUI_input(get_tx2(63), 0);

							gs_academy_daily_curse = 1;

							/* unset 'renegate' flag */
							and_ptr_bs(get_hero(cursed_hero_pos) + HERO_FLAGS1, 0xdf);

						} else {
							GUI_input(get_tx2(70), 0);
						}
					}

				} else if (item_id == -2) {

					p_money = get_party_money();
					p_money -= 2000;
					set_party_money(p_money);

					GUI_input(get_tx2(63), 0);

					gs_academy_daily_curse = 1;

					and_ptr_bs(get_hero(cursed_hero_pos) + HERO_FLAGS1, 0xdf); /* unset 'renegate' flag */

				} else {
					GUI_input(get_ttx(401), 0);
				}
			} else {
				GUI_input(get_tx2(70), 0);
			}
		}

	} else if (answer == 2) {

		/* identify item */

		if (gs_academy_daily_ident != 0) {

			GUI_input(get_tx2(66), 0);

		} else {

			do {
				answer = GUI_radio(get_tx2(54), 2, get_tx2(68), get_tx2(69));

			} while (answer == -1);

			if (answer == 1) {

				item_id = academy_get_equal_item(1000);

				if (item_id >= 0) {

					sprintf((char*)g_dtp2, get_tx2(56),
						(char*)GUI_names_grammar((signed short)0x8002, item_id, 0));

					do {
						answer = GUI_radio((char*)g_dtp2, 4, get_tx2(57), get_tx2(58), get_tx2(59), get_tx2(60));

					} while (answer == -1);

					if (answer == 1 || answer == 3) {

						GUI_input(get_tx2(61), 0);

					} else {

						hero = get_hero(get_first_hero_with_item(item_id));
						item_pos = get_item_pos(hero, item_id);

						if (drop_item(hero, item_pos, 1)) {

							academy_analues();

						} else {

							GUI_input(get_tx2(70), 0);
						}
					}

				} else if (item_id == -2) {

					p_money = get_party_money();
					p_money -= 1000;
					set_party_money(p_money);

					academy_analues();
				} else {
					GUI_input(get_ttx(401), 0);
				}
			} else {

				GUI_input(get_tx2(70), 0);

			}
		}
	} else {

		GUI_input(get_tx2(55), 0);
	}
}

/**
 * \brief   check if you can pay the price with an item
 *
 * \param   price       price you want to pay
 * \return              -2 = enough money, -1 no item found or the item_id
 */
/* should be static */
signed short academy_get_equal_item(signed short price)
{
	signed short item_pos;
	signed short retval;
	signed short i;
	Bit32s p_money;
	Bit8u *hero;
	Bit8u *p_item;

	retval = -2;
	p_money = get_party_money();

	if (price > p_money) {

		retval = -1;
		hero = get_hero(0);
		for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				!hero_dead(hero))
			{
				for (item_pos = 0; item_pos < NR_HERO_INVENTORY_SLOTS; item_pos++) {

					if (host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * item_pos) != ITEM_NONE &&
						!inventory_broken(hero + HERO_INVENTORY + SIZEOF_INVENTORY * item_pos))
						/* remark: armor with degraded RS is accepted */
					{
						p_item = get_itemsdat(host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * item_pos));

						if (host_readws(p_item + ITEM_STATS_PRICE) * host_readbs(p_item + ITEM_STATS_PRICE_UNIT) >= price)
						{
							retval = host_readws(hero + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * item_pos);
							break;
						}
					}
				}
			}

			if (retval != -1) break;
		}
	}

	return retval;
}

#if !defined(__BORLANDC__)
}
#endif
