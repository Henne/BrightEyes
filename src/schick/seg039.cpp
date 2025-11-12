/*
 *	Rewrite of DSA1 v3.02_de functions of seg039 (fight)
 *	Functions rewritten: 7/7 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg039.cpp
 */
#include <stdlib.h>
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg027.h"
#include "seg032.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif


static struct struct_point g_gfxtab_double_size_extra_cb[4] = {
	{ -1,  0 }, {  0,  1 }, {  1,  0 }, {  0, -1 } }; // ds:0x6018, ; { {-1,0}, , , {0,-1} }
signed char g_gfxtab_double_size_extra_ox[4] = { 10, -10, -10, 10 }; // ds:0x6028, { 10,-10,-10,10 }
signed char g_gfxtab_double_size_extra_oy[4] = { 5, 5, -5, -5 }; // ds:0x602c, { 10,-10,-5,-5 }
signed char g_gfxtab_double_size_x1[4] = { 16, 0, 0, 16 }; // ds:0x6030
signed char g_gfxtab_double_size_x2[4] = { 31, 15, 15, 31 }; // ds:0x6034
signed char g_gfxtab_double_size_extra_x1[4] = { 0, 16, 16, 0 }; // ds:0x6038
signed char g_gfxtab_double_size_extra_x2[4] = { 15, 31, 31, 15 }; // ds:0x603c


/**
 * \brief   calculates the beeline
 *
 *          This function calculates the between two points on the chessboard.
 *          It is also used as a heuristic.
 *
 * \param   x1          x-coordinate of the first point
 * \param   y1          y-coordinate of the first point
 * \param   x2          x-coordinate of the second point
 * \param   y2          y-coordinate of the seconn point
 * \return              beeline between the two points
 */
signed short calc_beeline(signed short x1, signed short y1, signed short x2, signed short y2)
{
	return __abs__(x1 - x2) + __abs__(y1 - y2);
}


/**
 * \brief   get the type of the range weapon of a hero
 *
 * \param   hero        pointer to hero
 * \return              range weapon type {-1, 3, 4, 5}: -1 = none, 3 = shooting, 4 = throwing, 5 = weapon of type spear, but not magic wand or quarterstaff
 */
signed int FIG_get_range_weapon_type(struct struct_hero *hero)
{
	struct item_stats *item_p;
	signed int retval = -1;
	signed int weapon_id;

	/* get equipped item in the right hand of the hero and make a pointer to the entry of ITEMS.DAT */
	item_p = &g_itemsdat[weapon_id = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id];

	if (item_p->flags.weapon) {
		/* is a weapon */

		/* MagicStaffs or Fightstaffs are spears, but no range weapons */
		if ((item_p->subtype == WEAPON_TYPE_SPEER) && (weapon_id != ITEM_ZAUBERSTAB) && (weapon_id != ITEM_KAMPFSTAB)) {
			/* TODO: according to original DSA2/3 rules, weapon type SPEER is a melee discipline... */

			retval = 5;

		} else if (item_p->subtype == WEAPON_TYPE_SCHUSSWAFFE) {

			retval = 3;

		} else if (item_p->subtype == WEAPON_TYPE_WURFWAFFE) {

			retval = 4;
		}
	}

	return retval;
}

/**
 * \brief   fills an enemies sheet from a template
 *
 * \param   sheet_no    the number of the sheet
 * \param   target_object_id    the ID of the enemy (MONSTER.DAT)
 * \param   round       the fight round the enemy appears
 *
 * \remark: special fight situations should be handled elsewhere
 */
