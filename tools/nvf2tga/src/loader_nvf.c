/*
 * NVF loader
 *
 * Loads/Dumps an NVF file to/from an ImageSet structure.
 * NVF files are used by DSA/ROA 2+3
 * Some files of DSA/ROA 1 work, too, but aren't supported.
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

static const struct struct_color pal_gray[32] = {
	{0x00, 0x00, 0x00},
	{0x38, 0x38, 0x38},
	{0x34, 0x34, 0x34},
	{0x30, 0x30, 0x30},
	{0x2c, 0x2c, 0x2c},
	{0x28, 0x28, 0x28},
	{0x24, 0x24, 0x24},
	{0x20, 0x20, 0x20},
	{0x1c, 0x1c, 0x1c},
	{0x18, 0x18, 0x18},
	{0x14, 0x14, 0x14},
	{0x10, 0x10, 0x10},
	{0x0c, 0x0c, 0x0c},
	{0x08, 0x08, 0x08},
	{0x04, 0x04, 0x04},
	{0x00, 0x00, 0x00},
	{0x00, 0x3c, 0x08},
	{0x3c, 0x30, 0x28},
	{0x3c, 0x2c, 0x28},
	{0x2c, 0x1c, 0x18},
	{0x24, 0x14, 0x10},
	{0x1c, 0x10, 0x0c},
	{0x14, 0x08, 0x08},
	{0x0c, 0x04, 0x04},
	{0x0c, 0x08, 0x00},
	{0x2c, 0x00, 0x00},
	{0x10, 0x14, 0x3c},
	{0x00, 0x08, 0x38},
	{0x00, 0x08, 0x34},
	{0x00, 0x00, 0x18},
	{0x3c, 0x3c, 0x00},
	{0x3c, 0x3c, 0x3c},
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
	const unsigned int length;
	const unsigned short mode;
	const unsigned short blocks;

	const struct struct_color *pal;
	const unsigned short colors;
	const unsigned short first_color;
};

/* DSA1/ROA1 NVF files without palette */
static const struct struct_special_nvf special_nvf[] = {
	/* All versions */
	{ "GGSTS.NVF", 16771, 0x02, 0xa5, pal_ggsts, 0x20, 0x40 },
	/* V1.0X */
	{ "COMPASS", 2183, 0x02, 0x04, pal_gray, 0x20, 0xe0},
	/* V3.0X */
	{ "COMPASS", 2167, 0x02, 0x04, pal_gray, 0x20, 0xe0},

	/* Terminator */
	{ "", 0, 0, 0, NULL, 0, 0}
};

/* DSA1/ROA1 NVF files with palette */
/* We need here the first color to set the palette the correct way */
static const struct struct_special_nvf special_nvf_pal[] = {
	/* All versions */
	{ "GUERTEL.NVF", 67387, 5, 0xc, NULL, 0x20, 0x80 },
	/* All versions */
	{ "SHIPSL.NVF", 155745, 5, 0x3a, NULL, 0x20, 0x80 },
	/* All versions */
	{ "STONESL.NVF", 188299, 5, 0x3d, NULL, 0x20, 0x80 },
	/* All V1.xx versions */
	{ "MARBLESL.NVF", 159754, 5, 0x3a, NULL, 0x20, 0x80 },
	/* All V3.xx versions */
	{ "MARBLESL.NVF", 159140, 5, 0x3a, NULL, 0x20, 0x80 },
	/* All versions */
	{ "TDIVERSE.NVF", 23249, 5, 0x09, NULL, 0x20, 0x80 },
	/* All versions */
	{ "LTURM.NVF", 21533, 5, 0x04, NULL, 0x20, 0x80 },
	/* Terminator */
	{ "", 0, 0, 0, NULL, 0, 0}
};

