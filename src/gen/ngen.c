#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

#if defined(_WIN32)
#include <wtypes.h>
#endif

#if defined(__BORLANDC__)
#include <IO.H>		// lseek, _read, _close, open, write
#include <DOS.H>
#include <BIOS.H>	// bioskey, int86x()
#include <CONIO.H>	// clrscr()
#include <ALLOC.H>	// farcalloc()
#include <MATH.H>	// abs()

#include "cda_code.h"
#include "AIL.H"
#else
#include <SDL.h>
#include <SDL_mixer.h>
#include <unistd.h> // lseek(), close(), read(), write()

#include "ail_stub.h"
#endif

#include "hero.h"

#include "powerp20.h"
#include "vgalib.h"

/* portable Memory Access */
#if !defined(__BORLANDC__)
static inline signed short readws(const unsigned char *p) { return *(const signed short*)p; }
static inline unsigned int readd(const unsigned char *p) { return *(const unsigned int*)p; }
static inline signed int readds(const unsigned char *p) { return *(const signed int*)p; }
#else
#define readw(p) (*(const unsigned short*)(p))
#define readws(p) (*(const signed short*)(p))
#define readd(p) (*(const unsigned long*)(p))
#define readds(p) (*(const signed long*)(p))
#endif

#if !defined(__BORLANDC__)
// DUMMY for BCC CLib func

static unsigned short gen_rotl(unsigned short op, unsigned short count)
{
	return (op << count) | (op >> (16 - count));
}

static inline void clrscr(void) { }
#define __abs__(v) abs(v)
#define _read read
#define _close close
#else
#define gen_rotl _rotl
#endif

/** Keyboard Constants */

#define KEY_ESC     (0x01)
#define KEY_1       (0x02)
#define KEY_2       (0x03)
#define KEY_3       (0x04)
#define KEY_4       (0x05)
#define KEY_5       (0x06)
#define KEY_6       (0x07)
#define KEY_DC1     (0x11)
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
struct nvf_desc {
#if defined (__BORLANDC__)
	unsigned char huge *dst;
	unsigned char huge *src;
#else
	unsigned char *dst;
	unsigned char *src;
#endif
	signed short no;
	signed char type;
	signed short *width;
	signed short *height;
};


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
	{9, 10, 10},  {13, 100, 100}, {19, 200, 200}, {20, 500, 500} };
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

struct struct_color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
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

/* MOUSE MANAGEMENT VARIABLES */

static signed short g_have_mouse = 2;		  /* {2 = enabled, 0 = disabled} */
static signed short g_mouse_leftclick_event = 0;  /* {0 = none, 1 = clicked} */
static signed short g_mouse_rightclick_event = 0; /* {0 = none, 1 = clicked} */

static signed short g_mouse_posx_min = 0;
static signed short g_mouse_posy_min = 0;
static signed short g_mouse_posx_max = O_WIDTH - 10;
static signed short g_mouse_posy_max = O_HEIGHT - 1;

static signed short g_mouse_posx = O_WIDTH / 2;
static signed short g_mouse_posy = O_HEIGHT / 2;
static signed short g_mouse_posx_bak = O_WIDTH / 2;
static signed short g_mouse_posy_bak = O_HEIGHT / 2;

static signed short g_mouse_moved = 0;
static signed short g_mouse_locked = 0;
static signed short g_mouse_refresh_flag = -1;

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

#if !defined(__BORLANDC__)
static SDL_Cursor *g_sdl_cursor = NULL;
static Uint8 *g_sdl_cursor_data_r = NULL;
static Uint8 *g_sdl_cursor_mask_r = NULL;
static Uint8 g_sdl_cursor_data_o[16*16];
static Uint8 g_sdl_cursor_mask_o[16*16];
#else
static char g_mouse_backbuffer[16 * 16];
static unsigned short *g_mouse_current_cursor = g_mouse_mask;
#endif


struct mouse_action {
	signed short x1;
	signed short y1;
	signed short x2;
	signed short y2;
	signed short action;
};

static const struct mouse_action g_action_base[9] = {
	{ 272, 8, 304, 41, 0xfd},		/* credits */
	{ 305, 7, 319, 21, KEY_CTRL_F3},	/* change sex */
	{ 145, 13, 175, 21, KEY_CTRL_F4},	/* enter name */
	{ 271, 42, 286, 56, KEY_UP},		/* previous head */
	{ 288, 42, 303, 56, KEY_DOWN},		/* next head */
	{ 145, 178, 164, 192, KEY_LEFT},	/* previous page */
	{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
	{ 0, 0, O_WIDTH - 1, O_HEIGHT - 1, 0xfe},
	{ -1, -1, -1, -1, -1}
};

static const struct mouse_action g_action_skills[4] = {
	{ 145, 178, 164, 192, KEY_LEFT},	/* previous page */
	{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
	{ 0, 0, O_WIDTH - 1, O_HEIGHT - 1, 0xfe},
	{ -1, -1, -1, -1, -1}
};

static const struct mouse_action g_action_spells[4] = {
	{ 16, 178, 35, 192, KEY_LEFT},		/* previous page */
	{ 284, 178, 303, 192, KEY_RIGHT},	/* next page */
	{ 0, 0, O_WIDTH - 1, O_HEIGHT - 1, 0xfe},
	{ -1, -1, -1, -1, -1}
};

static const struct mouse_action g_action_default[2] = {
	{ 0, 0, O_WIDTH - 1, O_HEIGHT - 1, 0xfe},
	{ -1, -1, -1, -1, -1}
};

static const struct mouse_action g_action_input[2] = {
	{ 0, 0, O_WIDTH - 1, O_HEIGHT -1, KEY_RET},
	{ -1, -1, -1, -1, -1}
};

static const struct mouse_action* g_action_page[] = {
	g_action_base, g_action_skills, g_action_skills,
	g_action_skills, g_action_skills, g_action_spells,
	g_action_spells, g_action_spells, g_action_spells,
	g_action_spells, g_action_spells
};

static const struct mouse_action* g_default_action = &g_action_default[0];

static const struct mouse_action* g_action_table = NULL;


static volatile struct struct_hero g_hero = {0};

#if !defined(__BORLANDC__)
static unsigned char* g_bg_buffer[]       = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
#endif
static unsigned char *g_typus_buffer[]    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static signed long g_typus_len[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* FILE MANAGEMENT VARIABLES */
static const char g_fname00_de[] = "GEN1.NVF";
static const char g_fname01_de[] = "GEN2.NVF";
static const char g_fname02_de[] = "GEN3.NVF";
static const char g_fname03_de[] = "GEN4.NVF";
static const char g_fname04_de[] = "GEN5.NVF";
static const char g_fname05_de[] = "GEN6.NVF";
static const char g_fname06_de[] = "GEN7.NVF";
static const char g_fname07_de[] = "GEN8.NVF";
static const char g_fname08_de[] = "GEN9.NVF";
static const char g_fname09_de[] = "GEN10.NVF";
static const char g_fname10_de[] = "GEN11.NVF";
static const char g_fname11_de[] = "HEADS.DAT";
static const char g_fname12_de[] = "SEX.DAT";
static const char g_fname13_de[] = "CREATECA.AWS";
static const char g_fname14_de[] = "FONT6";
static const char g_fname15_de[] = "GENTEXT";
static const char g_fname16_de[] = "DSALOGO.DAT";
static const char g_fname17_de[] = "GENTIT.DAT";
static const char g_fname18_de[] = "ATTIC";
static const char g_fname19_de[] = "POPUP.DAT";
static const char g_fname20_de[] = "DGAUKLER.DAT";
static const char g_fname21_de[] = "DJAEGER.DAT";
static const char g_fname22_de[] = "DKRIEGER.DAT";
static const char g_fname23_de[] = "DSTREUNE.DAT";
static const char g_fname24_de[] = "DTHORWAL.DAT";
static const char g_fname25_de[] = "DZWERG.DAT";
static const char g_fname26_de[] = "DHEXE.DAT";
static const char g_fname27_de[] = "DDRUIDE.DAT";
static const char g_fname28_de[] = "DMAGIER.DAT";
static const char g_fname29_de[] = "DAELF.DAT";
static const char g_fname30_de[] = "DFELF.DAT";
static const char g_fname31_de[] = "DWELF.DAT";
static const char g_fname32_de[] = "DMENGE.DAT";
static const char g_fname33_de[] = "GEN.XMI";
static const char g_fname34_de[] = "FANPRO.NVF";
static const char g_fname35_de[] = "SAMPLE.AD";
static const char g_fname36_de[] = "MT32EMUL.XMI";
static const char g_fname37_de[] = "ROALOGUS.DAT";

static const char* g_fnames_g105de[] = {
	g_fname00_de, g_fname01_de, g_fname02_de, g_fname03_de, g_fname04_de,
	g_fname05_de, g_fname06_de, g_fname07_de, g_fname08_de, g_fname09_de,
	g_fname10_de, g_fname11_de, g_fname12_de, g_fname13_de, g_fname14_de,
	g_fname15_de, g_fname16_de, g_fname17_de, g_fname18_de, g_fname19_de,
	g_fname20_de, g_fname21_de, g_fname22_de, g_fname23_de, g_fname24_de,
	g_fname25_de, g_fname26_de, g_fname27_de, g_fname28_de, g_fname29_de,
	g_fname30_de, g_fname31_de, g_fname32_de, g_fname33_de, g_fname34_de,
	g_fname35_de, g_fname36_de, g_fname37_de
};

static const char g_fname13_de_alt[] = "TYPPIC.DAT";
static const char g_fname33_de_alt[] = "GEN.AWS";

static const char* g_fnames_g100de[] = {
	g_fname00_de, g_fname01_de, g_fname02_de, g_fname03_de, g_fname04_de,
	g_fname05_de, g_fname06_de, g_fname07_de, g_fname08_de, g_fname09_de,
	g_fname10_de, g_fname11_de, g_fname12_de, g_fname13_de_alt, g_fname14_de,
	g_fname15_de, g_fname16_de, g_fname17_de, g_fname18_de, g_fname19_de,
	g_fname20_de, g_fname21_de, g_fname22_de, g_fname23_de, g_fname24_de,
	g_fname25_de, g_fname26_de, g_fname27_de, g_fname28_de, g_fname29_de,
	g_fname30_de, g_fname31_de, g_fname32_de, g_fname33_de_alt, g_fname34_de
};

static const char g_fname00_en[] = "E_GEN1.NVF";
static const char g_fname01_en[] = "E_GEN2.NVF";
static const char g_fname02_en[] = "E_GEN3.NVF";
static const char g_fname03_en[] = "E_GEN4.NVF";
static const char g_fname04_en[] = "E_GEN5.NVF";
static const char g_fname05_en[] = "E_GEN6.NVF";
static const char g_fname06_en[] = "E_GEN7.NVF";
static const char g_fname07_en[] = "E_GEN8.NVF";
static const char g_fname08_en[] = "E_GEN9.NVF";
static const char g_fname09_en[] = "E_GEN10.NVF";
static const char g_fname10_en[] = "E_GEN11.NVF";
static const char g_fname15_en[] = "E_GENTXT";
static const char g_fname16_en[] = "ROALOGUK.DAT";
static const char g_fname17_en[] = "E_GENTIT.NVF";

static const char* g_fnames_g300en[] = {
	g_fname00_en, g_fname01_en, g_fname02_en, g_fname03_en, g_fname04_en,
	g_fname05_en, g_fname06_en, g_fname07_en, g_fname08_en, g_fname09_en,
	g_fname10_en, g_fname11_de, g_fname12_de, g_fname13_de, g_fname14_de,
	g_fname15_en, g_fname16_en, g_fname17_en, g_fname18_de, g_fname19_de,
	g_fname20_de, g_fname21_de, g_fname22_de, g_fname23_de, g_fname24_de,
	g_fname25_de, g_fname26_de, g_fname27_de, g_fname28_de, g_fname29_de,
	g_fname30_de, g_fname31_de, g_fname32_de, g_fname33_de, g_fname34_de,
	g_fname35_de, g_fname36_de, g_fname37_de
};

static const char **g_fnames;

static const char g_str_dsagen_dat[] = "DSAGEN.DAT";
static const char g_str_file_missing[] = { "FILE %s IS MISSING!" };

enum e_language { LANG_UNDEF = 0, LANG_DE = 1, LANG_EN = 2};
static int g_dsagen_lang = 0;
enum e_medium { MED_UNDEF = 0, MED_DISK = 1, MED_CD = 2};
static int g_dsagen_medium = 0;

static signed long g_gendat_offset;
static signed long g_flen;

static const char g_str_chr[] = ".CHR";
#if defined(__BORLANDC__) || defined(_WIN32)
static const char g_str_temp_dir[] = "TEMP\\";
#endif
static char g_str_save_error_de[] = "@SPEICHER FEHLER!@EVENTUELL DISKETTE GESCH\x9aTZT?";
static char g_str_save_error_en[] = "@SAVE ERROR!@IS YOUR DISK PROTECTED?";


struct struct_chr_lookup {
	unsigned char chr;
	signed char index;
	signed char width;
};

static const struct struct_chr_lookup g_chr_lookup[74] = {
	{0x20, 0, 6}, // SPACE
	{0x41, 1, 6}, // A
	{0x42, 2, 6}, // B
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
	{0x5a, 26, 6}, // Z
	{0x8e, 27, 6}, // AE
	{0x99, 28, 6}, // OE
	{0x9a, 29, 6}, // UE
	{0x2c, 30, 3}, // ,
	{0x2e, 31, 2}, // .
	{0x2d, 32, 6}, // -
	{0x28, 33, 4}, // (
	{0x29, 34, 4}, // )
	{0x21, 35, 3}, // !
	{0x3f, 36, 5}, // ?
	{0x30, 37, 6}, // 0
	{0x31, 38, 3},
	{0x32, 39, 6},
	{0x33, 40, 6},
	{0x34, 41, 6},
	{0x35, 42, 6},
	{0x36, 43, 6},
	{0x37, 44, 6},
	{0x38, 45, 6},
	{0x39, 46, 6}, // 9
	{0x27, 47, 3}, // ´
	{0x3a, 48, 3}, // :
	{0x23, 49, 6}, // Star
	{0x26, 50, 5}, // Cross
	{0x25, 51, 5}, // Cross turned
	{0x3c, 53, 5}, 
	{0x3e, 52, 5},
	{0x2f, 54, 6}, // /
	{0x5f, 55, 6}, // _
	{0xc8, 56, 6}, // Rune: Naudiz
	{0xc9, 57, 6}, // Rune: Othala
	{0xca, 58, 6}, // Rune: Thurisaz
	{0xcb, 59, 5}, // Rune: Raidho
	{0xcc, 60, 5}, // Rune: Sowilo
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
	{0xd9, 73, 5}, // End Runes
};

static signed short g_bool_mode = 0;

static char g_clear_archetype_pic = 1;

struct type_bitmap {
	char t[13];
};

static const struct type_bitmap g_type_bitmap = { {0} };

static const char g_str_version[] = "V1.07";

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

static const unsigned char g_pal_col_white[] = { 0x3f, 0x3f, 0x3f };

static const unsigned char g_pal_col_black[] = { 0x00, 0x00, 0x00 };

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

static unsigned char g_pal_roalogo[768];

static const char g_str_malloc_error[] = "\xaMEMORY MALLOCATION ERROR!";

static signed short g_random_gen_seed = 0x327b;
/* END OF INITIALIZED GLOBAL VARIABLES _DATA */

/* START OF UNINITIALIZE GLOBAL VARIABLE _BSS DS:0x2474*/
#if defined(__BORLANDC__)
static signed short g_display_mode_bak;
static signed short g_display_page_bak;
#endif

/* VGA output buffer */
unsigned char* g_vga_memstart;
/* buffer for the modified background image */
static unsigned char* g_vga_backbuffer;
/* pointer to g_vga_memstart or g_vga_backbuffer */
static unsigned char* g_gfx_ptr;
/* intermediate buffer for RLE compressed GEN.NVF background and PP20 archetype images */
static unsigned char* g_page_buffer;

static char *g_textbuffer;  // buffer for dynamically created strings
static unsigned char *g_buffer_typus;
static unsigned char *g_buffer_dmenge_dat;
static unsigned char *g_buffer_current_head;
static unsigned char *g_name_bar;
static unsigned char *g_arrow_area;
static unsigned char *g_mr_bar;
static unsigned char *g_popup_box;

static int g_use_solid_bg = 0;
static int g_fg_color[4] = {0xff, 0xc8, 0xc9, 0xca}; /* {WHITE, RED, YELLOW, BLUE} */
static int g_bg_color = 0; /* BLACK */
static signed short g_col_index;

static unsigned char *g_buffer_font6;
static char  *g_buffer_text;
static unsigned char *g_buffer_heads_dat;
static unsigned char *g_buffer_popup_dat;
static unsigned char *g_buffer_sex_dat;

static signed short g_in_key_ext;
static signed short g_in_key_ascii;
static signed short g_random_gen_seed2;

static char* g_texts[301];
#if !defined(__BORLANDC__)
/* usage: output */
static inline char* get_text(signed short no) {
	return g_texts[no];
}
#else
#define get_text(no) (g_texts[no])
#endif

static signed char g_in_intro;

/* the typus for the heads e.G. all elves are 10 */
static signed char g_head_typus;
/* the index of the current head */
static signed char g_head_current;
/* the index of the first head */
static signed char g_head_first;
/* the index of the last head */
static signed char g_head_last;

static int g_got_mu_bonus;
static int g_got_ch_bonus;

static char* g_type_names[12];
static signed char g_attrib_changed[14];

struct inc_states {
	signed char tries;
	signed char incs;
};

static struct inc_states g_skill_incs[52];
static struct inc_states g_spell_incs[86];

signed short g_called_with_args;

/* MUSIC MANAGEMENT VARIABLES */

enum e_music { MUSIC_OFF = 0, MUSIC_CDA = 1, MUSIC_XMID = 2, MUSIC_AWS = 3};
static int g_music = MUSIC_CDA; /* default is CDA/FLAC */

#if defined(__BORLANDC__)
static const char g_str_sound_cfg[] = "SOUND.CFG";
static const char g_str_sound_adv[] = "SOUND.ADV";
static char g_str_soundhw_not_found[] = "SOUND HARDWARE NOT FOUND!";

static signed short g_snd_driver_handle;
static signed short g_snd_sequence;
static unsigned char* g_snd_driver_desc;
static unsigned char* g_snd_driver_base_addr;
static signed long g_state_table_size;
static signed short g_timbre_cache_size;
static signed short g_handle_timbre;

/* These 6 bytes are written at once from a file */
static unsigned long g_current_timbre_offset;
static signed char g_current_timbre_bank;
static signed char g_current_timbre_patch;

static unsigned short g_current_timbre_length;

static void* g_state_table;
static unsigned char* g_snd_timbre_cache;
static void* g_form_xmid;
static unsigned char* g_snd_driver;
#else
static Mix_Music *music = NULL;
#endif

#if defined(__BORLANDC__)
void far *g_irq78_bak;
void far *g_timer_isr_bak;
#else
static SDL_TimerID g_sdl_timer_id = 0;
static SDL_mutex *g_sdl_timer_mutex = NULL;
#endif

/* GUI VARIABLES AND FUNCS */
static int g_menu_tiles = 3; /* number of menu tiles width {3,4} */
static int g_in_infobox = 0;
static signed short g_text_x_mod = 0;
static signed short g_text_x_end;

/* MEMORY MANGEMENT VARIABLES */
static int g_allocs = 0;
static unsigned long g_max_mem_usage = 0;


/* MEMORY MANAGEMENT */

/**
 * \brief allocate memory
 * \param nelem number of bytes
 * \return pointer to memory location or NULL
 */
static unsigned char *gen_alloc(unsigned long nelem)
{
	unsigned char *p;
#if defined(__BORLANDC__)
	p = (unsigned char*)farcalloc(nelem, 1);
#else
	p = (unsigned char*)calloc(nelem, 1);
#endif

	if (p != NULL) {
		g_allocs++;
		g_max_mem_usage += nelem;
	}
	return p;
}

/**
 * \brief free memory
 * \param[in] p pointer
 */
static void gen_free(unsigned char *p)
{
	if (p != NULL) {
		free(p);
		g_allocs--;
	}
}

/**
 * \brief print memory stats
 */
static void gen_memstat(char *str)
{
	sprintf(str, "\xf1MEMSTAT\xf0@@NR ALLOCS \xf2%d\xf0@@MAX MEM USAGE \xf2%ld\xf0 KB\xd",
		       g_allocs, g_max_mem_usage / 1024);
}

/**
 * \brief allocate most of the buffers needed at runtime
 * \return number of failed allocations or 0 if none
 */
static int alloc_buffers(void)
{
	int errors = 0;

#if defined(__BORLANDC__)
	g_vga_memstart = (unsigned char*)MK_FP(0xa000, 0x0);
#else
	g_vga_memstart = (unsigned char*)calloc(O_WIDTH * O_HEIGHT, 1);
#endif
	if (g_vga_memstart == NULL) errors++;
	g_gfx_ptr = g_vga_memstart;

	g_vga_backbuffer = (gen_alloc(64108) + 8);
	if (g_vga_backbuffer == NULL) errors++;

	g_page_buffer = gen_alloc(50000);
	if (g_page_buffer == NULL) errors++;

	g_popup_box = gen_alloc(32 * (4 + 1) * O_HEIGHT);
	if (g_popup_box == NULL) errors++;

	g_textbuffer = (char*)gen_alloc(1524);
	if (g_textbuffer == NULL) errors++;

	g_buffer_text = (char*)gen_alloc(6000);
	if (g_buffer_text == NULL) errors++;

	g_buffer_font6 = gen_alloc(592);
	if (g_buffer_font6 == NULL) errors++;

	g_buffer_heads_dat = gen_alloc(39000);
	if (g_buffer_heads_dat == NULL) errors++;

	g_buffer_popup_dat = (gen_alloc(1673) + 8);
	if (g_buffer_popup_dat == NULL) errors++;

	g_buffer_sex_dat = gen_alloc(812);
	if (g_buffer_sex_dat == NULL) errors++;

	g_buffer_typus = (gen_alloc(23660) + 8);
	if (g_buffer_typus == NULL) errors++;

	g_buffer_dmenge_dat = (gen_alloc(23660) + 8);
	if (g_buffer_dmenge_dat == NULL) errors++;

	g_name_bar = gen_alloc(720);	/* 15 chars * 6 pixel / char * 8 pixel */
	if (g_name_bar == NULL) errors++;

	g_buffer_current_head = (gen_alloc(1024 + 8) + 8);
	if (g_buffer_current_head == NULL) errors++;

	g_arrow_area = gen_alloc(4096);
	if (g_arrow_area == NULL) errors++;

	g_mr_bar = gen_alloc(1024);
	if (g_mr_bar == NULL) errors++;

#if !defined(__BORLANDC__)
	for (int i = 0; i <= 10; i++) {
		g_bg_buffer[i] = gen_alloc(O_WIDTH * O_HEIGHT);
		if (g_bg_buffer[i] == NULL) errors++;
	}
#endif

	if (errors > 0) {
		fprintf(stderr, g_str_malloc_error);
	}

	return errors;
}

/**
 * \brief free all memory buffers
 */
void free_buffers(void)
{
	int i;

	if (g_vga_memstart) {
#if !defined(__BORLANDC__)
		gen_free(g_vga_memstart);
#endif
		g_vga_memstart = NULL;
		g_gfx_ptr = NULL;
	}

	if (g_buffer_sex_dat != NULL) {
		gen_free(g_buffer_sex_dat);
		g_buffer_sex_dat = NULL;
	}

	if (g_buffer_popup_dat != NULL) {
		gen_free(g_buffer_popup_dat - 8);
		g_buffer_popup_dat = NULL;
	}

	if (g_buffer_heads_dat != NULL) {
		gen_free(g_buffer_heads_dat);
		g_buffer_heads_dat = NULL;
	}

	if (g_buffer_text != NULL) {
		gen_free((unsigned char*)g_buffer_text);
		g_buffer_text = NULL;
	}

	if (g_buffer_font6 != NULL) {
		gen_free(g_buffer_font6);
		g_buffer_font6 = NULL;
	}

	if (g_name_bar != NULL) {
		gen_free(g_name_bar);
		g_name_bar = NULL;
	}

	if (g_buffer_current_head != NULL) {
		gen_free(g_buffer_current_head - 8);
		g_buffer_current_head = NULL;
	}

	if (g_arrow_area != NULL) {
		gen_free(g_arrow_area);
		g_arrow_area = NULL;
	}

	if (g_mr_bar != NULL) {
		gen_free(g_mr_bar);
		g_mr_bar = NULL;
	}

	if (g_popup_box != NULL) {
		gen_free(g_popup_box);
		g_popup_box = NULL;
	}

	if (g_buffer_dmenge_dat != NULL) {
		gen_free(g_buffer_dmenge_dat - 8);
		g_buffer_dmenge_dat = NULL;
	}

	if (g_buffer_typus != NULL) {
		gen_free(g_buffer_typus - 8);
		g_buffer_typus = NULL;
	}

	if (g_textbuffer != NULL) {
		gen_free((unsigned char*)g_textbuffer);
		g_textbuffer = NULL;
	}

	if (g_page_buffer != NULL) {
		gen_free(g_page_buffer);
		g_page_buffer = NULL;
	}

	if (g_vga_backbuffer != NULL) {
		gen_free(g_vga_backbuffer - 8);
		g_vga_backbuffer = NULL;
	}

#if defined(__BORLANDC__)
	if (g_snd_timbre_cache != NULL) {
		gen_free(g_snd_timbre_cache);
		g_snd_timbre_cache = NULL;
	}

	if (g_state_table != NULL) {
		gen_free(g_state_table);
		g_state_table = NULL;
	}

	if (g_snd_driver != NULL) {
		gen_free(g_snd_driver);
		g_snd_driver = NULL;
	}

	if (g_form_xmid != NULL) {
		gen_free(g_form_xmid);
		g_form_xmid = NULL;
	}
#endif

#if !defined(__BORLANDC__)
	for (i = 0; i <= 10; i++) {
		if (g_bg_buffer[i] != NULL) {
			gen_free(g_bg_buffer[i]);
			g_bg_buffer[i] = NULL;
		}
	}
#endif

	for (i = 0; i < 13; i++) {
		if (g_typus_buffer[i] != NULL) {
			gen_free(g_typus_buffer[i]);
			g_typus_buffer[i] = NULL;
		}
	}
}

/* RANDOM NUMBER GENERATOR MANAGEMENT */

/**
 * \brief generates a positive random number in {1, ..., n}
 * \param[in] n maximal random number
 * \return random number or 0
 */
static signed short random_gen(const signed short n)
{
	signed short retval;

	if (n == 0) {
		return 0;
	}

	retval = g_random_gen_seed ^ g_random_gen_seed2;
	retval = gen_rotl(retval, 2);
	retval = (retval + g_random_gen_seed2) ^ g_random_gen_seed;
	retval = gen_rotl(retval, 3);
	g_random_gen_seed = retval;

	return abs(retval) % n + 1;
}

/**
 * \brief generates a non-negative random number between lo and hi
 * \param[in] lo lowest value
 * \param[in] hi highest value
 * \return random number
*/
static signed short random_interval_gen(const signed short lo, const signed short hi)
{
	return lo + random_gen(hi - lo + 1) - 1;
}

/**
 * \brief checks if val is in a word array
 * \param[in] val value to check for
 * \param[in] p pointer to array
 * \return 1 iff found otherwise 0
 */
static int is_in_word_array(const int val, const signed short *p)
{
	while (*p >= 0) {
		if (*p++ == val) {
			return 1;
		}
	}

	return 0;
}

/* MOUSE MANAGEMENT */

#if defined(__BORLANDC__)
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
static void do_mouse_action(unsigned short *p1, unsigned short *p2, unsigned short *p3, unsigned short *p4, unsigned short *p5)
{
	union REGS myregs;
	struct SREGS mysregs;

	if (readws(p1) >= 0) {
		myregs.x.ax = *p1;
		myregs.x.bx = *p2;
		myregs.x.cx = *p3;

		switch (*p1) {
			case 0x9:	/* define Cursor in graphic mode */
			case 0xc:	/* install event handler */
			case 0x14:	/* swap event handler */
			case 0x16:	/* save mouse state */
			case 0x17:	/* load mouse state */
				myregs.x.dx = *p4;
				mysregs.es = *p5;
				break;
			case 0x10:	/* define screen region for update */
				myregs.x.cx = *p2;
				myregs.x.dx = *p3;
				myregs.x.si = *p4;
				myregs.x.di = *p5;
				break;
			default:
				myregs.x.dx = *p4;
		}

		int86x(0x33, &myregs, &myregs, &mysregs);

		if (*p1 == 0x14) {
			*p2 = mysregs.es;
		} else {
			*p2 = myregs.x.bx;
		}

		*p1 = myregs.x.ax;
		*p3 = myregs.x.cx;
		*p4 = myregs.x.dx;
	}
}

static void interrupt mouse_isr(void)
{
	signed short l_si = _AX;
	unsigned short p1;
	unsigned short p2;
	unsigned short p3;
	unsigned short p4;
	unsigned short p5;

	if (g_mouse_locked == 0) {
		if (l_si & 0x2) {
			g_mouse_leftclick_event = 1;
		}
		if (l_si & 0x8) {
			g_mouse_rightclick_event = 1;
		}
		if (l_si & 0x1) {
			/* Report status: position */
			p1 = 3;
			p3 = g_mouse_posx;
			p4 = g_mouse_posy;

			do_mouse_action(&p1, &p2, &p3, &p4, &p5);

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

			do_mouse_action(&p1, &p2, &p3, &p4, &p5);

			g_mouse_moved = 1;
		}
	}
}

static void mouse_call_isr(void)
{
	mouse_isr();
}

static void mouse_do_enable(const signed short val, unsigned char* ptr)
{
	unsigned short p1, p2, p3, p4, p5;

	p1 = 0x0c;
	p3 = val;
	p4 = (unsigned short)FP_OFF(mouse_call_isr);
	p5 = (unsigned short)FP_SEG(mouse_call_isr);

	/* save adress of old IRQ 0x78 */
	g_irq78_bak = ((void interrupt far (*)(void))getvect(0x78));

	/* set new IRQ 0x78 */
	setvect(0x78, (void interrupt far (*)(void))ptr);

	/* set the new mouse event handler */
	do_mouse_action(&p1, &p2, &p3, &p4, &p5);
}
#endif

#if !defined(__BORLANDC__)
static void sdl_mouse_cursor_scaled(void)
{
	unsigned char src_m[16*16];
	unsigned char *dst_m = NULL;
	const int ratio = sdl_get_ratio();
	int src_off = 0;
	int dst_off = 0;

	if (g_sdl_cursor) {
		SDL_FreeCursor(g_sdl_cursor);
		g_sdl_cursor = NULL;
	}

	if (g_sdl_cursor_data_r != NULL) {
		gen_free(g_sdl_cursor_data_r);
		g_sdl_cursor_data_r = NULL;
	}
	g_sdl_cursor_data_r = calloc(ratio * ratio * 16 * 16, 1);

	if (g_sdl_cursor_mask_r != NULL) {
		gen_free(g_sdl_cursor_mask_r);
		g_sdl_cursor_mask_r = NULL;
	}
	g_sdl_cursor_mask_r = calloc(ratio * ratio * 16 * 16, 1);

	dst_m = calloc(ratio * ratio * 16 * 16 * 8, 1);

	if (g_sdl_cursor_data_r && g_sdl_cursor_mask_r && dst_m) {

		//fprintf(stderr, "%s() ratio = %d\n", __func__, ratio);
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 7; k >= 0; k--) {
					src_m[src_off] = (g_sdl_cursor_mask_o[src_off] >> k) & 1;
					for (int l = 0; l < ratio; l++) {
						dst_m[dst_off] = src_m[src_off];
						dst_off++;
					}
					//fprintf(stderr, "%x", src_m[src_off]);
				}
				src_off++;
			}
			for (int j = 1; j < ratio; j++) {
				memcpy(dst_m + dst_off, dst_m + dst_off - 8 * 2 * ratio, 8 * 2 * ratio);
				dst_off += 8 * 2 * ratio;
			}
			//fprintf(stderr, "\n");
		}
		//fprintf(stderr, "ratio = %d src_off = %d dst_off = %d\n", ratio, src_off, dst_off);
#if 0
		fprintf(stderr, "----------------------\n");
		fflush(stderr);
		dst_off = 0;
		for (int i = 0; i < 16 * ratio; i++) {
			for (int j = 0; j < (2 * 8 * ratio); j++) {
				fprintf(stderr, "%x", dst_m[dst_off++]);
			}
			fprintf(stderr, "\n");
		}
		fflush(stderr);
		fprintf(stderr, "----------------------\n");
#endif

		src_off = 0;
		dst_off = 0;
		for (int i = 0; i < 16 * ratio; i++) {
			for (int j = 0; j < 16 * ratio; j++) {
				Uint8 v = 0;
				for (int k = 7; k >= 0; k--) {
					v |= (dst_m[src_off] << k);
					src_off++;
				}
				g_sdl_cursor_mask_r[dst_off]  = v;
				dst_off++;
			}
		}
		gen_free(dst_m);

		g_sdl_cursor = SDL_CreateCursor(g_sdl_cursor_data_r, g_sdl_cursor_mask_r, ratio * 16, ratio * 16, 0, 0);
		SDL_SetCursor(g_sdl_cursor);
	} else {
		fprintf(stderr, "ERROR: faile to allocate mouse scale memory\n");
	}
}
#endif

