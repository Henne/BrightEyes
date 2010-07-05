/*
 * Header file for decompressing algorithms of DSA/ROA
 */

/* Power Pack 2.0 - Modified */

/* pp20.c */
void ppdepack(const unsigned char *, unsigned char *,
		unsigned long, unsigned long);

unsigned long depackedlen(const unsigned char*, unsigned long);

/* RLE */
unsigned long un_rle(const unsigned char *, unsigned char *, unsigned long);
/* RL */
unsigned long un_rl(const char *, char *, unsigned long);
