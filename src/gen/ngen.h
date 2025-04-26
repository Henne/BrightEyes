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

	signed short open_datfile(unsigned short);
	signed long get_archive_offset(const char*, unsigned char*);
	signed short read_datfile(signed short, unsigned char*, unsigned short);
	signed long get_filelength(void);

#if defined(__BORLANDC__)
	unsigned long swap_u32(unsigned long);
#else
	unsigned int swap_u32(unsigned int);
#endif

	void exit_video(void);
	signed short infobox(const char*, signed short);
	signed short gui_bool(char*);
	signed short gui_radio(char*, signed char, ...);

	void restore_timer_isr(void);
	void init_colors(void);
	void init_stuff(void);
