/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* depack.c */

/* copyright (c) Marc Espie, 1995
 * see accompanying file README for distribution information
 */


static inline unsigned int val(const unsigned char *p) {
	return (p[0]<<16 | p[1] << 8 | p[2]);
}

static inline unsigned int le32_2_cpu(const unsigned char *p) {
	return (p[3]<<24 | p[2]<<16 | p[1]<<8 | p[0]);
}

static unsigned long depackedlen(const unsigned char *p, unsigned long plen) {
/*	DSA1/ROA1 doesn't use the first bytes as a signature "PP20".
 *	It's used instead for the lenght of the packed data. */
	if (le32_2_cpu(p) == plen)
		return val(p+plen-4);

	if (p[0] == 'P' || p[1] == 'P' || p[2] == '2' || p[3] == '0')
		return val(p+plen-4);

	/* not a powerpacker file */
	return 0;
}

static unsigned long shift_in;
static unsigned long counter;
static unsigned const char *source;

static unsigned long get_bits(unsigned long n) {

	unsigned long result = 0;
	int i;

	for (i = 0; i < n; i++)	{
		if (counter == 0) {
			counter = 8;
			shift_in = *--source;
		}
		result = (result<<1) | (shift_in & 1);
		shift_in >>= 1;
		counter--;
	}
	return result;
}

static void ppdepack(const unsigned char *packed, unsigned char *depacked,
				unsigned long plen, unsigned long unplen) {
	unsigned char *dest;
	int n_bits;
	int idx;
	unsigned long bytes;
	int to_add;
	unsigned long offset;
	unsigned char offset_sizes[4];
	int i;

	offset_sizes[0] = packed[4];	/* skip signature */
	offset_sizes[1] = packed[5];
	offset_sizes[2] = packed[6];
	offset_sizes[3] = packed[7];

	/* reset counter */
	counter=0;

	/* initialize source of bits */
	source = packed + plen - 4;

	dest = depacked + unplen;

	/* skip bits */
	get_bits(source[3]);

	/* do it forever, i.e., while the whole file isn't unpacked */
	while (1) {
		/* copy some bytes from the source anyway */
		if (get_bits(1) == 0) {
			bytes = 0;
			do {
				to_add = get_bits(2);
				bytes += to_add;
			} while (to_add == 3);

			for (i = 0; i <= bytes; i++)
				*--dest = get_bits(8);

			if (dest <= depacked)
				return;
		}
		/* decode what to copy from the destination file */
		idx = get_bits(2);
		n_bits = offset_sizes[idx];
		/* bytes to copy */
		bytes = idx+1;
		if (bytes == 4)	/* 4 means >=4 */
		{
			/* and maybe a bigger offset */
			if (get_bits(1) == 0)
				offset = get_bits(7);
			else
				offset = get_bits(n_bits);

			do {
				to_add = get_bits(3);
				bytes += to_add;
			} while (to_add == 7);
		} else offset = get_bits(n_bits);

		for (i = 0; i <= bytes; i++) {
			dest[-1] = dest[offset];
			dest--;
		}

		if (dest <= depacked)
			return;
	}
}

static inline unsigned short get_ushort(const char* buf) {
	return (*(unsigned char*)(buf+1)<<8) | *(unsigned char*)(buf);
}
static inline unsigned int get_uint(const char* buf) {
	return (*(unsigned char*)(buf+3)<<24) |
		(*(unsigned char*)(buf+2)<<16) |
		(*(unsigned char*)(buf+1)<<8) |
		*(unsigned char*)(buf);
}

void un_rle(const unsigned char *pdata, unsigned char *data, unsigned long plen, unsigned long len)
{
	unsigned long i,pos=0;

	for (i=0; i<plen; i++)
		if (pdata[i] == 0x7f) {
			unsigned long rl,col;

			rl=pdata[i+1];
			col=pdata[i+2];

			memset(data+pos, col, rl);
			i+=2;
			pos+=rl;
		} else
			data[pos++]=pdata[i];
}

