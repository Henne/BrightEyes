/*
 *	Rewrite of DSA1 v3.02_de functions of seg028 (file loader: maps, npcs, special textures, dialogs, fight backgrounds)
 *	Functions rewritten: 19/19 (complete)
 *
 *	Functions called rewritten: 16/16
 *	Functions uncalled rewritten: 3/3
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg028.cpp
 */
#include <string.h>

#if defined(__BORLANDC__)
#include <DOS.H>
#include <IO.H>
#else
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg004.h"
#include "seg008.h"
#include "seg009.h"
#include "seg010.h"
#include "seg026.h"
#include "seg028.h"
#include "seg066.h"

static signed int g_locations_tab_size = 0;	// ds:0x5eb8
static signed int g_areadescr_dng_level = 0;	// ds:0x5eba
static signed int g_areadescr_fileid = 0;	// ds:0x5ebc
static signed int g_areadescr_dng_flag = 0;	// ds:0x5ebe
#if defined(__BORLANDC__)
static unsigned int g_ems_unused_lpage = 0;	// ds:0x5ec0
static signed int g_ems_unused_offset = 0x0300; // ds:0x5ec2
#endif
static const unsigned char g_unkn_039[8] = { 0x0b, 0xc0, 0x75, 0x06, 0xb8, 0x01, 0x00, 0x00 }; // ds:0x5ec4

void prepare_dungeon_area(void)
{
	signed int dtx_index;
	uint32_t file_len;
	uint32_t offset;
	HugePt buf;

	signed int nvf_index;
	signed int handle;

	dtx_index = gs_dungeon_id + ARCHIVE_FILE_DNGS_DTX;

	if (g_dng_loaded_dungeon_id != gs_dungeon_id) {

		load_area_description(1);
		g_town_loaded_town_id = -1;
		load_dungeon_ddt();
	}

	load_tx(dtx_index);

	if ((g_area_prepared == AREA_TYPE_NONE) || (g_area_prepared == AREA_TYPE_TOWN)) { // idea: g_area_prepared != AREA_TYPE_DUNGEON

		disable_ani();
		g_current_ani = -1;

		nvf_index = (gs_dungeon_id == DUNGEON_ID_TOTENSCHIFF) ? ARCHIVE_FILE_SHIPSL_NVF :
			(((gs_dungeon_id == DUNGEON_ID_VERFALLENE_HERBERGE) ||
				(gs_dungeon_id == DUNGEON_ID_RUINE_DES_SCHWARZMAGIERS) ||
				(gs_dungeon_id == DUNGEON_ID_KULTSTAETTE_DES_NAMENLOSEN) ||
				(gs_dungeon_id == DUNGEON_ID_PIRATENHOEHLE) ||
				(gs_dungeon_id == DUNGEON_ID_ZWERGENFESTE) ||
				(gs_dungeon_id == DUNGEON_ID_VERLASSENE_MINE) ||
				(gs_dungeon_id == DUNGEON_ID_ZWINGFESTE) ||
				(gs_dungeon_id == DUNGEON_ID_HYGGELIKS_RUINE)) ? ARCHIVE_FILE_MARBLESL_NVF : ARCHIVE_FILE_STONESL_NVF);

		gs_dungeon_gfx_style = (nvf_index == ARCHIVE_FILE_SHIPSL_NVF) ? 0 : ((nvf_index == ARCHIVE_FILE_MARBLESL_NVF) ? 1 : 2);

		handle = load_archive_file(nvf_index);
		file_len = offset = 0;

		/* clear palette */
		buf = g_buffer9_ptr3;
		memset((uint8_t*)buf, 0, 0xc0);
		wait_for_vsync();
		set_palette((uint8_t*)buf, 0x80, 0x40);

		do {
			file_len = read_archive_file(handle, (uint8_t*)buf, 65000);
			buf += file_len;
			offset += file_len;

		} while (file_len);

		close(handle);

		g_buffer11_ptr = (g_buffer9_ptr3 + offset) - 0xc0L;

		g_area_prepared = !gs_dungeon_id;
	}

	g_dng_loaded_dungeon_id = gs_dungeon_id;
	g_town_loaded_town_id = -1;
	set_automap_tiles(gs_x_target, gs_y_target);
}

