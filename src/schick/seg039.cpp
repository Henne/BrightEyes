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
signed short FIG_get_range_weapon_type(Bit8u *hero)
{
	Bit8u *ptr;
	signed short retval = -1;
	signed short weapon;

	/* get equipped item in the right hand of the hero and make a pointer to the entry of ITEMS.DAT */
	ptr = get_itemsdat(weapon = host_readw(hero + HERO_INVENTORY + HERO_INVENTORY_SLOT_RIGHT_HAND * SIZEOF_INVENTORY + INVENTORY_ITEM_ID));


	if (item_weapon(ptr)) {
		/* is a weapon */

		/* MagicStaffs or Fightstaffs are spears, but no range weapons */
		if (host_readb(ptr + ITEM_STATS_SUBTYPE) == WEAPON_TYPE_SPEER && weapon != ITEM_MAGIC_WAND && weapon != ITEM_QUARTERSTAFF) {
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

	Bit8u *monster;
	struct enemy_sheet *sheet;
	signed short i;

	/* calculate the pointers */
	monster = g_monster_dat_buf + enemy_id * SIZEOF_MONSTER;
	sheet = &g_enemy_sheets[sheet_no];

	/* erease the sheet */
	memset(sheet, 0, sizeof(struct enemy_sheet));

	/* copy enemy id, gfx_id and RS to the sheet */
	sheet->mon_id = host_readb(monster + MONSTER_MON_ID);
	sheet->gfx_id = host_readb(monster + MONSTER_GFX_ID);
	sheet->rs = host_readb(monster + MONSTER_RS);

	/* roll attributes  and save them to the sheet */
	for (i = 0; i < 7; i++) {

		/* UGLY: a = b = dice_template() */
		sheet->attrib[i * 2] = sheet->attrib[i * 2 + 1] = dice_template(host_readw(monster + i * 2 + MONSTER_ATTRIB));
	}

	/* roll out LE and save it to the sheet */
	sheet->le_orig = dice_template(host_readw(monster + MONSTER_LE));

	/* Feature mod 1: avoid the a posteriori weakening of the enemies (5/6 LE) of the original game. */
#ifndef M302de_FEATURE_MOD
	/* recalculate LE = LE / 6 * 5; */
	sheet->le_orig = sheet->le_orig / 6 * 5;
#endif

	/* copy LE*/
	sheet->le = sheet->le_orig;

	/* roll out AE and save it to the sheet */
	sheet->ae_orig = sheet->ae = dice_template(host_readw(monster + MONSTER_AE));

	/* roll out MR  and save it */
	sheet->mr = (signed char)dice_template(host_readw(monster + MONSTER_MR));

	/* Terrible hack:
		if the current fight is FIGHTS_F084, set MR to 5 (Travel-Event 84),
		if the current fight is FIGHTS_F144 (final fight), and the enemy is no "Orkchampion" then set the 'tied' flag */
	if (g_current_fight_no == FIGHTS_F084) {

		sheet->mr = 5;

	} else if ((g_current_fight_no == FIGHTS_F144) && (sheet->mon_id != 0x48)) {

		/* set 'tied' flag */
		sheet->flags1.tied = 1;
	}

	sheet->first_ap = host_readb(monster + MONSTER_FIRSTAP);
	sheet->attacks = host_readb(monster + MONSTER_ATTACKS);
	sheet->at = host_readb(monster + MONSTER_AT);
	sheet->pa = host_readb(monster + MONSTER_PA);
	sheet->dam1 = host_readw(monster + MONSTER_DAM1);
	sheet->dam2 = host_readw(monster + MONSTER_DAM2);

	sheet->bp_orig = host_readb(monster + MONSTER_BP);

	if (sheet->bp_orig > 10)
		sheet->bp_orig = 10;

	sheet->magic = host_readb(monster + MONSTER_MAGIC);
	sheet->mag_id = host_readb(monster + MONSTER_MAG_ID);

	/* unset 'dead' flag */
	/* bogus this value is 0x00 or 0x20 */
	sheet->flags1.dead = 0;

	sheet->fighter_id = -1;
	sheet->level = host_readb(monster + MONSTER_LEVEL);
	sheet->size = host_readb(monster + MONSTER_SIZE);
	sheet->is_animal = host_readb(monster + MONSTER_IS_ANIMAL);
	sheet->round_appear = round;

	sheet->viewdir = host_readb(g_current_fight + sheet_no * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_VIEWDIR);

	sheet->shots = host_readb(monster + MONSTER_SHOTS);
	sheet->shot_dam = host_readw(monster + MONSTER_SHOT_DAM);
	sheet->throws = host_readb(monster + MONSTER_THROWS);
	sheet->throw_dam = host_readw(monster + MONSTER_THROW_DAM);
	sheet->le_flee = host_readb(monster + MONSTER_LE_FLEE);

	/* Another hack:
		If the current fight == FIGHTS_F126_08 (fleeing cultist) and the enemy is "Kultist", set the 'scared' flag */
	if ((g_current_fight_no == FIGHTS_F126_08) && (sheet->mon_id == 0x38)) {
		/* Kultist will flee */
		/* set 'scared' flag */
		sheet->flags2.scared = 1;
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
			if ((get_cb_val(x + ds_readws(GFXTAB_TWOFIELDED_EXTRA_CB + dir * 4), y + ds_readws((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + dir * 4)) != 0) ||
				(y + ds_readws((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + dir * 4) < 0) ||
				(y + ds_readws((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + dir * 4) > 23) ||
				(x + ds_readws(GFXTAB_TWOFIELDED_EXTRA_CB + dir * 4) < 0 ||
				(x + ds_readws(GFXTAB_TWOFIELDED_EXTRA_CB + dir * 4) > 23)))
			{
				return 0;
			}

			FIG_set_cb_field(y + ds_readws((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + dir * 4),
				x + ds_readws(GFXTAB_TWOFIELDED_EXTRA_CB + dir * 4),
				object + 20);
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

	g_fig_list_elem.figure = ds_readbs(GFXTAB_FIGURES_MAIN + enemy->gfx_id * 5);
	g_fig_list_elem.nvf_no = enemy->viewdir;
	g_fig_list_elem.cbx = (signed char)x;
	g_fig_list_elem.cby = (signed char)y;

	g_fig_list_elem.offsetx = ds_readb(GFXTAB_OFFSETS_MAIN + enemy->gfx_id * 10 + enemy->viewdir * 2);

	g_fig_list_elem.offsety = ds_readb((GFXTAB_OFFSETS_MAIN + 1) + enemy->gfx_id * 10 + enemy->viewdir * 2);

	if (is_in_byte_array(enemy->gfx_id, (Bit8u*)g_two_fielded_sprite_id)) {

		/* sprite uses two fields */
		g_fig_list_elem.x1 = ds_readbs(GFXTAB_TWOFIELDED_X1 + enemy->viewdir);
		g_fig_list_elem.x2 = ds_readbs(GFXTAB_TWOFIELDED_X2 + enemy->viewdir);

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

		g_fig_list_elem.cbx = x + ds_readbs(GFXTAB_TWOFIELDED_EXTRA_CB + enemy->viewdir * 4);
		g_fig_list_elem.cby = y + ds_readbs((GFXTAB_TWOFIELDED_EXTRA_CB + 2) + enemy->viewdir * 4);

		g_fig_list_elem.offsetx += ds_readbs(GFXTAB_TWOFIELDED_EXTRA_OX + enemy->viewdir);
		g_fig_list_elem.offsety += ds_readbs(GFXTAB_TWOFIELDED_EXTRA_OY + enemy->viewdir);
		g_fig_list_elem.x1 = ds_readb(GFXTAB_TWOFIELDED_EXTRA_X1 + enemy->viewdir);
		g_fig_list_elem.x2 = ds_readb(GFXTAB_TWOFIELDED_EXTRA_X2 + enemy->viewdir);
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

		g_enemy_sheets[i].flags1.dead = 1;
	}

	g_nr_of_enemies = 0;

	/* Fill the tables with new values */
	for (i = 0; i < 20; i++) {

		if (host_readb(g_current_fight + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_ID) != 0) {

			fill_enemy_sheet(i,
				host_readb(g_current_fight + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_ID),
				host_readb(g_current_fight + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_ROUND_APPEAR));

			g_nr_of_enemies++;
		}
	}

	/* place the enemies on the chessboard */
	for (i = 0; i < g_nr_of_enemies; i++) {

		x = host_readbs(g_current_fight + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_X);
		y = host_readbs(g_current_fight + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_Y);


		/* place only the enemies from round 0 */
		if (!host_readbs(g_current_fight + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_ROUND_APPEAR)) {

			place_obj_on_cb(x, y, i + 10, g_enemy_sheets[i].gfx_id,
				host_readb(g_current_fight + i * SIZEOF_FIGHT_MONSTER + FIGHT_MONSTERS_VIEWDIR));
		}

		/* load the sprites */
		FIG_load_enemy_sprites(&g_enemy_sheets[i], x, y);
	}
}

void FIG_init_heroes(void)
{
	Bit8u *hero;
	signed short cb_x;
	signed short cb_y;
	signed short l_si;
	signed short l_di; /* player char no */

	for (l_si = 0; l_si <= 6; l_si++) {

		if (host_readbs(get_hero(l_si) + HERO_FIGHTER_ID) != -1) {

			FIG_remove_from_list(host_readb(get_hero(l_si) + HERO_FIGHTER_ID), 0);
			host_writeb(get_hero(l_si) + HERO_FIGHTER_ID, 0xff);
		}
	}

	for (l_si = 0; l_si <= 6; l_si++) {
		hero = get_hero(l_si);

		if (host_readb(hero + HERO_TYPE) == HERO_TYPE_NONE)
			continue;
		/* check group */
		if (host_readb(hero + HERO_GROUP_NO) != gs_current_group)
			continue;

		host_writeb(hero + HERO_ACTION_ID, FIG_ACTION_WAIT);
		host_writeb(hero + HERO_ENEMY_ID, 0);

		/* FINAL FIGHT */
		if (g_current_fight_no == FIGHTS_F144) {

			if (hero == (Bit8u*)gs_main_acting_hero) {

				cb_x = host_readbs(g_current_fight + FIGHT_PLAYERS_X);
				cb_y = host_readbs(g_current_fight + FIGHT_PLAYERS_Y);
				host_writeb(hero + HERO_VIEWDIR, host_readb(g_current_fight + FIGHT_PLAYERS_VIEWDIR));

			} else {
				do {
					l_di = random_schick(6);

					cb_x = host_readbs(g_current_fight + l_di * SIZEOF_FIGHT_PLAYER + FIGHT_PLAYERS_X);
					cb_y = host_readbs(g_current_fight + l_di * SIZEOF_FIGHT_PLAYER + FIGHT_PLAYERS_Y);
					host_writeb(hero + HERO_VIEWDIR, host_readb(g_current_fight + l_di * SIZEOF_FIGHT_PLAYER + FIGHT_PLAYERS_VIEWDIR));

				} while (get_cb_val(cb_x, cb_y) != 0);
			}
		} else {
			cb_x = host_readbs(g_current_fight + FIGHT_PLAYERS_X + SIZEOF_FIGHT_PLAYER * l_si);
			cb_y = host_readbs(g_current_fight + FIGHT_PLAYERS_Y + SIZEOF_FIGHT_PLAYER * l_si);

			/* Direction */
			host_writeb(hero + HERO_VIEWDIR, host_readb(g_current_fight + FIGHT_PLAYERS_VIEWDIR + SIZEOF_FIGHT_PLAYER * l_si));
		}

		/* heroes sleep until they appear */
		if (host_readb(g_current_fight + l_si * SIZEOF_FIGHT_PLAYER + FIGHT_PLAYERS_ROUND_APPEAR) != 0) {

			if (!hero_dead(hero))
				or_ptr_bs(hero + HERO_FLAGS1, 2); /* set 'sleep' flag */
		}

		place_obj_on_cb(cb_x, cb_y, l_si + 1, host_readb(hero + HERO_TYPE), host_readb(hero + HERO_VIEWDIR));

		l_di = FIG_get_range_weapon_type(hero);

		if (l_di != -1) {
			g_fig_list_elem.nvf_no = g_nvftab_figures_rangeweapon[host_readbs(hero + HERO_SPRITE_NO) - 1][l_di][host_readbs(hero + HERO_VIEWDIR)];
		} else {
			g_fig_list_elem.nvf_no = host_readb(hero + HERO_VIEWDIR);
		}

		g_fig_list_elem.figure = ds_readbs(GFXTAB_FIGURES_MAIN + host_readbs(hero + HERO_SPRITE_NO) * 5);
		g_fig_list_elem.cbx = (signed char)cb_x;
		g_fig_list_elem.cby = (signed char)cb_y;
		g_fig_list_elem.offsetx = 0;
		g_fig_list_elem.offsety = 0;

		if (hero_dead(hero)) {
			/* hero is dead */
			g_fig_list_elem.nvf_no = g_nvftab_figures_dead[host_readbs(hero + HERO_SPRITE_NO)];
			g_fig_list_elem.offsetx = (
				ds_readb((GFXTAB_OFFSETS_MAIN + 8) + host_readbs(hero + HERO_SPRITE_NO) * 10));
			g_fig_list_elem.offsety = (
				ds_readb((GFXTAB_OFFSETS_MAIN + 9) + host_readbs(hero + HERO_SPRITE_NO) * 10));
		} else if (hero_asleep(hero) || hero_unconscious(hero)) {

			/* hero is asleep or unconscious */
			g_fig_list_elem.nvf_no = g_nvftab_figures_unconscious[host_readbs(hero + HERO_SPRITE_NO)] + host_readbs(hero + HERO_VIEWDIR);

			g_fig_list_elem.offsetx = (
				ds_readbs(GFXTAB_OFFSETS_UNCONSCIOUS + host_readbs(hero + HERO_SPRITE_NO) * 8 + host_readbs(hero + HERO_VIEWDIR) * 2));
			g_fig_list_elem.offsety = (
				ds_readbs(GFXTAB_OFFSETS_UNCONSCIOUS + 1 + host_readbs(hero + HERO_SPRITE_NO) * 8 + host_readbs(hero + HERO_VIEWDIR) * 2));
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

		g_fig_list_elem.sprite_no = host_readb(hero + HERO_SPRITE_NO);
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

		host_writeb(get_hero(l_si) + HERO_FIGHTER_ID, FIG_add_to_list(-1));
	}
}

#if !defined(__BORLANDC__)
}
#endif
