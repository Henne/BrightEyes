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
 * \param   ani_track_id       [0, 1]
 * \param   hero            pointer to hero
 * \param   weapon_gfx_id   the type of weapon for the animation [-1, 5], 3,4,5 are range weapons
 * \param   action_type     {FIG_ACTION_MELEE_ATTACK = 2, FIG_ACTION_RANGE_ATTACK = 15, FIG_ACTION_PARRY = 100, FIG_ACTION_UNKNOWN3 = 102, FIG_ACTION_UNKNOWN4 = 103}
 */
/* Borlandified and identical */
void FANI_prepare_fight_hero_ani(const signed int ani_track_id, struct struct_hero *hero, const signed int weapon_gfx_id, const signed int f_action, const signed int object_id_attacker, const signed int object_id_target, const signed int a7)
{
	signed int l1;
	signed int attacker_x;
	signed int attacker_y;
	signed int target_x;
	signed int target_y;
	signed int viewdir;
	signed int l7;
	signed int l8;
	signed int l9;
	signed int i;
	int8_t *p_ani_clip_base;
	int8_t *p_ani_clip_weapon;
	signed int weapon_id;
	int16_t *ani_index_ptr;

	ani_index_ptr = g_gfx_ani_index[hero->actor_sprite_id];
	weapon_id = hero->inventory[HERO_INVENTORY_SLOT_RIGHT_HAND].item_id;

	if ((signed char)object_id_target) {

		FIG_search_obj_on_cb((signed char)object_id_target, &target_x, &target_y);
		FIG_search_obj_on_cb((signed char)object_id_attacker, &attacker_x, &attacker_y);

		if (attacker_x == target_x) {
			if (target_y < attacker_y) {
				viewdir = FIG_VIEWDIR_DOWN;
			} else {
				viewdir = FIG_VIEWDIR_UP;
			}
		} else {
			if (target_x < attacker_x) {
				viewdir = FIG_VIEWDIR_LEFT;
			} else {
				viewdir = FIG_VIEWDIR_RIGHT;
			}
		}
	} else {
		viewdir = hero->viewdir;
	}

	if (
		(weapon_gfx_id == WEAPON_GFX_ID_NONE)
		|| ((hero->typus == HERO_TYPE_MAGIER) && (weapon_id == ITEM_ID_ZAUBERSTAB))
	) {

		l1 = (f_action == FIG_ACTION_MELEE_ATTACK) ? 45 :		/* melee attack */
			(f_action == FIG_ACTION_UNKNOWN3) ? 41 :		/* drink potion */
			(f_action == FIG_ACTION_UNKNOWN4) ? 53 :		/* cast spell */
			49;

	} else {
		l1 = (f_action == FIG_ACTION_MELEE_ATTACK) ?  21:		/* melee attack */
			(f_action == FIG_ACTION_UNKNOWN3) ? 41 :		/* drink potion */
			(f_action == FIG_ACTION_UNKNOWN4) ? 53 :		/* cast spell */
			(f_action != FIG_ACTION_RANGE_ATTACK) ? 25 :
			(weapon_gfx_id == WEAPON_GFX_ID_RANGED_MISSILE) ? 33 :
			(weapon_gfx_id == WEAPON_GFX_ID_RANGED_THROW) ? 57 :
			61;
	}

	l1 += viewdir;
	p_ani_clip_base = &g_fig_ani_tracks[ani_track_id][1];
	p_ani_clip_weapon = &g_fig_ani_tracks[FANI_TRACK_ID_BASE_TO_WEAPON(ani_track_id)][1];

	g_fig_ani_tracks[ani_track_id][0] = get_seq_header(ani_index_ptr[l1]);
	g_fig_ani_tracks[ani_track_id][242] = hero->actor_sprite_id;

	if (
		check_hero(hero)
		&& (hero->viewdir != viewdir)
		&& (
			(f_action == FIG_ACTION_MELEE_ATTACK)
			|| (f_action == FIG_ACTION_RANGE_ATTACK)
			|| (f_action == FIG_ACTION_UNKNOWN4)
			|| ((f_action == FIG_ACTION_PARRY) && !g_fig_hero_parry_action_used[(signed char)object_id_attacker - 1])
			|| ((g_fig_critical_fail_backfire_2 != 0) && (a7 == 0))
			|| ((g_fig_critical_fail_backfire_1 != 0) && (a7 == 1))
		)
	) {
			g_fig_ani_tracks[ani_track_id][0] = 0;
			l8 = l7 = -1;
			l9 = hero->viewdir;
			l8 = l9;
			l9++;

			if (l9 == 4) {
				l9 = 0;
			}

			if (l9 != viewdir) {
				l7 = l9;
				l9++;
				if (l9 == 4) {
					l9 = 0;
				}

				if (l9 != viewdir) {
					l8 = hero->viewdir + 4;
					l7 = -1;
				}
			}

			hero->viewdir = viewdir;

			if (l7 == -1) {
				for (i = 0; i < 2; i++) {
					*p_ani_clip_base++ = -5;
					*p_ani_clip_base++ = 0;
					*p_ani_clip_base++ = 0;
				}
			}

			p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l8], ANI_SRC_FILE_ID_ANI_DAT);

			if (l7 != -1) {
				p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l7], ANI_SRC_FILE_ID_ANI_DAT);
			}

			*p_ani_clip_base++ = -4;
			*p_ani_clip_base++ = get_seq_header(ani_index_ptr[l1]);
			*p_ani_clip_base++ = 0;
	} else {
		for (i = 0; i < 5; i++) {
			*p_ani_clip_base++ = -5;
			*p_ani_clip_base++ = 0;
			*p_ani_clip_base++ = 0;
		}
	}

	if (
		(
			check_hero(hero)
			&& (f_action == FIG_ACTION_MELEE_ATTACK)
		) || (
			(f_action == FIG_ACTION_RANGE_ATTACK)
			|| (f_action == FIG_ACTION_UNKNOWN3)
			|| (f_action == FIG_ACTION_UNKNOWN4)
			|| ((f_action == FIG_ACTION_PARRY) && !g_fig_hero_parry_action_used[(signed char)object_id_attacker - 1])
		)
	) {
		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l1], ANI_SRC_FILE_ID_ANI_DAT);

		if (
			(weapon_gfx_id != WEAPON_GFX_ID_NONE)
			&& (weapon_gfx_id < WEAPON_GFX_ID_RANGED_MISSILE)
			&& (hero->typus != HERO_TYPE_MAGIER)
			&& (hero->typus != HERO_TYPE_DRUIDE)
		) {
			for (i = 0; i < 5; i++) {
				*p_ani_clip_weapon++ = -5;
				*p_ani_clip_weapon++ = 0;
				*p_ani_clip_weapon++ = 0;
			}

			p_ani_clip_weapon += copy_ani_seq(p_ani_clip_weapon,
				*(int16_t*)((uint8_t*)g_weaponani_table + (g_weaponani_types[hero->actor_sprite_id] * 48 + weapon_gfx_id * 16) +
				((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + hero->viewdir * 2), ANI_SRC_FILE_ID_WEAPANI_DAT);
		}
	}

	if (
		(
			check_hero(hero)
			&& g_fig_critical_fail_backfire_2
			&& !a7
		) || (
			g_fig_critical_fail_backfire_1
			&& (a7 == 1)
		)
	) {

			p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l1], ANI_SRC_FILE_ID_ANI_DAT);

			if (
				(weapon_gfx_id != WEAPON_GFX_ID_NONE)
				&& (weapon_gfx_id < WEAPON_GFX_ID_RANGED_MISSILE)
				&& (hero->typus != HERO_TYPE_MAGIER)
				&& (hero->typus != HERO_TYPE_DRUIDE)
			) {
				p_ani_clip_weapon += copy_ani_seq(p_ani_clip_weapon,
					*(int16_t*)((uint8_t*)g_weaponani_table + (g_weaponani_types[hero->actor_sprite_id] * 48 + weapon_gfx_id * 16) +
					((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + hero->viewdir * 2), 3);
			}
	}

	if (
		(g_fig_attacker_dead && !a7)
		|| (g_fig_target_dead && (a7 == 1))
	) {
		*p_ani_clip_base++ = -4;
		*p_ani_clip_base++ = get_seq_header(ani_index_ptr[20]);
		*p_ani_clip_base++ = 0;

		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[20], ANI_SRC_FILE_ID_ANI_DAT);
	}

	if (
		check_hero(hero)
		|| (g_fig_attacker_dead && !a7)
		|| (g_fig_target_dead && (a7 == 1))
	) {
		FIG_set_ani_track_id_base(hero->fighter_id, (signed char)ani_track_id);
		*p_ani_clip_base = -1;

		if (
			(weapon_gfx_id != WEAPON_GFX_ID_NONE)
			&& (weapon_gfx_id < WEAPON_GFX_ID_RANGED_MISSILE)
			&& (hero->typus != HERO_TYPE_MAGIER)
			&& (hero->typus != HERO_TYPE_DRUIDE)
		) {
			FIG_set_ani_track_id_weapon(hero->fighter_id, FANI_TRACK_ID_BASE_TO_WEAPON(ani_track_id));

			*p_ani_clip_weapon = -1;
		}
	}

	*p_ani_clip_base = -1;
	if (f_action == FIG_ACTION_PARRY) {
		g_fig_hero_parry_action_used[(signed char)object_id_attacker - 1] = 1;
	}
}

