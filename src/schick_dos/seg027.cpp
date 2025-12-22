/*
 *	Rewrite of DSA1 v3.02_de functions of seg027 (file loader)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg027.cpp
 *
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
#include "seg009.h"
#include "seg027.h"

static uint8_t *g_pp20_buffers[9] = { 0 };			// ds:0x5e6a; uint8_t*
static signed long g_pp20_buffer_lengths[9] = {0};		// ds:0x5e8e
static signed int g_current_fight_id_2 = 0;			// ds:0x5eb2
static unsigned char g_unkn_038[4] = { 0xb8, 0x14, 0x00, 0x50 }; // ds:0x5eb4

void load_pp20(const signed int index)
{
	volatile signed int handle;
	uint8_t* buffer_ptr;
	signed int bi;

	if (index <= 5 || index == ARCHIVE_FILE_PLAYM_US || index == ARCHIVE_FILE_ZUSTA_UK || index == ARCHIVE_FILE_ZUSTA_US)
	{
		/* These pictures are buffered for faster access */
		bi = index;

		if (index == ARCHIVE_FILE_PLAYM_US)
			bi = 6;
		if (index == ARCHIVE_FILE_ZUSTA_UK)
			bi = 7;
		if (index == ARCHIVE_FILE_ZUSTA_US)
			bi = 8;

		if (g_pp20_buffers[bi]) {

			/* already buffered, just decomp */
			decomp_pp20(g_pp20_buffers[bi], g_renderbuf_ptr,
				/* TODO: check this out */
#if !defined(__BORLANDC__)
				g_pp20_buffers[bi] + 4,
#else
				FP_OFF(g_pp20_buffers[bi]) + 4,
				FP_SEG(g_pp20_buffers[bi]),
#endif
				g_pp20_buffer_lengths[bi]);

		} else {
			handle = load_archive_file(index);

			if ((buffer_ptr = schick_alloc(get_readlength2(handle)))) {
				/* successful allocation */

				/* save pointer */
				g_pp20_buffers[bi] = buffer_ptr;

				/* save length */
				g_pp20_buffer_lengths[bi] = get_readlength2(handle);

				/* read pic */
				read_archive_file(handle, g_pp20_buffers[bi], g_pp20_buffer_lengths[bi]);

				/* decompress */
				decomp_pp20(g_pp20_buffers[bi], g_renderbuf_ptr,
#if !defined(__BORLANDC__)
					g_pp20_buffers[bi] + 4,
#else
					FP_OFF(g_pp20_buffers[bi]) + 4,
					FP_SEG(g_pp20_buffers[bi]),
#endif
					g_pp20_buffer_lengths[bi]);

				close(handle);
			} else {
				/* failed allocation */

				/* read it directly */
				read_archive_file(handle, g_renderbuf_ptr - 8, 64000);

				/* decompress it */
				decomp_pp20(g_renderbuf_ptr - 8, g_renderbuf_ptr,
#if !defined(__BORLANDC__)
					g_renderbuf_ptr -8 +4,
#else
					FP_OFF(g_renderbuf_ptr -8) +4,
					FP_SEG(g_renderbuf_ptr -8),
#endif
					get_readlength2(handle));

				close(handle);
			}
		}

	} else {

		/* unbuffered picture */
		handle = load_archive_file(index);

		read_archive_file(handle, g_renderbuf_ptr - 8, 64000);

		/* decompress it */
		decomp_pp20(g_renderbuf_ptr - 8, g_renderbuf_ptr,
#if !defined(__BORLANDC__)
			(uint8_t*)(g_renderbuf_ptr - 8 + 4),
#else
			FP_OFF(g_renderbuf_ptr - 8) + 4,
			FP_SEG(g_renderbuf_ptr - 8),
#endif
			get_readlength2(handle));

		close(handle);
	}
}

/**
 * \brief   load fight sprites of actors
 *
 * \param   fig_old     the fig number
 * \return              a pointer to the location where the data is.
 */
