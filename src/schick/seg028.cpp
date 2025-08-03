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

#if !defined(__BORLANDC__)
namespace M302de {
#endif

void prepare_dungeon_area(void)
{
	signed short index;
	Bit32u v1;
	Bit32u v2;
	HugePt buf;

	signed short l_si;
	signed short handle;

	index = gs_dungeon_index + ARCHIVE_FILE_DNGS_DTX;

	if (g_dng_area_loaded != gs_dungeon_index) {

		load_area_description(1);
		g_city_area_loaded = -1;
		load_dungeon_ddt();
	}

	load_tx(index);

	if ((g_area_prepared == -1) || (g_area_prepared == 1)) {

		set_var_to_zero();
		g_current_ani = -1;

		l_si = (gs_dungeon_index == DUNGEONS_TOTENSCHIFF) ? ARCHIVE_FILE_SHIPSL_NVF :
			(((gs_dungeon_index == DUNGEONS_VERFALLENE_HERBERGE) ||
				(gs_dungeon_index == DUNGEONS_RUINE_DES_SCHWARZMAGIERS) ||
				(gs_dungeon_index == DUNGEONS_KULTSTAETTE_DES_NAMENLOSEN) ||
				(gs_dungeon_index == DUNGEONS_PIRATENHOEHLE) ||
				(gs_dungeon_index == DUNGEONS_ZWERGENFESTE) ||
				(gs_dungeon_index == DUNGEONS_VERLASSENE_MINE) ||
				(gs_dungeon_index == DUNGEONS_ZWINGFESTE) ||
				(gs_dungeon_index == DUNGEONS_HYGGELIKS_RUINE)) ? ARCHIVE_FILE_MARBLESL_NVF : ARCHIVE_FILE_STONESL_NVF);

		ds_writeb(DUNGEON_GFX_STYLE, (l_si == ARCHIVE_FILE_SHIPSL_NVF) ? 0 : ((l_si == ARCHIVE_FILE_MARBLESL_NVF) ? 1 : 2));

		handle = load_archive_file(l_si);
		v1 = v2 = 0;

		/* clear palette */
		buf = g_buffer9_ptr3;
		memset((Bit8u*)(buf), 0, 0xc0);
		wait_for_vsync();
		set_palette((Bit8u*)(buf), 0x80, 0x40);

		do {
			v1 = read_archive_file(handle, (Bit8u*)(buf), 65000);
#if !defined(__BORLANDC__)
			buf = F_PADD(buf, v1);
#else
			buf += v1;
#endif
			v2 += v1;
		} while (v1);

		close(handle);

		g_buffer11_ptr = (((HugePt)g_buffer9_ptr) + v2) - 0xc0L;

		g_area_prepared = !gs_dungeon_index;
	}

	g_dng_area_loaded = gs_dungeon_index;
	g_city_area_loaded = -1;
	set_automap_tiles(gs_x_target, gs_y_target);
}

void load_dungeon_ddt(void)
{
	signed short index;
	signed short low;
	signed short high;
	signed short handle;

	index = gs_dungeon_index + ARCHIVE_FILE_DNGS_DDT;
	handle = load_archive_file(index);
	read_archive_file(handle, (Bit8u*)&low, 2);
	read_archive_file(handle, (Bit8u*)&high, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix: */
	low = host_readws((Bit8u*)&low);
	high = host_readws((Bit8u*)&high);
#endif

	read_archive_file(handle, g_dungeon_fights_buf, low);
	read_archive_file(handle, g_dungeon_doors_buf, high - low);
	read_archive_file(handle, g_dungeon_stairs_buf, 0x7d0);

	close(handle);

}

void seg028_0224(void)
{
	signed short l_si;
	signed short l1;
	Bit8u* arr[4];

	l1 = gs_current_town + 77;

	if (g_city_area_loaded != gs_current_town) {
		load_area_description(1);
		g_dng_area_loaded = -1;
	}

	load_tx(l1);

	if ((g_area_prepared == -1) || (g_area_prepared == 0)) {

		set_var_to_zero();

		g_current_ani = -1;

		seg066_172b();

		g_buffer9_ptr4 = g_buffer9_ptr3;

		for (l_si = 0; l_si < 4; l_si++) {

			if (ds_readb(CITY_HOUSE_COUNT + l_si) != 0) {

				arr[l_si] = seg028_0444(!l_si ? ARCHIVE_FILE_HOUSE1_NVF :
				    (l_si == 1 ? ARCHIVE_FILE_HOUSE2_NVF :
				        (l_si == 2 ? ARCHIVE_FILE_HOUSE3_NVF :
				            ARCHIVE_FILE_HOUSE4_NVF)), 0, 0, 0);


			}
		}

		for (l_si = 0; l_si < 4; l_si++) {
			if (!ds_readb(CITY_HOUSE_COUNT + l_si)) {

				arr[l_si] = (!l_si ? arr[l_si + 1] : arr[l_si - 1]);
			}
		}

		ds_writed(TEX_HOUSE1, (Bit32u)arr[0]);
		ds_writed(TEX_HOUSE2, (Bit32u)arr[1]);
		ds_writed(TEX_HOUSE3, (Bit32u)arr[2]);
		ds_writed(TEX_HOUSE4, (Bit32u)arr[3]);

		if ((gs_day_timer >= HOURS(7)) && (gs_day_timer <= HOURS(20)))
		{
			ds_writed(TEX_SKY, (Bit32u) seg028_0444(ARCHIVE_FILE_TDIVERSE_NVF, 0x80, 0x40, 0));

			memcpy(gs_palette_buildings, g_buffer11_ptr, 0xc0);
		} else {
			ds_writed(TEX_SKY, (Bit32u) seg028_0444(ARCHIVE_FILE_TDIVERSE_NVF, 0x80, 0x40, 0));
		}

		ds_writed(TEX_FLOOR, (Bit32u) seg028_0444(!g_large_buf ? ARCHIVE_FILE_TFLOOR1_NVF : ARCHIVE_FILE_TFLOOR2_NVF, 0, 0x20, 0));

		if ((gs_day_timer >= HOURS(7)) && (gs_day_timer <= HOURS(20)))
		{
			memcpy(gs_palette_floor, g_buffer11_ptr, 0x60);
		}

		g_area_prepared = 1;
	}

	g_city_area_loaded = gs_current_town;
	g_dng_area_loaded = -1;

	set_automap_tiles(gs_x_target, gs_y_target);
}

Bit8u* seg028_0444(signed short index, signed short firstcol, signed short colors, signed short ref)
{
	signed short fd;
	Bit32s v1;
	Bit32s v2;
	Bit8u* ptr;

	ptr = (Bit8u*)g_buffer9_ptr4;

	fd = load_archive_file(index);

	v1 = v2 = 0L;

	do {
		v1 = read_archive_file(fd, (Bit8u*)g_buffer9_ptr4, 65000);

		g_buffer9_ptr4 += v1;

		v2 += v1;

	} while (v1);

	close(fd);

	if (colors) {

		g_buffer11_ptr = ptr + v2 - 3 * colors;

		if ((ref != 0) && (!g_fading_state)) {

			wait_for_vsync();

			set_palette(g_buffer11_ptr, firstcol, colors);
		}
	}

	return ptr;

}

void load_special_textures(signed short arg)
{
	signed short fd;

	fd = load_archive_file(arg == 9 ? ARCHIVE_FILE_FINGER_NVF : ARCHIVE_FILE_LTURM_NVF);
	read_archive_file(fd, (Bit8u*)g_buffer7_ptr, 64000);
	close(fd);

}

void call_load_buffer(void)
{
	load_tx(g_tx_file_index);
}

void seg028_0555(signed short town)
{
	if (town == 1) {
		seg028_0224();
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
void load_area_description(signed short type)
{
	signed short f_index;
	signed short fd;

	if (ds_readw(AREADESCR_FILEID) != 0) {
		if (type != 2) {
			fd = load_archive_file(ds_readw(AREADESCR_FILEID) + 0x8000);

			if ((ds_readw(AREADESCR_DNG_FLAG) == 0) && (ds_readb(DNG_MAP_SIZE) == 0x20)) {
				write(fd, (void*)MK_FP(datseg, DNG_MAP), 0x200);
			} else {
				lseek(fd, ds_readws(AREADESCR_DNG_LEVEL) * 0x140, 0);
				write(fd, (void*)MK_FP(datseg, DNG_MAP), 0x100);
			}
			/* write automap tiles */
			write(fd, (void*)MK_FP(datseg, AUTOMAP_BUF), 64);
			/* write location information */
			write(fd, (void*)MK_FP(datseg, LOCATIONS_LIST), ds_readw(LOCATIONS_LIST_SIZE));

			close(fd);

			ds_writew(AREADESCR_FILEID, ds_writew(AREADESCR_DNG_LEVEL, ds_writew(LOCATIONS_LIST_SIZE, ds_writew(AREADESCR_DNG_FLAG, 0))));
		}
	}

	if (type != 0) {

		/* calc archive file index */
		if (gs_dungeon_index != 0) {
			/* dungeon */
			ds_writew(AREADESCR_FILEID, f_index = gs_dungeon_index + (ARCHIVE_FILE_DNGS-1));
		} else {
			/* city */
			ds_writew(AREADESCR_FILEID, f_index = gs_current_town + (ARCHIVE_FILE_CITY_DAT-1));
		}

		/* save dungeon level */
		ds_writew(AREADESCR_DNG_LEVEL, gs_dungeon_level);

		/* save if we are in a dungeon */
		ds_writew(AREADESCR_DNG_FLAG, gs_dungeon_index != 0 ? 1 : 0);

		/* load DAT or DNG file */
		fd = load_archive_file(f_index + 0x8000);

		if (!gs_dungeon_index &&
			(gs_current_town == TOWNS_THORWAL
				|| gs_current_town == TOWNS_PREM
				|| gs_current_town == TOWNS_PHEXCAER))
		{
			/* path taken in THORWAL PREM and PHEXCAER */
			_read(fd, p_datseg + DNG_MAP, 0x200);
			/* read automap tiles */
			_read(fd, p_datseg + AUTOMAP_BUF, 0x40);

			/* TODO: is that neccessary ? */
			memset(p_datseg + LOCATIONS_LIST, -1, 900);

			ds_writew(LOCATIONS_LIST_SIZE, _read(fd, p_datseg + LOCATIONS_LIST, 1000));

			ds_writeb(DNG_MAP_SIZE, 0x20);
		} else {
			/* Seek to Dungeon Level * 320 */
			lseek(fd, gs_dungeon_level * 320, 0);
			_read(fd, p_datseg + DNG_MAP, 0x100);

			/* read automap tiles */
			_read(fd, p_datseg + AUTOMAP_BUF, 0x40);
			ds_writew(LOCATIONS_LIST_SIZE, 0);

			if (!gs_dungeon_index) {
				/* TODO: is that neccessary ? */
				memset(p_datseg + LOCATIONS_LIST, -1, 900);
				ds_writew(LOCATIONS_LIST_SIZE, _read(fd, p_datseg + LOCATIONS_LIST, 1000));
			}

			ds_writeb(DNG_MAP_SIZE, 0x10);
		}
		close(fd);
	}
}

void call_load_area(signed short type)
{
	load_area_description(type);
}

void unused_store(signed short no)
{
	signed short width;
	signed short height;
	Bit8u *ptr;
	struct nvf_desc nvf;
	signed short size;

	nvf.dst = g_renderbuf_ptr + 30000;
	nvf.src = (Bit8u*)g_buffer9_ptr4;
	nvf.no = no;
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	width = host_readws((Bit8u*)&width);
	height = host_readws((Bit8u*)&height);
#endif

	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), ds_readws(EMS_UNUSED_LPAGE), 0);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), ds_readws(EMS_UNUSED_LPAGE) + 1, 1);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), ds_readws(EMS_UNUSED_LPAGE) + 2, 2);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), 0, 3);

	size = width * height;
	memmove((Bit8u*)((Bit8u*)ds_readd(EMS_FRAME_PTR) + ds_readws(EMS_UNUSED_OFFSET)),
			(Bit8u*)(g_renderbuf_ptr + 0x7530),
			size);

	ptr = no * 5 + (Bit8u*)ds_readd(EMS_UNUSED_TAB);

	host_writeb(ptr, (signed char)ds_readws(EMS_UNUSED_LPAGE));
	host_writeb(ptr + 1, ds_readws(EMS_UNUSED_OFFSET) >> 8);
	host_writew(ptr + 2, width);
	host_writeb(ptr + 4, (signed char)height);

	ds_writew(EMS_UNUSED_LPAGE, (ds_readw(0x5ec0) + ((ds_readw(EMS_UNUSED_OFFSET) + size) >> 14)));
	ds_writew(EMS_UNUSED_OFFSET, ((((ds_readw(0x5ec2) + size) & 0x3fff) + 0x100) & 0xff00));
}

