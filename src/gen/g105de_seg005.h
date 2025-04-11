void set_video_mode(Bit16u mode);
void set_video_page(Bit16u mode);
void save_display_stat(signed short*);
void set_color(signed char*, unsigned char);
void set_palette(signed char*, unsigned char, unsigned short);
void draw_h_line(Bit16u, Bit16s, Bit16u);
void draw_h_spaced_dots(Bit16u, Bit16u, Bit16s, Bit16u);
void pic_copy(unsigned char*, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, unsigned char*, Bit16u);
#if defined(__BORLANDC__)
void save_rect(Bit16u, Bit16u, unsigned char*, signed short, signed short);
void fill_rect(Bit16u, Bit16u, signed short, signed short, signed short);
#else
void save_rect(unsigned char*, unsigned char*, signed short, signed short);
void fill_rect(unsigned char*, signed short, signed short, signed short);
#endif
unsigned short swap_u16(unsigned short val);
void copy_to_screen(RealPt, RealPt, Bit16s, Bit16s, Bit16s);

RealPt normalize_ptr(RealPt);
