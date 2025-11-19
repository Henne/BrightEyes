/*
 *	Rewrite of DSA1 v3.02_de functions of seg044 (Fight ANImation: Figures)
 *	Functions rewritten: 6/6 (complete)
 *
 *	Borlandified and identical
 *	Compiler:	Borland C++ 3.1
 *	Call:		BCC.EXE -mlarge -O- -c -1 -Yo seg044.cpp
 */
#include <string.h>

#include "v302de.h"
#include "common.h"

#include "seg002.h"
#include "seg006.h"
#include "seg007.h"
#include "seg036.h"
#include "seg038.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

/**
 * \brief   copies an animation sequence
 *
 * \param   dst         where to copy the sequence
 * \param   ani         the number of the animation sequence
 * \param   type        type of animation (3 = WEAPANI.DAT else ANI.DAT)
 * \return              the number of copied bytes
 */
/* Borlandified and identical */
static signed int copy_ani_seq(int8_t *dst, const signed int ani_num, const signed int type)
{
	int8_t *p_datbuffer;
	int8_t *p_datitem;
	signed int ani_max_num;
	signed int i;
	signed char len;

	/* get pointer from ANI.DAT */
	p_datbuffer = (int8_t*)g_buffer_anidat;

	/* check if we must use WEAPANI.DAT */
	if (type == 3)
		p_datbuffer = (int8_t*)g_buffer_weapanidat;

	/* get number of animation sequences */
	ani_max_num = *(int16_t*)p_datbuffer;

	/* sanity check */
	if (ani_num < 0)
		return 0;

	if (ani_num > ani_max_num)
		return 0;

	/* set p_datitem to the first sequence */
	p_datitem = p_datbuffer;
	p_datitem += ani_max_num + 2;

	/* set length to the length of the first sequence */
	len = p_datbuffer[2];

	/* fast forward to the requestet sequence */
	for (i = 1; i <= ani_num; i++) {
		p_datitem += len;
		len = *(p_datbuffer + i + 2);
	}

	/* skip the first byte */
	p_datitem++;
	/* calc the length of the sequence */
	len -= 2;
	/* REMARK: the first and the last byte of the sequence are skipped */

	/* copy them */
	for (i = 0; i < len; i++)
		*dst++ = *p_datitem++;

	return len;
}

/**
 * \brief   TODO
 *
 * \param[in]  ani_num the number of the animation sequence
 * \return  the first byte of the sequence from ANI.DAT {0,1,2,3,4}
 */
/* Borlandified and identical */
static signed char get_seq_header(const signed int ani_num)
{
	int8_t *p_datbuffer;
	int8_t *p_datitem;
	int8_t length;
	signed int ani_max_num;
	signed int i;

	/* get pointer from ANI.DAT */
	p_datbuffer = (int8_t*)g_buffer_anidat;

	/* get number of ani seqences in ANI.DAT */
	ani_max_num = *(int16_t*)p_datbuffer;

	if (ani_num < 0) {
		return 0;
	}

	if (ani_num > ani_max_num) {
		return 0;
	}

	p_datitem = p_datbuffer;
	p_datitem += ani_max_num + 2;

	length = p_datbuffer[2];

	for (i = 1; i <= ani_num; i++) {
		/* set pointer to the start of the next sequence */
		p_datitem += length;
		/* set length to the length of the next sequence */
		length = *(p_datbuffer + i + 2);
	}

	return *p_datitem;
}

/**
 * \brief   prepares the animation sequence of a hero in fights
 *
 * \param   sheet_id          [0, 1]
 * \param   hero        pointer to hero
 * \param   weapon_type the type of weapon for the animation [-1, 5], 3,4,5 are range weapons
 * \param   action_type {FIG_ACTION_MELEE_ATTACK = 2, FIG_ACTION_RANGE_ATTACK = 15, FIG_ACTION_PARRY = 100, FIG_ACTION_UNKNOWN3 = 102, FIG_ACTION_UNKNOWN4 = 103}
 */
