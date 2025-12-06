/*
 *	transitional functions for version 3.02_de
 *
 *	Here are function, which should make the transition from
 *	DOXBox emulation to native easier.
 *	These functions will only work with the version mentioned above.
 */

#if !defined V302DE_H
#define V302DE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__BORLANDC__)
#include <IO.H>
#include <DOS.H>
#include <BIOS.H>
#include <DIR.H>
#include <CTYPE.H>
#include <ALLOC.H>
#include <TIME.H>
#endif

#if !defined(__BORLANDC__)
/* comment this out to have the original, but buggy behaviour */
#define M302de_ORIGINAL_BUGFIX

/* BrightEyes implements a few (moderate) feature extensions/mods.
 * comment this out to avoid the new features */
#define M302de_FEATURE_MOD

#define SCHICK_INFO
#define SCHICK_INFO_VERBOSE
//#define SCHICK_LOG

#define D1_ERR(...) fprintf(stderr, __VA_ARGS__)

#ifdef SCHICK_LOG
#define D1_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_LOG(...) { }
#endif

#ifdef SCHICK_INFO
#define D1_INFO(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_INFO(...) { }
#endif

#ifdef SCHICK_INFO_VERBOSE
#define D1_INFO_VERBOSE(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_INFO_VERBOSE(...) { }
#endif
#endif

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
#if defined(__BORLANDC__)
typedef unsigned long uint32_t;
typedef signed long int32_t;
typedef uint8_t huge * HugePt;
#else
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef uint8_t* HugePt;
#endif

/**
	struct nvf_extract_desc - descriptor for extracting an image from a nvf file
	@src:	pointer to a buffer containing the nvf file
	@dst:	pointer where to extract the picture
	@image_num:	number of the picture to extract
	@compression_type:	kind of compression / direction (0 = PP20 / 2-5 RLE / copy)
	@p_height:	pointer where the height of the picture must be stored
	@p_width:	pointer where the width of the picture must be stored
*/
struct nvf_extract_desc {
#if !defined(__BORLANDC__)
	uint8_t *dst;
	uint8_t *src;
#else
	uint8_t huge *dst;
	uint8_t huge *src;
#endif
	signed int image_num;
	signed char compression_type;
	signed int *width;
	signed int *height;
};

/**
 *	struct screen_rect - coordinates for a rectangle on the screen
 *	@y1:	upper Y-Coordinate
 *	@x1:	upper X-Coordinate
 *	@y2:	lower Y-Coordinate
 *	@x2:	lower X-Coordinate
 */
struct screen_rect {
	short y1, x1, y2, x2;
};


#include "common.h"
#include "datseg.h"

#define ROUNDED_DIVISION(n,k)	((n + (k-1)/2)/k)
/* divide n/k and round to the closest integer. In ambigous cases, round down. */
/* only used in seg064.cpp */
/* (n + k/2)/k would be more natural, but it is done in this way in the original game */

#define SECONDS(n)      ((n/2) * 3L)
#define MINUTES(n)	((n) * 90L)
#define HOURS(n)	(MINUTES(n) * 60L)
#define DAYS(n)		(HOURS(n) * 24L)
#define MONTHS(n)	(DAYS(n) * 30L)

#define FIGHT_ROUNDS(n)	(n * 9L)
/* The duration of a fight round is 9 ticks, which is six seconds. */

/* for positions stored in 2 bytes, containing data level, x, y and possibly direction. */
#define TOWN_POS(x, y) (((x) << 8) + (y))
#define DNG_POS(level, x, y)	(((level) << 12) + ((x) << 8) + (y))
#define DNG_POS_DIR(level, x, y, dir) (DNG_POS(level, x, y) + ((dir) << 4))

/* for positions stored in 1 byte, containing x and y. */
#define MAP_POS(x,y) ((y) << 4) + (x) /* no outer parantheses, otherwise binary BCC-check will be broken! */

