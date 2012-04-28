/*
 * Dumps Picture to TGA
 *
 * Author: Henne_NWH <henne@nachtwindheim.de>
 * License: GPLv3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dump_tga(const char *fname,
		const unsigned short x, const unsigned short y,
		const char *data,
		const unsigned short colors, const unsigned short first_color,
		const char *pal)
{
	FILE *fd = NULL;
	unsigned long i;
	size_t result;
	char tga_palette[256 * 3];
	unsigned char tga_header[18];

	if (!fname)
		return;

	if (colors > 256) {
		printf("%s(): colors > 256\n", __func__);
		return;
	}


	/* prepare TGA header */

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
	tga_header[12] = x & 0xff;
	tga_header[13] = (x >> 8) & 0xff;
	/* Height */
	tga_header[14] = y & 0xff;
	tga_header[15] = (y >> 8) & 0xff;
	/* bpp */
	tga_header[16] = 8;
	/* Attribute byte, pic starts top left */
	tga_header[17] = 32;


	/* prepare TGA palette */

	memset(tga_palette, 0, 256 * 3);

	/* Write Palette BGR */
	for (i = first_color; i < colors; i++) {
		tga_palette[i * 3 + 0] = pal[(i - first_color) * 3 + 2 ] << 2;
		tga_palette[i * 3 + 1] = pal[(i - first_color) * 3 + 1 ] << 2;
		tga_palette[i * 3 + 2] = pal[(i - first_color) * 3] << 2;
	}


	/* open file */
	fd = fopen(fname, "wb+");

	if (!fd) {
		fprintf(stderr, "%s(%s ...) failed to open file\n",
			__func__, fname);
		return;
	}

	/* Write Header */
	result = fwrite(tga_header, sizeof(char), 18, fd);

	if (result != 18) {
		fprintf(stderr, "%s(%s ...) failed to write tga_header\n",
			__func__, fname);
		fclose(fd);
		return;
	}

	/* Write Palette */
	result = fwrite(tga_palette, sizeof(char), 256 * 3, fd);

	if (result != 256 * 3) {
		fprintf(stderr, "%s(%s ...) failed to write tga_palette\n",
			__func__, fname);
		fclose(fd);
		return;
	}

	/* Write Data */
	result = fwrite(data, sizeof(char), x * y, fd);

	if (result != x * y) {
		fprintf(stderr, "%s(%s ...) failed to write data\n",
			__func__, fname);
		fclose(fd);
		return;
	}


	/* Close File */
	fclose(fd);
}
