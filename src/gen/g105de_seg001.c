/*
 *	Rewrite of DSA1 GEN v1.05_de functions of seg001 (cdrom)
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
#define readb(p) (*(unsigned char*)(p))
#define writeb(p, v) (*(unsigned char*)(p) = (v))
#define writew(p, v) (*(unsigned short*)(p) = (v))
#define writed(p, v) (*(unsigned long*)(p) = (v))


#include "g105de_seg001.h"
#include "g105de_seg002.h"
#include "g105de_seg007.h"

/* extern variables from seg002 */
extern signed short g_called_with_args;
extern unsigned char *g_vga_memstart;

/* initialized global variables DATA */
static signed char cd_dummy0 = 0;
static signed short CD_INIT_SUCCESSFUL = 0;
static long cd_dummy1 = 0;
static signed short CD_AUDIO_REPEAT = 0;
static long cd_dummy2 = 0;
static signed short CD_INSERT_COUNTER = 5;
static char STR_INSERT_CD[] = "BITTE LEGEN SIE DIE \xf2SCHICKSALSKLINGE-CD\xf0 IN LAUFWERK %c: EIN. DIESE WIRD BEN\x99TIGT, DA DATEN W\x8eHREND DES SPIELS VON CD GELADEN WERDEN M\x9aSSEN.";
static char STR_REPEAT[] = "WIEDERHOLEN";
static char STR_QUIT[] = "BEENDEN";
static char STR_CD_EXEPATH[] = "X:\\DSA\\SCHICKM.EXE";

/* uninitialized global variables BSS */
/* Remark: ripped code from the binary had the BSS-variable adresses in reverse order */
static signed short CD_AUDIO_TRACK;
static signed short cd_dummy3_1;
#if !defined(__BORLANDC__)
/* For some reason CD_AUDIO_TRACK
 * is at DS:0x245c instead of DS:0x245a.
 * The two bytes at DS:0x243c and DS:0x243d from CLIB
 * code are responsible for that.
 */
static signed short cd_dummy3_2;
#endif
static long cd_dummy4;
static signed long CD_AUDIO_TOD;
static signed long CD_AUDIO_POS;
static signed short CD_DRIVE_NO;

static signed long cd_dummy5;
static signed short cd_dummy6;

/* internally used prototypes */
static void seg001_0312(void);
static signed short CD_check_file(char*);


/* Borlandified and identical */
static unsigned short CD_has_drives()
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
static unsigned short CD_count_drives()
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
static unsigned short CD_get_first_drive()
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

	CD_DRIVE_NO = CD_get_first_drive();

	_AX = 1;
}

/* Borlandified and identical */
static void CD_driver_request(struct driver_request far* request)
{
	asm {
		mov ax, 0x1510
		mov cx, [CD_DRIVE_NO]
		les bx, request
		int 0x2f
	}
}

