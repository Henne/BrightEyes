enum FB_VALUES {
	O_WIDTH = 320,
	O_HEIGHT = 200,
	MAX_RATIO = 10
};

void set_palette(const unsigned char*, const unsigned char, const unsigned short);

unsigned short swap_u16(const unsigned short);

void vgalib_fill_rect(unsigned char*, const int, const int, const int);
void vgalib_copy_to_screen(unsigned char*, unsigned char*, const int, const int);
void vgalib_copy_to_screen_bounded(unsigned char*, unsigned char*, const int, const int);
void vgalib_copy_to_screen_nonzero(unsigned char*, unsigned char*, const int, const int);
void vgalib_screen_copy(unsigned char*, unsigned char*, const int, const int);
void vgalib_copy_from_screen(unsigned char*, unsigned char*, const int, const int);

#if defined(__BORLANDC__)
void set_video_mode(unsigned short);
void set_video_page(unsigned short);
void save_display_stat(signed short*);
#define sdl_toggle_pal_logic()
#else
void sdl_init_video(void);
void sdl_exit_video(void);
void sdl_update_rect_window(const int, const int, const int, const int);
void sdl_forced_update(void);
SDL_Window* sdl_get_window(void);
int sdl_get_ratio(void);
void sdl_toggle_pal_logic(void);
void sdl_change_window_size(SDL_mutex*);
#endif
