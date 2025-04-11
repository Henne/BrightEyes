/*
 *	Rewrite of DSA1 Generator v1.05_de seg005 (Rasterlib)
 *	Functions rewritten: 2/2
 *
 *	We only rewrite those which differ from the game.
 *	These functions were written in assembler and are
 *      just a clean C-implementation.
*/

#include <stdlib.h>

#include "port.h"


void set_video_mode(Bit16u mode)
{
}

void set_video_page(Bit16u mode)
{
}

void save_display_stat(signed short *p)
{
}

void set_color(signed char *ptr, unsigned char color)
{
}

void set_palette(signed char *ptr, unsigned char first_color, unsigned short colors)
{
	unsigned short i;
	for (i = 0; i < colors; i++)
		set_color(ptr + 3 * i, first_color + i);
}

void draw_h_line(unsigned short offset, unsigned short count, unsigned short color)
{
	//unsigned char *ptr = MK_FP(0xa000, offset);
	unsigned char *ptr = NULL;
	unsigned short i;

	for (i = 0; i < count; i++)
		ptr[i] = color;
}

void draw_h_spaced_dots(unsigned short offset, unsigned short width, signed short color, unsigned short space)
{
	//unsigned char *ptr = MK_FP(0xa000, offset);
	unsigned char *ptr = NULL;
	unsigned short i;

	for (i = 0; i < width; i++) {
		ptr[0] = color;
		ptr += space;
	}
}

void pic_copy(unsigned char *dst, Bit16u x, Bit16u y, Bit16u d1, Bit16u d2,
		Bit16u v1, Bit16u v2, Bit16u d3, Bit16u d4,
		Bit16u w, Bit16u h, unsigned char *src, Bit16u mode)
{
	unsigned char *d;
	unsigned char *s;

	d = dst + y * 320 + x;
	s = src;

	switch (mode) {
		/* this is not used in GEN */
		case 1: {
			Bit16u diff, i;

			diff = 320 - w;
			do {
				for (i = w; i; i--) {
					if (d[0] < 0xc8)
						d[0] = s[0];
					s++;
					d++;
				}
				d += diff;
			} while (--h > 0);
			break;
		}
		case 2: {
			Bit16u diff, i;
			Bit8u al;

			diff = 320 - w;

			do {
				for (i = w; i != 0; i--) {
					if ((al = *s++))
						d[0] = al;
					d++;
				}
				d += diff;
			} while (--h > 0);

			break;
		}
		case 3: {
			Bit16u diff, i;

			s += v2 * 320 + v1;
			diff = 320 - w;

			do {
				for (i = w; i > 0; i--) {
					d[0] = s[0];
					d++;
					s++;
				}
				d += diff;
				s += diff;
			} while (--h > 0);
			break;
		}
		default: {
			Bit16u diff, i;

			diff = 320 - w;

			do {
				for (i = w; i; i--) {
					d[0] = s[0];
					d++;
					s++;
				}
				d += diff;
			} while (--h > 0);

			break;
		}
	}
}

#if defined(__BORLANDC__)
void save_rect(unsigned short p_seg, unsigned short p_off, unsigned char *src, signed short diffX, signed short diffY)
#else
void save_rect(unsigned char *p_in, unsigned char *src, signed short diffX, signed short diffY)
#endif
{

}

#if defined(__BORLANDC__)
void fill_rect(unsigned short p_seg, unsigned short p_off, signed short color, signed short width, signed short height)
#else
void fill_rect(unsigned char *p_in, signed short color, signed short width, signed short height)
#endif
{
#if defined(__BORLANDC__)
	unsigned char *p = MK_FP(p_seg, p_off);
#else
	unsigned char *p = p_in;
#endif
	signed short x;

	for (; height; height--) {
		for (x = 0; x < width; x++) {
			p[0] = color;
			p++;
		}
		p += 320 - width;
	}
}

unsigned short swap_u16(unsigned short val)
{
	return (val << 8) | (val >> 8);
}

void copy_to_screen(RealPt src, RealPt dst, Bit16s w, Bit16s h, Bit16s mode)
{

	Bit16s v1;
	Bit16s v2;
	Bit16s i;

	v1 = v2 = 320 - w;

	switch (mode & 0x7f) {
		case 0:	v2 = 0;
			break;
		case 2: v1 = 0;
			break;
	}

	if (mode & 0x80) {
		/* This does not happen */
		//D1_ERR("%s mode bit set\n", __func__);
		//exit(0);
	} else {
		for (; h; h--) {
			for (i = 0; i < w; i++)
				mem_writeb(Real2Phys(dst++), mem_readb(Real2Phys(src++)));
			dst += v1;
			src += v2;
		}
	}
}

RealPt normalize_ptr(RealPt ptr)
{
	return ptr;
}
