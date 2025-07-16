#if !defined(__BORLANDC__)

namespace M302de {

static inline void F_PADA(Bit8u *p, Bit32s o)		{ return *p += o; }
static inline Bit8u* F_PADD(Bit8u* ptr, Bit32s o)	{ return ptr + o; }
static inline Bit32s F_PSUB(Bit8u *p1, Bit8u *p2)	{ return p1 - p2; }

static inline char* my_itoa(int value, char *string, int radix)
{
	sprintf(string, "%d", value);
	return string;
}

}
#else

#include <STDLIB.H>
#include <IO.H>
#include <DOS.H>
#include <BIOS.H>
#include <DIR.H>
#include <CTYPE.H>
#include <ALLOC.H>
#include <TIME.H>

#define RealSeg(p) FP_SEG(p)
#define RealOff(p) FP_OFF(p)
#define RealMake(s, o) MK_FP(s, o)

/* helper, use only when neccessary */
#define struct_copy memcpy

#define F_PADA(p, o) (*((HugePt*)p) += o)
#define F_PADD(p, o) ((HugePt)(p) + o)
#define F_PSUB(p1, p2) ((HugePt)(p1) - (HugePt)(p2))

#define my_itoa itoa

#endif
