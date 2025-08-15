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

#if defined(__BORLANDC__)
signed short get_border_index(unsigned char);
#endif

signed short enter_location(signed short town_id)
{
	signed short map_pos;
	signed short b_index;
	Bit8u *locations_list_ptr;

	if (town_id == TOWNS_DASPOTA) {
		return enter_location_daspota();
	}

	map_pos = 256 * gs_x_target + gs_y_target;
	locations_list_ptr = p_datseg + LOCATIONS_LIST;
	ds_writeb(LOCATION_MARKET_FLAG, 0);

	do {
		if (host_readws(locations_list_ptr + LOCATION_XY) == map_pos) {

			/* found the location */
			gs_current_loctype_bak = LOCTYPE_NONE;
			gs_current_loctype = host_readbs(locations_list_ptr + LOCATION_LOCTYPE);
			gs_current_typeindex = host_readb(locations_list_ptr + LOCATION_TYPEINDEX);
			gs_current_locdata = (host_readw(locations_list_ptr + LOCATION_LOCDATA));

			if (gs_current_loctype == LOCTYPE_MARKET) {
				gs_current_loctype = LOCTYPE_NONE;
				ds_writeb(LOCATION_MARKET_FLAG, 1);
			}

			return 1;
		}

		locations_list_ptr += SIZEOF_LOCATION;

	} while (host_readws(locations_list_ptr) != -1);

	move();

	if ((b_index = get_border_index(cast_u16(ds_readbs((VISUAL_FIELD_VALS + 1))))) >= 2 && b_index <= 5) {

		gs_current_loctype_bak = LOCTYPE_NONE;
		gs_current_locdata = (ds_readb((TOWNS_CITYINDEX_TABLE-1) + town_id));

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
	Bit8u *locations_list_ptr;

	if (ds_readws(GAME_STATE) == GAME_STATE_FIGQUIT) {
		return 1;
	}

	map_pos = 256 * gs_x_target + gs_y_target;
	locations_list_ptr = p_datseg + LOCATIONS_LIST;
	ds_writeb(LOCATION_MARKET_FLAG, 0);

	do {

		if (host_readws(locations_list_ptr + LOCATION_XY) == map_pos) {

			gs_current_typeindex = host_readb(locations_list_ptr + LOCATION_TYPEINDEX);

			if (host_readb(locations_list_ptr + LOCATION_LOCTYPE) != LOCTYPE_SIGNPOST) {

				GUI_print_loc_line(get_tx(host_readw(locations_list_ptr + LOCATION_LOCDATA)));

				if (!gs_daspota_fightflags[host_readw(locations_list_ptr + LOCATION_LOCDATA)]) {

					do_talk(host_readbs(locations_list_ptr + LOCATION_LOCTYPE), host_readb(locations_list_ptr + LOCATION_TYPEINDEX) - 1);

					if (!gs_daspota_fightflags[host_readw(locations_list_ptr + LOCATION_LOCDATA)]) {
						leave_location();
						return 1;
					}
				}

				draw_main_screen();
				set_var_to_zero();

				load_ani(10);
				GUI_print_loc_line(get_tx(host_readw(locations_list_ptr + LOCATION_LOCDATA)));
				init_ani(0);

				if (ds_readd((DASPOTA_LOCLOOT_INDEX - 4) + 4 * host_readw(locations_list_ptr + LOCATION_LOCDATA))) {

					loot_multi_chest((Bit8u*)ds_readd((DASPOTA_LOCLOOT_INDEX - 4) + 4 * host_readw(locations_list_ptr + LOCATION_LOCDATA)), get_tx(21));

				} else {

					do {
						handle_gui_input();

					} while (ds_readws(ACTION) == 0 && ds_readws(MOUSE1_EVENT2) == 0);

					ds_writew(MOUSE1_EVENT2, 0);
				}

				set_var_to_zero();

				if (host_readw(locations_list_ptr + LOCATION_LOCDATA) == 6) {
					do_fight(FIGHTS_DASP6B);
				} else if (host_readw(locations_list_ptr + LOCATION_LOCDATA) == 12) {
					do_fight(FIGHTS_DASP12B);
				}

				leave_location();

			} else {
				gs_current_loctype_bak = LOCTYPE_NONE;
				gs_current_loctype = host_readbs(locations_list_ptr + LOCATION_LOCTYPE);
				gs_current_locdata = (host_readw(locations_list_ptr + LOCATION_LOCDATA));
			}

			return 1;
		}

		locations_list_ptr += SIZEOF_LOCATION;

	} while (host_readws(locations_list_ptr) != -1);

	move();

	if ((b_index = get_border_index(cast_u16(ds_readb((VISUAL_FIELD_VALS + 1))))) >= 2 && b_index <= 5) {

		gs_current_loctype_bak = LOCTYPE_NONE;
		gs_current_loctype = LOCTYPE_CITIZEN;
		gs_current_locdata = (19);
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
	signed short i;
	Bit8u *hero;

	if (!state) {

		g_dialog_next_state = (gs_hermit_visited ? 1 : 2);

	} else if (state == 6) {

		hero = get_hero(0);
		for (i = 0 ; i <= 6; i++, hero += SIZEOF_HERO) {

			/* remove hunger and thirst */
			host_writeb(hero + HERO_HUNGER, host_writebs(hero + HERO_THIRST, 0));

			/* heal all wounds */
			add_hero_le(hero, host_readws(hero + HERO_LE_ORIG));
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
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;

	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	nvf.dst = ((Bit8u*)g_renderbuf_ptr) + 208 * height;
	nvf.src = g_tex_floor[0];
	nvf.no = 0;
	nvf.type = 3;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;

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
	signed short bi;

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 3)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 3), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 7)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 7), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 8)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 8), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 14)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 14), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 15)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 15), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 21)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 21), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 0)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 9), ds_writeb((VISUAL_FIELD_VALS + 15), ds_writeb((VISUAL_FIELD_VALS + 22), 0)));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 4), ds_writeb((VISUAL_FIELD_VALS + 10), ds_writeb((VISUAL_FIELD_VALS + 16), ds_writeb((VISUAL_FIELD_VALS + 23), 0))));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 2)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 13), ds_writeb((VISUAL_FIELD_VALS + 21), ds_writeb((VISUAL_FIELD_VALS + 28),0)));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 6), ds_writeb((VISUAL_FIELD_VALS + 12), ds_writeb((VISUAL_FIELD_VALS + 20), ds_writeb((VISUAL_FIELD_VALS + 27), 0))));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 4)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 9), ds_writeb((VISUAL_FIELD_VALS + 16), ds_writeb((VISUAL_FIELD_VALS + 22), ds_writeb((VISUAL_FIELD_VALS + 23), 0))));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 10), 0);
		}

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 10), ds_writeb((VISUAL_FIELD_VALS + 17), ds_writeb((VISUAL_FIELD_VALS + 18), 0)));
			ds_writeb((VISUAL_FIELD_VALS + 24), ds_writeb((VISUAL_FIELD_VALS + 25), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 11), ds_writeb((VISUAL_FIELD_VALS + 17), ds_writeb((VISUAL_FIELD_VALS + 18), 0)));

		ds_writeb((VISUAL_FIELD_VALS + 19), ds_writeb((VISUAL_FIELD_VALS + 24), ds_writeb((VISUAL_FIELD_VALS + 25), 0)));

		ds_writeb((VISUAL_FIELD_VALS + 26), 0);
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 6)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 13), ds_writeb((VISUAL_FIELD_VALS + 20), ds_writeb((VISUAL_FIELD_VALS + 27), ds_writeb((VISUAL_FIELD_VALS + 28), 0))));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 5)))) >= 2 && bi <= 5) {

			ds_writeb((VISUAL_FIELD_VALS + 11), ds_writeb((VISUAL_FIELD_VALS + 12), ds_writeb((VISUAL_FIELD_VALS + 17), 0)));

			ds_writeb((VISUAL_FIELD_VALS + 18), ds_writeb((VISUAL_FIELD_VALS + 19), ds_writeb((VISUAL_FIELD_VALS + 24), 0)));

			ds_writeb((VISUAL_FIELD_VALS + 25), ds_writeb((VISUAL_FIELD_VALS + 26), 0));
		}

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {

			ds_writeb((VISUAL_FIELD_VALS + 12), ds_writeb((VISUAL_FIELD_VALS + 18), ds_writeb((VISUAL_FIELD_VALS + 19), 0)));

			ds_writeb((VISUAL_FIELD_VALS + 25), ds_writeb((VISUAL_FIELD_VALS + 26), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 9)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 22), 0);

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 17)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 16), ds_writeb((VISUAL_FIELD_VALS + 23), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 13)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 28), 0);

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 19)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 20), ds_writeb((VISUAL_FIELD_VALS + 27), 0));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 10)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 9), ds_writeb((VISUAL_FIELD_VALS + 16), ds_writeb((VISUAL_FIELD_VALS + 22), ds_writeb((VISUAL_FIELD_VALS + 23), 0))));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 18), 0);
		}

		if (((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) ||
			((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 18)))) >= 2 && bi <= 5)) {

			ds_writeb((VISUAL_FIELD_VALS + 17), ds_writeb((VISUAL_FIELD_VALS + 24), ds_writeb((VISUAL_FIELD_VALS + 25), 0)));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 12)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 13), ds_writeb((VISUAL_FIELD_VALS + 20), ds_writeb((VISUAL_FIELD_VALS + 27), ds_writeb((VISUAL_FIELD_VALS + 28), 0))));

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 18), 0);
		}

		if (((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) ||
			((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 18)))) >= 2 && bi <= 5)) {

			ds_writeb((VISUAL_FIELD_VALS + 19), ds_writeb((VISUAL_FIELD_VALS + 25), ds_writeb((VISUAL_FIELD_VALS + 26), 0)));
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 11)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 18), ds_writeb((VISUAL_FIELD_VALS + 25), 0));
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 16)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 22), 0);

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 24)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 23), 0);
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 20)))) >= 2 && bi <= 5) {

		ds_writeb((VISUAL_FIELD_VALS + 28), 0);

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 26)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 27), 0);
		}
	}

	if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 18)))) >= 2 && bi <= 5) {
		ds_writeb((VISUAL_FIELD_VALS + 25), 0);
	}

	if (((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 18)))) >= 2 && bi <= 5) ||
		((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 25)))) >= 2 && bi <= 5)) {

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 17)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 24), 0);
		}

		if ((bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + 19)))) >= 2 && bi <= 5) {
			ds_writeb((VISUAL_FIELD_VALS + 26), 0);
		}
	}
}

