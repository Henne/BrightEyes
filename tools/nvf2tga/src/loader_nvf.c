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
    const uint32_t length;
    const uint16_t mode;
    const uint16_t blocks;

    const struct struct_color *pal;
    const uint16_t colors;
    const uint16_t first_color;
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
    uint32_t  i;
    uint32_t  data_sum = 0;
    size_t    calc_len;
    char*     data;
    uint16_t  colors, x, y;
    Sequence* seq = &img->sequences[0];
    
    if (len < 4) {
	printf("The buffer is to small to hold valid values.\n");
	return NULL;
    }

    x = get_uint16(buf);
    y = get_uint16(buf + 2);

    data_sum = 4 + seq->imgCount * x * y;

    if (len < data_sum) {
	printf("The buffer is to small to hold valid values");
	return NULL;
    }

    colors = get_uint16(buf + data_sum);
    calc_len = data_sum + 2 + 3 * colors;

    if (len != calc_len) {
	printf("The data %lu has not the expected size %lu\n",
	       len, calc_len);
	return NULL;
    }

    printf("NVF-Mode 0 (same size/unpacked): %03d Pics\n", seq->imgCount);

    img->palette = (Color*)(buf + data_sum + 2);
    data = (char *)(buf + 4);

    for (i = 0; i < seq->imgCount; i++) {
	seq->img[i].x0 = seq->img[i].y0 = 0;
	seq->img[i].width  = x;
	seq->img[i].height = y;
	seq->img[i].palette = 0;
	seq->img[i].pixels = data;

	data += x * y;
    }
    return img;
}

static ImageSet* do_mode_1(ImageSet* img, const char *buf, size_t len) {
    uint32_t i;
    uint32_t data_sum = 0;
    size_t   calc_len;
    char     *data, *pal;
    uint16_t colors, x, y;
    Sequence* seq = &img->sequences[0];

    if (len < seq->imgCount * 4) {
	printf("The buffer is to small to hold valid values.\n");
	return NULL;
    }

    for (i = 0; i < seq->imgCount; i++) {
	data_sum += 4 + get_uint16(buf + 4*i) * get_uint16(buf + 4*i + 2);
	printf("datasum: %d = 4 + %d*%d\n", data_sum,
	       get_uint16(buf + 4*i), get_uint16(buf + 4*i + 2));
    }

    if (len < data_sum) {
	printf("The buffer is to small to hold valid values");
	return NULL;
    }

    colors = get_uint16(buf + data_sum);
    calc_len = data_sum + 2 + 3 * colors;

    if (len != calc_len) {
	printf("The data %lu has not the expected size %lu\n",
	       len, calc_len);
	return NULL;
    }

    printf("NVF-Mode 1 (different size/unpacked): %03d Pics\n", seq->imgCount);

    img->palette = (Color*)(buf + data_sum + 2);
    data = (char *)(buf + seq->imgCount * 4);

    for (i = 0; i < seq->imgCount; i++) {
	x = get_uint16(buf + i * 4);
	y = get_uint16(buf + i * 4 + 2);

	seq->img[i].x0 = seq->img[i].y0 = 0;
	seq->img[i].width  = x;
	seq->img[i].height = y;
	seq->img[i].palette = 0;
	seq->img[i].pixels = data;

	data += x * y;
    }
    return img;
}

