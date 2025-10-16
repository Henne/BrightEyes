/**
 *	Rewrite of DSA1 v3.02_de functions of seg066 (city)
 *	Functions rewritten: 18/18 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg066.cpp
 */
#include <stdlib.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg003.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg024.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg029.h"
#include "seg030.h"
#include "seg032.h"
#include "seg049.h"
#include "seg066.h"
#include "seg067.h"
#include "seg068.h"
#include "seg069.h"
#include "seg070.h"
#include "seg071.h"
#include "seg074.h"
#include "seg092.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

HugePt g_buffer9_ptr4;				// ds:0xe3fc, copy of BUFFER9_PTR
unsigned char g_city_house_count[4];		// ds:0xe400, counts of the four different kinds of houses on the current map
unsigned char *g_buffer11_ptr;			// ds:0xe404, to buffer of size 192, used for palettes
static signed short g_city_refresh_direction;	// ds:0xe408
static signed short g_city_refresh_y_target;	// ds:0xe40a
static signed short g_city_refresh_x_target;	// ds:0xe40c
static signed short g_always_zero1;		// ds:0xe40e, writeonly (0)
static signed short g_always_zero2;		// ds:0xe410, writeonly (0)
static signed short g_entrance_angle;		// ds:0xe412
unsigned char *g_tex_floor[6];			// ds:0xe414


signed short enter_location(signed short town_id)
{
	signed short map_pos;
	signed short b_index;
	struct location *locations_tab_ptr;

	if (town_id == TOWNS_DASPOTA) {
		return enter_location_daspota();
	}

	map_pos = 256 * gs_x_target + gs_y_target;
	locations_tab_ptr = &g_locations_tab[0];
	g_location_market_flag = 0;

	do {
		if (locations_tab_ptr->pos == map_pos) {

			/* found the location */
			gs_current_loctype_bak = LOCTYPE_NONE;
			gs_current_loctype = locations_tab_ptr->loctype;
			gs_current_typeindex = locations_tab_ptr->typeindex;
			gs_current_locdata = locations_tab_ptr->locdata;

			if (gs_current_loctype == LOCTYPE_MARKET) {
				gs_current_loctype = LOCTYPE_NONE;
				g_location_market_flag = 1;
			}

			return 1;
		}

		locations_tab_ptr++;

	} while (locations_tab_ptr->pos != -1);

	move();

	if ((b_index = get_border_index(cast_u16(g_visual_field_vals[1]))) >= 2 && b_index <= 5) {

		gs_current_loctype_bak = LOCTYPE_NONE;
		gs_current_locdata = g_towns_cityindex_table[town_id - 1];

		if (!((gs_direction + gs_x_target + gs_y_target) & 1)) {
			gs_current_loctype = LOCTYPE_CITIZEN;
		} else {
			gs_current_loctype = LOCTYPE_HOUSE;
			gs_current_locdata++;
		}

		return 1;
	}

	return 0;
}

signed short enter_location_daspota(void)
{
	signed short map_pos;
	signed short b_index;
	struct location *locations_tab_ptr;

	if (g_game_state == GAME_STATE_FIGQUIT) {
		return 1;
	}

	map_pos = 256 * gs_x_target + gs_y_target;
	locations_tab_ptr = &g_locations_tab[0];
	g_location_market_flag = 0;

	do {

		if (locations_tab_ptr->pos == map_pos) {

			gs_current_typeindex = locations_tab_ptr->typeindex;

			if (locations_tab_ptr->loctype != LOCTYPE_SIGNPOST) {

				GUI_print_loc_line(get_tx(locations_tab_ptr->locdata));

				if (!gs_daspota_fightflags[locations_tab_ptr->locdata]) {

					do_talk(locations_tab_ptr->loctype, locations_tab_ptr->typeindex - 1);

					if (!gs_daspota_fightflags[locations_tab_ptr->locdata]) {
						leave_location();
						return 1;
					}
				}

				draw_main_screen();
				set_var_to_zero();

				load_ani(10);
				GUI_print_loc_line(get_tx(locations_tab_ptr->locdata));
				init_ani(0);

				if (g_daspota_locloot_index[locations_tab_ptr->locdata - 1]) {

					loot_multi_chest(g_daspota_locloot_index[locations_tab_ptr->locdata - 1], get_tx(21));

				} else {

					do {
						handle_gui_input();

					} while (g_action == 0 && g_mouse1_event2 == 0);

					g_mouse1_event2 = 0;
				}

				set_var_to_zero();

				if (locations_tab_ptr->locdata == 6) {
					do_fight(FIGHTS_DASP6B);
				} else if (locations_tab_ptr->locdata == 12) {
					do_fight(FIGHTS_DASP12B);
				}

				leave_location();

			} else {
				gs_current_loctype_bak = LOCTYPE_NONE;
				gs_current_loctype = locations_tab_ptr->loctype;
				gs_current_locdata = locations_tab_ptr->locdata;
			}

			return 1;
		}

		locations_tab_ptr++;

	} while (locations_tab_ptr->pos != -1);

	move();

	if ((b_index = get_border_index(cast_u16(g_visual_field_vals[1]))) >= 2 && b_index <= 5) {

		gs_current_loctype_bak = LOCTYPE_NONE;
		gs_current_loctype = LOCTYPE_CITIZEN;
		gs_current_locdata = 19;
		return 1;
	}

	return 0;
}

