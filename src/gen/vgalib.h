void set_video_mode(unsigned short);
void set_video_page(unsigned short);
void save_display_stat(signed short*);
void set_color(const unsigned char*, const unsigned char);
void set_palette(signed char*, unsigned char, unsigned short);
void draw_h_line(unsigned short, signed short, unsigned short);
void draw_h_spaced_dots(unsigned short, unsigned short, signed short, unsigned short);
void pic_copy(unsigned char*, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned char*, unsigned short);
#if defined(__BORLANDC__)
void save_rect(unsigned short, unsigned short, unsigned char*, signed short, signed short);
void fill_rect(unsigned short, unsigned short, signed short, signed short, signed short);
#else
void save_rect(unsigned char*, unsigned char*, signed short, signed short);
void fill_rect(unsigned char*, signed short, signed short, signed short);
#endif
unsigned short swap_u16(unsigned short);
void copy_to_screen(unsigned char*, unsigned char*, signed short, signed short, signed short);

unsigned char* normalize_ptr(unsigned char*);
