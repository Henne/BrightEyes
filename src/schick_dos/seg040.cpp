/**
 *
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

static struct point8s g_cb_rear_border[15] = {
	{ 0x00, 0x10 },
	{ 0x01, 0x10 },
	{ 0x02, 0x10 },
	{ 0x03, 0x10 },
	{ 0x03, 0x11 },
	{ 0x03, 0x12 },
	{ 0x03, 0x13 },
	{ 0x04, 0x14 },
	{ 0x05, 0x14 },
	{ 0x06, 0x14 },
	{ 0x07, 0x14 },
	{ 0x07, 0x15 },
	{ 0x07, 0x16 },
	{ 0x07, 0x17 },
	{ -0x01, 0x00 }
}; // ds:0x6040, ; { , , , , , , , , , , , , , , {-1,0} }
static signed int g_fightobj_count = 0; // ds:0x605e
const signed int g_gfxtab_obj_offset_x[63] = { -0x0001, 0x0005, 0x0004, 0x0005, -0x0005, -0x0005, -0x0005, 0x0001, 0x0000, 0x0000, -0x0006, -0x0005, -0x0005, -0x0005, 0x0005, 0x0005, 0x0005, 0x0005, 0x0000, -0x0001, -0x0001, -0x0001, -0x0001, 0x0000, 0x0000, -0x0001, 0x0000, -0x0001, -0x0001, 0x0005, 0x0005, 0x0005, -0x0004, -0x0004, -0x0004, 0x0000, 0x0005, -0x0004, 0x0001, -0x0003, 0x0001, 0x0000, 0x0002, -0x0004, 0x0002, 0x0000, -0x0001, 0x0000, -0x0008, 0x0000, 0x0009, -0x0003, 0x0004, -0x0005, 0x0005, -0x0010, 0x0003, 0x0001, -0x0001, 0x0000, 0x0022, 0x0023, 0x0000 }; // ds:0x6060
const signed int g_gfxtab_obj_offset_y[63] = { 0x0006, 0x0006, 0x0006, 0x0008, 0x000a, 0x0005, 0x0005, 0x0005, 0x0004, 0x000a, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0003, 0x0004, 0x0003, 0x0002, 0x0002, 0x0002, 0x0003, 0x0003, 0x0003, 0x0004, 0x0003, 0x0003, 0x0003, 0x0003, 0x0005, 0x0004, 0x0002, 0x0004, 0x0002, 0x0003, 0x0003, 0x0004, 0x0003, 0x0003, 0x0004, 0x0002, 0x0004, 0x0002, 0x0007, 0x0006, 0x0003, 0x0007, 0x0004, 0x0005, 0x0003, 0x0001, 0x000b, 0x0003, 0x0003, 0x0028, 0x0001 }; // ds:0x60de


signed char g_fig_double_size_fighter_id_table[21]; // ds:0xe35a
signed char g_fig_double_size_count;	// ds:0xe36f
int32_t g_fightobj_buf_freespace;	// ds:0xe370
unsigned char *g_buffer_weapanidat;	// ds:0xe374, pointer to WEAPANI.DAT
unsigned char *g_buffer_anidat;		// ds:0xe378, pointer to ANI.DAT buffer
struct struct_fighter *g_fig_list_buffer;	// ds:0xe37c;
int16_t *g_figobj_gfxheight_table;	// ds:0xe380, to int16_t[63]
int16_t *g_figobj_gfxwidth_table;	// ds:0xe384, to int16_t[63]
unsigned char **g_figobj_gfxbuf_table;	// ds:0xe388, to long[63]; uint8_t*
signed char g_fig_spellgfx_id;		// ds:0xe38c
signed char g_fig_projectile_id;		// ds:0xe38d
signed char g_fig_cb_marker_id;		// ds:0xe38e
signed char g_fig_cb_selector_id[21];	// ds:0xe38f

void FIG_chessboard_init(void)
{
	signed int i;
	signed int cb_x;
	signed int cb_y;

	/* initialize the chessboard */
	memset(g_chessboard, 0, 600);

	for (cb_y = 0; cb_y < 24; cb_y++) {

		for (cb_x = 0; cb_x < 24; cb_x++) {

			signed int object_id = g_scenario_buf->board[cb_y * 25 + cb_x];

			if (object_id < 0) {

				FIG_set_cb_object(cb_y, cb_x, object_id);

			} else {

				if ((object_id >= 108) && (object_id <= 111)) {

					object_id -= 50;

					if (object_id == 60) {

						for (i = 1; i < 8; i++) {
							FIG_set_cb_object(cb_y + i, cb_x, -3);
						}

					} else if (object_id == 61) {

						for (i = 1; i < 8; i++) {
							FIG_set_cb_object(cb_y, cb_x + i, -3);
						}
					}

					FIG_set_cb_object(cb_y, cb_x, -3);
				}
			}
		}
	}

	i = 0;

	if (g_scenario_buf->fig_background_id <= FIG_BACKGROUND_ID_3) {

		while (g_cb_rear_border[i].x != -1) {

			FIG_set_cb_object(g_cb_rear_border[i].y, g_cb_rear_border[i].x, 50);
			i++;
		}
	}
}