void do_special_buildings(void)
{
	signed short type;
	signed short tw_bak;

	tw_bak = g_textbox_width;
	type = gs_current_typeindex;

	if (gs_current_town == TOWNS_THORWAL) {

		load_tx2(type < 41 ? ARCHIVE_FILE_THORWAL1_LTX : ARCHIVE_FILE_THORWAL2_LTX);
		g_textbox_width = 9;

		if (type == 28) {
			THO_hetmann();
		} else if (type == 29) {
			THO_windriders();
		} else if (type == 30) {
			THO_stormchildren();
		} else if (type == 31) {
			THO_garaldsson();
		} else if (type == 32) {
			THO_eisenhof();
		} else if (type == 33) {
			THO_imman();
		} else if (type == 34) {
			THO_botschaft();
		} else if (type == 35) {
			GUI_output(get_tx2(70));
		} else if (type == 36) {
			GUI_output(get_tx2(71));
		} else if (type == 37) {
			THO_bank();
		} else if (type == 38) {
			GUI_output(get_tx2(82));
		} else if (type == 39) {
			GUI_output(get_tx2(83));
		} else if (type == 40) {
			GUI_output(get_tx2(84));
		} else if (type == 41) {
			THO_arsenal();
		} else if (type == 42) {
			THO_magistracy();
		} else if (type == 43) {
			THO_mueller();
		} else if (type == 44) {
			THO_black_finger();
		} else if (type == 45) {
			THO_ugdalf();
		} else if (type == 46) {
			THO_academy();
		} else if (type == 11 || type == 14 || type == 17 || type == 21) {
			THO_tav_inn_combi();
		}

	} else if (gs_current_town == TOWNS_PHEXCAER) {

		load_tx2(type <= 3 ? ARCHIVE_FILE_PHEX2_LTX : ARCHIVE_FILE_PHEX1_LTX);
		g_textbox_width = 9;

		if (type == 1) {
			PHX_phextempel();
		} else if (type == 3) {
			PHX_drogentempel();
		} else if (type == 18) {
			PHX_apotheke();
		} else if (type == 20) {
			PHX_healer();
		} else if (type == 23) {
			PHX_bordell();
		} else if (type == 24) {
			PHX_villa_gremob();
		} else if (type == 25) {
			PHX_spielhaus();
		} else if (type == 26) {
			PHX_fuhrhaus();
		} else if (type == 27) {
			PHX_stadthaus();
		}


	} else if (gs_current_town == TOWNS_EINSIEDLERSEE) {
		/*  HERMITS LAKE / EINSIEDLERSEE */

		if (type == 1) {
			load_ani(8);
			init_ani(1);
			do_talk(18, 0);
			set_var_to_zero();
		}
	}

	g_textbox_width = tw_bak;
	leave_location();
}

void TLK_eremit(signed short state)
{
	signed int i;
	struct struct_hero *hero;

	if (!state) {

		g_dialog_next_state = (gs_hermit_visited ? 1 : 2);

	} else if (state == 6) {

		hero = get_hero(0);
		for (i = 0 ; i <= 6; i++, hero++) {

			/* remove hunger and thirst */
			hero->hunger = hero->thirst = 0;

			/* heal all wounds */
			add_hero_le(hero, hero->le_max);
		}

	} else if (state == 10) {

		/* group learns about two places to rest */
		gs_tevent137_flag = gs_tevent134_flag = 1;

	} else if (state == 13) {

		gs_hermit_visited = 1;

	} else if (state == 14) {

		timewarp(MINUTES(30));
	}
}