/**
 * \brief initialize mouse and cursor position
 * \note The variable g_have_mouse may become 0 iff no mouse was found (DOS)
 */
static void mouse_enable(void)
{
	if (g_have_mouse == 2) {

#if defined(__BORLANDC__)
		unsigned short p1, p2, p3, p4, p5;

		/* initialize mouse */
		p1 = 0;

		do_mouse_action(&p1, &p2, &p3, &p4, &p5);

		if (p1 == 0) {
			g_have_mouse = 0;
			g_mouse_refresh_flag = -2;
		}

		g_mouse_current_cursor = g_mouse_mask;

		/* move cursor to initial position */
		if (g_have_mouse == 2) {
			p1 = 4;
			p3 = g_mouse_posx;
			p4 = g_mouse_posy;

			do_mouse_action(&p1, &p2, &p3, &p4, &p5);
			mouse_do_enable(0x1f, (unsigned char*)&mouse_isr);
		}
#else

		signed short *mouse_cursor = (signed short*)g_mouse_mask + 16;
		int i = -1;

		SDL_ShowCursor(SDL_DISABLE);

		for (int Y = 0; Y < 16; Y++) {
			signed short mask = *mouse_cursor++;
			for (int X = 0; X < 16; X++) {
				if (X % 8) {
					g_sdl_cursor_data_o[i] <<= 0x01;
					g_sdl_cursor_mask_o[i] <<= 0x01;
				} else {
					i += 1;
					g_sdl_cursor_data_o[i] = 0x00;
					g_sdl_cursor_mask_o[i] = 0x00;
				}

				if ((0x8000 >> X) & mask) {
					g_sdl_cursor_data_o[i] |= 0x00;
					g_sdl_cursor_mask_o[i] |= 0x01;
					//fprintf(stderr, "X");
				} else {
					g_sdl_cursor_data_o[i] |= 0x00;
					g_sdl_cursor_mask_o[i] |= 0x00;
					//fprintf(stderr, " ");
				}
			}
			//fprintf(stderr, "\n");
		}

		sdl_mouse_cursor_scaled();

		/* move cursor to initial position */
		const int ratio = sdl_get_ratio();
		SDL_WarpMouseInWindow(sdl_get_window(), g_mouse_posx * ratio, g_mouse_posy * ratio);
#endif
	}
}

/**
 * \brief uninitialize mouse and cursor position
 * \note used by cda code (DOS)
 */
void mouse_disable(void)
{
	if (g_have_mouse == 2) {
#if defined(__BORLANDC__)
		unsigned short p1, p2, p3, p4, p5;

		/* restore the old int 0x78 handler */
		setvect(0x78, (void interrupt far (*)(void))g_irq78_bak);

		/* uninstall mouse event handler */
		p1 = 0x0c;
		p3 = 0;
		p4 = 0;
		p5 = 0;

		do_mouse_action(&p1, &p2, &p3, &p4, &p5);
#else
		SDL_ShowCursor(SDL_DISABLE);

		SDL_FreeCursor(g_sdl_cursor);

		gen_free(g_sdl_cursor_data_r);
		g_sdl_cursor_data_r = NULL;
		gen_free(g_sdl_cursor_mask_r);
		g_sdl_cursor_mask_r = NULL;
		g_sdl_cursor = NULL;
#endif
	}
}

/**
 * \brief move the mouse cursor to a position
 * \param[in] x X - coordinate
 * \param[in] y Y - coordinate
 */
static void mouse_move_cursor(const signed short x, const signed short y)
{
#if defined(__BORLANDC__)
	unsigned short p1, p2, p3, p4, p5;

	p1 = 4;
	p3 = x;
	p4 = y;

	do_mouse_action(&p1, &p2, &p3, &p4, &p5);
#else
	const int ratio = sdl_get_ratio();
	SDL_WarpMouseInWindow(sdl_get_window(), x * ratio, y * ratio);
#endif
}

static void mouse_cursor_draw(void)
{
#if defined(__BORLANDC__)
	unsigned char *vgaptr;
	signed short *cursor;
	signed short rangeX;
	signed short rangeY;
	signed short diffX;
	signed short diffY;
	signed char Y;
	signed char X;

	signed short mask;

	cursor = (signed short*)g_mouse_current_cursor + (32 / 2);

	rangeX = g_mouse_posx;
	rangeY = g_mouse_posy;

	diffX = diffY = 16;

	if (rangeX > (O_WIDTH - 16)) diffX = O_WIDTH - rangeX;
	if (rangeY > (O_HEIGHT - 16)) diffY = O_HEIGHT - rangeY;

	vgaptr = g_vga_memstart + O_WIDTH * rangeY + rangeX;

	for (Y = 0; Y < diffY; Y++) {
		mask = *cursor++;
		for (X = 0; X < diffX; X++)
			if ((0x8000 >> X) & mask)
				vgaptr[X] = 0xff;

		vgaptr += O_WIDTH;
	}
#endif
}

