#include <stdio.h>
#include <string.h>
#include <stdlib.h> // randomize()
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>

#if defined(__BORLANDC__)
#include <IO.H>		// lseek, _read, _close, _creat, open, write
#include <DOS.H>
#include <BIOS.H>	// bioskey, int86x()
#include <CONIO.H>	// clrsrc()
#include <TIME.H>	// by randomize()
#include <ALLOC.H>	// farcalloc()
#else
#include <unistd.h> // lseek(), close(), read(), write()
#endif

/* portable Memory Access */
#if !defined(__BORLANDC__)
static inline unsigned short host_readw(unsigned char *p) { return *(unsigned short*)p; }
static inline signed short host_readws(unsigned char *p) { return *(signed short*)p; }
static inline unsigned int host_readd(unsigned char *p) { return *(unsigned int*)p; }
static inline signed int host_readds(unsigned char *p) { return *(signed int*)p; }
static inline unsigned short host_writew(unsigned char *p, unsigned short v) { return *(unsigned short*)p = v; }
// TODO: Check if that works on the stack of 64-bit machines
static inline unsigned int host_writed(unsigned char *p, unsigned int v) { return *(unsigned int*)p = v; }
#else
#define host_readw(p) (*(unsigned short*)(p))
#define host_readws(p) (*(signed short*)(p))
#define host_readd(p) (*(unsigned long*)(p))
#define host_readds(p) (*(signed long*)(p))
#define host_writew(p, v) (*(unsigned short*)(p) = (v))
#define host_writed(p, v) (*(unsigned long*)(p) = (v))
#endif

#if !defined(__BORLANDC__)
// DUMMY for BCC CLib func
static inline void clrscr(void) { }
static inline void randomize(void) { }
static inline signed short bc_flushall(void) { return 0; }
#define __abs__(v) abs(v)
#define _creat creat
#define _read read
#define _close close
#else
// <STDIO.H>
#define bc_flushall flushall
#endif

#include "hero.h"

#include "g105de_seg001.h"
#include "g105de_seg002.h"
#include "g105de_seg003.h"
#include "g105de_seg004.h"
#include "g105de_seg005.h"

#if defined(__BORLANDC__)
#include "AIL.H"
#else
#include "g105de_seg006.h"
#endif

#include "port.h"

/** Keyboard Constants */

#define KEY_ESC     (0x01)
#define KEY_1       (0x02)
#define KEY_2       (0x03)
#define KEY_3       (0x04)
#define KEY_4       (0x05)
#define KEY_5       (0x06)
#define KEY_RET     (0x1c)
#define KEY_J       (0x24)
#define KEY_Y       (0x2c)
#define KEY_N       (0x31)
#define KEY_UP      (0x48)
#define KEY_LEFT    (0x4b)
#define KEY_RIGHT   (0x4d)
#define KEY_DOWN    (0x50)
#define KEY_PGUP    (0x49)
#define KEY_PGDOWN  (0x51)
#define KEY_CTRL_F3 (0x60)
#define KEY_CTRL_F4 (0x61)


/**
	struct nvf_desc - nvf descriptor
	@src:   pointer to a buffer containing the nvf file
	@dst:   pointer where to extract the picture
	@no:    number of the picture to extract
	@type:  kind of compression / direction (0 = PP20 / 2-5 RLE / copy)
	@p_height:      pointer where the height of the picture must be stored
	@p_width:       pointer where the width of the picture must be stored
*/

struct struct_spelltab {
	signed char origin; /* {0 = Druid, 1 = Mage, 2 = Elven, 3 = Warlock } */
	signed char att1;
	signed char att2;
	signed char att3;
	signed char cost;
};

/* Remark: only the field origin is used from this table */
static const struct struct_spelltab g_spell_tab[87] = {
	{ 0, 0, 0, 0, 0},
	{ 0, 1, 5, 2, -1},
	{ 1, 1, 5, 3, -1},
	{ 1, 5, 2, 6, -1},
	{ 1, 1, 5, 2, -1},
	{ 3, 0, 1, 2, -1},
	{ 2, 1, 2, 6, 12},
	{ 2, 5, 2, 2, 8},
	{ 0, 0, 2, 2, 8},
	{ 3, 1, 5, 2, 8},
	{ 0, 1, 1, 2, -1},
	{ 0, 0, 0, 2, -1},
	{ 1, 0, 5, 2, 7},
	{ 0, 0, 1, 6, 6},
	{ 1, 1, 5, 2, 5},
	{ 3, 0, 2, 2, -1},
	{ 2, 1, 2, 2, 8},
	{ 0, 0, 1, 2, 6},
	{ 1, 0, 0, 2, 13},
	{ 0, 0, 0, 2, 13},
	{ 0, 0, 0, 2, 11},
	{ 1, 0, 0, 2, 23},
	{ 3, 0, 2, 2, 13},
	{ 1, 0, 0, 2, 15},
	{ 0, 0, 1, 2, -1},
	{ 1, 1, 6, 6, 5},
	{ 2, 5, 4, 6, -1},
	{ 2, 1, 4, 6, 7},
	{ 1, 1, 1, 3, -1},
	{ 1, 1, 3, 6, -1},
	{ 2, 5, 4, 4, 7},
	{ 1, 1, 1, 6, -1},
	{ 2, 1, 4, 6, 5},
	{ 2, 1, 5, 2, -1},
	{ 3, 5, 2, 3, -1},
	{ 1, 1, 1, 2, -1},
	{ 2, 1, 2, 6, 7},
	{ 3, 0, 5, 2, -1},
	{ 2, 1, 5, 3, 5},
	{ 1, 1, 1, 5, 10},
	{ 2, 1, 5, 2, 7},
	{ 2, 1, 1, 5, 5},
	{ 2, 1, 5, 2, 5},
	{ 1, 1, 1, 6, -1},
	{ 2, 1, 5, 2, 5},
	{ 2, 5, 2, 4, 5},
	{ 1, 1, 2, 4, 8},
	{ 3, 1, 2, 4, 6},
	{ 3, 1, 5, 2, 4},
	{ 2, 1, 5, 4, 5},
	{ 1, 0, 1, 6, 5},
	{ 1, 1, 2, 4, 5},
	{ 2, 1, 4, 6, -1},
	{ 1, 1, 4, 3, -1},
	{ 1, 2, 4, 6, 5},
	{ 3, 0, 5, 2, 4},
	{ 1, 0, 5, 2, -1},
	{ 2, 5, 4, 3, 5},
	{ 3, 5, 5, 2, 2},
	{ 1, 0, 1, 2, 11},
	{ 2, 0, 5, 4, 7},
	{ 1, 0, 1, 2, 7},
	{ 2, 5, 4, 6, -1},
	{ 1, 1, 2, 2, 7},
	{ 3, 0, 0, 6, 7},
	{ 2, 1, 2, 3, 7},
	{ 2, 1, 2, 4, 7},
	{ 2, 1, 5, 2, 7},
	{ 2, 1, 2, 6, 7},
	{ 1, 1, 1, 2, 7},
	{ 2, 0, 1, 2, 7},
	{ 1, 1, 4, 6, -1},
	{ 1, 5, 2, 6, 13},
	{ 1, 1, 5, 2, -1},
	{ 2, 0, 1, 6, -1},
	{ 2, 1, 1, 4, -1},
	{ 2, 1, 1, 3, 5},
	{ 2, 1, 2, 6, 5},
	{ 1, 0, 1, 6, -1},
	{ 1, 1, 3, 6, -1},
	{ 0, 1, 1, 3, 3},
	{ 1, 0, 1, 6, -1},
	{ 2, 1, 1, 3, 1},
	{ 1, 0, 1, 6, -1},
	{ 2, 1, 1, 2, -1},
	{ 2, 1, 2, 6, 5},
	{ -1, 0, 0, 0, 0},
};

/* Antimagie */
static const signed short house00[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x2a, -1};
/* Beherrschung */
static const signed short house01[] = {0x07, 0x0c, 0x0e, 0x10, 0x2c, -1};
/* Beschwoerung */
static const signed short house02[] = {0x12, 0x13, 0x15, 0x17, 0x18, 0x3b, -1};
/* Bewegung */
static const signed short house03[] = {0x1b, 0x1c, 0x1d, 0x1e, 0x1f, -1};
/* Heilung */
static const signed short house04[] = {0x21, 0x23, 0x24, 0x3d, 0x46, -1};
/* Hellsicht */
static const signed short house05[] = {0x26, 0x27, 0x29, 0x28, 0x2a, 0x2b, 0x2c, -1};
/* Kampf */
static const signed short house06[] = {0x31, 0x33, 0x34, 0x35, 0x36, 0x39, 0x3e, -1};
/* Verwandlung */
static const signed short house07[] = {0x3c, 0x3e, 0x48, 0x49, 0x4b, -1};
/* Veraenderung */
static const signed short house08[] = {0x4c, 0x4e, 0x4f, 0x50, 0x52, 0x53, 0x54, -1};

static const signed short* g_house_spells[] = {
	house00, house01, house02,
	house03, house04, house05,
	house06, house07, house08
};



struct struct_school_tab {
	signed short first_spell;
	signed short spells;
};

static const struct struct_school_tab g_school_tab[] = {
	{0x01, 0x05},
	{0x06, 0x0c},
	{0x12, 0x09},
	{0x1b, 0x06},
	{0x21, 0x05},
	{0x26, 0x07},
	{0x31, 0x09},
	{0x3c, 0x10},
	{0x4c, 0x0a},
};

struct struct_reqs {
	unsigned char attrib;
	unsigned char value;
};

static const struct struct_reqs g_reqs[13][4] = {
	{ {0, 0 }, {0,  0}, {0,  0}, {0, 0} },	//DUMMY
	{ {0, 12}, {4, 12}, {3, 12}, {7, 7}, },
	{ {5, 12}, {4, 12}, {9,  7}, {2, 1}, },
	{ {0, 13}, {6, 12}, {13, 0x80 | 4}, {2, 1}, },
	{ {0, 12}, {4, 13}, {3, 13}, {2, 1}, },
	{ {0, 12}, {6, 13}, {7,  7}, {2, 1}, },
	{ {6, 13}, {3, 12}, {10, 7}, {9, 0x80 | 4}, },

	{ {5, 12}, {2, 13}, {8, 0x80 | 4}, {6, 1}, },
	{ {0, 13}, {1, 12}, {11, 0x80 |4}, {2, 1}, },
	{ {1, 13}, {2, 12}, {7, 0x80 | 4}, {6, 1}, },

	{ {1, 13}, {4, 12}, {10, 0x80 | 4}, {2, 1}, },
	{ {5, 12}, {4, 13}, {10, 0x80 | 4}, {2, 1}, },
	{ {5, 13}, {4, 13}, {10, 0x80 | 4}, {2, 1}, },
};

static const signed char g_skills[13][52] = {
	/* DUMMY */
	{ 0},
	/* Gaukler/ Jester*/
	{
		/* Kampf / Combat */
		4, 1, 3, 1, 0, 1, -2, 0, 3,
		/* Koerper / Body */
		5, 5, 5, 3, 4, 1, 0, 5, 4, 2,
		/* Gesellschaft / Social */
		-4, 2, 1, 3, 4, 2, 2,
		/* Natur / Nature */
		-3, 5, -1, -3, -2, -3,
		/* Handwerk / Craftsmanship */
		0, -4, 3, -4, 1, -6, -2, -2, 2,
		/* Wissen / Lore */
		3, 0, 4, 0, 0, 2, 4, 2, 2,
		/* Intuition */
		1, 1
	},
	/* Jaeger/ Hunter*/
	{
		/* Kampf / Combat */
		3, 2, 1, 1, 1, 4, -4, 5, 3,
		/* Koerper / Body */
		-1, 2, 3, 1, 6, 4, 4, -1, 5, 3,
		/* Gesellschaft / Social */
		-3, -3, -1, -3, 0, 0, 0,
		/* Natur / Nature */
		5, 4, 4, 4, 6, 5,
		/* Handwerk / Craftsmanship */
		-5, -3, 2, -4, 0, -4, -1, -3, 1,
		/* Wissen / Lore */
		2, -1, 1, 1, 1, 3, -1, -1, -2,
		/* Intuition */
		3, 3
	},
	/* Krieger/ Warrior*/
	{
		/* Kampf / Combat */
		4, 4, 1, 6, 2, 3, 4, 3, 3,
		/* Koerper / Body */
		0, 1, 3, 4, 0, 2, 4, 0, -1, 3,
		/* Gesellschaft / Social */
		-4, -2, -1, 0, 0, 0, 0,
		/* Natur / Nature */
		-1, -2, 1, -3, -2, 2,
		/* Handwerk / Craftsmanship */
		-5, -1, 1, 2, 3, 3, 2, 0, 0,
		/* Wissen / Lore */
		-2, 0, 0, -1, 0, 2, 0,	0, -2,
		/* Intuition */
		0, 0
	},
	/* Streuner/ Rogue*/
	{
		/* Kampf / Combat */
		4, 2, 4, 1, 1, 1, -2, -2, 3,
		/* Koerper / Body */
		1, 3, 4, 1, 4, 0, 1, 2, 4, 3,
		/* Gesellschaft / Social */
		2, 4, 3, 7, 5, 3, 3,
		/* Natur / Nature */
		-2, 2, 0, -2, -2, -2,
		/* Handwerk / Craftsmanship */
		-2, -2, 0, -3, 2, -4, -1, -2, 2,
		/* Wissen / Lore */
		-3, 0, 6, 3, 1, 3, 1, 4, 5,
		/* Intuition */
		3, 2
	},
	/* Thorwaler/ Thorwalian*/
	{
		/* Kampf / Combat */
		4, 3, 0, 1, 4, 2, 2, -3, 2,
		/* Koerper / Body */
		-1, 3, 2, -1, -1, 5, 4, 0, -2, 6,
		/* Gesellschaft / Social */
		-4, 0, -2, -2, -1, 0, 0,
		/* Natur / Nature */
		-5, 3, 4, -4, -2, -2,
		/* Handwerk / Craftsmanship */
		-4, -1, 2, 0, 2, 0, 1, -3, 1,
		/* Wissen / Lore */
		-4, -2, 0, -1, 1, 2, 1,	0, -2,
		/* Intuition */
		-3, 2
	},
	/* Zwerg/ Dwarf*/
	{
		/* Kampf / Combat */
		3, 3, 0, 2, 4, 3, 1, 2, 2,
		/* Koerper / Body */
		-2, 4, 1, -5, 0, -5, 5, -4, 3, 5,
		/* Gesellschaft / Social */
		-5, -2, 0, -4, -2, -3, 5,
		/* Natur / Nature */
		0, 2, 1, 0, -3, 0,
		/* Handwerk / Craftsmanship */
		2, 2, -2, 3, 0, 1, 1, 0, -2,
		/* Wissen / Lore */
		-3, 0, 0, 2, 3, 3, -1, 3, 0,
		/* Intuition */
		2, 4
	},
	/* Hexer/ Warlock*/
	{
		/* Kampf / Combat */
		1, 0, 2, -5, -5, 2, -6, 0, 2,
		/* Koerper / Body */
		-2, 0, 2, -1, 2, 1, 4, 4, 2, 1,
		/* Gesellschaft / Social */
		-2, 4, 2, -2, 3, 2, -1,
		/* Natur / Nature */
		2, 2, 2, 5, 3, 2,
		/* Handwerk / Craftsmanship */
		0, -2, 0, 0, 1, -6, 0, 2, 0,
		/* Wissen / Lore */
		2, -2, 3, 4, 3, 1, 2, 0, -2,
		/* Intuition */
		2, 3
	},
	/* Druide/ Druid*/
	{
		/* Kampf / Combat */
		1, -3, 0, -7, -6, 2, -7, 1, 1,
		/* Koerper / Body */
		-2, 2, 0, -1, 3, 1, 4, -2, 0, 1,
		/* Gesellschaft / Social */
		-7, -3, 0, -2, 2, 1, 0,
		/* Natur / Nature */
		2, 2, 2, 5, 4, 3,
		/* Handwerk / Craftsmanship */
		0, -4, 1, 2, 1, -4, 0, 4, 0,
		/* Wissen / Lore */
		2, -2, -1, 3, 4, 2, 0,-2, -4,
		/* Intuition */
		2, 3
	},
	/* Magier/ Magician*/
	{
		/* Kampf / Combat */
		1, 0, 2, -4, -5, 3, -5, -3, 0,
		/* Koerper / Body */
		-3, 1, 0, 1, 0, 0, 2, -1, 0, 1,
		/* Gesellschaft / Social */
		-2, 0, 1, 0, 3, 2, 1,
		/* Natur / Nature */
		-5, 0, -2, 3, -2, -4,
		/* Handwerk / Craftsmanship */
		6, 4, 0, 3, 3, -4, 6, 7, 6,
		/* Wissen / Lore */
		-2, 0, 0, 2, 2, 2, -2, 0, -2,
		/* Intuition */
		1, 3
	},
	/* Auelf/ Green Elf*/
	{
		/* Kampf / Combat */
		1, 2, 3, 3, -1, 2, -1, 4, 2,
		/* Koerper / Body */
		1, 2, 2, 2, 3, 2, 2, 4, 3, -1,
		/* Gesellschaft / Social */
		-1, 3, -1, -3, 3, 0, -2,
		/* Natur / Nature */
		3, 1, 2, 2, 2, 2,
		/* Handwerk / Craftsmanship */
		-5, 2, -1, 1, 0, -5, 1, 3, 2,
		/* Wissen / Lore */
		1, 0, 0, 2, 4, 3, 4, -1, -2,
		/* Intuition */
		1, 4
	},
	/* Firnelf/ Ice Elf*/
	{
		/* Kampf / Combat */
		3, 2, 2, 2, -2, 4, -5, 2, 4,
		/* Koerper / Body */
		0, 0, 2, -2, 5, 0, 3, 3, 5, 4,
		/* Gesellschaft / Social */
		-3, 2, -3, -7, 3, -2, -2,
		/* Natur / Nature */
		2, 0, 5, -2, 1, 3,
		/* Handwerk / Craftsmanship */
		-7, 2, -4, -2, -2, -5, 0, 1, 0,
		/* Wissen / Lore */
		1, 2, 0, 0, 2, 4, 3, -5, -4,
		/* Intuition */
		2, 6
	},
	/* Waldelf/ Silvan Elf*/
	{
		/* Kampf / Combat */
		2, 3, 3, 3, -1, 3, -3, 4, 2,
		/* Koerper / Body */
		2, 5, 3, 0, 4, -2, 2, 4, 5, -2,
		/* Gesellschaft / Social */
		-1, 2, -2, -6, 3, -1, -2,
		/* Natur / Nature */
		4, 1, 3, 4, 2, 4,
		/* Handwerk / Craftsmanship */
		-6, 2, -2, -1, -1, -5, 0, 2, 1,
		/* Wissen / Lore */
		2, -2, 0, 3, 4, 3, 3, -3, -3,
		/* Intuition */
		2, 5
	}
};

static const signed char g_spells[6][86] = {
	/* Hexer/ Warlock*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		-4, -10, -9, -7, -5,
		/* Beherrschung / Domination */
		-6, -10, -6, 1, -8, -8, -10, -15, -6, 2, -6, -12,
		/* Daemonologie / Demonology */
		-12, -7, -5, -10, 2, -15,
		/* Elemente / Elements */
		-18, -15, -10,
		/* Bewegung /Movement */
		-8, -6, -6, -5, -18, -15,
		/* Heilung / Healing */
		-8, 5, -8, -10, 3,
		/* Hellsicht / Clairvoyance */
		-4, -10, -4, 0, -1, -8, -5,
		/* Illusion */
		-7, -6, 2, 0,
		/* Kampf / Fight */
		0, -12, -8, -10, -10, -10, 2, -15, -8,
		/* Verstaendigung / Communication */
		5, -8,
		/* Verwandlung / Transformation */
		-7, -3, -8, 0, 2, 0, -3, 1, -3, -1, -3, -19, -15, -12, -10, -10,
		/* Veraenderung / Transmutation */
		-4, -4, -10, -8, -5, -10, 0, -10, -6, -12, },
	/* Druide/ Druid*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		2, -10, -8, -9, -12,
		/* Beherrschung / Domination */
		-6, -10, 3, -6, 2, 2, -6, -4, -4, -8, -8, 1,
		/* Daemonologie / Demonology */
		-10, -3, -2, -12, -8, -12,
		/* Elemente / Elements */
		-15, -15, -12,
		/* Bewegung /Movement */
		-10, -8, -12, -5, -19, -15,
		/* Heilung / Healing */
		-8, -19, -6, -10, -8,
		/* Hellsicht / Clairvoyance */
		-4, -10, -10, -4, -3, -8, -8,
		/* Illusion */
		-7, -6, -6, -10,
		/* Kampf / Fight */
		1, -8, -3, -8, -12, -7, -19, -10, -8,
		/* Verstaendigung / Communication */
		-19, -6,
		/* Verwandlung / Transformation */
		-8, -8, -8, -5, -6, 0, -3, 0, -3, -3, -1, -18, -15, -15, -10, -12,
		/* Veraenderung / Transmutation */
		-6, -4, -8, -8, -1, -8, 0, -8, -6, -15, },
	/* Magier/ Magician*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		-2, -6, -1, -5, -8,
		/* Beherrschung / Domination */
		-6, 0, -6, -9, -12, -10, -2, -10, -2, -10, -3, -15,
		/* Daemonologie / Demonology */
		-5, -7, -5, -8, -12, -6,
		/* Elemente / Elements */
		-18, -10, -10,
		/* Bewegung /Movement */
		-6, 0, -2, -10, -10, -15,
		/* Heilung / Healing */
		2, -19, -2, -10, -10,
		/* Hellsicht / Clairvoyance */
		-7, -4, -4, -10, 3, 0, 0,
		/* Illusion */
		-10, 0, -6, -15,
		/* Kampf / Fight */
		2, -6, -2, 0, -3, -1, -19, -3, -4,
		/* Verstaendigung / Communication */
		-19, -6,
		/* Verwandlung / Transformation */
		-10, -2, 0, 0, -10, 0, 0, 0, -1, 0, -1, -15, -5, -10, -15, -4,
		/* Veraenderung / Transmutation */
		-2, 0, -7, 0, -3, -6, 3, -6, -6, -15, },
	/* Auelf/ Green Elf*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		0, -12, -10, -7, -10,
		/* Beherrschung / Domination */
		-4, 4, -10, -9, -10, -5, -12, -15, -6, -4, -1, -15,
		/* Daemonologie / Demonology */
		-12, -8, -6, -15, -12, -19,
		/* Elemente / Elements */
		-19, -13, -8,
		/* Bewegung /Movement */
		-2, -6, -6, -2, -15, -7,
		/* Heilung / Healing */
		5, -19, -4, -3, -2,
		/* Hellsicht / Clairvoyance */
		2, -10, 2, 1, 3, -2, 2,
		/* Illusion */
		-1, -8, -6, -15,
		/* Kampf / Fight */
		4, -10, -4, 4, -10, -7, -19, -8, -2,
		/* Verstaendigung / Communication */
		-19, -10,
		/* Verwandlung / Transformation */
		-6, -4, 4, -1, -8, 2, 0, 1, -1, -1, 0, -19, -15, -15, -2, 2,
		/* Veraenderung / Transmutation */
		-2, 1, -10, -5, -5, -10, 5, -10, 0, -6, },
	/* Firnelf/ Ice Elf*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		-2, -16, -11, -9, -12,
		/* Beherrschung / Domination */
		-2, 0, -10, -9, -10, -7, -12, -15, -6, -4, 0, -15,
		/* Daemonologie / Demonology */
		-15, -10, -8, -19, -12, -19,
		/* Elemente / Elements */
		-19, -12, -4,
		/* Bewegung /Movement */
		-2, -8, -4, -2, -12, -2,
		/* Heilung / Healing */
		2, -19, -4, 1, -2,
		/* Hellsicht / Clairvoyance */
		1, -10, -2, 2, -1, -2, 1,
		/* Illusion */
		0, -8, -8, -15,
		/* Kampf / Fight */
		4, -12, -6, 0, -10, -7, -19, -6, -1,
		/* Verstaendigung / Communication */
		-19, -8,
		/* Verwandlung / Transformation */
		-5, -4, 2, -1, -12, 1, 0, 1, 0, -1, 1, -19, -15, -15, -4, 1,
		/* Veraenderung / Transmutation */
		-4, 3, -10, -7, -7, -10, 6, -10, -4, -2, },
	/* Waldelf/ Silvan Elf*/
	{
		/* DUMMY */
		0,
		/* Antimagie / Dispel */
		-1, -14, -10, -8, -11,
		/* Beherrschung / Domination */
		0, 2, -10, -9, -10, -5, -12, -15, -6, -2, 1, -15,
		/* Daemonologie / Demonology */
		-15, -9, -7, -19, -12, -19,
		/* Elemente / Elements */
		-19, -14, -8,
		/* Bewegung /Movement */
		2, -7, -5, -1, -15, -7,
		/* Heilung / Healing */
		4, -19, -4, -1, 0,
		/* Hellsicht / Clairvoyance */
		1, -10, 0, 3, 1, -4, -1,
		/* Illusion */
		2, -8, -7, -15,
		/* Kampf / Fight */
		5, -11, -5, 2, -10, -7, -19, -7, 0,
		/* Verstaendigung / Communication */
		-19, -9,
		/* Verwandlung / Transformation */
		-4, -4, 3, -1, -8, 1, 0, 2, -1, -1, 0, -19, -15, -15, -4, 1,
		/* Veraenderung / Transmutation */
		0, 1, -10, -6, -6, -10, 5, -10, -2, -6, },
};

static const signed short g_init_le[] = {0, 30, 30, 30, 30, 30, 40, 25, 30, 25, 25, 30, 25};

static const signed short g_init_ae[] = {0, 0, 0, 0, 0, 0, 0, 25, 25, 30, 25, 25, 25};

struct minmax {
	unsigned char min;
	unsigned char max;
};

