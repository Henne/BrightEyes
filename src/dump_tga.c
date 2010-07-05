/*
 * Dumps Picture to TGA
 *
 * Author: Henne_NWH <henne@nachtiwndheim.de>
 * License: GPLv3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dump_tga(char *fname, unsigned short x, unsigned short y, const char *data,
	      unsigned short colors, const char *pal)
{
	FILE *fd = NULL;
	unsigned long i;

	if (!fname)
		return;

	fd = fopen(fname, "wb");
	/* name and open file */

	if (!fd)
		return;

	/* No picture ID */
	fputc(0, fd);
	/* Having a color palette */
	fputc(1, fd);
	/* indiceed unpacked color palette */
	fputc(1, fd);
	/* first color (WORD) */
	fputc(0, fd);
	fputc(0, fd);
	/* colors in palette (WORD) */
	fputc(colors & 0xff, fd);
	fputc((colors >> 8) & 0xff, fd);
	/* bits per pixel palette */
	fputc(24, fd);
	/* X-Coordiante (WORD) */
	fputc(0, fd);
	fputc(0, fd);
	/* Y-Coordiante (WORD) */
	fputc(0, fd);
	fputc(0, fd);
	/* Width */
	fputc(x & 0xff, fd);
	fputc((x >> 8) & 0xff, fd);
	/* Height */
	fputc(y & 0xff, fd);
	fputc((y >> 8) & 0xff, fd);
	/* bpp */
	fputc(8, fd);
	/* Attribute byte, pic starts top left */
	fputc(32, fd);

	/* Write Palette BGR */
	for (i = 0; i < colors; i++) {
		fputc(pal[i * 3 + 2] << 2, fd);
		fputc(pal[i * 3 + 1] << 2, fd);
		fputc(pal[i * 3] << 2, fd);
	}

	/* Write Data */
	fwrite(data, sizeof(char), x * y, fd);

	/* Close File */
	fclose(fd);
}