void load_dungeon_ddt(void)
{
	signed int index;
	int16_t low;
	int16_t high;
	signed int handle;

	index = gs_dungeon_id + ARCHIVE_FILE_DNGS_DDT;
	handle = load_archive_file(index);
	read_archive_file(handle, (uint8_t*)&low, 2);
	read_archive_file(handle, (uint8_t*)&high, 2);

	read_archive_file(handle, g_dungeon_fights_buf, low);
	read_archive_file(handle, (uint8_t*)g_dungeon_doors_buf, high - low);
	read_archive_file(handle, g_dungeon_stairs_buf, 2000);

	close(handle);
}

void prepare_town_area(void)
{
	signed int i;
	const signed int ltx_index = gs_town_id + ARCHIVE_FILE_CITY_LTX;
	uint8_t* arr[4];


	if (g_town_loaded_town_id != gs_town_id) {
		load_area_description(1);
		g_dng_loaded_dungeon_id = -1;
	}

	load_tx(ltx_index);

	if ((g_area_prepared == AREA_TYPE_NONE) || (g_area_prepared == AREA_TYPE_DUNGEON)) { // idea: g_area_prepared != AREA_TYPE_TOWN

		disable_ani();

		g_current_ani = -1;

		town_update_house_count();

		g_buffer9_ptr4 = g_buffer9_ptr3;

		for (i = 0; i < 4; i++) {

			if (g_town_house_count[i]) {

				arr[i] = load_town_textures(!i ? ARCHIVE_FILE_HOUSE1_NVF :
				    (i == 1 ? ARCHIVE_FILE_HOUSE2_NVF :
				        (i == 2 ? ARCHIVE_FILE_HOUSE3_NVF :
				            ARCHIVE_FILE_HOUSE4_NVF)), 0, 0, 0);


			}
		}

		for (i = 0; i < 4; i++) {

			if (!g_town_house_count[i]) {

				arr[i] = (!i ? arr[i + 1] : arr[i - 1]);
			}
		}

		g_tex_floor[2] = arr[0]; // tex_house1
		g_tex_floor[3] = arr[1]; // tex_house2
		g_tex_floor[4] = arr[2]; // tex_house3
		g_tex_floor[5] = arr[3]; // tex_house4

		/* load tex_sky */
		if ((gs_day_timer >= HOURS(7)) && (gs_day_timer <= HOURS(20)))
		{
			g_tex_floor[1] = load_town_textures(ARCHIVE_FILE_TDIVERSE_NVF, 0x80, 0x40, 0);
#if defined(__BORLANDC__)
			memcpy(gs_palette_buildings, g_buffer11_ptr, 0xc0);
#else
			memcpy(gs_palette_buildings, g_buffer11_ptr, 3 * 0x20);
			memcpy(gs_palette_sky, g_buffer11_ptr + 3 * 0x20, 3 * 0x20);
#endif
		} else {
			g_tex_floor[1] = load_town_textures(ARCHIVE_FILE_TDIVERSE_NVF, 0x80, 0x40, 0);
		}

		g_tex_floor[0] = load_town_textures(!g_large_buf ? ARCHIVE_FILE_TFLOOR1_NVF : ARCHIVE_FILE_TFLOOR2_NVF, 0, 0x20, 0);

		if ((gs_day_timer >= HOURS(7)) && (gs_day_timer <= HOURS(20)))
		{
			memcpy(gs_palette_floor, g_buffer11_ptr, 0x60);
		}

		g_area_prepared = AREA_TYPE_TOWN;
	}

	g_town_loaded_town_id = gs_town_id;
	g_dng_loaded_dungeon_id = -1;

	set_automap_tiles(gs_x_target, gs_y_target);
}

