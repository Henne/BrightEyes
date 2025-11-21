#ifndef HERO_H
#define HERO_H

#if __cpp_static_assert  
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
	signed char normal; /* the base attribute value without any modifications */
	signed char current; /* the current attribute value, arising from base value + modifications */
	signed char mod; /* dummy: seems to be constant 0 and read only, essentially */
};

STATIC_ASSERT(sizeof(struct struct_attribs) == 3, struct_attribs_needs_to_be_3_bytes);


struct inventory_flags {
	/* inventory position + 0x04 */
	unsigned short broken		:1;
	unsigned short half_empty	:1; /* only used for ITEM_WASSERSCHLAUCH */
	unsigned short empty		:1; /* only used for ITEM_WASSERSCHLAUCH. for an empty waterskin, both  'empty' and 'half_empty' flags are set */
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
	/* describes an item in a single inventory slot of a hero */
	/* https://github.com/shihan42/BrightEyesWiki/wiki/CHR-NPC#inventarslots */
	signed short item_id; /* +0 */
	signed short quantity; /* +2 */ /* for stackable items: number of items in the stack; for items with magic charges: number of charges left */

	struct inventory_flags flags; /* +4 */
	signed char bf; /* +6 */ /* Bruchfaktor. -99 means unbreakable */
	signed char rs_lost; /* +7 */ /* so far only seen for body armour. (from 'Ignifaxius' spell or from traps in DNG03 (Spinnenhoehle)) */

	signed char lighting_timer; /* +8 */ /* for burning torch/lantern: number of remaining time, unit: 15 minutes */
	signed char poison_id; /* +9 */ /* for poisoned weapon: poison ID according to enum POISON_ID_... (0-kein Gift, 1-Schurinknollengift, 2-Arax, 3-Angstgift, 4-Schlafgift, 5-Goldleim, 6-Krötenschemelgift, 7-Lotosgift, 8-Kukris, 9-Bannstaub, 10-Expurgicum, 11-Vomicum). */
	signed char num_poison_charges; /* +10 */ /* for poisoned weapon: number of remaining poison charges (= successful attacks). */
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

struct hero_affliction {
	/* used for diseases and poisons */
	signed char status; /* {-1,0,1}. see enum DISEASE_STATUS and POISON_STATUS */
	signed char time_counter; /* Number of days (for diseases) or of units of 5 minutes (for poisons)
				   * since the status turned to -1 (diseased/poisoned) or to 1 (recover).
				   * Most of the time, the counter is reset on a status change, but not always. */
	/* the following three bytes log the negative effects which the disease/poison has already caused.
	 * Exact meaning depends on the concrete disease/poison (encoded by enums DISEASE_ID_... and POISON_ID_...) */
	signed char log_1;
	signed char log_2;
	signed char log_3;
};

struct struct_hero {
	/* see https://github.com/shihan42/BrightEyesWiki/wiki/CHR-NPC */
	/* Offset 0x00 */
	char name[16];
	/* Offset 0x10 */
	char alias[16];
	/* Offset 0x20 */
	signed char num_filled_inv_slots; /* number of filled inventory slots (both knapsack and equipped items) */
	signed char typus; /* see enum HERO_TYPE_* */
	signed char sex;
	unsigned char height; /* unit: cm */
	signed short weight; /* unit: ounces */
	signed char god; /* see enum GOD_* */
	signed char level;
	int32_t ap; /* Abenteuerpunkte = experience points */
	int32_t money; /* unit: heller */
	/* Offset 0x30 */
	signed char rs_bonus; /* rs = Ruestungsschutz. */
	signed char rs_bonus_dummy; /* read-only? */
	signed char rs_be; /* Ruestungsschutzbehinderung */
	signed char fight_bp_left; /* bp = Bewegungspunkte */
	struct struct_attribs attrib[14]; /* see enum ATTRIB_* */
	/* Offset 0x5e */
	signed short le_max;
	signed short le;
	/* Offset 0x60 */
	signed short ae_max;
	signed short ae;
	signed char mr; /* mr = Magieresistenz */

