/*
 *	Rewrite of DSA1 v3.02_de functions of seg000 (Borland C-Lib)
 *	Functions rewritten: 0/0
 */
#include "dosbox.h"
#include "regs.h"
#include "callback.h"
#include "dos_inc.h"
#include "paging.h"

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

Bit16s bc__read(Bit16u handle, Bit8u *buf, Bit16u count) {

	if ((ds_readw(BC_FILEHANDLE_FLAGS + handle * 2) & 2))
		return -1;

	if (!DOS_ReadFile(handle, buf, &count))
		return -1;

	if (count == 5952)
		schick_status_update(buf, count);

	return (Bit16s)count;
}

signed short bc_unlink(RealPt fname)
{
	CPU_Push32(fname);
	CALLBACK_RunRealFar(reloc_game + 0, 0x11a7);
	CPU_Pop32();
	return reg_ax;
}

void bc_farfree(RealPt ptr)
{
	CPU_Push32(ptr);
	CALLBACK_RunRealFar(reloc_game + 0, 0x1e55);
	CPU_Pop32();
}

Bit32u bc_farcoreleft(void)
{
	CALLBACK_RunRealFar(reloc_game + 0, 0x2315);
	return (reg_dx << 16) + reg_ax;
}

void bc_harderr(RealPt p)
{
	CPU_Push32(p);
	CALLBACK_RunRealFar(reloc_game, 0x2411);
	CPU_Pop32();
}

void bc_hardresume(Bit16s __axret)
{
	CPU_Push16(__axret);
	CALLBACK_RunRealFar(reloc_game, 0x2423);
	CPU_Pop16();
}

Bit16s bc_spawnl(Bit16s mode, RealPt a2, RealPt a3, RealPt a4, RealPt a5, RealPt a6, RealPt a7)
{
	CPU_Push32(a7);
	CPU_Push32(a6);
	CPU_Push32(a5);
	CPU_Push32(a4);
	CPU_Push32(a3);
	CPU_Push32(a2);
	CPU_Push16(mode);
	CALLBACK_RunRealFar(reloc_game, 0x2d82);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop32();
	CPU_Pop32();

	return reg_ax;
}

Bit16s bc__creat(RealPt name, Bit16u attrib)
{
	CPU_Push16(attrib);
	CPU_Push32(name);
	CALLBACK_RunRealFar(reloc_game + 0, 0x2f25);
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
}

void bc_perror(RealPt __s)
{
	CPU_Push32(__s);
	CALLBACK_RunRealFar(reloc_game + 0, 0x3685);
	CPU_Pop32();
}

Bit16s bc_open(RealPt fname, Bit16u attrib)
{
	CPU_Push16(attrib);
	CPU_Push32(fname);
	CALLBACK_RunRealFar(reloc_game + 0, 0x34c7);
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
}

void bc_qsort(RealPt base, Bit16u nmemb, Bit16u size, RealPt fcmp)
{
	CPU_Push32(fcmp);
	CPU_Push16(size);
	CPU_Push16(nmemb);
	CPU_Push32(base);
	CALLBACK_RunRealFar(reloc_game + 0, 0x3d74);
	CPU_Pop32();
	CPU_Pop16();
	CPU_Pop16();
	CPU_Pop32();
}

Bit32s bc__write(Bit16u fd, RealPt buf, Bit16u len)
{
	CPU_Push16(len);
	CPU_Push32(buf);
	CPU_Push16(fd);
	CALLBACK_RunRealFar(reloc_game + 0, 0x4a85);
	CPU_Pop16();
	CPU_Pop32();
	CPU_Pop16();

	return reg_ax;
}

}
