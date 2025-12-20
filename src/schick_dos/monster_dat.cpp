/* Use
 *
 * gcc -c monster_dat.cpp -o monster_dat.o
 * objcopy -O binary monster_dat.o MONSTER.DAT
 *
 * to create the file MONSTER.DAT which can be packed into SCHICK.DAT.
 */


#include "v302de.h"
#include "common.h"
#include "datseg.h"

static enum {
	D4_BUG = 0x0f
};

struct monster monster_dat[79] = {
	{ MONSTER_ID_NONE,
		0,
		0,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0,
		0,
		0,
		0,
		0x0000,
		0x0000,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0x0000,
		0,
		0x0000,
		0
	},

	{ // 1
		MONSTER_ID_OGER,                    // monster_id
		ACTOR_SPRITE_ID_OGER,               // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 17),       // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 1),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 0),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 3, 3),        // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 12),       // attrib_kk
		DICE_TEMPLATE_PACK(1, 20, 30),      // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(1, 6, -4),       // mr
		20,                                 // first_ap
		1,                                  // attacks
		9,                                  // at
		3,                                  // pa
		DICE_TEMPLATE_PACK(2, 6, 6),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		10,                                 // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		4,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 2
		MONSTER_ID_ORK__LVL_1,              // monster_id
		ACTOR_SPRITE_ID_ORK,                // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 4),        // attrib_mu
		DICE_TEMPLATE_PACK(1, 3, 4),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 3, 4),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 3, 6),        // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 5),        // attrib_kk
		DICE_TEMPLATE_PACK(3, 6, 5),        // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(4, 6, -28),      // mr
		8,                                  // first_ap
		1,                                  // attacks
		9,                                  // at
		5,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 2),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		2                                   // le_flee
	},

	{ // 3
		MONSTER_ID_GOBLIN__LVL_1,           // monster_id
		ACTOR_SPRITE_ID_GOBLIN,             // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 2),        // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 3),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 0),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 4),        // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 10),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(1, 6, -5),       // mr
		5,                                  // first_ap
		1,                                  // attacks
		7,                                  // at
		6,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 2),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		7,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		2                                   // le_flee
	},

	{ // 4
		MONSTER_ID_HESHTHOT__LVL_1,         // monster_id
		ACTOR_SPRITE_ID_HESHTHOT,           // sprite_id
		0,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(20),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		30,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 0),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		1,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 5
		MONSTER_ID_SAEBELZAHNTIGER,         // monster_id
		ACTOR_SPRITE_ID_TIGER,              // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(22),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK(1, 20, 35),      // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(-2),    // mr
		50,                                 // first_ap
		2,                                  // attacks
		15,                                 // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(3, 6, 2),        // dam1
		DICE_TEMPLATE_PACK(1, 6, 5),        // dam2
		13,                                 // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		1                                   // le_flee
	},

	{ // 6
		MONSTER_ID_SKELETT__LVL_1,          // monster_id
		ACTOR_SPRITE_ID_SKELETT,            // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(15),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(12),    // mr
		18,                                 // first_ap
		1,                                  // attacks
		7,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 7
		MONSTER_ID_WALDSCHRAT,              // monster_id
		ACTOR_SPRITE_ID_WALDSCHRAT,         // sprite_id
		6,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 17),       // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 2),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 3, 4),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 12),       // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 10),       // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 20),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(1, 6, 6),        // mr
		25,                                 // first_ap
		1,                                  // attacks
		14,                                 // at
		6,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 6),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		11,                                 // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 8
		MONSTER_ID_MUMIE__LVL_1,            // monster_id
		ACTOR_SPRITE_ID_MUMIE,              // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(35),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(15),    // mr
		23,                                 // first_ap
		1,                                  // attacks
		7,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 9
		MONSTER_ID_HARPYIE,                 // monster_id
		ACTOR_SPRITE_ID_HARPYIE,            // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(18),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(40),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		30,                                 // first_ap
		1,                                  // attacks
		15,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		15,                                 // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 10
		MONSTER_ID_WALDSPINNE,              // monster_id
		ACTOR_SPRITE_ID_SPINNE,             // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(7),     // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 15),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(7),     // mr
		8,                                  // first_ap
		1,                                  // attacks
		9,                                  // at
		0,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 1),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		3,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		2,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 11
		MONSTER_ID_GROSSE_SCHROETER,        // monster_id
		ACTOR_SPRITE_ID_KAEFER,             // sprite_id
		5,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(40),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		18,                                 // first_ap
		2,                                  // attacks
		8,                                  // at
		4,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 1),        // dam1
		DICE_TEMPLATE_PACK(1, 6, 1),        // dam2
		3,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		2,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 12
		MONSTER_ID_PIRAT__LVL_1,            // monster_id
		ACTOR_SPRITE_ID_PIRAT,              // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(30),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -5), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -5),       // mr
