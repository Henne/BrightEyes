/*
 *	Rewrite of DSA1 v3.02_de functions of seg007 (random, dice, min/max)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Y seg007.cpp
 */

#include <stdlib.h>

#include "v302de.h"

#include "seg007.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

unsigned short g_random_schick_seed = 0x327b; // ds:0x4ba0

/* REMARK: belong to seg010.cpp */
char g_emm_sig[8] = { 'E', 'M', 'M', 'X', 'X', 'X', 'X', '0'}; // ds:0x4ba2
uint8_t *g_ems_frame_ptr = NULL; // ds:0x4baa; uint8_t*

#if !defined(__BORLANDC__)
static inline
unsigned short _rotl(unsigned short op, unsigned char count) {
	return (op << count) | (op >> (16 - count));
}
#endif

/**
 * \brief   generates a random number in the range [lo .. hi]
 */
int random_interval(const int lo, const int hi)
{

	return lo + random_schick(hi - lo + 1) - 1;
}

/**
 * \brief   generates a u16 random number in the range [1 .. val]
 */
int random_schick(const int val)
{
	signed short retval;

	if (val == 0) {
		return 0;
	}

	/* rand_seed XOR rand_seed2 */
	retval = g_random_schick_seed ^ g_random_schick_seed2;
	retval = _rotl(retval, 2);		/* ROL retval */
	retval = (retval + g_random_schick_seed2) ^ g_random_schick_seed;
	retval = _rotl(retval, 3);

	/* update rand_seed */
	g_random_schick_seed = __abs__(retval) + 1;

	retval = __abs__(retval) % val;

	return ++retval;
}

/**
 * \brief   rolls a dice: n*Wm+x
 */
int dice_roll(const int n, const int m, const int x)
{
	int sum = 0;
	int i;

	for (i = 0; i < n; i++) {
		sum += random_schick(m);
	}

	sum += x;

	return sum;
}

/**
 * \brief   calculate min/max damage of a weapon with the damage formula n * Wm + x
 *
 * \param n	number of dice thrown
 * \param m	number of sides of the dice (outcome of a single dice roll is [1..m])
 * \param x	constant summand in the damage formula
 */
void calc_damage_range(const int n, const int m, const int x, signed int *min, signed int *max)
{
	*min = n + x;
	*max = n * m + x;
}

/**
 * \brief   checks if val is in a word array
 */
int is_in_word_array(const int val, signed short *p)
{
	int i;

	for (i = 1; *p >= 0; i++) {
		if (*(p++) == val)
			return i;
	}

	return 0;
}

/**
 * \brief   checks if val is in a byte array
 */
int is_in_byte_array(const signed char val, int8_t *p)
{
	int i;

	for (i = 1; *p != -1; i++) {
		if (*(p++) == val)
			return i;
	}

	return 0;
}

/**
 * \brief   rolls a dice from enemy templates
 */
int dice_template(const unsigned short val)
{
	signed short n;
	signed short m;
	signed char x;
	signed short i, sum = 0;

	/* get dice formula n*Wm+x */
	n = _rotl(val & 0xf000, 4);

	i =_rotl(val & 0x0f00, 8);

	m = (i == 1) ? 6 : ((i == 2) ? 20 : ((i == 3) ? 3 : 4));

	x = (signed char)val;

	/* roll the dices */
	for (i = 0; i < n; i++)
		sum += random_schick(m);

	sum += x;

	return sum;
}

/**
 * \brief   writes damage range from enemy templates to mem
 */
void damage_range_template(const unsigned short val, signed int *min, signed int *max)
{
	signed short n, m;
	signed char x;
	signed short i;
	signed short tmp = val;

	/* get dice formula n*Wm+x */
	n = _rotl(tmp & 0xf000, 4);

	i =_rotl(tmp & 0x0f00, 8);

	m = (i == 1) ? 6 : ((i == 2) ? 20 : ((i == 3) ? 3 : 4));

	x = (signed char)tmp;

	/* set vars to 0 */

	*min = *max = 0;

	for (i = 0; i < n; i++) {
		(*min)++;
		*max += m;
	}

	*min += x;
	*max += x;
}

#if !defined(__BORLANDC__)
}
#endif
