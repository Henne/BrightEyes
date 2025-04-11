#include "port.h"

#if !defined(__BORLANDC__)
RealPt bc_F_PADD(RealPt, Bit32s);
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
Bit16s bc__close(Bit16u);
Bit16s bc__create(RealPt, Bit16u);
Bit16s bc_write(Bit16s, RealPt, Bit16u);

#else
// __BORLANDC__

#include <IO.H>     // lseek, _read, _close, _creat, open, write
#include <STDLIB.H> // free, calloc (also in alloc.h)
#include <BIOS.H>   // bioskey, int86x (also in dos.h)
#include <DOS.H>
#include <CONIO.H> // clrscr
#include <STRING.H> // memcpy, memset
#include <MALLOC.H>  // farcalloc

#define bc_F_PADA(p, v) ((HugePt)(p) += (v))
#define bc_F_PADD(p, v) ((HugePt)(p) + v)
#define bc__read _read
#define bc__close _close
#define bc__creat _creat
#define bc_write write

#endif
