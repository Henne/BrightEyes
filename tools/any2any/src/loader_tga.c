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
#include <format.h>

int sanitycheck_tga(const char* buf, size_t len) {
    // TODO
    return 1;
}

ImageSet* process_tga(const char *buf, size_t len) {
    // TODO
    return NULL;
}

int write_tga(char* fname, uint16_t width, uint16_t height, Color* palette, uint8_t* pixels) {
    FILE *fd = NULL;
    unsigned i;
    size_t result;
    char tga_palette[256 * 3];
    unsigned char tga_header[18];
    
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
    tga_header[12] = width & 0xff;
    tga_header[13] = (width >> 8) & 0xff;
    /* Height */
    tga_header[14] = height & 0xff;
    tga_header[15] = (height >> 8) & 0xff;
    /* bpp */
    tga_header[16] = 8;
    /* Attribute byte, pic starts top left */
    tga_header[17] = 32;

    /* Write Palette BGR */
    for (i=0;  i < 256;  i++) {
	tga_palette[i * 3 + 0] = palette[i].b << 2;
	tga_palette[i * 3 + 1] = palette[i].g << 2;
	tga_palette[i * 3 + 2] = palette[i].r << 2;
    }
    
    fd = fopen(fname, "wb+");

    if (!fd) {
	fprintf(stderr, "%s(%s ...) failed to open file\n",
		__func__, fname);
	return 0;
    }

    result = fwrite(tga_header, sizeof(char), 18, fd);
    if (result != 18) {
	fprintf(stderr, "%s(%s ...) failed to write tga_header\n",
		__func__, fname);
	fclose(fd);
	return 0;
    }

    result = fwrite(tga_palette, sizeof(char), 256 * 3, fd);
    if (result != 256 * 3) {
	fprintf(stderr, "%s(%s ...) failed to write tga_palette\n",
		__func__, fname);
	fclose(fd);
	return 0;
    }

    result = fwrite(pixels, sizeof(char), width*height, fd);
    if (result != width*height) {
	fprintf(stderr, "%s(%s ...) failed to write data\n",
		__func__, fname);
	fclose(fd);
	return 0;
    }
    
    /* Close File */
    fclose(fd);
    return 1;
}

int dump_tga(ImageSet* img, char* prefix) {
    uint16_t i, j;
    size_t result;
    char fname[100];


    if (img->mainPixels != NULL) {
	printf("main image: %dx%d\n", img->width, img->height);
	sprintf(fname, "%s.tga", prefix);
	result = write_tga(fname, img->width, img->height, img->palette, img->mainPixels);
	if (result == 0) return 0;
    }


    for (int s=0;  s < img->seqCount;  s++) {
	Sequence* seq = &img->sequences[s];
	// TODO: frames berücksichtien
	if (img->mainPixels != NULL) {
	    
	}
	for (i=0;  i < seq->imgCount;  i++) {
	    MyImage* frame;
	    Color* palette;
	    sprintf(fname, "%s-S%02luI%03lu.tga", prefix, s, i);
	    frame = &seq->img[i];
	    if (frame->palette != NULL) palette = frame->palette;
	    else                        palette = img->palette;
	    
	    result = write_tga(fname, frame->width, frame->height, palette, frame->pixels);
	    if (result == 0) return 0;
	}
    }
    return 1;
}
