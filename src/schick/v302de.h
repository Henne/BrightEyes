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

typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
#if defined(__BORLANDC__)
typedef unsigned long Bit32u;
typedef signed long Bit32s;
typedef Bit8u huge * HugePt;
#else
typedef unsigned int Bit32u;
typedef signed int Bit32s;
typedef Bit8u* HugePt;
#endif

/**
	struct nvf_desc - nvf descriptor
	@src:	pointer to a buffer containing the nvf file
	@dst:	pointer where to extract the picture
	@no:	number of the picture to extract
	@type:	kind of compression / direction (0 = PP20 / 2-5 RLE / copy)
	@p_height:	pointer where the height of the picture must be stored
	@p_width:	pointer where the width of the picture must be stored
*/
struct nvf_desc {
#if !defined(__BORLANDC__)
	Bit8u *dst;
	Bit8u *src;
#else
	Bit8u huge *dst;
	Bit8u huge *src;
#endif
	short no;
	signed char type;
	Bit8u *width;
	Bit8u *height;
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

/* for positions stored in 2 bytes, containing data level, x, y and possibly direction. */
#define TOWN_POS(x, y) (((x) << 8) + (y))
#define DNG_POS(level, x, y)	(((level) << 12) + ((x) << 8) + (y))
#define DNG_POS_DIR(level, x, y, dir) (DNG_POS(level, x, y) + ((dir) << 4))

/* for positions stored in 1 byte, containing x and y. */
#define MAP_POS(x,y) ((y) << 4) + (x) /* no outer parantheses, otherwise binary BCC-check will be broken! */

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

/* comment this out to have the original, but buggy behaviour */
#define M302de_ORIGINAL_BUGFIX

/* BrightEyes implements a few (moderate) feature extensions/mods.
 * comment this out to avoid the new features */
#define M302de_FEATURE_MOD

/* slow down when too fast */
#define M302de_SPEEDFIX


#define SCHICK_INFO
#define SCHICK_INFO_VERBOSE
//#define SCHICK_LOG
//#define SCHICK_TRAC
//#define SCHICK_GFX

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

#ifdef SCHICK_TRAC
#define D1_TRAC(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_TRAC(...) { }
#endif

#ifdef SCHICK_GFX
#define D1_GFX(...) fprintf(stderr, __VA_ARGS__)
#else
static inline void D1_GFX(...) { }
#endif

/* help Visual C++ to know __func__ */
#if _MSC_VER >= 1300
	#define __func__ __FUNCTION__
#endif


static inline Bit8u host_readb(Bit8u* p)
{
	return ((Bit8u)*p);
}

static inline Bit8s host_readbs(Bit8u* p)
{
	return ((Bit8s)*p);
}

static inline Bit16u host_readw(Bit8u* p)
{
	return ((Bit16u)(host_readb(p + 1) << 8) | (host_readb(p)));
}

static inline Bit16s host_readws(Bit8u* p)
{
	return (Bit16s)host_readw(p);
}

static inline Bit32s host_readd(Bit8u* p)
{
	return ((Bit32s)(host_readw(p + 2) << 16) | host_readw(p));
}

static inline Bit32s host_readds(Bit8u* p)
{
	return (Bit32s)host_readd(p);
}

static inline Bit8u host_writeb(Bit8u* p, Bit8u val)
{
	return (*p = val);
}

static inline Bit8s host_writebs(Bit8u* p, Bit8s val)
{
	host_writeb(p, val);
	return val;
}

static inline Bit16u host_writew(Bit8u* p, Bit16u val)
{
	host_writeb(p, val & 0xff);
	host_writeb(p + 1, (val >> 8)& 0xff);
	return val;
}

static inline Bit16s host_writews(Bit8u* p, Bit16s val)
{
	host_writew(p, val);
	return val;
}

static inline Bit32u host_writed(Bit8u* p, Bit32u val)
{
	host_writew(p, val & 0xffff);
	host_writew(p + 2, (val >> 16) & 0xffff);
	return val;
}

static inline Bit32s host_writeds(Bit8u* p, Bit32s val)
{
	host_writed(p, val);
	return val;
}

/**
 * \brief mark informer only as known iff unknown
 * \param informer the index of the informer
 *
 * \note A often occuring Original-Bug resets some informers to older states.
 * 	This helper marks the informer only as known iff the informer is unknown (0).
 */
static inline void update_informer_cond(const int informer)
{
	if ((0 <= informer) && (informer < 15) && (M302de::gs_informer_flags[informer] == 0)) {
		M302de::gs_informer_flags[informer] = 1;
	}
}

static inline unsigned char *get_hero(signed short index) {
	if (index < 0 || index > 6) {
		D1_ERR("ERROR: Versuch auf Held an Position %d zuzugreifen\n", index);
	}
	return M302de::g_heroes + index * SIZEOF_HERO;
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

/**
 * hero_dead() -	check if hero is dead
 * @hero:	ptr to hero
 *
 * 0 = alive / 1 = dead
 */
static inline unsigned short hero_dead(Bit8u *hero) {
	if ((host_readb(hero + 0xaa) & 1))
		return 1;
	else
		return 0;
}

/**
 * hero_unconscious() -	check if hero is unconscious
 * @hero:	ptr to hero
 *
 * 0 = awake / 1 = unconscious
 */
static inline unsigned short hero_unconscious(Bit8u *hero) {
	if (((host_readb(hero + 0xaa) >> 6) & 1) == 0)
		return 0;
	else
		return 1;
}


static inline unsigned short hero_seen_phantom(Bit8u *hero) {

	if (((host_readb(hero + 0xab) >> 4) & 1) == 0)
		return 0;
	else
		return 1;
}

static inline unsigned short hero_seen_phantom_set(Bit8u *hero, unsigned short val)
{
	/* unset this bit */
	host_writeb(hero + HERO_FLAGS2, host_readb(hero + HERO_FLAGS2) & 0xef);
	host_writeb(hero + HERO_FLAGS2, host_readb(hero + HERO_FLAGS2) | ((val & 1) << 4));
	return (val & 1);
}

static inline Bit8u *get_spelltarget_e(void) {
	return (Bit8u*)M302de::g_spelltarget_e;
}


#if !defined(__BORLANDC__)
namespace M302de {

static inline struct struct_hero *get_spelltarget(void) {
	return (struct struct_hero*)g_spelltarget;
}

static inline struct struct_hero *get_spelluser(void) {
	return (struct struct_hero*)g_spelluser;
}

}
#endif


static inline char *get_monname(const int no)
{
	if ((0 <= no) && (no < 77))
		return M302de::g_monnames_index[no];
	else {
		fprintf(stderr, "ERROR: %s[%d] is out of bounds\n", __func__, no);
		return NULL;
	}
}

static inline char *get_tx2(unsigned short no) {
	return (char*)M302de::g_tx2_index[no];
}

static inline char *get_ttx(unsigned short no) {
	return (char*)M302de::g_text_ltx_index[no];
}

static inline char *get_tx(unsigned short no) {
	return (char*)M302de::g_tx_index[no];
}

static inline signed char get_cb_val(const signed short x, const signed short y) {
	return *(M302de::g_chessboard + 25 * y + x);
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

static inline char* get_itemname(unsigned short item)
{
	return (char*)M302de::g_itemsname[item];
}

#define DUMMY_WARNING() D1_ERR("Error: %s is not implemented\n", __func__)

// end #if !defined(__BORLANDC__)
#else

#define DUMMY_WARNING()

#undef M302de_ORIGINAL_BUGFIX

/* helper, use only when neccessary */

#define my_itoa itoa

#define get_hero(no) ((unsigned char*)g_heroes + SIZEOF_HERO * (no))

#ifdef M302de_ORIGINAL_BUGFIX
#define update_informer_cond(informer) (if (gs_informer_flags[informer] == 0) gs_informer_flags[informer] = 1)
#else
#define update_informer_cond(informer) (gs_informer_flags[informer] = 1)
#endif

#define host_readb(p) (*(Bit8u*)(p))
#define host_readw(p) (*(Bit16u*)(p))
#define host_readd(p) (*(Bit32u*)(p))

#define host_readbs(p) (*(Bit8s*)(p))
#define host_readws(p) (*(Bit16s*)(p))
#define host_readds(p) (*(Bit32s*)(p))

#define host_writeb(p, d)	(*(Bit8u*)(p) = (d))
#define host_writew(p, d)	(*(Bit16u*)(p) = (d))
#define host_writed(p, d)	(*(Bit32u*)(p) = (d))

#define host_writebs(p, d)	(*(Bit8s*)(p) = (d))
#define host_writews(p, d)	(*(Bit16s*)(p) = (d))
#define host_writeds(p, d)	(*(Bit32s*)(p) = (d))

#define hero_dead(hero)		((*(struct hero_flags*)(hero + HERO_FLAGS1)).dead)
#define hero_unconscious(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).unconscious)



#define hero_seen_phantom(hero)	((*(struct hero_flags*)(hero + HERO_FLAGS1)).seen_phantom)

#define hero_seen_phantom_set(hero, v) ((*(struct hero_flags*)(hero + HERO_FLAGS1)).seen_phantom = v)

#define get_spelltarget_e()	((Bit8u*)g_spelltarget_e)
#define get_spelltarget()	((struct struct_hero*)g_spelltarget)
#define get_spelluser()		((struct struct_hero*)g_spelluser)

#define get_ttx(no) ((char*)(g_text_ltx_index[(no)]))
#define get_tx(no) ((char*)(g_tx_index[(no)]))
#define get_tx2(no) ((char*)(g_tx2_index[(no)]))
#define get_monname(no) ((char*)g_monnames_index[no])
#define get_itemname(no) ((char*)g_itemsname[(no)])

#define get_cb_val(x, y) (*(g_chessboard + (y) * 25 + (x)))

#endif
#endif