/**
 * \brief   prepares the animation sequence of a hero in fights
 */
/* Borlandified and identical */
void FANI_prepare_fight_enemy_ani(const signed int ani_track_id, struct enemy_sheet *enemy, const signed int f_action, const signed int object_id_attacker, const signed int object_id_target, const signed int a7)
{
	signed int l1;
	signed int attacker_x;
	signed int attacker_y;
	signed int target_x;
	signed int target_y;
	signed int viewdir;
	signed int l7;
	signed int l8;
	signed int l9;
	signed int i;
	int8_t *p_ani_clip_base;
	int8_t *p_ani_clip_weapon;
	struct struct_fighter *fighter;			/* only user for two sprited figures */
	int16_t *ani_index_ptr;			/* read only */
	signed int weapon_gfx_id;

	/* initialize with bare hands */
	weapon_gfx_id = WEAPON_GFX_ID_NONE;

	/* every enemy with the actor_sprite_id < 22 has a sword type weapon */
	if (enemy->actor_sprite_id < ACTOR_SPRITE_ID_ELF__FEMALE + 1) {
		weapon_gfx_id = WEAPON_GFX_ID_MELEE_BLADE;
	}

	if (enemy->weapon_broken) {
		weapon_gfx_id = WEAPON_GFX_ID_NONE;
	}

	/* read a pointer from an array with the actor_sprite_id as offset, read only */
	ani_index_ptr = g_gfx_ani_index[enemy->actor_sprite_id];

	/* find both actors on the chessboard */
	FIG_search_obj_on_cb((signed char)object_id_target, &target_x, &target_y);
	FIG_search_obj_on_cb((signed char)object_id_attacker, &attacker_x, &attacker_y);

	/* find out which direction the action will have */
	if (attacker_x == target_x) {
		if (target_y < attacker_y) {
			viewdir = FIG_VIEWDIR_DOWN;
		} else {
			viewdir = FIG_VIEWDIR_UP;
		}
	} else {
		if (target_x < attacker_x) {
			viewdir = FIG_VIEWDIR_LEFT;
		} else {
			viewdir = FIG_VIEWDIR_RIGHT;
		}
	}

	/* melee attack */
	l1 = (f_action == FIG_ACTION_MELEE_ATTACK) ? 21 : 25;

	if (
		(enemy->actor_sprite_id == ACTOR_SPRITE_ID_DRUIDE__MALE)
		|| (enemy->actor_sprite_id == ACTOR_SPRITE_ID_MAGIER__MALE)
		|| (enemy->actor_sprite_id == ACTOR_SPRITE_ID_DRUIDE__FEMALE)
		|| (enemy->actor_sprite_id == ACTOR_SPRITE_ID_MAGIER__FEMALE)
	) {

		weapon_gfx_id = WEAPON_GFX_ID_NONE;
		l1 = (f_action == FIG_ACTION_MELEE_ATTACK) ? 45 : 49;
	}

	if (f_action == FIG_ACTION_RANGE_ATTACK) {
		l1 = 33;
		weapon_gfx_id = WEAPON_GFX_ID_NONE;
	}

	l1 += viewdir;

	p_ani_clip_base = &g_fig_ani_tracks[ani_track_id][1];
	p_ani_clip_weapon = &g_fig_ani_tracks[FANI_TRACK_ID_BASE_TO_WEAPON(ani_track_id)][1];


	g_fig_ani_tracks[ani_track_id][0] = get_seq_header(ani_index_ptr[l1]);
	g_fig_ani_tracks[ani_track_id][242] = enemy->actor_sprite_id;

	/* first the enemy may turn */
	if (
		(enemy->viewdir != viewdir)
		&& (
			(
				 (f_action == FIG_ACTION_MELEE_ATTACK)
				 || (f_action == FIG_ACTION_RANGE_ATTACK)
				 || ((f_action == FIG_ACTION_PARRY) && !g_fig_enemy_parry_action_used[(signed char)object_id_attacker])
			)
			|| (g_fig_critical_fail_backfire_2 && !a7)
			|| (g_fig_critical_fail_backfire_1 && (a7 == 1))
		)
	) {

		g_fig_ani_tracks[ani_track_id][0] = 0;

		/* find out the new direction */
		l8 = l7 = -1;

		/* try to turn right 90 degrees */
		l9 = enemy->viewdir;
		l8 = l9;
		l9++;

		if (l9 == 4) {
			l9 = 0;
		}

		if (l9 != viewdir) {
			l7 = l9;
			l9++;
			if (l9 == 4) {
				l9 = 0;
			}

			if (l9 != viewdir) {
				l8 = enemy->viewdir + 4;
				l7 = -1;
			}
		}


		/* set the new direction in enemy sheet */
		enemy->viewdir = viewdir;

		/* only if the turn is 90 degree */
		if (l7 == -1) {
			/* do not move for 2 frames */
			for (i = 0; i < 2; i++) {
				*p_ani_clip_base++ = -5;
				*p_ani_clip_base++ = 0;
				*p_ani_clip_base++ = 0;
			}
		}

		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l8], 1);

		if (l7 != -1) {
			p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l7], 1);
		}

		*p_ani_clip_base++ = -4;
		*p_ani_clip_base++ = get_seq_header(ani_index_ptr[l1]);
		*p_ani_clip_base++ = 0;
	} else {
		/* do not move for 5 frames */
		for (i = 0; i < 5; i++) {
			*p_ani_clip_base++ = -5;
			*p_ani_clip_base++ = 0;
			*p_ani_clip_base++ = 0;
		}
	}

	if (
		(f_action == FIG_ACTION_MELEE_ATTACK)
		|| (f_action == FIG_ACTION_RANGE_ATTACK)
		|| ((f_action == FIG_ACTION_PARRY) && !g_fig_enemy_parry_action_used[(signed char)object_id_attacker])
	) {
		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l1], 1);

		if (weapon_gfx_id != WEAPON_GFX_ID_NONE) {

			/* do not move for 5 frames */
			for (i = 0; i < 5; i++) {
				*p_ani_clip_weapon++ = -5;
				*p_ani_clip_weapon++ = 0;
				*p_ani_clip_weapon++ = 0;
			}

			/* copy the weapon ani */
			p_ani_clip_weapon += copy_ani_seq(p_ani_clip_weapon,
				*(int16_t*)((uint8_t*)g_weaponani_table + g_weaponani_types[enemy->actor_sprite_id] * 48 + weapon_gfx_id * 16 + ((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + enemy->viewdir * 2), 3);
		}
	}

	if (
		((g_fig_critical_fail_backfire_2 != 0) && (a7 == 0))
		|| ((g_fig_critical_fail_backfire_1 != 0) && (a7 == 1))
	) {

			p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l1], 1);

			if (weapon_gfx_id != WEAPON_GFX_ID_NONE) {

				/* copy the weapon ani */
				p_ani_clip_weapon += copy_ani_seq(p_ani_clip_weapon,
					*(int16_t*)((uint8_t*)g_weaponani_table + g_weaponani_types[enemy->actor_sprite_id] * 48 + weapon_gfx_id * 16 + ((f_action == FIG_ACTION_MELEE_ATTACK) ? 0 : 1) * 8 + enemy->viewdir * 2), 3);
			}
	}

	if (
		((g_fig_attacker_dead != 0) && (a7 == 0))
		|| ((g_fig_target_dead != 0) && (a7 == 1))
	) {
		*p_ani_clip_base++ = -4;
		*p_ani_clip_base++ = get_seq_header(ani_index_ptr[20]);
		*p_ani_clip_base++ = 0;

		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[20], 1);
	}

	FIG_set_ani_track_id_base(enemy->fighter_id, (signed char)ani_track_id);

	/* terminate figure animation array */
	*p_ani_clip_base = -1;

	/* does this sprite need two fields */
	if (is_in_byte_array(enemy->actor_sprite_id, g_double_size_actor_sprite_id_table))	{

		memcpy(&g_fig_ani_tracks[FANI_TRACK_ID_BASE_TO_TAIL(ani_track_id)], &g_fig_ani_tracks[ani_track_id], 243);

		fighter = FIG_get_fighter(enemy->fighter_id);

		FIG_set_ani_track_id_base(g_fig_double_size_fighter_id_table[fighter->double_size], FANI_TRACK_ID_BASE_TO_TAIL(ani_track_id));
	}

	if (weapon_gfx_id != WEAPON_GFX_ID_NONE) {

		FIG_set_ani_track_id_weapon(enemy->fighter_id, FANI_TRACK_ID_BASE_TO_WEAPON(ani_track_id));

		/* terminate weapon animation array */
		*p_ani_clip_weapon = -1;
	}

	if (f_action == FIG_ACTION_PARRY) {
		g_fig_enemy_parry_action_used[(signed char)object_id_attacker] = 1;
	}
}


