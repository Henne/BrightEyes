/*
 *	Rewrite of DSA1 v3.02_de functions of seg002 (misc)
 *	Functions rewritten: 148/148 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y -IAIL seg002.cpp
 *
 *	Remark:		Needs header of AIL (Miles Sound System)
 *
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>

#if defined(__BORLANDC__)
#include <ALLOC.H>
#include <CONIO.H>
#include <DIR.H>
#include <DOS.H>
#include <IO.H>
#include "AIL/AIL.H"
#else
#include <unistd.h>
#include "seg011.h"
#endif

#include "v302de.h"
#include "common.h"

/* all global variables are included here, since BCC.EXE would create another module */
#include "datseg.cpp"

#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg006.h"
#include "seg007.h"
#include "seg008.h"
#include "seg009.h"
#include "seg010.h"
#include "seg025.h"
#include "seg026.h"
#include "seg027.h"
#include "seg029.h"
#include "seg030.h"
#include "seg039.h"
#include "seg041.h"
#include "seg047.h"
#include "seg048.h"
#include "seg049.h"
#include "seg050.h"
#include "seg065.h"
#include "seg066.h"
#include "seg076.h"
#include "seg093.h"
#include "seg095.h"
#include "seg096.h"
#include "seg097.h"
#include "seg106.h"
#include "seg108.h"
#include "seg113.h"
#include "seg119.h"
#include "seg120.h"
#include "seg121.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
void sub_light_timers(Bit32s);
#endif


/* static */
void play_music_file(signed short index)
{
#if defined(__BORLANDC__)
	if (g_music_enabled != 0) {
		do_play_music_file(index);
	}
#endif
}

void set_audio_track(Bit16u index)
{
#if defined(__BORLANDC__)
	CD_check();
#endif

	/* only do something when index is not the current track */
	if (g_music_current_track != index) {

		g_music_current_track = index;

		if (ds_readw(USE_CDAUDIO_FLAG) != 0) {
			/* we use CD */
			CD_set_track(index);
		} else {
			/* we use AIL */
			play_music_file(index);
		}
	}
}

void sound_menu(void)
{
	signed short answer;

	answer = GUI_radio((char*)p_datseg + SND_MENU_QUESTION, 4,
				p_datseg + SND_MENU_RADIO1,
				p_datseg + SND_MENU_RADIO2,
				p_datseg + SND_MENU_RADIO3,
				p_datseg + SND_MENU_RADIO4);

	switch (answer - 1) {
		case 0: {
			g_music_enabled = 0;
			g_snd_effects_enabled = 0;
			break;
		}
		case 1: {
			g_music_enabled = 1;
			g_snd_effects_enabled = 0;
			break;
		}
		case 2: {
			g_music_enabled = 0;
			g_snd_effects_enabled = 1;
			break;
		}
		case 3: {
			g_music_enabled = 1;
			g_snd_effects_enabled = 1;
			break;
		}
	}

	if (g_music_enabled == 0) {
		/* music disabled */
		if (ds_readw(USE_CDAUDIO_FLAG) != 0) {
			CD_audio_pause();
		} else {
			stop_midi_playback();
		}
	} else {
		if (g_music_current_track != -1) {
			/* music enabled */
			if (ds_readw(USE_CDAUDIO_FLAG) != 0) {
				CD_audio_play();
			} else {
				play_music_file(g_music_current_track);
			}
		}
	}
}

void read_sound_cfg(void)
{
#if defined(__BORLANDC__)
	signed short midi_port;
	signed short dummy;
	signed short digi_port;
	signed short digi_irq;
	signed short handle;

	/* try to open SOUND.CFG */
	if ( (handle = open((char*)(p_datseg + FNAME_SOUND_CFG), O_BINARY | O_RDONLY)) != -1) {

		_read(handle, (Bit8u*)&midi_port, 2);
		_read(handle, (Bit8u*)&dummy, 2);
		_read(handle, (Bit8u*)&digi_port, 2);
		_read(handle, (Bit8u*)&digi_irq, 2);
		close(handle);

#if !defined(__BORLANDC__)
		/* be byte-ordering independent */
		midi_port = host_readws((Bit8u*)&midi_port);
		dummy = host_readws((Bit8u*)&dummy);
		digi_port = host_readws((Bit8u*)&digi_port);
		digi_irq = host_readws((Bit8u*)&digi_irq);
#endif

#if !defined(__BORLANDC__)
		/* menu to select the music source */
		const Bit16s tw_bak = g_textbox_width;
		char question[] = "WIE SOLL DIE MUSIK WIEDERGEGEBEN WERDEN?";
		char opt1[] = "AUDIO-CD";
		char opt2[] = "MIDI";
		signed short answer;

		g_textbox_width = 3;
		do {
			answer = GUI_radio(question, 2, opt1, opt2);

		} while (answer == -1);
		g_textbox_width = tw_bak;

		if (answer == 1)
		{
			/* AUDIO-CD selected */
			ds_writew(USE_CDAUDIO_FLAG, 1);
			ds_writew(LOAD_SOUND_DRIVER, 1);

		} else if (answer == 2)
		{
			/* MIDI selected */
			ds_writew(USE_CDAUDIO_FLAG, 0);
			ds_writew(LOAD_SOUND_DRIVER, 0);
		}

		if (ds_readw(USE_CDAUDIO_FLAG) == 0)
#else
		/* enable useage of audio-CD */
		ds_writew(USE_CDAUDIO_FLAG, ds_writew(LOAD_SOUND_DRIVER, 1));

		/* disable loading of the music driver */
		if (0)
#endif
		{
			if (midi_port != 0) {
				load_music_driver(((Bit8u*)p_datseg + FNAME_SOUND_ADV2), 3, midi_port);
			} else {

				/* music was disabled in SOUND.CFG */
				if (((Bit8u*)ds_readd(AIL_MIDI_BUFFER))) {
					free((void*)ds_readd(AIL_MIDI_BUFFER));
				}

				ds_writed(AIL_MIDI_BUFFER, 0);
			}
		}

		if (digi_port != 0) {

			if (g_snd_voc_enabled != 0) {

				if (!load_digi_driver(((Bit8u*)p_datseg + FNAME_DIGI_ADV), 2, digi_port, digi_irq))
				{
					g_snd_voc_enabled = 0;
				}
			} else {
				/* print that sound effects are disabled */
				GUI_output((char*)p_datseg + SND_TXT_DISABLED_MEM);
				g_snd_voc_enabled = 0;
			}
		} else {
			g_snd_voc_enabled = 0;
		}

	}
#endif
}

void init_AIL(Bit32u size)
{
#if defined(__BORLANDC__)
	if (((Bit8u*)ds_writed(AIL_MIDI_BUFFER, (Bit32u)schick_alloc(size)))) {
		AIL_startup();
		ds_writew(LOAD_SOUND_DRIVER, 1);
	}
#endif
}

void exit_AIL(void)
{
#if defined(__BORLANDC__)
	AIL_shutdown((Bit8u*)NULL);

	if (ds_readd(AIL_TIMBRE_CACHE) != 0) {
		free((void*)ds_readd(AIL_TIMBRE_CACHE));
	}

	if (ds_readd(AIL_STATE_TABLE) != 0) {
		free((void*)ds_readd(AIL_STATE_TABLE));
	}

	if (ds_readd(AIL_MIDI_BUFFER) != 0) {
		free((void*)ds_readd(AIL_MIDI_BUFFER));
	}

	if (ds_readd(AIL_MUSIC_DRIVER_BUF2) != 0) {
		free((void*)ds_readd(AIL_MUSIC_DRIVER_BUF2));
	}

	/* set all pointers to NULL */
	ds_writed(AIL_TIMBRE_CACHE, ds_writed(AIL_STATE_TABLE, ds_writed(AIL_MIDI_BUFFER, ds_writed(AIL_MUSIC_DRIVER_BUF2, 0))));

	if (g_snd_voc_enabled != 0) {
		free_voc_buffer();
	}
#endif
}

Bit8u* read_music_driver(Bit8u* fname)
{
#if defined(__BORLANDC__)
	Bit32u len;
	Bit8u* buf;
	Bit32u ptr;

	signed short handle;

	if ( (handle = open((char*)fname, O_BINARY | O_RDONLY)) != -1) {

		len = 16500L;

		ds_writed(AIL_MUSIC_DRIVER_BUF2, (Bit32u)schick_alloc(len + 16L));
		/* insane pointer casting */
		ptr = (ds_readd(AIL_MUSIC_DRIVER_BUF2) + 15L);
		ptr &= 0xfffffff0;
		buf = EMS_norm_ptr((Bit8u*)ptr);
		/* and_ptr_ds((Bit8u*)&ptr, 0xfffffff0); */
		_read(handle, (Bit8u*)buf, (unsigned short)len);
		close(handle);
		return buf;
	}
#endif
	return NULL;
}

/* static */
signed short prepare_midi_playback(signed short sequence)
{
#if defined(__BORLANDC__)
	unsigned short l_si;
	signed short l_di;
	signed short patch;
	Bit8u* ptr;

	if ((ds_writews(SAMPLE_AD_HANDLE, load_archive_file(ARCHIVE_FILE_SAMPLE_AD))) != -1) {

		if ((ds_writews(AIL_SEQUENCE, AIL_register_sequence(ds_readw(AIL_MUSIC_DRIVER_ID), (Bit8u*)ds_readd(AIL_MIDI_BUFFER), sequence, (Bit8u*)ds_readd(AIL_STATE_TABLE), 0))) != -1) {

			while ( (l_si = AIL_timbre_request(ds_readw(AIL_MUSIC_DRIVER_ID), ds_readw(AIL_SEQUENCE))) != (unsigned short)-1)
			{
				l_di = l_si >> 8;
				patch = l_si & 0xff;

				if ( (ptr = prepare_timbre(l_di, patch))) {
					AIL_install_timbre(ds_readw(AIL_MUSIC_DRIVER_ID), l_di, patch, ptr);
					free(ptr);
				}
			}

			close(ds_readw(SAMPLE_AD_HANDLE));
			return 1;
		}

		close(ds_readw(SAMPLE_AD_HANDLE));
	}
#endif
	return 0;
}

/* static */
signed short start_midi_playback(signed short seq)
{
#if defined(__BORLANDC__)
	if (prepare_midi_playback(seq)) {
		AIL_start_sequence(ds_readw(AIL_MUSIC_DRIVER_ID), seq);
		return 1;
	}
#endif
	return 0;
}


/* static */
Bit8u* prepare_timbre(signed short a1, signed short patch)
{
#if defined(__BORLANDC__)
	char *buf;

	seek_archive_file(ds_readws(SAMPLE_AD_HANDLE), 0, 0);

	do {
		read_archive_file(ds_readws(SAMPLE_AD_HANDLE), p_datseg + SAMPLE_AD_IDX_ENTRY, 6);

		if (ds_readbs((SAMPLE_AD_IDX_ENTRY+1)) == -1) {
			return NULL;
		}

	} while ((ds_readbs((SAMPLE_AD_IDX_ENTRY+1)) != a1) || (ds_readbs(SAMPLE_AD_IDX_ENTRY) != patch));

	seek_archive_file(ds_readws(SAMPLE_AD_HANDLE), ds_readd((SAMPLE_AD_IDX_ENTRY+2)), 0);

	read_archive_file(ds_readws(SAMPLE_AD_HANDLE), p_datseg + SAMPLE_AD_LENGTH, 2);

	buf = schick_alloc(ds_readw(SAMPLE_AD_LENGTH));

	host_writew(buf, ds_readw(SAMPLE_AD_LENGTH));

	read_archive_file(ds_readws(SAMPLE_AD_HANDLE), buf + 2, ds_readw(SAMPLE_AD_LENGTH) - 2);

	return buf;
#else
	return 0;
#endif
}

/* static */
signed short load_midi_file(signed short index)
{
	return do_load_midi_file(index);
}

/* static */
signed short do_load_midi_file(signed short index)
{
#if defined(__BORLANDC__)
	signed short handle;

	if ((handle = load_archive_file(index)) != -1) {
		read_archive_file(handle, (Bit8u*)ds_readd(AIL_MIDI_BUFFER), 0x7fff);
		close(handle);
		return 1;
	}
#endif
	return 0;
}

/* static */
signed short load_music_driver(Bit8u* fname, signed short type, signed short port)
{
#if defined(__BORLANDC__)
	if (port &&
		(((Bit8u*)ds_writed(AIL_MUSIC_DRIVER_BUF, (Bit32u)read_music_driver(fname)))) &&
		((ds_writew(AIL_MUSIC_DRIVER_ID, AIL_register_driver((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_BUF)))) != 0xffff))
	{

		ds_writed(AIL_MUSIC_DRIVER_DESCR, (Bit32u)AIL_describe_driver(ds_readw(AIL_MUSIC_DRIVER_ID)));

		if (host_readws((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 2) == type)
		{
			if (port == -1) {
				port = host_readws((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 0xc);
			}

			if (AIL_detect_device(ds_readw(AIL_MUSIC_DRIVER_ID), port,
				host_readws((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 0x0e),
				host_readws((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 0x10),
				host_readws((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 0x12)))
			{
				AIL_init_driver(ds_readw(AIL_MUSIC_DRIVER_ID), port,
					host_readws((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 0x0e),
					host_readws((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 0x10),
					host_readws((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 0x12));

				if (type == 3) {
					ds_writed(AIL_STATE_TABLE_SIZE, AIL_state_table_size(ds_readw(AIL_MUSIC_DRIVER_ID)));
					ds_writed(AIL_STATE_TABLE, (Bit32u)schick_alloc(ds_readd(AIL_STATE_TABLE_SIZE)));
					ds_writew(AIL_TIMBRE_CACHE_SIZE, AIL_default_timbre_cache_size(ds_readw(AIL_MUSIC_DRIVER_ID)));

					if (ds_readw(AIL_TIMBRE_CACHE_SIZE) != 0) {
						ds_writed(AIL_TIMBRE_CACHE, (Bit32u)schick_alloc(ds_readw(AIL_TIMBRE_CACHE_SIZE)));
						AIL_define_timbre_cache(ds_readw(AIL_MUSIC_DRIVER_ID),
								(Bit8u*)ds_readd(AIL_TIMBRE_CACHE),
								ds_readw(AIL_TIMBRE_CACHE_SIZE));
					}
				}

				ds_writew(LOAD_SOUND_DRIVER, 0);
				return 1;
			} else {

				/* no sound hardware found */
				GUI_output((char*)p_datseg + SND_TXT_HW_NOT_FOUND);
				exit_AIL();
			}
		}
	}

	ds_writew(LOAD_SOUND_DRIVER, 1);
#endif
	return 0;
}

/* static */
void do_play_music_file(signed short index)
{
#if defined(__BORLANDC__)
	if ((ds_readw(LOAD_SOUND_DRIVER) == 0) && (host_readw((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 2) == 3)) {

		stop_midi_playback();
		load_midi_file(index);
		start_midi_playback(0);
	}
#endif
}

/* static */
void stop_midi_playback(void)
{
#if defined(__BORLANDC__)
	if ((ds_readw(LOAD_SOUND_DRIVER) == 0) && (host_readw((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 2) == 3))
	{
		AIL_stop_sequence(ds_readws(AIL_MUSIC_DRIVER_ID), ds_readws(AIL_SEQUENCE));
		AIL_release_sequence_handle(ds_readws(AIL_MUSIC_DRIVER_ID), ds_readws(AIL_SEQUENCE));
	}
#endif
}

void start_midi_playback_IRQ(void)
{
#if defined(__BORLANDC__)
	if ((ds_readw(LOAD_SOUND_DRIVER) == 0) &&
		(g_music_enabled != 0) &&
		(host_readw((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 2) == 3))
	{
		if (AIL_sequence_status(ds_readws(AIL_MUSIC_DRIVER_ID), ds_readws(AIL_SEQUENCE)) == 2) {
			AIL_start_sequence(ds_readws(AIL_MUSIC_DRIVER_ID), ds_readws(AIL_SEQUENCE));
		}
	}
#endif
}

void cruft_1(void)
/* This function is never called */
{
	if ((ds_readw(LOAD_SOUND_DRIVER) == 0) &&
		(host_readw((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 2) == 3))
	{
		AIL_start_sequence(ds_readws(AIL_MUSIC_DRIVER_ID), ds_readws(AIL_SEQUENCE));
	}
}

void cruft_2(signed short volume)
/* This function is never called */
{
	if (ds_readw(LOAD_SOUND_DRIVER) == 0) {

		if (host_readw((Bit8u*)ds_readd(AIL_MUSIC_DRIVER_DESCR) + 2) == 3) {
			AIL_set_relative_volume(ds_readws(AIL_MUSIC_DRIVER_ID), ds_readws(AIL_SEQUENCE), volume, 0);
		}

		if (!volume) {
			stop_midi_playback();
		}
	}
}


signed short have_mem_for_sound(void)
{
#if defined(__BORLANDC__)
	Bit32s size;
	signed short retval;
	struct ffblk blk;

	if (!findfirst((char*)(p_datseg + FNAME_SOUND_ADV), &blk, 0)) {
		/* SOUND.ADV was found */
		size = host_readd((Bit8u*)(&blk) + 26);
		size += 4000L;

		if ((Bit32u)size < farcoreleft()) {
			retval = 1;

			if ((Bit32u)(size + 25000L) < farcoreleft()) {

				g_snd_voc_enabled = 1;
			}
		} else {
			retval = 0;
		}
	} else {
		/* SOUND.ADV was not found */
		retval = 1;

		if (25000L < farcoreleft()) {
			g_snd_voc_enabled = 1;
		}
	}

	return retval;
#else
	return 0;
#endif
}

void play_voc(signed short index)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled && g_snd_effects_enabled) {
		SND_set_volume(90);
		SND_play_voc(index);
	}
#endif
}

void play_voc_delay(signed short index)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled && g_snd_effects_enabled) {
		SND_set_volume(90);
		SND_play_voc(index);

		while (AIL_VOC_playback_status(ds_readw(AIL_DIGI_DRIVER_ID)) == 2) {
			wait_for_vsync();
		}
	}
#endif
}

void alloc_voc_buffer(Bit32u size)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled) {
		if ((((Bit8u*)ds_writed(AIL_VOC_BUFFER, (Bit32u)schick_alloc(size))))) ;
	}
#endif
}

/* static */
void free_voc_buffer(void)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled != 0) {

		if (ds_readd(AIL_VOC_BUFFER) != 0) {
			free((void*)ds_readd(AIL_VOC_BUFFER));
		}

		if (ds_readd(AIL_DIGI_DRIVER_BUF2) != 0) {
			free((void*)ds_readd(AIL_DIGI_DRIVER_BUF2));
		}

		ds_writed(AIL_VOC_BUFFER, ds_writed(AIL_DIGI_DRIVER_BUF2, 0));

	}
#endif
}

signed short read_new_voc_file(signed short index)
{
#if defined(__BORLANDC__)
	if (AIL_VOC_playback_status(ds_readw(AIL_DIGI_DRIVER_ID)) == 2) {
		SND_stop_digi();
	}

	if (read_voc_file(index)) {

		AIL_format_VOC_file(ds_readw(AIL_DIGI_DRIVER_ID), (Bit8u*)ds_readd(AIL_VOC_BUFFER), -1);
		return 1;
	}
#endif
	return 0;
}

signed short read_voc_file(signed short index)
{
#if defined(__BORLANDC__)
	signed short handle;

	if ( (handle = load_archive_file(index)) != -1) {
		read_archive_file(handle, (Bit8u*)ds_readd(AIL_VOC_BUFFER), 0x7fff);
		close(handle);
		return 1;
	}
#endif
	return 0;
}

void SND_play_voc(signed short index)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled) {

		AIL_stop_digital_playback(ds_readw(AIL_DIGI_DRIVER_ID));
		read_new_voc_file(index);
		AIL_play_VOC_file(ds_readw(AIL_DIGI_DRIVER_ID), (Bit8u*)ds_readd(AIL_VOC_BUFFER), -1);
		AIL_start_digital_playback(ds_readw(AIL_DIGI_DRIVER_ID));
	}
#endif
}

void SND_stop_digi(void)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled) {
		AIL_stop_digital_playback(ds_readw(AIL_DIGI_DRIVER_ID));
	}
#endif
}

void SND_set_volume(unsigned short volume)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled) {

		AIL_set_digital_playback_volume(ds_readw(AIL_DIGI_DRIVER_ID), volume);

		if (!volume) {
			SND_stop_digi();
		}
	}
#endif
}

/* static */
signed short load_digi_driver(Bit8u* fname, signed short type, signed short io, signed short irq)
{
#if defined(__BORLANDC__)
	if (io &&
		((Bit8u*)ds_writed(AIL_DIGI_DRIVER_BUF, (Bit32u)read_digi_driver(fname))) &&
		((ds_writew(AIL_DIGI_DRIVER_ID, AIL_register_driver((Bit8u*)ds_readd(AIL_DIGI_DRIVER_BUF)))) != 0xffff))
	{

		ds_writed(AIL_DIGI_DRIVER_DESCR, (Bit32u)AIL_describe_driver(ds_readw(AIL_DIGI_DRIVER_ID)));

		if (host_readws((Bit8u*)ds_readd(AIL_DIGI_DRIVER_DESCR) + 2) == type) {

			if (io == -1) {
				io = host_readws((Bit8u*)ds_readd(AIL_DIGI_DRIVER_DESCR) + 0xc);
				irq = host_readws((Bit8u*)ds_readd(AIL_DIGI_DRIVER_DESCR) + 0xe);
			}

			if (AIL_detect_device(ds_readw(AIL_DIGI_DRIVER_ID), io, irq,
				host_readws((Bit8u*)ds_readd(AIL_DIGI_DRIVER_DESCR) + 0x10),
				host_readws((Bit8u*)ds_readd(AIL_DIGI_DRIVER_DESCR) + 0x12)))
			{
				AIL_init_driver(ds_readw(AIL_DIGI_DRIVER_ID), io, irq,
					host_readws((Bit8u*)ds_readd(AIL_DIGI_DRIVER_DESCR) + 0x10),
					host_readws((Bit8u*)ds_readd(AIL_DIGI_DRIVER_DESCR) + 0x12));

				g_snd_effects_enabled = 1;
				return 1;
			} else {
				/* no sound hardware found */
				GUI_output((char*)p_datseg + SND_TXT_HW_NOT_FOUND2);
				free_voc_buffer();
			}
		}
	}
#endif

	return 0;
}

