void set_video_mode(unsigned short);
void set_video_page(unsigned short);
void save_display_stat(signed short*);
void set_palette(const unsigned char*, const unsigned char, const unsigned short);
void pic_copy(unsigned char*, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned char*, unsigned short);
void fill_rect(unsigned char*, const signed short, const signed short, const signed short);
unsigned short swap_u16(unsigned short);
void copy_to_screen(unsigned char*, unsigned char*, const signed short, const signed short, const signed short);

unsigned char* normalize_ptr(unsigned char*);

#if !defined(__BORLANDC__)
void update_sdl_window(void);
int sdl_event_loop(const int);
#endif
