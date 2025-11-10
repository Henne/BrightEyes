/*
 *	Rewrite of DSA1 v3.02_de functions of seg001 (cdrom)
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

#include "v302de.h"

#include "seg001.h"
#include "seg097.h"
#include "seg120.h"

#if defined(__BORLANDC__)
#include <DOS.H>
#include "seg013.h"
#endif

char cd_dummy0 = 2;
unsigned short g_cd_init_successful = 0;
static unsigned char cd_dummy1[4] = {0};
static unsigned short g_cd_audio_repeat = 0;
static unsigned char cd_dummy2[4] = {0};
static unsigned short g_cd_audio_paused = 0;
static unsigned short g_cd_audio_track_bak = 0x7fff;
static const int g_cd_audio_tracklist[15] = {
	0x7fff,	0x7fff,	0x7fff, 0x7fff,
	ARCHIVE_FILE_SUMMARY_XMI,
	ARCHIVE_FILE_THORWAL_XMI,
	ARCHIVE_FILE_TEMPLE_XMI,
	ARCHIVE_FILE_TERMS_XMI,
	ARCHIVE_FILE_SMITH_XMI,
	ARCHIVE_FILE_INN_XMI,
	ARCHIVE_FILE_HEALER_XMI,
	ARCHIVE_FILE_CAMP_XMI,
	ARCHIVE_FILE_VICTORY_XMI,
	ARCHIVE_FILE_COMBAT_XMI,
	ARCHIVE_FILE_DUNGEON_XMI
};
uint32_t g_cd_skipmagic = 0L;

static unsigned short g_cd_check_err_countdown = 5;
static const char g_str_insert_cd[142] = "BITTE LEGEN SIE DIE \xf2SCHICKSALSKLINGE-CD\xf0 IN LAUFWERK %c: EIN. DIESE WIRD BEN\x99TIGT, DA DATEN W\x8eHREND DES SPIELS VON CD GELADEN WERDEN M\x9aSSEN.";
static const char g_str_repeat[12] = "WIEDERHOLEN";
static const char g_str_quit[8] = "BEENDEN";
static const char g_str_cd_exepath[19] = "X:\\DSA\\SCHICKM.EXE";
static char g_str_cd_missing[204] = "DIESES PROGRAMM IST OHNE CD-ROM-LAUFWERK NICHT LAUFF\x8eHIG. BITTE INSTALLIEREN SIE DIE GER\x8eTETREIBER F\x9aR DAS CD-ROM-LAUFWERK GEM\x8eSS IHRER CD-ROM-DOKUMENTATION UND STARTEN SIE DIE 'SCHICKSALSKLINGE' ERNEUT.";
static const char g_str_cd_init[37] = "INITIALISIERE CD-ROM IN LAUFWERK %c:";
static unsigned char g_unkn_004 = 0x00;

static unsigned long g_cd_audio_pause_pos; // ds:0xbc38
static unsigned long g_cd_audio_pause_tod; // ds:0xbc3c
static unsigned short g_cd_audio_track; // ds:0xbc40
static unsigned char g_unkn_071[8]; // ds:0xbc42
static unsigned long g_cd_audio_tod; // ds:0xbc4a
static unsigned long g_cd_audio_pos; // ds:0xbc4e
static unsigned short g_cd_drive_num; // ds:0xbc52
static unsigned char g_unkn_072[4]; // ds:0xbc54

/* static prototypes */
static void CD_audio_stop_hsg(void);

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/**
 * \brief   check if cdrom-drives are installed
 *
 * \return  {0 = no drive, 1 = at least one drive}
 */
/* Borlandified and identical */
static unsigned short CD_has_drives(void)
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

	return _AX;
}

/* Borlandified and identical */
static unsigned short CD_count_drives(void)
{
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}

	return _BX;
}

/* Borlandified and identical */
static unsigned short CD_get_first_drive(void)
{
	asm {
		mov ax, 0x1500
		xor bx, bx
		int 0x2f
	}

	return _CX;
}

/* Borlandified and identical */
static unsigned short CD_set_drive_num(void)
{
	if (!CD_has_drives()) return 0;

	if (!CD_count_drives()) return 0;

	g_cd_drive_num = CD_get_first_drive();

	return 1;
}

/* Borlandified and identical */
static void CD_driver_request(driver_request far* req)
{
	asm {
		mov ax, 0x1510
		mov cx, [g_cd_drive_num]
		les bx, req
		int 0x2f
	}
}

