/*
 *	Rewrite of DSA1 GEN v1.05_de functions of seg001 (cdrom)
 *	Functions rewritten: 21/21 (complete)
 *
 *	Remarks:
 *		The first part of this file is for inclusion in DOSBox.
 *		Aim here is, that they work identical.
 *
 *		The second is a rewrite using the inline assembler of
 *		Borland C++ 3.1. The aim is, to produce exactly the same
 *		OPcodes like in the original. Seems to work. :)
 *
 *	MSCDEX:
 *		In this segment are mostly calls to the MSCDEX-API.
 *		It seems, that MSCDEX 2.1 is needed,
 *		since function 0x10 (driver request) is called.
 *		Further this code is written only on the first installed
 *		CD-ROM drive.
 *
 *
 */

#include <stdio.h>
#include <string.h>

#if defined(__BORLANDC__)
#include <DOS.H>	// _dos_open(), harderr()
#include <IO.H>		// lseek()
#include <BIOS.H>	// bioskey()
#include <CONIO.H>	// clrsrc()
#endif

#include "port.h"

#include "g105de_seg001.h"
#include "g105de_seg002.h"

/* extern variables from seg002 */
extern signed short g_called_with_args;
extern unsigned char *g_vga_memstart;

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

#define CDSEG (0xc83)

/* Borlandified and identical */
static unsigned short CD_has_drives()
{
#if defined(__BORLANDC__)
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
#endif
}

/* Borlandified and identical */
static unsigned short CD_count_drives()
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}
		_AX = _BX;

//	return _BX;
#endif
}

/* Borlandified and identical */
static unsigned short CD_get_first_drive()
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}

	_AX = _CX;
//	return _CX;
#endif
}

/* Borlandified and identical */
unsigned short CD_set_drive_no()
{
#if defined(__BORLANDC__)
	if (CD_has_drives() == 0) return 0;
	if (CD_count_drives() == 0) return 0;

	CD_DRIVE_NO = CD_get_first_drive();

	_AX = 1;
#endif
}

/* Borlandified and identical */
static void CD_driver_request(struct driver_request far* req)
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x1510
		mov cx, [CD_DRIVE_NO]
		les bx, req
		int 0x2f
	}
#endif
}

/* Borlandified and far from identical, but unused (8 diffs)*/
/* TODO: check adresses of seg013 */
static void CD_unused1(void)
{
	if (CD_INIT_SUCCESSFUL != 0) {

#if defined(__BORLANDC__)
	req[3].status = 0;
	req[3].ptr = cd_buf1;
	//cd_buf1[252] = 0x0c;
	asm { db 0x0f, 0x1f, 0x40, 0x00; } 	// BCC Sync-Point
	asm { db 0x0f, 0x1f, 0x40, 0x00; } 	// BCC Sync-Point
	CD_driver_request(&req[3]);
#else
	//DUMMY
#endif
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
Bit32s CD_get_tod(void)
{
#if defined(__BORLANDC__)
	asm {
		mov ah, 0x0
		int 0x1a
		mov ax, dx
		mov dx, cx
	}
#endif
}

/* Seem Unborlandifiable to me */
void seg001_00bb(Bit16s track_no)
{
#if defined(__BORLANDC__)
	Bit32s track_start;
	Bit32s track_end;
	unsigned int track_len, tmp;

	if (CD_INIT_SUCCESSFUL != 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x8f)), 0);

		host_writed(Real2Host(RealMake(reloc_gen + CDSEG, 0x9a)),
			(host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10b + track_no * 8))) << 8) +
			(host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10a + track_no * 8)))) +
			(host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10c + track_no * 8)))) << 16);

		/* calculate track_start */
		track_start = (60L * host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10b + track_no * 8)))
			+ host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10a + track_no * 8)))) * 75L
			+ host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x10c + track_no * 8)));

		/* calculate track_end */
		if (host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x422))) == track_no) {

			track_end = (60L * host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x425))) +
					   host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x424)))) * 75L +
					   host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x423)));
		} else {
			track_end = (60L * host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x114 + track_no * 8))) +
					   host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x113 + track_no * 8)))) * 75L +
					   host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x112 + track_no * 8)));

		}

		track_start -= track_end;
		// track_start is now track length
		host_writed(Real2Host(RealMake(reloc_gen + CDSEG, 0x9e)), track_start - 150);

#if !defined(__BORLANDC__)
		CD_driver_request(RealMake(reloc_gen + CDSEG, 0x8c));
		CD_AUDIO_POS = ((track_start - 150) * 0x1234e) / 0x4b000;
#else
		CD_driver_request((struct driver_request*)RealMake(reloc_gen + CDSEG, 0x8c));

		asm { db 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x44, 0x00, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point

		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
		asm { db 0x0F, 0x1F, 0x40, 0x00; } // BCC Sync-Point
#endif
		CD_AUDIO_TOD = CD_get_tod();
	}
#endif
}

/* Borlandified and nearly identical, but works */
static void seg001_02ba()
{
	if (CD_INIT_SUCCESSFUL != 0) {

		if ((CD_get_tod() - (Bit32s)CD_AUDIO_TOD) >= (Bit32s)CD_AUDIO_POS) {

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

#if !defined(__BORLANDC__)
	return bioskey(cmd);
#else
	// return
	bioskey(cmd); // implicit return
#endif
}

/* Borlandified and nearly identical */
void seg001_0312()
{
	if (CD_INIT_SUCCESSFUL != 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 3)), 0);
#if !defined(__BORLANDC__)
		CD_driver_request(RealMake(reloc_gen + CDSEG, 0));
#else
		asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
		asm { db 0x0f, 0x1f, 0x00; }
		asm { db 0x0f, 0x1f, 0x00; }
		asm { nop; }
		asm { nop; }
#endif
		CD_AUDIO_REPEAT = 0;
	}
}