/* Original-Bug: when using EMS for caching something strange happens. */
uint8_t* load_fight_figs(signed int fig)
{
	signed int i;

	uint8_t* dst;
	signed int ems_handle;
	uint32_t offset;
	uint32_t len;
	signed int handle;
	signed int max_entries;
	struct struct_memslot_fig *memslots;
	uint32_t *p_tab;
	signed int index;
	uint8_t *src;

	/* check if fig is at a known place */
	if (fig == g_fig_figure1) {
		return (uint8_t*)g_fig_figure1_buf;
	} else if (fig == g_fig_figure2) {
		return (uint8_t*)g_fig_figure2_buf;
	} else if (g_fig_figure2 != -1) {
		g_fig_figure1 = g_fig_figure2;
		memcpy((uint8_t*)g_fig_figure1_buf, (uint8_t*)g_fig_figure2_buf, 20000);
		src = (uint8_t*)g_fig_figure2_buf;
		g_fig_figure2 = fig;
	} else if (g_fig_figure1 != -1) {
		src = (uint8_t*)g_fig_figure2_buf;
		g_fig_figure2 = fig;
	} else {
		src = (uint8_t*)g_fig_figure1_buf;
		g_fig_figure1 = fig;
	}

	/* prepare archive access... */

	if (fig >= 88) {
		/* ...for foes */
		max_entries = 36;
		memslots = g_memslots_mon;
		p_tab = g_buffer_monster_tab;
		index = 16;
		fig -= 88;
	} else {
		/* ...for heroes */
		max_entries = 43;

		if (fig >= 44) {
			/* female */
			p_tab = g_buffer_wfigs_tab;
			index = ARCHIVE_FILE_WFIGS;
			memslots = g_memslots_wfig;
			fig -= 44;
		} else {
			/* male */
			p_tab = g_buffer_mfigs_tab;
			index = ARCHIVE_FILE_MFIGS;
			memslots = g_memslots_mfig;
		}
	}

	/* check if fig is already in memory */
	for (i = 0; i < max_entries; i++) {

		if (memslots[i].figure == fig)
			break;
	}

	if (i != max_entries) {
		/* Yes, it is */

		if (memslots[i].ems_handle) {
#if defined(__BORLANDC__)
			/* is in EMS */
			ems_handle = memslots[i].ems_handle;
			from_EMS(src, ems_handle, memslots[i].length);
#endif
		} else {
			/* is in HEAP */
#if !defined(__BORLANDC__)
			D1_LOG("cached from HEAP %d\n", fig);
#endif
			memcpy((uint8_t*)src, memslots[i].ptr, memslots[i].length);
		}
	} else {
#if !defined(__BORLANDC__)
		D1_LOG("not cached\n");
#endif

		/* read fig from file */
		offset = p_tab[fig - 1];
		len = p_tab[fig] - offset;

		handle = load_archive_file(index);

		seek_archive_file(handle, offset, 0);

		read_archive_file(handle, (uint8_t*)src, (uint16_t)len);

		close(handle);

		if ((dst = schick_alloc(len))) {
#if !defined(__BORLANDC__)
			D1_LOG("use HEAP for fig %d\n", fig);
#endif
			/* use heap */

			for (i = 0; i < max_entries - 1; i++) {
				if (!memslots[i].figure)
					break;
			}

			memslots[i].figure = fig;
			memslots[i].ptr = dst;
			memslots[i].ems_handle = 0;
			memslots[i].length = len;

			memcpy((uint8_t*)dst, (uint8_t*)src, (uint16_t)len);

		}
#if defined(__BORLANDC__)
	       	else if (g_ems_enabled != 0) {

			if ((ems_handle = alloc_EMS(len))) {

				/* find a free slot */
				for (i = 0; i < max_entries - 1; i++) {
					if (!memslots[i].figure)
						break;
				}

				/* write slot */
				memslots[i].figure = fig;
				memslots[i].ems_handle = ems_handle;
				memslots[i].ptr = NULL;
				memslots[i].length = len;

				/* copy to EMS */
				to_EMS(ems_handle, src, len);
			}
		}
#endif
	}

	return src;
}

