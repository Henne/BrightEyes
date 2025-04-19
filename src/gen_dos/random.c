/*
	Rewrite of DSA1 Generator v1.05_de seg003 (random)
	Functions rewritten: 4/4 (complete)
*/

#include <MATH.H> //abs()

#include "random.h"

extern signed short g_random_gen_seed;
extern signed short g_random_gen_seed2;

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
/* Borlandified and identical */
int random_gen(const int val)
{
	signed short retval;

	if (val == 0) {
		return 0;
	}

	retval = g_random_gen_seed ^ g_random_gen_seed2;
	retval = _rotl(retval, 2);
	retval = (retval + g_random_gen_seed2) ^ g_random_gen_seed;
	retval = _rotl(retval, 3);
	g_random_gen_seed = retval;

	retval = abs(retval) % val;

	return ++retval;
}

/**
 * \brief   checks if val is in a word array
 */
/* Borlandified and identical */
int is_in_word_array(const int val, const signed short *p)
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