static void mouse_save_bg(void)
{
#if defined(__BORLANDC__)
	unsigned char *vgaptr;
	signed short rangeX;
	signed short rangeY;
	signed short diffX;
	signed short diffY;
	signed short Y;
	signed short X;

	rangeX = g_mouse_posx;
	rangeY = g_mouse_posy;

	diffX = diffY = 16;

	if (rangeX > (O_WIDTH - 16)) diffX = O_WIDTH - rangeX;
	if (rangeY > (O_HEIGHT - 16)) diffY = O_HEIGHT - rangeY;

	vgaptr = g_vga_memstart + O_WIDTH * rangeY + rangeX;

	for (Y = 0; Y < diffY; vgaptr += O_WIDTH, Y++)
		for (X = 0; X < diffX; X++)
			g_mouse_backbuffer[16 * Y + X] = vgaptr[X];
#endif
}

static void mouse_restore_bg(void)
{
#if defined(__BORLANDC__)
	unsigned char *vgaptr;
	signed short rangeX;
	signed short rangeY;
	signed short diffX;
	signed short diffY;
	signed short i;
	signed short j;

	rangeX = g_mouse_posx_bak;
	rangeY = g_mouse_posy_bak;
	diffX = diffY = 16;

	if (rangeX > (O_WIDTH - 16))  diffX = O_WIDTH - rangeX;
	if (rangeY > (O_HEIGHT - 16)) diffY = O_HEIGHT - rangeY;

	vgaptr = g_vga_memstart + O_WIDTH * rangeY + rangeX;

	for (i = 0; i < diffY; vgaptr += O_WIDTH, i++)
		for (j = 0; j < diffX; j++)
			vgaptr[j] = g_mouse_backbuffer[16 * i + j];

#endif
}

/* used by cda code */
void mouse_bg(void)
{
	if (g_mouse_locked == 0) {

		if (g_mouse_refresh_flag == 0) {
			g_mouse_locked = 1;
			mouse_restore_bg();
			g_mouse_locked = 0;
		}
		g_mouse_refresh_flag--;
	}
}

static void mouse_cursor(void)
{
	if (g_mouse_locked == 0) {

		g_mouse_refresh_flag++;

		if (g_mouse_refresh_flag == 0) {

			g_mouse_locked = 1;

			if (g_mouse_posx < 0)	g_mouse_posx = 0;

			if (g_mouse_posx > O_WIDTH - 5)	g_mouse_posx = O_WIDTH - 5;

			if (g_mouse_posy < 0)	g_mouse_posy = 0;

			if (g_mouse_posy > O_HEIGHT - 5) g_mouse_posy = O_HEIGHT - 5;

			mouse_save_bg();

			g_mouse_posx_bak = g_mouse_posx;
			g_mouse_posy_bak = g_mouse_posy;

			mouse_cursor_draw();

			g_mouse_locked = 0;
		}
	}
}

/**
 * \brief checks for mouse motion and updates the cursor
 */
static void mouse_motion(void)
{
	if (g_mouse_moved) {
		g_mouse_moved = 0;

		mouse_bg();
		mouse_cursor();
	}
}

/**
 * \brief translate a leftclick into an extendet keycode
 * \param[in] x X-Coordinate of the leftclick
 * \param[in] y Y-Coordinate of the leftclick
 * \param[in] act the action table of of the current page
 * \return an extended keycode or 0 if none was found in the table
 **/
static signed short get_mouse_action(const signed short x, const signed short y, const struct mouse_action *act)
{
	int i;
	
	for (i = 0; act[i].x1 != -1; i++) {

		if (	(act[i].x1 <= x) && (act[i].x2 >= x) &&
			(act[i].y1 <= y) && (act[i].y2 >= y))
		{
			return act[i].action;
		}
	}

	return 0;
}

/* IMAGE MANAGEMENT */

/**
 * \brief decompress a RLE compressed picture of resolution 320x200
 * \param[in] dst destination
 * \param[in] src source
*/
static void decomp_rle(unsigned char *dst, unsigned char *src)
{
	int i;
	int j;
	int k;
	int n;

	for (i = 0; i < O_HEIGHT; i++) {

		j = 0;

		while (j < O_WIDTH) {

			if (*src == 0x7f) {
				src++;
				n = *src++;

				for (k = 0; k < n; k++)
					*dst++ = *src;

				src++;
				j += n;
			} else {
				*dst++ = *src++;
				j++;
			}
		}
	}
}

/**
 * \brief swaps bytes of an 32bit integer value
 * \param[in] v input value
 * \return swapped 32-bit integer value
 * \note let v = 0xdeadbeef => swap_u32(v) = 0xefbeadde
 * \note tested successfull with Linux GCC, CLANG on x86_64 and ARM
 * \note tested successfull with BCC with {-O, -G, -Od, -O1, -O2}
 */
static unsigned long swap_u32(const unsigned long v)
{
	unsigned char tmp[4] = {0};
	unsigned long retval = 0L;

	tmp[0] = (v >> 0) & 0xff;
	tmp[1] = (v >> 8) & 0xff;
	tmp[2] = (v >> 16) & 0xff;
	tmp[3] = (v >> 24) & 0xff;

	retval |= tmp[0];
	retval <<= 8L;
	retval |= tmp[1];
	retval <<= 8L;
	retval |= tmp[2];
	retval <<= 8L;
	retval |= tmp[3];

	return retval;
}

static signed long process_nvf(struct nvf_desc *nvf)
{
#if defined (__BORLANDC__)
	unsigned char huge *src = NULL;
#else
	unsigned char *src = NULL;
#endif
	signed long p_size = 0;
	signed long retval;
	signed long offs;
	signed short width = 0;
	signed short height = 0;
	signed short pics;
	signed short va;
	signed short i;
	signed char nvf_type;

	va = (nvf_type = *(unsigned char*)(nvf->src)) & 0x80;
	nvf_type &= 0x7f;
	pics = readws(nvf->src + 1L);

	if (nvf->no < 0)
		nvf->no = 0;

	if (nvf->no > pics - 1)
		nvf->no = pics - 1;

	switch (nvf_type) {

	case 0x00:
		width = readws(nvf->src + 3L);
		height = readws(nvf->src + 5L);
		p_size = width * height;
		memcpy(nvf->dst - 8L, nvf->src + p_size * nvf->no + 7L, p_size);
		break;
	case 0x01:
		offs = pics * 4 + 3L;
		for (i = 0; i < nvf->no; i++) {
			width = readws(nvf->src + i * 4 + 3L);
			height = readws(nvf->src + i * 4 + 5L);
			offs += width * height;
		}

		width = readws(nvf->src + nvf->no * 4 + 3L);
		height = readws(nvf->src + nvf->no * 4 + 5L);
		p_size = width * height;
		memcpy(nvf->dst - 8L, nvf->src + offs, p_size);
		break;

	case 0x02:
		width = readws(nvf->src + 3L);
		height = readws(nvf->src + 5L);
		offs = ((unsigned long)(pics * 4)) + 7L;
		for (i = 0; i < nvf->no; i++) {
			/* BCC adds here in offs = offs + value */
			offs += readds(nvf->src + i * 4 + 7L);
		}

		p_size = readds(nvf->src + nvf->no * 4 + 7L);
		memcpy(nvf->dst - 8L, nvf->src + offs, p_size);
		break;

	case 0x03:
		offs = pics * 8 + 3L;
		for (i = 0; i < (signed short)nvf->no; i++) {
			/* First two lines are not neccessary */
			width = readws(nvf->src + i * 8 + 3L);
			height = readws(nvf->src + i * 8 + 5L);

			offs += readds(nvf->src + i * 8 + 7L);
		}

		// Selected picture nvf->no, and copy it to nvf->dst
		width = readws(nvf->src + nvf->no * 8 + 3L);
		height = readws(nvf->src + nvf->no * 8 + 5L);
		p_size = readds(nvf->src + i * 8 + 7L);

		memcpy(nvf->dst - 8L, nvf->src + offs, p_size);
		break;
	}

	if (!nvf->type) {
		/* PP20 decompression */
		if (va != 0) {
			/* get size from unpacked picture */
			retval = readds(nvf->dst) - 8L;
			src = nvf->dst - 8L;
			src += (retval - 4L);
			retval = readd(src);
			retval = ((signed long)swap_u32(retval)) >> 8;

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


/* KEYBOARD AND INPUT MANAGEMENT */

#if !defined(__BORLANDC__)
static int g_sdl_quit_event = 0;

static int sdl_event_loop(const int cmd)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {

			if (!g_in_intro) {
				g_sdl_quit_event = 1;
			}

			if (cmd == 0) {
				/* return CTRL+Q as a keyboard event into the game */
				return (0x10 << 8) | KEY_DC1;
			}

		} else if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
				sdl_forced_update();
			}
		} else if (event.type == SDL_MOUSEMOTION) {
			int ratio = sdl_get_ratio();

			g_mouse_moved = 1;
			/* Assume 320x200 */
			g_mouse_posx = event.motion.x / ratio;
			g_mouse_posy = event.motion.y / ratio;


		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == 1) {
				g_mouse_leftclick_event = 1;
			}
			if (event.button.button == 3) {
				g_mouse_rightclick_event = 1;
			}

		} else if (event.type == SDL_KEYDOWN) {
			if (cmd == 1) {
				// check if a key was pressed
				int pressed = (event.type == SDL_KEYDOWN) ? 1 : 0;
				SDL_Delay(10);
				//fprintf(stdout, "%s(%d) = %d %d\n", __func__, cmd, pressed, key_cnt);
				SDL_PushEvent(&event);
				return pressed;
			} else {

				SDL_Keymod m = SDL_GetModState();

				if (m & KMOD_CTRL) {
					switch (event.key.keysym.sym) {
						case SDLK_q: {
							if (!g_in_intro) {
								g_sdl_quit_event = 1;
							}
							return (0x10 << 8) | KEY_DC1;
							break;
						}
						case SDLK_F3: return (KEY_CTRL_F3 << 8); break;
						case SDLK_F4: return (KEY_CTRL_F4 << 8); break;
					}
				}

				switch (event.key.keysym.sym) {
					case SDLK_TAB: {
								sdl_change_window_size(g_sdl_timer_mutex);
								sdl_mouse_cursor_scaled();
								break;
					}
					case SDLK_ESCAPE:   return (KEY_ESC << 8) | 0x1b; break; //OK
					case SDLK_1:        return (KEY_1 << 8) | 0x31; break; //OK
					case SDLK_2:        return (KEY_2 << 8) | 0x32; break; //OK
					case SDLK_3:        return (KEY_3 << 8) | 0x33; break; //OK
					case SDLK_4:        return (KEY_4 << 8) | 0x34; break; //OK
					case SDLK_5:        return (KEY_5 << 8) | 0x35; break; //OK
					case SDLK_6:   	    return (KEY_6 << 8) | 0x36; break; //OK
					case SDLK_KP_ENTER:
					case SDLK_RETURN:   return (KEY_RET << 8) | 0x0d; break; //OK
					case SDLK_j:        return (KEY_J << 8) | 0x6a; break; //OK
					case SDLK_y:        return (0x15 << 8) | 0x79; break; //DE
					case SDLK_z:        return (KEY_Y << 8) | 0x7a; break; //DE
					case SDLK_n:        return (KEY_N << 8) | 0x6e; break; //OK
					case SDLK_KP_8:
					case SDLK_UP:       return (KEY_UP << 8); break; //OK
					case SDLK_KP_4:
					case SDLK_LEFT:     return (KEY_LEFT << 8); break; //OK
					case SDLK_KP_6:
					case SDLK_RIGHT:    return (KEY_RIGHT << 8); break; //OK
					case SDLK_KP_2:
					case SDLK_DOWN:     return (KEY_DOWN << 8); break; //OK
					case SDLK_KP_9:
					case SDLK_PAGEUP:   return (KEY_PGUP << 8); break; //OK
					case SDLK_KP_3:
					case SDLK_PAGEDOWN: return (KEY_PGDOWN << 8); break; //OK
					case 0xe4:          return (0x28 << 8) | 0x84; break; //AE
					case 0xf6:          return (0x27 << 8) | 0x94; break; //OE
					case 0xfc:          return (0x1a << 8) | 0x81; break; //UE
					default:	    return event.key.keysym.sym & 0xff;
				}
			}
		}
	}

	return 0;
}
#endif

static void wait_for_vsync(void)
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
#else
	// wait 16ms
	SDL_Delay(16);
#endif
}

static signed short get_bioskey(const int cmd)
{
#if !defined(__BORLANDC__)
	if (cmd == 0) {
		// return the pressed key imediately
		int keycode = sdl_event_loop(0);
		return keycode;
	} if (cmd == 1) {
		// check if a key was pressed
		int pressed = sdl_event_loop(1);
		return pressed;
	} else return 0;
#else
	return bioskey(cmd);
#endif
}

static void flush_keyboard_queue(void)
{
#if defined(__BORLANDC__)
	while (get_bioskey(1)) {
		get_bioskey(0);
	}
#endif
}

/**
 * \brief high-level input function
 */
static void handle_input(void)
{
	int l_key_ext = 0;

	/* keyboard events */
	g_in_key_ascii = 0;
	g_in_key_ext = 0;

	if (get_bioskey(1)) {

		g_in_key_ascii = get_bioskey(0);
		l_key_ext = g_in_key_ascii >> 8;
		g_in_key_ascii &= 0xff;

		if (l_key_ext == KEY_J)
			l_key_ext = KEY_Y;

		/* exit Program with CRTL+Q */
		if ((g_in_key_ascii == KEY_DC1) && !g_in_intro) {
			return;
		}
	}

	/* mouse events */
	if (g_have_mouse == 2) {
		if (g_mouse_leftclick_event) {

			int i;

			l_key_ext = 0;

			if (g_action_table)
				l_key_ext = get_mouse_action(g_mouse_posx, g_mouse_posy, g_action_table);

			if ((l_key_ext == 0) && (g_default_action))
				l_key_ext = get_mouse_action(g_mouse_posx, g_mouse_posy, g_default_action);

			for (i = 0; i < 15; i++)
				wait_for_vsync();

			g_mouse_leftclick_event = 0;
		}

		mouse_motion();
	}

	g_in_key_ext = l_key_ext;
}

static void vsync_or_key(const int val)
{
	int i;

	for (i = 0; i < val; i++) {
		handle_input();
		if (g_in_key_ext || g_mouse_rightclick_event) {
			g_mouse_rightclick_event = 0;
			g_in_key_ext = KEY_RET;
			return;
		}
		wait_for_vsync();
	}
}


/* FILE MANAGEMENT */

static signed long get_filelength(void)
{
	return g_flen;
}

/**
 * \brief get the offset of a file inside DSAGEN.DAT
 * \param[in] name name of the file
 * \param[in] table offset table
 * \param[in] length length of DSAGEN.DAT
 * \return offset inside DSAGEN.DAT otherwise -1
 * \note sets g_flen
 */
static signed long get_archive_offset(const char *name, const unsigned char *table, const signed long length)
{
	signed long retval = -1;
	signed long flen;
	int i;
	int last = 50;
	int entry = -1;


	/* determine the last entry in the table */
	for (i = 0; i < 50; i++) {
		if (table[16 * i] == 0) {
			last = i - 1;
			break;
		}
	}

	/* determine the entry in the table */
	for (i = 0; i < 50; i++) {
		if (!strncmp(name, (const char*)table + i * 16, 12)) {
			entry = i;
			break;
		}
	}

	if (entry == -1) {
		/* not found */
		g_flen = -1;
		retval = -1;
	} else {
		const signed long o_cur = readd(table + 16 * entry + 0x0c);

		if (entry == last) {
			/* found, but last entry */
			flen = length - o_cur;
		} else {
			/* found */
			flen = readd(table + 16 * (i + 1) + 0x0c) - o_cur;
		}

		g_flen = flen;
		retval = o_cur;
	}

	return retval;
}

/**
 * \brief open the file DSAGEN.DAT
 * \param[in] index the index of the file inside DSAGEN.DAT
 * \return -1 on error otherwise a file handle
 */
static signed short open_datfile(const signed short index)
{
	unsigned char table[50 * 16];
	signed long gendat_offset;
	int handle;

#if defined(__BORLANDC__)
	flushall();
#endif

#if defined(__BORLANDC__) || defined(_WIN32)
	/* 0x8001 = O_BINARY | O_RDONLY */
	handle = open(g_str_dsagen_dat, O_BINARY | O_RDONLY);
#else
	handle = open(g_str_dsagen_dat, O_RDONLY);
#endif

	/* failed to open DSAGEN.DAT */
	if (handle != -1) {
		signed long gendat_length;

		/* determine filelength */
		gendat_length = lseek(handle, 0, SEEK_END);
		lseek(handle, 0, SEEK_SET);

		/* read offset table from file */
		_read(handle, table, 50 * 16);

		gendat_offset = get_archive_offset(g_fnames[index], table, gendat_length);

		/* update the global variable */
		g_gendat_offset = gendat_offset;

		if (gendat_offset != -1) {
			lseek(handle, gendat_offset, SEEK_SET);
			return handle;
		}
	}

	/* update the global variables on error */
	g_gendat_offset = -1;
	g_flen = -1;

	return -1;
}

/**
 * \brief reads an open file from DSAGEN.DAT into a buffer
 * \param[in] handle handle of DSAGEN.DAT
 * \param[out] buffer the buffer to write into
 * \param[in] len_in length of the file to read
 * \return number of bytes that were actually read
 * \note BCC/DOS length of a read is maximal 32kb
 **/
static signed long read_datfile(const signed short handle, unsigned char *buffer, const signed long len_in)
{
	const signed long chunk = 16 * 1024;
	signed long len = 0;

	if (len_in > chunk) {
		const int reads = len_in / chunk;
		const int remainder = len_in - reads * chunk;
		signed long i;

		for (i = 0; i < reads; i++) {
			len += _read(handle, buffer + i * chunk, chunk);
			if (len != (i + 1) * chunk) {
				fprintf(stderr, "ERROR: read_datfile() chunk = %ld len = %ld\n", chunk, len);
			}
		}

		if (remainder) {
			len += _read(handle, buffer + reads * chunk, remainder);
			if (len != reads * chunk + remainder) {
				fprintf(stderr, "ERROR: read_datfile() remainder = %d len = %ld\n", remainder, len);
			}
		}

	} else {
		/* small file */
		len = _read(handle, buffer, len_in);
	}

	if (len != len_in)
		fprintf(stderr, "ERROR: read_datfile() len_in = %ld len = %ld\n", len_in, len);

	return len;
}

/**
 * \brief detect and validate the file DSAGEN.DAT
 * \return -1 on error otherwise 0
 */