static ImageSet* do_mode_0(ImageSet* img, const char *buf, size_t len)
{
	unsigned long i;
	unsigned long data_sum = 0;
	size_t calc_len;
	char *data;
	unsigned short colors, x, y;

	if (len < 4) {
		printf("The buffer is to small to hold valid values.\n");
		return NULL;
	}

	x = get_ushort(buf);
	y = get_ushort(buf + 2);

	data_sum = 4 + img->frameCount * x * y;

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values");
		return NULL;
	}

	colors = get_ushort(buf + data_sum);
	calc_len = data_sum + 2 + 3 * colors;

	if (len != calc_len) {
		printf("The data %lu has not the expected size %lu\n",
		       len, calc_len);
		return NULL;
	}

	printf("NVF-Mode 0 (same size/unpacked): %03d Pics\n", img->frameCount);

	img->globalPalette = (Color*)(buf + data_sum + 2);
	data = (char *)(buf + 4);

	for (i = 0; i < img->frameCount; i++) {
		img->frames[i]->x0 = img->frames[i]->y0 = 0;
		img->frames[i]->width  = x;
		img->frames[i]->height = y;
		img->frames[i]->delay  = 200;
		img->frames[i]->localPalette = 0;
		img->frames[i]->pixels = data;

		data += x * y;
	}
	return img;
}

static ImageSet* do_mode_1(ImageSet* img, const char *buf, size_t len)
{
	unsigned long i;
	unsigned long data_sum = 0;
	size_t calc_len;
	char *data, *pal;
	unsigned short colors, x, y;

	if (len < img->frameCount * 4) {
		printf("The buffer is to small to hold valid values.\n");
		return NULL;
	}

	for (i = 0; i < img->frameCount; i++) {
	    data_sum += 4 + get_ushort(buf + 4*i) * get_ushort(buf + 4*i + 2);
	    printf("datasum: %d = 4 + %d*%d\n", data_sum, get_ushort(buf + 4*i), get_ushort(buf + 4*i + 2));
	}

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values");
		return NULL;
	}

	colors = get_ushort(buf + data_sum);
	calc_len = data_sum + 2 + 3 * colors;

	if (len != calc_len) {
		printf("The data %lu has not the expected size %lu\n",
		       len, calc_len);
		return NULL;
	}

	printf("NVF-Mode 1 (different size/unpacked): %03d Pics\n", img->frameCount);

	img->globalPalette = (Color*)(buf + data_sum + 2);
	data = (char *)(buf + img->frameCount * 4);

	for (i = 0; i < img->frameCount; i++) {
		x = get_ushort(buf + i * 4);
		y = get_ushort(buf + i * 4 + 2);

		img->frames[i]->x0 = img->frames[i]->y0 = 0;
		img->frames[i]->width  = x;
		img->frames[i]->height = y;
		img->frames[i]->delay  = 200;
		img->frames[i]->localPalette = 0;
		img->frames[i]->pixels = data;

		data += x * y;
	}
	return img;
}

static ImageSet* do_mode_same(ImageSet* img, const char *buf, size_t len,
			 unsigned char mode)
{
	unsigned long i;
	unsigned long data_sum = 0;
	size_t calc_len;
	char *data, *pdata;
	unsigned short x, y;
	unsigned short colors = 0, first_color = 0;

	if (len < 4 + img->frameCount * 4) {
		printf("The buffer is to small to hold valid values.\n");
		return NULL;
	}

	x = get_ushort(buf);
	y = get_ushort(buf + 2);

	data_sum = 4 + 4 * img->frameCount;
	for (i = 0; i < img->frameCount; i++)
		data_sum += get_uint(buf + 4 + i * 4);

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values\n");
		return NULL;
	}
	img->globalPalette = NULL;

	/* This is the case in DSA1/ROA1 */
	if (len == data_sum) {
		for (i = 0; special_nvf[i].name[0] != '\0'; i++) {
		    
			if (special_nvf[i].length != len + 3)
				continue;
			if (special_nvf[i].mode != mode)
				continue;
			if (special_nvf[i].blocks != img->frameCount)
				continue;

			printf("Seems to be %s. Using hardcoded palette\n",
				special_nvf[i].name);

			first_color = special_nvf[i].first_color;
			colors = special_nvf[i].colors + first_color;
			// TODO: dirty hack
			img->globalPalette = (Color*)(special_nvf[i].pal - first_color*3);
		}

		if (img->globalPalette == NULL) {
		    printf("Unknown NVF-file without palette\n");
		    return NULL;
		}
	} else {

		for (i = 0; special_nvf_pal[i].name[0] != '\0'; i++) {

			if (special_nvf_pal[i].length != len + 3)
				continue;
			if (special_nvf_pal[i].mode != mode)
				continue;
			if (special_nvf_pal[i].blocks != img->frameCount)
				continue;

			printf("Seems to be %s Setting first_color\n",
				special_nvf_pal[i].name);

			first_color = special_nvf_pal[i].first_color;
			colors = special_nvf_pal[i].colors;
		}

		colors += get_ushort(buf + data_sum);
		calc_len = data_sum + 2 + 3 * colors;

		if (len != calc_len) {
			printf("The data %lu has not the expected size %lu\n",
			       len, calc_len);
			return NULL;
		}

		// TODO: dirty hack
		img->globalPalette = (Color*)((buf + data_sum + 2) - (first_color*3));
	}

	pdata = (char *)(buf + 4 + 4 * img->frameCount);

	if (mode == 2)
		printf("NVF-Mode 2 (same size/PP20): %03d Pics\n", img->frameCount);
	else
		printf("NVF-Mode 4 (same size/RLE): %03d Pics\n", img->frameCount);

	for (i = 0; i < img->frameCount; i++) {
		unsigned long plen = get_uint(buf + 4 + i * 4);
		data = malloc(x * y);
		if (!data) {
		    fprintf(stderr, "Not enough memory\n");
		    return NULL;
		}
		memset(data, 0, x * y);

		if (mode == 2)
			ppdepack(pdata, data, plen, x * y);
		else
			un_rle(pdata, data, plen);

		img->frames[i]->x0 = img->frames[i]->y0 = 0;
		img->frames[i]->width  = x;
		img->frames[i]->height = y;
		img->frames[i]->delay  = 200;
		img->frames[i]->localPalette = 0;
		img->frames[i]->pixels = data;

		pdata += get_uint(buf + 4 + i * 4);
	}

	return img;
}

