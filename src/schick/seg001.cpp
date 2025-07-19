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

#define CDA_DATASEG (0x1238)
#define cda_readb(o) (*(Bit8u*)(MK_FP(CDA_DATASEG, (o))))

/* static prototpyes */
static void CD_audio_stop_hsg(void);

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
/**
 * \brief   check if cdrom-drives are installed
 *
 * \return              0 - no drive, 1 - at least one drive
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
static unsigned short CD_set_drive_no(void)
{

	if (CD_has_drives() == 0)
		return 0;

	if (CD_count_drives() == 0)
		return 0;

	ds_writew(CD_DRIVE_NO, CD_get_first_drive());
	return 1;
}

/* Borlandified and identical */
static void CD_driver_request(driver_request far* req)
{
	asm {
		mov ax, 0x1510
		mov cx, [CD_DRIVE_NO]
		les bx, req
		int 0x2f
	}
}

/* Borlandified and identical */
/* TODO: check adresses of seg013 */
static void CD_unused1(void)
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	req[3].status = 0;
	req[3].ptr = cd_buf1;
	cd_buf1[252] = 0x0c;
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
static Bit32s CD_get_tod(void)
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
static void seg001_00c1(signed short track_no)
{
	Bit32s track_start;
	Bit32s track_end;

	if (ds_readw(CD_INIT_SUCCESSFUL) != 0) {

		req[5].status = 0;

		req[5].ptr =
			MK_FP(cd_buf1[0x10c + track_no * 8],
			(cd_buf1[0x10b + track_no * 8] << 8) +
			(cd_buf1[0x10a + track_no * 8]));

		/* calculate track_start */
		track_start = (60L * (unsigned long)cd_buf1[0x10c + track_no * 8]
			+ cd_buf1[0x10b + track_no * 8]) * 75L
			+ cd_buf1[0x10a + track_no * 8];

		/* calculate track_end */
		if (cd_buf1[0x422] == track_no) {

			track_end = (((unsigned long)cd_buf1[0x425] * 60
					+ cd_buf1[0x424]) * 75
					+ cd_buf1[0x423]);
		} else {
			track_end = (((unsigned long)cd_buf1[0x114 + track_no * 8] * 60
					+ cd_buf1[0x113 + track_no * 8]) * 75
					+ cd_buf1[0x112 + track_no * 8]);

		}

		track_start = track_end - track_start;

		req[5].dummy4 = ((unsigned short)track_start) - 150;
		req[5].dummy6 = (signed long)(track_start >> 16);

		CD_driver_request(&req[5]);

		ds_writed(CD_AUDIO_POS, (((Bit32u)track_start - 150) * 0x1234e) / 0x4b000);
		ds_writed(CD_AUDIO_TOD, CD_get_tod());
	}
}

/* Borlandified and identical */
void seg001_02c4(void)
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	if (CD_get_tod() - (Bit32s)ds_readd(CD_AUDIO_TOD) < (Bit32s)ds_readd(CD_AUDIO_POS))
		return;

	if (ds_readw(CD_AUDIO_REPEAT) == 1)
	{
		CD_audio_stop_hsg();
		CD_audio_stop_hsg();
		seg001_00c1(ds_readw(CD_AUDIO_TRACK));
		ds_writew(CD_AUDIO_REPEAT, 1);
	}
}

/* Borlandified and identical */
int CD_bioskey(const int cmd)
{
	seg001_02c4();
	return bioskey(cmd);
}

/* CD_audio_stop_hsg() - stop audio playback in HSG format */
/* Borlandified and identical */
static void CD_audio_stop_hsg(void)
{

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	req[0].status = 0;
	CD_driver_request(&req[0]);

	ds_writew(CD_AUDIO_REPEAT, 0);
}

/**
 *\brief	stop audio playback in HSG and redbook format
 */
/* Borlandified and identical */
void CD_audio_stop(void)
{
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	CD_audio_stop_hsg();
	req[1].status = 0;
	CD_driver_request(&req[1]);
}

/* Borlandified and identical */
void CD_audio_pause(void)
{
	/* Is CD initialized ? */
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	/* Is CD already paused ? */
	if (ds_readw(CD_AUDIO_PAUSED) != 0)
		return;

	/* set CD pause */
	ds_writew(CD_AUDIO_PAUSED, 1);
	ds_writed(CD_AUDIO_PAUSE_TOD, CD_get_tod());
	/* save current position */
	ds_writed(CD_AUDIO_PAUSE_POS, ds_readd(CD_AUDIO_POS));
	/* set current position to maximum singned int */
	ds_writed(CD_AUDIO_POS, 0x7fffffff);

	host_writew(MK_FP(CDA_DATASEG, 0xab), 0);

	CD_driver_request((driver_request*)MK_FP(CDA_DATASEG, 0xa8));
}

/* Borlandified and identical */
void CD_audio_play(void)
{
	/* Is CD initialized ? */
	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	/* Is CD paused ? */
	if (ds_readw(CD_AUDIO_PAUSED) == 0)
		return;

	CD_check();

	/* reset CD pause */
	ds_writew(CD_AUDIO_PAUSED, 0);
	ds_writed(CD_AUDIO_POS, ds_readd(CD_AUDIO_PAUSE_POS));
	add_ds_ds(CD_AUDIO_TOD, (CD_get_tod() - ds_readds(CD_AUDIO_PAUSE_TOD)));
	host_writew(MK_FP(CDA_DATASEG, 0xc7), 0);

	CD_driver_request((driver_request*)MK_FP(CDA_DATASEG, 0xc4));
}

