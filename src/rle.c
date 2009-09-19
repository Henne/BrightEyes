/*
 * Header file for RLE decompressor of DSA/ROA
 *
 * Author: Henne_NWH <henne@nachtiwndheim.de>
 * License: GPLv3
 *
 */

#include <string.h>

unsigned long un_rle(const unsigned char *pdata, unsigned char *data,
		     unsigned long plen)
{
	unsigned long i, pos = 0;

	for (i = 0; i < plen; i++)
		if (pdata[i] == 0x7f) {
			unsigned char rl, col;

			rl = pdata[i + 1];
			col = pdata[i + 2];

			memset(data + pos, col, rl);
			i += 2;
			pos += rl;
		} else
			data[pos++] = pdata[i];

	return pos;
}