Bit8u* unused_load(signed short no)
{
	signed short l_si;

	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), 0, 3);

	l_si = host_readb((Bit8u*)ds_readd(EMS_UNUSED_TAB) + 5 * no);

	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), l_si, 0);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), l_si + 1, 1);
	EMS_map_memory(ds_readws(EMS_UNUSED_HANDLE), l_si + 2, 2);

	return (Bit8u*)ds_readd(EMS_FRAME_PTR) + 256 * host_readb((Bit8u*)ds_readd(EMS_UNUSED_TAB) + 5 * no + 1);
}

void load_map(void)
{
	signed short fd;
	signed short wallclock_update_bak;
	struct nvf_desc nvf;

	wallclock_update_bak = g_wallclock_update;
	g_wallclock_update = 0;

	g_area_prepared = -1;
	/* set current_ani to -1 */
	g_current_ani = -1;

	/* open OBJECTS.NVF */
	fd = load_archive_file(ARCHIVE_FILE_OBJECTS_NVF);
	read_archive_file(fd, g_renderbuf_ptr, 2000);
	close(fd);

	/* load the grey border for the wallclock overlay */
	nvf.src = g_renderbuf_ptr;
	nvf.type = 0;
	nvf.width = (Bit8u*)&fd;
	nvf.height = (Bit8u*)&fd;
	nvf.dst = (Bit8u*)(F_PADD((Bit8u*)g_buffer9_ptr, 18000));
	nvf.no = 16;

	process_nvf(&nvf);

	array_add((Bit8u*)(F_PADD((Bit8u*)g_buffer9_ptr, 18000)), 3003, 0xe0, 2);

	g_pp20_index = ARCHIVE_FILE_KARTE_DAT;

	/* if the ems_map_hanlder exists */
	if (ds_readw(EMS_TRAVEL_MAP) != 0) {
		/* get data from EMS */
		EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 0, 0);
		EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 1, 1);
		EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 2, 2);
		EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 3, 3);
		/* set map pointer to EMS */
		ds_writed(TRAVEL_MAP_PTR, ds_readd(EMS_FRAME_PTR));
	} else {
		/* or read KARTE.DAT from file */
		fd = load_archive_file(ARCHIVE_FILE_KARTE_DAT);

		read_archive_file(fd, (Bit8u*)(ds_writed(TRAVEL_MAP_PTR, (Bit32u)g_renderbuf_ptr)), 64098);
		close(fd);

		if (g_ems_enabled != 0) {

			if ((ds_writew(EMS_TRAVEL_MAP, alloc_EMS(64100)))) {
				/* map the map into EMS */
				EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 0, 0);
				EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 1, 1);
				EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 2, 2);
				EMS_map_memory(ds_readw(EMS_TRAVEL_MAP), 3, 3);

				/* TODO: update window */
				memmove((void*)((Bit8u*)ds_readd(EMS_FRAME_PTR)),
					(void*)(g_renderbuf_ptr), 320 * 200 + 98);
			}
		}
	}

	/* load LROUT.DAT */
	fd = load_archive_file(ARCHIVE_FILE_LROUT_DAT);
	read_archive_file(fd, (Bit8u*)g_buffer9_ptr, 7600);
	close(fd);

	/* load HSROUT.DAT */
	fd = load_archive_file(ARCHIVE_FILE_HSROUT_DAT);
	read_archive_file(fd, (Bit8u*)(F_PADD((Bit8u*)g_buffer9_ptr, 7600)), 3800);
	close(fd);

	/* load SROUT.DAT */
	fd = load_archive_file(ARCHIVE_FILE_SROUT_DAT);
	read_archive_file(fd, (Bit8u*)(F_PADD((Bit8u*)g_buffer9_ptr, 11400)), 5900);
	close(fd);

	load_tx(ARCHIVE_FILE_MAPTEXT_LTX);

	g_wallclock_update = wallclock_update_bak;
}