/**
 * \brief   loads an animation
 * \param[in] ani_id number of the animation
 */
void load_ani(const signed int ani_id)
{
	signed int i_area;
	signed int area_pics;
	signed int area_changes;
	signed int handle;
	signed int i;
	struct ani_area_in *p_area;
	signed int ems_handle;
#if !defined(__BORLANDC__)
	uint8_t* ani_buffer;
	uint8_t *unplen_ptr;
#else
	uint8_t huge *ani_buffer;
	uint8_t huge *unplen_ptr;
#endif
	uint8_t *area_changes_ptr;
	int32_t area_offset;
	int32_t area_data_offset;
	struct ani_area *p_area2;
	int32_t ani_off;
	int32_t ani_len;
#if !defined(__BORLANDC__)
	uint8_t *ani_residue_ptr;
	uint8_t *ani_end_ptr;
#else
	uint8_t huge *ani_residue_ptr;
	uint8_t huge *ani_end_ptr;
#endif
	int32_t packed_delta;
	int32_t unplen;
	int32_t plen;
	int32_t packed_delta2;
	int32_t ani_residue_len;
	int32_t area_size;

	signed int j;


	/* sanity check */
	if (ani_id == -1)
		return;

	/* no need to reload  the same ani*/
	if (ani_id == g_current_ani)
		return;

	g_ani_enabled = 0;
	/* set the new ani_id */
	g_current_ani = ani_id;
	/* clear the old ani */
	clear_ani();

	/* count to the ordered ani in an array */
	for (i = 0; i < 37; i++) {
		if (ani_id == g_memslots_anis[i].figure)
			break;
	}

	if (i != 37) {
#if defined(__BORLANDC__)
		/* already buffered in EMS, get from there */
		ems_handle = g_memslots_anis[i].ems_handle;
		from_EMS((uint8_t*)g_buffer9_ptr, ems_handle, g_memslots_anis[i].length);
#endif
	} else {
		/* load it from file */
		ani_off = g_buffer_anis_tab[ani_id - 1];
		ani_len = g_buffer_anis_tab[ani_id]- ani_off;

		/* load ANIS */
		handle = load_archive_file(ARCHIVE_FILE_ANIS);

		/* seek to ordered ani */
		seek_archive_file(handle, ani_off, 0);
		read_archive_file(handle, (uint8_t*)g_buffer9_ptr, (unsigned int)ani_len);

#if defined(__BORLANDC__)
		/* if EMS is enabled buffer it */
		if ((g_ems_enabled != 0) && (ems_handle = alloc_EMS(ani_len))) {

			/* find an empty EMS slot */
			for (i = 0; i < 36; i++) {
				if (!g_memslots_anis[i].figure)
					break;
			}

			/* fill the entry */
			g_memslots_anis[i].figure = ani_id;
			g_memslots_anis[i].ems_handle = ems_handle;
			g_memslots_anis[i].length = ani_len;

			/* copy data to EMS */
			to_EMS(ems_handle, (uint8_t*)g_buffer9_ptr, ani_len);
		}
#endif

		close(handle);
	}

	ani_buffer = g_buffer9_ptr;

	/* set start of picture data */
	g_ani_main_ptr = ani_buffer + *(uint32_t*)g_buffer9_ptr;
	/* set start of palette */
	g_ani_palette = (ani_buffer + *(uint32_t*)(g_buffer9_ptr + 4L)) + 6L;

	/* read some bytes between data and palette */
	g_ani_unknown1 = *(uint16_t*)(g_ani_palette - 6L);
	g_ani_unknown2 = *(uint16_t*)(g_ani_palette - 4L);
	/* compression type */
	g_ani_compr_flag = *(g_ani_palette - 1L);
	g_ani_palette_size = *(g_ani_palette - 2L);

	ani_end_ptr = g_ani_palette + 3 * g_ani_palette_size;

	/* set picture size */
	g_ani_width = *(int16_t*)(g_buffer9_ptr + 8L);
	g_ani_height = *(g_buffer9_ptr + 10L);
	/* set number of areas */
	g_ani_areacount = *(g_buffer9_ptr + 11L);

	/* Process Main Picture */
	if (g_ani_compr_flag) {

		plen = *(uint32_t*)g_ani_main_ptr;
		unplen_ptr = g_ani_main_ptr;

		unplen_ptr += (plen - 4);

		unplen = *(uint32_t*)unplen_ptr;
		unplen = swap_u32(unplen) >> 8;

		decomp_pp20((uint8_t*)g_ani_main_ptr, g_renderbuf_ptr,
#if !defined(__BORLANDC__)
			(uint8_t*)(g_ani_main_ptr + 4),
#else
			FP_OFF((uint8_t*)g_ani_main_ptr) + 4,
			FP_SEG((uint8_t*)g_ani_main_ptr),
#endif
			plen);

		packed_delta = unplen - plen;
		ani_residue_ptr = (uint8_t*)g_ani_main_ptr;
		ani_residue_ptr += plen;
		ani_residue_len = ani_end_ptr - ani_residue_ptr;
		memcpy(ani_end_ptr + packed_delta, ani_residue_ptr, ani_residue_len);

		memcpy((uint8_t*)g_ani_main_ptr, g_renderbuf_ptr, unplen);
		ani_residue_ptr += packed_delta;
		memcpy(ani_residue_ptr, ani_end_ptr + packed_delta, ani_residue_len);

		g_ani_palette += packed_delta;
		ani_end_ptr += packed_delta;
	}

	/* Process the Areas */
	for (i_area = 0; g_ani_areacount > i_area; i_area++) {

		p_area2 = &g_ani_area_table[i_area];
		area_offset = *(uint32_t*)((g_buffer9_ptr + 4 * i_area) + 0xc);
		p_area = (struct ani_area_in*)(g_buffer9_ptr + area_offset);
		strncpy(p_area2->name, (char*)p_area, 4);

		p_area2->x = p_area->x;
		p_area2->y = p_area->y;
		p_area2->height = p_area->height;
		p_area2->width = p_area->width;
		p_area2->cyclic = p_area->cyclic;
		p_area2->pics = (signed char)(area_pics = p_area->pics);

		if (g_ani_compr_flag) {

			area_data_offset = *(uint32_t*)((uint8_t*)p_area + 0xc);
			area_data_offset += packed_delta;
			unplen_ptr = g_buffer9_ptr + area_data_offset;

			plen = *(uint32_t*)unplen_ptr;
			unplen_ptr += (plen - 4);
			area_size = *(uint32_t*)unplen_ptr;
			area_size = swap_u32(area_size) >> 8;

			decomp_pp20(g_buffer9_ptr + area_data_offset, g_renderbuf_ptr,
#if !defined(__BORLANDC__)
				(uint8_t*)g_buffer9_ptr + area_data_offset + 4,
#else
				FP_OFF(g_buffer9_ptr + area_data_offset) + 4,
				FP_SEG(g_buffer9_ptr + area_data_offset),
#endif
				plen);

			packed_delta2 = area_size - plen;
			packed_delta += packed_delta2;

			ani_residue_ptr = (uint8_t*)g_buffer9_ptr;
			ani_residue_ptr += area_data_offset;
			ani_residue_ptr += plen;
			ani_residue_len = ani_end_ptr - ani_residue_ptr;
			memcpy(ani_end_ptr + packed_delta2, ani_residue_ptr, (unsigned int)ani_residue_len);

			memcpy(g_buffer9_ptr + area_data_offset, g_renderbuf_ptr, (unsigned int)area_size);
			ani_residue_ptr += packed_delta2;
			memcpy(ani_residue_ptr, ani_end_ptr + packed_delta2, (unsigned int)ani_residue_len);

			g_ani_palette += packed_delta2;
			ani_end_ptr += packed_delta2;

			area_size = p_area2->height * (signed int)p_area2->width;

			for (j = 0; j < area_pics; j++) {
				p_area2->pics_tab[j] = (uint8_t*)((g_buffer9_ptr + area_data_offset) + j * area_size);
			}
		} else {
			for (j = 0; j < area_pics; j++) {
				area_data_offset = *(uint32_t*)((uint8_t*)p_area + j * 4 + 0xc);
				p_area2->pics_tab[j] = g_buffer9_ptr + area_data_offset;
			}
		}

		p_area2->changes = area_changes = *(uint16_t*)((uint8_t*)p_area + area_pics * 4 + 0x0c);

		area_changes_ptr = (((uint8_t*)p_area) + area_pics * 4 + 0x0e);

		for (j = 0; j < area_changes; j++) {
			p_area2->changes_tb[j].pic = *(int16_t*)(area_changes_ptr + ((j << 1) << 1));
			p_area2->changes_tb[j].duration = *(int16_t*)(area_changes_ptr + ((j << 1) << 1) + 2);
		}
	}

	ani_len = ani_end_ptr - (uint8_t*)g_buffer9_ptr;

	/* this is always true */
	if (ani_len > g_ani_unknown4) {
		g_area_prepared = AREA_TYPE_NONE;
	}
}