/* Borlandified and nearly identical */
static void CD_0432(void)
{
	signed short track_no;

	if (ds_readw(CD_INIT_SUCCESSFUL) == 0)
		return;

	host_writew(MK_FP(CDA_DATASEG, 0x3b), 0);
	host_writed(MK_FP(CDA_DATASEG, 0x46), (Bit32u)(MK_FP(CDA_DATASEG, 0x420)));
	host_writeb(MK_FP(CDA_DATASEG, 0x420), 0x0a);

	CD_driver_request((driver_request*)&req[2]);

	track_no = host_readb(MK_FP(CDA_DATASEG, 0x421));

	while (host_readb(MK_FP(CDA_DATASEG, 0x422)) >= track_no)
	{
		host_writew(MK_FP(CDA_DATASEG, 0x3b), 0);
		host_writed(MK_FP(CDA_DATASEG, 0x46), (Bit32u)(MK_FP(CDA_DATASEG, 8 * track_no + 0x108)));
		host_writeb(MK_FP(CDA_DATASEG, 8 * track_no + 0x108), 0x0b);
		host_writeb(MK_FP(CDA_DATASEG, 8 * track_no + 0x109), track_no);

		CD_driver_request((driver_request*)&req[2]);

		track_no++;
	}
}

struct dummy15 {
	signed short a[15];
};

/* Borlandified and identical */
void CD_set_track(const int index)
{
	signed short i;
#if defined(__BORLANDC__)
	struct dummy15 tracks = *(struct dummy15*)(p_datseg + CD_AUDIO_TRACKLIST);
#else
	struct dummy15 tracks;
	tracks.a[0] = 0x7fff;
	tracks.a[1] = 0x7fff;
	tracks.a[2] = 0x7fff;
	tracks.a[3] = 0x7fff;
	tracks.a[4] = ARCHIVE_FILE_SUMMARY_XMI;
	tracks.a[5] = ARCHIVE_FILE_THORWAL_XMI;
	tracks.a[6] = ARCHIVE_FILE_TEMPLE_XMI;
	tracks.a[7] = ARCHIVE_FILE_TERMS_XMI;
	tracks.a[8] = ARCHIVE_FILE_SMITH_XMI;
	tracks.a[9] = ARCHIVE_FILE_INN_XMI;
	tracks.a[10] = ARCHIVE_FILE_HEALER_XMI;
	tracks.a[11] = ARCHIVE_FILE_CAMP_XMI;
	tracks.a[12] = ARCHIVE_FILE_VICTORY_XMI;
	tracks.a[13] = ARCHIVE_FILE_COMBAT_XMI;
	tracks.a[14] = ARCHIVE_FILE_DUNGEON_XMI;
#endif

	for (i = 0; i < 15; i++)
	{
		if (tracks.a[i] == index) break;
	}

	ds_writew(CD_AUDIO_TRACK, i + 1);

	if (ds_readw(CD_AUDIO_TRACK_BAK) == ds_readw(CD_AUDIO_TRACK))
	{
	} else {
		ds_writew(CD_AUDIO_TRACK_BAK, ds_readw(CD_AUDIO_TRACK));
		CD_audio_stop_hsg();
		CD_audio_stop_hsg();

		seg001_00c1(ds_readw(CD_AUDIO_TRACK));

		ds_writew(CD_AUDIO_REPEAT, 1);

		if (ds_readw(CD_AUDIO_PAUSED) != 0)
		{
			ds_writew(CD_AUDIO_PAUSED, 0);
			CD_audio_pause();
		}
	}
}

/* Borlandified and identical */
static signed short CD_read_exe(char *path)
{
	int handle;
	signed short buffer;
	unsigned short nread;

	/* skip read check */
	if (ds_readd(CD_CHECK_SKIPMAGIC) == 0x682772e4) return 1;

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

	sprintf(str, (char*)p_datseg + STR_INSERT_CD, ds_readws(CD_DRIVE_NO) + 'A');

	answer = -2;
	while (answer == -2)
	{
		answer = GUI_radio((char*)str, 2,
					(char*)p_datseg + STR_REPEAT,
					(char*)p_datseg + STR_QUIT);
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
	if (ds_readw(CD_CHECK_ERR_COUNTDOWN) == 0)
	{
		CD_insert_msg();
		ds_writew(CD_CHECK_ERR_COUNTDOWN, 5);
	}

	dec_ds_ws_post(CD_CHECK_ERR_COUNTDOWN);

#if defined(__BORLANDC__)
	hardresume(1);
#endif

	return 1;
}

/* Borlandified and identical */
void CD_check(void)
{
	char text[80];

#if defined(__BORLANDC__)
	harderr((int(*)(int, int, int, int))CD_harderr_handler);
#endif

	strcpy(text, (char*)p_datseg + STR_CD_EXEPATH);

	text[0] = ds_readw(CD_DRIVE_NO) + 'A';

	while (CD_read_exe(text) <= 0)
	{
		CD_insert_msg();
	}
}

/* Borlandified and identical */
int CD_init(void)
{
	char str[80];

	if (!CD_set_drive_no())
	{
		GUI_output(p_datseg + STR_CD_MISSING);

		return 0;
	}

	sprintf(str, (char*)p_datseg + STR_CD_INIT, ds_readws(CD_DRIVE_NO) + 'A');

	GUI_output((Bit8u*)str);

	ds_writew(CD_INIT_SUCCESSFUL, 1);

	CD_audio_stop();

	CD_check();

	CD_0432();

	return 1;
}
#endif

#if !defined(__BORLANDC__)
}
#endif