unsigned char* read_digi_driver(char *fname)
{
#if defined(__BORLANDC__)
	Bit32u len;
	Bit8u *buf;
	Bit32u ptr;

	signed short handle;

	if ( (handle = open(fname, O_BINARY | O_RDONLY)) != -1) {

		len = 5000L;

		ds_writed(AIL_DIGI_DRIVER_BUF2, (Bit32u)schick_alloc(len + 16L));
		ptr = ds_readd(AIL_DIGI_DRIVER_BUF2) + 15L;
		ptr &= 0xfffffff0;
		buf = EMS_norm_ptr((Bit8u*)ptr);
		_read(handle, (Bit8u*)buf, (unsigned short)len);
		close(handle);
		return buf;
	}
#endif
	return NULL;
}

/**
 * \brief   opens SCHICK.DAT and seeks to desired position
 *
 * \param   fileindex   the index of the file in SCHICK.DAT
 * \return              the filehandle or 0xffff.
 */
/* static */
signed short open_and_seek_dat(unsigned short fileindex)
{
	Bit32u start, end;
	signed short fd;

	/* open SCHICK.DAT */
	if ( (fd = open((char*)(p_datseg + FNAME_SCHICK_DAT), O_BINARY | O_RDONLY)) != -1) {

		/* seek to the fileindex position in the offset table */
		lseek(fd, fileindex * 4, SEEK_SET);

		/* read the start offset of the desired file */
		_read(fd, (Bit8u*)&start, 4);

		/* read the start offset of the next file */
		_read(fd, (Bit8u*)&end, 4);
#if !defined(__BORLANDC__)
		/* BE-Fix */
		start = host_readd((Bit8u*)&start);
		end = host_readd((Bit8u*)&end);
#endif

		/* seek to the desired file */
		lseek(fd, start, SEEK_SET);

		/* save the offset of the desired file */
		ds_writed(ARCHIVE_FILE_OFFSET, start);

		/* save the length of the desired file in 2 variables */
		ds_writed(ARCHIVE_FILE_LENGTH, (g_archive_file_remaining = end - start));
	}

	return fd;
}

Bit32u get_readlength2(signed short index)
{
	return index != -1 ? ds_readd(ARCHIVE_FILE_LENGTH) : 0;
}

/**
 * \brief   reads len bytes from the current position in SCHICK.DAT
 *
 * \param   handle      handle returned by load_archive_file
 * \param   buffer      target buffer for the data
 * \param   len         number of bytes to read
 * \return              number of bytes read
 */
unsigned short read_archive_file(Bit16u handle, Bit8u *buffer, Bit16u len)
{

	/* no need to read */
	if (g_archive_file_remaining != 0) {

		/* adjust number of bytes to read */
		if (len > g_archive_file_remaining)
			len = (Bit16u)g_archive_file_remaining;

		g_archive_file_remaining -= len;

		return _read(handle, buffer, len);
	} else {
		return 0;
	}
}

/**
 * \brief   seeks to a position in a file in SCHICK.DAT
 *
 * \param   handle      handle returned by load_archive_file
 * \param   off         position to seek for
 */
void seek_archive_file(Bit16u handle, Bit32s off, ...)
{

	Bit32u file_off;

	g_archive_file_remaining = (Bit32s)(ds_readd(ARCHIVE_FILE_LENGTH) - off);

	file_off = ds_readd(ARCHIVE_FILE_OFFSET) + off;

	lseek(handle, file_off, SEEK_SET);

	return;
}

signed short load_regular_file(Bit16u index)
{
	signed short handle;

	if ( (handle = open((char*)ds_readd(FNAMES + index * 4), O_BINARY | O_RDWR)) == -1) {

		sprintf((char*)g_dtp2, g_str_file_missing_ptr, (char*)ds_readd(FNAMES + index * 4));

		ds_writeb(MISSING_FILE_GUILOCK, 1);
		GUI_output((char*)g_dtp2);
		ds_writeb(MISSING_FILE_GUILOCK, 0);
	}

	return handle;
}

/**
 * \brief   opens a file stored in temp or in SCHICK.DAT
 *
 * \param   index       index of the file in SCHICK.DAT or in temp (bitwise or 0x8000)
 * \return              a file handle that can be used with read_archive_file etc.
 */
signed short load_archive_file(Bit16u index)
{
#if defined(__BORLANDC__)
	flushall();
#endif

	return (index & 0x8000) ? open_temp_file(index & 0x7fff) : open_and_seek_dat(index);
}

signed short open_temp_file(unsigned short index)
{
	char tmppath[40];
	signed short handle;

	sprintf((char*)tmppath, (char*)ds_readd(STR_TEMP_XX_PTR2), (char*)ds_readd(FNAMES + index * 4));

	while ( (handle = open(tmppath, O_BINARY | O_RDWR)) == -1) {

		copy_from_archive_to_temp(index, tmppath);
	}

	/* get the length of the file */
	ds_writed(ARCHIVE_FILE_LENGTH, (g_archive_file_remaining = lseek(handle, 0, 2)));
	/* seek to start */
	lseek(handle, 0, 0);

	ds_writed(ARCHIVE_FILE_OFFSET, 0);

	return handle;
}

void copy_from_archive_to_temp(unsigned short index, char* fname)
{
	signed short handle1;
	signed short handle2;
	signed short len;

	if ( (handle1 = load_archive_file(index)) != -1) {

		/* create new file in TEMP */
		/* TODO: should be O_BINARY | O_WRONLY */
		handle2 = _creat(fname, 0);

		/* copy it */
		while ( (len = read_archive_file(handle1, g_renderbuf_ptr, 60000)) && (len != -1))
		{
			write(handle2, g_renderbuf_ptr, len);
		}

		close(handle1);
		close(handle2);
	}
}

void copy_file_to_temp(char* src_file, char* fname)
{
	signed short handle1;
	signed short handle2;
	signed short len;

	if ( (handle1 = open(src_file, O_BINARY | O_RDONLY)) != -1) {

		/* create new file in TEMP */
		/* TODO: should be O_BINARY | O_WRONLY */
		handle2 = _creat(fname, 0);

		/* copy it */
		while ( (len = _read(handle1, g_renderbuf_ptr, 60000)) && (len != -1))
		{
			write(handle2, g_renderbuf_ptr, len);
		}

		close(handle1);
		close(handle2);
	}
}

Bit32s process_nvf(struct nvf_desc *nvf)
{
	signed short i;
	Bit32u offs;
	signed short pics;
	signed short width;
	signed short height;
	signed short va;
	Bit32u p_size;
	Bit32u retval;
	signed char nvf_type;
#if !defined(__BORLANDC__)
	Bit8u *nvf_no;
	Bit8u *src;
#else
	Bit8u huge *nvf_no;
	Bit8u huge *src;
#endif
	Bit8u *dst;

	nvf_type = host_readb(nvf->src);
	va = nvf_type & 0x80;
	nvf_type &= 0x7f;

	pics = host_readws(F_PADD(nvf->src, 1L));

	if (nvf->no < 0)
		nvf->no = 0;

	if (nvf->no > pics - 1)
		nvf->no = pics - 1;

	switch (nvf_type) {

	case 0x00:
		width = host_readws(F_PADD(nvf->src, 3));
		height = host_readws(F_PADD(nvf->src, 5));
		p_size = width * height;
		src =  F_PADD(nvf->src, nvf->no * p_size + 7);
		break;

	case 0x01:
		offs = pics * 4 + 3L;
		for (i = 0; i < nvf->no; i++) {
#if !defined(__BORLANDC__)
			width = host_readw(nvf->src + i * 4 + 3);
			height = host_readw(nvf->src + i * 4 + 5);
#endif
			offs += width * height;
		}

		width = host_readw(F_PADD(nvf->src, nvf->no * 4 + 3));
		height = host_readw(F_PADD(nvf->src, nvf->no * 4 + 5));
		p_size = width * height;
		src = F_PADD(nvf->src, offs);
		break;

	case 0x02: case 0x04:
		width = host_readw(F_PADD(nvf->src, 3L));
		height = host_readw(F_PADD(nvf->src, 5));
		offs = pics * 4 + 7L;
		for (i = 0; i < nvf->no; i++) {
			offs += host_readd(F_PADD(nvf->src, (i * 4) + 7));
		}

		p_size = host_readd(F_PADD(nvf->src, nvf->no * 4 + 7));
		src = F_PADD(nvf->src, offs);
		break;

	case 0x03: case 0x05:
		offs = pics * 8 + 3L;
		for (i = 0; i < nvf->no; i++)
			offs += host_readd(F_PADD(nvf->src, (i * 8) + 7));

		width = host_readw(F_PADD(nvf->src, nvf->no * 8 + 3));
		height = host_readw(F_PADD(nvf->src, nvf->no * 8 + 5));
		p_size = host_readd(F_PADD(nvf->src, i * 8 + 7));
		src = F_PADD(nvf->src, offs);
		break;
	}

	if (!nvf->type) {

		/* PP20 decompression */

		if (va != 0) {

			/* get size from unpacked picture */
			retval = host_readd(src);
			nvf_no = src;
			nvf_no += (retval + (-4L));
			retval = host_readd(nvf_no);
			retval = swap_u32(retval) >> 8;

		} else {
			retval = width * height;
		}

#if !defined(__BORLANDC__)
		decomp_pp20(src, nvf->dst, src + (signed char)4, p_size);
#else
		decomp_pp20(src,
			nvf->dst,
			FP_OFF(src) + 4,
			FP_SEG(src),
			p_size);
#endif

	} else if (nvf->type >= 2 && nvf->type <= 5) {

		dst = nvf->dst;

		/* RLE decompression */
		decomp_rle(width, height, dst, src, (Bit8u*)g_text_output_buf, nvf->type);
#ifdef M302de_ORIGINAL_BUGFIX
		/* retval was originally neither set nor used here.
			VC++2008 complains about an uninitialized variable
			on a Debug build, so we fix this for debuggings sake */
		/* Orig-Fix */
		retval = p_size;
#endif
	} else {
		/* No decompression, just copy */
		memmove(nvf->dst, src, (short)p_size);
		retval = p_size;
	}

	host_writew(nvf->width, width);
	host_writew(nvf->height, height);

	return retval;
}

/**
 * \brief   does mouse programming
 *
 * \param   p1          function AX
 * \param   p2          depends on AX
 * \param   p3          depends on AX
 * \param   p4          depends on AX
 * \param   p5          depends on AX
 *
 * This function differs a bit. The Borland C-Library has a special function
 * to call interrupts. We use the one of DOSBox, which means, that we
 * put the values in the emulated registers, instead in a structure.
 */
void mouse_action(Bit8u *p1, Bit8u *p2, Bit8u *p3, Bit8u *p4, Bit8u *p5)
{
#if defined(__BORLANDC__)
	struct SREGS sregs;
	union REGS wregs;

	if (host_readws(p1) >= 0) {

		wregs.x.ax = host_readw(p1);
		wregs.x.bx = host_readw(p2);
		wregs.x.cx = host_readw(p3);

		switch (host_readw(p1)) {
		case 0x9:	/* define Cursor in graphic mode */
		case 0xc:	/* install event handler */
		case 0x14:	/* swap event handler */
		case 0x16:	/* save mouse state */
		case 0x17:	/* load mouse state */
		{
			wregs.x.dx = host_readw(p4);
			sregs.es = host_readw(p5);
			break;
		}
		case 0x10: {
			wregs.x.cx = host_readw(p2);
			wregs.x.dx = host_readw(p3);
			wregs.x.si = host_readw(p4);
			wregs.x.di = host_readw(p5);
			break;
		}
		default : {
			wregs.x.dx = host_readw(p4);
		}
		}

		int86x(0x33, &wregs, &wregs, &sregs);

		if (host_readw(p1) == 0x14) {
			host_writew(p2, sregs.es);
		} else {
			host_writew(p2, wregs.x.bx);
		}

		host_writew(p1, wregs.x.ax);
		host_writew(p3, wregs.x.cx);
		host_writew(p4, wregs.x.dx);
	}
#endif
}

#if defined(__BORLANDC__)
void interrupt mouse_isr(void)
{
	signed short l_si = _AX;
	signed short l1;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;

	if (ds_readws(MOUSE_LOCKED) == 0) {

		if (l_si & 0x2) {
			ds_writew(MOUSE1_EVENT2, 1);
			ds_writew(MOUSE1_EVENT1, 1);
		}

		if (l_si & 0x8) {
			ds_writew(MOUSE2_EVENT, 1);
		}

		if (((gs_dungeon_index != DUNGEONS_NONE) || (gs_current_town != TOWNS_NONE)) &&
				!gs_current_loctype &&
				!ds_readbs(DIALOGBOX_LOCK) &&
				(ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK))
		{
			ds_writed(CURRENT_CURSOR, (Bit32u) (is_mouse_in_rect(68, 4, 171, 51) ? (p_datseg + CURSOR_ARROW_UP):
							(is_mouse_in_rect(68, 89, 171, 136) ? (p_datseg + CURSOR_ARROW_DOWN) :
							(is_mouse_in_rect(16, 36, 67, 96) ? (p_datseg + CURSOR_ARROW_LEFT) :
							(is_mouse_in_rect(172, 36, 223, 96) ? (p_datseg + CURSOR_ARROW_RIGHT) :
							(!is_mouse_in_rect(16, 4, 223, 138) ? (p_datseg + DEFAULT_MOUSE_CURSOR) :
								(void*)ds_readd(CURRENT_CURSOR)))))));
		} else {
			if (ds_readbs(DIALOGBOX_LOCK) != 0) {
				ds_writed(CURRENT_CURSOR, (Bit32u) (p_datseg + DEFAULT_MOUSE_CURSOR));
			}
		}

		if (l_si & 1) {
			l1 = 3;
			l4 = ds_readws(MOUSE_POSX);
			l5 = ds_readws(MOUSE_POSY);

			mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);

			ds_writew(MOUSE_POSX, l4);
			ds_writew(MOUSE_POSY, l5);

			if (ds_readws(MOUSE_POSX) > ds_readws(MOUSE_POSX_MAX)) {
				ds_writew(MOUSE_POSX, ds_readws(MOUSE_POSX_MAX));
			}
			if (ds_readws(MOUSE_POSX) < ds_readws(MOUSE_POSX_MIN)) {
				ds_writew(MOUSE_POSX, ds_readws(MOUSE_POSX_MIN));
			}
			if (ds_readws(MOUSE_POSY) < ds_readws(MOUSE_POSY_MIN)) {
				ds_writew(MOUSE_POSY, ds_readws(MOUSE_POSY_MIN));
			}
			if (ds_readws(MOUSE_POSY) > ds_readws(MOUSE_POSY_MAX)) {
				ds_writew(MOUSE_POSY, ds_readws(MOUSE_POSY_MAX));
			}

			l1 = 4;
			l4 = ds_readws(MOUSE_POSX);
			l5 = ds_readws(MOUSE_POSY);

			mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);

			ds_writew(MOUSE_MOVED, 1);
		}
	}
}
#endif

/**
 * \brief   checks if the mouse cursor is in a rectangle
 *
 * \param   x1          upper left x coordinate
 * \param   y1          upper left y coordinate
 * \param   x2          bottom right x coordinate
 * \param   y2          bottom right y coordinate
 * \return              1 if the pointer is in this rectangle, otherwise 0
 */
signed short is_mouse_in_rect(signed short x1, signed short y1,
				signed short x2, signed short y2)
{
	signed short m_x;
	signed short m_y;

	m_x = ds_readws(MOUSE_POSX);
	m_y = ds_readws(MOUSE_POSY);

	return ((m_x >= x1) && (m_x <= x2) && (m_y >= y1) && (m_y <= y2)) ? 1 : 0;
}

void mouse_init(void)
{
	if (ds_readw(HAVE_MOUSE) == 2) {

#if defined(__BORLANDC__)
		unsigned short p1, p2, p3, p4, p5;

		p1 = 0;

		mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);

		if (p1 == 0) {
			ds_writew(HAVE_MOUSE, 0);
		}

		ds_writed(CURRENT_CURSOR, (Bit32u)(p_datseg + DEFAULT_MOUSE_CURSOR));
		ds_writed(LAST_CURSOR, (Bit32u)(p_datseg + DEFAULT_MOUSE_CURSOR));

		if (ds_readw(HAVE_MOUSE) == 2) {

			p1 = 4;
			p3 = ds_readws(MOUSE_POSX);
			p4 = ds_readws(MOUSE_POSY);

			mouse_action((Bit8u*)&p1, (Bit8u*)&p2, (Bit8u*)&p3, (Bit8u*)&p4, (Bit8u*)&p5);

			mouse_irq_init(0x1f, (unsigned char*)&mouse_isr);
		}
#endif
	}
}

void disable_mouse(void)
{
	if (ds_readw(HAVE_MOUSE) == 2) {
		mouse_reset_ehandler();
	}
}

void seg002_170e(Bit8u *a1, Bit8u *a2, Bit8u *a3, Bit8u *a4)
{
	signed short tmp;

	host_writew(a1, 5);

	mouse_action(a1, a2, a3, a4, (Bit8u*)&tmp);
}

#if defined(__BORLANDC__)
void call_mouse_isr(void)
{
	mouse_isr();
}

void mouse_irq_init(signed short irq_no, void interrupt *(isr))
{
	signed short l1;
	signed short l3;
	signed short l4;
	signed short l5;
	signed short l6;

	l1 = 12;
	l4 = irq_no;

	/* TODO : keep the numbers here until we can build the binary */
/*	l5 = FP_OFF(call_mouse_isr);
	l6 = FP_SEG(call_mouse_isr); */
	l5 = 0x1742;
	l6 = 0x51e;

	ds_writed(MOUSE_HANDLER_BAK, (Bit32u)getvect(0x78));
	setvect(0x78, (void interrupt far (*)(...))isr);

	mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);

	g_mouse_irq_init = 1;
}
#endif


void mouse_reset_ehandler(void)
{
#if defined(__BORLANDC__)
	signed short l1;
	signed short l2;
	signed short l3;
	signed short l4;
	signed short l5;

	setvect(0x78, (void interrupt far (*)(...))ds_readd(MOUSE_HANDLER_BAK));

	l1 = 12;
	l3 = 0;
	l4 = 0;
	l5 = 0;

	mouse_action((Bit8u*)&l1, (Bit8u*)&l2, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5);

	g_mouse_irq_init = 1;
#endif
}

/**
 * \brief   move the mouse cursor to a position on the screen
 *
 * \param   x           X - coordinate
 * \param   y           Y - coordinate
 */
void mouse_move_cursor(signed short x, signed short y)
{
	signed short l1 = 4;
	signed short l3;
	signed short l4 = x;
	signed short l5 = y;
	signed short l6;

	mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);
}

/* unused */
void seg002_1838(signed short a1, signed short a2, signed short a3, signed short a4)
{
	signed short l1 = 9;
	signed short l3 = a1;
	signed short l4 = a2;
	signed short l5 = a3;
	signed short l6 = a4;

	mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);
}

/* unused */
void seg002_1880(signed short a1)
{
	signed short l1 = 29;
	signed short l3 = a1;
	signed short l4;
	signed short l5;
	signed short l6;

	mouse_action((Bit8u*)&l1, (Bit8u*)&l3, (Bit8u*)&l4, (Bit8u*)&l5, (Bit8u*)&l6);
}


/**
 * \brief   makes a mouse cursor from a selected item
 *
 * \param   p           pointer to the icon of the item
 */
void make_ggst_cursor(Bit8u *icon)
{
	signed short y;
	signed short x;

	/* clear the bitmask */
	for (y = 0; y < 16; y++) {
		ds_writew((GGST_CURSOR + 32) + y * 2, 0);
	}

	/* make a bitmask from the icon */
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			/* if pixelcolor of the icon is not black */
			if (*icon++ != 0x40) {
				or_ds_ws((GGST_CURSOR + 32) + y * 2, (0x8000 >> x));
			}
		}
	}

	/* copy and negate the bitmask */
	for (y = 0; y < 16; y++) {
		ds_writew(GGST_CURSOR + y * 2, ~ds_readw((GGST_CURSOR + 32) + y * 2));
	}
}

void update_mouse_cursor(void)
{
	update_mouse_cursor1();
}

void refresh_screen_size(void)
{
	refresh_screen_size1();
}

void update_mouse_cursor1(void)
{
	if (ds_readw(MOUSE_LOCKED) == 0) {

		if  (ds_readw(MOUSE_REFRESH_FLAG) == 0) {
			ds_writew(MOUSE_LOCKED, 1);
			restore_mouse_bg();
			ds_writew(MOUSE_LOCKED, 0);
		}

		dec_ds_ws(MOUSE_REFRESH_FLAG);
	}
}