void fill_enemy_sheet(signed short sheet_no, signed char target_object_id, signed char round)
{
	struct struct_monster *monster;
	struct enemy_sheet *sheet;
	signed short i;

	/* calculate the pointers */
	monster = &g_monster_dat_buf[target_object_id];
	sheet = &g_enemy_sheets[sheet_no];

	/* erease the sheet */
	memset(sheet, 0, sizeof(struct enemy_sheet));

	/* copy enemy id, gfx_id and RS to the sheet */
	sheet->mon_id = monster->mon_id;
	sheet->gfx_id = monster->gfx_id;
	sheet->rs = monster->rs;

	/* roll attributes  and save them to the sheet */
	for (i = 0; i < 7; i++) {

		/* UGLY: a = b = dice_template() */
		sheet->attrib[i * 2] = sheet->attrib[i * 2 + 1] = dice_template(monster->attrib[i]);
	}

	/* roll out LE and save it to the sheet */
	sheet->le_orig = dice_template(monster->le);

	/* Feature mod 1: avoid the a posteriori weakening of the enemies (5/6 LE) of the original game. */
#ifndef M302de_FEATURE_MOD
	/* recalculate LE = LE / 6 * 5; */
	sheet->le_orig = sheet->le_orig / 6 * 5;
#endif

	/* copy LE*/
	sheet->le = sheet->le_orig;

	/* roll out AE and save it to the sheet */
	sheet->ae_orig = sheet->ae = dice_template(monster->ae);

	/* roll out MR  and save it */
	sheet->mr = (signed char)dice_template(monster->mr);

	/* Terrible hack:
		if the current fight is FIGHTS_F084, set MR to 5 (Travel-Event 84),
		if the current fight is FIGHTS_F144 (final fight), and the enemy is no "Orkchampion" then set the 'tied' flag */
	if (g_current_fight_no == FIGHTS_F084) {

		sheet->mr = 5;

	} else if ((g_current_fight_no == FIGHTS_F144) && (sheet->mon_id != 0x48)) {

		/* set 'tied' flag */
		sheet->flags.tied = 1;
	}

	sheet->first_ap = monster->first_ap;
	sheet->attacks = monster->attacks;
	sheet->at = monster->at;
	sheet->pa = monster->pa;
	sheet->dam1 = monster->dam1;
	sheet->dam2 = monster->dam2;

	sheet->bp_orig = monster->bp;

	if (sheet->bp_orig > 10)
		sheet->bp_orig = 10;

	sheet->magic = monster->magic;
	sheet->spellbook_id = monster->spellbook_id;

	/* unset 'dead' flag */
	/* bogus this value is 0x00 or 0x20 */
	sheet->flags.dead = 0;

	sheet->fighter_id = -1;
	sheet->level = monster->level;
	sheet->size = monster->size;
	sheet->is_animal = monster->is_animal;
	sheet->round_appear = round;

	sheet->viewdir = g_current_fight->monsters[sheet_no].viewdir;

	sheet->shots = monster->shots;
	sheet->shot_dam = monster->shot_dam;
	sheet->throws = monster->throws;
	sheet->throw_dam = monster->throw_dam;
	sheet->le_flee = monster->le_flee;

	/* Another hack:
		If the current fight == FIGHTS_F126_08 (fleeing cultist) and the enemy is "Kultist", set the 'scared' flag */
	if ((g_current_fight_no == FIGHTS_F126_08) && (sheet->mon_id == 0x38)) {
		/* Kultist will flee */
		/* set 'scared' flag */
		sheet->flags.scared = 1;
	}
}

/**
 * \brief   places an object on the chessboard
 *
 * \param   x           X-Coordinate
 * \param   y           Y-Coordinate
 * \param   object      object ID
 * \param   type        typus for heroes, monster_id for enemies
 * \param   dir         looking direction
 * \return              1 if the placement was successful or 0 if not.
 */
