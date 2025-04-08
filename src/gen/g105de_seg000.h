#if !defined(__BORLANDC__)
void bc_exit(Bit16u);
RealPt bc__dos_getvect(Bit16s);
void bc__dos_setvect(Bit16s, RealPt);
RealPt bc_F_PADD(RealPt, Bit32s);
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
static inline signed short bioskey(signed short) { return 0; };
static inline void clrscr() { };
RealPt bc_farcalloc(Bit32s, Bit32s);
Bit16s bc__close(Bit16u);
Bit16s bc__create(RealPt, Bit16u);
Bit16s bc_flushall();
Bit16s bc_write(Bit16s, RealPt, Bit16u);

/* this will set an unused variable, so we put a dummy here */
static inline void bc_randomize() { }

#else
// __BORLANDC__

#include <IO.H>     // lseek, _read, _close, _creat, open, write
#include <STDLIB.H> // free, calloc (also in alloc.h)
#include <BIOS.H>   // bioskey, int86x (also in dos.h)
#include <DOS.H>
#include <CONIO.H> // clrscr
#include <STRING.H> // memcpy, memset
#include <MALLOC.H>  // farcalloc

#include "port.h"

#define bc_exit exit
#define bc__dos_getvect _dos_getvect
#define bc__dos_setvect _dos_setvect
#define bc_F_PADA(p, v) ((HugePt)(p) += (v))
#define bc_F_PADD(p, v) ((HugePt)(p) + v)
#define bc__read _read
#define bc_farcalloc farcalloc
#define bc_int86x int86x
#define bc__exit _exit
#define bc__close _close
#define bc__creat _creat
#define bc_flushall flushall
#define bc_write write
#define bc_randomize randomize

#endif