static int detect_datfile(void)
{
	signed long gendat_length;
	int handle;
	int i;
	int max_files;
	int retval = 0;
	char textbuffer[80];

#if defined(__BORLANDC__) || defined(_WIN32)
	/* 0x8001 = O_BINARY | O_RDONLY */
	handle = open(g_str_dsagen_dat, O_BINARY | O_RDONLY);
#else
	handle = open(g_str_dsagen_dat, O_RDONLY);
#endif
	if (handle == -1) {
		fprintf(stderr, "ERROR: DSAGEN.DAT not found\n");
		return -1;
	}

	/* determine filelength */
	gendat_length = lseek(handle, 0, SEEK_END);
	lseek(handle, 0, SEEK_SET);
	close(handle);

	if (gendat_length == -1) return -1;

	/* only these 3 versions are known so far */
	if (gendat_length == 671236) { /* EN DISK */ g_dsagen_lang = LANG_EN; g_dsagen_medium = MED_DISK; } else
	if (gendat_length == 663221) { /* DE CD   */ g_dsagen_lang = LANG_DE; g_dsagen_medium = MED_CD; } else
	if (gendat_length == 634785) { /* DE DISK */ g_dsagen_lang = LANG_DE; g_dsagen_medium = MED_DISK; }

	/* print info */
	fprintf(stderr, "DSAGEN.DAT: %s_%s",
			g_dsagen_lang == LANG_DE ? "DE" : "EN",
			g_dsagen_medium == MED_DISK ? "DISK" : "CD");

	/* set local pointer to the correct filename table */
	if (g_dsagen_lang == LANG_DE) {
		g_fnames = (g_dsagen_medium == MED_DISK) ? g_fnames_g100de : g_fnames_g105de;
	} else {
		if (g_dsagen_lang == LANG_EN) g_fnames = g_fnames_g300en;
	}

	/* validate DSAGEN.DAT */
	max_files = ((g_dsagen_lang == LANG_DE) && (g_dsagen_medium == MED_DISK)) ? 35 : 38;

	for (i = 0; (i < max_files) && (retval == 0); i++) {

		/* index == 13 (TYPPIC.DAT) is not included in any known DSAGEN.DAT */
		if ((i == 13) && (g_dsagen_lang == LANG_DE) && (g_dsagen_medium == MED_DISK)) continue;

		handle = open_datfile(i);
		//fprintf(stderr, "TRY: %02d %s %ld\n", i, g_fnames[i], get_filelength());
		if (handle == -1) {
			sprintf(textbuffer, g_str_file_missing, g_fnames[i]);
			fprintf(stderr, "ERROR: %s\n", textbuffer);
			retval = -1;
		} else {
			close(handle);
		}
	}

	fprintf(stderr, " is %s\n", retval == -1 ? "INVALID" : "VALID");

	return retval;
}

static void split_textbuffer(char **dst, char *src, const unsigned long len)
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

/**
 * \brief load and prepares essential GUI files
 * \return 0 on success otherwise 1
 */
static int load_essential_files(void)
{
	signed short handle;
	signed long len;
	signed short count = 0;

	/* load FONT6 */
	handle = open_datfile(14);
	if (handle != -1) {
		read_datfile(handle, g_buffer_font6, get_filelength());
		close(handle);
		count++;
	}

	/* load GENTEXT */
	handle = open_datfile(15);
	if (handle != -1) {
		len = read_datfile(handle, (unsigned char*)g_buffer_text, get_filelength());
		close(handle);

		split_textbuffer(g_texts, g_buffer_text, len);
		count++;
	}

	/* load POPUP.DAT */
	handle = open_datfile(19);
	if (handle != -1) {
		len = read_datfile(handle, g_buffer_popup_dat - 8, get_filelength());
		close(handle);

		decomp_pp20(g_buffer_popup_dat, g_buffer_popup_dat - 8, len);

		count++;
	}

	/* copy arrow area from GEN4.NVF/E_GEN4.NVF */
	handle = open_datfile(4);
	if (handle != -1) {
		read_datfile(handle, g_page_buffer, get_filelength());
		close(handle);

		decomp_rle(g_vga_backbuffer, g_page_buffer);

		vgalib_copy_from_screen(g_arrow_area, g_vga_backbuffer + 178 * O_WIDTH + 145, 170, 20);

		memset(g_vga_backbuffer, 0, 64100);
		memset(g_page_buffer, 0, 50000);

		count++;
	}

	/* copy mr_bar and name_bar from GEN1.NVF/E_GEN1.NVF */
	handle = open_datfile(0);
	if (handle != -1) {
		read_datfile(handle, g_page_buffer, get_filelength());
		close(handle);

		decomp_rle(g_vga_backbuffer, g_page_buffer);

		vgalib_copy_from_screen(g_mr_bar, g_vga_backbuffer + 182 * O_WIDTH + 145, 77, 9);

		vgalib_copy_from_screen(g_name_bar, g_vga_backbuffer + 12 * O_WIDTH + 180, 15 * 6, 8);

		memset(g_vga_backbuffer, 0, 64100);
		memset(g_page_buffer, 0, 50000);

		count++;
	}

	return (count == 5) ? 0 : 1;
}

static void load_common_files(void)
{
	signed short handle;
	signed short len;

	/* load HEADS.DAT */
	handle = open_datfile(11);
	if (handle != -1) {
		read_datfile(handle, g_buffer_heads_dat, get_filelength());
		close(handle);
	}

	/* load SEX.DAT */
	handle = open_datfile(12);
	if (handle != -1) {
		read_datfile(handle, g_buffer_sex_dat, get_filelength());
		close(handle);
	}

	/* load DMENGE.DAT */
	handle = open_datfile(32);
	if (handle != -1) {
		len = read_datfile(handle, g_buffer_dmenge_dat - 8, get_filelength());
		close(handle);

		decomp_pp20(g_buffer_dmenge_dat, g_buffer_dmenge_dat - 8, len);
	}
}

/**
 * \brief load a requested page to g_vga_backbuffer
 * \note newer systems: copy decompressed data from buffer
 * \note older systems: load it from DSAGEN.DAT
 */
static void load_page(const int page)
{
#if !defined(__BORLANDC__)
	if ((0 <= page) && (page <= 10)) {

		memcpy(g_vga_backbuffer, g_bg_buffer[page], O_WIDTH * O_HEIGHT);
	}
#else
	int handle;

	if ((0 <= page) && (page <= 10)) {

		handle = open_datfile(page);

		if (handle != -1) {

			read_datfile(handle, g_page_buffer, get_filelength());
			decomp_rle(g_vga_backbuffer, g_page_buffer);

			close(handle);
			memset(g_page_buffer, 0x00, 50000);
		}
	}
#endif
}

#if !defined(__BORLANDC__)
/**
 * \brief buffer and prepare all background images
 * \note only on newer systems with enough memory
 */
static void load_pages(void)
{
	for (int i = 0; i <= 10; i++) {

		int handle = open_datfile(i);

		if (handle != -1) {

			read_datfile(handle, g_page_buffer, get_filelength());
			decomp_rle(g_bg_buffer[i], g_page_buffer);
			close(handle);
		}
	}

	memset(g_page_buffer, 0x00, 50000);

	/* prepare page 0 of the german version */
	if (g_dsagen_lang == LANG_DE) {

		/* copy arrow_area on the image */
		vgalib_copy_to_screen(g_bg_buffer[0] + 178 * O_WIDTH + 145, g_arrow_area, 170, 20);
		/* copy mr_bar onto the image */
		vgalib_copy_to_screen(g_bg_buffer[0] + 182 * O_WIDTH + 166, g_mr_bar, 77, 9);

	}
}
#endif

/**
 * \brief load archetype image into g_buffer_typus
 * \param[in] typus number of typus
 */
static void load_typus(const int typus)
{
	const int index = typus + 19;

	/* check if the compressed image is already buffered */
	if (g_typus_buffer[typus]) {
		decomp_pp20(g_buffer_typus, g_typus_buffer[typus], g_typus_len[typus]);
	} else {

		const int handle = open_datfile(index);
		const signed long length = get_filelength();
		unsigned char *ptr = gen_alloc(length);


		if (ptr != NULL) {
			/* load the file into the typus buffer */
			g_typus_buffer[typus] = ptr;
			g_typus_len[typus] = length;

			read_datfile(handle, g_typus_buffer[typus], length);

			decomp_pp20(g_buffer_typus, g_typus_buffer[typus], length);
		} else {
			/* load the file direct */
			read_datfile(handle, g_page_buffer, length);
			decomp_pp20(g_buffer_typus, g_page_buffer, length);
		}
		close(handle);
		memset(g_page_buffer, 0x00, 50000);
	}
}

/* COLOR MANAGEMENT */

/**
 * \brief set fore- and background color of texts
 * \param[in] fg_color foreground color index
 * \param[in] bg_color background color index
 */
static void set_textcolor(const int fg_color, const int bg_color)
{
	g_fg_color[0] = fg_color;
	g_bg_color = bg_color;
}

/**
 * \brief get fore- and background color of texts
 * \param[out] p_fg foreground color index
 * \param[out] p_bg background color index
 */
static void get_textcolor(int *p_fg, int *p_bg)
{
	*p_fg = g_fg_color[0];
	*p_bg = g_bg_color;
}

/**
 * \brief initialize predefined colors
 */
static void init_colors(void)
{
	set_palette(g_pal_col_black, 0x00, 1);
	set_palette(g_pal_col_white, 0xff, 1);
	set_palette((const unsigned char*)g_pal_heads, 0x20, 0x20);
	set_palette((const unsigned char*)g_pal_genbg, 0x40, 0x20);
	set_palette((const unsigned char*)g_pal_misc, 0xc8, 3);
	set_palette((const unsigned char*)g_pal_popup, 0xd8, 8);

	set_textcolor(0xff, 0x0); // WHITE ON BLACK
}

/**
 * \brief initialize video mode/create window
 */
static void init_video(void)
{
#if defined(__BORLANDC__)
	save_display_stat(&g_display_page_bak);

	/* set the video mode to 320x200 8bit */
	set_video_mode(0x13);
#else
	sdl_init_video();
#endif

	init_colors();
}

/**
 * \brief uninitialize video mode/close window
 */
void exit_video(void)
{
#if defined(__BORLANDC__)
	/* restore old mode */
	set_video_mode(g_display_mode_bak);
	/* restore old page */
	set_video_page(g_display_page_bak);
#else
	sdl_exit_video();
#endif
}

void call_fill_rect_gen(unsigned char *ptr, const signed short x1, const signed short y1, const signed short x2, const signed short y2, const signed short color)
{
	const int width = x2 - x1 + 1;
	const int height = y2 - y1 + 1;

	ptr += y1 * O_WIDTH + x1;

	vgalib_fill_rect(ptr, color, width, height);
}


/* CHARACTER AND FONT FUNCTIONS */

/**
 * \brief calculate the width of a popup window
 * \param[in] popup_tiles number of tiles (16 + 32 * popup_tiles + 16)
 * \return width value in pixel
 */
static int popup_width(const int popup_tiles)
{
	return 32 * (popup_tiles + 1);
}

/**
 * \brief calculate the height of a popup window
 * \param[in] lines_header number of header lines (without radio button)
 * \param[in] lines_body number of body lines (with radio button)
 * \return height value in pixel
 */
static int popup_height(const int lines_header, const int lines_body)
{
	return 8 * (1 + lines_header + lines_body + 1);
}

/**
 * \brief gets font information of a character
 * \param[in] c the character
 * \param[out] width pointer to save the width of the character
 * \return font index
 */
static signed short get_chr_info(const unsigned char c, signed short *width)
{
	int i;

	/* lookup printable characters */
	for (i = 0; i != 222; i += 3) {
		/* search for the printable character */
		if (*(&g_chr_lookup[0].chr + i) == c) {

			*width = *(&g_chr_lookup[0].width + i) & 0xff;
			return *(&g_chr_lookup[0].index + i) & 0xff;
		}
	}

	if (c == 0x7e || c == 0xf0 || c == 0xf1 || c == 0xf2 || c == 0xf3) {
		*width = 0;
	} else {
		*width = 6;
	}

	return 0;
}

/**
 * \brief draws the font symbol into a buffer
 * \param[out] char_buffer the destination
 * \param[in] font_ptr the source
 * \param[in] color the color value of the font symbol
 */
static void prepare_chr_foreground(unsigned char *char_buffer, const unsigned char* font_ptr, const unsigned char color)
{
	unsigned char *ptr = char_buffer;
	int i;
	int j;
	unsigned char mask;

	for (i = 0; i < 8; ptr += 8, i++) {
		mask = *font_ptr++;
		for (j = 0; j < 8; j++) {
			if ((0x80 >> j) & mask) {
				ptr[j] = color;
			}
		}
	}
}

/**
 * \brief copy a character to the screen
 * \param[out] gfx_ptr destination address with popup measures
 * \param[in] char_buffer the source
 * \param[in] chr_height height of the character
 * \param[in] chr_width width of the character
 * \param[in] popup_width width of the popup box in pixels
 * \note requires screen measures
 */
static void blit_chr_popup(unsigned char *gfx_ptr, unsigned char *char_buffer, const signed short chr_height, const signed short chr_width, const int popup_width)
{
	unsigned char *src = char_buffer;
	int i;

	for (i = 0; i < chr_height; i++) {
		memcpy(gfx_ptr, src, chr_width);
		src += 8;
		gfx_ptr += popup_width;
	}
}

/**
 * \brief calculate the address in a popup box
 * \param[in] x X-coordinate
 * \param[in] y Y-coordinate
 * \param[in] popup_width width of the popup window
 * \return pointer to the address
 */
static unsigned char* get_popup_ptr(const signed short x, const signed short y, const int popup_width)
{
	return g_popup_box + popup_width * y + x;
}

/**
 * \brief calculate the address on the screen
 * \param[in] x X-coordinate
 * \param[in] y Y-coordinate
 * \return pointer to the address
 */
static unsigned char* get_gfx_ptr(const signed short x, const signed short y)
{
	return g_gfx_ptr + (y * O_WIDTH + x);
}

/**
 * \brief copy a character to the screen
 * \param[out] gfx_ptr destination address with screen measures
 * \param[in] char_buffer the source
 * \param[in] chr_height height of the character
 * \param[in] chr_width width of the character
 * \note requires screen measures
 */
static void blit_chr(unsigned char *gfx_ptr, unsigned char *char_buffer, const signed short chr_height, const signed short chr_width)
{
	unsigned char *src = char_buffer;
	int i;

	for (i = 0; i < chr_height; i++) {
		memcpy(gfx_ptr, src, chr_width);
		src += 8;
		gfx_ptr += O_WIDTH;
	}
}

static void print_chr_to_screen(const int chr_index, const int chr_width, const int x, const int y)
{
	unsigned char char_buffer[8 * 8];

	if (g_use_solid_bg) {
		memset(char_buffer, g_bg_color, 8 * 8);
	} else {
		vgalib_copy_from_screen(char_buffer, g_vga_backbuffer + y * O_WIDTH + x, 8, 8);
	}

	prepare_chr_foreground(char_buffer, g_buffer_font6 + 8 * chr_index, g_fg_color[g_col_index]);

	if (g_gfx_ptr == g_popup_box) {
		const int width = popup_width(g_menu_tiles);
		blit_chr_popup(get_popup_ptr(x, y, width), char_buffer, 7, chr_width, width);
	} else {
		blit_chr(get_gfx_ptr(x, y), char_buffer, 7, chr_width);
	}
}

static signed short print_chr(const unsigned char c, const int x, const int y)
{
	signed short width;
	signed short idx;

	idx = get_chr_info(c, &width);

	print_chr_to_screen(idx, width, x, y);

	return width;
}

/* STRING FUNCTIONS */

/**
 * \brief sets the line breaks for a string
 * \param str string
 * \return the number of lines the string needs.
 */