signed int place_obj_on_cb(const signed int x, const signed int y, const signed int object, const signed int type, const signed char dir)
{
	signed int i;

	/* check if an object is already on that field
		check if the object no is valid */
	if ((get_cb_val(x, y) > 0) || (object < 0)) {
		return 0;
	}

	/* check if the object is decoration */
	if (object >= 50) {
		if ((signed char)type == 57 || (signed char)type == 56 || (signed char)type == 62) {

			FIG_set_cb_object(y + 1, x, object);
			FIG_set_cb_object(y + 1, x - 1, object);
			FIG_set_cb_object(y, x - 1, object);

		} else if ((signed char)type == 9) {

			FIG_set_cb_object(y, x + 1, object);
			FIG_set_cb_object(y - 1, x, object);

		} else if ((signed char)type == 43 || (signed char)type == 44 || (signed char)type == 48 ||
				(signed char)type == 49 || (signed char)type == 50 || (signed char)type == 51 ||
				(signed char)type == 52 || (signed char)type == 53 || (signed char)type == 54 ||
				(signed char)type == 55) {

			FIG_set_cb_object(y + 1, x, object);

		} else if ((signed char)type == 60) {

			for (i = 0; i < 7; i++)
				FIG_set_cb_object(y + i, x, object);

		} else if ((signed char)type == 61) {

			for (i = 0; i < 7; i++)
				FIG_set_cb_object(y, x + i, object);
		}

	} else {
		/* if object is an enemy and needs 2 fields */
		if (object >= 10 && is_in_byte_array(type, g_double_size_gfx_id_table))
		{

			/* check if field is empty */
			if ((get_cb_val(x + g_gfxtab_double_size_extra_cb[dir].x, y + g_gfxtab_double_size_extra_cb[dir].y)) ||
				(y + g_gfxtab_double_size_extra_cb[dir].y < 0) ||
				(y + g_gfxtab_double_size_extra_cb[dir].y > 23) ||
				(x + g_gfxtab_double_size_extra_cb[dir].x < 0 ||
				(x + g_gfxtab_double_size_extra_cb[dir].x > 23)))
			{
				return 0;
			}

			FIG_set_cb_object(y + g_gfxtab_double_size_extra_cb[dir].y, x + g_gfxtab_double_size_extra_cb[dir].x, object + 20);
		}
	}

	/* set the object to the chessboard */
	FIG_set_cb_object(y, x, object);

	return 1;
}

/**
 * \brief   load the sprites from monsters
 *
 * \param   enemy       pointer to a monster datasheet
 * \param   x           x-coordinate on the chessboard
 * \param   y           y-coordinate on the chessboard
 */
void FIG_load_enemy_sprites(struct enemy_sheet *enemy, signed short x, signed short y)
{
	struct nvf_extract_desc nvf;
	signed int l1;

	g_fig_list_elem.figure = g_gfxtab_figures_main[enemy->gfx_id][0];
	g_fig_list_elem.nvf_no = enemy->viewdir;
	g_fig_list_elem.cbx = (signed char)x;
	g_fig_list_elem.cby = (signed char)y;

	g_fig_list_elem.offsetx = g_gfxtab_offsets_main[enemy->gfx_id][enemy->viewdir].x;
	g_fig_list_elem.offsety = g_gfxtab_offsets_main[enemy->gfx_id][enemy->viewdir].y;

	if (is_in_byte_array(enemy->gfx_id, g_double_size_gfx_id_table)) {

		/* sprite uses two fields */
		g_fig_list_elem.x1 = g_gfxtab_double_size_x1[enemy->viewdir];
		g_fig_list_elem.x2 = g_gfxtab_double_size_x2[enemy->viewdir];

		/* TODO: b = ++a; */
		g_fig_list_elem.double_size = g_fig_double_size_count = g_fig_double_size_count + 1;
	} else {
		/* sprite uses one field */
		g_fig_list_elem.x1 = 0;
		g_fig_list_elem.x2 = 0x1f;
		g_fig_list_elem.double_size = -1;
	}

	g_fig_list_elem.y1 = 0;
	g_fig_list_elem.y2 = 0x27;
	g_fig_list_elem.height = 0x28;
	g_fig_list_elem.width = 0x20;
	g_fig_list_elem.is_enemy = 1;
	g_fig_list_elem.sprite_id = enemy->gfx_id;
	g_fig_list_elem.reload = -1;
	g_fig_list_elem.wsheet = -1;
	g_fig_list_elem.sheet = -1;
	g_fig_list_elem.gfxbuf = g_fightobj_buf_seek_ptr; /* ->prev */
	g_fig_list_elem.object_id = 0; /* ->next */

	g_fightobj_buf_seek_ptr += 0x508;

	g_fightobj_buf_freespace -= 0x508;
	g_fig_list_elem.z = 0x63;

	/* check presence in the first round */
	g_fig_list_elem.visible = (enemy->round_appear == 0 ? 1 : 0);

	if (is_in_byte_array(enemy->gfx_id, g_double_size_gfx_id_table)) {

		nvf.src = (uint8_t*)load_fight_figs(g_fig_list_elem.figure);
		nvf.dst = g_fig_list_elem.gfxbuf;
		nvf.image_num = g_fig_list_elem.nvf_no;
		nvf.compression_type = 0;
		nvf.width = &l1;
		nvf.height = &l1;
		process_nvf_extraction(&nvf);
		g_fig_list_elem.reload = 0;
	}

	enemy->fighter_id = FIG_add_to_list(-1);

	if (is_in_byte_array(enemy->gfx_id, g_double_size_gfx_id_table)) {

		/* create fighter entry for the tail of a double-size enemy */

		g_fig_list_elem.cbx = x + g_gfxtab_double_size_extra_cb[enemy->viewdir].x;
		g_fig_list_elem.cby = y + g_gfxtab_double_size_extra_cb[enemy->viewdir].y;

		g_fig_list_elem.offsetx += g_gfxtab_double_size_extra_ox[enemy->viewdir];
		g_fig_list_elem.offsety += g_gfxtab_double_size_extra_oy[enemy->viewdir];
		g_fig_list_elem.x1 = g_gfxtab_double_size_extra_x1[enemy->viewdir];
		g_fig_list_elem.x2 = g_gfxtab_double_size_extra_x2[enemy->viewdir];
		g_fig_list_elem.y1 = 0;
		g_fig_list_elem.y2 = 0x27;
		g_fig_list_elem.is_enemy = 1;
		g_fig_list_elem.z = 10;
		g_fig_list_elem.double_size = g_fig_double_size_count + 20;
		g_fig_double_size_fighter_id_table[g_fig_double_size_count] = FIG_add_to_list(-1);
	}
}