/* Borlandified and identical */
/* static would be removed in link stage */
static void CD_unused1(void)
{
	if (CD_INIT_SUCCESSFUL != 0) {

		req[3].status = 0;
		req[3].ptr = cd_buf1;
		writeb(&cd_buf1[0x00], 0x0c);
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

	if (CD_INIT_SUCCESSFUL != 0) {

		writew(&req[5].status, 0);

		writed(&req[5].ptr,
			(unsigned long)MK_FP(readb(&cd_buf1[0x10 + track_no * 8]),
			(readb(&cd_buf1[0x0f + track_no * 8]) << 8) +
			(readb(&cd_buf1[0x0e + track_no * 8]))
			)
		);

		/* calculate track_start */
		track_start = (60L * (unsigned long)readb(&cd_buf1[0x10 + track_no * 8])
			+ readb(&cd_buf1[0x0f + track_no * 8])) * 75L
			+ readb(&cd_buf1[0x0e + track_no * 8]);

		/* calculate track_end */
		if (readb(&cd_buf1[0x326]) == track_no) {

			track_end = (((unsigned long)readb(&cd_buf1[0x329]) * 60
						+ readb(&cd_buf1[0x328])) * 75
						+ readb(&cd_buf1[0x327]));
		} else {
			track_end = (((unsigned long)readb(&cd_buf1[0x18 + track_no * 8]) * 60
						+ readb(&cd_buf1[0x17 + track_no * 8])) * 75
						+ readb(&cd_buf1[0x16 + track_no * 8]));

		}

		track_start = track_end - track_start;
		// track_start is now track length

		// OK till here: 0x251, but works identical
		writew(&req[5].dummy4, track_start - 150);
		writew(&req[5].dummy6, (signed long)(*((signed short*)(&track_start) + 1)));

		// req[5] starts at 0x8c
		//CD_driver_request((struct driver_request*)&cd_buf1[0x8c]);
		CD_driver_request((struct driver_request*)&req[5]);

		// CD_AUDIO_POS = ((track_start - 150L) * 74574) / 307200;
		CD_AUDIO_POS = ((track_start - 150L) * 0x1234e) / 0x4b000;
		CD_AUDIO_TOD = CD_get_tod();
	}
}

/* Borlandified and nearly identical, but works */
static void seg001_02ba()
{
	if (CD_INIT_SUCCESSFUL != 0) {

		if ((CD_get_tod() - (signed long)CD_AUDIO_TOD) >= (signed long)CD_AUDIO_POS) {

			if (CD_AUDIO_REPEAT == 1) {
				seg001_0312();
				seg001_0312();
				seg001_00bb(CD_AUDIO_TRACK);
				CD_AUDIO_REPEAT = 1;
			}
		}
	}
}

/* Borlandified and identical */
signed short CD_bioskey(signed short cmd)
{
	seg001_02ba();

	// GEN uses an implicit return here
	//return bioskey(cmd);
	bioskey(cmd);
}

/* Borlandified and identical */
static void seg001_0312(void)
{
	if (CD_INIT_SUCCESSFUL != 0) {

		writew(&req[0].status, 0);
		CD_driver_request(&req[0]);
		CD_AUDIO_REPEAT = 0;
	}
}

/* Borlandified and identical */
void seg001_033b(void)
{
	if (CD_INIT_SUCCESSFUL != 0) {

		seg001_0312();

		writew(&req[1].status, 0);
		CD_driver_request(&req[1]);
	}
}

/* Borlandified and nearly identical */
void CD_unused2(void)
{
	if (CD_INIT_SUCCESSFUL != 0) {

		writew(&req[6].status, 0);
		CD_driver_request(&req[6]);
	}
}
/* Borlandified and nearly identical */
void CD_unused3(void)
{
	if (CD_INIT_SUCCESSFUL != 0) {

		writew(&req[7].status, 0);
		CD_driver_request(&req[7]);
	}
}

/* Borlandified and identical */
static void seg001_03a8(void)
{
	signed short v;

	if (CD_INIT_SUCCESSFUL != 0) {

		writew(&req[2].status, 0);
		writed(&req[2].ptr, &cd_buf1[0x324]);
		writeb(&cd_buf1[0x324], 0x0a);
		CD_driver_request((struct driver_request*)&req[2]);

		v = readb(&cd_buf1[0x325]);
		for (; readb(&cd_buf1[0x326]) >= v; v++) {
			writew(&req[2].status, 0);
			writed(&req[2].ptr, &cd_buf1[0x0c + 8 * v]);
			writeb(&cd_buf1[0x0c + 8 * v], 11);
			writeb(&cd_buf1[0x0d + 8 * v], (unsigned char)v);

			CD_driver_request((struct driver_request*)&req[2]);
		}
	}
}

/* Borlandified and identical */
void seg001_0465(unsigned short track)
{
	seg001_0312();
	seg001_0312();
	CD_AUDIO_TRACK = 4;
	seg001_00bb(CD_AUDIO_TRACK);
	CD_AUDIO_REPEAT = 1;
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
	
	sprintf(text_buffer, (char*)STR_INSERT_CD, CD_DRIVE_NO + 'A');

	si = -2;
	
	while (si == -2) {

		si = gui_radio((unsigned char*)text_buffer,
				2,
				(char*)STR_REPEAT,	
				(char*)STR_QUIT);	
	}

	if (si == 2) {
		stop_music();
		update_mouse_cursor();
		mouse_disable();
		restore_timer_isr();
		
		if (g_called_with_args != 0) {
			call_fill_rect_gen((unsigned char*)g_vga_memstart, 0, 0, 319, 199, 0);
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
	if (CD_INSERT_COUNTER == 0) {
		CD_radio_insert_cd();
		CD_INSERT_COUNTER = 5;
	}
	CD_INSERT_COUNTER--;
	hardresume(1);
	return 1;
}

/* Borlandified and nearly identical */
static void CD_check_cd(void)
{
	char fname[80];

	harderr((int(*)(int, int, int, int))CD_insert_loop);

	strcpy(fname, (char*)STR_CD_EXEPATH);
	fname[0] = CD_DRIVE_NO + 'A';

	while (CD_check_file(fname) <= 0) {
		CD_radio_insert_cd();
	}
}

/* Borlandified and identical */
signed short seg001_0600(void)
{
	if (CD_set_drive_no() == 0)
		return 0;

	CD_INIT_SUCCESSFUL = 1;
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
