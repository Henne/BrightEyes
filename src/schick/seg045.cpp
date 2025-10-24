/*
 *	Rewrite of DSA1 v3.02_de functions of seg045 (fight helper)
 *	Functions rewritten: 8/8 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg045.cpp
 */
#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg038.h"
#include "seg039.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

static const signed short g_gfxtab_shotbolt_ox[6][4] = {
	-17, -3, -3, 3,
	-17, -3, -3, 3,
	-17, -3, -3, 3,
	-17, -3, -3, 3,
	-17, -3, -3, 3,
	-17, -3, -3, 3
}; // ds:0x6198
static const signed short g_gfxtab_shotbolt_oy[6][4] = {
	-30, -20, -20, -20,
	-30, -20, -20, -20,
	-30, -20, -20, -20,
	-30, -20, -20, -20,
	-30, -20, -20, -20,
	-30, -20, -20, -20
}; // ds:0x61c8
static signed short g_gfxtab_shotbolt_nvfno[6] = { 0, 4, 8, 12, 16, 20 }; // ds:0x61f8
static signed short g_gfxtab_shotbolt_height[6] = { 15, 15, 15, 5, 5, 8 }; // ds:0x6204
static signed short g_gfxtab_shotbolt_width[6] = { 25, 25, 25, 9, 9, 16 }; // ds:0x6210
static signed char g_anitab_shotbolt_data[24][7] = {

	{-2,  1,  0,  0, 0, 0, -1},
	{-2,  0, -1,  1, 0, 0, -1},
	{-2, -1,  0,  2, 0, 0, -1},
	{-2,  0,  1,  3, 0, 0, -1},

	{-2,  1,  0,  4, 0, 0, -1},
	{-2,  0, -1,  5, 0, 0, -1},
	{-2, -1,  0,  6, 0, 0, -1},
	{-2,  0,  1,  7, 0, 0, -1},

	{-2,  1,  0,  8, 0, 0, -1},
	{-2,  0, -1,  8, 0, 0, -1},
	{-2, -1,  0,  8, 0, 0, -1},
	{-2,  0,  1,  8, 0, 0, -1},

	{-2,  1,  0, 12, 0, 0, -1},
	{-2,  0, -1, 13, 0, 0, -1},
	{-2, -1,  0, 14, 0, 0, -1},
	{-2,  0,  1, 15, 0, 0, -1},

	{-2,  1,  0, 16, 0, 0, -1},
	{-2,  0, -1, 17, 0, 0, -1},
	{-2, -1,  0, 18, 0, 0, -1},
	{-2,  0,  1, 19, 0, 0, -1},

	{-2,  1,  0, 20, 0, 0, -1},
	{-2,  0, -1, 21, 0, 0, -1},
	{-2, -1,  0, 22, 0, 0, -1},
	{-2,  0,  1, 23, 0, 0, -1}
}; // ds:0x621c, arrays, each terminated by -1
static signed char *g_anitab_shotbolt_index2[6][4] = {
	{g_anitab_shotbolt_data[0],  g_anitab_shotbolt_data[1],  g_anitab_shotbolt_data[2],  g_anitab_shotbolt_data[3]},
	{g_anitab_shotbolt_data[4],  g_anitab_shotbolt_data[5],  g_anitab_shotbolt_data[6],  g_anitab_shotbolt_data[7]},
	{g_anitab_shotbolt_data[8],  g_anitab_shotbolt_data[9],  g_anitab_shotbolt_data[10], g_anitab_shotbolt_data[11]},
	{g_anitab_shotbolt_data[12], g_anitab_shotbolt_data[13], g_anitab_shotbolt_data[14], g_anitab_shotbolt_data[15]},
	{g_anitab_shotbolt_data[16], g_anitab_shotbolt_data[17], g_anitab_shotbolt_data[18], g_anitab_shotbolt_data[19]},
	{g_anitab_shotbolt_data[20], g_anitab_shotbolt_data[21], g_anitab_shotbolt_data[22], g_anitab_shotbolt_data[23]}
}; // ds:0x62c4, 6 long arrays of length 4 (viewdir); Bit8u*
static signed char **g_anitab_shotbolt_index[6] = {
	g_anitab_shotbolt_index2[0],
	g_anitab_shotbolt_index2[1],
	g_anitab_shotbolt_index2[2],
	g_anitab_shotbolt_index2[3],
	g_anitab_shotbolt_index2[4],
	g_anitab_shotbolt_index2[5]
}; // ds:0x6324; Bit8u*