/* Borlandified and nearly identical */
void seg001_033b()
{
	if (CD_INIT_SUCCESSFUL != 0) {

		seg001_0312();
		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x1f)), 0);
#if !defined(__BORLANDC__)
		CD_driver_request(RealMake(reloc_gen + CDSEG, 0x1c));
#else
		asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
		asm { db 0x0f, 0x1f, 0x00; }
		asm { db 0x0f, 0x1f, 0x00; }
		asm { nop; }
		asm { nop; }
#endif
	}
}

#if defined (__BORLANDC__)
/* Borlandified and nearly identical */
void CD_unused2()
{
	if (CD_INIT_SUCCESSFUL != 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0xab)), 0);
		//CD_driver_request(RealMake(reloc_gen + CDSEG, 0xa8));

		asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
		asm { db 0x0f, 0x1f, 0x00; }
		asm { db 0x0f, 0x1f, 0x00; }
		asm { nop; }
		asm { nop; }
	}
}
/* Borlandified and nearly identical */
void CD_unused3()
{
	if (CD_INIT_SUCCESSFUL != 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0xc7)), 0);
		//CD_driver_request(RealMake(reloc_gen + CDSEG, 0xc4));

		asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
		asm { db 0x0f, 0x1f, 0x00; }
		asm { db 0x0f, 0x1f, 0x00; }
		asm { nop; }
		asm { nop; }
	}
}
#endif

/* Borlandified and nearly identical */
void seg001_03a8()
{
	Bit16u v;

	if (CD_INIT_SUCCESSFUL == 0) {

		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x3b)), 0);
		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x48)), reloc_gen + CDSEG);
		host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x46)), 0x420);
		host_writeb(Real2Host(RealMake(reloc_gen + CDSEG, 0x420)), 10);
#if !defined(__BORLANDC__)
		CD_driver_request(RealMake(reloc_gen + CDSEG, 0x38));
#else
		CD_driver_request((struct driver_request*)RealMake(reloc_gen + CDSEG, 0x38));
#endif

		v = host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x421)));
		for (; host_readb(Real2Host(RealMake(reloc_gen + CDSEG, 0x422))) >= v; v++) {
			host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x3b)), 0);
			host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x48)), reloc_gen + CDSEG);
#if !defined(__BORLANDC__)
			host_writew(Real2Host(RealMake(reloc_gen + CDSEG, 0x46)), 0x108 + v * 8);
#else
			asm { db 0x66, 0x90; } // BCC Sync-Point
			asm { db 0x66, 0x90; }
			asm { nop; }
#endif
			host_writeb(Real2Host(RealMake(reloc_gen + CDSEG, v * 8 + 0x108)), 11);
			host_writeb(Real2Host(RealMake(reloc_gen + CDSEG, v * 8 + 0x109)), (unsigned char)v);

#if !defined(__BORLANDC__)
			CD_driver_request(RealMake(reloc_gen + CDSEG, 0x38));
#else
			CD_driver_request((struct driver_request*)RealMake(reloc_gen + CDSEG, 0x38));
#endif
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

/* Borlandified and nearly identical, but works correctly */
//static
Bit16s CD_check_file(char *pathP)
{
	int handle;
	Bit16s buf;
	unsigned int nread;
	
#if defined(__BORLANDC__)
	if (_dos_open((char*)pathP, 1, &handle)) return -1;

	if (_dos_read(handle, (Bit8u*)&buf, 1, &nread)) return -1;

	lseek(handle, 2000L, 0);

	if (_dos_read(handle, (Bit8u*)&buf, 1, &nread)) return -1;
#endif

	_dos_close(handle);

#if !defined(__BORLANDC__)	
	return nread;
#else
	asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
#endif
}

/* Borlandified and identical */
void CD_radio_insert_cd()
{
	char text_buffer[160];

	Bit16s si;
	
	sprintf(text_buffer, (char*)STR_INSERT_CD, CD_DRIVE_NO + 'A');

	si = -2;
	
	while (si == -2) {

		si = gui_radio((Bit8u*)text_buffer,
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
			call_fill_rect_gen((RealPt)g_vga_memstart, 0, 0, 319, 199, 0);
		} else {
			exit_video();
			clrscr();
		}
		bc_exit(0);
	}
}

/* Borlandified and identical */
Bit16s CD_insert_loop()
{
	if (CD_INSERT_COUNTER == 0) {
		CD_radio_insert_cd();
		CD_INSERT_COUNTER = 5;
	}
	CD_INSERT_COUNTER--;
#if defined(__BORLANDC__)
	bc__exit(0);
#endif

#if !defined(__BORLANDC__)
	return 1;
#else
	asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
#endif
}

/* Borlandified and nearly identical */
void CD_check_cd()
{
	char fname[80];

#if defined(__BORLANDC__)
	harderr((int(*)(int, int, int, int))CD_insert_loop);
#endif

	strcpy(fname, (char*)STR_CD_EXEPATH);
	fname[0] = CD_DRIVE_NO + 'A';

	while (CD_check_file(fname) <= 0) {
		CD_radio_insert_cd();
	}
}

/* Borlandified and identical */
signed short seg001_0600()
{
	if (CD_set_drive_no() == 0)
		return 0;

	CD_INIT_SUCCESSFUL = 1;
	CD_check_cd();
	seg001_033b();
	seg001_03a8();

#if !defined(__BORLANDC__)
	return 1;
#else
	asm { db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
#endif
}

void dummy()
{
}