uint8_t* load_town_textures(const signed int index, const signed int firstcol, const signed int colors, const signed int ref)
{
	signed int handle;
	int32_t file_len;
	int32_t offset;
	uint8_t* ptr = (uint8_t*)g_buffer9_ptr4;

	handle = load_archive_file(index);

	file_len = offset = 0L;

	do {
		file_len = read_archive_file(handle, (uint8_t*)g_buffer9_ptr4, 65000);

		g_buffer9_ptr4 += file_len;

		offset += file_len;

	} while (file_len);

	close(handle);

	if (colors) {

		g_buffer11_ptr = ptr + offset - 3 * colors;

		if ((ref != 0) && !g_fading_state) {

			wait_for_vsync();

			set_palette(g_buffer11_ptr, firstcol, colors);
		}
	}

	return ptr;
}

void load_special_textures(const signed int arg)
{
	const signed int handle = load_archive_file(arg == 9 ? ARCHIVE_FILE_FINGER_NVF : ARCHIVE_FILE_LTURM_NVF);

	read_archive_file(handle, (uint8_t*)g_buffer7_ptr, 64000);

	close(handle);
}

void call_load_buffer(void)
{
	load_tx(g_tx_file_index);
}

/**
 * \brief prepares a town- or a dungeon-area
 * \param[in] town { 1 = town, otherwise dungeon }
 */
void prepare_area(const signed int town)
{
	if (town == 1) {
		prepare_town_area();
	} else {
		prepare_dungeon_area();
	}
}

/**
 * \brief   writes the old area and reads a new one
 *
 * \param   type        either 0,1,2
 *
 *	0 = only write old area
 *	1 = do both
 *	2 = only read new area (loading a savegame)
 */
void load_area_description(const signed int type)
{
	signed int f_index;

	if (g_areadescr_fileid && (type != 2)) {

		signed int handle = load_archive_file(g_areadescr_fileid + 0x8000);

		if (!g_areadescr_dng_flag && (g_map_size_x == 32)) {
			write(handle, (void*)g_dng_map, 512);
		} else {
			// assert(g_map_size_x == 16)
			lseek(handle, g_areadescr_dng_level * 0x140, 0);
			write(handle, (void*)g_dng_map, 256);
		}

		/* write automap tiles */
		write(handle, (void*)g_automap_buf, 64);

		/* write location information */
		write(handle, (void*)g_locations_tab, g_locations_tab_size);

		close(handle);

		g_areadescr_fileid = g_areadescr_dng_level = g_locations_tab_size = g_areadescr_dng_flag = 0;
	}

	if (type != 0) {

		signed int handle;

		/* calc archive file index */
		if (gs_dungeon_id != 0) {
			/* dungeon */
			g_areadescr_fileid = f_index = gs_dungeon_id + (ARCHIVE_FILE_DNGS-1);
		} else {
			/* town */
			g_areadescr_fileid = f_index = gs_town_id + (ARCHIVE_FILE_CITY_DAT-1);
		}

		/* save dungeon level */
		g_areadescr_dng_level = gs_dungeon_level;

		/* save if we are in a dungeon */
		g_areadescr_dng_flag = (gs_dungeon_id != 0 ? 1 : 0);

		/* load DAT or DNG file */
		handle = load_archive_file(f_index + 0x8000);

		if (!gs_dungeon_id && (gs_town_id == TOWN_ID_THORWAL || gs_town_id == TOWN_ID_PREM || gs_town_id == TOWN_ID_PHEXCAER))
		{
			/* A large city, i.e. x coordinates up to 32 */
			/* path taken in THORWAL PREM and PHEXCAER */
			_read(handle, g_dng_map, 512);
			/* read automap tiles */
			_read(handle, g_automap_buf, 64);

			/* TODO: is that neccessary ? */
			memset(g_locations_tab, -1, 150 * sizeof(struct location));

#if defined(__BORLANDC__)
			g_locations_tab_size = _read(handle, g_locations_tab, 150 * sizeof(struct location) + 100);
#else
			g_locations_tab_size = _read(handle, g_locations_tab, 150 * sizeof(struct location));
#endif

			g_map_size_x = 32;
		} else {
			/* Seek to Dungeon Level * 320 */
			lseek(handle, gs_dungeon_level * 320, 0);
			_read(handle, g_dng_map, 256);

			/* read automap tiles */
			_read(handle, g_automap_buf, 64);
			g_locations_tab_size = 0;

			if (!gs_dungeon_id) {
				/* TODO: is that neccessary ? */
				memset(g_locations_tab, -1, 150 * sizeof(struct location));
#if defined(__BORLANDC__)
				g_locations_tab_size = _read(handle, g_locations_tab, 150 * sizeof(struct location) + 100);
#else
				g_locations_tab_size = _read(handle, g_locations_tab, 150 * sizeof(struct location));
#endif
			}

			g_map_size_x = 16;
		}
		close(handle);
	}
}

