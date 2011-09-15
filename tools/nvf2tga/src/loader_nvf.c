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
#include <string.h>

#include <packer.h>
#include <dump.h>
#include <format.h>

struct struct_color {
	signed char r, g, b;
};

static const struct struct_color pal_ggsts[32] = {
	{0x00, 0x00, 0x00},
	{0x38, 0x30, 0x28},
	{0x30, 0x24, 0x1c},
	{0x24, 0x18, 0x10},
	{0x20, 0x14, 0x0c},
	{0x18, 0x0c, 0x08},
	{0x14, 0x08, 0x04},
	{0x0c, 0x04, 0x00},
	{0x3c, 0x38, 0x00},
	{0x38, 0x28, 0x00},
	{0x30, 0x1c, 0x00},
	{0x2c, 0x14, 0x00},
	{0x3c, 0x24, 0x00},
	{0x3c, 0x0c, 0x00},
	{0x3c, 0x00, 0x08},
	{0x3c, 0x00, 0x20},
	{0x00, 0x10, 0x00},
	{0x00, 0x14, 0x00},
	{0x00, 0x1c, 0x00},
	{0x04, 0x20, 0x00},
	{0x04, 0x28, 0x00},
	{0x1c, 0x38, 0x3c},
	{0x14, 0x2c, 0x34},
	{0x0c, 0x1c, 0x30},
	{0x04, 0x0c, 0x28},
	{0x00, 0x00, 0x24},
	{0x10, 0x10, 0x10},
	{0x14, 0x14, 0x14},
	{0x1c, 0x1c, 0x1c},
	{0x24, 0x24, 0x24},
	{0x30, 0x30, 0x30},
	{0x3c, 0x3c, 0x3c},
};

/* This palette from V3.02de differs a bit from all the others */
static const struct struct_color pal_ggsts_alt[32] = {
	{0x30, 0x00, 0x00},
	{0x30, 0x30, 0x00},
	{0x00, 0x00, 0x30},
	{0x00, 0x00, 0x00},
	{0x38, 0x30, 0x28},
	{0x30, 0x24, 0x1c},
	{0x24, 0x18, 0x10},
	{0x20, 0x14, 0x0c},
	{0x18, 0x0c, 0x08},
	{0x14, 0x08, 0x04},
	{0x0c, 0x04, 0x00},
	{0x3c, 0x38, 0x00},
	{0x38, 0x28, 0x00},
	{0x30, 0x1c, 0x00},
	{0x2c, 0x14, 0x00},
	{0x3c, 0x24, 0x00},
	{0x3c, 0x0c, 0x00},
	{0x3c, 0x00, 0x08},
	{0x3c, 0x00, 0x20},
	{0x00, 0x10, 0x00},
	{0x00, 0x14, 0x00},
	{0x00, 0x1c, 0x00},
	{0x04, 0x20, 0x00},
	{0x04, 0x28, 0x00},
	{0x1c, 0x38, 0x3c},
	{0x14, 0x2c, 0x34},
	{0x0c, 0x1c, 0x30},
	{0x04, 0x0c, 0x28},
	{0x00, 0x00, 0x24},
	{0x10, 0x10, 0x10},
	{0x14, 0x14, 0x14},
	{0x1c, 0x1c, 0x1c},
};

struct struct_special_nvf {
	const char name[16];
	const unsigned short length;
	const unsigned short mode;
	const unsigned short blocks;

	const struct struct_color *pal;
	const unsigned short colors;
	const unsigned short first_color;
};

static const struct struct_special_nvf special_nvf[] = {
	{ "GGSTS.NVF", 16771, 0x02, 0xa5, pal_ggsts, 0x20, 0x40 },
	/* Terminator */
	{ "", 0, 0, 0, NULL, 0, 0}
};

static void do_mode_0(unsigned short blocks, const char *buf, size_t len)
{
	unsigned long i;
	unsigned long data_sum = 0;
	size_t calc_len;
	char *data, *pal;
	unsigned short colors, x, y;

	if (len < 4) {
		printf("The buffer is to small to hold valid values.\n");
		return;
	}

	x = get_ushort(buf);
	y = get_ushort(buf + 2);

	data_sum = 4 + blocks * x * y;

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values");
		return;
	}

	colors = get_ushort(buf + data_sum);
	calc_len = data_sum + 2 + 3 * colors;

	if (len != calc_len) {
		printf("The data %lu has not the expected size %lu\n",
		       len, calc_len);
		return;
	}

	printf("NVF-Mode 0 (same size/unpacked): %03d Pics\n", blocks);

	pal = (char *)(buf + data_sum + 2);
	data = (char *)(buf + 4);

	for (i = 0; i < blocks; i++) {
		char fname[100];

		sprintf(fname, "PIC%03lu.TGA", i);

		dump_tga(fname, x, y, data, colors, 0, pal);
		data += x * y;
	}
}

static void do_mode_1(unsigned short blocks, const char *buf, size_t len)
{
	unsigned long i;
	unsigned long data_sum = 0;
	size_t calc_len;
	char *data, *pal;
	unsigned short colors, x, y;

	if (len < blocks * 4) {
		printf("The buffer is to small to hold valid values.\n");
		return;
	}

	for (i = 0; i < blocks; i++)
		data_sum +=
		    4 + get_ushort(buf + 4 * i) * get_ushort(buf + 4 * i + 2);

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values");
		return;
	}

	colors = get_ushort(buf + data_sum);
	calc_len = data_sum + 2 + 3 * colors;

	if (len != calc_len) {
		printf("The data %lu has not the expected size %lu\n",
		       len, calc_len);
		return;
	}

	printf("NVF-Mode 1 (different size/unpacked): %03d Pics\n", blocks);

	pal = (char *)(buf + data_sum + 2);
	data = (char *)(buf + blocks * 4);

	for (i = 0; i < blocks; i++) {
		char fname[100];

		sprintf(fname, "PIC%03lu.TGA", i);

		x = get_ushort(buf + i * 4);
		y = get_ushort(buf + i * 4 + 2);

		dump_tga(fname, x, y, data, colors, 0, pal);
		data += x * y;
	}
}

