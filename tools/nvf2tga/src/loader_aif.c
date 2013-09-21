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
	// TODO
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
	img->globalWidth  = w;
	img->globalHeight = h;
	img->frameCount   = 1;
	img->frames       = (AnimFrame**)malloc(img->frameCount * sizeof(AnimFrame*));
	for (int i = 0; i<img->frameCount; i++) img->frames[i] = (AnimFrame*)malloc(sizeof(AnimFrame));
	
	AnimFrame* frame = img->frames[0];
	switch (mode) {
		case 0: /* Unpacked */
			pal = (char *)(buf + 0x1e + w * h);

			if (h && w) {
				frame->x0 = frame->y0 = 0;
				frame->width  = img->globalWidth;
				frame->height = img->globalHeight;
				frame->delay  = 0;
				frame->localPalette = 0;
				frame->pixels = (char*)buf+0x1E;
				frame->comment= "";
			}
			break;

		case 2: /* RLE */
			pal = (char *)(buf + len - col * 3);
			paclen = len - col * 3 - 0x1e;

			data = malloc(w * h);
			if (!data) {
				fprintf(stderr, "Not enough Memory\n");
				return NULL;
			}

			un_rl(buf+0x1e, data, paclen);
			frame->x0 = frame->y0 = 0;
			frame->width  = img->globalWidth;
			frame->height = img->globalHeight;
			frame->delay  = 0;
			frame->localPalette = 0;
			frame->pixels = data;
			frame->comment= "";
			break;

		case 3: /* PP20 */
			paclen = get_uint32(buf+0x1e) + 8;
			deplen = w * h;
			pal = (char *)(buf + 0x1e + paclen);

			data = malloc(deplen);
			if (data == NULL) {
				fprintf(stderr, "Not enough Memory\n");
				return NULL;
			}

			if (col * 3 != buf + len - pal) {
				printf("Pal = 0x%lx\n", pal - buf);
				printf("len - Pal = 0x%lx\n", buf + len - pal);
			}

			ppdepack(buf+0x1e, data, paclen, deplen);
			frame->x0 = frame->y0 = 0;
			frame->width  = img->globalWidth;
			frame->height = img->globalHeight;
			frame->delay  = 0;
			frame->localPalette = 0;
			frame->pixels = data;
			frame->comment= "";
			break;
		default:
			fprintf(stderr, "AIF mode %u not supported\n", mode);
			exit(1);
	}
	img->globalPalette = (Color*)pal;
	return img;
}

int dump_aif(ImageSet* img, char* prefix) {
	// TODO
	return 1;
}