void do_town(void)
{
	if ((g_city_area_loaded != gs_current_town) ||	(g_area_prepared != 1))
	{
		seg028_0555(1);

		set_audio_track(ARCHIVE_FILE_THORWAL_XMI);

		g_request_refresh = 1;

		diary_new_entry();
	}

	g_current_ani = -1;

	gs_current_town_bak = gs_current_town;

	city_step();
}

#if defined(__BORLANDC__)
static
#endif
void refresh_floor_and_sky(void)
{
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	nvf.dst = g_renderbuf_ptr;
	nvf.src = g_tex_floor[1]; // tex_sky
	nvf.no = 0;
	nvf.type = 3;
	nvf.width = &width;
	nvf.height = &height;

	process_nvf(&nvf);

	nvf.dst = ((Bit8u*)g_renderbuf_ptr) + 208 * height;
	nvf.src = g_tex_floor[0];
	nvf.no = 0;
	nvf.type = 3;
	nvf.width = &width;
	nvf.height = &height;

	process_nvf(&nvf);
}

void seg066_0692(void)
{
	refresh_floor_and_sky();
	move();
	seg066_06c1();
	seg066_0bad();

	/* TODO: these are write only variables */
	g_always_zero2 = g_always_zero1 = 0;

	city_water_and_grass();
	city_building_textures();
	city_fade_and_colors();
}

