/*
	Rewrite of DSA1 Generator v1.05_de seg003 (random)
	Functions rewritten: 4/4 (complete)
*/

#include <stdlib.h>

#include "port.h"
#include "symbols.h"

#include "g105de_seg003.h"

extern signed short g_random_gen_seed;
extern signed short g_random_gen_seed2;

#if !defined(__BORLANDC__)
static inline
unsigned short _rotl(unsigned short op, unsigned short count) {
	return (op << count) | (op >> (16 - count));
}
#endif

/**
	random_interval_gen - generates a u16 random number between lo and hi
*/
/* Borlandified and identical */
unsigned short random_interval_gen(unsigned short lo, unsigned short hi)
{
	return lo + random_gen(hi - lo + 1) - 1;
}

/**
	random_gen - generates a u16 random number
*/
/* Borlandified and nearly identical */
int random_gen(const int val)
{
	register Bit16s retval;

	if (val == 0) {
		return 0;
	}

	retval = g_random_gen_seed ^ g_random_gen_seed2;
	retval = _rotl(retval, 2);
	retval = (retval + g_random_gen_seed2) ^ g_random_gen_seed;
	g_random_gen_seed = retval = _rotl(retval, 3);

	/* update rand_seed */

	retval = (Bit32s)abs(retval) % val;
#if defined(__BORLANDC__)
	asm { db 0x90 }
#endif

	return ++retval;
}

/**
 * \brief   checks if val is in a word array
 */
/* Borlandified and identical */
int is_in_word_array(const int val, signed short *p)
{
	while (*p >= 0) {
		if (*p++ == val) {
			return 1;
		}
	}

	return 0;
}

/**
 * \brief   checks if val is in a byte array
 */
/* Borlandified and identical */
int is_in_byte_array(const signed char val, signed char *p)
{
	int i;

	for (i = 1; *p != -1; i++) {
		if (*p++ == val)
			return i;
	}

	return 0;
}
