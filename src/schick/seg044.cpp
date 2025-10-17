/*
 *	Rewrite of DSA1 v3.02_de functions of seg044 (Fightsystem)
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
#if defined(__BORLANDC__)
static
#endif
Bit16s copy_ani_seq(Bit8s *dst, Bit16s ani, Bit16u type)
{
	Bit8s *p_start, *p_seq;
	signed short nr_anis;
	signed short i;
	signed char length;

	/* get pointer from ANI.DAT */
	p_start = (Bit8s*)g_buffer_anidat;

	/* check if we must use WEAPANI.DAT */
	if (type == 3)
		p_start = (Bit8s*)g_buffer_weapanidat;

	/* get number of animation sequences */
	nr_anis = *(Bit16s*)p_start;

	/* sanity check */
	if (ani < 0)
		return 0;

	if (ani > nr_anis)
		return 0;

	/* set p_seq to the first sequence */
	p_seq = p_start;
	p_seq += nr_anis + 2;

	/* set length to the length of the first sequence */
	length = p_start[2];

	/* fast forward to the requestet sequence */
	for (i = 1; i <= ani; i++) {
		p_seq += length;
		length = *(p_start + i + 2);
	}

	/* skip the first byte */
	p_seq++;
	/* calc the length of the sequence */
	length -= 2;
	/* REMARK: the first an the last byte of the sequence are skipped */

	/* copy them */
	for (i = 0; i < length; i++)
		*dst++ = *p_seq++;

	return length;
}

/**
 * \brief   TODO
 *
 * \param   ani         the number of the animation sequence
 * \return              the first byte of the sequence from ANI.DAT {0,1,2,3,4}
 */
/* Borlandified and identical */
#if defined(__BORLANDC__)
static
#endif
Bit8s get_seq_header(Bit16s ani)
{
	Bit8s *p_start, *p_seq;
	Bit8s length;
	Bit16s nr_anis;
	Bit16s i;

	/* get pointer from ANI.DAT */
	p_start = (Bit8s*)g_buffer_anidat;

	/* get number of ani seqences in ANI.DAT */
	nr_anis = *(Bit16s*)p_start;

	if (ani < 0) {
		return 0;
	}

	if (ani > nr_anis) {
		return 0;
	}

	p_seq = p_start;
	p_seq += nr_anis + 2;

	length = p_start[2];

	for (i = 1; i <= ani; i++) {
		/* set pointer to the start of the next sequence */
		p_seq += length;
		/* set length to the length of the next sequence */
		length = *(p_start + i + 2);
	}

	return *p_seq;
}

/**
 * \brief   prepares the animation sequence of a hero in fights
 *
 * \param   a1          [0, 1]
 * \param   hero        pointer to hero
 * \param   weapon_type the type of weapon for the animation [-1, 5], 3,4,5 are range weapons
 * \param   action_type {FIG_ACTION_MELEE_ATTACK = 2, FIG_ACTION_RANGE_ATTACK = 15, FIG_ACTION_UNKNOWN2 = 100, FIG_ACTION_UNKNOWN3 = 102, FIG_ACTION_UNKNOWN4 = 103}
 */