static const struct minmax g_height_range[] = {
	{0, 0},
	{150, 188},
	{155, 170},
	{165, 205},
	{156, 194},
	{167, 210},
	{130, 140},
	{154, 188},
	{154, 188},
	{164, 197},
	{170, 204},
	{160, 194},
	{170, 210}
};

static const unsigned char g_weight_mod[] = {
	0,
	120, 110, 100, 110, 100, 90,
	120, 110, 110, 120, 120, 120
};

static const signed char g_mr_mod[] = {
	0,
	2, 0, 0, 2, -2, 2,
	2, 2, 2, 3, 4, 3
};


struct struct_money {
	signed short value;
	signed short min;
	signed short max;
};

static const struct struct_money money_jester[] = {
	{14, 5, 5}, {19, 100, 100}, {20, 300, 300} };
static const struct struct_money money_hunter[] = {
	{1, 1, 1},  {20, 0, 0} };
static const struct struct_money money_warrior[] = {
	{2, 1, 1},  {4, 20, 20}, {7, 50, 50}, {10, 30, 30}, {18, 200, 200}, {20, 1000, 1000} };
static const struct struct_money money_rouge[] = {
	{10, 0, 0},  {17, 2, 2}, {18, 5, 5}, {20, 10, 10} };
static const struct struct_money money_thorwal[] = {
	{9, 10, 10},  {13, 0x64, 0x64}, {19, 300, 300}, {20, 508, 508} };
static const struct struct_money money_dwarf[] = {
	{7, 100, 100},  {16, 200, 200}, {19, 300, 300}, {20, 1200, 1200} };

static const struct struct_money money_warlock[] = {
	{20, 1, 20} };
static const struct struct_money money_druid[] = {
	{14, 0, 1},  {15, 1, 6}, {16, 2, 12}, {18, 1, 20}, {19, 10, 60}, {20, 10, 200} };
static const struct struct_money money_mage[] = {
	{3, 1, 6},  {13, 10, 60}, {16, 10, 200}, {19, 20, 400}, {20, 5, 1000} };
static const struct struct_money money_greenelf[] = {
	{4, 1, 6},  {6, 1, 20}, {13, 10, 60}, {19, 10, 200}, {20, 20, 400} };
static const struct struct_money money_iceelf[] = {
	{1, 1, 6},  {20, 1, 20} };
static const struct struct_money money_silvanelf[] = {
	{14, 1, 6}, {19, 1, 20}, {20, 10, 60} };

static const struct struct_money* g_money_tab[] = {
	money_jester,
	money_hunter,
	money_warrior,
	money_rouge,
	money_thorwal,
	money_dwarf,
	money_warlock,
	money_druid,
	money_mage,
	money_greenelf,
	money_iceelf,
	money_silvanelf
};

static const unsigned char g_initial_skill_incs[] = {
	20, 20, 20, 20, 20, 20, 20, 20, 15, 20, 20, 20
};

static const signed char g_initial_spell_incs[] = {
	25, 25, 40, 20, 20, 20
};

static const signed char g_initial_conv_incs[] = {
	5, 5, 5, 0, 0, 0
};

struct struct_house_mod {
	signed char no;
	signed short spells[7];
	signed short mod[7];
};

static const struct struct_house_mod g_house_mod[9] = {
	{6, {0x1, 0x2, 0x3, 0x4, 0x5, 0x2a, 0x0}, {3, 1, 2, 2, 3, 1, 0}},
	{5, {0x7, 0xc, 0xe, 0x10, 0x2c, 0x0, 0x0}, {3, 4, 2, 2, 1, 0, 0}},
	{6, {0x12, 0x13, 0x15, 0x17, 0x18, 0x3b, 0x0}, {3, 2, 2, 2, 2, 1, 0}},
	{5, {0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x0, 0x0}, {3, 3, 2, 2, 2, 0, 0}},
	{5, {0x21, 0x23, 0x24, 0x3d, 0x46, 0x0, 0x0}, {3, 3, 3, 2, 1, 0, 0}},
	{7, {0x26, 0x27, 0x29, 0x28, 0x2a, 0x2b, 0x2c}, {2, 2, 1, 1, 2, 2, 2}},
	{7, {0x31, 0x33, 0x34, 0x35, 0x36, 0x39, 0x3e}, {2, 1, 2, 2, 2, 2, 1}},
	{5, {0x3c, 0x3e, 0x48, 0x49, 0x4b, 0x0, 0x0}, {3, 2, 2, 3, 2, 0, 0}},
	{7, {0x4c, 0x4e, 0x4f, 0x50, 0x52, 0x53, 0x54}, {2, 1, 2, 2, 2, 1, 2}}
};

static const signed short g_autoskills[][25] = {
	{0 },
	{9, 11, 2, 47, 0, 8, 16, 9, 10, 11, 43, 34, 40,
		13, 20, 24, 22, 46, 50, 15, 47, 12, 27, 23, 49},
	{7, 5, 26, 26, 0, 30, 31, 28, 29, 13, 11, 17,
		15, 10, 1, 27, 31, 30, 51, 50, 46, 41, 13, 14, 28},
	{3, 6, 0, 1, 15, 11, 4, 7, 12, 46, 30, 37, 31,
		5, 28, 8, 50, 51, 15, 2, 35, 36, 12, 46, 10},
	{2, 13, 8, 49, 0, 22, 17, 20, 13, 10, 48, 22,
		11, 21, 49, 50, 51, 17, 10, 46, 24, 18, 48, 22, 12},
	{4, 1, 0, 10, 15, 14, 18, 28, 11, 3, 8, 51,
		10, 14, 18, 35, 25, 27, 34, 40, 46, 47, 50, 20, 24},
	{1, 18, 0, 10, 25, 15, 7, 10, 25, 21, 51, 8,
		21, 32, 35, 33, 43, 46, 13, 44, 50, 18, 17, 42, 40},
	{2, 46, 45, 0, 8, 29, 32, 46, 44, 20, 24, 13,
		39, 5, 51, 50, 17, 31, 45, 20, 29, 30, 23, 44, 11},
	{2, 29, 1, 39, 0, 32, 23, 29, 30, 31, 26, 15,
		46, 51, 45, 44, 32, 13, 14, 35, 33, 41, 28, 45, 27},
	{5, 39, 2, 32, 33, 39, 32, 29, 40, 8, 35, 44,
		51, 46, 50, 36, 25, 33, 29, 38, 40, 13, 44, 23, 30},
	{7, 2, 3, 47, 31, 13, 20, 30, 29, 11, 14, 46,
		16, 0, 51, 23, 17, 47, 31, 51, 39, 40, 33, 20, 30},
	{7, 2, 31, 26, 0, 26, 31, 30, 29, 47, 51, 5,
		13, 15, 11, 10, 14, 30, 29, 46, 11, 10, 13, 20, 17},
	{1, 26, 31, 8, 2, 31, 0, 30, 26, 15, 11, 47,
		13, 10, 14, 28, 46, 15, 17, 7, 27, 11, 13, 28, 41}
};

static signed short g_autospells[][45] = {
	{5, 9, 15, 22, 34, 37, 47, 48, 55, 58, 64, 42, 49, 5, 9, 15,
	22, 34, 37, 47, 48, 55, 58, 64,	5, 9, 15, 22, 34, 37, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, },

	{1, 8, 10, 11, 13, 17, 19, 20, 80, 24, 42, 5, 41, 1, 8, 10,
	11, 13, 17, 19, 20, 80, 24, 1, 8, 10, 11, 13, 17, 19, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, },

	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24,	25, 26, 27, 28, 29, 30, 31, 32,
	33, 34, 35, 36, 37, 38, 39, 40,	41, 42, 43, 44, 45, },

	{7, 33, 38, 40, 42, 44, 52, 84,	62, 74, 75, 82, 49, 27, 7, 33,
	38, 40, 42, 44, 52, 84, 62, 74,	75, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, },

	{32, 77, 82, 26, 85, 70, 68, 67, 65, 45, 49, 38, 32, 77, 82, 26,
	85, 70, 68, 67, 65, 32, 77, 82,	26, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },

	{6, 16, 27, 30, 41, 45, 49, 57,	76, 60, 62, 52, 38, 6, 16, 27,
	30, 41, 45, 49, 57, 76, 60, 6,	16, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, 0, 0, }
};

static const signed char g_head_first_male[] = { 0,
	0, 6, 12, 18, 24, 30, 36, 42, 48, 54, 62 };
static const signed char g_head_first_female[] = { 0,
	3, 9, 15, 21, 27, 34, 37, 46, 51, 58 };

struct struct_attrib_coords {
	signed short x;
	signed short y;
};

static const struct struct_attrib_coords g_attrib_coords[] = {
	{205, 73}, {205, 85}, {205, 97}, {205, 109}, {205, 121}, {205, 133}, {205, 145},
	{273, 73}, {273, 85}, {273, 97}, {273, 109}, {273, 121}, {273, 133}, {273, 145}
};

static const signed short dummy1 = 0;
static const signed char dummy2 = 15;

// Remark: g_mask_switch is read only
static const signed char g_mask_switch = 0;

static const signed char dummy3[160] = {0};

struct struct_color {
	signed char r;
	signed char g;
	signed char b;
};

static struct struct_color g_pal_tmp[32] = {0};

static const struct struct_color g_pal_genbg[32] = {
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
	{0x3c, 0x3c, 0x3c},
	{0x3c, 0x34, 0x18},
	{0x3c, 0x34, 0x08},
	{0x3c, 0x34, 0x00},
	{0x38, 0x30, 0x00},
	{0x34, 0x2c, 0x00},
	{0x30, 0x28, 0x00},
	{0x2c, 0x24, 0x00},
	{0x28, 0x20, 0x00},
	{0x24, 0x1c, 0x00},
	{0x20, 0x18, 0x00},
	{0x1c, 0x14, 0x00},
	{0x38, 0x14, 0x0c},
	{0x34, 0x0c, 0x00},
	{0x2c, 0x08, 0x00},
	{0x24, 0x04, 0x00},
	{0x1c, 0x00, 0x00},
	{0x14, 0x00, 0x00},
};

static const signed short dummy4 = 50;
static const signed char dummy5 = -1;


static signed short g_screen_var = 0;

static unsigned short g_mouse_mask[32] = {
        0x7fff, 0x9fff, 0x87ff, 0xc1ff,
        0xc07f, 0xe01f, 0xe007, 0xf00f,
        0xf01f, 0xf80f, 0xf887, 0xfdc3,
        0xffe3, 0xfff7, 0xffff, 0xffff,
        0x8000, 0x6000, 0x7800, 0x3e00,
        0x3f80, 0x1fe0, 0x1ff8, 0x0ff0,
        0x0fe0, 0x07f0, 0x0778, 0x023c,
        0x001c, 0x0008, 0x0000, 0x0000,
};

static signed short g_mouse_posy_min = 0;
static signed short g_mouse_posx_min = 0;
static signed short g_mouse_posy_max = 0xc7;
static signed short g_mouse_posx_max = 0x136;
static signed short g_mouse_locked = 0;
static signed short g_mouse_refresh_flag = -1;

static signed short g_mouse_posx = 0xa0;
static signed short g_mouse_posy = 0x64;
static signed short g_mouse_posx_bak = 0xa0;
static signed short g_mouse_posy_bak = 0x64;

static signed short g_mouse_moved = 0;

static signed short g_mouse_pointer_offsetx = 0;
static signed short g_mouse_pointer_offsety = 0;
static signed short g_mouse_pointer_offsetx_bak = 0;
static signed short g_mouse_pointer_offsety_bak = 0;

static const struct mouse_action g_action_default[2] = {
	{ 0, 0, 319, 199, 0xfe},
	{ -1, -1, -1, -1, -1}
};

static const struct mouse_action* g_default_action = &g_action_default[0];

static struct mouse_action* g_action_table = NULL;