void refresh_screen_size1(void)
{
	/* check lock */
	if (ds_readw(MOUSE_LOCKED) == 0) {

		inc_ds_ws(MOUSE_REFRESH_FLAG);

		if (ds_readw(MOUSE_REFRESH_FLAG) == 0) {

			/* get lock */
			ds_writew(MOUSE_LOCKED, 1);

			if (ds_readws(MOUSE_POSX) < ds_readws(MOUSE_POINTER_OFFSETX))
				ds_writew(MOUSE_POSX, ds_readw(MOUSE_POINTER_OFFSETX));

			if (ds_readws(MOUSE_POSX) > 315)
				ds_writew(MOUSE_POSX, 315);

			if (ds_readws(MOUSE_POSY) < ds_readws(MOUSE_POINTER_OFFSETY))
				ds_writew(MOUSE_POSY, ds_readw(MOUSE_POINTER_OFFSETY));

			if (ds_readws(MOUSE_POSY) > 195)
				ds_writew(MOUSE_POSY, 195);

			save_mouse_bg();
			ds_writew(MOUSE_POSX_BAK, ds_readw(MOUSE_POSX));
			ds_writew(MOUSE_POSY_BAK, ds_readw(MOUSE_POSY));
			ds_writew(MOUSE_POINTER_OFFSETX_BAK, ds_readw(MOUSE_POINTER_OFFSETX));
			ds_writew(MOUSE_POINTER_OFFSETY_BAK, ds_readw(MOUSE_POINTER_OFFSETY));
			draw_mouse_cursor();

			/* put lock */
			ds_writew(MOUSE_LOCKED, 0);
		}
	}
}

void mouse_19dc(void)
{
	/* return if mouse was not moved and the cursor remains */
	if ((ds_readw(MOUSE_MOVED) != 0) || (ds_readd(LAST_CURSOR) != ds_readd(CURRENT_CURSOR))) {

		/* set new cursor */
		ds_writed(LAST_CURSOR, ds_readd(CURRENT_CURSOR));

		/* check if the new cursor is the default cursor */
		if ((Bit8u*)ds_readd(CURRENT_CURSOR) == p_datseg + DEFAULT_MOUSE_CURSOR) {
			/* set cursor size 0x0 */
			ds_writew(MOUSE_POINTER_OFFSETX, ds_writew(MOUSE_POINTER_OFFSETY, 0));
		} else {
			/* set cursor size 8x8 */
			ds_writew(MOUSE_POINTER_OFFSETX, ds_writew(MOUSE_POINTER_OFFSETY, 8));
		}

		/* reset mouse was moved */
		ds_writew(MOUSE_MOVED, 0);
		update_mouse_cursor1();
		refresh_screen_size1();
	}
}

void handle_gui_input(void)
{
	signed short l_si;
	signed short tw_bak;
	signed short l1;

	ds_writew(BIOSKEY_EVENT, ds_writew(ACTION, l_si = 0));

	herokeeping();

	if (CD_bioskey(1)) {

		l_si = (ds_writews(BIOSKEY_EVENT, bioskey(0))) >> 8;
		and_ds_ws(BIOSKEY_EVENT, 0xff);

		if (l_si == 0x24) {
			l_si = 0x2c;
		}

		/* Ctrl + Q -> quit */
		if ((ds_readw(BIOSKEY_EVENT) == 0x11) && (ds_readw(PREGAME_STATE) == 0)) {
			cleanup_game();

			exit(0);
		}
	}

	if (ds_readw(MOUSE1_EVENT2) == 0) {

		ds_writew(ALWAYS_ZERO3, 0);

		if (ds_readw(HAVE_MOUSE) == 0) {
		}

		/* Ctrl + E */
		if (ds_readw(BIOSKEY_EVENT) == 5) {
			status_select_hero();
			l_si = 0;
		}

		/* Ctrl + O -> swap heroes */
		if (ds_readw(BIOSKEY_EVENT) == 15) {
			GRP_swap_heroes();
			l_si = 0;
		}

		/* Ctrl + S -> sound menu */
		if ((ds_readw(BIOSKEY_EVENT) == 0x13) && !ds_readbs(DIALOGBOX_LOCK)) {
			sound_menu();
		}

		/* Ctrl + P -> pause game */
		if ((ds_readw(BIOSKEY_EVENT) == 0x10) &&
			(ds_readws(BIOSKEY_EVENT10) == 0) &&
			!ds_readbs(DIALOGBOX_LOCK) &&
			(ds_readws(PREGAME_STATE) == 0))
		{
			ds_writew(BIOSKEY_EVENT10, 1);
			g_timers_disabled++;
			ds_writew(GUI_TEXT_CENTERED, 1);
			tw_bak = g_textbox_width;
			g_textbox_width = 2;
			GUI_output(g_pause_string);		/* P A U S E */
			g_textbox_width = tw_bak;
			ds_writew(GUI_TEXT_CENTERED, 0);
			ds_writew(BIOSKEY_EVENT10, l_si = ds_writew(BIOSKEY_EVENT, 0));
			g_timers_disabled--;
		}
	} else {
		play_voc(ARCHIVE_FILE_FX1_VOC);
		ds_writew(MOUSE1_EVENT2, 0);
		l_si = 0;

		if (((Bit8u*)ds_readd(ACTION_TABLE_SECONDARY))) {
			l_si = get_mouse_action(ds_readw(MOUSE_POSX),
					ds_readw(MOUSE_POSY),
					(Bit8u*)ds_readd(ACTION_TABLE_SECONDARY));
		}

		if (!l_si && ((Bit8u*)ds_readd(ACTION_TABLE_PRIMARY))) {
			l_si = get_mouse_action(ds_readw(MOUSE_POSX),
					ds_readw(MOUSE_POSY),
					(Bit8u*)ds_readd(ACTION_TABLE_PRIMARY));
		}

		if (ds_readw(HAVE_MOUSE) == 2) {

			for (l1 = 0; l1 < 15; l1++) {
				wait_for_vsync();
			}

			if (ds_readw(MOUSE1_EVENT2) != 0) {
				ds_writew(MOUSE1_DOUBLECLICK, 1);
				ds_writew(MOUSE1_EVENT2, 0);
			}
		}

		if ((l_si >= 0xf1) && (l_si <= 0xf8)) {

			if (ds_readws(MOUSE1_DOUBLECLICK) != 0) {

				/* open character screen by double click on hero picture */
				if ((host_readbs(get_hero(l_si - 241) + HERO_TYPE) != HERO_TYPE_NONE) &&
						host_readbs(get_hero(l_si - 241) + HERO_GROUP_NO) == gs_current_group)
				{
					status_menu(l_si - 241);
					l_si = 0;
					ds_writew(MOUSE1_DOUBLECLICK, 0);
					ds_writew(MOUSE1_EVENT2, 0);
				}
			} else {
				/* swap heroes by click - move mouse - click */
				if ((ds_readws(HEROSWAP_ALLOWED) != 0) &&
					(host_readbs(get_hero(l_si - 241) + HERO_TYPE) != HERO_TYPE_NONE) &&
						host_readbs(get_hero(l_si - 241) + HERO_GROUP_NO) == gs_current_group)
				{
					/* the destination will be selected by a mouse klick in the following function call */
					GRP_move_hero(l_si - 241);
					l_si = 0;
					ds_writew(MOUSE1_DOUBLECLICK, 0);
					ds_writew(MOUSE1_EVENT2, 0);
				}
			}
		} else if (l_si == 0xfd) {
			/* Credits */

			l_si = 0;
			tw_bak = g_textbox_width;
			g_textbox_width = 5;
			ds_writew(GUI_TEXT_CENTERED, 1);
			GUI_output(get_ttx(394));
			ds_writew(GUI_TEXT_CENTERED, 0);
			g_textbox_width = tw_bak;

		} else if (l_si == 0xfc) {
			/* Clock */
			l_si = 0;
			tw_bak = g_textbox_width;
			g_textbox_width = 5;
			ds_writew(GUI_TEXT_CENTERED, 1);
			prepare_date_str();
			GUI_output((char*)g_dtp2);
			ds_writew(GUI_TEXT_CENTERED, 0);
			g_textbox_width = tw_bak;

		}
	}

	mouse_19dc();
	ds_writew(ACTION, l_si);
}

/**
 * \brief   finds a rectangle containing (x,y)
 *
 * \param   x the x-coordinate
 * \param   y the y-coordinate
 * \param   a pointer to a (-1)-terminated list of signed 2-byte-values.
 *          each block of 5 * 2 bytes is interpreted as the coordinates of a rectangle, and a return value,
*           in the order lower-x-bound, upper-x-bound, lower-y-bound, upper-y-bound, return value
 * \return  if (x,y) is in one of the rectangles, return the return value of the first fitting rectangle.
 *          otherwise, return 0.
 */
signed short get_mouse_action(signed short x, signed short y, Bit8u *rectangles)
{
	signed short i;

	for (i = 0; host_readws(rectangles + i * 10) != -1; i++) {

		if ((host_readws(rectangles + i * 10) <= x) &&
			(host_readws(rectangles + i * 10 + 4) >= x) &&
			(host_readws(rectangles + i * 10 + 2) <= y) &&
			(host_readws(rectangles + i * 10 + 6) >= y))
		{
			return host_readw(rectangles + i * 10 + 8);
		}

	}

	return 0;
}

void handle_input(void)
{
	signed short l_si;
	signed short l_di;

	ds_writew(BIOSKEY_EVENT, ds_writew(ACTION, l_si = 0));

	herokeeping();

	if (CD_bioskey(1)) {

		l_si = (ds_writews(BIOSKEY_EVENT, bioskey(0))) >> 8;
		and_ds_ws(BIOSKEY_EVENT, 0xff);

		if (l_si == 0x24) {
			l_si = 0x2c;
		}

		/* Ctrl + Q -> quit */
		if ((ds_readw(BIOSKEY_EVENT) == 0x11) && (ds_readw(PREGAME_STATE) == 0)) {
			cleanup_game();

			exit(0);
		}
	}

	if (ds_readw(MOUSE1_EVENT2) == 0) {

		if (ds_readw(HAVE_MOUSE) == 0) {
		}

		/* Ctrl + S -> sound menu */
		if ((ds_readw(BIOSKEY_EVENT) == 0x13) && !ds_readbs(DIALOGBOX_LOCK)) {
			sound_menu();
		}

		/* Ctrl + P -> pause game */
		/* TODO: use tw_bak here */
		if ((ds_readw(BIOSKEY_EVENT) == 0x10) &&
			(ds_readws(BIOSKEY_EVENT10) == 0) &&
			!ds_readbs(DIALOGBOX_LOCK) &&
			(ds_readws(PREGAME_STATE) == 0))
		{
			g_timers_disabled++;
			ds_writew(BIOSKEY_EVENT10, 1);
			ds_writew(GUI_TEXT_CENTERED, 1);
			g_textbox_width = 2;
			GUI_output(g_pause_string);		/* P A U S E */
			g_textbox_width = 3;
			ds_writew(GUI_TEXT_CENTERED, 0);
			g_timers_disabled--;

			ds_writew(BIOSKEY_EVENT10, l_si = ds_writew(BIOSKEY_EVENT, 0));
		}
	} else {
		play_voc(ARCHIVE_FILE_FX1_VOC);
		ds_writew(MOUSE1_EVENT2, 0);
		l_si = 0;

		if (((Bit8u*)ds_readd(ACTION_TABLE_SECONDARY))) {
			l_si = get_mouse_action(ds_readw(MOUSE_POSX), ds_readw(MOUSE_POSY),
					(Bit8u*)ds_readd(ACTION_TABLE_SECONDARY));
		}

		if (!l_si && ((Bit8u*)ds_readd(ACTION_TABLE_PRIMARY))) {
			l_si = get_mouse_action(ds_readw(MOUSE_POSX), ds_readw(MOUSE_POSY),
					(Bit8u*)ds_readd(ACTION_TABLE_PRIMARY));
		}

		if (ds_readw(HAVE_MOUSE) == 2) {

			for (l_di = 0; l_di < 25; l_di++) {

				wait_for_vsync();

				if (ds_readw(MOUSE1_EVENT2) != 0) {
					ds_writew(MOUSE1_DOUBLECLICK, 1);
					ds_writew(MOUSE1_EVENT2, 0);
					break;
				}
			}
		}
	}

	mouse_19dc();
	ds_writew(ACTION, l_si);
}

void wait_for_keyboard1(void)
{
	while (CD_bioskey(1)) {

		bioskey(0);
	}
}