void FIG_init_enemies(void)
{
	signed short i;
	signed short x;
	signed short y;

	/* Cleanup the old enemy tables */
	for (i = 0; i < 20; i++) {

		if (g_enemy_sheets[i].fighter_id != -1) {

			FIG_remove_from_list(g_enemy_sheets[i].fighter_id, 0);

			g_enemy_sheets[i].fighter_id = -1;
		}

		g_enemy_sheets[i].flags.dead = 1;
	}

	g_nr_of_enemies = 0;

	/* Fill the tables with new values */
	for (i = 0; i < 20; i++) {

		if (g_current_fight->monsters[i].id) {

			fill_enemy_sheet(i, g_current_fight->monsters[i].id, g_current_fight->monsters[i].round_appear);

			g_nr_of_enemies++;
		}
	}

	/* place the enemies on the chessboard */
	for (i = 0; i < g_nr_of_enemies; i++) {

		x = g_current_fight->monsters[i].x;
		y = g_current_fight->monsters[i].y;

		/* place only the enemies from round 0 */
		if (!g_current_fight->monsters[i].round_appear) {
#if !defined(__BORLANDC__)
			place_obj_on_cb(x, y, i + 10, g_enemy_sheets[i].gfx_id,	g_current_fight->monsters[i].viewdir);
#else
			place_obj_on_cb(x, y, i + 10, (_AL = g_enemy_sheets[i].gfx_id, _AX), g_current_fight->monsters[i].viewdir);
#endif
		}

		/* load the sprites */
		FIG_load_enemy_sprites(&g_enemy_sheets[i], x, y);
	}
}

