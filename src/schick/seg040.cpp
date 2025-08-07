/**
 *	Rewrite of DSA1 v3.02_de functions of seg040 (scenarios)
 *	Functions rewritten: 3/3 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg040.cpp
 */
#include <string.h>

#if !defined(__BORLANDC__)
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg032.h"
#include "seg039.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void FIG_chessboard_init(void)
{
	signed short i;
	signed short cb_x;
	signed short cb_y;
	signed short obj_id;

	/* initialize the chessboard */
	memset(g_chessboard, 0, 600);

	for (cb_y = 0; cb_y < 24; cb_y++) {

		for (cb_x = 0; cb_x < 24; cb_x++) {

			obj_id = *(g_scenario_buf + cb_y * 25 + cb_x + 0x15);

			if (obj_id < 0) {

				FIG_set_cb_field(cb_y, cb_x, obj_id);

			} else {

				if (obj_id >= 0x6c && obj_id <= 0x6f) {

					obj_id -= 0x32;

					if (obj_id == 0x3c) {

						for (i = 1; i < 8; i++) {
							FIG_set_cb_field(cb_y + i, cb_x, -3);
						}

					} else if (obj_id == 0x3d) {

						for (i = 1; i < 8; i++) {
							FIG_set_cb_field(cb_y, cb_x + i, -3);
						}
					}

					FIG_set_cb_field(cb_y, cb_x, -3);
				}
			}
		}
	}

	i = 0;

	if (g_scenario_buf[0x14] <= 3) {

		while (ds_readbs(CB_REAR_BORDER + i * 2) != -1) {

			FIG_set_cb_field(ds_readbs((CB_REAR_BORDER + 1) + i * 2), ds_readbs(CB_REAR_BORDER + i * 2), 50);
			i++;
		}
	}
}

void FIG_preload_gfx(void)
{
	Bit8u *p1;
	signed short i;
	struct nvf_desc nvf;
	signed short handle;

	ds_writed(FIG_LIST_HEAD, 0);

	g_fig_list_buffer = (Bit8u*)(((HugePt)g_fig_figure2_buf) - 0x115d);

	memset(g_fig_list_buffer, 0, 0x115d);
	p1 = g_fig_list_buffer;

	for (i = 0; i < 127; i++) {
		host_writeb(p1 + 0x10, -1);
		p1 += SIZEOF_FIGHTER;
		ds_writeb(FIG_LIST_ARRAY + i, 0);
	}

	ds_writed(WEAPONS_NVF_BUF, (Bit32u)(((HugePt)g_current_fight) + SIZEOF_FIGHT));
	ds_writed(SPELLOBJ_NVF_BUF, (Bit32u)((Bit8u*)ds_readd(WEAPONS_NVF_BUF) + 0x1953));
	g_figobj_gfxbuf_table = (unsigned char**)(((HugePt)ds_readd(SPELLOBJ_NVF_BUF)) + 0xf5fL);
	g_figobj_gfxwidth_table = (signed short*)(((HugePt)g_figobj_gfxbuf_table) + 0xfcL);
	g_figobj_gfxheight_table = (signed short*)(((HugePt)g_figobj_gfxwidth_table) + 0x7eL);
	g_fightobj_buf_seek_ptr = (unsigned char*)(((HugePt)g_figobj_gfxheight_table) + 0x7eL);
	g_fightobj_buf = (unsigned char*)(((HugePt)g_fig_list_buffer) - 0x4217L);

	/* set something in the hero charactersheet to -1 */
	for (i = 0; i <= 6; i++) {
		host_writeb(get_hero(i) + HERO_FIGHTER_ID, -1);
	}

	for (i = 0; i < 20; i++) {
		ds_writeb(i * SIZEOF_ENEMY_SHEET + ENEMY_SHEETS + ENEMY_SHEET_MON_ID, 0);
		ds_writeb(i * SIZEOF_ENEMY_SHEET + (ENEMY_SHEETS + ENEMY_SHEET_FIGHTER_ID), -1);
	}

	for (i = 0; i < 90; i++) {
		ds_writeb(FIGHTOBJ_LIST + i, -1);
	}

	for (i = 0; i <= 62; i++) {
		g_figobj_gfxbuf_table[i] = NULL;
		g_figobj_gfxwidth_table[i] = g_figobj_gfxheight_table[i] = 0;
	}

	g_fig_cb_marker_id = -1;

	for (i = 0; i < 20; i++) {
		g_fig_cb_selector_id[i] = -1;
	}

	g_fig_shot_bolt_id = -1;
	g_fig_spellgfx_id = -1;

	/* load ANI.DAT */
	ds_writed(BUFFER_ANIDAT, (Bit32u)g_fightobj_buf_seek_ptr);
	g_fightobj_buf_seek_ptr += 9851;
	handle = load_archive_file(ARCHIVE_FILE_ANI_DAT);
	read_archive_file(handle, (Bit8u*)ds_readd(BUFFER_ANIDAT), 9851);
	close(handle);

	/* load WEAPANI.DAT */
	ds_writed(BUFFER_WEAPANIDAT, (Bit32u)g_fightobj_buf_seek_ptr);
	g_fightobj_buf_seek_ptr += 1370;
	handle = load_archive_file(ARCHIVE_FILE_WEAPANI_DAT);
	read_archive_file(handle, (Bit8u*)ds_readd(BUFFER_WEAPANIDAT), 1370);
	close(handle);

	/* process NVFs */

	ds_writed(FIG_CB_MARKER_BUF, (Bit32u)g_fightobj_buf_seek_ptr);
	g_fightobj_buf_seek_ptr += 300;

	nvf.dst = (Bit8u*)ds_readd(FIG_CB_MARKER_BUF);
	nvf.src = g_objects_nvf_buf;
	nvf.no = 10;
	nvf.type = 0;
	nvf.width = (Bit8u*)&i;
	nvf.height = (Bit8u*)&i;
	process_nvf(&nvf);

	ds_writed(FIG_CB_SELECTOR_BUF, (Bit32u)g_fightobj_buf_seek_ptr);
	g_fightobj_buf_seek_ptr += 300;

	nvf.dst = (Bit8u*)ds_readd(FIG_CB_SELECTOR_BUF);
	nvf.src = g_objects_nvf_buf;
	nvf.no = 11;
	nvf.type = 0;
	process_nvf(&nvf);

	g_fig_star_gfx = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 0xe8c;

	nvf.dst = g_fig_star_gfx;
	nvf.src = g_objects_nvf_buf;
	nvf.no = 17;
	nvf.type = 0;
	process_nvf(&nvf);

	ds_writed(FIG_SHOT_BOLT_BUF, (Bit32u)g_fightobj_buf_seek_ptr);
	g_fightobj_buf_seek_ptr += 400;
	ds_writed(FIG_SPELLGFX_BUF, (Bit32u)g_fightobj_buf_seek_ptr);
	g_fightobj_buf_seek_ptr += 1300;

	/* TODO: check if pointer arithmetics works with other pointer types: NO! */
	g_fightobj_buf_freespace = (Bit32s)((HugePt)g_fightobj_buf - (Bit8u*)g_fightobj_buf_seek_ptr);

	g_fightobj_count = 0;
	ds_writeb(FIG_TWOFIELDED_COUNT, 0);
}

