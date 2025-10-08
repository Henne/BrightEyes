/**
 *	Rewrite of DSA1 v3.02_de functions of seg120 (misc: rabies, game init, DOS-related)
 *	Functions rewritten: 11/11 (complete)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#if defined(__BORLANDC__)
#include <ALLOC.H>
#include <CONIO.H>
#include <DIR.H>
#include <DOS.H>
#include <IO.H>
#include <PROCESS.H>
#else
#include <unistd.h>
#endif

#include "v302de.h"
#include "common.h"

#include "seg001.h"
#include "seg002.h"
#include "seg004.h"
#include "seg007.h"
#include "seg008.h"
#include "seg010.h"
#include "seg026.h"
#include "seg027.h"
#include "seg028.h"
#include "seg029.h"
#include "seg047.h"
#include "seg049.h"
#include "seg063.h"
#include "seg096.h"
#include "seg097.h"
#include "seg098.h"
#include "seg104.h"
#include "seg120.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static Bit8u g_color_white[3] = { 0x3f, 0x3f, 0x3f }; // ds:0xb22a
Bit8u g_color_black[3] = { 0, 0, 0 }; // ds:0xb22d
static Bit8u g_palette_unknown2[8][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x38, 0x30, 0x28 },
	{ 0x38, 0x30, 0x10 },
	{ 0x30, 0x28, 0x0c },
	{ 0x2c, 0x24, 0x08 },
	{ 0x28, 0x20, 0x04 },
	{ 0x18, 0x14, 0x00 },
	{ 0x0b, 0x19, 0x0c }
}; // ds:0xb230
static Bit8u g_palette_unknown3[3][3] = {
	{ 0x30, 0x00, 0x00 },
	{ 0x30, 0x30, 0x00 },
	{ 0x00, 0x00, 0x30 }
}; // ds:0xb248
static Bit8u g_palette_unknown4[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x38, 0x30, 0x28 },
	{ 0x30, 0x24, 0x1c },
	{ 0x24, 0x18, 0x10 },
	{ 0x20, 0x14, 0x0c },
	{ 0x18, 0x0c, 0x08 },
	{ 0x14, 0x08, 0x04 },
	{ 0x0c, 0x04, 0x00 },
	{ 0x3c, 0x38, 0x00 },
	{ 0x38, 0x28, 0x00 },
	{ 0x30, 0x1c, 0x00 },
	{ 0x2c, 0x14, 0x00 },
	{ 0x3c, 0x24, 0x00 },
	{ 0x3c, 0x0c, 0x00 },
	{ 0x3c, 0x00, 0x08 },
	{ 0x3c, 0x00, 0x20 },
	{ 0x00, 0x10, 0x00 },
	{ 0x00, 0x14, 0x00 },
	{ 0x00, 0x1c, 0x00 },
	{ 0x04, 0x20, 0x00 },
	{ 0x04, 0x28, 0x00 },
	{ 0x1c, 0x38, 0x3c },
	{ 0x14, 0x2c, 0x34 },
	{ 0x0c, 0x1c, 0x30 },
	{ 0x04, 0x0c, 0x28 },
	{ 0x00, 0x00, 0x24 },
	{ 0x10, 0x10, 0x10 },
	{ 0x14, 0x14, 0x14 },
	{ 0x1c, 0x1c, 0x1c },
	{ 0x24, 0x24, 0x24 },
	{ 0x30, 0x30, 0x30 },
	{ 0x3c, 0x3c, 0x3c }
}; // ds:0xb251
static Bit8u g_palette_general[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x3f },
	{ 0x00, 0x00, 0x24 },
	{ 0x3c, 0x30, 0x28 },
	{ 0x3c, 0x2c, 0x24 },
	{ 0x3c, 0x28, 0x1c },
	{ 0x38, 0x24, 0x18 },
	{ 0x34, 0x20, 0x14 },
	{ 0x30, 0x1c, 0x14 },
	{ 0x28, 0x18, 0x10 },
	{ 0x24, 0x14, 0x0c },
	{ 0x1c, 0x10, 0x0c },
	{ 0x18, 0x0c, 0x08 },
	{ 0x14, 0x0c, 0x08 },
	{ 0x38, 0x38, 0x38 },
	{ 0x30, 0x30, 0x30 },
	{ 0x2c, 0x2c, 0x2c },
	{ 0x28, 0x28, 0x28 },
	{ 0x20, 0x20, 0x20 },
	{ 0x1c, 0x1c, 0x1c },
	{ 0x14, 0x14, 0x14 },
	{ 0x10, 0x10, 0x10 },
	{ 0x00, 0x3f, 0x00 },
	{ 0x00, 0x24, 0x00 },
	{ 0x3c, 0x38, 0x00 },
	{ 0x34, 0x2c, 0x00 },
	{ 0x28, 0x20, 0x00 },
	{ 0x3c, 0x18, 0x10 },
	{ 0x30, 0x10, 0x08 },
	{ 0x24, 0x0c, 0x04 },
	{ 0x10, 0x08, 0x04 },
	{ 0x3c, 0x3c, 0x3c }
}; // ds:0xb2b1
static char g_str_backslash_temp[6] = "\\TEMP"; // ds:0xb311
static char g_str_not_enough_mem[124] = "\x0a\x0aNot enough memory!\x0a\"Realms of Arkania - Blade of Destiny\" needs %ld Byte more memory!\x0aPlease uninstall resident programs!"; // ds:0xb317
static char g_str_drive_x[4] = "X:\\"; // ds:0xb393
static char g_str_temp_dir_fail[280] = "\xf2!!!!! ACHTUNG !!!!!\xf0\x40\x40""DAS SCHWARZE AUGE\x40KANN KEIN TEMPOR\x8eRES VERZEICHNIS ANLEGEN UND IST DAHER NICHT LAUFF\x8eHIG. WAHRSCHEINLICH VERSUCHEN SIE, DAS SPIEL AUF EINEM\xf2 SCHREIBGESCH\x9aTZTEN LAUFWERK\xf0\x40ZU STARTEN. BITTE INSTALLIEREN SIE DAS SPIEL AUF EINE FESTPLATTE.\x40\x40-TASTE ZUM BEENDEN-"; // ds:0xb397
static char g_all_files_wildcard2[4] = "*.*"; // ds:0xb4af
static char g_all_chr_wildcard4[6] = "*.CHR"; // ds:0xb4b3
static char g_all_files_wildcard3[4] = "*.*"; // ds:0xb4b9
static char g_str_gen_exe[8] = "gen.exe"; // ds:0xb4bd
static char g_str_gen_exe2[8] = "gen.exe"; // ds:0xb4c5
static const char g_str_gen_b[2] = "b"; // ds:0xb4cd
static const char g_str_gen_a[2] = "a"; // ds:0xb4cf
static const char g_str_gen_n[2] = "n"; // ds:0xb4d1
static const char g_str_gen_1[2] = "1"; // ds:0xb4d3
static const char g_str_gen_generation[11] = "Generation"; // ds:0xb4d5


static Bit32u g_buffersize;		// ds:0xe5dc, size of the global buffer
static HugePt g_global_buffer_ptr;	// ds:0xe5e0, points to the start of the global buffer
signed char g_large_buf; 		// ds:0xe5e4

/* Borlandified and identical */
void rabies(struct struct_hero* hero, signed short hero_pos)
{
	signed short answer;
	signed short l_di;
	signed short done;
	signed short tw_bak;
	struct struct_hero *hero2;
	signed short group_bak;
	signed short group_no;
	signed char sex_bak;

	done = 0;

	group_bak = gs_current_group;
	sex_bak = hero->sex;
	group_no = hero->group_no;

	/* TODO : Sex = 50, what means 50 ? */
	hero->sex = 50;

	/* switch to the group of the hero */
	while (gs_current_group != group_no) {
		GRP_switch_to_next(1);
	}

	hero_pos = 0;
	while (((struct struct_hero*)get_hero(hero_pos))->sex != 50) {
		hero_pos++;
	}

	hero = (struct struct_hero*)get_hero(hero_pos);
	hero->sex = sex_bak;

	if (g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {
		draw_status_line();
	}

	while (done == 0) {

		if (count_heroes_available_in_group() > 1) {

			sprintf(g_dtp2, get_ttx(741), hero->alias, GUI_get_ptr(hero->sex, 2), GUI_get_ptr(hero->sex, 2));

			sprintf(g_dtp2 + 500, get_ttx(742), hero->alias);

			sprintf(g_dtp2 + 600, get_ttx(743), hero->alias);

			tw_bak = g_textbox_width;
			g_textbox_width = 6;

			answer = GUI_dialogbox(hero->pic, hero->alias,
						g_dtp2, 3, g_dtp2 + 500, g_dtp2 + 600, get_ttx(744));

			g_textbox_width = tw_bak;

			if (answer == 1) {
				/* knock the infected hero out */

				sub_hero_le(hero, hero->le / 2);

				sprintf(g_dtp2, get_ttx(745), hero->alias);

				GUI_output(g_dtp2);

				g_hero_sel_exclude = (signed char)hero_pos;

				answer = select_hero_ok(get_ttx(395));

				if (answer != -1) {

					skill_cure_disease((struct struct_hero*)get_hero(answer), hero, 10, 1);
				}

				done = 1;

			} else if (answer == 2) {
				/* calm the hero */

				for (l_di = 0; l_di <= 6; l_di++) {

					/* one of the other heroes must pass CH+0 */
					if ((l_di != hero_pos) &&
						(test_attrib((struct struct_hero*)get_hero(l_di), ATTRIB_CH, 0) != 0))
						/* Original-Bug: should be 'test_attrib((struct struct_hero*)get_hero(l_di), ATTRIB_CH, 0) > 0'
						 * (found by siebenstreich 2021-08-15) */
					{
						done = 1;
						sprintf(g_dtp2, get_ttx(746), hero->alias);

						GUI_output(g_dtp2);

						g_hero_sel_exclude = (signed char)hero_pos;

						answer = select_hero_ok(get_ttx(395));

						if (answer != -1) {
							skill_cure_disease((struct struct_hero*)get_hero(answer), hero, 10, 1);
						}
						break;
					}
				}
			} else if (answer == 3) {
				/* cast a spell */

				g_hero_sel_exclude = (signed char)hero_pos;

				answer = select_hero_ok(get_ttx(213));

				if (answer != -1) {

					hero2 = (struct struct_hero*)get_hero(answer);

					/* check that hero2 is a magic user */
					if (hero2->typus >= HERO_TYPE_WITCH) {

						/* need 15 AE */
						if (hero2->ae >= 15) {

							/* spell must succeed */
							if (test_spell(hero2, SP_SANFTMUT, 0)) {

								done = 1;

								sub_ae_splash(hero2, 15);

								sprintf(g_dtp2, get_ttx(746), hero->alias);

								GUI_output(g_dtp2);

								g_hero_sel_exclude = (signed char)hero_pos;

								answer = select_hero_ok(get_ttx(395));

								if ((answer != -1) && (answer != hero_pos)) {
									skill_cure_disease((struct struct_hero*)get_hero(answer), hero, 10, 1);
								}
							}
						} else {
							sprintf(g_dtp2, get_ttx(607), hero2->alias);
							GUI_output(g_dtp2);
						}
					}
				}
			}
		} else {

			/* Hero has rabies / Tollwut */
			sprintf(g_dtp2, get_ttx(747), hero->alias);
			GUI_output(g_dtp2);

			done = 1;
		}

		if (done == 0) {
			/* every other hero in the group looses 1W6+2 LE */
			hero2 = (struct struct_hero*)get_hero(0);
			for (l_di = 0; l_di <= 6; l_di++, hero2++) {

				if ((l_di != hero_pos) && (hero2->typus != HERO_TYPE_NONE) &&
					(hero2->group_no == gs_current_group) && !hero_dead((Bit8u*)hero2))
				{
					sub_hero_le(hero2, dice_roll(1, 6, 2));
				}
			}

			/* hero has berserker fury / Berserkerwut */
			sprintf(g_dtp2, get_ttx(791), hero->alias);
			GUI_output(g_dtp2);

			done = 1;
		}
	}

	/* switch back to the group */
	while (gs_current_group != group_bak) {
		GRP_switch_to_next(1);
	}

	if (g_pp20_index == ARCHIVE_FILE_PLAYM_UK) {
		draw_status_line();
	}
}

/* Borlandified and identical */
void init_global_buffer(void)
{
	g_global_buffer_ptr = (HugePt)schick_alloc(g_buffersize);
	g_renderbuf_ptr = g_global_buffer_ptr + 8L;

	g_text_ltx_buffer = (char*)(((HugePt)g_renderbuf_ptr) + 65000L);

	g_text_ltx_index = (char**)(((HugePt)g_text_ltx_buffer) + 30500L);
	g_tx_index = &g_text_ltx_index[840];
	g_tx2_index = &g_text_ltx_index[990];

	g_objects_nvf_buf = (((HugePt)g_text_ltx_index) + 4760L);
	g_dtp2 = (char*)(((HugePt)g_objects_nvf_buf) + 3400L);
	g_text_input_buf = (char*)(g_dtp2 + 1500);
	g_text_output_buf = (char*)(((HugePt)g_dtp2) + 1524L);
	g_buffer5_ptr = (char*)(((HugePt)g_text_output_buf) + 300L);
	g_buffer6_ptr = (unsigned char*)(((HugePt)g_buffer5_ptr) + 3880L);
	g_buffer7_ptr = (char*)(((HugePt)g_buffer6_ptr) + 2200L);
	g_buffer8_ptr = (unsigned char*)(((HugePt)g_buffer7_ptr) + 10000L);
	g_buffer9_ptr = (HugePt)(((HugePt)g_buffer7_ptr) + 22008L);

	g_buffer9_ptr3 = (HugePt)(g_buffer9_ptr2 = (unsigned char*)g_buffer9_ptr);
	g_ani_unknown4 = 0;
}

/* Borlandified and identical */
signed short init_memory(void)
{
	signed short error = 0;
	Bit32u freemem;

	/* disable EMS memory */
	g_ems_enabled = 0;

#if defined(__BORLANDC__)
	if (EMS_init()) {
		_OvrInitEms(0, 0, 0);
	} else {
		;
	}

	/* set the pointer for the framebuffer */
	g_vga_backbuffer = g_vga_memstart = (unsigned char*)MK_FP(0x0a000, 0x0000);
#else
	g_vga_memstart = (unsigned char*)calloc(320 * 200, sizeof(unsigned char));
	g_vga_backbuffer = g_vga_memstart;

#endif

	/* allocate small chunks of memory */
#if defined(__BORLANDC__)
	g_itemsname		= (char**)schick_alloc(254 * sizeof(char*));
#else
	g_itemsname		= (char**)schick_alloc(255 * sizeof(char*));
#endif
	g_itemsdat 		= (unsigned char*)schick_alloc(255 * sizeof(item_stats));
	g_monnames_buffer	= (char*)schick_alloc(950);
#if defined(__BORLANDC__)
	g_monnames_index	= (char**)schick_alloc(77 * sizeof(char*));
#else
	g_monnames_index	= (char**)schick_alloc(79 * sizeof(char*));
#endif
	g_memslots_anis		= (struct struct_memslot_ani*)schick_alloc(37 * sizeof(struct struct_memslot_ani));
	g_memslots_mfig		= (struct struct_memslot_fig*)schick_alloc(43 * sizeof(struct struct_memslot_fig));
	g_memslots_wfig		= (struct struct_memslot_fig*)schick_alloc(43 * sizeof(struct struct_memslot_fig));
	g_memslots_mon		= (struct struct_memslot_fig*)schick_alloc(36 * sizeof(struct struct_memslot_fig));
	g_heroes		= (unsigned char*)schick_alloc(7 * sizeof(struct struct_hero));
	g_dungeon_fights_buf	= (unsigned char*)schick_alloc(630);
	g_dungeon_doors_buf	= (struct dungeon_door*)schick_alloc(45 * sizeof(struct dungeon_door));
	g_dungeon_stairs_buf	= (unsigned char*)schick_alloc(80);
	g_buf_font6		= (unsigned char*)schick_alloc(592);
	g_splash_buffer		= (unsigned char*)schick_alloc(1000);
	g_trv_track_pixel_bak	= (unsigned char*)schick_alloc(500);
	g_chessboard		= (signed char*)schick_alloc(625);
	g_popup			= (unsigned char*)(schick_alloc(1673) + 8);
	g_icon			= (unsigned char*)(schick_alloc(1500) + 8);
	g_buf_icon		= (unsigned char*)schick_alloc(5184);
	g_townpal_buf		= (unsigned char*)schick_alloc(288);

#if defined(__BORLANDC__)
	freemem = farcoreleft();
#else
	/* DUMMY value for newer systems */
	freemem = 357000;
#endif

	if (freemem > 334000) {

		if (freemem >= 357000) {
			g_buffersize = 357000L;
			g_large_buf = 1;
		} else {
			g_buffersize = 334000L;
			g_large_buf = 0;
		}

		init_global_buffer();

	} else {

		printf(g_str_not_enough_mem, 329000 - freemem);

		wait_for_keypress();
		error = 1;
	}

	if (!error) {

		init_text();

		g_fig_figure1_buf = (unsigned char*)(g_buffer9_ptr3 + 180000L);
		if (g_large_buf == 1) {
			g_fig_figure1_buf += 23000L;
		}

		g_fig_figure2_buf = (unsigned char*)(((HugePt)g_fig_figure1_buf) - 20000L);
		g_buffer10_ptr = (unsigned char*)(((HugePt)g_fig_figure2_buf) - 16771L);
	}

	return error;
}

/* Borlandified and identical */
void refresh_colors(void)
{
	set_color((Bit8u*)g_color_black, 0);
	set_color((Bit8u*)g_color_white, 0xff);
	set_palette((Bit8u*)g_palette_unknown2, 0xd8, 8);
	set_palette((Bit8u*)g_palette_unknown3, 0xc8, 3);
	set_palette((Bit8u*)g_palette_unknown4, 0x40, 0x20);
	set_palette((Bit8u*)g_palette_general, 0x20, 0x20);
	set_palette((Bit8u*)g_palette_unknown1, 0x60, 0x20);
}


/* Borlandified and identical */
Bit32s get_diskspace(void)
{
#if defined(__BORLANDC__)
	unsigned short a[4];
	Bit32s space;

	getdfree(getdisk() + 1, (struct dfree*)&a);

	space = (Bit32s)a[0] * (Bit32s)a[2] * (Bit32s)a[3];

	return space - 204800;
#else
	return 8 * 1024 * 1024;
#endif
}

/* Borlandified and identical */
void init_game_state(void)
{
	signed short i;

	refresh_colors();

	g_textcolor_fg[1] = 0xc8;
	g_textcolor_fg[2] = 0xc9;
	g_textcolor_fg[3] = 0xca;

	clear_menu_icons();

	for (i = 0; i < 254; i++) {
		g_market_itemsaldo_table[i] = 0;
	}

	gs_current_loctype_bak = LOCTYPE_NONE;
	/* Travia Temple in Thorwal */
	gs_current_loctype = LOCTYPE_TEMPLE;
	gs_current_typeindex = 1;
	gs_x_target_bak = 9;
	gs_y_target_bak = 9;
	gs_x_target = 9;
	gs_y_target = 8;
	gs_direction_bak = 0;
	gs_direction = 0;
	gs_dungeon_index = DUNGEONS_NONE;

	gs_current_town_bak = gs_current_town = TOWNS_THORWAL;

	g_textbox_width = 3;

	/* timer */
	gs_day_timer = HOURS(24) - 1;
	timewarp_until_time_of_day(1);
	gs_day_timer = HOURS(8);
	gs_day_of_week = 4;
	gs_day_of_month = 17;
	gs_month = 1;
	gs_year = 15;

	g_pic_copy.dst = g_vga_memstart;

	load_wallclock_nvf();
	passages_init();

	g_current_ani = -1;
	g_wallclock_update = 1;

	g_gui_buffer_unkn = g_renderbuf_ptr;
	load_splashes();
}

/* Borlandified and identical */
int err_handler(void)
{
#if defined(__BORLANDC__)
	hardresume(3);
#endif
	return 1;
}

/* Borlandified and nearly identical */
void prepare_dirs(void)
{
#if defined(__BORLANDC__)
	signed short l_si;
	signed short l_di;
	signed short drive;
	signed short drive_bak;
	signed short errorval;
	struct ffblk blk;
	char gamepath[40];

	/* BC-TODO: only the adress differs, should be the stub adress */
	harderr((int(*)(int, int, int, int))err_handler);

	drive_bak = drive = getdisk();

	errorval = 0;
	while (errorval != 2) {

		setdisk(drive);

		/* copy "X:\\" */
		strcpy(gamepath, g_str_drive_x);

		/* set drive name in path */
		gamepath[0] = drive + 'A';

		if (drive_bak == drive) {
			getcurdir(0, &gamepath[3]);
		} else {
			gamepath[2] = '\0';
		}

		strcpy(g_text_output_buf, gamepath);
		/* "\\TEMP" */
		strcat(g_text_output_buf, g_str_backslash_temp);

		if (!chdir(g_text_output_buf)) {
			/*	check if it's possible to change to TEMP-dir: OK
				change to gamepath */

			chdir(gamepath);
			errorval = 2;

		} else {

			if (mkdir(g_text_output_buf)) {
				errorval = 1;
			} else {
				errorval = 2;
			}
		}

		if (errorval == 1) {

			/* ERROR, cant write => exit */

			GUI_output(g_str_temp_dir_fail);

			cleanup_game();

			exit(0);
		}
	}

	/* delete *.* in TEMP-dir */
	sprintf(g_text_output_buf, g_str_temp_xx_ptr2, g_all_files_wildcard2);

	l_si = findfirst(g_text_output_buf, &blk, 0);

	if (!l_si) {

		do {
			sprintf(g_text_output_buf,
				g_str_temp_xx_ptr2,
				((char*)&blk) + 30);			/* contains a filename */

			unlink(g_text_output_buf);

			l_si = findnext(&blk);

		} while (!l_si);
	}

	/* search for "*.CHR" in the gamepath */
	l_si = findfirst(g_all_chr_wildcard4, &blk, 0);

	while (!l_si) {

		/* open CHR-file and copy it into TEMP-dir */
		l_di = open(((char*)&blk) + 30, O_BINARY | O_RDWR);

		/* REMARK: size of CHR files differs from floppy to CD version */
		_read(l_di, g_renderbuf_ptr, sizeof(struct struct_hero));

		close(l_di);

		sprintf(g_text_output_buf, g_str_temp_xx_ptr2, ((char*)(&blk)) + 30); /* contains a filename */

		l_di = _creat(g_text_output_buf, 0);

		write(l_di, g_renderbuf_ptr, sizeof(struct struct_hero));

		close(l_di);

		l_si = findnext(&blk);
	}

	setdisk(drive_bak);
#endif
}

/* Borlandified and identical */
void cleanup_game(void)
{
#if defined(__BORLANDC__)
	struct ffblk blk;
#endif
	signed short l_si;
	signed short l_di;

#if defined(__BORLANDC__)
	/* disable AIL */
	exit_AIL();

	/* disable CD-Audio */
	if (g_cd_init_successful) {
		CD_audio_stop();
	}

	/* free EMS memory */
	if (g_ems_enabled != 0) {

		for (l_si = 0; l_si < 37; l_si++) {
			/* TODO: test for ems_handle missing */
			if (g_memslots_anis[l_si].figure) {
				EMS_free_pages(g_memslots_anis[l_si].ems_handle);
			}
		}

		/* free male and female figures */
		for (l_si = 0; l_si < 43; l_si++) {

			if (g_memslots_mfig[l_si].figure && g_memslots_mfig[l_si].ems_handle)
			{
				EMS_free_pages(g_memslots_mfig[l_si].ems_handle);
			}

			if (g_memslots_wfig[l_si].figure && g_memslots_wfig[l_si].ems_handle)
			{
				EMS_free_pages(g_memslots_wfig[l_si].ems_handle);
			}
		}

		/* free monster figures */
		for (l_si = 0; l_si < 36; l_si++) {

			if (g_memslots_mon[l_si].figure && g_memslots_mon[l_si].ems_handle)
			{
				EMS_free_pages(g_memslots_mon[l_si].ems_handle);
			}
		}

		if (g_ems_unused_handle) {
			EMS_free_pages(g_ems_unused_handle);
		}

		/* free map memory */
		if (g_ems_travelmap_handle != 0) {
			EMS_free_pages(g_ems_travelmap_handle);
		}
	}

	/* delete all files in TEMP */

	sprintf(g_text_output_buf,
		g_str_temp_xx_ptr2,	/* contains "TEMP\\%s" */
		g_all_files_wildcard3);			/* contains "*.*" */

	l_di = findfirst(g_text_output_buf, &blk, 0);

	if (l_di == 0) {

		do {
			/* delete each found file */
			sprintf(g_text_output_buf,
				g_str_temp_xx_ptr2,	/* contains "TEMP\\%s" */
				((char*)(&blk)) + 30);			/* contains a filename */

			unlink(g_text_output_buf);

			l_di = findnext(&blk);

		} while (!l_di);
	}
#endif

	/* misc cleanups */
	update_mouse_cursor();
	disable_mouse();
	reset_timer();
	schick_reset_video();
#if defined(__BORLANDC__)
	clrscr();
#endif

#if !defined(__BORLANDC__)
	if (g_vga_memstart) {
		free(g_vga_memstart);
		g_vga_memstart = NULL;
		g_vga_backbuffer = NULL;
	}
#endif
}

/**
 * \brief   shows the game over screen
 */
/* Borlandified and identical */
void game_over_screen(void)
{
	signed short handle;

	set_var_to_zero();

	set_audio_track(ARCHIVE_FILE_VICTORY_XMI);

	/* load SKULL.NVF */
	handle = load_archive_file(ARCHIVE_FILE_SKULL_NVF);

	read_archive_file(handle, g_renderbuf_ptr, 64200);

	close(handle);

	update_mouse_cursor();

	g_wallclock_update = 0;

	wait_for_vsync();

	set_palette(g_palette_allblack2, 0x00, 0x20);
	set_palette(g_palette_allblack2, 0x20, 0x20);

	memcpy((void*)g_vga_memstart, (void*)g_renderbuf_ptr, 320 * 200);

	set_palette((Bit8u*)g_renderbuf_ptr + 64002L, 0x00, 0x40);

	wait_for_keypress();

	/* TODO: update window */
	memset((void*)g_vga_memstart, 0, 320 * 200);

	wait_for_vsync();

	set_palette(g_palette_allblack2, 0x00, 0x20);
	set_palette(g_palette_allblack2, 0x20, 0x20);

	refresh_screen_size();

	g_pp20_index = -1;
}

/* Borlandified and identical */
void call_gen(void)
{
	Bit32u freemem;
	signed short ret;

	update_mouse_cursor();

	exit_AIL();

	/* free the global buffer */
	free((HugePt)g_global_buffer_ptr);

#if defined(__BORLANDC__)
	freemem = farcoreleft();

	/* ret = spawnl(0, "gen.exe", "gen.exe", "b", gamemode == 2 ? "a" : "n", "1", NULL); */
	ret = spawnl(0, g_str_gen_exe, g_str_gen_exe2, g_str_gen_b,
			(g_game_mode == GAME_MODE_ADVANCED ? g_str_gen_a : g_str_gen_n),
			g_str_gen_1, NULL);

#endif
	refresh_screen_size();

	if (ret == -1) {

		/* perror("Generation") */
		perror(g_str_gen_generation);

		wait_for_keypress();

		cleanup_game();
	} else {

		init_AIL(16000);

		init_global_buffer();

		mouse_init();

		set_timer();

		init_text();

		init_common_buffers();

		refresh_colors();

		load_wallclock_nvf();

		if (have_mem_for_sound()) {

			read_sound_cfg();

			alloc_voc_buffer(20000);
		} else {
			exit_AIL();
		}

		g_fig_figure1_buf = (unsigned char*)(g_buffer9_ptr3 + 180000L);
		if (g_large_buf == 1) {
			g_fig_figure1_buf += 23000L;
		}

		g_fig_figure2_buf = (unsigned char*)(((HugePt)g_fig_figure1_buf) - 20000L);
		g_buffer10_ptr = (unsigned char*)(((HugePt)g_fig_figure2_buf) - 16771L);

		gs_day_timer = HOURS(24) - 1;
		timewarp_until_time_of_day(1);
		gs_day_timer = HOURS(8);

		gs_day_of_week = 4;
		gs_day_of_month = 17;
		gs_month = 1;
		gs_year = 15;
	}
}

#if !defined(__BORLANDC__)
}
#endif