/**
 * \brief   reads an entry in SCENARIO.LST and stores it in g_scenario_buf
 *
 * \param   scenario_id number of the scenario in SCENARIO.LST
 */
void load_scenario(signed int scenario_id)
{
	signed int handle;
	signed int scenarios_num;

	/* load SCENARIO.LST */
	handle = load_archive_file(ARCHIVE_FILE_SCENARIO_LST);

	/* read the first two bytes == scenario_id of scenarios */
	read_archive_file(handle, (uint8_t*)&scenarios_num, 2);

	/* check if scenario_id is valid */
	if ((scenario_id > scenarios_num) || (scenario_id < 1))
		scenario_id = 1;

	/* seek to the scenario */
	seek_archive_file(handle, (int32_t)(scenario_id - 1) * sizeof(struct scenario) + 2, 0);

	/* read scenario */
	read_archive_file(handle, (uint8_t*)g_scenario_buf, sizeof(struct scenario));

	/* close archive */
	close(handle);
}

/**
 * \brief   counts the number of enemies that are present in the first round
 *
 *          This information is stored in FIGHT.LST.
 *
 * \param   fight_id    number of the fight in FIGHT.LST
 * \return              number of enemies present in first round
 */
signed int count_fight_enemies(signed int fight_id)
{
	signed int enemy_i;
	signed int enemy_count;
	struct fight *fight_lst_buf;
	signed int handle;
	signed int fight_count;

	enemy_count = 0;

	fight_lst_buf = (struct fight*)g_dtp2;

	/* load FIGHT.LST from TEMP dir */
	handle = load_archive_file(0x8000 | ARCHIVE_FILE_FIGHT_LST);

	/* read the first 2 bytes (fight_count number of fights) */
	_read(handle, (void*)&fight_count, 2);

	/* sanity check for parameter fight_id */
	if ((fight_id > (fight_count - 1)) || (fight_id < 0))
		fight_id = 0;

	/* seek to file position */
	lseek(handle, (int32_t)sizeof(struct fight) * fight_id + 2, SEEK_SET);

	/* read the fight entry */
	_read(handle, (void*)fight_lst_buf, sizeof(struct fight));

	/* close FIGHT.LST */
	close(handle);

	/* check all enemies */
	for (enemy_i = 0; enemy_i < 20; enemy_i++) {

		/* no enemy and enemy does not appear in the first round */
		if (fight_lst_buf->enemies[enemy_i].monster_id && !fight_lst_buf->enemies[enemy_i].round_appear)
		{
			/* increment counter */
			enemy_count++;
		}
	}

	return enemy_count;
}