/* Borlandified and identical */
void FANI_prepare_fight_hero_ani(const signed int sheet_id, struct struct_hero *hero, const signed int weapon_type, const signed int f_action, const signed int object_id_attacker, const signed int object_id_target, const signed int a7)
{
	signed int l1;
	signed int attacker_x;
	signed int attacker_y;
	signed int target_x;
	signed int target_y;
	signed int dir;
	signed int l7;
	signed int l8;
	signed int l9;
	signed int i;
	int8_t *sheet_ptr1;
	int8_t *sheet_ptr2;
	signed int weapon_id;
	int16_t *ani_index_ptr;

	ani_index_ptr = g_gfx_ani_index[hero->sprite_id];
	weapon_id = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;

	if ((signed char)object_id_target) {

		FIG_search_obj_on_cb((signed char)object_id_target, &target_x, &target_y);
		FIG_search_obj_on_cb((signed char)object_id_attacker, &attacker_x, &attacker_y);

		if (attacker_x == target_x) {
			if (target_y < attacker_y) {
				dir = 1;
			} else {
				dir = 3;
			}
		} else {
			if (target_x < attacker_x) {
				dir = 2;
			} else {
				dir = 0;
			}
		}
	} else {
		dir = hero->viewdir;
	}

	if ((weapon_type == -1) || ((hero->typus == HERO_TYPE_MAGIER) && (weapon_id == ITEM_ZAUBERSTAB))) {

		l1 = (f_action == FIG_ACTION_MELEE_ATTACK) ? 45 :		/* melee attack */
			(f_action == FIG_ACTION_UNKNOWN3) ? 41 :		/* drink potion */
			(f_action == FIG_ACTION_UNKNOWN4) ? 53 :		/* cast spell */
			49;

	} else {
		l1 = (f_action == FIG_ACTION_MELEE_ATTACK) ?  21:		/* melee attack */
			(f_action == FIG_ACTION_UNKNOWN3) ? 41 :		/* drink potion */
			(f_action == FIG_ACTION_UNKNOWN4) ? 53 :		/* cast spell */
			(f_action != FIG_ACTION_RANGE_ATTACK) ? 25 :
			(weapon_type == 3) ? 33 :
			(weapon_type == 4) ? 57 :
			61;
	}

	l1 += dir;
	sheet_ptr1 = &g_fig_anisheets[sheet_id][1];
	sheet_ptr2 = &g_fig_anisheets[sheet_id + 4][1];

	g_fig_anisheets[sheet_id][0] = get_seq_header(ani_index_ptr[l1]);
	g_fig_anisheets[sheet_id][242] = hero->sprite_id;

	if (check_hero(hero) && (hero->viewdir != dir) &&

		((f_action == FIG_ACTION_MELEE_ATTACK) || (f_action == FIG_ACTION_RANGE_ATTACK) || (f_action == FIG_ACTION_UNKNOWN4) ||
			((f_action == FIG_ACTION_PARRY) && !g_fig_hero_has_parried[(signed char)object_id_attacker - 1]) ||
			((g_fig_critical_fail_backfire_2 != 0) && (a7 == 0)) ||
			((g_fig_critical_fail_backfire_1 != 0) && (a7 == 1))))
	{
			g_fig_anisheets[sheet_id][0] = 0;
			l8 = l7 = -1;
			l9 = hero->viewdir;
			l8 = l9;
			l9++;

			if (l9 == 4) {
				l9 = 0;
			}

			if (l9 != dir) {
				l7 = l9;
				l9++;
				if (l9 == 4) {
					l9 = 0;
				}

				if (l9 != dir) {
					l8 = hero->viewdir + 4;
					l7 = -1;
				}
			}

			hero->viewdir = dir;

			if (l7 == -1) {
				for (i = 0; i < 2; i++) {
					*sheet_ptr1++ = -5;
					*sheet_ptr1++ = 0;
					*sheet_ptr1++ = 0;
				}
			}

			sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[l8], 2);

			if (l7 != -1) {
				sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[l7], 2);
			}

			*sheet_ptr1++ = -4;
			*sheet_ptr1++ = get_seq_header(ani_index_ptr[l1]);
			*sheet_ptr1++ = 0;
	} else {
		for (i = 0; i < 5; i++) {
			*sheet_ptr1++ = -5;
			*sheet_ptr1++ = 0;
			*sheet_ptr1++ = 0;
		}
	}

	if ((check_hero(hero) && (f_action == FIG_ACTION_MELEE_ATTACK)) ||
		((f_action == FIG_ACTION_RANGE_ATTACK) || (f_action == FIG_ACTION_UNKNOWN3) || (f_action == FIG_ACTION_UNKNOWN4) ||
			((f_action == FIG_ACTION_PARRY) && !g_fig_hero_has_parried[(signed char)object_id_attacker - 1])))
	{
		sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[l1], 2);

		if ((weapon_type != -1) && (weapon_type < 3) &&
			(hero->typus != HERO_TYPE_MAGIER) &&
			(hero->typus != HERO_TYPE_DRUIDE))
		{
			for (i = 0; i < 5; i++) {
				*sheet_ptr2++ = -5;
				*sheet_ptr2++ = 0;
				*sheet_ptr2++ = 0;
			}

			sheet_ptr2 += copy_ani_seq(sheet_ptr2,
				*(int16_t*)((uint8_t*)g_weaponani_table + (g_weaponani_types[hero->sprite_id] * 48 + weapon_type * 16) +
				((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + hero->viewdir * 2), 3);
		}
	}

	if ((check_hero(hero) && g_fig_critical_fail_backfire_2 && !a7) || (g_fig_critical_fail_backfire_1 && (a7 == 1))) {

			sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[l1], 2);

			if ((weapon_type != -1) && (weapon_type < 3) &&	(hero->typus != HERO_TYPE_MAGIER) && (hero->typus != HERO_TYPE_DRUIDE))
			{
				sheet_ptr2 += copy_ani_seq(sheet_ptr2,
					*(int16_t*)((uint8_t*)g_weaponani_table + (g_weaponani_types[hero->sprite_id] * 48 + weapon_type * 16) +
					((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + hero->viewdir * 2), 3);
			}
	}

	if ((g_attacker_dead && !a7) || (g_defender_dead && (a7 == 1)))
	{
		*sheet_ptr1++ = -4;
		*sheet_ptr1++ = get_seq_header(ani_index_ptr[20]);
		*sheet_ptr1++ = 0;

		sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[20], 2);
	}

	if (check_hero(hero) ||	(g_attacker_dead && !a7) || (g_defender_dead && (a7 == 1)))
	{
		FIG_set_sheet(hero->fighter_id, (signed char)sheet_id);
		*sheet_ptr1 = -1;

		if ( (weapon_type != -1) && (weapon_type < 3) && (hero->typus != HERO_TYPE_MAGIER) && (hero->typus != HERO_TYPE_DRUIDE))
		{
			FIG_set_weapon_sheet(hero->fighter_id, sheet_id + 4);
			*sheet_ptr2 = -1;
		}
	}

	*sheet_ptr1 = -1;
	if (f_action == FIG_ACTION_PARRY) {
		g_fig_hero_has_parried[(signed char)object_id_attacker - 1] = 1;
	}
}

