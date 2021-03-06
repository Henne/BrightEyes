/*
 * RAW loader
 *
 * Loads/Dumps a RAW image to/from an ImageSet structure.
 * RAW files are used by DSA/ROA 2+3, mostly for textures.
 *
 * Authors: Henne_NWH <henne@nachtwindheim.de>
 *          Hendrik <hermes9@web.de>
 * License: GPLv3
 *
 *
 */

/* File Format:
 * | Offset | Zweck                       |
 * |   0-25 | Bildinformation (Copyright) |
 * |  26-27 | 0x1A00 (???)                |
 * |  28-31 | ID-String ROH (0x524F4800)  |
 * |  32-33 | Bildbreite-1                |
 * |  34-35 | Bildhöhe-1                  |
 * |  36-37 | Anzahl der Paletteneinträge |
 * Ab Offset 38+3*Paletteneinträge beginnen die Bildrohdaten.
 * Ein Paletteneintrag umfasst 3 Bytes in der Reihenfolge RGB. 0x40 (64) ist Maximalwert.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <format.h>

struct raw_header {
    char     label[26];                    /* Bildinformation (Copyright)  */
    uint16_t version;                      /* 0x1A00 (Zweck unbekannt)     */
    char     magic_nr[4];                  /* ID-String ROH (0x524F4800)   */
    uint16_t width;                        /* Bildbreite-1                 */
    uint16_t height;                       /* Bildhöhe-1                   */
    uint16_t palette_size;                 /* Anzahl der Paletteneinträge  */
} __attribute__((__packed__));


int sanitycheck_raw(const char* buf, size_t len) {
    // TODO
    return 1;
}

ImageSet* process_raw(const char *buf, size_t len) {
    struct raw_header raw;
    uint32_t datalen = 0, i;
    char *data, *pal;
    ImageSet* img;

    if (!buf) {
	fprintf(stderr, "%s() got NULL Ptr\n", __func__);
	return NULL;
    }

    if (len < sizeof(struct raw_header)) {
	fprintf(stderr, "Buffer is too small for RAW header\n");
	return NULL;
    }

    strncpy(raw.label, buf, 26);
    raw.version = get_sint16(buf + 26);
    strncpy(raw.magic_nr, buf + 28, 4);
    raw.width  = get_sint16(buf + 32) + 1;
    raw.height = get_sint16(buf + 34) + 1;
    raw.palette_size = get_sint16(buf + 36);

    if (strncmp(raw.label, "(c) 1991 by Ulrich Walther", 26)) {
	fprintf(stderr, "Warning: Strange RAW Comment \"%26s\".\n", raw.label);
    }

    if (raw.version != 0x001A) {
	fprintf(stderr, "RAW Version is not 0x1A\n");
	return NULL;
    }

    if (strncmp(raw.magic_nr, "ROH\0", 4)) {
	fprintf(stderr, "Invalid RAW Signature\n");
	return NULL;
    }

    printf("Image Format: %ux%u with %d colours\n", raw.width, raw.height, raw.palette_size);

    datalen += sizeof(struct raw_header);

    /* Palette */
    pal = (char*)buf+datalen;
    datalen += 3*raw.palette_size;

    if (datalen+raw.width*raw.height != len) {
	fprintf(stderr, "Invalid file size: Should be %lu, is %lu \n", datalen+raw.width*raw.height, len);
	return NULL;
    }

    data = malloc(raw.width * raw.height);
    memcpy(data, buf+datalen, raw.width * raw.height);
    datalen += raw.width * raw.height;
    for (i=0; i<raw.width*raw.height;i++) {
	if (data[i] > raw.palette_size) printf("Farbüberlauf: %d\n", data[i]);
    }

    img = (ImageSet*)malloc(sizeof(ImageSet));
    img->width     = raw.width;
    img->height    = raw.height;
    img->seqCount  = 0;
    img->sequences = NULL;
    img->palette   = (Color*)pal;
    img->mainPixels = data;

    return img;
}

int dump_raw(ImageSet* img, char* prefix) {
    // TODO
/*
    AnimFrame* frame;
    char *buf, *buf_start;
    int i, j;
    for (i=0; i<img->frameCount; i++) {
	frame = img->frames[i];
	buf_start = buf = (char*)malloc(38 + 3*256 + frame->width * frame->height);
	strncpy(buf, "image made by any2any tool\n\0", 28);
	strncpy(buf, "ROH\0", 4);
	set_uint16(buf+32, frame->width - 1);
	set_uint16(buf+34, frame->height - 1);
	set_uint16(buf+36, 256);
	buf += 38;
	for (j=0; j<256; j++) {
	    buf[3*j+0] = img->globalPalette[j].r;
	    buf[3*j+1] = img->globalPalette[j].g;
	    buf[3*j+2] = img->globalPalette[j].b;
	}
	buf += 3*256;
	for (j=0; j<frame->width*frame->height; j++) {
	    buf[j] = frame->pixels[j];
	}
    }
    // TODO: Datei schreiben
*/
    return 0;
}
