void set_video_mode(Bit16u mode);
void set_video_page(Bit16u mode);
void save_display_stat(signed short*);
void set_color(signed char*, unsigned char);
void set_palette(signed char*, unsigned char, unsigned short);
void draw_h_line(Bit16u, Bit16s, Bit16u);
void draw_h_spaced_dots(Bit16u, Bit16u, Bit16s, Bit16u);
void pic_copy(RealPt, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, Bit16u, RealPt, Bit16u);
void save_rect(Bit16u, Bit16u, RealPt, Bit16s, Bit16s);
void fill_rect(Bit16u, Bit16u, Bit16s, Bit16s, Bit16s);
unsigned short swap_u16(unsigned short val);
void copy_to_screen(RealPt, RealPt, Bit16s, Bit16s, Bit16s);

RealPt normalize_ptr(RealPt);
