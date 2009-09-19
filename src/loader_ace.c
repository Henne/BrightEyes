/*
 * NVF to TGA Converter
 *
 * Converts a NVF file to the TGA format.
 * NVF files are used by DSA/ROA 2+3
 * Some files of DSA/ROA 1 work, too, but aren't supported.
 *
 * Author: Henne_NWH <henne@nachtiwndheim.de>
 * License: GPLv3
 *
 * Compilation: gcc -o nvf2tga nvf2tga.c
 * Usage:	./nvf2tga <*.TGA>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <dump.h>
#include <format.h>

void process_ace(const char *buf, size_t len) {
	struct ace_header ace;

	if (!buf) {
		fprintf(stderr, "%s() got NULL Ptr\n", __func__);
		return;
	}

	if (len < 8) {
		fprintf(stderr, "Buffer is too small for ACE header\n");
		return;
	}

	strncpy(ace.label, buf, 4);
	ace.version=get_uint(buf+4);
	ace.sequences=buf[6];
	ace.speed=buf[7];

	if (strncmp(ace.label, "ACE\0", 4)) {
		fprintf(stderr, "No ACE Signature\n");
		return;
	}

	if (ace.version != 1) {
		fprintf(stderr, "ACE Version is not 1\n");
		return;
	}

	if (ace.sequences > 250) {
		fprintf(stderr, "ACE sequences %u is not a valid value\n",
				ace.sequences);
		return;
	}
	if (ace.speed > 99) {
		fprintf(stderr, "ACE speed %u is not a valid value\n",
				ace.speed);

	}

	printf("Sequences: %03u\tSpeed: %02u\n", ace.sequences, ace.speed);
	return;
}