static ImageSet* do_mode_same(ImageSet* img, const char *buf, size_t len, uint8_t mode) {
    uint32_t i;
    uint32_t data_sum = 0;
    size_t   calc_len;
    char     *data, *pdata;
    uint16_t x, y;
    uint16_t colors = 0, first_color = 0;
    Sequence* seq = &img->sequences[0];

    if (len < 4 + seq->imgCount * 4) {
	printf("The buffer is to small to hold valid values.\n");
	return NULL;
    }

    x = get_uint16(buf);
    y = get_uint16(buf + 2);

    data_sum = 4 + 4 * seq->imgCount;
    for (i = 0; i < seq->imgCount; i++)
	data_sum += get_uint32(buf + 4 + i * 4);

    if (len < data_sum) {
	printf("The buffer is to small to hold valid values\n");
	return NULL;
    }
    img->palette = NULL;

    /* This is the case in DSA1/ROA1 */
    if (len == data_sum) {
	for (i = 0; special_nvf[i].name[0] != '\0'; i++) {
		    
	    if (special_nvf[i].length != len + 3)
		continue;
	    if (special_nvf[i].mode != mode)
		continue;
	    if (special_nvf[i].blocks != seq->imgCount)
		continue;

	    printf("Seems to be %s. Using hardcoded palette\n",
		   special_nvf[i].name);

	    first_color = special_nvf[i].first_color;
	    colors = special_nvf[i].colors + first_color;
	    // TODO: dirty hack
	    img->palette = (Color*)(special_nvf[i].pal - first_color*3);
	}

	if (img->palette == NULL) {
	    printf("Unknown NVF-file without palette\n");
	    return NULL;
	}
    } else {

	for (i = 0; special_nvf_pal[i].name[0] != '\0'; i++) {

	    if (special_nvf_pal[i].length != len + 3)
		continue;
	    if (special_nvf_pal[i].mode != mode)
		continue;
	    if (special_nvf_pal[i].blocks != seq->imgCount)
		continue;

	    printf("Seems to be %s Setting first_color\n",
		   special_nvf_pal[i].name);

	    first_color = special_nvf_pal[i].first_color;
	    colors = special_nvf_pal[i].colors;
	}

	colors += get_uint16(buf + data_sum);
	calc_len = data_sum + 2 + 3 * colors;

	if (len != calc_len) {
	    printf("The data %lu has not the expected size %lu\n",
		   len, calc_len);
	    return NULL;
	}

	// TODO: dirty hack
	img->palette = (Color*)((buf + data_sum + 2) - (first_color*3));
    }

    pdata = (char *)(buf + 4 + 4 * seq->imgCount);

    if (mode == 2)
	printf("NVF-Mode 2 (same size/PP20): %03d Pics\n", seq->imgCount);
    else
	printf("NVF-Mode 4 (same size/RLE): %03d Pics\n", seq->imgCount);

    for (i = 0; i < seq->imgCount; i++) {
	uint32_t plen = get_uint32(buf + 4 + i * 4);
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

	seq->img[i].x0 = seq->img[i].y0 = 0;
	seq->img[i].width  = x;
	seq->img[i].height = y;
	seq->img[i].palette = 0;
	seq->img[i].pixels = data;

	pdata += get_uint32(buf + 4 + i * 4);
    }

    return img;
}

