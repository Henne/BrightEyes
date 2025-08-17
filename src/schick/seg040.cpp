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
	struct struct_fighter *fighter;
	signed short i;
	struct nvf_desc nvf;
	signed short handle;

	g_fig_list_head = NULL;

	g_fig_list_buffer = (struct struct_fighter*)(((HugePt)g_fig_figure2_buf) - 0x115d);

	memset(g_fig_list_buffer, 0, 0x115d);
	fighter = g_fig_list_buffer;

	for (i = 0; i < 127; i++) {
		fighter->id = -1;
		fighter++;
		g_fig_list_array[i] = 0;
	}

	g_weapons_nvf_buf =(HugePt)g_current_fight + SIZEOF_FIGHT;
	g_spellobj_nvf_buf = g_weapons_nvf_buf + 0x1953;
	g_figobj_gfxbuf_table = (unsigned char**)(((HugePt)g_spellobj_nvf_buf) + 0xf5fL);
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
		g_fightobj_list[i] = -1;
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
	g_buffer_anidat = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 9851;
	handle = load_archive_file(ARCHIVE_FILE_ANI_DAT);
	read_archive_file(handle, g_buffer_anidat, 9851);
	close(handle);

	/* load WEAPANI.DAT */
	g_buffer_weapanidat = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 1370;
	handle = load_archive_file(ARCHIVE_FILE_WEAPANI_DAT);
	read_archive_file(handle, g_buffer_weapanidat, 1370);
	close(handle);

	/* process NVFs */

	g_fig_cb_marker_buf = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 300;

	nvf.dst = g_fig_cb_marker_buf;
	nvf.src = g_objects_nvf_buf;
	nvf.no = 10;
	nvf.type = 0;
	nvf.width = (Bit8u*)&i;
	nvf.height = (Bit8u*)&i;
	process_nvf(&nvf);

	g_fig_cb_selector_buf = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 300;

	nvf.dst = g_fig_cb_selector_buf;
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

	g_fig_shot_bolt_buf = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 400;
	g_fig_spellgfx_buf = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 1300;

	/* TODO: check if pointer arithmetics works with other pointer types: NO! */
	g_fightobj_buf_freespace = (Bit32s)((HugePt)g_fightobj_buf - (Bit8u*)g_fightobj_buf_seek_ptr);

	g_fightobj_count = 0;
	g_fig_twofielded_count = 0;
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

					g_fig_list_elem.figure = 0;
					g_fig_list_elem.nvf_no = ((signed char)obj_id);
					g_fig_list_elem.cbx = ((signed char)cb_x);
					g_fig_list_elem.cby = ((signed char)cb_y);
					g_fig_list_elem.offsetx = (g_gfxtab_obj_offset_x[obj_id]);
					g_fig_list_elem.offsety = (g_gfxtab_obj_offset_y[obj_id]);
					g_fig_list_elem.height = (g_figobj_gfxheight_table[obj_id]);
					g_fig_list_elem.width = (g_figobj_gfxwidth_table[obj_id]);
					g_fig_list_elem.x1 = (0);
					g_fig_list_elem.y1 = (0);
					g_fig_list_elem.x2 = (g_figobj_gfxwidth_table[obj_id] - 1);
					g_fig_list_elem.y2 = (g_figobj_gfxheight_table[obj_id] - 1);
					g_fig_list_elem.is_enemy = (0);
					g_fig_list_elem.reload = (0);
					g_fig_list_elem.wsheet = (-1);
					g_fig_list_elem.sheet = (-1);
					g_fig_list_elem.gfxbuf = (ptr);
					g_fig_list_elem.z = (obj_id >= 58 && obj_id <= 61 ? -1 : 50);
					g_fig_list_elem.visible = (1);
					g_fig_list_elem.twofielded = (-1);

					g_fightobj_list[g_fightobj_count] = FIG_add_to_list(-1);
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

