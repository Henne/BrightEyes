/*
 * AIF loader
 *
 * Loads/Dumps an AIF picture to/from an ImageSet structure.
 * AIF files are used by DSA/ROA 2+3.
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

int sanitycheck_aif(const char *buf, size_t len) {
    if (len < 0x1d) {
	fprintf(stderr, "Buffer is too small for AIF header\n");
	return 0;
    }
    if (strncmp(buf, "AIF\01", 4)) {
	fprintf(stderr, "AIF-Signature does not match\n");
	return 0;
    }
    return 1;
}

ImageSet* process_aif(const char *buf, size_t len)
{
    uint32_t deplen,paclen;
    uint16_t h,w,col,mode;
    char *data,*pal;
    ImageSet* img;

    if (!buf) {
	fprintf(stderr, "%s() got NULL Ptr\n", __func__);
	return NULL;
    }

    if (len < 0x1d) {
	fprintf(stderr, "Buffer is too small for AIF header\n");
	return NULL;
    }

    if (strncmp(buf, "AIF\01", 4)) {
	fprintf(stderr, "AIF-Signature does not match\n");
	return NULL;
    }

    mode = get_uint16(buf + 4);
    w = get_uint16(buf + 6);
    h = get_uint16(buf + 8);
    col = get_uint16(buf + 10);
    printf("Mode: %d\tWidth: %d\tHeight: %d\tColors: %d\n", mode ,w, h, col);

    img = (ImageSet*)malloc(sizeof(ImageSet));
    img->width    = w;
    img->height   = h;
    img->seqCount = 0;
    img->sequences= NULL;
    
    switch (mode) {
    case 0: /* Unpacked */
	pal = (char *)(buf + 0x1e + w * h);
	
	if (h && w) {
	    img->mainPixels = (char*)buf+0x1E;
	}
	break;
	    
    case 2: /* RLE */
	pal = (char *)(buf + len - col * 3);
	paclen = len - col * 3 - 0x1e;
	    
	img->mainPixels = malloc(w * h);
	if (img->mainPixels == NULL) {
	    fprintf(stderr, "Not enough Memory\n");
	    return NULL;
	}
	un_rl(buf+0x1e, img->mainPixels, paclen);
	break;
	    
    case 3: /* PP20 */
	paclen = get_uint32(buf+0x1e) + 8;
	deplen = w * h;
	pal = (char *)(buf + 0x1e + paclen);
	    
	img->mainPixels = malloc(deplen);
	if (img->mainPixels == NULL) {
	    fprintf(stderr, "Not enough Memory\n");
	    return NULL;
	}
	    
	if (col * 3 != buf + len - pal) {
	    printf("Pal = 0x%lx\n", pal - buf);
	    printf("len - Pal = 0x%lx\n", buf + len - pal);
	}
	    
	ppdepack(buf+0x1e, img->mainPixels, paclen, deplen);
	break;
    default:
	fprintf(stderr, "AIF mode %u not supported\n", mode);
	exit(1);
    }
    img->palette = (Color*)pal;
    return img;
}

int dump_aif(ImageSet* img, char* prefix) {
    // TODO
    return 1;
}
