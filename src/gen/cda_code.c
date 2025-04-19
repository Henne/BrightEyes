/*
 *	Rewrite of DSA1 GEN v1.05_de functions of cda_code (cdrom)
 *	Functions rewritten: 21/21 (complete)
 *
 *	Remarks:
 *		This code is a rewrite using the inline assembler of
 *		Borland C++ 3.1. The aim is, to produce exactly the same
 *		OPcodes like in the original. Seems to work. :)
 *
 *		Verified on 2025-04-14
 *
 *		This code is not intended to run directly on
 *		modern system since it's not portable and
 *		CD-Drives have become a bit outdated meanwhile.
 *
 *	MSCDEX:
 *		In this segment are mostly calls to the MSCDEX-API.
 *		It seems, that MSCDEX 2.1 is needed,
 *		since function 0x10 (driver request) is called.
 *		Further this code is written only on the first installed
 *		CD-ROM drive.
 *
 */

#if defined(__BORLANDC__)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <DOS.H>	// _dos_open(), harderr()
#include <IO.H>		// lseek()
#include <BIOS.H>	// bioskey()
#include <CONIO.H>	// clrsrc()

/* non-portable Memory Access */
#define readws(p) (*(signed short*)(p))

#include "cda_code.h"
#include "gen105de.h"
#include "cda_data.h"

/* extern variables from seg002 */
extern signed short g_called_with_args;
extern unsigned char *g_vga_memstart;

/* initialized global variables DATA */
char cd_dummy0 = 2;
static signed short g_cd_init_successful = 0;
static long cd_dummy1 = 0;
static signed short g_cd_audio_repeat = 0;
static long cd_dummy2 = 0;
static signed short g_cd_insert_counter = 5;
static const char g_str_insert_cd[] = "BITTE LEGEN SIE DIE \xf2SCHICKSALSKLINGE-CD\xf0 IN LAUFWERK %c: EIN. DIESE WIRD BEN\x99TIGT, DA DATEN W\x8eHREND DES SPIELS VON CD GELADEN WERDEN M\x9aSSEN.";
static const char g_str_repeat[] = "WIEDERHOLEN";
static const char g_str_quit[] = "BEENDEN";
static const char g_str_cd_exepath[] = "X:\\DSA\\SCHICKM.EXE";

/* uninitialized global variables BSS */
/* Remark: ripped code from the binary had the BSS-variable adresses in reverse order */
static signed short g_cd_drive_no;
static signed long g_cd_audio_pos;
static signed long g_cd_audio_tod;
static signed short g_cd_audio_track;

/* externally used prototypes from (mainfile) */
extern stop_music(void);
extern void update_mouse_cursor(void);
extern void mouse_disable(void);
extern void call_fill_rect_gen(unsigned char*, signed short, signed short,
			signed short, signed short, signed short);

/* internally used prototypes */
static signed short CD_has_drives(void);
static signed short CD_count_drives(void);
static signed short CD_get_first_drive(void);
static void CD_enable_repeat(void);
static void CD_audio_stop_hsg(void);
static signed short CD_check_file(char*);


/* Borlandified and identical */
static signed short CD_has_drives(void)
{
	/* al ==  0: return number of drive letters */
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
		xor ax, ax
		or  bx, bx
		jz has_cd
		inc ax
	}
has_cd:
//	return _AX;
}

/* Borlandified and identical */
static signed short CD_count_drives(void)
{
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}
		_AX = _BX;

//	return _BX;
}

/* Borlandified and identical */
static signed short CD_get_first_drive(void)
{
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}

	_AX = _CX;
//	return _CX;
}

/* Borlandified and identical */
static signed short CD_set_drive_no(void)
{
	if (CD_has_drives() == 0) return 0;
	if (CD_count_drives() == 0) return 0;

	g_cd_drive_no = CD_get_first_drive();

	_AX = 1;
}

/* Borlandified and identical */
static void CD_driver_request(struct driver_request far* request)
{
	asm {
		mov ax, 0x1510
		mov cx, [g_cd_drive_no]
		les bx, request
		int 0x2f
	}
}

/* Borlandified and identical */
/* static would be removed in link stage */
static void CD_unused1(void)
{
	if (g_cd_init_successful != 0) {

		req[3].status = 0;
		req[3].ptr = cd_buf1;
		cd_buf1[0x00] = 0x0c;
		CD_driver_request(&req[3]);
	}
}


/**
 * \brief   get time of day
 *
 * \return              clock ticks since midnight, the system time.
 *
 * \todo    produces a compiler warning and is a bit hacky
 */
/* Borlandified and identical */
static signed long CD_get_tod(void)
{
	asm {
		mov ah, 0x0
		int 0x1a
		mov ax, dx
		mov dx, cx
	}
}

