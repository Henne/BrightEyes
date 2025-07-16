/*
 *	Rewrite of DSA1 v3.02_de functions of seg000 (Borland C-Lib)
 *	Functions rewritten: 0/0
 */
#include "regs.h"
#include "callback.h"

#include "v302de.h"

#include "seg000.h"

namespace M302de {

void F_PADA(RealPt p, Bit32s off)
{
	reg_dx = RealSeg(p);
	reg_ax = RealOff(p);
	reg_cx = off >> 16;
	reg_bx = off & 0xffff;
	CALLBACK_RunRealFar(reloc_game, 0x928);
}

RealPt F_PADD(RealPt p, Bit32s off)
{
	reg_dx = RealSeg(p);
	reg_ax = RealOff(p);
	reg_cx = off >> 16;
	reg_bx = off & 0xffff;
	CALLBACK_RunRealFar(reloc_game, 0x9b0);
	return RealMake(reg_dx, reg_ax);
}

Bit32s F_PSUB(RealPt p1, RealPt p2)
{
	reg_dx = RealSeg(p1);
	reg_ax = RealOff(p1);

	reg_cx = RealSeg(p2);
	reg_bx = RealOff(p2);

	CALLBACK_RunRealFar(reloc_game, 0xa10);
	return ((reg_dx << 16) | reg_ax);
}

}
