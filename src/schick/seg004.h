#if !defined(__BORLANDC__)
namespace M302de {
#endif

void save_and_set_timer(void);
void set_timer(void);
void reset_timer(void);
void init_ani(uint16_t);
void disable_ani(void);
void init_ani_busy_loop(unsigned short);
void clear_ani(void);
#if defined(__BORLANDC__)
void interrupt timer_isr(void);
#endif
void update_status_bars(void);
void draw_bar(unsigned short, signed short, signed short, signed short, signed short);
void restore_rect(uint8_t*, uint8_t*, unsigned short, unsigned short, signed char, signed char);
void restore_rect_rle(uint8_t*, uint8_t*, unsigned short, unsigned short, signed char, signed char, unsigned short);
void mouse_cursor_draw(void);
void mouse_save_bg(void);
void restore_mouse_bg(void);
void load_wallclock_nvf(void);
void update_wallclock(void);
void draw_wallclock(signed short, signed short);
void array_add(uint8_t*, signed short, signed short, signed short);
void schick_set_video(void);
void schick_reset_video(void);
void clear_ani_pal(void);
void set_ani_pal(uint8_t*);
void do_h_line(uint8_t*, signed short, signed short, signed short, signed char);
void do_v_line(uint8_t*, signed short, signed short, signed short, signed char);
void do_border(uint8_t*, signed short, signed short, signed short, signed short, signed char);
void do_pic_copy(unsigned short mode);
void do_save_rect(void);
void do_fill_rect(uint8_t*, signed short, signed short, signed short, signed short, signed short);
void wait_for_vsync(void);
void map_effect(uint8_t*);

#if !defined(__BORLANDC__)
}
#endif