static void do_mode_same(unsigned short blocks, const char *buf, size_t len,
			 unsigned char mode)
{
	unsigned long i;
	unsigned long data_sum = 0;
	size_t calc_len;
	char *pal;
	char *data, *pdata;
	unsigned short x, y;
	unsigned short colors, first_color = 0;

	if (len < 4 + blocks * 4) {
		printf("The buffer is to small to hold valid values.\n");
		return;
	}

	x = get_ushort(buf);
	y = get_ushort(buf + 2);

	data_sum = 4 + 4 * blocks;
	for (i = 0; i < blocks; i++)
		data_sum += get_uint(buf + 4 + i * 4);

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values\n");
		return;
	}

	/* This is the case in DSA1/ROA1 */
	if (len == data_sum) {
		for (i = 0; special_nvf[i].name[0] != '\0'; i++) {

			if (special_nvf[i].length != len + 3)
				continue;
			if (special_nvf[i].mode != mode)
				continue;
			if (special_nvf[i].blocks != blocks)
				continue;

			printf("Seems to be %s. Using hardcoded palette\n",
				special_nvf[i].name);

			first_color = special_nvf[i].first_color;
			colors = special_nvf[i].colors + first_color;
			pal = (char*)special_nvf[i].pal;
		}

		if (pal == NULL) {
			printf("Unknown NVF-file without palette\n");
			return;
		}
	} else {

		colors = get_ushort(buf + data_sum);
		calc_len = data_sum + 2 + 3 * colors;

		if (len != calc_len) {
			printf("The data %lu has not the expected size %lu\n",
			       len, calc_len);
			return;
		}

		pal = (char *)(buf + data_sum + 2);
	}

	pdata = (char *)(buf + 4 + 4 * blocks);

	if (mode == 2)
		printf("NVF-Mode 2 (same size/PP20): %03d Pics\n", blocks);
	else
		printf("NVF-Mode 4 (same size/RLE): %03d Pics\n", blocks);

	data = malloc(x * y);
	if (!data) {
		fprintf(stderr, "Not enough memory\n");
		return;
	}

	for (i = 0; i < blocks; i++) {
		unsigned long plen = get_uint(buf + 4 + i * 4);
		char fname[100];

		sprintf(fname, "PIC%03lu.TGA", i);

		memset(data, 0, x * y);

		if (mode == 2)
			ppdepack(pdata, data, plen, x * y);
		else
			un_rle(pdata, data, plen);

		dump_tga(fname, x, y, data, colors, first_color, pal);
		pdata += get_uint(buf + 4 + i * 4);
	}

	free(data);
}

/* Packed Images with different sizes */
static void do_mode_diff(unsigned short blocks, const char *buf, size_t len,
			 unsigned char mode)
{
	unsigned long i;
	unsigned long data_sum = 0;
	size_t calc_len;
	char *pal;
	char *data, *pdata;
	unsigned short colors, first_color = 0;

	if (len < blocks * 8) {
		printf("The buffer is to small to hold valid values.\n");
		return;
	}

	data_sum = 8 * blocks;

	for (i = 0; i < blocks; i++)
		data_sum += get_uint(buf + i * 8 + 4);

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values");
		return;
	}

	colors = get_ushort(buf + data_sum);
	calc_len = data_sum + 2 + 3 * colors;

	if (len != calc_len) {
		printf("The data %lu has not the expected size %lu\n",
		       len, calc_len);
		return;
	}

	if (mode == 3)
		printf("NVF-Mode 2 (different size/PP20): %03d Pics\n", blocks);
	else
		printf("NVF-Mode 4 (different size/RLE): %03d Pics\n", blocks);

	pal = (char *)(buf + data_sum + 2);
	pdata = (char *)(buf + 8 * blocks);

	for (i = 0; i < blocks; i++) {
		unsigned long plen;
		unsigned short x, y;
		char fname[100];

		sprintf(fname, "PIC%03lu.TGA", i);

		x = get_ushort(buf + i * 8);
		y = get_ushort(buf + i * 8 + 2);
		plen = get_uint(buf + i * 8 + 4);

		data = malloc(x * y);
		if (!data) {
			fprintf(stderr, "Not enough memory\n");
			return;
		}

		memset(data, 0, x * y);
		if (mode == 3)
			ppdepack(pdata, data, plen, x * y);
		else
			un_rle(pdata, data, plen);

		dump_tga(fname, x, y, data, colors, first_color, pal);
		pdata += plen;
		free(data);
	}
}

void process_nvf(const char *buf, size_t len)
{
	unsigned char mode;
	unsigned short blocks;

	mode = *(unsigned char *)(buf);

	if (mode >= 6) {
		fprintf(stderr, "No valid crunchmode %d\n", mode);
		return;
	}

	blocks = get_ushort(buf + 1);
	switch (mode) {
	case 0:
		do_mode_0(blocks, buf + 3, len - 3);
		break;
	case 1:
		do_mode_1(blocks, buf + 3, len - 3);
		break;
	case 2:
	case 4:
		do_mode_same(blocks, buf + 3, len - 3, mode);
		break;
	case 3:
	case 5:
		do_mode_diff(blocks, buf + 3, len - 3, mode);
		break;
	default:
		printf("NVF-Mode %u is not supported\n", mode);
	}
}