static signed char g_anitab_spell_nvfno[2] = { 0x09, 0x1b }; // ds:0x633c
static signed short g_anitab_spell_ox[2] = { 0, 0 }; // ds:0x633e
static signed short g_anitab_spell_oy[2] = { 0, 0 }; // ds:0x6342
static signed char g_anitab_spell_data[2][16] = {
	{  9, 0, 0, 10, 0, 0, 11, 0, 0, 10, 0, 0,  9, 0, 0, -1},
	{ 27, 0, 0, 28, 0, 0, 29, 0, 0, 28, 0, 0, 27, 0, 0, -1},
}; // ds:0x6346, 2 arrays, each terminated by -1
static signed char* g_anitab_spell_index[2] = {
	(signed char*)&g_anitab_spell_data[0],
	(signed char*)&g_anitab_spell_data[1],
}; // ds:0x6366; Bit8u*


void seg045_0000(signed short fighter_id, signed short type, signed short a3)
{
	signed short obj_x;
	signed short obj_y;
	struct nvf_desc nvf;

	FIG_search_obj_on_cb(fighter_id, &obj_x, &obj_y);

	g_fig_list_elem.figure = 0;

	g_fig_list_elem.nvf_no = g_gfxtab_shotbolt_nvfno[type];

	if (type != 2) {
		g_fig_list_elem.nvf_no = g_fig_list_elem.nvf_no + a3;
	}

	if (a3 == 0) {
		obj_x += 2;
	}

	g_fig_list_elem.cbx = (signed char)obj_x;
	g_fig_list_elem.cby = (signed char)obj_y;
	g_fig_list_elem.offsetx = g_gfxtab_shotbolt_ox[type][a3];
	g_fig_list_elem.offsety = g_gfxtab_shotbolt_oy[type][a3];
	g_fig_list_elem.height = g_gfxtab_shotbolt_height[type];
	g_fig_list_elem.width = g_gfxtab_shotbolt_width[type];

	g_fig_list_elem.x1 = 0;
	g_fig_list_elem.y1 = 0;
	g_fig_list_elem.x2 = g_gfxtab_shotbolt_width[type] - 1;
	g_fig_list_elem.y2 = g_gfxtab_shotbolt_height[type] - 1;

	g_fig_list_elem.is_enemy = 0;
	g_fig_list_elem.reload = 0;
	g_fig_list_elem.wsheet = -1;
	g_fig_list_elem.sheet = -1;
	g_fig_list_elem.gfxbuf = g_fig_shot_bolt_buf;
	g_fig_list_elem.z = 100;
	g_fig_list_elem.visible = 0;
	g_fig_list_elem.twofielded = -1;

	nvf.dst = g_fig_shot_bolt_buf;
	nvf.src = g_spellobj_nvf_buf;
	nvf.no = g_fig_list_elem.nvf_no;
	nvf.type = 0;
	nvf.width = &obj_x;
	nvf.height = &obj_x;

	process_nvf(&nvf);

	g_fig_shot_bolt_id = FIG_add_to_list(-1);
}

void FIG_remove_smth(void)
{
	FIG_remove_from_list(g_fig_shot_bolt_id, 0);
	g_fig_shot_bolt_id = -1;
}

/**
 * \brief   copies chunks of 3 bytes
 *
 * \param   dst         pointer to destination
 * \param   src         pointer to sources
 * \param   term        termination sign
 */
signed short FIG_copy_it(Bit8s *dst, Bit8s *src, signed char term)
{
	signed short i;

	for (i = 0; *src != term; i += 3) {

		*dst = *src;
		src++;
		dst++;

		*dst = *src;
		src++;
		dst++;

		*dst = *src;
		src++;
		dst++;
	}

	return i;
}