void game_loop(void)
{
	signed short answer;

	while (ds_readw(GAME_STATE) == GAME_STATE_MAIN) {

		if (gs_current_loctype != LOCTYPE_NONE) {
			do_location();
		} else if (gs_current_town != TOWNS_NONE) {
			do_town();
		} else if (gs_dungeon_index != DUNGEONS_NONE) {
			do_dungeon();
		} else if (ds_readbs(SHOW_TRAVEL_MAP) != 0) {
			do_travel_mode();
		}

		if (gs_datseg_status_start == 99) {
			ds_writew(GAME_STATE, GAME_STATE_OUTRO);
		}

		if (g_check_disease) {
			disease_effect();
		}

		if (g_check_poison) {
			poison_effect();
		}

		if (g_check_party) {
			/* check the party for active heroes.
			 * If necessary, switch to another group or in fatal case: game over */

			g_check_party = 0;

			if (!count_heroes_available() || ((count_heroes_available() == 1) && check_hero(get_hero(6)))) // count_heroes_available_ignore_npc() == 0
			{
				/* no heroes or only the NPC can act => GAME OVER */
				ds_writew(GAME_STATE, GAME_STATE_DEAD);

			} else if (!count_heroes_available_in_group() || ((count_heroes_available_in_group() == 1) && is_hero_available_in_group(get_hero(6)))) // count_heroes_available_in_group_ignore_npc() == 0
			{
				/* no heroes or only the NPC in this group can act => switch to next */
				GRP_switch_to_next(2);
			}

		}

		if ((host_readbs(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE) &&
			((gs_current_town != TOWNS_NONE) || (ds_readws(GAME_STATE) == GAME_STATE_VICTORY)) &&
			(gs_npc_months >= 1) &&	(g_npc_last_farewellcheck != gs_npc_months))
		{
			npc_farewell();
			g_npc_last_farewellcheck = gs_npc_months;
		}

		if (!g_in_fight &&
			((ds_readws(GAME_STATE) == GAME_STATE_MAIN) || (ds_readws(GAME_STATE) == GAME_STATE_VICTORY)) &&
			!gs_current_loctype)
		{
			check_level_up();
		}

		if (ds_readbs(REFRESH_STATUS_LINE) != 0) {

			ds_writeb(REFRESH_STATUS_LINE, 0);

			if (ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
				draw_status_line();
			}
		}

		if ((ds_readws(GAME_STATE) != GAME_STATE_MAIN) && (g_fading_state != 0)) {
			refresh_colors();
		}

		if (ds_readws(GAME_STATE) == GAME_STATE_DEAD) {
			game_over_screen();
		}

		if (ds_readws(GAME_STATE) == GAME_STATE_TIMEUP) {
			show_times_up();
		}

		if ((ds_readws(GAME_STATE) == GAME_STATE_DEAD) ||
			ds_readws(GAME_STATE) == GAME_STATE_UNKNOWN ||
			ds_readws(GAME_STATE) == GAME_STATE_TIMEUP ||
			ds_readws(GAME_STATE) == GAME_STATE_OUTRO ||
			ds_readws(GAME_STATE) == GAME_STATE_FIGQUIT)
		{
			gs_current_loctype = LOCTYPE_NONE;

			do {
				answer = load_game_state();

			} while (answer == -1);

			if (answer) {
				ds_writew(GAME_STATE, GAME_STATE_MAIN);
				refresh_colors();
			}
		}

		if (ds_readw(GAME_STATE) == GAME_STATE_VICTORY) {
			show_outro();
			cleanup_game();

			exit(0);
		}
	}
}

//static
/*
 *	This function does daily accounting stuff.
 *	The g_check_disease flag is set here, indicating that the disease status should be updated. */

void timers_daily(void)
{
	Bit8u *hero_i;
	signed short i = 0;

	/* Smith / items to repair */
	for (i = 0; i < 50; i++) {

		if (gs_smith_repairitems[i].item_id) {
			/* set time to 6:00 am */
			gs_smith_repairitems[i].pickup_time = HOURS(6);
		}
	}

	/* reset offended merchants */
	for (i = 0; i < 94; i++) {
		gs_merchant_offended_flags[i] = 0;
	}

	hero_i = get_hero(0);
	for (i = 0; i <=6; i++, hero_i += SIZEOF_HERO) {

		if ((host_readb(get_hero(i) + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(hero_i + HERO_RECIPE_TIMER) > 0))
		{
			dec_ptr_bs(hero_i + HERO_RECIPE_TIMER);
		}
	}

	/* enable disease check */
	g_check_disease = 1;

#ifdef M302de_ORIGINAL_BUGFIX
	/* Original-Bug: Reenable identifying item in the academy */
	gs_academy_daily_ident = 0;
#endif

	/* Decrase monthly credit cens timer (bank) */
	if (gs_days_to_cens > 0) {

		gs_days_to_cens--;

		if (!gs_days_to_cens) {
			gs_monthly_credit = 0;
		}
	}

	/* Days until you run in trouble, if you have more
		than 1000S debt at the bank */
	if (gs_debt_days > 0) {

		gs_debt_days--;

		if (!gs_debt_days) {
			gs_debt_days = -1;
		}
	}
}

/* static */
void seg002_2177(void)
{
	signed short i;

	for (i = 0; ds_readws(MARKET_DESCR_TABLE + i * 8) != -1; i++) {

		ds_writew((MARKET_DESCR_TABLE + 6) + i * 8,
			random_interval(ds_readws(MARKET_DESCR_TABLE + i * 8), 20));
	}
}

void pal_fade(Bit8u *dst, Bit8u *p2)
{
	signed short i;

	for (i = 0; i < 32; i++) {

		if ((host_readbs(p2 + 3 * i) < host_readbs(dst + 3 * i)) &&
			(host_readbs(dst + 3 * i) > 0))
		{

			dec_ptr_bs(dst + i * 3);

		} else {
			if ((host_readbs(p2 + 3 * i) > host_readbs(dst + 3 * i)) &&
				(host_readbs(dst + 3 * i) < 0x3f))
			{
				inc_ptr_bs(dst + i * 3);
			}
		}

		if ((host_readbs((p2 + 1) + 3 * i) < host_readbs((dst + 1) + 3 * i)) &&
			(host_readbs((dst + 1) + 3 * i) > 0))
		{

			dec_ptr_bs((dst + 1) + i * 3);

		} else {
			if ((host_readbs((p2 + 1) + 3 * i) > host_readbs((dst + 1) + 3 * i)) &&
				(host_readbs((dst + 1) + 3 * i) < 0x3f))
			{
				inc_ptr_bs((dst + 1) + i * 3);
			}
		}

		if ((host_readbs((p2 + 2) + 3 * i) < host_readbs((dst + 2) + 3 * i)) &&
			(host_readbs((dst + 2) + 3 * i) > 0))
		{

			dec_ptr_bs((dst + 2) + i * 3);

		} else {
			if ((host_readbs((p2 + 2) + 3 * i) > host_readbs((dst + 2) + 3 * i)) &&
				(host_readbs((dst + 2) + 3 * i) < 0x3f))
			{
				inc_ptr_bs((dst + 2) + i * 3);
			}
		}
	}
}

void pal_fade_in(Bit8u *dst, Bit8u *p2, signed short v3, signed short colors)
{
	signed short i, si;

	si = 0x40 - v3;

	for (i = 0; i < colors; i++) {

		if ((host_readbs((p2 + 0) + 3 * i) >= si) &&
			(host_readbs((p2 + 0) + 3 * i) > host_readbs((dst + 0) + 3 * i)))
		{
			inc_ptr_bs((dst + 0) + i * 3);
		}

		if ((host_readbs((p2 + 1) + 3 * i) >= si) &&
			(host_readbs((p2 + 1) + 3 * i) > host_readbs((dst + 1) + 3 * i)))
		{
			inc_ptr_bs((dst + 1) + i * 3);
		}

		if ((host_readbs((p2 + 2) + 3 * i) >= si) &&
			(host_readbs((p2 + 2) + 3 * i) > host_readbs((dst + 2) + 3 * i)))
		{
			inc_ptr_bs((dst + 2) + i * 3);
		}
	}
}

/**
 * \brief   adjusts palettes in the morning
 */
void dawning(void)
{
	/* Between 6 and 7, in 64 steps (i.e. each 56 seconds) */
	if ((gs_day_timer >= HOURS(6)) &&
		(gs_day_timer <= HOURS(7)) &&
		!((gs_day_timer - HOURS(6)) % SECONDS(56)))
	{

		/* floor */
		pal_fade(gs_palette_floor, g_townpal_buf);
		/* buildings */
		pal_fade(gs_palette_buildings, g_townpal_buf + 0x60);
		/* sky */
		pal_fade(gs_palette_sky, g_townpal_buf + 0xc0);

		/* in a town */
		if (gs_current_town &&
			/* not in a dungeon */
			!gs_dungeon_index &&
			/* not in a location */
			!gs_current_loctype &&
			/* not in a travel mode */
			!ds_readb(SHOW_TRAVEL_MAP) &&
			/* no event animation */
			!g_event_ani_busy &&
			/* unknown */
			!g_special_screen &&
			/* unknown */
			(ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK))
		{
			wait_for_vsync();

			set_palette(gs_palette_floor, 0, 0x20);
			set_palette(gs_palette_buildings, 0x80, 0x40);
		}
	}
}

/**
 * \brief   adjusts palettes in the evening
 */
void nightfall(void)
{
	/* Between 20 and 21, in 64 steps (i.e. each 56 seconds) */
	if ((gs_day_timer >= HOURS(20)) &&
		(gs_day_timer <= HOURS(21)) &&
		!((gs_day_timer - HOURS(20)) % SECONDS(56)))
	{

		/* floor */
		pal_fade(gs_palette_floor, (Bit8u*)g_floor_fade_palette);
		/* buildings */
		pal_fade(gs_palette_buildings, (Bit8u*)g_building_fade_palette);
		/* sky */
		pal_fade(gs_palette_sky, (Bit8u*)g_sky_fade_palette);

		/* in a town */
		if (gs_current_town &&
			/* not in a dungeon */
			!gs_dungeon_index &&
			/* not in a location */
			!gs_current_loctype &&
			/* not in a travel mode */
			!ds_readb(SHOW_TRAVEL_MAP) &&
			/* no event animation */
			!g_event_ani_busy &&
			/* unknown */
			!g_special_screen &&
			/* unknown */
			(ds_readbs(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK))
		{
			wait_for_vsync();

			set_palette(gs_palette_floor, 0, 0x20);
			set_palette(gs_palette_buildings, 0x80, 0x40);
		}
	}
}

/**
 * \brief   get season
 *
 * \return              number of the season {0 = WINTER, 1,2,3}
 */
signed short get_current_season(void)
{
	if (is_in_byte_array(gs_month, p_datseg + MONTHS_WINTER)) {
		return SEASON_WINTER;
	} else if (is_in_byte_array(gs_month, p_datseg + MONTHS_SUMMER)) {
		return SEASON_SUMMER;
	} else if (is_in_byte_array(gs_month, p_datseg + MONTHS_SPRING)) {
		return SEASON_SPRING;
	} else {
		return SEASON_AUTUMN;
	}
}


/**
 * \brief   calc census for the bank depot
 *
 *          If you put money on the bank, you get 5%.
 *          If you borrowed money you pay 15%.
 */
/* static */
void do_census(void)
{

	signed short si = 0;
	Bit32s val;

	if (gs_bank_deposit > 0) {
		si = 1;
	} else if (gs_bank_deposit < 0) {
			si = -1;
	}

	/* bank transactions, no census */
	if (si == 0)
		return;

	/* convert to heller */
	val = gs_bank_deposit * 10L;
	val += ds_readws(BANK_HELLER);

	if (val < 0) {
		/* 15% Interest for borrowed money */
		val += val * 15 / 100L / 12L;
	} else if (val > 0) {
		/* 5% Interest for deposited money */
		val += val * 5 / 100L / 12L;
	}

	/* remember the heller */
	ds_writew(BANK_HELLER, val % 10);

	if (val < 0) {
		ds_writew(BANK_HELLER, -__abs__(ds_readws(BANK_HELLER)));
	}

	/* save the new deposit */
	gs_bank_deposit = val / 10;

	/* fixup over- and underflows */
	if ((gs_bank_deposit < 0) && (si == 1)) {
		gs_bank_deposit = 32760;
	} else if ((gs_bank_deposit > 0) && (si == -1)) {
		gs_bank_deposit = -32760;
	}

}

/* called from timewarp(..), timewarp_until_time_of_day(..),
 * timewarp_until_midnight(..) and interrupt timer_isr() */
void do_timers(void)
{
	Bit8u *hero_i;
	signed char afternoon;
	Bit8u *ptr;
	signed short i, di;

	afternoon = 0;

	/* TIMERS_DISABLED is set during a fight, a level-up or if
	 * the game is paused (Ctrl + P) */
	if (g_timers_disabled)
		return;

	dawning();

	nightfall();

	/* inc day timer */
	gs_day_timer += 1L;

	if (!ds_readbs(FREEZE_TIMERS)) {
		/* FREEZE_TIMERS is set in timewarp(..) and timewarp_until_time_of_day(..) for efficiency reasons,
		 *  where certain timers are updated separately in a single step (instead of many 1-tick update calls). */
		sub_ingame_timers(1);
		sub_mod_timers(1);
	}

	if (!ds_readbs(FREEZE_TIMERS)) {

		/* set day timer to pm */
		/* TODO: afternoon is useless */
		if (gs_day_timer >= HOURS(12)) {
			gs_day_timer -= HOURS(12);
			afternoon = 1;
		}

		/* every 5 minutes ingame */
		if (!(gs_day_timer % MINUTES(5))) {
			sub_heal_staffspell_timers(1);
		}

		/* every 15 minutes ingame */
		if (!(gs_day_timer % MINUTES(15))) {
			sub_light_timers(1L);
		}
		/* every hour ingame */
		if (!(gs_day_timer % HOURS(1))) {

			magical_chainmail_damage();

			/* What about herokeeping()? Original-Bug? */

			/* decrement unicorn timer */
			if (ds_readb(UNICORN_GET_MAP) != 0 && ds_readb(UNICORN_TIMER) != 0)
			{
				dec_ds_bs_post(UNICORN_TIMER);
			}

			/* handle sphere timer */
			if (ds_readb(DNG02_SPHERE_TIMER) != 0) {

				if (!add_ds_bu(DNG02_SPHERE_TIMER, -1)) {
					ds_writeb(DNG02_SPHERE_ACTIVE, 1);
				}
			}

			/* barrels with orc muck in the orc dungeon */
			if (ds_readbs(DNG08_TIMER1) != 0) {
				dec_ds_bs_post(DNG08_TIMER1);
			}
			if (ds_readbs(DNG08_TIMER2) != 0) {
				dec_ds_bs_post(DNG08_TIMER2);
			}
		}

		/* reset the day timer to 24h time */
		if (afternoon) {
			gs_day_timer += HOURS(12);
		}
	}

	/* at 6 o'clock in the morninig */
	if (gs_day_timer == HOURS(6)) {

		hero_i = get_hero(0);

		for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

			if ((host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readb(hero_i + HERO_JAIL) != 0))
			{
				host_writeb(hero_i + HERO_JAIL, 0);

				gs_groups_current_loctype[host_readbs(hero_i + HERO_GROUP_NO)] =
					gs_groups_current_loctype_bak[host_readbs(hero_i + HERO_GROUP_NO)];

				gs_groups_x_target[host_readbs(hero_i + HERO_GROUP_NO)] =
					gs_groups_x_target_bak[host_readbs(hero_i + HERO_GROUP_NO)];

				gs_groups_y_target[host_readbs(hero_i + HERO_GROUP_NO)] =
					gs_groups_y_target_bak[host_readbs(hero_i + HERO_GROUP_NO)];
			}
		}
	}

	/* at 10 o'clock */
	if (gs_day_timer == HOURS(10)) {

		hero_i = get_hero(0);

		for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
			if ((host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readb(hero_i + HERO_DRUNK) != 0))
			{
				hero_get_sober(hero_i);
			}
		}
	}

	/* poison timer in the mage dungeon */
	if (gs_dng07_poison_timer) {

		/* decrement the timer */
		gs_dng07_poison_timer -= 1L;

		/* every 15 minutes do damage */
		if (gs_dng07_poison_timer % MINUTES(15) == 0) {

			ptr = get_hero(0);

			for (i = 0; i <= 6; i++, ptr += SIZEOF_HERO) {

				if (host_readb(ptr + HERO_TYPE) != HERO_TYPE_NONE) {
					di = host_readbs(ptr + HERO_GROUP_NO);

					/* hero is in group and in mage dungeon */
					if ((gs_current_group == di) && (gs_dungeon_index == DUNGEONS_RUINE_DES_SCHWARZMAGIERS))
					{

						if (gs_dungeon_level == 1) {

							/* 1W6-1 */
							sub_hero_le(ptr, dice_roll(1, 6, -1));

						} else if (gs_dungeon_level == 2) {
							/* 1W6+1 */
							sub_hero_le(ptr, dice_roll(1, 6, 1));
						}

					} else {
						if (gs_groups_dng_index[di] == 7) {

							if (gs_groups_dng_level[di] == 1) {
								/* 1W6-1 */
								sub_hero_le(ptr, dice_roll(1, 6, -1));

							} else if (gs_groups_dng_level[di] == 2) {
								/* 1W6+1 */
								sub_hero_le(ptr, dice_roll(1, 6, 1));
							}
						}
					}
				}
			}
		}
	}

	if (gs_dng12_watertrap_timer) {
		gs_dng12_watertrap_timer -= 1L;
	}

	/* at 24 o'clock, daily stuff */
	if (gs_day_timer >= HOURS(24)) {

		timers_daily();

		seg002_2177();

		/* reset day timer */
		gs_day_timer = 0L;

		/* inc DAY date */
		gs_day_of_week++;
		gs_day_of_month++;

		if (gs_day_of_week == 7) {
			gs_day_of_week = 0;
		}

		/* decrement NPC timers */
		for (i = 1; i < 7; i++) {

			if ((ds_readbs(NPC_TIMERS + i) != 0) && (ds_readbs(NPC_TIMERS + i) != -1))
			{
				dec_ds_bs_post(NPC_TIMERS + i);
			}
		}

		/* drug timer (phexcaer) */
		if (gs_drug_timer) {
			gs_drug_timer--;
		}

		/* unknown timer */
		if (ds_readb(FORCEDMARCH_TIMER) != 0) {
			dec_ds_bs_post(FORCEDMARCH_TIMER);
		}

		/* calendar */
		if (gs_day_of_month == 31) {

			/* new month */
			gs_day_of_month = 1;
			gs_month++;

			/* increment quested months counter */
			if (gs_got_main_quest) {
				gs_quested_months++;
			}

			/* increment the months the NPC is in the group */
			if (host_readb(get_hero(6) + HERO_TYPE) != HERO_TYPE_NONE) {
				gs_npc_months++;
			}

			do_census();

			/* set days of the nameless (negative) */
			if (gs_month == 13) {
				gs_day_of_month = -5;
			}
		} else {
			/* new year */
			if (gs_day_of_month == 0) {
				gs_month = 1;
				gs_year++;
				gs_day_of_month = 1;
			}
		}

		/* check if we have a special day */
		gs_special_day = 0;

		for (i = 0; g_special_days[i].month != -1; i++) {

			if ((g_special_days[i].month == gs_month) && (g_special_days[i].day == gs_day_of_month))
			{
				gs_special_day = g_special_days[i].id;
				break;
			}
		}

		passages_recalc();

		/* roll out the weather, used for passages */
		gs_weather1 = random_schick(6);
		gs_weather2 = random_schick(7);
#ifndef __BORLANDC__
		i = (gs_weather2 + 6) * (gs_weather1 * 15 + 100); /* between 805 and 2470 */
		D1_INFO_VERBOSE("WEATHER1=%d, WEATHER2=%d -> ",gs_weather1, gs_weather2);
		D1_INFO("Heutige Wetter-Anpassung der Schiffs-Geschwindigkeiten: %d,%d%%.\n",i/10,i%10);
#endif

		/* check if times up */
		if ((gs_year == 17) && (gs_month >= 10) && (gs_day_of_month >= 17)) {
			ds_writew(GAME_STATE, GAME_STATE_TIMEUP);
		}
	}

	/* at 9 o'clock */
	if (gs_day_timer == HOURS(9)) {
		/* ships leave the harbor at 9 o'clock */
		passages_reset();
	}
}

/**
 * \brief   subtracts val from the ingame timers
 *
 * \param   val         vaule to subtract from the ingame timers
 * \note improvable
 */
void sub_ingame_timers(Bit32s val)
{
	signed short i = 0;

	if (g_timers_disabled) return;

	for (i = 0; i < 26; i++) {

		/* only subtract if greater than zero */
		if (gs_ingame_timers[i] > 0) {

			/* subtract val from timer*/
			gs_ingame_timers[i] -= val;

			/* if the timer is now lower than zero, set the timer to zero */
			if (gs_ingame_timers[i] < 0) {
				gs_ingame_timers[i] = 0;
			}
		}
	}
}

/**
 * \brief   subtracts val from the modification timers
 *
 * \param   val         vaule to subtract from the modification timers
 */
void sub_mod_timers(Bit32s val)
{
	signed short i;
	signed short j;
	signed short h_index;
#if !defined(__BORLANDC__)
	Bit8u *mp;
#else
	Bit8u huge *mp;
#endif
	signed char target;
	unsigned char reset_target;
	Bit8u *sp;

	h_index = -1;

	if (g_timers_disabled)
		return;

	for (i = 0; i < 100; i++) {

		/* if timer is 0 continue */
		if (ds_readd(MODIFICATION_TIMERS + 8 * i) == 0)
			continue;

		/* subtract diff from timer */
		sub_ds_ds(MODIFICATION_TIMERS + 8 * i, val);


		/* if timer > 0 continue */
		if (ds_readds(MODIFICATION_TIMERS + 8 * i) <= 0) {


#if !defined(__BORLANDC__)
			D1_LOG("Mod Timer %d rueckgesetzt\n", i);
#endif

			/* set timer to 0 */
			ds_writed(MODIFICATION_TIMERS + 8 * i, 0);

			/* make a pointer to the slot */
			sp = p_datseg + MODIFICATION_TIMERS + i * 8;

			if (host_readb(sp + 6) != 0) {
				/* target is a hero/npc */

				/* get the hero index from the target */
				target = host_readb(sp + 6);
				for (j = 0; j <= 6; j++) {
					if (host_readbs(get_hero(j) + HERO_TIMER_ID) == target) {
						h_index = j;
						break;
					}
				}

				if (h_index != -1) {
					/* if a hero/npc is determined */

					mp = get_hero(h_index);
					/* make a pointer to the hero's attribute mod */
					mp += (Bit32u)host_readw(sp + 4);
					/* subtract the mod */
					sub_ptr_bs(mp, host_readbs(sp + 7));

					if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_ZUSTA_UK) {
						ds_writew(REQUEST_REFRESH, 1);
					}

					/* reset target */
					host_writeb(sp + 6, 0);

					/* reset target if no other slots of target */
					reset_target = 1;
					for (j = 0; j < 100; j++) {
						if (ds_readbs((MODIFICATION_TIMERS+6) + j * 8) == target) {
							reset_target = 0;
							break;
						}
					}

					if (reset_target) {
						host_writeb(get_hero(h_index) + HERO_TIMER_ID, 0);
					}
				} else {
#if !defined(__BORLANDC__)
					D1_ERR("Invalid Mod Timer Target %d\n", target);
#endif

					/* reset all slots of invalid target */
					for (j = 0; j < 100; j++) {
						if (ds_readbs((MODIFICATION_TIMERS+6) + j * 8) == target) {
							host_writeb(sp + 6, host_writebs(sp + 7, 0));
							host_writew(sp + 4, 0);
						}
					}
				}

			} else {
				/* target affects the savegame */
				mp = (Bit8u*)&gs_datseg_status_start;
				mp += host_readw(sp + 4);
				sub_ptr_bs(mp, host_readbs(sp + 7));
			}

			/* reset offset, target, and modificator */
			host_writeb(sp + 6, host_writebs(sp + 7, 0));
			host_writew(sp + 4, 0);
		}
	}
}

/**
 * \brief   get a free modification slot
 *
 * \return              number of the modification slot
 */
signed short get_free_mod_slot(void)
{
	signed short i;

	for (i = 0; i < 100; i++) {

		if (ds_readw(MODIFICATION_TIMERS + i * 8 + 4) == 0) {
			break;
		}
	}

	if (i == 100) {
		/* all 100 mod timers are in use. apply hack to free timer in slot 0 */

		/* set timer of slot 0 to 1 */
		host_writed(p_datseg + MODIFICATION_TIMERS, 1);
		/* subtract one from each mod timer -> timer in slot 0 will be freed. */
		sub_mod_timers(1);

		/* now timer slot 0 can be used again. */
		return 0;
	}

	return i;
}

void set_mod_slot(signed short slot_no, Bit32s timer_value, Bit8u *ptr, signed char mod, signed char who)
{
	signed short j;

#if !defined (__BORLANDC__)
	Bit8u *mod_ptr;
#else
	Bit8u huge *mod_ptr;
#endif
	signed char target;
	signed short i;
	signed short new_target;


	if (who == -1) {
		/* mod slot is on savegame */
		mod_ptr = (Bit8u*)&gs_datseg_status_start;
	} else {
		/* mod slot is on a hero/npc */
		mod_ptr = get_hero(who);

		if (host_readb(get_hero(who) + HERO_TIMER_ID) != 0) {
			/* hero/npc has a target number */
			target = host_readbs(get_hero(who) + HERO_TIMER_ID);
		} else {
			/* hero/npc has no target number */

			for (i = 1; i < 8; i++) {

				new_target = 1;
				for (j = 0; j <= 6; j++) {
					if (host_readbs(get_hero(j) + HERO_TIMER_ID) == i) {
						new_target = 0;
						break;
					}
				}

				if (new_target != 0) {
					target = (signed char)i;
					break;
				}
			}

			host_writeb(get_hero(who) + HERO_TIMER_ID, target);
		}

		ds_writeb(MODIFICATION_TIMERS + slot_no * 8 + 6, target);
	}

	ds_writeb(MODIFICATION_TIMERS + slot_no * 8 + 7, mod);
#if !defined (__BORLANDC__)
	ds_writew(MODIFICATION_TIMERS + slot_no * 8 + 4, ptr - mod_ptr);
#else
	ds_writew(MODIFICATION_TIMERS + slot_no * 8 + 4, (Bit8u huge*)ptr - mod_ptr);
#endif
	ds_writed(MODIFICATION_TIMERS + slot_no * 8, timer_value);
	add_ptr_bs(ptr, mod);
}

/**
 * \param   fmin        five minutes
 *
 *	This function decrements the timers for the healing and staffspell timeouts.
 *	Furthermore, the g_check_poison flag is set.
 */
void sub_heal_staffspell_timers(Bit32s fmin)
{
	signed short i;
	Bit8u *hero_i;

	if (g_timers_disabled)
		return;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		if (host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) {

			/* Timer to the next healing attempt */
			if (host_readds(hero_i + HERO_HEAL_TIMER) > 0) {

				sub_ptr_ds(hero_i + HERO_HEAL_TIMER, fmin * MINUTES(5));
#if !defined(__BORLANDC__)
				if (host_readds(hero_i + HERO_HEAL_TIMER) <= 0) {
					D1_INFO("%s kann wieder geheilt werden\n",
						(char*)hero_i + HERO_NAME2);
				}
#endif

				if (host_readds(hero_i + HERO_HEAL_TIMER) < 0) {
					host_writed(hero_i + HERO_HEAL_TIMER, 0);
				}
			}

			/* Timer set after Staffspell */
			if (host_readds(hero_i + HERO_STAFFSPELL_TIMER) > 0) {
				sub_ptr_ds(hero_i + HERO_STAFFSPELL_TIMER, fmin * MINUTES(5));
#if !defined(__BORLANDC__)
				if (host_readds(hero_i + HERO_STAFFSPELL_TIMER) <= 0) {
					D1_INFO("%s kann wieder einen Stabzauber versuchen\n",
						(char*)(hero_i + HERO_NAME2));
				}

#endif
				if (host_readds(hero_i + HERO_STAFFSPELL_TIMER) < 0) {

					host_writed(hero_i + HERO_STAFFSPELL_TIMER, 0);
				}
			}

			/* Original-Bug:
			 * Apparently, the following line is supposed to trigger poison_effect() every 5 minutes.
			 * This won't work if the game does not jump back to game_loop() where poison_effect() is called.
			 * Also, fmin > 1 triggers only one call of poison_effect().
			 * Solution could be to move the poison_effect() call from game_loop() to do_timers() and timewarp(..). */
			g_check_poison = 1;
		}
	}
}

/**
 * \brief   decrements the light timers
 *
 * \param   quarter     the time in quarters of an hour
 *
 *	This function decrements the timers of burning torches and lanterns.
 *	If the time of the lightsource is up the torch is removed from the
 *	inventory and the lantern is turned off.
 */
void sub_light_timers(Bit32s quarter)
{
	signed short j;
	signed short i;

	Bit8u *hero_i;
	signed char tmp;

	if (g_timers_disabled)
		return;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		if (host_readb(hero_i + HERO_TYPE) != HERO_TYPE_NONE) {

			if (quarter > 120) {
				tmp = 120;
			} else {
				tmp = (signed char)quarter;
			}

			for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {

				if (host_readw(hero_i + (HERO_INVENTORY + INVENTORY_ITEM_ID) + SIZEOF_INVENTORY * j) == ITEM_TORCH_ON) {

					/* Torch, burning */

					sub_ptr_bs(hero_i + HERO_INVENTORY + INVENTORY_LIGHTING_TIMER + SIZEOF_INVENTORY * j, tmp);

					if (host_readbs(hero_i + HERO_INVENTORY + INVENTORY_LIGHTING_TIMER + SIZEOF_INVENTORY * j) <= 0)
					{
						/* decrement item counter */
						dec_ptr_bs(hero_i + HERO_NR_INVENTORY_SLOTS_FILLED);

						/* subtract weight of a torch */
						sub_ptr_ws(hero_i + HERO_LOAD,
							host_readws(get_itemsdat(ITEM_TORCH_ON) + ITEM_STATS_WEIGHT));

						/* Remove Torch from inventory */
						memset(hero_i + HERO_INVENTORY + SIZEOF_INVENTORY * j, 0, SIZEOF_INVENTORY);
					}

				} else if (host_readw(hero_i + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * j) == ITEM_LANTERN_ON) {

					/* Lantern, burning */
					sub_ptr_bs(hero_i + HERO_INVENTORY + INVENTORY_LIGHTING_TIMER + SIZEOF_INVENTORY * j, tmp);

					if (host_readbs(hero_i + HERO_INVENTORY + INVENTORY_LIGHTING_TIMER + SIZEOF_INVENTORY * j) <= 0) {
						/* Set timer to 0 */
						host_writeb(hero_i + HERO_INVENTORY + INVENTORY_LIGHTING_TIMER + SIZEOF_INVENTORY * j, 0);
						/* Set burning lantern to a not burning lantern */
						host_writew(hero_i + HERO_INVENTORY + INVENTORY_ITEM_ID + SIZEOF_INVENTORY * j, ITEM_LANTERN_OFF);
					}
				}
			}
		}
	}
}

