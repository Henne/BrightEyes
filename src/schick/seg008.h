#if !defined(__BORLANDC__)
namespace M302de {
#else
extern "C" {
#endif

unsigned short swap_u16(unsigned short);
void set_video_mode(signed short);
void set_video_page(signed short);
void save_display_stat(Bit8u*);
void set_color(Bit8u *ptr, unsigned char color);
void set_palette(Bit8u *ptr, unsigned short first_color, unsigned short colors);
void draw_h_line(Bit8u*, unsigned short, signed short);
void draw_h_spaced_dots(Bit8u*, signed short, signed short, signed short);

void pic_copy(Bit8u *dst, short x1, short y1, short x2, short y2,
        unsigned short val1, unsigned short val2,
        unsigned short val3, unsigned short val4,
        unsigned short width, unsigned short height,
        Bit8u *src, unsigned short mode);

void save_rect(Bit16u, Bit16u, Bit8u*, unsigned short, unsigned short);
void fill_rect(Bit16u, Bit16u, signed short, signed short, signed short);
void copy_solid_permuted(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid);
void copy_solid(Bit8u *dst, Bit8u *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid);
void decomp_rle(unsigned short width, unsigned short height,
	Bit8u *dst, Bit8u *src, Bit8u *tmp_buffer, unsigned short mode);

#if !defined(__BORLANDC__)
}
#else
}
#endif
