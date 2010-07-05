/*
 * ACE Loader
 *
 * Author: Henne_NWH <henne@nachtiwndheim.de>
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

/*		case 2: ??? */
		case 3: /* PP20 */
			data = malloc(w*h+172);
			if (!data) {
				fprintf(stderr, "Not enough Memory\n");
				return;
			}
			pal = buf + 0x1e + get_uint(buf+0x1e);
			printf("Pal = 0x%lx\n", pal - buf);
			printf("len - Pal = 0x%lx\n", buf + len - pal);
			printf("Deplen = %lu\n", depackedlen(buf+0x1e, get_uint(buf+0x1e)));
			ppdepack(buf+0x1e, (unsigned char*)data, get_uint(buf+0x1e), w*h);
			dump_tga("PIC01.TGA", w, h, data+0x172, col, pal+8);
			free(data);
			break;
		default:
			fprintf(stderr, "AIF mode %u not supported\n", get_ushort(buf+4));
	}
	return;
}
