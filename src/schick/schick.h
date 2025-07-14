#ifndef SCHICK_H
#define SCHICK_H

#include <stdio.h>
#include <string.h>


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

static inline char* getString(RealPt p) {
        return (char*)(p);
}

static inline Bit8u* Real2Host(RealPt p) {
        return (Bit8u*)(p);
}


/**
 * struct_copy() - wrapper for struct assignments
 * @dst:	destination
 * @src:	source
 * @len:	length
 *
 * This is only a marker for copying structs.
 */
static inline void struct_copy(Bit8u *dst, Bit8u *src, int len)
{

	if (len < 0) {
		D1_ERR("ERROR: %s copy negative amount of bytes\n", __func__);
		return;
	}

	memcpy(dst, src, len);
}

extern const char* names_attrib[];
extern const char* names_skill[];
extern const char* names_spell[];
extern const char* names_mspell[];

extern unsigned short datseg;
extern Bit8u *p_datseg;
#define reloc_game (0)

static inline Bit8u ds_readb(unsigned short offs) {
	return host_readb(p_datseg + offs);
}
static inline Bit16u ds_readw(unsigned short offs) {
	return host_readw(p_datseg + offs);
}
static inline Bit32u ds_readd(unsigned short offs) {
	return host_readd(p_datseg + offs);
}

static inline Bit8s ds_readbs(unsigned short offs) {
	return (Bit8s)host_readb(p_datseg + offs);
}
static inline Bit16s ds_readws(unsigned short offs) {
	return (Bit16s)host_readw(p_datseg + offs);
}
static inline Bit32s ds_readds(unsigned short offs) {
	return (Bit32s)host_readd(p_datseg + offs);
}

static inline Bit8s ds_writebs(unsigned short offs, Bit8s val) {
	host_writeb(p_datseg + offs, val);
	return val;
}
static inline Bit8u ds_writeb(unsigned short offs, Bit8u val) {
	host_writeb(p_datseg + offs, val);
	return val;
}
static inline Bit16u ds_writew(unsigned short offs, Bit16u val) {
	host_writew(p_datseg + offs, val);
	return val;
}
static inline Bit16s ds_writews(unsigned short offs, Bit16s val) {
	host_writew(p_datseg + offs, val);
	return val;
}
static inline Bit32u ds_writed(unsigned short offs, Bit32u val) {
	host_writed(p_datseg + offs, val);
	return val;
}

static inline Bit16s ds_inc_ws(unsigned short offs) {
	return ++*(Bit16s*)(p_datseg + offs);
}

static inline Bit16s ds_dec_ws(unsigned short offs) {
	return --*(Bit16s*)(p_datseg + offs);
}

static inline Bit16s ds_and_ws(unsigned short offs, unsigned short val) {
	return *(Bit16s*)(p_datseg + offs) &= val;
}
#endif