void dump_tga(unsigned long nr, unsigned short x, unsigned short y, const char *data, unsigned short colors, const char *pal)
{
	FILE *fd=NULL;
	unsigned long i;
	char fname[100];

	sprintf(fname, "PIC%03lu.TGA", nr);
	fd=fopen(fname, "wb");
	/* name and open file */

	if (!fd)
		return;

	/* No picture ID */
	fputc(0, fd);
	/* Having a color palette */
	fputc(1, fd);
	/* indiceed unpacked color palette */
	fputc(1, fd);
	/* first color (WORD)*/
	fputc(0, fd);
	fputc(0, fd);
	/* colors in palette (WORD)*/
	fputc(colors & 0xff, fd);
	fputc((colors>>8)&0xff, fd);
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
	fputc((x>>8)&0xff, fd);
	/* Height */
	fputc(y & 0xff, fd);
	fputc((y>>8)&0xff, fd);
	/* bpp */
	fputc(8, fd);
	/* Attribute byte, pic starts top left*/
	fputc(32,fd);

	/* Write Palette BGR */
	for (i=0; i<colors; i++) {
		fputc(*(unsigned char*)(pal+i*3+2)*4, fd);
		fputc(*(unsigned char*)(pal+i*3+1)*4, fd);
		fputc(*(unsigned char*)(pal+i*3)*4, fd);
	}

	/* Write Data */
	fwrite(data, x*y, sizeof(char), fd);

	/* Close File */
	fclose(fd);
}

void do_mode_0(unsigned short blocks, const char *buf, size_t len)
{
	unsigned long i;
	unsigned long data_sum=0;
	size_t calc_len;
	char *data,*pal;
	unsigned short colors,x,y;

	if (len < 4) {
		printf("The buffer is to small to hold valid values.\n");
		return;
	}

	x=get_ushort(buf);
	y=get_ushort(buf+2);

	printf("Pictures %03ux%03u\n", x, y);

	data_sum=blocks*x*y;

	if (len < data_sum+3) {
		printf("The buffer is to small to hold valid values");
		return;
	}

	colors=get_ushort(buf+4+data_sum);
	calc_len=3+4+data_sum+2+3*colors;

	if (len+3 != calc_len) {
		printf("The filelen %lu is not as expected %lu\n",
				len+3, calc_len);
		return;
	}

	pal=(char*)(buf+4+data_sum+2);
	data=(char*)(buf+4);

	for (i=0; i<blocks; i++){
		dump_tga(i, x, y, data, colors, pal);
		data+=x*y;
	}
}

void do_mode_1(unsigned short blocks, const char *buf, size_t len)
{
	unsigned long i;
	unsigned long data_sum=0;
	size_t calc_len;
	char *data,*pal;
	unsigned short colors;

	if (len < blocks*4) {
		printf("The buffer is to small to hold valid values.\n");
		return;
	}

	for (i=0; i<blocks; i++) {
		printf("Picture %03lu: %03ux%03u\n", i,	get_ushort(buf+i*4),
							get_ushort(buf+i*4+2));
		data_sum+=4+get_ushort(buf+4*i)*get_ushort(buf+4*i+2);

	}

	if (len < data_sum+3) {
		printf("The buffer is to small to hold valid values");
		return;
	}

	colors=get_ushort(buf+data_sum);
	calc_len=3+data_sum+2+3*colors;

	if (len != calc_len) {
		printf("The filelen %lu is not as expected %lu\n",
				len+3, calc_len);
		return;
	}

	pal=(char*)(buf+data_sum+2);
	data=(char*)(buf+blocks*4);

	for (i=0; i<blocks; i++){
		unsigned short x,y;
		x=get_ushort(buf+i*4);
		y=get_ushort(buf+i*4+2);

		dump_tga(i, x, y, data, colors, pal);
		data+=x*y;
	}
}

void do_mode_2(unsigned short blocks, const char *buf, size_t len, unsigned char mode)
{
	unsigned long i;
	unsigned long data_sum=0;
	size_t calc_len;
	char *pal;
	unsigned char *data,*pdata;
	unsigned short colors,x,y;

	if (len < 4+blocks*4) {
		printf("The buffer is to small to hold valid values.\n");
		return;
	}

	x=get_ushort(buf);
	y=get_ushort(buf+2);

	printf("Pictures %03ux%03u\n", x, y);

	data_sum=4+4*blocks;
	for (i=0; i < blocks; i++)
		data_sum+=get_uint(buf+4+i*4);

	if (len < data_sum) {
		printf("The buffer is to small to hold valid values");
		return;
	}

	colors=get_ushort(buf+data_sum);
	calc_len=3+data_sum+2+3*colors;

	if (len+3 != calc_len) {
		printf("The filelen %lu is not as expected %lu\n",
				len+3, calc_len);
		return;
	}

	pal=(char*)(buf+data_sum+2);
	pdata=(unsigned char*)(buf+4+4*blocks);

	data=malloc(x*y);
	if (!data) {
		fprintf(stderr, "Not enough memory\n");
		return;
	}

	for (i=0; i<blocks; i++){
		unsigned long plen=get_uint(buf+4+i*4);

		memset(data, 0, x*y);

		if (mode == 2)
			ppdepack(pdata, data, plen, x*y);
		else
			un_rle(pdata, data, plen, x*y);

		dump_tga(i, x, y, (char*)data, colors, pal);
		pdata+=get_uint(buf+4+i*4);
	}

	free(data);
}