/**
 * \brief   prepares the animation sequence of a hero in fights
 */
/* Borlandified and identical */
void FANI_prepare_fight_enemy_ani(const signed int sheet_id, struct enemy_sheet *enemy, const signed int f_action, const signed int object_id_attacker, const signed int object_id_target, const signed int a7)
{
	signed int l1;
	signed int attacker_x;
	signed int attacker_y;
	signed int target_x;
	signed int target_y;
	signed int dir;
	signed int l7;
	signed int l8;
	signed int l9;
	signed int i;
	int8_t *sheet_ptr1;
	int8_t *sheet_ptr2;
	struct struct_fighter *fighter;			/* only user for two sprited figures */
	int16_t *ani_index_ptr;			/* read only */
	signed int weapon_type;

	/* initialize with bare hands */
	weapon_type = -1;

	/* every enemy with the gfx_id < 22 has a sword type weapon */
	if (enemy->gfx_id < 22) {
		weapon_type = 2;
	}

	/* This byte is unknown atm */
	if (enemy->weapon_broken) {
		weapon_type = -1;
	}

	/* read a pointer from an array with the gfx_id as offset, read only */
	ani_index_ptr = g_gfx_ani_index[enemy->gfx_id];

	/* find both actors on the chessboard */
	FIG_search_obj_on_cb((signed char)object_id_target, &target_x, &target_y);
	FIG_search_obj_on_cb((signed char)object_id_attacker, &attacker_x, &attacker_y);

	/* find out which direction the action will have */
	if (attacker_x == target_x) {
		if (target_y < attacker_y) {
			dir = 1;
		} else {
			dir = 3;
		}
	} else {
		if (target_x < attacker_x) {
			dir = 2;
		} else {
			dir = 0;
		}
	}



	/* melee attack */
	l1 = (f_action == FIG_ACTION_MELEE_ATTACK) ? 21 : 25;

	if ((enemy->gfx_id == 8) || (enemy->gfx_id == 9) || (enemy->gfx_id == 19) || (enemy->gfx_id == 20)) {

		weapon_type = -1;
		l1 = (f_action == FIG_ACTION_MELEE_ATTACK) ? 45 : 49;
	}

	if (f_action == FIG_ACTION_RANGE_ATTACK) {
		l1 = 33;
		weapon_type = -1;
	}

	l1 += dir;

	sheet_ptr1 = &g_fig_anisheets[sheet_id][1];
	sheet_ptr2 = &g_fig_anisheets[sheet_id + 4][1];


	g_fig_anisheets[sheet_id][0] = get_seq_header(ani_index_ptr[l1]);
	g_fig_anisheets[sheet_id][242] = enemy->gfx_id;

	/* first the enemy may turn */
	if ((enemy->viewdir != dir) &&
		(	((f_action == FIG_ACTION_MELEE_ATTACK) || (f_action == FIG_ACTION_RANGE_ATTACK) ||
			((f_action == FIG_ACTION_PARRY) && !g_fig_enemy_has_parried[(signed char)object_id_attacker])) ||
			(g_fig_critical_fail_backfire_2 && !a7) ||
			(g_fig_critical_fail_backfire_1 && (a7 == 1))))
		{

		g_fig_anisheets[sheet_id][0] = 0;

		/* find out the new direction */
		l8 = l7 = -1;

		/* try to turn right 90 degrees */
		l9 = enemy->viewdir;
		l8 = l9;
		l9++;

		if (l9 == 4) {
			l9 = 0;
		}

		if (l9 != dir) {
			l7 = l9;
			l9++;
			if (l9 == 4) {
				l9 = 0;
			}

			if (l9 != dir) {
				l8 = enemy->viewdir + 4;
				l7 = -1;
			}
		}


		/* set the new direction in enemy sheet */
		enemy->viewdir = dir;

		/* only if the turn is 90 degree */
		if (l7 == -1) {
			/* do not move for 2 frames */
			for (i = 0; i < 2; i++) {
				*sheet_ptr1++ = -5;
				*sheet_ptr1++ = 0;
				*sheet_ptr1++ = 0;
			}
		}

		sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[l8], 1);

		if (l7 != -1) {
			sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[l7], 1);
		}

		*sheet_ptr1++ = -4;
		*sheet_ptr1++ = get_seq_header(ani_index_ptr[l1]);
		*sheet_ptr1++ = 0;
	} else {
		/* do not move for 5 frames */
		for (i = 0; i < 5; i++) {
			*sheet_ptr1++ = -5;
			*sheet_ptr1++ = 0;
			*sheet_ptr1++ = 0;
		}
	}

	if ((f_action == FIG_ACTION_MELEE_ATTACK) || (f_action == FIG_ACTION_RANGE_ATTACK) ||
		((f_action == FIG_ACTION_PARRY) && !g_fig_enemy_has_parried[(signed char)object_id_attacker]))
	{
		sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[l1], 1);

		if (weapon_type != -1) {

			/* do not move for 5 frames */
			for (i = 0; i < 5; i++) {
				*sheet_ptr2++ = -5;
				*sheet_ptr2++ = 0;
				*sheet_ptr2++ = 0;
			}

			/* copy the weapon ani */
			sheet_ptr2 += copy_ani_seq(sheet_ptr2,
				*(int16_t*)((uint8_t*)g_weaponani_table + g_weaponani_types[enemy->gfx_id] * 48 + weapon_type * 16 + ((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + enemy->viewdir * 2), 3);
		}
	}

	if (((g_fig_critical_fail_backfire_2 != 0) && (a7 == 0)) ||
		((g_fig_critical_fail_backfire_1 != 0) && (a7 == 1))) {

			sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[l1], 1);

			if (weapon_type != -1) {

				/* copy the weapon ani */
				sheet_ptr2 += copy_ani_seq(sheet_ptr2,
					*(int16_t*)((uint8_t*)g_weaponani_table + g_weaponani_types[enemy->gfx_id] * 48 + weapon_type * 16 + ((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + enemy->viewdir * 2), 3);
			}
	}

	if ( ((g_attacker_dead != 0) && (a7 == 0)) ||
		((g_defender_dead != 0) && (a7 == 1)))
	{
		*sheet_ptr1++ = -4;
		*sheet_ptr1++ = get_seq_header(ani_index_ptr[20]);
		*sheet_ptr1++ = 0;

		sheet_ptr1 += copy_ani_seq(sheet_ptr1, ani_index_ptr[20], 1);
	}

	FIG_set_sheet(enemy->fighter_id, (signed char)sheet_id);

	/* terminate figure animation array */
	*sheet_ptr1 = -1;

	/* does this sprite need two fields */
	if (is_in_byte_array(enemy->gfx_id, g_double_size_gfx_id_table))	{

		memcpy(&g_fig_anisheets[sheet_id + 2], &g_fig_anisheets[sheet_id], 243);

		fighter = FIG_get_fighter(enemy->fighter_id);

		FIG_set_sheet(g_fig_double_size_fighter_id_table[fighter->double_size], sheet_id + 2);
	}

	if (weapon_type != -1) {

		FIG_set_weapon_sheet(enemy->fighter_id, sheet_id + 4);

		/* terminate weapon animation array */
		*sheet_ptr2 = -1;
	}

	if (f_action == FIG_ACTION_PARRY) {
		g_fig_enemy_has_parried[(signed char)object_id_attacker] = 1;
	}
}


/**
 *
 * \param[in] sheet_id  0 or 1
 * \param   hero        pointer to a hero
 * \param   max_range          99 or 4
 * \param   obj1
 * \param   obj2
 * \param   v5		-1 or a var
 * \param   v6          0 or 1
 */

/* Borlandified and identical */
void FANI_prepare_spell_hero(const signed int sheet_id, struct struct_hero *hero, const signed int max_range, const signed int obj1, const signed int obj2, const signed int v5, const signed int v6)
{
	signed int x_obj1;
	signed int y_obj1;
	signed int x_obj2;
	signed int y_obj2;
	signed int dir;
	signed int l2;
	signed int l3;
	int8_t *sheet_ptr;
	int16_t *ani_index_ptr;

	signed int l_di;

	/* get a pointer from an array where the Monster-ID serves as index */
	ani_index_ptr = g_gfx_ani_index[hero->sprite_id];

	FIG_search_obj_on_cb((signed char)obj2, &x_obj2, &y_obj2);
	FIG_search_obj_on_cb((signed char)obj1, &x_obj1, &y_obj1);

	if (x_obj1 == x_obj2) {
		if (y_obj2 < y_obj1)
			dir = 1;
		else
			dir = 3;
	} else {
		if (x_obj2 < x_obj1)
			dir = 2;
		else
			dir = 0;
	}

	if ((signed char)obj2 == (signed char)obj1)
		dir = hero->viewdir;


	l_di = (max_range == 4) ? ((v5 == 1) ? 37 : 29) : 16;

	l_di += (max_range == 4) ? dir : hero->viewdir;

	sheet_ptr = &g_fig_anisheets[sheet_id][1];

	g_fig_anisheets[sheet_id][0] = get_seq_header(ani_index_ptr[l_di]);

	g_fig_anisheets[sheet_id][242] = hero->sprite_id;

	if ((hero->viewdir != dir) && (max_range == 4)) {

		signed int viewdir;

		g_fig_anisheets[sheet_id][0] = 0;

		l3 = l2 = -1;
		viewdir = hero->viewdir;
		l3 = viewdir;
		viewdir++;
		if (viewdir == 4)
			viewdir = 0;

		if (viewdir != dir) {
			l2 = viewdir;
			viewdir++;
			if (viewdir == 4)
				viewdir = 0;

			if (viewdir != dir) {
				l3 = hero->viewdir + 4;
				l2 = -1;
			}
		}

		hero->viewdir = dir;
		sheet_ptr += copy_ani_seq(sheet_ptr, ani_index_ptr[l3], 2);

		if (l2 != -1) {
			sheet_ptr += copy_ani_seq(sheet_ptr, ani_index_ptr[l2], 2);
		}

		*sheet_ptr = -4;
		sheet_ptr++;

		*sheet_ptr = get_seq_header(ani_index_ptr[l_di]);
		sheet_ptr++;

		*sheet_ptr = 0x00;
		sheet_ptr++;
	}

	if ((max_range == 4) || check_hero(hero) ||
		((g_attacker_dead != 0) && (v6 == 0)) ||
		((g_defender_dead != 0) && (v6 == 1))) {

		sheet_ptr += copy_ani_seq(sheet_ptr, ani_index_ptr[l_di], 2);
	}

	if (((g_attacker_dead != 0) && (v6 == 0)) ||
		((g_defender_dead != 0) && (v6 == 1))) {

		*sheet_ptr = -4;
		sheet_ptr++;

		*sheet_ptr = get_seq_header(ani_index_ptr[20]);
		sheet_ptr++;

		*sheet_ptr = 0;
		sheet_ptr++;

		sheet_ptr += copy_ani_seq(sheet_ptr, ani_index_ptr[20], 2);
	}

	*sheet_ptr = -1;
}


/**
 * \brief   prepares a spell animation
 *
 *          This is used for "Blitz", "Fulminictus", "Ignifaxius"
 *
 * \param[in] sheet_id  0 or 1
 * \param   p           pointer to an entry of g_enemy_sheets
 * \param   max_range          4 of 99
 * \param   target      the id of the target
 * \param   caster      the id of the caster
 * \param   v5          0 or 1
 */
/* Borlandified and identical */
void FANI_prepare_spell_enemy(const signed int sheet_id, struct enemy_sheet *enemy, const signed int max_range, const signed int target, const signed int caster, const signed int v5)
{
	signed int l1;
	signed int x_target;
	signed int y_target;
	signed int x_caster;
	signed int y_caster;
	signed int dir;
	signed int l2;
	signed int l3;		/* indicees to ani_index_ptr */
	int8_t *sheet_ptr;	/* mostly written */
	int16_t *ani_index_ptr;	/* read only */

	signed int dir2;


	/* get a pointer from an array where the gfx_id of the enemy serves as index */
	ani_index_ptr = g_gfx_ani_index[enemy->gfx_id];

	FIG_search_obj_on_cb((signed char)caster, &x_caster, &y_caster);
	FIG_search_obj_on_cb((signed char)target, &x_target, &y_target);

	if (x_target == x_caster) {
		if (y_caster < y_target)
			dir = 1;
		else
			dir = 3;
	} else {
		if (x_caster < x_target)
			dir = 2;
		else
			dir = 0;
	}

	if ((signed char)caster == (signed char)target)
		dir = enemy->viewdir;

	/* this is true if a monster attacks a hero */
	l1 = (max_range == 4) ? 29 : 16;

	sheet_ptr = &g_fig_anisheets[sheet_id][1];

	/* this is true if a monster attacks a hero */
	l1 += (max_range == 4) ? dir : enemy->viewdir;

	g_fig_anisheets[sheet_id][0] = get_seq_header(ani_index_ptr[l1]);

	g_fig_anisheets[sheet_id][242] = enemy->gfx_id;

	if ((enemy->viewdir != dir) && (max_range == 4)) {

		g_fig_anisheets[sheet_id][0] = 0;

		l3 = l2 = -1;

		dir2 = enemy->viewdir;
		l3 = dir2;
		dir2++;
		if (dir2 == 4)
			dir2 = 0;

		if (dir2 != dir) {

			l2 = dir2;
			dir2++;
			if (dir2 == 4)
				dir2 = 0;
			if (dir2 != dir) {
				l3 = enemy->viewdir + 4;
				l2 = -1;
			}
		}

		enemy->viewdir = dir;

		sheet_ptr += copy_ani_seq(sheet_ptr, ani_index_ptr[l3], 1);

		if (l2 != -1)
			sheet_ptr += copy_ani_seq(sheet_ptr, ani_index_ptr[l2], 1);

		*sheet_ptr = -4;
		sheet_ptr++;

		*sheet_ptr = get_seq_header(ani_index_ptr[l1]);
		sheet_ptr++;

		*sheet_ptr = 0;
		sheet_ptr++;
	}

	sheet_ptr += copy_ani_seq(sheet_ptr, ani_index_ptr[l1], 1);

	if ((g_attacker_dead && !v5) || (g_defender_dead && (v5 == 1))) {

		*sheet_ptr = -4;
		sheet_ptr++;

		*sheet_ptr = get_seq_header(ani_index_ptr[20]);
		sheet_ptr++;

		*sheet_ptr = 0;
		sheet_ptr++;

		sheet_ptr += copy_ani_seq(sheet_ptr, ani_index_ptr[20], 1);
	}

	*sheet_ptr = -1;

	/* check if the moster sprite ID needs two fields */
	if (is_in_byte_array(enemy->gfx_id, g_double_size_gfx_id_table)) {
		memcpy(&g_fig_anisheets[sheet_id + 2], &g_fig_anisheets[sheet_id], 243);
	}
}

#if !defined(__BORLANDC__)
}
#endif
