/*
 *	Rewrite of DSA1 v3.02_de functions of segment 008 (Rasterlib)
 *	Functions rewritten: 14/14 (complete)
 *
 *	Borlandified and identical
 *	This is not that true, but since this segment is written in assembler,
 *	and the stats are generated from cpp-files only this is the way to do it.
 *
 *	In the assembly, there is also code for OPL3-Programming, which is not used in the game,
 *	so we do not need to rewrite them in C.
 *
 *	Take a look at seg008.asm
 *
 */

#include "v302de.h"


#if !defined(__BORLANDC__)
namespace M302de {
#endif

unsigned short swap_u16(unsigned short val)
{
	return (val << 8) | (val >> 8);
}

void set_video_mode(signed short mode)
{
}

void set_video_page(signed short page)
{
}

void save_display_stat(Bit8u* p)
{
}

void set_color(Bit8u *ptr, unsigned char color)
{
}

void set_palette(Bit8u *ptr, unsigned short first_color, unsigned short colors)
{
}

void draw_h_line(Bit8u *ptr, unsigned short count, signed short color) {
	unsigned short i;

	for (i = 0; i < count; i++)
		*(ptr + i) = color;
}

void draw_h_spaced_dots(Bit8u *ptr, signed short count, signed short color, signed short space)
{
	unsigned short i;

	for (i = 0; i < count; i++) {
		*ptr = color;
		ptr += space;
	}
}

void pic_copy(Bit8u *dst, short x1, short y1, short x2, short y2,
	unsigned short val1, unsigned short val2,
	unsigned short val3, unsigned short val4,
	unsigned short src_width, unsigned short src_height,
	Bit8u *src, unsigned short mode) {

	signed int r_y2, r_y1, r_x1, r_x2;
	signed int cur_height;
	signed int cur_width;
	signed int lv1, lv2, lv3, lv4, lv5;

	cur_width = src_width;
	cur_height = src_height;

	lv1 = 0;
	lv2 = 0;
	lv3 = 0;
	lv4 = 0;

	r_y1 = g_pic_copy_rect.y1;
	r_x1 = g_pic_copy_rect.x1;
	r_y2 = g_pic_copy_rect.y2;
	r_x2 = g_pic_copy_rect.x2;

	if (y1 < r_y1) {
		lv3 = r_y1 - y1;
		cur_height -= lv3;
		src += lv3 * src_width;
	}

	if (y2 > r_y2) {
		lv4 = y2 - r_y2;
		cur_height -= lv4;
	}

	if (x1 < r_x1) {
		lv1 = r_x1 - x1;
		cur_width -= lv1;
		src += lv1;
	}


	if (x2 > r_x2) {
		lv2 = x2 - r_x2;
		cur_width -= lv2;
	}

	if (cur_height <= 0)
		return;

	if (cur_width <= 0)
		return;

	dst += r_y1 * 320 + r_x1;

	if (y1 > r_y1)
		dst += (y1 - r_y1) * 320;

	if (x1 > r_x1)
		dst += x1 - r_x1;

	lv5 = 0;
	lv5 = src_width - cur_width;

	switch (mode) {
	case 1: {
		short bx, cols, lines;

		lines = cur_height;
		bx = 320 - cur_width;
		do {
			cols = cur_width;
			if (g_pic_copy_flag) {
				do {
					if (lines >= 40 || cols <= 75 || cols >= 150)
						if (*dst >= 0xc8)
							continue;

					*dst = *src;

				} while (src++ && dst++ && --cols);
			} else {
				while (cols--) {
					if (*dst < 0xc8)
						*dst = *src;
					src++;
					dst++;
				}
			}
			dst += bx;
			src += lv5;
		} while (--lines > 0);

		break;
	}
	case 2: {
		short bx, lines, cols;

		lines = cur_height;
		bx = 320 - cur_width;

		do {
			cols = cur_width;
			do {
				if (*src != 0)
					*dst = *src;
				src++;
				dst++;
			} while (--cols);
			dst += bx;
			src += lv5;
		} while (--lines > 0);

		break;
	}
	case 3: {
		short bx, lines, cols;

		src += val2 * 320 + val1;

		lines = cur_height;
		bx = 320 - cur_width;
		do {
			for (cols = cur_width; cols; cols--)
				*dst++ = *src++;
			dst += bx;
			src += bx;
		} while (--lines > 0);
		break;
	}
	default: {
		short bx, lines, cols;
		lines = cur_height;
		bx = 320 - cur_width;
		do {
			for (cols = cur_width; cols; cols--)
				*dst++ = *src++;
			dst += bx;
			src += lv5;
		} while (--lines > 0);
	}
	}
}

#if defined(__BORLANDC__)
void save_rect(Bit16u seg, Bit16u off, Bit8u *dst, unsigned short width, unsigned short height)
#else
void save_rect(Bit8u* src, Bit8u *dst, unsigned short width, unsigned short height)
#endif
{
	for (; height; height--) {
#if defined(__BORLANDC__)
		memcpy((void*)dst, MK_FP(seg, off), width);
#else
		memcpy((void*)dst, (void*)src, width);
#endif
#if defined(__BORLANDC__)
		off += 320;
#else
		src += 320;
#endif
		dst += width;
	}
}

#if defined(__BORLANDC__)
void fill_rect(Bit16u seg, Bit16u off, signed short color, signed short width, signed short height)
#else
void fill_rect(Bit8u *dst, signed short color, signed short width, signed short height)
#endif
{
	unsigned short x;

	for (; height; height--) {
		for (x = 0; x < width; x++) {
#if defined(__BORLANDC__)
			*MK_FP(seg, off++) = color;
#else
			*dst++ = color;
#endif
		}

#if defined(__BORLANDC__)
		off += 320 - width;
#else
		dst += 320 - width;
#endif
	}
}

void copy_solid_permuted(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid) {

	Bit8u *s, *d;
	unsigned short y,x;

	s = src;
	d = dst;
	for (y = 0; y < height; y++, s = src += src_width, d = dst += dst_width)
		for (x = width_to_copy; x ; x--) {
			if (*s != solid)
				*d = *s;
			d++;
			s--;
		}
}

void copy_solid(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid) {

	Bit8u *s, *d;
	unsigned short y,x;

	s = src;
	d = dst;
	for (y = 0; y < height; y++, s = src += src_width, d = dst += dst_width)
		for (x = width_to_copy; x ; x--) {
			if (*s != solid)
				*d = *s;
			s++;
			d++;
		}
}

void decomp_rle(unsigned short width, unsigned short height, Bit8u *dst,
	Bit8u *src, Bit8u *tmp_buffer, unsigned short mode) {

	Bit8u *my_dst;
	unsigned short i,x;
	unsigned char tmp;
	unsigned char cnt;

	/* select destination buffer */
	if (mode == 5 || mode == 4)
		my_dst = tmp_buffer;
	else
		my_dst = dst;

	do {
		/* decode one line */
		x = width;
		do {
			tmp = *src++;
			if (tmp == 0x7f) {
				cnt = *src++;
				tmp = *src++;
				for (i = 0; i < cnt; i++)
					*my_dst++ = tmp;
				x -= cnt;
			} else {
				*my_dst++ = tmp;
				x--;
			}
		} while (x);

		/* reverse line */
		if (mode == 5 || mode == 4) {
			my_dst--;
			for (i = width; i; i--)
				*dst++ = *my_dst--;
		}

		/* set destination to next line */
		switch (mode) {
		case	5:	break;
		case	3:	dst += width;
				break;
		case	2:	dst += 320;
				break;
		case	4:	dst += 320 - width;
				break;
		}

	} while (--height);
}

#if !defined(__BORLANDC__)
}
#endif