#endif
		20,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		8                                   // le_flee
	},

	{ // 13
		MONSTER_ID_STEUERMANN,              // monster_id
		ACTOR_SPRITE_ID_PIRAT,              // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -3), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -3),       // mr
#endif
		30,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		6                                   // le_flee
	},

	{ // 14
		MONSTER_ID_KAPITAEN,                // monster_id
		ACTOR_SPRITE_ID_PIRAT,              // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -2), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -2),       // mr
#endif
		30,                                 // first_ap
		1,                                  // attacks
		12,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		4,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		2,                                  // throws
		DICE_TEMPLATE_PACK(1, 6, 1),        // throw_dam
		3                                   // le_flee
	},

	{ // 15
		MONSTER_ID_STREUNER__LVL_1,         // monster_id
		ACTOR_SPRITE_ID_STREUNER__MALE,     // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(30),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -3), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -3),       // mr
#endif
		20,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		10                                  // le_flee
	},

	{ // 16
		MONSTER_ID_ZOMBIE,                  // monster_id
		ACTOR_SPRITE_ID_ZOMBIE,             // sprite_id
		0,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(20),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		15,                                 // first_ap
		1,                                  // attacks
		7,                                  // at
		0,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 2),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		2,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 17
		MONSTER_ID_HESHTHOT__LVL_2,         // monster_id
		ACTOR_SPRITE_ID_HESHTHOT,           // sprite_id
		0,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(20),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		30,                                 // first_ap
		2,                                  // attacks
		12,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(2, 6, 0),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		1,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 18
		MONSTER_ID_DRUIDE__LVL_2,           // monster_id
		ACTOR_SPRITE_ID_DRUIDE__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 30),       // le
		DICE_TEMPLATE_PACK(1, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(4),     // mr
		20,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		0,                                  // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		10                                  // le_flee
	},

	{ // 19
		MONSTER_ID_DRUIDE__LVL_3,           // monster_id
		ACTOR_SPRITE_ID_DRUIDE__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK(2, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(5),     // mr
		25,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		1,                                  // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		10                                  // le_flee
	},

	{ // 20
		MONSTER_ID_THORWALER__LVL_1,        // monster_id
		ACTOR_SPRITE_ID_THORWALER__MALE,    // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(30),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -5), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -5),       // mr
#endif
		20,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 21
		MONSTER_ID_THORWALER__LVL_3,        // monster_id
		ACTOR_SPRITE_ID_THORWALER__MALE,    // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -3), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -3),       // mr
#endif
		25,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 22
		MONSTER_ID_THORWALER__LVL_5,        // monster_id
		ACTOR_SPRITE_ID_THORWALER__MALE,    // sprite_id
		4,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(4, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(0),     // mr
		30,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		5,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 23
		MONSTER_ID_THORWALER__LVL_6,        // monster_id
		ACTOR_SPRITE_ID_THORWALER__MALE,    // sprite_id
		4,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(5, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(0),     // mr
		35,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		6,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 24
		MONSTER_ID_GOBLIN__LVL_2,           // monster_id
		ACTOR_SPRITE_ID_GOBLIN,             // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 3),        // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 3),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 0),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 5),        // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 15),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(1, 6, -4),       // mr
		10,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 2),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		7,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 25
		MONSTER_ID_GOBLIN__LVL_3,           // monster_id
		ACTOR_SPRITE_ID_GOBLIN,             // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 4),        // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 4),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 1),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 9),        // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 6),        // attrib_kk
		DICE_TEMPLATE_PACK(2, 20, 0),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(1, 6, -2),       // mr
		15,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		7,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 26
		MONSTER_ID_ORK__LVL_2,              // monster_id
		ACTOR_SPRITE_ID_ORK,                // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 5),        // attrib_mu
		DICE_TEMPLATE_PACK(1, 3, 4),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 3, 4),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 3, 7),        // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(4, 6, 10),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(4, 6, -22),      // mr
		10,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 2),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 27
		MONSTER_ID_ORK__LVL_3,              // monster_id
		ACTOR_SPRITE_ID_ORK,                // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_mu
		DICE_TEMPLATE_PACK(1, 3, 4),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 3, 5),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 3, 7),        // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 10),       // attrib_kk
		DICE_TEMPLATE_PACK(5, 6, 15),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(4, 6, -16),      // mr
		20,                                 // first_ap
		1,                                  // attacks
		14,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 2),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 28
		MONSTER_ID_PIRAT__LVL_2,            // monster_id
		ACTOR_SPRITE_ID_PIRAT,              // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -4), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -4),       // mr