void FIG_draw_scenario(void)
{
	signed short cb_x;
	signed short cb_y;
	signed short obj_id;
	signed short width;
	signed short height;
	Bit8u *ptr;
	struct nvf_desc nvf;


	for (cb_x = 0; cb_x < 24; cb_x++) {
		for (cb_y = 0; cb_y < 24; cb_y++) {

			obj_id = *(g_scenario_buf + cb_y * 25 + cb_x + 0x15);

			if (obj_id >= 0x32) {
				if (obj_id < 0x6c || obj_id > 0x6f) {

					obj_id -= 50;

					/* NULL check */
					if (g_figobj_gfxbuf_table[obj_id]) {
						ptr = g_figobj_gfxbuf_table[obj_id];
					} else {
						ptr = g_fightobj_buf_seek_ptr;

						nvf.dst = ptr;
						nvf.src = g_fightobj_buf;
						nvf.no = obj_id;
						nvf.type = 0;
						nvf.width = (Bit8u*)&width;
						nvf.height = (Bit8u*)&height;
						process_nvf(&nvf);
#if !defined(__BORLANDC__)
						/* BE-fix */
						width = host_readws((Bit8u*)&width);
						height = host_readws((Bit8u*)&height);
#endif

						/* save sprite info */
						g_figobj_gfxbuf_table[obj_id] = g_fightobj_buf_seek_ptr;
						g_figobj_gfxwidth_table[obj_id] = width;
						g_figobj_gfxheight_table[obj_id] = height;

						/* adjust pointer */
						g_fightobj_buf_seek_ptr += width * height + 8;
						g_fightobj_buf_freespace -= width * height + 8L;
					}

					ds_writew(FIG_LIST_ELEM, 0);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_NVF_NO), (signed char)obj_id);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_CBX), (signed char)cb_x);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_CBY), (signed char)cb_y);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETX), g_gfxtab_obj_offset_x[obj_id]);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_OFFSETY), g_gfxtab_obj_offset_y[obj_id]);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_HEIGHT), g_figobj_gfxheight_table[obj_id]);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_WIDTH), g_figobj_gfxwidth_table[obj_id]);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_X1), 0);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_Y1), 0);
					ds_writebs((FIG_LIST_ELEM+FIGHTER_X2), (g_figobj_gfxwidth_table[obj_id] - 1));
					ds_writebs((FIG_LIST_ELEM+FIGHTER_Y2), (g_figobj_gfxheight_table[obj_id] - 1));
					ds_writeb((FIG_LIST_ELEM+FIGHTER_IS_ENEMY), 0);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_RELOAD), 0);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_WSHEET), -1);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_SHEET), -1);
					ds_writed((FIG_LIST_ELEM+FIGHTER_GFXBUF), (Bit32u)ptr);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_Z), obj_id >= 58 && obj_id <= 61 ? -1 : 50);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_VISIBLE), 1);
					ds_writeb((FIG_LIST_ELEM+FIGHTER_TWOFIELDED), -1);
					ds_writeb(FIGHTOBJ_LIST + g_fightobj_count, FIG_add_to_list(-1));
					g_fightobj_count++;
#if !defined(__BORLANDC__)
					place_obj_on_cb(cb_x, cb_y, obj_id + 50, (signed char)obj_id, 0);
#else
					place_obj_on_cb(cb_x, cb_y, obj_id + 50, obj_id, 0);
#endif
				}
			}
		}
	}
}

#if !defined(__BORLANDC__)
}
#endif