/**
 *
 * \param[in] ani_track_id  0 or 1
 * \param   hero        pointer to a hero
 * \param   max_range          99 or 4
 * \param   caster_object_id
 * \param   target_object_id
 * \param   v5		-1 or a var
 * \param   v6          0 or 1
 */

/* Borlandified and identical */
void FANI_prepare_spell_hero(const signed int ani_track_id, struct struct_hero *hero, const signed int max_range, const signed int caster_object_id, const signed int target_object_id, const signed int v5, const signed int v6)
{
	signed int x_caster;
	signed int y_caster;
	signed int x_target;
	signed int y_target;
	signed int viewdir;
	signed int l2;
	signed int l3;
	int8_t *p_ani_clip_base;
	int16_t *ani_index_ptr;

	signed int l_di;

	/* get a pointer from an array where the Monster-ID serves as index */
	ani_index_ptr = g_gfx_ani_index[hero->actor_sprite_id];

	FIG_search_obj_on_cb((signed char)target_object_id, &x_target, &y_target);
	FIG_search_obj_on_cb((signed char)caster_object_id, &x_caster, &y_caster);

	if (x_caster == x_target) {
		if (y_target < y_caster)
			viewdir = FIG_VIEWDIR_DOWN;
		else
			viewdir = FIG_VIEWDIR_UP;
	} else {
		if (x_target < x_caster)
			viewdir = FIG_VIEWDIR_LEFT;
		else
			viewdir = FIG_VIEWDIR_RIGHT;
	}

	if ((signed char)target_object_id == (signed char)caster_object_id)
		viewdir = hero->viewdir;


	l_di = (max_range == 4) ? ((v5 == 1) ? 37 : 29) : 16;

	l_di += (max_range == 4) ? viewdir : hero->viewdir;

	p_ani_clip_base = &g_fig_ani_tracks[ani_track_id][1];

	g_fig_ani_tracks[ani_track_id][0] = get_seq_header(ani_index_ptr[l_di]);

	g_fig_ani_tracks[ani_track_id][242] = hero->actor_sprite_id;

	if ((hero->viewdir != viewdir) && (max_range == 4)) {

		signed int viewdir_2;

		g_fig_ani_tracks[ani_track_id][0] = 0;

		l3 = l2 = -1;
		viewdir_2 = hero->viewdir;
		l3 = viewdir_2;
		viewdir_2++;
		if (viewdir_2 == 4)
			viewdir_2 = 0;

		if (viewdir_2 != viewdir) {
			l2 = viewdir_2;
			viewdir_2++;
			if (viewdir_2 == 4)
				viewdir_2 = 0;

			if (viewdir_2 != viewdir) {
				l3 = hero->viewdir + 4;
				l2 = -1;
			}
		}

		hero->viewdir = viewdir;
		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l3], ANI_SRC_FILE_ID_ANI_DAT);

		if (l2 != -1) {
			p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l2], ANI_SRC_FILE_ID_ANI_DAT);
		}

		*p_ani_clip_base = -4;
		p_ani_clip_base++;

		*p_ani_clip_base = get_seq_header(ani_index_ptr[l_di]);
		p_ani_clip_base++;

		*p_ani_clip_base = 0x00;
		p_ani_clip_base++;
	}

	if (
		(max_range == 4)
		|| check_hero(hero)
		|| ((g_fig_attacker_dead != 0) && (v6 == 0))
		|| ((g_fig_target_dead != 0) && (v6 == 1))
	) {

		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l_di], ANI_SRC_FILE_ID_ANI_DAT);
	}

	if (
		((g_fig_attacker_dead != 0) && (v6 == 0))
		|| ((g_fig_target_dead != 0) && (v6 == 1))
	) {

		*p_ani_clip_base = -4;
		p_ani_clip_base++;

		*p_ani_clip_base = get_seq_header(ani_index_ptr[20]);
		p_ani_clip_base++;

		*p_ani_clip_base = 0;
		p_ani_clip_base++;

		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[20], ANI_SRC_FILE_ID_ANI_DAT);
	}

	*p_ani_clip_base = -1;
}


