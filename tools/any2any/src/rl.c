/*
 * Header file for RLE decompressor of DSA2+3/ROA2+3
 *
 * Author: Henne_NWH <henne@nachtwindheim.de>
 * License: GPLv3
 *
 */

unsigned long un_rl(const char *pdata, char *data, unsigned long plen)
{
	unsigned long i = 0;
	char val, rl;

	while (i < plen) {
		val = *pdata++;
		if (val < 0) {
			rl = 1 - val;
			val = *pdata++;
			i++;

			do {
				*data++ = val;
			} while (--rl);
		} else {
			rl = val + 1;
			do {
				*data++ = *pdata++;
				i++;
			} while (--rl);
		}
		i++;
	}

	return 0;
}