/* Borlandified and identical */
void FIG_prepare_hero_fight_ani(signed short a1, struct struct_hero *hero, signed short weapon_type, signed short f_action, signed short fid_attacker, signed short fid_target, signed short a7)
{
	signed short l1;
	signed short attacker_x;
	signed short attacker_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	signed short l7;
	signed short l8;
	signed short l9;
	signed short l10;
	Bit8s *p1;
	Bit8s *p2;
	signed short weapon;
	Bit16s *p3;

	p3 = g_gfx_ani_index[hero->sprite_no];
	weapon = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;

	if ((signed char)fid_target) {

		FIG_search_obj_on_cb((signed char)fid_target, &target_x, &target_y);
		FIG_search_obj_on_cb((signed char)fid_attacker, &attacker_x, &attacker_y);

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

	if ((weapon_type == -1) || ((hero->typus == HERO_TYPE_MAGE) && (weapon == ITEM_MAGIC_WAND))) {

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
	p1 = (Bit8s*)&g_fig_anisheets[a1][1];
	p2 = (Bit8s*)&g_fig_anisheets[a1 + 4][1];

	g_fig_anisheets[a1][0] = get_seq_header(p3[l1]);
	g_fig_anisheets[a1][242] = hero->sprite_no;

	if (check_hero(hero) && (hero->viewdir != dir) &&

		((f_action == FIG_ACTION_MELEE_ATTACK) || (f_action == FIG_ACTION_RANGE_ATTACK) || (f_action == FIG_ACTION_UNKNOWN4) ||
			((f_action == FIG_ACTION_UNKNOWN2) && !g_hero_is_target[(signed char)fid_attacker - 1]) ||
			((g_attacker_attacks_again != 0) && (a7 == 0)) ||
			((g_defender_attacks != 0) && (a7 == 1))))
	{
			g_fig_anisheets[a1][0] = 0;
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

			hero->viewdir = (signed char)dir;

			if (l7 == -1) {
				for (l10 = 0; l10 < 2; l10++) {
					*p1++ = -5;
					*p1++ = 0;
					*p1++ = 0;
				}
			}

			p1 += copy_ani_seq(p1, p3[l8], 2);

			if (l7 != -1) {
				p1 += copy_ani_seq(p1, p3[l7], 2);
			}

			*p1++ = -4;
			*p1++ = get_seq_header(p3[l1]);
			*p1++ = 0;
	} else {
		for (l10 = 0; l10 < 5; l10++) {
			*p1++ = -5;
			*p1++ = 0;
			*p1++ = 0;
		}
	}

	if ((check_hero(hero) && (f_action == FIG_ACTION_MELEE_ATTACK)) ||
		((f_action == FIG_ACTION_RANGE_ATTACK) || (f_action == FIG_ACTION_UNKNOWN3) || (f_action == FIG_ACTION_UNKNOWN4) ||
			((f_action == FIG_ACTION_UNKNOWN2) && !g_hero_is_target[(signed char)fid_attacker - 1])))
	{
		p1 += copy_ani_seq(p1, p3[l1], 2);

		if ((weapon_type != -1) && (weapon_type < 3) &&
			(hero->typus != HERO_TYPE_MAGE) &&
			(hero->typus != HERO_TYPE_DRUID))
		{
			for (l10 = 0; l10 < 5; l10++) {
				*p2++ = -5;
				*p2++ = 0;
				*p2++ = 0;
			}

			p2 += copy_ani_seq(p2,
				*(Bit16s*)((Bit8u*)g_weaponani_table + (g_weaponani_types[hero->sprite_no] * 48 + weapon_type * 16) +
				((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + hero->viewdir * 2), 3);
		}
	}

	if ((check_hero(hero) && g_attacker_attacks_again && !a7) || (g_defender_attacks && (a7 == 1))) {

			p1 += copy_ani_seq(p1, p3[l1], 2);

			if ((weapon_type != -1) && (weapon_type < 3) &&
				(hero->typus != HERO_TYPE_MAGE) &&
				(hero->typus != HERO_TYPE_DRUID))
			{
				p2 += copy_ani_seq(p2,
					*(Bit16s*)((Bit8u*)g_weaponani_table + (g_weaponani_types[hero->sprite_no] * 48 + weapon_type * 16) +
					((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + hero->viewdir * 2), 3);
			}
	}

	if ((g_attacker_dead && !a7) || (g_defender_dead && (a7 == 1)))
	{
		*p1++ = -4;
		*p1++ = get_seq_header(p3[20]);
		*p1++ = 0;

		p1 += copy_ani_seq(p1, p3[20], 2);
	}

	if (check_hero(hero) ||	(g_attacker_dead && !a7) || (g_defender_dead && (a7 == 1)))
	{
		FIG_set_sheet(hero->fighter_id, (signed char)a1);
		*p1 = -1;

		if ( (weapon_type != -1) && (weapon_type < 3) &&
			(hero->typus != HERO_TYPE_MAGE) &&
			(hero->typus != HERO_TYPE_DRUID))
		{
			FIG_set_weapon_sheet(hero->fighter_id, a1 + 4);
			*p2 = -1;
		}
	}

	*p1 = -1;
	if (f_action == FIG_ACTION_UNKNOWN2) {
		g_hero_is_target[(signed char)fid_attacker - 1] = 1;
	}
}

/**
 * \brief   prepares the animation sequence of a hero in fights
 */
/* Borlandified and identical */
void FIG_prepare_enemy_fight_ani(signed short a1, struct enemy_sheet *enemy, signed short f_action, signed short fid_attacker, signed short fid_target, signed short a7)
{
	signed short l1;
	signed short attacker_x;
	signed short attacker_y;
	signed short target_x;
	signed short target_y;
	signed short dir;
	signed short l7;
	signed short l8;
	signed short l9;
	signed short i;
	Bit8s *p1;
	Bit8s *p2;
	struct struct_fighter *fighter;			/* only user for two sprited figures */
	Bit16s *p4;			/* read only */
	signed short weapon_type;

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
	p4 = g_gfx_ani_index[enemy->gfx_id];

	/* find both actors on the chessboard */
	FIG_search_obj_on_cb((signed char)fid_target, &target_x, &target_y);
	FIG_search_obj_on_cb((signed char)fid_attacker, &attacker_x, &attacker_y);

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

	p1 = (Bit8s*)&g_fig_anisheets[a1][1];
	p2 = (Bit8s*)&g_fig_anisheets[a1 + 4][1];


	g_fig_anisheets[a1][0] = get_seq_header(p4[l1]);
	g_fig_anisheets[a1][242] = enemy->gfx_id;

	/* first the enemy may turn */
	if ((enemy->viewdir != dir) &&
		(	((f_action == FIG_ACTION_MELEE_ATTACK) || (f_action == FIG_ACTION_RANGE_ATTACK) ||
			((f_action == FIG_ACTION_UNKNOWN2) && !g_fig_actors_unkn[(signed char)fid_attacker])) ||
			(g_attacker_attacks_again && !a7) ||
			(g_defender_attacks && (a7 == 1))))
		{

		g_fig_anisheets[a1][0] = 0;

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
		enemy->viewdir = (signed char)dir;

		/* only if the turn is 90 degree */
		if (l7 == -1) {
			/* do not move for 2 frames */
			for (i = 0; i < 2; i++) {
				*p1++ = -5;
				*p1++ = 0;
				*p1++ = 0;
			}
		}

		p1 += copy_ani_seq(p1, p4[l8], 1);

		if (l7 != -1) {
			p1 += copy_ani_seq(p1, p4[l7], 1);
		}

		*p1++ = -4;
		*p1++ = get_seq_header(p4[l1]);
		*p1++ = 0;
	} else {
		/* do not move for 5 frames */
		for (i = 0; i < 5; i++) {
			*p1++ = -5;
			*p1++ = 0;
			*p1++ = 0;
		}
	}

	if ((f_action == FIG_ACTION_MELEE_ATTACK) || (f_action == FIG_ACTION_RANGE_ATTACK) ||
		((f_action == FIG_ACTION_UNKNOWN2) && !g_fig_actors_unkn[(signed char)fid_attacker]))
	{
		p1 += copy_ani_seq(p1, p4[l1], 1);

		if (weapon_type != -1) {

			/* do not move for 5 frames */
			for (i = 0; i < 5; i++) {
				*p2++ = -5;
				*p2++ = 0;
				*p2++ = 0;
			}

			/* copy the weapon ani */
			p2 += copy_ani_seq(p2,
				*(Bit16s*)((Bit8u*)g_weaponani_table + g_weaponani_types[enemy->gfx_id] * 48 + weapon_type * 16 + ((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + enemy->viewdir * 2), 3);
		}
	}

	if (((g_attacker_attacks_again != 0) && (a7 == 0)) ||
		((g_defender_attacks != 0) && (a7 == 1))) {

			p1 += copy_ani_seq(p1, p4[l1], 1);

			if (weapon_type != -1) {

				/* copy the weapon ani */
				p2 += copy_ani_seq(p2,
					*(Bit16s*)((Bit8u*)g_weaponani_table + g_weaponani_types[enemy->gfx_id] * 48 + weapon_type * 16 + ((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + enemy->viewdir * 2), 3);
			}
	}

	if ( ((g_attacker_dead != 0) && (a7 == 0)) ||
		((g_defender_dead != 0) && (a7 == 1)))
	{
		*p1++ = -4;
		*p1++ = get_seq_header(p4[20]);
		*p1++ = 0;

		p1 += copy_ani_seq(p1, p4[20], 1);
	}

	FIG_set_sheet(enemy->fighter_id, (signed char)a1);

	/* terminate figure animation array */
	*p1 = -1;

	/* does this sprite need two fields */
	if (is_in_byte_array(enemy->gfx_id, g_two_fielded_sprite_id))	{

		memcpy(&g_fig_anisheets[a1 + 2], &g_fig_anisheets[a1], 0xf3);

		fighter = FIG_get_fighter(enemy->fighter_id);

		FIG_set_sheet(g_fig_twofielded_table[fighter->twofielded], a1 + 2);
	}

	if (weapon_type != -1) {

		FIG_set_weapon_sheet(enemy->fighter_id, a1 + 4);

		/* terminate weapon animation array */
		*p2 = -1;
	}

	if (f_action == FIG_ACTION_UNKNOWN2) {
		g_fig_actors_unkn[(signed char)fid_attacker] = 1;
	}
}


/**
 *
 * \param   v1          0 or 1
 * \param   hero        pointer to a hero
 * \param   v2          99 or 4
 * \param   obj1        * \param obj2	* \param v5	-1 or a var
 * \param   v6          0 or 1
 */

/* Borlandified and identical */
void seg044_002a(Bit16u v1, struct struct_hero *hero, Bit16u v2, Bit16s obj1, Bit16s obj2,
			Bit16u v5, Bit16u v6)
{
	signed short x_obj1, y_obj1;
	signed short x_obj2, y_obj2;
	signed short dir;
	signed short l2;
	signed short l3;
	Bit8s *lp1;
	Bit16s *lp2;

	signed short l_di;
	signed short l_si;

	/* get a pointer from an array where the Monster-ID serves as index */
	lp2 = g_gfx_ani_index[hero->sprite_no];

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


	l_di = (v2 == 4) ? ((v5 == 1) ? 37 : 29) : 16;

	l_di += (v2 == 4) ? dir : hero->viewdir;

	lp1 = (Bit8s*)&g_fig_anisheets[v1][1];

	g_fig_anisheets[v1][0] = get_seq_header(lp2[l_di]);

	g_fig_anisheets[v1][242] = hero->sprite_no;

	if ((hero->viewdir != dir) && (v2 == 4)) {

		g_fig_anisheets[v1][0] = 0;

		l3 = l2 = -1;
		l_si = hero->viewdir;
		l3 = l_si;
		l_si++;
		if (l_si == 4)
			l_si = 0;

		if (l_si != dir) {
			l2 = l_si;
			l_si++;
			if (l_si == 4)
				l_si = 0;

			if (l_si != dir) {
				l3 = hero->viewdir + 4;
				l2 = -1;
			}
		}

		hero->viewdir = (signed char)dir;
		lp1 += copy_ani_seq(lp1, lp2[l3], 2);

		if (l2 != -1) {
			lp1 += copy_ani_seq(lp1, lp2[l2], 2);
		}

		*lp1 = -4;
		lp1++;

		*lp1 = get_seq_header(lp2[l_di]);
		lp1++;

		*lp1 = 0x00;
		lp1++;
	}

	if ((v2 == 4) || check_hero(hero) ||
		((g_attacker_dead != 0) && (v6 == 0)) ||
		((g_defender_dead != 0) && (v6 == 1))) {

		lp1 += copy_ani_seq(lp1, lp2[l_di], 2);
	}

	if (((g_attacker_dead != 0) && (v6 == 0)) ||
		((g_defender_dead != 0) && (v6 == 1))) {

		*lp1 = -4;
		lp1++;

		*lp1 = get_seq_header(lp2[20]);
		lp1++;

		*lp1 = 0;
		lp1++;

		lp1 += copy_ani_seq(lp1, lp2[20], 2);
	}

	*lp1 = -1;
}


/**
 * \brief   prepares a spell animation
 *
 *          This is used for "Blitz", "Fulminictus", "Ignifaxius"
 *
 * \param   v1          0 or 1
 * \param   p           pointer to an entry of g_enemy_sheets
 * \param   v2          4 of 99
 * \param   target      the id of the target
 * \param   caster      the id of the caster
 * \param   v5          0 or 1
 */
/* Borlandified and identical */
void seg044_002f(signed short v1, struct enemy_sheet *enemy, signed short v2, signed short target, signed short caster,
								signed short v5)
{
	signed short l1;
	signed short x_target, y_target;
	signed short x_caster, y_caster;
	signed short dir;
	signed short l2, l3;	/* indicees to lp2 */
	Bit8s *lp1;		/* mostly written */
	Bit16s *lp2;		/* read only */

	signed short dir2;


	/* get a pointer from an array where the gfx_id of the enemy serves as index */
	lp2 = g_gfx_ani_index[enemy->gfx_id];

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
	l1 = (v2 == 4) ? 29 : 16;

	lp1 = (Bit8s*)&g_fig_anisheets[v1][1];

	/* this is true if a monster attacks a hero */
	l1 += (v2 == 4) ? dir : enemy->viewdir;

	g_fig_anisheets[v1][0] = get_seq_header(lp2[l1]);

	g_fig_anisheets[v1][242] = enemy->gfx_id;

	if ((enemy->viewdir != dir) && (v2 == 4)) {

		g_fig_anisheets[v1][0] = 0;

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

		lp1 += copy_ani_seq(lp1, lp2[l3], 1);

		if (l2 != -1)
			lp1 += copy_ani_seq(lp1, lp2[l2], 1);

		*lp1 = -4;
		lp1++;

		*lp1 = get_seq_header(lp2[l1]);
		lp1++;

		*lp1 = 0;
		lp1++;
	}

	lp1 += copy_ani_seq(lp1, lp2[l1], 1);

	if ((g_attacker_dead && !v5) || (g_defender_dead && (v5 == 1))) {

		*lp1 = -4;
		lp1++;

		*lp1 = get_seq_header(lp2[20]);
		lp1++;

		*lp1 = 0;
		lp1++;

		lp1 += copy_ani_seq(lp1, lp2[20], 1);
	}

	*lp1 = -1;

	/* check if the moster sprite ID needs two fields */
	if (is_in_byte_array(enemy->gfx_id, g_two_fielded_sprite_id)) {
		memcpy(&g_fig_anisheets[v1 + 2], &g_fig_anisheets[v1], 0xf3);
	}
}

#if !defined(__BORLANDC__)
}
#endif