/**
 * \brief   prepares a spell animation
 *
 *          This is used for "Blitz", "Fulminictus", "Ignifaxius"
 *
 * \param[in] ani_track_id  0 or 1
 * \param   p           pointer to an entry of g_enemy_sheets
 * \param   max_range   4 of 99
 * \param   target_object_id      the id of the target
 * \param   caster_object_id      the id of the caster
 * \param   v5          0 or 1
 */
/* Borlandified and identical */
void FANI_prepare_spell_enemy(const signed int ani_track_id, struct enemy_sheet *enemy, const signed int max_range, const signed int target_object_id, const signed int caster_object_id, const signed int v5)
{
	signed int l1;
	signed int x_target;
	signed int y_target;
	signed int x_caster;
	signed int y_caster;
	signed int viewdir;
	signed int l2;
	signed int l3;		/* indicees to ani_index_ptr */
	int8_t *p_ani_clip_base;	/* mostly written */
	int16_t *ani_index_ptr;	/* read only */

	signed int viewdir_2;


	/* get a pointer from an array where the actor_sprite_id of the enemy serves as index */
	ani_index_ptr = g_gfx_ani_index[enemy->actor_sprite_id];

	FIG_search_obj_on_cb((signed char)caster_object_id, &x_caster, &y_caster);
	FIG_search_obj_on_cb((signed char)target_object_id, &x_target, &y_target);

	if (x_target == x_caster) {
		if (y_caster < y_target)
			viewdir = FIG_VIEWDIR_DOWN;
		else
			viewdir = FIG_VIEWDIR_UP;
	} else {
		if (x_caster < x_target)
			viewdir = FIG_VIEWDIR_LEFT;
		else
			viewdir = FIG_VIEWDIR_RIGHT;
	}

	if ((signed char)caster_object_id == (signed char)target_object_id)
		viewdir = enemy->viewdir;

	/* this is true if an enemy attacks a hero */
	l1 = (max_range == 4) ? 29 : 16;

	p_ani_clip_base = &g_fig_ani_tracks[ani_track_id][1];

	/* this is true if an enemy attacks a hero */
	l1 += (max_range == 4) ? viewdir : enemy->viewdir;

	g_fig_ani_tracks[ani_track_id][0] = get_seq_header(ani_index_ptr[l1]);

	g_fig_ani_tracks[ani_track_id][242] = enemy->actor_sprite_id;

	if ((enemy->viewdir != viewdir) && (max_range == 4)) {

		g_fig_ani_tracks[ani_track_id][0] = 0;

		l3 = l2 = -1;

		viewdir_2 = enemy->viewdir;
		l3 = viewdir_2;
		viewdir_2++;
		if (viewdir_2 == 4)
			viewdir_2 = 0;

		if (viewdir_2 != viewdir) {

			l2 = viewdir_2;
			viewdir_2++;
			if (viewdir_2 == 4)
				viewdir_2 = 0;
			if (viewdir_2 != viewdir) {
				l3 = enemy->viewdir + 4;
				l2 = -1;
			}
		}

		enemy->viewdir = viewdir;

		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l3], 1);

		if (l2 != -1)
			p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l2], 1);

		*p_ani_clip_base = -4;
		p_ani_clip_base++;

		*p_ani_clip_base = get_seq_header(ani_index_ptr[l1]);
		p_ani_clip_base++;

		*p_ani_clip_base = 0;
		p_ani_clip_base++;
	}

	p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[l1], 1);

	if (
		(g_fig_attacker_dead && !v5)
		|| (g_fig_target_dead && (v5 == 1))
	) {

		*p_ani_clip_base = -4;
		p_ani_clip_base++;

		*p_ani_clip_base = get_seq_header(ani_index_ptr[20]);
		p_ani_clip_base++;

		*p_ani_clip_base = 0;
		p_ani_clip_base++;

		p_ani_clip_base += copy_ani_seq(p_ani_clip_base, ani_index_ptr[20], 1);
	}

	*p_ani_clip_base = -1;

	/* check if the moster sprite ID needs two squares */
	if (is_in_byte_array(enemy->actor_sprite_id, g_double_size_actor_sprite_id_table)) {
		memcpy(&g_fig_ani_tracks[FANI_TRACK_ID_BASE_TO_TAIL(ani_track_id)], &g_fig_ani_tracks[ani_track_id], 243);
	}
}
