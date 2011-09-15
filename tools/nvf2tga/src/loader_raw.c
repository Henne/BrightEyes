/*
 * RAW Loader
 *
 * Author: Hendrik Radke <hermes9@gmx.net>
 * License: GPLv3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <dump.h>
#include <format.h>


void process_raw(const char *buf, size_t len)
{
	struct raw_header raw;
	unsigned long datalen = 0, i;
	char *data, *pal;
	
	if (!buf) {
		fprintf(stderr, "%s() got NULL Ptr\n", __func__);
		return;
	}

	if (len < sizeof(struct raw_header)) {
		fprintf(stderr, "Buffer is too small for RAW header\n");
		return;
	}

	strncpy(raw.label, buf, 26);
	raw.version = get_sshort(buf + 26);
	strncpy(raw.magic_nr, buf + 28, 4);
	raw.width  = get_sshort(buf + 32) + 1;
	raw.height = get_sshort(buf + 34) + 1;
	raw.palette_size = get_sshort(buf + 36);

	if (strncmp(raw.label, "(c) 1991 by Ulrich Walther", 26)) {
		fprintf(stderr, "Warning: Strange RAW Comment \"%26s\".\n", raw.label);
	}

	if (raw.version != 0x001A) {
		fprintf(stderr, "RAW Version is not 0x1A\n");
		return;
	}

	if (strncmp(raw.magic_nr, "ROH\0", 4)) {
		fprintf(stderr, "Invalid RAW Signature\n");
		return;
	}

	printf("Image Format: %ux%u with %d colours\n", raw.width, raw.height, raw.palette_size);

	datalen += sizeof(struct raw_header);

	/* Palette */
	pal = (char*)buf+datalen;
	datalen += 3*raw.palette_size;

	if (datalen+raw.width*raw.height != len) {
		fprintf(stderr, "Invalid file size: Should be %lu, is %lu \n", datalen+raw.width*raw.height, len);
		return;
	}

	data = malloc(raw.width * raw.height);
	memcpy(data, buf+datalen, raw.width * raw.height);
	datalen += raw.width * raw.height;
	for (i=0; i<raw.width*raw.height;i++) {
		if (data[i] > raw.palette_size) printf("Farbüberlauf: %d\n", data[i]);
	}
	dump_tga("raw.tga", raw.width, raw.height, data, raw.palette_size, 0, pal);
	free(data);

	return;
}