void load_npc(signed short index)
{
	Bit8u *npc_dst;
	signed short fd;

	npc_dst = get_hero(6);

	/* load from temp directory */
	fd = load_archive_file(index | 0x8000);
	_read(fd, (void*)npc_dst, SIZEOF_HERO);
	close(fd);

	if (host_readb(npc_dst + HERO_SEX) == 1) {
		/* female */
		host_writeb(npc_dst + HERO_SPRITE_NO, host_readb(npc_dst + HERO_TYPE) + 11);
		if (host_readbs(npc_dst + HERO_SPRITE_NO) > 21)
			host_writeb(npc_dst + HERO_SPRITE_NO, 21);
	} else {
		/* male */
		host_writeb(npc_dst + HERO_SPRITE_NO, host_readb(npc_dst + HERO_TYPE));
		if (host_readbs(npc_dst + HERO_SPRITE_NO) > 10)
			host_writeb(npc_dst + HERO_SPRITE_NO, 10);
	}
}

void save_npc(signed short index)
{
	signed short fd;

	fd = load_archive_file(index | 0x8000);

	write(fd, get_hero(6), SIZEOF_HERO);

	close(fd);
}

void load_splashes(void)
{
	signed short fd;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	/* read SPLASHES.DAT */
	fd = load_archive_file(ARCHIVE_FILE_SPLASHES_DAT);
	read_archive_file(fd, g_renderbuf_ptr, 3000);
	close(fd);

	/* nvf.dst = splash_le = ds_readd() */
	nvf.dst = g_splash_le = g_splash_buffer;
	nvf.src = g_renderbuf_ptr;
	nvf.no = 0;
	nvf.type = 1;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	fd = (signed short)process_nvf(&nvf);

	/* nvf.dst = splash_ae = ds_readd() */
	nvf.dst = g_splash_ae = (g_splash_buffer + fd);
	nvf.src = g_renderbuf_ptr;
	nvf.no = 1;
	nvf.type = 1;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);
}