/* Packed Images with different sizes */
static ImageSet* do_mode_diff(ImageSet* img, const char *buf, size_t len,
			 unsigned char mode)
{
	unsigned long i;
	unsigned long data_sum = 0;
	size_t calc_len;
	char *pal;
	char *data, *pdata;
	unsigned short colors = 0, first_color = 0;

	if (len < img->frameCount * 8) {
		printf("The buffer is to small to hold valid values.\n");
		return NULL;
	}

	data_sum = 8 * img->frameCount;

	for (i = 0; i < img->frameCount; i++)
		data_sum += get_uint(buf + i * 8 + 4);

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values");
		return NULL;
	}

	/* This is the case in DSA1/ROA1 */
	if (len == data_sum) {
		for (i = 0; special_nvf[i].name[0] != '\0'; i++) {

			if (special_nvf[i].length != len + 3)
				continue;
			if (special_nvf[i].mode != mode)
				continue;
			if (special_nvf[i].blocks != img->frameCount)
				continue;

			printf("Seems to be %s. Using hardcoded palette\n",
				special_nvf[i].name);

			first_color = special_nvf[i].first_color;
			colors = special_nvf[i].colors + first_color;
			// TODO: dirty hack
			img->globalPalette = (Color*)(special_nvf[i].pal - first_color*3);
		}

		if (pal == NULL) {
			printf("Unknown NVF-file without palette\n");
			return NULL;
		}
	} else {

		for (i = 0; special_nvf_pal[i].name[0] != '\0'; i++) {

			if (special_nvf_pal[i].length != len + 3)
				continue;
			if (special_nvf_pal[i].mode != mode)
				continue;
			if (special_nvf_pal[i].blocks != img->frameCount)
				continue;

			printf("Seems to be %s Setting first_color\n",
				special_nvf_pal[i].name);

			first_color = special_nvf_pal[i].first_color;
			colors = first_color;
		}

		colors += get_ushort(buf + data_sum);
		calc_len = data_sum + 2 + 3 * get_ushort(buf + data_sum);
		if (len != calc_len) {
			printf("The data %lu has not the expected size %lu\n",
			       len, calc_len);
			return NULL;
		}

		// TODO: dirty hack
		img->globalPalette = (Color*)((buf + data_sum + 2) - (first_color*3));
	}

	if (len != calc_len) {
		printf("The data %lu has not the expected size %lu\n",
		       len, calc_len);
		return NULL;
	}

	if (mode == 3)
		printf("NVF-Mode 3 (different size/PP20): %03d Pics\n", img->frameCount);
	else
		printf("NVF-Mode 5 (different size/RLE): %03d Pics\n", img->frameCount);

	// TODO: dirty hack
	img->globalPalette = (Color*)((buf + data_sum + 2) - (first_color*3));
	pdata = (char *)(buf + 8 * img->frameCount);

	for (i = 0; i < img->frameCount; i++) {
		unsigned long plen;
		unsigned short x, y;

		x = get_ushort(buf + i * 8);
		y = get_ushort(buf + i * 8 + 2);
		plen = get_uint(buf + i * 8 + 4);

		data = malloc(x * y);
		if (!data) {
			fprintf(stderr, "Not enough memory\n");
			return NULL;
		}

		memset(data, 0, x * y);
		if (mode == 3)
			ppdepack(pdata, data, plen, x * y);
		else
			un_rle(pdata, data, plen);

		img->frames[i]->x0 = img->frames[i]->y0 = 0;
		img->frames[i]->width  = x;
		img->frames[i]->height = y;
		img->frames[i]->delay  = 200;
		img->frames[i]->localPalette = (Color*)pdata;
		img->frames[i]->pixels = data;

		pdata += plen;
	}
	return img;
}

