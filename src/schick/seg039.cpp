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
	Bit8u *ptr;
	signed int retval = -1;
	signed int weapon_id;

	/* get equipped item in the right hand of the hero and make a pointer to the entry of ITEMS.DAT */
	ptr = get_itemsdat(weapon_id = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id);


	if (item_weapon(ptr)) {
		/* is a weapon */

		/* MagicStaffs or Fightstaffs are spears, but no range weapons */
		if ((host_readb(ptr + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_SPEER) && (weapon_id != ITEM_MAGIC_WAND) && (weapon_id != ITEM_QUARTERSTAFF)) {
			/* TODO: according to original DSA2/3 rules, weapon type SPEER is a melee discipline... */

			retval = 5;

		} else if (host_readb(ptr + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_SCHUSSWAFFE) {

			retval = 3;

		} else if (host_readb(ptr + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_WURFWAFFE) {

			retval = 4;
		}
	}

	return retval;
}

/**
 * \brief   fills an enemies sheet from a template
 *
 * \param   sheet_no    the number of the sheet
 * \param   enemy_id    the ID of the enemy (MONSTER.DAT)
 * \param   round       the fight round the enemy appears
 *
 * \remark: special fight situations should be handled elsewhere
 */
void fill_enemy_sheet(signed short sheet_no, signed char enemy_id, signed char round)
{
	struct enemy_sheet *monster;
	struct enemy_sheet *sheet;
	signed short i;

	/* calculate the pointers */
	monster = &g_monster_dat_buf[enemy_id];
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
		sheet->attrib[i * 2] = sheet->attrib[i * 2 + 1] = dice_template(host_readw((Bit8u*)monster + i * 2 + MONSTER_ATTRIB));
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
	sheet->mr = (signed char)dice_template(host_readw((Bit8u*)monster + MONSTER_MR));

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
	sheet->mag_id = monster->mag_id;

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
unsigned short place_obj_on_cb(signed short x, signed short y, signed short object, signed char type, signed char dir)
{

	signed short i;

	/* check if an object is already on that field
		check if the object no is valid */
	if ((get_cb_val(x, y) > 0) || (object < 0)) {
		return 0;
	}

	/* check if the object is decoration */
	if (object >= 50) {
		if (type == 57 || type == 56 || type == 62) {

			FIG_set_cb_field(y + 1, x, object);
			FIG_set_cb_field(y + 1, x - 1, object);
			FIG_set_cb_field(y, x - 1, object);

		} else if (type == 9) {

			FIG_set_cb_field(y, x + 1, object);
			FIG_set_cb_field(y - 1, x, object);

		} else if (type == 43 || type == 44 || type == 48 ||
				type == 49 || type == 50 || type == 51 ||
				type == 52 || type == 53 || type == 54 ||
				type == 55) {

			FIG_set_cb_field(y + 1, x, object);

		} else if (type == 60) {

			for (i = 0; i < 7; i++)
				FIG_set_cb_field(y + i, x, object);

		} else if (type == 61) {

			for (i = 0; i < 7; i++)
				FIG_set_cb_field(y, x + i, object);
		}

	} else {
		/* if object is an enemy and needs 2 fields */
		if (object >= 10 && is_in_byte_array(type, (Bit8u*)g_two_fielded_sprite_id))
		{

			/* check if field is empty */
			if ((get_cb_val(x + g_gfxtab_twofielded_extra_cb[dir].x, y + g_gfxtab_twofielded_extra_cb[dir].y)) ||
				(y + g_gfxtab_twofielded_extra_cb[dir].y < 0) ||
				(y + g_gfxtab_twofielded_extra_cb[dir].y > 23) ||
				(x + g_gfxtab_twofielded_extra_cb[dir].x < 0 ||
				(x + g_gfxtab_twofielded_extra_cb[dir].x > 23)))
			{
				return 0;
			}

			FIG_set_cb_field(y + g_gfxtab_twofielded_extra_cb[dir].y, x + g_gfxtab_twofielded_extra_cb[dir].x, object + 20);
		}
	}

	/* set the object to the chessboard */
	FIG_set_cb_field(y, x, object);

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
	struct nvf_desc nvf;
	signed short l1;

	g_fig_list_elem.figure = g_gfxtab_figures_main[enemy->gfx_id][0];
	g_fig_list_elem.nvf_no = enemy->viewdir;
	g_fig_list_elem.cbx = (signed char)x;
	g_fig_list_elem.cby = (signed char)y;

	g_fig_list_elem.offsetx = g_gfxtab_offsets_main[enemy->gfx_id][enemy->viewdir].x;
	g_fig_list_elem.offsety = g_gfxtab_offsets_main[enemy->gfx_id][enemy->viewdir].y;

	if (is_in_byte_array(enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {

		/* sprite uses two fields */
		g_fig_list_elem.x1 = g_gfxtab_twofielded_x1[enemy->viewdir];
		g_fig_list_elem.x2 = g_gfxtab_twofielded_x2[enemy->viewdir];

		/* TODO: b = ++a; */
		g_fig_list_elem.twofielded = g_fig_twofielded_count = g_fig_twofielded_count + 1;
	} else {
		/* sprite uses one field */
		g_fig_list_elem.x1 = 0;
		g_fig_list_elem.x2 = 0x1f;
		g_fig_list_elem.twofielded = -1;
	}

	g_fig_list_elem.y1 = 0;
	g_fig_list_elem.y2 = 0x27;
	g_fig_list_elem.height = 0x28;
	g_fig_list_elem.width = 0x20;
	g_fig_list_elem.is_enemy = 1;
	g_fig_list_elem.sprite_no = enemy->gfx_id;
	g_fig_list_elem.reload = -1;
	g_fig_list_elem.wsheet = -1;
	g_fig_list_elem.sheet = -1;
	g_fig_list_elem.gfxbuf = g_fightobj_buf_seek_ptr; /* ->prev */
	g_fig_list_elem.obj_id = 0; /* ->next */

	g_fightobj_buf_seek_ptr += 0x508;

	g_fightobj_buf_freespace -= 0x508;
	g_fig_list_elem.z = 0x63;

	/* check presence in the first round */
	g_fig_list_elem.visible = (enemy->round_appear == 0 ? 1 : 0);

	if (is_in_byte_array(enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {

		nvf.src = (Bit8u*)load_fight_figs(g_fig_list_elem.figure);
		nvf.dst = g_fig_list_elem.gfxbuf;
		nvf.no = g_fig_list_elem.nvf_no;
		nvf.type = 0;
		nvf.width = (Bit8u*)&l1;
		nvf.height = (Bit8u*)&l1;
		process_nvf(&nvf);
		g_fig_list_elem.reload = 0;
	}

	enemy->fighter_id = FIG_add_to_list(-1);

	if (is_in_byte_array(enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {

		/* create fighter entry for the tail of a two-fielded enemy */

		g_fig_list_elem.cbx = x + g_gfxtab_twofielded_extra_cb[enemy->viewdir].x;
		g_fig_list_elem.cby = y + g_gfxtab_twofielded_extra_cb[enemy->viewdir].y;

		g_fig_list_elem.offsetx += g_gfxtab_twofielded_extra_ox[enemy->viewdir];
		g_fig_list_elem.offsety += g_gfxtab_twofielded_extra_oy[enemy->viewdir];
		g_fig_list_elem.x1 = g_gfxtab_twofielded_extra_x1[enemy->viewdir];
		g_fig_list_elem.x2 = g_gfxtab_twofielded_extra_x2[enemy->viewdir];
		g_fig_list_elem.y1 = 0;
		g_fig_list_elem.y2 = 0x27;
		g_fig_list_elem.is_enemy = 1;
		g_fig_list_elem.z = 10;
		g_fig_list_elem.twofielded = g_fig_twofielded_count + 20;
		g_fig_twofielded_table[g_fig_twofielded_count] = FIG_add_to_list(-1);
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

			place_obj_on_cb(x, y, i + 10, g_enemy_sheets[i].gfx_id,	g_current_fight->monsters[i].viewdir);
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

		if (((struct struct_hero*)get_hero(l_si))->fighter_id != -1) {

			FIG_remove_from_list(((struct struct_hero*)get_hero(l_si))->fighter_id, 0);

			((struct struct_hero*)get_hero(l_si))->fighter_id = -1;
		}
	}

	for (l_si = 0; l_si <= 6; l_si++) {

		hero = (struct struct_hero*)get_hero(l_si);

		if (hero->typus == HERO_TYPE_NONE)
			continue;

		/* check group */
		if (hero->group_no != gs_current_group)
			continue;

		hero->action_id = FIG_ACTION_WAIT;
		hero->enemy_id = 0;

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

			if (!hero_dead((Bit8u*)hero))
				or_ptr_bs((Bit8u*)hero + HERO_FLAGS1, 2); /* set 'sleep' flag */
		}

		place_obj_on_cb(cb_x, cb_y, l_si + 1, hero->typus, hero->viewdir);

		l_di = FIG_get_range_weapon_type(hero);

		if (l_di != -1) {
			g_fig_list_elem.nvf_no = g_nvftab_figures_rangeweapon[hero->sprite_no - 1][l_di][hero->viewdir];
		} else {
			g_fig_list_elem.nvf_no = hero->viewdir;
		}

		g_fig_list_elem.figure = g_gfxtab_figures_main[hero->sprite_no][0];
		g_fig_list_elem.cbx = (signed char)cb_x;
		g_fig_list_elem.cby = (signed char)cb_y;
		g_fig_list_elem.offsetx = 0;
		g_fig_list_elem.offsety = 0;

		if (hero_dead((Bit8u*)hero)) {

			/* hero is dead */
			g_fig_list_elem.nvf_no = g_nvftab_figures_dead[hero->sprite_no];
			g_fig_list_elem.offsetx = g_gfxtab_offsets_main[hero->sprite_no][4].x;
			g_fig_list_elem.offsety = g_gfxtab_offsets_main[hero->sprite_no][4].y;

		} else if (hero_asleep((Bit8u*)hero) || hero_unconscious((Bit8u*)hero)) {

			/* hero is asleep or unconscious */
			g_fig_list_elem.nvf_no = g_nvftab_figures_unconscious[hero->sprite_no] + hero->viewdir;
			g_fig_list_elem.offsetx = g_gfxtab_offsets_unconscious[hero->sprite_no][hero->viewdir].x;
			g_fig_list_elem.offsety = g_gfxtab_offsets_unconscious[hero->sprite_no][hero->viewdir].y;
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

		g_fig_list_elem.sprite_no = hero->sprite_no;
		g_fig_list_elem.reload = -1;
		g_fig_list_elem.wsheet = -1;
		g_fig_list_elem.sheet = -1;
		g_fig_list_elem.gfxbuf = g_fightobj_buf_seek_ptr;
		g_fig_list_elem.obj_id = 0;
		g_fightobj_buf_seek_ptr += 0x508;
		g_fightobj_buf_freespace -= 0x508L;
		g_fig_list_elem.z = 99;
		g_fig_list_elem.visible = 1;
		g_fig_list_elem.twofielded = -1;

		((struct struct_hero*)get_hero(l_si))->fighter_id = FIG_add_to_list(-1);
	}
}

#if !defined(__BORLANDC__)
}
#endif
