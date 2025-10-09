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


struct inventory_flags {
	/* inventory position + 0x04 */
	unsigned short broken		:1;
	unsigned short half_empty	:1; /* only used for ITEM_WATERSKIN */
	unsigned short empty		:1; /* only used for ITEM_WATERSKIN. for an empty waterskin, both  'empty' and 'half_empty' flags are set */
	unsigned short magic		:1;
	unsigned short bit4		:1; /* unused */
	unsigned short poison_expurgicum:1; /* flag set if a weapon is poisoned by Expurgicum */
	unsigned short poison_vomicum	:1; /* flag set if a weapon is poisoned by Vomicum */
	unsigned short magic_revealed	:1; /* flag set if it is known to the heroes that the item is magic.
					       obtained from academy in Thorwal or from 'Odem Arcanum' or 'Analues' spell */

	unsigned short bit8		:1; /* unused */
	unsigned short bit9		:1; /* unused */
	unsigned short bit10		:1; /* unused */
	unsigned short bit11		:1; /* unused */
	unsigned short bit12		:1; /* unused */
	unsigned short bit13		:1; /* unused */
	unsigned short bit14		:1; /* unused */
	unsigned short bit15		:1; /* unused */
};

struct inventory {
	/* for future use, currently the HERO_INVENTORY enum is used. */
	signed short item_id; /* +0 */
	signed short quantity; /* +2 */

	struct inventory_flags flags; /* +4 */
	signed char bf; /* +6 */
	signed char rs_lost; /* +7 */

	signed char lighting_timer; /* +8 */
	signed char poison_type; /* +9 */
	signed char nr_poison_charges; /* +10 */
	signed char unused1; /* +11 */

	signed char unused2; /* +12 */
	signed char unused3; /* +13 */
};

STATIC_ASSERT(sizeof(struct inventory) == 14, struct_inventory_needs_to_be_14_bytes);


/**
 *	struct hero_flags - status of the hero
 *	@dead:		1 = dead / 0 = not dead
 *	@asleep:	1 = asleep / 0 = awake
 *	@petrified:	1 = petrified / 0 = not petrified
 *	@brewing:	1 = brewing an alchemical recipe in an inn / 0 = not brewing
 *	@chamaelioni:	1 = 'Chamaelioni' spell active / 0 = spell not active (effect of active 'Chamaelioni': AT-5 for attacker)
 *	@renegade:	1 = renegade (from 'Boeser Blick' spell) / 0 = normal
 *	@unconscious:	1 = unconscious / 0 = conscious
 *	@tied:		1 = hero tied / 0 = not tied (from hero 'Band und Fessel' spell)
 *
 *	@scared:	1 = scared and wants to flee (from 'Horriphobus' spell or Angstgift) / 0 = not scared
 *	@dummy2:	probably unused
 *	@duplicatus:	1 = 'Duplicatus' spell active / spell not active (effect: AT/2 for attacker, applied after the 'Chamaelioni' effect)
 *	@tame:		1 = tame (from enemy 'Bannbaladin' spell) / 0 = normal
 *	@seen_phantom:	1 = hero has seen the horrible phantom in the 'Verlassene Herberge' dungeon which gave MU -3 for 5 hours (same position where the Sphaerenriss can happen).
 *	@gods_pissed:	1 = gods pissed (no more miracles) / 0 = gods normal (activated by praising the nameless god, deactivated by destroying the statue of the nameless god)
 *	@transformed:	1 = hero is transformed (all positive attributes -1, from the cursed chest on the Totenschiff) / 0 not transformed (can be cured by 'Verwandlung beenden' spell or Praios/Hesinde miracle)
 *	@encouraged:	1 = MU increased by 3 (seg082.cpp, probably a dungeon event) / 0 = attribute back to normal.
 */
struct hero_flags {
	/* hero + 0xaa */
	unsigned short dead		:1;
	unsigned short asleep		:1;
	unsigned short petrified	:1;
	unsigned short brewing		:1;
	unsigned short chamaelioni	:1;
	unsigned short renegade		:1;
	unsigned short unconscious	:1;
	unsigned short tied		:1;
	/* hero + 0xab */
	unsigned short scared		:1;
	unsigned short dummy2		:1;
	unsigned short duplicatus	:1;
	unsigned short tame		:1;
	unsigned short seen_phantom	:1;
	unsigned short gods_pissed	:1;
	unsigned short transformed	:1;
	unsigned short encouraged	:1;
};

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
	signed char bp_left;
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
	signed char le_malus;
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
	//signed char flags1;
	//signed char flags2;
	struct hero_flags flags;
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
	struct inventory inventory[23];
	signed short load;

	unsigned char pic[1024];
};

STATIC_ASSERT(sizeof(struct struct_hero) == 1754, struct_hero_needs_to_be_1754_bytes);

#pragma pack ()
#endif
