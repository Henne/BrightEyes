#if !defined(__BORLANDC__)
namespace M302de {
#else
extern "C" {
#endif

uint16_t swap_u16(const uint16_t);
void set_video_mode(const int16_t);
void set_video_page(const int16_t);
void save_display_stat(int16_t*);
void set_color(uint8_t*, const uint8_t);
void set_palette(uint8_t*, const signed int, const signed int);
void draw_h_line(uint8_t*, signed int, const signed int);
void draw_h_spaced_dots(uint8_t*, const signed int, const signed int, const signed int);

void pic_copy(uint8_t*, const signed int, const signed int, const signed int, const signed int,
        const signed int, const signed int, const signed int, const signed int,
        const signed int, const signed int, uint8_t*, const signed int);

#if defined(__BORLANDC__)
void save_rect(uint16_t, uint16_t, uint8_t*, const signed int, signed int);
void fill_rect(uint16_t, uint16_t, signed short, signed short, signed short);
#else
void save_rect(uint8_t*, uint8_t*, const signed int, signed int);
void fill_rect(uint8_t*, const signed int, const signed int, signed int);
#endif

void copy_solid_permuted(uint8_t*, uint8_t*, const signed int,
	const signed int, const signed int, const signed int, const signed int);

void copy_solid(uint8_t *dst, uint8_t *src, const signed int width_to_copy,
	const signed int height, const signed int dst_width,
	const signed int src_width, const signed int solid);

void decomp_rle(const signed int, signed int, uint8_t*, uint8_t*, uint8_t*, const signed int);

#if !defined(__BORLANDC__)
}
#else
}
#endif