void seg066_06c1(void)
{
	signed int bi;

	if ((bi = get_border_index(g_visual_field_vals[3])) >= 2 && bi <= 5) {
		g_visual_field_vals[3] = 0;
	}

	if ((bi = get_border_index(g_visual_field_vals[7])) >= 2 && bi <= 5) {
		g_visual_field_vals[7] = 0;
	}

	if ((bi = get_border_index(g_visual_field_vals[8])) >= 2 && bi <= 5) {
		g_visual_field_vals[8] = 0;
	}

	if ((bi = get_border_index(g_visual_field_vals[14])) >= 2 && bi <= 5) {
		g_visual_field_vals[14] = 0;
	}

	if ((bi = get_border_index(g_visual_field_vals[15])) >= 2 && bi <= 5) {
		g_visual_field_vals[15] = 0;
	}

	if ((bi = get_border_index(g_visual_field_vals[21])) >= 2 && bi <= 5) {
		g_visual_field_vals[21] = 0;
	}

	if ((bi = get_border_index(g_visual_field_vals[0])) >= 2 && bi <= 5) {

		g_visual_field_vals[9] = g_visual_field_vals[15] = g_visual_field_vals[22] = 0;

		if ((bi = get_border_index(g_visual_field_vals[5])) >= 2 && bi <= 5) {
			g_visual_field_vals[4] = g_visual_field_vals[10] = g_visual_field_vals[16] = g_visual_field_vals[23] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[2])) >= 2 && bi <= 5) {

		g_visual_field_vals[13] = g_visual_field_vals[21] = g_visual_field_vals[28] = 0;

		if ((bi = get_border_index(g_visual_field_vals[5])) >= 2 && bi <= 5) {
			g_visual_field_vals[6] = g_visual_field_vals[12] = g_visual_field_vals[20] = g_visual_field_vals[27] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[4])) >= 2 && bi <= 5) {

		g_visual_field_vals[9] = g_visual_field_vals[16] = g_visual_field_vals[22] = g_visual_field_vals[23] = 0;

		if ((bi = get_border_index(g_visual_field_vals[5])) >= 2 && bi <= 5) {
			g_visual_field_vals[10] = 0;
		}

		if ((bi = get_border_index(g_visual_field_vals[11])) >= 2 && bi <= 5) {
			g_visual_field_vals[10] = g_visual_field_vals[17] = g_visual_field_vals[18] = 0;
			g_visual_field_vals[24] = g_visual_field_vals[25] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[5])) >= 2 && bi <= 5) {

		g_visual_field_vals[11] = g_visual_field_vals[17] = g_visual_field_vals[18] = 0;

		g_visual_field_vals[19] = g_visual_field_vals[24] = g_visual_field_vals[25] = 0;

		g_visual_field_vals[26] = 0;
	}

	if ((bi = get_border_index(g_visual_field_vals[6])) >= 2 && bi <= 5) {

		g_visual_field_vals[13] = g_visual_field_vals[20] = g_visual_field_vals[27] = g_visual_field_vals[28] = 0;

		if ((bi = get_border_index(g_visual_field_vals[5])) >= 2 && bi <= 5) {

			g_visual_field_vals[11] = g_visual_field_vals[12] = g_visual_field_vals[17] = 0;

			g_visual_field_vals[18] = g_visual_field_vals[19] = g_visual_field_vals[24] = 0;

			g_visual_field_vals[25] = g_visual_field_vals[26] = 0;
		}

		if ((bi = get_border_index(g_visual_field_vals[11])) >= 2 && bi <= 5) {

			g_visual_field_vals[12] = g_visual_field_vals[18] = g_visual_field_vals[19] = 0;

			g_visual_field_vals[25] = g_visual_field_vals[26] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[9])) >= 2 && bi <= 5) {

		g_visual_field_vals[22] = 0;

		if ((bi = get_border_index(g_visual_field_vals[17])) >= 2 && bi <= 5) {
			g_visual_field_vals[16] = g_visual_field_vals[23] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[13])) >= 2 && bi <= 5) {

		g_visual_field_vals[28] = 0;

		if ((bi = get_border_index(g_visual_field_vals[19])) >= 2 && bi <= 5) {
			g_visual_field_vals[20] = g_visual_field_vals[27] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[10])) >= 2 && bi <= 5) {

		g_visual_field_vals[9] = g_visual_field_vals[16] = g_visual_field_vals[22] = g_visual_field_vals[23] = 0;

		if ((bi = get_border_index(g_visual_field_vals[11])) >= 2 && bi <= 5) {
			g_visual_field_vals[18] = 0;
		}

		if (((bi = get_border_index(g_visual_field_vals[11])) >= 2 && bi <= 5) ||
			((bi = get_border_index(g_visual_field_vals[18])) >= 2 && bi <= 5)) {

			g_visual_field_vals[17] = g_visual_field_vals[24] = g_visual_field_vals[25] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[12])) >= 2 && bi <= 5) {

		g_visual_field_vals[13] = g_visual_field_vals[20] = g_visual_field_vals[27] = g_visual_field_vals[28] = 0;

		if ((bi = get_border_index(g_visual_field_vals[11])) >= 2 && bi <= 5) {
			g_visual_field_vals[18] = 0;
		}

		if (((bi = get_border_index(g_visual_field_vals[11])) >= 2 && bi <= 5) ||
			((bi = get_border_index(g_visual_field_vals[18])) >= 2 && bi <= 5)) {

			g_visual_field_vals[19] = g_visual_field_vals[25] = g_visual_field_vals[26] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[11])) >= 2 && bi <= 5) {
		g_visual_field_vals[18] = g_visual_field_vals[25] = 0;
	}

	if ((bi = get_border_index(g_visual_field_vals[16])) >= 2 && bi <= 5) {

		g_visual_field_vals[22] = 0;

		if ((bi = get_border_index(g_visual_field_vals[24])) >= 2 && bi <= 5) {
			g_visual_field_vals[23] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[20])) >= 2 && bi <= 5) {

		g_visual_field_vals[28] = 0;

		if ((bi = get_border_index(g_visual_field_vals[26])) >= 2 && bi <= 5) {
			g_visual_field_vals[27] = 0;
		}
	}

	if ((bi = get_border_index(g_visual_field_vals[18])) >= 2 && bi <= 5) {
		g_visual_field_vals[25] = 0;
	}

	if (((bi = get_border_index(g_visual_field_vals[18])) >= 2 && bi <= 5) ||
		((bi = get_border_index(g_visual_field_vals[25])) >= 2 && bi <= 5)) {

		if ((bi = get_border_index(g_visual_field_vals[17])) >= 2 && bi <= 5) {
			g_visual_field_vals[24] = 0;
		}

		if ((bi = get_border_index(g_visual_field_vals[19])) >= 2 && bi <= 5) {
			g_visual_field_vals[26] = 0;
		}
	}
}