void call_load_area(const signed int type)
{
	load_area_description(type);
}

#if defined(__BORLANDC__)
void unused_store(const signed int image_num)
{
	signed int width;
	signed int height;
	struct ems_tab *ptr;
	struct nvf_extract_desc nvf;
	signed int size;

	nvf.dst = g_renderbuf_ptr + 30000;
	nvf.src = (uint8_t*)g_buffer9_ptr3;
	nvf.image_num = image_num;
	nvf.compression_type = 0;
	nvf.width = &width;
	nvf.height = &height;
	process_nvf_extraction(&nvf);

	EMS_map_memory(g_ems_unused_handle, g_ems_unused_lpage, 0);
	EMS_map_memory(g_ems_unused_handle, g_ems_unused_lpage + 1, 1);
	EMS_map_memory(g_ems_unused_handle, g_ems_unused_lpage + 2, 2);
	EMS_map_memory(g_ems_unused_handle, 0, 3);

	size = width * height;
	memmove((uint8_t*)(g_ems_frame_ptr + g_ems_unused_offset), (g_renderbuf_ptr + 0x7530), size);

	ptr = &g_ems_unused_tab[image_num];

	ptr->lpage = g_ems_unused_lpage;
	ptr->offset = g_ems_unused_offset >> 8;
	ptr->width = width;
	ptr->height = height;

	g_ems_unused_lpage = g_ems_unused_lpage + ((uint16_t)(g_ems_unused_offset + size) >> 14);
	g_ems_unused_offset = ((((g_ems_unused_offset + size) & 0x3fff) + 0x100) & 0xff00);
}

uint8_t* unused_load(const signed int image_num)
{
	unsigned int lpage;

	EMS_map_memory(g_ems_unused_handle, 0, 3);

	lpage = g_ems_unused_tab[image_num].lpage;

	EMS_map_memory(g_ems_unused_handle, lpage, 0);
	EMS_map_memory(g_ems_unused_handle, lpage + 1, 1);
	EMS_map_memory(g_ems_unused_handle, lpage + 2, 2);

	return g_ems_frame_ptr + 256 * g_ems_unused_tab[image_num].offset;
}
#endif

