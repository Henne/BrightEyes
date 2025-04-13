#include "port.h"

struct mouse_action {
	signed short x1;
	signed short y1;
	signed short x2;
	signed short y2;
	signed short action;
};

#if defined(__BORLANDC__)
/* BCC/DOS-specifig pointer arithmetics */
#define HUGEPTR huge
#else
#define HUGEPTR
#endif

struct nvf_desc {
	unsigned char HUGEPTR *dst;
	unsigned char HUGEPTR *src;
	signed short no;
	signed char type;
	signed short *width;
	signed short *height;
};

	signed short *get_timbre(signed short, signed short);
	unsigned short load_file(signed short);
	signed short load_driver(const char*, signed short, signed short);
	void play_midi(unsigned short);
	void stop_sequence(void);

	void mouse_do_enable(unsigned short, RealPt);
	void mouse_do_disable(void);
	void update_mouse_cursor1(void);
	void mouse(void);
	void handle_input(void);
	unsigned short get_mouse_action(signed short, signed short, struct mouse_action*);
	void draw_mouse_cursor(void);
	void save_mouse_bg(void);
	void restore_mouse_bg(void);
	void split_textbuffer(char**, char*, unsigned long);
	signed long process_nvf(struct nvf_desc*);
	signed short open_datfile(unsigned short);
	signed long get_archive_offset(const char*, unsigned char*);
	signed short read_datfile(signed short, unsigned char*, unsigned short);
	signed long get_filelength(signed short);
	void wait_for_keypress(void);
	void error_msg(const char*);
	void vsync_or_key(signed short);
	Bit32u swap_u32(Bit32u);

	void exit_video(void);
	void wait_for_vsync(void);
	unsigned short print_line(const char*);
	void print_str(const char *, signed short, signed short);
	signed short print_chr(unsigned char, signed short, signed short);
	signed short get_chr_info(unsigned char, signed short*);
	void call_them_all(signed short, signed short, signed short, signed short);
	void fill_smth(void);
	void fill_smth2(unsigned char*);
	RealPt get_gfx_ptr(signed short, signed short, signed short*);
	signed short ret_zero(signed short, signed short);
	void call_blit_smth3(RealPt, signed short, signed short, signed short, signed short);
	signed short get_line_start_c(const char*, signed short, signed short);
	signed short infobox(const char*, signed short);
	signed short gui_bool(char*);
	signed short gui_radio(char*, signed char, ...);

	void refresh_screen(void);
	void clear_hero(void);
	void new_values(void);
	void skill_inc_novice(signed short);
	void spell_inc_novice(signed short);
	void select_typus(void);
	signed short can_change_attribs(void);
	void change_attribs(void);

	void save_picbuf(void);
	void print_values(void);
	void make_valuta_str(char *, signed long);
	void select_skill(void);
	void select_spell(void);
	void choose_atpa(void);
	void choose_typus(void);
	void restore_timer_isr(void);
	void alloc_buffers(void);
	void init_colors(void);
	void init_stuff(void);
	RealPt gen_alloc(Bit32u);