/**
 * \brief   reads an entry in FIGHT.LST and stores it in g_current_fight
 *
 * \param   fight_id    number of the fight in FIGHT.LST
 */
void read_fight_lst(signed int fight_id)
{
	signed int handle;
	signed int fight_count;

	/* load FIGHT.LST from TEMP dir */
	handle = load_archive_file(0x8000 | ARCHIVE_FILE_FIGHT_LST);

	/* read the first 2 bytes (fight_count number of fights) */
	_read(handle, (void*)&fight_count, 2);

	/* sanity check for parameter fight_id */
	if ((fight_count - 1) < fight_id || fight_id < 0)
		fight_id = 0;

	/* write the fight number to a global var */
	g_current_fight_id_2 = fight_id;

	/* seek to file position */
	lseek(handle, (int32_t)sizeof(struct fight) * fight_id + 2, SEEK_SET);

	/* read the fight entry */
	_read(handle, (uint8_t*)g_current_fight, sizeof(struct fight));

	/* close FIGHT.LST */
	close(handle);

#if !defined(__BORLANDC__)
	char fight_name[21];
	/* Improvement */
	strncpy(fight_name, g_current_fight->name, 20);
	fight_name[20] = '\0';
	D1_INFO("Lade Kampf fight_id %3d\t Name \"%s\"\n", fight_id, fight_name);
	/* Improvement end */
#endif

}

