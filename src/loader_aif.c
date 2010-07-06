/*
 * ACE Loader
 *
 * Author: Henne_NWH <henne@nachtwindheim.de>
 * License: GPLv3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <dump.h>
#include <format.h>

void process_aif(const char *buf, size_t len)
{
	unsigned long deplen,paclen;
	unsigned short h,w,col,mode;
	char *data,*pal;

	if (!buf) {
		fprintf(stderr, "%s() got NULL Ptr\n", __func__);
		return;
	}

	if (len < 0x1d) {
		fprintf(stderr, "Buffer is too small for AIF header\n");
		return;
	}

	if (strncmp(buf, "AIF\01", 4)) {
		fprintf(stderr, "AIF-Signature does not match\n");
		return;
	}

	mode = get_ushort(buf + 4);
	w = get_ushort(buf + 6);
	h = get_ushort(buf + 8);
	col = get_ushort(buf + 10);

	printf("Mode: %d\tWidth: %d\tHeight: %d\tColors: %d\n", mode ,w, h, col);

	switch (mode) {
		case 0: /* Unpacked */
			pal = buf + 0x1e + w * h;
			if ( h && w)
				dump_tga("PIC01.TGA", w, h, buf+0x1e, col, pal);
			break;

		case 2: /* RLE */
			pal = buf + len - col * 3;
			paclen = len - col * 3 - 0x1e;

			data = malloc(w * h);
			if (!data) {
				fprintf(stderr, "Not enough Memory\n");
				return;
			}

			un_rl(buf+0x1e, data, paclen);
			dump_tga("PIC01.TGA", w, h, data, col, pal);

			free(data);
			break;

		case 3: /* PP20 */
			paclen = get_uint(buf+0x1e) + 8;
			deplen = depackedlen(buf+0x1e, paclen);
			pal = buf + 0x1e + paclen;

			data = malloc(deplen);
			if (!data) {
				fprintf(stderr, "Not enough Memory\n");
				return;
			}

			if ( col * 3 != buf + len - pal) {
				printf("Pal = 0x%lx\n", pal - buf);
				printf("len - Pal = 0x%lx\n", buf + len - pal);
			}

			if (deplen != w * h) {
				printf("Deplen = %lu\n", deplen);
				printf("H*W = %d\n", h * w);
			}
			ppdepack(buf+0x1e, (unsigned char*)data, paclen, deplen);
			dump_tga("PIC01.TGA", w, h, data, col, pal);
			free(data);
			break;
		default:
			fprintf(stderr, "AIF mode %u not supported\n", mode);
	}
	return;
}
