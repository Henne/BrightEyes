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

void save_display_stat(RealPt p)
{
}

void set_color(RealPt ptr, unsigned char color)
{
}

void set_palette(RealPt ptr, unsigned char first_color, unsigned short colors)
{
	unsigned short i;
	for (i = 0; i < colors; i++)
		set_color(ptr + 3 * i, first_color + i);
}

void draw_h_line(Bit16u offset, Bit16s count, Bit16u color)
{
	PhysPt ptr = Real2Phys(RealMake(0xa000, offset));
	Bit16u i;

	for (i = 0; i < count; i++)
		mem_writeb(ptr + i, color);
}

void draw_h_spaced_dots(Bit16u offset, Bit16u width, Bit16s color, Bit16u space)
{
	PhysPt ptr = Real2Phys(RealMake(0xa000, offset));
	Bit16u i;

	for (i = 0; i < width; i++) {
		mem_writeb(ptr, color);
		ptr += space;
	}
}

void pic_copy(RealPt dst, Bit16u x, Bit16u y, Bit16u d1, Bit16u d2,
		Bit16u v1, Bit16u v2, Bit16u d3, Bit16u d4,
		Bit16u w, Bit16u h, RealPt src, Bit16u mode)
{
	PhysPt d, s;

	d = Real2Phys(dst) + y * 320 + x;
	s = Real2Phys(src);

	switch (mode) {
		/* this is not used in GEN */
		case 1: {
			Bit16u diff, i;

			diff = 320 - w;
			do {
				for (i = w; i; i--) {
					if (mem_readb_inline(d) < 0xc8)
						mem_writeb_inline(d, mem_readb_inline(s));
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
					if ((al = mem_readb_inline(s++)))
						mem_writeb_inline(d, al);
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
				for (i = w; i > 0; i--)
					mem_writeb_inline(d++,
						mem_readb_inline(s++));
				d += diff;
				s += diff;
			} while (--h > 0);
			break;
		}
		default: {
			Bit16u diff, i;

			diff = 320 - w;

			do {
				for (i = w; i; i--)
					mem_writeb_inline(d++,
						mem_readb_inline(s++));
				d += diff;
			} while (--h > 0);

			break;
		}
	}
}

void save_rect(Bit16u dst1, Bit16u dst2, RealPt src, Bit16s diffX, Bit16s diffY)
{

}

void fill_rect(Bit16u p_seg, Bit16u p_off, Bit16s color, Bit16s width, Bit16s height)
{
	PhysPt p = Real2Phys(RealMake(p_seg, p_off));
	Bit16s x;

	for (; height; height--) {
		for (x = 0; x < width; x++) {
			mem_writeb(p++ , color);
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