/**
 * \brief   damage if a cursed chainmail is worn
 */
void magical_chainmail_damage(void)
{
	signed short i;
	Bit8u *hero_i;

	if (g_timers_disabled) {
		return;
	}

	ds_writeb(HEROKEEPING_FLAG, (ds_readb(SHOW_TRAVEL_MAP) != 0) ? 1 : 2);

	for (i = 0; i <= 6; i++) {

		if (host_readb(get_hero(i) + HERO_TYPE) != HERO_TYPE_NONE) {

			hero_i = get_hero(i);

			if (!hero_dead(hero_i) &&
				/* check if not in jail (the argument might be: heroes are forced to take off armor in jail) */
				!host_readbs(hero_i + HERO_JAIL) &&
				/* check if cursed chainmail is equipped */
				(host_readw(hero_i + HERO_INVENTORY + INVENTORY_ITEM_ID + HERO_INVENTORY_SLOT_BODY * SIZEOF_INVENTORY) == ITEM_CHAIN_MAIL_CURSED))
			{
				sub_hero_le(hero_i, 1);
			}
		}
	}
}

/**
 * \brief   consume food if needed and print warnings to the user
 */
void herokeeping(void)
{
	signed short i;
	signed short pos;
	Bit8u *hero;
	char buffer[100];

	if (ds_readw(GAME_STATE) != GAME_STATE_MAIN)
		return;

	/* The actual food consumption is done only if HEROKEEPING_FLAG is set.
	 * This happens hourly in magical_chainmail_damage()
	 * The flag is reset at the end of this function. */

	/* for each hero ..*/
	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		/* consume food and set messages */
		if (host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE &&
			ds_readb(HEROKEEPING_FLAG) != 0 &&
			check_hero_no3(hero) &&			/* must be vital */
			!host_readbs(hero + HERO_JAIL) &&
			!g_travel_herokeeping)
		{
			/* Do the eating */

			/* check for magic bread bag in the group */
			if (get_first_hero_with_item_in_group(ITEM_MAGIC_BREADBAG, host_readbs(hero + HERO_GROUP_NO)) == -1) {
				/* if not, check if the hero has the food amulet */
				if (get_item_pos(hero, ITEM_TRAVIA_AMULET) == -1) {
					/* if not... */

					/* eat if hunger > 90 % */
					if (host_readbs(hero + HERO_HUNGER) > 90) {

						/* search for Lunchpack */
						pos = get_item_pos(hero, ITEM_FOOD_PACKAGE);

						if (pos != -1) {
							/* Lunchpack found, consume quiet */
							ds_writeb(CONSUME_QUIET, 1);
							consume(hero, hero, pos);
#if !defined(__BORLANDC__)
							D1_INFO("%s isst etwas\n", (char*)hero + HERO_NAME2);
#endif
							ds_writeb(CONSUME_QUIET, 0);

							/* search for another Lunchpack */
							/* print last ration message */
							if (get_item_pos(hero, ITEM_FOOD_PACKAGE) == -1) {
								gs_food_message[i] = 6;
							}
						} else {
							/* print ration warning */
							if (host_readbs(hero + HERO_HUNGER) < 100) {
								gs_food_message[i] = 4;
							}
						}

					}

					if (host_readbs(hero + HERO_HUNGER) < 100) {
						/* increase hunger value. FOOD_MOD is always 0 or 1 */
						if (host_readbs(hero + HERO_HUNGER_TIMER) <= 0) {
							/* increase more (FOOD_MOD == 0 -> increase by 2. FOOD_MOD == 1 -> increase by 0.) */
							add_ptr_bs(hero + HERO_HUNGER, 2 / (g_food_mod * 2 + 1));
						} else {
							/* increase less (FOOD_MOD == 0 -> increase by 1. FOOD_MOD == 1 -> increase by 0.) */
							add_ptr_bs(hero + HERO_HUNGER, 1 / (g_food_mod * 2 + 1));
						}

						/* adjust hunger */
						if (host_readbs(hero + HERO_HUNGER) > 100) {
							host_writeb(hero + HERO_HUNGER, 100);
						}
					} else {

						/* */
						if (host_readbs(hero + HERO_HUNGER_TIMER) <= 0) {
							do_starve_damage(hero, i, 0);
						}
					}
				}
			} else {

				/* set hunger to 20 % */
				if (host_readbs(hero + HERO_HUNGER) > 20) {
					host_writeb(hero + HERO_HUNGER, 20);
				}
			}

			/* Do the drinking */

			/* check if someone in the group of the hero has the magic bread bag */
			/* check for magic waterskin in group */
			if ((get_first_hero_with_item_in_group(ITEM_MAGIC_WATERSKIN, host_readbs(hero + HERO_GROUP_NO)) == -1) &&
				((host_readbs(hero + HERO_GROUP_NO) == gs_current_group &&
				(!gs_current_town || (gs_current_town != TOWNS_NONE && ds_readb(SHOW_TRAVEL_MAP) != 0))) ||
				(host_readbs(hero + HERO_GROUP_NO) != gs_current_group &&
				!gs_groups_town[host_readbs(hero + HERO_GROUP_NO)]))) {

					/* check for food amulett */
					if (get_item_pos(hero, ITEM_TRAVIA_AMULET) == -1) {

						/* hero should drink something */
						if (host_readbs(hero + HERO_THIRST) > 90) {

							ds_writeb(CONSUME_QUIET, 1);

							/* first check for beer :) */
							pos = get_item_pos(hero, ITEM_BEER);

							/* and then for water */
							if (pos == -1) {
								pos = get_full_waterskin_pos(hero);
							}

							if (pos != -1) {
								/* drink it */
								consume(hero, hero, pos);
#if !defined(__BORLANDC__)
								D1_INFO("%s trinkt etwas\n", (char*)hero + HERO_NAME2);
#endif
								/* nothing to drink message */
								if ((get_item_pos(hero, ITEM_BEER) == -1)
									&& (get_full_waterskin_pos(hero) == -1)) {
									gs_food_message[i] = 5;
								}

							} else {
								/* hero has nothing to drink */
								if (host_readbs(hero + HERO_THIRST) < 100) {
									gs_food_message[i] = 3;
								}
							}

							ds_writeb(CONSUME_QUIET, 0);
						}

						if (host_readbs(hero + HERO_THIRST) < 100) {
							/* increase thirst counter food_mod is always 0 or 1 */
							if (host_readbs(hero + HERO_HUNGER_TIMER) <= 0) {

								/* increase more (FOOD_MOD == 0 -> increase by 4. FOOD_MOD == 1 -> increase by 1.) */
								add_ptr_bs(hero + HERO_THIRST, 4 / (g_food_mod * 2 + 1));
							} else {

								/* increase less (FOOD_MOD == 0 -> increase by 2. FOOD_MOD == 1 -> increase by 0.) */
								add_ptr_bs(hero + HERO_THIRST, 2 / (g_food_mod * 2 + 1));
							}

							/* adjust thirst */
							if (host_readbs(hero + HERO_THIRST) > 100) {
								host_writeb(hero + HERO_THIRST, 100);
							}

						} else {
							if (host_readbs(hero + HERO_HUNGER_TIMER) <= 0) {
								do_starve_damage(hero, i, 1);
							}
						}

					}
			} else {

				/* set thirst to 20 % */
				if (host_readbs(hero + HERO_THIRST) > 20) {
					host_writeb(hero + HERO_THIRST, 20);
				}
			}
		}

		/* print hero message */
		if (gs_food_message[i] && !ds_readbs(DIALOGBOX_LOCK) &&	!g_in_fight && !ds_readbs(FREEZE_TIMERS))
		{

			if ((host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
				!hero_dead(hero) &&
				(!ds_readb(SHOW_TRAVEL_MAP) || (g_food_message_shown[i] != gs_food_message[i]))) {

					sprintf(buffer,	 (gs_food_message[i] == 1) ? get_ttx(224):
							((gs_food_message[i] == 2) ? get_ttx(223) :
							((gs_food_message[i] == 3) ? get_ttx(797) :
							((gs_food_message[i] == 4) ? get_ttx(798) :
							((gs_food_message[i] == 5) ? get_ttx(799) :
							get_ttx(800))))),
							(char*)hero + HERO_NAME2, (char*)GUI_get_ptr(host_readbs(hero + HERO_SEX), 1));

					g_food_message_shown[i] = gs_food_message[i];

					GUI_output(buffer);

					if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_ZUSTA_UK) {
						ds_writew(REQUEST_REFRESH, 1);
					}
			}

			gs_food_message[i] = 0;
		}


		/* print unconscious message */
		if (gs_unconscious_message[i] && !ds_readbs(DIALOGBOX_LOCK)) {

			if (host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE &&
				(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
				!hero_dead(hero)) {

					/* prepare output */
					sprintf(buffer, get_ttx(789), (char*)hero + HERO_NAME2);

					/* print output */
					GUI_output(buffer);

					if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_ZUSTA_UK) {
						ds_writew(REQUEST_REFRESH, 1);
					}
			}

			/* reset condition */
			gs_unconscious_message[i] = 0;
		}
	}

	ds_writeb(HEROKEEPING_FLAG, 0);
}