/* Borlandified and identical */
/* TODO: check adresses of seg013 */
static void CD_unused1(void)
{
	if (!g_cd_init_successful) return;

	req[3].status = 0;
	req[3].ptr = cd_buf1;
	cd_buf1[0] = 0x0c;
	CD_driver_request(&req[3]);
}

/**
 * \brief   get time of day
 *
 * \return              clock ticks since midnight, the system time.
 *
 * \todo    produces a compiler warning and is a bit hacky
 */
/* Borlandified and identical */
static int32_t CD_get_tod(void)
{
	asm {
		mov ah, 0x0
		int 0x1a
		mov ax, dx
		mov dx, cx
		jmp near leave_tod
	}
leave_tod:
}

/* Borlandified and nearly identical */
static void CD_audio_track_start(signed short track_num)
{
	int32_t track_start;
	int32_t track_end;

	if (g_cd_init_successful) {

		req[5].status = 0;

		req[5].ptr = MK_FP(cd_buf1[0x10 + track_num * 8],
				(cd_buf1[0x0f + track_num * 8] << 8) +
				(cd_buf1[0x0e + track_num * 8]));

		/* calculate track_start */
		track_start = (60L * (unsigned long)cd_buf1[0x10 + track_num * 8]
			+ cd_buf1[0x0f + track_num * 8]) * 75L
			+ cd_buf1[0x0e + track_num * 8];

		/* calculate track_end */
		if (cd_buf1[0x326] == track_num) {

			track_end = (((unsigned long)cd_buf1[0x329] * 60
					+ cd_buf1[0x328]) * 75
					+ cd_buf1[0x327]);
		} else {
			track_end = (((unsigned long)cd_buf1[0x18 + track_num * 8] * 60
					+ cd_buf1[0x17 + track_num * 8]) * 75
					+ cd_buf1[0x16 + track_num * 8]);

		}

		track_start = track_end - track_start;

		req[5].dummy4 = ((unsigned short)track_start) - 150;
		req[5].dummy6 = (signed long)(track_start >> 16);

		CD_driver_request(&req[5]);

		g_cd_audio_pos = (((uint32_t)track_start - 150) * 0x1234e) / 0x4b000;
		g_cd_audio_tod = CD_get_tod();
	}
}
#endif

/* Borlandified and identical */
void CD_enable_repeat(void)
{
#if defined(__BORLANDC__)
	if (!g_cd_init_successful) return;

	if (CD_get_tod() - (int32_t)g_cd_audio_tod < (int32_t)g_cd_audio_pos)
		return;

	if (g_cd_audio_repeat == 1)
	{
		CD_audio_stop_hsg();
		CD_audio_stop_hsg();
		CD_audio_track_start(g_cd_audio_track);
		g_cd_audio_repeat = 1;
	}
#endif
}

/* Borlandified and identical */
int CD_bioskey(const int cmd)
{
#if defined(__BORLANDC__)
	CD_enable_repeat();
	return bioskey(cmd);
#else
	return 0;
#endif
}

#if defined(__BORLANDC__)
/* CD_audio_stop_hsg() - stop audio playback in HSG format */
/* Borlandified and identical */
static void CD_audio_stop_hsg(void)
{
	if (!g_cd_init_successful) return;

	req[0].status = 0;
	CD_driver_request(&req[0]);

	g_cd_audio_repeat = 0;
}

/**
 *\brief	stop audio playback in HSG and redbook format
 */
/* Borlandified and identical */
void CD_audio_stop(void)
{
	if (!g_cd_init_successful) return;

	CD_audio_stop_hsg();
	req[1].status = 0;
	CD_driver_request(&req[1]);
}
#endif

/* Borlandified and identical */
void CD_audio_pause(void)
{
#if defined(__BORLANDC__)
	/* Is CD initialized ? */
	if (!g_cd_init_successful) return;

	/* Is CD already paused ? */
	if (g_cd_audio_paused) return;

	/* set CD pause */
	g_cd_audio_paused = 1;
	g_cd_audio_pause_tod = CD_get_tod();
	/* save current position */
	g_cd_audio_pause_pos = g_cd_audio_pos;
	/* set current position to maximum signed int */
	g_cd_audio_pos = 0x7fffffff;

	req[6].status = 0;
	CD_driver_request(&req[6]);
#endif
}

