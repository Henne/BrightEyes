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
	nvf.width = (Bit8u*)&obj_x;
	nvf.height = (Bit8u*)&obj_x;

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
	unsigned short height;
	unsigned short width;

	/* TODO: some kind of initialized structure */
	struct dummy2 a = *(struct dummy2*)&g_anitab_spell_nvfno;
	struct dummy4 b = *(struct dummy4*)&g_anitab_spell_ox;
	struct dummy4 c = *(struct dummy4*)&g_anitab_spell_oy;

	struct nvf_desc nvf;

	nvf.dst = g_fig_spellgfx_buf;
	nvf.src = g_spellobj_nvf_buf;

	nvf.no = a.a[spell_ani_id - 1];
	nvf.type = 0;
	nvf.width = (Bit8u*)&width;
	nvf.height = (Bit8u*)&height;
	process_nvf(&nvf);

#if !defined(__BORLANDC__)
	/* BE-fix */
	height = host_readws((Bit8u*)&height);
	width = host_readws((Bit8u*)&width);
#endif

	g_fig_list_elem.figure = 0;
	g_fig_list_elem.nvf_no = a.a[spell_ani_id - 1];
	g_fig_list_elem.cbx = (signed char)x;
	g_fig_list_elem.cby = (signed char)y;

	g_fig_list_elem.offsetx = (unsigned char)b.a[spell_ani_id - 1];
	g_fig_list_elem.offsety = (unsigned char)c.a[spell_ani_id - 1];

	g_fig_list_elem.height = ((unsigned char)height);
	g_fig_list_elem.width = ((unsigned char)width);
	g_fig_list_elem.x1 = 0;
	g_fig_list_elem.y1 = 0;
	g_fig_list_elem.x2 = (unsigned char)(width - 1);
	g_fig_list_elem.y2 = (unsigned char)(height - 1);
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

void seg045_0394(signed short a1, Bit8u *hero, signed short spell_ani_id)
{
	Bit8s *ptr;
	signed short x;
	signed short y;

	/* search the target on the chessboard */
	FIG_search_obj_on_cb(host_readbs(hero + HERO_ENEMY_ID), &x, &y);

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
