#if !defined(__BORLANDC__)

namespace M302de {

struct ffblk {
	Bit8u a[44];
};

void F_PADA(RealPt, Bit32s);
RealPt F_PADD(RealPt, Bit32s);
Bit32s F_PSUB(RealPt, RealPt);

static inline Bit8u* H_PADD(Bit8u *p, Bit32s o) { return p + o; }

Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
signed short bc_unlink(RealPt);
static inline void clrscr(void) { };
void bc_farfree(RealPt);
Bit32u bc_farcoreleft(void);
void bc_harderr(RealPt);
void bc_hardresume(Bit16s);

Bit16s bc_spawnl(Bit16s, RealPt, RealPt, RealPt, RealPt, RealPt, RealPt);

Bit16s bc__creat(RealPt, Bit16u);
void bc_perror(RealPt);
Bit16s bc_open(RealPt, Bit16u);

void bc_qsort(RealPt, Bit16u, Bit16u, RealPt);

static inline char* my_itoa(int value, char *string, int radix)
{
	sprintf(string, "%d", value);
	return string;
}

Bit32s bc__write(Bit16u, RealPt, Bit16u);

}
#else

#include <STDLIB.H>
#include <IO.H>
#include <DOS.H>
#include <BIOS.H>
#include <DIR.H>
#include <CTYPE.H>
#include <ALLOC.H>
#include <CONIO.H>
#include <TIME.H>
#include <PROCESS.H>

#define RealSeg(p) FP_SEG(p)
#define RealOff(p) FP_OFF(p)
#define RealMake(s, o) MK_FP(s, o)

/* helper, use only when neccessary */
#define struct_copy memcpy

#define F_PADA(p, o) (*((HugePt*)p) += o)
#define F_PADD(p, o) ((HugePt)(p) + o)
#define F_PSUB(p1, p2) ((HugePt)(p1) - (HugePt)(p2))

#define H_PADD(p, o) F_PADD(p, o)

#define bc_harderr harderr
#define bc_hardresume hardresume
#define bc_spawnl spawnl

#define bc__creat _creat

#define bc_perror perror
#define bc__read _read
#define bc__write write

#define bc_unlink unlink

#define bc_farfree farfree
#define bc_farcoreleft farcoreleft

#define bc_itoa itoa

#define bc_open open

#define my_itoa itoa

#define bc_qsort qsort

#define DOS_SEEK_SET SEEK_SET
#endif
