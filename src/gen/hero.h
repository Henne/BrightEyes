#ifndef HERO_H
#define HERO_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
  #include <assert.h>
#define STATIC_ASSERT(expr, msg) static_assert(expr, #msg)
#else
  #define STATIC_ASSERT(expr, msg) typedef char static_assert_##msg[(expr) ? 1 : -1]
#endif

#if defined(__BORLANDC__) || defined(__WATCOMC__)
typedef signed long int32_t;
#else
typedef signed int int32_t;
#endif

STATIC_ASSERT(sizeof(int32_t) == 4, int32_needs_to_be_4_bytes);

#pragma pack(1)

struct struct_attribs {
	signed char normal;
	signed char current;
	signed char mod;
};

STATIC_ASSERT(sizeof(struct struct_attribs) == 3, struct_attribs_needs_to_be_3_bytes);

struct struct_hero {
	/* Offset 0x00 */
	char name[16];
	/* Offset 0x10 */
	char alias[16];
	/* Offset 0x20 */
	signed char items_num;
	signed char typus;
	signed char sex;
	unsigned char height;
	signed short weight;
	signed char god;
	signed char level;
	int32_t ap;
	int32_t money;
	/* Offset 0x30 */
	signed char rs_bonus1;
	signed char rs_bonus2;
	signed char rs_be;
	unsigned char bp_left;
	struct struct_attribs attrib[14];
	/* Offset 0x5e */
	signed short le_max;
	signed short le;
	/* Offset 0x60 */
	signed short ae_max;
	signed short ae;
	signed char mr;

	signed char atpa_base;
	signed char at_weapon[7];
	signed char pa_weapon[7];
	signed char w_at_mod;
	signed char w_pa_mod;
	signed char w_type;
	signed char atpa_mod;
	unsigned char le_malus;
	signed char timer_id;
	signed char start_gear;
	signed char herbs;
	signed char hunger_timer;
	signed char hunger;
	/* Offset 0x80 */
	signed char thirst;
	signed char fighter_id;
	signed char viewdir;
	signed char actions;
	signed char action_id;
	signed char spell_id;
	signed char enemy_id;
	signed char group_no;
	signed char temple_id;
	signed char npc_id;
	signed char group_pos;
	/* Offset 0x8b */
	int32_t heal_timer;
	int32_t staffspell_timer;
	signed char recipe_id;
	signed char recipe_timer;
	signed char ruhe_koerper;
	signed char blind_timer;
	signed char ecliptifactus_timer;
	signed char saftkraft;
	signed char fireban;
	signed char invisible;
	signed char sprite_no;
	signed char alchemy_inn_id;
	signed short escape_position;
	signed char jail;
	/* Offset 0xa0 */
	signed char axxeleratus;
	signed char drunk;
	signed char unkn10[8];
	signed char flags1;
	signed char flags2;
	signed short unkn11;
	/* Not figured out yet, but reserve space */
	signed char sick[8][5];
	signed char poison[10][5];

	/* Offset 0x108 */
	signed char skills[52];
	signed char skill_incs;
	/* Offset 0x13d */
	signed char spells[86];
	signed char spell_incs;
	signed char spell_school;
	signed char staff_level;

	/* Offset 0x196 */
	/* Not figured out yet, but reserve space */
	signed char inventory[23][14];
	signed short load;

	unsigned char pic[1024];
};

STATIC_ASSERT(sizeof(struct struct_hero) == 1754, struct_hero_needs_to_be_1754_bytes);

#pragma pack ()
#endif
