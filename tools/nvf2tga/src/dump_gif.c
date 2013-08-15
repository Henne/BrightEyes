/*
 * Dumps Picture to GIF
 *
 * Author: Hendrik Radke <hermes9@gmx.net>, Henne_NWH <henne@nachtwindheim.de>
 * License: GPLv3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
#include <dump.h>

#define true 1
#define false 0

void dump_gif(const char *fname,
	      const unsigned short global_width, const unsigned short global_height,
	      const unsigned short framecount, AnimFrame* frames,
	      const unsigned short colors, const unsigned short first_color,
	      const char *pal)
{
	FILE *fd = NULL;
	gdImagePtr imgBack, imgAnim = NULL;
	size_t result;
	int size;
	
	if (!fname)
		return;
	
	if (colors > 256) {
		printf("%s(): colors > 256\n", __func__);
		return;
	}
	
	/* open file */
	fd = fopen(fname, "wb+");
	
	if (!fd) {
		fprintf(stderr, "%s(%s ...) failed to open file\n",
			__func__, fname);
		return;
	}
	
	imgBack = gdImageCreate(global_width, global_height);
	
	// Palette initialisieren
	int i;
	for (i = 0; i < colors; i++) {
	    gdImageColorAllocate(imgBack, pal[(i-first_color)*3+0]<<2, pal[(i-first_color)*3+1]<<2, pal[(i-first_color)*3+2]<<2);
	}

	printf("new gif: %dx%d\n", global_width, global_height);
	gdImageGifAnimBegin(imgBack, fd, true, -1);
	//gdFree(img);
	// Globale Palette?
	// Frames schreiben.
	AnimFrame frame;
	for (i = 0; i < framecount; i++) {
	    frame = frames[i];
	    imgAnim = gdImageCreate(frame.width, frame.height);

	    gdImagePaletteCopy(imgAnim, imgBack);
	    int j;
	    // TODO: x0, y0 != 0 berücksichtigen
	    // TODO: geht das nicht auch im Block?
	    for (j = 0; j < frame.width * frame.height; j++) gdImageSetPixel(imgAnim, j%frame.width, j/frame.width, frame.pixels[j]);
	    //printf("create frame: %dx%d@%dx%d, delay %d, pixels@ %p, %d colors\n", frame.width, frame.height, frame.x0, frame.y0, frame.delay, frame.pixels, imgAnim->colorsTotal);
	    gdImageGifAnimAdd(imgAnim, fd, true, frame.x0, frame.y0, frame.delay, gdDisposalNone, NULL);
	    gdFree(imgAnim);
	}
	
	// Close File
	gdImageGifAnimEnd(fd);
	fclose(fd);
	gdFree(imgBack);
}
