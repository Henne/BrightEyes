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

#if defined(__BORLANDC__)
/* all global variables are included here, since BCC.EXE would create another module */
#include "datseg.cpp"
#endif

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

/* BSS Variables */
unsigned char dummy;
unsigned short g_sample_ad_length;		// ds:0xbc5a
struct sample_idx g_sample_ad_idx_entry;	// ds:0xbc5c
unsigned char g_playmask_us;			// ds:0xbc62, 1 = PLAYM_US, 0 = PLAYM_UK
unsigned char g_gfxbuf_wait_keypress[100];	// ds:0xbc63
unsigned char *g_splash_ae;			// ds:0xbcc7
unsigned char *g_splash_le;			// ds:0xbccb
Bit8u g_hero_splash_timer[7];			// ds:0xbccf
signed short g_spinlock_flag;			// ds:0xbcd6
signed short g_map_townmark_state;		// ds:0xbcd8
signed char g_freeze_timers;			// ds:0xbcda

#if defined(__BORLANDC__)
void interrupt far (*g_mouse_handler_bak)(...); // ds:0xbcdb; seg002
#endif

static Bit32s g_archive_file_offset;	// ds:0xbcdf, start offset in SCHICK.DAT
static Bit32s g_archive_file_remaining; // ds:0xbce3, flen - off
static Bit32s g_archive_file_length;	// ds:0xbce7

#if defined(__BORLANDC__)
static Bit8u *g_ail_digi_driver_buf2;		// ds:0xbceb, to buffer of size 5016
static Bit8u *g_ail_voc_buffer;		// ds:0xbcef
static Bit8u *g_ail_digi_driver_buf;		// ds:0xbcf3
static Bit16s *g_ail_digi_driver_descr;		// ds:0xbcf7
static signed short g_ail_digi_driver_id;	// ds:0xbcfb
#endif

signed short g_use_cdaudio_flag; // ds:0xbcfd

#if defined(__BORLANDC__)
static signed short g_load_sound_driver;	// ds:0xbcff
static signed short g_sample_ad_handle;	// ds:0xbd01
static unsigned short g_ail_timbre_cache_size; // ds:0xbd03
static signed long g_ail_state_table_size;	// ds:0xbd05
static Bit8u *g_ail_music_driver_buf2;		// ds:0xbd09
static Bit8u *g_ail_midi_buffer;		// ds:0xbd0d
static Bit8u *g_ail_timbre_cache;		// ds:0xbd11
static Bit8u *g_ail_state_table;		// ds:0xbd15
static Bit8u *g_ail_music_driver_buf;		// ds:0xbd19
static Bit16s *g_ail_music_driver_descr;	// ds:0xbd1d
static signed short g_ail_sequence;		// ds:0xbd21
static signed short g_ail_music_driver_id;	// ds:0xbd23
#endif

signed short g_pregame_state;		// ds:0xbd25
signed char g_citycamp_city;		// ds:0xbd27, {0 = camp in dungeon, 1}
struct fight *g_current_fight;		// ds:0xbd28
signed char *g_scenario_buf;		// ds:0xbd2c
unsigned char *g_fightobj_buf;		// ds:0xbd30
struct struct_hero *g_heroes;		// ds:0xbd34
signed char g_new_menu_icons[9];	// ds:0xbd38
unsigned char g_unkn_073[12];		// ds:0xbd41
unsigned char g_steptarget_front;	// ds:0xbd4d
unsigned char g_steptarget_back;	// ds:0xbd4e
signed char g_direction_unkn;		// ds:0xbd4f, writeonly (1)
signed char g_visual_fields_tex[29];	// ds:0xbd50
unsigned char g_unkn_074[1];		// ds:0xbd6d
unsigned char g_visual_field_vals[29];	// ds:0xbd6e
unsigned char g_unkn_075[1];		// ds:0xbd8b
#if defined(__BORLANDC__)
struct ems_tab *g_ems_unused_tab;	// ds:0xbd8c
#endif
signed short g_ems_travelmap_handle;	// ds:0xbd90
#if defined(__BORLANDC__)
signed short g_ems_unused_handle;	// ds:0xbd92
#endif
unsigned char g_dng_map_size;		// ds:0xbd94
unsigned char g_dng_map[512];		// ds:0xbd95
char *g_radio_name_list[25];		// ds:0xbf95, used for items, heroes, spells, skills, recipes
unsigned char *g_gui_buffer_unkn;	// ds:0xbff9
signed short g_textbox_width;		// ds:0xbffd
signed short g_textbox_pos_x;		// ds:0xbfff, coordinate of upper left corner

signed short g_textbox_pos_y; // ds:0xc001, coordinate of upper left corner
signed short g_game_mode; // ds:0xc003, {-1 = Input error, 1 = Beginner, 2 = Advanced }
struct shop_item *g_sellitems; // ds:0xc005, also used for repair items
struct shop_item *g_buyitems; // ds:0xc009, merchant's assortment
struct struct_pic_copy g_pic_copy;	// ds:0xc00d
struct location g_locations_tab[150]; // ds:0xc025
unsigned char *g_buffer8_ptr; // ds:0xc3a9, to buffer of size 12008
char **g_tx2_index; // ds:0xc3ad, to index table of secondary text file
char **g_tx_index; // ds:0xc3b1, to index table of primary text file
char **g_text_ltx_index; // ds:0xc3b5, to index table of TEXT.LTX
unsigned char g_unkn_077[6]; // ds:0xc3b9
signed short g_random_schick_seed2; // ds:0xc3bf
signed short g_game_state; // ds:0xc3c1, see enum GAME_STATE_*
unsigned char g_unkn_078[2]; // ds:0xc3c3
signed short g_bioskey_event10; // ds:0xc3c5
signed short g_have_mouse; // ds:0xc3c7
signed short g_unused_spinlock_flag; // ds:0xc3c9
signed short g_update_statusline; // ds:0xc3cb
unsigned char g_unkn_079[2]; // ds:0xc3cd
signed short g_mouse1_doubleclick; // ds:0xc3cf
signed short g_mouse1_event1; // ds:0xc3d1
signed short g_mouse2_event; // ds:0xc3d3
signed short g_mouse1_event2; // ds:0xc3d5
signed short g_bioskey_event; // ds:0xc3d7
signed short g_action; // ds:0xc3d9
HugePt g_buffer9_ptr; // ds:0xc3db, to buffer of size 180000 (or 203000 if LARGE_BUF), used for NVF
unsigned char g_unkn_080[8]; // ds:0xc3df
signed short g_ani_width; // ds:0xc3e7
unsigned short g_ani_unknown1; // ds:0xc3e9
unsigned short g_ani_unknown2; // ds:0xc3eb
unsigned char g_ani_height; // ds:0xc3ed
signed char g_ani_areacount; // ds:0xc3ee
struct ani_area g_ani_area_table[10]; // ds:0xc3ef
unsigned char *g_ani_main_ptr; // ds:0xce35
unsigned char g_ani_compr_flag; // ds:0xce39, {0,1 = compressed}
unsigned char g_ani_palette_size; // ds:0xce3a
HugePt g_ani_palette; // ds:0xce3b
signed int g_ani_posy; // ds:0xce3f
signed int g_ani_posx; // ds:0xce41
Bit32s g_ani_unknown4; // ds:0xce43, writeonly (0)
unsigned char g_unkn_081[64]; // ds:0xce47
unsigned char g_gui_text_buffer[64]; // ds:0xce87
static struct mouse_cursor *g_last_cursor; // ds:0xcec7
struct mouse_cursor *g_current_cursor; // ds:0xcecb
struct mouse_cursor g_ggst_cursor; // ds:0xcecf
unsigned char g_mouse_bg_bak[256]; // ds:0xcf0f

signed short g_dng_init_flag;		// ds:0xd00f
signed short g_dng_extra_action;	// ds:0xd011, {0 = warehouse,1 = open door,2 = open chest,3 = close door,4 = lever,5 = smash door}
signed short g_redraw_menuicons;	// ds:0xd013
unsigned char *g_buffer9_ptr2;		// ds:0xd015, copy of BUFFER9_PTR
HugePt g_buffer9_ptr3;			// ds:0xd019, copy of BUFFER9_PTR
Bit32u g_buffer_monster_tab[36];	// ds:0xd01d
Bit32u g_buffer_wfigs_tab[43];		// ds:0xd0ad
Bit32u g_buffer_mfigs_tab[43];		// ds:0xd159
Bit32s g_buffer_anis_tab[37];		// ds:0xd205

unsigned char *g_trv_track_pixel_bak; // ds:0xd299, to buffer of size 500
unsigned char *g_fig_star_gfx; // ds:0xd29d
char *g_monnames_buffer; // ds:0xd2a1
char *g_buffer5_ptr; // ds:0xd2a5, to buffer of size 3880
unsigned char *g_buffer10_ptr; // ds:0xd2a9, to buffer of size 16771, used for NVF and text
unsigned char *g_popup; // ds:0xd2ad
unsigned char *g_buffer6_ptr; // ds:0xd2b1, to buffer of size 2200, used for NVF
char *g_buffer7_ptr; // ds:0xd2b5, to buffer of size 10000, used for NVF and text
char *g_text_ltx_buffer; // ds:0xd2b9, to buffer of size 30500
unsigned char *g_splash_buffer; // ds:0xd2bd, to buffer of size 1000, used for Splashes
unsigned char *g_buf_font6; // ds:0xd2c1
signed short g_textcolor_index; // ds:0xd2c5
signed short g_textcolor_bg; // ds:0xd2c7
signed short g_textcolor_fg[4]; // ds:0xd2c9
signed short g_gui_text_centered; // ds:0xd2d1
signed short g_textline_unknown; // ds:0xd2d3, writeonly (103)
signed short g_textline_maxlen; // ds:0xd2d5
signed short g_textline_posy; // ds:0xd2d7
signed short g_textline_posx; // ds:0xd2d9
unsigned char *g_fig_figure2_buf; // ds:0xd2db, to buffer of size 20000
unsigned char *g_fig_figure1_buf; // ds:0xd2df
unsigned char *g_objects_nvf_buf; // ds:0xd2e3, to buffer of size 3400
unsigned char *g_buf_icon; // ds:0xd2e7
char *g_text_output_buf; // ds:0xd2eb, to buffer of size 300
char *g_text_input_buf; // ds:0xd2ef, to buffer of size 24
char *g_dtp2; // ds:0xd2f3
unsigned char *g_icon; // ds:0xd2f7
unsigned char *g_vga_backbuffer; // ds:0xd2fb
unsigned char *g_vga_memstart; // ds:0xd2ff
unsigned char *g_renderbuf_ptr; // ds:0xd303, to buffer of size 65000


#if defined(__BORLANDC__)
static unsigned char g_unkn_082[2]; // ds:0xd307
static signed short g_gameinit_flag; // ds:0xd309, writeonly (1)
signed short g_video_page_bak; // ds:0xd30b
signed short g_video_mode_bak; // ds:0xd30d
static unsigned char g_unkn_083[4]; // ds:0xd30f
#endif

signed short g_txt_tabpos[7]; // ds:0xd313
unsigned char *g_townpal_buf; // ds:0xd321
signed short g_fig_flee_position[4]; // ds:0xd325, see HERO_ESCAPE_POSITION
signed short g_wildcamp_sleep_quality; // ds:0xd32d
signed short g_gather_herbs_mod; // ds:0xd32f
signed short g_replenish_stocks_mod; // ds:0xd331
struct fight_msg g_fig_msg_data[6]; // ds:0xd333
struct enemy_sheet g_enemy_sheets[20]; // ds:0xd34b
signed char g_fig_move_pathdir[10]; // ds:0xd823 /* TODO: 10 steps is to short */
signed char g_fig_actors_unkn[30]; // ds:0xd82d, see FIG_ACTION_UNKNOWN2
signed char g_hero_is_target[7]; // ds:0xd84b
signed char *g_chessboard;// ds:0xd852
Bit8u *g_fig_spellgfx_buf; // ds:0xd856
Bit8u *g_fig_shot_bolt_buf; // ds:0xd85a
Bit8u *g_fig_cb_selector_buf; // ds:0xd85e
Bit8u *g_fig_cb_marker_buf; // ds:0xd862
Bit8u *g_spellobj_nvf_buf; // ds:0xd866, to buffer of size 0xf5f
Bit8u *g_weapons_nvf_buf; // ds:0xd86a, to buffer of size 0x1953
unsigned char *g_fightobj_buf_seek_ptr; // ds:0xd86e, points to end of FIGHTOBJ buffer
signed int g_nr_of_enemies; // ds:0xd872, ?
signed char g_fightobj_list[90]; // ds:0xd874
Bit8s g_fig_anisheets[8][243]; // ds:0xd8ce

struct struct_fighter g_fig_list_elem; // ds:0xe066
signed char g_fig_list_array[127]; // ds:0xe089
struct struct_fighter *g_fig_list_head; // ds:0xe108, to a list
signed char g_location_market_flag; // ds:0xe10c
signed short g_wallclock_redraw; // ds:0xe10d
signed short g_wallclock_y; // ds:0xe10f
signed short g_wallclock_x; // ds:0xe111
signed short g_wallclock_update; // ds:0xe113, 0 = don't update the wallclock
struct struct_memslot_fig *g_memslots_mon; // ds:0xe115
struct struct_memslot_fig *g_memslots_wfig; // ds:0xe119
struct struct_memslot_fig *g_memslots_mfig; // ds:0xe11d
struct struct_memslot_ani *g_memslots_anis; // ds:0xe121, to ()[36]
struct struct_monster *g_monster_dat_buf; // ds:0xe125
char **g_monnames_index; // ds:0xe129
signed char g_market_itemsaldo_table[254]; // ds:0xe12d
struct item_stats *g_itemsdat; // ds:0xe22b
char **g_itemsname; // ds:0xe22f


#if !defined(__BORLANDC__)
static inline Bit8u readb(Bit8u *p)
{
	return ((Bit8u)*p);
}

static inline Bit16s readws(Bit8u *p)
{
	return ((Bit16s)(readb(p + 1) << 8) | (readb(p)));
}

static inline Bit32s readds(Bit8u *p)
{
	return ((Bit32s)(readws(p + 2) << 16) | readws(p));
}