void check_level_up(void)
{
	signed short i;
	signed short not_done;
	Bit8u *hero;

	if (g_timers_disabled) {
		return;
	}

	do {
		not_done = 0;
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if (
				(host_readbs(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
				!hero_dead(hero) &&
				(host_readbs(hero + HERO_LEVEL) < 20) &&

#ifndef M302de_FEATURE_MOD
				/* Feature mod 8:
				 * Adjust AP requirement for level up to match the original DSA 2/3 rules.
				 * Without this mod, it is "off by one".
				 * For example, for level 2, 100 AP should be enough, but the game requires 101 AP. */
				(g_level_ap_tab[host_readbs(hero + HERO_LEVEL)] < host_readds(hero + HERO_AP))
				/* could be easily done without accessing the data segment by the formula level_ap_tab[i] = 50 * i * (i+1) */
#else
				(50 * host_readbs(hero + HERO_LEVEL) * (host_readbs(hero + HERO_LEVEL) + 1) <= host_readds(hero + HERO_AP))
				/* while we're at it, avoid accessing the data segment... */
#endif
			) {
				level_up(i);
				not_done = 1;
			}
		}

	} while (not_done);
}

struct dummy {
	char a[24];
};

void seg002_37c4(void)
{
	signed short l_si = 0;
	Bit8u* p1;
	Bit8u* p2;
	Bit8u* p3;
	struct dummy a = *(struct dummy*)(p_datseg + PIC_COPY_DST);

	p1 = g_buffer6_ptr + 2000;
	p2 = g_buffer6_ptr + 2100;
	p3 = g_buffer6_ptr + 1000;

	if ((ds_readws(TRV_MENU_SELECTION) != 0) && (ds_readb(SHOW_TRAVEL_MAP))) {

		ds_writew(SELECTED_TOWN_ANIX,
				ds_readws((TOWN_POSITIONS-4) + 4 * ds_readbs((TRV_MENU_TOWNS - 1) + ds_readws(TRV_MENU_SELECTION))));
		ds_writew(SELECTED_TOWN_ANIY,
				ds_readws((TOWN_POSITIONS-4) + 2 + 4 * ds_readbs((TRV_MENU_TOWNS - 1) + ds_readws(TRV_MENU_SELECTION))));

		ds_writew(PIC_COPY_X1, ds_readws(SELECTED_TOWN_ANIX) - 4);
		ds_writew(PIC_COPY_Y1, ds_readws(SELECTED_TOWN_ANIY) - 4);
		ds_writew(PIC_COPY_X2, ds_readws(SELECTED_TOWN_ANIX) + 4);
		ds_writew(PIC_COPY_Y2, ds_readws(SELECTED_TOWN_ANIY) + 4);
		ds_writed(PIC_COPY_SRC, (Bit32u)p1);

		if (is_mouse_in_rect(ds_readws(PIC_COPY_X1) - 16, ds_readws(PIC_COPY_Y1) - 16, ds_readws(PIC_COPY_X2) + 16, ds_readws(PIC_COPY_Y2) + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_pic_copy(0);

		if (l_si) {
			refresh_screen_size();
		}

		ds_writew(TRV_MENU_SELECTION, l_si = 0);
	}

	if (ds_readws(CURRENT_TOWN_OVER) != 0) {

		ds_writew(PIC_COPY_X1, ds_readws(CURRENT_TOWN_OVERX) - 4);
		ds_writew(PIC_COPY_Y1, ds_readws(CURRENT_TOWN_OVERY) - 4);
		ds_writew(PIC_COPY_X2, ds_readws(CURRENT_TOWN_OVERX) + 4);
		ds_writew(PIC_COPY_Y2, ds_readws(CURRENT_TOWN_OVERY) + 4);
		ds_writed(PIC_COPY_SRC, (Bit32u)p2);

		if (is_mouse_in_rect(ds_readws(PIC_COPY_X1) - 16, ds_readws(PIC_COPY_Y1) - 16, ds_readws(PIC_COPY_X2) + 16, ds_readws(PIC_COPY_Y2) + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		if (ds_readws(CURRENT_TOWN_OVER) != 0) {
			do_pic_copy(0);
		}

		if (l_si) {
			refresh_screen_size();
		}

		l_si = ds_writew(CURRENT_TOWN_OVER, 0);
	}

	if (ds_readws(CURRENT_TOWN_ANIX) != 0) {

		ds_writew(PIC_COPY_X1, ds_readws(CURRENT_TOWN_ANIX) - 4);
		ds_writew(PIC_COPY_Y1, ds_readws(CURRENT_TOWN_ANIY) - 4);
		ds_writew(PIC_COPY_X2, ds_readws(CURRENT_TOWN_ANIX) + 4);
		ds_writew(PIC_COPY_Y2, ds_readws(CURRENT_TOWN_ANIY) + 4);
		ds_writed(PIC_COPY_SRC, (Bit32u)p2);

		if (is_mouse_in_rect(ds_readws(PIC_COPY_X1) - 16, ds_readws(PIC_COPY_Y1) - 16, ds_readws(PIC_COPY_X2) + 16, ds_readws(PIC_COPY_Y2) + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_save_rect();

		if (l_si) {
			refresh_screen_size();
		}

		ds_writew(CURRENT_TOWN_OVER, 1);
		ds_writew(CURRENT_TOWN_OVERX, ds_readw(CURRENT_TOWN_ANIX));
		ds_writew(CURRENT_TOWN_OVERY, ds_readw(CURRENT_TOWN_ANIY));
		l_si = 0;

		if ((ds_readws(MENU_INPUT_BUSY) != 0) && (ds_readb(SHOW_TRAVEL_MAP))) {

			ds_writew(SELECTED_TOWN_ANIX,
					ds_readws((TOWN_POSITIONS-4) + 4 * ds_readbs((TRV_MENU_TOWNS - 1) + ds_readws(MENU_SELECTED))));
			ds_writew(SELECTED_TOWN_ANIY,
					ds_readws((TOWN_POSITIONS-4) + 2 + 4 * ds_readbs((TRV_MENU_TOWNS - 1) + ds_readws(MENU_SELECTED))));

			ds_writew(PIC_COPY_X1, ds_readws(SELECTED_TOWN_ANIX) - 4);
			ds_writew(PIC_COPY_Y1, ds_readws(SELECTED_TOWN_ANIY) - 4);
			ds_writew(PIC_COPY_X2, ds_readws(SELECTED_TOWN_ANIX) + 4);
			ds_writew(PIC_COPY_Y2, ds_readws(SELECTED_TOWN_ANIY) + 4);
			ds_writed(PIC_COPY_SRC, (Bit32u)p1);

			if (is_mouse_in_rect(ds_readws(PIC_COPY_X1) - 16, ds_readws(PIC_COPY_Y1) - 16, ds_readws(PIC_COPY_X2) + 16, ds_readws(PIC_COPY_Y2) + 16))
			{
				update_mouse_cursor();
				l_si = 1;
			}

			do_save_rect();
			ds_writed(PIC_COPY_SRC, (Bit32u)(p3 + 100 * g_map_townmark_state));
			do_pic_copy(2);

			if (l_si) {
				refresh_screen_size();
			}

			ds_writew(TRV_MENU_SELECTION, ds_readws(MENU_SELECTED));
			l_si = 0;
		}

		ds_writew(PIC_COPY_X1, ds_readws(CURRENT_TOWN_ANIX) - 4);
		ds_writew(PIC_COPY_Y1, ds_readws(CURRENT_TOWN_ANIY) - 4);
		ds_writew(PIC_COPY_X2, ds_readws(CURRENT_TOWN_ANIX) + 4);
		ds_writew(PIC_COPY_Y2, ds_readws(CURRENT_TOWN_ANIY) + 4);
		ds_writed(PIC_COPY_SRC, (Bit32u)(p3 + 100 * (g_map_townmark_state + 5)));

		if (is_mouse_in_rect(ds_readws(PIC_COPY_X1) - 16, ds_readws(PIC_COPY_Y1) - 16, ds_readws(PIC_COPY_X2) + 16, ds_readws(PIC_COPY_Y2) + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_pic_copy(2);

		if (l_si) {
			refresh_screen_size();
		}

		ds_writew(CURRENT_TOWN_OVER, 1);
	}


	ds_writew(SPINLOCK_FLAG, 0);
	g_map_townmark_state++;
	g_map_townmark_state %= 5;

	*(struct dummy*)(p_datseg + PIC_COPY_DST) = a;
}

void set_and_spin_lock(void)
{
	ds_writew(SPINLOCK_FLAG, 1);

	while (ds_readw(SPINLOCK_FLAG)) {
#if !defined(__BORLANDC__)
		/* deadlock avoidance */
		static int cnt = 0;

		if (cnt % 256 == 0) {
			wait_for_vsync();
		}

		cnt++;
#else
	/* just spin */
#endif
	}
}

/**
 * \brief   called once every day at midnight
 */
void passages_recalc(void)
{
	signed short i;
	signed short di;
	signed short frequency_modifier; /* passages are rarer in summer and still rarer in winter */
	Bit8u *p;

	p = p_datseg + SEA_ROUTES;

	i = get_current_season();

	frequency_modifier = (i == SEASON_SUMMER) ? 2 : ((i == SEASON_WINTER) ? 4 : 0);
		/* winter -> 4
		 * summer -> 2
		 * spring, autumn -> 0 */

	for (i = 0; i < NR_SEA_ROUTES; p += SIZEOF_SEA_ROUTE, i++) {

		if (dec_ptr_bs(p + SEA_ROUTE_PASSAGE_TIMER) == -1) { /* note that dec_ptr_bs returns the old (still un-decremented) value */
			/* ship of a sea passage has left yesterday -> set up a new ship of this passage */

			host_writeb(p + SEA_ROUTE_PASSAGE_PRICE_MOD, (unsigned char)random_interval(70, 130));
			/* random price modifier in the range 70% -- 130% */

			host_writeb(p + SEA_ROUTE_PASSAGE_TIMER, random_interval(0, host_readbs(p + SEA_ROUTE_FREQUENCY) * 10 + host_readbs(p + SEA_ROUTE_FREQUENCY) * frequency_modifier) / 10);
			/* setup timer: In how many days will a ship of this passage be available? */
			/* This results in a random number in the interval [ 0..(SEA_ROUTE_FREQUENCY + frequency_modifier) ], where
			 * all numbers have the same probabilty, except the upper end SEA_ROUTE_FREQUENCY + frequency_modifier
			 * of the interval which has only 1/10 of the probabilty of each other number. */

			di = random_schick(100);

			host_writeb(p + SEA_ROUTE_PASSAGE_SHIP_TYPE,
				(!host_readbs(p + SEA_ROUTE_COSTAL_ROUTE)) ?
					((di <= 50) ? SHIP_TYPE_LANGSCHIFF_HIGH_SEAS : ((di <= 80) ? SHIP_TYPE_KARRACKE : (di <= 95) ? SHIP_TYPE_SCHNELLSEGLER : SHIP_TYPE_SCHNELLSEGLER_LUXURY)) :
					((di <= 10) ? SHIP_TYPE_LANGSCHIFF_COSTAL : ((di <= 40) ? SHIP_TYPE_KUESTENSEGLER : (di <= 80) ? SHIP_TYPE_KUTTER : SHIP_TYPE_FISCHERBOOT)));
#ifndef __BORLANDC__
			D1_INFO_VERBOSE("Neue Passage auf Seeroute ID %d (%s -- %s): Abfahrt in %d Tagen, Schiffstyp: %d, Preisanpassung: %d%%.\n",
					i,
					get_ttx(host_readb(p + SEA_ROUTE_TOWN_1) + 235),
					get_ttx(host_readb(p + SEA_ROUTE_TOWN_2) + 235),
					host_readb(p + SEA_ROUTE_PASSAGE_TIMER),
					host_readb(p + SEA_ROUTE_PASSAGE_SHIP_TYPE), // TODO: Ausgabe fuer Schiffstyp verbessern (momentan nur Zahl)
					host_readb(p + SEA_ROUTE_PASSAGE_PRICE_MOD)
			);
#endif
		}
	}

	/* If a passage is hired decrement Passage days timer */
	if (ds_readb(SEA_TRAVEL_PSGBOOKED_FLAG) == 0xaa) {
		dec_ds_bs_post(SEA_TRAVEL_PSGBOOKED_TIMER);
	}
}

/**
 * \brief   called once every day at 9 o'clock when the ships leave the harbors
 */
void passages_reset(void)
{
	signed short i;
	Bit8u *p = p_datseg + SEA_ROUTES;

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 36: the loop operates only on the first sea route (which is Thorwal-Prem) */
	for (i = 0; i < NR_SEA_ROUTES; i++)
#else
	for (i = 0; i < NR_SEA_ROUTES; p += SIZEOF_SEA_ROUTE, i++)
#endif
	{
		if (!host_readbs(p + SEA_ROUTE_PASSAGE_TIMER)) {
			host_writeb(p + SEA_ROUTE_PASSAGE_TIMER, -1);
		}
	}

	/* If a passage is booked and the timer is zero, the ship leaves the harbor.
	 * Therefore, reset the flag for the passage (i.e., unbook the passage) */
	if ((ds_readb(SEA_TRAVEL_PSGBOOKED_FLAG) == 0xaa) && !ds_readb(SEA_TRAVEL_PSGBOOKED_TIMER)) {
		ds_writeb(SEA_TRAVEL_PSGBOOKED_FLAG, 0);
	}
}

/**
 * \brief   forwards the ingame time
 *
 * \param   time        ticks to forward
 */
void timewarp(Bit32s time)
{
	signed short hour_old;
	signed short hour_new;
	Bit32s i;
	signed short td_bak;
	signed short hour_diff;
	Bit32s timer_bak;

	timer_bak = gs_day_timer;
	td_bak = g_timers_disabled;
	g_timers_disabled = 0;

	ds_writeb(FREEZE_TIMERS, 1);
	/* this deactivates the function calls sub_ingame_timers(1); and sub_mod_timers(1); in do_timers(); within the following loop.
	 * these timers will be dealt with in a single call sub_ingame_timers(time); and sub_mod_timers(time); for efficiency reasons.
	 */

	for (i = 0; i < time; i++) {
		do_timers();
#ifdef M302de_SPEEDFIX
		if (i % 768 == 0)
			wait_for_vsync();
#endif
	}

	sub_ingame_timers(time);

	sub_mod_timers(time);

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 37:
	 * Healing time-offs, staffspell time-offs and timers for the consumption of burning lanterns and torches
	 * do not get updated in certain situations, like a step forward in a city or dungeon.
	 * Reason: Because of rounding down, time / MINUTES(5) and time / MINUTES(15) will be 0 in many situations.
	 *
	 * Not exactly a masterpiece in modular arithmetics...
	 * see https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=146023#pid146023 */
	sub_heal_staffspell_timers(time / MINUTES(5));
	sub_light_timers(time / MINUTES(15));
#else
	sub_heal_staffspell_timers((time + (timer_bak % MINUTES(5))) / MINUTES(5));
	sub_light_timers((time + (timer_bak % MINUTES(15)))/ MINUTES(15));
#endif

#ifndef M302de_ORIGINAL_BUGFIX
	/* fixes Original-Bugs 38 and 39.
	 *
	 * Original-Bug 38:
	 * In some situations, the hourly damage from the magic chain mail is not correctly applied.
	 * For example, if the hero stays in an inn for from 8:30 till 8:00 the next day, he doesn't take any damage (while he should have suffered the damage 12 times).
	 *
	 * Below, the case (hour_old == hour_new) might also come from a difference of 23 hours (which has not been considered).
	 * Furthermore, according to do_timers(..), magical_chainmail_damage() should be called at *full* hours.
	 * For example, hour_new - hour_old == 1 could mean that 0 or 1 full hours have been passed.
	 *
	 * Again, sloppy treatment of modular arithmetics. */
	hour_old = (signed short)(timer_bak / HOURS(1));
	hour_new = (signed short)(gs_day_timer / HOURS(1));

	if (hour_old != hour_new) {
		if (hour_new > hour_old) {
			hour_diff = hour_new - hour_old;
		} else {
			hour_diff = 23 - hour_old + hour_new;
		}

		for (i = 0; hour_diff > i; i++) {
			magical_chainmail_damage();
			herokeeping();
			/* Original-Bug 39: Timers for 2nd encounter of the unicorn, Sphaerenriss in verfallene Herberge
			 * and two timers for barrels with orc muck in the orc dungeon are not affected from passing in-game time.
			 * Reason: They are present in do_timers(), but missing at this place.
			 * See https://www.crystals-dsa-foren.de/showthread.php?tid=5191&pid=146026#pid146026 */
		}
	}
#else
	/* fix Original-Bug 38. better not overcomplicate things... */
	for (i = 0; i < (time + (timer_bak % HOURS(1))) / HOURS(1); ++i) {
		magical_chainmail_damage();
		herokeeping();

		/* fix Original-Bug 39. add missing timers. */
		/* timer for 2nd encounter of the unicorn */
		if (ds_readb(UNICORN_GET_MAP) != 0 &&
			ds_readb(UNICORN_TIMER) != 0)
		{
			dec_ds_bs_post(UNICORN_TIMER);
		}

		/* timer for Sphaerenriss in verfallene Herberge */
		if (ds_readb(DNG02_SPHERE_TIMER) != 0) {

			if (!add_ds_bu(DNG02_SPHERE_TIMER, -1)) {
				ds_writeb(DNG02_SPHERE_ACTIVE, 1);
			}
		}

		/* timer for barrels with orc muck in the orc dungeon */
		if (ds_readbs(DNG08_TIMER1) != 0) {
			dec_ds_bs_post(DNG08_TIMER1);
		}
		if (ds_readbs(DNG08_TIMER2) != 0) {
			dec_ds_bs_post(DNG08_TIMER2);
		}
	}
#endif

	/* restore variables */
	ds_writeb(FREEZE_TIMERS, 0);
	g_timers_disabled = td_bak;
}

/**
 * \brief   forwards the ingame time till the given time of the day.
 *
 * \param   time        ticks to forward to, e.g to 6 AM. If the passed time agrees with the current time of the day, 24 hours will be forwarded.
 */
void timewarp_until_time_of_day(Bit32s time)
{
#ifdef M302de_ORIGINAL_BUGFIX
	/* The code of the function replicates the one of timewarp(..)
	 * Better call timewarp(..), such that we don't have to apply the same bugfixes twice.
	 * The bypassed code below suffers from Original-Bug 37 and 38. */
	if (gs_day_timer < time) {
		timewarp(time - gs_day_timer);
	} else {
		timewarp(DAYS(1) + time - gs_day_timer);
	}
#else
	signed short hour_old;
	signed short hour_new;
	Bit32s i;
	signed short td_bak;
	signed short j;
	signed short hour_diff;
	Bit32s timer_bak;

	i = 0;
	timer_bak = gs_day_timer;
	td_bak = g_timers_disabled;
	g_timers_disabled = 0;

	ds_writeb(FREEZE_TIMERS, 1);

	do {
		do_timers();
		i++;
#ifdef M302de_SPEEDFIX
		if (i % 768 == 0)
			wait_for_vsync();
#endif
	} while (gs_day_timer != time);

	sub_ingame_timers(i);

	sub_mod_timers(i);

	/* Original-Bug 37: see above. */
	sub_heal_staffspell_timers(i / MINUTES(5));
	sub_light_timers(i / MINUTES(15));

	hour_old = (signed short)(timer_bak / HOURS(1));
	hour_new = (signed short)(gs_day_timer / HOURS(1));

	/* Original-Bug 38: see above */
	if (hour_old != hour_new) {
		if (hour_new > hour_old) {
			hour_diff = hour_new - hour_old;
		} else {
			hour_diff = 23 - hour_old + hour_new;
		}

		for (j = 0; j < hour_diff; j++) {
			magical_chainmail_damage();
			herokeeping();
		}
	}

	/* Original-Bug:
	 * forgotten hourly timers: UNICORN_TIMER, DNG02_SPHERE_TIMER,DNG08_TIMER1, DNG08_TIMER2
	 * see do_timers(..).
	 * For a bugfix either add code here (and in timewarp(..)), or modify do_timers(..)
	 * */

	/* restore variables */
	ds_writeb(FREEZE_TIMERS, 0);
	g_timers_disabled = td_bak;
#endif
}

/**
 * \brief   decrements splash timer and restores picture
 */
void dec_splash(void)
{
	signed short i;

	for (i = 0; i <= 6; i++) {

		/* I have no clue */
		if (
			!ds_readbs(DIALOGBOX_LOCK) &&
			/* Check if splash timer is 0 */
			(ds_readbs(HERO_SPLASH_TIMER + i) != 0) &&
			!add_ds_bu(HERO_SPLASH_TIMER + i, -1) &&
			/* Check splash timer again if 0 */
			/* I have no clue */
			/* Could be in fight */
			(ds_readb(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) &&
			/* check if hero is dead */
			!hero_dead(get_hero(i))
		) {
			restore_rect((Bit8u*)ds_readd(VGA_MEMSTART), get_hero(i) + HERO_PORTRAIT, ds_readw(HERO_PIC_POSX + i * 2), 157, 32, 32);
		}
	}
}

/**
 * \brief   draws a splash on a hero portrait
 *
 * \param   hero_pos    on which slot the splash is drawn
 * \param   type        kind of damage (0 = red,LE / !0 = yellow,AE)
 */
/* static */
void draw_splash(signed short hero_pos, signed short type)
{
	/* Could be in fight */
	if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {

		Bit8u *splash = (type == 0 ? g_splash_le : g_splash_ae);

		restore_rect_rle((Bit8u*)ds_readd(VGA_MEMSTART), splash, ds_readw(HERO_PIC_POSX + 2 * hero_pos), 157, 32, 32, 2);

		/* how long the splash should be displayed */
		ds_writeb(HERO_SPLASH_TIMER + hero_pos, 10);
	}
}

/**
 * \brief   fast forward the ingame time to midnight
 *          If it is precicely midnight, 1 full day will be passing.
 */
void timewarp_until_midnight(void)
	/* only called at a single place, at the Swafnild encounter in seg072.cpp */
{
	/* TODO: This doesn't look all correct to me... Have all timers been considered? Why not call timewarp_until_time_of_day(..)? */
	Bit32s ticks_left;
	signed short td_bak;

	/* save the timers status */
	td_bak = g_timers_disabled;

	/* enable timers */
	g_timers_disabled = 0;

	/* calculate the ticks left on this day */
	ticks_left = (HOURS(24) - 1) - gs_day_timer;

	/* Set the day timer to one tick before midnight.
	 * Original-Bug: Hard setting the time skips events at a fixed time of the day, like getting sober at 10 o'clock. */
	gs_day_timer = HOURS(24) - 1L;

	do_timers(); /* now it is precisely midnight */
	sub_ingame_timers(ticks_left);
	sub_mod_timers(ticks_left);
	sub_heal_staffspell_timers(ticks_left / MINUTES(5)); /* I think the rounding effects are indeed correct this time. More luck than reason... */
	sub_light_timers(100); /* Original-Bug: why not sub_light_timers(ticks_left / MINUTES(15)) ?? */

	/* Original-Bug: missing: magical_chainmail_damage();
	 * Original-Bug 39: Timers for 2nd encounter of the unicorn, Sphaerenriss in verfallene Herberge
	 * and two timers for barrels with orc muck in the orc dungeon are not affected from passing in-game time. */

	/* restore the timer status */
	g_timers_disabled = td_bak;
}

void wait_for_keyboard2(void)
{
	while (CD_bioskey(1)) {
#if !defined(__BORLANDC__)
		D1_LOG("loop in %s\n", __func__);
#endif
		bioskey(0);
	}
}


/* unused */
#if defined(__BORLANDC__)
static void seg002_4031(char *ptr)
{
	delay_or_keypress(150 * GUI_print_header(ptr));
}
#endif

void wait_for_keypress(void)
{
	signed short si;

#if defined(__BORLANDC__)
	flushall();
#endif

	ds_writew(MOUSE1_EVENT2, 0);

	do {
		if (CD_bioskey(1)) {

			si = bioskey(0);

			if (((si & 0xff) == 0x20) &&
				(ds_readw(BIOSKEY_EVENT10) == 0))
			{

				seg002_47e2();

				while (!CD_bioskey(1)) {;}

				seg002_484f();
				break;
			}
		}

	} while (!CD_bioskey(1) && ds_readw(MOUSE1_EVENT2) == 0);

	if (CD_bioskey(1))
		si = bioskey(0);

	ds_writew(MOUSE1_EVENT2, 0);
}

/**
 * \brief   wait until duration time is elapsed or a key is pressed
 *
 * \param   duration    the maximal time to wait (1 = 15ms, 66 = 1s)
 */
void delay_or_keypress(signed short duration)
{
	signed short done = 0;
	signed short counter = 0;

	while (counter < duration) {

		ds_writeb(DELAY_OR_KEYPRESS_FLAG, 1);
		handle_input();
		ds_writeb(DELAY_OR_KEYPRESS_FLAG, 0);

		if (ds_readb(C_EVENT_ACTIVE) != 0) {

			if (ds_readw(ACTION) != 0) {

				if (ds_readw(ACTION) == ACTION_ID_SPACE) {

					seg002_47e2();
					while (!CD_bioskey(1)) { ; }
					seg002_484f();
					done = 1;

				} else {

					if ((ds_readw(ACTION) != ACTION_ID_UP) &&
						(ds_readw(ACTION) != ACTION_ID_DOWN) &&
						(ds_readw(ACTION) != ACTION_ID_RIGHT) &&
						(ds_readw(ACTION) != ACTION_ID_LEFT))
					{
						done = 1;
					}
				}
			} else {

				if (ds_readw(MOUSE2_EVENT) != 0) {
					done = 1;
				}
			}
		} else {

			if (ds_readw(ACTION) != 0) {

				if (ds_readw(ACTION) == ACTION_ID_SPACE) {

					seg002_47e2();
					while (!CD_bioskey(1)) { ; }
					seg002_484f();
				}

				done = 1;

			} else {

				if (ds_readw(MOUSE2_EVENT) != 0) {
					done = 1;
				}
			}
		}


		if (done) {
			ds_writew(MOUSE2_EVENT, 0);
			ds_writew(ACTION, ACTION_ID_RETURN);
			break;
		}

		wait_for_vsync();

		counter++;
	}
}

/* unused */
void unused_delay(signed short no)
{
	signed short i = 0;

	while (i < no) {
		wait_for_vsync();
		i++;
	}
}

/* unused */
void unused_spinlock(void)
{
	ds_writew(UNUSED_SPINLOCK_FLAG, 1);

	while (ds_readw(UNUSED_SPINLOCK_FLAG) != 0) {
	}
}

/**
 * \brief   calculates a 32bit BigEndian value into LittleEndian
 *
 * \param   v           32bit BE value
 * \return              32bit LE value
 */
Bit32s swap_u32(Bit32u v)
{
	register signed short tmp;
	signed short a[2];
	Bit32s *ptr = (Bit32s*)(&a[0]);

	*ptr = host_readd((Bit8u*)&v);

	tmp = a[0];
	a[0] = swap_u16(a[1]);
	a[1] = swap_u16(tmp);

	return host_readd((Bit8u*)ptr);
}

/* unused */
Bit32u swap_u32_unused(Bit32u v)
{
	signed short a[2];
	signed short tmp;
	Bit32s *ptr = (Bit32s*)(&a[0]);

	tmp = (signed short)(*ptr = host_readd((Bit8u*)&v));

	a[0] = a[1];
	a[1] = tmp;

	return host_readd((Bit8u*)ptr);
}

/**
 * \brief   allocates EMS memory
 *
 * \param   bytes       bytes to allocate
 * \return              an EMS handle, to access the memory.
 */
signed short alloc_EMS(Bit32s bytes)
{
	signed short handle;

	/* calculate the number of needes EMS pages */
	handle = (signed short)((bytes / 0x4000) + 1);

	/* check if enought EMS is free */
	if (EMS_get_num_pages_unalloced() >= handle) {

		/* if allocation was successful return the handle */
		if ((handle = EMS_alloc_pages(handle)) != 0) {
			return handle;
		}
	}

	return 0;
}

void from_EMS(Bit8u* dst, signed short handle, Bit32s bytes)
{
#if defined(__BORLANDC__)
	signed short si;
	signed short di;
	signed short v1;
	signed short len;
	Bit8u* ptr;

	di = (signed short)(bytes / 0x4000 + 1);
	v1 = si = 0;

	do {
		EMS_map_memory(handle, v1++, 0);
		ptr = (Bit8u*)F_PADD(dst, (((Bit32s)si) << 0x0e));
		si++;

		len = (bytes - 0x4000 > 0) ? 0x4000 : (signed short)bytes;

		bytes -= 0x4000;

		memmove((void*)EMS_norm_ptr(ptr), (void*)ds_readd(EMS_FRAME_PTR), len);

	} while (--di != 0);
#endif
}

void to_EMS(signed short handle, Bit8u* src, Bit32s bytes)
{
#if defined(__BORLANDC__)
	signed short si;
	signed short di;
	signed short v1;
	signed short len;
	Bit8u* ptr;

	di = (signed short)(bytes / 0x4000 + 1);
	v1 = si = 0;

	do {
		EMS_map_memory(handle, v1++, 0);
		ptr = (Bit8u*)F_PADD(src, ((((Bit32s)si) << 0x0e)));
		si++;

		len = (bytes - 0x4000 > 0) ? 0x4000 : (signed short)bytes;

		bytes -= 0x4000;

		memmove((void*)ds_readd(EMS_FRAME_PTR), (void*)EMS_norm_ptr(ptr), len);

	} while (--di != 0);
#endif
}

void set_to_ff(void)
{
	signed short i;

	for (i = 0; i < 9; i++) {
		ds_writeb(NEW_MENU_ICONS + i, MENU_ICON_NONE);
	}
}

/**
 * \brief   draws the icons of locations
 *
 * \param   icons       number of icons
 * \param   ...         icon ids
 */
void draw_loc_icons(signed short icons, ...)
{
	signed short icons_bak[9];
	va_list arguments;
	signed short i, changed;

	changed = 0;

	/* save icon ids in local variable */
	for (i = 0; i < 9; i++) {
		icons_bak[i] = ds_readbs(NEW_MENU_ICONS + i);
		ds_writeb(NEW_MENU_ICONS + i, MENU_ICON_NONE);
	}

	va_start(arguments, icons);

	for (i = 0; i < icons; i++) {
		ds_writeb(NEW_MENU_ICONS + i, va_arg(arguments, int));

		if (ds_readbs(NEW_MENU_ICONS + i) != icons_bak[i]) {
			changed = 1;
		}
	}

	va_end(arguments);

	if (icons_bak[i] != -1)
		changed = 1;

	if (changed && ds_readb(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
		draw_icons();
	}
}

signed short mod_day_timer(signed short val)
{
	return ((gs_day_timer % val) == 0) ? 1 : 0;
}

void draw_compass(void)
{
	signed short width;
	signed short height;
	struct nvf_desc n;

	/* No compass in a location */
	if (!gs_current_loctype &&
		/* Has something to do with traveling */
		!ds_readbs(TRAVEL_EVENT_ACTIVE) &&
		/* Not in town or dungeon */
		((gs_dungeon_index != DUNGEONS_NONE) || (gs_current_town != TOWNS_NONE)) &&
		/* I have no clue */
		(g_fading_state != 2))
	{

		/* set src */
		n.dst = g_icon;
		/* set dst */
		n.src = g_buffer6_ptr;
		/* set no */
		n.no = gs_direction;
		/* set type*/
		n.type = 0;

		n.width = (Bit8u*)&width;
		n.height = (Bit8u*)&height;

		/* process the nvf */
		process_nvf(&n);

		/* set x and y values */
		ds_writew(PIC_COPY_X1, 94);
		ds_writew(PIC_COPY_Y1, 115);
		ds_writew(PIC_COPY_X2, 145);
		ds_writew(PIC_COPY_Y2, 136);

		/* set source */
		ds_writed(PIC_COPY_SRC, (Bit32u)g_icon);

		update_mouse_cursor();
		do_pic_copy(2);
		refresh_screen_size();
	}
}

signed short can_merge_group(void)
{
	signed short i;
	signed short retval = -1;

	if (gs_group_member_counts[gs_current_group] == gs_total_hero_counter) {

		retval = -1;

	} else {

		for (i = 0; i < 6; i++)	{

			if ((i != gs_current_group) &&
				(0 != gs_group_member_counts[i]) &&
				/* check XTarget */
				(gs_groups_x_target[i] == gs_x_target) &&
				/* check YTarget */
				(gs_groups_y_target[i] == gs_y_target) &&
				/* check Location */
				(gs_groups_current_loctype[i] == gs_current_loctype) &&
				/* check currentTown */
				(gs_groups_town[i] == gs_current_town) &&
				/* check DungeonIndex */
				(gs_groups_dng_index[i] == gs_dungeon_index) &&
				/* check DungeonLevel */
				(gs_groups_dng_level[i] == gs_dungeon_level))
			{
				retval = i;
			}
		}
	}

	return retval;
}

unsigned short div16(unsigned short val)
{
	return ((unsigned char)val) >> 4;
}

void select_with_mouse(Bit8u *p1, Bit8u *p2)
/* This function is called in shops at sell/buy screens */
{
	signed short i;

	if (ds_readw(HAVE_MOUSE) != 2) {
		return;
	}

	for (i = 0; i < 15; i++) {
		if ((ds_readws(MERCHANT_ITEMS_POSX + i * 2) <= ds_readws(MOUSE_POSX)) &&
			(ds_readws(MERCHANT_ITEMS_POSX + i * 2) + 50 >= ds_readws(MOUSE_POSX)) &&
			(ds_readws(MERCHANT_ITEMS_POSY + i * 2) <= ds_readws(MOUSE_POSY)) &&
			(ds_readws(MERCHANT_ITEMS_POSY + i * 2) + 17 >= ds_readws(MOUSE_POSY)) &&
			(host_readws(p2 + i * 7) != 0))
		{
			host_writew(p1, i);
			return;
		}
	}
}

void select_with_keyboard(Bit8u *p1, Bit8u *p2)
{
	signed short pos = host_readws(p1);

	if (ds_readw(ACTION) == ACTION_ID_UP) {
		/* Key UP */
		if (pos) {
			pos--;
		} else {
			pos = 14;
			while (host_readw(p2 + pos * 7) == 0) {
				pos--;
			}
		}
	} else if (ds_readw(ACTION) == ACTION_ID_DOWN) {
		/* Key DOWN */
		if (pos < 14) {
			if (host_readw(p2 + (pos + 1) * 7) != 0) {
				pos++;
			} else {
				pos = 0;
			}
		} else {
			pos = 0;
		}
	} else if (ds_readw(ACTION) == ACTION_ID_RIGHT) {
		/* Key RIGHT */
		if (pos < 10) {
			if (host_readw(p2 + (pos + 5) * 7) != 0) {
				pos += 5;
			}
		} else {
			pos -= 10;
		}
	} else if (ds_readw(ACTION) == ACTION_ID_LEFT) {
		/* Key LEFT */
		if (pos > 4) {
			pos -= 5;
		} else {
			if (host_readw(p2 + (pos + 10) * 7) != 0) {
				pos += 10;
			}
		}
	}

	host_writew(p1, pos);
}

/**
 * \brief   marks a tile in the automap as seen
 *
 * \param   x           X coordinate
 * \param   y           Y coordinate
 */
void set_automap_tile(signed short x, signed short y)
{
	or_ds_bs(AUTOMAP_BUF + (4 * y + (x >> 3)), ds_readb(AUTOMAP_BITMASK + (x & 0x7)));
}

/**
 * \brief   marks all sourrounding tiles in the automap as seen
 *
 * \param   x           X xoordinate
 * \param   y           Y xoordinate
 */
void set_automap_tiles(signed short x, signed short y)
{
	/* set upper line */
	if (y > 0) {

		if (x > 0) {
			set_automap_tile(x - 1, y - 1);
		}

		set_automap_tile(x, y - 1);

		if (ds_readb(DNG_MAP_SIZE) - 1 > x) {
			set_automap_tile(x + 1, y - 1);
		}
	}

	/* set middle line */
	if (x > 0) {
		set_automap_tile(x - 1, y);
	}

	set_automap_tile(x, y);

	if (ds_readb(DNG_MAP_SIZE) - 1 > x) {
		set_automap_tile(x + 1, y);
	}

	/* set lower line */
	if (y < 15) {
		if (x > 0) {
			set_automap_tile(x - 1, y + 1);
		}

		set_automap_tile(x, y + 1);

		if (ds_readb(DNG_MAP_SIZE) - 1 > x) {
			set_automap_tile(x + 1, y + 1);
		}
	}
}

/**
 * \brief   */
void seg002_47e2(void)
{
	/* save gfx settings to stack */
	struct dummy a = *(struct dummy*)(p_datseg + PIC_COPY_DST);

	/* set range 0,0 - 7,7 */
	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 7);
	ds_writew(PIC_COPY_Y2, 7);

	/* set destination */
	ds_writed(PIC_COPY_DST, ds_readd(VGA_MEMSTART));
	/* set source */
	ds_writed(PIC_COPY_SRC, (Bit32u)g_gfxbuf_wait_keypress);

	do_save_rect();

	GUI_print_char('P', 0, 0);

	/* restore gfx settings from stack */
	*(struct dummy*)(p_datseg + PIC_COPY_DST) = a;
}

/**
 */
void seg002_484f(void)
{
	/* save gfx settings to stack */
	struct dummy a = *(struct dummy*)(p_datseg + PIC_COPY_DST);

	/* set range 0,0 - 7,7 */
	ds_writew(PIC_COPY_X1, 0);
	ds_writew(PIC_COPY_Y1, 0);
	ds_writew(PIC_COPY_X2, 7);
	ds_writew(PIC_COPY_Y2, 7);

	/* set destination */
	ds_writed(PIC_COPY_DST, ds_readd(VGA_MEMSTART));
	/* set source */
	ds_writed(PIC_COPY_SRC, (Bit32u)g_gfxbuf_wait_keypress);

	do_pic_copy(0);

	/* restore gfx settings from stack */
	*(struct dummy*)(p_datseg + PIC_COPY_DST) = a;
}

/**
 * \brief   returns true if the hero is not sleeping, dead, petrified, unconscious, renegade or fleeing
 *
 * \param   hero        pointer to the hero
 * \return              {0, 1}
 */
/* should be static */
signed short check_hero(Bit8u *hero)
{
	if (
		!host_readbs(hero + HERO_TYPE) ||
		hero_asleep(hero) ||
		hero_dead(hero) ||
		hero_petrified(hero) ||
		hero_unconscious(hero) ||
		hero_renegade(hero) ||
		(host_readb(hero + HERO_ACTION_ID) == FIG_ACTION_FLEE)
	) {
		return 0;
	}

	return 1;
}

/**
 * \brief   returns true if the hero is not dead, petrified, unconscious or renegade
 */
/* should be static */
signed short check_hero_no2(Bit8u *hero)
{

	if (
		!host_readbs(hero + HERO_TYPE) ||
		hero_dead(hero) ||
		hero_petrified(hero) ||
		hero_unconscious(hero) ||
		hero_renegade(hero)
	) {
		return 0;
	}

	return 1;
}

/**
 * \brief   check if hero is not dead, petrified or unconscious
 *
 * \param   hero        pointer to the hero
 * \return              {0, 1}
 */
/* should be static */
signed short check_hero_no3(Bit8u *hero)
{
	if (
		!host_readbs(hero + HERO_TYPE) ||
		hero_dead(hero) ||
		hero_petrified(hero) ||
		hero_unconscious(hero)
	) {
		return 0;
	}

	return 1;
}

signed short is_hero_available_in_group(Bit8u *hero)
{

	if (
		check_hero(hero) &&
		(host_readbs(hero + HERO_GROUP_NO) == gs_current_group)
	) {
		return 1;
	}

	return 0;
}

/**
 * \brief   subtract current ae with a splash
 *
 * \param   hero        the magicuser
 * \param   ae          astralenergy to subtract
 */
void sub_ae_splash(Bit8u *hero, signed short ae)
{
	if (!hero_dead(hero) && (ae > 0)) {

		signed short tmp = ds_readw(UPDATE_STATUSLINE);
		ds_writew(UPDATE_STATUSLINE, 0);

		if ((host_readb(hero + HERO_TYPE) == HERO_TYPE_MAGE) &&
		    (host_readbs(hero + HERO_STAFFSPELL_LVL) >= 4)) {
			/* 4th staff spell reduces AE cost by 2 */
			ae -= 2;
			if (ae < 0)
				ae = 0;
		}

		/* Calc new AE */
		sub_ptr_ws(hero + HERO_AE, ae);

		/* Draw the splash */
		draw_splash(get_hero_index(hero), 1);

		/* set AE to 0 if they have gotten lower than 0 */
		if (host_readws(hero + HERO_AE) < 0) {
			host_writew(hero + HERO_AE, 0);
		}

		ds_writew(UPDATE_STATUSLINE, tmp);

#ifdef M302de_ORIGINAL_BUGFIX
		/* AE bar was not updated in pseudo 3D mode */
		if (!g_in_fight && ds_readw(MOUSE1_DOUBLECLICK) != 0) {
			/* redraw AE bar */
			draw_bar(1, get_hero_index(hero), host_readw(hero + HERO_AE),
				host_readw(hero + HERO_AE_ORIG), 0);
		}
#endif
	}

}

/**
 * \brief   add AE points to the current AE of a hero.
 */
void add_hero_ae(Bit8u* hero, signed short ae)
{
	/* dont add AE if hero is dead or ae = 0 */
	if (!hero_dead(hero) && (ae > 0)) {

		signed short tmp = ds_readw(UPDATE_STATUSLINE);
		ds_writew(UPDATE_STATUSLINE, 0);

		/* add AE to hero's current AE */
		add_ptr_ws(hero + HERO_AE, ae);

		/* if current AE is greater than AE maximum
			set current AE to AE maximum */
		if (host_readws(hero + HERO_AE) > host_readws(hero + HERO_AE_ORIG))
			host_writew(hero + HERO_AE, host_readws(hero + HERO_AE_ORIG));

		ds_writew(UPDATE_STATUSLINE, tmp);
	}
}

/**
 * \brief   subtracts LE from a hero
 *
 * \param   hero        pointer to the hero
 * \param   le          LE the hero looses
 */
void sub_hero_le(Bit8u *hero, signed short le)
{
	signed short i;
	signed short bak;
	signed short old_le;
	Bit8u *ptr;
	Bit8u *hero_i;

	if (!hero_dead(hero) && (le > 0)) {

		bak = ds_readw(UPDATE_STATUSLINE);
		ds_writew(UPDATE_STATUSLINE, 0);

		/* do the damage */
		old_le = host_readw(hero + HERO_LE);
		sub_ptr_ws(hero + HERO_LE, le);

		if (hero_asleep(hero)) {

			/* awake him/her */
			and_ptr_bs(hero + HERO_FLAGS1, 0xfd); /* unset 'asleep' flag */

			/* in fight mode */
			if (g_in_fight) {
				ptr = (Bit8u*)FIG_get_ptr(host_readb(hero + HERO_FIGHTER_ID));

				/* update looking dir and other  */
				host_writeb(ptr + FIGHTER_NVF_NO, host_readb(hero + HERO_VIEWDIR));
				host_writeb(ptr + FIGHTER_RELOAD, -1);
				host_writeb(ptr + FIGHTER_OFFSETX, 0);
				host_writeb(ptr + FIGHTER_OFFSETY, 0);
			}
		}

		draw_splash(get_hero_index(hero), 0);

		if (host_readws(hero + HERO_LE) <= 0) {

			/* set LE to 0 */
			host_writew(hero + HERO_LE, 0);

			/* mark hero as dead */
			or_ptr_bs(hero + HERO_FLAGS1, 1); /* set 'dead' flag */

			gs_unconscious_message[get_hero_index(hero)] = 0;

			/* unknown */
			host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_UNKNOWN2);

			if (ds_readb(PP20_INDEX) == ARCHIVE_FILE_PLAYM_UK) {
				ds_writeb(REFRESH_STATUS_LINE, 1);
			}

			/* reset sickness */
			for (i = 1; i <= 7; i++) {
				host_writeb(hero + HERO_ILLNESS + i * SIZEOF_HERO_ILLNESS, 0);
				host_writeb(hero + (HERO_ILLNESS + 1) + i * SIZEOF_HERO_ILLNESS, 0);
			}

			/* reset poison */
			for (i = 1; i <= 9; i++) {
				host_writeb(hero + HERO_POISON + i * SIZEOF_HERO_POISON, 0);
				host_writeb(hero + (HERO_POISON + 1) + i * SIZEOF_HERO_POISON, 0);
			}

			/* FINAL FIGHT */
			if (g_current_fight_no == FIGHTS_F144) {
				if (hero == (Bit8u*)ds_readd(MAIN_ACTING_HERO)) {
					ds_writew(GAME_STATE, GAME_STATE_DEAD);
					g_in_fight = 0;
				}
			}

			if ((ds_readb(TRAVELING) != 0)
				&& !g_in_fight &&
				(!count_heroes_available_in_group() || ((count_heroes_available_in_group() == 1) && is_hero_available_in_group(get_hero(6))))) /* count_heroes_available_in_group_ignore_npc() == 0 */
			{
				/* if traveling, not in a fight, and no hero in the group (except possibly the NPC) is available. */

				ds_writeb(TRAVEL_DETOUR, 99);

				hero_i = get_hero(0);
				for (i = 0; i <=6; i++, hero_i += SIZEOF_HERO) {
					if ((host_readbs(hero_i + HERO_TYPE) != HERO_TYPE_NONE) &&
						(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group))
					{
						hero_disappear(hero_i, i, -1);
					}
				}
			}

		} else {
			if ((old_le >= 5) && (host_readws(hero + HERO_LE) < 5)) {

				/* make hero unsonscious */
				or_ptr_bs(hero + HERO_FLAGS1, 0x40); /* set 'unconscious' flag */

				/* unknown yet */
				host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);

				/* unknown yet */
				gs_unconscious_message[get_hero_index(hero)] = 1;

				/* in fight mode */
				if (g_in_fight) {

					ptr = (Bit8u*)FIG_get_ptr(host_readb(hero + HERO_FIGHTER_ID));

					host_writeb(ptr + 2,
						ds_readb(NVFTAB_FIGURES_UNCONSCIOUS + host_readbs(hero + HERO_SPRITE_NO) * 2) + host_readbs(hero + HERO_VIEWDIR));

					host_writeb(ptr + 0x0d, -1);

					host_writeb(ptr + 5,
						ds_readb(GFXTAB_OFFSETS_UNCONSCIOUS + host_readbs(hero + HERO_SPRITE_NO) * 8 + host_readbs(hero + HERO_VIEWDIR) * 2));

					host_writeb(ptr + 6,
						ds_readb((GFXTAB_OFFSETS_UNCONSCIOUS + 1) + host_readbs(hero + HERO_SPRITE_NO) * 8 + host_readbs(hero + HERO_VIEWDIR) * 2));


					FIG_add_msg(7, 0);

					/* FINAL FIGHT */
					if (g_current_fight_no == FIGHTS_F144) {
						if (hero == (Bit8u*)ds_readd(MAIN_ACTING_HERO)) {
							ds_writew(GAME_STATE, GAME_STATE_DEAD);
							g_in_fight = 0;
						}
					}
				}
			}
		}

		ds_writew(UPDATE_STATUSLINE, bak);
	}

	if (!g_in_fight) {
		g_check_party = 1;
	}
}

/**
 * \brief   regenerates LE of a hero
 *
 *          This functions does some magic in fights, when a hero awakes.
 *
 * \param   hero        pointer to the hero
 * \param   le          LE to be regenerated
 */
void add_hero_le(Bit8u *hero, signed short le)
{
	signed short val_bak;
	Bit8u *ptr;
	signed short ret;

	/* dead heroes never get LE */
	if (!hero_dead(hero) && (le > 0)) {

		val_bak = ds_readw(UPDATE_STATUSLINE);
		ds_writew(UPDATE_STATUSLINE, 0);

		/* add LE */
		add_ptr_ws(hero + HERO_LE, le);

		/* set LE to maximum if greater than maximum */
		if (host_readws(hero + HERO_LE) > host_readws(hero + HERO_LE_ORIG))
			host_writew(hero + HERO_LE, host_readws(hero + HERO_LE_ORIG));

		/* if current LE is >= 5 and the hero is unconscious */
		if ((host_readws(hero + HERO_LE) >= 5) && hero_unconscious(hero)) {

			/* awake */
			and_ptr_bs(hero + HERO_FLAGS1, 0xbf); /* set 'conscious' flag */

			/* maybe if we are in a fight */
			if (g_in_fight) {
				ptr = (Bit8u*)FIG_get_ptr(host_readb(hero + HERO_FIGHTER_ID));
				ret = FIG_get_range_weapon_type(hero);

				if (ret != -1) {
					host_writeb(ptr + 2, ds_readb((NVFTAB_FIGURES_RANGEWEAPON - 12) +
						host_readbs(hero + HERO_SPRITE_NO) * 12 + 4 * ret + host_readbs(hero + HERO_VIEWDIR)));
				} else {
					host_writeb(ptr + 2, host_readb(hero + HERO_VIEWDIR));
				}

				host_writeb(ptr + 0x0d, -1);
				host_writeb(ptr + 5, 0);
				host_writeb(ptr + 6, 0);
			}
		}

		ds_writew(UPDATE_STATUSLINE, val_bak);
	}
}

/**
 * \brief   regenerates LE of a group
 *
 * \param   le          LE to be regenerated
 */
void add_group_le(signed short le)
{

	Bit8u *hero;
	signed short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

		if ((host_readb(hero + HERO_TYPE) != HERO_TYPE_NONE) &&
			(host_readbs(hero + HERO_GROUP_NO) == gs_current_group))
		{
			add_hero_le(hero, le);
		}
	}
}

/**
 * \brief   damages starving heroes
 *
 * \param   hero        a pointer to the hero
 * \param   index       the index number of the hero
 * \param   type        the type of message which should be printed (0 = hunger / 1 = thirst)
 */
void do_starve_damage(Bit8u *hero, signed short index, signed short type)
{
	/* check if the hero is dead */
	if (!hero_dead(hero)) {

		/* save this value locally */
		const int sl_bak = ds_readw(UPDATE_STATUSLINE);
		ds_writew(UPDATE_STATUSLINE, 0);

		/* decrement LE of the hero */
		dec_ptr_ws(hero + HERO_LE);

		/* set the critical message type for the hero */
		gs_food_message[index] = (type != 0 ? 1 : 2);

		if (host_readws(hero + HERO_LE) <= 0) {

			/* don't let the hero die */
			host_writew(hero + HERO_LE, 1);

			/* decrement the max LE and save them at 0x7a */
			if (host_readws(hero + HERO_LE_ORIG) >= 2) {
				dec_ptr_ws(hero + HERO_LE_ORIG);
				inc_ptr_bs(hero + HERO_LE_MOD);
			}
		}

		/* restore the locally save value */
		ds_writew(UPDATE_STATUSLINE, sl_bak);
	}
}

/* unused */
signed short compare_name(Bit8u *name)
{
	signed short i;

	for (i = 0; i < 6; i++) {

		if (!strcmp((char*)(get_hero(i) + HERO_NAME2), (char*)name)) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief   make an attribute test
 *
 * \param   hero        pointer to the hero
 * \param   attrib      number of the attribute
 * \param   handicap    may be positive or negative. The higher the value, the harder the test.
 * \return              the result of the test. > 0: success; <= 0: failure; -99: critical failure
 */
signed short test_attrib(Bit8u* hero, signed short attrib, signed short handicap)
{
	signed short si = random_schick(20);
	signed short tmp;

#if !defined(__BORLANDC__)
	D1_INFO("Eigenschaftsprobe %s auf %s %+d: W20 = %d",
		(char*)(hero + HERO_NAME2), names_attrib[attrib], handicap, si);
#endif

	if (si == 20) {
#if !defined(__BORLANDC__)
		D1_INFO("Ungluecklich\n");
#endif
		return -99;
	} else {

		si += handicap;
	}

	tmp = host_readbs(hero + 3 * attrib + HERO_ATTRIB) + host_readbs(hero + 3 * attrib + HERO_ATTRIB_MOD);

#if !defined(__BORLANDC__)
	D1_INFO(" -> %s mit %d\n",
		(tmp - si + 1) > 0 ? "bestanden" : "nicht bestanden", (tmp - si + 1));
#endif

	return tmp - si + 1;
}

/**
 * \brief   make three attribute tests
 *
 * \param   hero        pointer to the hero
 * \param   attrib1     attribute 1
 * \param   attrib2     attribute 2
 * \param   attrib3     attribute 3
 * \param   handicap    may be positive or negative. The higher the value, the harder the test.
 * \return              a test is successful if the return value is greater than zero
 *                      in detail:
 *                      unlucky fail: -99
 *                      lucky success: +99 (only if M302de_FEATURE_MOD is activated)
 *                      ordinary fail: any value between -98 and 0.
 *                      ordinary success: any value between 1 and 98.
 */

signed short test_attrib3(Bit8u* hero, signed short attrib1, signed short attrib2, signed short attrib3, signed char handicap)
{
#ifndef M302de_FEATURE_MOD
	/* Feature mod 6: The implementation of the skill test logic differs from the original DSA2/3 rules.
	 * It is sometimes called the 'pool' variant, where '3W20 + handicap' is compared to the sum of the attributes.
	 * It is significantly easier than the original rule, where each individuall roll must be at most the corresponding attribute,
	 * where positive handicap must be used up during the process, and negative handicap may be used for compensation. */

	/* in the original pool variant, the ramining pool, increased by 1, is returned. */

	/* in the modified variant, ordinary failure gives always return value 0.
	 * Ordinary success leads to return value 1, or, in the case of a negative handicap (in other words, a positive bonus),
	 * the number of remaining bonus points, increased by 1.  */

	signed short i;
	signed short rolls_sum;
	signed short tmp;
	signed short nr_rolls_20;

	nr_rolls_20 = 0;
	rolls_sum = 0;

#if !defined(__BORLANDC__)
	D1_INFO(" (%s/%s/%s) %+d -> ",
		names_attrib[attrib1], names_attrib[attrib2], names_attrib[attrib3], handicap);
#endif

	for (i = 0; i < 3; i++) {

		tmp = random_schick(20);

#if !defined(__BORLANDC__)
		D1_INFO("%d ", tmp);
#endif

		if (tmp == 20) {
			if (++nr_rolls_20 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> UNGLUECKLICH! nicht bestanden.\n");
#endif
				return -99;
			}
		}

		rolls_sum += tmp;
	}

	rolls_sum += handicap;

	tmp = host_readbs(hero + 3 * attrib1 + HERO_ATTRIB) +
		host_readbs(hero + 3 * attrib1 + HERO_ATTRIB_MOD) +
		host_readbs(hero + 3 * attrib2 + HERO_ATTRIB) +
		host_readbs(hero + 3 * attrib2 + HERO_ATTRIB_MOD) +
		host_readbs(hero + 3 * attrib3 + HERO_ATTRIB) +
		host_readbs(hero + 3 * attrib3 + HERO_ATTRIB_MOD);

#if !defined(__BORLANDC__)
	D1_INFO(" -> %s mit %d\n",
		(tmp - rolls_sum + 1) > 0 ? "bestanden" : "nicht bestanden", (tmp - rolls_sum + 1));
#endif
	return tmp - rolls_sum + 1; // in a nutshell: sum of the 3 attributes - 3*D20 - handicap + 1

#else
	/* Here, the original DSA2/3 skill test logic is implemented.
	 * WARNING: This makes skill tests, and thus the game, significantly harder!
	 * Note that we are not implementing the DSA4 rules, where tests with a positive handicap are yet harder. */
	signed short i;
	signed short tmp;
	signed short nr_rolls_1 = 0;
	signed short nr_rolls_20 = 0;
	signed short fail = 0;
	signed char attrib [3];

	attrib[0] = host_readbs(hero + 3 * attrib1 + HERO_ATTRIB) + host_readbs(hero + 3 * attrib1 + HERO_ATTRIB_MOD);
	attrib[1] = host_readbs(hero + 3 * attrib2 + HERO_ATTRIB) + host_readbs(hero + 3 * attrib2 + HERO_ATTRIB_MOD);
	attrib[2] = host_readbs(hero + 3 * attrib3 + HERO_ATTRIB) + host_readbs(hero + 3 * attrib3 + HERO_ATTRIB_MOD);

#if !defined(__BORLANDC__)
	D1_INFO(" (%s %d/%s %d/%s %d) ->",
		names_attrib[attrib1],
		attrib[0],
		names_attrib[attrib2],
		attrib[1],
		names_attrib[attrib3],
		attrib[2]
	);
#endif

	for (i = 0; i < 3; i++) {

		tmp = random_schick(20);

#if !defined(__BORLANDC__)
		D1_INFO(" W20 = %d;", tmp);
#endif

		if (tmp == 20) {
			if (++nr_rolls_20 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> UNGLUECKLICH nicht bestanden\n");
#endif
				return -99;
			}
		}

		if (tmp == 1) {
			if (++nr_rolls_1 == 2) {
#if !defined(__BORLANDC__)
				D1_INFO(" -> GLUECKLICH bestanden\n");
#endif
				return 99;
			}
		}

		if (!fail) {
			tmp -= attrib[i];
			if (handicap <= 0) {
				if (tmp > 0) {
					if (tmp > -handicap) {
						fail = 1;
#if !defined(__BORLANDC__)
						D1_INFO(" zu hoch!");
#endif
					} else  {
						handicap += tmp;
					}
				}
			}
			if (handicap > 0) {
				if (tmp > 0) {
					fail = 1;
#if !defined(__BORLANDC__)
					D1_INFO(" zu hoch!");
#endif
				} else {
					handicap += tmp;
					if (handicap < 0) {
						handicap = 0;
					}
				}
			}
		}
	}
	if (fail || (handicap > 0)) {
#if !defined(__BORLANDC__)
		D1_INFO(" -> nicht bestanden\n");
#endif
		return 0;
	} else {
#if !defined(__BORLANDC__)
		D1_INFO(" -> bestanden mit %d.\n",-handicap);
#endif
		return 1 - handicap;
	}
#endif
}

signed short unused_cruft(void)
{

	signed short l_si;

	if (!gs_total_hero_counter) {
		return -1;
	}

	do {
		l_si = random_schick(6) - 1;

	} while (!(host_readbs(get_hero(l_si) + HERO_TYPE)) || (host_readbs(get_hero(l_si) + HERO_GROUP_NO) != gs_current_group));

	return l_si;
}

/**
 * \brief   selects a hero randomly
 *
 * \return              position of a randomly choosen hero
 */
/* Original-Bug: can loop forever if the position is greater than the
	number of heroes in the group */
signed short get_random_hero(void)
{
	signed short cur_hero;

	do {
		/* get number of current group */
		cur_hero = random_schick(gs_group_member_counts[gs_current_group]) - 1;

#ifdef M302de_ORIGINAL_BUGFIX
		signed short pos = 0;

		Bit8u *hero = get_hero(0);
		for (int i = 0; i <= 6; i++, hero += SIZEOF_HERO) {

			if (host_readbs(hero + HERO_TYPE) == HERO_TYPE_NONE)
				continue;
			/* Check if in current group */
			if (host_readbs(hero + HERO_GROUP_NO) != gs_current_group)
				continue;

			if (pos == cur_hero) {
				pos = i;
				break;
			}

			pos++;
		}
		cur_hero = pos;
#endif

	} while (
		!host_readbs(get_hero(cur_hero) + HERO_TYPE) ||
		(host_readbs(get_hero(cur_hero) + HERO_GROUP_NO) != gs_current_group) ||
		hero_dead(get_hero(cur_hero))
	);

	return cur_hero;
}

/**
 * \brief   get the money of the current group
 *
 * \return              the sum of the money of all heroes in the current group
 */
Bit32s get_party_money(void)
{
	signed short i;
	Bit32s sum = 0;
	Bit8u* hero;

	hero = get_hero(0);

	for (i=0; i < 6; i++, hero += SIZEOF_HERO) {
		if (host_readbs(hero + HERO_TYPE) &&
			(host_readbs(hero + HERO_GROUP_NO) == gs_current_group))
		{
			sum += host_readds(hero + HERO_MONEY);
		}
	}

	return sum;
}

/**
 * \brief   shares money between current party members
 *
 * \param   money       the money to share
 *
 *	If only a NPC is in that party, he gets all the money.
 *	If a hero is dead and in the current party, his money is set to 0.
 */
void set_party_money(Bit32s money)
{
	signed short heroes = 0;
	signed short i;
	Bit32s hero_money;
	Bit8u *hero;

	if (money < 0)
		money = 0;

	heroes = count_heroes_in_group();

	/* set hero to NPC */
	hero = get_hero(6);

	/* if we have an NPC in current group and alive */
	if (
		host_readbs(hero + HERO_TYPE) &&
		(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
		!hero_dead(hero)
	) {

		/* If only the NPC is in that group give him all the money */
		if (heroes > 1) {
			heroes--;
		} else {
			add_ptr_ds(hero + HERO_MONEY, money);
			heroes = 0;
		}
	}

	if (heroes != 0) {

		hero_money = money / heroes;

		hero = get_hero(0);

		for (i = 0; i < 6; i++, hero += SIZEOF_HERO) {

			if (
				host_readbs(hero + HERO_TYPE) &&
				(host_readbs(hero + HERO_GROUP_NO) == gs_current_group) &&
				!hero_dead(hero)
			) {
				/* account the money to hero */
				host_writed(hero + HERO_MONEY, hero_money);
			} else {
				if (host_readbs(hero + HERO_GROUP_NO) == gs_current_group) {
					host_writed(hero + HERO_MONEY, 0);
				}
			}
		}
	}
}

/**
 * \brief   adds money to the current group
 *
 * \param   money       money to add
 */
void add_party_money(Bit32s money)
{
	set_party_money(get_party_money() + money);
}

/**
 * \brief   add AP to a hero
 *
 * \param   hero        pointer to the hero
 * \param   ap          AP the hero should get
 */
void add_hero_ap(Bit8u *hero, Bit32s ap)
{
	add_ptr_ds(hero + HERO_AP, ap);
}

/**
 * \brief   shares AP in the current group
 *
 * \param   ap          AP to share
 */
void add_group_ap(Bit32s ap)
{
	signed short i;
	Bit8u *hero_i;

	if (ap < 0) {
		return;
	}

	ap = ap / count_heroes_in_group();

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group) &&
			!hero_dead(hero_i))
		{
			add_hero_ap(hero_i, ap);
		}
	}
}

/**
 * \brief   add AP to every non-dead hero in the group
 *
 * \param   ap          AP to add
 */
void add_hero_ap_all(signed short ap)
{
	Bit8u *hero_i;
	signed short i;

	if (ap < 0)
		return;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group) &&
			!hero_dead(hero_i))
		{
#if !defined(__BORLANDC__)
			D1_INFO("%s erhaelt %d AP\n",(char*)(hero_i + HERO_NAME2), ap);
#endif

			add_hero_ap(hero_i, ap);
		}
	}
}

/**
 * \brief   subtracts AP from every non-dead hero in the group
 *
 * \param   ap          AP to subtract
 */
void sub_hero_ap_all(signed short ap)
{
	signed short i;
	Bit8u *hero_i;

	if (ap < 0)
		return;

	hero_i = get_hero(0);
	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group) &&
			!hero_dead(hero_i))
		{
			if ((Bit32u)ap <= host_readd(hero_i + HERO_AP)) {
#if !defined(__BORLANDC__)
				D1_INFO("%s erhaelt %+d AP\n",(char*)(hero_i + HERO_NAME2), -ap);
#endif
				add_hero_ap(hero_i, -((Bit32s)ap));
			} else {
#if !defined(__BORLANDC__)
				D1_INFO("%s wird auf 0 AP gesetzt\n",(char*)(hero_i + HERO_NAME2));
#endif
				host_writed(hero_i + HERO_AP, 0);
			}
		}
	}
}

/**
 * \brief   gets the position of a hero
 *
 * \param   hero        pointer to the hero
 * \return              position of the hero
 */
signed short get_hero_index(Bit8u *hero)
{
	signed short i = 0;
	Bit8u *p;

	p = get_hero(i);
	while (hero != p) {
		i++;
		p = get_hero(i);
	}

	return i;
}

/**
 * \brief   gets item position
 *
 * \param   hero        pointer to the hero
 * \param   item        item ID to look for
 * \return              position of the item or -1 if the item is not in the inventory.
 */
signed short get_item_pos(Bit8u *hero, signed short item)
{

	signed short i;

	for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++) {
		if (item == host_readws(hero + i * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_ITEM_ID)) {
			return i;
		}
	}

	return -1;
}

