enum FB_VALUES {
	O_WIDTH = 320,
	O_HEIGHT = 200,
	MAX_RATIO = 10
};

void set_video_mode(unsigned short);
void set_video_page(unsigned short);
void save_display_stat(signed short*);
void set_palette(const unsigned char*, const unsigned char, const unsigned short);

void pic_copy(unsigned char*, const signed short, const signed short, const signed short, const signed short, const signed short, const signed short, unsigned char*, const signed short);

void fill_rect(unsigned char*, const signed short, const signed short, const signed short);
const unsigned short swap_u16(const unsigned short);

void vgalib_copy_to_screen(unsigned char*, unsigned char*, const int, const int);

void vgalib_copy_from_screen(unsigned char*, unsigned char*, const int, const int);

#if !defined(__BORLANDC__)
void sdl_update_rect_window(const int, const int, const int, const int);
SDL_Window* sdl_get_window(void);
int sdl_get_ratio(void);
void sdl_change_window_size(SDL_mutex*);
#endif
