/*
 * Power Pack 2.0 decompressor for DSA/ROA
 */

static inline unsigned int val(const char *p)
{
	return (p[0] << 16 | p[1] << 8 | p[2]);
}

static inline unsigned int le32_2_cpu(const char *p)
{
	return (p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0]);
}

unsigned long depackedlen(const char *p, unsigned long plen)
{
/*	DSA1/ROA1 doesn't use the first bytes as a signature "PP20".
 *	It's used instead for the lenght of the packed data. */
	if (le32_2_cpu(p) == plen)
		return val(p + plen - 4);

	if (le32_2_cpu(p) + 8 == plen)
		return val(p + plen - 4);

	if (p[0] == 'P' || p[1] == 'P' || p[2] == '2' || p[3] == '0')
		return val(p + plen - 4);

	/* not a powerpacker file */
	return 0;
}

static unsigned long shift_in;
static unsigned long counter;
static const char *source;

static unsigned long get_bits(unsigned long n)
{

	unsigned long result = 0;
	int i;

	for (i = 0; i < n; i++) {
		if (counter == 0) {
			counter = 8;
			shift_in = *--source;
		}
		result = (result << 1) | (shift_in & 1);
		shift_in >>= 1;
		counter--;
	}
	return result;
}

void ppdepack(const char *packed, char *depacked,
	      unsigned long plen, unsigned long unplen)
{
	char *dest;
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
	counter = 0;

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
		bytes = idx + 1;
		if (bytes == 4) {	/* 4 means >=4 */
			/* and maybe a bigger offset */
			if (get_bits(1) == 0)
				offset = get_bits(7);
			else
				offset = get_bits(n_bits);

			do {
				to_add = get_bits(3);
				bytes += to_add;
			} while (to_add == 7);
		} else
			offset = get_bits(n_bits);

		for (i = 0; i <= bytes; i++) {
			dest[-1] = dest[offset];
			dest--;
		}

		if (dest <= depacked)
			return;
	}
}