static signed short str_splitter(char *str)
{
	char *tp;
	signed short unknown_var1;
	signed short lines;
	signed short c_width;
	signed short l_width;

	signed short last_space;
	signed short i;

	lines = 1;

	if (!str) {
		return 0;
	}

	/* replace all CR and LF with spaces */
	for (tp = (char*)str; *tp; tp++) {
		if ((*tp == 0x0d) || (*tp == 0x0a))
		{
			*tp = ' '; //0x20;
		}
	}

	tp = (char*)str;

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

/**
 * \brief calculates the start positon for a centered line
 * \param[in] str the string
 * \param[in] x start position of the string
 * \param[in] x_max end position of the string
 * \return X coordinate where the string starts
 */
static signed short get_line_start_c(const char *str, const signed short x, const signed short x_max)
{
	signed short width;
	signed short pos_x;
	signed short val;
	
	for (pos_x = 0; ((val = *str) && (val != 0x40) && (val != 0x0d)); )
	{
		get_chr_info(*str++, &width);
		pos_x += width;
	}

	return (x_max - pos_x) / 2 + x;
}

static void print_str(const char *str, const signed short x_in, const signed short y_in)
{
	signed short i;
	signed short x_bak;
	signed short x = x_in;
	signed short y = y_in;
	signed short x_max = x;
	unsigned char c;

	i = 0;

	mouse_bg();

	if (g_in_infobox)
		x = get_line_start_c(str, x, g_text_x_end);

	x_bak = x;

	while (str[i]) {

		c = str[i++];

		if (x > x_max) x_max = x;

		if ((c == 0x0d) || (c == '@')) {

			/* newline */
			y += 7;

			x = (g_in_infobox) ?
				get_line_start_c(str + i, 5, g_text_x_end) :
				x_bak;

		} else if ((c == 0xf0) || (c == 0xf1) || (c == 0xf2) || (c == 0xf3)) {

			/* change text color */
			g_col_index = c - 0xf0;

		} else {
			/* print normal */
			x += print_chr(c, x, y);
		}
	}

#if !defined(__BORLANDC__)
	if (g_gfx_ptr == g_vga_memstart)
		sdl_forced_update();
#endif

	mouse_cursor();
}

/**
 * \brief print a string with multiple lines as the header of a popup box
 * \param[in] str
 */
static void print_header(char *str)
{
	str_splitter(str);

	print_str(str, 5, 7);
}

/**
 * \brief get the width of a string in terms of character font size
 * \param[in] str
 */
static int get_str_width(const char *str)
{
	int sum = 0;
	signed short width;

	while (*str) {
		get_chr_info(*str++, &width);
		sum += width;
	}

	return sum;
}

/**
 * \brief read a string from the keyboard
 * \param[in] dst memory location to store the string
 * \param[in] x screen X Coordinate of the input field
 * \param[in] y screen Y Coordinate of the input field
 * \param[in] len maximal number of allowed characters
 * \param[in] txt iff 1 enter text otherwise a number
 */
static signed short enter_string(char *dst, const signed short x, const signed short y, const signed short len, const signed short txt)
{
	unsigned char * const gfx_bak = g_gfx_ptr;
	signed short x_pos = x;	// position on the screen
	signed short s_pos = 0;	// position in the string
	signed short c;
	signed short width;
	signed short i;

	g_gfx_ptr = g_vga_memstart;

	mouse_bg();

	if (txt == 0) {
		for (i = 0; i < len; i++) {
			print_chr(' ', x_pos, y);
			print_chr('_', x_pos, y);
			x_pos += 6;
		}
		x_pos = x;
	} else {
		print_chr(' ', x_pos, y);
		print_chr('_', x_pos, y);
	}

#if !defined(__BORLANDC__)
	if (g_gfx_ptr == g_vga_memstart)
		sdl_forced_update();
#endif

	/* clear all input events */
	flush_keyboard_queue();
	g_mouse_leftclick_event = 0;

	c = 0;
	while ((c != 0xd) || (s_pos == 0)) {
		do {
			/* Poll an input event */
			do {
			} while (!get_bioskey(1) && (g_mouse_leftclick_event == 0));

			if (g_mouse_leftclick_event) {
				/* transform mouse leftclick to a keyboard return */
				g_in_key_ascii = 0x0d;
				g_in_key_ext = 0x1c;
				g_mouse_leftclick_event = 0;
			} else {
				/* keyboard event */
				g_in_key_ascii = get_bioskey(0);
				g_in_key_ext = g_in_key_ascii >> 8;
				g_in_key_ascii &= 0xff;
			}
		} while ((g_in_key_ext == 0) && (g_in_key_ascii == 0));

		c = g_in_key_ascii;

		if (c == 0xd)
			continue;

		if (g_in_key_ext == KEY_ESC) {
			*dst = 0;
			mouse_cursor();
			g_in_key_ext = 0;
			g_gfx_ptr = gfx_bak;

			return 1;
		}

		if (c == 8) {
			/* Backspace: remove the last symbol */
			if (s_pos > 0) {

				if (txt == 1 && s_pos != len)
					print_chr(' ', x_pos, y);
				s_pos--;
				dst--;
				get_chr_info(*dst, &width);

				x_pos -= (txt != 0) ? width : 6;

				print_chr(' ', x_pos, y);
				print_chr('_', x_pos, y);
			}
		} else {
			/* check allowed input symbols */
			if ((isalnum(c) == 0) &&
				(((unsigned char)c) != 0x84) && (((unsigned char)c) != 0x94) &&
				(((unsigned char)c) != 0x81) && (((unsigned char)c) != 0x8e) &&
				(((unsigned char)c) != 0x99) && (((unsigned char)c) != 0x9a) &&
				(c != ' ') && (c != '.'))
					continue;

			/* transform input char to uppercase */
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
			if (s_pos == len) {
				dst--;

				get_chr_info(*dst, &width);

				x_pos -= (txt != 0) ? width : 6;

				s_pos--;
			}

			*dst++ = (unsigned char)c;
			print_chr(' ', x_pos, y);
			print_chr((unsigned char)c, x_pos, y);
			get_chr_info((unsigned char)c, &width);

			x_pos += (txt != 0) ? width : 6;

			s_pos++;

			if ((txt == 1) && (s_pos != len)) {
				print_chr(' ', x_pos, y);
				print_chr('_', x_pos, y);
			}
		}

#if !defined(__BORLANDC__)
	if (g_gfx_ptr == g_vga_memstart)
		sdl_forced_update();
#endif
	}

	if (txt == 0) {
		while (s_pos < len) {
			print_chr(' ', x_pos, y);
			x_pos += 6;
			s_pos++;
		}
	}

	*dst = 0;
	mouse_cursor();
	g_gfx_ptr = gfx_bak;

	return 0;
}

/**
 * \brief draws a popup line
 * \param[in] line line number
 * \param[in] type line type
 */
static void draw_popup_line(const signed short line, const signed short type)
{
	const int tiles = g_menu_tiles;
	const int width = popup_width(tiles);
	unsigned char *dst = g_popup_box + 8 * width * line;

	/* tile offsets in POPUP.DAT */
	int popup_left;
	int popup_middle;
	int popup_right;
	int i;
	int j;

	if (type == 0) {
		/* upper edge */
		popup_left = 0; popup_middle = 7; popup_right = 1;
	} else if (type == 1) {
		/* middle part (without radio box) */
		popup_left = 2; popup_middle = 9; popup_right = 3;
	} else if (type == 2) {
		/* middle part (with radio box) */
		popup_left = 4; popup_middle = 9; popup_right = 3;
	} else if (type == 3) {
		/* lower edge */
		popup_left = 5; popup_middle = 11; popup_right = 6;
	} else {
		/* middle part (without radio box) as fallback */
		popup_left = 2; popup_middle = 9; popup_right = 3;
	}

	popup_left *= 128; popup_middle *= 128; popup_right *= 128;

	for (i = 0; i < 8; i++) {
		memcpy(dst, g_buffer_popup_dat + popup_left + 16 * i, 16);
		dst += 16;
		for (j = 0; j < tiles; j++) {
			memcpy(dst, g_buffer_popup_dat + popup_middle + 32 * i, 32);
			dst += 32;
		}
		memcpy(dst, g_buffer_popup_dat + popup_right + 16 * i, 16);
		dst += 16;
	}
}

/**
 * \brief draws a complete empty popup box in memory
 * \param[in] lines_header number of lines for the header
 * \param[in] lines_body number of lines for the body
 **/
static void draw_popup_box(const int lines_header, const int lines_body)
{
	int i;

	memset(g_popup_box, 0x00, O_WIDTH / 2 * O_HEIGHT);

	/* top border */
	draw_popup_line(0, 0);

	/* header lines */
	for (i = 0; i < lines_header; i++)
		draw_popup_line(1 + i, 1);

	/* body lines */
	for (i = 0; i < lines_body; i++)
		draw_popup_line(1 + lines_header + i, 2);

	/* bottom border */
	draw_popup_line(1 + lines_header + lines_body, 3);
}

/**
 * \brief draws and info- or enter_numberbox
 * \param[in] header the message for the box
 * \param[in] digits number of digits to enter
 *
 * \note if digits is zero the function just delays.
 */
static int infobox(char *header, const signed short digits)
{
	unsigned char* vga_ptr;
	unsigned char* gfx_bak = g_gfx_ptr;
	int fg_bak;
	int bg_bak;
	int retval;
	int lines;
	int height;
	int upper_border;
	char digitbuffer[32];

	const int width = popup_width(g_menu_tiles);
	const int left_border = (O_WIDTH - width) / 2 + g_text_x_mod;
	g_text_x_end = width - 10;
	lines = str_splitter(header);

	if (digits != 0)
		lines += 2;

	height = popup_height(lines, 0);
	upper_border = (O_HEIGHT - height) / 2;

	g_in_infobox = 1;

	/* save and set text colors */
	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0xdf); // WHITE ON GREEN

	mouse_bg();

	/* save the current background in g_page_buffer*/
	vga_ptr = g_vga_memstart + upper_border * O_WIDTH + left_border;
	vgalib_copy_from_screen(g_page_buffer, vga_ptr, width, height);

	/* draw popup */
	draw_popup_box(lines, 0);

	g_use_solid_bg = 1;
	g_gfx_ptr = g_popup_box;
	print_header(header);
	g_gfx_ptr = gfx_bak;

	vgalib_copy_to_screen(vga_ptr, g_popup_box, width, height);

	g_mouse_rightclick_event = 0;
	mouse_cursor();

	if (digits) {
		enter_string(digitbuffer,
			left_border + (width - digits * 6) / 2,
			upper_border + 8 * lines - 2, digits, 0);

		retval = (unsigned short)atol(digitbuffer);
	} else {
		g_action_table = g_action_input;
		vsync_or_key(150 * lines);
		g_action_table = NULL;

		retval = 0;
	}

	g_use_solid_bg = 0;

	set_textcolor(fg_bak, bg_bak);
	mouse_bg();

	/* restore background from g_page_buffer */
	vgalib_copy_to_screen(vga_ptr, g_page_buffer, width, height);

	mouse_cursor();

	g_text_x_end = 0;

	g_in_infobox = 0;
	g_in_key_ext = 0;

	return retval;
}


/**
 * \brief marks the active radio button
 * \param[in] old_pos the position of the last active button (or -1)
 * \param[in] new_pos the position of the current active button
 * \param[in] offset the offset of the first radio line
 * \param[in] left_border x - coordinate of the left border of the gui box
 * \param[in] upper_border y - coordinate of the upper border of the gui box
 */
static void fill_radio_button(const signed short old_pos, const signed short new_pos, const signed short offset, const int left_border, const int upper_border)
{
	signed short x;
	signed short y;

	mouse_bg();

	/* unmark the old radio button, if any */
	if (old_pos != -1) {
		x = left_border + 6;

		y = upper_border + (offset + old_pos) * 8 + 2;

		call_fill_rect_gen(g_vga_memstart, x, y, x + 3, y + 3, 0xd8);
	}

	/* mark the new radio button */
	x = left_border + 6;

	y = upper_border + (offset + new_pos) * 8 + 2;

	call_fill_rect_gen(g_vga_memstart, x, y, x + 3, y + 3, 0xd9);

	mouse_cursor();
}

/**
 * \brief displays a radio menu
 * \param[in] header the header of the menu
 * \param[in] options the number of options
 */
signed short gui_radio(char *header, const signed int options, ...)
{
	va_list arguments;
	unsigned char* vga_ptr;
	unsigned char* gfx_bak = g_gfx_ptr;
	char *str;
	int fg_bak;
	int bg_bak;
	int i;
	signed short str_y;
	signed short done = 0;
	signed short retval;
	signed short lines_header;

	signed short l_opt_bak = -1;
	signed short l_opt_new = 1;

	signed short mx_bak;
	signed short my_bak;
	signed short r7;
	signed short r8;
	int height;
	int upper_border;


	const int width = popup_width(g_menu_tiles);
	const int left_border = (O_WIDTH - width) / 2 + g_text_x_mod;

	g_text_x_end = width - 10;
	lines_header = str_splitter(header);
	height = popup_height(lines_header, options);
	upper_border = (O_HEIGHT - height) / 2;

	/* save and set text colors */
	get_textcolor(&fg_bak, &bg_bak);
	set_textcolor(0xff, 0xdf); // WHITE ON GREEN

	mouse_bg();

	/* save the current background in g_page_buffer */
	vga_ptr = g_vga_memstart + upper_border * O_WIDTH + left_border;
	vgalib_copy_from_screen(g_page_buffer, vga_ptr, width, height);

	/* draw popup */
	draw_popup_box(lines_header, options);
	g_use_solid_bg = 1;
	g_gfx_ptr = g_popup_box;

	/* print header */
	if (lines_header) {
		print_header(header);
	}

	str_y = 8 * (lines_header + 1);

	/* print radio options */
	va_start(arguments, options);
	for (i = 1; i <= options; str_y += 8, i++) {
		str = va_arg(arguments, char*);
		print_str(str, 8 + 5, str_y);
	}
	va_end(arguments);

	g_gfx_ptr = gfx_bak;
	g_use_solid_bg = 0;
	vgalib_copy_to_screen(vga_ptr, g_popup_box, width, height);

	/* save and set mouse position */
	mx_bak = g_mouse_posx;
	my_bak = g_mouse_posy;
	g_mouse_posx_bak = g_mouse_posx = left_border + 90;
	g_mouse_posy_bak = g_mouse_posy = r8 = r7 = upper_border + 8 * (lines_header + 1);

	mouse_move_cursor(g_mouse_posx, g_mouse_posy);

	g_mouse_posx_max = left_border + width - 16;
	g_mouse_posx_min = left_border;
	g_mouse_posy_min = upper_border + 8 * (lines_header + 1);
	g_mouse_posy_max = (upper_border + 8 * (lines_header + 1) + 8 * options) - 1;
	mouse_cursor();
	g_mouse_rightclick_event = 0;

	while (!done) {
		g_action_table = g_action_input;
		handle_input();

#if !defined(__BORLANDC__)
		/* check if SDL got a quit event set by handle_input() */
		if (g_sdl_quit_event) {
			/* mimic an ESC to return from gui_radio() */
			retval = -1;
			done = 1;
			g_mouse_rightclick_event = 0;
		}
#endif
		g_action_table = NULL;

		if (l_opt_bak != l_opt_new) {
#if !defined(__BORLANDC__)
			if (l_opt_new < 1) {
				/* select the first option */
				l_opt_new = 1;
			} else if (l_opt_new > options) {
				/* select the last option */
				l_opt_new = options;
			}
#endif
			/* default behaviour */
			fill_radio_button(l_opt_bak, l_opt_new, lines_header, left_border, upper_border);
			l_opt_bak = l_opt_new;
		}

#if !defined(__BORLANDC__)
		SDL_Delay(25);
#endif

		if ((g_mouse_rightclick_event != 0) || (g_in_key_ext == KEY_ESC) || (g_in_key_ext == KEY_PGDOWN)) {
			/* has the selection been canceled */
			retval = -1;
			done = 1;
			g_mouse_rightclick_event = 0;
		}
		if (g_in_key_ext == KEY_RET) {
			/* has the return key been pressed */
			retval = l_opt_new;
			done = 1;
		}
		if (g_in_key_ext == KEY_UP) {
			/* has the up key been pressed */
			if (l_opt_new == 1)
				l_opt_new = options;
			else
				l_opt_new--;
		}
		if (g_in_key_ext == KEY_DOWN) {
			/* has the down key been pressed */
			if (l_opt_new == options)
				l_opt_new = 1;
			else
				l_opt_new++;
		}
		if (g_mouse_posy != r8) {
			/* has the mouse been moved */
			r8 = g_mouse_posy;
			l_opt_new = (r8 - r7) / 8 + 1;
		}
		/* is this a bool radiobox ? */
		if (g_bool_mode) {
			if (g_in_key_ext == KEY_Y) {
				/* has the 'j' key been pressed */
				retval = 1;
				done = 1;
			} else if (g_in_key_ext == KEY_N) {
				/* has the 'n' key been pressed */
				retval = 2;
				done = 1;
			}
		}
	}

	mouse_bg();

	g_mouse_posx_bak = g_mouse_posx = mx_bak;
	g_mouse_posy_bak = g_mouse_posy = my_bak;

	g_mouse_posx_max = O_WIDTH - 1;
	g_mouse_posx_min = 0;
	g_mouse_posy_min = 0;
	g_mouse_posy_max = O_HEIGHT - 1;

	mouse_move_cursor(g_mouse_posx, g_mouse_posy);

	/* restore the previous background from g_page_buffer */
	vgalib_copy_to_screen(vga_ptr, g_page_buffer, width, height);

	mouse_cursor();

	set_textcolor(fg_bak, bg_bak);

	g_text_x_end = 0;
	g_in_key_ext = 0;

	return retval;
}

/**
 * \brief displays a yes - no radio box
 * \param[in] header header of the menu
 * \return 1 = yes otherwise 0
 */
static signed short gui_bool(char *header)
{
	signed short retval;

	g_bool_mode = 1;
	retval = gui_radio(header, 2, get_text(4), get_text(5));
	g_bool_mode = 0;

	return (retval == 1) ? 1 : 0;
}

/* AIL MANAGEMENT */
#if defined(__BORLANDC__)
static unsigned char *load_snd_driver(const char *fname)
{
	signed long size;
	unsigned char *norm_ptr;
	unsigned long in_ptr;
	signed short handle;

	/* 0x8001 = O_BINARY | O_RDONLY */
	handle = open(fname, O_BINARY | O_RDONLY);
	if (handle != -1) {
		size = 16500;
		g_snd_driver = (unsigned char*)gen_alloc(size + 0x10);
		// BCC: far pointer normalizaion (DOS only)
		norm_ptr = (FP_OFF(g_snd_driver) != 0 ?
				MK_FP(FP_SEG(g_snd_driver) + 1, 0) :
				MK_FP(FP_SEG(g_snd_driver), 0));
		_read(handle, norm_ptr, size);
		_close(handle);
		return norm_ptr;
	} else {
		return (unsigned char*)NULL;
	}
}

static signed short load_driver(const char* fname, const signed short type, signed short port)
{
	if ((port != 0) &&
		(g_snd_driver_base_addr = load_snd_driver(fname)) &&
		((g_snd_driver_handle = AIL_register_driver(g_snd_driver_base_addr)) != -1))
	{
		g_snd_driver_desc = (unsigned char*)AIL_describe_driver(g_snd_driver_handle);

		if (readws(g_snd_driver_desc + 0x02) == type)
		{
			if (port == -1) {
				port = readws(g_snd_driver_desc + 0x0c);
			}
			if (AIL_detect_device(g_snd_driver_handle, port,
					readw(g_snd_driver_desc + 0x0e),
					readw(g_snd_driver_desc + 0x10),
					readw(g_snd_driver_desc + 0x12)) != 0)
			{
				AIL_init_driver(g_snd_driver_handle, port,
					readw(g_snd_driver_desc + 0x0e),
					readw(g_snd_driver_desc + 0x10),
					readw(g_snd_driver_desc + 0x12));
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

				return 1;
			} else {
				infobox(g_str_soundhw_not_found, 0);
			}
		}
	}

	return 0;
}

static signed short *get_timbre(const signed short bank, const signed short patch)
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

	read_datfile(g_handle_timbre, (unsigned char*)&timbre_ptr[1], (unsigned short)(timbre_ptr[0] = g_current_timbre_length) - 2);

	return timbre_ptr;
}

static signed short load_seq(const signed short sequence_num)
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
					gen_free(src_ptr);
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

static signed short play_sequence(const signed short sequence_num)
{
	if (load_seq(sequence_num) != 0) {
		AIL_start_sequence(g_snd_driver_handle, sequence_num);
		return 1;
	}

	return 0;
}

static signed short load_sequence(const signed short index)
{
	signed short handle;

	if ((handle = open_datfile(index)) != -1) {
		read_datfile(handle, g_form_xmid, get_filelength());
		close(handle);
		return 1;
	}

	return 0;
}

static void stop_sequence(void)
{
	if ((g_music == MUSIC_XMID) && (readw(g_snd_driver_desc + 0x02) == 3))
	{
		AIL_stop_sequence(g_snd_driver_handle, g_snd_sequence);
		AIL_release_sequence_handle(g_snd_driver_handle, g_snd_sequence);
	}
}
#endif

static void restart_midi(void)
{
#if defined(__BORLANDC__)
	if ((g_music == MUSIC_XMID) && (readw(g_snd_driver_desc + 0x02) == 3) &&
		(AIL_sequence_status(g_snd_driver_handle, g_snd_sequence) == 2))
	{
		AIL_start_sequence(g_snd_driver_handle, g_snd_sequence);
	}
#endif
}

#if defined(__BORLANDC__)
static void play_midi(const signed short index)
{
	if ((g_music == MUSIC_XMID) && (readw(g_snd_driver_desc + 0x02) == 3))
	{
		stop_sequence();
		load_sequence(index);
		play_sequence(0);
	}
}
#endif

static void start_music(const signed short track)
{
#if defined(__BORLANDC__)
	if (g_music == MUSIC_XMID) {
		play_midi(track);
	} else if (g_music == MUSIC_CDA) {
		CD_play_track(4);
	}
#else
	if (g_music == MUSIC_CDA) {
		if (track == 33) {
#if defined(_WIN32)
			music = Mix_LoadMUS("..\\..\\music\\1992-Die Schicksalsklinge\\04 A New Life is Born.flac");
#else
			music = Mix_LoadMUS("../../music/1992-Die Schicksalsklinge/04 A New Life is Born.flac");
#endif
			if (music == NULL) {
				fprintf(stderr, "ERROR: failed to load music file: %s\n", SDL_GetError());
				g_music = MUSIC_OFF;
				return;
			}

			if (Mix_PlayMusic(music, -1) == -1) {
				fprintf(stderr, "ERROR: music cannot be played: %s\n", SDL_GetError());
			}
		}
	}
#endif
}

static void init_music(void)
{
#if defined(__BORLANDC__)

	if (g_music != MUSIC_OFF) {

		signed short handle;
		signed short port;

		/* try to enable MIDI instead of CD-Audio */
		/* 0x8001 = O_BINARY | O_RDONLY */
		handle = open(g_str_sound_cfg, O_BINARY | O_RDONLY);
		if (handle != -1) {
			_read(handle, (unsigned char*)&port, 2);
			_close(handle);

			g_form_xmid = gen_alloc(12500);

			if (g_form_xmid != NULL) {

				AIL_startup();

				if ((port) && (load_driver(g_str_sound_adv, 3, port))) {
					g_music = MUSIC_XMID;
					return;
				}

				AIL_shutdown(NULL);

				gen_free(g_form_xmid);
				g_form_xmid = NULL;
			}
		}

		if (g_music == MUSIC_CDA) {
			/* initialize audio-cd */
			CD_audio_init();
		}
	}
#else
	if (g_music == MUSIC_CDA) {

		if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
			fprintf(stderr, "ERROR: failed to initialize Sound: %s\n", SDL_GetError());
			g_music = MUSIC_OFF;
			return;
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
			fprintf(stderr, "ERROR: failed to initialize SDL2_mixer: %s\n", SDL_GetError());
			g_music = MUSIC_OFF;
			return;
		}
	}