void FIG_preload_gfx(void)
{
	struct struct_fighter *fighter;
	signed int i;
	struct nvf_extract_desc nvf;
	signed int handle;

	g_fig_list_head = NULL;

	g_fig_list_buffer = (struct struct_fighter*)(((HugePt)g_fig_figure2_buf) - 0x115d);

	memset(g_fig_list_buffer, 0, 0x115d);
	fighter = g_fig_list_buffer;

	for (i = 0; i < 127; i++) {
		fighter->id = -1;
		fighter++;
		g_fig_list_array[i] = 0;
	}

	g_weapons_nvf_buf =(HugePt)g_current_fight + sizeof(struct fight);
	g_spellobj_nvf_buf = g_weapons_nvf_buf + 0x1953;
	g_figobj_gfxbuf_table = (unsigned char**)(((HugePt)g_spellobj_nvf_buf) + 0xf5fL);
	g_figobj_gfxwidth_table = (int16_t*)(((HugePt)g_figobj_gfxbuf_table) + 0xfcL);
	g_figobj_gfxheight_table = (int16_t*)(((HugePt)g_figobj_gfxwidth_table) + 0x7eL);
	g_fightobj_buf_seek_ptr = (unsigned char*)(((HugePt)g_figobj_gfxheight_table) + 0x7eL);
	g_fightobj_buf = (unsigned char*)(((HugePt)g_fig_list_buffer) - 0x4217L);

	/* set something in the hero charactersheet to -1 */
	for (i = 0; i <= 6; i++) {
		get_hero(i)->fighter_id = -1;
	}

	for (i = 0; i < 20; i++) {
		g_enemy_sheets[i].monster_id = MONSTER_ID_NONE;
		g_enemy_sheets[i].fighter_id = -1;
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

	g_fig_projectile_id = -1;
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
	nvf.image_num = 10;
	nvf.compression_type = 0;
	nvf.width = &i;
	nvf.height = &i;
	process_nvf_extraction(&nvf);

	g_fig_cb_selector_buf = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 300;

	nvf.dst = g_fig_cb_selector_buf;
	nvf.src = g_objects_nvf_buf;
	nvf.image_num = 11;
	nvf.compression_type = 0;
	process_nvf_extraction(&nvf);

	g_fig_star_gfx = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 0xe8c;

	nvf.dst = g_fig_star_gfx;
	nvf.src = g_objects_nvf_buf;
	nvf.image_num = 17;
	nvf.compression_type = 0;
	process_nvf_extraction(&nvf);

	g_fig_projectile_buf = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 400;
	g_fig_spellgfx_buf = g_fightobj_buf_seek_ptr;
	g_fightobj_buf_seek_ptr += 1300;

	/* TODO: check if pointer arithmetics works with other pointer types: NO! */
	g_fightobj_buf_freespace = (int32_t)((HugePt)g_fightobj_buf - (uint8_t*)g_fightobj_buf_seek_ptr);

	g_fightobj_count = 0;
	g_fig_double_size_count = 0;
}

void FIG_draw_scenario(void)
{
	signed int cb_x;
	signed int cb_y;

	for (cb_x = 0; cb_x < 24; cb_x++) {

		for (cb_y = 0; cb_y < 24; cb_y++) {

			signed int object_id = g_scenario_buf->board[cb_y * 25 + cb_x];


			if ((object_id >= 50) && (object_id < 108 || object_id > 111)) {

				signed int width;
				signed int height;
				uint8_t *ptr;

				object_id -= 50;

				/* NULL check */
				if (g_figobj_gfxbuf_table[object_id]) {
					ptr = g_figobj_gfxbuf_table[object_id];
				} else {
					struct nvf_extract_desc nvf;

					ptr = g_fightobj_buf_seek_ptr;

					nvf.dst = ptr;
					nvf.src = g_fightobj_buf;
					nvf.image_num = object_id;
					nvf.compression_type = 0;
					nvf.width = &width;
					nvf.height = &height;
					process_nvf_extraction(&nvf);

					/* save sprite info */
					g_figobj_gfxbuf_table[object_id] = g_fightobj_buf_seek_ptr;
					g_figobj_gfxwidth_table[object_id] = width;
					g_figobj_gfxheight_table[object_id] = height;

					/* adjust pointer */
					g_fightobj_buf_seek_ptr += width * height + 8;
					g_fightobj_buf_freespace -= width * height + 8L;
				}

				g_fig_list_elem.figure = 0;
				g_fig_list_elem.nvf_no = object_id;
				g_fig_list_elem.cbx = cb_x;
				g_fig_list_elem.cby = cb_y;
				g_fig_list_elem.offsetx = g_gfxtab_obj_offset_x[object_id];
				g_fig_list_elem.offsety = g_gfxtab_obj_offset_y[object_id];
				g_fig_list_elem.height = g_figobj_gfxheight_table[object_id];
				g_fig_list_elem.width = g_figobj_gfxwidth_table[object_id];
				g_fig_list_elem.x1 = 0;
				g_fig_list_elem.y1 = 0;
				g_fig_list_elem.x2 = g_figobj_gfxwidth_table[object_id] - 1;
				g_fig_list_elem.y2 = g_figobj_gfxheight_table[object_id] - 1;
				g_fig_list_elem.is_enemy = 0;
				g_fig_list_elem.reload = 0;
				g_fig_list_elem.ani_track_id_weapon = FANI_TRACK_ID_NONE;
				g_fig_list_elem.ani_track_id_base = FANI_TRACK_ID_NONE;
				g_fig_list_elem.gfxbuf = ptr;
				g_fig_list_elem.z = (object_id >= 58 && object_id <= 61 ? -1 : 50);
				g_fig_list_elem.visible = 1;
				g_fig_list_elem.double_size = -1;

				g_fightobj_list[g_fightobj_count] = FIG_add_to_list(-1);
				g_fightobj_count++;

				place_obj_on_cb(cb_x, cb_y, object_id + 50, object_id, 0);
			}
		}
	}
}