/* for positions in cities (i.e, x_size 32) stored in 2 bytes */
#define LARGE_MAP_POS(x,y) ((y) << 5) + (x) /* no outer parantheses, otherwise binary BCC-check will be broken! */

/* HACK: this cast is not optimized by Borland C++ 3.1 */
static inline unsigned short cast_u16(unsigned char v)
{
	return (unsigned short)v;
}

#if !defined(__BORLANDC__)

#define _open open
#define _creat creat
#define _read read

static inline int bioskey(const int cmd) { return 0; }

/* help Visual C++ to know __func__ */
#if _MSC_VER >= 1300
	#define __func__ __FUNCTION__
#endif

/**
 * \brief mark informer only as known iff unknown
 * \param informer the index of the informer
 *
 * \note A often occuring Original-Bug resets some informers to older states.
 * 	This helper marks the informer only as known iff the informer is unknown (0).
 */
static inline void update_informer_cond(const int informer)
{
	if ((0 <= informer) && (informer < 15) && (gs_informer_flags[informer] == 0)) {
		gs_informer_flags[informer] = 1;
	}
}

static inline int __abs__(int j)
{
	return abs(j);
}

static inline char* my_itoa(int value, char *string, int radix)
{
	sprintf(string, "%d", value);
	return string;
}

#if !defined(__BORLANDC__)
static inline struct struct_hero *get_hero(signed short index) {
	if (index < 0 || index > 6) {
		D1_ERR("ERROR: Versuch auf Held an Position %d zuzugreifen\n", index);
	}
	return &g_heroes[index];
}

static inline struct struct_hero *get_spelltarget(void) {
	return (struct struct_hero*)g_spelltarget;
}

static inline struct struct_hero *get_spelluser(void) {
	return (struct struct_hero*)g_spelluser;
}
#endif


static inline char *get_tx2(unsigned short no) {
	return (char*)g_tx2_index[no];
}

static inline char *get_ttx(unsigned short no) {
	return (char*)g_text_ltx_index[no];
}

static inline char *get_tx(unsigned short no) {
	return (char*)g_tx_index[no];
}

static inline signed char get_cb_val(const signed short x, const signed short y) {
	return *(g_chessboard + 25 * y + x);
}

static inline void dump_cb(void)
{
	FILE *fd;
	int y, x;
	signed char val;

	fd = fopen("cb_dump.txt", "w");

	if (fd == NULL)
		return;
	fprintf(fd, "\n");
	for (y = 24; y >= 0; y--) {
		for (x = 0; x < 25; x++) {
			val = get_cb_val(x, y);
			if (val != 0)
				fprintf(fd, "%3d ", val);
			else
				fprintf(fd, "    ");
		}
		fprintf(fd, "\n");
	}
	fclose(fd);
}

#define DUMMY_WARNING() D1_ERR("Error: %s is not implemented\n", __func__)

// end #if !defined(__BORLANDC__)
#else

#define DUMMY_WARNING()

#undef M302de_ORIGINAL_BUGFIX

/* helper, use only when neccessary */

#define my_itoa itoa

#define get_hero(no) ((struct struct_hero*)(((unsigned char*)g_heroes) + sizeof(struct struct_hero) * (no)))

#ifdef M302de_ORIGINAL_BUGFIX
#define update_informer_cond(informer) (if (gs_informer_flags[informer] == 0) gs_informer_flags[informer] = 1)
#else
#define update_informer_cond(informer) (gs_informer_flags[informer] = 1)
#endif

#define get_spelltarget()	((struct struct_hero*)g_spelltarget)
#define get_spelluser()		((struct struct_hero*)g_spelluser)

#define get_ttx(no) ((char*)(g_text_ltx_index[(no)]))
#define get_tx(no) ((char*)(g_tx_index[(no)]))
#define get_tx2(no) ((char*)(g_tx2_index[(no)]))

#define get_cb_val(x, y) (*(g_chessboard + (y) * 25 + (x)))

#endif
#endif
