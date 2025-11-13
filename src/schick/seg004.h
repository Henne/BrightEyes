#if !defined(__BORLANDC__)
namespace M302de {
#endif

void save_and_set_timer(void);
void set_timer(void);
void reset_timer(void);
void init_ani(uint16_t);
void disable_ani(void);
void init_ani_busy_loop(const signed int);
void clear_ani(void);
#if defined(__BORLANDC__)
void interrupt timer_isr(void);
#endif
void update_status_bars(void);
void draw_bar(const signed int, const signed int, const signed int, const signed int, const signed int);
void restore_rect(uint8_t*, uint8_t*, const signed int, const signed int, const signed char, const signed char);
void restore_rect_rle(uint8_t*, uint8_t*, const signed int, const signed int, const signed char, const signed char, const signed int);
void mouse_cursor_draw(void);
void mouse_save_bg(void);
void restore_mouse_bg(void);
void load_wallclock_nvf(void);
void update_wallclock(void);
void draw_wallclock(signed int, const signed int);
void array_add(uint8_t*, const signed int, const signed int, const signed int);
void schick_set_video(void);
void schick_reset_video(void);
void clear_ani_pal(void);
void set_ani_pal(uint8_t*);
void do_h_line(uint8_t*, signed int, signed int, const signed int, const signed char);
void do_v_line(uint8_t*, const signed int, signed int, signed int, const signed char);
void do_border(uint8_t*, const signed int, const signed int, const signed int, const signed int, const signed char);
void do_pic_copy(const signed int);
void do_save_rect(void);
void do_fill_rect(uint8_t*, const signed int, const signed int, const signed int, const signed int, const signed int);
void wait_for_vsync(void);
void map_effect(uint8_t*);

#if !defined(__BORLANDC__)
}
#endif