void load_informer_tlk(signed short index)
{
	signed short i;
	signed short fd;

	Bit32s text_len;
	Bit32s off;
	signed short partners;
	Bit8u *ptr;


	g_text_file_index = index;

	fd = load_archive_file(index);

	/* read the header */
	read_archive_file(fd, (Bit8u*)&off, 4);
	read_archive_file(fd, (Bit8u*)&partners, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix */
	off = host_readd((Bit8u*)&off);
	partners = host_readw((Bit8u*)&partners);
#endif

	/* read the partner structures */
	read_archive_file(fd, ptr = (p_datseg + DIALOG_PARTNERS), partners * 0x26);

	/* read the dialog layouts */
	read_archive_file(fd, p_datseg + DIALOG_STATES, (Bit16u)(off - partners * 0x26));

	/* read the text */
	text_len = (signed short)read_archive_file(fd, g_buffer8_ptr, 10000);

	close(fd);

	split_textbuffer((char**)g_tx2_index, (char*)g_buffer8_ptr, text_len);

	/* adjust the pointers to the layouts */
	for (i = 0; i < partners; i++, ptr += 0x26) {
		host_writed(ptr, (Bit32u)(p_datseg + host_readw(ptr) + DIALOG_STATES));
	}
}

void load_tlk(signed short index)
{
	signed short i;
	signed short fd;
	Bit32s text_len;
	Bit32s off;
	signed short partners;
	Bit8u *ptr;

	g_text_file_index = index;

	fd = load_archive_file(index);

	/* read the header */
	read_archive_file(fd, (Bit8u*)&off, 4);
	read_archive_file(fd, (Bit8u*)&partners, 2);

#if !defined(__BORLANDC__)
	/* BE-Fix */
	off = host_readd((Bit8u*)&off);
	partners = host_readw((Bit8u*)&partners);
#endif

	/* read the partner structures */
	read_archive_file(fd, ptr = p_datseg + DIALOG_PARTNERS, partners * 0x26);

	/* read the dialog layouts */
	read_archive_file(fd, (Bit8u*)(p_datseg + DIALOG_STATES), off - partners * 0x26);

	/* read the text */
	text_len = (signed short)read_archive_file(fd, (Bit8u*)g_buffer7_ptr, 64000);

	close(fd);

	split_textbuffer((char**)g_tx_index, g_buffer7_ptr, text_len);

	/* adjust the pointers to the layouts */
	for (i = 0; i < partners; i++, ptr += 0x26) {
		host_writed(ptr, (Bit32u)(p_datseg + host_readw(ptr) + DIALOG_STATES));
	}
}

#if defined(__BORLANDC__)
void unused_load_archive_file(signed short index, signed short a2, Bit32u seg)
{
	signed short fd;

	fd = load_archive_file(index);
	read_archive_file(fd, (Bit8u*)MK_FP(seg, a2), 64000);
	close(fd);
}
#endif

void load_fightbg(signed short index)
{
	signed short fd;

	fd = load_archive_file(index);
	read_archive_file(fd, g_renderbuf_ptr, 30000);
	decomp_pp20(g_renderbuf_ptr, g_buffer8_ptr,
#if !defined(__BORLANDC__)
			g_renderbuf_ptr + 4,
#else
			FP_OFF(g_renderbuf_ptr) + 4,
			FP_SEG(g_renderbuf_ptr),
#endif
			get_readlength2(fd));
	close(fd);
}

#if !defined(__BORLANDC__)
}
#endif
