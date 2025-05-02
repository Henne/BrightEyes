void set_video_mode(unsigned short);
void set_video_page(unsigned short);
void save_display_stat(signed short*);
void set_palette(const unsigned char*, const unsigned char, const unsigned short);

void pic_copy(unsigned char*, const signed short, const signed short, const signed short, const signed short, const signed short, const signed short, unsigned char*, const signed short);

void fill_rect(unsigned char*, const signed short, const signed short, const signed short);
const unsigned short swap_u16(const unsigned short);
void copy_to_screen(unsigned char*, unsigned char*, const signed short, const signed short, const signed short);

#if defined(__BORLANDC__)
unsigned char* normalize_ptr(unsigned char*);
#endif

#if !defined(__BORLANDC__)
void update_sdl_window(void);
void sdl_change_window_size(void);
#endif