#endif
		25,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		10                                  // le_flee
	},

	{ // 29
		MONSTER_ID_PIRAT__LVL_3,            // monster_id
		ACTOR_SPRITE_ID_PIRAT,              // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -2), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -2),       // mr
#endif
		30,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		1,                                  // throws
		DICE_TEMPLATE_PACK(1, 6, 1),        // throw_dam
		7                                   // le_flee
	},

	{ // 30
		MONSTER_ID_STREUNER__LVL_2,         // monster_id
		ACTOR_SPRITE_ID_STREUNER__MALE,     // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -2), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -2),       // mr
#endif
		25,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 31
		MONSTER_ID_STREUNER__LVL_3,         // monster_id
		ACTOR_SPRITE_ID_STREUNER__MALE,     // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -1), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -1),       // mr
#endif
		28,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 32
		MONSTER_ID_STREUNER__LVL_4,         // monster_id
		ACTOR_SPRITE_ID_STREUNER__MALE,     // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 9),        // attrib_kk
		DICE_TEMPLATE_PACK(3, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -2), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -2),       // mr
#endif
		22,                                 // first_ap
		1,                                  // attacks
		12,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		4,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		10,                                 // shots
		DICE_TEMPLATE_PACK(1, 6, 4),        // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 33
		MONSTER_ID_SKELETT__LVL_2,          // monster_id
		ACTOR_SPRITE_ID_SKELETT,            // sprite_id
		4,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(25),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(12),    // mr
		18,                                 // first_ap
		1,                                  // attacks
		7,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 34
		MONSTER_ID_FOLTERKNECHT,            // monster_id
		ACTOR_SPRITE_ID_RAEUBER,            // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(41),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -1), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -1),       // mr
#endif
		30,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		12,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 35
		MONSTER_ID_GRIMWOLF__LVL_1,         // monster_id
		ACTOR_SPRITE_ID_WOLF,               // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(9),     // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(15),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(0),     // mr
		6,                                  // first_ap
		1,                                  // attacks
		9,                                  // at
		4,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 1),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		12,                                 // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 36
		MONSTER_ID_GRIMWOLF__LVL_3,         // monster_id
		ACTOR_SPRITE_ID_WOLF,               // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(25),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(0),     // mr
		6,                                  // first_ap
		1,                                  // attacks
		10,                                 // at
		4,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 1),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		12,                                 // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 37
		MONSTER_ID_HOEHLENSPINNE__LVL_1,    // monster_id
		ACTOR_SPRITE_ID_SPINNE,             // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(25),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(6),     // mr
		18,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		0,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		4,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		2,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 38
		MONSTER_ID_HOEHLENSPINNE__LVL_2,    // monster_id
		ACTOR_SPRITE_ID_SPINNE,             // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(35),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(6),     // mr
		20,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		4,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		2,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 39
		MONSTER_ID_HOEHLENSPINNE__LVL_3,    // monster_id
		ACTOR_SPRITE_ID_SPINNE,             // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(45),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(6),     // mr
		25,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		4,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		2,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 40
		MONSTER_ID_KRIEGER,                 // monster_id
		ACTOR_SPRITE_ID_KRIEGER__MALE,      // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(30),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -1), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -1),       // mr