#endif
}

/* Remark: used by CD-Audio code => extern */
void exit_music(void)
{
#if defined(__BORLANDC__)
	if (g_music == MUSIC_XMID) {
		AIL_shutdown(NULL);
	} else if (g_music == MUSIC_CDA) {
		CD_audio_stop();
	}
#else
	if (g_music == MUSIC_CDA) {

		if (music != NULL) {
			Mix_FreeMusic(music);
			music = NULL;
		}

		Mix_CloseAudio();
	}
	g_music = MUSIC_OFF;
#endif
}

/* TIMER MANAGEMENT */
/* Intel 8253 ticks every 55 ms */
#if defined(__BORLANDC__)
static void interrupt timer_isr(void)
{
	/* update RNG seed */
	g_random_gen_seed2++;
	if (g_random_gen_seed2 < 0)
		g_random_gen_seed2 = 0;

	/* restart music */
	if (g_music == MUSIC_XMID) {
		restart_midi();
	} else if (g_music == MUSIC_CDA) {
		CD_enable_repeat();
	}

	/* call the former timer interrupt */
	((void interrupt far (*)(void))g_timer_isr_bak)();
}
#else
static Uint32 gen_timer_isr(Uint32 interval, __attribute__((unused)) void* param)
{
	if (SDL_LockMutex(g_sdl_timer_mutex) == 0) {

		/* update RNG */
		g_random_gen_seed2++;
		if (g_random_gen_seed2 < 0) {
			g_random_gen_seed2 = 0;
		}

		/* update MIDI */
		if (g_music == MUSIC_XMID) {
			restart_midi();
		}

		if (SDL_UnlockMutex(g_sdl_timer_mutex) == -1) {
			fprintf(stderr, "ERROR: Unlock Mutex in %s\n", __func__);
		}
	} else {
		fprintf(stderr, "ERROR: Lock Mutex in %s\n", __func__);
	}

	return interval;
}
#endif

static void set_timer_isr(void)
{
#if defined(__BORLANDC__)
	/* save adress of the old ISR */
	g_timer_isr_bak = ((void interrupt far (*)(void))getvect(0x1c));
	/* set a the new one */
	setvect(0x1c, timer_isr);
#else
	g_sdl_timer_mutex = SDL_CreateMutex();
	if (g_sdl_timer_mutex == NULL) {
		fprintf(stderr, "ERROR: failed to create g_sdl_timer_mutex\n");
		SDL_Quit();
		exit(-1);
	}

	if (SDL_LockMutex(g_sdl_timer_mutex) == 0) {

		g_sdl_timer_id = SDL_AddTimer(55, gen_timer_isr, NULL);
		if (g_sdl_timer_id == 0) {
			fprintf(stderr, "WARNING: Failed to add timer: %s\n", SDL_GetError());
		}

		if (SDL_UnlockMutex(g_sdl_timer_mutex) == -1) {
			fprintf(stderr, "ERROR: Unlock Mutex in %s\n", __func__);
		}
	} else {
		fprintf(stderr, "ERROR: Lock Mutex in %s\n", __func__);
	}
#endif
}

void restore_timer_isr(void)
{
#if defined(__BORLANDC__)
	setvect(0x1c, (void interrupt far (*)(void))g_timer_isr_bak);
#else
	if (SDL_LockMutex(g_sdl_timer_mutex) == 0) {

		SDL_bool timer_removed = SDL_RemoveTimer(g_sdl_timer_id);

		if (timer_removed == SDL_FALSE) {
			fprintf(stderr, "WARNING: Failed to remove timer\n");
		}

		if (SDL_UnlockMutex(g_sdl_timer_mutex) == -1) {
			fprintf(stderr, "ERROR: Unlock Mutex in %s\n", __func__);
		}
	} else {
		fprintf(stderr, "ERROR: Lock Mutex in %s\n", __func__);
	}
#endif
}


/**
 * \brief save the hero to a CHR file
 * \param[in] hero the hero
 */