signed short get_border_index(unsigned char val)
{
	signed int i;

	i = 0;
	while (g_mapval_to_loctype[i] < val) {
		i++;
	}

	g_entrance_angle = ((((val & 3) + 4) - gs_direction) & 3);

	if (i == 0)
		i = 1;

	i--;

	return i;
}

void seg066_0bad(void)
{
	signed int i;
	signed int bi;

	for (i = 28; i >= 0; i--) {

		bi = get_border_index(g_visual_field_vals[i]);

		if (bi == -1) {
			g_visual_fields_tex[i] = -1;
		} else {
			g_visual_fields_tex[i] = (
						bi == 2 ? g_seg066_0bad_unkn0[i] : (
						bi == 3 ? g_seg066_0bad_unkn1[i] : (
						bi == 4 ? g_seg066_0bad_unkn2[i] : (
						bi == 5 ? g_seg066_0bad_unkn3[i] : (
						bi == 6 ? g_seg066_0bad_unkn4[i] : (
						bi == 7 ? g_seg066_0bad_unkn4[i] : (
						bi == 8 ? g_seg066_0bad_unkn5[i] : (
						bi == 9 ? g_seg066_0bad_unkn6[i] : (
						bi == 10 ? g_seg066_0bad_unkn7[i] : (
						bi == 1 ? g_seg066_0bad_unkn2[i] : -1))))))))));
		}
	}
}

/**
 * \brief   draws water and grass textures
 */
void city_water_and_grass(void)
{
	signed int i;
	signed int nvf_no;
	signed int x;
	signed int y;
	signed char c1;
	signed char bi;
	unsigned char c2;
	Bit16s *ptr;

	for (i = 0; i < 29; i++) {

		c1 = g_visual_field_draw_order[i];
		c2 = g_visual_field_vals[c1];

		if (c2 != 0) {

			bi = get_border_index(c2);

			if (bi == 6 || bi == 7) {

				/* water or grass */
				ptr = (Bit16s*)&g_visual_field_offsets_grass[c1];

				x = ptr[0];
				y = ptr[1];

				c1 = g_visual_fields_tex[c1];

				if (c1 != -1) {

					ptr = (Bit16s*)&g_tex_descr_table[c1 - 1][0];

					if ((nvf_no = ptr[2]) != -1) {

						if (bi == 7) {
							nvf_no += 15;
						}

						load_city_texture(x + ptr[0], y + ptr[1], nvf_no, 184);
					}
				}
			}
		}
	}
}

/**
 * \brief   draws building textures
 */
void city_building_textures(void)
{
	signed short nvf_no;
	signed short i;
	signed short x;
	signed short y;
	signed short l4;
	signed char c1;
	signed char bi;
	unsigned char c2;
	Bit16s *ptr;

	for (i = 0; i < 29; i++) {

		c1 = g_visual_field_draw_order[i];
		c2 = g_visual_field_vals[c1];

		if (c2 != 0) {

			bi = get_border_index(c2);

			if (bi != 7 && bi != 6) {
			    /* if not grass or water */

				ptr = (Bit16s*)&g_visual_field_offsets_std[c1];

				if (bi == 8) {
					/* direction sign */
					ptr = (Bit16s*)&g_visual_field_offsets_sign[c1];
				} else if (bi == 9 || bi == 10) {
					/* tavern/inn or shop */
					ptr = (Bit16s*)&g_visual_field_offsets_inn[c1];
				}

				x = ptr[0];
				y = ptr[1];

				c1 = g_visual_fields_tex[c1];

				if (c1 != -1) {

					ptr = (Bit16s*)&g_tex_descr_table[c1 - 1][0];

					l4 =	bi == 2 ? 186 : (
						bi == 3 ? 187 : (
						bi == 4 ? 188 : (
						bi == 5 ? 189 : (
						bi == 1 ? 188 : (
						bi == 9 ? 232 : (
						bi == 10 ? 233 : 185))))));

					if ((nvf_no = ptr[2]) != -1) {

						if (g_entrance_angle == 2 && bi >= 1 && bi <= 5) {

							if (bi == 1) {
								nvf_no -= 5;
								l4 = 185;
							} else {
								nvf_no -= 10;
							}
						}

						if (bi == 9 || bi == 10) {
							load_special_textures(bi);
						}

						load_city_texture(x + ptr[0], y + ptr[1], nvf_no, l4);

						if (bi == 9 || bi == 10) {
							call_load_buffer();
						}
					}

					if ((nvf_no = ptr[5]) != -1) {

						if (bi == 1) {
							l4 = 188;
						}

						if (g_entrance_angle == 1 && !(nvf_no & 0x8000) &&
							bi >= 1 && bi <= 5)
						{
							nvf_no -= 10;

						} else if (g_entrance_angle == 3 && (nvf_no & 0x8000))
						{
							nvf_no = ((unsigned short)(nvf_no & 0x7fff) - 10) | 0x8000;
						}

						load_city_texture(x + ptr[3], y + ptr[4], nvf_no, l4);
					}

					if ((nvf_no = ptr[8]) != -1) {
						load_city_texture(x + ptr[6], y + ptr[7], nvf_no, l4);
					}
				}
			}
		}
	}
}