/* Borlandified and identical */
void CD_audio_play(void)
{
#if defined(__BORLANDC__)
	/* Is CD initialized ? */
	if (!g_cd_init_successful) return;

	/* Is CD paused ? */
	if (!g_cd_audio_paused)	return;

	CD_check();

	/* reset CD pause */
	g_cd_audio_paused = 0;
	g_cd_audio_pos = g_cd_audio_pause_pos;
	g_cd_audio_tod += (CD_get_tod() - g_cd_audio_pause_tod);

	req[7].status = 0;
	CD_driver_request(&req[7]);
#endif
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
static void CD_0432(void)
{
	signed short track_num;

	if (!g_cd_init_successful) return;

	req[2].status = 0;
	req[2].ptr = &cd_buf1[0x324];
	cd_buf1[0x324] = 0x0a;
	CD_driver_request(&req[2]);

	track_num = (unsigned short)cd_buf1[0x325];

	while (cd_buf1[0x326] >= track_num)
	{
		req[2].status = 0;
		req[2].ptr = &cd_buf1[0x0c + 8 * track_num];
		cd_buf1[0x0c + 8 * track_num] = 0x0b;
		cd_buf1[0x0d + 8 * track_num] = track_num;
		CD_driver_request(&req[2]);

		track_num++;
	}
}
#endif

struct dummy15 {
	signed short a[15];
};

/* Borlandified and identical */
void CD_set_track(const int index)
{
#if defined(__BORLANDC__)
	signed short i;
	struct dummy15 tracks = *(struct dummy15*)g_cd_audio_tracklist;

	for (i = 0; i < 15; i++)
	{
		if (tracks.a[i] == index) break;
	}

	g_cd_audio_track = i + 1;

	if (g_cd_audio_track_bak == g_cd_audio_track)
	{
	} else {
		g_cd_audio_track_bak = g_cd_audio_track;
		CD_audio_stop_hsg();
		CD_audio_stop_hsg();

		CD_audio_track_start(g_cd_audio_track);

		g_cd_audio_repeat = 1;

		if (g_cd_audio_paused)
		{
			g_cd_audio_paused = 0;
			CD_audio_pause();
		}
	}
#endif
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
static signed short CD_read_exe(char *path)
{
	int handle;
	signed short buffer;
	unsigned short nread;

	/* skip read check */
	if (g_cd_skipmagic == 0x682772e4L) return 1;

	if (_dos_open(path, 1, (int*)&handle)) return -1;

	if (_dos_read(handle, &buffer, 1, (unsigned int*)&nread)) return -1;

	lseek(handle, 2000L, 0);

	if (_dos_read(handle, &buffer, 1, (unsigned int*)&nread)) return -1;

	_dos_close(handle);

	return nread;
}

/* Borlandified and identical */
static void CD_insert_msg(void)
{
	signed short answer;
	char str[160];

	sprintf(str, g_str_insert_cd, g_cd_drive_num + 'A');

	/* TODO: do {} while; would be better */
	answer = -2;
	while (answer == -2)
	{
		answer = GUI_radio(str, 2, g_str_repeat, g_str_quit);
	}

	if (answer == 2)
	{
		cleanup_game();
		exit(0);
	}
}

/* Borlandified and identical */
static signed short CD_harderr_handler(void)
{
	if (g_cd_check_err_countdown == 0)
	{
		CD_insert_msg();
		g_cd_check_err_countdown = 5;
	}

	g_cd_check_err_countdown--;

#if defined(__BORLANDC__)
	hardresume(1);
#endif

	return 1;
}
#endif

/* Borlandified and identical */
void CD_check(void)
{
#if defined(__BORLANDC__)
	char text[80];

	harderr((int(*)(int, int, int, int))CD_harderr_handler);

	strcpy(text, g_str_cd_exepath);

	text[0] = g_cd_drive_num + 'A';

	while (CD_read_exe(text) <= 0)
	{
		CD_insert_msg();
	}
#endif
}

/* Borlandified and identical */
int CD_init(void)
{
#if defined(__BORLANDC__)
	char str[80];

	if (!CD_set_drive_num())
	{
		GUI_output((char*)g_str_cd_missing);

		return 0;
	}

	sprintf(str, g_str_cd_init, g_cd_drive_num + 'A');

	GUI_output(str);

	g_cd_init_successful = 1;

	CD_audio_stop();

	CD_check();

	CD_0432();

	return 1;
#else
	return 1;
#endif
}

#if !defined(__BORLANDC__)
}
#endif
