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

	// used by external
	void exit_video(void);
	signed short gui_radio(char*, signed char, ...);
	void restore_timer_isr(void);