signed short seg045_01a0(signed short a1, signed short a2, signed short fighter_id1, signed short fighter_id2, signed short a5)
{
	signed short i;
	Bit8s *ptr;
	signed short id1_x;
	signed short id1_y;
	signed short id2_x;
	signed short id2_y;
	signed short beeline;

	FIG_search_obj_on_cb(fighter_id2, &id2_x, &id2_y);
	FIG_search_obj_on_cb(fighter_id1, &id1_x, &id1_y);

	beeline = calc_beeline(id1_x, id1_y, id2_x, id2_y);

	if (beeline <= 1) {
		return 0;
	}

	ptr = &g_fig_anisheets[a1][1];
	g_fig_anisheets[a1][0] = 0;
	g_fig_anisheets[a1][242] = 0;

	for (i = 0; beeline - 1 > i; i++) {
		ptr += FIG_copy_it(ptr, g_anitab_shotbolt_index[a2][a5], -1);
	}
	*ptr = -1;

	seg045_0000(fighter_id1, a2, a5);

	return 1;
}

struct dummy2 {
	signed char a[2];
};

struct dummy4 {
	signed short a[2];
};

void seg045_0273(signed short x, signed short y, signed short spell_ani_id)
{
	signed short height;
	signed short width;

	/* TODO: some kind of initialized structure */
	struct dummy2 a = *(struct dummy2*)&g_anitab_spell_nvfno;
	struct dummy4 b = *(struct dummy4*)&g_anitab_spell_ox;
	struct dummy4 c = *(struct dummy4*)&g_anitab_spell_oy;

	struct nvf_desc nvf;

	nvf.dst = g_fig_spellgfx_buf;
	nvf.src = g_spellobj_nvf_buf;

	nvf.no = a.a[spell_ani_id - 1];
	nvf.type = 0;
	nvf.width = &width;
	nvf.height = &height;
	process_nvf(&nvf);

	g_fig_list_elem.figure = 0;
	g_fig_list_elem.nvf_no = a.a[spell_ani_id - 1];
	g_fig_list_elem.cbx = x;
	g_fig_list_elem.cby = y;

	g_fig_list_elem.offsetx = (unsigned char)b.a[spell_ani_id - 1];
	g_fig_list_elem.offsety = (unsigned char)c.a[spell_ani_id - 1];

	g_fig_list_elem.height = height;
	g_fig_list_elem.width = width;
	g_fig_list_elem.x1 = 0;
	g_fig_list_elem.y1 = 0;
	g_fig_list_elem.x2 = (signed char)width - 1;
	g_fig_list_elem.y2 = (signed char)height - 1;
	g_fig_list_elem.is_enemy = 0;
	g_fig_list_elem.reload = 0;
	g_fig_list_elem.wsheet = -1;
	g_fig_list_elem.sheet = -1;
	g_fig_list_elem.gfxbuf = g_fig_spellgfx_buf;
	g_fig_list_elem.z = 99;
	g_fig_list_elem.visible = 0;
	g_fig_list_elem.twofielded = -1;
	g_fig_spellgfx_id = FIG_add_to_list(-1);
}

void FIG_remove_smth2(void)
{
	FIG_remove_from_list(g_fig_spellgfx_id, 0);
	g_fig_spellgfx_id = -1;
}

void seg045_0394(const signed int a1, const struct struct_hero *hero, const signed int spell_ani_id)
{
	Bit8s *ptr;
	signed short x;
	signed short y;

	/* search the target on the chessboard */
	FIG_search_obj_on_cb(hero->enemy_id, &x, &y);

	ptr = &g_fig_anisheets[a1][1];

	g_fig_anisheets[a1][0] = 0;
	g_fig_anisheets[a1][242] = -1;

	/* copy the ani sequence and terminate it */
	ptr += FIG_copy_it(ptr, g_anitab_spell_index[spell_ani_id - 1], -1);
	*ptr = -1;

	seg045_0273(x, y, spell_ani_id);
}

void seg045_041b(signed short a1, struct enemy_sheet *enemy, signed short spell_ani_id)
{
	Bit8s *ptr;
	signed short x;
	signed short y;

	/* search the target on the chessboard */
	FIG_search_obj_on_cb(enemy->enemy_id, &x, &y);

	ptr = &g_fig_anisheets[a1][1];

	g_fig_anisheets[a1][0] = 0;
	g_fig_anisheets[a1][242] = -1;

	/* copy the ani sequence and terminate it */
	ptr += FIG_copy_it(ptr, g_anitab_spell_index[spell_ani_id - 1], -1);
	*ptr = -1;

	seg045_0273(x, y, spell_ani_id);
}

#if !defined(__BORLANDC__)
}
#endif