/**
 * \brief   gets the position of the first hero with an item
 *
 * \param   item        item ID to look for
 * \return              position of the hero or -1 if nobody of the group has this item
 */
signed short get_first_hero_with_item(signed short item)
{
	signed short j;
	signed short i;
	Bit8u *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group))
		{
			/* Search inventory */
			for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {
				if (host_readw(hero_i + j * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_ITEM_ID) == item) {
					return i;
				}
			}
		}
	}

	return -1;
}

/**
 * \brief   gets the position of the first hero with an item in a specified group
 *
 * \param   item        item ID to look for
 * \param   group       group number
 * \return              position of the hero or -1 if nobody in the specified group has this item
 */
signed short get_first_hero_with_item_in_group(signed short item, signed short group)
{
	signed short j;
	signed short i;
	Bit8u *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == (signed char)group))
		{
			/* Search inventory */
			for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {
				if (host_readws(hero_i + j * SIZEOF_INVENTORY + HERO_INVENTORY + INVENTORY_ITEM_ID) == item) {
					return i;
				}
			}
		}
	}

	return -1;
}


/**
 * \brief   subtracts LE from every group member
 *
 * \param   le          LE to subtract
 */
void sub_group_le(signed short le)
{
	signed short i;
	Bit8u *hero_i;

	for (i = 0; i <= 6; i++) {

		hero_i = get_hero(i);

		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group))
		{
			sub_hero_le(hero_i, le);
		}
	}
}

