/*
 *	Rewrite of DSA1 v3.02_de functions of seg010 (EMS)
 *	Functions rewritten: 8/8 (complete)
 *	Borlandified and identical
 *
 *	Remark: In the original game the EMS functions were written
 *		with Turbo assembler, so look in seg010.asm
 */
#include <string.h>

#include "v302de.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static signed int EMS_installed(void)
{
#if 0
	unsigned char *IRQ_67;
	char *sig;

	/* get interrupt vector of interrupt 0x67 */
	IRQ_67 = *(unsigned char*)MK_FP(0x0000, 0x67 * 4);

	/* Make a pointer to where "EMMXXXX0" stands if EMS is enabled */
	sig = (char*)MK_FP(FP_SEG(IRQ_67), 0x0a);

	/* return 1 if signature is found */
	if (!strncmp(sig, "EMMXXXX0", 8))
		return 1;
#endif

	return 0;
}

static uint8_t* EMS_get_frame_ptr()
{
#if 0
	reg_ax = 0x4100;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return MK_FP(reg_bx, 0);

#endif
	return NULL;
}

signed int EMS_get_num_pages_unalloced(void)
{
#if 0
	reg_ax = 0x04200;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return reg_bx;

#endif
	return 0;
}

signed int EMS_alloc_pages(const int16_t pages)
{
#if 0
	reg_ax = 0x4300;
	reg_bx = pages;
	CALLBACK_RunRealInt(0x67);

	if (reg_ax == 0)
		return reg_dx;
#endif

	return 0;
}

int16_t EMS_free_pages(const int16_t handle)
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x4500
		mov dx, handle;

		int 0x67

		mov ah, al
	}
	return _AX;
#else
	return 0;
#endif
}

int16_t EMS_map_memory(const int16_t handle, const int16_t lpage, const int16_t ppage)
{
#if defined(__BORLANDC__)
	asm {
		mov ax, 0x4400
		mov al, ppage
		mov bx, lpage
		mov dx, handle

		int 0x67

		mov ah, al
	}

	return _AX;
#else
	return 0;
#endif
}

uint8_t* EMS_norm_ptr(uint8_t* p)
{
#if 0
	uint8_t* retval;

	retval = FP_SEG(p) << 4;
	retval += FP_OFF(p);
	retval = retval << 12;
	retval |= (FP_OFF(p) & 0x0f);

	return retval;
#else
	return NULL;
#endif
}

signed short EMS_init(void)
{
	if (EMS_installed()) {
		g_ems_frame_ptr = EMS_get_frame_ptr();
		return 1;
	}

	return 0;
}

#if !defined(__BORLANDC__)
}
#endif
