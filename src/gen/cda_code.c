/*
 *	Rewrite of DSA1 GEN v1.05_de functions of cda_code (cdrom)
 *	Functions rewritten: 21/21 (complete)
 *
 *	Remarks:
 *		This code is a rewrite using the inline assembler of
 *		Borland C++ 3.1. The aim is, to produce exactly the same
 *		behaviour like the original. Seems to work. :)
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

/* initialized global variables DATA */
static char cd_dummy0 = 2;
static signed short g_cd_init_successful = 0;
static signed short g_cd_audio_repeat = 0;
static signed short g_cd_insert_counter = 5;
static const char g_str_insert_cd[] = "BITTE LEGEN SIE DIE \xf2SCHICKSALSKLINGE-CD\xf0 IN LAUFWERK %c: EIN. DIESE WIRD BEN\x99TIGT, DA DATEN W\x8eHREND DES SPIELS VON CD GELADEN WERDEN M\x9aSSEN.";
static const char g_str_repeat[] = "WIEDERHOLEN";
static const char g_str_quit[] = "BEENDEN";
static const char g_str_cd_exepath[] = "X:\\DSA\\SCHICKM.EXE";

struct driver_request {
	char dummy1;		/*@0x00 */
	char subunit;		/*@0x01 */
	char function;		/*@0x02 */
	unsigned short status;  /*@0x03 */
	char dummy2[8];		/*@0x05 */
	char redbook;		/*@0x0d 0 or 1 */
	void *ptr;		/*@0x0e */
	signed short dummy4;	/*@0x12 0,1,7,b */
	signed short dummy6;	/*@0x14 0 or 2 */
	char dummy7[6];		/*@0x16 */
};

struct driver_request req[9] = {
 {0x16, 0x00, 0x85, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x16, 0x00, 0x85, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x1a, 0x00, 0x03, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00000000, 0x0007, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x1a, 0x00, 0x03, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00000000, 0x000b, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x18, 0x00, 0x83, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00000000, 0x0000, 0x0002, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x16, 0x00, 0x84, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x16, 0x00, 0x85, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x16, 0x00, 0x88, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 {0x16, 0x00, 0x0c, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, &cd_dummy0, 0x0001, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};


/* uninitialized global variables BSS */
/* Remark: ripped code from the binary had the BSS-variable adresses in reverse order */
static signed short g_cd_drive_no;
static signed long g_cd_audio_pos;
static signed long g_cd_audio_tod;
static signed short g_cd_audio_track;

static unsigned char far cd_buf1[824];

/* extern variables from seg002 */
extern signed short g_called_with_args;
extern unsigned char *g_vga_memstart;

/* externally used prototypes from (mainfile) */
extern void free_buffers(void);
extern void mouse_bg(void);
extern void mouse_disable(void);
extern void exit_video(void);
extern void exit_music(void);
extern void call_fill_rect_gen(unsigned char*, const signed short, const signed short,
			const signed short, const signed short, const signed short);
extern void restore_timer_isr(void);
extern signed short gui_radio(char*, const signed int, ...);

/* non-portable Memory Access */
#define readws(p) (*(const signed short*)(p))

/**
 * \brief check if CD-Drives are available
 *
 * \return iff available 1 otherwise 0
 */
static signed short CD_has_drives(void)
{
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f

		xor ax, ax
		or  bx, bx
		jz has_no_cd
		inc ax
	}
has_no_cd:
	return _AX;
}

/**
 * \brief count the number of available CD-Drives
 *
 * \return number of CD-Drives
 */
static signed short CD_count_drives(void)
{
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}
	return _BX;

}

/**
 * \brief get the drive letter of the first CD-Drive
 *
 * \return drive letter 0 = A:, 1 = B:, 2 = C:, 3 = D:, ...
 */
static signed short CD_get_first_drive(void)
{
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}

	return _CX;
}

/**
 * \brief sets the internal variable of the used CD-Drive
 *
 * \return 1 iff CD-Drive is used otherwise 0
 */
static signed short CD_set_drive_no(void)
{
	if (!CD_has_drives() || !CD_count_drives()) return 0;

	g_cd_drive_no = CD_get_first_drive();

	return 1;
}

static void CD_driver_request(struct driver_request far* request)
{
	asm {
		mov ax, 0x1510
		mov cx, [g_cd_drive_no]
		les bx, request
		int 0x2f
	}
}

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
 * \return  clock ticks since midnight, the system time.
 */
static signed long CD_get_tod(void)
{
	asm {
		mov ah, 0x0
		int 0x1a
		mov ax, dx
		mov dx, cx
	}
	return ((signed long)_DX << 16) | _AX;
}

static void CD_audio_start_track(const signed short track_no)
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

static void CD_audio_stop_hsg(void)
{
	if (!g_cd_init_successful) {

		req[0].status = 0;
		CD_driver_request(&req[0]);
		g_cd_audio_repeat = 0;
	}
}

void CD_enable_repeat(void)
{
	if (!g_cd_init_successful) return;
	if ((CD_get_tod() - g_cd_audio_tod) < g_cd_audio_pos) return;

	if (g_cd_audio_repeat == 1) {
		CD_audio_stop_hsg();
		CD_audio_stop_hsg();
		CD_audio_start_track(g_cd_audio_track);
		g_cd_audio_repeat = 1;
	}
}

static signed short CD_bioskey(const signed short cmd)
{
	CD_enable_repeat();

	return bioskey(cmd);
}

void CD_audio_stop(void)
{
	if (g_cd_init_successful != 0) {

		CD_audio_stop_hsg();

		req[1].status = 0;
		CD_driver_request(&req[1]);
	}
}

static void CD_unused2(void)
{
	if (g_cd_init_successful != 0) {

		req[6].status = 0;
		CD_driver_request(&req[6]);
	}
}

static void CD_unused3(void)
{
	if (g_cd_init_successful != 0) {

		req[7].status = 0;
		CD_driver_request(&req[7]);
	}
}

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

/* used externally */
void CD_play_track(const signed short track)
{
	CD_audio_stop_hsg();
	CD_audio_stop_hsg();

	g_cd_audio_track = track;

	CD_audio_start_track(g_cd_audio_track);

	g_cd_audio_repeat = 1;
}

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
		exit_music();
		mouse_bg();
		mouse_disable();
		restore_timer_isr();
		
		call_fill_rect_gen(g_vga_memstart, 0, 0, 319, 199, 0);

		if (!g_called_with_args) {
			exit_video();
			clrscr();
		}

		free_buffers();

		exit(0);
	}
}

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

signed short CD_audio_init(void)
{
	if (!CD_set_drive_no()) return 0;

	g_cd_init_successful = 1;
	CD_check_cd();
	CD_audio_stop();
	seg001_03a8();

	return 1;
}
#endif