/**
 * \brief   return a pointer to the first available hero
 *
 * \return              a pointer to the first available hero. If none in available it returns a pointer to the first hero.
 */
Bit8u* get_first_hero_available_in_group(void)
{
	signed short i;
	unsigned char *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {

		/* Check class, group, deadness and check_hero() */
		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group) &&
			!hero_dead(hero_i) &&
			check_hero(hero_i))
		{
			return hero_i;
		}
	}

	return (Bit8u*)get_hero(0);
}

/**
 * \brief   return a pointer to the second available hero in the group
 *
 * \return              a pointer to the second available hero in the group or NULL.
 */
Bit8u* get_second_hero_available_in_group(void)
{
	signed short i;
	signed short tmp;
	unsigned char *hero_i;

	hero_i = get_hero(0);

	for (i = tmp = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		/* Check class, group and check_hero() */
		if (host_readbs(hero_i + HERO_TYPE) &&
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group) &&
			check_hero(hero_i))
		{
			if (tmp) {
				return hero_i;
			}

			tmp++;
		}
	}

	return NULL;
}

/**
 * \brief   count available heroes
 *
 * \return              number of available heroes in all groups, including NPC
 */
signed short count_heroes_available(void)
{
	signed short i;
	signed short retval;
	Bit8u *hero_i;

	retval = 0;
	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		/* Check if hero is available */
		if (host_readbs(hero_i + HERO_TYPE) &&
			(check_hero(hero_i) || check_hero_no2(hero_i)))
		{
			retval++;
		}
	}

	return retval;
}

#ifdef M302de_ORIGINAL_BUGFIX
/* this function allows a cleaner fix for Original-Bug 15 */
signed short count_heroes_available_ignore_npc(void)
{
	signed short i;
	signed short retval;
	Bit8u *hero_i;

	retval = 0;
	hero_i = get_hero(0);

	for (i = 0; i < 6; i++, hero_i += SIZEOF_HERO) {
		/* Check if hero is available */
		if (host_readbs(hero_i + HERO_TYPE) &&
			(check_hero(hero_i) || check_hero_no2(hero_i)))
		{
			retval++;
		}
	}

	return retval;
}
#endif

/**
 * \brief   count available (= not dead, petrified, unconscious, renegade) heroes in current group, including NPC
 *
 * \return   number of available (= not dead, petrified, unconscious, renegade) heroes in current group, including NPC
 */
signed short count_heroes_available_in_group(void)
{
	signed short heroes = 0;
	signed short i;
	Bit8u *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i += SIZEOF_HERO) {
		if (host_readbs(hero_i + HERO_TYPE) && /* != HERO_TYPE_NONE */
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group) && /* hero in current group */
			check_hero_no2(hero_i)) /* hero not dead, petrified, unconscious or renegade */
		{
			heroes++;
		}
	}

	return heroes;
}

#ifdef M302de_ORIGINAL_BUGFIX
/* this function allows cleaner fixes for Original-Bug 12, 13, 14 and 15 */
signed short count_heroes_available_in_group_ignore_npc(void)
{
	signed short heroes = 0;
	signed short i;
	Bit8u *hero_i = get_hero(0);

	for (i = 0; i < 6; i++, hero_i += SIZEOF_HERO) {
		if (host_readbs(hero_i + HERO_TYPE) && /* != HERO_TYPE_NONE */
			(host_readbs(hero_i + HERO_GROUP_NO) == gs_current_group) && /* hero in current group */
			check_hero_no2(hero_i)) /* hero not dead, petrified, unconscious or renegade */
		{
			heroes++;
		}
	}

	return heroes;
}
#endif

void check_group(void)
/* called from only a single position, namely the petrification trap in 'Verfallene Herberge' in DNG02_handler in seg078.cpp */
{
	/* Original-Bug 15:
	 * If the group steps into the petrification trap in "Verfallene Herberge" and all but the NPC get petrified
	 * (for example, Curian (MR 6) equipped with the red ring (MR +2) of Gorah's chest), the group is still active
	 * with the NPC as the only active member (which is impossible in other circumstances). If this is the only group,
	 * a click on "switch group" results in an infinite loop with the window "In dieser Gruppe ist niemand in der Lage etwas zu tun!". */

	if
#ifndef M302de_ORIGINAL_BUGFIX
		(!count_heroes_available())
#else
		(!count_heroes_available_ignore_npc())
#endif
	{
		/* game over */
		ds_writew(GAME_STATE, GAME_STATE_DEAD);

	} else if
#ifndef M302de_ORIGINAL_BUGFIX
		(!count_heroes_available_in_group())
#else
		(!count_heroes_available_in_group_ignore_npc())
#endif
	{

		GRP_switch_to_next(2);

	}
}

int schick_main(int argc, char** argv)
{
	signed short l_si;
	signed short l_di;
	Bit32s l3;
	signed short savegame;
	signed short len;

	ds_writew(PREGAME_STATE, 1);
	g_playmask_us = 1;

	init_AIL(16000);

#if defined(__BORLANDC__)
	randomize();
#endif

	save_display_stat(((Bit8u*)p_datseg + VIDEO_PAGE_BAK));

	if (!init_memory()) {

		ds_writew(GAMEINIT_FLAG, 1);

		schick_set_video();

		ds_writew(HAVE_MOUSE, 2);

		mouse_init();

		if (ds_readw(HAVE_MOUSE) == 0) {
			ds_writew(MOUSE_REFRESH_FLAG, -10);
		}

		init_game_state();

		save_and_set_timer();

		init_common_buffers();

		g_textbox_width = 3;

		refresh_screen_size();

		if (argc == 2) {

			/* some trick to disable the cd check */

			len = strlen(argv[1]);

			l_si = 0;
			g_cd_skipmagic = 1;

			while (l_si < len) {


				g_cd_skipmagic = argv[1][0] * g_cd_skipmagic;
				argv[1]++;
				l_si++;
			}
		}

		prepare_dirs();

		if (have_mem_for_sound()) {

			read_sound_cfg();
			alloc_voc_buffer(20000);

		} else {
			/* disable sound */
			exit_AIL();
			GUI_output((char*)p_datseg + SND_TXT_DISABLED_MEM2);
		}

		CD_init();

		if (!g_cd_init_successful) {

			/* CD init failed */
			cleanup_game();
			exit(0);
		}


		/* select game mode */
		ds_writew(GAME_MODE, GAME_MODE_UNSPECIFIED);

		while (ds_readws(GAME_MODE) == GAME_MODE_UNSPECIFIED) {
			ds_writew(GAME_MODE, GUI_radio(get_ttx(5), 2, get_ttx(6), get_ttx(7)));
		}

		if (copy_protection()) {

			g_textbox_width = 3;

			l3 = get_diskspace();

			if (l3 < 0) {

				sprintf((char*)g_dtp2, get_ttx(807), -l3);
				GUI_output((char*)g_dtp2);
				cleanup_game();

			} else {

				/* ask for generation or game */
				do {
					l_di = GUI_radio(get_ttx(820), 2, get_ttx(821), get_ttx(822)) - 1;

				} while (l_di == -1);

				if (l_di == 1) {
					call_gen();
				}

				wait_for_keyboard2();


				/* load a savegame */
				do {
					savegame = load_game_state();
				} while (savegame == -1);

				ds_writew(PREGAME_STATE, 0);

				/* start the game */
				game_loop();

				cleanup_game();
			}
		} else {
			cleanup_game();
		}

	} else {
		/* not enough memory */
		exit_AIL();
		schick_reset_video();
#if defined(__BORLANDC__)
		clrscr();
#endif
	}

#if !defined(__BORLANDC__)
	return 0;
#endif
}

Bit8u* schick_alloc(Bit32u size)
{
#if defined(__BORLANDC__)
	return (Bit8u*)farcalloc(size, 1);
#else
	return (Bit8u*)calloc(size, 1);
#endif
}

signed short copy_protection(void)
{
	signed short i;
	signed short randval;
	signed short tries;
	signed short len;

#ifndef M302de_FEATURE_MOD
	load_tx(ARCHIVE_FILE_FIGHTTXT_LTX);

	g_textbox_width = 4;

	set_textcolor(0xff, 0);

	tries = 0;

	while (tries < 3) {

		if (random_schick(100) <= 50) {
			/* handbook question */

			/* select a question */
			randval = random_schick(10) - 1;

			/* prepare the string */
			sprintf((char*)g_dtp2, get_tx(39),
				ds_readbs((QUESTIONS_HANDBOOK + 3) + 19 * randval),
				ds_readbs((QUESTIONS_HANDBOOK + 2) + 19 * randval),
				ds_readbs((QUESTIONS_HANDBOOK + 1) + 19 * randval),
				ds_readbs((QUESTIONS_HANDBOOK + 0) + 19 * randval));

			/* print version number */
			GUI_print_string(g_game_version, 290, 190);

			/* ask the question */
			GUI_input((char*)g_dtp2, 20);

			len = strlen(g_text_input_buf);

			/* transform the input string in uppercase letters and bitwise invert them */
			for (i = 0; i < len; i++) {
				g_text_input_buf[i] = ~toupper(g_text_input_buf[i]);
			}

			if (!strcmp((char*)(p_datseg + (QUESTIONS_HANDBOOK + 4)) + 19 * randval, g_text_input_buf)) {
				return 1;
			}
		} else {
			/* map question */

			/* select a question */
			randval = random_schick(10) - 1;

			/* prepare the string */
			sprintf((char*)g_dtp2, get_tx(40),
				get_tx(41 + ds_readbs((QUESTIONS_MAP + 0) + 3 * randval)),
				get_ttx(235 + ds_readbs((QUESTIONS_MAP + 1) + 3 * randval)));

			/* print version number */
			GUI_print_string(g_game_version, 290, 190);

			/* ask the question */
			GUI_input((char*)g_dtp2, 20);

			len = strlen(g_text_input_buf);

			/* transform the input string in uppercase letters */
			for (i = 0; i < len; i++) {
				g_text_input_buf[i] = toupper(g_text_input_buf[i]);
			}

			if (!strcmp(get_ttx(235 + ds_readbs((QUESTIONS_MAP + 2) + 3 * randval)), g_text_input_buf)) {
				return 1;
			}
		}

		tries++;
	}

	return 0;
#else
	/* Feature mod 5: disable copy protection */
	return 1;
#endif
}

#if !defined(__BORLANDC__)
}
#endif