	signed char atpa_base; /* AT-PA-base value; at = Attacke, pa = Parade. */
	signed char at_talent_bonus[7]; /* talent-based AT bonus in melee weapon categories as in enum WEAPON_TYPE_* */
	signed char pa_talent_bonus[7]; /* talent-based PA bonus in melee weapon categories as in enum WEAPON_TYPE_* */
	signed char weapon_at_mod; /* AT bonus the equipped weapon (may be negative) */
	signed char weapon_pa_mod; /* PA bonus of the equipped weapon (may be negative) */
	signed char weapon_type; /* type of the equipped weapon */
	signed char fight_atpa_mod; /* last used attack mode in fight. +2: aggressive (AT+2, PA-2); 0: normal (AT+0, PA+0); -2: cautious (AT-2, PA+2) */
	signed char le_max_malus; /* le_max lost from starvation */
	signed char timer_id;
	signed char start_gear__monolith_reward;
	  /* has been picked up at a temple and therefore got the initial gear set: 1 = true, 0 = false. 
	   * Bit 1 is used as a flag if the hero got the IN attribute bonus at the black eye at the Monolith (Einsiedlersee <-> Einsiedlersee, tevent135).
	   * Bits 2--7 appear to be unused.
	   * This is strange as the 'start gear' functionality is done on byte level (affecting all bits together).
	   * Note that this is not a bug because it it should never happen in the game that bit 0 is unset and bit 1 is set.
	   * Maybe the bit 1 functionality was actually meant to be applied to the second byte of hero_flags, whose bit 1 is unused. Who knows. */
	signed char herbs; /* none = 0, Belmart = 1, Menchalkaktus = 2 */
	signed char hunger_timer; /* timer for no-hunger-miracle */
	signed char hunger; /* percentage */
	/* Offset 0x80 */
	signed char thirst; /* percentage */
	signed char fighter_id;
	signed char viewdir;
	signed char actions; /* corresponds to enemy_sheet.attacks */
	signed char action_id; /* last fight action */
	signed char spell_id; /* last spell in fight */
	signed char target_object_id; /* last enemy in fight */
	signed char group_id; /* id of the group the hero  */
	signed char temple_id;
	signed char npc_id; /* see enum NPC_* */
	signed char slot_pos; /* position in the hero slots at the bottom, counted from 1. value 0: not in party (consisting of all groups) */
	/* Offset 0x8b */
	int32_t heal_timer;
	int32_t staffspell_timer;
	signed char recipe_id; /* id of the alchemic recipe the hero is brewing */
	signed char recipe_timer; /* timer till completion of alchemical brewing process, in days */
	signed char ruhe_koerper; /* 1 = Ruhe Koerper spell is active */
	signed char blind_timer; /* remaining fight rounds the hero is blinded from 'Blitz' spell */
	signed char ecliptifactus_timer; /* remaining fight rounds the hero is shadowed from 'Ecliptifactus' spell */
	signed char saftkraft; /* stores extra damage of spell 'Saft, Kraft, Monstermacht' */
	signed char fireban; /* 1 = 'Feuerbann' spell is active, 0 = inactive */ /* Original-Bug? Effect seems not to be implemented. */
	signed char invisible; /* 1 = 'Visibili' spell is active, 0 = inactive */
	signed char sprite_id; /* fight gfx of hero, depending on typus and sex.
                                * 1: Gaukler, male. 2: Jaeger, male, ..., 10: any Elf, male,
				* 11: Gaukler, female, 12: Jaeger, female, ..., 20: any Elf, female. */
	signed char alchemy_inn_id; /* the id of the inn where the hero is doing alchemy */
	signed short escape_position; /* the dungeon square the hero escaped to in a fight. read from fig_escape_position */
	signed char jail; /* 1 = true, i.e. hero is in prison (from breaking into a house), 0 = false */
	/* Offset 0xa0 */
	signed char axxeleratus; /* 1 = 'Axxeleratus' spell is active, 0 = inactive */
	signed char drunk; /* 1 = true (hero drunk), 0 = false */
	signed char unkn10[8];
	//signed char flags1;
	//signed char flags2;
	struct hero_flags flags;
	signed short unkn11;
	/* Not figured out yet, but reserve space */
	struct hero_affliction disease[8]; /* 40 = 8 * 5 bytes */ /* for the index, see enum DISEASE_ID. Note that no disease has disease_id == 0, so the first 5 bytes are unused. */
	struct hero_affliction poison[10]; /* 50 = 10 * 5 bytes */ /* for the index, see enum POISON_ID. NOTE that no poison has poison_id == 0, so the first 5 bytes are unused. */

	/* Offset 0x108 */
	signed char talents[52]; /* see enum TA_* */
	/* The first entry does not belong to an actual spell talent and is apparently unused. */
	signed char saved_talent_increases;
	/* Offset 0x13d */
	signed char spells[86]; /* see enum SP_* */
	signed char saved_spell_increases;
	signed char spell_school; /* only for mages */
	signed char staff_level; /* only for mages */

	/* Offset 0x196 */
	/* Not figured out yet, but reserve space */
	struct inventory inventory[23]; /* 23 inventory slots according to enum HERO_INVENTORY_SLOT_* */
	signed short load;

	unsigned char pic[1024]; /* 32 x 32 pixels, 8 bpp */
};

STATIC_ASSERT(sizeof(struct struct_hero) == 1754, struct_hero_needs_to_be_1754_bytes);

#pragma pack ()
#endif
