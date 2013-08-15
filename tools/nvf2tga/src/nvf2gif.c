/*
 * NVF to TGA Converter
 *
 * Converts a NVF file to the TGA format.
 * NVF files are used by DSA/ROA 2+3
 * Some files of DSA/ROA 1 work, too, but aren't supported.
 *
 * Author: Henne_NWH <henne@nachtwindheim.de>
 * License: GPLv3
 *
 * Compilation: gcc -o nvf2tga nvf2tga.c
 * Usage:	./nvf2tga <*.TGA>
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include <loader.h>
#include <version.h>

int main(int argc, char **argv)
{

	FILE *fd = NULL;
	char *buf;
	int32_t flen, readlen;

	if (argc == 1) {
		printf("%s %s\n", BANNER, VERSION);
		printf("Usage: %s <NVF-File>\n", argv[0]);
		exit(1);
	}

	fd = fopen(argv[1], "rb");
	if (!fd) {
		printf("Cant open file %s\n", argv[1]);
		exit(1);
	}

	fseek(fd, 0, SEEK_END);
	flen = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	/* flen == -1 happens when opening a directory with fopen() under Linux. */
	if (flen < 3) {
		printf("File %s is to small.\n", argv[1]);
		fclose(fd);
		exit(1);
	}

	buf = calloc(flen, sizeof(char));
	if (!buf) {
		printf("Not enough memory\n");
		fclose(fd);
		exit(1);
	}

	readlen = fread(buf, sizeof(char), flen, fd);
	fclose(fd);
	if (readlen != flen) {
		printf("Could not read the whole file.\n");
		free(buf);
		exit(1);
	}

	/* Everythings fine here, process the data */
	process_nvf(buf, flen);

	free(buf);
	exit(0);
}