#if !defined(__BORLANDC__)
signed short get_border_index(unsigned short val)
#else
signed short get_border_index(unsigned char val)
#endif
{
	signed short i;

	i = 0;
	while (ds_readb(MAPVAL_TO_LOCTYPE + i) < val) {
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
	signed short i;
	signed short bi;

	for (i = 28; i >= 0; i--) {

		bi = get_border_index(ds_readb((VISUAL_FIELD_VALS + i)));

		if (bi == -1) {
			ds_writebs(VISUAL_FIELDS_TEX + i, -1);
		} else {
			ds_writebs(VISUAL_FIELDS_TEX + i,	bi == 2 ? ds_readbs(SEG066_0BAD_UNKN0 + i) : (
						bi == 3 ? ds_readbs(SEG066_0BAD_UNKN1 + i) : (
						bi == 4 ? ds_readbs(SEG066_0BAD_UNKN2 + i) : (
						bi == 5 ? ds_readbs(SEG066_0BAD_UNKN3 + i) : (
						bi == 6 ? ds_readbs(SEG066_0BAD_UNKN4 + i) : (
						bi == 7 ? ds_readbs(SEG066_0BAD_UNKN4 + i) : (
						bi == 8 ? ds_readbs(SEG066_0BAD_UNKN5 + i) : (
						bi == 9 ? ds_readbs(SEG066_0BAD_UNKN6 + i) : (
						bi == 10 ? ds_readbs(SEG066_0BAD_UNKN7 + i) : (
						bi == 1 ? ds_readbs(SEG066_0BAD_UNKN2 + i) : -1))))))))));
		}
	}
}

/**
 * \brief   draws water and grass textures
 */
void city_water_and_grass(void)
{
	signed short i;
	signed short nvf_no;
	signed short x;
	signed short y;
	signed char c1;
	signed char bi;
	unsigned char c2;
	Bit8u *ptr;

	for (i = 0; i < 29; i++) {

		c1 = g_visual_field_draw_order[i];
		c2 = ds_readb(VISUAL_FIELD_VALS + c1);

		if (c2 != 0) {

			bi = (signed char)get_border_index(c2);

			if (bi == 6 || bi == 7) {
				/* water or grass */

				ptr = (Bit8u*)&g_visual_field_offsets_grass[c1];

				x = host_readws(ptr);
				y = host_readws(ptr + 2);

				c1 = ds_readbs(VISUAL_FIELDS_TEX + c1);

				if (c1 != -1) {

					ptr = &g_tex_descr_table[c1 - 1][0];

					if ((nvf_no = host_readws(ptr + 4)) != -1) {

						if (bi == 7) {
							nvf_no += 15;
						}

						load_city_texture(x + host_readws(ptr), y + host_readws(ptr + 2), nvf_no, 184);
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
	Bit8u *ptr;

	for (i = 0; i < 29; i++) {

		c1 = g_visual_field_draw_order[i];
		c2 = ds_readb(VISUAL_FIELD_VALS + c1);

		if (c2 != 0) {

			bi = (signed char)get_border_index(c2);

			if (bi != 7 && bi != 6) {
			    /* if not grass or water */

				ptr = (Bit8u*)&g_visual_field_offsets_std[c1];

				if (bi == 8) {
					/* direction sign */
					ptr = (Bit8u*)&g_visual_field_offsets_sign[c1];
				} else if (bi == 9 || bi == 10) {
					/* tavern/inn or shop */
					ptr = (Bit8u*)&g_visual_field_offsets_inn[c1];
				}

				x = host_readws(ptr);
				y = host_readws(ptr + 2);

				c1 = ds_readbs(VISUAL_FIELDS_TEX + c1);

				if (c1 != -1) {

					ptr = &g_tex_descr_table[c1 - 1][0];

					l4 =	bi == 2 ? 186 : (
						bi == 3 ? 187 : (
						bi == 4 ? 188 : (
						bi == 5 ? 189 : (
						bi == 1 ? 188 : (
						bi == 9 ? 232 : (
						bi == 10 ? 233 : 185))))));

					if ((nvf_no = host_readws(ptr + 4)) != -1) {

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

						load_city_texture(x + host_readws(ptr), y + host_readws(ptr + 2), nvf_no, l4);

						if (bi == 9 || bi == 10) {
							call_load_buffer();
						}
					}

					if ((nvf_no = host_readws(ptr + 10)) != -1) {

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

						load_city_texture(x + host_readws(ptr + 6), y + host_readws(ptr + 8), nvf_no, l4);
					}

					if ((nvf_no = host_readws(ptr + 0x10)) != -1) {
						load_city_texture(x + host_readws(ptr + 12), y + host_readws(ptr + 14), nvf_no, l4);
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
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	height = host_readws((Bit8u*)&height);
	width = host_readws((Bit8u*)&width);
#endif

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

	ds_writebs((NEW_MENU_ICONS + 0), MENU_ICON_SPLIT_GROUP);
	l4 = ds_readbs((NEW_MENU_ICONS + 1));
	ds_writebs((NEW_MENU_ICONS + 1), g_can_merge_group == -1 ? MENU_ICON_MERGE_GROUP_GRAYED : MENU_ICON_MERGE_GROUP);

	if (ds_readbs((NEW_MENU_ICONS + 1)) != l4) {
		ds_writew(REDRAW_MENUICONS, 1);
	}

	ds_writebs((NEW_MENU_ICONS + 2), MENU_ICON_SWITCH_GROUP);
	ds_writebs((NEW_MENU_ICONS + 3), MENU_ICON_INFO);
	ds_writebs((NEW_MENU_ICONS + 4), MENU_ICON_MAP);
	ds_writebs((NEW_MENU_ICONS + 5), MENU_ICON_MAGIC);
	ds_writebs((NEW_MENU_ICONS + 6), MENU_ICON_CAMP);

	if (g_request_refresh != 0) {

		draw_main_screen();
		GUI_print_loc_line(get_tx(0));

		g_request_refresh = ds_writews(REDRAW_MENUICONS, 0);
		g_city_refresh_x_target = -1;
	}

	if (ds_readw(REDRAW_MENUICONS) != 0 && g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {
		draw_icons();
		ds_writews(REDRAW_MENUICONS, 0);
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

	if (ds_readw(MOUSE2_EVENT) != 0 || ds_readws(ACTION) == ACTION_ID_PAGE_UP) {

		for (i = options = 0; i < 9; i++) {
			if (ds_readbs(NEW_MENU_ICONS + i) != MENU_ICON_NONE) {
				options++;
			}
		}

		i = GUI_radio(get_ttx(570), (signed char)options,
				get_ttx(535), get_ttx(536), get_ttx(537),
				get_ttx(538), get_ttx(539), get_ttx(213),
				get_ttx(306), get_ttx(569)) - 1;

		if (i != -2) {
			ds_writew(ACTION, i + ACTION_ID_ICON_1);
		}
	}

	i = 0;

	if (ds_readws(ACTION) == ACTION_ID_ICON_1) {

		GRP_split();
		g_can_merge_group = can_merge_group();

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_2) {

		GRP_merge();
		g_can_merge_group = -1;

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_3) {

		GRP_switch_to_next(0);
		i = 1;

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_4) {

		game_options();

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_5) {

		show_automap();

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_6) {

		select_magic_user();

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_7) {

		gs_current_loctype = LOCTYPE_CITYCAMP;
		g_citycamp_city = 1; /* CITYCAMP takes place in a town */
		i = 1;

	} else if (ds_readws(ACTION) == ACTION_ID_ICON_8 && ds_readbs((NEW_MENU_ICONS + 7)) != MENU_ICON_NONE) {

		gs_current_loctype = LOCTYPE_MARKET;
		i = 1;

	} else if (ds_readws(ACTION) == ACTION_ID_LEFT) {

		update_direction(3);

	} else if (ds_readws(ACTION) == ACTION_ID_RIGHT) {

		update_direction(1);

	} else if (ds_readws(ACTION) == ACTION_ID_UP) {

		bi = get_border_index(ds_readb(STEPTARGET_FRONT));

		if (!bi || bi == 7 || bi == 8) {
			seg066_14dd(1);
		} else if (bi >= 1 && bi <= 5 && g_entrance_angle == 2) {
			seg066_14dd(1);
		} else {
			no_way();
		}

	} else if (ds_readws(ACTION) == ACTION_ID_DOWN) {

		bi = get_border_index(ds_readb(STEPTARGET_BACK));

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
		if ((gs_y_target != gs_y_target_bak ||
			(gs_x_target != gs_x_target_bak)) &&

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

		if (ds_readb(LOCATION_MARKET_FLAG) != 0 && ds_readb((NEW_MENU_ICONS + 7)) != MENU_ICON_MARKET) {

			if (((i = ds_readws((MARKET_DESCR_TABLE + 4) + 8 * gs_current_typeindex)) == -1 ||
				gs_day_of_week == i) &&
				gs_day_timer >= HOURS(6) &&
				gs_day_timer <= HOURS(16))
			{
				ds_writebs((NEW_MENU_ICONS + 7), MENU_ICON_MARKET);
				draw_icons();
			}

		} else if (!ds_readbs(LOCATION_MARKET_FLAG) && ds_readbs((NEW_MENU_ICONS + 7)) == MENU_ICON_MARKET) {

			ds_writebs((NEW_MENU_ICONS + 7), MENU_ICON_NONE);
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

		gs_x_target = (0);
		no_way();

	} else if (g_dng_map_size - 1 < gs_x_target) {

		gs_x_target = (g_dng_map_size - 1);
		no_way();

	}

	if (gs_y_target < 0) {

		gs_y_target = (0);
		no_way();

	} else if (gs_y_target > 15) {

		gs_y_target = (15);
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
	signed short l_si;
	signed short l_di;
	Bit8u *ptr = g_dng_map;

	g_city_house_count[0] = g_city_house_count[1]
				= g_city_house_count[2] = g_city_house_count[3] = 0;

	for (l_di = 0; g_dng_map_size * 16 > l_di; l_di++) {

		l_si = get_border_index(host_readb(ptr + l_di));

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

	l_di = 5;
	l_si = 2000;

	/* find house with lowest count on current map */
	if (g_city_house_count[0] < l_si) {
		l_si = g_city_house_count[(l_di = 0)];
	}

	if (g_city_house_count[1] < l_si) {
		l_si = g_city_house_count[(l_di = 1)];
	}

	if (g_city_house_count[2] < l_si) {
		l_si = g_city_house_count[(l_di = 2)];
	}

	if (g_city_house_count[3] < l_si) {
		l_si = g_city_house_count[(l_di = 3)];
	}

	/* the kind of house with lowest count is deactivated, i.e. it's texture is
	 * not loaded and replaced by another texture in seg028_0224 */
	g_city_house_count[l_di] = 0;
}

#if !defined(__BORLANDC__)
}
#endif

