/*
 * Header file for file dumper
 * Author: Henne_NWH <henne@nachtwindheim.de>
 * License: GPLv3
 *
 */

void dump_tga(const char *,
	const unsigned short, const unsigned short,
	const char*, const unsigned short, const unsigned short, const char*);

typedef struct {
	unsigned short x0, y0;
	unsigned short width, height;
	unsigned short delay;
	unsigned char* pixels;
	char* comment;
} AnimFrame;

void dump_gif(const char *fname,
	      const unsigned short global_width, const unsigned short global_height,
	      const unsigned short framecount, AnimFrame* frames,
	      const unsigned short colors, const unsigned short first_color,
	      const char *pal);