/* Borlandified and nearly identical */
static void seg001_00bb(signed short track_no)
{
	signed long track_start;
	signed long track_end;

	if (g_cd_init_successful != 0) {

		req[5].status = 0;

		req[5].ptr =
			MK_FP(cd_buf1[0x10 + track_no * 8],
			(cd_buf1[0x0f + track_no * 8] << 8) +
			(cd_buf1[0x0e + track_no * 8]));

		/* calculate track_start */
		track_start = (60L * (unsigned long)cd_buf1[0x10 + track_no * 8]
			+ cd_buf1[0x0f + track_no * 8]) * 75L
			+ cd_buf1[0x0e + track_no * 8];

		/* calculate track_end */
		if (cd_buf1[0x326] == track_no) {

			track_end = (((unsigned long)cd_buf1[0x329] * 60
					+ cd_buf1[0x328]) * 75
					+ cd_buf1[0x327]);
		} else {
			track_end = (((unsigned long)cd_buf1[0x18 + track_no * 8] * 60
					+ cd_buf1[0x17 + track_no * 8]) * 75
					+ cd_buf1[0x16 + track_no * 8]);

		}

		track_start = track_end - track_start;
		// track_start is now track length

		// TODO: write to dummy6 produces different code, but works identical
		req[5].dummy4 = track_start - 150;
		req[5].dummy6 = readws(((unsigned char*)&track_start) + 2);
		CD_driver_request(&req[5]);

		// g_cd_audio_pos = ((track_start - 150L) * 74574) / 307200;
		g_cd_audio_pos = ((track_start - 150L) * 0x1234e) / 0x4b000;
		g_cd_audio_tod = CD_get_tod();
	}
}

/* Borlandified and identical */
static void CD_enable_repeat(void)
{
	if (g_cd_init_successful == 0) return;
	if ((CD_get_tod() - g_cd_audio_tod) < g_cd_audio_pos) return;

	if (g_cd_audio_repeat == 1) {
		CD_audio_stop_hsg();
		CD_audio_stop_hsg();
		seg001_00bb(g_cd_audio_track);
		g_cd_audio_repeat = 1;
	}
}

/* Borlandified and identical */
signed short CD_bioskey(signed short cmd)
{
	CD_enable_repeat();

	// GEN uses an implicit return here
	//return bioskey(cmd);
	bioskey(cmd);
}

/* Borlandified and identical */
static void CD_audio_stop_hsg(void)
{
	if (g_cd_init_successful != 0) {

		req[0].status = 0;
		CD_driver_request(&req[0]);
		g_cd_audio_repeat = 0;
	}
}

/* Borlandified and identical */
void seg001_033b(void)
{
	if (g_cd_init_successful != 0) {

		CD_audio_stop_hsg();

		req[1].status = 0;
		CD_driver_request(&req[1]);
	}
}

/* Borlandified and nearly identical */
void CD_unused2(void)
{
	if (g_cd_init_successful != 0) {

		req[6].status = 0;
		CD_driver_request(&req[6]);
	}
}
/* Borlandified and nearly identical */
void CD_unused3(void)
{
	if (g_cd_init_successful != 0) {

		req[7].status = 0;
		CD_driver_request(&req[7]);
	}
}

/* Borlandified and identical */
static void seg001_03a8(void)
{
	signed short v;

	if (g_cd_init_successful != 0) {

		req[2].status = 0;
		req[2].ptr = &cd_buf1[0x324];
		cd_buf1[0x324] = 0x0a;
		CD_driver_request(&req[2]);

		v = cd_buf1[0x325];
		for (; cd_buf1[0x326] >= v; v++) {
			req[2].status = 0;
			req[2].ptr = &cd_buf1[0x0c + 8 * v];
			cd_buf1[0x0c + 8 * v] = 11;
			cd_buf1[0x0d + 8 * v] = (unsigned char)v;

			CD_driver_request(&req[2]);
		}
	}
}

/* Borlandified and identical */
void CD_play_track(signed short track)
{
	CD_audio_stop_hsg();
	CD_audio_stop_hsg();
	g_cd_audio_track = 4;
	seg001_00bb(g_cd_audio_track);
	g_cd_audio_repeat = 1;
}

/* Borlandified and identical */
static signed short CD_check_file(char *pathP)
{
	int handle;
	signed short buf;
	unsigned int nread;
	
	if (_dos_open((char*)pathP, 1, &handle)) return -1;

	if (_dos_read(handle, (unsigned char*)&buf, 1, &nread)) return -1;

	lseek(handle, 2000L, 0);

	if (_dos_read(handle, (unsigned char*)&buf, 1, &nread)) return -1;

	_dos_close(handle);

	return nread;
}

/* Borlandified and identical */
static void CD_radio_insert_cd(void)
{
	char text_buffer[160];

	signed short si;
	
	sprintf(text_buffer, g_str_insert_cd, g_cd_drive_no + 'A');

	si = -2;
	
	while (si == -2) {

		si = gui_radio((unsigned char*)text_buffer,
				2, g_str_repeat, g_str_quit);
	}

	if (si == 2) {
		stop_music();
		update_mouse_cursor();
		mouse_disable();
		restore_timer_isr();
		
		if (g_called_with_args != 0) {
			call_fill_rect_gen(g_vga_memstart, 0, 0, 319, 199, 0);
		} else {
			exit_video();
			clrscr();
		}

		exit(0);
	}
}

/* Borlandified and identical */
static signed short CD_insert_loop(void)
{
	if (g_cd_insert_counter == 0) {
		CD_radio_insert_cd();
		g_cd_insert_counter = 5;
	}
	g_cd_insert_counter--;
	hardresume(1);
	return 1;
}

/* Borlandified and nearly identical */
static void CD_check_cd(void)
{
	char fname[80];

	harderr((int(*)(int, int, int, int))CD_insert_loop);

	strcpy(fname, g_str_cd_exepath);
	fname[0] = g_cd_drive_no + 'A';

	while (CD_check_file(fname) <= 0) {
		CD_radio_insert_cd();
	}
}

/* Borlandified and identical */
signed short seg001_0600(void)
{
	if (CD_set_drive_no() == 0)
		return 0;

	g_cd_init_successful = 1;
	CD_check_cd();
	seg001_033b();
	seg001_03a8();

	return 1;
}

// Empty function which code overlaps into the next segment
static void dummy(void)
{
}
#endif
