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

MyImage* read_tga(const char *buf, size_t len) {
    MyImage* img = (MyImage*)malloc(sizeof(MyImage));
    uint32_t offset = 0;

    img->x0     = (buf[9]  << 8)  |  buf[8];
    img->y0     = (buf[11] << 8)  |  buf[10];
    img->width  = (buf[13] << 8)  |  buf[12];
    img->height = (buf[15] << 8)  |  buf[14];
    offset += 18 + buf[0]; // Skip over 18 bytes header and image ID
    uint16_t pal_offset = ((buf[4]<<8) | buf[3]);
    
    if (buf[1] == 0) { // no color palette
	img->palette = NULL;
    } else { // read color palette
	// jump to palette offset
	offset += pal_offset;
	const char* pal_start = buf + offset;
	uint16_t pal_size = (buf[6]<<8) | buf[5];
	printf("reading palette of %d entries from offset %d\n", pal_size, offset);
	if (pal_size > 256) {
	    fprintf(stderr, "%s: palette size too big (%d)\n", __func__, pal_size);
	    return NULL;
	}
	img->palette = (Color*)malloc(pal_size * 3);
	for (int i=0;  i < pal_size;  i++) {
	    switch(buf[7]) {
	    case 24:
		img->palette[i].b = pal_start[i*3 + 0];
		img->palette[i].g = pal_start[i*3 + 1];
		img->palette[i].r = pal_start[i*3 + 2];
		break;
	    default:
		fprintf(stderr, "%s: TGA image has %d-bit palette; any2any only supports 24-bit palettes.\n", __func__, pal_size);
		return NULL;
	    }
	}
	offset += pal_size * (buf[7]/8);
    }

    // read pixels
    // theoretisch erlaubt sind 1,8,15,16,24 und 32 bpp. any2any unterstützt (vorerst) nur 8 bpp.
    if (buf[16] != 8) {
	fprintf(stderr, "%s: TGA image has %d bits per pixel; any2any only supports 8 bpp images.\n", __func__, buf[16]);
	return NULL;
    }
    uint32_t plen = img->width * img->height * sizeof(uint8_t);
    if (offset + plen > len) {
	fprintf(stderr, "%s: TGA image has only %d bytes, but %d bytes should be read.\n", __func__, len, offset + plen);
	return NULL;
    }

    img->pixels = (uint8_t*)malloc(plen);
    memcpy(img->pixels, buf + offset, plen);
    return img;
}

ImageSet* process_tga(const char *buf, size_t len) {
    MyImage* img = read_tga(buf, len);
    if (!img) {
	fprintf(stderr, "%s: error reading image.\n", __func__);
	return NULL;
    }
    ImageSet* set = (ImageSet*)malloc(sizeof(ImageSet));
    set->width      = img->width;
    set->height     = img->height;
    set->palette    = img->palette;
    set->mainPixels = img->pixels;
    set->seqCount   = 0;
    set->sequences  = NULL;
    /*
    set->palette    = NULL;
    set->mainPixels = NULL;
    set->seqCount   = 1;
    Sequence* seq = (Sequence*)malloc(1 * sizeof(Sequence));
    seq->name = "main";
    seq->frameCount = 0;
    seq->frames = NULL;
    seq->defaultDelay = 0;
    seq->imgCount = 1;
    seq->img = img;
    set->sequences  = seq;*/
    return set;
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
	tga_palette[i * 3 + 0] = palette[i].b;
	tga_palette[i * 3 + 1] = palette[i].g;
	tga_palette[i * 3 + 2] = palette[i].r;
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
	    sprintf(fname, "%s-S%02lu-I%03lu.tga", prefix, s, i);
	    frame = &seq->img[i];
	    if (frame->palette != NULL) palette = frame->palette;
	    else                        palette = img->palette;
	    
	    result = write_tga(fname, frame->width, frame->height, palette, frame->pixels);
	    if (result == 0) return 0;
	}
    }
    return 1;
}