/**
 * \brief   writes the data from g_current_fight to FIGHT.LST
 */
void write_fight_lst(void)
{
	const signed int fight_id = g_current_fight_id_2;

	/* load FIGHT.LST from TEMP dir */
	signed int handle = load_archive_file(0x8000 | ARCHIVE_FILE_FIGHT_LST);

	/* seek to the entry */
	lseek(handle, (int32_t)fight_id * sizeof(struct fight) + 2, SEEK_SET);

	/* write it */
	write(handle, g_current_fight, sizeof(struct fight));

	/* close the file */
	close(handle);
}

void init_common_buffers(void)
{
	signed int handle;
	signed int bytes;

	handle = load_archive_file(ARCHIVE_FILE_POPUP_DAT);
	bytes = read_archive_file(handle, g_popup - 8, 500);
	close(handle);

	/* decompress POPUP.DAT */
	decomp_pp20(g_popup - 8, g_popup,
#if !defined(__BORLANDC__)
		g_popup - 8 + 4,
#else
		FP_OFF(g_popup - 8) + 4,
		FP_SEG(g_popup - 8),
#endif
		bytes);

	handle = load_archive_file(ARCHIVE_FILE_COMPASS);
	bytes = read_archive_file(handle, g_buffer6_ptr, 5000);
	close(handle);

	handle = load_archive_file(ARCHIVE_FILE_ITEMS_DAT);
	bytes = read_archive_file(handle, (uint8_t*)g_itemsdat, 255 * sizeof(item_stats));
	close(handle);

	handle = load_archive_file(ARCHIVE_FILE_ANIS_TAB);
	read_archive_file(handle, (uint8_t*)&g_buffer_anis_tab, 148);
	close(handle);

	handle = load_archive_file(ARCHIVE_FILE_MFIGS_TAB);
	read_archive_file(handle, (uint8_t*)&g_buffer_mfigs_tab, 172);
	close(handle);

	handle = load_archive_file(ARCHIVE_FILE_WFIGS_TAB);
	read_archive_file(handle, (uint8_t*)&g_buffer_wfigs_tab, 172);
	close(handle);

	handle = load_archive_file(ARCHIVE_FILE_MONSTER_TAB);
	read_archive_file(handle, (uint8_t*)&g_buffer_monster_tab, 144);
	close(handle);

	handle = load_regular_file(ARCHIVE_FILE_GAMES_NAM);
	_read(handle, (uint8_t*)&g_savegame_names, 45);
	close(handle);

	handle = load_archive_file(ARCHIVE_FILE_TOWNPAL_DAT);
	read_archive_file(handle, g_townpal_buf, 288);
	close(handle);
}