#else

#define readb(p) (*(Bit8u*)(p))
#define readws(p) (*(Bit16u*)(p))
#define readds(p) (*(Bit32u*)(p))

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

		if (g_use_cdaudio_flag) {
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

	answer = GUI_radio(g_snd_menu_title, 4,
				g_snd_menu_radio1, g_snd_menu_radio2,
				g_snd_menu_radio3, g_snd_menu_radio4);

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
		if (g_use_cdaudio_flag) {
			CD_audio_pause();
		} else {
			stop_midi_playback();
		}
	} else {
		if (g_music_current_track != -1) {
			/* music enabled */
			if (g_use_cdaudio_flag) {
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
	signed int handle;

	/* try to open SOUND.CFG */
	if ( (handle = open(g_fname_sound_cfg, O_BINARY | O_RDONLY)) != -1) {

		_read(handle, (Bit8u*)&midi_port, 2);
		_read(handle, (Bit8u*)&dummy, 2);
		_read(handle, (Bit8u*)&digi_port, 2);
		_read(handle, (Bit8u*)&digi_irq, 2);
		_close(handle);

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
			g_use_cdaudio_flag = 1;
			g_load_sound_driver = 1;

		} else if (answer == 2)
		{
			/* MIDI selected */
			g_use_cdaudio_flag = 0;
			g_load_sound_driver = 0;
		}

		if (g_use_cdaudio_flag == 0)
#else
		/* enable usage of audio-CD */
		g_use_cdaudio_flag = g_load_sound_driver = 1;

		/* disable loading of the music driver */
		if (0)
#endif
		{
			if (midi_port) {
				load_music_driver(g_fname_sound_adv2, 3, midi_port);
			} else {

				/* music was disabled in SOUND.CFG */
				if (g_ail_midi_buffer) {
					free((void*)g_ail_midi_buffer);
				}

				g_ail_midi_buffer = NULL;
			}
		}

		if (digi_port) {

			if (g_snd_voc_enabled) {

				if (!load_digi_driver(g_fname_digi_adv, 2, digi_port, digi_irq))
				{
					g_snd_voc_enabled = 0;
				}
			} else {
				/* print that sound effects are disabled */
				GUI_output(g_snd_txt_disabled_mem);
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
	if ((g_ail_midi_buffer = (Bit8u*)schick_alloc(size))) {
		AIL_startup();
		g_load_sound_driver = 1;
	}
#endif
}

void exit_AIL(void)
{
#if defined(__BORLANDC__)
	AIL_shutdown((Bit8u*)NULL);

	if (g_ail_timbre_cache) {
		free((void*)g_ail_timbre_cache);
	}

	if (g_ail_state_table) {
		free((void*)g_ail_state_table);
	}

	if (g_ail_midi_buffer) {
		free((void*)g_ail_midi_buffer);
	}

	if (g_ail_music_driver_buf2) {
		free((void*)g_ail_music_driver_buf2);
	}

	/* set all pointers to NULL */
	g_ail_timbre_cache = g_ail_state_table = g_ail_midi_buffer = g_ail_music_driver_buf2 = NULL;

	if (g_snd_voc_enabled) {
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

	signed int handle;

	if ( (handle = open((char*)fname, O_BINARY | O_RDONLY)) != -1) {

		len = 16500L;

		g_ail_music_driver_buf2 = (Bit8u*)schick_alloc(len + 16L);
		/* insane pointer casting */
		ptr = (Bit32u)g_ail_music_driver_buf2 + 15L;
		ptr &= 0xfffffff0;
		buf = EMS_norm_ptr((Bit8u*)ptr);
		_read(handle, (Bit8u*)buf, (unsigned short)len);
		_close(handle);
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

	if ((g_sample_ad_handle = load_archive_file(ARCHIVE_FILE_SAMPLE_AD)) != -1) {

		if ((g_ail_sequence = AIL_register_sequence(g_ail_music_driver_id, g_ail_midi_buffer, sequence, g_ail_state_table, 0)) != -1) {

			while ( (l_si = AIL_timbre_request(g_ail_music_driver_id, g_ail_sequence)) != (unsigned short)-1)
			{
				l_di = l_si >> 8;
				patch = l_si & 0xff;

				if ( (ptr = prepare_timbre(l_di, patch))) {
					AIL_install_timbre(g_ail_music_driver_id, l_di, patch, ptr);
					free(ptr);
				}
			}

			close(g_sample_ad_handle);
			return 1;
		}

		close(g_sample_ad_handle);
	}
#endif
	return 0;
}

/* static */
signed short start_midi_playback(signed short seq)
{
#if defined(__BORLANDC__)
	if (prepare_midi_playback(seq)) {
		AIL_start_sequence(g_ail_music_driver_id, seq);
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

	seek_archive_file(g_sample_ad_handle, 0, 0);

	do {
		read_archive_file(g_sample_ad_handle, (Bit8u*)&g_sample_ad_idx_entry, 6);

		if (g_sample_ad_idx_entry.bank == -1) {
			return NULL;
		}

	} while ((g_sample_ad_idx_entry.bank != a1) || (g_sample_ad_idx_entry.patch != patch));

	seek_archive_file(g_sample_ad_handle, g_sample_ad_idx_entry.offset, 0);

	read_archive_file(g_sample_ad_handle, (Bit8u*)&g_sample_ad_length, 2);

	buf = schick_alloc(g_sample_ad_length);

	*((unsigned short*)buf) = g_sample_ad_length;

	read_archive_file(g_sample_ad_handle, buf + 2, g_sample_ad_length - 2);

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
	signed int handle;

	if ((handle = load_archive_file(index)) != -1) {
		read_archive_file(handle, g_ail_midi_buffer, 0x7fff);
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
	if (port && (g_ail_music_driver_buf = (Bit8u*)read_music_driver(fname)) &&
		((g_ail_music_driver_id = AIL_register_driver(g_ail_music_driver_buf)) != -1))
	{
		g_ail_music_driver_descr = (Bit16s*)AIL_describe_driver(g_ail_music_driver_id);

		if (g_ail_music_driver_descr[1] == type)
		{
			if (port == -1) {
				port = g_ail_music_driver_descr[6];
			}

			if (AIL_detect_device(g_ail_music_driver_id, port,
				g_ail_music_driver_descr[7],
				g_ail_music_driver_descr[8],
				g_ail_music_driver_descr[9]))
			{
				AIL_init_driver(g_ail_music_driver_id, port,
					g_ail_music_driver_descr[7],
					g_ail_music_driver_descr[8],
					g_ail_music_driver_descr[9]);

				if (type == 3) {
					g_ail_state_table_size = AIL_state_table_size(g_ail_music_driver_id);
					g_ail_state_table = (Bit8u*)schick_alloc(g_ail_state_table_size);
					g_ail_timbre_cache_size = AIL_default_timbre_cache_size(g_ail_music_driver_id);

					if (g_ail_timbre_cache_size) {

						g_ail_timbre_cache = (Bit8u*)schick_alloc(g_ail_timbre_cache_size);

						AIL_define_timbre_cache(g_ail_music_driver_id, g_ail_timbre_cache, g_ail_timbre_cache_size);
					}
				}

				g_load_sound_driver = 0;

				return 1;
			} else {

				/* no sound hardware found */
				GUI_output(g_snd_txt_hw_not_found);
				exit_AIL();
			}
		}
	}

	g_load_sound_driver = 1;
#endif
	return 0;
}

/* static */
void do_play_music_file(signed short index)
{
#if defined(__BORLANDC__)
	if (!g_load_sound_driver && (g_ail_music_driver_descr[1] == 3)) {

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
	if (!g_load_sound_driver && (g_ail_music_driver_descr[1] == 3))
	{
		AIL_stop_sequence(g_ail_music_driver_id, g_ail_sequence);
		AIL_release_sequence_handle(g_ail_music_driver_id, g_ail_sequence);
	}
#endif
}

void start_midi_playback_IRQ(void)
{
#if defined(__BORLANDC__)
	if (!g_load_sound_driver && g_music_enabled && (g_ail_music_driver_descr[1] == 3))
	{
		if (AIL_sequence_status(g_ail_music_driver_id, g_ail_sequence) == 2) {
			AIL_start_sequence(g_ail_music_driver_id, g_ail_sequence);
		}
	}
#endif
}

#if defined(__BORLANDC__)
void cruft_1(void)
/* This function is never called */
{
	if (!g_load_sound_driver && (g_ail_music_driver_descr[1] == 3))
	{
		AIL_stop_sequence(g_ail_music_driver_id, g_ail_sequence);
	}
}

void cruft_2(signed short volume)
/* This function is never called */
{
	if (!g_load_sound_driver) {

		if (g_ail_music_driver_descr[1] == 3) {
			AIL_set_relative_volume(g_ail_music_driver_id, g_ail_sequence, volume, 0);
		}

		if (!volume) {
			stop_midi_playback();
		}
	}
}
#endif

signed short have_mem_for_sound(void)
{
#if defined(__BORLANDC__)
	Bit32s size;
	signed short retval;
	struct ffblk blk;

	if (!findfirst(g_fname_sound_adv, &blk, 0)) {

		/* SOUND.ADV was found */
		size = blk.ff_fsize;
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

		while (AIL_VOC_playback_status(g_ail_digi_driver_id) == 2) {
			wait_for_vsync();
		}
	}
#endif
}

void alloc_voc_buffer(Bit32u size)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled) {
		if ((g_ail_voc_buffer = (Bit8u*)schick_alloc(size))) ;
	}
#endif
}

/* static */
void free_voc_buffer(void)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled != 0) {

		if (g_ail_voc_buffer) {
			free((void*)g_ail_voc_buffer);
		}

		if (g_ail_digi_driver_buf2) {
			free((void*)g_ail_digi_driver_buf2);
		}

		g_ail_voc_buffer = g_ail_digi_driver_buf2 = NULL;

	}
#endif
}

signed short read_new_voc_file(signed short index)
{
#if defined(__BORLANDC__)
	if (AIL_VOC_playback_status(g_ail_digi_driver_id) == 2) {
		SND_stop_digi();
	}

	if (read_voc_file(index)) {

		AIL_format_VOC_file(g_ail_digi_driver_id, g_ail_voc_buffer, -1);
		return 1;
	}
#endif
	return 0;
}

signed short read_voc_file(signed short index)
{
#if defined(__BORLANDC__)
	signed int handle;

	if ( (handle = load_archive_file(index)) != -1) {
		read_archive_file(handle, g_ail_voc_buffer, 0x7fff);
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

		AIL_stop_digital_playback(g_ail_digi_driver_id);
		read_new_voc_file(index);
		AIL_play_VOC_file(g_ail_digi_driver_id, g_ail_voc_buffer, -1);
		AIL_start_digital_playback(g_ail_digi_driver_id);
	}
#endif
}

void SND_stop_digi(void)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled) {
		AIL_stop_digital_playback(g_ail_digi_driver_id);
	}
#endif
}

void SND_set_volume(unsigned short volume)
{
#if defined(__BORLANDC__)
	if (g_snd_voc_enabled) {

		AIL_set_digital_playback_volume(g_ail_digi_driver_id, volume);

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
	if (io && (g_ail_digi_driver_buf = (Bit8u*)read_digi_driver(fname)) &&
		((g_ail_digi_driver_id = AIL_register_driver(g_ail_digi_driver_buf)) != -1))
	{

		g_ail_digi_driver_descr = (Bit16s*)AIL_describe_driver(g_ail_digi_driver_id);

		if (g_ail_digi_driver_descr[1] == type) {

			if (io == -1) {
				io = g_ail_digi_driver_descr[6];
				irq = g_ail_digi_driver_descr[7];
			}

			if (AIL_detect_device(g_ail_digi_driver_id, io, irq,
				g_ail_digi_driver_descr[8], g_ail_digi_driver_descr[9]))
			{
				AIL_init_driver(g_ail_digi_driver_id, io, irq,
					g_ail_digi_driver_descr[8],
					g_ail_digi_driver_descr[9]);

				g_snd_effects_enabled = 1;
				return 1;
			} else {
				/* no sound hardware found */
				GUI_output(g_snd_txt_hw_not_found2);
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

	signed int handle;

	if ( (handle = open(fname, O_BINARY | O_RDONLY)) != -1) {

		len = 5000L;

		g_ail_digi_driver_buf2 = (Bit8u*)schick_alloc(len + 16L);
		ptr = (Bit32u)g_ail_digi_driver_buf2 + 15L;
		ptr &= 0xfffffff0;
		buf = EMS_norm_ptr((Bit8u*)ptr);
		_read(handle, (Bit8u*)buf, (unsigned short)len);
		_close(handle);
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
	Bit32u start;
	Bit32u end;
	signed int handle;

	/* open SCHICK.DAT */
#if defined(__BORLANDC__) || defined(_WIN32)
	if ( (handle = open(g_fname_schick_dat, O_BINARY | O_RDONLY)) != -1) {
#else
	if ( (handle = open(g_fname_schick_dat, O_RDONLY)) != -1) {
#endif

		/* seek to the fileindex position in the offset table */
		lseek(handle, fileindex * 4, SEEK_SET);

		/* read the start offset of the desired file */
		_read(handle, (Bit8u*)&start, 4);

		/* read the start offset of the next file */
		_read(handle, (Bit8u*)&end, 4);

		/* seek to the desired file */
		lseek(handle, start, SEEK_SET);

		/* save the offset of the desired file */
		g_archive_file_offset = start;

		/* save the length of the desired file in 2 variables */
		g_archive_file_length = g_archive_file_remaining = end - start;
	}

	return handle;
}

Bit32u get_readlength2(signed short index)
{
	return index != -1 ? g_archive_file_length : 0;
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

	g_archive_file_remaining = g_archive_file_length - off;

	file_off = g_archive_file_offset + off;

	lseek(handle, file_off, SEEK_SET);

	return;
}

signed short load_regular_file(Bit16u index)
{
	signed int handle;

#if defined(__BORLANDC__) || defined(_WIN32)
	if ( (handle = open(g_fnames_v302de[index], O_BINARY | O_RDWR)) == -1) {
#else
	if ( (handle = open(g_fnames_v302de[index], O_RDWR)) == -1) {
#endif

		sprintf(g_dtp2, g_str_file_missing_ptr, g_fnames_v302de[index]);

		g_missing_file_guilock = 1;
		GUI_output(g_dtp2);
		g_missing_file_guilock = 0;
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
	signed int handle;

	sprintf((char*)tmppath, g_str_temp_xx_ptr2, g_fnames_v302de[index]);

#if defined(__BORLANDC__) || defined(_WIN32)
	while ( (handle = open(tmppath, O_BINARY | O_RDWR)) == -1) {
#else
	while ( (handle = open(tmppath, O_RDWR)) == -1) {
#endif

		copy_from_archive_to_temp(index, tmppath);
	}

	/* get the length of the file */
	g_archive_file_length = g_archive_file_remaining = lseek(handle, 0, 2);
	/* seek to start */
	lseek(handle, 0, 0);

	g_archive_file_offset = 0;

	return handle;
}

void copy_from_archive_to_temp(unsigned short index, char* fname)
{
	signed int handle1;
	signed int handle2;
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
	signed int handle1;
	signed int handle2;
	signed short len;

#if defined(__BORLANDC__) || defined(_WIN32)
	if ( (handle1 = open(src_file, O_BINARY | O_RDONLY)) != -1) {
#else
	if ( (handle1 = open(src_file, O_RDONLY)) != -1) {
#endif

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

	nvf_type = *(signed char*)(nvf->src);
	va = nvf_type & 0x80;
	nvf_type &= 0x7f;

	pics = readws(nvf->src + 1L);

	if (nvf->no < 0)
		nvf->no = 0;

	if (nvf->no > pics - 1)
		nvf->no = pics - 1;

	switch (nvf_type) {

	case 0x00:
		width = readws(nvf->src + 3L);
		height = readws(nvf->src + 5L);
		p_size = width * height;
		src =  nvf->src + nvf->no * p_size + 7L;
		break;

	case 0x01:
		offs = pics * 4 + 3L;
		for (i = 0; i < nvf->no; i++) {
#if !defined(__BORLANDC__)
			width = readws(nvf->src + i * 4 + 3L);
			height = readws(nvf->src + i * 4 + 5L);
#endif
			offs += width * height;
		}

		width = readws(nvf->src + nvf->no * 4 + 3L);
		height = readws(nvf->src + nvf->no * 4 + 5L);
		p_size = width * height;
		src = nvf->src + offs;
		break;

	case 0x02: case 0x04:
		width = readws(nvf->src + 3L);
		height = readws(nvf->src + 5L);
		offs = pics * 4 + 7L;
		for (i = 0; i < nvf->no; i++) {
			offs += readds(nvf->src + (i * 4) + 7L);
		}

		p_size = readds(nvf->src + nvf->no * 4 + 7L);
		src = nvf->src + offs;
		break;

	case 0x03: case 0x05:
		offs = pics * 8 + 3L;
		for (i = 0; i < nvf->no; i++)
			offs += readds(nvf->src + (i * 8) + 7L);

		width = readws(nvf->src + nvf->no * 8 + 3L);
		height = readws(nvf->src +  nvf->no * 8 + 5L);
		p_size = readds(nvf->src + i * 8 + 7L);
		src = nvf->src + offs;
		break;
	}

	if (!nvf->type) {

		/* PP20 decompression */

		if (va != 0) {

			/* get size from unpacked picture */
			retval = readds(src);
			nvf_no = src;
			nvf_no += (retval + (-4L));
			retval = readds(nvf_no);
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

	*nvf->width = width;
	*nvf->height = height;

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
void mouse_action(unsigned short *p1, unsigned short *p2, unsigned short *p3, unsigned short *p4, unsigned short *p5)
{
#if defined(__BORLANDC__)
	struct SREGS sregs;
	union REGS wregs;

	if (*(signed short*)p1 >= 0) {

		wregs.x.ax = *p1;
		wregs.x.bx = *p2;
		wregs.x.cx = *p3;

		switch (*p1) {
		case 0x9:	/* define Cursor in graphic mode */
		case 0xc:	/* install event handler */
		case 0x14:	/* swap event handler */
		case 0x16:	/* save mouse state */
		case 0x17:	/* load mouse state */
		{
			wregs.x.dx = *p4;
			sregs.es = *p5;
			break;
		}
		case 0x10: {
			wregs.x.cx = *p2;
			wregs.x.dx = *p3;
			wregs.x.si = *p4;
			wregs.x.di = *p5;
			break;
		}
		default : {
			wregs.x.dx = *p4;
		}
		}

		int86x(0x33, &wregs, &wregs, &sregs);

		if (*p1 == 0x14) {
			*p2 = sregs.es;
		} else {
			*p2 = wregs.x.bx;
		}

		*p1 = wregs.x.ax;
		*p3 = wregs.x.cx;
		*p4 = wregs.x.dx;
	}
#endif
}

#if defined(__BORLANDC__)
void interrupt mouse_isr(void)
{
	signed short l_si = _AX;
	unsigned short l1;
	unsigned short l3;
	unsigned short l4;
	unsigned short l5;
	unsigned short l6;

	if (!g_mouse_locked) {

		if (l_si & 0x2) {
			g_mouse1_event2 = 1;
			g_mouse1_event1 = 1;
		}

		if (l_si & 0x8) {
			g_mouse2_event = 1;
		}

		if (((gs_dungeon_index != DUNGEONS_NONE) || (gs_current_town != TOWNS_NONE)) &&
				!gs_current_loctype && !g_dialogbox_lock && (g_pp20_index == ARCHIVE_FILE_PLAYM_UK))
		{
			g_current_cursor =	(is_mouse_in_rect( 68,  4, 171,  51) ?	&g_cursor_arrow_up :
						(is_mouse_in_rect( 68, 89, 171, 136) ?	&g_cursor_arrow_down :
						(is_mouse_in_rect( 16, 36,  67,  96) ?	&g_cursor_arrow_left :
						(is_mouse_in_rect(172, 36, 223,  96) ?	&g_cursor_arrow_right :
						(!is_mouse_in_rect(16,  4, 223, 138) ?	&g_default_mouse_cursor : g_current_cursor)))));
		} else {
			if (g_dialogbox_lock) {
				g_current_cursor = &g_default_mouse_cursor;
			}
		}

		if (l_si & 1) {
			l1 = 3;
			l4 = g_mouse_posx;
			l5 = g_mouse_posy;

			mouse_action(&l1, &l3, &l4, &l5, &l6);

			g_mouse_posx = l4;
			g_mouse_posy = l5;

			if (g_mouse_posx > g_mouse_posx_max) {
				g_mouse_posx = g_mouse_posx_max;
			}
			if (g_mouse_posx < g_mouse_posx_min) {
				g_mouse_posx = g_mouse_posx_min;
			}
			if (g_mouse_posy < g_mouse_posy_min) {
				g_mouse_posy = g_mouse_posy_min;
			}
			if (g_mouse_posy > g_mouse_posy_max) {
				g_mouse_posy = g_mouse_posy_max;
			}

			l1 = 4;
			l4 = g_mouse_posx;
			l5 = g_mouse_posy;

			mouse_action(&l1, &l3, &l4, &l5, &l6);

			g_mouse_moved = 1;
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

	m_x = g_mouse_posx;
	m_y = g_mouse_posy;

	return ((m_x >= x1) && (m_x <= x2) && (m_y >= y1) && (m_y <= y2)) ? 1 : 0;
}

void mouse_init(void)
{
	if (g_have_mouse == 2) {

#if defined(__BORLANDC__)
		unsigned short p1, p2, p3, p4, p5;

		p1 = 0;

		mouse_action(&p1, &p2, &p3, &p4, &p5);

		if (p1 == 0) {
			g_have_mouse = 0;
		}
#endif

		g_current_cursor = &g_default_mouse_cursor;
		g_last_cursor = &g_default_mouse_cursor;

#if defined(__BORLANDC__)
		if (g_have_mouse == 2) {

			p1 = 4;
			p3 = g_mouse_posx;
			p4 = g_mouse_posy;

			mouse_action(&p1, &p2, &p3, &p4, &p5);

			mouse_irq_init(0x1f, (unsigned char*)&mouse_isr);
		}
#endif
	}
}

void disable_mouse(void)
{
	if (g_have_mouse == 2) {
		mouse_reset_ehandler();
	}
}

void seg002_170e(unsigned short *a1, unsigned short *a2, unsigned short *a3, unsigned short *a4)
{
	unsigned short tmp;

	*a1 = 5;

	mouse_action(a1, a2, a3, a4, &tmp);
}

#if defined(__BORLANDC__)
void call_mouse_isr(void)
{
	mouse_isr();
}

void mouse_irq_init(signed short irq_no, void interrupt *(isr))
{
	unsigned short l1;
	unsigned short l3;
	unsigned short l4;
	unsigned short l5;
	unsigned short l6;

	l1 = 12;
	l4 = irq_no;

	l5 = FP_OFF(call_mouse_isr);
	l6 = FP_SEG(call_mouse_isr);

	g_mouse_handler_bak = getvect(0x78);
	setvect(0x78, (void interrupt far (*)(...))isr);

	mouse_action(&l1, &l3, &l4, &l5, &l6);

	g_mouse_irq_init = 1;
}
#endif


void mouse_reset_ehandler(void)
{
#if defined(__BORLANDC__)
	unsigned short l1;
	unsigned short l2;
	unsigned short l3;
	unsigned short l4;
	unsigned short l5;

	setvect(0x78, g_mouse_handler_bak);

	l1 = 12;
	l3 = 0;
	l4 = 0;
	l5 = 0;

	mouse_action(&l1, &l2, &l3, &l4, &l5);

	g_mouse_irq_init = 0;
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
#if defined(__BORLANDC__)
	unsigned short l1 = 4;
	unsigned short l3;
	unsigned short l4 = x;
	unsigned short l5 = y;
	unsigned short l6;

	mouse_action(&l1, &l3, &l4, &l5, &l6);
#endif
}

#if defined(__BORLANDC__)
/* unused */
void seg002_1838(signed short a1, signed short a2, signed short a3, signed short a4)
{
	unsigned short l1 = 9;
	unsigned short l3 = a1;
	unsigned short l4 = a2;
	unsigned short l5 = a3;
	unsigned short l6 = a4;

	mouse_action(&l1, &l3, &l4, &l5, &l6);
}

/* unused */
void seg002_1880(signed short a1)
{
	unsigned short l1 = 29;
	unsigned short l3 = a1;
	unsigned short l4;
	unsigned short l5;
	unsigned short l6;

	mouse_action(&l1, &l3, &l4, &l5, &l6);
}
#endif


/**
 * \brief   makes a mouse cursor from a selected item
 *
 * \param   p pointer to the icon of the item
 */
void make_ggst_cursor(Bit8u *icon)
{
	signed short y;
	signed short x;

	/* clear the bitmask */
	for (y = 0; y < 16; y++) {
		g_ggst_cursor.mask[y] = 0;
	}

	/* make a bitmask from the icon */
	for (y = 0; y < 16; y++) {
		for (x = 0; x < 16; x++) {
			/* if pixelcolor of the icon is not black */
			if (*icon++ != 0x40) {
				g_ggst_cursor.mask[y] |= (0x8000 >> x);
			}
		}
	}

	/* copy and negate the bitmask */
	for (y = 0; y < 16; y++) {
		g_ggst_cursor.data[y] = ~g_ggst_cursor.mask[y];
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
	if (!g_mouse_locked) {

		if  (!g_mouse_refresh_flag) {

			g_mouse_locked = 1;
			restore_mouse_bg();
			g_mouse_locked = 0;
		}

		g_mouse_refresh_flag--;
	}
}

void refresh_screen_size1(void)
{
	/* check lock */
	if (!g_mouse_locked) {

		g_mouse_refresh_flag++;

		if (!g_mouse_refresh_flag) {

			/* get lock */
			g_mouse_locked = 1;

			if (g_mouse_posx < g_mouse_pointer_offsetx)
				g_mouse_posx = g_mouse_pointer_offsetx;

			if (g_mouse_posx > 315)
				g_mouse_posx = 315;

			if (g_mouse_posy < g_mouse_pointer_offsety)
				g_mouse_posy = g_mouse_pointer_offsety;

			if (g_mouse_posy > 195)
				g_mouse_posy = 195;

			save_mouse_bg();
			g_mouse_posx_bak = g_mouse_posx;
			g_mouse_posy_bak = g_mouse_posy;
			g_mouse_pointer_offsetx_bak = g_mouse_pointer_offsetx;
			g_mouse_pointer_offsety_bak = g_mouse_pointer_offsety;
			draw_mouse_cursor();

			/* put lock */
			g_mouse_locked = 0;
		}
	}
}

void mouse_19dc(void)
{
	/* return if mouse was not moved and the cursor remains */
	if (g_mouse_moved || (g_last_cursor != g_current_cursor)) {

		/* set new cursor */
		g_last_cursor = g_current_cursor;

		/* check if the new cursor is the default cursor */
		if (g_current_cursor == &g_default_mouse_cursor) {
			/* set cursor size 0x0 */
			g_mouse_pointer_offsetx = g_mouse_pointer_offsety = 0;
		} else {
			/* set cursor size 8x8 */
			g_mouse_pointer_offsetx = g_mouse_pointer_offsety = 8;
		}

		/* reset mouse was moved */
		g_mouse_moved = 0;
		update_mouse_cursor1();
		refresh_screen_size1();
	}
}

void handle_gui_input(void)
{
	signed short l_si;
	signed short tw_bak;
	signed short l1;

	g_bioskey_event = g_action = l_si = 0;

	herokeeping();

	if (CD_bioskey(1)) {

		l_si = (g_bioskey_event = bioskey(0)) >> 8;
		g_bioskey_event &= 0xff;

		if (l_si == 0x24) {
			l_si = 0x2c;
		}

		/* Ctrl + Q -> quit */
		if ((g_bioskey_event == 0x11) && !g_pregame_state) {
			cleanup_game();

			exit(0);
		}
	}

	if (g_mouse1_event2 == 0) {

		g_always_zero3 = 0;

		if (g_have_mouse == 0) {
		}

		/* Ctrl + E */
		if (g_bioskey_event == 5) {
			status_select_hero();
			l_si = 0;
		}

		/* Ctrl + O -> swap heroes */
		if (g_bioskey_event == 15) {
			GRP_swap_heroes();
			l_si = 0;
		}

		/* Ctrl + S -> sound menu */
		if ((g_bioskey_event == 0x13) && !g_dialogbox_lock) {
			sound_menu();
		}

		/* Ctrl + P -> pause game */
		if ((g_bioskey_event == 0x10) &&
			(g_bioskey_event10 == 0) &&
			!g_dialogbox_lock &&
			!g_pregame_state)
		{
			g_bioskey_event10 = 1;
			g_timers_disabled++;
			g_gui_text_centered = 1;
			tw_bak = g_textbox_width;
			g_textbox_width = 2;
			GUI_output(g_pause_string);		/* P A U S E */
			g_textbox_width = tw_bak;
			g_gui_text_centered = 0;
			g_bioskey_event10 = l_si = g_bioskey_event = 0;
			g_timers_disabled--;
		}
	} else {
		play_voc(ARCHIVE_FILE_FX1_VOC);
		g_mouse1_event2 = 0;
		l_si = 0;

		if (g_action_table_secondary) {
			l_si = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_secondary);
		}

		if (!l_si && (g_action_table_primary)) {
			l_si = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_primary);
		}

		if (g_have_mouse == 2) {

			for (l1 = 0; l1 < 15; l1++) {
				wait_for_vsync();
			}

			if (g_mouse1_event2 != 0) {
				g_mouse1_doubleclick = 1;
				g_mouse1_event2 = 0;
			}
		}

		if ((l_si >= 0xf1) && (l_si <= 0xf8)) {

			if (g_mouse1_doubleclick) {

				/* open character screen by double click on hero picture */
				if ((get_hero(l_si - 241)->typus != HERO_TYPE_NONE) && (get_hero(l_si - 241)->group_no == gs_current_group))
				{
					status_menu(l_si - 241);
					l_si = 0;
					g_mouse1_doubleclick = 0;
					g_mouse1_event2 = 0;
				}
			} else {
				/* swap heroes by click - move mouse - click */
				if (g_heroswap_allowed &&
					(get_hero(l_si - 241)->typus != HERO_TYPE_NONE) && (get_hero(l_si - 241)->group_no == gs_current_group))
				{
					/* the destination will be selected by a mouse klick in the following function call */
					GRP_move_hero(l_si - 241);
					l_si = 0;
					g_mouse1_doubleclick = 0;
					g_mouse1_event2 = 0;
				}
			}
		} else if (l_si == 0xfd) {
			/* Credits */

			l_si = 0;
			tw_bak = g_textbox_width;
			g_textbox_width = 5;
			g_gui_text_centered = 1;
			GUI_output(get_ttx(394));
			g_gui_text_centered = 0;
			g_textbox_width = tw_bak;

		} else if (l_si == 0xfc) {
			/* Clock */
			l_si = 0;
			tw_bak = g_textbox_width;
			g_textbox_width = 5;
			g_gui_text_centered = 1;
			prepare_date_str();
			GUI_output(g_dtp2);
			g_gui_text_centered = 0;
			g_textbox_width = tw_bak;

		}
	}

	mouse_19dc();
	g_action = (l_si);
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
signed short get_mouse_action(signed short x, signed short y, struct mouse_action *act)
{
	signed short i;

	for (i = 0; act[i].x1 != -1; i++) {

		if ((act[i].x1 <= x) && (act[i].x2 >= x) && (act[i].y1 <= y) && (act[i].y2 >= y))
		{
			return act[i].action;
		}

	}

	return 0;
}

void handle_input(void)
{
	signed short l_si;
	signed short l_di;

	g_bioskey_event = g_action = l_si = 0;

	herokeeping();

	if (CD_bioskey(1)) {

		l_si = (g_bioskey_event = bioskey(0)) >> 8;
		g_bioskey_event &= 0xff;

		if (l_si == 0x24) {
			l_si = 0x2c;
		}

		/* Ctrl + Q -> quit */
		if ((g_bioskey_event == 0x11) && !g_pregame_state) {
			cleanup_game();

			exit(0);
		}
	}

	if (g_mouse1_event2 == 0) {

		if (g_have_mouse == 0) {
		}

		/* Ctrl + S -> sound menu */
		if ((g_bioskey_event == 0x13) && !g_dialogbox_lock) {
			sound_menu();
		}

		/* Ctrl + P -> pause game */
		/* TODO: use tw_bak here */
		if ((g_bioskey_event == 0x10) && (g_bioskey_event10 == 0) &&
			!g_dialogbox_lock && !g_pregame_state)
		{
			g_timers_disabled++;
			g_bioskey_event10 = 1;
			g_gui_text_centered = 1;
			g_textbox_width = 2;
			GUI_output(g_pause_string);		/* P A U S E */
			g_textbox_width = 3;
			g_gui_text_centered = 0;
			g_timers_disabled--;

			g_bioskey_event10 = l_si = g_bioskey_event = 0;
		}
	} else {
		play_voc(ARCHIVE_FILE_FX1_VOC);
		g_mouse1_event2 = 0;
		l_si = 0;

		if (g_action_table_secondary) {
			l_si = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_secondary);
		}

		if (!l_si && g_action_table_primary) {
			l_si = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table_primary);
		}

		if (g_have_mouse == 2) {

			for (l_di = 0; l_di < 25; l_di++) {

				wait_for_vsync();

				if (g_mouse1_event2) {
					g_mouse1_doubleclick = 1;
					g_mouse1_event2 = 0;
					break;
				}
			}
		}
	}

	mouse_19dc();
	g_action = (l_si);
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

	while (g_game_state == GAME_STATE_MAIN) {

		if (gs_current_loctype != LOCTYPE_NONE) {
			do_location();
		} else if (gs_current_town != TOWNS_NONE) {
			do_town();
		} else if (gs_dungeon_index != DUNGEONS_NONE) {
			do_dungeon();
		} else if (gs_show_travel_map != 0) {
			do_travel_mode();
		}

		if (gs_datseg_status_start == 99) {
			g_game_state = (GAME_STATE_OUTRO);
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
				g_game_state = GAME_STATE_DEAD;

			} else if (!count_heroes_available_in_group() || ((count_heroes_available_in_group() == 1) && is_hero_available_in_group(get_hero(6)))) // count_heroes_available_in_group_ignore_npc() == 0
			{
				/* no heroes or only the NPC in this group can act => switch to next */
				GRP_switch_to_next(2);
			}

		}

		if ((get_hero(6)->typus != HERO_TYPE_NONE) &&
			((gs_current_town != TOWNS_NONE) || (g_game_state == GAME_STATE_VICTORY)) &&
			(gs_npc_months >= 1) &&	(g_npc_last_farewellcheck != gs_npc_months))
		{
			npc_farewell();
			g_npc_last_farewellcheck = gs_npc_months;
		}

		if (!g_in_fight &&
			((g_game_state == GAME_STATE_MAIN) || (g_game_state == GAME_STATE_VICTORY)) &&
			!gs_current_loctype)
		{
			check_level_up();
		}

		if (g_refresh_status_line) {

			g_refresh_status_line = 0;

			if (g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {
				draw_status_line();
			}
		}

		if ((g_game_state != GAME_STATE_MAIN) && (g_fading_state != 0)) {
			refresh_colors();
		}

		if (g_game_state == GAME_STATE_DEAD) {
			game_over_screen();
		}

		if (g_game_state == GAME_STATE_TIMEUP) {
			show_times_up();
		}

		if ((g_game_state == GAME_STATE_DEAD) ||
			g_game_state == GAME_STATE_UNKNOWN ||
			g_game_state == GAME_STATE_TIMEUP ||
			g_game_state == GAME_STATE_OUTRO ||
			g_game_state == GAME_STATE_FIGQUIT)
		{
			gs_current_loctype = LOCTYPE_NONE;

			do {
				answer = load_game_state();

			} while (answer == -1);

			if (answer) {
				g_game_state = (GAME_STATE_MAIN);
				refresh_colors();
			}
		}

		if (g_game_state == GAME_STATE_VICTORY) {
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
	struct struct_hero *hero_i;
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
	for (i = 0; i <= 6; i++, hero_i++) {

		if ((get_hero(i)->typus != HERO_TYPE_NONE) && (hero_i->recipe_timer > 0))
		{
			hero_i->recipe_timer--;
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

	for (i = 0; g_market_descr_table[i].min_size != -1; i++) {

		g_market_descr_table[i].size = random_interval(g_market_descr_table[i].min_size, 20);
	}
}

void pal_fade(Bit8s *dst, Bit8s *p2)
{
	signed int i;

	for (i = 0; i < 32; i++) {

		if ((p2[3 * i] < dst[3 * i]) && (dst[3 * i] > 0))
		{
			dst[i * 3]--;

		} else {
			if ((*(p2 + 3 * i) > *(dst + 3 * i)) && (*(dst + 3 * i) < 0x3f))
			{
				(*(dst + i * 3))++;
			}
		}

		if ((*((p2 + 1) + 3 * i) < *((dst + 1) + 3 * i)) && (*((dst + 1) + 3 * i) > 0))
		{

			(*((dst + 1) + i * 3))--;

		} else {
			if ((*((p2 + 1) + 3 * i) > *((dst + 1) + 3 * i)) &&	(*((dst + 1) + 3 * i) < 0x3f))
			{
				(*((dst + 1) + i * 3))++;
			}
		}

		if ((*((p2 + 2) + 3 * i) < *((dst + 2) + 3 * i)) && (*((dst + 2) + 3 * i) > 0))
		{

			(*((dst + 2) + i * 3))--;

		} else {
			if ((*((p2 + 2) + 3 * i) > *((dst + 2) + 3 * i)) &&	(*((dst + 2) + 3 * i) < 0x3f))
			{
				(*((dst + 2) + i * 3))++;
			}
		}
	}
}

void pal_fade_in(Bit8s *dst, Bit8s *p2, const signed int v3, const signed int colors)
{
	signed int i;
	signed int si;

	si = 0x40 - v3;

	for (i = 0; i < colors; i++) {

		if ((*((p2 + 0) + 3 * i) >= si) && (*((p2 + 0) + 3 * i) > *((dst + 0) + 3 * i)))
		{
			(*((dst + 0) + i * 3))++;
		}

		if ((*((p2 + 1) + 3 * i) >= si) && (*((p2 + 1) + 3 * i) > *((dst + 1) + 3 * i)))
		{
			(*((dst + 1) + i * 3))++;
		}

		if ((*((p2 + 2) + 3 * i) >= si) && (*((p2 + 2) + 3 * i) > *((dst + 2) + 3 * i)))
		{
			(*((dst + 2) + i * 3))++;
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
		pal_fade((Bit8s*)gs_palette_floor, (Bit8s*)g_townpal_buf);
		/* buildings */
		pal_fade((Bit8s*)gs_palette_buildings, (Bit8s*)g_townpal_buf + 0x60);
		/* sky */
		pal_fade((Bit8s*)gs_palette_sky, (Bit8s*)g_townpal_buf + 0xc0);

		/* in a town */
		if (gs_current_town &&
			/* not in a dungeon */
			!gs_dungeon_index &&
			/* not in a location */
			!gs_current_loctype &&
			/* not in a travel mode */
			!gs_show_travel_map &&
			/* no event animation */
			!g_event_ani_busy &&
			/* unknown */
			!g_special_screen &&
			/* unknown */
			(g_pp20_index == ARCHIVE_FILE_PLAYM_UK))
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
		pal_fade((Bit8s*)gs_palette_floor, (Bit8s*)&g_floor_fade_palette[0][0]);
		/* buildings */
		pal_fade((Bit8s*)gs_palette_buildings, (Bit8s*)&g_building_fade_palette[0][0]);
		/* sky */
		pal_fade((Bit8s*)gs_palette_sky, (Bit8s*)&g_sky_fade_palette[0][0]);

		/* in a town */
		if (gs_current_town &&
			/* not in a dungeon */
			!gs_dungeon_index &&
			/* not in a location */
			!gs_current_loctype &&
			/* not in a travel mode */
			!gs_show_travel_map &&
			/* no event animation */
			!g_event_ani_busy &&
			/* unknown */
			!g_special_screen &&
			/* unknown */
			(g_pp20_index == ARCHIVE_FILE_PLAYM_UK))
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
	if (is_in_byte_array(gs_month, g_months_winter)) {
		return SEASON_WINTER;
	} else if (is_in_byte_array(gs_month, g_months_summer)) {
		return SEASON_SUMMER;
	} else if (is_in_byte_array(gs_month, g_months_spring)) {
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
	val += g_bank_heller;

	if (val < 0) {
		/* 15% Interest for borrowed money */
		val += val * 15 / 100L / 12L;
	} else if (val > 0) {
		/* 5% Interest for deposited money */
		val += val * 5 / 100L / 12L;
	}

	/* remember the heller */
	g_bank_heller = val % 10;

	if (val < 0) {
		g_bank_heller = -__abs__(g_bank_heller);
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
	struct struct_hero *hero_i;
	signed char afternoon;
	struct struct_hero *ptr;
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

	if (!g_freeze_timers) {
		/* g_freeze_timers is set in timewarp(..) and timewarp_until_time_of_day(..) for efficiency reasons,
		 *  where certain timers are updated separately in a single step (instead of many 1-tick update calls). */
		sub_ingame_timers(1);
		sub_mod_timers(1);
	}

	if (!g_freeze_timers) {

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
			if (gs_unicorn_get_map && gs_unicorn_timer)
			{
				gs_unicorn_timer--;
			}

			/* handle sphere timer */
			if (gs_dng02_sphere_timer) {

				if (!(--gs_dng02_sphere_timer)) {
					gs_dng02_sphere_active = 1;
				}
			}

			/* barrels with orc muck in the orc dungeon */
			if (gs_dng08_timer1) {
				gs_dng08_timer1--;
			}
			if (gs_dng08_timer2) {
				gs_dng08_timer2--;
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

		for (i = 0; i <= 6; i++, hero_i++) {

			if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->jail != 0))
			{
				hero_i->jail = 0;

				gs_groups_current_loctype[hero_i->group_no] =
					gs_groups_current_loctype_bak[hero_i->group_no];

				gs_groups_x_target[hero_i->group_no] =
					gs_groups_x_target_bak[hero_i->group_no];

				gs_groups_y_target[hero_i->group_no] =
					gs_groups_y_target_bak[hero_i->group_no];
			}
		}
	}

	/* at 10 o'clock */
	if (gs_day_timer == HOURS(10)) {

		hero_i = get_hero(0);

		for (i = 0; i <= 6; i++, hero_i++) {

			if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->drunk != 0))
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

			for (i = 0; i <= 6; i++, ptr++) {

				if (ptr->typus != HERO_TYPE_NONE) {

					di = ptr->group_no;

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

			if (gs_npc_timers[i] && (gs_npc_timers[i] != -1))
			{
				gs_npc_timers[i]--;
			}
		}

		/* drug timer (phexcaer) */
		if (gs_drug_timer) {
			gs_drug_timer--;
		}

		/* unknown timer */
		if (gs_forcedmarch_timer) {
			gs_forcedmarch_timer--;
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
			if (get_hero(6)->typus != HERO_TYPE_NONE) {
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
			g_game_state = (GAME_STATE_TIMEUP);
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
	HugePt mp;
	signed char target;
	unsigned char reset_target;
	struct struct_modification_timer *sp;

	h_index = -1;

	if (g_timers_disabled)
		return;

	for (i = 0; i < 100; i++) {

		/* if timer is 0 continue */
		if (gs_modification_timers[i].time_left == 0)
			continue;

		/* subtract diff from timer */
		gs_modification_timers[i].time_left -= val;


		/* if timer > 0 continue */
		if (gs_modification_timers[i].time_left <= 0) {


#if !defined(__BORLANDC__)
			D1_LOG("Mod Timer %d rueckgesetzt\n", i);
#endif

			/* set timer to 0 */
			gs_modification_timers[i].time_left = 0;

			/* make a pointer to the slot */
			sp = &gs_modification_timers[i];

			if (sp->target) {
				/* target is a hero/npc */

				/* get the hero index from the target */
				target = sp->target;
				for (j = 0; j <= 6; j++) {
					if (get_hero(j)->timer_id == target) {
						h_index = j;
						break;
					}
				}

				if (h_index != -1) {
					/* if a hero/npc is determined */

					mp = (Bit8u*)get_hero(h_index);
					/* make a pointer to the hero's attribute mod */
					mp += (Bit32u)sp->offset;
					/* subtract the mod */
					*mp -= sp->modifier;

					if (g_pp20_index == ARCHIVE_FILE_ZUSTA_UK) {
						g_request_refresh = 1;
					}

					/* reset target */
					sp->target = 0;

					/* reset target if no other slots of target */
					reset_target = 1;
					for (j = 0; j < 100; j++) {
						if (gs_modification_timers[j].target == target) {
							reset_target = 0;
							break;
						}
					}

					if (reset_target) {
						get_hero(h_index)->timer_id = 0;
					}
				} else {
#if !defined(__BORLANDC__)
					D1_ERR("Invalid Mod Timer Target %d\n", target);
#endif

					/* reset all slots of invalid target */
					for (j = 0; j < 100; j++) {
						if (gs_modification_timers[j].target == target) {
							sp->target = sp->modifier = 0;
							sp->offset = 0;
						}
					}
				}

			} else {
				/* target affects the savegame */
				mp = (Bit8u*)&gs_datseg_status_start;
				mp += sp->offset;
				*mp -= sp->modifier;
			}

			/* reset offset, target, and modificator */
			sp->target = sp->modifier = 0;
			sp->offset = 0;
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

		if (gs_modification_timers[i].offset == 0) {
			break;
		}
	}

	if (i == 100) {
		/* all 100 mod timers are in use. apply hack to free timer in slot 0 */

		/* set timer of slot 0 to 1 */
		gs_modification_timers[0].time_left = 1;
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
		mod_ptr = (Bit8u*)get_hero(who);

		if (get_hero(who)->timer_id != 0) {
			/* hero/npc has a target number */
			target = get_hero(who)->timer_id;
		} else {
			/* hero/npc has no target number */

			for (i = 1; i < 8; i++) {

				new_target = 1;
				for (j = 0; j <= 6; j++) {
					if (get_hero(j)->timer_id == i) {
						new_target = 0;
						break;
					}
				}

				if (new_target != 0) {
					target = (signed char)i;
					break;
				}
			}

			get_hero(who)->timer_id = target;
		}

		gs_modification_timers[slot_no].target = target;
	}

	gs_modification_timers[slot_no].modifier = mod;
	gs_modification_timers[slot_no].offset = (HugePt)ptr - mod_ptr;
	gs_modification_timers[slot_no].time_left = timer_value;
	*((Bit8s*)ptr) += mod;
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
	struct struct_hero *hero_i;

	if (g_timers_disabled)
		return;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if (hero_i->typus != HERO_TYPE_NONE) {

			/* Timer to the next healing attempt */
			if (hero_i->heal_timer > 0) {

				hero_i->heal_timer -= fmin * MINUTES(5);
#if !defined(__BORLANDC__)
				if (hero_i->heal_timer <= 0) {
					D1_INFO("%s kann wieder geheilt werden\n", hero_i->alias);
				}
#endif

				if (hero_i->heal_timer < 0) {
					hero_i->heal_timer = 0;
				}
			}

			/* Timer set after Staffspell */
			if (hero_i->staffspell_timer > 0) {

				hero_i->staffspell_timer -= fmin * MINUTES(5);
#if !defined(__BORLANDC__)
				if (hero_i->staffspell_timer <= 0) {
					D1_INFO("%s kann wieder einen Stabzauber versuchen\n", hero_i->alias);
				}

#endif
				if (hero_i->staffspell_timer < 0) {
					hero_i->staffspell_timer = 0;
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

	struct struct_hero *hero_i;
	signed char tmp;

	if (g_timers_disabled)
		return;

	hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {
		if (hero_i->typus != HERO_TYPE_NONE) {

			if (quarter > 120) {
				tmp = 120;
			} else {
				tmp = (signed char)quarter;
			}

			for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {

				if (hero_i->inventory[j].item_id == ITEM_TORCH_ON) {

					/* Torch, burning */
					hero_i->inventory[j].lighting_timer -= tmp;

					if (hero_i->inventory[j].lighting_timer <= 0)
					{
						/* decrement item counter */
						hero_i->items_num--;

						/* subtract weight of a torch */
						hero_i->load -= g_itemsdat[ITEM_TORCH_ON].weight;

						/* Remove Torch from inventory */
						memset(&hero_i->inventory[j], 0, sizeof(inventory));
					}

				} else if (hero_i->inventory[j].item_id == ITEM_LANTERN_ON) {

					/* Lantern, burning */
					hero_i->inventory[j].lighting_timer -= tmp;

					if (hero_i->inventory[j].lighting_timer <= 0) {

						/* Set timer to 0 */
						hero_i->inventory[j].lighting_timer = 0;
						/* Set burning lantern to a not burning lantern */
						hero_i->inventory[j].item_id = ITEM_LANTERN_OFF;
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
	struct struct_hero *hero_i;

	if (g_timers_disabled) {
		return;
	}

	g_herokeeping_flag = (gs_show_travel_map ? 1 : 2);

	for (i = 0; i <= 6; i++) {

		if (get_hero(i)->typus != HERO_TYPE_NONE) {

			hero_i = get_hero(i);

			if (!hero_i->flags.dead &&
				/* check if not in jail (the argument might be: heroes are forced to take off armor in jail) */
				!hero_i->jail &&
				/* check if cursed chainmail is equipped */
				(hero_i->inventory[HERO_INVENTORY_SLOT_BODY].item_id == ITEM_CHAIN_MAIL_CURSED))
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
	struct struct_hero *hero;
	char buffer[100];

	if (g_game_state != GAME_STATE_MAIN)
		return;

	/* The actual food consumption is done only if g_herokeeping_flag is set.
	 * This happens hourly in magical_chainmail_damage()
	 * The flag is reset at the end of this function. */

	/* for each hero ..*/
	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

		/* consume food and set messages */
		/* must be vital */
		if ((hero->typus != HERO_TYPE_NONE) && g_herokeeping_flag &&
				check_hero_no3(hero) &&	!hero->jail && !g_travel_herokeeping)
		{
			/* Do the eating */

			/* check for magic bread bag in the group */
			if (get_first_hero_with_item_in_group(ITEM_MAGIC_BREADBAG, hero->group_no) == -1) {

				/* if not, check if the hero has the food amulet */
				if (get_item_pos(hero, ITEM_TRAVIA_AMULET) == -1) {
					/* if not... */

					/* eat if hunger > 90 % */
					if (hero->hunger > 90) {

						/* search for Lunchpack */
						pos = get_item_pos(hero, ITEM_FOOD_PACKAGE);

						if (pos != -1) {
							/* Lunchpack found, consume quiet */
							g_consume_quiet = 1;
							consume(hero, hero, pos);
#if !defined(__BORLANDC__)
							D1_INFO("%s isst etwas\n", hero->alias);
#endif
							g_consume_quiet = 0;

							/* search for another Lunchpack */
							/* print last ration message */
							if (get_item_pos(hero, ITEM_FOOD_PACKAGE) == -1) {
								gs_food_message[i] = 6;
							}
						} else {
							/* print ration warning */
							if (hero->hunger < 100) {
								gs_food_message[i] = 4;
							}
						}

					}

					if (hero->hunger < 100) {

						/* increase hunger value. FOOD_MOD is always 0 or 1 */
						if (hero->hunger_timer <= 0) {
							/* increase more (FOOD_MOD == 0 -> increase by 2. FOOD_MOD == 1 -> increase by 0.) */
							hero->hunger += 2 / (g_food_mod * 2 + 1);
						} else {
							/* increase less (FOOD_MOD == 0 -> increase by 1. FOOD_MOD == 1 -> increase by 0.) */
							hero->hunger += 1 / (g_food_mod * 2 + 1);
						}

						/* adjust hunger */
						if (hero->hunger > 100) {
							hero->hunger = 100;
						}
					} else {

						/* */
						if (hero->hunger_timer <= 0) {
							do_starve_damage(hero, i, 0);
						}
					}
				}
			} else {

				/* set hunger to 20 % */
				if (hero->hunger > 20) {
					hero->hunger = 20;
				}
			}

			/* Do the drinking */

			/* check if someone in the group of the hero has the magic bread bag */
			/* check for magic waterskin in group */
			if ((get_first_hero_with_item_in_group(ITEM_MAGIC_WATERSKIN, hero->group_no) == -1) &&
				(((hero->group_no == gs_current_group) &&
					(!gs_current_town || (gs_current_town != TOWNS_NONE && gs_show_travel_map != 0))) ||
				((hero->group_no != gs_current_group) && !gs_groups_town[hero->group_no])))
			{
					/* check for food amulett */
					if (get_item_pos(hero, ITEM_TRAVIA_AMULET) == -1) {

						/* hero should drink something */
						if (hero->thirst > 90) {

							g_consume_quiet = 1;

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
								D1_INFO("%s trinkt etwas\n", hero->alias);
#endif
								/* nothing to drink message */
								if ((get_item_pos(hero, ITEM_BEER) == -1)
									&& (get_full_waterskin_pos(hero) == -1)) {
									gs_food_message[i] = 5;
								}

							} else {
								/* hero has nothing to drink */
								if (hero->thirst < 100) {
									gs_food_message[i] = 3;
								}
							}

							g_consume_quiet = 0;
						}

						if (hero->thirst < 100) {

							/* increase thirst counter food_mod is always 0 or 1 */
							if (hero->hunger_timer <= 0) {

								/* increase more (FOOD_MOD == 0 -> increase by 4. FOOD_MOD == 1 -> increase by 1.) */
								hero->thirst += 4 / (g_food_mod * 2 + 1);
							} else {

								/* increase less (FOOD_MOD == 0 -> increase by 2. FOOD_MOD == 1 -> increase by 0.) */
								hero->thirst += 2 / (g_food_mod * 2 + 1);
							}

							/* adjust thirst */
							if (hero->thirst > 100) {
								hero->thirst = 100;
							}

						} else {
							if (hero->hunger_timer <= 0) {
								do_starve_damage(hero, i, 1);
							}
						}
					}
			} else {

				/* set thirst to 20 % */
				if (hero->thirst > 20) {
					hero->thirst = 20;
				}
			}
		}

		/* print hero message */
		if (gs_food_message[i] && !g_dialogbox_lock &&	!g_in_fight && !g_freeze_timers)
		{
			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && !hero->flags.dead &&
				(!gs_show_travel_map || (g_food_message_shown[i] != gs_food_message[i]))) {

					sprintf(buffer,	 (gs_food_message[i] == 1) ? get_ttx(224):
							((gs_food_message[i] == 2) ? get_ttx(223) :
							((gs_food_message[i] == 3) ? get_ttx(797) :
							((gs_food_message[i] == 4) ? get_ttx(798) :
							((gs_food_message[i] == 5) ? get_ttx(799) :
							get_ttx(800))))),
							hero->alias, GUI_get_ptr(hero->sex, 1));

					g_food_message_shown[i] = gs_food_message[i];

					GUI_output(buffer);

					if (g_pp20_index == ARCHIVE_FILE_ZUSTA_UK) {
						g_request_refresh = 1;
					}
			}

			gs_food_message[i] = 0;
		}


		/* print unconscious message */
		if (gs_unconscious_message[i] && !g_dialogbox_lock) {

			if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group) && !hero->flags.dead)
			{
					sprintf(buffer, get_ttx(789), hero->alias);
					GUI_output(buffer);

					if (g_pp20_index == ARCHIVE_FILE_ZUSTA_UK) {
						g_request_refresh = 1;
					}
			}

			/* reset condition */
			gs_unconscious_message[i] = 0;
		}
	}

	g_herokeeping_flag = 0;
}

void check_level_up(void)
{
	signed short i;
	signed short not_done;
	struct struct_hero *hero;

	if (g_timers_disabled) {
		return;
	}

	do {
		not_done = 0;
		hero = get_hero(0);
		for (i = 0; i <= 6; i++, hero++) {

			if ((hero->typus != HERO_TYPE_NONE) && !hero->flags.dead && (hero->level < 20) &&

#ifndef M302de_FEATURE_MOD
				/* Feature mod 8:
				 * Adjust AP requirement for level up to match the original DSA 2/3 rules.
				 * Without this mod, it is "off by one".
				 * For example, for level 2, 100 AP should be enough, but the game requires 101 AP. */
				(g_level_ap_tab[hero->level] < hero->ap)
				/* could be easily done without accessing the data segment by the formula level_ap_tab[i] = 50 * i * (i+1) */
#else
				(50 * hero->level * (hero->level + 1) <= hero->ap)
				/* while we're at it, avoid accessing the data segment... */
#endif
			) {
				level_up(i);
				not_done = 1;
			}
		}

	} while (not_done);
}

void seg002_37c4(void)
{
	signed short l_si = 0;
	Bit8u* p1;
	Bit8u* p2;
	Bit8u* p3;
	struct struct_pic_copy pic_copy_bak = g_pic_copy;

	p1 = g_buffer6_ptr + 2000;
	p2 = g_buffer6_ptr + 2100;
	p3 = g_buffer6_ptr + 1000;

	if (g_trv_menu_selection && gs_show_travel_map) {

		g_selected_town_anix = g_town_positions[gs_trv_menu_towns[g_trv_menu_selection - 1] - 1].x;
		g_selected_town_aniy = g_town_positions[gs_trv_menu_towns[g_trv_menu_selection - 1] - 1].y;

		g_pic_copy.x1 = g_selected_town_anix - 4;
		g_pic_copy.y1 = g_selected_town_aniy - 4;
		g_pic_copy.x2 = g_selected_town_anix + 4;
		g_pic_copy.y2 = g_selected_town_aniy + 4;
		g_pic_copy.src = p1;

		if (is_mouse_in_rect(g_pic_copy.x1 - 16, g_pic_copy.y1 - 16, g_pic_copy.x2 + 16, g_pic_copy.y2 + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_pic_copy(0);

		if (l_si) {
			refresh_screen_size();
		}

		g_trv_menu_selection = l_si = 0;
	}

	if (g_current_town_over) {

		g_pic_copy.x1 = g_current_town_overx - 4;
		g_pic_copy.y1 = g_current_town_overy - 4;
		g_pic_copy.x2 = g_current_town_overx + 4;
		g_pic_copy.y2 = g_current_town_overy + 4;
		g_pic_copy.src = p2;

		if (is_mouse_in_rect(g_pic_copy.x1 - 16, g_pic_copy.y1 - 16, g_pic_copy.x2 + 16, g_pic_copy.y2 + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		if (g_current_town_over) {
			do_pic_copy(0);
		}

		if (l_si) {
			refresh_screen_size();
		}

		l_si = g_current_town_over = 0;
	}

	if (g_current_town_anix) {

		g_pic_copy.x1 = g_current_town_anix - 4;
		g_pic_copy.y1 = g_current_town_aniy - 4;
		g_pic_copy.x2 = g_current_town_anix + 4;
		g_pic_copy.y2 = g_current_town_aniy + 4;
		g_pic_copy.src = p2;

		if (is_mouse_in_rect(g_pic_copy.x1 - 16, g_pic_copy.y1 - 16, g_pic_copy.x2 + 16, g_pic_copy.y2 + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_save_rect();

		if (l_si) {
			refresh_screen_size();
		}

		g_current_town_over = 1;
		g_current_town_overx = g_current_town_anix;
		g_current_town_overy = g_current_town_aniy;
		l_si = 0;

		if (g_menu_input_busy && gs_show_travel_map) {

			g_selected_town_anix = g_town_positions[gs_trv_menu_towns[g_menu_selected - 1] - 1].x;
			g_selected_town_aniy = g_town_positions[gs_trv_menu_towns[g_menu_selected - 1] - 1].y;

			g_pic_copy.x1 = g_selected_town_anix - 4;
			g_pic_copy.y1 = g_selected_town_aniy - 4;
			g_pic_copy.x2 = g_selected_town_anix + 4;
			g_pic_copy.y2 = g_selected_town_aniy + 4;
			g_pic_copy.src = p1;

			if (is_mouse_in_rect(g_pic_copy.x1 - 16, g_pic_copy.y1 - 16, g_pic_copy.x2 + 16, g_pic_copy.y2 + 16))
			{
				update_mouse_cursor();
				l_si = 1;
			}

			do_save_rect();
			g_pic_copy.src = p3 + 100 * g_map_townmark_state;
			do_pic_copy(2);

			if (l_si) {
				refresh_screen_size();
			}

			g_trv_menu_selection = g_menu_selected;

			l_si = 0;
		}

		g_pic_copy.x1 = g_current_town_anix - 4;
		g_pic_copy.y1 = g_current_town_aniy - 4;
		g_pic_copy.x2 = g_current_town_anix + 4;
		g_pic_copy.y2 = g_current_town_aniy + 4;
		g_pic_copy.src = p3 + 100 * (g_map_townmark_state + 5);

		if (is_mouse_in_rect(g_pic_copy.x1 - 16, g_pic_copy.y1 - 16, g_pic_copy.x2 + 16, g_pic_copy.y2 + 16))
		{
			update_mouse_cursor();
			l_si = 1;
		}

		do_pic_copy(2);

		if (l_si) {
			refresh_screen_size();
		}

		g_current_town_over = 1;
	}

	g_spinlock_flag = 0;

	g_map_townmark_state++;
	g_map_townmark_state %= 5;

	g_pic_copy = pic_copy_bak;
}

void set_and_spin_lock(void)
{
	g_spinlock_flag = 1;

	while (g_spinlock_flag) {
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
	struct sea_route *route;

	route = &g_sea_routes[0];

	i = get_current_season();

	frequency_modifier = (i == SEASON_SUMMER) ? 2 : ((i == SEASON_WINTER) ? 4 : 0);
		/* winter -> 4
		 * summer -> 2
		 * spring, autumn -> 0 */

	for (i = 0; i < NR_SEA_ROUTES; route++, i++) {

		if (route->passage_timer-- == -1) { /* note that dec_ptr_bs returns the old (still un-decremented) value */
			/* ship of a sea passage has left yesterday -> set up a new ship of this passage */

			route->price_mod = random_interval(70, 130);
			/* random price modifier in the range 70% -- 130% */

			route->passage_timer = random_interval(0, route->frequency * 10 + route->frequency * frequency_modifier) / 10;
			/* setup timer: In how many days will a ship of this passage be available? */
			/* This results in a random number in the interval [ 0..(route->frequency + frequency_modifier) ], where
			 * all numbers have the same probabilty, except the upper end route->frequency + frequency_modifier
			 * of the interval which has only 1/10 of the probabilty of each other number. */

			di = random_schick(100);

			route->ship_type =
				(!route->coastal_route ?
					((di <= 50) ? SHIP_TYPE_LANGSCHIFF_HIGH_SEAS : ((di <= 80) ? SHIP_TYPE_KARRACKE : (di <= 95) ? SHIP_TYPE_SCHNELLSEGLER : SHIP_TYPE_SCHNELLSEGLER_LUXURY)) :
					((di <= 10) ? SHIP_TYPE_LANGSCHIFF_COSTAL : ((di <= 40) ? SHIP_TYPE_KUESTENSEGLER : (di <= 80) ? SHIP_TYPE_KUTTER : SHIP_TYPE_FISCHERBOOT)));

#if !defined(__BORLANDC__)
			// TODO: Ausgabe fuer Schiffstyp verbessern (momentan nur Zahl)
			D1_INFO_VERBOSE("Neue Passage auf Seeroute ID %d (%s -- %s): Abfahrt in %d Tagen, Schiffstyp: %d, Preisanpassung: %d%%.\n",
					i, get_ttx(route->town1 + 235), get_ttx(route->town2 + 235),
					route->passage_timer, route->ship_type,	route->price_mod);
#endif
		}
	}

	/* If a passage is hired decrement Passage days timer */
	if (gs_sea_travel_psgbooked_flag == 0xaa) {
		gs_sea_travel_psgbooked_timer--;
	}
}

/**
 * \brief   called once every day at 9 o'clock when the ships leave the harbors
 */
void passages_reset(void)
{
	signed short i;
	struct sea_route *route = &g_sea_routes[0];

#ifndef M302de_ORIGINAL_BUGFIX
	/* Original-Bug 36: the loop operates only on the first sea route (which is Thorwal-Prem) */
	for (i = 0; i < NR_SEA_ROUTES; i++)
#else
	for (i = 0; i < NR_SEA_ROUTES; route++, i++)
#endif
	{
		if (!route->passage_timer) {
			route->passage_timer = -1;
		}
	}

	/* If a passage is booked and the timer is zero, the ship leaves the harbor.
	 * Therefore, reset the flag for the passage (i.e., unbook the passage) */
	if ((gs_sea_travel_psgbooked_flag == 0xaa) && !gs_sea_travel_psgbooked_timer) {
		gs_sea_travel_psgbooked_flag = 0;
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

	g_freeze_timers = 1;
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
		if (gs_unicorn_get_map && gs_unicorn_timer != 0)
		{
			gs_unicorn_timer--;
		}

		/* timer for Sphaerenriss in verfallene Herberge */
		if (gs_dng02_sphere_timer) {

			if (!(--gs_dng02_sphere_timer)) {
				gs_dng02_sphere_active = 1;
			}
		}

		/* timer for barrels with orc muck in the orc dungeon */
		if (gs_dng08_timer1) {
			gs_dng08_timer1--;
		}
		if (gs_dng08_timer2) {
			gs_dng08_timer2--;
		}
	}
#endif

	/* restore variables */
	g_freeze_timers = 0;
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

	g_freeze_timers = 1;

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
	 * forgotten hourly timers: UNICORN_TIMER, gs_dng02_sphere_timer, gs_dng08_timer1, gd_dng08_timer2
	 * see do_timers(..).
	 * For a bugfix either add code here (and in timewarp(..)), or modify do_timers(..)
	 * */

	/* restore variables */
	g_freeze_timers = 0;
	g_timers_disabled = td_bak;
#endif
}

/**
 * \brief   decrements splash timer and restores pictures
 */
void dec_splash(void)
{
	signed int i;

	for (i = 0; i <= 6; i++) {

		if (!g_dialogbox_lock && g_hero_splash_timer[i] && !(--g_hero_splash_timer[i]) &&
			(g_pp20_index == ARCHIVE_FILE_PLAYM_UK) && !get_hero(i)->flags.dead) {

			restore_rect(g_vga_memstart, get_hero(i)->pic, g_hero_pic_posx[i], 157, 32, 32);
		}
	}
}

/**
 * \brief   draws a splash on a hero portrait
 *
 * \param   hero_pos    on which slot the splash is drawn
 * \param   type        kind of damage (0 = red,LE / !0 = yellow,AE)
 */
static void draw_splash(signed short hero_pos, signed short type)
{
	if (g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {

		Bit8u *splash = (type == 0 ? g_splash_le : g_splash_ae);

		restore_rect_rle(g_vga_memstart, splash, g_hero_pic_posx[hero_pos], 157, 32, 32, 2);

		/* how long the splash should be displayed */
		g_hero_splash_timer[hero_pos] = 10;
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

	g_mouse1_event2 = 0;

	do {
		if (CD_bioskey(1)) {

			si = bioskey(0);

			if (((si & 0xff) == 0x20) && (g_bioskey_event10 == 0))
			{

				seg002_47e2();

				while (!CD_bioskey(1)) {;}

				seg002_484f();
				break;
			}
		}

	} while (!CD_bioskey(1) && g_mouse1_event2 == 0);

	if (CD_bioskey(1))
		si = bioskey(0);

	g_mouse1_event2 = 0;
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

		g_delay_or_keypress_flag = 1;
		handle_input();
		g_delay_or_keypress_flag = 0;

		if (g_c_event_active) {

			if (g_action != 0) {

				if (g_action == ACTION_ID_SPACE) {

					seg002_47e2();
					while (!CD_bioskey(1)) { ; }
					seg002_484f();
					done = 1;

				} else {

					if ((g_action != ACTION_ID_UP) &&
						(g_action != ACTION_ID_DOWN) &&
						(g_action != ACTION_ID_RIGHT) &&
						(g_action != ACTION_ID_LEFT))
					{
						done = 1;
					}
				}
			} else {

				if (g_mouse2_event) {
					done = 1;
				}
			}
		} else {

			if (g_action != 0) {

				if (g_action == ACTION_ID_SPACE) {

					seg002_47e2();
					while (!CD_bioskey(1)) { ; }
					seg002_484f();
				}

				done = 1;

			} else {

				if (g_mouse2_event) {
					done = 1;
				}
			}
		}


		if (done) {
			g_mouse2_event = 0;
			g_action = (ACTION_ID_RETURN);
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
	g_unused_spinlock_flag = 1;

	while (g_unused_spinlock_flag) {
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

	*ptr = readds((Bit8u*)&v);

	tmp = a[0];
	a[0] = swap_u16(a[1]);
	a[1] = swap_u16(tmp);

	return readds((Bit8u*)ptr);
}

/* unused */
Bit32u swap_u32_unused(Bit32u v)
{
	signed short a[2];
	signed short tmp;
	Bit32s *ptr = (Bit32s*)(&a[0]);

	tmp = (signed short)(*ptr = readds((Bit8u*)&v));

	a[0] = a[1];
	a[1] = tmp;

	return readds((Bit8u*)ptr);
}

/**
 * \brief   allocates EMS memory
 *
 * \param   bytes       bytes to allocate
 * \return              an EMS handle, to access the memory.
 */
signed short alloc_EMS(Bit32s bytes)
{
	signed int handle;

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
		ptr = (Bit8u*)((HugePt)dst + (((Bit32s)si) << 0x0e));
		si++;

		len = (bytes - 0x4000 > 0) ? 0x4000 : (signed short)bytes;

		bytes -= 0x4000;

		memmove((void*)EMS_norm_ptr(ptr), (void*)g_ems_frame_ptr, len);

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
		ptr = (Bit8u*)((HugePt)src + (((Bit32s)si) << 0x0e));
		si++;

		len = (bytes - 0x4000 > 0) ? 0x4000 : (signed short)bytes;

		bytes -= 0x4000;

		memmove((void*)g_ems_frame_ptr, (void*)EMS_norm_ptr(ptr), len);

	} while (--di != 0);
#endif
}

void clear_menu_icons(void)
{
	signed int i;

	for (i = 0; i < 9; i++) {
		g_new_menu_icons[i] = MENU_ICON_NONE;
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
	signed int i;
	signed int changed = 0;

	/* save icon ids in local variable */
	for (i = 0; i < 9; i++) {
		icons_bak[i] = g_new_menu_icons[i];
		g_new_menu_icons[i] = MENU_ICON_NONE;
	}

	va_start(arguments, icons);

	for (i = 0; i < icons; i++) {
		g_new_menu_icons[i] = va_arg(arguments, int);

		if (g_new_menu_icons[i] != icons_bak[i]) {
			changed = 1;
		}
	}

	va_end(arguments);

	if (icons_bak[i] != -1)
		changed = 1;

	if (changed && g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {
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
		!g_travel_event_active &&
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

		n.width = &width;
		n.height = &height;

		/* process the nvf */
		process_nvf(&n);

		/* set x and y values */
		g_pic_copy.x1 = 94;
		g_pic_copy.y1 = 115;
		g_pic_copy.x2 = 145;
		g_pic_copy.y2 = 136;

		/* set source */
		g_pic_copy.src = g_icon;

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

unsigned short div16(unsigned char val)
{
	return ((unsigned char)val) >> 4;
}

void select_with_mouse(signed short *item_pos, struct shop_item *shop_item)
/* This function is called in shops at sell/buy screens */
{
	signed short i;

	if (g_have_mouse != 2) {
		return;
	}

	for (i = 0; i < 15; i++) {
		if ((g_merchant_items_posx[i] <= g_mouse_posx) &&
			(g_merchant_items_posx[i] + 50 >= g_mouse_posx) &&
			(g_merchant_items_posy[i] <= g_mouse_posy) &&
			(g_merchant_items_posy[i] + 17 >= g_mouse_posy) &&
			(shop_item[i].item_id != 0))
		{
			*item_pos = i;
			return;
		}
	}
}

void select_with_keyboard(signed short *item_pos, struct shop_item *shop_item)
{
	signed short pos = *item_pos;

	if (g_action == ACTION_ID_UP) {

		/* Key UP */
		if (pos) {
			pos--;
		} else {
			pos = 14;
			while (shop_item[pos].item_id == 0) {
				pos--;
			}
		}

	} else if (g_action == ACTION_ID_DOWN) {

		/* Key DOWN */
		if (pos < 14) {
			if (shop_item[pos + 1].item_id != 0) {
				pos++;
			} else {
				pos = 0;
			}
		} else {
			pos = 0;
		}

	} else if (g_action == ACTION_ID_RIGHT) {

		/* Key RIGHT */
		if (pos < 10) {
			if (shop_item[pos + 5].item_id != 0) {
				pos += 5;
			}
		} else {
			pos -= 10;
		}

	} else if (g_action == ACTION_ID_LEFT) {

		/* Key LEFT */
		if (pos > 4) {
			pos -= 5;
		} else {
			if (shop_item[pos + 10].item_id != 0) {
				pos += 10;
			}
		}
	}

	*item_pos = pos;
}

/**
 * \brief   marks a tile in the automap as seen
 *
 * \param   x           X coordinate
 * \param   y           Y coordinate
 */
void set_automap_tile(signed short x, signed short y)
{
	g_automap_buf[4 * y + (x >> 3)] |= g_automap_bitmask[x & 0x07];
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

		if (g_dng_map_size - 1 > x) {
			set_automap_tile(x + 1, y - 1);
		}
	}

	/* set middle line */
	if (x > 0) {
		set_automap_tile(x - 1, y);
	}

	set_automap_tile(x, y);

	if (g_dng_map_size - 1 > x) {
		set_automap_tile(x + 1, y);
	}

	/* set lower line */
	if (y < 15) {
		if (x > 0) {
			set_automap_tile(x - 1, y + 1);
		}

		set_automap_tile(x, y + 1);

		if (g_dng_map_size - 1 > x) {
			set_automap_tile(x + 1, y + 1);
		}
	}
}

/**
 * \brief   */
void seg002_47e2(void)
{
	/* save gfx settings to stack */
	struct struct_pic_copy pic_copy_bak = g_pic_copy;

	/* set range 0,0 - 7,7 */
	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 7;
	g_pic_copy.y2 = 7;

	/* set destination */
	g_pic_copy.dst = g_vga_memstart;
	/* set source */
	g_pic_copy.src = g_gfxbuf_wait_keypress;

	do_save_rect();

	GUI_print_char('P', 0, 0);

	/* restore gfx settings from stack */
	g_pic_copy = pic_copy_bak;
}

/**
 */
void seg002_484f(void)
{
	/* save gfx settings to stack */
	struct struct_pic_copy pic_copy_bak = g_pic_copy;

	/* set range 0,0 - 7,7 */
	g_pic_copy.x1 = 0;
	g_pic_copy.y1 = 0;
	g_pic_copy.x2 = 7;
	g_pic_copy.y2 = 7;

	/* set destination */
	g_pic_copy.dst = g_vga_memstart;
	/* set source */
	g_pic_copy.src = g_gfxbuf_wait_keypress;

	do_pic_copy(0);

	/* restore gfx settings from stack */
	g_pic_copy = pic_copy_bak;
}

/**
 * \brief   returns true if the hero is not sleeping, dead, petrified, unconscious, renegade or fleeing
 *
 * \param   hero        pointer to the hero
 * \return              {0, 1}
 */
signed int check_hero(const struct struct_hero *hero)
{
	if (!hero->typus || hero->flags.asleep || hero->flags.dead || hero->flags.petrified ||
		hero->flags.unconscious || hero->flags.renegade || (hero->action_id == FIG_ACTION_FLEE))
	{
		return 0;
	}

	return 1;
}

/**
 * \brief   returns true if the hero is not dead, petrified, unconscious or renegade
 */
/* should be static */
signed int check_hero_no2(const struct struct_hero *hero)
{

	if (!hero->typus || hero->flags.dead || hero->flags.petrified || hero->flags.unconscious || hero->flags.renegade)
	{
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
signed int check_hero_no3(const struct struct_hero *hero)
{
	if (!hero->typus || hero->flags.dead || hero->flags.petrified || hero->flags.unconscious)
	{
		return 0;
	}

	return 1;
}

signed int is_hero_available_in_group(const struct struct_hero *hero)
{
	if (check_hero(hero) &&	(hero->group_no == gs_current_group)) {

		return 1;
	}

	return 0;
}

/**
 * \brief   subtract current ae with a splash
 *
 * \param   hero        the magicuser
 * \param   ae_cost     astralenergy to subtract
 */
void sub_ae_splash(struct struct_hero *hero, signed int ae_cost)
{
	if (!hero->flags.dead && (ae_cost > 0)) {

		signed short tmp = g_update_statusline;
		g_update_statusline = 0;

		if ((hero->typus == HERO_TYPE_MAGE) && (hero->staff_level >= 4)) {
			/* 4th staff spell reduces AE cost by 2 */
			ae_cost -= 2;
			if (ae_cost < 0)
				ae_cost = 0;
		}

		/* Calc new AE */
		hero->ae -= ae_cost;

		/* Draw the splash */
		draw_splash(get_hero_index(hero), 1);

		/* set AE to 0 if they have gotten lower than 0 */
		if (hero->ae < 0) {
			hero->ae = 0;
		}

		g_update_statusline = tmp;

#ifdef M302de_ORIGINAL_BUGFIX
		/* AE bar was not updated in pseudo 3D mode */
		if (!g_in_fight && g_mouse1_doubleclick) {
			/* redraw AE bar */
			draw_bar(1, get_hero_index(hero), hero->ae, hero->ae_max, 0);
		}
#endif
	}
}

/**
 * \brief   add AE points to the current AE of a hero.
 */
void add_hero_ae(struct struct_hero* hero, const signed int ae)
{
	/* dont add AE if hero is dead or ae = 0 */
	if (!hero->flags.dead && (ae > 0)) {

		signed short tmp = g_update_statusline;
		g_update_statusline = 0;

		/* add AE to hero's current AE */
		hero->ae += ae;

		/* if current AE is greater than AE maximum set current AE to AE maximum */
		if (hero->ae > hero->ae_max) {

			hero->ae = hero->ae_max;
		}

		g_update_statusline = tmp;
	}
}

/**
 * \brief   subtracts LE from a hero
 *
 * \param   hero        pointer to the hero
 * \param   le          LE the hero looses
 */
void sub_hero_le(struct struct_hero *hero, const signed short le)
{
	signed short i;
	signed short bak;
	signed short old_le;
	struct struct_fighter *fighter;
	struct struct_hero *hero_i;

	if (!hero->flags.dead && (le > 0)) {

		bak = g_update_statusline;
		g_update_statusline = 0;

		/* do the damage */
		old_le = hero->le;
		hero->le -= le;

		if (hero->flags.asleep) {

			/* awake him/her */
			hero->flags.asleep = 0;

			/* in fight mode */
			if (g_in_fight) {

				fighter = FIG_get_fighter(hero->fighter_id);

				/* update looking dir and other  */
				fighter->nvf_no = hero->viewdir;
				fighter->reload = -1;
				fighter->offsetx = 0;
				fighter->offsety = 0;
			}
		}

		draw_splash(get_hero_index(hero), 0);

		if (hero->le <= 0) {

			/* set LE to 0 */
			hero->le = 0;

			/* mark hero as dead */
			hero->flags.dead = 1;

			gs_unconscious_message[get_hero_index(hero)] = 0;

			/* unknown */
			hero->action_id = FIG_ACTION_UNKNOWN2;

			if (g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {
				g_refresh_status_line = 1;
			}

			/* reset sickness */
			for (i = 1; i <= 7; i++) {
				hero->sick[i][0] = 0;
				hero->sick[i][1] = 0;
			}

			/* reset poison */
			for (i = 1; i <= 9; i++) {
				hero->poison[i][0] = 0;
				hero->poison[i][1] = 0;
			}

			/* FINAL FIGHT */
			if (g_current_fight_no == FIGHTS_F144) {
				if (hero == gs_main_acting_hero) {
					g_game_state = GAME_STATE_DEAD;
					g_in_fight = 0;
				}
			}

			if (g_traveling	&& !g_in_fight &&
				(!count_heroes_available_in_group() || ((count_heroes_available_in_group() == 1) && is_hero_available_in_group(get_hero(6))))) /* count_heroes_available_in_group_ignore_npc() == 0 */
			{
				/* if traveling, not in a fight, and no hero in the group (except possibly the NPC) is available. */

				gs_travel_detour = 99;

				hero_i = get_hero(0);
				for (i = 0; i <= 6; i++, hero_i++) {

					if ((hero_i->typus != HERO_TYPE_NONE) && (hero_i->group_no == gs_current_group))
					{
						hero_disappear(hero_i, i, -1);
					}
				}
			}

		} else {
			if ((old_le >= 5) && (hero->le < 5)) {

				/* make hero unsonscious */
				hero->flags.unconscious = 1;

				/* unknown yet */
				hero->action_id = FIG_ACTION_WAIT;

				/* unknown yet */
				gs_unconscious_message[get_hero_index(hero)] = 1;

				/* in fight mode */
				if (g_in_fight) {

					fighter = FIG_get_fighter(hero->fighter_id);

					fighter->nvf_no = g_nvftab_figures_unconscious[hero->sprite_no] + hero->viewdir;

					fighter->reload = -1;

					fighter->offsetx = g_gfxtab_offsets_unconscious[hero->sprite_no][hero->viewdir].x;
					fighter->offsety = g_gfxtab_offsets_unconscious[hero->sprite_no][hero->viewdir].y;


					FIG_add_msg(7, 0);

					/* FINAL FIGHT */
					if (g_current_fight_no == FIGHTS_F144) {

						if (hero == gs_main_acting_hero) {

							g_game_state = GAME_STATE_DEAD;

							g_in_fight = 0;
						}
					}
				}
			}
		}

		g_update_statusline = bak;
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
void add_hero_le(struct struct_hero *hero, const signed short le)
{
	signed short val_bak;
	struct struct_fighter *fighter;
	signed short ret;

	/* dead heroes never get LE */
	if (!hero->flags.dead && (le > 0)) {

		val_bak = g_update_statusline;
		g_update_statusline = 0;

		/* add LE */
		hero->le += le;

		/* set LE to maximum if greater than maximum */
		if (hero->le > hero->le_max) {

			hero->le = hero->le_max;
		}

		/* if current LE is >= 5 and the hero is unconscious */
		if ((hero->le >= 5) && hero->flags.unconscious) {

			/* awake */
			hero->flags.unconscious = 0;

			/* maybe if we are in a fight */
			if (g_in_fight) {

				fighter = FIG_get_fighter(hero->fighter_id);

				ret = FIG_get_range_weapon_type(hero);

				if (ret != -1) {

					fighter->nvf_no = g_nvftab_figures_rangeweapon[hero->sprite_no - 1][ret][hero->viewdir];
				} else {
					fighter->nvf_no = hero->viewdir;
				}

				fighter->reload = -1;
				fighter->offsetx = 0;
				fighter->offsety = 0;
			}
		}

		g_update_statusline = val_bak;
	}
}

/**
 * \brief   regenerates LE of a group
 *
 * \param   le          LE to be regenerated
 */
void add_group_le(signed short le)
{
	struct struct_hero *hero;
	signed short i;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

		if ((hero->typus != HERO_TYPE_NONE) && (hero->group_no == gs_current_group))
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
void do_starve_damage(struct struct_hero *hero, const signed int index, const signed int type)
{
	/* check if the hero is dead */
	if (!hero->flags.dead) {

		/* save this value locally */
		const int sl_bak = g_update_statusline;
		g_update_statusline = 0;

		/* decrement LE of the hero */
		hero->le--;

		/* set the critical message type for the hero */
		gs_food_message[index] = (type != 0 ? 1 : 2);

		if (hero->le <= 0) {

			/* don't let the hero die */
			hero->le = 1;

			/* decrement the max LE and save them at 0x7a */
			if (hero->le_max >= 2) {
				hero->le_max--;
				hero->le_malus++;
			}
		}

		/* restore the locally save value */
		g_update_statusline = sl_bak;
	}
}

#if defined(__BORLANDC__)
/* unused */
signed short compare_name(char *name)
{
	signed int i;

	for (i = 0; i < 6; i++) {

		if (!strcmp(get_hero(i)->alias, name)) {
			return 1;
		}
	}

	return 0;
}
#endif

/**
 * \brief   make an attribute test
 *
 * \param   hero        pointer to the hero
 * \param   attrib      number of the attribute
 * \param   handicap    may be positive or negative. The higher the value, the harder the test.
 * \return              the result of the test. > 0: success; <= 0: failure; -99: critical failure
 */
signed int test_attrib(const struct struct_hero* hero, const signed int attrib_id, const signed int handicap)
{
	signed int randval = random_schick(20);
	signed int att_val;

#if !defined(__BORLANDC__)
	D1_INFO("Eigenschaftsprobe %s auf %s %+d: W20 = %d", hero->alias, names_attrib[attrib_id], handicap, randval);
#endif

	if (randval == 20) {
#if !defined(__BORLANDC__)
		D1_INFO("Ungluecklich\n");
#endif
		return -99;
	} else {

		randval += handicap;
	}

	att_val = hero->attrib[attrib_id].current + hero->attrib[attrib_id].mod;

#if !defined(__BORLANDC__)
	D1_INFO(" -> %s mit %d\n", (att_val - randval + 1) > 0 ? "bestanden" : "nicht bestanden", (att_val - randval + 1));
#endif

	return att_val - randval + 1;
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

signed int test_attrib3(const struct struct_hero* hero, const signed int attrib1, const signed int attrib2, const signed int attrib3, signed char handicap)
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
	D1_INFO(" (%s/%s/%s) %+d -> ", names_attrib[attrib1], names_attrib[attrib2], names_attrib[attrib3], handicap);
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

	tmp = hero->attrib[attrib1].current + hero->attrib[attrib1].mod +
		hero->attrib[attrib2].current + hero->attrib[attrib2].mod +
		hero->attrib[attrib3].current + hero->attrib[attrib3].mod;

#if !defined(__BORLANDC__)
	D1_INFO(" -> %s mit %d\n", (tmp - rolls_sum + 1) > 0 ? "bestanden" : "nicht bestanden", (tmp - rolls_sum + 1));
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

	attrib[0] = hero->attrib[attrib1].current + hero->attrib[attrib1].mod;
	attrib[1] = hero->attrib[attrib2].current + hero->attrib[attrib2].mod;
	attrib[2] = hero->attrib[attrib3].current + hero->attrib[attrib3].mod;

#if !defined(__BORLANDC__)
	D1_INFO(" (%s %d/%s %d/%s %d) ->",
			names_attrib[attrib1], attrib[0],
			names_attrib[attrib2], attrib[1],
			names_attrib[attrib3], attrib[2]
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

	} while (!get_hero(l_si)->typus || (get_hero(l_si)->group_no != gs_current_group));

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
	signed int cur_hero;

	do {
		/* get number of current group */
		cur_hero = random_schick(gs_group_member_counts[gs_current_group]) - 1;

#ifdef M302de_ORIGINAL_BUGFIX
		signed short pos = 0;

		struct struct_hero *hero = get_hero(0);
		for (int i = 0; i <= 6; i++, hero++) {

			if (hero->typus == HERO_TYPE_NONE)
				continue;
			/* Check if in current group */
			if (hero->group_no != gs_current_group)
				continue;

			if (pos == cur_hero) {
				pos = i;
				break;
			}

			pos++;
		}
		cur_hero = pos;
#endif

	} while (!get_hero(cur_hero)->typus || (get_hero(cur_hero)->group_no != gs_current_group) || get_hero(cur_hero)->flags.dead);

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
	struct struct_hero *hero = get_hero(0);

	for (i = 0; i < 6; i++, hero++) {

		if (hero->typus && (hero->group_no == gs_current_group))
		{
			sum += hero->money;
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
	struct struct_hero *hero;

	if (money < 0)
		money = 0;

	heroes = count_heroes_in_group();

	/* set hero to NPC */
	hero = get_hero(6);

	/* if we have an NPC in current group and alive */
	if (hero->typus && (hero->group_no == gs_current_group) && !hero->flags.dead) {

		/* If only the NPC is in that group give him all the money */
		if (heroes > 1) {
			heroes--;
		} else {
			hero->money += money;
			heroes = 0;
		}
	}

	if (heroes != 0) {

		hero_money = money / heroes;

		hero = get_hero(0);

		for (i = 0; i < 6; i++, hero++) {

			if (hero->typus && (hero->group_no == gs_current_group) && !hero->flags.dead) {

				/* account the money to hero */
				hero->money = hero_money;
			} else {
				if (hero->group_no == gs_current_group) {
					hero->money = 0;
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
void add_hero_ap(struct struct_hero *hero, Bit32s ap)
{
	hero->ap += ap;
}

/**
 * \brief   shares AP in the current group
 *
 * \param   ap          AP to share
 */
void add_group_ap(Bit32s ap)
{
	signed short i;
	struct struct_hero *hero;

	if (ap < 0) {
		return;
	}

	ap = ap / count_heroes_in_group();

	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		if (hero->typus && (hero->group_no == gs_current_group) && !hero->flags.dead)
		{
			add_hero_ap(hero, ap);
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
	struct struct_hero *hero;
	signed short i;

	if (ap < 0)
		return;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

		if (hero->typus && (hero->group_no == gs_current_group) && !hero->flags.dead)
		{
#if !defined(__BORLANDC__)
			D1_INFO("%s erhaelt %d AP\n", hero->alias, ap);
#endif

			add_hero_ap(hero, ap);
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
	struct struct_hero *hero;

	if (ap < 0)
		return;

	hero = get_hero(0);
	for (i = 0; i <= 6; i++, hero++) {

		if (hero->typus && (hero->group_no == gs_current_group) && !hero->flags.dead)
		{
			if ((Bit32u)ap <= hero->ap) {
#if !defined(__BORLANDC__)
				D1_INFO("%s erhaelt %+d AP\n", hero->alias, -ap);
#endif
				add_hero_ap(hero, -((Bit32s)ap));
			} else {
#if !defined(__BORLANDC__)
				D1_INFO("%s wird auf 0 AP gesetzt\n", hero->alias);
#endif
				hero->ap = 0;
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
signed int get_hero_index(const struct struct_hero *hero)
{
	signed short i = 0;
	struct struct_hero *p;

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
signed int get_item_pos(struct struct_hero *hero, const signed int item_id)
{
	signed int i;

	for (i = 0; i < NR_HERO_INVENTORY_SLOTS; i++) {
		if (item_id == hero->inventory[i].item_id) {
			return i;
		}
	}

	return -1;
}

/**
 * \brief   gets the position of the first hero with an item
 *
 * \param   item_id     item ID to look for
 * \return              position of the hero or -1 if nobody of the group has this item
 */
signed short get_first_hero_with_item(signed short item_id)
{
	signed int j;
	signed int i;
	struct struct_hero *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if ((hero_i->typus) && (hero_i->group_no == gs_current_group))
		{
			/* Search inventory */
			for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {
				if (hero_i->inventory[j].item_id == item_id) {
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
 * \param   item_id     item ID to look for
 * \param   group       group number
 * \return              position of the hero or -1 if nobody in the specified group has this item
 */
signed short get_first_hero_with_item_in_group(signed short item_id, signed short group)
{
	signed int j;
	signed int i;
	struct struct_hero *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		if ((hero_i->typus) && (hero_i->group_no == (signed char)group))
		{
			/* Search inventory */
			for (j = 0; j < NR_HERO_INVENTORY_SLOTS; j++) {
				if (hero_i->inventory[j].item_id == item_id) {
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
	struct struct_hero *hero_i;

	for (i = 0; i <= 6; i++) {

		hero_i = get_hero(i);

		if (hero_i->typus && (hero_i->group_no == gs_current_group))
		{
			sub_hero_le(hero_i, le);
		}
	}
}

/**
 * \brief   return a pointer to the first available hero
 *
 * \return a pointer to the first available hero. If none in available it returns a pointer to the first hero.
 */
struct struct_hero* get_first_hero_available_in_group(void)
{
	signed short i;
	struct struct_hero *hero_i = get_hero(0);

	for (i = 0; i <= 6; i++, hero_i++) {

		/* Check class, group, deadness and check_hero() */
		if (hero_i->typus && (hero_i->group_no == gs_current_group) &&
			!hero_i->flags.dead && check_hero(hero_i))
		{
			return hero_i;
		}
	}

	return get_hero(0);
}

/**
 * \brief   return a pointer to the second available hero in the group
 *
 * \return              a pointer to the second available hero in the group or NULL.
 */
struct struct_hero* get_second_hero_available_in_group(void)
{
	signed short i;
	signed short tmp;
	struct struct_hero *hero_i;

	hero_i = get_hero(0);

	for (i = tmp = 0; i <= 6; i++, hero_i++) {

		/* Check class, group and check_hero() */
		if ((hero_i->typus) && (hero_i->group_no == gs_current_group) && check_hero(hero_i))
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
	struct struct_hero *hero;

	retval = 0;
	hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {

		/* Check if hero is available */
		if (hero->typus && (check_hero(hero) || check_hero_no2(hero)))
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
	struct struct_hero *hero;

	retval = 0;
	hero = get_hero(0);

	for (i = 0; i < 6; i++, hero++) {
		/* Check if hero is available */
		if (hero->typus && (check_hero(hero) || check_hero_no2(hero)))
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
	struct struct_hero *hero = get_hero(0);

	for (i = 0; i <= 6; i++, hero++) {
		if (hero->typus && (hero->group_no == gs_current_group) && check_hero_no2(hero))
		{
			/* hero not dead, petrified, unconscious or renegade */
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
	struct struct_hero *hero = get_hero(0);

	for (i = 0; i < 6; i++, hero++) {

		if (hero->typus && (hero->group_no == gs_current_group) && check_hero_no2(hero))
		{
			/* hero not dead, petrified, unconscious or renegade */
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
		g_game_state = (GAME_STATE_DEAD);

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

int main(int argc, char** argv)
{
	signed short l_si;
	signed short l_di;
	Bit32s l3;
	signed short savegame;
	signed short len;

	g_pregame_state = 1;

	g_playmask_us = 1;

	init_AIL(16000);

#if defined(__BORLANDC__)
	randomize();

	save_display_stat((Bit8u*)&g_video_page_bak);
#endif

	if (!init_memory()) {

#if defined(__BORLANDC__)
		g_gameinit_flag = 1;
#endif

		schick_set_video();

		g_have_mouse = 2;

		mouse_init();

		if (g_have_mouse == 0) {
			g_mouse_refresh_flag = -10;
		}

		init_game_state();

		save_and_set_timer();

		init_common_buffers();

		g_textbox_width = 3;

		refresh_screen_size();

		if (argc == 2) {

#if defined(__BORLANDC__)
			/* some trick to disable the cd check */

			len = strlen(argv[1]);

			l_si = 0;

			g_cd_skipmagic = 1;
			while (l_si < len) {


				g_cd_skipmagic = argv[1][0] * g_cd_skipmagic;
				argv[1]++;
				l_si++;
			}
#endif
		}

		prepare_dirs();

		if (have_mem_for_sound()) {

			read_sound_cfg();
			alloc_voc_buffer(20000);

		} else {
			/* disable sound */
			exit_AIL();
			GUI_output(g_snd_txt_disabled_mem2);
		}

		CD_init();

#if defined(__BORLANDC__)
		if (!g_cd_init_successful) {

			/* CD init failed */
			cleanup_game();
			exit(0);
		}
#endif

		/* select game mode */
		g_game_mode = GAME_MODE_UNSPECIFIED;

		while (g_game_mode == GAME_MODE_UNSPECIFIED) {
			g_game_mode = GUI_radio(get_ttx(5), 2, get_ttx(6), get_ttx(7));
		}

		if (copy_protection()) {

			g_textbox_width = 3;

			l3 = get_diskspace();

			if (l3 < 0) {

				sprintf(g_dtp2, get_ttx(807), -l3);
				GUI_output(g_dtp2);
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

				g_pregame_state = 0;

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
			sprintf(g_dtp2, get_tx(39),
				g_questions_handbook[randval][3],
				g_questions_handbook[randval][2],
				g_questions_handbook[randval][1],
				g_questions_handbook[randval][0]);

			/* print version number */
			GUI_print_string(g_game_version, 290, 190);

			/* ask the question */
			GUI_input(g_dtp2, 20);

			len = strlen(g_text_input_buf);

			/* transform the input string in uppercase letters and bitwise invert them */
			for (i = 0; i < len; i++) {
				g_text_input_buf[i] = ~toupper(g_text_input_buf[i]);
			}

			if (!strcmp((char*)&g_questions_handbook[randval][4], g_text_input_buf)) {
				return 1;
			}
		} else {
			/* map question */

			/* select a question */
			randval = random_schick(10) - 1;

			/* prepare the string */
			sprintf(g_dtp2, get_tx(40),
				get_tx(41 + g_questions_map[randval][0]),
				get_ttx(235 + g_questions_map[randval][1]));

			/* print version number */
			GUI_print_string(g_game_version, 290, 190);

			/* ask the question */
			GUI_input(g_dtp2, 20);

			len = strlen(g_text_input_buf);

			/* transform the input string in uppercase letters */
			for (i = 0; i < len; i++) {
				g_text_input_buf[i] = toupper(g_text_input_buf[i]);
			}

			if (!strcmp(get_ttx(235 + g_questions_map[randval][2]), g_text_input_buf)) {
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

#if !defined(__BORLANDC__)
/* REAMARK: reason == namespaces */
int main(int argc, char** argv)
{
	return M302de::main(argc, argv);
}
#endif