int sanitycheck_nvf(const char* buf, size_t len) {
	// TODO
	return 1;
}

ImageSet* process_nvf(const char *buf, size_t len)
{
	unsigned char mode;
	ImageSet* img;

	mode = *(unsigned char *)(buf);

	if (mode >= 6) {
		fprintf(stderr, "No valid crunchmode %d\n", mode);
		return NULL;
	}

	img = (ImageSet*)malloc(sizeof(ImageSet));
	img->frameCount   = get_ushort(buf + 1);
	img->frames       = (AnimFrame**)malloc(img->frameCount * sizeof(AnimFrame*));
	for (int i = 0; i<img->frameCount; i++) img->frames[i] = (AnimFrame*)malloc(sizeof(AnimFrame));
	
	switch (mode) {
	case 0:
		img = do_mode_0(img, buf + 3, len - 3);
		break;
	case 1:
		img = do_mode_1(img, buf + 3, len - 3);
		break;
	case 2:
	case 4:
		img = do_mode_same(img, buf + 3, len - 3, mode);
		break;
	case 3:
	case 5:
		img = do_mode_diff(img, buf + 3, len - 3, mode);
		break;
	default:
		printf("NVF-Mode %u is not supported\n", mode);
	}
	img->globalWidth  = img->frames[0]->width;
	img->globalHeight = img->frames[0]->height;;
	return img;
}

int dump_nvf(ImageSet* img, char* prefix) {
	int buflen, i, length;
	char *buf, *buf_start;
	char *filename;

	// Größe der Datei bestimmen, Speicher bereitstellen
	buflen = 3 + img->frameCount * 4;
	for (i=0; i<img->frameCount; i++) {
		buflen += img->frames[i]->width * img->frames[i]->height;
	}
	buflen += 2 + 3 * 256; // Farbpalette
	buf_start = buf = (char*)malloc(buflen);

	// Globalen Header + Größenangaben (width/height) schreiben
	buf[0] = 1; // Bildmodus 1: verschiedene Bildgrößen, keine Komprimierung
	set_ushort(buf+1, img->frameCount);
	buf+= 3;
	// Bildgrößen schreiben
	for (i=0; i<img->frameCount; i++) {
		set_ushort(buf + 0, img->frames[i]->width);
		set_ushort(buf + 2, img->frames[i]->height);
		buf += 4;
	}
	// Bilddaten schreiben
	for (i=0; i<img->frameCount; i++) {
		length = img->frames[i]->width * img->frames[i]->height;
		memcpy(buf, img->frames[i]->pixels, length);
		buf += length;
	}
	// Farbpalette schreiben
	set_ushort(buf, 256);
	buf+= 2;
	for (i=0; i<256; i++) {
		buf[3*i+0] = img->globalPalette[i].r;
		buf[3*i+1] = img->globalPalette[i].g;
		buf[3*i+2] = img->globalPalette[i].b;
	}
	// Schreiben der Datei
	filename = (char*)malloc((strlen(prefix)+5) * sizeof(char));
	sprintf(filename, "%s.NVF", prefix);
	FILE* fd = fopen(filename, "wb+");
	fwrite(buf_start, sizeof(char), buflen, fd);
	fclose(fd);
	free(filename);
	return 1;
}
