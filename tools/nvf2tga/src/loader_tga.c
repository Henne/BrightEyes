/*
 * TGA loader
 *
 * Loads/Dumps a TGA file to/from an ImageSet structure.
 *
 * Authors: Henne_NWH <henne@nachtwindheim.de>
 *          Hendrik <hermes9@web.de>
 * License: GPLv3
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <loader.h>
#include <format.h>

int sanitycheck_tga(const char* buf, size_t len) {
	// TODO
	return 1;
}

ImageSet* process_tga(const char *buf, size_t len) {
	// TODO
	return NULL;
}

int dump_tga(ImageSet* img, char* prefix) {
	FILE *fd = NULL;
	unsigned long i, j;
	size_t result;
	char tga_palette[256 * 3];
	unsigned char tga_header[18];
	const unsigned short colors = 256;
	const unsigned short first_color = 0;

	for (i=0;  i < img->frameCount;  i++) {
	    char fname[100];
	    sprintf(fname, "%s%03lu.TGA", prefix, i);
	    
	    AnimFrame* frame = img->frames[i];
	    /* prepare TGA header */

	    /* No picture ID */
	    tga_header[0] = 0;
	    /* Having a color palette */
	    tga_header[1] = 1;
	    /* indiceed unpacked color palette */
	    tga_header[2] = 1;
	    /* first color (WORD) */
	    tga_header[3] = 0;
	    tga_header[4] = 0;
	    /* colors in palette (WORD) */
	    tga_header[5] = 0;
	    tga_header[6] = 1;
	    /* bits per pixel palette */
	    tga_header[7] = 24;
	    /* X-Coordiante (WORD) */
	    tga_header[8] = 0;
	    tga_header[9] = 0;
	    /* Y-Coordiante (WORD) */
	    tga_header[10] = 0;
	    tga_header[11] = 0;
	    /* Width */
	    tga_header[12] = frame->width & 0xff;
	    tga_header[13] = (frame->width >> 8) & 0xff;
	    /* Height */
	    tga_header[14] = frame->height & 0xff;
	    tga_header[15] = (frame->height >> 8) & 0xff;
	    /* bpp */
	    tga_header[16] = 8;
	    /* Attribute byte, pic starts top left */
	    tga_header[17] = 32;


	    /* prepare TGA palette */

	    memset(tga_palette, 0, 256 * 3);

	    /* Write Palette BGR */
	    for (j = first_color;  j < colors;  j++) {
		tga_palette[j * 3 + 0] = img->globalPalette[(j - first_color) * 3 + 2 ] << 2;
		tga_palette[j * 3 + 1] = img->globalPalette[(j - first_color) * 3 + 1 ] << 2;
		tga_palette[j * 3 + 2] = img->globalPalette[(j - first_color) * 3] << 2;
	    }


	    /* open file */
	    fd = fopen(fname, "wb+");

	    if (!fd) {
		fprintf(stderr, "%s(%s ...) failed to open file\n",
			__func__, fname);
		return 0;
	    }

	    /* Write Header */
	    result = fwrite(tga_header, sizeof(char), 18, fd);

	    if (result != 18) {
		fprintf(stderr, "%s(%s ...) failed to write tga_header\n",
			__func__, fname);
		fclose(fd);
		return 0;
	    }

	    /* Write Palette */
	    result = fwrite(tga_palette, sizeof(char), 256 * 3, fd);

	    if (result != 256 * 3) {
		fprintf(stderr, "%s(%s ...) failed to write tga_palette\n",
			__func__, fname);
		fclose(fd);
		return 0;
	    }

	    /* Write Data */
	    result = fwrite(frame->pixels, sizeof(char), frame->width * frame->height, fd);

	    if (result != frame->width * frame->height) {
		fprintf(stderr, "%s(%s ...) failed to write data\n",
			__func__, fname);
		fclose(fd);
		return 0;
	    }


	    /* Close File */
	    fclose(fd);
	}
	return 1;
}
