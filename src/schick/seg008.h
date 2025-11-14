#if !defined(__BORLANDC__)
namespace M302de {
#else
extern "C" {
#endif

unsigned short swap_u16(unsigned short);
void set_video_mode(int16_t);
void set_video_page(int16_t);
void save_display_stat(int16_t*);
void set_color(uint8_t *ptr, unsigned char color);
void set_palette(uint8_t *ptr, unsigned short first_color, unsigned short colors);
void draw_h_line(uint8_t*, unsigned short, signed short);
void draw_h_spaced_dots(uint8_t*, signed short, signed short, signed short);

void pic_copy(uint8_t *dst, short x1, short y1, short x2, short y2,
        unsigned short val1, unsigned short val2,
        unsigned short val3, unsigned short val4,
        unsigned short width, unsigned short height,
        uint8_t *src, unsigned short mode);

#if defined(__BORLANDC__)
void save_rect(uint16_t, uint16_t, uint8_t*, unsigned short, unsigned short);
void fill_rect(uint16_t, uint16_t, signed short, signed short, signed short);
#else
void save_rect(uint8_t*, uint8_t*, unsigned short, unsigned short);
void fill_rect(uint8_t*, signed short, signed short, signed short);
#endif
void copy_solid_permuted(uint8_t *dst, uint8_t *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid);
void copy_solid(uint8_t *dst, uint8_t *src, unsigned short width_to_copy,
	unsigned short height, unsigned short dst_width,
	unsigned short src_width, unsigned short solid);
void decomp_rle(unsigned short width, unsigned short height,
	uint8_t *dst, uint8_t *src, uint8_t *tmp_buffer, unsigned short mode);

#if !defined(__BORLANDC__)
}
#else
}
#endif
