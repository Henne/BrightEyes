/*
 * Header file for RLE decompressor of DSA/ROA
 *
 * Author: Henne_NWH <henne@nachtwindheim.de>
 * License: GPLv3
 *
 */

unsigned long un_rle(const char *pdata, char *data, unsigned long plen)
{
	unsigned long i, pos = 0;

	for (i = 0; i < plen; i++)
		if (pdata[i] == 0x7f) {
			unsigned char rl, col, j;
			char *ptr;


			rl = pdata[i + 1];
			col = pdata[i + 2];
			ptr = data + pos;
			for (j = 0; j < rl; j++)
				*ptr++ = col;
			i += 2;
			pos += rl;
		} else
			data[pos++] = pdata[i];

	return pos;
}
