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
#define bc_F_PADD(p, v) ((unsigned char huge*)(p) + (signed long)(v))
#else
#define HUGEPTR
static inline unsigned char* bc_F_PADD(unsigned char *p, signed long v) { return p + v; }
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
	unsigned short load_file(Bit16s);
	signed short load_driver(const char*, signed short, signed short);
	void play_midi(Bit16u);
	void stop_sequence(void);

	void mouse_do_enable(Bit16u, RealPt);
	void mouse_do_disable(void);
	void update_mouse_cursor1(void);
	void mouse(void);
	void handle_input(void);
	Bit16u get_mouse_action(Bit16s, Bit16s, struct mouse_action*);
	void draw_mouse_cursor(void);
	void save_mouse_bg(void);
	void restore_mouse_bg(void);
	void split_textbuffer(char**, char*, unsigned long);
	signed long process_nvf(struct nvf_desc*);
	Bit16s open_datfile(Bit16u);
	Bit32s get_archive_offset(const char*, Bit8u*);
	Bit16s read_datfile(Bit16u, Bit8u*, Bit16u);
	Bit32s get_filelength(Bit16s);
	void wait_for_keypress(void);
	void error_msg(const char*);
	void vsync_or_key(Bit16s);
	Bit32u swap_u32(Bit32u);

	void exit_video(void);
	void wait_for_vsync(void);
	unsigned short print_line(const char*);
	void print_str(const char *, Bit16s, Bit16s);
	Bit16s print_chr(unsigned char, Bit16s, Bit16s);
	Bit16s get_chr_info(unsigned char, Bit16s*);
	void call_them_all(Bit16s, Bit16s, Bit16s, Bit16s);
	void fill_smth(void);
	void fill_smth2(Bit8u*);
	RealPt get_gfx_ptr(Bit16s, Bit16s, Bit16s*);
	Bit16s ret_zero(Bit16s, Bit16s);
	void call_blit_smth3(RealPt, Bit16s, Bit16s, Bit16s, Bit16s);
	Bit16s get_line_start_c(const char*, Bit16s, Bit16s);
	signed short infobox(const char*, signed short);
	signed short gui_bool(char*);
	signed short gui_radio(char*, signed char, ...);

	void refresh_screen(void);
	void clear_hero(void);
	void new_values(void);
	void skill_inc_novice(Bit16s);
	void spell_inc_novice(Bit16s);
	void select_typus(void);
	Bit16s can_change_attribs(void);
	void change_attribs(void);

	void save_picbuf(void);
	void print_values(void);
	void make_valuta_str(char *, Bit32s);
	void select_skill(void);
	void select_spell(void);
	void choose_atpa(void);
	void choose_typus(void);
	void restore_timer_isr(void);
	void alloc_buffers(void);
	void init_colors(void);
	void init_stuff(void);
	RealPt gen_alloc(Bit32u);