void load_map(void)
{
	signed int handle;	/* REMARK: reused differently */
	signed int wallclock_update_bak;
	struct nvf_extract_desc nvf;

	wallclock_update_bak = g_wallclock_update;
	g_wallclock_update = 0;

	g_area_prepared = AREA_TYPE_NONE;
	/* set current_ani to -1 */
	g_current_ani = -1;

	/* open OBJECTS.NVF */
	handle = load_archive_file(ARCHIVE_FILE_OBJECTS_NVF);
	read_archive_file(handle, g_renderbuf_ptr, 2000);
	close(handle);

	/* load the grey border for the wallclock overlay */
	nvf.src = g_renderbuf_ptr;
	nvf.compression_type = 0;
	nvf.width = &handle;
	nvf.height = &handle;
	nvf.dst = (uint8_t*)(g_buffer9_ptr + 18000L);
	nvf.image_num = 16;

	process_nvf_extraction(&nvf);

	array_add((uint8_t*)(g_buffer9_ptr + 18000L), 3003, 0xe0, 2);

	g_pp20_index = ARCHIVE_FILE_KARTE_DAT;

#if defined(__BORLANDC__)
	/* if the ems_map_handler exists */
	if (g_ems_travelmap_handle) {
		/* get data from EMS */
		EMS_map_memory(g_ems_travelmap_handle, 0, 0);
		EMS_map_memory(g_ems_travelmap_handle, 1, 1);
		EMS_map_memory(g_ems_travelmap_handle, 2, 2);
		EMS_map_memory(g_ems_travelmap_handle, 3, 3);
		/* set map pointer to EMS */
		gs_travel_map_ptr = (uint8_t*)g_ems_frame_ptr;
	} else {
#endif
		/* or read KARTE.DAT from file */
		handle = load_archive_file(ARCHIVE_FILE_KARTE_DAT);

		read_archive_file(handle, (gs_travel_map_ptr = g_renderbuf_ptr), 64098);
		close(handle);

#if defined(__BORLANDC__)
		if (g_ems_enabled) {

			if ((g_ems_travelmap_handle = alloc_EMS(64100))) {

				/* map the map into EMS */
				EMS_map_memory(g_ems_travelmap_handle, 0, 0);
				EMS_map_memory(g_ems_travelmap_handle, 1, 1);
				EMS_map_memory(g_ems_travelmap_handle, 2, 2);
				EMS_map_memory(g_ems_travelmap_handle, 3, 3);

				/* TODO: update window */
				memmove((void*)g_ems_frame_ptr,	(void*)g_renderbuf_ptr, 320 * 200 + 98);
			}
		}
	}
#endif

	/* load LROUT.DAT */
	handle = load_archive_file(ARCHIVE_FILE_LROUT_DAT);
	read_archive_file(handle, g_buffer9_ptr, 7600);
	close(handle);

	/* load HSROUT.DAT */
	handle = load_archive_file(ARCHIVE_FILE_HSROUT_DAT);
	read_archive_file(handle, g_buffer9_ptr + 7600L, 3800);
	close(handle);

	/* load SROUT.DAT */
	handle = load_archive_file(ARCHIVE_FILE_SROUT_DAT);
	read_archive_file(handle, g_buffer9_ptr + 11400L, 5900);
	close(handle);

	load_tx(ARCHIVE_FILE_MAPTEXT_LTX);

	g_wallclock_update = wallclock_update_bak;
}

void load_npc(const signed int index)
{
	struct struct_hero *npc;
	signed int handle;

	npc = get_hero(6);

	/* load from temp directory */
	handle = load_archive_file(index | 0x8000);
	_read(handle, (void*)npc, sizeof(struct struct_hero));
	close(handle);

	if (npc->sex == 1) {
		/* female */
		npc->sprite_id = npc->typus + 11;
		if (npc->sprite_id > 21)
			npc->sprite_id = 21;
	} else {
		/* male */
		npc->sprite_id = npc->typus;
		if (npc->sprite_id > 10)
			npc->sprite_id = 10;
	}
}

void save_npc(const signed int index)
{
	signed int handle = load_archive_file(index | 0x8000);

	write(handle, get_hero(6), sizeof(struct struct_hero));

	close(handle);
}

void load_splashes(void)
{
	signed int handle;
	signed int width;
	signed int height;
	struct nvf_extract_desc nvf;

	/* read SPLASHES.DAT */
	handle = load_archive_file(ARCHIVE_FILE_SPLASHES_DAT);
	read_archive_file(handle, g_renderbuf_ptr, 3000);
	close(handle);

	nvf.dst = g_splash_le = g_splash_buffer;
	nvf.src = g_renderbuf_ptr;
	nvf.image_num = 0;
	nvf.compression_type = 1;
	nvf.width = &width;
	nvf.height = &height;
	/* REMARK: use another variable instead of handle */
	handle = (signed int)process_nvf_extraction(&nvf);

	nvf.dst = g_splash_ae = g_splash_buffer + handle;
	nvf.src = g_renderbuf_ptr;
	nvf.image_num = 1;
	nvf.compression_type = 1;
	nvf.width = &width;
	nvf.height = &height;
	process_nvf_extraction(&nvf);
}