static void save_chr(volatile struct struct_hero *hero)
{
	signed short handle;
	signed short i;
	char filename[20];
	char path[80];
	char textbuffer[20];


	/* check for typus */
	if (!hero->typus) {
		infobox(get_text(72), 0);
		return;
	}
	/* check for name */
	if (!hero->name[0]) {
		infobox(get_text(154), 0);
		return;
	}

	/* copy picture to the character struct */
	memcpy((void*)hero->pic, g_buffer_current_head, 1024);

	/* wanna save ? */
	if (!gui_bool(get_text(3)))
		return;

	/* copy name to alias */
	strncpy((char*)hero->alias, (const char*)hero->name, 15);

	/* copy name to buffer */
	strncpy(textbuffer, (const char*)hero->name, 15);

	/* prepare filename */
	for (i = 0; i < 8; i++) {
		/* leave the loop if the string ends */
		if (!textbuffer[i])
			break;
		if (!isalnum(textbuffer[i])) {
			/* replace non alphanumerical characters with underscore */
			textbuffer[i] = '_';
		}
	}

	strncpy(filename, textbuffer, 8);
	filename[8] = 0;
	strcat(filename, g_str_chr);

#if defined(__BORLANDC__) || defined(_WIN32)
	/* 0x8001 = O_BINARY | O_RDONLY */
	handle = open(filename, O_BINARY | O_RDONLY);
#else
	handle = open(filename, O_RDONLY);
#endif
	if ((handle == -1) || gui_bool(get_text(261))) {

		/* close an existing file before overwriting it */
		if (handle != -1) close(handle);

#if defined(__BORLANDC__)
		handle = open(filename, O_BINARY | _O_WRITABLE | O_TRUNC | O_CREAT | O_DENYNONE | O_RDWR, S_IREAD | S_IWRITE);
#elif  defined(_WIN32)
		handle = _open(filename, (_O_BINARY | _O_CREAT | _O_TRUNC | _O_WRONLY), _S_IWRITE);
#else
		handle = open(filename, (O_TRUNC | O_CREAT| O_RDWR), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
#endif

		if (handle != -1) {
			int flen = 0;
			int written = write(handle, (const void*)hero, sizeof(*hero));
			flen = lseek(handle, 0, SEEK_END);
			close(handle);

			if (sizeof(*hero) != 0x6da) {
				fprintf(stderr, "ERROR: sizeof(*hero) = %u\n", (unsigned short)sizeof(*hero));
			}
			if (flen != written) {
				fprintf(stderr, "ERROR: file %12s filelength = %d bytes != written %d\n", filename, flen, written);
			}
			if (g_called_with_args == 0) return;

#if defined(__BORLANDC__)
			strncpy(path, g_str_temp_dir, 20);
			strcat(path, filename);
			handle = open(filename, O_BINARY | _O_WRITABLE | O_TRUNC | O_CREAT | O_DENYNONE | O_RDWR, S_IREAD | S_IWRITE);
#elif defined(_WIN32)
			strncpy(path, g_str_temp_dir, 20);
			strcat(path, filename);
			handle = _open(filename, (_O_BINARY | _O_CREAT | _O_TRUNC | _O_WRONLY), _S_IWRITE);
#else
			strncpy(path, "TEMP/", 6);
			strcat(path, filename);
			handle = open(path, (O_TRUNC | O_CREAT| O_RDWR), (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
#endif
			if (handle != -1) {
				written = write(handle, (const void*)hero, sizeof(*hero));
				flen = lseek(handle, 0, SEEK_END);
				close(handle);

				if (sizeof(*hero) != 0x6da) {
					fprintf(stderr, "ERROR: sizeof(*hero) = %u\n", (unsigned short)sizeof(*hero));
				}
				if (flen != written) {
					fprintf(stderr, "ERROR: file %12s filelength = %d bytes != written %d\n", filename, flen, written);
				}
			}
		} else {
			infobox(g_dsagen_lang == LANG_DE ? g_str_save_error_de : g_str_save_error_en, 0);
		}
	}
}

/**
 * \brief enter the name of a hero
 */
static void enter_name(void)
{
	mouse_bg();

	/* restore background underneath the name */
	vgalib_copy_to_screen(get_gfx_ptr(180, 12), g_name_bar, 15 * 6, 8);

	enter_string((char*)g_hero.name, 180, 12, 15, 1);

	/* restore background underneath the name */
	vgalib_copy_to_screen(get_gfx_ptr(180, 12), g_name_bar, 15 * 6, 8);

	mouse_cursor();

	/* print the name on top */
	print_str((const char*)g_hero.name, 180, 12);
}

/**
 * \brief draws the current head image
 * \param[in] page the current page
 */
static void draw_head(const int page)
{
	struct nvf_desc nvf;
	signed short width;
	signed short height;

	nvf.dst = g_buffer_current_head;
	nvf.src = g_buffer_heads_dat;
	nvf.no = g_head_current;
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;

	process_nvf(&nvf);

	vgalib_copy_to_screen(get_gfx_ptr(272, (page == 0 ? 8 : 4)), g_buffer_current_head, 32, 32);
}

/**
 * \brief changes the sex of the hero
 * \return 0 = no full screen refresh needed otherwise 1
 */
static int change_sex(void)
{
	unsigned char* dst;
	unsigned char* src;
	int retval = 0;

	/* change sex of the hero */
	g_hero.sex ^= 1;

	/* hero has a typus */
	if (g_hero.typus) {
		if (g_hero.sex != 0) {
			/* from male to female */
			g_head_first = g_head_current = g_head_first_female[g_head_typus];
			g_head_last = g_head_first_male[g_head_typus + 1] - 1;
		} else {
			/* from female to male */
			g_head_first = g_head_current = g_head_first_male[g_head_typus];
			g_head_last = g_head_first_female[g_head_typus] - 1;
		}

		retval = 1;

	} else {
		/* draw the gender icon */
		dst = g_vga_memstart + 7 * O_WIDTH + 305;
		src = g_buffer_sex_dat + 256 * g_hero.sex;
		mouse_bg();
		vgalib_copy_to_screen(dst, src, 16, 16);
		mouse_cursor();
	}

	return retval;
}

/**
 * \brief refresh the screen content
 * \param[in] page the current page
 * \param[in] level 1 = Novice / 2 = Advanced
 */
static void refresh_background(const int page, const int level)
{
	g_gfx_ptr = g_vga_backbuffer;

	load_page(page);

	/* page with base values */
	if (page == 0) {

		unsigned char* src;

		/* hero is female */
		if (g_hero.sex) {
			src = g_buffer_sex_dat + 256 * g_hero.sex;
			vgalib_copy_to_screen(get_gfx_ptr(305, 7), src, 16, 16);
		}

#if defined(__BORLANDC__)
		if (g_dsagen_lang == LANG_DE) {
			/* copy arrow_area to backbuffer */
			vgalib_copy_to_screen(get_gfx_ptr(145, 178), g_arrow_area, 170, 20);
			/* copy mr_bar to backbuffer */
			vgalib_copy_to_screen(get_gfx_ptr(166, 182), g_mr_bar, 77, 9);

		}
#endif

		/* level is novice */
		if (level == 1) {

			/* Hide the arrow buttons to the other pages */
			src = g_buffer_sex_dat + 512;

			/* Hide right arrow */
			vgalib_copy_to_screen(get_gfx_ptr(284, 178), src, 20, 15);

			/* Hide left arrow */
			vgalib_copy_to_screen(get_gfx_ptr(145, 178), src, 20, 15);
		}
	}

	/* if the page is lower than 5 */
	if (page < 5) {

		/* draw DMENGE.DAT or the archetype image and name */
		if (g_hero.typus) {

			g_clear_archetype_pic = 1;

			/* copy archetype picture */
			vgalib_copy_to_screen(get_gfx_ptr(16, 8), g_buffer_typus, 128, 184);

		} else {
			if (g_clear_archetype_pic) {
				call_fill_rect_gen(g_gfx_ptr, 16, 8, 143, 191, 0);
				g_clear_archetype_pic = 0;
			}

			wait_for_vsync();
			set_palette(g_buffer_dmenge_dat + 128 * 184 + 2, 0, 32);
			vgalib_copy_to_screen(get_gfx_ptr(16, 8), g_buffer_dmenge_dat, 128, 184);
		}
	}

	/* draw the head to the backbuffer */
	if (g_hero.typus && ((page == 0) || (page > 4))) {
		draw_head(page);
	}

	g_gfx_ptr = g_vga_memstart;
}

/**
 * \brief makes a valuta string
 * \param[out] dst the destination
 * \param[in] money the money in Heller
 */
static void make_valuta_str(char * const dst, const signed long money)
{
	int d = 0, s = 0, h = 0;

	if (money > 0) {
		d = money / 100;
		s = money % 100 / 10;
		h = money % 10;
	}

	sprintf(dst, get_text(69), d, s, h);
}

/**
 * \brief calculate x coordinate for an attribute
 * \return x coordinate
 */
static int attrib_coords_x(const int attrib)
{
	return (attrib < 7 ? 205 : 273);
}

/**
 * \brief calculate y coordinate for an attribute
 * \return y coordinate
 */
static int attrib_coords_y(const int attrib)
{
	return 73 + 12 * (attrib % 7);
}

/**
 * \brief print the attribute values
 */
static void print_attribs(void)
{
	volatile signed char *p;
	char buf[8];

	int i;

	p = &g_hero.attrib[0].normal;

	for (i = 0; i < 14; p += 3, i++) {
		/* don't print 0s */
		if (p[0] != 0) {
			/* print attribute value in decimal form */
			sprintf(buf, "%d", p[0]);
			print_str(buf, attrib_coords_x(i), attrib_coords_y(i));
		}
	}
}

/**
 * \brief print archetype name to the backbuffer
 */
static void print_typusname(void)
{
	if (g_hero.sex) {
		/* print female archetype name */
		print_str(get_text(271 + g_hero.typus),
			get_line_start_c(get_text(271 + g_hero.typus), 16, 128), 184);
	} else {
		/* print male archetype name */
		print_str(get_text(17 + g_hero.typus),
			get_line_start_c(get_text(17 + g_hero.typus), 16, 128),	184);
	}
}

/**
 * \brief print the values of the hero
 * \param[in] page the current page
 * \param[in] level 1 = Novice / 2 = Advanced
 */
static void print_values(const int page, const int level)
{
	int i;
	char tmp[16];
	signed short width;
	signed short align_left = 222;
	signed short align_right = 302;

	signed short feet;
	signed short inches;

	signed short pos;

	if (g_dsagen_lang == LANG_EN) { align_left = 225; align_right = 313; }

	refresh_background(page, level);

#if defined(__BORLANDC__)
	/* copy the complete backbuffer to the screen */
	mouse_bg();
	vgalib_copy_to_screen(g_vga_memstart, g_vga_backbuffer, O_WIDTH, O_HEIGHT);
	mouse_cursor();
#endif

#if !defined(__BORLANDC__)
	unsigned char *p = calloc(O_WIDTH * O_HEIGHT, 1);
	unsigned char *gfx_bak = g_gfx_ptr;

	if (p != NULL) {
		memcpy(p, g_vga_backbuffer, O_WIDTH * O_HEIGHT);
		g_gfx_ptr = p;
	} else {
		fprintf(stderr, "%s() failed\n", __func__);
	}
#endif

	if ((g_hero.typus) && (0 <= page) && (page <= 4)) {
		print_typusname();
	}

	switch (page) {

		case 0: {
			/* print name */
			print_str((const char*)g_hero.name, 180, 12);

			/* print attributes */
			print_attribs();

			/* return if no typus */
			if (!g_hero.typus) break;

			/* print height */
			if (g_dsagen_lang == LANG_DE) {
				sprintf(g_textbuffer, get_text(70), g_hero.height);
			} else {
				feet = g_hero.height * 100 / 3048;
				inches = g_hero.height * 100 - feet * 3048;
				inches = inches / 254;
				sprintf(g_textbuffer, get_text(70), feet, inches);
			}
			print_str(g_textbuffer, 205, 25);

			/* print weight */
			sprintf(g_textbuffer, get_text(71), g_hero.weight);

			print_str(g_textbuffer, 205, 37);

			/* print god name */
			print_str(get_text(56 + g_hero.god), 205, 49);

			/* print money */
			make_valuta_str(g_textbuffer, g_hero.money);
			print_str(g_textbuffer, 205, 61);

			/* print LE */
			sprintf(tmp, "%d", g_hero.le_max); print_str(tmp, 172, 164);
			/* print AE */
			sprintf(tmp, "%d", g_hero.ae_max); print_str(tmp, 221, 164);
			/* print Endurance */
			sprintf(tmp, "%d", g_hero.le_max + g_hero.attrib[6].current);
			print_str(tmp, 296, 164);

			/* print MR */
			sprintf(tmp, "%d", g_hero.mr);
			print_str(tmp, (g_dsagen_lang == LANG_DE ? 232 + 15 : 255), 184);
			break;
		}
		case 1: {
			/* SKILLS Page 1/3 */
			/* print fight skills */
			for (i = 0; i < 9; i++) {
				sprintf(tmp, "%d", g_hero.skills[i]);
				width = get_str_width(tmp);
				// i & 1 = right column else left column
				print_str(tmp, ((i & 1) ? align_right - width : align_left - width), (i / 2) * 12 + 42);
			}

			/* print body skills */
			for (i = 9; i < 19; i++) {
				pos = i - 9;
				sprintf(tmp, "%d", g_hero.skills[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 119);
			}

			/* remaining attempts for skills */
			sprintf(tmp, "%d", g_hero.skill_incs); print_str(tmp, 271, 184);
			break;
		}
		case 2: {
			/* SKILLS Page 2/3 */
			/* print social skills */
			for (i = 19; i < 26; i++) {
				pos = i - 19;
				sprintf(tmp, "%d", g_hero.skills[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 42);
			}

			/* print lore skills */
			for (i = 32; i < 41; i++) {
				pos = i - 32;
				sprintf(tmp, "%d", g_hero.skills[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 113);
			}

			/* remaining attempts for skills */
			sprintf(tmp, "%d", g_hero.skill_incs); print_str(tmp, 271, 184);
			break;
		}
		case 3: {
			/* SKILLS Page 3/3 */
			/* print craftmansship skills */
			for (i = 41; i < 50; i++) {
				pos = i - 41;
				sprintf(tmp, "%d", g_hero.skills[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 42);

			}

			/* print nature skills */
			for (i = 26; i < 32; i++) {
				pos = i - 26;
				sprintf(tmp, "%d", g_hero.skills[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 119);
			}

			/* print intuition skills */
			for (i = 50; i < 52; i++) {
				pos = i - 50;
				sprintf(tmp, "%d", g_hero.skills[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? align_right - width : align_left - width), (pos / 2) * 12 + 170);
			}

			/* remaining attempts for skills */
			sprintf(tmp, "%d", g_hero.skill_incs); print_str(tmp, 271, 184);
			break;
		}
		case 4: {
			/* ATPA Page */
			/* Print base value  2x the same */
			sprintf(tmp, "%d", g_hero.atpa_base);
			print_str(tmp, 231, 30); print_str(tmp, 268, 30);

			for (i = 0; i < 7; i++) {

				/* print AT value */
				sprintf(tmp, "%d", g_hero.at_weapon[i]);
				print_str(tmp, 237 - get_str_width(tmp), i * 12 + 48);

				/* print PA value */
				sprintf(tmp, "%d", g_hero.pa_weapon[i]);
				print_str(tmp, 274 - get_str_width(tmp), i * 12 + 48);

				/* print skill value */
				sprintf(tmp, "%d", g_hero.skills[i]);
				print_str(tmp, 315 - get_str_width(tmp), i * 12 + 48);
			}

			/* calc range base value (KL+GE+KK) /4 */
			pos = (g_hero.attrib[1].normal
					+ g_hero.attrib[4].normal
					+ g_hero.attrib[6].normal) / 4;

			/* print missle and thrown weapon values */
			sprintf(tmp, "%d", pos + g_hero.skills[7]); print_str(tmp, 231, 144);
			sprintf(tmp, "%d", pos + g_hero.skills[8]); print_str(tmp, 231, 156);
			break;
		}

		case 5: {
			/* Spells Page 1/6 */
			for (i = 1; i < 6; i++) {
				pos = i - 1;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}
			for (i = 33; i < 38; i++) {
				pos = i - 33;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);
			}
			for (i = 6; i <= 11; i++) {
				pos = i - 6;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 146);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", g_hero.spell_incs); print_str(tmp, 217, 184);
			break;
		}
		case 6: {
			/* Spells Page 2/6 */
			for (i = 12; i <= 17; i++) {
				pos = i - 12;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}
			for (i = 18; i < 24; i++) {
				pos = i - 18;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);
			}
			for (i = 24; i < 27; i++) {
				pos = i - 24;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 148);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", g_hero.spell_incs); print_str(tmp, 217, 184);
			break;
		}
		case 7: {
			/* Spells Page 3/6 */
			for (i = 27; i < 33; i++) {
				pos = i - 27;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			for (i = 38; i < 45; i++) {
				pos = i - 38;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 95);
			}

			for (i = 45; i <= 46; i++) {
				pos = i - 45;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 160);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", g_hero.spell_incs); print_str(tmp, 217, 184);
			break;
		}
		case 8: {
			/* Spells Page 4/6 */
			for (i = 47; i <= 48; i++) {
				pos = i - 47;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			for (i = 49; i < 58; i++) {
				pos = i - 49;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 71);
			}

			for (i = 58; i < 60; i++) {
				pos = i - 58;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 148);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", g_hero.spell_incs); print_str(tmp, 217, 184);
			break;
		}
		case 9: {
			/* Spells Page 5/6 */
			for (i = 60; i < 76; i++) {
				pos = i - 60;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", g_hero.spell_incs); print_str(tmp, 217, 184);
			break;
		}
		case 10: {
			/* Spells Page 6/6 */
			for (i = 76; i < 86; i++) {
				pos = i - 76;
				sprintf(tmp, "%d", g_hero.spells[i]);
				width = get_str_width(tmp);
				// pos & 1 = right column else left column
				print_str(tmp, ((pos & 1) ? 302 - width : 157 - width), (pos / 2) * 12 + 42);
			}

			/* print spell attempts */
			sprintf(tmp, "%d", g_hero.spell_incs); print_str(tmp, 217, 184);
			break;
		}
	}

#if !defined(__BORLANDC__)
	if (p != NULL) {
		/* copy the complete backbuffer to the screen */
		mouse_bg();
		vgalib_copy_to_screen(g_vga_memstart, p, O_WIDTH, O_HEIGHT);
		mouse_cursor();

		free(p);
		p = NULL;

		g_gfx_ptr = gfx_bak;
	}
#endif
}


/**
 * \brief initializes the hero structure and global variables
 * \param[in] hero the hero
 * \return 1 hero cleared
 */
static int clear_hero(volatile struct struct_hero* hero)
{
	int i;

	/* clear global variables */
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

	/* clear and set hero structure */
	memset((void*)hero, 0, sizeof(*hero));

	hero->level = 1;
	hero->group = 1;

	return 1;
}

/**
 * \brief roll out new attribute values
 * \param[in] page the current page
 * \param[in] level 1 = Novice / 2 = Advanced
 */
static void new_attributes(const int page, const int level)
{
	volatile signed char *att_ptr;
	char name_bak[20];
	int i;
	int j;
	int randval;
	int di;
	int values[8];
	int pos_attribs[8];
	int unset_attribs;
	int full_refresh = 0;
	signed char sex_bak;

	/* set variable if hero has a typus */
	if (g_hero.typus)
		full_refresh = 1;

	/* save the name of the hero */
	strncpy(name_bak, (const char*)g_hero.name, 15);

	/* save the sex of the hero */
	sex_bak = g_hero.sex;

	/* clear the hero */
	clear_hero(&g_hero);

	g_hero.sex = sex_bak;

	/* restore the name of the hero */
	strncpy((char*)g_hero.name, name_bak, 15);

	if (full_refresh) {
		print_values(page, level);
	}

	att_ptr = &g_hero.attrib[0].normal;

	/* positive attributes ala DSA3 */
	/* roll 8 times: W6 + 7 and skip a minimal value */
	for (i = 0; i < 8; i++) {
		pos_attribs[i] = random_interval_gen(8, 13);
	}

	/* make sure the last element is a minimal one */
	for (i = 1; i < 8; i++) {
		if (pos_attribs[i - 1] < pos_attribs[i]) {
			const int tmp = pos_attribs[i];
			pos_attribs[i] = pos_attribs[i - 1];
			pos_attribs[i - 1] = tmp;
		}
	}

	for (j = 0; j < 7; j++) {
		randval = pos_attribs[j];
		unset_attribs = 0;

		for (i = 0; i < 7; i++) {
			// NORMAL
			if (!att_ptr[3 * i]) {
                                // not initialized attribute
				values[unset_attribs] = (signed char)i;
				g_type_names[unset_attribs] = get_text(32 + i);
				unset_attribs++;
			}
		}

		sprintf(g_textbuffer, get_text(46), randval);

		do {
			g_text_x_mod = -80;

			di = gui_radio(g_textbuffer, unset_attribs,
				g_type_names[0], g_type_names[1], g_type_names[2],
				g_type_names[3], g_type_names[4], g_type_names[5],
				g_type_names[6]);

			g_text_x_mod = 0;

		} while (di == -1);

		di = values[di - 1];
		/* write randval to the selected positive attribute */
		//g_hero.attrib[di].normal = g_hero.attrib[di].current = randval;
		att_ptr[3 * di + 0] = att_ptr[3 * di + 1] = randval;

		mouse_bg();
		print_values(page, level);
		mouse_cursor();
	}

	att_ptr = &g_hero.attrib[7].normal;

	for (j = 0; j < 7; j++) {
		randval = random_interval_gen(2, 7);
		unset_attribs = 0;

		for (i = 0; i < 7; i++) {
			// NORMAL
			if (!att_ptr[3 * i]) {
				values[unset_attribs] = (signed char)i;
				g_type_names[unset_attribs] = get_text(39 + i);
				unset_attribs++;
			}
		}

		sprintf(g_textbuffer, get_text(46), randval);

		do {
			g_text_x_mod = -80;

			di = gui_radio(g_textbuffer, unset_attribs,
				g_type_names[0], g_type_names[1], g_type_names[2],
				g_type_names[3], g_type_names[4], g_type_names[5],
				g_type_names[6]);

			g_text_x_mod = 0;

		} while (di == -1);

		di = values[di - 1];

		/* write randval to the selected negative attribute */
		//g_hero.attrib[di].normal = g_hero.attrib[di].current = randval;
		att_ptr[3 * di + 0] = att_ptr[3 * di + 1] = randval;

		mouse_bg();
		print_values(page, level);
		mouse_cursor();
	}
}

/**
 * \brief calculate AT and PA values
 */
static void calc_at_pa(void)
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
 * \brief tries to increment a skill in novice mode
 * \param skill	index of the skill which should be incremented
 */
static void skill_inc_novice(const signed short skill)
{
	int done = 0;

	while (!done) {

		/* leave the loop if 3 tries have been done or no attempts are left */
		if ((g_skill_incs[skill].tries == 3) || (g_hero.skill_incs == 0)) {
			/* set the flag to leave this loop */
			done = 1;
		} else {
			/* decrement counter for skill increments */
			g_hero.skill_incs--;

			/* check if the test is passed */
			if (random_interval_gen(2, 12) > g_hero.skills[skill]) {
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
 * \brief tries to increment a spell in novice mode
 * \param[in] spell index of the spell which should be incremented
 */
static void spell_inc_novice(const signed short spell)
{
	int done = 0;

	while (!done) {

		/* leave the loop if 3 tries have been done or no attempts are left */
		if ((g_spell_incs[spell].tries == 3) || (g_hero.spell_incs == 0)) {
			done = 1;
		} else {

			/* decrement counter for spell increments */
			g_hero.spell_incs--;

			/* check if the test is passed */
			if (random_interval_gen(2, 12) > g_hero.spells[spell]) {

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
}

/**
 * \brief fills the values if typus is chosen
 * \param[in] level 1 = Novice / 2 = Advanced
 */
static void fill_values(const int level)
{
	const struct struct_money *money_ptr;
	char textbuffer[92]; // 84 would be enough
	signed short i;
	signed short skill;
	signed short spell;
	signed short si;
	int conv_incs = 0;

	/* fill skill values */
	for (i = 0; i < 52; i++) {

		g_hero.skills[i] = g_skills[g_hero.typus][i];

		/* set skill_incs and skill_tries to zero */
		g_skill_incs[i].tries = g_skill_incs[i].incs = 0;
	}

	/* set skill_attempts */
	g_hero.skill_incs = g_initial_skill_incs[g_hero.typus - 1];

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
				g_hero.spell_school = gui_radio(get_text(47), 9,
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

		/* set spell and convertable increase attempts */
		if (g_hero.typus <= 12) { // check to silence the BCC
			g_hero.spell_incs = g_initial_spell_incs[g_hero.typus - 7];
			conv_incs = g_initial_conv_incs[g_hero.typus - 7];
		}

		/* get convertable increase attempts */
		if ((conv_incs != 0) && (level == 2) && gui_bool(get_text(269))) {
			/* create string */
			sprintf(textbuffer, get_text(270), conv_incs);

			i = infobox(textbuffer, 1);

			if (i > 0) {
				/* spell attempts to skill attempts */
				if (i > conv_incs)
					i = conv_incs;
				conv_incs -= i;
				/* change spell attempts */
				g_hero.spell_incs -= i;
				/* change skill attempts */
				g_hero.skill_incs += i;
			} else {

				/* create string */
				sprintf(textbuffer, get_text(271), conv_incs);

				i = infobox(textbuffer, 1);
				if (i > 0) {
					if (i > conv_incs)
						i = conv_incs;
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
	if ((g_hero.typus == 9) && (level == 2) && gui_bool(get_text(268))) {
		/* change spell_attempts */
		g_hero.spell_incs -= 10;
		g_hero.ae_max += random_interval_gen(3, 8);
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
	money_ptr = g_money_tab[g_hero.typus - 1];
	for (si = 0; money_ptr[si].value < i; si++);

	g_hero.money = (signed long)(10 * random_interval_gen(money_ptr[si].min, money_ptr[si].max));

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
	/* calculate AT and PA values */
	calc_at_pa();

	/* if mode == novice */
	if (level == 1) {
		/* increase skills automatically */
		for (i = 0; g_hero.skill_incs > 0; i++) {
			skill = g_autoskills[g_hero.typus][i];
			skill_inc_novice(skill);
		}

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
			spell = g_autospells[g_hero.typus - 7][i];
			spell_inc_novice(spell);
		}
	}
}

#define INC (1)
#define DEC (2)

/**
 * \brief tests if attribute changes are possible
 */
static signed short can_change_attributes(void)
{
	volatile signed char *p;
	signed short i;
	signed short na_inc = 0;
	signed short na_dec = 0;
	signed short pa_inc = 0;
	signed short pa_dec = 0;

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
 * \brief change attributes
 * \param[in] page the current page
 * \param[in] level 1 = Novice / 2 = Advanced
 */
static void change_attributes(const int page, const int level)
{
	volatile signed char *ptr1;
	volatile signed char *ptr2;
	signed short tmp1;
	signed short tmp2;
	signed short tmp3;
	signed char c;

	signed short si;
	signed short di;

	/* check if attributes have been set */
	if (!g_hero.attrib[0].normal) {
		infobox(get_text(16), 0);
		return;
	}
	/* check if changing is possible */
	if (!can_change_attributes()) {
		infobox(get_text(266), 0);
		return;
	}
	/* if hero has an archtype */
	if (g_hero.typus) {

		if (gui_bool(get_text(73))) {

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

			print_values(page, level);

		} else {
			return;
		}
	}

	/* check again if changing is possible */
	if (can_change_attributes() == 0) {
		infobox(get_text(266), 0);
		return;
	}
	/* select a positive attribute to change */
	g_text_x_mod = -80;
	tmp2 = gui_radio(get_text(78), 7,
			get_text(32), get_text(33), get_text(34), get_text(35),
			get_text(36), get_text(37), get_text(38));
	g_text_x_mod = 0;

	if (tmp2 != -1) {

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

			g_attrib_changed[tmp2] = INC;

			print_values(page, level);

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

					print_values(page, level);
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

			print_values(page, level);

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

				print_values(page, level);
			}
		}
	}
}

#undef INC
#undef DEC

/**
 * \brief select a possible typus with current attribute values
 * \param[in] level 1 = Novice / 2 = Advanced
 * \return 0 = no changes / 1 = archetype selected
 */
static int select_typus(const int level)
{
	volatile signed char *ptr;
	signed short i;
	signed short impossible;
	signed short di;
	signed short si;
	signed char old_typus;
	signed char possible_types;
	signed char ltmp2;


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
			if (!can_change_attributes()) {
				/* totally messed up values */
				infobox(get_text(284), 0);
			} else {
				infobox(get_text(31), 0);
			}
			return 0;
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

			load_typus(g_hero.typus);

			mouse_bg();
			call_fill_rect_gen(g_vga_memstart, 16, 8, 143, 191, 0);
			wait_for_vsync();
			set_palette(g_buffer_typus + 128 * 184 + 2, 0, 32);
			mouse_cursor();

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
			fill_values(level);

			return 1;
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
	}

	return 0;
}

/**
 * \brief increment a skill
 * \param[in] skill index of the skill
 * \param[in] max maximum number of increase attempts
 * \param[in] msg textmessage iff max is reached
 */
static void inc_skill(const signed short skill, const signed short max, char *msg)
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
	if (random_interval_gen(2, 12) > g_hero.skills[skill]) {
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
}

/**
 * \brief select a skill
 * \param[in] page the current page
 */
static void select_skill(const int page)
{
	int group = -1;

	do {
		int skill = -2;

		/* check skill attempts */
		if (!g_hero.skill_incs) {
			infobox(get_text(94), 0);
			g_text_x_mod = 0;
			return;
		}

		g_text_x_mod = -80;

		if (page == 1) {

			group = gui_radio(get_text(93), 2, get_text(86), get_text(87));

			if (group != -1) {
				if (group == 1) {

					/* Fight */
					skill = gui_radio(get_text(147), 9,
						get_text(95), get_text(96), get_text(97),
						get_text(98), get_text(99), get_text(100),
						get_text(101), get_text(102), get_text(103)) - 1;

					if (skill != -2) {
						inc_skill(skill, 1, get_text(148));
					}

				} else if (group == 2) {

					/* Body */
					skill = gui_radio(get_text(147), 10,
						get_text(104), get_text(105),
						get_text(106), get_text(107),
						get_text(108), get_text(109),
						get_text(110), get_text(111),
						get_text(112), get_text(113)) - 1;

					if (skill != -2) {
						skill += 9;
						inc_skill(skill, 2, get_text(149));
					}
				}
			}


		} else if (page == 2) {

			group = gui_radio(get_text(93), 2, get_text(88), get_text(89));

			if (group != -1) {
				if (group == 1) {

					skill = gui_radio(get_text(147), 7,
							get_text(114), get_text(115), get_text(116),
							get_text(117), get_text(118), get_text(119),
							get_text(120)) - 1;

					if (skill != -2) {
						skill += 19;
						inc_skill(skill, 2, get_text(149));
					}

				} else if (group == 2) {

					skill = gui_radio(get_text(147), 9,
							get_text(127), get_text(128), get_text(129),
							get_text(130), get_text(131), get_text(132),
							get_text(133), get_text(134), get_text(135)) - 1;

					if (skill != -2) {
						skill += 32;
						inc_skill(skill, 3, get_text(150));
					}
				}
			}

		} else if (page == 3) {

			group = gui_radio(get_text(93), 3, get_text(90), get_text(91), get_text(92));

			if (group != -1) {
				if (group == 1) {
					skill = gui_radio(get_text(147), 9,
						get_text(136), get_text(137), get_text(138),
						get_text(139), get_text(140), get_text(141),
						get_text(142), get_text(143), get_text(144)) - 1;

					if (skill != -2) {
						skill += 41;
						inc_skill(skill, 2, get_text(149));
					}

				} else if (group == 2) {
					skill = gui_radio(get_text(147), 6,
						get_text(121), get_text(122), get_text(123),
						get_text(124), get_text(125), get_text(126)) - 1;

					if (skill != -2) {
						skill += 26;
						inc_skill(skill, 2, get_text(149));
					}

				} else if (group == 3) {
					skill = gui_radio(get_text(147), 2,
						get_text(145),
						get_text(146)) - 1;

					if (skill != -2) {
						skill += 50;
						inc_skill(skill, 1, get_text(148));
					}
				}
			}
		}

		g_text_x_mod = 0;

		if (skill != -2) {
			print_values(page, 2);
		}

	} while (group != -1);
}

/**
 * \brief increment spell value
 * \param[in] spell index of the spell to increment
 */
static void inc_spell(const signed short spell)
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
	} else {

		/* decrement spell attempts */
		g_hero.spell_incs--;

		if (random_interval_gen(2, 12) > g_hero.spells[spell]) {
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
	}
}

/**
 * \brief	select a spell
 * \param[in] page the current page
 */
static void select_spell(const int page)
{
	int group = -1;

	do {
		int spell = -2;

		/* check if we have spell attempts */
		if (!g_hero.spell_incs) {
			infobox(get_text(94), 0);
			g_text_x_mod = 0;
			return;
		}

		g_text_x_mod = -90;

		switch (page) {
			case 5: {
				group = gui_radio(get_text(155), 3,
						get_text(157), get_text(162),
						get_text(158));
				if (group != -1) {
				switch (group)
				{
					case 1: {
						spell = gui_radio(get_text(156), 5,
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

		if (spell != -2) {
			print_values(page, 2);
		}

	} while (group != -1);
}

/**
 * \brief select attack and parade values
 * \param[in] page the current page
 * \param[in] level 1 = Novice / 2 = Advanced
 * \return 0 = no changes / 1 = at/pa values changed
 */
static int select_atpa(const int page, const int level)
{
	int skill;
	int retval = 0;

	g_text_x_mod = -80;

	do {
		int full_refresh = 0;

		/* print menu with all melee weapons skills */
		skill = gui_radio(get_text(78), 7,
			get_text(95), get_text(96), get_text(97), get_text(98),
			get_text(99), get_text(100), get_text(101)) - 1;

		if (skill != -2) {
			if (g_hero.skills[skill] <= 0) {
				infobox(get_text(260), 0);
			} else {

				int increase = gui_radio(get_text(254), 2, get_text(75), get_text(76));

				if (increase != -1) {
					if (increase == 1) {
						/* increase attack */
						if (g_hero.skills[skill] >= 0 &&
							(g_hero.pa_weapon[skill] > g_hero.atpa_base)) {
							/* inc AT */
							g_hero.at_weapon[skill]++;
							/* dec PA */
							g_hero.pa_weapon[skill]--;

							full_refresh = 1;
							retval = 1;
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

							full_refresh = 1;
							retval = 1;
						} else {
							infobox(get_text(256), 0);
						}
					}
				}
			}
		}

		if (full_refresh) print_values(page, level);

	} while (skill != -2);

	g_text_x_mod = 0;

	return retval;
}

/**
 * \brief	choose a typus manually
 * \param[in] level 1 = Novice / 2 = Advanced
 * \return 0 = no changes / 1 = archetype chosen
 */
static int choose_typus(const int level)
{
	volatile signed char *ptr;
	int choosen_typus;
	int randval;
	char name_bak[20];
	signed char sex_bak;

	int i;
	int typus_names;

	if (!gui_bool(get_text(264)))
		return 0;

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
		return 0;

	/* clear the hero area with saved name and sex */
	strncpy(name_bak, (const char*)g_hero.name, 15);
	sex_bak = g_hero.sex;

	clear_hero(&g_hero);

	g_hero.sex = sex_bak;
	strncpy((char*)g_hero.name, name_bak, 15);

	/* set typus */
	g_hero.typus = (signed char)choosen_typus;

	ptr = &g_hero.attrib[0].normal;

	/* roll out positive attribute values */
	for (i = 0; i < 7; i ++) {

		randval = random_interval_gen(8, 13);

		if (randval > 8)
			randval--;
		/* set attrib.current ant attrib.normal */
		ptr[3 * i] = ptr[3 * i + 1] = (signed char)randval;
	}

	ptr = &g_hero.attrib[7].normal;

	/* roll out negative attribute values */
	for (i = 0; i < 7; i ++) {

		randval = random_interval_gen(2, 7);

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
	mouse_bg();
	call_fill_rect_gen(g_vga_memstart, 16, 8, 143, 191, 0);
	wait_for_vsync();
	set_palette(g_buffer_typus + 128 * 184 + 2, 0, 32);
	mouse_cursor();


	g_head_typus = (g_hero.typus > 10 ? 10 : g_hero.typus);

	if (g_hero.sex) {
		g_head_first = g_head_current = g_head_first_female[g_head_typus];
		g_head_last = g_head_first_male[g_head_typus + 1] - 1;
	} else {
		g_head_first = g_head_current = g_head_first_male[g_head_typus];
		g_head_last = g_head_first_female[g_head_typus] - 1;
	}
	fill_values(level);

	return 1;
}

/**
 * \brief main loop of gen
 * \param[in] init_level 1 = Novice / 2 = Advanced
 */
static void do_gen(const int init_level)
{
	int done = 0;
	int menu_option;
	int target_page;
	int level = init_level;
	int full_refresh;
	int page = 0;

	/* initialize the hero structure */
	full_refresh = clear_hero(&g_hero);

	/* emulate a right click to open the menu */
	g_mouse_rightclick_event = 1;

	/* main loop */
	while (!done) {

		if (full_refresh) {
			print_values(page, level);
			full_refresh = 0;
		}

		flush_keyboard_queue();

		g_action_table = g_action_page[page];
		handle_input();
		g_action_table = NULL;

		if (g_mouse_rightclick_event || g_in_key_ext == KEY_PGUP) {

			/* print the menu for each page */
			if (page == 0) {

				menu_option = gui_radio(get_text(7), 9,
					get_text(10), get_text(11), get_text(15),
					get_text(8),  get_text(14), get_text(12),
					get_text(262),get_text(9),  get_text(258));

				if (menu_option != -1) {
					if ((menu_option >= 4) && (menu_option < 6) && (g_hero.attrib[0].normal) && !gui_bool(get_text(13))) {
						menu_option = 0;
					}
					g_in_key_ext = 0;
					switch (menu_option) {
						case 1: {
							enter_name();
							break;
						}
						case 2: {
							full_refresh = change_sex();
							break;
						}
						case 3: {
							change_attributes(page, level);
							break;
						}
						case 4: {
							full_refresh = clear_hero(&g_hero);

							/* imediately open the menu */
							g_mouse_rightclick_event = 1;
							break;
						}
						case 5: {
							new_attributes(page, level);
							break;
						}
						case 6: {
							full_refresh = select_typus(level);
							break;
						}
						case 7: {
							full_refresh = choose_typus(level);
							break;
						}
						case 8: {
							save_chr(&g_hero);
							break;
						}
						case 9: {
							if (gui_bool(get_text(259)))
								done = 1;
							break;
						}
					}
				}

			} else if ((1 <= page) && (page <= 3)) 	select_skill(page);
			  else if (page == 4)			select_atpa(page, level);
			  else if ((5 <= page) && (page <= 10)) select_spell(page);
		}

		if (g_in_key_ext == KEY_CTRL_F3)
			full_refresh = change_sex();

		if (g_in_key_ext == KEY_CTRL_F4)
			enter_name();

		if ((page == 0) && (g_in_key_ext == KEY_6)) {
			level = (level == 1 ? 2 : 1);
			full_refresh = 1;
		}

		/* show credits in an infobox() */
		if ((page == 0) && (g_in_key_ext == 0xfd)) {

			char str[100];
			gen_memstat(str);

			g_menu_tiles = 4;
			infobox(get_text(267), 0);
			infobox(str, 0);
			g_menu_tiles = 3;
		}

		/* Change Head Logic */
		if ((page == 0) && ((g_in_key_ext == KEY_UP) || (g_in_key_ext == KEY_DOWN))) {

			if (g_hero.typus) {
				if (g_in_key_ext == KEY_UP) {
					g_head_current = (g_head_current < g_head_last) ?
								g_head_current + 1 : g_head_first;

				} else if (g_in_key_ext == KEY_DOWN) {
					g_head_current = (g_head_current > g_head_first) ?
								g_head_current - 1 : g_head_last;
				}
				draw_head(page);
			} else {
				infobox(get_text(17), 0);
			}
		}

		/* Change Page Logic */
		if (level == 2) {

			if (g_in_key_ext == KEY_RIGHT) {

				if (g_hero.typus) {
					if (((g_hero.typus < 7) ? 4 : 10) > page) {
						page++;
					} else {
						page = 0;
					}

					full_refresh = 1;

				} else {
					infobox(get_text(72), 0);
				}
			}

			if (g_in_key_ext == KEY_LEFT) {

				if (g_hero.typus) {
					if (page > 0) {
						page--;
					} else {
						page = (g_hero.typus < 7 ? 4 : 10);
					}

					full_refresh = 1;

				} else {
					infobox(get_text(72), 0);
				}
			}

			if ((g_in_key_ext >= KEY_1) && (g_in_key_ext <= KEY_5)) {

				if (g_hero.typus) {
					target_page = ((g_in_key_ext == KEY_1) ? 0 : (
						(g_in_key_ext == KEY_2) ? 1 : (
						(g_in_key_ext == KEY_3) ? 4 : (
						(g_in_key_ext == KEY_4) ? 5 : 10))));

					if ((target_page != page) && (target_page < 5 || g_hero.typus >= 7)) {
						page = target_page;
						full_refresh = 1;
					}
				} else {
					infobox(get_text(72), 0);
				}
			}
		}
#if !defined(__BORLANDC__)
		SDL_Delay(75);
		if (g_sdl_quit_event) done = 1;
#endif
	}
}

/* INTRO */

/**
 * \brief draw the ATTIC animation
 */
static void intro_attic(void)
{
	int handle;
	int ani_height;
	int ani_y;
	int i;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	/* load ATTIC */
	handle = open_datfile(18);
	if (handle != -1) {
		read_datfile(handle, g_buffer_heads_dat, get_filelength());
		close(handle);

		nvf.src = g_buffer_heads_dat;
		nvf.type = 0;
		nvf.width = &width;
		nvf.height = &height;

		for (i = 7; i >= 0; i--) {
			nvf.dst = g_vga_backbuffer + i * 960L + 9600;
			nvf.no = i + 1;
			process_nvf(&nvf);

		}

		/* set dst */
		nvf.dst = g_vga_backbuffer;
		/* set no */
		nvf.no = 0;
		process_nvf(&nvf);

		wait_for_vsync();

		set_palette((const unsigned char*)g_pal_attic, 0, 16);

		ani_height = 1;
		ani_y = 99;

		/* glowing at the bottom */
		for (i = 0; i < 4; i++) {
			vgalib_copy_to_screen(g_vga_memstart + 140 * O_WIDTH + 112,
					g_vga_backbuffer + i * 960 + 9600, 96, 10);
			vsync_or_key(20);
		}

		/* elevate the attic logo */
		i = 4;
		g_in_key_ext = 0;
		while ((ani_height <= 100) && (g_in_key_ext == 0)) {

			vgalib_copy_to_screen(g_vga_backbuffer + (ani_y + 60) * O_WIDTH,
					g_vga_backbuffer, 96, ani_height);

			if (ani_height != 100) {

				if (ani_height % 4 == 1)
					i++;

				if (i == 8)
					i = 4;

				vgalib_copy_to_screen_nonzero(g_vga_backbuffer + 150 * O_WIDTH,
								g_vga_backbuffer + i * 960 + 9600,
								96, 10);
			}

			vgalib_screen_copy(g_vga_memstart + 50 * O_WIDTH + 112,
						g_vga_backbuffer + 60 * O_WIDTH,
						96, 100);

			ani_height++;
			ani_y--;
			if (ani_height < 37)
				vsync_or_key(2);
			else
				vsync_or_key(1);
		}

		if (g_in_key_ext == 0)
			vsync_or_key(200);
	}
}

/**
 * \brief draw the FANPRO logo
 */
static void intro_fanpro(void)
{
	int handle;
	int flen;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	/* load FANPRO.NVF */
	handle = open_datfile(34);
	if (handle != -1) {
		flen = read_datfile(handle, g_buffer_heads_dat, get_filelength());
		close(handle);

		nvf.src = g_buffer_heads_dat;
		nvf.type = 0;
		nvf.width = &width;
		nvf.height = &height;
		nvf.dst = g_vga_backbuffer;
		nvf.no = 0;

		process_nvf(&nvf);

		/* clear screen */
		vgalib_fill_rect(g_vga_memstart, 0, O_WIDTH, O_HEIGHT);
		wait_for_vsync();

		/* set palette of FANPRO.NVF */
		set_palette(g_buffer_heads_dat + flen - 32 * 3, 0, 32);

		/* draw the picture */
		vgalib_copy_to_screen(g_vga_memstart + 50 * O_WIDTH + 60,
					g_vga_backbuffer, 200, 100);

		vsync_or_key(200);
	}
}

/**
 * \brief fade out for a color palette
 * \param[out] dst destination palette
 * \param[in] src source palette
 * \param[in] n the number of colors
 */
static void pal_fade_out(unsigned char *dst, const unsigned char *src, const signed short n)
{
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
	}
}

/**
 * \brief fade in for a color palette
 * \param[out] dst destination palette
 * \param[in] src source palette
 * \param[in] col the first color of the palette
 * \param[in] n the number of colors
 */
static void pal_fade_in(unsigned char *dst, const unsigned char *src, const signed short col, const signed short n)
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

/**
 * \brief draw the FANPRO logo
 */
static void intro_title(void)
{
	unsigned char *pal_src;
	unsigned char *pal_dst;
	int i;
	int handle;
	int flen;
	signed short width;
	signed short height;
	struct nvf_desc nvf;

	if (g_dsagen_lang == LANG_DE) {

		/* load DSALOGO.DAT */
		handle = open_datfile(16);
		read_datfile(handle, g_buffer_heads_dat, get_filelength());
		close(handle);

		nvf.src = g_buffer_heads_dat;
		nvf.type = 0;
		nvf.width = &width;
		nvf.height = &height;
		nvf.dst = g_vga_backbuffer;
		nvf.no = 0;

		process_nvf(&nvf);

		/* clear screen */
		vgalib_fill_rect(g_vga_memstart, 0, O_WIDTH, O_HEIGHT);
		wait_for_vsync();

		set_palette((const unsigned char*)g_pal_tmp, 0, 32);

		/* draw DSALOGO.DAT */
		vgalib_copy_to_screen(g_vga_memstart, g_vga_backbuffer, O_WIDTH, 100);

	} else {

		/* load ROALOGUS.DAT */
		handle = open_datfile(37);
		flen = read_datfile(handle, g_buffer_heads_dat, get_filelength());
		close(handle);

		decomp_pp20(g_vga_backbuffer, g_buffer_heads_dat, (unsigned short)flen);

		/* clear screen */
		vgalib_fill_rect(g_vga_memstart, 0, O_WIDTH, O_HEIGHT);
		memset(g_pal_roalogo, 0, 3 * 256);
		set_palette(g_pal_roalogo, 0, 256);

		/* draw ROALOGUS.DAT */
		vgalib_copy_to_screen(g_vga_memstart, g_vga_backbuffer, O_WIDTH, 140);

		memcpy(g_pal_roalogo, g_vga_backbuffer + 140 * O_WIDTH + 2, 3 * 256);
	}

	/* load GENTIT.DAT / E_GENTIT.DAT */
	handle = open_datfile(17);
	read_datfile(handle, g_buffer_heads_dat, get_filelength());
	close(handle);

	nvf.src = g_buffer_heads_dat;
	nvf.dst = g_vga_backbuffer;
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;
	nvf.no = 0;

	process_nvf(&nvf);

	if (g_dsagen_lang == LANG_DE) {
		/* draw GENTIT.DAT */
		vgalib_copy_to_screen(g_vga_memstart + 110 * O_WIDTH + 10, g_vga_backbuffer, O_WIDTH, 50);
	} else {
		/* draw E_GENTIT.DAT */
		vgalib_copy_to_screen(g_vga_memstart + 140 * O_WIDTH, g_vga_backbuffer, O_WIDTH, 50);
	}

	/* palette fade in */
	sdl_toggle_pal_logic();
	if (g_dsagen_lang == LANG_DE) {

		memcpy(g_vga_backbuffer + 500, &g_pal_dsalogo, 96);

		pal_src = g_vga_backbuffer + 500;
		pal_dst = g_vga_backbuffer;

		memset(pal_dst, 0, 96);

		for (i = 0; i < 64; i++) {
			pal_fade_in(pal_dst, pal_src, i, 32);
			wait_for_vsync();
			set_palette(pal_dst, 0, 32);
		}

	} else {

		memcpy(g_pal_roalogo + 3 * 32, &g_pal_dsalogo, 3 * 32);
		set_palette(g_pal_roalogo + 0x180, 128, 128);
		memcpy(g_vga_backbuffer + 500, &g_pal_dsalogo, 3 * 32);

		pal_src = g_vga_backbuffer + 500;
		pal_dst = g_vga_backbuffer;

		memset(pal_dst, 0, 96);

		for (i = 0; i < 64; i++) {
			pal_fade_in(pal_dst, pal_src, i, 32);
			wait_for_vsync();
			set_palette(pal_dst, 32, 32);
		}

	}
	sdl_toggle_pal_logic();

	/* print version */
	set_textcolor(0xff, 0x00); // WHITE ON BLACK
	print_str((char*)g_str_version, 290, 190);
	vsync_or_key(400);

	/* palette fade out */
	sdl_toggle_pal_logic();
	if (g_dsagen_lang == LANG_DE) {

		memcpy(g_vga_backbuffer, &g_pal_dsalogo, 96);

		pal_src = g_vga_backbuffer + 500;
		pal_dst = g_vga_backbuffer;

		memset(g_vga_backbuffer + 500, 0, 96);

		for (i = 0; i < 64; i++) {
			pal_fade_out(pal_dst, pal_src, 32);
			wait_for_vsync();
			set_palette(pal_dst, 0, 32);
		}
	} else {

		memcpy(g_vga_backbuffer, &g_pal_roalogo, 3 * 256);

		pal_src = g_vga_backbuffer + 800;
		pal_dst = g_vga_backbuffer;

		memset(g_vga_backbuffer + 800, 0, 3 * 256);

		for (i = 0; i < 64; i++) {
			pal_fade_out(pal_dst, pal_src, 256);
			wait_for_vsync();
			set_palette(pal_dst, 0, 256);
		}
	}	
	sdl_toggle_pal_logic();
}

/**
 * \brief play the intro
 */
static void intro(void)
{
	g_in_intro = 1;

	intro_attic();

	intro_fanpro();

	intro_title();

	/* clear screen */
	vgalib_fill_rect(g_vga_memstart, 0, O_WIDTH, O_HEIGHT);

	/* clear used buffers */
	memset(g_buffer_heads_dat, 0, 39000);
	memset(g_vga_backbuffer, 0, 64100);

	g_in_intro = 0;
}

#if defined(_WIN32)
int WinMain(__attribute__((unused)) HINSTANCE hInstance,
		__attribute__((unused)) HINSTANCE hPrevInstance,
		__attribute__((unused)) LPSTR lpCmdLine,
		__attribute__((unused)) int ShowCmd)
#else
#define main_gen main
int main_gen(int argc, char **argv)
#endif
{
#if defined(_WIN32)
	int argc;
	LPWSTR cmdline = GetCommandLineW();
	LPWSTR *argv = CommandLineToArgvW(cmdline, &argc);
#endif
	int l_level = -1;
	char param_level;

	if (argc > 1) {
		g_called_with_args = 1;

		if (argc > 2) {
			param_level = argv[2][0];

			/* try to set the level from parameters */
			l_level = ((param_level == 'a') ? 2 : ((param_level == 'n') ? 1 : -1));

			if ((argc > 3) && (argv[3][0] == '0')) {
				g_music = MUSIC_OFF;
			}
		}
	}

	if (detect_datfile() == -1) return -1;

	if (sizeof(g_hero) != 0x6da) {
		fprintf(stderr, "ERROR: sizeof(hero) = %u\n", (unsigned short)sizeof(g_hero));
		return -1;
	}

	if (alloc_buffers() > 0) {
		free_buffers();
		return -1;
	}

	load_essential_files();

	init_video();

	/* Remark: gui elements are usable at this point at runtime */

	set_timer_isr();

	mouse_enable();

	if (g_music != MUSIC_OFF) {
		init_music();
		start_music(33);
	}

	if (g_called_with_args == 0) {
		intro();
		init_colors();
	}

	load_common_files();

#if !defined(__BORLANDC__)
	load_pages();

	SDL_ShowCursor(SDL_ENABLE);
#else
	mouse_cursor();
#endif

	flush_keyboard_queue();

	/* ask for level */
	while (l_level == -1) {
		l_level = gui_radio(get_text(0), 2, get_text(1), get_text(2));
#if !defined(__BORLANDC__)
		if (g_sdl_quit_event) return 0;
#endif
	}

	do_gen(l_level);

	if (g_music != MUSIC_OFF) {
		exit_music();
	}

#if defined(__BORLANDC__)
	mouse_bg();
#endif

	mouse_disable();

	/* Clear the screen and return to SCHICKM.EXE/BLADEM.EXE */
	call_fill_rect_gen(g_vga_memstart, 0, 0, O_WIDTH - 1, O_HEIGHT - 1, 0);

	restore_timer_isr();

	if (!g_called_with_args) {
		/* Clear the screen and return to DOS */
		exit_video();
		clrscr();
	}

	free_buffers();

	return 0;
}