static const struct mouse_action g_action_base[9] = {
	{ 272, 8, 304, 41, 0xfd},	/* credits */
	{ 305, 7, 319, 21, KEY_CTRL_F3},	/* change sex */
	{ 145, 13, 175, 21, KEY_CTRL_F4},	/* enter name */
	{ 271, 42, 286, 56, KEY_UP},	/* previous head */
	{ 288, 42, 303, 56, KEY_DOWN},	/* next head */
	{ 145, 178, 164, 192, KEY_LEFT},	/* previous page */
	{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
	{ 0, 0, 319, 199, 0xfe},
	{ -1, -1, -1, -1, -1}
};

static const struct mouse_action g_action_skills[4] = {
	{ 145, 178, 164, 192, KEY_LEFT},	/* previous page */
	{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
	{ 0, 0, 319, 199, 0xfe},
	{ -1, -1, -1, -1, -1}
};

static const struct mouse_action g_action_spells[4] = {
	{ 16, 178, 35, 192, KEY_LEFT},		/* previous page */
	{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
	{ 0, 0, 319, 199, 0xfe},
	{ -1, -1, -1, -1, -1}
};

static signed short g_gen_page = 0;
static signed char g_useless_variable = 0;
static signed short g_text_x_mod = 0;
static const signed short g_ro_zero = 0;

static const signed char dummy6 = 0;

static struct struct_hero g_hero = {0};

static const signed char dummy_6_1 = 0;

static signed short g_midi_disabled = 0;
static signed short g_use_cda = 0;
static signed short g_mouse_handler_installed = 0;

static signed short dummy7 = -1;
static signed short dummy8 = 0;

static unsigned char* g_bg_buffer[]       = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static signed long g_bg_len[]    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static unsigned char *g_typus_buffer[]    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static signed long g_typus_len[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* declare the filenames here, to use them in g_fnames */
static const char g_fname00[], g_fname01[], g_fname02[], g_fname03[], g_fname04[];
static const char g_fname05[], g_fname06[], g_fname07[], g_fname08[], g_fname09[];
static const char g_fname10[], g_fname11[], g_fname12[], g_fname13[], g_fname14[];
static const char g_fname15[], g_fname16[], g_fname17[], g_fname18[], g_fname19[];
static const char g_fname20[], g_fname21[], g_fname22[], g_fname23[], g_fname24[];
static const char g_fname25[], g_fname26[], g_fname27[], g_fname28[], g_fname29[];
static const char g_fname30[], g_fname31[], g_fname32[], g_fname33[], g_fname34[];
static const char g_fname35[], g_fname36[];

static const char* g_fnames_g105de[] = {
	g_fname00, g_fname01, g_fname02, g_fname03, g_fname04,
	g_fname05, g_fname06, g_fname07, g_fname08, g_fname09,
	g_fname10, g_fname11, g_fname12, g_fname13, g_fname14,
	g_fname15, g_fname16, g_fname17, g_fname18, g_fname19,
	g_fname20, g_fname21, g_fname22, g_fname23, g_fname24,
	g_fname25, g_fname26, g_fname27, g_fname28, g_fname29,
	g_fname30, g_fname31, g_fname32, g_fname33, g_fname34,
	g_fname35, g_fname36
};

static const char g_str_file_missing[] = { "FILE %s IS MISSING!" };

static const struct struct_color g_col_white2 = { 0x3f, 0x3f, 0x3f };

static const char dummy9[] = { 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff };

struct struct_chr_lookup {
	unsigned char chr;
	signed char index;
	signed char width;
};

static const struct struct_chr_lookup g_chr_lookup[74] = {
	{0x20, 0, 6},
	{0x41, 1, 6},
	{0x42, 2, 6},
	{0x43, 3, 6},
	{0x44, 4, 6},
	{0x45, 5, 6},
	{0x46, 6, 6},
	{0x47, 7, 6},
	{0x48, 8, 6},
	{0x49, 9, 4},
	{0x4a, 10, 6},
	{0x4b, 11, 6},
	{0x4c, 12, 5},
	{0x4d, 13, 6},
	{0x4e, 14, 6},
	{0x4f, 15, 6},
	{0x50, 16, 6},
	{0x51, 17, 6},
	{0x52, 18, 6},
	{0x53, 19, 6},
	{0x54, 20, 6},
	{0x55, 21, 6},
	{0x56, 22, 6},
	{0x57, 23, 6},
	{0x58, 24, 6},
	{0x59, 25, 6},
	{0x5a, 26, 6},
	{0x8e, 27, 6},
	{0x99, 28, 6},
	{0x9a, 29, 6},
	{0x2c, 30, 3},
	{0x2e, 31, 2},
	{0x2d, 32, 6},
	{0x28, 33, 4},
	{0x29, 34, 4},
	{0x21, 35, 3},
	{0x3f, 36, 5},
	{0x30, 37, 6},
	{0x31, 38, 3},
	{0x32, 39, 6},
	{0x33, 40, 6},
	{0x34, 41, 6},
	{0x35, 42, 6},
	{0x36, 43, 6},
	{0x37, 44, 6},
	{0x38, 45, 6},
	{0x39, 46, 6},
	{0x27, 47, 3},
	{0x3a, 48, 3},
	{0x23, 49, 6},
	{0x26, 50, 5},
	{0x25, 51, 5},
	{0x3c, 53, 5},
	{0x3e, 52, 5},
	{0x2f, 54, 6},
	{0x5f, 55, 6},
	{0xc8, 56, 6},
	{0xc9, 57, 6},
	{0xca, 58, 6},
	{0xcb, 59, 5},
	{0xcc, 60, 5},
	{0xcd, 61, 4},
	{0xce, 62, 5},
	{0xcf, 63, 5},
	{0xd0, 64, 2},
	{0xd1, 65, 6},
	{0xd2, 66, 6},
	{0xd3, 67, 5},
	{0xd4, 68, 6},
	{0xd5, 69, 5},
	{0xd6, 70, 6},
	{0xd7, 71, 5},
	{0xd8, 72, 6},
	{0xd9, 73, 5},
};

static const struct mouse_action g_action_input[2] = {
	{ 0, 0, 319, 199, KEY_RET},
	{ -1, -1, -1, -1, -1}
};

static signed short g_bool_mode = 0;

static const struct mouse_action* g_action_page[] = {
	(struct mouse_action*)&g_action_base,
	(struct mouse_action*)&g_action_skills,
	(struct mouse_action*)&g_action_skills,
	(struct mouse_action*)&g_action_skills,
	(struct mouse_action*)&g_action_skills,
	(struct mouse_action*)&g_action_spells,
	(struct mouse_action*)&g_action_spells,
	(struct mouse_action*)&g_action_spells,
	(struct mouse_action*)&g_action_spells,
	(struct mouse_action*)&g_action_spells,
	(struct mouse_action*)&g_action_spells
};

static char g_need_refresh = 1;

struct type_bitmap {
	char t[13];
};

static const struct type_bitmap g_type_bitmap = { {0} };

static const char g_str_version[] = "V1.05";

static const struct struct_color g_pal_attic[16] = {
	{0x00, 0x00, 0x00},
	{0x24, 0x24, 0x3c},
	{0x1c, 0x1c, 0x38},
	{0x18, 0x18, 0x34},
	{0x14, 0x14, 0x30},
	{0x0c, 0x10, 0x28},
	{0x0c, 0x0c, 0x24},
	{0x08, 0x08, 0x20},
	{0x04, 0x04, 0x1c},
	{0x00, 0x00, 0x18},
	{0x00, 0x00, 0x14},
	{0x3c, 0x30, 0x28},
	{0x34, 0x34, 0x3c},
	{0x30, 0x30, 0x3c},
	{0x28, 0x28, 0x3c},
	{0x20, 0x26, 0x3c},
};

static const struct struct_color g_pal_dsalogo[32] = {
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
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x2c, 0x00, 0x2c},
	{0x3c, 0x30, 0x30},
	{0x38, 0x28, 0x28},
	{0x34, 0x18, 0x18},
	{0x30, 0x0c, 0x0c},
	{0x2c, 0x00, 0x00},
	{0x28, 0x00, 0x00},
	{0x24, 0x00, 0x00},
	{0x1c, 0x00, 0x00},
	{0x18, 0x00, 0x00},
	{0x14, 0x00, 0x00},
	{0x3c, 0x3c, 0x3c},
};

static const struct struct_color g_pal_col_white = { 0x3f, 0x3f, 0x3f };

static const struct struct_color g_pal_col_black = { 0x00, 0x00, 0x00 };

static const struct struct_color g_pal_popup[8] = {
	{0x00, 0x00, 0x00 },
	{0x38, 0x30, 0x28 },
	{0x38, 0x30, 0x10 },
	{0x30, 0x28, 0x0c },
	{0x2c, 0x24, 0x08 },
	{0x28, 0x20, 0x04 },
	{0x18, 0x14, 0x00 },
	{0x0b, 0x19, 0x0c }
};

static const struct struct_color g_pal_misc[3] = {
	{0x28, 0x00, 0x00 },
	{0x28, 0x28, 0x00 },
	{0x00, 0x00, 0x28 }
};

static const struct struct_color g_pal_heads[32] = {
	{0x00, 0x00, 0x00},
	{0x00, 0x00, 0x3f},
	{0x00, 0x00, 0x24},
	{0x3c, 0x30, 0x28},
	{0x3c, 0x2c, 0x24},
	{0x3c, 0x28, 0x1c},
	{0x38, 0x24, 0x18},
	{0x34, 0x20, 0x14},
	{0x30, 0x1c, 0x14},
	{0x28, 0x18, 0x10},
	{0x24, 0x14, 0x0c},
	{0x1c, 0x10, 0x0c},
	{0x18, 0x0c, 0x08},
	{0x14, 0x0c, 0x08},
	{0x38, 0x38, 0x38},
	{0x30, 0x30, 0x30},
	{0x2c, 0x2c, 0x2c},
	{0x28, 0x28, 0x28},
	{0x20, 0x20, 0x20},
	{0x1c, 0x1c, 0x1c},
	{0x14, 0x14, 0x14},
	{0x10, 0x10, 0x10},
	{0x00, 0x3f, 0x00},
	{0x00, 0x24, 0x00},
	{0x3c, 0x38, 0x00},
	{0x34, 0x2c, 0x00},
	{0x28, 0x20, 0x00},
	{0x3c, 0x18, 0x10},
	{0x30, 0x10, 0x08},
	{0x24, 0x0c, 0x04},
	{0x10, 0x08, 0x04},
	{0x3c, 0x3c, 0x3c},
};

static const char g_str_sound_cfg[] = "SOUND.CFG";
static const char g_str_sound_adv[] = "SOUND.ADV";
static const char g_str_soundhw_not_found[] = "SOUND HARDWARE NOT FOUND!";
static const char g_str_chr[] = ".CHR";
static const char g_str_temp_dir[] = "TEMP\\";
static const char g_str_save_error[] = "@SPEICHER FEHLER!@EVENTUELL DISKETTE GESCH\x9aTZT?";

/* Remark: these are stored at DS:0x1e39 */
static const char g_fname00[] = "GEN1.NVF";
static const char g_fname01[] = "GEN2.NVF";
static const char g_fname02[] = "GEN3.NVF";
static const char g_fname03[] = "GEN4.NVF";
static const char g_fname04[] = "GEN5.NVF";
static const char g_fname05[] = "GEN6.NVF";
static const char g_fname06[] = "GEN7.NVF";
static const char g_fname07[] = "GEN8.NVF";
static const char g_fname08[] = "GEN9.NVF";
static const char g_fname09[] = "GEN10.NVF";
static const char g_fname10[] = "GEN11.NVF";
static const char g_fname11[] = "HEADS.DAT";
static const char g_fname12[] = "SEX.DAT";
static const char g_fname13[] = "TYPPIC.DAT";
static const char g_fname14[] = "FONT6";
static const char g_fname15[] = "GENTEXT";
static const char g_fname16[] = "DSALOGO.DAT";
static const char g_fname17[] = "GENTIT.DAT";
static const char g_fname18[] = "ATTIC";
static const char g_fname19[] = "POPUP.DAT";
static const char g_fname20[] = "DGAUKLER.DAT";
static const char g_fname21[] = "DJAEGER.DAT";
static const char g_fname22[] = "DKRIEGER.DAT";
static const char g_fname23[] = "DSTREUNE.DAT";
static const char g_fname24[] = "DTHORWAL.DAT";
static const char g_fname25[] = "DZWERG.DAT";
static const char g_fname26[] = "DHEXE.DAT";
static const char g_fname27[] = "DDRUIDE.DAT";
static const char g_fname28[] = "DMAGIER.DAT";
static const char g_fname29[] = "DAELF.DAT";
static const char g_fname30[] = "DFELF.DAT";
static const char g_fname31[] = "DWELF.DAT";
static const char g_fname32[] = "DMENGE.DAT";
static const char g_fname33[] = "GEN.XMI";
static const char g_fname34[] = "FANPRO.NVF";
static const char g_fname35[] = "SAMPLE.AD";
static const char g_fname36[] = "MT32EMUL.XMI";

static const char g_str_dsagen_dat[] = "DSAGEN.DAT";
static const char g_str_malloc_error[] = "\xaMEMORY MALLOCATION ERROR!";

signed short g_random_gen_seed = 0x327b;
/* END OF INITIALIZED GLOBAL VARIABLES _DATA */

/* START OF UNINITIALIZE GLOBAL VARIABLE _BSS DS:0x2474*/
static unsigned short g_current_timbre_length;
/* These 6 bytes are written at once from a file */
static signed char g_current_timbre_patch;
static signed char g_current_timbre_bank;
static unsigned long g_current_timbre_offset;

#if defined(__BORLANDC__)
void far *g_timer_isr_bak;

static char dummy10[768];
#endif

static signed short g_got_ch_bonus;
static signed short g_got_mu_bonus;

static char dummy11[0x26f];

static signed long g_flen;
static signed long g_flen_left;

#if defined(__BORLANDC__)
void far *g_irq78_bak;
#endif
static signed long g_gendat_offset;

static signed short g_handle_timbre;
static signed short g_timbre_cache_size;
static signed long g_state_table_size;
static unsigned char* g_snd_driver;
static void* g_form_xmid;
static unsigned char* g_snd_timbre_cache;
static void* g_state_table;
static unsigned char* g_snd_driver_base_addr;
static unsigned char* g_snd_driver_desc;
static signed short g_snd_sequence;
static signed short g_snd_driver_handle;

static signed short g_param_level;
signed short g_called_with_args;

struct inc_states {
	signed char tries;
	signed char incs;
};

static struct inc_states g_spell_incs[86];
static struct inc_states g_skill_incs[52];
static signed char g_attrib_changed[14];
static char* g_type_names[12];

/* the index of the last head */
static signed char g_head_last;
/* the index of the first head */
static signed char g_head_first;
/* the index of the current head */
static signed char g_head_current;
/* the typus for the heads e.G. all elves are 10 */
static signed char g_head_typus;

static signed char g_in_intro;

static signed short g_menu_tiles;
static signed short g_left_border;
static signed short g_upper_border;

static signed short g_level; /* {-1, 0, 1 (= Novice), 2 (= Advanced) } */

static RealPt g_dst_dst;
static signed short g_dst_x1;
static signed short g_dst_y1;
static signed short g_dst_x2;
static signed short g_dst_y2;
static RealPt g_dst_src;
static signed short g_unkn1;
static signed short g_unkn2;
static signed short g_unkn3;
static signed short g_unkn4;

static char* g_texts[301];
static signed short dummy12;

#if !defined(__BORLANDC__)
/* usage: output */
static inline char* get_text(signed short no) {
	return g_texts[no];
}
#else
#define get_text(no) (g_texts[no])
#endif

/* used by external module */
signed short g_random_gen_seed2;

static signed short g_have_mouse;
static signed short dummy13;
static signed short g_wo_var1;

static signed short g_mouse1_event1;
static signed short g_mouse2_event;
static signed short g_mouse1_event2;
static signed short g_in_key_ascii;
static signed short g_in_key_ext;
static unsigned char g_array_1[64];
static unsigned char g_array_2[64];

static unsigned short *g_mouse_last_cursor;
static unsigned short *g_mouse_current_cursor;
static char dummy14[0x40];

static char g_mouse_backbuffer[256];

static unsigned char *g_buffer_sex_dat;
static unsigned char *g_buffer_popup_nvf;
static unsigned char *g_buffer_heads_dat;
static char  *g_buffer_text;
static unsigned char *g_buffer_font6;

static signed short g_col_index;
static signed short g_bg_color;
static signed short g_fg_color[6];
static signed short g_text_x_end;
static signed short g_text_y;
static signed short g_text_x;
static signed long dummy15;

static unsigned char *g_picbuf3;
static unsigned char *g_picbuf2;
static unsigned char *g_picbuf1;
static unsigned char *g_gen_ptr6;
static unsigned char *g_buffer_dmenge_dat;
static unsigned char* dummy16;
static unsigned char* dummy17;
static unsigned char *g_gen_ptr5;
static unsigned char *g_gen_ptr4;
static char *g_gen_ptr3;
static char *g_gen_ptr2;
static char *dummy18;

static unsigned char* g_gfx_ptr;
unsigned char* g_vga_memstart;

static unsigned char* g_page_buffer;
static unsigned char* g_gen_ptr1_dis;
static signed short g_wo_var2;
static signed short g_wo_var3;
static signed short g_display_page_bak;
static signed short g_display_mode_bak;

static signed short g_ro_var[7];

#if !defined(__BORLANDC__)
/* use sprintf() for compatibility */
static inline char* itoa(int value, char* string, int radix)
{
	sprintf(string, "%d", value);
	return string;
}

// Quit Signal from SDL
extern int g_lets_quit;
#endif

#if defined(__BORLANDC__)
/* A little quirk here:
 * This Segment starts at offset 0x0005 with some overlapping code from CD-Audio (5 Bytes).
 * To get the correct code we start at offset 0x0000 with some alignement code
 * to obtain correct alignment and the correct adresses from the switch jump-tables.
 */
#if 1
void dummy0()
{
	asm {nop; };
}
void dummy()
{
}
#endif
#endif

/* Borlandified and identical */
void start_music(unsigned short track)
{
	if (!g_use_cda) {
		if (g_midi_disabled == 0) {
			play_midi(track);
		}
	} else {
		seg001_0465(track);
	}
}

/* Borlandified and identical */
void read_soundcfg(void)
{
	signed short handle;
	signed short port;

	g_use_cda = 0;
	g_midi_disabled = 1;

	if ((handle = open(g_str_sound_cfg, 0x8001)) != -1) {
		_read(handle, (unsigned char*)&port, 2);
		_close(handle);

#if !defined(__BORLANDC__)
		/* Small hack: enable MIDI instead of CD-Audio */
		//D1_INFO("MIDI port 0x%x\n", port);
		if ((port != 0) && (load_driver(g_str_sound_adv, 3, port))) {
			/* disable audio-cd */
			g_use_cda = 0;
			return;
		}
#endif
		/* enable audio-cd, disable midi */
		g_use_cda = g_midi_disabled = 1;

		/* play audio-cd */
		seg001_0600();
	}
}

/* Borlandified and identical */
void init_music(unsigned long size)
{
	if ((g_form_xmid = gen_alloc(size))) {
		AIL_startup();
		g_midi_disabled = 1;
	}
}

/* Borlandified and identical */
void stop_music(void)
{
	AIL_shutdown(NULL);

	/* Remark: set pointer to NULL */
	if (g_snd_timbre_cache)
		free(g_snd_timbre_cache);

	if (g_state_table)
		free(g_state_table);

	if (g_form_xmid)
		free(g_form_xmid);

	if (g_snd_driver)
		free(g_snd_driver);

	seg001_033b();
}

/* Borlandified and nearly identical, but works identically */
unsigned char *load_snd_driver(const char *fname)
{
	signed long size;
	unsigned char *norm_ptr;
	unsigned long in_ptr;
	signed short handle;

	if ((handle = open(fname, 0x8001)) != -1) {
		size = 16500;
		g_snd_driver = (unsigned char*)gen_alloc(size + 0x10);
#if defined(__BORLANDC__)
		// BCC: far pointer normalizaion (DOS only)
		in_ptr = ((unsigned long)g_snd_driver) + 0x0f;
		in_ptr &= 0xfffffff0;

		/* The arguments of read are working, but not identical */
		_read(handle, (norm_ptr = normalize_ptr((unsigned char*)in_ptr)), size);
#else
		_read(handle, (norm_ptr = g_snd_driver), size);
#endif
		_close(handle);
		return norm_ptr;
	} else {
		return (RealPt)0L;
	}
}

/* Borlandified and identical */
void unload_snd_driver(void)
{
	if (g_snd_driver) {
		free(g_snd_driver);
		g_snd_driver = NULL;
	}
}

/* Borlandified and identical */
unsigned short load_seq(signed short sequence_num)
{
	signed short patch;
	signed short *src_ptr;
	unsigned short answer; // si = {bank, patch}
	signed short bank; // di = bank

	/* open SAMPLE.AD */
	if ((g_handle_timbre = open_datfile(35)) != -1) {

		if ((g_snd_sequence = AIL_register_sequence(g_snd_driver_handle,
			g_form_xmid, sequence_num, g_state_table, (void*)NULL)) != -1) {

			while ((answer = AIL_timbre_request(g_snd_driver_handle, g_snd_sequence)) != 0xffff)
			{
				bank = answer >> 8;

				if ((src_ptr = get_timbre(bank, patch = (answer & 0xff))) != 0) {
					/* ptr is passed differently */
					AIL_install_timbre(g_snd_driver_handle, bank, patch, src_ptr);
					free(src_ptr);
				}
			}
			/* Remark: set g_handle_timbre to 0 after close() */

			close(g_handle_timbre);

			return 1;
		} else {
			close(g_handle_timbre);
		}
	}
	return 0;

}

/* Borlandified and identical */
unsigned short play_sequence(signed short sequence_num)
{
	if (load_seq(sequence_num) != 0) {
		AIL_start_sequence(g_snd_driver_handle, sequence_num);
		return 1;
	}

	return 0;
}

/* Borlandified and identical */
signed short *get_timbre(signed short bank, signed short patch)
{
	signed short *timbre_ptr;

	lseek(g_handle_timbre, g_gendat_offset, SEEK_SET);

	do {
		read_datfile(g_handle_timbre, (unsigned char*)&g_current_timbre_patch, 6);

		if (g_current_timbre_bank == -1)
			return 0;

	} while ((g_current_timbre_bank != bank) || (g_current_timbre_patch != patch));
//	Remark: Try out the next line instead and get a different sound:
//	} while ((g_current_timbre_bank != bank) && (g_current_timbre_patch != patch));

	lseek(g_handle_timbre, g_gendat_offset + g_current_timbre_offset, SEEK_SET);
	read_datfile(g_handle_timbre, (unsigned char*)&g_current_timbre_length, 2);

	timbre_ptr = (signed short*)gen_alloc(g_current_timbre_length);

	read_datfile(g_handle_timbre, (unsigned char*)&timbre_ptr[1], (timbre_ptr[0] = g_current_timbre_length) - 2);

	return timbre_ptr;
}

/* Borlandified and identical */
unsigned short call_load_file(signed short index)
{
	return load_file(index);
}

/* Borlandified and identical */
unsigned short load_file(signed short index)
{
	signed short handle;

	if ((handle = open_datfile(index)) != -1) {
		read_datfile(handle, g_form_xmid, 32767);
		close(handle);
		return 1;
	}

	return 0;
}

/* Borlandified and identical */
signed short load_driver(const char* fname, signed short type, signed short port)
{
	if ((port != 0) &&
		(g_snd_driver_base_addr = (unsigned char*)load_snd_driver(fname)) &&
		((g_snd_driver_handle = AIL_register_driver((unsigned char*)g_snd_driver_base_addr)) != -1))
	{
		g_snd_driver_desc = (unsigned char*)AIL_describe_driver(g_snd_driver_handle);

		if (host_readws(g_snd_driver_desc + 0x02) == type)
		{
			if (port == -1) {
				port = host_readws(g_snd_driver_desc + 0x0c);
			}
			if (AIL_detect_device(g_snd_driver_handle, port,
					host_readw(g_snd_driver_desc + 0x0e),
					host_readw(g_snd_driver_desc + 0x10),
					host_readw(g_snd_driver_desc + 0x12)) != 0)
			{
				AIL_init_driver(g_snd_driver_handle, port,
					host_readw(g_snd_driver_desc + 0x0e),
					host_readw(g_snd_driver_desc + 0x10),
					host_readw(g_snd_driver_desc + 0x12));
				if (type == 3) {
					g_state_table_size = AIL_state_table_size(g_snd_driver_handle);

					g_state_table = (void*)gen_alloc(g_state_table_size);

					g_timbre_cache_size = AIL_default_timbre_cache_size(g_snd_driver_handle);

					if (g_timbre_cache_size != 0) {
						g_snd_timbre_cache = (unsigned char*)gen_alloc((unsigned short)g_timbre_cache_size);
						AIL_define_timbre_cache(g_snd_driver_handle,
							g_snd_timbre_cache,
							g_timbre_cache_size);
					}
				}

				g_midi_disabled = 0;
				return 1;
			} else {
				infobox(g_str_soundhw_not_found, 0);
			}
		}
	}

	g_midi_disabled = 1;
	return 0;
}

/* Borlandified and identical */
void play_midi(unsigned short index)
{
	if ((g_midi_disabled == 0) && (host_readw(g_snd_driver_desc + 0x02) == 3))
	{
		stop_sequence();
		call_load_file(index);
		play_sequence(0);
	}
}

/* Borlandified and identical */
void stop_sequence(void)
{
	if ((g_midi_disabled == 0) && (host_readw(g_snd_driver_desc + 0x02) == 3))
	{
		AIL_stop_sequence(g_snd_driver_handle, g_snd_sequence);
		AIL_release_sequence_handle(g_snd_driver_handle, g_snd_sequence);
	}
}

/* Borlandified and identical */
void restart_midi(void)
{
	if ((g_midi_disabled == 0) && (host_readw(g_snd_driver_desc + 0x02) == 3) &&
		(AIL_sequence_status(g_snd_driver_handle, g_snd_sequence) == 2))
	{
		AIL_start_sequence(g_snd_driver_handle, g_snd_sequence);
	}
}

/**
 * mouse_action -	does mouse programming
 * @p1:		function AX
 * @p2:		depends on AX
 * @p3:		depends on AX
 * @p4:		depends on AX
 * @p5:		depends on AX
 *
 * This function differs a bit. Borlands C-Library has a special function
 * to call interrupts. We use the one of DOSBox, which means, that we
 * put the values in the emulated registers, instead in a structure.
 */
/* Borlandified and identical */
void do_mouse_action(unsigned char *p1, unsigned char *p2, unsigned char *p3, unsigned char *p4, unsigned char *p5)
{
#if defined(__BORLANDC__)
	union REGS myregs;
	struct SREGS mysregs;

	if (host_readws(p1) >= 0) {
		myregs.x.ax = host_readw(p1);
		myregs.x.bx = host_readw(p2);
		myregs.x.cx = host_readw(p3);

		switch (host_readws(p1)) {
			case 0x9:	/* define Cursor in graphic mode */
			case 0xc:	/* install event handler */
			case 0x14:	/* swap event handler */
			case 0x16:	/* save mouse state */
			case 0x17:	/* load mouse state */
				myregs.x.dx = host_readw(p4);
				mysregs.es = host_readw(p5);
				break;
			case 0x10:	/* define screen region for update */
				myregs.x.cx = host_readw(p2);
				myregs.x.dx = host_readw(p3);
				myregs.x.si = host_readw(p4);
				myregs.x.di = host_readw(p5);
				break;
			default:
				myregs.x.dx = host_readw(p4);
		}

		int86x(0x33, &myregs, &myregs, &mysregs);

		if (host_readw(p1) == 0x14) {
			host_writew(p2, mysregs.es);
		} else {
			host_writew(p2, myregs.x.bx);
		}

		host_writew(p1, myregs.x.ax);
		host_writew(p3, myregs.x.cx);
		host_writew(p4, myregs.x.dx);
	}
#endif
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void interrupt mouse_isr(void)
{
	signed short l_si = _AX;
	signed short p1;
	signed short p2;
	signed short p3;
	signed short p4;
	signed short p5;
	
	if (g_mouse_locked == 0) {
		if (l_si & 0x2) {
			g_mouse1_event2 = 1;
			g_mouse1_event1 = 1;
		}
		if (l_si & 0x8) {
			g_mouse2_event = 1;
		}
		if (l_si & 0x1) {
			p1 = 3;
			p3 = g_mouse_posx;
			p4 = g_mouse_posy;
			
			do_mouse_action((unsigned char*)&p1, (unsigned char*)&p2, (unsigned char*)&p3, (unsigned char*)&p4, (unsigned char*)&p5);

			g_mouse_posx = p3;
			g_mouse_posy = p4;
			
			if (g_mouse_posx > g_mouse_posx_max) {
				g_mouse_posx = g_mouse_posx_max;
			}
			if (g_mouse_posx < g_mouse_posx_min) {
				g_mouse_posx = g_mouse_posx_min;
			}
			if (g_mouse_posy < g_mouse_posy_min) {
				g_mouse_posy = g_mouse_posy_min;
			}
			if (g_mouse_posy > g_mouse_posy_max) {
				g_mouse_posy = g_mouse_posy_max;
			}
			
			p1 = 4;
			p3 = g_mouse_posx;
			p4 = g_mouse_posy;
			
			do_mouse_action((unsigned char*)&p1, (unsigned char*)&p2, (unsigned char*)&p3, (unsigned char*)&p4, (unsigned char*)&p5);
			
			g_mouse_moved = 1;
		}
	}
}
#endif


/* Borlandified and identical */
void mouse_enable(void)
{
	unsigned short p1, p2, p3, p4, p5;

	if (g_have_mouse == 2) {

		/* initialize mouse */
		p1 = 0;

		do_mouse_action((unsigned char*)&p1, (unsigned char*)&p2, (unsigned char*)&p3, (unsigned char*)&p4, (unsigned char*)&p5);

		if (p1 == 0) {
			g_have_mouse = 0;
		}

		g_mouse_current_cursor = g_mouse_mask;
		g_mouse_last_cursor = g_mouse_mask;

		if (g_have_mouse == 2) {

			/* move cursor  to initial position */
			p1 = 4;
			p3 = g_mouse_posx;
			p4 = g_mouse_posy;

			do_mouse_action((unsigned char*)&p1, (unsigned char*)&p2, (unsigned char*)&p3, (unsigned char*)&p4, (unsigned char*)&p5);
#if defined(__BORLANDC__)
			mouse_do_enable(0x1f, (RealPt)&mouse_isr);
#endif
		}
	}
}

/* Borlandified and identical */
void mouse_disable(void)
{
	if (g_have_mouse == 2) {
		mouse_do_disable();
	}
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void mouse_unused1(unsigned char *p1, unsigned char *p2, unsigned char *p3, unsigned char *p4)
{
	unsigned short l_var;
	host_writew(p1, 5);
	do_mouse_action(p1, p2, p3, p4, (unsigned char*)&l_var);
}

/* Borlandified and identical */
void mouse_call_isr(void)
{
	mouse_isr();
}
#endif

/* Borlandified and identical */
void mouse_do_enable(unsigned short val, RealPt ptr)
{
#if defined(__BORLANDC__)
	unsigned short p1, p2, p3, p4, p5;

	p1 = 0x0c;
	p3 = val;
	p4 = (unsigned short)FP_OFF(mouse_isr);
	p5 = (unsigned short)FP_SEG(mouse_isr);

	/* save adress of old IRQ 0x78 */
	g_irq78_bak = ((void interrupt far (*)(void))_dos_getvect(0x78));

	/* set new IRQ 0x78 */
	_dos_setvect(0x78, (void interrupt far (*)(void))ptr);

	/* set the new mouse event handler */
	do_mouse_action((unsigned char*)&p1, (unsigned char*)&p2, (unsigned char*)&p3, (unsigned char*)&p4, (unsigned char*)&p5);

	g_mouse_handler_installed = 1;
#endif
}

/* Borlandified and identical */
void mouse_do_disable(void)
{
#if defined(__BORLANDC__)
	unsigned short v1, v2, v3, v4, v5;

	/* restore the old int 0x78 handler */
	_dos_setvect(0x78, (void interrupt far (*)(void))g_irq78_bak);

	/* uninstall mouse event handler */
	v1 = 0x0c;
	v3 = 0;
	v4 = 0;
	v5 = 0;

	do_mouse_action((unsigned char*)&v1, (unsigned char*)&v2, (unsigned char*)&v3, (unsigned char*)&v4, (unsigned char*)&v5);

	g_mouse_handler_installed = 0;
#endif
}

/**
 * mouse_move_cursor -	move the mouse cursor to a position
 * @x:	X - coordinate
 * @y:	Y - coordinate
 */
/* Borlandified and identical */
void mouse_move_cursor(unsigned short x, unsigned short y)
{
	unsigned short p1, p2, p3, p4, p5;

	p1 = 4;
	p3 = x;
	p4 = y;

	do_mouse_action((unsigned char*)&p1, (unsigned char*)&p2, (unsigned char*)&p3, (unsigned char*)&p4, (unsigned char*)&p5);
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void mouse_unused2(unsigned short a1, unsigned short a2, unsigned short a3, unsigned short a4)
{
	unsigned short p1, p2, p3, p4, p5;

	p1 = 9;
	p2 = a1;
	p3 = a2;
	p4 = a3;
	p5 = a4;

	do_mouse_action((unsigned char*)&p1, (unsigned char*)&p2, (unsigned char*)&p3, (unsigned char*)&p4, (unsigned char*)&p5);
}

/* Borlandified and identical */
void mouse_unused3(unsigned short a1)
{
	unsigned short p1, p2, p3, p4, p5;

	p1 = 0x1d;
	p2 = a1;

	do_mouse_action((unsigned char*)&p1, (unsigned char*)&p2, (unsigned char*)&p3, (unsigned char*)&p4, (unsigned char*)&p5);
}
#endif

/* Borlandified and identical */
/* static */
void update_mouse_cursor(void)
{
	update_mouse_cursor1();
}

/* Borlandified and identical */
/* static */
void call_mouse(void)
{
	mouse();
}

/* Borlandified and identical */
/* static */
void update_mouse_cursor1(void)
{
	if (g_mouse_locked == 0) {

		if (g_mouse_refresh_flag == 0) {
			g_mouse_locked = 1;
			restore_mouse_bg();
			g_mouse_locked = 0;
		}
		g_mouse_refresh_flag--;
	}
}

/* Borlandified and identical */
/* static */
void mouse(void)
{
	if (g_mouse_locked == 0) {

		g_mouse_refresh_flag++;

		if (g_mouse_refresh_flag == 0) {

			g_mouse_locked = 1;

			if (g_mouse_posx < g_mouse_pointer_offsetx)
				g_mouse_posx = g_mouse_pointer_offsetx;

			if (g_mouse_posx > 315)
				g_mouse_posx = 315;

			if (g_mouse_posy < g_mouse_pointer_offsety)
				g_mouse_posy = g_mouse_pointer_offsety;

			if (g_mouse_posy > 195)
				g_mouse_posy = 195;

			save_mouse_bg();

			g_mouse_posx_bak = g_mouse_posx;
			g_mouse_posy_bak = g_mouse_posy;
			g_mouse_pointer_offsetx_bak = g_mouse_pointer_offsetx;
			g_mouse_pointer_offsety_bak = g_mouse_pointer_offsety;

			draw_mouse_cursor();

			g_mouse_locked = 0;
		}
	}
}

/* Borlandified and identical */
/* static */
void mouse_compare(void)
{
	/* these pointers never differ in gen */
	if (g_mouse_moved || g_mouse_last_cursor != g_mouse_current_cursor) {

		/* copy a pointer */
		g_mouse_last_cursor = g_mouse_current_cursor;

		if (g_mouse_mask == (unsigned short*)g_mouse_current_cursor) {
			g_mouse_pointer_offsetx = g_mouse_pointer_offsety = 0;
		} else {
			g_mouse_pointer_offsetx = g_mouse_pointer_offsety = 8;
		}
		g_mouse_moved = 0;
		update_mouse_cursor1();
		mouse();
	}
}


/* Borlandified and identical */
void handle_input(void)
{
	signed short si, i;

	g_in_key_ascii = g_in_key_ext = si = 0;

	if (CD_bioskey(1)) {

		si = (g_in_key_ascii = CD_bioskey(0)) >> 8;
		g_in_key_ascii &= 0xff;

		if (si == KEY_J)
			si = KEY_Y;

		if ((g_in_key_ascii == 0x11) && !g_in_intro) {

			update_mouse_cursor();
			mouse_disable();
			stop_music();
			restore_timer_isr();
			exit_video();
			clrscr();
			exit(0);
		}
	}

	if (g_mouse1_event2 == 0) {
		// Hm, ...
		if (g_have_mouse == 0);
	} else {
		g_mouse1_event2 = 0;
		si = 0;

		if (g_action_table)
			si = get_mouse_action(g_mouse_posx, g_mouse_posy,
				(struct mouse_action*)g_action_table);
				
		if ((si == 0) && (g_default_action))
			si = get_mouse_action(g_mouse_posx, g_mouse_posy,
				(struct mouse_action*)g_default_action);

		if (g_have_mouse == 2) {
			for (i = 0; i < 15; i++)
				wait_for_vsync();

			if (g_mouse1_event2 != 0) {
				g_wo_var1 = 1;
				g_mouse1_event2 = 0;
			}

			if (si == 0xfd) {
				si = 0;
				g_menu_tiles = 4;
				g_fg_color[4] = 1;
				infobox(get_text(267), 0);
				g_fg_color[4] = 0;
				g_menu_tiles = 3;
			}
		}
	}
	mouse_compare();
	g_in_key_ext = si;
}

/* Borlandified and identical */
/* static */
unsigned short get_mouse_action(signed short x, signed short y, struct mouse_action *act)
{
	signed short i;
	
	for (i = 0; act[i].x1 != -1; i++) {

		if (act[i].x1 > x)
			continue;
		if (act[i].x2 < x)
			continue;
		if (act[i].y1 > y)
			continue;
		if (act[i].y2 < y)
			continue;

		return act[i].action;
	}

	return 0;
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void unused_func1(signed char *in_ptr, signed short x, signed short y, signed char c1, signed char c2)
{
	signed char val;
	unsigned char *ptr;
	signed short i;
	signed short j;

	update_mouse_cursor();

	ptr = g_vga_memstart;
	ptr += 320 * y + x;

	for (i = 0; i < c2; ptr += 320 , i++) {
		for (j = 0; j < c1; j++) {
			if ((val = *in_ptr++) != 0) {
#if !defined(__BORLANDC__)
				ptr[j] = val;
#else
				ptr[j] = _AL;
#endif
			}
		}
	}

	call_mouse();
}
#endif




/**
 * decomp_rle() - decompress a RLE compressed picture
 * @dst:	destination
 * @src:	source
 * @y:		y - Coordinate to start
 * @x:		x - Coordinate to start
 * @width:	width of the picture
 * @height:	height of the picture
 * @mode:	if 2 copy pixels with the value 0
 *
*/
/* Borlandified and identical */
void decomp_rle(unsigned char *dst, unsigned char *src, signed short x, signed short y,
				signed short width, signed short height, unsigned short mode)
{
	signed short i, j, k;
	signed char val;
	unsigned char n;
	signed char pix;
	unsigned char *dst_loc;

	dst_loc = dst;
	dst_loc += 320 * y + x;
	update_mouse_cursor();

	for (i = 0; i < height; dst_loc += 320, i++) {

		j = 0;

		while (j < width) {

			if ((val = *(src++)) == 0x7f) {
				n = *src++;
				pix = *src++;

				if ((pix != 0) || (mode != 2))
					for (k = 0; k < n; k++)
						*(dst_loc + j + k) = pix;
				j += n;
			} else {
				if ((val != 0) || (mode != 2))
					*(dst_loc + j) = val;
				j++;
			}
		}
	}

	call_mouse();
}

/* Borlandified and nearly identical, but works correct */
/* static */
void draw_mouse_cursor(void)
{
	signed char Y;
	signed char X;
	unsigned char *vgaptr;
	signed short *mouse_cursor;
	signed short rangeY;
	signed short diffX;
	signed short diffY;

	register signed short rangeX; //di
	register signed short mask; //si

	vgaptr = g_vga_memstart;
	mouse_cursor = (signed short*)g_mouse_current_cursor + (32 / 2);

	rangeX = g_mouse_posx - g_mouse_pointer_offsetx;
	rangeY = g_mouse_posy - g_mouse_pointer_offsety;

	diffX = diffY = 16;

	if (rangeX > 304) diffX = 320 - rangeX;
	if (rangeY > 184) diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (Y = 0; Y < diffY; Y++) {
		mask = *mouse_cursor++;
		for (X = 0; X < diffX; X++)
			if ((0x8000 >> X) & mask)
				vgaptr[X] = 0xff;
		vgaptr += 320;
	}
}

/* Borlandified and identical */
/* static */
void save_mouse_bg(void)
{
	unsigned char *vgaptr;
	signed short rangeX;
	signed short rangeY;
	signed short diffX;
	signed short diffY;
	signed short Y;
	signed short X;

	vgaptr = g_vga_memstart;

	rangeX = g_mouse_posx - g_mouse_pointer_offsetx;
	rangeY = g_mouse_posy - g_mouse_pointer_offsety;

	diffX = diffY = 16;

	if (rangeX > 304) diffX = 320 - rangeX;
	if (rangeY > 184) diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (Y = 0; Y < diffY; vgaptr += 320, Y++)
		for (X = 0; X < diffX; X++)
			g_mouse_backbuffer[16 * Y + X] = vgaptr[X];
}

/* Borlandified and identical */
/* static */
void restore_mouse_bg(void)
{
	unsigned char *vgaptr;
	signed short rangeX;
	signed short rangeY;
	signed short diffX;
	signed short diffY;
	signed short i;
	signed short j;

	vgaptr = g_vga_memstart;

	rangeX = g_mouse_posx_bak - g_mouse_pointer_offsetx_bak;
	rangeY = g_mouse_posy_bak - g_mouse_pointer_offsety_bak;
	diffX = diffY = 16;

	if (rangeX > 304)
		diffX = 320 - rangeX;
	if (rangeY > 184)
		diffY = 200 - rangeY;

	vgaptr += rangeY * 320 + rangeX;

	for (i = 0; i < diffY; vgaptr += 320, i++)
		for (j = 0; j < diffX; j++)
			vgaptr[j] = g_mouse_backbuffer[16 * i + j];
}


/* Borlandified and nearly identical */
void load_font_and_text(void)
{
	signed short handle;
	signed long len;

	/* load FONT6 */
	handle = open_datfile(14);
	read_datfile(handle, g_buffer_font6, 1000);
	close(handle);

	/* load GENTEXT */
	handle = open_datfile(15);
	len = read_datfile(handle, (unsigned char*)g_buffer_text, 64000);
	close(handle);

	split_textbuffer(g_texts, g_buffer_text, len);
}

/* Borlandified and identical */
void split_textbuffer(char **dst, char *src, unsigned long len)
{
	unsigned long i;

	for (i = 0, *dst++ = src; i != len; src++, i++) {
		/* continue if not the end of the string */
		if (!*src) {
			/* return if "\0\0" (never happens) */
			if (!*(src + 1)) return;

			/* write the adress of the next string */
			*dst = src + 1;
			dst++;
		}
	}
}

/* Borlandified and identical */
void load_page(signed short page)
{
	unsigned char* ptr;
	signed short handle;

	if (page <= 10) {
		/* check if this image is in the buffer */
		if (g_bg_buffer[page]) {
			decomp_rle(g_gen_ptr1_dis, g_bg_buffer[page], 0, 0, 320, 200, 0);
			return;
		}

		if ((ptr = gen_alloc(get_filelength(handle = open_datfile(page))))) {
			g_bg_buffer[page] = ptr;
			g_bg_len[page] = get_filelength(handle);

			read_datfile(handle, g_bg_buffer[page], g_bg_len[page]);
			decomp_rle(g_gen_ptr1_dis, g_bg_buffer[page], 0, 0, 320, 200, 0);
			close(handle);
		} else {
			read_datfile(handle, g_page_buffer, 64000);
			decomp_rle(g_gen_ptr1_dis, g_page_buffer, 0, 0, 320, 200, 0);
			close(handle);
		}
	} else {
		/* this should not happen */
		handle = open_datfile(page);
		read_datfile(handle, g_gen_ptr1_dis - 8, 64000);
		close(handle);
		decomp_pp20(g_gen_ptr1_dis, g_gen_ptr1_dis - 8, get_filelength(handle));
	}
}

#if defined (__BORLANDC__)
/* Borlandified and nearly identical */
void read_datfile_to_buffer(signed short index, unsigned char *dst)
{
	signed short handle;
	handle = open_datfile(index);
	read_datfile(handle, dst, 64000);
	close(handle);
}
#endif

/* Borlandified and identical */
void load_typus(signed short typus)
{
	signed short index;
	unsigned char *ptr;
	signed short handle;

	index = typus + 19;

	/* check if this image is in the buffer */
	if (g_typus_buffer[typus]) {
		decomp_pp20(g_gen_ptr5,	g_typus_buffer[typus], g_typus_len[typus]);
		return;
	}

	if ((ptr = gen_alloc(get_filelength(handle = open_datfile(index))))) {
		/* load the file into the typus buffer */
		g_typus_buffer[typus] = ptr;
		g_typus_len[typus] = get_filelength(handle);

		read_datfile(handle, g_typus_buffer[typus], g_typus_len[typus]);

		decomp_pp20(g_gen_ptr5, g_typus_buffer[typus], g_typus_len[typus]);
	} else {
		/* load the file direct */
		read_datfile(handle, g_gen_ptr1_dis, 25000);
		decomp_pp20(g_gen_ptr5, g_gen_ptr1_dis, get_filelength(handle));
	}
	close(handle);
}

/**
 * save_chr() - save the hero the a CHR file
 */
/* Borlandified and nearly identical */
void save_chr(void)
{
	signed short tmpw;
	signed short tmph;
	char filename[20];
	struct nvf_desc nvf;
	char path[80];

	signed short handle; //si
	signed short i;      //di

	/* check for typus */
	if (!g_hero.typus) {
		infobox(get_text(72), 0);
		return;
	}
	/* check for name */
	if (!g_hero.name[0]) {
		infobox(get_text(154), 0);
		return;
	}

	/* Load picture from nvf */
	/* TODO: why not just copy? */
	nvf.dst = g_gen_ptr1_dis;
	nvf.src = g_buffer_heads_dat;
	nvf.no = g_head_current;
	nvf.type = 0;
	nvf.width = &tmpw;
	nvf.height = &tmph;

	process_nvf(&nvf);

	/* copy picture to the character struct */
	memcpy(g_hero.pic, g_gen_ptr1_dis, 1024);

	/* put the hero in the first group */
	g_hero.group = 1;

	/* wanna save ? */
	if (!gui_bool(get_text(3)))
		return;

	/* copy name to alias */
	/* TODO: should use strncpy() here */
	strcpy(g_hero.alias, g_hero.name);

	/* copy name to buffer */
	/* TODO: should use strncpy() here */
	strcpy(g_gen_ptr2, g_hero.name);

	/* prepare filename */
	for (i = 0; i < 8; i++) {
		/* leave the loop if the string ends */
		if (!g_gen_ptr2[i])
			break;
		if (!isalnum(g_gen_ptr2[i])) {
			/* replace non alphanumerical characters with underscore */
			g_gen_ptr2[i] = '_';
		}
	}

	strncpy(filename, g_gen_ptr2, 8);
	filename[8] = 0;
	strcat(filename, g_str_chr);

	if (((handle = open(filename, 0x8001)) == -1) || gui_bool(get_text(261))) {

#if !defined(__BORLANDC__)
		/* close an existing file before overwriting it */
		if (handle != -1) close(handle);
#endif
		handle = _creat(filename, 0);

		if (handle != -1) {
			write(handle, &g_hero, sizeof(g_hero));
			close(handle);

			if (g_called_with_args == 0) return;

			strcpy(path, g_str_temp_dir);
			strcat(path, filename);

			if ((handle = _creat(path, 0)) != -1) {
				write(handle, &g_hero, sizeof(g_hero));
				close(handle);
			}
		} else {
			/* should be replaced with infobox() */
			error_msg(g_str_save_error);
		}
	}
}

/* Borlandified and nearly identical */
void read_common_files(void)
{
	signed short handle; //si
	signed short len; //di

	/* load HEADS.DAT */
	handle = open_datfile(11);
	len = read_datfile(handle, g_buffer_heads_dat, 64000);
	close(handle);

	/* load POPUP.NVF */
	handle = open_datfile(19);
	len = read_datfile(handle, g_buffer_popup_nvf - 8, 500);
	close(handle);

	decomp_pp20(g_buffer_popup_nvf, g_buffer_popup_nvf - 8, len);

	/* load SEX.DAT */
	handle = open_datfile(12);
	read_datfile(handle, g_buffer_sex_dat, 900);
	close(handle);

	/* load DMENGE.DAT */
	handle = open_datfile(32);
	len = read_datfile(handle, g_buffer_dmenge_dat - 8, 25000);
	close(handle);

	decomp_pp20(g_buffer_dmenge_dat, g_buffer_dmenge_dat - 8, len);
}

/* Borlandified and far from identical, but works */
signed long process_nvf(struct nvf_desc *nvf)
{
	signed long offs;
	signed short pics;
	signed short height;
	signed short va;
	signed long p_size;
	signed long retval;
	signed char nvf_type;

	unsigned char *src;

	signed short i;     // si
	signed short width; // di
#if 0
	/* Fix: GCC warns about uninitialized values */
	width = height = 0;
	p_size = 0;
	src = NULL;
#endif

	va = (nvf_type = *(unsigned char*)(nvf->src)) & 0x80;
	nvf_type &= 0x7f;
	pics = host_readws(nvf->src + 1L);

	if (nvf->no < 0)
		nvf->no = 0;

	if (nvf->no > pics - 1)
		nvf->no = pics - 1;

	switch (nvf_type) {

	case 0x00:
		width = host_readws(nvf->src + 3L);
		height = host_readws(nvf->src + 5L);
		p_size = height * width;
		memcpy(nvf->dst - 8L, nvf->src + p_size * nvf->no + 7L, p_size);
		break;
	case 0x01:
		offs = pics * 4 + 3L;
		for (i = 0; i < nvf->no; i++) {
			width = host_readws(nvf->src + i * 4 + 3L);
			height = host_readws(nvf->src + i * 4 + 5L);
			offs += width * height;
		}

		width = host_readws(nvf->src + nvf->no * 4 + 3L);
		height = host_readws(nvf->src + nvf->no * 4 + 5L);
		p_size = width * height;
		memcpy(nvf->dst - 8L, nvf->src + offs, p_size);
		break;

	case 0x02:
		width = host_readws(nvf->src + 3L);
		height = host_readws(nvf->src + 5L);
		offs = ((unsigned long)(pics * 4)) + 7L;
		for (i = 0; i < nvf->no; i++) {
			/* BCC adds here in offs = offs + value */
			offs += host_readd(nvf->src + i * 4 + 7L);
		}

		p_size = host_readd(nvf->src + nvf->no * 4 + 7L);
		memcpy(nvf->dst - 8L, nvf->src + offs, p_size);
		break;

	case 0x03:
		offs = pics * 8 + 3L;
		for (i = 0; i < (signed short)nvf->no; i++) {
			/* First two lines are not neccessary */
			width = host_readws(nvf->src + i * 8 + 3L);
#if !defined(__BORLANDC__)
			height = host_readws(nvf->src + i * 8 + 5L);
#else
			// Sync-Point
			asm {db 0x0f, 0x1f, 0x00};
			asm {nop; nop}
			asm {nop; nop}
			asm {nop; nop}
			asm {nop; nop}
			asm {nop; nop}
			asm {nop; nop}
			asm {nop; nop}
			asm {nop; nop}
			asm {nop; nop}
			asm {nop; nop}
			asm {db 0x66, 0x90}
#endif
			/* BCC adds here in offs = offs + value */
			offs += host_readd(nvf->src + i * 8 + 7L);
		}

		// Selected picture nvf->no, and copy it to nvf->dst
		width = host_readws(nvf->src + nvf->no * 8 + 3L);
		height = host_readws(nvf->src + nvf->no * 8 + 5L);
		p_size = host_readd(nvf->src + i * 8 + 7L);

		memcpy(nvf->dst - 8L, nvf->src + offs, p_size);
		break;
	}

	if (!nvf->type) {
		/* PP20 decompression */
		if (va != 0) {
			/* get size from unpacked picture */
			retval = host_readds(nvf->dst) - 8L;
			src = nvf->dst - 8L;
			src += (retval - 4L);
			retval = host_readd(src);
			retval = swap_u32(retval) >> 8;

		} else {
			retval = width * height;
		}

		decomp_pp20(nvf->dst, nvf->dst - 8L, p_size);

	} else {
		/* No decompression, just copy */
		memmove(nvf->dst, nvf->dst - 8L, (signed short)p_size);
		retval = p_size;
	}

	*nvf->width = width;
	*nvf->height = height;

	return retval;
}

/* Borlandified and identical */
signed short open_datfile(unsigned short index)
{
	unsigned char buf[800];
	signed short handle;

	bc_flushall();

	/* 0x8001 = O_BINARY | O_RDONLY */
#if defined(__BORLANDC__)
	while ((handle = open(g_str_dsagen_dat, O_BINARY | O_RDONLY)) == -1)
#else
	while ((handle = open(g_str_dsagen_dat, O_RDONLY)) == -1)
#endif
	{
		sprintf(g_gen_ptr2,
			(const char*)g_str_file_missing,
			(const char*)g_fnames_g105de[index]);

		g_useless_variable = 1;
		infobox(g_gen_ptr2, 0);
		g_useless_variable = 0;
	}

	/* read offset table from file */
	_read(handle, buf, 800);

	if ((signed long)(g_gendat_offset = get_archive_offset((char*)g_fnames_g105de[index], buf)) != -1) {
		lseek(handle, g_gendat_offset, SEEK_SET);
		return handle;
	} else {
		return 0;
	}
}

/* Borlandified and nearly identical */
/* static */
signed long get_archive_offset(const char *name, unsigned char *table)
{
	signed short i;

	for (i = 0; i < 50; i++) {

		/* check the filename */
		if (!strncmp((char*)name, (char*)table + i * 16, 12)) {

			/* calculate length */
			g_flen_left = g_flen =
				host_readd(table + (i + 1) * 16 + 0x0c) - host_readd(table + i * 16 + 0x0c);

			/* return offset */
			return host_readd(table + i * 16 + 0x0c);
		}
	}

	return -1;
}

/* Borlandified and identical */
signed short read_datfile(signed short handle, unsigned char *buf, unsigned short len)
{
	if (len > (unsigned long)g_flen_left)
		len = (unsigned short)g_flen_left;

	len = _read(handle, buf, len);

	g_flen_left -= len;

	return len;
}

/* Borlandified and identical */
signed long get_filelength(signed short unused)
{
	return g_flen;
}

/* Borlandified and identical */
unsigned short ret_zero1(void)
{
	return 0;
}

/* Borlandified and identical */
void wait_for_keypress(void)
{
	while (CD_bioskey(1)) {
		CD_bioskey(0);
	}
}

/* Borlandified and identical */
void error_msg(const char *msg)
{
	vsync_or_key(print_line(msg) * 150);
}

#if defined(__BORLANDC__)
/* unused */
/* Borlandified and identical */
signed short get_bioskey(void)
{
	return CD_bioskey(0);
}
#endif

/* Borlandified and identical */
void vsync_or_key(signed short val)
{
	signed short i;

	for (i = 0; i < val; i++) {
		handle_input();
		if (g_in_key_ext || g_mouse2_event) {
			g_mouse2_event = 0;
			g_in_key_ext = KEY_RET;
			return;
		}
		wait_for_vsync();
	}
}

#if defined(__BORLANDC__)
/* unused */
/* Borlandified and identical */
void unused_func09(signed short reps)
{
	signed short i;

	for (i = 0; i < reps; i++) {
		wait_for_vsync();
	}
}
#endif

/* seems unused on available input values */
/* Borlandified and identical */
Bit32u swap_u32(Bit32u v)
{
	unsigned short l1;
	unsigned short l2;
	unsigned char *p = (unsigned char*)&l2;

	register unsigned short l_si;

	host_writed(p, v); // write v to stack and access subvalues with l1 and l2
	l_si = l2;
	l2 = swap_u16(l1);
	l1 = swap_u16(l_si);

	return host_readd(p);
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
unsigned long unused_func10(unsigned long v)
{
	unsigned short l1;
	unsigned short l2;
	unsigned short l_si;
	unsigned char *p = (unsigned char*)&l2;

	l_si = host_writed(p, v); // write v to stack and access subvalues with l1 and l2
	l2 = l1;
	l1 = l_si;

	return host_readd(p);
}
#endif

/* Borlandified and identical */
void init_video(signed short unused)
{
	struct struct_color l_white = *(struct struct_color*)&g_col_white2;

	/* set the video mode to 320x200 8bit */
	set_video_mode(0x13);

	set_color((signed char*)&l_white, 0xff);
}

/* Borlandified and identical */
void exit_video(void)
{
	/* restore old mode */
	set_video_mode(g_display_mode_bak);
	/* restore old page */
	set_video_page(g_display_page_bak);
}

#if defined(__BORLANDC__)
/* unused EGA hardware io functions */

/* Borlandified and identical */
void ega_unused1(unsigned char val)
{
	outportb(0x3ce, 5);
	outportb(0x3cf, val);
}

/* Borlandified and identical */
void ega_unused2(unsigned char val)
{
	outportb(0x3c4, 2);
	outportb(0x3c5, val);
}

/* Borlandified and identical */
void ega_unused3(unsigned char val)
{
	outportb(0x3ce, 0);
	outportb(0x3cf, val);
}

/* Borlandified and identical */
void ega_unused4(unsigned char val)
{
	outportb(0x3ce, 1);
	outportb(0x3cf, val);
}

/* Borlandified and identical */
void ega_unused5(unsigned char val)
{
	outportb(0x3ce, 4);
	outportb(0x3cf, val);
}

/* Borlandified and identical */
void ega_unused6(unsigned char val)
{
	outportb(0x3ce, 8);
	outportb(0x3cf, val);
}
#endif

#if defined(__BORLANDC__)
/* Borlandified and identical */
void unused_func11(signed short x1, signed short x2, signed short y, signed short color)
{
	signed short tmp;
	signed short count;
	signed short offset;
	signed short width;

	signed short l_si = x1;
	signed short l_di = x2;
	width = 320;

	if (l_si > l_di) {
		tmp = l_si;
		l_si = l_di;
		l_di = tmp;
	}
	
	count = l_di - l_si + 1;
	/* not very readable, but here the length of the function fits */
	offset = y * width + l_si;
	draw_h_line(offset, count, color);
}
#endif

/* Borlandified and nearly identical */
void draw_v_line(signed short x, signed short y1, signed short y2, unsigned short color)
{
	signed short tmp;
	signed short diffY;
	signed short offset;
	signed short width = 320;

	if (y1 > y2) {
		tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	diffY = y2 - y1 + 1;
	/* not very readable, but here the length of the function fits */
	offset = y1 * width + x;
	draw_h_spaced_dots(offset, diffY, color, width);
}

/* Borlandified and identical */
void do_draw_pic(unsigned short mode)
{
	signed short d1;
	signed short d2;
	signed short v1;
	signed short v2;
	signed short d3;
	signed short d4;
	signed short w;
	signed short h;
	RealPt src;
	RealPt dst;

	register signed short x;
	register signed short y;

	x = g_dst_x1;
	y = g_dst_y1;

	d1 = g_dst_x2;
	d2 = g_dst_y2;
	v1 = g_unkn1;
	v2 = g_unkn2;
	d3 = g_unkn3;
	d4 = g_unkn4;

	w = d1 - x + 1;
	h = d2 - y + 1;

	src = g_dst_src;
	dst = g_dst_dst;

	update_mouse_cursor();

	pic_copy(dst, x, y, d1, d2, v1, v2, d3, d4, w, h, src, mode);

	call_mouse();
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
void unused_func12(void)
{
	signed short diffX;
	signed short diffY;
	signed short dx2;
	signed short dy2;
	unsigned char *src;
	unsigned char HUGEPTR *dst;

	signed short x1 = g_dst_x1; // si
	signed short y1 = g_dst_y1; // di

	dx2 = g_dst_x2;
	dy2 = g_dst_y2;
	src = g_dst_src;
	dst = g_dst_dst;

	dst += y1 * 320 + x1;

	diffX = dx2 - x1 + 1;
	//diffY = dy2 - y1 + 1;


	save_rect(FP_SEG(dst), FP_OFF(dst), src, diffX, (diffY = dy2 - y1 + 1));
}
#endif

/* Borlandified and identical */
void call_fill_rect_gen(unsigned char *ptr, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	signed short width;
	signed short height;

	width = x2 - x1 + 1;
	height = y2 - y1 + 1;
	ptr += y1 * 320 + x1;

#if defined(__BORLANDC__)
	fill_rect(FP_SEG(ptr), FP_OFF(ptr), color, width, height);
#else
	fill_rect(ptr, color, width, height);
#endif
}

/* Borlandified and identical */
void wait_for_vsync(void)
{
#if defined(__BORLANDC__)
	outportb(0x3d4, 0x11);
	_AL = inportb(0x3d5);
	_AH = 0;
	_BX = _AX;
	_BX &= 0xffdf;
	outportb(0x3d4, 0x11);
	outportb(0x3d5, _BL);

	do {
		_AL = inportb(0x3da);
		_AH = 0;
		_BX = _AX;
	} while (_BX & 0x8);

	do {
		_AL = inportb(0x3da);
		_AH = 0;
		_BX = _AX;
	} while (!(_BX & 0x8));

#endif
}

/* Borlandified and identical */
/* static */
void blit_smth3(unsigned char *ptr, signed short v1, signed short v2)
{
	unsigned char *src;
	signed short i;
	signed short j;

	src = g_array_2;

	for (i = 0; i < v1; src += 8 - v2, ptr += 320, i++)
		for (j = 0; j < v2; src++, j++)
			ptr[j] = *src;
}

/**
 * str_splitter() - sets the line breaks for a string
 * @s:	string
 *
 * Returns the number of lines the string needs.
 */
/* Borlandified and identical */
/* static */
unsigned short str_splitter(const char *s)
{
	char *tp;
	signed short unknown_var1;
	signed short lines;
	signed short c_width;
	signed short l_width;

	signed short last_space; //di
	signed short i; //si

	lines = 1;

	if (!s) {
		return 0;
	}

	/* replace all CR and LF with spaces */
	for (tp = (char*)s; *tp; tp++) {
		if ((*tp == 0x0d) || (*tp == 0x0a))
		{
			*tp = ' '; //0x20;
		}
	}

	tp = (char*)s;

	i = last_space = unknown_var1 = 0;

	for (l_width = 0; tp[i] != 0; i++) {

		get_chr_info(tp[i], &c_width);
		l_width += c_width;

		if (l_width >= g_text_x_end) {
			if (last_space != unknown_var1) {
				tp[last_space] = 0x0d;
				tp = tp + last_space;
			} else {
				tp[i] = 0x0d;
				tp = &tp[i + 1];
			}
			lines++;
			unknown_var1 = i = last_space = l_width = 0;
		}

		if (tp[i] == 0x20) {
			last_space = i;
		}

		if (tp[i] == 0x40) {
			tp = &tp[i + 1];
			i = -1;
			unknown_var1 = last_space = l_width = 0;
			lines++;
		}
	}

	if (l_width >= g_text_x_end) {
		if (unknown_var1 == last_space) {
			tp[i-1] = 0;
		} else {
			tp[last_space] = 0x0d;
			lines++;
		}
	}

	return lines;
}

/* Borlandified and identical */
/* static */
unsigned short print_line(const char *str)
{
	unsigned short lines = 1;

	update_mouse_cursor();

	lines = str_splitter(str);

	print_str(str, g_text_x, g_text_y);

	call_mouse();

	return lines;
}

/* Borlandified and identical */
/* static */
void print_str(const char *str, signed short x, signed short y)
{
	signed short i;
	signed short x_bak;
	unsigned char c;

	i = 0;

	update_mouse_cursor();

	if (g_fg_color[4] == 1) x = get_line_start_c(str, x, g_text_x_end);
	x_bak = x;

	while ((c = str[i++])) {
		if ((c == 0x0d) || (c == 0x40)) {
			/* newline */
			y += 7;

			x = (g_fg_color[4] == 1) ? get_line_start_c(str + i, g_text_x, g_text_x_end) : x_bak;

		} else if (c == 0x7e) {
			/* CRUFT */
			if (x < g_ro_var[0]) {
				x = g_ro_var[0];
				continue;
			}
			if (x < g_ro_var[1]) {
				x = g_ro_var[1];
				continue;
			}
			if (x < g_ro_var[2]) {
				x = g_ro_var[2];
				continue;
			}
			if (x < g_ro_var[3]) {
				x = g_ro_var[3];
				continue;
			}
			if (x < g_ro_var[4]) {
				x = g_ro_var[4];
				continue;
			}
			if (x < g_ro_var[5]) {
				x = g_ro_var[5];
				continue;
			}
			if (x < g_ro_var[6]) {
				x = g_ro_var[6];
				continue;
			}
		} else if ((c == 0xf0) || (c == 0xf1) || (c == 0xf2) || (c == 0xf3)) {
			/* change text color */
			g_col_index = c - 0xf0;
		} else {
			/* print normal */
			x += print_chr(c, x, y);
		}
	}

	call_mouse();
}


/* Borlandified and identical */
signed short print_chr(unsigned char c, signed short x, signed short y)
{
	signed short width;
	signed short idx;

	idx = get_chr_info(c, &width);

	call_them_all(idx, width, x, y);

	return width;
}

/**
 * get_chr_info() - gets font information of a character
 * @c:		the character
 * @width:	pointer to save width
 *
 * Returns the font index.
 */
/* Borlandified and identical */
signed short get_chr_info(unsigned char c, signed short *width)
{
	signed short i;

	for (i = 0; i != 222; i += 3) {
		/* search for the character */
		if (*(&g_chr_lookup[0].chr + i) == c) {

			*width = *(&g_chr_lookup[0].width + i) & 0xff;
			return *(&g_chr_lookup[0].index + i) & 0xff;
		}
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		return *width = 0;
	} else {
		*width = 6;
		return 0;
	}
}

/* Borlandified and identical */
/* static */
void call_them_all(signed short v1, signed short v2, signed short x, signed short y)
{
	RealPt gfx_ptr;
	signed short l2;
	signed long bogus;

	fill_smth();
	fill_smth2(v1 * 8 + g_buffer_font6);

	gfx_ptr = get_gfx_ptr(x, y, &l2);
	bogus = (signed long)ret_zero(v2, l2);

	call_blit_smth3(gfx_ptr, 7, (signed short)bogus, l2, v2);
}

/* Borlandified and identical */
/* static */
void fill_smth(void)
{
	unsigned char *ptr;
	signed short i;
	signed short j;

	if (g_mask_switch != 0)
		ptr = g_array_1;
	else
		ptr = g_array_2;

	for (i = 0; i < 8; ptr += 8, i++)
		for (j = 0; j < 8; j++)
			ptr[j] = (unsigned char)g_bg_color;
}

/* Borlandified and identical */
/* static */
void fill_smth2(unsigned char* sptr)
{
	unsigned char *ptr;
	signed short i;
	signed short j;
	unsigned char mask;

	if (g_mask_switch != 0)
		ptr = g_array_1;
	else
		ptr = g_array_2;

	for (i = 0; i < 8; ptr += 8, i++) {
		mask = *sptr++;
		for (j = 0; j < 8; j++) {
			if ((0x80 >> j) & mask) {
				ptr[j] = (unsigned char)g_fg_color[g_col_index];
			}
		}
	}
}

/* Borlandified and identical */
/* static */
RealPt get_gfx_ptr(signed short x, signed short y, signed short* unused)
{
	RealPt start;
	return start = g_gfx_ptr + (y * 320 + x);
}

/* Borlandified and identical */
/* static */
signed short ret_zero(signed short unused1, signed short unused2)
{
	return 0;
}

/* Borlandified and identical */
/* static */
void call_blit_smth3(RealPt dst, signed short v1, signed short v2, signed short v3, signed short v4)
{
	blit_smth3(dst, v1, v4);
}

/* Borlandified and identical */
/* static */
void set_textcolor(signed short fg, signed short bg)
{
	g_fg_color[0] = fg;
	g_bg_color = bg;
}

/* Borlandified and identical */
/* static */
void get_textcolor(signed short *p_fg, signed short *p_bg)
{
	host_writew((unsigned char*)p_fg, g_fg_color[0]);
	host_writew((unsigned char*)p_bg, g_bg_color);
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
/* static */
signed short count_linebreaks(unsigned char *ptr)
{
	signed short i = 0;
	
	while (*ptr) {
		if (*ptr++ == 0x0d) {
			i++;
		}
	}

	return i;
}
#endif

/* Borlandified and identical */
signed short get_str_width(char *str)
{
	signed short sum = 0;
	signed short width;

	while (*str) {
		get_chr_info(*str++, &width);
		sum += width;
	}

	return sum;
}

/**
 * get_line_start_c() - calculates the start positon for a centered line
 * @str:	the string
 * @x:		start position of the string
 * @x_max:	end position of the string
 *
 * Returns the X coordinate where the strin must start.
 */
/* Borlandified and identical */
signed short get_line_start_c(const char *str, signed short x, signed short x_max)
{
	signed short width;

	register signed short pos_x;	// si
	register signed short val;	// di
	
	for (pos_x = 0; ((val = *str) && (val != 0x40) && (val != 0x0d)); )
	{
		get_chr_info(*str++, &width);
		pos_x += width;
	}

	x_max -= pos_x;
	x_max >>= 1;
	x_max += x;
	// readable: xmax = (x_max - pos_x) / 2 + x;
	return x_max;
}

/* Borlandified and nearly identical */
signed short enter_string(char *dst, signed short x, signed short y, signed short num, signed short zero)
{
	signed short pos;
	signed short c;
	signed short width;

	signed short di;
	register signed short si;

	update_mouse_cursor();
	di = x;
	pos = 0;

	if (zero == 0) {
		for (si = 0; si < num; si++) {
			print_chr(0x20, di, y);
			print_chr(0x5f, di, y);
			di += 6;
		}
		di = x;
	} else {
		print_chr(0x20, di, y);
		print_chr(0x5f, di, y);
	}
	wait_for_keypress();
	g_mouse1_event1 = 0;

	c = 0;
	while ((c != 0xd) || (pos == 0)) {
		do {
			do {} while (!CD_bioskey(1) && (g_mouse1_event1 == 0));

			if (g_mouse1_event1) {
				g_in_key_ascii = 0x0d;
				g_mouse1_event1 = g_mouse1_event2 = 0;
			} else {
				g_in_key_ext = (g_in_key_ascii = CD_bioskey(0)) >> 8;
				g_in_key_ascii &= 0xff;
			}
		} while ((g_in_key_ext == 0) && (g_in_key_ascii == 0));

		c = g_in_key_ascii;

		if (c == 0xd)
			continue;

		if (g_in_key_ext == KEY_ESC) {
			*dst = 0;
			call_mouse();
			g_in_key_ext = 0;
			return 1;
		}

		if (c == 8) {
			if (pos <= 0)
				continue;

			if (zero == 1 && pos != num)
				print_chr(0x20, di, y);
			pos--;
			dst--;
			get_chr_info(*dst, &width);

			di -= (zero != 0) ? width : 6;

			print_chr(0x20, di, y);
			print_chr(0x5f, di, y);
		} else {
			/* isalnum(c) */
			if (!(isalnum(c)) &&
				(((unsigned char)c) != 0x84) && (((unsigned char)c) != 0x94) &&
				(((unsigned char)c) != 0x81) && (((unsigned char)c) != 0x8e) &&
				(((unsigned char)c) != 0x99) && (((unsigned char)c) != 0x9a) &&
				(c != 0x20) && (c != 0x2e))
					continue;

			/* isalpha(c) */
			if (isalpha(c))
				c = toupper(c);

			/* ae */
			if ((unsigned char)c == 0x84)
				c = (signed short)0xff8e;
			/* oe */
			if ((unsigned char)c == 0x94)
				c = (signed short)0xff99;
			/* ue */
			if ((unsigned char)c == 0x81)
				c = (signed short)0xff9a;

			/* are we at the end of the input field */
			if (pos == num) {
				dst--;

				get_chr_info(*dst, &width);

				di -= (zero != 0) ? width : 6;

				pos--;
			}

			*dst++ = (unsigned char)c;
			print_chr(0x20, di, y);
			print_chr((unsigned char)c, di, y);
			get_chr_info((unsigned char)c, &width);

			di += (zero != 0) ? width : 6;

			pos++;

			if ((zero == 1) && (pos != num)) {
				print_chr(0x20, di, y);
				print_chr(0x5f, di, y);
			}
		}
	}

	/* OK from here */
	if (zero == 0) {
		while (pos < num) {
			print_chr(0x20, di, y);
			di += 6;
			pos++;
		}
	}

	*dst = 0;
	call_mouse();

	return 0;
}

/* Borlandified and identical */
void draw_popup_line(signed short line, signed short type)
{
	unsigned char *dst;
	unsigned char *src;
	signed short i;
	signed short popup_right;

	register signed short popup_left;   // si
	register signed short popup_middle; // di

	/* This is a bit bogus */
	dst = g_vga_memstart;

	/* (line * 8 + y) * 320  + x */
	dst = (g_vga_memstart) + 320 * (g_upper_border + 8 * line) + g_left_border;

	switch (type) {
		case 0: {
			popup_left = 0;
			popup_middle = 0x380;
			popup_right = 0x80;
			break;
		}
		case 1: {
			popup_left = 0x100;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		}
		case 2: {
			popup_left = 0x200;
			popup_middle = 0x480;
			popup_right = 0x180;
			break;
		}
		case 3: {
			popup_left = 0x280;
			popup_middle = 0x580;
			popup_right = 0x300;
			break;
		}
	}

	src = g_buffer_popup_nvf + popup_left;
	copy_to_screen(src, dst, 16, 8, 0);

	src = g_buffer_popup_nvf + popup_middle;
	dst += 16;
	for (i = 0; i < g_menu_tiles; dst += 32, i++)
		copy_to_screen(src, dst, 32, 8, 0);

	src = g_buffer_popup_nvf + popup_right;
	copy_to_screen(src, dst, 16, 8, 0);
}

/**
 *	infobox() - draws and info- or enter_numberbox
 *	@msg:		the message for the box
 *	@digits:	number of digits to enter
 *
 *	if @digits is zero the function just delays.
 */
/* Borlandified and identical */
signed short infobox(const char *msg, signed short digits)
{
	RealPt src;
	RealPt dst;
	signed short retval;
	signed short fg;
	signed short bg;
	signed short v2;
	signed short v3;
	signed short v4;
	signed short i;

	signed short lines; // si
	signed short di;    // di
		      //
#if !defined(__BORLANDC__)
	/* Issue: Try to print an infobox without having the background loaded */
	if (!g_buffer_popup_nvf) {
		fprintf(stderr, "in %s(): %s => copy DSAGEN.DAT in this directory\n", __func__, msg);
		exit(-1);
	}
#endif

	retval = 0;
	g_fg_color[4] = 1;
	v2 = g_text_x;
	v3 = g_text_y;
	v4 = g_text_x_end;

	di = 32 * g_menu_tiles + 32;
	g_text_x = (g_left_border = ((320 - di) / 2 + g_text_x_mod)) + 5;
	g_text_x_end = di - 10;
	lines = str_splitter(msg);

	if (digits != 0)
		lines += 2;

	g_upper_border = (200 - (lines + 2) * 8) / 2;
	g_upper_border += g_ro_zero;
	g_text_y = g_upper_border + 7;

	update_mouse_cursor();

	src = g_vga_memstart;
	src += g_upper_border * 320 + g_left_border;
	dst = g_gen_ptr1_dis;

	copy_to_screen(src, dst, di, (lines + 2) * 8, 2);

	/* draw the popup box */
	draw_popup_line(0, 0);

	for (i = 0; i < lines; i++)
		draw_popup_line(i + 1, 1);

	draw_popup_line(lines + 1, 3);

	get_textcolor((signed short*)&fg, (signed short*)&bg);
	set_textcolor(0xff, 0xdf); // WHITE ON GREEN

	print_line(msg);

	g_mouse2_event = 0;
	call_mouse();

	if (digits) {
		enter_string(g_gen_ptr3,
			g_left_border + (di - digits * 6) / 2,
			g_upper_border + 8 * lines - 2, digits, 0);

		retval = (unsigned short)atol(g_gen_ptr3);
	} else {
		g_action_table = (struct mouse_action*)g_action_input;
		vsync_or_key(150 * lines);
		g_action_table = (struct mouse_action*)NULL;
	}

	set_textcolor(fg, bg);
	update_mouse_cursor();

	dst = g_vga_memstart;
	dst += g_upper_border * 320 + g_left_border;
	src = g_gen_ptr1_dis;

	copy_to_screen(src, dst, di, (lines + 2) * 8, 0);

	call_mouse();

	g_text_x = v2;
	g_text_y = v3;
	g_text_x_end = v4;

	g_fg_color[4] = 0;
	g_in_key_ext = 0;

	return retval;
}


/**
 * gui_bool() - displays a yes - no radio box
 * @header:	the header of menu
 *
 */
/* Borlandified and identical */
signed short gui_bool(char *msg)
{
	signed short retval;

	g_bool_mode = 1;
	retval = gui_radio(msg, 2, get_text(4), get_text(5));
	g_bool_mode = 0;

	if (retval == 1)
		return 1;
	else
		return 0;
}

/**
 * fill_radio_button() - marks the active radio button
 * @old_pos:	the position of the last active button (or -1)
 * @new_pos:	the position of the current active button
 * @offset:	the offset of the first radio line
 *
 */
/* Borlandified and identical */
void fill_radio_button(signed short old_pos, signed short new_pos, signed short offset)
{
	signed short y;

	signed short i;
	signed short x;

	update_mouse_cursor();

	/* unmark the old radio button, if any */
	if (old_pos != -1) {
		y = g_left_border + 6;

		x = g_upper_border + (offset + old_pos) * 8 + 2;

		for (i = 0; i < 4; i++)
			draw_v_line(y + i, x, x + 3, 0xd8);
	}

	/* mark the new radio button */
	y = g_left_border + 6;

	x = g_upper_border + (offset + new_pos) * 8 + 2;

	for (i = 0; i < 4; i++)
		draw_v_line(y + i, x, x + 3, 0xd9);

	call_mouse();
}

/**
 * gui_radio() - displays a radio menu
 * @header:	the header of the menu
 * @options:	the number of options
 *
 */
/* Borlandified and nearly identical */
signed short gui_radio(char *header, signed char options, ...)
{
	va_list arguments;
	char *str;
	signed short r3;
	signed short r4;
	signed short r5;
	signed short retval;
	signed short lines_sum;
	signed short lines_header;
	signed short r6;
	signed short fg_bak;
	signed short bg_bak;
	signed short bak1;
	signed short bak2;
	signed short bak3;
	RealPt src;
	RealPt dst;
	signed short mx_bak;
	signed short my_bak;
	signed short r7;
	signed short r8;
	signed short r9;

	signed short i;
	signed short di;

	r5 = 0;
	r6 = -1;
	di = 1;

	bak1 = g_text_x;
	bak2 = g_text_y;
	bak3 = g_text_x_end;
	r9 = 32 * g_menu_tiles + 32;
	g_text_x = (g_left_border = (((320 - r9) / 2) + g_text_x_mod)) + 5;
	g_text_x_end = 32 * g_menu_tiles + 22;
	lines_header = str_splitter((char*)header);
	lines_sum = lines_header + options;
	g_text_y = (g_upper_border = ((200 - (lines_sum + 2) * 8) / 2)) + 7;
	update_mouse_cursor();

	/* save old background */
	src = g_vga_memstart;
	src += g_upper_border * 320 + g_left_border;
	dst = g_gen_ptr1_dis;

	copy_to_screen(src, dst, r9, (lines_sum + 2) * 8, 2);

	/* draw popup */
	draw_popup_line(0, 0);
	for (i = 0; i < lines_header; i++)
		draw_popup_line(i + 1, 1);
	for (i = 0; options > i; i++)
		draw_popup_line(lines_header + i + 1, 2);
	draw_popup_line(lines_sum + 1, 3);

	/* save and set text colors */
	get_textcolor((signed short*)&fg_bak, (signed short*)&bg_bak);
	set_textcolor(0xff, 0xdf); // WHITE ON GREEN

	/* print header */
	if (lines_header)
		print_line((char*)header);

	r3 = g_text_x + 8;
	r4 = g_upper_border + 8 * (lines_header + 1);

	/* print radio options */
	va_start(arguments, options);
	for (i = 1; i <= options; r4 += 8, i++) {
		str = va_arg(arguments, char*);
		print_str(str, r3, r4);
	}
	va_end(arguments);

	/* save and set mouse position */
	mx_bak = g_mouse_posx;
	my_bak = g_mouse_posy;
	g_mouse_posx_bak = g_mouse_posx = g_left_border + 90;
	g_mouse_posy_bak = g_mouse_posy = r8 = r7 = g_upper_border + 8 * (lines_header + 1);

	mouse_move_cursor(g_mouse_posx, g_mouse_posy);

	g_mouse_posx_max = g_left_border + r9 - 16;
	g_mouse_posx_min = g_left_border;
	g_mouse_posy_min = g_upper_border + 8 * (lines_header + 1);
	g_mouse_posy_max = (g_upper_border + 8 * (lines_header + 1) + 8 * options) - 1;
	call_mouse();
	g_mouse2_event = 0;

	while (r5 == 0) {
		g_action_table = (struct mouse_action*)g_action_input;
		handle_input();
		g_action_table = (struct mouse_action*)NULL;

		if (r6 != di) {
			fill_radio_button(r6, di, lines_header);
			r6 = di;
		}
		if ((g_mouse2_event != 0) || (g_in_key_ext == KEY_ESC) || (g_in_key_ext == KEY_PGDOWN)) {
			/* has the selection been canceled */
			retval = -1;
			r5 = 1;
			g_mouse2_event = 0;
		}
		if (g_in_key_ext == KEY_RET) {
			/* has the return key been pressed */
			retval = di;
			r5 = 1;
		}
		if (g_in_key_ext == KEY_UP) {
			/* has the up key been pressed */
			if (di == 1)
				di = options;
			else
				di--;
		}
		if (g_in_key_ext == KEY_DOWN) {
			/* has the down key been pressed */
			if (di == options)
				di = 1;
			else
				di++;
		}
		if (g_mouse_posy != r8) {
			/* has the mouse been moved */
			di = ((r8 = g_mouse_posy) - r7) / 8 + 1;
		}
		/* is this a bool radiobox ? */
		if (g_bool_mode) {
			if (g_in_key_ext == KEY_Y) {
				/* has the 'j' key been pressed */
				retval = 1;
				r5 = 1;
			} else if (g_in_key_ext == KEY_N) {
				/* has the 'n' key been pressed */
				retval = 2;
				r5 = 1;
			}
		}
	}

	update_mouse_cursor();

	g_mouse_posx_bak = g_mouse_posx = mx_bak;
	g_mouse_posy_bak = g_mouse_posy = my_bak;

	g_mouse_posx_max = 319;
	g_mouse_posx_min = 0;
	g_mouse_posy_min = 0;
	g_mouse_posy_max = 199;

	mouse_move_cursor(g_mouse_posx, g_mouse_posy_bak);

	dst = g_vga_memstart;
	dst += g_upper_border * 320 + g_left_border;
	src = g_gen_ptr1_dis;
	copy_to_screen(src, dst, r9, (lines_sum + 2) * 8, 0);

	call_mouse();

	set_textcolor(fg_bak, bg_bak);

	g_text_x = bak1;
	g_text_y = bak2;
	g_text_x_end = bak3;
	g_in_key_ext = 0;

	return retval;
}

/**
 * enter_name() - enter the name of a hero
 */
/* Borlandified and identical */
void enter_name(void)
{
	RealPt dst;

	dst = g_vga_memstart + 12 * 320 + 176;

	update_mouse_cursor();
	copy_to_screen(g_picbuf1, dst, 94, 8, 0);
	enter_string(g_hero.name, 180, 12, 15, 1);
	copy_to_screen(g_picbuf1, dst, 94, 8, 0);
	call_mouse();
	print_str(g_hero.name, 180, 12);
}

/* Borlandified and identical */
void change_head(void)
{
	struct nvf_desc nvf;
	signed short width;
	signed short height;

	nvf.dst = g_gen_ptr6;
	nvf.src = g_buffer_heads_dat;
	nvf.no = g_head_current;
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;

	process_nvf(&nvf);

	g_dst_src = g_gen_ptr6;

	g_dst_x1 = 272;
	g_dst_x2 = 303;

	if (g_gen_page == 0) {
		g_dst_y1 = 8;
		g_dst_y2 = 39;
		do_draw_pic(0);
	} else if (g_gen_page > 4) {
		g_dst_y1 = 4;
		g_dst_y2 = 35;
		do_draw_pic(0);
	}
}

/**
 * change_sex() - changes the sex of the hero
 *
 */
/* Borlandified and identical */
void change_sex(void)
{
	RealPt dst;
	unsigned char* src;

	/* change sex of the hero */
	g_hero.sex ^= 1;

	/* hero has a typus */
	if (g_hero.typus) {
		if (g_hero.sex != 0) {
			/* To female */
			g_head_first = g_head_current = g_head_first_female[g_head_typus];
			g_head_last = g_head_first_male[g_head_typus + 1] - 1;
		} else {
			/* To male */
			g_head_first = g_head_current = g_head_first_male[g_head_typus];
			g_head_last = g_head_first_female[g_head_typus] - 1;
		}
		g_screen_var = 1;
		return;
	} else {
		dst = g_vga_memstart + 7 * 320 + 305;
		src = g_buffer_sex_dat + 256 * g_hero.sex;
		update_mouse_cursor();
		copy_to_screen(src, dst, 16, 16, 0);
		call_mouse();
	}
}

/* Borlandified and identical */
void do_gen(void)
{
	signed short si;
	signed short di;

	di = 0;

	g_screen_var = 1;

	/* try to set the level from parameters */
	g_level = ((g_param_level == 'a') ? 2 : ((g_param_level == 'n') ? 1 : -1));

	/* ask for level */
	while (g_level == -1) {
		g_level = gui_radio(get_text(0), 2, get_text(1), get_text(2));
	}

	g_mouse2_event = 1;

	/* main loop */
	while (!di) {
		if (g_screen_var) {
			refresh_screen();
			g_screen_var = 0;
		}

		g_action_table = (struct mouse_action*)g_action_page[g_gen_page];
		handle_input();
		g_action_table = (struct mouse_action*)NULL;

		if (g_mouse2_event || g_in_key_ext == KEY_PGUP) {
			/* print the menu for each page */
			switch (g_gen_page) {
				case 0: {
					si = gui_radio(get_text(7), 9,
						get_text(10), get_text(11), get_text(15),
						get_text(8),  get_text(14), get_text(12),
						get_text(262),get_text(9),  get_text(258));

					if (si != -1) {
						if ((si >= 4) && (si < 6) && (g_hero.attrib[0].normal) && !gui_bool(get_text(13))) {
							si = 0;
						}
						g_in_key_ext = 0;
						switch (si) {
							case 1: {
								enter_name();
								break;
							}
							case 2: {
								change_sex();
								break;
							}
							case 3: {
								change_attribs();
								break;
							}
							case 4: {
								memset(&g_hero, 0, sizeof(g_hero));
								clear_hero();
								g_mouse2_event = 1;
								g_screen_var = 1;
								break;
							}
							case 5: {
								new_values();
								break;
							}
							case 6: {
								select_typus();
								break;
							}
							case 7: {
								choose_typus();
								break;
							}
							case 8: {
								save_chr();
								break;
							}
							case 9: {
								if (gui_bool(get_text(259)))
									di = 1;
								break;
							}
						}
					}
					break;
				}
				case 1:
				case 2:
				case 3: {
					select_skill();
					break;
				}
				case 4: {
					choose_atpa();
					break;
				}
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10: {
					select_spell();
					break;
				}
			}
		}

		if (g_in_key_ext == KEY_CTRL_F3)
			change_sex();

		if (g_in_key_ext == KEY_CTRL_F4)
			enter_name();

		if ((g_in_key_ext == KEY_UP) && (g_gen_page == 0)) {
			if (!g_hero.typus) {
				infobox(get_text(17), 0);
			} else {
				if (g_head_current < g_head_last) {
					g_head_current++;
				} else {
					g_head_current = g_head_first;
				}
				change_head();
			}
		}

		if ((g_in_key_ext == KEY_DOWN) && (g_gen_page == 0)) {
			if (!g_hero.typus) {
				infobox(get_text(17), 0);
			} else {
				if (g_head_current > g_head_first) {
					g_head_current--;
				} else {
					g_head_current = g_head_last;
				}
				change_head();
			}
		}

		if ((g_in_key_ext == KEY_RIGHT) && (g_level != 1)) {
			if (!g_hero.typus) {
				infobox(get_text(72), 0);
			} else {
				g_screen_var = 1;

				if (((g_hero.typus < 7) ? 4 : 10) > g_gen_page) {
					g_gen_page++;
				} else {
					g_gen_page = 0;
				}
			}
		}

		if (g_in_key_ext == KEY_LEFT) {
			if (g_gen_page > 0) {
				g_screen_var = 1;
				g_gen_page--;
			} else {
				if (g_level != 1) {

					if (!g_hero.typus) {
						infobox(get_text(72), 0);
					} else {
						g_screen_var = 1;
						g_gen_page = (g_hero.typus < 7 ? 4 : 10);
					}
				}
			}
		}

		if ((g_in_key_ext >= KEY_1) && (g_in_key_ext <= KEY_5) &&
			(g_level == 2) && g_hero.typus) {

			si = ((g_in_key_ext == KEY_1) ? 0 : (
				(g_in_key_ext == KEY_2) ? 1 : (
				(g_in_key_ext == KEY_3) ? 4 : (
				(g_in_key_ext == KEY_4) ? 5 : 10))));

			if ((si != g_gen_page) && (si < 5 || g_hero.typus >= 7)) {
				g_gen_page = si;
				g_screen_var = 1;
			}
		}
#if !defined(__BORLANDC__)
		if (g_lets_quit == 1) di = 1;
#endif
	}
}

/* Borlandified and identical */
void refresh_screen(void)
{
	RealPt src;
	RealPt dst;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	if (g_screen_var) {
		g_gfx_ptr = g_gen_ptr1_dis;
		load_page(g_gen_page);
		save_picbuf();

		/* page with base values and hero is not male */
		if ((g_gen_page == 0) && (g_hero.sex != 0)) {

			dst = g_gen_ptr1_dis + 7 * 320 + 305;
			src = g_buffer_sex_dat + 256 * g_hero.sex;
			copy_to_screen(src, dst, 16, 16, 0);
		}

		/* page with base values and level is advanced */
		if ((g_gen_page == 0) && (g_level == 1)) {
			dst = g_gen_ptr1_dis + 178 * 320 + 284;

			src = g_buffer_sex_dat + 512;

			copy_to_screen(src, dst, 20, 15, 0);
		}
		/* if the page is lower than 5 */
		if (g_gen_page < 5) {
			/* draw DMENGE.DAT or the typus name */
			dst = g_gen_ptr1_dis + 8 * 320 + 16;

			if (g_hero.typus != 0) {

				g_need_refresh = 1;
				copy_to_screen(g_gen_ptr5, dst, 128, 184, 0);

				if (g_hero.sex != 0) {
					print_str(get_text(271 + g_hero.typus),
						get_line_start_c(get_text(271 + g_hero.typus), 16, 128),
						184);
				} else {
					print_str(get_text(17 + g_hero.typus),
						get_line_start_c(get_text(17 + g_hero.typus), 16, 128),
						184);
				}

			} else {
				if (g_need_refresh) {
					call_fill_rect_gen((RealPt)g_vga_memstart, 16, 8, 143, 191, 0);
					g_need_refresh = 0;
				}

				wait_for_vsync();
				set_palette((signed char*)g_buffer_dmenge_dat + 128 * 184 + 2, 0 , 32);
				copy_to_screen(g_buffer_dmenge_dat, dst, 128, 184, 0);
			}
		}
		/* if hero has a typus */
		if (g_hero.typus != 0) {
			/* draw the head */

			nvf.dst = g_gen_ptr6;
			nvf.src = g_buffer_heads_dat;
			nvf.no = g_head_current;
			nvf.type = 0;
			nvf.width = &width;
			nvf.height = &height;
			process_nvf(&nvf);

			g_dst_src = g_gen_ptr6;
			g_dst_x1 = 272;
			g_dst_x2 = 303;
			g_dst_dst = g_gen_ptr1_dis;

			/* draw the head */
			if (g_gen_page == 0) {
				/* on the base page */
				g_dst_y1 = 8;
				g_dst_y2 = 39;
				do_draw_pic(0);
			} else if (g_gen_page > 4) {
				/* on the spell pages */
				g_dst_y1 = 4;
				g_dst_y2 = 35;
				do_draw_pic(0);
			}

			g_dst_dst = g_vga_memstart;

		}

		print_values();
		dst = g_gfx_ptr = g_vga_memstart;
		src = g_gen_ptr1_dis;
		update_mouse_cursor();
		copy_to_screen(src, dst, 320, 200, 0);
		call_mouse();
	} else {
		print_values();
	}
}

/* Borlandified and identical */
/* static */
void clear_hero(void)
{
	signed short i;

	g_got_mu_bonus = g_got_ch_bonus = 0;

	g_head_typus = g_head_first = g_head_last = g_head_current = 0;

	for (i = 0; i < 14; i++)
		g_attrib_changed[i] = 0;

	for (i = 0; i < 86; i++) {
		g_spell_incs[i].tries = g_spell_incs[i].incs = 0;
	}
	for (i = 0; i < 52; i++) {
		g_skill_incs[i].tries = g_skill_incs[i].incs = 0;
	}

	g_hero.level = 1;
}

/**
 * new_values() - roll out new attribute values
 *
 */
/* Borlandified and nearly identical */
void new_values(void)
{
	/* Original-Bugfix:	there once was a char[11],
				which could not hold a char[16] */
	signed char *att_ptr;
	signed char randval;
	signed char unset_attribs;
	signed char values[8];
	signed char sex_bak;
#if !defined(__BORLANDC__)
	char name_bak[17];
#else
	char name_bak[10];
#endif
	signed short j;
	signed short i;

	signed short di;

	/* set variable if hero has a typus */
	if (g_hero.typus)
		g_screen_var = 1;

	/* save the name of the hero */
	/* TODO strncpy() would be better here */

	strcpy(name_bak, g_hero.name);

	/* save the sex of the hero */
	sex_bak = g_hero.sex;

	/* clear the hero */
	memset(&g_hero, 0, sizeof(g_hero));

	clear_hero();

	g_hero.sex = sex_bak;

	/* restore the name of the hero */
	/* TODO strncpy() would be better here */

	strcpy(g_hero.name, name_bak);

	refresh_screen();

	g_screen_var = 0;

	att_ptr = &g_hero.attrib[0].normal;

	for (j = 0; j < 7; j++) {
		randval = (signed char)random_interval_gen(8, 13);
		unset_attribs = 0;

		for (i = 0; i < 7; i++) {
			// NORMAL
			if (!att_ptr[ 3 * i]) {
                                // not initialized attribute
				values[unset_attribs] = (signed char)i;
				g_type_names[unset_attribs] = get_text(32 + i);
				unset_attribs++;
			}
		}

		sprintf(g_gen_ptr2, get_text(46), randval);

		do {
			g_text_x_mod = -80;

			di = gui_radio(g_gen_ptr2,
				unset_attribs,
				g_type_names[0], g_type_names[1], g_type_names[2],
				g_type_names[3], g_type_names[4], g_type_names[5],
				g_type_names[6]);

			g_text_x_mod = 0;

		} while (di == -1);

		di = values[di - 1];
		/* write randval to the selected positive attribute */
		//g_hero.attrib[di].normal = g_hero.attrib[di].current = randval;
		att_ptr[3 * di + 0] = att_ptr[3 * di + 1] = randval;

		update_mouse_cursor();
		refresh_screen();
		call_mouse();
	}

	att_ptr = &g_hero.attrib[7].normal;

	for (j = 0; j < 7; j++) {
		randval = (signed char)random_interval_gen(2, 7);
		unset_attribs = 0;

		for (i = 0; i < 7; i++) {
			// NORMAL
			if (!att_ptr[3 * i]) {
				values[unset_attribs] = (signed char)i;
				g_type_names[unset_attribs] = get_text(39 + i);
				unset_attribs++;
			}
		}

		sprintf(g_gen_ptr2, get_text(46), randval);

		do {
			g_text_x_mod = -80;

			di = gui_radio(g_gen_ptr2,
				unset_attribs,
				g_type_names[0], g_type_names[1], g_type_names[2],
				g_type_names[3], g_type_names[4], g_type_names[5],
				g_type_names[6]);

			g_text_x_mod = 0;

		} while (di == -1);

		di = values[di - 1];

		/* write randval to the selected negative attribute */
		//g_hero.attrib[di].normal = g_hero.attrib[di].current = randval;
		att_ptr[3 * di + 0] = att_ptr[3 * di + 1] = randval;

		update_mouse_cursor();
		refresh_screen();
		call_mouse();
	}
}

/**
 * calc_at_pa() - calculate AT and PA values
 */
/* Borlandified and identical */
/* static */
void calc_at_pa(void)
{
	div_t res; // BCC <STDLIB.H>
	signed short tmp;
	signed short i;

	res = div(g_hero.attrib[5].normal + g_hero.attrib[6].normal + g_hero.attrib[4].normal, 5);
	/* round up if neccessary */
	if (res.rem >= 3) {
		res.quot++;
	}

	g_hero.atpa_base = res.quot;

	for (i = 0; i < 7; i++) {
		/* Set base AT/PA value for each weapon */
		g_hero.at_weapon[i] = g_hero.pa_weapon[i] = g_hero.atpa_base;

		if (g_hero.skills[i] < 0) {
			tmp = __abs__(g_hero.skills[i]) / 2;

			/* Calculate weapon AT value */
			g_hero.at_weapon[i] -= tmp;

			/* Calculate weapon PA value */
			g_hero.pa_weapon[i] -= tmp;

			if (__abs__(g_hero.skills[i]) != 2 * tmp) {
				g_hero.pa_weapon[i]--;
			}
		} else {
			/* calculate ATPA for positive weapon skill */
			tmp = g_hero.skills[i] / 2;

			/* Calculate weapon AT value */
			g_hero.at_weapon[i] += tmp;

			/* Calculate weapon PA value */
			g_hero.pa_weapon[i] += tmp;

			if (g_hero.skills[i] != 2 * tmp) {
				g_hero.at_weapon[i]++;
			}
		}
	}
}

/**
 * fill_values() - fills the values if typus is chosen
 *
 */
/* Borlandified and nearly identical */
void fill_values(void)
{
	signed short i;
	signed short v1;
	signed short v2;
	const struct struct_money *money_ptr;

	signed short si, di;

	/* fill skill values */
	for (i = 0; i < 52; i++) {

		g_hero.skills[i] = g_skills[g_hero.typus][i];

		/* set skill_incs and skill_tries to zero */
		g_skill_incs[i].tries = g_skill_incs[i].incs = 0;
	}

	/* set skill_attempts */
	g_hero.skill_incs = g_initial_skill_incs[g_hero.typus];

	/* do magic user init */
	if (g_hero.typus >= 7) {
		/* fill initial spell values */
		for (i = 0; i < 86; i++) {
			g_hero.spells[i] = g_spells[g_hero.typus - 7][i];

			/* set spell_incs and spell_tries to zero */
			g_spell_incs[i].tries = g_spell_incs[i].incs = 0;
		}

		/* special mage values */
		if (g_hero.typus == 9) {
			/* set staff spell to level 1 */
			g_hero.staff_level = 1;
			/* select mage school */
			do {
				g_hero.spell_school = gui_radio(get_text(47),
							9,
							get_text(48), get_text(49),
							get_text(50), get_text(51),
							get_text(52), get_text(53),
							get_text(54), get_text(55),
							get_text(56)) - 1;

			} while (g_hero.spell_school == -2);


			/* add magic school modifications */
			for (i = 0; g_house_mod[g_hero.spell_school].no > i; i++) {

				g_hero.spells[g_house_mod[g_hero.spell_school].spells[i]] +=
					g_house_mod[g_hero.spell_school].mod[i];
			}
		}

		/* set spell attempts */
		g_hero.spell_incs = g_initial_spell_incs[g_hero.typus - 7];

		/* get convertable increase attempts */
		if ((di = g_initial_conv_incs[g_hero.typus - 7]) && (g_level == 2) && gui_bool(get_text(269))) {
			/* create string */
			sprintf(g_gen_ptr2, get_text(270), di);

			i = infobox(g_gen_ptr2, 1);

			if (i > 0) {
				/* spell attempts to skill attempts */
				if (i > di)
					i = di;
				di -= i;
				/* change spell attempts */
				g_hero.spell_incs -= i;
				/* change skill attempts */
				g_hero.skill_incs += i;
			} else {

				/* create string */
				sprintf(g_gen_ptr2, get_text(271), di);

				i = infobox(g_gen_ptr2, 1);
				if (i > 0) {
					if (i > di)
						i = di;
					/* change spell attempts */
					g_hero.spell_incs += i;
					/* change skill attempts */
					g_hero.skill_incs -= i;
				}
			}
		}
	}

	/* set LE */
	g_hero.le = g_hero.le_max = g_init_le[g_hero.typus];

	/* set AE */
	g_hero.ae = g_hero.ae_max = g_init_ae[g_hero.typus];

	/* wanna change 10 spell_attempts against 1W6+2 AE ? */
	if ((g_hero.typus == 9) && (g_level == 2) && gui_bool(get_text(268))) {
		/* change spell_attempts */
		g_hero.spell_incs -= 10;
		g_hero.ae_max += (signed short)random_interval_gen(3, 8);
		g_hero.ae = g_hero.ae_max;
	}

	/* roll out size */
	g_hero.height =
		(unsigned char)random_interval_gen(g_height_range[g_hero.typus].min,
						   g_height_range[g_hero.typus].max);

	/* calculate weight i = (height - weight_mod) * 40 */
	g_hero.weight = 40 * ((unsigned short)g_hero.height - g_weight_mod[g_hero.typus]);

	/* roll out the money */
	i = random_gen(20);
	money_ptr = g_money_tab[g_hero.typus];
	for (si = 0; money_ptr[si].value < i; si++);

	g_hero.money = (signed long)(10 * (signed short)random_interval_gen(money_ptr[si].min, money_ptr[si].max));

	/* calculate MR  = (KL + MU + Stufe) / 3 - 2 * AG
	 * 		 = (WD + CO + Level) / 3 - 2 * SN */
	g_hero.mr =
		(g_hero.attrib[1].normal + g_hero.attrib[0].normal + g_hero.level) / 3
			- 2 * g_hero.attrib[7].normal;
	/* add typus MR Modificator */
	g_hero.mr += g_mr_mod[g_hero.typus];

	/* roll out god */
	g_hero.god = random_gen(12);

	/* add gods boni */
	switch (g_hero.god) {
		case 1 : {
			/* Praios: MU + 1 */
			g_hero.attrib[0].current = ++g_hero.attrib[0].normal;
			g_got_mu_bonus = 1;
			break;
		}
		case 2 : {
			/* Rondra: skill swords + 1 */
			g_hero.skills[3]++;
			break;
		}
		case 3 : {
			/* Efferd: skill swim + 1 */
			g_hero.skills[14]++;
			break;
		}
		case 4 : {
			/* Travia: skill treat poison + 1 */
			g_hero.skills[44]++;
			break;
		}
		case 5 : {
			/* Boron: skill human nature + 1 */
			g_hero.skills[24]++;
			break;
		}
		case 6 : {
			/* Hesinde: skill alchemy + 1 */
			g_hero.skills[32]++;
			break;
		}
		case 7 : {
			/* Firun: skills track and missle weapons + 1  */
			g_hero.skills[26]++;
			g_hero.skills[7]++;
			break;
		}
		case 8 : {
			/* Tsa: CH + 1 */
			g_hero.attrib[2].current = ++g_hero.attrib[2].normal;
			g_got_ch_bonus = 1;
			break;
		}
		case 9 : {
			/* Phex: skills hide and pickpocket + 1 */
			g_hero.skills[49]++;
			g_hero.skills[13]++;
			break;
		}
		case 10 : {
			/* Peraine: skills treat disease and wounds + 1 */
			g_hero.skills[45]++;
			g_hero.skills[46]++;
			break;
		}
		case 11 : {
			/* Ingerimm: skill tactics + 1*/
			g_hero.skills[37]++;
			break;
		}
		case 12 : {
			/* Rhaja: skills dance, seduce and instrument + 1*/
			g_hero.skills[20]++;
			g_hero.skills[16]++;
			g_hero.skills[47]++;
			break;
		}
	}
	/* calclate AT and PA values */
	calc_at_pa();

	/* if mode == novice */
	if (g_level == 1) {
		/* increase skills automatically */
		for (i = 0; g_hero.skill_incs > 0; i++) {
			skill_inc_novice(v1 = g_autoskills[g_hero.typus][i]);
		}

		// Okay, till here !

		si = 0;
		/* prepare mage automatic spell list */
		if (g_hero.typus == 9) {
			/* Remark: HERO_TYPUS is equal to 9, g_autospells starts with typus = 7,
			 * so g_autospells[2] is that of the Mage */

			/* 1. house spells */
			for (i = 0; g_house_mod[g_hero.spell_school].no > i; si++, i++) {
				g_autospells[2][si] =
						g_house_mod[g_hero.spell_school].spells[i];
			}
			/* 2. all schools spells */
			for (i = 0; g_school_tab[g_hero.spell_school].spells > i; si++, i++) {
				g_autospells[2][si] =
					g_school_tab[g_hero.spell_school].first_spell + i;
			}
			/* 3. five domination spells */
				/* Herr der Tiere */
			g_autospells[2][si++] = 0x52;
				/* Horriphobus */
			g_autospells[2][si++] = 0x31;
				/* Mag. Raub */
			g_autospells[2][si++] = 0x35;
				/* Respondami */
			g_autospells[2][si++] = 0x21;
				/* Sanftmut */
			g_autospells[2][si++] = 0x4f;

			/* 4. all house spells */
			for (i = 0; g_house_mod[g_hero.spell_school].no > i; si++, i++) {
				g_autospells[2][si] = g_house_mod[g_hero.spell_school].spells[i];
			}
			/* 5. all house spells */
			for (i = 0; g_house_mod[g_hero.spell_school].no > i; si++, i++) {
				g_autospells[2][si] = g_house_mod[g_hero.spell_school].spells[i];
			}
			/* 6. random spells */
			while (si < 45) {
				g_autospells[2][si++] = random_gen(85);
			}
		}

		/* automatic increase spells */
		for (i = 0; g_hero.spell_incs > 0; i++) {
			spell_inc_novice((v2 = g_autospells[g_hero.typus - 7][i]));
		}
	}
#if defined(__BORLANDC__)
	asm {db 0x0f, 0x1f, 0x00 } // BCC Sync-Point
	asm {db 0x0f, 0x1f, 0x00 }
#endif
}

/**
 * skill_inc_novice() - tries to increment a skill in novice mode
 * @skill:	the skill which should be incremented
 *
 *
 */
/* Borlandified and identical */
void skill_inc_novice(signed short skill)
{
	signed short done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (g_skill_incs[skill].tries == 3) {
			/* set the flag to leave this loop */
			done = 1;
#if !defined(__BORLANDC__)
		} else

		/* Original-Bugfix: add check if skill_attempts are left */
		if (g_hero.skill_incs == 0) {
			done++;
#endif
		} else {
			/* decrement counter for skill increments */
			g_hero.skill_incs--;

			/* check if the test is passed */
			if ((signed short)random_interval_gen(2, 12) > g_hero.skills[skill]) {
				/* increment skill */
				g_hero.skills[skill]++;

				/* set inc tries for this skill to zero */
				g_skill_incs[skill].tries = 0;

				/* set the flag to leave this loop */
				done = 1;

				if (skill <= 6) {

					/* set increment the lower AT/PA value */
					if (g_hero.at_weapon[skill] > g_hero.pa_weapon[skill])
						g_hero.pa_weapon[skill]++;
					else
						g_hero.at_weapon[skill]++;
				}
			} else {
				/* inc tries for that skill */
				g_skill_incs[skill].tries++;
			}
		}
	}
}


/**
 * spell_inc_novice() - tries to increment a spell in novice mode
 * @spell:	the spell which should be incremented
 *
 *
 */
/* Borlandified and identical */
void spell_inc_novice(signed short spell)
{
	signed short done = 0;

	while (!done) {
		/* leave the loop if 3 tries have been done */
		if (g_spell_incs[spell].tries == 3) {
			done = 1;
			continue;
		}

#if !defined(__BORLANDC__)
			/* Original-Bugfix: add check if skill_attempts are left */
			if (g_hero.spell_incs == 0) {
				done = 1;
				continue;
			}
#endif
		/* decrement counter for spell increments */
		g_hero.spell_incs--;

		/* check if the test is passed */
		if ((signed short)random_interval_gen(2, 12) > g_hero.spells[spell]) {

			/* increment spell */
			g_hero.spells[spell]++;

			/* set inc tries for this spell to zero */
			g_spell_incs[spell].tries = 0;

			/* set the flag to leave this loop */
			done = 1;
		} else {
			g_spell_incs[spell].tries++;
		}
	}
}

/**
 * select_typus() - select a possible typus with current attribute values
 *
 */
/* Borlandified and identical */
void select_typus(void)
{
	signed char old_typus;
	signed char possible_types;
	signed char ltmp2;
	signed char *ptr;
	signed short i;
	signed short impossible;

	register signed short di;
	register signed short si;


	struct type_bitmap t;

	old_typus = -1;
	
	t = *(struct type_bitmap*)&g_type_bitmap;

	/* check if attribs have been set */
	if (g_hero.attrib[0].normal != 0) {

		/* save the old typus */
		old_typus = g_hero.typus;
		
		/* disable MU bonus */
		if (g_got_mu_bonus) {
			g_hero.attrib[0].current = --g_hero.attrib[0].normal;
		}
		/* disable CH bonus */
		if (g_got_ch_bonus) {
			g_hero.attrib[2].current = --g_hero.attrib[2].normal;
		}
		possible_types = 0;

		for (i = 1; i <= 12; i++) {
			impossible = 0;
			for (si = 0; si < 4; si++) {

				ptr = &g_hero.attrib[g_reqs[i][si].attrib].normal;

				ltmp2 = *(ptr);

				if ((g_reqs[i][si].value & 0x80) != 0) {
					if (ltmp2 > (g_reqs[i][si].value & 0x7f))
						impossible = 1;
				} else {
					if (g_reqs[i][si].value > ltmp2)
						impossible = 1;
				}
			}

			if (!impossible) {

				g_type_names[possible_types] = 	get_text( (g_hero.sex ? 271 : 17 ) + i);
				t.t[possible_types] = (char)i;
				possible_types++;
			}
		}

		if (!possible_types) {
			if (!can_change_attribs()) {
				/* totally messed up values */
				infobox(get_text(284), 0);
				return;
			} else {
				infobox(get_text(31), 0);
				return;
			}
			return;
		}

		di = gui_radio(get_text(30), possible_types,
				g_type_names[0], g_type_names[1], g_type_names[2],
				g_type_names[3], g_type_names[4], g_type_names[5],
				g_type_names[6], g_type_names[7], g_type_names[8],
				g_type_names[9], g_type_names[10], g_type_names[11]);

		/*	restore attibute boni when selection is canceled
		 *	or the same typus is selected.
		 */
		if ((di != -1) && (t.t[di - 1] != old_typus)) {

			/* set new typus */
			g_hero.typus = t.t[di - 1];
			g_screen_var = 1;

			load_typus((signed short)g_hero.typus);
#if defined(__BORLANDC__)
			/* Sync-Point-Reason: Load supression optimizes g_hero.typus to well */
			asm { db 0x0f, 0x1f, 0x00 }
#endif
			update_mouse_cursor();
			call_fill_rect_gen((RealPt)g_vga_memstart, 16, 8, 143, 191, 0);
			wait_for_vsync();
			set_palette((signed char*)g_gen_ptr5 + 0x5c02, 0, 32);
			call_mouse();

			g_head_typus = (g_hero.typus > 10 ? 10 : g_hero.typus);

			if (g_hero.sex) {
				g_head_first = g_head_current = g_head_first_female[g_head_typus];
				g_head_last = g_head_first_male[g_head_typus + 1] - 1;
			} else {
				g_head_first = g_head_current = g_head_first_male[g_head_typus];
				g_head_last = g_head_first_female[g_head_typus] - 1;
			}

			/* reset boni flags */
			g_got_mu_bonus = g_got_ch_bonus = 0;
			fill_values();
		} else {
			if (g_got_mu_bonus) {
				g_hero.attrib[0].current = ++g_hero.attrib[0].normal;
			}
			if (g_got_ch_bonus) {
				g_hero.attrib[2].current = ++g_hero.attrib[2].normal;
			}
		}
	} else {
		infobox(get_text(265), 0);
		return;
	}
}

#define INC (1)
#define DEC (2)

/**
 * can_change_attribs() - checks if attribute changes are possible
 *
 */
/* Borlandified and nearly identical */
signed short can_change_attribs(void)
{
	signed short na_inc;
	volatile signed short na_dec;
	signed char *p;
	signed short i;
	register signed short pa_inc;
	register signed short pa_dec;

	pa_inc = 0;
	pa_dec = 0;
	na_inc = 0;
	na_dec = 0;

	for (i = 0; i < 7; i++) {
		p = &g_hero.attrib[i].normal;

		if ((g_attrib_changed[i] != INC) && (p[0] > 8))
			pa_dec += 8 - p[0];
		if ((g_attrib_changed[i] != DEC) && (p[0] < 13))
			pa_inc += 13 - p[0];
	}

	for (i = 7; i < 14; i++) {
		p = &g_hero.attrib[i].normal;

		if ((g_attrib_changed[i] != INC) && (p[0] > 2))
			na_dec += 2 - p[0];
		if ((g_attrib_changed[i] != DEC) && (p[0] < 8))
			na_inc += 8 - p[0];
	}

	/* no values from positive attributes left */
	if (((pa_inc == 0) && (pa_dec == 0)) ||
	    ((pa_inc == 0) && (na_dec < 2))  ||
	    ((na_inc < 2) && (pa_dec == 0))  ||
	   ((na_dec < 2) && (na_inc < 2)))  return 0;

	return 1;
}

/**
 * change_attribs() - change attributes
 */
/* Borlandified and nearly identical */
void change_attribs(void)
{
	signed short tmp1;
	volatile signed short tmp2;
	volatile signed short tmp3;
	signed char *ptr1;
	signed char *ptr2;
	signed char c;

	signed short si;
	signed short di;

	/* check if attributes have been set */
	if (!g_hero.attrib[0].normal) {
		infobox(get_text(16), 0);
		return;
	}
	/* check if changing is possible */
	if (!can_change_attribs()) {
		infobox(get_text(266), 0);
		return;
	}
	/* if typus != 0 */
	if (g_hero.typus) {

		if (!gui_bool(get_text(73)))
			return;

		/* set typus to 0 */
		g_hero.typus = 0;

		/* remove MU boni */
		if (g_got_mu_bonus) {
			g_hero.attrib[0].current = --g_hero.attrib[0].normal;
			g_got_mu_bonus = 0;
		}
		/* remove CH boni */
		if (g_got_ch_bonus) {
			g_hero.attrib[2].current = --g_hero.attrib[2].normal;
			g_got_ch_bonus = 0;
		}
		g_screen_var = 1;
		refresh_screen();
		g_screen_var = 0;

#if defined(__BORLANDC__)
		/* Sync-Point-Reason: jump out of the if-statement and an
		 *                    unused return */
		asm { db 0x66, 0x90; db 0x0f, 0x1f, 0x00; } // BCC Sync-Point
#endif
	}


	/* check again if changing is possible */
	if (can_change_attribs() == 0) {
		infobox(get_text(266), 0);
		return;
	}
	/* select a positive attribute to change */
	g_text_x_mod = -80;
	tmp2 = gui_radio(get_text(78), 7,
			get_text(32), get_text(33), get_text(34), get_text(35),
			get_text(36), get_text(37), get_text(38));
	g_text_x_mod = 0;

	if (tmp2 == -1)
		return;
	tmp2--;
	/* get the modification type */
	if (!g_attrib_changed[tmp2]) {
		/* ask user if inc or dec */
		g_text_x_mod = -80;
		tmp3 = gui_radio((char*)NULL, 2, get_text(75), get_text(76));
		g_text_x_mod = 0;

		if (tmp3 == -1)
			return;
	} else {
		tmp3 = g_attrib_changed[tmp2];
	}

	ptr1 = &g_hero.attrib[tmp2].normal;

	if (tmp3 == INC) {
		/* increment */
		if (ptr1[0] == 13) {
			infobox(get_text(77), 0);
			return;
		}
		c = 0;
		for (di = 7; di < 14; di++) {
			if (g_attrib_changed[di] != DEC) {
				ptr2 = &g_hero.attrib[di].normal;
				if (ptr2[0] < 8) {
					c += 8 - ptr2[0];
				}
			}
		}
		if (c < 2) {
			infobox(get_text(85), 0);
			return;
		}
		/* increment positive attribute */
		//g_hero.attrib[tmp2].normal = ++g_ghero.attrib[tmp2].current;
		ptr1[0] = ++ptr1[1];

		g_attrib_changed[tmp1] = INC;

		refresh_screen();

		tmp1 = 0;
		while (tmp1 != 2) {
		
			do {
				/* ask which negative attribute to increment */
				g_text_x_mod = -80;
				si = gui_radio(get_text(80), 7,
						get_text(39), get_text(40), get_text(41),
						get_text(42), get_text(43), get_text(44),
						get_text(45));
				g_text_x_mod = 0;

			} while (si == -1);

			si--;
			/* check if this attribute has been decremented */
			if (g_attrib_changed[si + 7] == DEC) {
				infobox(get_text(83), 0);
				continue;
			}
			ptr1 = &g_hero.attrib[si + 7].normal;
			/* check if attribute can be incremented */
			if (ptr1[0] == 8) {
				infobox(get_text(77), 0);
			} else {
				/* increment the negative attribute */
				tmp1++;
				g_attrib_changed[si + 7] = INC;

				//g_hero.attrib[si + 7].normal = ++g_ghero.attrib[si + 7].current;
				ptr1[0] = ++ptr1[1];

				refresh_screen();
			}
		}
	} else {
		/* decrement */
		/* check if the positive attribute can be decremented */
		if (ptr1[0] == 8) {
			infobox(get_text(81), 0);
			return;
		}
		c = 0;
		for (di = 7; di < 14; di++) {
			if (g_attrib_changed[di] != INC) {
				ptr2 = &g_hero.attrib[di].normal;
				if (ptr2[0] > 2) {
					c += ptr2[0] - 2;
				}
			}
		}
		if (c < 2) {
			infobox(get_text(84), 0);
			return;
		}
		/* decrement positive attribute */
		// g_hero.attrib[tmp3].normal = --g_hero.attrib[tmp3].current;
		ptr1[0] = --ptr1[1];

		/* mark this attribute as decremented */
		g_attrib_changed[tmp2] = DEC;

		refresh_screen();

		tmp1 = 0;
		while (tmp1 != 2) {

			do {
				/* ask which negative attribute to increment */
				g_text_x_mod = -80;
				si = gui_radio(get_text(79), 7,
						get_text(39), get_text(40), get_text(41),
						get_text(42), get_text(43), get_text(44),
						get_text(45));
				g_text_x_mod = 0;
			} while (si == -1);


			si--;
			/* check if this attribute has been incremented */
			if (g_attrib_changed[si + 7] == INC) {
				infobox(get_text(82), 0);
				continue;
			}
				
			ptr1 = &g_hero.attrib[si + 7].normal;
			
			/* check if attribute can be decremented */
			if (ptr1[0] == 2) {
				infobox(get_text(81), 0);
				continue;
			}
			/* decrement the negative attribute */
			tmp1++;

			// g_hero.attrib[si + 7].normal = --g_hero.attrib[si + 7].current;
			ptr1[0] = --ptr1[1];

			g_attrib_changed[si + 7] = DEC;

			refresh_screen();
		}
	}
}

#undef INC
#undef DEC

/* Borlandified and identical */
void save_picbuf(void)
{
	RealPt p;
	signed short x_3;
	signed short y_1;
	signed short y_2;
	signed short y_3;
	signed short w_1;
	signed short w_2;
	signed short w_3;
	signed short h_1;
	signed short h_2;
	signed short h_3;

	register signed short x_1;
	register signed short x_2;

	x_1 = 0;

	/* check on which page we are */
	switch (g_gen_page) {
		/* main page */
		case 0: {
			/* name field */
			x_1 = 176; y_1 = 12; w_1 = 94; h_1 = 8;

			/* positive attributes values */
			x_2 = 205; y_2 = 73; w_2 = 20; h_2 = 90;

			/* negative attribute values */
			x_3 = 273; y_3 = 73; w_3 = 20; h_3 = 90;
			break;
		}
		/* skill pages */
		case 1:	case 2: case 3: {
			/* remaining skill augmention tries */
			x_1 = 270; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of skills */
			x_2 = 205; y_2 = 42; w_2 = 20; h_2 = 140;

			/* right row of skills */
			x_3 = 287; y_3 = 42; w_3 = 20; h_3 = 140;
			break;
		}
		/* ATPA page */
		case 4: {
			/* left row AT values */
			x_2 = 225; y_2 = 48; w_2 = 20; h_2 = 130;

			/* right row PA values */
			x_3 = 260; y_3 = 48; w_3 = 20; h_3 = 130;
			break;
		}
		/* spell pages */
		case 5: case 6:	case 7:
		case 8:	case 9:	case 10: {
			/* remaining spell augmention tries */
			x_1 = 215; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of spells */
			x_2 = 141; y_2 = 42; w_2 = 20;	h_2 = 140;

			/* right row of spells */
			x_3 = 286; y_3 = 42; w_3 = 20;	h_3 = 140;
			break;
		}
	}

	if (x_1) {
		p = g_gen_ptr1_dis + y_1 * 320 + x_1;
		copy_to_screen(p, g_picbuf1, w_1, h_1, 2);
	}

	p = g_gen_ptr1_dis + y_2 * 320 + x_2;
	copy_to_screen(p, g_picbuf2, w_2, h_2, 2);

	p = g_gen_ptr1_dis + y_3 * 320 + x_3;
	copy_to_screen(p, g_picbuf3, w_3, h_3, 2);
}

/* Borlandified and identical */
void restore_picbuf(RealPt ptr)
{
	RealPt p;
	unsigned short x_1, x_2, x_3;
	unsigned short y_1, y_2, y_3;
	unsigned short w_1, w_2, w_3;
	unsigned short h_1, h_2, h_3;

	x_1 = 0;

	/* check on which page we are */
	switch (g_gen_page) {
		/* main page */
		case 0: {
			/* name field */
			x_1 = 176; y_1 = 12; w_1 = 94; h_1 = 8;

			/* positive attributes values */
			x_2 = 205; y_2 = 73; w_2 = 20; h_2 = 90;

			/* negative attribute values */
			x_3 = 273; y_3 = 73; w_3 = 20; h_3 = 90;
			break;
		}
		/* skill pages */
		case 1:	case 2: case 3: {
			/* remaining skill augmention tries */
			x_1 = 270; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of skills */
			x_2 = 205; y_2 = 42; w_2 = 20; h_2 = 140;

			/* right row of skills */
			x_3 = 287; y_3 = 42; w_3 = 20; h_3 = 140;
			break;
		}
		/* ATPA page */
		case 4: {
			/* left row AT values */
			x_2 = 225; y_2 = 48; w_2 = 20; h_2 = 130;

			/* right row PA values */
			x_3 = 260; y_3 = 48; w_3 = 20; h_3 = 130;
			break;
		}
		/* spell pages */
		case 5: case 6:	case 7:
		case 8:	case 9:	case 10: {
			/* remaining spell augmention tries */
			x_1 = 215; y_1 = 184; w_1 = 15; h_1 = 8;

			/* left row of spells */
			x_2 = 141; y_2 = 42; w_2 = 20;	h_2 = 140;

			/* right row of spells */
			x_3 = 286; y_3 = 42; w_3 = 20;	h_3 = 140;
			break;
		}
	}

	if (x_1) {
		p = ptr + y_1 * 320 + x_1;
		copy_to_screen(g_picbuf1, p, w_1, h_1, 0);
	}

	p = ptr + y_2 * 320 + x_2;
	copy_to_screen(g_picbuf2, p, w_2, h_2, 0);

	p = ptr + y_3 * 320 + x_3;
	copy_to_screen(g_picbuf3, p, w_3, h_3, 0);
}

/**
 * print_attribs() -	print the attribute values
 *
 */
/* Borlandified and identical */
void print_attribs(void)
{
	signed char *p;
	char buf[10];

	signed short i;

	p = &g_hero.attrib[0].normal;

	for (i = 0; i < 14; p += 3, i++) {
		/* don't print 0s */
		if (p[0] != 0) {
			/* print attribute value in decimal form */
			print_str(itoa(p[0], buf, 10),	g_attrib_coords[i].x, g_attrib_coords[i].y);
		}
	}
}


/**
 * print_values() - print the values of the character
 *
 */
/* Borlandified and identical */
void print_values(void)
{
	char tmp[4];
	signed short width;
	signed short align_left = 222;
	signed short align_right = 302;

	register signed short i;
	register signed short pos;


	switch (g_gen_page) {

		case 0: {
			restore_picbuf((RealPt)g_gfx_ptr);

			/* print name */
			print_str(g_hero.name, 180, 12);

			/* print attributes */
			print_attribs();

			/* return if no typus */
			if (g_hero.typus == 0) return;

			/* print height */
			sprintf(g_gen_ptr2, get_text(70), g_hero.height);
			print_str(g_gen_ptr2, 205, 25);

			/* print weight */
			sprintf(g_gen_ptr2, get_text(71), g_hero.weight);

			print_str(g_gen_ptr2, 205, 37);

			/* print god name */
			print_str(get_text(56 + g_hero.god), 205, 49);

			/* print money */
			make_valuta_str(g_gen_ptr2, g_hero.money);
			print_str(g_gen_ptr2, 205, 61);

			/* print LE */
			print_str(itoa(g_hero.le_max, tmp, 10), 172, 164);
			/* print AE */
			print_str(itoa(g_hero.ae_max, tmp, 10), 221, 164);
			/* print Endurance */
			print_str(itoa(g_hero.le_max + g_hero.attrib[6].current, tmp, 10), 296, 164);
			/* print MR */
			print_str(itoa(g_hero.mr, tmp, 10), 232, 184);
			break;
		}
#if defined(__BORLANDC__)
		/* Sync-Point-Reason: an unused return call */
		asm { db 0xe9, 0x85, 0x0a}; // BCC Sync-Point
#endif
		case 1: {
			/* SKILLS Page 1/3 */
			restore_picbuf((RealPt)g_gfx_ptr);


			/* print fight skills */
			for (i = 0; i < 9; i++) {
				itoa(g_hero.skills[i], tmp, 10);

				width = get_str_width(tmp);

				// i & 1 = right column else left column
				print_str(tmp, ((i & 1) ? align_right - width : align_left - width), (i / 2) * 12 + 42);
			}

			/* print body skills */
			for (i = 9; i < 19; i++) {
				pos = i - 9;

				itoa(g_hero.skills[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 119);
			}

			/* remaining attempts for skills */
			print_str(itoa(g_hero.skill_incs, tmp, 10), 271, 184);

			break;
		}
		case 2: {
			/* SKILLS Page 2/3 */
			restore_picbuf((RealPt)g_gfx_ptr);

			/* print social skills */
			for (i = 19; i < 26; i++) {
				pos = i - 19;

				itoa(g_hero.skills[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 42);
			}

			/* print lore skills */
			for (i = 32; i < 41; i++) {
				pos = i - 32;

				itoa(g_hero.skills[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 113);
			}

			/* remaining attempts for skills */

			print_str(itoa(g_hero.skill_incs, tmp, 10), 271, 184);

			break;
		}
		case 3: {
			/* SKILLS Page 3/3 */
			restore_picbuf((RealPt)g_gfx_ptr);

			/* print craftmansship skills */
			for (i = 41; i < 50; i++) {

				pos = i - 41;

				itoa(g_hero.skills[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 42);

			}

			/* print nature skills */
			for (i = 26; i < 32; i++) {

				pos = i - 26;

				itoa(g_hero.skills[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 119);

			}

			/* print intuition skills */
			for (i = 50; i < 52; i++) {

				pos = i - 50;

				itoa(g_hero.skills[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 170);
			}

			/* remaining attempts for skills */

			print_str(itoa(g_hero.skill_incs, tmp, 10), 271, 184);

			break;
		}
		case 4: {
			/* ATPA Page */
			restore_picbuf((RealPt)g_gfx_ptr);

			/* Print base value  2x the same */
			print_str(itoa(g_hero.atpa_base, tmp, 10), 231, 30);
			print_str(itoa(g_hero.atpa_base, tmp, 10), 268, 30);

			for (i = 0; i < 7; i++) {

				/* print AT value */
				itoa(g_hero.at_weapon[i], tmp, 10);
				print_str(tmp, 237 - get_str_width(tmp), i * 12 + 48);

				/* print PA value */
				itoa(g_hero.pa_weapon[i], tmp, 10);
				print_str(tmp, 274 - get_str_width(tmp), i * 12 + 48);

				/* print skill value */
				itoa(g_hero.skills[i], tmp, 10);
				print_str(tmp, 315 - get_str_width(tmp), i * 12 + 48);
			}

			/* calc range base value (KL+GE+KK) /4 */
			pos = (g_hero.attrib[1].normal
					+ g_hero.attrib[4].normal
					+ g_hero.attrib[6].normal) / 4;

			/* print missle and thrown weapon values */
			print_str(itoa(pos + g_hero.skills[7], tmp, 10), 231, 144);
			print_str(itoa(pos + g_hero.skills[8], tmp, 10), 231, 156);

			break;
		}

		case 5: {
			/* Spells Page 1/6 */
			restore_picbuf((RealPt)g_gfx_ptr);

			for (i = 1; i < 6; i++) {

				pos = i - 1;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);

			}
			for (i = 33; i < 38; i++) {

				pos = i - 33;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);
			}
			for (i = 6; i <= 11; i++) {

				pos = i - 6;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 146);
			}

			/* print spell attempts */
			print_str(itoa(g_hero.spell_incs, tmp, 10), 217, 184);

			break;
		}
		case 6: {
			/* Spells Page 2/6 */
			restore_picbuf((RealPt)g_gfx_ptr);

			for (i = 12; i <= 17; i++) {

				pos = i - 12;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}
			for (i = 18; i < 24; i++) {

				pos = i - 18;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);

			}
			for (i = 24; i < 27; i++) {

				pos = i - 24;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 148);

			}

			/* print spell attempts */
			print_str(itoa(g_hero.spell_incs, tmp, 10), 217, 184);

			break;
		}
		case 7: {
			/* Spells Page 3/6 */
			restore_picbuf((RealPt)g_gfx_ptr);

			for (i = 27; i < 33; i++) {

				pos = i - 27;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			for (i = 38; i < 45; i++) {

				pos = i - 38;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);
			}

			for (i = 45; i <= 46; i++) {

				pos = i - 45;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 160);
			}

			/* print spell attempts */
			print_str(itoa(g_hero.spell_incs, tmp, 10), 217, 184);

			break;
		}
		case 8: {
			/* Spells Page 4/6 */
			restore_picbuf((RealPt)g_gfx_ptr);

			for (i = 47; i <= 48; i++) {

				pos = i - 47;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			for (i = 49; i < 58; i++) {

				pos = i - 49;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 71);
			}

			for (i = 58; i < 60; i++) {

				pos = i - 58;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 148);
			}

			/* print spell attempts */
			print_str(itoa(g_hero.spell_incs, tmp, 10), 217, 184);

			break;
		}
		case 9: {
			/* Spells Page 5/6 */
			restore_picbuf((RealPt)g_gfx_ptr);

			for (i = 60; i < 76; i++) {

				pos = i - 60;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			/* print spell attempts */
			print_str(itoa(g_hero.spell_incs, tmp, 10), 217, 184);

			break;
		}
		case 10: {
			/* Spells Page 6/6 */
			restore_picbuf((RealPt)g_gfx_ptr);

			for (i = 76; i < 86; i++) {

				pos = i - 76;

				itoa(g_hero.spells[i], tmp, 10);

				width = get_str_width(tmp);

				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);

			}

			/* print spell attempts */
			print_str(itoa(g_hero.spell_incs, tmp, 10), 217, 184);

			break;
		}
	}
}

/**
 *	make_valuta_str	-	makes a valuta string
 *	@dst:	the destination
 *	@money:	the money in Heller
 *
 *	This funcion is buggy.
 */
/* Borlandified and identical */
void make_valuta_str(char *dst, signed long money)
{
	/* Orig-BUG: d can overflow  on D > 65536*/
	unsigned short d = 0;
	unsigned short s = 0;

	/*	These loops are not very performant.
		They take longer the more money you have.
		Here is a much better solution.
	*/

	/*
	d = money / 100;
	money -= d * 100;

	s = money / 10;
	money -= s * 10;
	*/
	while (money / 100) {
		d++;
		money -= 100;
	}

	while (money / 10) {
		s++;
		money -= 10;
	}

	sprintf(dst, get_text(69), d, s, (signed short)money);
}

/* Borlandified and nearly identical */
static void inc_skill(signed short skill, signed short max, char *msg)
{
	/* no more increments than the maximum */
	if (g_skill_incs[skill].incs >= max) {
		infobox(msg, 0);
		return;
	}
	/* we just have 3 tries to increment */
	if (g_skill_incs[skill].tries == 3) {
		infobox(get_text(151), 0);
		return;
	}

	/* decrement total number of skill inc tries */
	g_hero.skill_incs--;
	if ((signed short)random_interval_gen(2, 12) > g_hero.skills[skill]) {
		/* print sucess message */
		infobox(get_text(152), 0);
		/* increment skill */
		g_hero.skills[skill]++;
		/* reset tries */
		g_skill_incs[skill].tries = 0;
		/* increment skill increments */
		g_skill_incs[skill].incs++;

		/* check if we have a melee attack skill */
		if (skill <= 6) {
			/* check if AT > PA */
			if (g_hero.at_weapon[skill] > g_hero.pa_weapon[skill]) {
				/* inc PA */
				g_hero.pa_weapon[skill]++;
			} else {
				/* inc AT */
				g_hero.at_weapon[skill]++;
			}
		}
	} else {
		/* print failure message */
		infobox(get_text(153), 0);
		/* increment try */
		g_skill_incs[skill].tries++;
	}

	refresh_screen();
}

/* Borlandified and identical */
void select_skill(void)
{
	signed short skill;
	signed short group;

	do {

		/* check skill attempts */
		if (!g_hero.skill_incs) {
			infobox(get_text(94), 0);
			g_text_x_mod = 0;
			return;
		}

		g_text_x_mod = -80;

		switch (g_gen_page) {
		case 1: {
			group = gui_radio(get_text(93), 2, get_text(86), get_text(87));
			if (group != -1) {
				switch (group) {
				case 1: {
					/* Fight */
					skill = gui_radio(get_text(147),
						9,
						get_text(95), get_text(96), get_text(97),
						get_text(98), get_text(99), get_text(100),
						get_text(101), get_text(102), get_text(103)) - 1;

					if (skill != -2) {
						inc_skill(skill, 1, get_text(148));
					}
					break;
				}
				case 2: {
					/* Body */
					skill = gui_radio(get_text(147),
						10,
						get_text(104), get_text(105),
						get_text(106), get_text(107),
						get_text(108), get_text(109),
						get_text(110), get_text(111),
						get_text(112), get_text(113)) - 1;

					if (skill != -2) {
						skill += 9;
						inc_skill(skill, 2, get_text(149));
					}
					break;
				}
				}
			}
			break;
		}
		case 2: {
			group = gui_radio(get_text(93), 2, get_text(88), get_text(89));
			if (group != -1) {
				switch (group) {
				case 1: {
					skill = gui_radio(get_text(147),
							7,
							get_text(114), get_text(115), get_text(116),
							get_text(117), get_text(118), get_text(119),
							get_text(120)) - 1;

					if (skill != -2) {
						skill += 19;
						inc_skill(skill, 2, get_text(149));
					}
					break;
				}
				case 2: {
					skill = gui_radio(get_text(147),
							9,
							get_text(127), get_text(128), get_text(129),
							get_text(130), get_text(131), get_text(132),
							get_text(133), get_text(134), get_text(135)) - 1;

					if (skill != -2) {
						skill += 32;
						inc_skill(skill, 3, get_text(150));
					}
					break;
				}
				}
			}
			break;
		}
		case 3: {
			group = gui_radio(get_text(93), 3, get_text(90), get_text(91), get_text(92));
			if (group != -1) {
				switch (group)
				{
					case 1: {
						skill = gui_radio(get_text(147),
							9,
							get_text(136), get_text(137), get_text(138),
							get_text(139), get_text(140), get_text(141),
							get_text(142), get_text(143), get_text(144)) - 1;

						if (skill != -2) {
							skill += 41;
							inc_skill(skill, 2, get_text(149));
						}
						break;
					}
					case 2: {
						skill = gui_radio(get_text(147),
							6,
							get_text(121), get_text(122), get_text(123),
							get_text(124), get_text(125), get_text(126)) - 1;

						if (skill != -2) {
							skill += 26;
							inc_skill(skill, 2, get_text(149));
						}
						break;
					}
					case 3: {
						skill = gui_radio(get_text(147),
							2,
							get_text(145),
							get_text(146)) - 1;

						if (skill != -2) {
							skill += 50;
							inc_skill(skill, 1, get_text(148));
						}
						break;
					}
				}
				}
				break;
			}
		}

		g_text_x_mod = 0;

	} while (group != -1);
}

/* Borlandified and identical */
static void inc_spell(signed short spell)
{
	signed short max_incs = 1;

	/* if typus == warlock and the origin of the spell is warlock */
	if ((g_hero.typus == 7) && (g_spell_tab[spell].origin == 3))
		max_incs = 2;
	/* if typus == elf and the origin of the spell is elven */
	if ((g_hero.typus >= 10) && (g_spell_tab[spell].origin == 2))
		max_incs = 2;
	/* if typus == druid and the origin of the spell is druid */
	if ((g_hero.typus == 8) && (g_spell_tab[spell].origin == 0))
		max_incs = 2;
	/* if typus == mage */
	if (g_hero.typus == 9) {
		/* and the origin of the spell is mage */
		if (g_spell_tab[spell].origin == 1)
			max_incs = 2;

		/* and is a school spell */
		if (is_in_word_array(spell, g_house_spells[g_hero.spell_school]))
			max_incs = 3;
	}

	/* all spell increments used for that spell */
	if (g_spell_incs[spell].incs >= max_incs) {
		infobox(get_text(257), 0);
		return;

	}
	/* all tries used for that spell */
	if (g_spell_incs[spell].tries == 3) {
		infobox(get_text(151), 0);
#if !defined(__BORLANDC__)
		/* Sync-Point-Reason: this return would result in different code */
		return;
#else
		// Fool the BCC a bit with a handcoded return
		asm { db 0xeb, 0x60; }
#endif
	}

	/* decrement spell attempts */
	g_hero.spell_incs--;

	if ((signed short)random_interval_gen(2, 12) > g_hero.spells[spell]) {
		/* show success */
		infobox(get_text(152), 0);
		/* increment spell value */
		g_hero.spells[spell]++;
		/* reset tries */
		g_spell_incs[spell].tries = 0;
		/* increment incs */
		g_spell_incs[spell].incs++;
	} else {
		/* show failure */
		infobox(get_text(153), 0);
		/* increment tries */
		g_spell_incs[spell].tries++;
	}

	refresh_screen();
}

/* Borlandified and identical */
void select_spell(void)
{
	signed short group;
	signed short spell;

	do {

		/* check if we have spell attempts */
		if (!g_hero.spell_incs) {
			infobox(get_text(94), 0);
			g_text_x_mod = 0;
			return;
		}

		g_text_x_mod = -90;

		switch (g_gen_page) {
			case 5: {
				group = gui_radio(get_text(155),
						3,
						get_text(157), get_text(162),
						get_text(158));
				if (group != -1) {
				switch (group)
				{
					case 1: {
						spell = gui_radio(get_text(156),
								5,
								get_text(169), get_text(170),
								get_text(171), get_text(172),
								get_text(173)) - 1;

						if (spell != -2) {
							spell++;
							inc_spell(spell);
						}
						break;
					}
					case 2: {
						spell = gui_radio(get_text(156), 5,
								get_text(201),
								get_text(202),
								get_text(203),
								get_text(204),
								get_text(205)) - 1;

						if (spell != -2) {
							spell += 33;
							inc_spell(spell);
						}
						break;
					}
					case 3: {
						spell = gui_radio(get_text(156), 6,
								get_text(174),
								get_text(175),
								get_text(176),
								get_text(177),
								get_text(178),
								get_text(179)) - 1;
						if (spell != -2) {
							spell += 6;
							inc_spell(spell);
						}
						break;
					}
				}
				}
				break;
			}
			case 6: {
				group = gui_radio(get_text(155), 3,
						get_text(158), get_text(159),
						get_text(160));
				if (group != -1) {

				switch (group) {
					case 1: {
						spell = gui_radio(get_text(156), 6,
								get_text(180),
								get_text(181),
								get_text(182),
								get_text(183),
								get_text(184),
								get_text(185)) - 1;

						if (spell != -2) {
							spell += 12;
							inc_spell(spell);
						}
						break;
					}
					case 2: {
						spell = gui_radio(get_text(156), 6,
								get_text(186),
								get_text(187),
								get_text(188),
								get_text(189),
								get_text(190),
								get_text(191)) - 1;

						if (spell != -2) {
							spell += 18;
							inc_spell(spell);
						}
						break;
					}
					case 3: {
						spell = gui_radio(get_text(156), 3,
								get_text(192), get_text(193),
								get_text(194)) - 1;

						if (spell != -2) {
							spell += 24;
							inc_spell(spell);
						}
						break;
					}
				}
				}
				break;
			}
			case 7: {
				group = gui_radio(get_text(155), 3,
						get_text(161), get_text(163),
						get_text(164));
				if (group != -1) {

				switch (group) {
					case 1: {
						spell = gui_radio(get_text(156), 6,
								get_text(195),
								get_text(196),
								get_text(197),
								get_text(198),
								get_text(199),
								get_text(200)) - 1;

						if (spell != -2) {
							spell += 27;
							inc_spell(spell);
						}
						break;
					}
					case 2: {
						spell = gui_radio(get_text(156), 7,
								get_text(206),
								get_text(207),
								get_text(208),
								get_text(209),
								get_text(210),
								get_text(211),
								get_text(212)) - 1;

						if (spell != -2) {
							spell += 38;
							inc_spell(spell);
						}
						break;
					}
					case 3: {
						spell = gui_radio(get_text(156), 2,
								get_text(213),
								get_text(214)) - 1;

						if (spell != -2) {
							spell += 45;
							inc_spell(spell);
						}
						break;
					}
				}
				}
				break;
			}
			case 8: {
				group = gui_radio(get_text(155), 3,
						get_text(164), get_text(86),
						get_text(166));
				if (group != -1) {
				switch (group) {
					case 1: {
						spell = gui_radio(get_text(156), 2,
								get_text(215),
								get_text(216)) - 1;

						if (spell != -2) {
							spell += 47;
							inc_spell(spell);
						}
						break;
					}
					case 2: {
						spell = gui_radio(get_text(156), 9,
								get_text(217), get_text(218), get_text(219),
								get_text(220), get_text(221), get_text(222),
								get_text(223), get_text(224), get_text(225)) - 1;

						if (spell != -2) {
							spell += 49;
							inc_spell(spell);
						}
						break;
					}
					case 3: {
						spell = gui_radio(get_text(156), 2,
								get_text(226),
								get_text(227)) - 1;


						if (spell != -2) {
							spell += 58;
							inc_spell(spell);
						}
						break;
					}
				}
				}
				break;
			}
			case 9: {
				spell = gui_radio(get_text(156), 16,
						get_text(228), get_text(229),
						get_text(230), get_text(231),
						get_text(232), get_text(233),
						get_text(234), get_text(235),
						get_text(236), get_text(237),
						get_text(238), get_text(239),
						get_text(240), get_text(241),
						get_text(242), get_text(243)) - 1;

				if (spell != -2) {
					spell += 60;
					inc_spell(spell);
				} else {
					group = -1;
				}
				break;
			}
			case 10: {
				spell = gui_radio(get_text(156), 10,
						get_text(244), get_text(245),
						get_text(246), get_text(247),
						get_text(248), get_text(249),
						get_text(250), get_text(251),
						get_text(252), get_text(253)) - 1;

				if (spell != -2) {
					spell += 76;
					inc_spell(spell);
				} else {
					group = -1;
				}
				break;
			}
		}

		g_text_x_mod = 0;

	} while (group != -1);
}

/* Borlandified and identical */
void choose_atpa(void)
{
	signed short skill;
	signed short increase;

	g_text_x_mod = -80;

	do {
		/* print menu with all melee weapons skills */
		skill = gui_radio(get_text(78), 7,
			get_text(95), get_text(96), get_text(97), get_text(98),
			get_text(99), get_text(100), get_text(101)) - 1;

		if (skill != -2) {
			if (g_hero.skills[skill] <= 0) {
				infobox(get_text(260), 0);
			} else {

				increase = gui_radio(get_text(254), 2,
					get_text(75), get_text(76));

				if (increase != -1) {
					if (increase == 1) {
						/* increase attack */
						if (g_hero.skills[skill] >= 0 &&
							(g_hero.pa_weapon[skill] > g_hero.atpa_base)) {
							/* inc AT */
							g_hero.at_weapon[skill]++;
							/* dec PA */
							g_hero.pa_weapon[skill]--;
							refresh_screen();
						} else {
							infobox(get_text(255), 0);
						}
					} else {
						if (g_hero.skills[skill] >= 0 &&
							(g_hero.at_weapon[skill] > g_hero.atpa_base)) {
							/* dec AT */
							g_hero.at_weapon[skill]--;
							/* inc PA */
							g_hero.pa_weapon[skill]++;
							refresh_screen();
						} else {
							infobox(get_text(256), 0);
						}
					}
				}
			}
		}

	} while (skill != -2);

	g_text_x_mod = 0;
}

/**
 * choose_typus() - choose a typus manually
 *
 */
/* Borlandified and far from identical */
void choose_typus(void)
{
	signed short choosen_typus;
	signed short randval;
	signed char sex_bak;
	signed char *ptr;
	char name_bak[20];

	signed short i;
	signed short typus_names;

	if (!gui_bool(get_text(264)))
		return;
	/* female or male typus names */
	typus_names = (g_hero.sex ? 271 : 17);

	choosen_typus = gui_radio(get_text(30), 12,
				get_text(typus_names + 1), get_text(typus_names + 2),
				get_text(typus_names + 3), get_text(typus_names + 4),
				get_text(typus_names + 5), get_text(typus_names + 6),
				get_text(typus_names + 7), get_text(typus_names + 8),
				get_text(typus_names + 9), get_text(typus_names + 10),
				get_text(typus_names + 11), get_text(typus_names + 12));

	if (choosen_typus == -1)
		return;

	/* clear the hero area with saved name and sex */
	strcpy(name_bak, g_hero.name);
	sex_bak = g_hero.sex;

	memset(&g_hero, 0, sizeof(g_hero));

	clear_hero();
	g_hero.sex = sex_bak;

	strcpy(g_hero.name, name_bak);

	/* set typus */
	g_hero.typus = (signed char)choosen_typus;

	ptr = &g_hero.attrib[0].normal;

	/* roll out positive attribute values */
	for (i = 0; i < 7; i ++) {

		randval = (signed short)random_interval_gen(8, 13);

		if (randval > 8)
			randval--;
		/* set attrib.current ant attrib.normal */
		ptr[3 * i] = ptr[3 * i + 1] = (signed char)randval;
	}

	ptr = &g_hero.attrib[7].normal;

	/* roll out negative attribute values */
	for (i = 0; i < 7; i ++) {

		randval = (signed short)random_interval_gen(2, 7);

		if (randval < 7)
			randval++;

		ptr[3 * i] = ptr[3 * i + 1] = (signed char)randval;
	}

	/* adjust typus attribute requirements */
	for (i = 0; i < 4; i++) {

		/* calc pointer to attribute */
		ptr = &g_hero.attrib[g_reqs[choosen_typus][i].attrib].normal;
		/* get the required value */
		randval = g_reqs[choosen_typus][i].value;

		if (randval != 1) {

			if (randval & 0x80) {
				/* attribute upper bound */
				if ((signed char)ptr[0] > (randval & 0x7f)) {
					ptr[0] = ptr[1] = (signed char)(randval & 0x7f);
				}
			} else {
				/* attribute lower bound */
				if ((signed char)ptr[0] < randval) {
					ptr[0] = ptr[1] = (signed char)randval;
				}
			}
		}
	}

	load_typus(g_hero.typus);
	update_mouse_cursor();
	call_fill_rect_gen((RealPt)g_vga_memstart, 16, 8, 143, 191, 0);
	wait_for_vsync();
	set_palette((signed char*)g_gen_ptr5 + 0x5c02, 0, 32);
	call_mouse();


	g_head_typus = (g_hero.typus > 10 ? 10 : g_hero.typus);

	if (g_hero.sex) {
		g_head_first = g_head_current = g_head_first_female[g_head_typus];
		g_head_last = g_head_first_male[g_head_typus + 1] - 1;
	} else {
		g_head_first = g_head_current = g_head_first_male[g_head_typus];
		g_head_last = g_head_first_female[g_head_typus] - 1;
	}
	fill_values();
	g_screen_var = 1;
}

/* Borlandified and nearly identical, but works correctly */
static void pal_fade_out(signed char *dst, signed char *src, signed short n)
{
//	struct struct_color *d = (struct struct_color*)dst;
//	struct struct_color *s = (struct struct_color*)src;
	signed short i;

	for (i = 0; i < n; i++) {

		/* RED */
		if (*(src + 3 * i + 0) < *(dst + 3 * i + 0)) {
			(*(signed char*)(dst + 3 * i + 0))--;
		} else if (*(src + 3 * i + 0) > *(dst + 3 * i + 0)) {
			(*(signed char*)(dst + 3 * i + 0))++;
		}

		/* GREEN */
		if (*(src + 3 * i + 1) < *(dst + 3 * i + 1)) {
			(*(signed char*)(dst + 3 * i + 1))--;
		} else if (*(src + 3 * i + 1) > *(dst + 3 * i + 1)) {
			(*(signed char*)(dst + 3 * i + 1))++;
		}
		
		/* BLUE */
		if (*(src + 3 * i + 2) < *(dst + 3 * i + 2)) {
			(*(signed char*)(dst + 3 * i + 2))--;
		} else if (*(src + 3 * i + 2) > *(dst + 3 * i + 2)) {
			(*(signed char*)(dst + 3 * i + 2))++;
		}
#if 0
		if (s[i].r < d[i].r) {
			d[i].r--;
		} else {
			if (s[i].r > d[i].r)
				d[i].r++;
		}

		if (s[i].g < d[i].g) {
			d[i].g--;
		} else {
			if (s[i].g > d[i].g)
				d[i].g++;
		}

		if (s[i].b < d[i].b) {
			d[i].b--;
		} else {
			if (s[i].b > d[i].b)
				d[i].b++;
		}
#endif
	}
}

/* Borlandified and nearly identical, but works correctly */
static void pal_fade_in(signed char *dst, signed char *src, signed short col, signed short n)
{
	signed short i;
	signed short si;

	si = 0x40 - col;

	for (i = 0; i < n; i++) {
		//Remark: using this version produces different code
		//if ((src[3 * i] >= si) && (src[3 * i] > dst[3 * i])) dst[3 * i]++;

		/* RED */
		if (*(src + 3 * i + 0) >= si) {
			if (*(src + i * 3 + 0) > *(dst + i * 3 + 0))
				(*(signed char*)(dst + 3 * i + 0))++;
		}

		/* GREEN */
		if (*(src + 3 * i + 1) >= si) {
			if (*(src + i * 3 + 1) > *(dst + i * 3 + 1))
				(*(signed char*)(dst + 3 * i + 1))++;
		}

		/* BLUE */
		if (*(src + 3 * i + 2) >= si) {
			if (*(src + i * 3 + 2) > *(dst + i * 3 + 2))
				(*(signed char*)(dst + 3 * i + 2))++;
		}
	}
}

#if !defined(__BORLANDC__)
static void BE_cleanup(void)
{
	unsigned char *host_ptr;

	if (g_vga_memstart) {
		free(g_vga_memstart);
		g_vga_memstart = NULL;
		g_gfx_ptr = NULL;
	}

	if ((host_ptr = g_buffer_sex_dat) != 0) {
		free(host_ptr);
		g_buffer_sex_dat = NULL;
	}

	if ((host_ptr = g_buffer_popup_nvf - 8) != 0) {
		free(host_ptr);
		g_buffer_popup_nvf = NULL;
	}

	if ((host_ptr = g_buffer_heads_dat) != 0) {
		free(host_ptr);
		g_buffer_heads_dat = NULL;
	}

	if (g_buffer_text != NULL) {
		free(g_buffer_text);
		g_buffer_text = NULL;
	}

	if ((host_ptr = g_buffer_font6) != 0) {
		free(host_ptr);
		g_buffer_font6 = NULL;
	}

	if ((host_ptr = g_picbuf3) != 0) {
		free(host_ptr);
		g_picbuf3 = NULL;
	}

	if ((host_ptr = g_picbuf2) != 0) {
		free(host_ptr);
		g_picbuf2 = NULL;
	}

	if ((host_ptr = g_picbuf1) != 0) {
		free(host_ptr);
		g_picbuf1 = NULL;
	}

	if ((host_ptr = g_gen_ptr6 - 8) != 0) {
		free(host_ptr);
		g_gen_ptr6 = NULL;
	}

	if ((host_ptr = g_buffer_dmenge_dat - 8) != 0) {
		free(host_ptr);
		g_buffer_dmenge_dat = NULL;
	}

	if ((host_ptr = g_gen_ptr5 - 8) != 0) {
		free(host_ptr);
		g_gen_ptr5 = NULL;
	}

	if ((host_ptr = g_gen_ptr4) != 0) {
		free(host_ptr);
		g_gen_ptr4 = NULL;
	}

	if (g_gen_ptr2 != NULL) {
		free(g_gen_ptr2);
		g_gen_ptr2 = NULL;
	}

	if ((host_ptr = g_page_buffer) != 0) {
		free(host_ptr);
		g_page_buffer = NULL;
	}

	if ((host_ptr = g_gen_ptr1_dis - 8) != 0) {
		free(host_ptr);
		g_gen_ptr1_dis = NULL;
	}

	// missed ones
	if ((host_ptr = g_snd_timbre_cache) != 0) {
		free(host_ptr);
		g_snd_timbre_cache = NULL;
	}

	if ((host_ptr = g_state_table) != 0) {
		free(host_ptr);
		g_state_table = NULL;
	}

	if ((host_ptr = g_snd_driver) != 0) {
		free(host_ptr);
		g_snd_driver = NULL;
	}

	if ((host_ptr = g_form_xmid) != 0) {
		free(host_ptr);
		g_form_xmid = NULL;
	}

	for (int i = 0; i < 11; i++) {
		if ((host_ptr = g_bg_buffer[i]) != 0) {
			free(host_ptr);
			g_bg_buffer[i] = NULL;
		}
	}

	for (int i = 0; i < 13; i++) {
		if ((host_ptr = g_typus_buffer[i]) != 0) {
			free(host_ptr);
			g_typus_buffer[i] = NULL;
		}
	}
}
#endif


/**
 *	intro() - play the intro
 */
/* Borlandified and nearly identical, but works correctly */
static void intro(void)
{
	signed char cnt1;
	signed char cnt2;
	signed short width;
	signed short height;
	signed short flen;
	signed char *pal_src;
	signed char *pal_dst;
	struct nvf_desc nvf;

	signed short i;
	signed short handle;

	g_in_intro = 1;

	/* load ATTIC */
	handle = open_datfile(18);
	read_datfile(handle, g_buffer_heads_dat, 20000);
	close(handle);

	nvf.src = g_buffer_heads_dat;
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;

	for (i = 7; i >= 0; i--) {
		nvf.dst = g_gen_ptr1_dis + i * 960L + 9600;
		nvf.no = i + 1;
		process_nvf(&nvf);

	}
	/* set dst */
	nvf.dst = g_gen_ptr1_dis;
	/* set no */
	nvf.no = 0;
	process_nvf(&nvf);

	wait_for_vsync();

	set_palette((signed char*)&g_pal_attic, 0, 16);

	cnt1 = 1;
	cnt2 = 99;

	/* glowing at the bottom */
	for (i = 0; i < 4; i++) {
		g_dst_x1 = 112;
		g_dst_y1 = 140;
		g_dst_x2 = 207;
		g_dst_y2 = 149;
		g_dst_src = (RealPt)(i * 960 + g_gen_ptr1_dis + 9600);
		do_draw_pic(0);
		vsync_or_key(20);
	}

	/* elevate the attic logo */
	i = 4;
	g_in_key_ext = 0;
	while ((cnt1 <= 100) && (g_in_key_ext == 0)) {
		g_dst_x1 = 0;
		g_dst_y1 = cnt2 + 60;
		g_dst_x2 = 95;
		g_dst_y2 = cnt2 + cnt1 + 59;
		g_dst_src = g_dst_dst = g_gen_ptr1_dis;
		do_draw_pic(0);

		if (cnt1 != 100) {

			g_dst_src = g_gen_ptr1_dis + i * 960 + 9600;
			if (cnt1 % 4 == 1)
				i++;

			if (i == 8)
				i = 4;

			g_dst_x1 = 0;
			g_dst_y1 = 150;
			g_dst_x2 = 95;
			g_dst_y2 = 159;
			g_dst_dst = g_gen_ptr1_dis;
			do_draw_pic(2);
		}

		g_dst_x1 = 112;
		g_dst_y1 = 50;
		g_dst_x2 = 207;
		g_dst_y2 = 149;
		g_dst_src = g_gen_ptr1_dis;

		g_unkn1 = 0;
		g_unkn2 = 60;
		g_unkn3 = 95;
		g_unkn4 = 159;
		g_dst_dst = g_vga_memstart;
		do_draw_pic(3);
		cnt1++;
		cnt2--;
		if (cnt1 < 37)
			vsync_or_key(2);
		else
			vsync_or_key(1);
	}

	if (g_in_key_ext == 0)
		vsync_or_key(200);

	/* load FANPRO.NVF */
	handle = open_datfile(34);
	flen = read_datfile(handle, g_buffer_heads_dat, 20000);
	close(handle);

	nvf.src = g_buffer_heads_dat;
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;
	nvf.dst = g_gen_ptr1_dis;
	nvf.no = 0;

	process_nvf(&nvf);

	/* clear screen */
	call_fill_rect_gen((RealPt)g_vga_memstart, 0, 0, 319, 199, 0);
	wait_for_vsync();

	/* set palette of FANPRO.NVF */
	set_palette(g_buffer_heads_dat + flen - 32 * 3, 0, 32);

	/* draw the picture */
	g_dst_x1 = 60;
	g_dst_y1 = 50;
	g_dst_x2 = 259;
	g_dst_y2 = 149;
	g_dst_src = g_gen_ptr1_dis;
	do_draw_pic(0);
	vsync_or_key(200);

	/* load DSALOGO.DAT */
	handle = open_datfile(16);
	read_datfile(handle, g_buffer_heads_dat, 20000);
	close(handle);

	nvf.src = g_buffer_heads_dat;
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;
	nvf.dst = g_gen_ptr1_dis;
	nvf.no = 0;

	process_nvf(&nvf);

	/* clear screen */
	call_fill_rect_gen((RealPt)g_vga_memstart, 0, 0, 319, 199, 0);
	wait_for_vsync();


	set_palette((signed char*)&g_pal_tmp, 0, 32);

	/* draw DSALOGO.DAT */
	g_dst_x1 = 0;
	g_dst_y1 = 0;
	g_dst_x2 = 319;
	g_dst_y2 = 99;
	g_dst_src = g_gen_ptr1_dis;
	do_draw_pic(0);

	/* load GENTIT.DAT */
	handle = open_datfile(17);
	read_datfile(handle, g_buffer_heads_dat, 20000);
	close(handle);

	nvf.src = g_buffer_heads_dat;
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;
	nvf.dst = g_gen_ptr1_dis;
	nvf.no = 0;

	process_nvf(&nvf);

	/* draw DSALOGO.DAT */
	g_dst_x1 = 10;
	g_dst_y1 = 110;
	g_dst_x2 = 329;
	g_dst_y2 = 159;
	g_dst_src = g_gen_ptr1_dis;
	do_draw_pic(0);

	memcpy(g_gen_ptr1_dis + 500, &g_pal_dsalogo, 96);

	pal_src = (signed char*)g_gen_ptr1_dis + 500;
	pal_dst = (signed char*)g_gen_ptr1_dis;

	memset(pal_dst, 0, 96);

	for (i = 0; i < 64; i++) {
		pal_fade_in(pal_dst, pal_src, i, 32);
		wait_for_vsync();
		set_palette(pal_dst, 0, 32);
	}

	set_textcolor(0xff, 0x00); // WHITE ON BLACK
	print_str((char*)g_str_version, 290, 190);
	vsync_or_key(400);

	memcpy(g_gen_ptr1_dis, &g_pal_dsalogo, 96);

	pal_src = (signed char*)g_gen_ptr1_dis + 500;
	pal_dst = (signed char*)g_gen_ptr1_dis;

	memset(g_gen_ptr1_dis + 500, 0, 96);

	for (i = 0; i < 64; i++) {
		pal_fade_out(pal_dst, pal_src, 32);
		wait_for_vsync();
		set_palette(pal_dst, 0, 32);
	}

	/* clear screen */
	call_fill_rect_gen((RealPt)g_vga_memstart, 0, 0, 319, 199, 0);

	g_in_intro = 0;
	return;
}

#if defined(__BORLANDC__)
/* Borlandified and identical */
static void interrupt timer_isr(void)
{
	g_random_gen_seed2++;
	if (g_random_gen_seed2 < 0)
		g_random_gen_seed2 = 0;
	restart_midi();
	((void interrupt far (*)(void))g_timer_isr_bak)();
}
#endif

/* Borlandified and identical */
static void set_timer_isr(void)
{
#if defined(__BORLANDC__)
	/* save adress of the old ISR */
	g_timer_isr_bak = ((void interrupt far (*)(void))_dos_getvect(0x1c));
	/* set a the new one */
	_dos_setvect(0x1c, timer_isr);
#endif
}

/* Borlandified and identical */
void restore_timer_isr(void)
{
#if defined(__BORLANDC__)
	_dos_setvect(0x1c, (void interrupt far (*)(void))g_timer_isr_bak);
#endif
}

#if defined(__BORLANDC__)
void print_addr(void);
#endif

/* Borlandified and nearly identical */
#define main_gen main
int main_gen(int argc, char **argv)
{
	signed short sound_off = 0;

	if (argc > 1)
		g_called_with_args = 1;

	if (argc > 2)
		g_param_level = argv[2][0];

	if ((argc > 3) && (argv[3][0] == '0')) {
		g_midi_disabled = 1;
		sound_off = 1;
	}

#if 0
	print_addr();
#endif
	g_in_intro = 1;

	if (sound_off == 0)
		init_music(13000);

	g_wo_var2 = ret_zero1();

	set_timer_isr();

	randomize();

	save_display_stat(&g_display_page_bak);

	alloc_buffers();

	g_wo_var3 = 2;

	init_video(2);

	g_have_mouse = 2;

	mouse_enable();

	if (g_have_mouse == 0)
		g_mouse_refresh_flag = -2;

	init_stuff();

	read_common_files();

	if (sound_off == 0)
		read_soundcfg();

	start_music(33);

	if (g_called_with_args == 0) {
		intro();
		read_common_files();
	}

	init_colors();
	wait_for_keypress();
	call_mouse();
	do_gen();
	stop_music();
	update_mouse_cursor();
	mouse_disable();
	restore_timer_isr();

	if (g_called_with_args != 0) {
		/* Clear the screen and return to SCHICKM.EXE/BLADEM.EXE */
		call_fill_rect_gen((RealPt)g_vga_memstart, 0, 0, 319, 199, 0);
	} else {
		/* Clear the screen and return to DOS */
		exit_video();
		clrscr();
	}

#if !defined(__BORLANDC__)
	BE_cleanup();

	/* to make MSVC happy */
	return 0;
#endif
}

/* Borlandified and nearly identical, but works correctly */
void alloc_buffers(void)
{
#if defined(__BORLANDC__)
	g_gfx_ptr = g_vga_memstart = (unsigned char*)MK_FP(0xa000, 0x0);
#else
	g_gfx_ptr = g_vga_memstart = (unsigned char*)calloc(320 * 200, 1);
#endif

	g_gen_ptr1_dis = (gen_alloc(64108) + 8);

	g_page_buffer = gen_alloc(50000);

	g_gen_ptr2 = (char*)gen_alloc(1524);
	g_gen_ptr3 = g_gen_ptr2 + 1500;

	// unused
	g_gen_ptr4 = gen_alloc(200);

	g_buffer_text = (char*)gen_alloc(6000);

	g_buffer_font6 = gen_alloc(592);

	load_font_and_text();

	g_buffer_heads_dat = gen_alloc(39000);

	g_buffer_popup_nvf = (gen_alloc(1673) + 8);

	g_buffer_sex_dat = gen_alloc(812);

	g_gen_ptr5 = (gen_alloc(23660) + 8);

	g_buffer_dmenge_dat = (gen_alloc(23660) + 8);

	g_picbuf1 = gen_alloc(800);

	g_picbuf2 = gen_alloc(2800);

	g_picbuf3 = gen_alloc(2800);

	if (!(g_gen_ptr6 = (gen_alloc(1100) + 8))) {
#if defined(__BORLANDC__)
		/* Sync-Point-Reason: code for if statement is different */
		asm { db 0x66, 0x90;}; // 2 Byte-Nop
#endif
		printf((char*)g_str_malloc_error);
	}
}

/* Borlandified and identical */
void init_colors(void)
{
	set_palette((signed char*)&g_pal_col_black, 0x00, 1);
	set_palette((signed char*)&g_pal_col_white, 0xff, 1);
	set_palette((signed char*)&g_pal_popup, 0xd8, 8);
	set_palette((signed char*)&g_pal_misc, 0xc8, 3);
	set_palette((signed char*)&g_pal_genbg, 0x40, 0x20);
	set_palette((signed char*)&g_pal_heads, 0x20, 0x20);
	set_textcolor(0xff, 0x0); // WHITE ON BLACK
}

/* Borlandified and identical */
void init_stuff(void)
{
	init_colors();

	/* these 3 variables are different text colors */
	g_fg_color[1] = 0xc8; //RED
	g_fg_color[2] = 0xc9; //YELLOW
	g_fg_color[3] = 0xca; //BLUE

	/* number of menu tiles width */
	g_menu_tiles = 3;

	g_dst_dst = g_vga_memstart;
}

/* Borlandified and identical */
unsigned char *gen_alloc(unsigned long nelem)
{
#if defined(__BORLANDC__)
	return (unsigned char*)farcalloc(nelem, 1);
#else
	return (unsigned char*)calloc(nelem, 1);
#endif
}

#if defined(__BORLANDC__)
void print_addr(void)
{
	printf("Sizeof(hero) =           0x%04x (0x06da)\n\n", sizeof(g_hero));
	printf("&g_spell_tab =           0x%04x (0x0158)\n", &g_spell_tab);
	printf("&g_screen_var =          0x%04x (0x11fe)\n", &g_screen_var);
	printf("&g_mouse_mask =          0x%04x (0x1200)\n", &g_mouse_mask);
	printf("&hero =                  0x%04x (0x132c)\n", &g_hero);
	printf("&hero.money =            0x%04x (0x1358)\n", &(g_hero.money));
	printf("&hero.mr =               0x%04x (0x1392)\n", &(g_hero.mr));
	printf("&hero.staff_level =      0x%04x (0x14c1)\n", &(g_hero.staff_level));
	printf("&hero.pic =              0x%04x (0x1606)\n", &(g_hero.pic));
	printf("&midi_disabled =         0x%04x (0x1a07)\n\n", &g_midi_disabled);
	printf("&random_gen_seed =       0x%04x (0x1fd6)\n\n", &g_random_gen_seed);
	printf("&_ctype =                0x%04x (0x1ff9)\n\n", &_ctype);
	printf("&current_timbre_length = 0x%04x (0x2474)\n", &g_current_timbre_length);
	printf("&flen =                  0x%04x (0x3f2a)\n", &g_flen);
	printf("&texts =                 0x%04x (0x40d9)\n", &g_texts);
	printf("&ro_var =                0x%04x (0x47e3)\n", &g_ro_var);
	exit(0);
}
#endif