void load_city_texture(signed short v1, signed short v2, signed short nvf_no,
		signed short v4)
{
	signed short width;
	signed short height;
	signed short copy_width;
	signed short copy_height;
	signed short direction;
	Bit8u *src;
	Bit8u *dst;
	struct nvf_desc nvf;

	direction = (nvf_no & 0x8000 ? 1: 0);
	nvf_no &= 0x3fff;

	v4 -= 184;

	nvf.dst = src = g_renderbuf_ptr + 30000;

	/*
	 * the following line accesses memory outside of the
	 * texture array if v4 is 48 or 49!?
	 */
	nvf.src = g_tex_floor[v4];

	if (v4 == 48 || v4 == 49) {
		nvf.src = (Bit8u*)g_buffer7_ptr;
	}

	nvf.no = nvf_no;
	nvf.type = (direction == 0 ? 3: 5);
	nvf.width = &width;
	nvf.height = &height;
	process_nvf(&nvf);

	copy_width = width;
	copy_height = height;

	if (v1 < 0) {
		if (v1 + copy_width > 0) {
			copy_width += v1;
			src += __abs__(v1);
			v1 = 0;
		}
	}

	if (v2 < 0) {
		if (v2 + copy_height > 0) {
			copy_height -= v2;
			src += __abs__(v2) * width;
			v2 = 0;
		}
	}


	if (v1 < 208 && v2 < 135 && v1 >= 0 && v2 >= 0) {

		if (v1 + copy_width > 208) {
			copy_width = 208 - v1;
		}

		if (v2 + copy_height > 135) {
			copy_height = 135 - v2;
		}

		dst = ((Bit8u*)g_renderbuf_ptr) + v2 * 208 + v1;

		copy_solid(dst, src, copy_width, copy_height, 208, width, v4 == 0 ? 0 : 128);
	}
}

void seg066_10c8(void)
{
	set_var_to_zero();
	seg066_0692();
	g_city_refresh_x_target = gs_x_target;
	g_city_refresh_y_target = gs_y_target;
	g_city_refresh_direction = gs_direction;
}