void FIG_init_heroes(void)
{
	struct struct_hero *hero;
	signed short cb_x;
	signed short cb_y;
	signed short l_si;
	signed short l_di; /* player char no */

	for (l_si = 0; l_si <= 6; l_si++) {

		if (get_hero(l_si)->fighter_id != -1) {

			FIG_remove_from_list(get_hero(l_si)->fighter_id, 0);

			get_hero(l_si)->fighter_id = -1;
		}
	}

	for (l_si = 0; l_si <= 6; l_si++) {

		hero = get_hero(l_si);

		if (hero->typus == HERO_TYPE_NONE)
			continue;

		/* check group */
		if (hero->group_id != gs_active_group_id)
			continue;

		hero->action_id = FIG_ACTION_WAIT;
		hero->target_object_id = 0;

		/* FINAL FIGHT */
		if (g_current_fight_no == FIGHTS_F144) {

			if (hero == gs_main_acting_hero) {

				cb_x = g_current_fight->heroes[0].x;
				cb_y = g_current_fight->heroes[0].y;

				hero->viewdir = g_current_fight->heroes[0].viewdir;

			} else {
				do {
					l_di = random_schick(6);

					cb_x = g_current_fight->heroes[l_di].x;
					cb_y = g_current_fight->heroes[l_di].y;
					hero->viewdir = g_current_fight->heroes[l_di].viewdir;

				} while (get_cb_val(cb_x, cb_y) != 0);
			}
		} else {

			cb_x = g_current_fight->heroes[l_si].x;
			cb_y = g_current_fight->heroes[l_si].y;
			hero->viewdir = g_current_fight->heroes[l_si].viewdir;
		}

		/* heroes sleep until they appear */
		if (g_current_fight->heroes[l_si].round_appear) {

			if (!hero->flags.dead) {
				hero->flags.asleep = 1;
			}
		}

#if !defined(__BORLANDC__)
		place_obj_on_cb(cb_x, cb_y, l_si + 1, hero->typus, hero->viewdir);
#else
		place_obj_on_cb(cb_x, cb_y, l_si + 1, (_AL = hero->typus, _AX), hero->viewdir);
#endif

		l_di = FIG_get_range_weapon_type(hero);

		if (l_di != -1) {
			g_fig_list_elem.nvf_no = g_nvftab_figures_rangeweapon[hero->sprite_id - 1][l_di][hero->viewdir];
		} else {
			g_fig_list_elem.nvf_no = hero->viewdir;
		}

		g_fig_list_elem.figure = g_gfxtab_figures_main[hero->sprite_id][0];
		g_fig_list_elem.cbx = (signed char)cb_x;
		g_fig_list_elem.cby = (signed char)cb_y;
		g_fig_list_elem.offsetx = 0;
		g_fig_list_elem.offsety = 0;

		if (hero->flags.dead) {

			/* hero is dead */
			g_fig_list_elem.nvf_no = g_nvftab_figures_dead[hero->sprite_id];
			g_fig_list_elem.offsetx = g_gfxtab_offsets_main[hero->sprite_id][4].x;
			g_fig_list_elem.offsety = g_gfxtab_offsets_main[hero->sprite_id][4].y;

		} else if (hero->flags.asleep || hero->flags.unconscious) {

			/* hero is asleep or unconscious */
			g_fig_list_elem.nvf_no = g_nvftab_figures_unconscious[hero->sprite_id] + hero->viewdir;
			g_fig_list_elem.offsetx = g_gfxtab_offsets_unconscious[hero->sprite_id][hero->viewdir].x;
			g_fig_list_elem.offsety = g_gfxtab_offsets_unconscious[hero->sprite_id][hero->viewdir].y;
		}


		g_fig_list_elem.height = 40;
		g_fig_list_elem.width = 32;
		g_fig_list_elem.x1 = 0;
		g_fig_list_elem.y1 = 0;
		g_fig_list_elem.x2 = 31;
		g_fig_list_elem.y2 = 39;

		/* in the next line, value 0 (hero) would make better sense.
		 * however, the apparently only read operation checks for ==1, so the value 2 does not make a difference */
		g_fig_list_elem.is_enemy = 2;

		g_fig_list_elem.sprite_id = hero->sprite_id;
		g_fig_list_elem.reload = -1;
		g_fig_list_elem.wsheet = -1;
		g_fig_list_elem.sheet = -1;
		g_fig_list_elem.gfxbuf = g_fightobj_buf_seek_ptr;
		g_fig_list_elem.object_id = 0;
		g_fightobj_buf_seek_ptr += 0x508;
		g_fightobj_buf_freespace -= 0x508L;
		g_fig_list_elem.z = 99;
		g_fig_list_elem.visible = 1;
		g_fig_list_elem.double_size = -1;

		get_hero(l_si)->fighter_id = FIG_add_to_list(-1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