void load_informer_tlk(const signed int index)
{
	signed int i;
	signed int handle;

	int32_t text_len;
	int32_t off;
	signed int partners;
	struct struct_dialog_partner *partner;

	g_text_file_index = index;

	handle = load_archive_file(index);

	/* read the header */
	read_archive_file(handle, (uint8_t*)&off, 4);
	read_archive_file(handle, (uint8_t*)&partners, 2);

	/* read the partner structures */
	read_archive_file(handle, (uint8_t*)(partner = &gs_dialog_partners[0]), partners * sizeof(struct struct_dialog_partner));

	/* read the dialog layouts */
	read_archive_file(handle, (uint8_t*)&gs_dialog_states, (uint16_t)(off - partners * sizeof(struct struct_dialog_partner)));

	/* read the text */
	text_len = (signed int)read_archive_file(handle, g_buffer8_ptr, 10000);

	close(handle);

	split_textbuffer((char**)g_tx2_index, (char*)g_buffer8_ptr, text_len);

#if defined(__BORLANDC__)
	/* adjust the pointers to the layouts */
	/* TODO: Not portable! */
	for (i = 0; i < partners; i++, partner++) {
		partner->states_offset = (uint32_t)((uint8_t*)&gs_dialog_states + (uint16_t)partner->states_offset);
	}
#endif
}

void load_tlk(const signed int index)
{
	signed int i;
	signed int handle;
	int32_t text_len;
	int32_t off;
	signed int partners;
	struct struct_dialog_partner *partner;

	g_text_file_index = index;

	handle = load_archive_file(index);

	/* read the header */
	read_archive_file(handle, (uint8_t*)&off, 4);
	read_archive_file(handle, (uint8_t*)&partners, 2);

	/* read the partner structures */
	read_archive_file(handle, (uint8_t*)(partner = &gs_dialog_partners[0]), partners * sizeof(struct struct_dialog_partner));

	/* read the dialog layouts */
	read_archive_file(handle, (uint8_t*)&gs_dialog_states, off - partners * sizeof(struct struct_dialog_partner));

	/* read the text */
	text_len = (signed int)read_archive_file(handle, (uint8_t*)g_buffer7_ptr, 64000);

	close(handle);

	split_textbuffer((char**)g_tx_index, g_buffer7_ptr, text_len);

#if defined(__BORLANDC__)
	/* adjust the pointers to the layouts */
	/* TODO: Not portable! */
	for (i = 0; i < partners; i++, partner++) {
		partner->states_offset = (uint32_t)((uint8_t*)&gs_dialog_states + partner->states_offset);
	}
#endif
}

#if defined(__BORLANDC__)
void unused_load_archive_file(const signed int index, const uint16_t off, const uint32_t seg)
{
	const signed int handle = load_archive_file(index);

	read_archive_file(handle, (uint8_t*)MK_FP(seg, off), 64000);

	close(handle);
}
#endif

void load_fightbg(const signed int index)
{
	const signed int handle = load_archive_file(index);

	read_archive_file(handle, g_renderbuf_ptr, 30000);

#if !defined(__BORLANDC__)
	decomp_pp20(g_renderbuf_ptr, g_buffer8_ptr, g_renderbuf_ptr + 4, get_readlength2(handle));
#else
	decomp_pp20(g_renderbuf_ptr, g_buffer8_ptr, FP_OFF(g_renderbuf_ptr) + 4, FP_SEG(g_renderbuf_ptr), get_readlength2(handle));
#endif

	close(handle);
}