#endif
		20,                                 // first_ap
		1,                                  // attacks
		9,                                  // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		6                                   // le_flee
	},

	{ // 41
		MONSTER_ID_STADTGARDIST__LVL_3,     // monster_id
		ACTOR_SPRITE_ID_KRIEGER__MALE,      // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(0),     // mr
		25,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 42
		MONSTER_ID_STADTGARDIST__LVL_4,     // monster_id
		ACTOR_SPRITE_ID_KRIEGER__MALE,      // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(3, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(2),     // mr
		28,                                 // first_ap
		1,                                  // attacks
		12,                                 // at
		11,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		4,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 43
		MONSTER_ID_STADTGARDIST__LVL_5,     // monster_id
		ACTOR_SPRITE_ID_KRIEGER__MALE,      // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kk
		DICE_TEMPLATE_PACK(4, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(4),     // mr
		30,                                 // first_ap
		1,                                  // attacks
		14,                                 // at
		11,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 5),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		5,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		2                                   // le_flee
	},

	{ // 44
		MONSTER_ID_KRIEGSVETERAN,           // monster_id
		ACTOR_SPRITE_ID_KRIEGER__MALE,      // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(15),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(15),    // attrib_kk
		DICE_TEMPLATE_PACK(10, 6, 30),      // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(6),     // mr
		40,                                 // first_ap
		1,                                  // attacks
		16,                                 // at
		14,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 6),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		10,                                 // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		1                                   // le_flee
	},

	{ // 45
		MONSTER_ID_MUMIE__LVL_2,            // monster_id
		ACTOR_SPRITE_ID_MUMIE,              // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(40),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(15),    // mr
		23,                                 // first_ap
		1,                                  // attacks
		7,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 5),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 46
		MONSTER_ID_MAGIER__LVL_3,           // monster_id
		ACTOR_SPRITE_ID_MAGIER__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 25),       // le
		DICE_TEMPLATE_PACK(2, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(5),     // mr
		25,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		3,                                  // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		10                                  // le_flee
	},

	{ // 47
		MONSTER_ID_MAGIER__LVL_6,           // monster_id
		ACTOR_SPRITE_ID_MAGIER__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kk
		DICE_TEMPLATE_PACK(5, 6, 25),       // le
		DICE_TEMPLATE_PACK(5, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		40,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		5,                                  // spellbook_id
		6,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 48
		MONSTER_ID_ZAUBERER__LVL_3,         // monster_id
		ACTOR_SPRITE_ID_ZAUBERER,           // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 25),       // le
		DICE_TEMPLATE_PACK(2, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(5),     // mr
		25,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		6,                                  // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 49
		MONSTER_ID_ZAUBERER__LVL_6,         // monster_id
		ACTOR_SPRITE_ID_ZAUBERER,           // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kk
		DICE_TEMPLATE_PACK(5, 6, 25),       // le
		DICE_TEMPLATE_PACK(5, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		40,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		7,                                  // spellbook_id
		6,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 50
		MONSTER_ID_ZWERG,                   // monster_id
		ACTOR_SPRITE_ID_ZWERG__MALE,        // sprite_id
		4,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(15),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(60),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(6),     // mr
		30,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		11,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 6),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		4,                                  // level
		2,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		6                                   // le_flee
	},

	{ // 51
		MONSTER_ID_LINDWURM,                // monster_id
		ACTOR_SPRITE_ID_HARPYIE,            // sprite_id
		6,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(18),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(17),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(-106),  // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(18),    // mr
		-6,                                 // first_ap
		3,                                  // attacks
		16,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(2, 6, 5),        // dam1
		DICE_TEMPLATE_PACK(2, 6, 5),        // dam2
		18,                                 // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		4,                                  // level
		5,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 52
		MONSTER_ID_HOEHLENDRACHE,           // monster_id
		ACTOR_SPRITE_ID_HARPYIE,            // sprite_id
		6,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(18),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(35),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(80),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(12),    // mr
		80,                                 // first_ap
		1,                                  // attacks
		17,                                 // at
		12,                                 // pa
		DICE_TEMPLATE_PACK(2, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		5,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 53
		MONSTER_ID_KULTIST__LVL_2,          // monster_id
		ACTOR_SPRITE_ID_STREUNER__MALE,     // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -2), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -2),       // mr
#endif
		25,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		10                                  // le_flee
	},

	{ // 54
		MONSTER_ID_KULTIST__LVL_3__1,       // monster_id
		ACTOR_SPRITE_ID_STREUNER__MALE,     // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -1), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -1),       // mr
#endif
		28,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		8                                   // le_flee
	},

	{ // 55
		MONSTER_ID_KULTIST__LVL_3__2,       // monster_id
		ACTOR_SPRITE_ID_THORWALER__MALE,    // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -3), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -3),       // mr
#endif
		25,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 56
		MONSTER_ID_KULTIST__LVL_4,          // monster_id
		ACTOR_SPRITE_ID_KRIEGER__MALE,      // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(3, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(2),     // mr
		28,                                 // first_ap
		1,                                  // attacks
		12,                                 // at
		11,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		4,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		10,                                 // shots
		DICE_TEMPLATE_PACK(1, 6, 4),        // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 57
		MONSTER_ID_PRIESTER__LVL_2,         // monster_id
		ACTOR_SPRITE_ID_MAGIER__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 30),       // le
		DICE_TEMPLATE_PACK(1, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(4),     // mr
		20,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		8,                                  // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		10                                  // le_flee
	},

	{ // 58
		MONSTER_ID_PRIESTER__LVL_3,         // monster_id
		ACTOR_SPRITE_ID_MAGIER__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK(2, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(5),     // mr
		25,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		7,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		9,                                  // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 59
		MONSTER_ID_HOHEPRIESTER,            // monster_id
		ACTOR_SPRITE_ID_MAGIER__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kk
		DICE_TEMPLATE_PACK(5, 6, 25),       // le
		DICE_TEMPLATE_PACK(5, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		40,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		10,                                 // spellbook_id
		6,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		2                                   // le_flee
	},

	{ // 60
		MONSTER_ID_ESTORIK,                 // monster_id
		ACTOR_SPRITE_ID_THORWALER__MALE,    // sprite_id
		4,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(5, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(0),     // mr
		35,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		6,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 61
		MONSTER_ID_PIRATENBOSS,             // monster_id
		ACTOR_SPRITE_ID_PIRAT,              // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kk
		DICE_TEMPLATE_PACK(5, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(1),     // mr
		35,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 5),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		6,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		5,                                  // throws
		DICE_TEMPLATE_PACK(1, 6, 1),        // throw_dam
		1                                   // le_flee
	},

	{ // 62
		MONSTER_ID_PIRAT__LVL_5,            // monster_id
		ACTOR_SPRITE_ID_PIRAT,              // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(15),    // attrib_kk
		DICE_TEMPLATE_PACK(4, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(0),     // mr
		33,                                 // first_ap
		1,                                  // attacks
		12,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		5,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		3,                                  // throws
		DICE_TEMPLATE_PACK(1, 6, 1),        // throw_dam
		3                                   // le_flee
	},

	{ // 63
		MONSTER_ID_MAGIER__LVL_4,           // monster_id
		ACTOR_SPRITE_ID_MAGIER__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(15),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kk
		DICE_TEMPLATE_PACK(3, 6, 25),       // le
		DICE_TEMPLATE_PACK(3, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(6),     // mr
		30,                                 // first_ap
		1,                                  // attacks
		8,                                  // at
		6,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		4,                                  // spellbook_id
		4,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		10                                  // le_flee
	},

	{ // 64
		MONSTER_ID_ZWERGENVETERAN,          // monster_id
		ACTOR_SPRITE_ID_ZWERG__MALE,        // sprite_id
		4,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(15),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(11),    // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(17),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(80),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(8),     // mr
		50,                                 // first_ap
		1,                                  // attacks
		16,                                 // at
		15,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 8),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		10,                                 // level
		2,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		1                                   // le_flee
	},

	{ // 65
		MONSTER_ID_RAEUBER__LVL_2,          // monster_id
		ACTOR_SPRITE_ID_RAEUBER,            // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -4), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -4),       // mr
#endif
		25,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		5                                   // le_flee
	},

	{ // 66
		MONSTER_ID_RAEUBER__LVL_3,          // monster_id
		ACTOR_SPRITE_ID_RAEUBER,            // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kk
		DICE_TEMPLATE_PACK(2, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -2), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -2),       // mr
#endif
		30,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		9,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		3,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 67
		MONSTER_ID_RAEUBER__LVL_5,          // monster_id
		ACTOR_SPRITE_ID_RAEUBER,            // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(15),    // attrib_kk
		DICE_TEMPLATE_PACK(4, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(0),     // mr
		33,                                 // first_ap
		1,                                  // attacks
		12,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		5,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 68
		MONSTER_ID_RAEUBER__LVL_6,          // monster_id
		ACTOR_SPRITE_ID_RAEUBER,            // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kk
		DICE_TEMPLATE_PACK(5, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(3),     // mr
		35,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 5),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		6,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		3,                                  // shots
		DICE_TEMPLATE_PACK(1, 6, 4),        // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		2                                   // le_flee
	},

	{ // 69
		MONSTER_ID_RAEUBER__LVL_8,          // monster_id
		ACTOR_SPRITE_ID_RAEUBER,            // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(15),    // attrib_kk
		DICE_TEMPLATE_PACK(7, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(5),     // mr
		45,                                 // first_ap
		1,                                  // attacks
		15,                                 // at
		12,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 6),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		8,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		6,                                  // shots
		DICE_TEMPLATE_PACK(1, 6, 4),        // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		1                                   // le_flee
	},

	{ // 70
		MONSTER_ID_GORAH,                   // monster_id
		ACTOR_SPRITE_ID_DRUIDE__MALE,       // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kk
		DICE_TEMPLATE_PACK(7, 6, 30),       // le
		DICE_TEMPLATE_PACK(7, 4, 25),       // ae
		DICE_TEMPLATE_PACK_CONSTANT(9),     // mr
		55,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		2,                                  // spellbook_id
		8,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		7                                   // le_flee
	},

	{ // 71
		MONSTER_ID_STEPPENHUND,             // monster_id
		ACTOR_SPRITE_ID_HUND,               // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(5),     // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(4),     // attrib_kk
		DICE_TEMPLATE_PACK(1, 6, 5),        // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(-2),    // mr
		4,                                  // first_ap
		1,                                  // attacks
		5,                                  // at
		2,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 0),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		9,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		3                                   // le_flee
	},

	{ // 72
		MONSTER_ID_ORKCHAMPION,             // monster_id
		ACTOR_SPRITE_ID_ORK,                // sprite_id
		2,                                  // rs
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_mu
		DICE_TEMPLATE_PACK(1, 3, 4),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 3, 5),        // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK(1, 3, 7),        // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK(1, 6, 17),       // attrib_kk
		DICE_TEMPLATE_PACK(9, 6, 15),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK(7, 6, -16),      // mr
		60,                                 // first_ap
		1,                                  // attacks
		18,                                 // at
		14,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 9),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		10,                                 // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 73
		MONSTER_ID_PIRAT__LVL_10,           // monster_id
		ACTOR_SPRITE_ID_PIRAT,              // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(14),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 8),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(17),    // attrib_kk
		DICE_TEMPLATE_PACK(9, 6, 30),       // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(1),     // mr
		45,                                 // first_ap
		1,                                  // attacks
		15,                                 // at
		12,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 8),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		10,                                 // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ // 74
		MONSTER_ID_STADTGARDIST__LVL_2,     // monster_id
		ACTOR_SPRITE_ID_KRIEGER__MALE,      // sprite_id
		3,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(12),    // attrib_mu
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_kl
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ch
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ff
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_ge
		DICE_TEMPLATE_PACK(1, 6, 7),        // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(13),    // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(34),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
#ifndef M302de_ORIGINAL_BUGFIX
		// Original-Bug 57
		DICE_TEMPLATE_PACK(15, D4_BUG, -1), // mr
#else
		DICE_TEMPLATE_PACK(1, 6, -1),       // mr
#endif
		22,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		10,                                 // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		2,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 75
		MONSTER_ID_WALDLOEWE,               // monster_id
		ACTOR_SPRITE_ID_LOEWE,              // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(4),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(20),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(4),     // mr
		10,                                 // first_ap
		1,                                  // attacks
		11,                                 // at
		6,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 4),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		9,                                  // bp
		0,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		4                                   // le_flee
	},

	{ // 76
		MONSTER_ID_SPINNENDAEMON,           // monster_id
		ACTOR_SPRITE_ID_SPINNE,             // sprite_id
		1,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(10),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(45),    // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(6),     // mr
		25,                                 // first_ap
		1,                                  // attacks
		10,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 3),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		4,                                  // bp
		1,                                  // magic
		-1,                                 // spellbook_id
		5,                                  // level
		2,                                  // target_size
		1,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	/* The following is the infamous fleeing Heshthot.
	 * It has only LE 1. By the LE reduction to 5/6 when loading the monster
	 * (which probably was created late in the testing phase of the game),
	 * this value 1 is reduced to LE 0, making the Heshthot flee immediately.
	 * Also, note that "HESHTHOT" is spelled as "HESHTOT" here.
	 * It is not clear if this is a mistake, or if it was the intention to have the modified part "tot" (German: "dead"). */
	{ // 77
		MONSTER_ID_HESHTOT__WEAK,           // monster_id
		ACTOR_SPRITE_ID_HESHTHOT,           // sprite_id
		0,                                  // rs
		DICE_TEMPLATE_PACK_CONSTANT(30),    // attrib_mu
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kl
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ch
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ff
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_ge
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_in
		DICE_TEMPLATE_PACK_CONSTANT(0),     // attrib_kk
		DICE_TEMPLATE_PACK_CONSTANT(1),     // le
		DICE_TEMPLATE_PACK_CONSTANT(0),     // ae
		DICE_TEMPLATE_PACK_CONSTANT(10),    // mr
		30,                                 // first_ap
		1,                                  // attacks
		13,                                 // at
		8,                                  // pa
		DICE_TEMPLATE_PACK(1, 6, 0),        // dam1
		DICE_TEMPLATE_PACK_CONSTANT(0),     // dam2
		8,                                  // bp
		1,                                  // magic
		-1,                                 // spellbook_id
		1,                                  // level
		3,                                  // target_size
		0,                                  // is_animal
		0,                                  // shots
		DICE_TEMPLATE_PACK_CONSTANT(0),     // shot_dam
		0,                                  // throws
		DICE_TEMPLATE_PACK_CONSTANT(0),     // throw_dam
		0                                   // le_flee
	},

	{ -1, // 78
		0,
		0,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0x0000,
		0,
		0,
		0,
		0,
		0x0000,
		0x0000,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0x0000,
		0,
		0x0000,
		0
	}
};