/* Packed Images with different sizes */
static ImageSet* do_mode_diff(ImageSet* img, const char *buf, size_t len, uint8_t mode) {
    uint32_t i;
    uint32_t data_sum = 0;
    size_t   calc_len;
    char     *pal;
    char     *data, *pdata;
    uint16_t colors = 0, first_color = 0;
    Sequence* seq = &img->sequences[0];

    if (len < seq->imgCount * 8) {
	printf("The buffer is to small to hold valid values.\n");
	return NULL;
    }

    data_sum = 8 * seq->imgCount;

    for (i = 0; i < seq->imgCount; i++)
	data_sum += get_uint32(buf + i * 8 + 4);

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
	    if (special_nvf[i].blocks != seq->imgCount)
		continue;

	    printf("Seems to be %s. Using hardcoded palette\n",
		   special_nvf[i].name);

	    first_color = special_nvf[i].first_color;
	    colors = special_nvf[i].colors + first_color;
	    // TODO: dirty hack
	    img->palette = (Color*)(special_nvf[i].pal - first_color*3);
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
	    if (special_nvf_pal[i].blocks != seq->imgCount)
		continue;

	    printf("Seems to be %s Setting first_color\n",
		   special_nvf_pal[i].name);

	    first_color = special_nvf_pal[i].first_color;
	    colors = first_color;
	}

	colors += get_uint16(buf + data_sum);
	calc_len = data_sum + 2 + 3 * get_uint16(buf + data_sum);
	if (len != calc_len) {
	    printf("The data %lu has not the expected size %lu\n",
		   len, calc_len);
	    return NULL;
	}

	// TODO: dirty hack
	img->palette = (Color*)((buf + data_sum + 2) - (first_color*3));
    }

    if (len != calc_len) {
	printf("The data %lu has not the expected size %lu\n",
	       len, calc_len);
	return NULL;
    }

    if (mode == 3)
	printf("NVF-Mode 3 (different size/PP20): %03d Pics\n", seq->imgCount);
    else
	printf("NVF-Mode 5 (different size/RLE): %03d Pics\n", seq->imgCount);

    // TODO: dirty hack
    img->palette = (Color*)((buf + data_sum + 2) - (first_color*3));
    pdata = (char *)(buf + 8 * seq->imgCount);

    for (i = 0; i < seq->imgCount; i++) {
	uint32_t plen;
	uint16_t x, y;

	x = get_uint16(buf + i * 8);
	y = get_uint16(buf + i * 8 + 2);
	plen = get_uint32(buf + i * 8 + 4);

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

	seq->img[i].x0 = seq->img[i].y0 = 0;
	seq->img[i].width  = x;
	seq->img[i].height = y;
	seq->img[i].palette = (Color*)pdata;
	seq->img[i].pixels = data;

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
    uint8_t mode;
    ImageSet* img;
    Sequence* seq;

    mode = *(uint8_t *)(buf);

    if (mode >= 6) {
	fprintf(stderr, "No valid crunchmode %d\n", mode);
	return NULL;
    }

    img = (ImageSet*)malloc(sizeof(ImageSet));
    img->mainPixels = NULL;
    img->seqCount   = 1;
    img->sequences  = (Sequence*)malloc(sizeof(Sequence));
    seq = &img->sequences[0];
    seq->frameCount = 0; // Keine feste Sequenz: Spiele einfach alle Bilder nacheinander ab.
    seq->frames = NULL;
    seq->defaultDelay = 100;

    seq->imgCount   = get_uint16(buf + 1);
    seq->img = malloc(seq->imgCount * sizeof(MyImage));
    
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

    return img;
}

int dump_nvf(ImageSet* img, char* prefix) {
    int buflen, i, length;
    char *buf, *buf_start;
    char *filename;
    Sequence* seq = &img->sequences[0];    // Nur die 1. Sequenz wird berücksichtigt
    
    // Größe der Datei bestimmen, Speicher bereitstellen
    buflen = 3 + seq->imgCount * 4;
    for (i=0; i<seq->imgCount; i++) {
	buflen += seq->img[i].width * seq->img[i].height;
    }
    buflen += 2 + 3 * 256; // Farbpalette
    buf_start = buf = (char*)malloc(buflen);

    // Globalen Header + Größenangaben (width/height) schreiben
    buf[0] = 1; // Bildmodus 1: verschiedene Bildgrößen, keine Komprimierung
    set_uint16(buf+1, seq->imgCount);
    buf+= 3;
    // Bildgrößen schreiben
    for (i=0;  i < seq->imgCount;  i++) {
	set_uint16(buf + 0, seq->img[i].width);
	set_uint16(buf + 2, seq->img[i].height);
	buf += 4;
    }
    // Bilddaten schreiben
    for (i=0;  i < seq->imgCount;  i++) {
	length = seq->img[i].width * seq->img[i].height;
	memcpy(buf, seq->img[i].pixels, length);
	buf += length;
    }
    // Farbpalette schreiben
    set_uint16(buf, 256);
    buf+= 2;
    for (i=0; i<256; i++) {
	buf[3*i+0] = img->palette[i].r;
	buf[3*i+1] = img->palette[i].g;
	buf[3*i+2] = img->palette[i].b;
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
