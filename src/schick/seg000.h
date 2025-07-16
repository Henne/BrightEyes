#if !defined(__BORLANDC__)

namespace M302de {

void F_PADA(RealPt, Bit32s);
RealPt F_PADD(RealPt, Bit32s);
Bit32s F_PSUB(RealPt, RealPt);

static inline Bit8u* H_PADD(Bit8u *p, Bit32s o) { return p + o; }

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

#define H_PADD(p, o) F_PADD(p, o)

#define bc_itoa itoa

#define my_itoa itoa

#endif