/* Packed Images with different sizes*/
void do_mode_3(unsigned short blocks, const char *buf, size_t len, unsigned char mode)
{
	unsigned long i;
	unsigned long data_sum=0;
	size_t calc_len;
	char *pal;
	unsigned char *data,*pdata;
	unsigned short colors;

	if (len < blocks*8) {
		printf("The buffer is to small to hold valid values.\n");
		return;
	}

	data_sum=8*blocks;

	for (i=0; i<blocks; i++) {
		unsigned short x,y;

		x=get_ushort(buf);
		y=get_ushort(buf+2);

		data_sum+=get_uint(buf+i*8+4);

		printf("Picture %03lu: %03ux%03u\n", i, x, y);
	}


	if (len < data_sum) {
		printf("The buffer is to small to hold valid values");
		return;
	}

	colors=get_ushort(buf+data_sum);
	calc_len=3+data_sum+2+3*colors;

	if (len+3 != calc_len) {
		printf("The filelen %lu is not as expected %lu\n",
				len+3, calc_len);
		return;
	}

	pal=(char*)(buf+data_sum+2);
	pdata=(unsigned char*)(buf+8*blocks);


	for (i=0; i<blocks; i++){
		unsigned long plen;
		unsigned short x,y;

		x=get_ushort(buf+i*8);
		y=get_ushort(buf+i*8+2);
		plen=get_uint(buf+i*8+4);

		data=malloc(x*y);
		if (!data) {
			fprintf(stderr, "Not enough memory\n");
			return;
		}

		memset(data, 0, x*y);
		if (mode == 3)
			ppdepack(pdata, data, plen, x*y);
		else
			un_rle(pdata, data, plen, x*y);

		dump_tga(i, x, y, (char*)data, colors, pal);
		pdata+=plen;
		free(data);
	}

}


void process_nvf(const char *buf, size_t len) {
	unsigned char mode;
	unsigned short blocks;

	mode=*(unsigned char*)(buf);

	if (mode >= 6) {
		fprintf(stderr, "No valid crunchmode %d\n", mode);
		return;
	}

	blocks=get_ushort(buf+1);
	printf("NVF-Mode: %u ", mode);
	switch (mode) {
		case 0: printf("(same size/unpacked)\n");
			do_mode_0(blocks, buf+3, len-3);
			break;
		case 1: printf("(different size/unpacked)\n");
			do_mode_1(blocks, buf+3, len-3);
			break;
		case 2: printf("(same size/PP20)\n");
			do_mode_2(blocks, buf+3, len-3, mode);
			break;
		case 3: printf("(different size/PP20)\n");
			do_mode_3(blocks, buf+3, len-3, mode);
			break;
		case 4: printf("(same size/RLE)\n");
			do_mode_2(blocks, buf+3, len-3, mode);
			break;
		case 5: printf("(different size/RLE)\n");
			do_mode_3(blocks, buf+3, len-3, mode);
			break;
		default:
			printf("is not supported\n");
	}
}

int main(int argc, char **argv) {

	FILE *fd=NULL;
	char *buf;
	size_t flen,readlen;

	if (argc == 1) {
		printf("More arguments expected.\n");
		exit(1);
	}

	fd=fopen(argv[1],"rb");
	if (!fd) {
		printf("Cant open file %s\n", argv[1]);
		exit(1);
	}

	fseek(fd, 0, SEEK_END);
	flen=ftell(fd);
	fseek(fd, 0, SEEK_SET);

	if (flen < 3 ) {
		printf("File %s is to small.\n", argv[1]);
		fclose(fd);
		exit(1);
	}

	buf=calloc(flen, sizeof(char));
	if (!buf) {
		printf("No Memory\n");
		fclose(fd);
		exit(1);
	}

	readlen=fread(buf, sizeof(char), flen, fd);
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
