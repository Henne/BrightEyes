/* Use
 *
 * gcc -c items_dat.cpp -o items_dat.o
 * objcopy -O binary items_dat.o ITEMS.DAT
 *
 * to create the file ITEMS.DAT which can be packed into SCHICK.DAT.
 */

#include "v302de.h"
#include "common.h"
#include "datseg.h"

#define NUTRITION_VALUE(val) (val) // to mark nutrition values entries item_type_stats_id

item_stats items_dat[ITEM_ID__END + 1] = {
	{ // ITEM_ID_NONE == 0
		0
	},
	{ // ITEM_ID_SCHWERT == 1
		ITEM_SPRITE_ID_SCHWERT,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHWERT,
		WEAPON_STATS_ID_SCHWERT,
		80, // weight
		10, // price_unit
		85, // price
		19, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KNUEPPEL == 2
		ITEM_SPRITE_ID_KNUEPPEL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_KNUEPPEL,
		60, // weight
		10, // price_unit
		5, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SAEBEL == 3
		ITEM_SPRITE_ID_SAEBEL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_SAEBEL,
		60, // weight
		10, // price_unit
		60, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MESSER == 4
		ITEM_SPRITE_ID_MESSER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_MESSER,
		10, // weight
		10, // price_unit
		5, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SPEER == 5
		ITEM_SPRITE_ID_SPEER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SPEER,
		WEAPON_STATS_ID_SPEER,
		80, // weight
		10, // price_unit
		30, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KURZSCHWERT == 6
		ITEM_SPRITE_ID_KURZSCHWERT,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHWERT,
		WEAPON_STATS_ID_KURZSCHWERT,
		40, // weight
		10, // price_unit
		45, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHILD == 7
		ITEM_SPRITE_ID_SCHILD,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_LEFT_HAND,
		ARMOR_STATS_ID_SCHILD,
		140, // weight
		10, // price_unit
		25, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KRIEGSBEIL__SPECIAL == 8
		ITEM_SPRITE_ID_KRIEGSBEIL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_AXT,
		WEAPON_STATS_ID_KRIEGSBEIL__SPECIAL,
		120, // weight
		10, // price_unit
		45, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KURZBOGEN == 9
		ITEM_SPRITE_ID_KURZBOGEN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHUSSWAFFE,
		WEAPON_STATS_ID_KURZBOGEN,
		20, // weight
		10, // price_unit
		45, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_PFEIL == 10
		ITEM_SPRITE_ID_PFEIL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_WAFFENLOS,
		WEAPON_STATS_ID_PROJECTILE,
		4, // weight
		1, // price_unit
		5, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_STREITAXT == 11
		ITEM_SPRITE_ID_STREITAXT,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_ZWEIHAENDER,
		WEAPON_STATS_ID_STREITAXT,
		150, // weight
		10, // price_unit
		110, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ARMBRUST == 12
		ITEM_SPRITE_ID_ARMBRUST,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHUSSWAFFE,
		WEAPON_STATS_ID_ARMBRUST,
		200, // weight
		10, // price_unit
		123, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BOLZEN == 13
		ITEM_SPRITE_ID_BOLZEN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_WAFFENLOS,
		WEAPON_STATS_ID_PROJECTILE,
		5, // weight
		1, // price_unit
		8, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DOLCH == 14
		ITEM_SPRITE_ID_DOLCH,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_DOLCH,
		20, // weight
		10, // price_unit
		20, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_EISENSCHILD == 15
		ITEM_SPRITE_ID_EISENSCHILD,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_LEFT_HAND,
		ARMOR_STATS_ID_EISENSCHILD,
		200, // weight
		10, // price_unit
		45, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WURFBEIL == 16
		ITEM_SPRITE_ID_WURFBEIL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_WURFWAFFE,
		WEAPON_STATS_ID_WURFBEIL,
		120, // weight
		10, // price_unit
		35, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WURFSTERN == 17
		ITEM_SPRITE_ID_WURFSTERN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_WURFWAFFE,
		WEAPON_STATS_ID_WURFSTERN,
		15, // weight
		10, // price_unit
		25, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ZWEIHAENDER == 18
		ITEM_SPRITE_ID_ZWEIHAENDER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_ZWEIHAENDER,
		WEAPON_STATS_ID_ZWEIHAENDER,
		160, // weight
		10, // price_unit
		135, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LANGBOGEN == 19
		ITEM_SPRITE_ID_LANGBOGEN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHUSSWAFFE,
		WEAPON_STATS_ID_LANGBOGEN,
		30, // weight
		10, // price_unit
		50, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MORGENSTERN == 20
		ITEM_SPRITE_ID_MORGENSTERN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_MORGENSTERN,
		120, // weight
		10, // price_unit
		50, // price
		8, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_VULKANGLASDOLCH == 21
		ITEM_SPRITE_ID_VULKANGLASDOLCH,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_VULKANGLASDOLCH,
		30, // weight
		10, // price_unit
		50, // price
		1, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_FACKEL__LIT == 22
		ITEM_SPRITE_ID_FACKEL__LIT,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		20, // weight
		1, // price_unit
		5, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BIER == 23
		ITEM_SPRITE_ID_BIER,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		1, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		NUTRITION_TYPE_DRINK,
		NUTRITION_VALUE(40),
		40, // weight
		1, // price_unit
		1, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WURFHAKEN == 24
		ITEM_SPRITE_ID_WURFHAKEN,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		30, // weight
		10, // price_unit
		10, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LATERNE__UNLIT == 25
		ITEM_SPRITE_ID_LATERNE__UNLIT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_LIGHTING,
		15, // weight
		10, // price_unit
		3, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BRECHEISEN == 26
		ITEM_SPRITE_ID_BRECHEISEN,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		90, // weight
		10, // price_unit
		10, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HAMMER == 27
		ITEM_SPRITE_ID_HAMMER,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		20, // weight
		10, // price_unit
		6, // price
		17, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ANGELHAKEN == 28
		ITEM_SPRITE_ID_ANGELHAKEN,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		1, // weight
		1, // price_unit
		2, // price
		13, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHRIFTROLLE == 29
		ITEM_SPRITE_ID_SCHRIFTROLLE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		30, // weight
		10, // price_unit
		40, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WASSERSCHLAUCH == 30
		ITEM_SPRITE_ID_WASSERSCHLAUCH,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		1, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		NUTRITION_TYPE_DRINK,
		NUTRITION_VALUE(40),
		40, // weight
		10, // price_unit
		2, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GLASFLASCHE == 31
		ITEM_SPRITE_ID_GLASFLASCHE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		12, // price
		5, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_STRICKLEITER == 32
		ITEM_SPRITE_ID_STRICKLEITER,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		70, // weight
		10, // price_unit
		12, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WURFAXT == 33
		ITEM_SPRITE_ID_WURFBEIL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_WURFWAFFE,
		WEAPON_STATS_ID_WURFAXT,
		120, // weight
		10, // price_unit
		35, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MESSINGSPIEGEL == 34
		ITEM_SPRITE_ID_MESSINGSPIEGEL,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		20, // weight
		10, // price_unit
		12, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DIETRICHE == 35
		ITEM_SPRITE_ID_DIETRICHE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		15, // weight
		10, // price_unit
		120, // price
		3, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHREIBZEUG == 36
		ITEM_SPRITE_ID_SCHREIBZEUG,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		15, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HARFE == 37
		ITEM_SPRITE_ID_HARFE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		40, // weight
		10, // price_unit
		100, // price
		6, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_TRINKHORN == 38
		ITEM_SPRITE_ID_TRINKHORN,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		20, // weight
		10, // price_unit
		1, // price
		7, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SILBERSCHMUCK == 39
		ITEM_SPRITE_ID_SILBERSCHMUCK,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		5, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KLETTERHAKEN == 40
		ITEM_SPRITE_ID_KLETTERHAKEN,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		5, // weight
		1, // price_unit
		5, // price
		15, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_OEL == 41
		ITEM_SPRITE_ID_OEL,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		10, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BRONZEFLASCHE == 42
		ITEM_SPRITE_ID_BRONZEFLASCHE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		50, // weight
		10, // price_unit
		8, // price
		8, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_EISENHELM == 43
		ITEM_SPRITE_ID_EISENHELM,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_HEAD,
		ARMOR_STATS_ID_EISENHELM,
		180, // weight
		10, // price_unit
		80, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_PIKE == 44
		ITEM_SPRITE_ID_PIKE,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_PIKE,
		150, // weight
		10, // price_unit
		50, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_PROVIANTPAKET == 45
		ITEM_SPRITE_ID_PROVIANTPAKET,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		1, // flags.nutrition
		1, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		NUTRITION_TYPE_FOOD,
		NUTRITION_VALUE(50),
		20, // weight
		10, // price_unit
		3, // price
		19, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_FLOETE == 46
		ITEM_SPRITE_ID_FLOETE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		8, // weight
		10, // price_unit
		2, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ALCHIMIESET == 47
		ITEM_SPRITE_ID_ALCHIMIESET,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		100, // weight
		100, // price_unit
		20, // price
		2, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HEMD == 48
		ITEM_SPRITE_ID_HEMD,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_HEMD,
		30, // weight
		10, // price_unit
		2, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HOSE == 49
		ITEM_SPRITE_ID_HOSE,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_LEGS,
		ARMOR_STATS_ID_HOSE,
		30, // weight
		10, // price_unit
		3, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHUHE == 50
		ITEM_SPRITE_ID_SCHUHE,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_FEET,
		ARMOR_STATS_ID_SCHUHE,
		40, // weight
		10, // price_unit
		1, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_STIEFEL == 51
		ITEM_SPRITE_ID_STIEFEL,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_FEET,
		ARMOR_STATS_ID_STIEFEL,
		75, // weight
		10, // price_unit
		9, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHNEESCHUHE == 52
		ITEM_SPRITE_ID_SCHNEESCHUHE,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_FEET,
		ARMOR_STATS_ID_SCHNEESCHUHE,
		40, // weight
		10, // price_unit
		5, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LEDERHARNISCH == 53
		ITEM_SPRITE_ID_LEDERHARNISCH,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_LEDERHARNISCH,
		180, // weight
		10, // price_unit
		80, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHUPPENRUESTUNG == 54
		ITEM_SPRITE_ID_SCHUPPENRUESTUNG,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_SCHUPPENRUESTUNG,
		480, // weight
		100, // price_unit
		100, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SHURINKNOLLEKNOLLENGIFT == 55
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		70, // price
		15, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ARAXGIFT == 56
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		35, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ANGSTGIFT == 57
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		25, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHLAFGIFT == 58
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		35, // price
		7, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GOLDLEIM == 59
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		30, // price
		3, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_EINBEERE == 60
		ITEM_SPRITE_ID_EINBEERE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		10, // price_unit
		7, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WIRSELKRAUT == 61
		ITEM_SPRITE_ID_WIRSELKRAUT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		7, // price
		19, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_EITRIGER_KROETENSCHEMEL == 62
		ITEM_SPRITE_ID_EITRIGER_KROETENSCHEMEL,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		15, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GULMOND_BLATT == 63
		ITEM_SPRITE_ID_GULMOND_BLATT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		6, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_TARNELE == 64
		ITEM_SPRITE_ID_TARNELE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		10, // price_unit
		5, // price
		19, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_FACKEL__UNLIT == 65
		ITEM_SPRITE_ID_FACKEL__UNLIT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_LIGHTING,
		20, // weight
		1, // price_unit
		5, // price
		19, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_STREITKOLBEN == 66
		ITEM_SPRITE_ID_STREITKOLBEN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_STREITKOLBEN,
		110, // weight
		10, // price_unit
		50, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DEGEN == 67
		ITEM_SPRITE_ID_DEGEN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_DEGEN,
		35, // weight
		10, // price_unit
		55, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_FLORETT == 68
		ITEM_SPRITE_ID_FLORETT,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_FLORETT,
		30, // weight
		10, // price_unit
		50, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KAMPFSTAB == 69
		ITEM_SPRITE_ID_KAMPFSTAB,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SPEER,
		WEAPON_STATS_ID_KAMPFSTAB,
		70, // weight
		10, // price_unit
		15, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KRISTALLKUGEL == 70
		ITEM_SPRITE_ID_KRISTALLKUGEL,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		50, // weight
		100, // price_unit
		20, // price
		4, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_PEITSCHE == 71
		ITEM_SPRITE_ID_PEITSCHE,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_PEITSCHE,
		60, // weight
		10, // price_unit
		30, // price
		8, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DECKE == 72
		ITEM_SPRITE_ID_DECKE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		60, // weight
		10, // price_unit
		2, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHAUFEL == 73
		ITEM_SPRITE_ID_SCHAUFEL,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		80, // weight
		10, // price_unit
		10, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GOLDSCHMUCK == 74
		ITEM_SPRITE_ID_GOLDSCHMUCK,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		15, // weight
		100, // price_unit
		20, // price
		7, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ROBE__GREEN_1 == 75
		ITEM_SPRITE_ID_ROBE__GREEN,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_ROBE__GREEN,
		50, // weight
		10, // price_unit
		5, // price
		17, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ROBE__RED == 76
		ITEM_SPRITE_ID_ROBE__RED,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_ROBE__RED,
		200, // weight
		10, // price_unit
		200, // price
		15, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_TOPFHELM == 77
		ITEM_SPRITE_ID_TOPFHELM,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_HEAD,
		ARMOR_STATS_ID_TOPFHELM,
		100, // weight
		10, // price_unit
		50, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LEDERHELM == 78
		ITEM_SPRITE_ID_LEDERHELM,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_HEAD,
		ARMOR_STATS_ID_LEDERHELM,
		90, // weight
		10, // price_unit
		40, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WAFFENROCK == 79
		ITEM_SPRITE_ID_WAFFENROCK,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_WAFFENROCK,
		120, // weight
		10, // price_unit
		50, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KETTENHEMD == 80
		ITEM_SPRITE_ID_KETTENHEMD,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_KETTENHEMD,
		320, // weight
		10, // price_unit
		200, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KROETENHAUT == 81
		ITEM_SPRITE_ID_KROETENHAUT,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_KROETENHAUT,
		150, // weight
		10, // price_unit
		60, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_PLATTENZEUG == 82
		ITEM_SPRITE_ID_PLATTENZEUG,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_ARMS,
		ARMOR_STATS_ID_PLATTENZEUG,
		100, // weight
		10, // price_unit
		60, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KETTENZEUG == 83
		ITEM_SPRITE_ID_KETTENZEUG,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_ARMS,
		ARMOR_STATS_ID_KETTENZEUG,
		80, // weight
		10, // price_unit
		50, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LEDERZEUG == 84
		ITEM_SPRITE_ID_LEDERZEUG,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_ARMS,
		ARMOR_STATS_ID_LEDERZEUG,
		70, // weight
		10, // price_unit
		40, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ZUNDERKAESTCHEN == 85
		ITEM_SPRITE_ID_ZUNDERKAESTCHEN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_LIGHTING,
		25, // weight
		10, // price_unit
		8, // price
		17, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHLEIFSTEIN == 86
		ITEM_SPRITE_ID_SCHLEIFSTEIN,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		25, // weight
		10, // price_unit
		9, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ESSBESTECK == 87
		ITEM_SPRITE_ID_ESSBESTECK,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		5, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ESSGESCHIRR == 88
		ITEM_SPRITE_ID_ESSGESCHIRR,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		100, // weight
		10, // price_unit
		60, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LAKRITZE == 89
		ITEM_SPRITE_ID_LAKRITZE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		1, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		NUTRITION_TYPE_FOOD,
		NUTRITION_VALUE(2),
		10, // weight
		1, // price_unit
		3, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BONBONS == 90
		ITEM_SPRITE_ID_BONBONS,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		1, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		NUTRITION_TYPE_FOOD,
		NUTRITION_VALUE(2),
		10, // weight
		10, // price_unit
		1, // price
		6, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WEINFLASCHE == 91
		ITEM_SPRITE_ID_WEINFLASCHE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		1, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		NUTRITION_TYPE_DRINK,
		NUTRITION_VALUE(5),
		40, // weight
		1, // price_unit
		5, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHNAPSFLASCHE == 92
		ITEM_SPRITE_ID_SCHNAPSFLASCHE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		1, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		NUTRITION_TYPE_DRINK,
		NUTRITION_VALUE(2),
		40, // weight
		10, // price_unit
		1, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HACKE == 93
		ITEM_SPRITE_ID_HACKE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		200, // weight
		10, // price_unit
		20, // price
		17, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_PRAIOS_AMULETT == 94
		ITEM_SPRITE_ID_AMULETT_CROSS_GREEN_GEM,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		15, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LAUTE == 95
		ITEM_SPRITE_ID_LAUTE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		80, // weight
		100, // price_unit
		15, // price
		3, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WINTERMANTEL == 96
		ITEM_SPRITE_ID_WINTERMANTEL,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_WINTERMANTEL,
		120, // weight
		10, // price_unit
		50, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_NETZ == 97
		ITEM_SPRITE_ID_NETZ,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		35, // weight
		10, // price_unit
		10, // price
		11, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WURFMESSER == 98
		ITEM_SPRITE_ID_WURFMESSER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_WURFWAFFE,
		WEAPON_STATS_ID_WURFMESSER,
		10, // weight
		10, // price_unit
		10, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SICHEL == 99
		ITEM_SPRITE_ID_SICHEL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_SICKLE,
		30, // weight
		10, // price_unit
		25, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SENSE == 100
		ITEM_SPRITE_ID_SENSE,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_SENSE,
		100, // weight
		10, // price_unit
		35, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KRIEGSHAMMER == 101
		ITEM_SPRITE_ID_KRIEGSHAMMER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_KRIEGSHAMMER,
		150, // weight
		10, // price_unit
		100, // price
		7, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DREIZACK == 102
		ITEM_SPRITE_ID_DREIZACK,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_DREIZACK,
		90, // weight
		10, // price_unit
		45, // price
		6, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HELLEBARDE == 103
		ITEM_SPRITE_ID_HELLEBARDE,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_AXT,
		WEAPON_STATS_ID_HELLEBARDE,
		150, // weight
		10, // price_unit
		75, // price
		12, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DRESCHFLEGEL == 104
		ITEM_SPRITE_ID_DRESCHFLEGEL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_DRESCHFLEGEL,
		100, // weight
		10, // price_unit
		15, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ZWEILILIEN == 105
		ITEM_SPRITE_ID_ZWEILILIEN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_ZWEILILIEN,
		80, // weight
		10, // price_unit
		95, // price
		1, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_OCHSENHERDE == 106
		ITEM_SPRITE_ID_OCHSENHERDE,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_OCHSENHERDE,
		240, // weight
		10, // price_unit
		110, // price
		5, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BASILISKENZUNGE == 107
		ITEM_SPRITE_ID_BASILISKENZUNGE,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_BASILISKENZUNGE,
		25, // weight
		10, // price_unit
		25, // price
		4, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_OGERFAENGER == 108
		ITEM_SPRITE_ID_OGERFAENGER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_OGERFAENGER,
		30, // weight
		10, // price_unit
		95, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MENGBILAR == 109
		ITEM_SPRITE_ID_MENGBILAR,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_MENGBILAR,
		20, // weight
		10, // price_unit
		110, // price
		2, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHWERER_DOLCH == 110
		ITEM_SPRITE_ID_SCHWERER_DOLCH,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_SCHWERER_DOLCH,
		30, // weight
		10, // price_unit
		30, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_RONDRAKAMM == 111
		ITEM_SPRITE_ID_RONDRAKAMM,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_ZWEIHAENDER,
		WEAPON_STATS_ID_RONDRAKAMM,
		150, // weight
		10, // price_unit
		140, // price
		6, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ENTERMESSER == 112
		ITEM_SPRITE_ID_ENTERMESSER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_ENTERMESSER,
		70, // weight
		10, // price_unit
		45, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BASTARDSCHWERT == 113
		ITEM_SPRITE_ID_BASTARDSCHWERT,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_ZWEIHAENDER,
		WEAPON_STATS_ID_BASTARDSCHWERT,
		140, // weight
		10, // price_unit
		110, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_TUZAKMESSER == 114
		ITEM_SPRITE_ID_TUZAKMESSER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_ZWEIHAENDER,
		WEAPON_STATS_ID_TUZAKMESSER,
		135, // weight
		10, // price_unit
		200, // price
		2, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_RABENSCHNABEL == 115
		ITEM_SPRITE_ID_RABENSCHNABEL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_RABENSCHNABEL,
		90, // weight
		10, // price_unit
		110, // price
		4, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BRABAKBENGEL == 116
		ITEM_SPRITE_ID_BRABAKBENGEL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_BRABAKBENGEL,
		120, // weight
		10, // price_unit
		80, // price
		4, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_RAPIER == 117
		ITEM_SPRITE_ID_RAPIER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_RAPIER,
		35, // weight
		10, // price_unit
		60, // price
		13, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KHUNCHOMER == 118
		ITEM_SPRITE_ID_KHUNCHOMER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_KUNCHOMER,
		70, // weight
		10, // price_unit
		70, // price
		6, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DOPPELKHUNCHOMER == 119
		ITEM_SPRITE_ID_DOPPELKHUNCHOMER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_DOPPELKUNCHOMER,
		150, // weight
		10, // price_unit
		125, // price
		2, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHLEUDER == 120
		ITEM_SPRITE_ID_SCHLEUDER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHUSSWAFFE,
		WEAPON_STATS_ID_SCHLEUDER,
		10, // weight
		10, // price_unit
		15, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SEIL == 121
		ITEM_SPRITE_ID_SEIL,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		55, // weight
		10, // price_unit
		7, // price
		20, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SHURINKNOLLE == 122
		ITEM_SPRITE_ID_SHURINKNOLLE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		8, // price
		15, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BELMART_BLATT == 123
		ITEM_SPRITE_ID_BELMART_BLATT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		12, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DONFSTENGEL == 124
		ITEM_SPRITE_ID_DONFSTENGEL,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		20, // price
		11, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MENCHALKAKTUS == 125
		ITEM_SPRITE_ID_MENCHALKAKTUS,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		20, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ALRAUNE == 126
		ITEM_SPRITE_ID_ALRAUNE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		10, // price_unit
		8, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ATMONBLUETE == 127
		ITEM_SPRITE_ID_ATMONBLUETE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		30, // price
		8, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ILMENBLATT == 128
		ITEM_SPRITE_ID_ILMENBLATT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		11, // price
		8, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_FINAGEBAEMCHEN == 129
		ITEM_SPRITE_ID_FINAGEBAEMCHEN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		8, // price
		6, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_JORUGAWURZEL == 130
		ITEM_SPRITE_ID_JORUGAWURZEL,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		18, // price
		6, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_THONNYSBLUETE == 131
		ITEM_SPRITE_ID_THONNYSBLUETE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		35, // price
		3, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LOTUSBLUETE == 132
		ITEM_SPRITE_ID_LOTUSBLUETE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		20, // price
		2, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ZAUBERSTAB == 133
		ITEM_SPRITE_ID_KAMPFSTAB,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		1, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SPEER,
		WEAPON_STATS_ID_ZAUBERSTAB_HEXENBESEN,
		70, // weight
		100, // price_unit
		100, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_SKRAJA == 134
		ITEM_SPRITE_ID_SKRAJA,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_SKRAJA,
		90, // weight
		10, // price_unit
		50, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KRIEGSBEIL == 135
		ITEM_SPRITE_ID_KRIEGSBEIL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_AXT,
		WEAPON_STATS_ID_KRIEGSBEIL,
		120, // weight
		10, // price_unit
		50, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ORKNASE == 136
		ITEM_SPRITE_ID_ORKNASE,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_AXT,
		WEAPON_STATS_ID_ORKNASE,
		120, // weight
		10, // price_unit
		60, // price
		16, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHNEIDZAHN == 137
		ITEM_SPRITE_ID_SCHNEIDZAHN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_WURFWAFFE,
		WEAPON_STATS_ID_SCHNEIDZAHN,
		50, // weight
		10, // price_unit
		50, // price
		14, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ROBBENTOETER == 138
		ITEM_SPRITE_ID_ROBBENTOETER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_ROBBENTOETER,
		70, // weight
		10, // price_unit
		60, // price
		1, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WOLFSMESSER == 139
		ITEM_SPRITE_ID_WOLFSMESSER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_WOLFSMESSER,
		50, // weight
		10, // price_unit
		90, // price
		1, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HEXENBESEN == 140
		ITEM_SPRITE_ID_HEXENBESEN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		1, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_ZAUBERSTAB_HEXENBESEN,
		60, // weight
		100, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LOTUSGIFT == 141
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		100, // price
		2, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KUKRIS == 142
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		90, // price
		2, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BANNSTAUB == 143
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		250, // price
		1, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KROETENSCHEMELGIFT == 144
		ITEM_SPRITE_ID_POTION_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		15, // price
		15, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HEILTRANK == 145
		ITEM_SPRITE_ID_POTION_BLUE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		10, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_STARKER_HEILTRANK == 146
		ITEM_SPRITE_ID_POTION_BLUE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		24, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MU_ELIXIER == 147
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KL_ELIXIER == 148
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_CH_ELIXIER == 149
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_FF_ELIXIER == 150
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GE_ELIXIER == 151
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_IN_ELIXIER == 152
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KK_ELIXIER == 153
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		9, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ZAUBERTRANK == 154
		ITEM_SPRITE_ID_POTION_GREEN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		10, // price
		18, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ZAUBERTRANK_STARK == 155
		ITEM_SPRITE_ID_POTION_GREEN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		10, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_OLGINWURZEL == 156
		ITEM_SPRITE_ID_OLGINWURZEL,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		100, // price_unit
		2, // price
		4, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KAIRANHALM == 157
		ITEM_SPRITE_ID_KAIRANHALM,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		1, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_HERB_AND_POISON,
		USABLE_ITEM_STATS_ID_NONE,
		1, // weight
		10, // price_unit
		15, // price
		4, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BASTARDSCHWERT__MAGIC == 158
		ITEM_SPRITE_ID_BASTARDSCHWERT,
		0, // flags.armor
		1, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_ZWEIHAENDER,
		WEAPON_STATS_ID_BASTARDSCHWERT__MAGIC,
		50, // weight
		10, // price_unit
		110, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_ORKNASE__MAGIC == 159
		ITEM_SPRITE_ID_ORKNASE,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_AXT,
		WEAPON_STATS_ID_ORKNASE__MAGIC,
		120, // weight
		10, // price_unit
		60, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_KURZSCHWERT__MAGIC == 160
		ITEM_SPRITE_ID_KURZSCHWERT,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHWERT,
		WEAPON_STATS_ID_KURZSCHWERT__MAGIC,
		40, // weight
		10, // price_unit
		45, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_SICHEL__MAGIC == 161
		ITEM_SPRITE_ID_SICHEL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_SICKLE,
		30, // weight
		10, // price_unit
		25, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_AMULETT__ANTI_IGNIFAXIUS == 162
		ITEM_SPRITE_ID_AMULETT_STAR_BLUE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_AMULETT__MR_BONUS == 163
		ITEM_SPRITE_ID_AMULETT_CIRCULAR_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_KNOCHEN_MIT_RUNE == 164
		ITEM_SPRITE_ID_KNOCHEN_MIT_RUNE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_RING__RED == 165
		ITEM_SPRITE_ID_RING__RED,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_ARCANO_PSYCHOSTABILIS,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_EXPURGICUM == 166
		ITEM_SPRITE_ID_POTION_GREEN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_EXPURGICUM == 167
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_VOMICUM == 168
		ITEM_SPRITE_ID_POTION_GREEN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_WEAPON_POISON,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_VOMICUM == 169
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DOKUMENT__UNICORN == 170
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SILBERNER_STIRNREIF__1 == 171
		ITEM_SPRITE_ID_CORONET_SILVER,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_ARMATRUTZ,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_SAEBEL__MAGIC == 172
		ITEM_SPRITE_ID_SAEBEL,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_SAEBEL__MAGIC,
		60, // weight
		10, // price_unit
		60, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_AMULETT__FEUERBANN == 173
		ITEM_SPRITE_ID_AMULETT_STAR_BLUE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_AMULETT__FLIM_FLAM == 174
		ITEM_SPRITE_ID_AMULETT_STAR_BLUE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_FLIM_FLAM,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_AMULETT__TRAVIA == 175
		ITEM_SPRITE_ID_AMULETT_CROSS_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_SCHULDBUCH == 176
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_SCHULDBUCH,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ROTE_MONDSCHEIBE == 177
		ITEM_SPRITE_ID_ROTE_MONDSCHEIBE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ZWEIHAENDER__MAGIC == 178
		ITEM_SPRITE_ID_ZWEIHAENDER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_ZWEIHAENDER,
		WEAPON_STATS_ID_ZWEIHAENDER__MAGIC,
		160, // weight
		100, // price_unit
		50, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_ORKDOKUMENT == 179
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_ORKDOKUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GEGENGIFT == 180
		ITEM_SPRITE_ID_POTION_BLUE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		16, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DAS_SCHWERT_GRIMRING == 181
		ITEM_SPRITE_ID_DAS_SCHWERT_GRIMRING,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHWERT,
		WEAPON_STATS_ID_DAS_SCHWERT_GRIMRING,
		80, // weight
		100, // price_unit
		100, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_TOTENKOPFGUERTEL == 182
		ITEM_SPRITE_ID_TOTENKOPFGUERTEL,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KRAFTGUERTEL == 183
		ITEM_SPRITE_ID_KRAFTGUERTEL,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		20, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_MAGISCHER_BROTBEUTEL == 184
		ITEM_SPRITE_ID_MAGISCHER_BROTBEUTEL,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_MAGISCHER_BEUTEL,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_MAGISCHER_WASSERSCHLAUCH == 185
		ITEM_SPRITE_ID_MAGISCHER_WASSERSCHLAUCH,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_HEILTRANK == 186
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHREIBEN_VON_JADRA == 187
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_AMULETT__ORDINARY == 188
		ITEM_SPRITE_ID_AMULETT_CROSS_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LOBPREISUNGEN == 189
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MITGLIEDERLISTE == 190
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DOKUMENT__2 == 191
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SEEKARTE == 192
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KUKRISDOLCH == 193
		ITEM_SPRITE_ID_DOLCH,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_DOLCH,
		20, // weight
		100, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHLUESSEL__YELLOW == 194
		ITEM_SPRITE_ID_KEY_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GOLDENER_SCHLUESSEL == 195
		ITEM_SPRITE_ID_KEY_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HELM__CURSED == 196
		ITEM_SPRITE_ID_LEDERHELM,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_HEAD,
		ARMOR_STATS_ID_HELM__CURSED,
		90, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KETTENHEMD__CURSED == 197
		ITEM_SPRITE_ID_KETTENHEMD,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_KETTENHEMD__CURSED,
		320, // weight
		100, // price_unit
		40, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_KETTENHEMD__MAGIC == 198
		ITEM_SPRITE_ID_KETTENHEMD,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_KETTENHEMD__MAGIC,
		320, // weight
		100, // price_unit
		100, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_SCHWERT__MAGIC == 199
		ITEM_SPRITE_ID_SCHWERT,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SCHWERT,
		WEAPON_STATS_ID_SCHWERT__MAGIC,
		80, // weight
		100, // price_unit
		20, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_KUKRIS_MENGBILAR == 200
		ITEM_SPRITE_ID_MENGBILAR,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_STICHWAFFE,
		WEAPON_STATS_ID_MENGBILAR,
		20, // weight
		100, // price_unit
		40, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_PLATINSCHLUESSEL == 201
		ITEM_SPRITE_ID_KEY_SILVER,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_GEGENGIFT == 202
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_HYLAILER_FEUER == 203
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_KRAFTELIXIER == 204
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_MUTELIXIER == 205
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_ZAUBERTRANK == 206
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_RING__BLUE == 207
		ITEM_SPRITE_ID_RING__BLUE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		50, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHLUESSEL__SILVER == 208
		ITEM_SPRITE_ID_KEY_SILVER,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_EISENSCHLUESSEL__1 == 209
		ITEM_SPRITE_ID_KEY_SILVER,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_EISENSCHLUESSEL__2 == 210
		ITEM_SPRITE_ID_KEY_SILVER,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ROTER_SCHLUESSEL == 211
		ITEM_SPRITE_ID_KEY_RED,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BRONZESCHLUESSEL == 212
		ITEM_SPRITE_ID_KEY_RED,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SILBERHELM == 213
		ITEM_SPRITE_ID_EISENHELM,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_HEAD,
		ARMOR_STATS_ID_EISENHELM,
		180, // weight
		10, // price_unit
		150, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SILBERSTREITKOLBEN == 214
		ITEM_SPRITE_ID_STREITKOLBEN,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_HIEBWAFFE,
		WEAPON_STATS_ID_STREITKOLBEN,
		110, // weight
		100, // price_unit
		30, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SILBERSCHMUCK__MAGIC == 215
		ITEM_SPRITE_ID_SILBERSCHMUCK,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		20, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_SPEER__MAGIC == 216
		ITEM_SPRITE_ID_SPEER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_SPEER,
		WEAPON_STATS_ID_SPEER__MAGIC,
		80, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_STIRNREIF__MR_BONUS == 217
		ITEM_SPRITE_ID_CORONET_SILVER,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_WURFDOLCH__MAGIC == 218
		ITEM_SPRITE_ID_WURFMESSER,
		0, // flags.armor
		1, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		WEAPON_TYPE_WURFWAFFE,
		WEAPON_STATS_ID_WURFDOLCH__MAGIC,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_GOLDSCHLUESSEL == 219
		ITEM_SPRITE_ID_KEY_YELLOW,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		10, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_RING__GREEN == 220
		ITEM_SPRITE_ID_RING__GREEN,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		20, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_BEUTEL == 221
		ITEM_SPRITE_ID_MAGISCHER_BROTBEUTEL,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_MAGISCHER_BEUTEL,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_BUCH__PIRATE_ACCOUNTING == 222
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ANTIKRANKHEITSELIXIER == 223
		ITEM_SPRITE_ID_POTION_BLUE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_BUCH__KAISERSPRUECHE_HALS == 224
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KRISTALL == 225
		ITEM_SPRITE_ID_KRISTALL,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		5, // weight
		10, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MU_ELIXIER__BAD == 226
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KL_ELIXIER__BAD == 227
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_CH_ELIXIER__BAD == 228
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_FF_ELIXIER__BAD == 229
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GE_ELIXIER__BAD == 230
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_IN_ELIXIER__BAD == 231
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_KK_ELIXIER__BAD == 232
		ITEM_SPRITE_ID_POTION_BROWN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		40, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_ROBE__GREEN_2 == 233
		ITEM_SPRITE_ID_ROBE__GREEN,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_BODY,
		ARMOR_STATS_ID_ROBE__GREEN,
		50, // weight
		10, // price_unit
		5, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_GOLDENER_SCHILD == 234
		ITEM_SPRITE_ID_EISENSCHILD,
		1, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		ARMOR_TYPE_LEFT_HAND,
		ARMOR_STATS_ID_EISENSCHILD,
		200, // weight
		100, // price_unit
		25, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_EMPFEHLUNGSSCHREIBEN__HETMAN == 235
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_WUNDERKUR == 236
		ITEM_SPRITE_ID_POTION_BLUE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		60, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHLAFTRUNK == 237
		ITEM_SPRITE_ID_POTION_BLUE,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		1, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		HERB_POTION_TYPE_POTION,
		USABLE_ITEM_STATS_ID_NONE,
		10, // weight
		100, // price_unit
		6, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_MIASTHMATICUM == 238
		ITEM_SPRITE_ID_POTION_GREEN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_MIASTHMATICUM,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_HYLAILER_FEUER == 239
		ITEM_SPRITE_ID_POTION_GREEN,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_HYLAILER_FEUER,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_STARKEN_HEILTRANK == 240
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_WUNDERKUR == 241
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_SCHLAFTRUNK == 242
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_STARKER_ZAUBERTRANK == 243
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_REZEPT_FUER_MIASTHMATICUM == 244
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_RECIPE,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SILBERNER_STIRNREIF__2 == 245
		ITEM_SPRITE_ID_CORONET_SILVER,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_ARMATRUTZ,
		10, // weight
		100, // price_unit
		10, // price
		0, // commonness
		1  // is_magic
	},
	{ // ITEM_ID_BUCH__HEPTAGON == 246
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_SPELLBOOK_HEPTAGON,
		5, // weight
		100, // price_unit
		10, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_EMPFEHLUNGSSCHREIBEN__SIEBENSTEIN == 247
		ITEM_SPRITE_ID_DOCUMENT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_DOCUMENT,
		5, // weight
		10, // price_unit
		2, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_SCHWARZE_STATUETTE == 248
		ITEM_SPRITE_ID_SCHWARZE_STATUETTE,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		10, // weight
		100, // price_unit
		20, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_LATERNE__LIT == 249
		ITEM_SPRITE_ID_LATERNE__LIT,
		0, // flags.armor
		0, // flags.weapon
		1, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		USABLE_ITEM_STATS_ID_LIGHTING,
		15, // weight
		10, // price_unit
		3, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_DUKATEN == 250
		ITEM_SPRITE_ID_SCHWERT,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		1, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		0, // weight
		100, // price_unit
		1, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_200_PFEILE == 251
		-1, // item_sprite_id
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		0, // weight
		0, // price_unit
		0, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_50_BOLZEN == 252
		ITEM_SPRITE_ID_SCHWERT,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		0, // weight
		0, // price_unit
		0, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID_20_KLETTERHAKEN == 253
		ITEM_SPRITE_ID_SCHWERT,
		0, // flags.armor
		0, // flags.weapon
		0, // flags.usable
		0, // flags.nutrition
		0, // flags.stackable
		0, // flags.herb_potion
		0, // flags.undroppable
		0, // flags.dummy
		0, // subtype
		0, // item_type_stats_id
		0, // weight
		0, // price_unit
		0, // price
		0, // commonness
		0  // is_magic
	},
	{ // ITEM_ID__END == 254
		// terminator entry
		0
	}
};