signed short city_step(void)
{
	signed short i;
	signed short bi;
	signed short options;
	signed short l4;

	g_new_menu_icons[0] = MENU_ICON_SPLIT_GROUP;
	l4 = g_new_menu_icons[1];
	g_new_menu_icons[1] = (g_can_merge_group == -1 ? MENU_ICON_MERGE_GROUP_GRAYED : MENU_ICON_MERGE_GROUP);

	if (g_new_menu_icons[1] != l4) {
		g_redraw_menuicons = 1;
	}

	g_new_menu_icons[2] = MENU_ICON_SWITCH_GROUP;
	g_new_menu_icons[3] = MENU_ICON_INFO;
	g_new_menu_icons[4] = MENU_ICON_MAP;
	g_new_menu_icons[5] = MENU_ICON_MAGIC;
	g_new_menu_icons[6] = MENU_ICON_CAMP;

	if (g_request_refresh != 0) {

		draw_main_screen();
		GUI_print_loc_line(get_tx(0));

		g_request_refresh = g_redraw_menuicons = 0;
		g_city_refresh_x_target = -1;
	}

	if (g_redraw_menuicons && g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {
		draw_icons();
		g_redraw_menuicons = 0;
	}

	/* check if position or direction has changed */
	if (gs_direction != g_city_refresh_direction ||
		gs_x_target != g_city_refresh_x_target ||
		gs_y_target != g_city_refresh_y_target)
	{
		seg066_10c8();
	}

	if (gs_x_target != gs_x_target_bak ||
		gs_y_target != gs_y_target_bak)
	{
		g_can_merge_group = can_merge_group();
		set_automap_tiles(gs_x_target, gs_y_target);
	}

	gs_x_target_bak = gs_x_target;
	gs_y_target_bak = gs_y_target;

	handle_gui_input();

	if (g_mouse2_event || g_action == ACTION_ID_PAGE_UP) {

		for (i = options = 0; i < 9; i++) {
			if (g_new_menu_icons[i] != MENU_ICON_NONE) {
				options++;
			}
		}

		i = GUI_radio(get_ttx(570), (signed char)options,
				get_ttx(535), get_ttx(536), get_ttx(537),
				get_ttx(538), get_ttx(539), get_ttx(213),
				get_ttx(306), get_ttx(569)) - 1;

		if (i != -2) {
			g_action = (i + ACTION_ID_ICON_1);
		}
	}

	i = 0;

	if (g_action == ACTION_ID_ICON_1) {

		GRP_split();
		g_can_merge_group = can_merge_group();

	} else if (g_action == ACTION_ID_ICON_2) {

		GRP_merge();
		g_can_merge_group = -1;

	} else if (g_action == ACTION_ID_ICON_3) {

		GRP_switch_to_next(0);
		i = 1;

	} else if (g_action == ACTION_ID_ICON_4) {

		game_options();

	} else if (g_action == ACTION_ID_ICON_5) {

		show_automap();

	} else if (g_action == ACTION_ID_ICON_6) {

		select_magic_user();

	} else if (g_action == ACTION_ID_ICON_7) {

		gs_current_loctype = LOCTYPE_CITYCAMP;
		g_citycamp_city = 1; /* CITYCAMP takes place in a town */
		i = 1;

	} else if (g_action == ACTION_ID_ICON_8 && g_new_menu_icons[7] != MENU_ICON_NONE) {

		gs_current_loctype = LOCTYPE_MARKET;
		i = 1;

	} else if (g_action == ACTION_ID_LEFT) {

		update_direction(3);

	} else if (g_action == ACTION_ID_RIGHT) {

		update_direction(1);

	} else if (g_action == ACTION_ID_UP) {

		bi = get_border_index(g_steptarget_front);

		if (!bi || bi == 7 || bi == 8) {
			seg066_14dd(1);
		} else if (bi >= 1 && bi <= 5 && g_entrance_angle == 2) {
			seg066_14dd(1);
		} else {
			no_way();
		}

	} else if (g_action == ACTION_ID_DOWN) {

		bi = get_border_index(g_steptarget_back);

		if (!bi || bi == 7 || bi == 8) {
			seg066_14dd(-1);
		} else {
			no_way();
		}
	}

	if (gs_current_town != TOWNS_NONE && g_city_area_loaded != -1) {

		if (!i) {
			options = enter_location(gs_current_town);
		}

		/* random city event? */
		/* check if the party has moved to another square */
		if ((gs_y_target != gs_y_target_bak || (gs_x_target != gs_x_target_bak)) &&

			/* only in big town */
			(gs_current_town == TOWNS_THORWAL || gs_current_town == TOWNS_PREM ||
			gs_current_town == TOWNS_PHEXCAER || gs_current_town == TOWNS_OBERORKEN))
		{

			if (random_schick(100) <= 1 && /* 1% chance */
				gs_day_timer > HOURS(8) && /* only between 8:00 and 20:00 o'clock */
				gs_day_timer < HOURS(20))
			{
				city_event_switch();
			}
		}

		if (g_location_market_flag && g_new_menu_icons[7] != MENU_ICON_MARKET) {

			if (((i = g_market_descr_table[gs_current_typeindex].market_day) == -1 || gs_day_of_week == i) &&
				gs_day_timer >= HOURS(6) && gs_day_timer <= HOURS(16))
			{
				g_new_menu_icons[7] = MENU_ICON_MARKET;
				draw_icons();
			}

		} else if (!g_location_market_flag && g_new_menu_icons[7] == MENU_ICON_MARKET) {

			g_new_menu_icons[7] = MENU_ICON_NONE;
			draw_icons();
		}
	}

	return 0;
}

void seg066_14dd(signed short forward)
{
	signed short dir;

	timewarp(MINUTES(2));

	dir = gs_direction;

	if (forward == 1) {

		if (!dir) {
			gs_y_target--;
		} else if (dir == 1) {
			gs_x_target++;
		} else if (dir == 2) {
			gs_y_target++;
		} else {
			gs_x_target--;
		}

	} else {

		if (!dir) {
			gs_y_target++;
		} else if (dir == 1) {
			gs_x_target--;
		} else if (dir == 2) {
			gs_y_target--;
		} else {
			gs_x_target++;
		}
	}

	if (gs_x_target < 0) {

		gs_x_target = 0;
		no_way();

	} else if (g_dng_map_size - 1 < gs_x_target) {

		gs_x_target = g_dng_map_size - 1;
		no_way();
	}

	if (gs_y_target < 0) {

		gs_y_target = 0;
		no_way();

	} else if (gs_y_target > 15) {

		gs_y_target = 15;
		no_way();
	}
}

void city_fade_and_colors(void)
{
	signed short i;
	Bit8u *dst;
	Bit8u *pal_ptr;

	if (g_fading_state == 2) {

		fade_into();
		g_fading_state = 1;

	}

	if (g_fading_state == 3) {

		set_palette(g_palette_allblack2, 0x00, 0x20);
		set_palette(g_palette_allblack2, 0x80, 0x20);
		set_palette(g_palette_allblack2, 0xa0, 0x20);

		g_fading_state = 1;
	}

	draw_compass();

	g_pic_copy.x1 = g_ani_posx;
	g_pic_copy.y1 = g_ani_posy;
	g_pic_copy.x2 = g_ani_posx + 207;
	g_pic_copy.y2 = g_ani_posy + 134;
	g_pic_copy.src = g_renderbuf_ptr;

	g_special_screen = 0;

	update_mouse_cursor();
	wait_for_vsync();

	do_pic_copy(1);

	refresh_screen_size();

	if (g_fading_state != 0) {

		dst = ((Bit8u*)g_renderbuf_ptr) + 500;
		pal_ptr = g_renderbuf_ptr;

		memset(g_renderbuf_ptr, 0, 0x120);
		memcpy(dst, gs_palette_floor, 0x120);

		for (i = 0; i < 64; i += 2) {

			pal_fade_in(pal_ptr, dst, i, 0x60);
			pal_fade_in(pal_ptr, dst, i + 1, 0x60);

			wait_for_vsync();

			set_palette(pal_ptr, 0x00, 0x20);
			set_palette(pal_ptr + 0x60, 0x80, 0x40);
		}

		g_fading_state = 0;

	} else {

		wait_for_vsync();

		set_palette(gs_palette_floor, 0x00, 0x20);
		set_palette(gs_palette_buildings, 0x80, 0x40);
	}
}

void seg066_172b(void)
{
	signed int l_si;
	signed int i;
	Bit8u *map_ptr = g_dng_map;

	g_city_house_count[0] = g_city_house_count[1]
				= g_city_house_count[2] = g_city_house_count[3] = 0;

	for (i = 0; g_dng_map_size * 16 > i; i++) {

		l_si = get_border_index(map_ptr[i]);

		/* count number of houses of certain kind */
		if (l_si == 2) {
			g_city_house_count[0]++;
		} else if (l_si == 3) {
			g_city_house_count[1]++;
		} else if ((l_si == 4) || (l_si == 1)) {
			g_city_house_count[2]++;
		} else if (l_si == 5) {
			g_city_house_count[3]++;
		}
	}

	i = 5;
	l_si = 2000;

	/* find house with lowest count on current map */
	if (g_city_house_count[0] < l_si) {
		l_si = g_city_house_count[(i = 0)];
	}

	if (g_city_house_count[1] < l_si) {
		l_si = g_city_house_count[(i = 1)];
	}

	if (g_city_house_count[2] < l_si) {
		l_si = g_city_house_count[(i = 2)];
	}

	if (g_city_house_count[3] < l_si) {
		l_si = g_city_house_count[(i = 3)];
	}

	/* the kind of house with lowest count is deactivated, i.e. it's texture is
	 * not loaded and replaced by another texture in seg028_0224 */
	g_city_house_count[i] = 0;
}

#if !defined(__BORLANDC__)
}
#endif

