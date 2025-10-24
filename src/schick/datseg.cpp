/**
 *	Rewrite of DSA1 v3.02_de datasegment
 *	No code is allowed here, only the global data
 */

#include "v302de.h"

#include "seg025.h"
#include "seg051.h"
#include "seg052.h"
#include "seg053.h"
#include "seg054.h"
#include "seg055.h"
#include "seg058.h"
#include "seg059.h"
#include "seg061.h"
#include "seg063.h"
#include "seg065.h"
#include "seg066.h"

#include "seg077.h"
#include "seg078.h"
#include "seg079.h"
#include "seg080.h"
#include "seg081.h"
#include "seg082.h"
#include "seg083.h"
#include "seg084.h"
#include "seg085.h"
#include "seg086.h"
#include "seg087.h"
#include "seg088.h"
#include "seg089.h"
#include "seg090.h"
#include "seg091.h"
#include "seg092.h"

#include "seg099.h"
#include "seg100.h"
#include "seg101.h"
#include "seg102.h"
#include "seg107.h"
#include "seg109.h"
#include "seg110.h"
#include "seg111.h"
#include "seg112.h"
#include "seg113.h"
#include "seg114.h"
#include "seg115.h"
#include "seg116.h"
#include "seg117.h"
#include "seg118.h"
#include "seg122.h"

#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
char ds[1];
#endif

signed short g_items_noplural[23] = {
	ITEM_LICORICE,
	ITEM_BONBONS,
	ITEM_SHURIN_POISON,
	ITEM_ARAX_POISON,
	ITEM_ANGST_POISON,
	ITEM_SLEEP_POISON,
	ITEM_GOLDLEIM,
	ITEM_LOTUS_POISON,
	ITEM_KUKRIS,
	ITEM_BANNSTAUB,
	ITEM_KROETEN_POISON,
	ITEM_OIL,
	ITEM_EXPURGICUM,
	ITEM_VOMICUM,
	ITEM_ANTIVENOM,
	ITEM_GRIMRING,
	ITEM_LOBPREISUNGEN,
	ITEM_PLATTENZEUG,
	ITEM_LEDERZEUG,
	ITEM_KETTENZEUG,
	ITEM_MIASTHMATICUM,
	ITEM_HYLAILIC_FIRE,
	-1
}; // ds:0x0270
signed short g_items_pluralwords[7] = {
	ITEM_PICKLOCKS,
	ITEM_SHOES,
	ITEM_BOOTS,
	ITEM_LACEUP_SHOE,
	ITEM_BONBONS,
	ITEM_LOBPREISUNGEN,
	-1
}; // ds:0x029e
signed char g_items_genders[254] = { 1, 2, 0, 0, 2, 0, 2, 0, 2, 0, 0, 1, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 2, 0, 1, 2, 0, 0, 1, 0, 1, 1, 1, 0, 1, 2, 1, 2, 0, 0, 1, 1, 0, 1, 0, 1, 0, 2, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 0, 1, 2, 0, 2, 1, 1, 0, 0, 2, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 2, 1, 2, 2, 2, 0, 0, 2, 2, 1, 1, 1, 1, 1, 2, 1, 0, 2, 2, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 0, 0, 1, 2, 1, 2, 0, 0, 1, 1, 2, 2, 1, 1, 1, 0, 2, 2, 1, 0, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 1, 0, 2, 1, 2, 1, 2, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 1, 0, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 1, 1, 2, 1, 0, 0, 0, 0, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 1, 0, 2, 1, 0, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 1, 1, 1, 1, 1, 1 }; // ds:0x02ac

signed short g_wearable_items_juggler[19] = { 0x0050, 0x000b, 0x0071, 0x0067, 0x0012, 0x0014, 0x000c, 0x0065, 0x006f, 0x0036, 0x006a, 0x0052, 0x0077, 0x0072, 0x009e, 0x00b2, 0x00c5, 0x00c6, -1 }; // ds:0x03aa, array terminated by -1
signed short g_wearable_items_hunter[19] = { 0x000b, 0x0071, 0x0067, 0x0050, 0x0012, 0x0014, 0x000c, 0x0065, 0x006f, 0x0052, 0x0036, 0x006a, 0x0077, 0x0072, 0x009e, 0x00b2, 0x00c5, 0x00c6, -1 }; // ds:0x03d0, array terminated by -1
signed short g_wearable_items_warrior[12] = { 0x0039, 0x0037, 0x008d, 0x0090, 0x0038, 0x003a, 0x003b, 0x008f, 0x008e, 0x00c1, 0x00c8, -1 }; // ds:0x03f6, array terminated by -1
signed short g_wearable_items_estray[18] = { 0x000b, 0x0071, 0x0067, 0x0050, 0x0012, 0x0014, 0x0065, 0x006f, 0x0052, 0x006a, 0x0036, 0x0077, 0x0072, 0x009e, 0x00b2, 0x00c5, 0x00c6, -1 }; // ds:0x040e, array terminated by -1
signed short g_wearable_items_thorwalian[12] = { 0x0071, 0x0067, 0x0012, 0x0014, 0x0065, 0x006f, 0x006a, 0x0077, 0x0072, 0x009e, 0x00b2, -1 }; // ds:0x0432, array terminated by -1
signed short g_wearable_items_dwarf[9] = { 0x006f, 0x0077, 0x0012, 0x0072, 0x002c, 0x0067, 0x006a, 0x0068, -1 }; // ds:0x044a, array terminated by -1
signed short g_wearable_items_witch[64] = { 0x0005, 0x0002, 0x0001, 0x0003, 0x0070, 0x0086, 0x0006, 0x0042, 0x0087, 0x0088, 0x0009, 0x000b, 0x0044, 0x0043, 0x0068, 0x0071, 0x0089, 0x000c, 0x0075, 0x002c, 0x0067, 0x0010, 0x0063, 0x0064, 0x0012, 0x0013, 0x0014, 0x0065, 0x0066, 0x0076, 0x006f, 0x006a, 0x0074, 0x0073, 0x0077, 0x0072, 0x0069, 0x008a, 0x008b, 0x0035, 0x0007, 0x0051, 0x000f, 0x0050, 0x0052, 0x0054, 0x0053, 0x004d, 0x004e, 0x002b, 0x0036, 0x009e, 0x00a1, 0x009f, 0x00a0, 0x00b2, 0x00ac, 0x00b5, 0x00c7, 0x00c5, 0x00c6, 0x00d6, 0x00d8, -1 }; // ds:0x045c, array terminated by -1
signed short g_wearable_items_druid[57] = { 0x0005, 0x0001, 0x0003, 0x0070, 0x0086, 0x0006, 0x0042, 0x0087, 0x0088, 0x000b, 0x0044, 0x0043, 0x0071, 0x0089, 0x000c, 0x0075, 0x002c, 0x0067, 0x0010, 0x0063, 0x0064, 0x0012, 0x0014, 0x0065, 0x0066, 0x0076, 0x006f, 0x006a, 0x0074, 0x0073, 0x0077, 0x0072, 0x0069, 0x008a, 0x008b, 0x0007, 0x0051, 0x000f, 0x0050, 0x0052, 0x0053, 0x004d, 0x002b, 0x0036, 0x009e, 0x009f, 0x00a1, 0x00a0, 0x00b2, 0x00ac, 0x00b5, 0x00c7, 0x00c5, 0x00c6, 0x00d6, 0x00d8, -1 }; // ds:0x04dc, array terminated by -1
signed short g_wearable_items_mage[64] = { 0x0005, 0x0002, 0x0001, 0x0003, 0x0070, 0x0086, 0x0006, 0x0042, 0x0087, 0x0088, 0x0009, 0x000b, 0x0044, 0x0043, 0x0068, 0x0071, 0x0089, 0x000c, 0x0075, 0x002c, 0x0067, 0x0010, 0x0063, 0x0064, 0x0012, 0x0013, 0x0014, 0x0065, 0x0066, 0x0076, 0x006f, 0x006a, 0x0074, 0x0073, 0x0077, 0x0072, 0x0069, 0x008a, 0x008b, 0x0035, 0x0007, 0x0051, 0x000f, 0x0050, 0x0052, 0x0054, 0x0053, 0x004d, 0x004e, 0x002b, 0x0036, 0x009e, 0x009f, 0x00a0, 0x00a1, 0x00b2, 0x00ac, 0x00b5, 0x00c7, 0x00c5, 0x00c6, 0x00d6, 0x00d8, -1 }; // ds:0x054e, array terminated by -1
signed short g_wearable_items_green_elf[15] = { 0x000b, 0x0071, 0x0067, 0x0012, 0x0014, 0x0065, 0x006f, 0x006a, 0x0052, 0x0036, 0x0077, 0x0072, 0x009e, 0x00b2, -1 }; // ds:0x05ce, array terminated by -1
signed short g_wearable_items_ice_elf[19] = { 0x000b, 0x0071, 0x0067, 0x0012, 0x0014, 0x0065, 0x006f, 0x006a, 0x0052, 0x0036, 0x0077, 0x0072, 0x0050, 0x0053, 0x009e, 0x00b2, 0x00c5, 0x00c6, -1 }; // ds:0x05ec, array terminated by -1
signed short g_wearable_items_sylvan_elf[19] = { 0x000b, 0x0071, 0x0067, 0x0012, 0x0014, 0x0065, 0x006f, 0x006a, 0x0052, 0x0036, 0x0077, 0x0072, 0x0050, 0x0053, 0x009e, 0x00b2, 0x00c5, 0x00c6, -1 }; // ds:0x0612, array terminated by -1

signed short *g_wearable_items_index[12] = {
    g_wearable_items_juggler,
    g_wearable_items_hunter,
    g_wearable_items_warrior,
    g_wearable_items_estray,
    g_wearable_items_thorwalian,
    g_wearable_items_dwarf,
    g_wearable_items_witch,
    g_wearable_items_druid,
    g_wearable_items_mage,
    g_wearable_items_green_elf,
    g_wearable_items_ice_elf,
    g_wearable_items_sylvan_elf
}; // ds:0x0638, items wearable depending on hero type

const struct ranged_weapon_descr g_ranged_weapons_table[9] = {
	{  1,  0,  0, -1,-99,-99,-99,  6 }, /* 0 <- ITEM_THROWING_STAR, ITEM_THROWING_KNIFE */
	{  1,  1,  0,  0, -1,-99,-99,  6 }, /* 1 <- ITEM_SPEAR, ITEM_SLING */
	{  2,  1,  0,  0,  0,-99,-99,  7 }, /* 2 <- ITEM_FRANCESCA, ITEM_THROWING_AXE */
	{  1,  1,  0,  0,  0, -1,-99,  4 }, /* 3 <- ITEM_SHORTBOW */
	{  2,  2,  1,  0,  0, -1, -2,  4 }, /* 4 <- ITEM_LONGBOW */
	{  2,  2,  1,  0, -1, -2, -3,  3 }, /* 5 <- ITEM_CROSSBOW */
	{  2,  2,  1,  1,  0,  0, -1,  4 }, /* 6 <- ITEM_SPEAR_MAGIC */
	{  9,  9,  9,  9,  9,  9,  9,  2 }, /* 7 <- ITEM_THROWING_DAGGER_MAGIC */
	{ -1,  0,  0,  0,  0,  0,  0,  0 }
}; // ds:0x0668

struct weapon_descr g_weapons_table[65] = {
	{ 1, 4, 14,  2, -1,  0,  0 }, /*  0 <- ITEM_SWORD */
	{ 1, 1, 14,  6, -1, -1, -3 }, /*  1 <- ITEM_CLUB */
	{ 1, 3, 15,  2, -1,  0,  0 }, /*  2 <- ITEM_SABER */
	{ 1, 0, 16,  4, -1, -3, -4 }, /*  3 <- ITEM_KNIFE */
	{ 1, 3, 99,  5,  1,  0, -3 }, /*  4 <- ITEM_SPEAR */
	{ 1, 2, 15,  1, -1,  0, -1 }, /*  5 <- ITEM_SHORT_SWORD */
	{ 1, 4, 14,  5, -1,  0, -3 }, /*  6 <- ITEM_KRIEGSBEIL_SPECIAL */
	{ 1, 3, 99,  0,  3,  0,  0 }, /*  7 <- ITEM_SHORTBOW */
	{ 2, 4, 14,  3, -1, -1, -4 }, /*  8 <- ITEM_STREITAXT */
	{ 1, 6, 99,  0,  5,  0,  0 }, /*  9 <- ITEM_CROSSBOW */
	{ 1, 1, 15,  3, -1, -2, -3 }, /* 10 <- ITEM_DAGGER, ITEM_KUKRIS_DAGGER */
	{ 1, 3, 99,  4,  2, -1, -3 }, /* 11 <- ITEM_FRANCESCA */
	{ 1, 1, 99,  0,  0,  0,  0 }, /* 12 <- ITEM_THROWING_STAR */
	{ 2, 4, 14,  3, -1, -2, -3 }, /* 13 <- ITEM_TWO_HANDED_SWORD */
	{ 1, 4, 99,  0,  4,  0,  0 }, /* 14 <- ITEM_LONGBOW */
	{ 1, 5, 15,  5, -1, -1, -3 }, /* 15 <- ITEM_MORNING_STAR */
	{ 1, 0, 16,  6, -1, -2, -3 }, /* 16 <- ITEM_VOLCANIC_GLASS_DAGGER */
	{ 1, 3, 99,  4,  2, -1, -4 }, /* 17 <- ITEM_THROWING_AXE */
	{ 1, 3, 19,  7, -1, -1, -4 }, /* 18 <- ITEM_PIKE */
	{ 1, 4, 13,  1, -1,  0, -2 }, /* 19 <- ITEM_MACE, ITEM_SILVER_MACE */
	{ 1, 3, 16,  3, -1,  0, -1 }, /* 20 <- ITEM_DEGEN */
	{ 1, 3, 16,  3, -1,  0, -1 }, /* 21 <- ITEM_FLORET */
	{ 1, 1, 15,  5, -1,  0, -1 }, /* 22 <- ITEM_QUARTERSTAFF */
	{ 1, 0, 19,  2, -1,  0, -6 }, /* 23 <- ITEM_WHIP */
	{ 1, 0, 99,  4,  0, -3, -4 }, /* 24 <- ITEM_THROWING_KNIFE */
	{ 1, 2, 17,  5, -1, -3, -4 }, /* 25 <- ITEM_SICKLE, ITEM_SICKLE_MAGIC */
	{ 1, 3, 17,  6, -1, -3, -4 }, /* 26 <- ITEM_SCYTHE */
	{ 2, 3, 15,  2, -1, -2, -4 }, /* 27 <- ITEM_WAR_HAMMER */
	{ 1, 3, 15,  3, -1,  0, -3 }, /* 28 <- ITEM_TRIDENT */
	{ 1, 4, 15,  5, -1, -1, -3 }, /* 29 <- ITEM_HALBERD */
	{ 1, 2, 13,  6, -1, -2, -3 }, /* 30 <- ITEM_FLAIL */
	{ 1, 3, 18,  4, -1, -1, -1 }, /* 31 <- ITEM_ZWEILILIEN */
	{ 3, 3, 17,  4, -1, -3, -4 }, /* 32 <- ITEM_OCHSENHERDE */
	{ 1, 1, 16,  4, -1, -2, -3 }, /* 33 <- ITEM_BASILISKENZUNGE */
	{ 1, 2, 15,  4, -1, -2, -3 }, /* 34 <- ITEM_OGERFAENGER */
	{ 1, 1, 16,  7, -1, -3, -4 }, /* 35 <- ITEM_MENGBILAR, ITEM_KUKRIS_MENGBILAR */
	{ 1, 2, 15,  2, -1, -1, -2 }, /* 36 <- ITEM_HEAVY_DAGGER */
	{ 2, 2, 15,  3, -1, -2, -2 }, /* 37 <- ITEM_RONDRAKAMM */
	{ 1, 3, 15,  2, -1,  0, -1 }, /* 38 <- ITEM_CUTLASS */
	{ 1, 5, 14,  2, -1, -1, -2 }, /* 39 <- ITEM_BASTARD_SWORD */
	{ 1, 6, 15,  1, -1, -2, -2 }, /* 40 <- ITEM_TUZAK_KNIFE */
	{ 1, 4, 16,  3, -1, -1, -3 }, /* 41 <- ITEM_RABENSCHNABEL */
	{ 1, 5, 14,  1, -1, -1, -2 }, /* 42 <- ITEM_BRABAKBENGEL */
	{ 1, 3, 16,  4, -1,  0, -1 }, /* 43 <- ITEM_RAPIER */
	{ 1, 4, 14,  2, -1,  0, -1 }, /* 44 <- ITEM_KUNCHOMER */
	{ 1, 6, 15,  3, -1, -1, -3 }, /* 45 <- ITEM_DOPPELKUNCHOMER */
	{ 1, 2, 99,  0,  1,  0,  0 }, /* 46 <- ITEM_SLING */
	{ 1, 3, 13,  4, -1,  0, -2 }, /* 47 <- ITEM_SKRAJA */
	{ 1, 4, 13,  2, -1, -2, -4 }, /* 48 <- ITEM_KRIEGSBEIL_2 */
	{ 1, 5, 14,  2, -1, -1, -3 }, /* 49 <- ITEM_ORKNASE */
	{ 1, 4, 99,  0, -1,  0,  0 }, /* 50 <- ITEM_SCHNEIDZAHN */ /* Original-Bug: entry -1 for ranged_index is not o.k. for a throwing weapon */
	{ 1, 3, 15,  2, -1,  0,  0 }, /* 51 <- ITEM_ROBBENTOETER */
	{ 1, 3, 15,  2, -1,  0, -1 }, /* 52 <- ITEM_WOLFSMESSER */
	{ 0, 0, 99,  0, -1,  0,  0 }, /* 53 <- ITEM_ARROWS, ITEM_BOLTS */
	{ 1, 5, 14, -5, -1, -2, -3 }, /* 54 <- ITEM_BASTARD_SWORD_MAGIC */
	{ 1,10, 14,-99, -1, -2, -8 }, /* 55 <- ITEM_ORKNASE_MAGIC */
	{ 1, 2, 15,-99, -1,  2, -1 }, /* 56 <- ITEM_SHORT_SWORD_MAGIC */
	{ 1, 3, 15,  0, -1,  0,  0 }, /* 57 <- ITEM_SABER_MAGIC */
	{ 2, 4, 14,-99, -1, -3, -4 }, /* 58 <- ITEM_TWO_HANDED_SWORD_MAGIC */
	{ 1, 4, 14,-99, -1,  2,  2 }, /* 59 <- ITEM_GRIMRING */
	{ 1, 4, 14,  0, -1,  2,  0 }, /* 60 <- ITEM_SWORD_MAGIC */
	{ 1, 3, 99,  5,  6,  1,  1 }, /* 61 <- ITEM_SPEAR_MAGIC */
	{ 1, 0, 99,  4,  7, -3, -4 }, /* 62 <- ITEM_THROWING_DAGGER_MAGIC */
	{ 1, 1, 15,-99, -1,  0,  0 }, /* 63 <- ITEM_MAGIC_WAND, ITEM_WITCHES_BROOM */
	{-1, 0,  0,  0,  0,  0,  0 }
}; // ds:0x06b0

struct armors_descr g_armors_table[25] = {
	{ 1, 0 }, /*  0 <- ITEM_SHIELD */
	{ 2, 1 }, /*  1 <- ITEM_IRON_SHIELD, ITEM_GOLDEN_SHIELD */
	{ 3, 2 }, /*  2 <- ITEM_IRON_HELMET, ITEM_SILVER_HELMET */
	{ 1, 0 }, /*  3 <- ITEM_SHIRT */
	{ 0, 0 }, /*  4 <- ITEM_TROWSERS */
	{ 0, 0 }, /*  5 <- ITEM_SHOES */
	{ 0, 0 }, /*  6 <- ITEM_BOOTS */
	{ 0, 0 }, /*  7 <- ITEM_LACEUP_SHOE */
	{ 3, 3 }, /*  8 <- ITEM_LEATHER_ARMOR */
	{ 5, 4 }, /*  9 <- ITEM_SCALE_ARMOR */
	{ 0, 0 }, /* 10 <- ITEM_ROBE_GREEN, ITEM_ROBE_GREEN_2 */
	{ 0, 0 }, /* 11 <- ITEM_ROBE_RED */
	{ 2, 1 }, /* 12 <- ITEM_POT_HELMET */
	{ 1, 0 }, /* 13 <- ITEM_LEATHER_HELMET */
	{ 2, 2 }, /* 14 <- ITEM_SURCOAT */
	{ 4, 4 }, /* 15 <- ITEM_CHAIN_MAIL */
	{ 3, 3 }, /* 16 <- ITEM_KROETENHAUT */
	{ 2, 1 }, /* 17 <- ITEM_PLATTENZEUG */
	{ 1, 1 }, /* 18 <- ITEM_KETTENZEUG */
	{ 1, 0 }, /* 19 <- ITEM_LEDERZEUG */
	{ 1, 1 }, /* 20 <- ITEM_WINTER_COAT */
	{ 1, 0 }, /* 21 <- ITEM_HELMET_CURSED */
	{ 2, 4 }, /* 22 <- ITEM_CHAIN_MAIL_CURSED */
	{ 5, 3 }, /* 23 <- ITEM_CHAIN_MAIL_MAGIC */
	{-1, 0 }
}; // ds:0x0877

const struct specialitem_descr g_specialitems_table[14] = {
	{ 0, 0,  0 }, /*  0 <- DUMMY */
	{ 2, 5,  1 }, /*  1 <- arcano */
	{ 0, 1,  2 }, /*  2 <- read_recipe */
	{ 0, 1,  3 }, /*  3 <- read_document */
	{ 2, 3,  4 }, /*  4 <- armatrutz */
	{ 0, 6,  5 }, /*  5 <- flim_flam */
	{ 0, 1,  6 }, /*  6 <- debtbook */
	{ 0, 1,  7 }, /*  7 <- orcdocument */
	{ 0, 1,  8 }, /*  8 <- weapon_poison */
	{ 0, 1,  9 }, /*  9 <- myastmatic */
	{ 0, 1, 10 }, /* 10 <- hylailic */
	{ 0, 1, 11 }, /* 11 <- magic_book */
	{ 0, 1, 12 }, /* 12 <- brenne */
	{ 0, 1, 13 }  /* 13 <- bag */
}; // ds:0x08a9

signed short g_poison_potions[10] = {
	ITEM_SHURIN_POISON,
	ITEM_ARAX_POISON,
	ITEM_ANGST_POISON,
	ITEM_SLEEP_POISON,
	ITEM_GOLDLEIM,
	ITEM_LOTUS_POISON,
	ITEM_KUKRIS,
	ITEM_BANNSTAUB,
	ITEM_KROETEN_POISON,
	-1
}; // ds:0x08d3
signed short g_herbs_toxic[5] = {
	ITEM_SHURIN,
	ITEM_ALRAUNE,
	ITEM_LOTUS,
	ITEM_KROTENSCHEMEL,
	-1
}; // ds:0x08e7
signed short g_herbs_uneatable[7] = {
	ITEM_ILMENBLATT,
	ITEM_FINAGE_TREE,
	ITEM_JORUGA_ROOT,
	ITEM_KAIRANHALM,
	ITEM_OLGIN_ROOT,
	ITEM_DONF_SPRING,
	-1
}; // ds:0x08f1
signed short g_elixir_potions[8] = {
	ITEM_MU_ELIXIR,
	ITEM_KL_ELIXIR,
	ITEM_CH_ELIXIR,
	ITEM_FF_ELIXIR,
	ITEM_GE_ELIXIR,
	ITEM_IN_ELIXIR,
	ITEM_KK_ELIXIR,
	-1
}; // ds:0x08ff
signed short g_bad_elixirs[8] = {
	ITEM_MU_ELIXIR_BAD,
	ITEM_KL_ELIXIR_BAD,
	ITEM_CH_ELIXIR_BAD,
	ITEM_FF_ELIXIR_BAD,
	ITEM_GE_ELIXIR_BAD,
	ITEM_IN_ELIXIR_BAD,
	ITEM_KK_ELIXIR_BAD,
	-1
}; // ds:0x090f
signed short g_attack_items[3] = {
	ITEM_HYLAILIC_FIRE,
	ITEM_MIASTHMATICUM,
	-1
}; // ds:0x091f
signed char g_monname_genders[78] = { 0, 0, 0, 0, 0, 0, 2, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // ds:0x0925
struct staffspell_descr g_staffspell_descriptions[7] = {
	/* The following are the older skill test attributes from DSA2 / early DSA3. */
	{ ATTRIB_KL, ATTRIB_KL, ATTRIB_CH,  1, 21, 1 },
	{ ATTRIB_KL, ATTRIB_KL, ATTRIB_CH,  0, 23, 0 }, /* maybe handicap 0 should be handicap 2 instead. has to be checked with the official DSA2/early DSA3 rules. */
	{ ATTRIB_KL, ATTRIB_CH, ATTRIB_CH,  3, 19, 0 },
	{ ATTRIB_KL, ATTRIB_CH, ATTRIB_CH,  4, 27, 0 },
	{ ATTRIB_MU, ATTRIB_KL, ATTRIB_KL, 99, 30, 0 }, /* staff spell benefits are not implemented from the 5th one on. For that reason probably, the handicap is set to 99. */
	{ ATTRIB_MU, ATTRIB_CH, ATTRIB_CH, 99, 26, 0 },
	{ ATTRIB_MU, ATTRIB_KL, ATTRIB_CH, 99, 21, 2 }
}; // ds:0x0973
struct spell_descr g_spell_descriptions[87] = {
	{  0                        , 0        , 0        , 0        ,  0,  0, 0, 0, 0, 0 }, /* ... strangely, an all-zero entry */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_KL, ATTRIB_IN, ATTRIB_CH, -1,  0, 1, 2, 0, 0 }, /* Beherrschung brechen */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_IN, ATTRIB_FF, -1,  0, 0, 0, 0, 0 }, /* Destructibo Arcanitas */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_IN, ATTRIB_CH, ATTRIB_KK, -1,  1, 1, 0, 0, 0 }, /* Gardianum Paradei */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_IN, ATTRIB_CH, -1,  1, 0, 1, 1, 0 }, /* Illusionen zerstoeren */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_MU, ATTRIB_KL, ATTRIB_CH, -1,  0, 1, 2, 0, 0 }, /* Verwandlung beenden */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_KL, ATTRIB_CH, ATTRIB_KK, 12,  1, 0, 3, 0, 1 }, /* Band und Fessel */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_IN, ATTRIB_CH, ATTRIB_CH,  8,  1, 0, 1, 0, 1 }, /* Bannbaladin */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_MU, ATTRIB_CH, ATTRIB_CH,  8,  1, 0, 1, 0, 1 }, /* Boeser Blick */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_KL, ATTRIB_IN, ATTRIB_CH,  8,  1, 0, 1, 0, 1 }, /* Grosse Gier */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_KL, ATTRIB_KL, ATTRIB_CH, -1,  1, 0, 1, 0, 1 }, /* Grosse Verwirrung */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_MU, ATTRIB_MU, ATTRIB_CH, -1,  1, 0, 1, 0, 1 }, /* Herr ueber das Tierreich */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_IN, ATTRIB_CH,  7,  1, 0, 1, 0, 1 }, /* Horriphobus */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_MU, ATTRIB_KL, ATTRIB_KK,  6, -1, 0, 0, 0, 1 }, /* Magischer Raub */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_IN, ATTRIB_CH,  5, -1, 1, 0, 0, 1 }, /* Respondami Veritar */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_MU, ATTRIB_CH, ATTRIB_CH, -1,  1, 1, 1, 0, 1 }, /* Sanftmut */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_KL, ATTRIB_CH, ATTRIB_CH,  8,  1, 0, 3, 1, 1 }, /* Somnigravis Tausendschlaf */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_MU, ATTRIB_KL, ATTRIB_CH,  6,  1, 0, 1, 1, 1 }, /* Zwingtanz */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_MU, ATTRIB_CH, 13, -1, 0, 0, 0, 0 }, /* Furor Blut und Sulphurdampf */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_MU, ATTRIB_MU, ATTRIB_CH, 13, -1, 0, 0, 0, 0 }, /* Geister austreiben */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_MU, ATTRIB_MU, ATTRIB_CH, 11, -1, 0, 0, 0, 0 }, /* Geister beschwoeren */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_MU, ATTRIB_CH, 23, -1, 0, 0, 0, 0 }, /* Heptagon und Kroetenei */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_MU, ATTRIB_CH, ATTRIB_CH, 13,  1, 2, 3, 1, 0 }, /* Kraehenruf */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_MU, ATTRIB_CH, 15,  1, 0, 1, 0, 0 }, /* Skelettarius Kryptaduft */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_MU, ATTRIB_KL, ATTRIB_CH, -1, -1, 0, 0, 0, 0 }, /* Elementare Beschwoeren */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_KK, ATTRIB_KK,  5, -1, 0, 0, 0, 0 }, /* Nihilatio Gravitas */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_IN, ATTRIB_GE, ATTRIB_KK, -1, -1, 0, 0, 0, 0 }, /* Solidirid Farbenspiel */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_KL, ATTRIB_GE, ATTRIB_KK,  7,  1, 1, 2, 0, 0 }, /* Axxeleratus Blitzgeschwind */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_KL, ATTRIB_FF,  6, -1, 0, 0, 0, 0 }, /* Foramen Foraminor */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_FF, ATTRIB_KK, -1, -1, 0, 0, 0, 0 }, /* Motoricus Motilitich */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_IN, ATTRIB_GE, ATTRIB_GE,  7, -1, 0, 0, 0, 0 }, /* Spurlos trittlos faehrtenlos */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_KL, ATTRIB_KK, -1, -1, 0, 0, 0, 0 }, /* Transversalis Teleport */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_KL, ATTRIB_GE, ATTRIB_KK,  5, -1, 0, 0, 0, 0 }, /* Ueber Eis und ueber Schnee */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_IN, ATTRIB_CH, -1,  0, 1, 2, 0, 0 }, /* Balsam Salabunde */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_IN, ATTRIB_CH, ATTRIB_FF, -1,  0, 1, 2, 0, 0 }, /* Hexenspeichel */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_KL, ATTRIB_CH, -1,  0, 1, 2, 0, 0 }, /* Klarum Purum Kraeutersud */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_KL, ATTRIB_CH, ATTRIB_KK,  7, -1, 1, 2, 0, 0 }, /* Ruhe Koerper ruhe Geist */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_MU, ATTRIB_IN, ATTRIB_CH, -1,  0, 1, 0, 0, 0 }, /* Tiere besprechen */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_IN, ATTRIB_FF,  5,  0, 1, 0, 0, 0 }, /* Adleraug und Luchsenohr */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_KL, ATTRIB_IN, 10, -1, 0, 0, 0, 0 }, /* Analues Arcanstruktur */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_IN, ATTRIB_CH,  7,  1, 1, 1, 1, 1 }, /* Eigenschaften seid gelesen */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_KL, ATTRIB_KL, ATTRIB_IN,  5,  1, 1, 0, 0, 0 }, /* Exposami Creatur */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_IN, ATTRIB_CH,  5, -1, 1, 0, 0, 0 }, /* Odem Arcanum */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_KL, ATTRIB_KK,  5, -1, 1, 0, 0, 0 }, /* Penetrizzel Holz und Stein */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_IN, ATTRIB_CH,  5, -1, 1, 0, 0, 1 }, /* Sensibar wahr und klar */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_IN, ATTRIB_CH, ATTRIB_GE,  5,  1, 1, 0, 0, 0 }, /* Chamaelioni Mimikry */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_CH, ATTRIB_GE,  8,  1, 1, 0, 0, 0 }, /* Duplicatus Doppelpein */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_KL, ATTRIB_CH, ATTRIB_GE,  6, -1, 1, 0, 0, 0 }, /* Harmlose Gestalt */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_KL, ATTRIB_IN, ATTRIB_CH,  4,  1, 3, 0, 0, 0 }, /* Hexenknoten */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_KL, ATTRIB_IN, ATTRIB_GE,  5,  1, 0, 3, 1, 0 }, /* Blitz dich Find */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_KL, ATTRIB_KK, -1,  1, 0, 0, 0, 0 }, /* Ecliptifactus */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_CH, ATTRIB_GE,  5,  1, 0, 3, 0, 0 }, /* Eisenrost und gruener Span */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_GE, ATTRIB_KK, -1,  1, 0, 3, 1, 0 }, /* Fulminictus Donnerkeil */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_GE, ATTRIB_FF, -1,  1, 0, 3, 1, 0 }, /* Ignifaxius Flammenstrahl */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_CH, ATTRIB_GE, ATTRIB_KK,  5,  1, 0, 3, 1, 1 }, /* Plumbumbarum und Narretei */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_MU, ATTRIB_IN, ATTRIB_CH,  4,  1, 0, 3, 1, 0 }, /* Radau */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_IN, ATTRIB_CH, -1,  1, 1, 2, 0, 0 }, /* Saft Kraft Monstermacht */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_IN, ATTRIB_GE, ATTRIB_FF,  5,  1, 1, 2, 0, 0 }, /* Scharfes Auge und sichre Hand */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_IN, ATTRIB_IN, ATTRIB_CH,  2, -1, 1, 0, 0, 0 }, /* Hexenblick */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_KL, ATTRIB_CH, 11, -1, 1, 0, 0, 0 }, /* Nekropathia modernd Leich */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_MU, ATTRIB_IN, ATTRIB_GE,  7, -1, 1, 0, 0, 0 }, /* Adler Wolf und Hammerhai */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_KL, ATTRIB_CH,  7,  1, 1, 2, 1, 0 }, /* Arcano Psychostabilis */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_IN, ATTRIB_GE, ATTRIB_KK, -1,  1, 1, 0, 0, 0 }, /* Armatrutz */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_CH, ATTRIB_CH,  7,  0, 1, 2, 0, 0 }, /* Charisma steigern */
	{  SPELL_DESC_HEROTYPE_WITCH, ATTRIB_MU, ATTRIB_MU, ATTRIB_KK,  7,  0, 1, 0, 0, 0 }, /* Feuerbann */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_KL, ATTRIB_CH, ATTRIB_FF,  7,  0, 1, 2, 0, 0 }, /* Fingerfertigkeit steigern */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_CH, ATTRIB_GE,  7,  0, 1, 2, 0, 0 }, /* Gewandheit steigern */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_KL, ATTRIB_IN, ATTRIB_CH,  7,  0, 1, 2, 0, 0 }, /* Intuition steigern */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_KL, ATTRIB_CH, ATTRIB_KK,  7,  0, 1, 2, 0, 0 }, /* Koerperkraft steigern */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_KL, ATTRIB_CH,  7,  0, 1, 2, 0, 0 }, /* Klugheit steigern */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_MU, ATTRIB_KL, ATTRIB_CH,  7,  0, 1, 2, 0, 0 }, /* Mut steigern */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_GE, ATTRIB_KK, -1,  0, 0, 0, 0, 0 }, /* Mutabili Hybridil */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_IN, ATTRIB_CH, ATTRIB_KK, 13,  1, 0, 3, 1, 1 }, /* Paralue Paralein */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_IN, ATTRIB_CH, -1,  1, 4, 1, 0, 1 }, /* Salander Mutanderer */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_MU, ATTRIB_KL, ATTRIB_KK, -1, -1, 1, 0, 0, 0 }, /* In See und Fluss */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_KL, ATTRIB_GE, -1,  0, 1, 0, 0, 0 }, /* Visibili Vanitar */
	{  SPELL_DESC_HEROTYPE_SELF , ATTRIB_KL, ATTRIB_KL, ATTRIB_FF,  5, -1, 0, 0, 0, 0 }, /* Abvenenum Pest und Galle */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_KL, ATTRIB_CH, ATTRIB_KK,  5, -1, 0, 0, 0, 0 }, /* Aeolitus Windgebraus */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_KL, ATTRIB_KK, -1, -1, 0, 0, 0, 0 }, /* Brenne toter Stoff */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_KL, ATTRIB_FF, ATTRIB_KK, -1, -1, 0, 0, 0, 0 }, /* Claudibus Clavistibor */
	{  SPELL_DESC_HEROTYPE_DRUID, ATTRIB_KL, ATTRIB_KL, ATTRIB_FF,  3,  1, 0, 0, 0, 0 }, /* Dunkelheit */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_KL, ATTRIB_KK, -1, -1, 0, 0, 0, 0 }, /* Weiches erstarre */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_KL, ATTRIB_KL, ATTRIB_FF,  1, -1, 0, 0, 0, 0 }, /* Flim Flam Funkel */
	{  SPELL_DESC_HEROTYPE_MAGE , ATTRIB_MU, ATTRIB_KL, ATTRIB_KK, -1, -1, 0, 0, 0, 0 }, /* Hartes schmelze */
	{  SPELL_DESC_HEROTYPE_GELF , ATTRIB_KL, ATTRIB_KL, ATTRIB_CH, -1, -1, 0, 0, 0, 0 }, /* Silentium Silentille */
	{  SPELL_DESC_HEROTYPE_IELF , ATTRIB_KL, ATTRIB_CH, ATTRIB_KK,  5, -1, 0, 0, 0, 0 }, /* Sturmgebruell besaenftge dich */
	{                        -1 ,         0,         0,         0,  0,  0, 0, 0, 0, 0 }  /* ... terminator entry */
}; // ds:0x099d
const struct spells_index g_spells_index[8] = {
	{ 0x01, 0x05 },
	{ 0x06, 0x0c },
	{ 0x12, 0x06 },
	{ 0x18, 0x03 },
	{ 0x1b, 0x06 },
	{ 0x21, 0x05 },
	{ 0x26, 0x07 },
	{ 0x2d, 0x04 }
}; // ds:0x0d03
const struct spells_index g_spells_index2[4] = {
	{ 0x31, 0x09 },
	{ 0x3a, 0x02 },
	{ 0x3c, 0x10 },
	{ 0x4c, 0x0a }
}; // ds:0x0d13

const signed short g_magic_schools_1[7] = { 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x002a, -1 }; // ds:0x0d1b, array terminated by -1
const signed short g_magic_schools_2[6] = { 0x0007, 0x000c, 0x000e, 0x0010, 0x002c, -1 }; // ds:0x0d29, array terminated by -1
const signed short g_magic_schools_3[7] = { 0x0012, 0x0013, 0x0015, 0x0017, 0x0018, 0x003b, -1 }; // ds:0x0d35, array terminated by -1
const signed short g_magic_schools_4[6] = { 0x001b, 0x001c, 0x001d, 0x001e, 0x001f, -1 }; // ds:0x0d43, array terminated by -1
const signed short g_magic_schools_5[6] = { 0x0021, 0x0023, 0x0024, 0x003d, 0x0046, -1 }; // ds:0x0d4f, array terminated by -1
const signed short g_magic_schools_6[8] = { 0x0026, 0x0027, 0x0029, 0x0028, 0x002a, 0x002b, 0x002c, -1 }; // ds:0x0d5b, array terminated by -1
const signed short g_magic_schools_7[8] = { 0x0031, 0x0033, 0x0034, 0x0035, 0x0036, 0x0039, 0x003e, -1 }; // ds:0x0d6b, array terminated by -1
const signed short g_magic_schools_8[6] = { 0x003c, 0x003e, 0x0048, 0x0049, 0x004b, -1 }; // ds:0x0d7b, array terminated by -1
const signed short g_magic_schools_9[8] = { 0x004c, 0x004e, 0x004f, 0x0050, 0x0052, 0x0053, 0x0054, -1 }; // ds:0x0d87, array terminated by -1
//long g_magic_schools_index[9] = { 0x14fc0d1b, 0x14fc0d29, 0x14fc0d35, 0x14fc0d43, 0x14fc0d4f, 0x14fc0d5b, 0x14fc0d6b, 0x14fc0d7b, 0x14fc0d87 }; // ds:0x0d97; Bit8u*
const signed short* g_magic_schools_index[9] = {
	g_magic_schools_1,
	g_magic_schools_2,
	g_magic_schools_3,
	g_magic_schools_4,
	g_magic_schools_5,
	g_magic_schools_6,
	g_magic_schools_7,
	g_magic_schools_8,
	g_magic_schools_9
};

void (*g_spell_handlers[86])(void) = {
	NULL,
	/* Dispel / Antimagie */
	spell_beherrschung,
	spell_destructibo,
	spell_gardanium,
	spell_illusionen,
	spell_verwandlung,
	/* Domionation / Beherrschung */
	spell_band,
	spell_bannbaladin,
	spell_boeser_blick,
	spell_grosse_gier,
	spell_grosse_ver,
	spell_herrdertiere,
	spell_horriphobus,
	spell_magischerraub,
	spell_respondami,
	spell_sanftmut,
	spell_somnigravis,
	spell_zwingtanz,
	/* Demonology / Demonologie */
	spell_furor_blut,
	spell_geister_bannen,
	spell_geister_rufen,
	spell_heptagon,
	spell_kraehenruf,
	spell_skelettarius,
	/* Elements / Elemente */
	spell_elementare,
	spell_nihilatio,
	spell_solidirid,
	/* Movement / Bewegung */
	spell_axxeleratus,
	spell_foramen,
	spell_motoricus,
	spell_spurlos,
	spell_transversalis,
	spell_ueber_eis,
	/* Healing / Heilung */
	spell_balsam,
	spell_hexenspeichel,
	spell_klarum_purum,
	spell_ruhe_koerper,
	spell_tiere_heilen,
	/* Clairvoyance / Hellsicht */
	spell_adleraug,
	(void(*)(void))spell_analues,
	spell_eigenschaften,
	spell_exposami,
	spell_odem_arcanum,
	spell_penetrizzel,
	spell_sensibar,
	/* Illusion */
	spell_chamaelioni,
	spell_duplicatus,
	spell_harmlos,
	spell_hexenknoten,
	/* Combat / Kampf */
	spell_blitz,
	spell_ecliptifactus,
	spell_eisenrost,
	spell_fulminictus,
	spell_ignifaxius,
	spell_plumbumbarum,
	spell_radau,
	spell_saft_kraft,
	spell_scharfes_auge,
	/* Communication / Verstaendigung */
	spell_hexenblick,
	spell_necropathia,
	/* Transformation / Verwandlung */
	spell_adler,
	spell_arcano,
	spell_armatrutz,
	spell_inc_ch,
	spell_feuerbann,
	spell_inc_ff,
	spell_inc_ge,
	spell_inc_in,
	spell_inc_kk,
	spell_inc_kl,
	spell_inc_mu,
	spell_mutabili,
	spell_paralue,
	spell_salander,
	spell_see,
	spell_visibili,
	/* Transmutation / Veraenderung */
	spell_abvenenum,
	spell_aeolitus,
	spell_brenne,
	spell_claudibus,
	spell_dunkelheit,
	spell_erstarre,
	spell_flimflam,
	spell_schmelze,
	spell_silentium,
	spell_sturmgebr
}; // ds:0x0dbb, function pointer[86]


struct mon_spell_description g_mon_spell_descriptions[15] = {
	{  0, 0, 0, 0, 0, 0, 0, 0 },	/* DUMMY */
	{ 20, 1, 0, 0, 1, 2, 0, 1 },	/* Verwandlung Beenden */
	{  8, 2, 0, 5, 2, 2, 1, 0 },	/* Bannbaladin */
	{  8, 2, 0, 0, 2, 2, 1, 0 },	/* Boeser Blick */
	{  7, 2, 0, 0, 5, 2, 1, 0 },	/* Horriphobus */
	{  7, 1, 0, 1, 4, 6, 0, 1 },	/* Axxeleratus */
	{ -1, 1, 0, 1, 5, 2, 0, 1 },	/* Balsam */
	{  5, 3, 1, 1, 5, 4, 1, 0 },	/* Blitz */
	{  5, 2, 1, 1, 2, 4, 0, 0 },	/* Eisenrost */
	{ -1, 3, 1, 1, 4, 6, 1, 0 },	/* Fulminictus */
	{ -1, 3, 1, 1, 4, 3, 1, 0 },	/* Ignifaxius */
	{  5, 3, 1, 2, 4, 6, 1, 0 },	/* Plumbumbarum */
	{ -1, 1, 0, 0, 5, 2, 0, 1 },	/* Saft, Kraft, Monstermacht */
	{ -1, 0, 0, 5, 4, 6, 0, 1 },	/* Armatrutz */
	{ 13, 3, 0, 5, 2, 6, 1, 0 }	/* Paralue */
}; // ds:0x0f13

Bit8s g_mon_spell_repertoire[11][5] = {
	{ 1,  7,  8, -1, -1 },
	{ 1,  7,  3,  4, -1 },
	{ 1,  7,  4,  2,  9 },
	{ 9, 14, 10, -1, -1 },
	{ 9, 10, 12, 13, -1 },
	{ 9, 10, 12, 11,  7 },
	{ 9, 14,  7, -1, -1 },
	{ 9, 14,  7, 12,  4 },
	{ 6,  1,  8, -1, -1 },
	{ 6,  1,  5,  7, -1 },
	{ 6, 14,  5, 12,  9 }
}; // ds:0x0f8b


void (*g_mon_spellhandlers[15])(void) = {
	NULL,
	mspell_verwandlung,		/*  1 */
	mspell_bannbaladin,		/*  2 */
	mspell_boeser_blick,		/*  3 */
	mspell_horriphobus,		/*  4 */
	mspell_axxeleratus,		/*  5 */
	mspell_balsam,			/*  6 */
	mspell_blitz,			/*  7 */
	mspell_eisenrost,		/*  8 */
	mspell_fulminictus,		/*  9 */
	mspell_ignifaxius,		/* 10 */
	mspell_plumbumbarum,		/* 11 */
	mspell_saft_kraft,		/* 12 */
	mspell_armatrutz,		/* 13 */
	mspell_paralue,			/* 14 */
}; // ds:0x0fc2, function pointer[15]

const struct skill_descr g_skill_descriptions[52] = {
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x00, 0x01 },
	{ 0x00, 0x04, 0x06, 0x02 },
	{ 0x00, 0x04, 0x06, 0x02 },
	{ 0x00, 0x05, 0x04, 0x02 },
	{ 0x02, 0x04, 0x06, 0x02 },
	{ 0x00, 0x05, 0x04, 0x02 },
	{ 0x00, 0x04, 0x06, 0x02 },
	{ 0x00, 0x06, 0x06, 0x02 },
	{ 0x00, 0x05, 0x04, 0x02 },
	{ 0x02, 0x04, 0x04, 0x02 },
	{ 0x01, 0x05, 0x06, 0x02 },
	{ 0x01, 0x05, 0x02, 0x02 },
	{ 0x05, 0x02, 0x02, 0x02 },
	{ 0x00, 0x01, 0x02, 0x02 },
	{ 0x01, 0x05, 0x02, 0x02 },
	{ 0x00, 0x05, 0x02, 0x02 },
	{ 0x01, 0x05, 0x02, 0x02 },
	{ 0x01, 0x05, 0x05, 0x02 },
	{ 0x01, 0x05, 0x04, 0x02 },
	{ 0x03, 0x04, 0x06, 0x02 },
	{ 0x01, 0x05, 0x03, 0x02 },
	{ 0x01, 0x05, 0x05, 0x02 },
	{ 0x00, 0x01, 0x05, 0x02 },
	{ 0x05, 0x03, 0x04, 0x02 },
	{ 0x00, 0x01, 0x03, 0x03 },
	{ 0x01, 0x01, 0x05, 0x03 },
	{ 0x01, 0x01, 0x05, 0x03 },
	{ 0x01, 0x01, 0x05, 0x03 },
	{ 0x01, 0x05, 0x02, 0x03 },
	{ 0x00, 0x01, 0x02, 0x03 },
	{ 0x01, 0x01, 0x03, 0x03 },
	{ 0x01, 0x01, 0x03, 0x03 },
	{ 0x01, 0x05, 0x02, 0x03 },
	{ 0x00, 0x05, 0x02, 0x02 },
	{ 0x05, 0x02, 0x03, 0x02 },
	{ 0x00, 0x02, 0x03, 0x02 },
	{ 0x00, 0x01, 0x05, 0x02 },
	{ 0x00, 0x01, 0x02, 0x02 },
	{ 0x01, 0x02, 0x03, 0x02 },
	{ 0x01, 0x05, 0x03, 0x02 },
	{ 0x05, 0x03, 0x03, 0x02 },
	{ 0x00, 0x05, 0x03, 0x02 },
	{ 0x01, 0x05, 0x05, 0x01 },
	{ 0x01, 0x05, 0x05, 0x01 }
}; // ds:0x0ffe
const struct skills_index g_skills_index[7] = {
	{  0,  9 },
	{  9, 10 },
	{ 19,  7 },
	{ 26,  6 },
	{ 32,  9 },
	{ 41,  9 },
	{ 50,  2 }
}; // ds:0x10ce
signed char g_nvftab_figures_rangeweapon[22][3][4] = {
	{{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}},
	{{0x30, 0x31, 0x32, 0x33}, {0x00, 0x01, 0x02, 0x03}, {0x34, 0x35, 0x36, 0x37}},
	{{0x34, 0x35, 0x36, 0x37}, {0x00, 0x01, 0x02, 0x03}, {0x38, 0x39, 0x3a, 0x3b}},
	{{0x30, 0x31, 0x32, 0x33}, {0x00, 0x01, 0x02, 0x03}, {0x34, 0x35, 0x36, 0x37}},
	{{0x34, 0x35, 0x36, 0x37}, {0x00, 0x01, 0x02, 0x03}, {0x38, 0x39, 0x3a, 0x3b}},
	{{0x30, 0x31, 0x32, 0x33}, {0x00, 0x01, 0x02, 0x03}, {0x34, 0x35, 0x36, 0x37}},
	{{0x2e, 0x2f, 0x30, 0x31}, {0x00, 0x01, 0x02, 0x03}, {0x32, 0x33, 0x34, 0x35}},
	{{0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}},
	{{0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}},
	{{0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}},
	{{0x30, 0x31, 0x32, 0x33}, {0x00, 0x01, 0x02, 0x03}, {0x34, 0x35, 0x36, 0x37}},
	{{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}},
	{{0x30, 0x31, 0x32, 0x33}, {0x00, 0x01, 0x02, 0x03}, {0x34, 0x35, 0x36, 0x37}},
	{{0x39, 0x3a, 0x3b, 0x3c}, {0x00, 0x01, 0x02, 0x03}, {0x3d, 0x3e, 0x3f, 0x40}},
	{{0x39, 0x3a, 0x3b, 0x3c}, {0x00, 0x01, 0x02, 0x03}, {0x3d, 0x3e, 0x3f, 0x40}},
	{{0x34, 0x35, 0x36, 0x37}, {0x00, 0x01, 0x02, 0x03}, {0x38, 0x39, 0x3a, 0x3b}},
	{{0x39, 0x3a, 0x3b, 0x3c}, {0x00, 0x01, 0x02, 0x03}, {0x3d, 0x3e, 0x3f, 0x40}},
	{{0x2e, 0x2f, 0x30, 0x31}, {0x00, 0x01, 0x02, 0x03}, {0x32, 0x33, 0x34, 0x35}},
	{{0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}},
	{{0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}},
	{{0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}, {0x00, 0x01, 0x02, 0x03}},
	{{0x39, 0x3a, 0x3b, 0x3c}, {0x00, 0x01, 0x02, 0x03}, {0x3d, 0x3e, 0x3f, 0x40}}
}; // ds:0x10dc, by rwt/viewdir
short g_nvftab_figures_unconscious[22] = { 0x0000, 0x0038, 0x003c, 0x0038, 0x003c, 0x0038, 0x0038, 0x003f, 0x0043, 0x0032, 0x0038, 0x0000, 0x0038, 0x0041, 0x0041, 0x003c, 0x0041, 0x0038, 0x003f, 0x0045, 0x0032, 0x0041 }; // ds:0x11e4
const struct point8s g_gfxtab_offsets_unconscious[22][4] = {
	{ {  0, 0 }, {  0, 0 }, {  0, 0 }, {  0, 0 } },
	{ {  1, 8 }, {  0, 7 }, {  1, 5 }, {  0, 4 } },
	{ {  1, 8 }, {  0, 7 }, {  1, 5 }, {  0, 4 } },
	{ {  1, 7 }, {  0, 7 }, {  1, 6 }, { -1, 5 } },
	{ {  1, 8 }, {  0, 7 }, {  0, 5 }, {  0, 5 } },
	{ {  1, 7 }, {  0, 7 }, {  1, 6 }, { -1, 5 } },
	{ { -3, 5 }, {  0, 4 }, {  1, 6 }, {  0, 4 } },
	{ {  1, 7 }, { -1, 8 }, {  0, 6 }, { -1, 6 } },
	{ {  1, 6 }, { -1, 6 }, {  0, 5 }, {  0, 4 } },
	{ {  1, 6 }, { -1, 6 }, {  1, 5 }, {  0, 4 } },
	{ {  1, 8 }, {  0, 7 }, {  1, 5 }, {  0, 4 } },
	{ {  0, 0 }, {  0, 0 }, {  0, 0 }, {  0, 0 } },
	{ {  1, 8 }, {  0, 7 }, {  1, 5 }, {  0, 4 } },
	{ {  1, 8 }, {  0, 7 }, {  1, 5 }, {  0, 4 } },
	{ {  1, 7 }, {  0, 7 }, {  1, 6 }, { -1, 5 } },
	{ {  1, 8 }, {  0, 7 }, {  0, 5 }, {  0, 5 } },
	{ {  1, 7 }, {  0, 7 }, {  1, 6 }, { -1, 5 } },
	{ { -3, 5 }, {  0, 4 }, {  1, 6 }, {  0, 4 } },
	{ {  1, 7 }, { -1, 8 }, {  0, 6 }, { -1, 6 } },
	{ {  1, 6 }, { -1, 6 }, {  0, 5 }, {  0, 4 } },
	{ {  1, 6 }, { -1, 6 }, {  1, 5 }, {  0, 4 } },
	{ {  1, 8 }, {  0, 7 }, {  1, 5 }, {  0, 4 } }
}; // ds:0x1210, ; by viewdir
const Bit8s g_gfxtab_figures_main[125][5] = {
	{ -0x01, -0x01, -0x01, -0x01, -1 },
	{ 0x01, 0x02, 0x03, 0x04, -1 },
	{ 0x05, 0x06, 0x07, 0x08, -1 },
	{ 0x09, 0x0a, 0x0b, 0x0c, -1 },
	{ 0x0d, 0x0e, 0x0f, 0x10, -1 },
	{ 0x11, 0x12, 0x13, 0x14, -1 },
	{ 0x15, 0x16, 0x17, 0x18, -1 },
	{ 0x19, 0x1a, 0x1b, 0x1c, 0x1d },
	{ 0x1e, 0x1f, 0x20, -0x01, 0x21 },
	{ 0x22, 0x23, 0x24, 0x25, 0x26 },
	{ 0x27, 0x28, 0x29, 0x2a, 0x2b },
	{ -0x01, -0x01, -0x01, -0x01, -1 },
	{ 0x2d, 0x2e, 0x2f, 0x30, -1 },
	{ 0x31, 0x32, 0x33, 0x34, -1 },
	{ 0x35, 0x36, 0x37, 0x38, -1 },
	{ 0x39, 0x3a, 0x3b, 0x3c, -1 },
	{ 0x3d, 0x3e, 0x3f, 0x40, -1 },
	{ 0x41, 0x42, 0x43, 0x44, -1 },
	{ 0x45, 0x46, 0x47, 0x48, 0x49 },
	{ 0x4a, 0x4b, 0x4c, -0x01, 0x4d },
	{ 0x4e, 0x4f, 0x50, 0x51, 0x52 },
	{ 0x53, 0x54, 0x55, 0x56, 0x57 },
	{ -0x01, -0x01, -0x01, -0x01, -1 },
	{ 0x59, 0x5a, -0x01, -0x01, -1 },
	{ 0x5b, 0x5c, -0x01, -0x01, -1 },
	{ 0x5d, 0x5e, -0x01, -0x01, -1 },
	{ 0x5f, 0x60, -0x01, -0x01, -1 },
	{ 0x61, 0x62, -0x01, -0x01, -1 },
	{ 0x63, 0x64, -0x01, -0x01, -1 },
	{ 0x65, 0x66, -0x01, -0x01, -1 },
	{ 0x67, 0x68, -0x01, -0x01, -1 },
	{ 0x69, 0x6a, -0x01, -0x01, -1 },
	{ 0x6b, 0x6c, -0x01, -0x01, -1 },
	{ 0x6d, 0x6e, -0x01, -0x01, -1 },
	{ 0x6f, 0x70, -0x01, -0x01, -1 },
	{ 0x71, 0x72, -0x01, -0x01, -1 },
	{ 0x73, 0x74, -0x01, -0x01, -1 },
	{ 0x75, 0x76, -0x01, -0x01, -1 },
	{ 0x77, 0x78, -0x01, -0x01, -1 },
	{ 0x79, 0x7a, -0x01, -0x01, -1 },
	{ 0x7b, 0x7c, -0x01, -0x01, -1 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00 }
}; // ds:0x12c0
const struct point8s g_gfxtab_offsets_main[125][5] = {
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 6, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 3,15 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ {-8, 2 }, { 8, 3 }, { 7, 7 }, {-7, 8 }, { 3, 5} },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0} },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 1, 0 }, { 1, 0 }, { 1, 6 } },
	{ { 0, 0 }, { 0, 0 }, {-1, 1 }, {-1, 0 }, {-4, 3 } },
	{ { 0, 0 }, {-2, 0 }, { 0, 1 }, { 0, 1 }, {-5, 5 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 4, 5 } },
	{ { 0, 1 }, { 3, 0 }, { 6, 6 }, {-6, 6 }, { 5, 2 } },
	{ {-8, 2 }, { 8, 3 }, { 7, 7 }, {-7, 8 }, { 3, 5 } },
	{ { 0, 1 }, { 3, 0 }, { 6, 6 }, {-6, 6 }, { 5, 2 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } }
}; // ds:0x1531, ; by viewdir+dead
signed short g_nvftab_figures_dead[22] = { 0x0000, 0x002f, 0x0033, 0x002f, 0x0033, 0x002f, 0x002d, 0x003e, 0x0042, 0x0031, 0x002f, 0x0000, 0x002f, 0x0038, 0x0038, 0x0033, 0x0038, 0x002d, 0x003e, 0x0044, 0x0031, 0x0038 }; // ds:0x1a13
unsigned char g_unkn_005[38] = { 0x00, 0x00, 0x2c, 0x00, 0x38, 0x00, 0x38, 0x00, 0x3c, 0x00, 0x32, 0x00, 0x38, 0x00, 0x48, 0x00, 0x38, 0x00, 0x2c, 0x00, 0x30, 0x00, 0x44, 0x00, 0x3c, 0x00, 0x38, 0x00, 0x3c, 0x00, 0x40, 0x00, 0x48, 0x00, 0x3c, 0x00, 0x48, 0x00 }; // ds:0x1a3f
short g_gfx_ani_descriptions_01[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x0018, 0x0019, 0x001a, 0x001b, 0x0028, 0x0029, 0x002a, 0x002b, 0x0044, 0x004d, 0x004e, 0x004f, 0x0050, 0x005b, 0x005c, 0x005d, 0x005e, -0x0001, -0x0001, -0x0001, -0x0001, 0x00a0, 0x00a1, 0x00a2, 0x00a3, -0x0001, -0x0001, -0x0001, -0x0001, 0x006e, 0x006f, 0x0070, 0x0071, 0x007a, 0x007b, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00b8, 0x00b9, 0x00ba, 0x00bb }; // ds:0x1a65
short g_gfx_ani_descriptions_02[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x0018, 0x0019, 0x001a, 0x001b, 0x002c, 0x002d, 0x002e, 0x002f, 0x0045, 0x004d, 0x004e, 0x004f, 0x0050, 0x005b, 0x005c, 0x005d, 0x005e, -0x0001, -0x0001, -0x0001, -0x0001, 0x00a0, 0x00a1, 0x00a2, 0x00a3, -0x0001, -0x0001, -0x0001, -0x0001, 0x006e, 0x006f, 0x0070, 0x0071, 0x007a, 0x007b, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00b8, 0x00b9, 0x00ba, 0x00bb }; // ds:0x1ae7
short g_gfx_ani_descriptions_03[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x0018, 0x0019, 0x001a, 0x001b, 0x0028, 0x0029, 0x002a, 0x002b, 0x0046, 0x004d, 0x004e, 0x004f, 0x0050, 0x005b, 0x005c, 0x005d, 0x005e, -0x0001, -0x0001, -0x0001, -0x0001, 0x00a0, 0x00a1, 0x00a2, 0x00a3, -0x0001, -0x0001, -0x0001, -0x0001, 0x006e, 0x006f, 0x0070, 0x0071, 0x007a, 0x007b, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00b8, 0x00b9, 0x00ba, 0x00bb }; // ds:0x1b69
short g_gfx_ani_descriptions_04[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x0018, 0x0019, 0x001a, 0x001b, 0x002c, 0x002d, 0x002e, 0x002f, 0x0045, 0x004d, 0x004e, 0x004f, 0x0050, 0x005b, 0x005c, 0x005d, 0x005e, -0x0001, -0x0001, -0x0001, -0x0001, 0x00a0, 0x00a1, 0x00a2, 0x00a3, -0x0001, -0x0001, -0x0001, -0x0001, 0x006e, 0x006f, 0x0070, 0x0071, 0x007a, 0x007b, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00b8, 0x00b9, 0x00ba, 0x00bb }; // ds:0x1beb
short g_gfx_ani_descriptions_05[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x0018, 0x0019, 0x001a, 0x001b, 0x0028, 0x0029, 0x002a, 0x002b, 0x0046, 0x004d, 0x004e, 0x004f, 0x0050, 0x005b, 0x005c, 0x005d, 0x005e, -0x0001, -0x0001, -0x0001, -0x0001, 0x00a0, 0x00a1, 0x00a2, 0x00a3, -0x0001, -0x0001, -0x0001, -0x0001, 0x006e, 0x006f, 0x0070, 0x0071, 0x007a, 0x007b, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00b8, 0x00b9, 0x00ba, 0x00bb }; // ds:0x1c6d
short g_gfx_ani_descriptions_06[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x000c, 0x000d, 0x000e, 0x000f, 0x001c, 0x001d, 0x001e, 0x001f, 0x0030, 0x0031, 0x0032, 0x0033, 0x0047, 0x0051, 0x0052, 0x0053, 0x0054, 0x005f, 0x0060, 0x0061, 0x0062, -0x0001, -0x0001, -0x0001, -0x0001, 0x00a4, 0x00a5, 0x00a6, 0x00a7, -0x0001, -0x0001, -0x0001, -0x0001, 0x0076, 0x0077, 0x0078, 0x0079, 0x007e, 0x007f, 0x0080, 0x0081, 0x008c, 0x008d, 0x008e, 0x008f, 0x0098, 0x0099, 0x009a, 0x009b, 0x00ac, 0x00ad, 0x00ae, 0x00af, 0x00bc, 0x00bd, 0x00be, 0x00bf }; // ds:0x1cef
short g_gfx_ani_descriptions_07[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0010, 0x0011, 0x0012, 0x0013, 0x0020, 0x0021, 0x0022, 0x0023, 0x0034, 0x0035, 0x0036, 0x0037, 0x0048, 0x0055, 0x0056, 0x004f, 0x0050, 0x0063, 0x005c, 0x0064, 0x0065, 0x00c0, 0x00c1, 0x00c2, 0x00c3, -0x0001, -0x0001, -0x0001, -0x0001, 0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x006e, 0x006f, 0x0070, 0x0071, 0x0082, 0x0083, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00b0, 0x00b1, 0x00b2, 0x00b3, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x1d71
short g_gfx_ani_descriptions_08[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0014, 0x0015, 0x0016, 0x0017, 0x0024, 0x0025, 0x0026, 0x0027, 0x0038, 0x0039, 0x003a, 0x003b, 0x0049, 0x0057, 0x0058, 0x0059, 0x005a, 0x0066, 0x0067, 0x0068, 0x0069, 0x00c4, 0x00c5, 0x00c6, 0x00c7, -0x0001, -0x0001, -0x0001, -0x0001, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x0072, 0x0073, 0x0074, 0x0075, 0x007a, 0x007b, 0x007c, 0x007d, 0x0090, 0x0091, 0x0092, 0x0093, 0x0098, 0x0099, 0x009a, 0x009b, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x1df3
short g_gfx_ani_descriptions_09[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0014, 0x0015, 0x0016, 0x0017, 0x0024, 0x0025, 0x0026, 0x0027, 0x003c, 0x003d, 0x003e, 0x003f, 0x004a, 0x0057, 0x0058, 0x0059, 0x005a, 0x006a, 0x006b, 0x006c, 0x006d, 0x00c4, 0x00c5, 0x00c6, 0x00c7, -0x0001, -0x0001, -0x0001, -0x0001, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x0076, 0x0077, 0x0078, 0x0079, 0x0084, 0x0085, 0x0086, 0x0087, 0x0094, 0x0095, 0x0096, 0x0097, 0x009c, 0x009d, 0x009e, 0x009f, 0x00b4, 0x00b5, 0x00b6, 0x00b7, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x1e75
short g_gfx_ani_descriptions_10[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x0018, 0x0019, 0x001a, 0x001b, 0x0028, 0x0029, 0x002a, 0x002b, 0x0044, 0x004d, 0x004e, 0x004f, 0x0050, 0x005b, 0x005c, 0x005d, 0x005e, 0x00c4, 0x00c5, 0x00c6, 0x00c7, 0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x006e, 0x006f, 0x0070, 0x0071, 0x007a, 0x007b, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00b8, 0x00b9, 0x00ba, 0x00bb }; // ds:0x1ef7
short g_gfx_ani_descriptions_11[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0010, 0x0011, 0x0012, 0x0013, 0x0020, 0x0021, 0x0022, 0x0023, 0x0034, 0x0035, 0x0036, 0x0037, 0x004b, 0x0055, 0x0056, 0x004f, 0x0050, 0x0063, 0x005c, 0x005d, 0x0065, -0x0001, -0x0001, -0x0001, -0x0001, 0x00a0, 0x00a1, 0x00a2, 0x00a3, -0x0001, -0x0001, -0x0001, -0x0001, 0x006e, 0x006f, 0x0070, 0x0071, 0x0082, 0x0083, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b8, 0x00b9, 0x00ba, 0x00bb }; // ds:0x1f79
short g_gfx_ani_descriptions_12[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0010, 0x0011, 0x0012, 0x0013, 0x0020, 0x0021, 0x0022, 0x0023, 0x0040, 0x0041, 0x0042, 0x0043, 0x004c, 0x0055, 0x0056, 0x004f, 0x0050, 0x0063, 0x005c, 0x0064, 0x0065, 0x00cc, 0x00cd, 0x00ce, 0x00cf, -0x0001, -0x0001, -0x0001, -0x0001, 0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x0072, 0x0073, 0x0074, 0x0075, 0x0082, 0x0083, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x1ffb
short g_gfx_ani_descriptions_13[65] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0010, 0x0011, 0x0012, 0x0013, 0x0020, 0x0021, 0x0022, 0x0023, 0x0034, 0x0035, 0x0036, 0x0037, 0x004b, 0x0055, 0x0056, 0x004f, 0x0050, 0x0063, 0x005c, 0x005d, 0x0065, 0x00c4, 0x00c5, 0x00c6, 0x00c7, 0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x006e, 0x006f, 0x0070, 0x0071, 0x0082, 0x0083, 0x007c, 0x007d, 0x0088, 0x0089, 0x008a, 0x008b, 0x0098, 0x0099, 0x009a, 0x009b, 0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b8, 0x00b9, 0x00ba, 0x00bb }; // ds:0x207d
short g_gfx_ani_descriptions_14[37] = { 0x00d4, 0x00d5, 0x00d6, 0x00d7, 0x00d8, 0x00d9, 0x00da, 0x00db, 0x0106, 0x0107, 0x0108, 0x0109, 0x0126, 0x0127, 0x0128, 0x0129, 0x01b6, 0x01b7, 0x01b8, 0x01b9, 0x01dd, 0x0146, 0x0147, 0x0148, 0x0149, 0x017a, 0x017b, 0x017c, 0x017d, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x20ff
short g_gfx_ani_descriptions_15[37] = { 0x00dc, 0x00dd, 0x00de, 0x00df, 0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x010a, 0x010b, 0x010c, 0x010d, 0x012a, 0x012b, 0x012c, 0x012d, 0x01ba, 0x01bb, 0x01bc, 0x01bd, 0x01de, 0x014a, 0x014b, 0x014c, 0x014d, 0x017e, 0x017f, 0x0180, 0x0181, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x2149
short g_gfx_ani_descriptions_16[37] = { 0x00dc, 0x00dd, 0x00de, 0x00df, 0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x010e, 0x010f, 0x0110, 0x0111, 0x012e, 0x012f, 0x0130, 0x0131, 0x01be, 0x01bf, 0x01c0, 0x01c1, 0x01df, 0x014e, 0x014f, 0x0150, 0x0151, 0x0182, 0x0183, 0x0184, 0x0185, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x2193
short g_gfx_ani_descriptions_17[37] = { 0x00e4, 0x00e5, 0x00e6, 0x00e7, 0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x0112, 0x0113, 0x0114, 0x0115, 0x0132, 0x0133, 0x0134, 0x0135, 0x01c2, 0x01c3, 0x01c4, 0x01c5, 0x01e0, 0x0152, 0x0153, 0x0154, 0x0155, 0x0186, 0x0187, 0x0188, 0x0189, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x21dd
short g_gfx_ani_descriptions_18[37] = { 0x00dc, 0x00dd, 0x00de, 0x00df, 0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x010a, 0x010b, 0x010c, 0x010d, 0x012a, 0x012b, 0x012c, 0x012d, 0x01ba, 0x01bb, 0x01bc, 0x01bd, 0x01e2, 0x014a, 0x014b, 0x014c, 0x014d, 0x018e, 0x018f, 0x0190, 0x0191, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x2227
short g_gfx_ani_descriptions_19[37] = { 0x00dc, 0x00dd, 0x00de, 0x00df, 0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x010a, 0x010b, 0x010c, 0x010d, 0x012a, 0x012b, 0x012c, 0x012d, 0x01ba, 0x01bb, 0x01bc, 0x01bd, 0x01e1, 0x0156, 0x0157, 0x0158, 0x0159, 0x018a, 0x018b, 0x018c, 0x018d, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x2271
short g_gfx_ani_descriptions_20[37] = { 0x00dc, 0x00dd, 0x00de, 0x00df, 0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x010a, 0x010b, 0x010c, 0x010d, 0x012a, 0x012b, 0x012c, 0x012d, 0x01ba, 0x01bb, 0x01bc, 0x01bd, 0x01de, 0x015a, 0x015b, 0x015c, 0x015d, 0x0192, 0x0193, 0x0194, 0x0195, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x22bb
short g_gfx_ani_descriptions_21[37] = { 0x00ec, 0x00ed, 0x00ee, 0x00ef, 0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x0116, 0x0117, 0x0118, 0x0119, 0x0136, 0x0137, 0x0138, 0x0139, 0x01c6, 0x01c7, 0x01c8, 0x01c9, 0x01e3, 0x015e, 0x015f, 0x0160, 0x0161, 0x0196, 0x0197, 0x0198, 0x0199, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x2305
short g_gfx_ani_descriptions_22[37] = { 0x00dc, 0x00dd, 0x00f4, 0x00f5, 0x00f6, 0x00e1, 0x00e2, 0x00f7, 0x011a, 0x011b, 0x011c, 0x011d, 0x013a, 0x013b, 0x013c, 0x013d, 0x01ca, 0x01cb, 0x01cc, 0x01cd, 0x01e4, 0x0162, 0x0163, 0x0164, 0x0165, 0x019a, 0x019b, 0x019c, 0x019d, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x234f
short g_gfx_ani_descriptions_23[37] = { 0x00f8, 0x00f9, 0x00de, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00e3, 0x011e, 0x011f, 0x0120, 0x0121, 0x013e, 0x013f, 0x0140, 0x0141, 0x01ca, 0x01cb, 0x01cc, 0x01cd, 0x01e5, 0x0166, 0x0167, 0x0168, 0x0169, 0x019e, 0x019f, 0x01a0, 0x01a1, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x2399
short g_gfx_ani_descriptions_24[37] = { 0x00dc, 0x00dd, 0x00de, 0x00df, 0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x010a, 0x010b, 0x010c, 0x010d, 0x012a, 0x012b, 0x012c, 0x012d, 0x01ba, 0x01ce, 0x01cf, 0x01d0, 0x01e6, 0x016a, 0x016b, 0x016c, 0x016d, 0x01a2, 0x01a3, 0x01a4, 0x01a5, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x23e3
short g_gfx_ani_descriptions_25[37] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x0018, 0x0019, 0x001a, 0x001b, 0x01d1, 0x01d2, 0x01d3, 0x01d4, 0x01e2, 0x016e, 0x016f, 0x0170, 0x0171, 0x01a6, 0x01a7, 0x01a8, 0x01a9, -0x0001, -0x0001, -0x0001, -0x0001, 0x01ea, 0x01eb, 0x01ec, 0x01ed }; // ds:0x242d
short g_gfx_ani_descriptions_26[37] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x0018, 0x0019, 0x001a, 0x001b, 0x01d1, 0x01d2, 0x01d3, 0x01d4, 0x01e7, 0x016e, 0x016f, 0x0170, 0x0171, 0x01aa, 0x01ab, 0x01ac, 0x01ad, -0x0001, -0x0001, -0x0001, -0x0001, 0x01ee, 0x01ef, 0x01f0, 0x01f1 }; // ds:0x2477
short g_gfx_ani_descriptions_27[37] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0014, 0x0015, 0x0016, 0x0017, 0x0024, 0x0025, 0x0026, 0x0027, 0x01d5, 0x01d6, 0x01d7, 0x01d8, 0x01e8, 0x0172, 0x0173, 0x0174, 0x0175, 0x01ae, 0x01af, 0x01b0, 0x01b1, 0x01f2, 0x01f3, 0x01f4, 0x01f5, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x24c1
short g_gfx_ani_descriptions_28[37] = { 0x00fe, 0x00ff, 0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0122, 0x0123, 0x0124, 0x0125, 0x0142, 0x0143, 0x0144, 0x0145, 0x01d9, 0x01da, 0x01db, 0x01dc, 0x01e9, 0x0176, 0x0177, 0x0178, 0x0179, 0x01b2, 0x01b3, 0x01b4, 0x01b5, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -0x0001, -1 }; // ds:0x250b
Bit16s *g_gfx_ani_index[41] = {
	NULL,
	g_gfx_ani_descriptions_01,
	g_gfx_ani_descriptions_02,
	g_gfx_ani_descriptions_03,
	g_gfx_ani_descriptions_04,
	g_gfx_ani_descriptions_05,
	g_gfx_ani_descriptions_06,
	g_gfx_ani_descriptions_07,
	g_gfx_ani_descriptions_08,
	g_gfx_ani_descriptions_09,
	g_gfx_ani_descriptions_10,
	NULL,
	g_gfx_ani_descriptions_01,
	g_gfx_ani_descriptions_11,
	g_gfx_ani_descriptions_11,
	g_gfx_ani_descriptions_04,
	g_gfx_ani_descriptions_11,
	g_gfx_ani_descriptions_06,
	g_gfx_ani_descriptions_07,
	g_gfx_ani_descriptions_12,
	g_gfx_ani_descriptions_09,
	g_gfx_ani_descriptions_13,
	NULL,
	g_gfx_ani_descriptions_14,
	g_gfx_ani_descriptions_15,
	g_gfx_ani_descriptions_15,
	g_gfx_ani_descriptions_16,
	g_gfx_ani_descriptions_17,
	g_gfx_ani_descriptions_18,
	g_gfx_ani_descriptions_19,
	g_gfx_ani_descriptions_20,
	g_gfx_ani_descriptions_21,
	g_gfx_ani_descriptions_22,
	g_gfx_ani_descriptions_23,
	g_gfx_ani_descriptions_24,
	g_gfx_ani_descriptions_25,
	g_gfx_ani_descriptions_26,
	g_gfx_ani_descriptions_27,
	g_gfx_ani_descriptions_28,
	g_gfx_ani_descriptions_17,
	g_gfx_ani_descriptions_28,
}; // ds:0x2555; Bit8u*
signed char g_two_fielded_sprite_id[5] = { 0x1b, 0x26, 0x28, 0x27, -1 }; // ds:0x25f9
signed short g_weaponani_table[72] = {
	0x0000, 0x0003, 0x0006, 0x0009, 0x000c, 0x000f, 0x0012, 0x0015,
	0x0001, 0x0004, 0x0007, 0x000a, 0x000d, 0x0010, 0x0013, 0x0016,
	0x0002, 0x0005, 0x0008, 0x000b, 0x000e, 0x0011, 0x0014, 0x0017,

	0x0018, 0x001b, 0x001e, 0x0021, 0x0024, 0x0027, 0x002a, 0x002d,
	0x0019, 0x001c, 0x001f, 0x0022, 0x0025, 0x0028, 0x002b, 0x002e,
	0x001a, 0x001d, 0x0020, 0x0023, 0x0026, 0x0029, 0x002c, 0x002f,

	0x0030, 0x0033, 0x0036, 0x0039, 0x000c, 0x003c, 0x0012, 0x003f,
	0x0031, 0x0034, 0x0037, 0x003a, 0x000d, 0x003d, 0x0013, 0x0040,
	0x0032, 0x0035, 0x0038, 0x003b, 0x000e, 0x003e, 0x0014, 0x0041
}; // ds:0x25fe
signed char g_weaponani_types[22] = { 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0 }; // ds:0x268e
signed char g_food_message_shown[7] = { 0, 0, 0, 0, 0, 0, 0 }; // ds:0x26a4
unsigned char g_ems_enabled = 0; // ds:0x26ab
signed char g_fig_initiative = 0; // ds:0x26ac, 0 = random, 1 = enemies, 2 = heroes (attack first)
signed short g_fig_msg_counter = 0; // ds:0x26ad
unsigned short g_always_one = 0; // ds:0x26af, writeonly (1)
signed short g_fig_continue_print = 0; // ds:0x26b1
signed short g_fig_char_pic = 0; // ds:0x26b3
signed short g_fig_enemy_pic = 0; // ds:0x26b5
signed short g_gui_entering_savegame = 0; // ds:0x26b7
signed short g_check_disease = 0; // ds:0x26b9
signed short g_check_poison = 0; // ds:0x26bb
signed short g_text_file_index = -1; // ds:0x26bd
signed short g_tx_file_index = -1; // ds:0x26bf, index of file stored with load_tx
signed short g_fig_discard = 0; // ds:0x26c1, whether to discard the fight data after the fight
Bit8u g_palette_allblack2[3 * 32] = {0}; // ds:0x26c3

Bit8u g_palette_unknown1[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x29 },
	{ 0x00, 0x00, 0x0e },
	{ 0x26, 0x1a, 0x12 },
	{ 0x26, 0x16, 0x0e },
	{ 0x26, 0x12, 0x06 },
	{ 0x22, 0x0e, 0x02 },
	{ 0x1e, 0x0a, 0x00 },
	{ 0x1a, 0x06, 0x00 },
	{ 0x12, 0x02, 0x00 },
	{ 0x0e, 0x00, 0x00 },
	{ 0x06, 0x00, 0x00 },
	{ 0x02, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x22, 0x22, 0x22 },
	{ 0x1a, 0x1a, 0x1a },
	{ 0x16, 0x16, 0x16 },
	{ 0x12, 0x12, 0x12 },
	{ 0x0a, 0x0a, 0x0a },
	{ 0x06, 0x06, 0x06 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x20, 0x00 },
	{ 0x00, 0x0e, 0x00 },
	{ 0x26, 0x22, 0x00 },
	{ 0x1e, 0x16, 0x00 },
	{ 0x12, 0x0a, 0x00 },
	{ 0x26, 0x02, 0x00 },
	{ 0x1a, 0x00, 0x00 },
	{ 0x0e, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x26, 0x26, 0x26 }
}; // ds:0x2723
Bit8u g_palette_fight1[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x3c, 0x2c, 0x24 },
	{ 0x3c, 0x28, 0x1c },
	{ 0x34, 0x20, 0x18 },
	{ 0x28, 0x18, 0x14 },
	{ 0x20, 0x14, 0x10 },
	{ 0x18, 0x0c, 0x08 },
	{ 0x10, 0x08, 0x08 },
	{ 0x0c, 0x04, 0x04 },
	{ 0x34, 0x34, 0x34 },
	{ 0x28, 0x28, 0x28 },
	{ 0x20, 0x20, 0x20 },
	{ 0x18, 0x18, 0x18 },
	{ 0x14, 0x14, 0x14 },
	{ 0x10, 0x10, 0x10 },
	{ 0x0c, 0x0c, 0x0c },
	{ 0x08, 0x08, 0x08 },
	{ 0x14, 0x0c, 0x08 },
	{ 0x00, 0x00, 0x00 },
	{ 0x18, 0x14, 0x3c },
	{ 0x14, 0x00, 0x00 },
	{ 0x1c, 0x00, 0x00 },
	{ 0x24, 0x00, 0x00 },
	{ 0x2c, 0x00, 0x00 },
	{ 0x3c, 0x00, 0x00 },
	{ 0x1c, 0x10, 0x10 },
	{ 0x00, 0x1c, 0x00 },
	{ 0x00, 0x28, 0x00 },
	{ 0x00, 0x04, 0x20 },
	{ 0x2c, 0x2c, 0x0c },
	{ 0x38, 0x38, 0x10 },
	{ 0x3c, 0x3c, 0x3c }
}; // ds:0x2783
//struct{unsigned char r,g,b;} g_palette_special[32] = {
Bit8u g_palette_special[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x38, 0x38, 0x38 },
	{ 0x34, 0x34, 0x34 },
	{ 0x30, 0x30, 0x30 },
	{ 0x2c, 0x2c, 0x2c },
	{ 0x28, 0x28, 0x28 },
	{ 0x24, 0x24, 0x24 },
	{ 0x20, 0x20, 0x20 },
	{ 0x1c, 0x1c, 0x1c },
	{ 0x18, 0x18, 0x18 },
	{ 0x14, 0x14, 0x14 },
	{ 0x10, 0x10, 0x10 },
	{ 0x0c, 0x0c, 0x0c },
	{ 0x08, 0x08, 0x08 },
	{ 0x04, 0x04, 0x04 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x3c, 0x08 },
	{ 0x3c, 0x30, 0x28 },
	{ 0x3c, 0x2c, 0x28 },
	{ 0x2c, 0x1c, 0x18 },
	{ 0x24, 0x14, 0x10 },
	{ 0x1c, 0x10, 0x0c },
	{ 0x14, 0x08, 0x08 },
	{ 0x0c, 0x04, 0x04 },
	{ 0x0c, 0x08, 0x00 },
	{ 0x2c, 0x00, 0x00 },
	{ 0x10, 0x14, 0x3c },
	{ 0x00, 0x08, 0x38 },
	{ 0x00, 0x08, 0x34 },
	{ 0x00, 0x00, 0x18 },
	{ 0x3c, 0x3c, 0x00 },
	{ 0x3c, 0x3c, 0x3c }
}; // ds:0x27e3
unsigned char g_unkn_006[2] = { 0x32, 0x00 }; // ds:0x2843
signed char g_pp20_index = -1; // ds:0x2845, archive file index of current pp20
signed short g_request_refresh = 0; // ds:0x2846
struct mouse_cursor g_default_mouse_cursor = {
	{	0x7fff, 0x9fff, 0x87ff, 0xc1ff,
		0xc07f, 0xe01f, 0xe007, 0xf00f,
		0xf01f, 0xf80f, 0xf887, 0xfdc3,
		0xffe3, 0xfff7, 0xffff, 0xffff },

	{	0x8000, 0x6000, 0x7800, 0x3e00,
		0x3f80, 0x1fe0, 0x1ff8, 0x0ff0,
		0x0fe0, 0x07f0, 0x0778, 0x023c,
		0x001c, 0x0008, 0x0000, 0x0000 }
}; // ds:0x2848
struct mouse_cursor g_cursor_arrow_up = {
	{	0xffff, 0xffff, 0xff7f, 0xfe3f,
		0xfc1f, 0xf80f, 0xf007, 0xe003,
		0xfc1f, 0xfc1f, 0xfc1f, 0xfc1f,
		0xfc1f, 0xfc1f, 0xffff, 0xffff },

	{	0x0000, 0x0000, 0x0080, 0x01c0,
		0x03e0, 0x07f0, 0x0ff8, 0x1ffc,
		0x03e0, 0x03e0, 0x03e0, 0x03e0,
		0x03e0, 0x03e0, 0x0000, 0x0000 }
}; // ds:0x2888
struct mouse_cursor g_cursor_arrow_down = {
	{	0xffff, 0xffff, 0xf83f, 0xf83f,
		0xf83f, 0xf83f, 0xf83f, 0xf83f,
		0xc007, 0xe00f, 0xf01f, 0xf83f,
		0xfc7f, 0xfeff, 0xffff, 0xffff },

	{	0x0000, 0x0000, 0x07c0, 0x07c0,
		0x07c0, 0x07c0, 0x07c0, 0x07c0,
		0x3ff8, 0x1ff0, 0x0fe0, 0x07c0,
		0x0380, 0x0100, 0x0000, 0x0000
} }; // ds:0x28c8
struct mouse_cursor g_cursor_arrow_left = {
	{	0xffff, 0xffff, 0xfdff, 0xf9ff,
		0xf1ff, 0xe01f, 0xc007, 0xe003,
		0xf183, 0xf9c1, 0xfdc1, 0xffc1,
		0xffc1, 0xffc1, 0xffff, 0xffff },

	{	0x0000, 0x0000, 0x0200, 0x0600,
		0x0e00, 0x1fe0, 0x3ff8, 0x1ffc,
		0x0e7c, 0x063e, 0x023e, 0x003e,
		0x003e, 0x003e, 0x0000, 0x0000
} }; // ds:0x2908
struct mouse_cursor g_cursor_arrow_right = {
	{	0xffff, 0xffff, 0xffbf, 0xff9f,
		0xff8f, 0xf807, 0xe003, 0xc007,
		0xc18f, 0x839f, 0x83bf, 0x83ff,
		0x83ff, 0x83ff, 0xffff, 0xffff },

	{	0x0000, 0x0000, 0x0040, 0x0060,
		0x0070, 0x07f8, 0x1ffc, 0x3ff8,
		0x3e70, 0x7c60, 0x7c40, 0x7c00,
		0x7c00, 0x7c00, 0x0000, 0x0000 }
}; // ds:0x2948
signed short g_mouse_posy_min = 0; // ds:0x2988
signed short g_mouse_posx_min = 0; // ds:0x298a
signed short g_mouse_posy_max = 0x00c7; // ds:0x298c
signed short g_mouse_posx_max = 0x0136; // ds:0x298e
struct struct_rect g_pic_copy_rect = { 0, 0, 200 - 1, 320 - 1 }; // ds:0x2990, struct struct_rect
signed short g_mouse_locked = 0; // ds:0x2998
signed short g_mouse_refresh_flag = -1; // ds:0x299a
signed short g_mouse_posx = 0x00a0; // ds:0x299c
signed short g_mouse_posy = 0x0064; // ds:0x299e
signed short g_mouse_posx_bak = 0x00a0; // ds:0x29a0
signed short g_mouse_posy_bak = 0x0064; // ds:0x29a2
signed short g_mouse_moved = 0; // ds:0x29a4
signed short g_mouse_pointer_offsetx = 0; // ds:0x29a6
signed short g_mouse_pointer_offsety = 0; // ds:0x29a8
signed short g_mouse_pointer_offsetx_bak = 0; // ds:0x29aa
signed short g_mouse_pointer_offsety_bak = 0; // ds:0x29ac
signed short g_ani_enabled = 0; // ds:0x29ae
unsigned char g_unkn_007[2] = { 0xff, 0xff }; // ds:0x29b0
signed short g_statusmenu_allowed = 1; // ds:0x29b2, , readonly
signed short g_heroswap_allowed = 1; // ds:0x29b4
signed short g_intemple = 1; // ds:0x29b6, writeonly (0,1), 0 while in temple
signed short g_intemple2 = 1; // ds:0x29b8, , copy of INTEMPLE
signed short g_always_zero3 = 0; // ds:0x29ba, writeonly (0)
unsigned char g_unkn_008[16] = { 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00 }; // ds:0x29bc
struct mouse_action g_action_table_menu[2] = {
	{ 0x0000, 0x0000, 0x013f, 0x00c7, 0x001c },
	{ -0x0001, -0x0001, -0x0001, -0x0001, 0xffff }
}; // ds:0x29cc
struct mouse_action *g_action_table_primary = &g_action_table_menu[0]; // ds:0x29e0, to ACTION_TABLE_MENU; Bit8u*
struct mouse_action *g_action_table_secondary = 0; // ds:0x29e4; Bit8u*
struct mouse_action g_action_table_playmask[24] = {
	{ 0x00f0, 0x0000, 0x0140, 0x0014, 0x00fd },
	{ 0x00f0, 0x0015, 0x0140, 0x0038, 0x00fc },
	{ 0x0010, 0x0024, 0x0043, 0x0060, 0x004b },
	{ 0x00ac, 0x0024, 0x00df, 0x0060, 0x004d },
	{ 0x0044, 0x0004, 0x00ab, 0x0033, 0x0048 },
	{ 0x0044, 0x0059, 0x00ab, 0x0088, 0x0050 },
	{ 0x00f1, 0x0039, 0x0108, 0x0050, 0x0081 },
	{ 0x010c, 0x0039, 0x0123, 0x0050, 0x0082 },
	{ 0x0127, 0x0039, 0x013e, 0x0050, 0x0083 },
	{ 0x00f1, 0x0054, 0x0108, 0x006b, 0x0084 },
	{ 0x010c, 0x0054, 0x0123, 0x006b, 0x0085 },
	{ 0x0127, 0x0054, 0x013e, 0x006b, 0x0086 },
	{ 0x00f1, 0x006f, 0x0108, 0x0086, 0x0087 },
	{ 0x010c, 0x006f, 0x0123, 0x0086, 0x0088 },
	{ 0x0127, 0x006f, 0x013e, 0x0086, 0x0089 },
	{ 0x0003, 0x009d, 0x002d, 0x00c5, 0x00f1 },
	{ 0x002e, 0x009d, 0x0058, 0x00c5, 0x00f2 },
	{ 0x0059, 0x009d, 0x0083, 0x00c5, 0x00f3 },
	{ 0x0084, 0x009d, 0x00ae, 0x00c5, 0x00f4 },
	{ 0x00af, 0x009d, 0x00d9, 0x00c5, 0x00f5 },
	{ 0x00da, 0x009d, 0x0104, 0x00c5, 0x00f6 },
	{ 0x0113, 0x009d, 0x013d, 0x00c5, 0x00f7 },
	{ 0x0000, 0x0000, 0x013f, 0x00c7, 0x00fe },
	{ -0x0001, -0x0001, -0x0001, -0x0001, 0xffff }
}; // ds:0x29e8
struct mouse_action g_action_table_status[30] = {
	{ 0x0010, 0x0029, 0x001f, 0x0031, 0x004b },
	{ 0x0020, 0x0029, 0x0030, 0x0031, 0x004d },
	{ 0x0044, 0x0037, 0x0054, 0x0047, 0x0080 },
	{ 0x003a, 0x0050, 0x004a, 0x0060, 0x0081 },
	{ 0x0091, 0x0050, 0x00a1, 0x0060, 0x0082 },
	{ 0x0044, 0x006a, 0x0054, 0x007a, 0x0083 },
	{ 0x0087, 0x006a, 0x0097, 0x007a, 0x0084 },
	{ 0x0044, 0x008c, 0x0054, 0x009c, 0x0085 },
	{ 0x0087, 0x00ae, 0x0097, 0x00be, 0x0086 },
	{ 0x000f, 0x0037, 0x001f, 0x0047, 0x0087 },
	{ 0x0020, 0x0037, 0x0030, 0x0047, 0x0088 },
	{ 0x000f, 0x0048, 0x001f, 0x0058, 0x0089 },
	{ 0x0020, 0x0048, 0x0030, 0x0058, 0x008a },
	{ 0x000f, 0x0059, 0x001f, 0x0069, 0x008b },
	{ 0x0020, 0x0059, 0x0030, 0x0069, 0x008c },
	{ 0x000f, 0x006a, 0x001f, 0x007a, 0x008d },
	{ 0x0020, 0x006a, 0x0030, 0x007a, 0x008e },
	{ 0x000f, 0x007b, 0x001f, 0x008b, 0x008f },
	{ 0x0020, 0x007b, 0x0030, 0x008b, 0x0090 },
	{ 0x000f, 0x008c, 0x001f, 0x009c, 0x0091 },
	{ 0x0020, 0x008c, 0x0030, 0x009c, 0x0092 },
	{ 0x000f, 0x009d, 0x001f, 0x00ad, 0x0093 },
	{ 0x0020, 0x009d, 0x0030, 0x00ad, 0x0094 },
	{ 0x000f, 0x00ae, 0x001f, 0x00bd, 0x0095 },
	{ 0x0020, 0x00ae, 0x0030, 0x00bd, 0x0096 },
	{ 0x0080, 0x0037, 0x0090, 0x0047, 0x0097 },
	{ 0x0091, 0x0037, 0x00a1, 0x0047, 0x0098 },
	{ 0x009b, 0x001e, 0x00b9, 0x0024, 0x00f0 },
	{ 0x0000, 0x0000, 0x013f, 0x00c7, 0x00fe },
	{ -0x0001, -0x0001, -0x0001, -0x0001, 0xffff }
}; // ds:0x2ad8
struct mouse_action g_action_table_merchant[2] = {
	{ 0x0010, 0x0004, 0x00df, 0x008a, 0x0090 },
	{ -0x0001, -0x0001, -0x0001, -0x0001, 0xffff }
}; // ds:0x2c04
Bit16s g_char_status_bars[7][4] = {
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 },
	{ 0x0000, 0x0000, 0x0000, 0x0000 }
}; // ds:0x2c18
signed short g_disease_prices[8] = { 0x0000, 0x0006, 0x0004, 0x0009, 0x0008, 0x0008, 0x0007, 0x000a }; // ds:0x2c50
signed short g_disease_delays[8] = { 0x0000, -0x000a, 0x0014, -0x0014, -0x001e, -0x0014, 0x0000, -0x001e }; // ds:0x2c60
signed short g_poison_prices[10] = { 0x0000, 0x0006, 0x0005, 0x0004, 0x0005, 0x0005, 0x0004, 0x000a, 0x000c, 0x000f }; // ds:0x2c70
signed short g_poison_delays[10] = { 0x0000, -0x001e, -0x0019, -0x0014, -0x0019, -0x0019, -0x0014, -0x0032, -0x003c, -0x004b }; // ds:0x2c84
signed char  g_dialogbox_lock = 0; // ds:0x2c98
signed short g_timers_disabled = 0; // ds:0x2c99
signed short g_status_page_mode = 1; // ds:0x2c9b, one of 1,2,3,4,5
signed short g_status_page_hero = 0; // ds:0x2c9d
signed char g_status_page_hunger = -1; // ds:0x2c9f
signed char g_status_page_thirst = -1; // ds:0x2ca0
signed char g_missing_file_guilock = 0; // ds:0x2ca1, writeonly (0,1)
signed short g_basepos_x = 0; // ds:0x2ca2
signed short g_basepos_y = 0; // ds:0x2ca4
signed char g_dng_area_loaded = 0; // ds:0x2ca6
signed char g_city_area_loaded = 0; // ds:0x2ca7
signed char g_travel_map_loaded = 0; // ds:0x2ca8
unsigned char g_unkn_009[33] = { 0x00, 0x00, 0x0a, 0x46, 0x82, 0xbe, 0xff, 0xe5, 0xea, 0xf4, 0xf5, 0xf6, 0xff, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01 }; // ds:0x2ca9
signed char g_ani_unknown_flag = 1; // ds:0x2cca, writeonly (0,1)
signed short g_area_prepared = -1; // ds:0x2ccb, -1 = unprepared, 0 = dungeon, 1 = town
unsigned char g_unkn_010[1] = { 0x00 }; // ds:0x2ccd
unsigned char g_always_zero4 = 0; // ds:0x2cce, writeonly (0)
signed short g_current_ani = -1; // ds:0x2ccf
signed short g_fig_figure1 = -1; // ds:0x2cd1
signed short g_fig_figure2 = -1; // ds:0x2cd3
signed short g_in_fight = 0; // ds:0x2cd5
signed short g_fight_round = 0; // ds:0x2cd7
unsigned char g_unkn_011[2] = { 0, 0 }; // ds:0x2cd9
signed short g_skilled_hero_pos = -1; // ds:0x2cdb, {-1, 0..6}
const struct struct_point g_gui_buttons_pos[9] = {
	{ 0x00f1, 0x0039 },
	{ 0x010c, 0x0039 },
	{ 0x0127, 0x0039 },
	{ 0x00f1, 0x0054 },
	{ 0x010c, 0x0054 },
	{ 0x0127, 0x0054 },
	{ 0x00f1, 0x006f },
	{ 0x010c, 0x006f },
	{ 0x0127, 0x006f }
}; // ds:0x2cdd
const signed int g_hero_pic_posx[7] = { 3, 46, 89, 132, 175, 218, 275 }; // ds:0x2d01
signed char g_levelup_ta_rise[12] = { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x0f, 0x14, 0x14, 0x14 }; // ds:0x2d0f
signed char g_levelup_sp_rise[6] = { 0x19, 0x19, 0x28, 0x14, 0x14, 0x14 }; // ds:0x2d1b
signed char g_levelup_spta_conv[6] = { 5, 5, 5, 0, 0, 0 }; // ds:0x2d21
signed char g_mr_modificators[13] = { 0, 2, 0, 0, 2, -2, 2, 2, 2, 2, 3, 4, 3 }; // ds:0x2d27


Bit8s gs_datseg_status_start = 0; // ds:0x2d34, 99 = game finished, area of the datseg that is stored one to one in savegame files
Bit8s  gs_current_group = 0; // ds:0x2d35
Bit8s  gs_group_member_counts[6] = {0}; // ds:0x2d36, members per group
Bit8s  gs_total_hero_counter = 0; // ds:0x2d3c
Bit8s  gs_direction = 0; // ds:0x2d3d
Bit8s  gs_groups_direction[6] = {0}; // ds:0x2d3e
Bit16s gs_x_target = 0; // ds:0x2d44
Bit16s gs_y_target = 0; // ds:0x2d46
Bit16s gs_groups_x_target[6] = {0}; // ds:0x2d48
Bit16s gs_groups_y_target[6] = {0}; // ds:0x2d54
Bit8s  gs_current_loctype = 0; // ds:0x2d60
Bit8s  gs_groups_current_loctype[6] = {0}; // ds:0x2d61
Bit8s  gs_current_town = 0; // ds:0x2d67
Bit8s  gs_groups_town[6] = {0}; // ds:0x2d68
Bit8s  gs_dungeon_index = 0; // ds:0x2d6e
Bit8s  gs_groups_dng_index[6] = {0}; // ds:0x2d6f
Bit8s  gs_dungeon_level = 0; // ds:0x2d75
Bit8s  gs_groups_dng_level[6] = {0}; // ds:0x2d76
Bit8s  gs_direction_bak = 0;	// ds:0x2d7c
Bit8s  gs_groups_direction_bak[6] = {0}; // ds:0x2d7d
Bit16s gs_x_target_bak = 0; // ds:0x2d83
Bit16s gs_y_target_bak = 0; // ds:0x2d85
Bit16s gs_groups_x_target_bak[6] = {0}; // ds:0x2d87
Bit16s gs_groups_y_target_bak[6] = {0}; // ds:0x2d93
Bit8s  gs_current_loctype_bak = 0; // ds:0x2d9f
Bit8s  gs_groups_current_loctype_bak[6] = {0}; // ds:0x2da0
Bit8s  gs_current_town_bak = 0; // ds:0x2da6
Bit8s  gs_groups_town_bak[6] = {0}; // ds:0x2da7
Bit8s  gs_dungeon_index_bak = 0; // ds:0x2dad
Bit8s  gs_groups_dng_index_bak[6] = {0}; // ds:0x2dae
Bit8s  gs_dungeon_level_bak = 0; // ds:0x2db4
Bit8s  gs_groups_dng_level_bak[6] = {0}; // ds:0x2db5
Bit32s gs_day_timer = 0; // ds:0x2dbb
Bit8s  gs_day_of_week = 0; // ds:0x2dbf
Bit8s  gs_day_of_month = 0; // ds:0x2dc0
Bit8s  gs_month = 0; // ds:0x2dc1
Bit8s  gs_year = 0; // ds:0x2dc2
Bit8s  gs_special_day = 0; // ds:0x2dc3
Bit32s gs_ingame_timers[26] = {0}; // ds:0x2dc4
struct struct_modification_timer gs_modification_timers[100] = { {0} }; // ds:0x2e2c
unsigned char g_unkn_012[2] = { 0, 0 }; // ds:0x314c
Bit32s gs_gods_estimation[15] = {0};	// ds:0x314e
Bit8s  gs_tav_cheated_flags[88] = {0};	// ds:0x318a
struct struct_smith_repairitems gs_smith_repairitems[50] = {0}; // ds:0x31e2
Bit16s gs_dng_handled_pos = 0; // ds:0x330e
Bit16s gs_deathtrap_steps = 0; // ds:0x3310 // for activated deathtrap: number of remaining steps in dungeon
Bit16s gs_deathtrap = 0; // ds:0x3312 // 0: no death trap activated. 1: death trap on Totenschiff activated. 2: death trap in Spinnenhoehle activated
Bit8s  gs_dng01_amulet_taken = 0; // ds:0x3314
Bit8s  gs_dng01_sabre_taken = 0; // ds:0x3315
Bit8s  gs_dng01_crossbow_taken = 0; // ds:0x3316
Bit8s  gs_dng01_ardora_freed = 0; // ds:0x3317
unsigned char g_unkn_013[1] = { 0x00 }; // ds:0x3318
Bit8s  gs_dng01_key_taken = 0; // ds:0x3319
Bit8s  gs_dng01_corpse_looted = 0; // ds:0x331a
Bit16u gs_weather1 = 0; // ds:0x331b, random(6)
Bit16u gs_weather2 = 0; // ds:0x331d, random(7)
Bit8u  gs_daspota_fightflags[19] = {0}; // ds:0x331f
Bit8s  gs_treasure_maps[10] = {0}; // ds:0x3332 /* flags for the received parts of Hyggelik's map */
Bit8s  gs_debtbook_read_flag = 1; // ds:0x333c
Bit8s  gs_orcdocument_read_flag = 0; // ds:0x333d
Bit16s gs_gardanium_used_ae = 0; // ds:0x333e	WRITEONLY
Bit16s gs_min_map_parts = 0;	// ds:0x3340
Bit16s gs_subvention = 0;	// ds:0x3342, {0-4}
Bit16s gs_otta_windriders = 0;	// ds:0x3344
Bit16s gs_otta_stormchildren = 0; // ds:0x3346
Bit16s gs_otta_garaldsson = 0;	// ds:0x3348
Bit16s gs_heard_announce = 0;	// ds:0x334a, {0 = not heard, 1 = heard, 3 = spoke with hetman}
Bit16s gs_got_main_quest = 0;	// ds:0x334c
Bit16s gs_quested_months = 0;	// ds:0x334e
Bit16s gs_monthly_credit = 0;	// ds:0x3350
Bit16s gs_got_letter_jad = 0;	// ds:0x3352
Bit16s gs_visited_miller = 0;	// ds:0x3354
Bit16s gs_quest_ugdalf = 0;	// ds:0x3356
Bit16s gs_academy_daily_curse = 0; // ds:0x3358
Bit16s gs_academy_daily_ident = 0; // ds:0x335a
Bit16s gs_bank_deposit = 0;	 // ds:0x335c
Bit16s gs_days_to_cens = 0;	 // ds:0x335e
Bit16s gs_debt_days = 0;	 // ds:0x3360
Bit16s gs_in_academy = 0;	 // ds:0x3362
Bit8u  gs_informer_flags[16] = {0}; // ds:0x3364
Bit8u  gs_tav_kicked_flags[88] = {0}; // ds:0x3374
Bit8u  gs_town_outlawed_flags[52] = {0}; // ds:0x33cc WRITEONLY
Bit8u  gs_herberg_kicked_flags[74] = {0}; // ds:0x3400
Bit8u  gs_got_letter_het = 0; // ds:0x344a
unsigned char g_unkn_015[1] = { 0x00 }; // ds:0x344b
Bit8u  gs_jurge_awaits_letter = 0; // ds:0x344c
Bit8u  gs_umbrik_quest_gorah = 0; // ds:0x344d
unsigned char g_unkn_016[1] = { 0x00 }; // ds:0x344e
Bit8u  gs_isleif_jandas_regards = 0; // ds:0x344f
Bit8u  gs_isleif_map_given = 0; // ds:0x3450
Bit8u  gs_ragna_asked_map = 0; // ds:0x3451
Bit8u  gs_beorn_litt_bold = 0; // ds:0x3452
Bit8u  gs_beorn_medi_bold = 0; // ds:0x3453
Bit8u  gs_beorn_very_bold = 0; // ds:0x3454
Bit8u  gs_eliane_quest_nameless = 0; // ds:0x3455
Bit8u  gs_olvir_start_hetmann = 0; // ds:0x3456
Bit8u  gs_olvir_start_hyggelik = 0; // ds:0x3457
Bit8u  gs_olvir_start_hasgar = 0; // ds:0x3458
Bit8u  gs_olvir_interrupted = 0; // ds:0x3459
Bit8u  gs_olvir_singing_hetmann = 0; // ds:0x345a
Bit8u  gs_olvir_singing_hyggelik = 0; // ds:0x345b
Bit8u  gs_olvir_singing_hasgar = 0; // ds:0x345c
Bit8u  gs_swafnild_tp1 = 0; // ds:0x345d
Bit8u  gs_swafnild_tp2 = 0; // ds:0x345e
Bit8u  gs_swafnild_tp3 = 0; // ds:0x345f
Bit8u  gs_swafnild_tp4 = 0; // ds:0x3460
Bit8u  gs_swafnild_destination = 0; // ds:0x3461
Bit8u  gs_swafnild_nomap = 0; // ds:0x3462
Bit8u  gs_treborn_date = 0; // ds:0x3463
Bit8u  gs_unicorn_hero_pos = 0; // ds:0x3464, {0-6}
Bit8u  gs_unicorn_get_map = 0; // ds:0x3465
Bit8u  gs_unicorn_timer = 0; // ds:0x3466
Bit8u  gs_algrid_treborns_hint = 0; // ds:0x3467
Bit8u  gs_tiomar_drinkmate = 0; // ds:0x3468, {0-6}
Bit8u  gs_tiomar_awaits_letter = 0; // ds:0x3469
/* REMARK: pointer in gamestate are no good */
Bit8u *gs_unicorn_hero_ptr = NULL; // ds:0x346a; Bit8u*
Bit16s gs_npc_meet_tavern = 0;	// ds:0x346e
Bit16s gs_npc_months = 0;	// ds:0x3470
Bit8u  gs_smith_kicked_flags[50] = {0};		// ds:0x3472
Bit8u  gs_smith_flogged_flags[50] = {0};	// ds:0x34a4
Bit8u  gs_merchant_kicked_flags[94] = {0};	// ds:0x34d6
Bit8u  gs_merchant_kicked2_flags[94] = {0};	// ds:0x3534
Bit8u  gs_merchant_offended_flags[94] = {0};	// ds:0x3592
Bit8u  gs_quest_deadship = 0; // ds:0x35f0
unsigned char g_unkn_017[1] = { 0x00 }; // ds:0x35f1
Bit8u  gs_quest_deadship_done = 0; // ds:0x35f2
Bit8s  gs_always_one2 = 0; // ds:0x35f3, writeonly (1)
Bit8u  gs_dng03_spidereggs_burned = 0; // ds:0x35f4
unsigned char g_unkn_018[2] = { 0, 0 }; // ds:0x35f5
Bit8u  gs_dng06_dasptreas_found = 0; // ds:0x35f7
unsigned char g_unkn_019[2] = { 0, 0 }; // ds:0x35f8
Bit8u  gs_quest_nameless_done = 0; // ds:0x35fa
unsigned char g_unkn_020[4] = { 0x00, 0x00, 0x00, 0x00 }; // ds:0x35fb
Bit8u  gs_dng14_ugdalf_done = 0; // ds:0x35ff
Bit8u  gs_got_grimring = 0; // ds:0x3600
Bit8s  gs_npc_timers[7] = { 0, 0, 0, 0, 0, 0, 0 }; // ds:0x3601
Bit8u  gs_tlk_tav_firstinfo = 0; // ds:0x3608
Bit8u  gs_tlk_tav_infopromise = 0; // ds:0x3609
Bit8u  gs_tlk_tav_round = 0; // ds:0x360a
Bit8u  gs_tlk_tav_freebeer = 0; // ds:0x360b
Bit8u  gs_tlk_tav_drinkcount = 0; // ds:0x360c
Bit8u  gs_tlk_tav_informersex = 0; // ds:0x360d, {1 = male,2 = female}
unsigned char g_unkn_021[1] = { 0x00 }; // ds:0x360e
Bit8u  gs_tlk_tav_followinformer = 0; // ds:0x360f
Bit8u  gs_tlk_tav_fullness = 0; // ds:0x3610
Bit8u  gs_tlk_tav_testdrunk = 0; // ds:0x3611
Bit8u  gs_current_informer = 0; // ds:0x3612
Bit8u  gs_dungeon_light = 0; // ds:0x3613, 0 = light is on, 1, 2 = light is off
Bit8u  gs_show_travel_map = 0; // ds:0x3614
Bit8u  gs_hermit_visited = 0; // ds:0x3615
Bit8u  gs_dungeon_gfx_style = 0; // ds:0x3616, { 0 = SHIPS, 1 = MARBLES, 2 = STONES }
Bit8u  gs_estorik_known = 0; // ds:0x3617
struct struct_dialog_partner gs_dialog_partners[10] = { {0} }; // ds:0x3618
struct struct_dialog_state gs_dialog_states[160] = { {0} }; // ds:0x3794
Bit8u  gs_dng02_sphere_known = 0; // ds:0x3c94
Bit8u  gs_dng02_corpse_flag = 0; // ds:0x3c95
Bit8u  gs_dng02_apparature_destroyed = 0; // ds:0x3c96
Bit8u  gs_dng02_sphere_timer = 0; // ds:0x3c97, {0,...,7}
Bit8u  gs_dng02_sphere_active = 0; // ds:0x3c98
Bit8u  gs_dng02_raven_destroyed = 0; // ds:0x3c99
Bit8u  gs_dng02_secret_door1 = 0; // ds:0x3c9a
Bit8u  gs_dng02_secret_door2 = 0; // ds:0x3c9b
Bit8u  gs_dng02_secret_door3 = 0; // ds:0x3c9c
Bit8u  gs_dng03_lever_trap = 1; // ds:0x3c9d
Bit8u  gs_dng03_lever_chest11 = 1; // ds:0x3c9e
Bit8u  gs_dng03_sanctum_smashed = 0; // ds:0x3c9f
Bit8u  gs_dng04_corpse0_flag = 0; // ds:0x3ca0
Bit8u  gs_dng04_corpse1_flag = 0; // ds:0x3ca1
Bit8u  gs_dng04_corpse2_flag = 0; // ds:0x3ca2
Bit8u  gs_dng04_gap_flag = 0; // ds:0x3ca3
Bit8u  gs_dng04_helmet_flag = 0; // ds:0x3ca4
Bit8u  gs_dng04_lantern_flag = 0; // ds:0x3ca5
Bit8u  gs_dng04_deepgap_flag = 0; // ds:0x3ca6
Bit8u  gs_dng06_pitdoor_flag = 0; // ds:0x3ca7
Bit8u  gs_dng06_proviant_flag = 0; // ds:0x3ca8
Bit8u  gs_dng06_money_flag = 0; // ds:0x3ca9
Bit8u  gs_dng06_goldkey_flag = 0; // ds:0x3caa
Bit8s  gs_dng06_countdown_timer = 5; // ds:0x3cab
Bit8u  gs_dng06_lever_flag = 0; // ds:0x3cac
Bit8u  gs_dng06_book_flag = 0; // ds:0x3cad
Bit8u  gs_dng06_fight19_flag = 0; // ds:0x3cae
Bit8u  gs_dng05_proviant_flag = 0; // ds:0x3caf
Bit8u  gs_dng05_bats_flag = 0; // ds:0x3cb0
Bit8u  gs_dng05_god_flag = 0; // ds:0x3cb1
Bit8u  gs_dng07_muelixier_flag = 0; // ds:0x3cb2
Bit8u  gs_dng07_antimuelixier_flag = 0; // ds:0x3cb3
Bit8u  gs_dng07_flicker_flag = 0; // ds:0x3cb4
Bit8u  gs_dng07_poison_flag = 0; // ds:0x3cb5
Bit32s gs_dng07_poison_timer = 0; // ds:0x3cb6
Bit8u  gs_dng08_bed_00 = 0; // ds:0x3cba
Bit8u  gs_dng08_bed_01 = 0; // ds:0x3cbb
Bit8u  gs_dng08_bed_02 = 0; // ds:0x3cbc
Bit8u  gs_dng08_bed_03 = 0; // ds:0x3cbd
Bit8u  gs_dng08_bed_04 = 0; // ds:0x3cbe
Bit8u  gs_dng08_bed_05 = 0; // ds:0x3cbf
Bit8u  gs_dng08_bed_06 = 0; // ds:0x3cc0
Bit8u  gs_dng08_bed_07 = 0; // ds:0x3cc1
Bit8u  gs_dng08_bed_08 = 0; // ds:0x3cc2
Bit8u  gs_dng08_bed_09 = 0; // ds:0x3cc3
Bit8u  gs_dng08_bed_10 = 0; // ds:0x3cc4
Bit8u  gs_dng08_bed_11 = 0; // ds:0x3cc5
Bit8u  gs_dng08_timer1 = 0; // ds:0x3cc6, {0, 1, ..., 24}
Bit8u  gs_dng08_timer2 = 0; // ds:0x3cc7, {0, 1, ..., 24}
Bit8u  gs_dng08_bundles_done = 0; // ds:0x3cc8
Bit8u  gs_dng08_ballista_done = 0; // ds:0x3cc9
Bit8u  gs_dng08_chest2_looted = 0; // ds:0x3cca
Bit8u  gs_dng08_secret_door = 0; // ds:0x3ccb, {0,1 = found,2 = opened}
Bit8u  gs_dng08_chest0_looted = 0; // ds:0x3ccc
Bit8u  gs_dng08_chest35_looted = 0; // ds:0x3ccd
Bit8u  gs_daspota_location01_loot[29] = { 0x36, 0x01, 0x0c, 0x01, 0x0d, 0x1e, 0x9e, 0x01, 0x9f, 0x01, 0x5c, 0x0a, 0x17, 0x32, 0x5b, 0x14, 0x4a, 0x01, 0x4a, 0x01, 0x22, 0x01, 0x92, 0x01, 0x8e, 0x01, 0xfa, 0xff, 0xff }; // ds:0x3cce
Bit8u  gs_daspota_location02_loot[15] = { 0x0c, 0x01, 0x0c, 0x01, 0x0d, 0x1e, 0x07, 0x01, 0x07, 0x01, 0x19, 0x01, 0x29, 0x01, 0xff }; // ds:0x3ceb
Bit8u  gs_daspota_location03_loot[15] = { 0x0c, 0x01, 0x0c, 0x01, 0x0d, 0x1e, 0x07, 0x01, 0x07, 0x01, 0x19, 0x01, 0x29, 0x01, 0xff }; // ds:0x3cfa
Bit8u  gs_daspota_location04_loot[9] = { 0x08, 0x01, 0x17, 0x0a, 0x56, 0x01, 0x4d, 0x01, 0xff }; // ds:0x3d09
Bit8u  gs_daspota_location05_loot[31] = { 0xa0, 0x01, 0xa1, 0x01, 0x15, 0x01, 0x6d, 0x01, 0x5b, 0x14, 0x4c, 0x02, 0x24, 0x01, 0x1d, 0x01, 0x22, 0x01, 0x2a, 0x01, 0x26, 0x01, 0x23, 0x01, 0x39, 0x01, 0x3b, 0x01, 0x8f, 0x01, 0xff }; // ds:0x3d12
Bit8u  gs_daspota_location06_loot[25] = { 0x03, 0x03, 0x01, 0x03, 0x07, 0x03, 0x08, 0x03, 0x10, 0x03, 0x13, 0x03, 0x36, 0x03, 0x4e, 0x03, 0x54, 0x03, 0x75, 0x03, 0x52, 0x03, 0x51, 0x03, 0xff }; // ds:0x3d31
Bit8u  gs_daspota_location07_loot[7] = { 0x17, 0x14, 0x5b, 0x14, 0x5c, 0x0a, 0xff }; // ds:0x3d4a
Bit8u  gs_daspota_location08_loot[15] = { 0x4c, 0x05, 0x4a, 0x05, 0x22, 0x01, 0x22, 0x01, 0x1f, 0x01, 0x2e, 0x01, 0x22, 0x01, 0xff }; // ds:0x3d51
Bit8u  gs_daspota_location10_loot[7] = { 0x17, 0x32, 0x5b, 0x14, 0x5c, 0x0a, 0xff }; // ds:0x3d60
Bit8u  gs_daspota_location12_loot[21] = { 0x1a, 0x03, 0x20, 0x03, 0x28, 0x03, 0x2e, 0x03, 0x48, 0x03, 0x55, 0x03, 0x5f, 0x03, 0x41, 0x03, 0x79, 0x03, 0x1f, 0x03, 0xff }; // ds:0x3d67
Bit8u  gs_daspota_location13_loot[9] = { 0x08, 0x03, 0x79, 0x03, 0x1b, 0x03, 0x10, 0x02, 0xff }; // ds:0x3d7c
Bit8u  gs_daspota_location14_loot[5] = { 0x17, 0x50, 0x5c, 0x0a, 0xff }; // ds:0x3d85
Bit8u  gs_daspota_location16_loot[5] = { 0x17, 0x32, 0x2d, 0x0a, 0xff }; // ds:0x3d8a
Bit8u  gs_daspota_location18_loot[19] = { 0x5c, 0x0a, 0x17, 0x28, 0x4c, 0x05, 0x4a, 0x05, 0x1f, 0x02, 0x5f, 0x01, 0x7e, 0x01, 0x3a, 0x01, 0x80, 0x01, 0xff }; // ds:0x3d8f
Bit8u  gs_tevent004_flag = 0; // ds:0x3da2
Bit8u  gs_tevent005_flag = 0; // ds:0x3da3
Bit8u  gs_tevent008_flag = 0; // ds:0x3da4
Bit8u  gs_tevent009_flag = 0; // ds:0x3da5
Bit8u  gs_tevent011_flag = 0; // ds:0x3da6
Bit8u  gs_tevent013_flag = 0; // ds:0x3da7
Bit8u  gs_tevent014_flag = 0; // ds:0x3da8
Bit8u  gs_tevent016_flag = 0; // ds:0x3da9
Bit8u  gs_tevent017_flag = 0; // ds:0x3daa
Bit8u  gs_tevent020_flag = 0; // ds:0x3dab
Bit8u  gs_tevent021_flag = 0; // ds:0x3dac
Bit8u  gs_tevent022_flag = 0; // ds:0x3dad
Bit8u  gs_tevent022_track_flag = 0; // ds:0x3dae
Bit8u  gs_tevent024_flag = 0; // ds:0x3daf
Bit8u  gs_tevent025_flag = 0; // ds:0x3db0
Bit8u  gs_tevent028_flag = 0; // ds:0x3db1
Bit8u  gs_tevent030_flag = 0; // ds:0x3db2
Bit8u  gs_tevent031_flag = 0; // ds:0x3db3
Bit8u  gs_tevent032_flag = 0; // ds:0x3db4
Bit8u  gs_tevent032_herb_flag = 0; // ds:0x3db5
Bit8u  gs_tevent034_flag = 0; // ds:0x3db6
Bit8u  gs_tevent035_flag = 0; // ds:0x3db7
Bit8u  gs_tevent036_flag = 0; // ds:0x3db8
Bit8u  gs_tevent036_herb_flag = 0; // ds:0x3db9
Bit8u  gs_tevent037_flag = 0; // ds:0x3dba
Bit8u  gs_tevent039_flag = 0; // ds:0x3dbb
Bit8u  gs_tevent040_flag = 0; // ds:0x3dbc
Bit8u  gs_tevent041_flag = 0; // ds:0x3dbd
Bit8u  gs_tevent042_flag = 0; // ds:0x3dbe
Bit8u  gs_tevent043_flag = 0; // ds:0x3dbf
Bit8u  gs_tevent048_flag = 0; // ds:0x3dc0
Bit8u  gs_tevent048_track_flag = 0; // ds:0x3dc1
Bit8u  gs_tevent049_flag = 0; // ds:0x3dc2
Bit8u  gs_tevent050_flag = 0; // ds:0x3dc3
Bit8u  gs_tevent050_herb_flag = 0; // ds:0x3dc4
Bit8u  gs_tevent051_flag = 0; // ds:0x3dc5
Bit8u  gs_tevent053_flag = 0; // ds:0x3dc6
Bit8u  gs_tevent058_flag = 0; // ds:0x3dc7
Bit8u  gs_tevent061_flag = 0; // ds:0x3dc8
Bit8u  gs_tevent064_flag = 0; // ds:0x3dc9
Bit8u  gs_tevent066_flag = 0; // ds:0x3dca
Bit8u  gs_tevent066_track_flag = 0; // ds:0x3dcb
Bit8u  gs_tevent067_flag = 0; // ds:0x3dcc
Bit8u  gs_tevent069_flag = 0; // ds:0x3dcd
Bit8u  gs_tevent070_flag = 0; // ds:0x3dce
Bit8u  gs_tevent070_herb_flag = 0; // ds:0x3dcf
Bit8u  gs_tevent070_trail_flag = 0; // ds:0x3dd0
Bit8u  gs_tevent071_flag = 0; // ds:0x3dd1
Bit8u  gs_teventu01_flag = 0; // ds:0x3dd2
Bit8u  gs_tevent064_silent_flag = 0; // ds:0x3dd3
unsigned char g_unkn_022[1] = { 0x00 }; // ds:0x3dd4
Bit8u  gs_teventu02_flag = 0; // ds:0x3dd5
Bit8u  gs_teventu03_flag = 0; // ds:0x3dd6
Bit8u  gs_teventu04_flag = 0; // ds:0x3dd7
Bit8u  gs_teventu05_flag = 0; // ds:0x3dd8
Bit8u  gs_teventu06_flag = 0; // ds:0x3dd9
Bit8u  gs_tevent071_orcstatue = 0; // ds:0x3dda
Bit8u  gs_met_unicorn_flag = 0; // ds:0x3ddb	1st (tevent087, tevent038), 2nd tevent136
Bit8u  gs_find_hyggelik = 0; // ds:0x3ddc, {0,1 = you are able to find Hyggelik} tevent031, tevent115
Bit8u  gs_tevent072_flag = 0; // ds:0x3ddd
Bit8u  gs_tevent073_flag = 0; // ds:0x3dde
Bit8u  gs_tevent074_flag = 0; // ds:0x3ddf
Bit8u  gs_tevent075_flag = 0; // ds:0x3de0
Bit8u  gs_tevent076_flag = 0; // ds:0x3de1
Bit8u  gs_tevent077_flag = 0; // ds:0x3de2
Bit8u  gs_tevent079_flag = 0; // ds:0x3de3
Bit8u  gs_tevent080_flag = 0; // ds:0x3de4
Bit8u  gs_tevent080_tatzelwurm = 0; // ds:0x3de5
Bit8u  gs_tevent081_flag = 0; // ds:0x3de6
Bit8u  gs_tevent083_flag = 0; // ds:0x3de7
Bit8u  gs_tevent084_flag = 0; // ds:0x3de8
Bit8u  gs_tevent085_flag = 0; // ds:0x3de9
Bit8u  gs_tevent085_herb_flag = 0; // ds:0x3dea
Bit8u  gs_tevent086_flag = 0; // ds:0x3deb
Bit8u  gs_tevent088_flag = 0; // ds:0x3dec
Bit8u  gs_tevent091_flag = 0; // ds:0x3ded
Bit8u  gs_tevent093_flag = 0; // ds:0x3dee
Bit8u  gs_tevent094_flag = 0; // ds:0x3def
Bit8u  gs_teventu07_flag = 0; // ds:0x3df0
Bit8u  gs_teventu08_flag = 0; // ds:0x3df1
Bit8u  gs_tevent099_flag = 0; // ds:0x3df2
Bit8u  gs_tevent100_flag = 0; // ds:0x3df3
Bit8u  gs_tevent101_flag = 0; // ds:0x3df4
Bit8u  gs_tevent106_flag = 0; // ds:0x3df5
Bit8u  gs_tevent108_flag = 0; // ds:0x3df6
Bit8u  gs_tevent109_flag = 0; // ds:0x3df7
Bit8u  gs_tevent111_flag = 0; // ds:0x3df8
Bit8u  gs_tevent112_flag = 0; // ds:0x3df9
Bit8u  gs_tevent112_herb_flag = 0; // ds:0x3dfa
Bit8u  gs_tevent115_flag = 0; // ds:0x3dfb
Bit8u  gs_tevent116_flag = 0; // ds:0x3dfc
Bit8u  gs_tevent118_flag = 0; // ds:0x3dfd
Bit8u  gs_tevent119_flag = 0; // ds:0x3dfe
Bit8u  gs_tevent120_flag = 0; // ds:0x3dff
Bit8u  gs_tevent122_flag = 0; // ds:0x3e00
unsigned char g_unkn_023[1] = { 0x00 }; // ds:0x3e01
Bit8u  gs_tevent128_flag = 0; // ds:0x3e02
Bit8u  gs_tevent128_replen_flag = 0; // ds:0x3e03
Bit8u  gs_tevent129_flag = 0; // ds:0x3e04
Bit8u  gs_tevent131_flag = 0; // ds:0x3e05
Bit8u  gs_tevent132_flag = 0; // ds:0x3e06
Bit8u  gs_tevent132_herb_flag = 0; // ds:0x3e07
Bit8u  gs_tevent134_flag = 0; // ds:0x3e08
Bit8u  gs_tevent137_flag = 0; // ds:0x3e09
Bit8u  gs_tevent138_flag = 0; // ds:0x3e0a
Bit8u  gs_teventu09_flag = 0; // ds:0x3e0b
Bit8u  gs_tevent140_flag = 0; // ds:0x3e0c
Bit8u  gs_tevent140_herb_flag = 0; // ds:0x3e0d
Bit8u  gs_tevent141_flag = 0; // ds:0x3e0e
Bit8u  gs_tevent142_flag = 0; // ds:0x3e0f
unsigned char g_unkn_024[1] = { 0x00 }; // ds:0x3e10
Bit8u  gs_teventu10_flag = 0; // ds:0x3e11
Bit8u  gs_teventu11_flag = 0; // ds:0x3e12
Bit8u  gs_teventu12_flag = 0; // ds:0x3e13
Bit8u  gs_teventu13_flag = 0; // ds:0x3e14
Bit8u  gs_tevent114_olimone_flag = 0; // ds:0x3e15
Bit8u  gs_tevent074_fight_flag = 0xff; // ds:0x3e16
Bit8u  gs_tevent077_fight_flag = 0xff; // ds:0x3e17

#if defined(__BORLANDC__)
struct struct_hero *gs_random_tlk_hero = NULL;	// ds:0x3e18;
struct struct_hero *gs_ruin_hero = NULL;		// ds:0x3e1c;
struct struct_hero *gs_main_acting_hero = NULL;	// ds:0x3e20;
#else
Bit32u gs_random_tlk_hero_obsolete = 0; // ds:0x3e18; This is a dummy now!
Bit32u gs_ruin_hero_obsolete = 0;	// ds:0x3e1c; This is a dummy now!
Bit32u gs_main_acting_hero_obsolete = 0;// ds:0x3e20; This is a dummy now!
#endif

Bit8u gs_tevent073_corpse[3] = { 0xaa, 0x01, 0xff }; // ds:0x3e24
Bit8u gs_tevent014_chest[4] = { 0x01, 0x6e, 0xa2, 0xff }; // ds:0x3e27
Bit8u gs_tevent064_hut_content[19] = { 0x4b, 0x01, 0x5c, 0x01, 0x5b, 0x01, 0x7e, 0x01, 0x78, 0x01, 0x80, 0x01, 0x1d, 0x01, 0x46, 0x01, 0x92, 0x01, 0xff }; // ds:0x3e2b
Bit8u gs_tevent064_chest[21] = { 0xa4, 0x01, 0xa5, 0x01, 0x92, 0x01, 0xb4, 0x02, 0xa6, 0x01, 0xa7, 0x01, 0x7e, 0x02, 0x82, 0x01, 0x9d, 0x01, 0x83, 0x01, 0xff }; // ds:0x3e3e

Bit8u  gs_palette_floor[3 * 32] = { 0 };	// ds:0x3e53
Bit8u  gs_palette_buildings[3 * 32] = { 0 };	// ds:0x3eb3
Bit8u  gs_palette_sky[3 * 32] = { 0 };		// ds:0x3f13
Bit8u  gs_need_letter = 0;	// ds:0x3f73
Bit8u  gs_gremob_invited = 0;	// ds:0x3f74
Bit8u  gs_harlot_date = 0;	// ds:0x3f75
Bit8u  gs_drug_timer = 0;	// ds:0x3f76, {0, 1, ..., 10}
Bit8u  gs_ptemple_closed = 0;	// ds:0x3f77
Bit8u  gs_alrik_derondan = 0;	// ds:0x3f78
Bit8u  gs_alrik_derondan_insulted = 0; // ds:0x3f79
Bit8u  gs_dng13_lantern_flag = 0; // ds:0x3f7a
Bit8s  gs_dng13_corpse0_flag = 0; // ds:0x3f7b
Bit8s  gs_dng13_corpse1_flag = 0; // ds:0x3f7c
Bit8u  gs_dng13_passage1_flag = 0; // ds:0x3f7d
Bit8u  gs_dng13_passage2_flag = 0; // ds:0x3f7e
Bit8u  gs_dng13_passage3_flag = 0; // ds:0x3f7f
Bit8u  gs_dng13_passage4_flag = 0; // ds:0x3f80
Bit8u  gs_dng13_passage5_flag = 0; // ds:0x3f81
Bit8u  gs_dng13_money_flag = 0; // ds:0x3f82
Bit8u  gs_dng13_chest_equips[13] = {
	ITEM_PICKAXE,	 3,
	ITEM_SHOVEL, 	 1,
	ITEM_ROPE,	 3,
	ITEM_HAMMER,	 3,
	ITEM_TORCH_OFF, 15,
	ITEM_POT_HELMET, 1,
	0xff
}; // ds:0x3f83
Bit8u  gs_dng13_chest0_content[6] = {
	ITEM_DAGGER, ITEM_TINDERBOX, ITEM_TORCH_OFF, ITEM_TORCH_OFF, ITEM_ROPE, 0xff }; // ds:0x3f90
Bit8u  gs_dng13_chest1_content[5] = {
	ITEM_DAGGER, ITEM_SWORD, ITEM_TORCH_OFF, ITEM_TINDERBOX, 0xff }; // ds:0x3f96
Bit8s  gs_dng12_tunnel1 = 45; // ds:0x3f9b
Bit8s  gs_dng12_tunnel2 = 40; // ds:0x3f9c
Bit8s  gs_dng12_tunnel3 = 35; // ds:0x3f9d
Bit8s  gs_dng12_tunnel4 = 30; // ds:0x3f9e
Bit8u  gs_dng12_ingerimm_sacrifice = 0; // ds:0x3f9f
Bit8u  gs_dng12_ingerimm_hint = 0; // ds:0x3fa0
Bit8s  gs_dng12_watertrap_water_runs = 0; // ds:0x3fa1
Bit32s gs_dng12_watertrap_timer = MINUTES(50); // ds:0x3fa2
Bit8u  gs_dng12_watertrap_active = 1; // ds:0x3fa6
Bit8s  gs_dng12_obstacle_hp = 40; // ds:0x3fa7, {-25,...,40}
Bit8u  gs_dng12_obstacle_active = 1; // ds:0x3fa8
Bit8u  gs_dng12_speartrap_active = 1; // ds:0x3fa9
Bit8u  gs_dng12_chest1_content[12] = {
	ITEM_CHAIN_MAIL,
	ITEM_CHAIN_MAIL,
	ITEM_KRIEGSBEIL,
	ITEM_KRIEGSBEIL,
	ITEM_SHORT_SWORD,
	ITEM_SHORT_SWORD,
	ITEM_SHORT_SWORD,
	ITEM_LEATHER_HELMET,
	ITEM_LEATHER_HELMET,
	ITEM_LE_POTION,
	ITEM_AE_POTION,
	0xff }; // ds:0x3faa
Bit8u  gs_dng14_money_flag = 0; // ds:0x3fb6
Bit8u  gs_dng14_alarm_flag = 0; // ds:0x3fb7
Bit8u  gs_dng14_secretdoor1 = 0; // ds:0x3fb8, {0, 1 = found, 2 = unlocked}
Bit8u  gs_dng14_secretdoor2 = 0; // ds:0x3fb9, {0, 1 = found, 2 = unlocked}
Bit8u  gs_dng14_secretdoor3 = 0; // ds:0x3fba, {0, 1 = found, 2 = unlocked}
Bit8u  gs_dng14_secretdoor4 = 0; // ds:0x3fbb, {0, 1 = found, 2 = unlocked}
Bit8u  gs_dng14_secretdoor5 = 0; // ds:0x3fbc, {0, 1 = found, 2 = unlocked}
Bit8u  gs_dng14_poisontrap = 0; // ds:0x3fbd
Bit8u  gs_dng14_lvl2_fight = 0; // ds:0x3fbe
Bit8u  gs_dng14_booty_flag = 0; // ds:0x3fbf
Bit8u  gs_dng14_torches_flag = 0; // ds:0x3fc0
Bit8u  gs_dng14_spear_flag = 0; // ds:0x3fc1
Bit8u  gs_dng14_hatchet_flag = 0; // ds:0x3fc2
Bit8u  gs_dng14_ring_flag = 0; // ds:0x3fc3
Bit8u  gs_dng14_orknase_flag = 0; // ds:0x3fc4
Bit8u  gs_dng14_spooky_flag = 0; // ds:0x3fc5
Bit8u  gs_dng14_cellarexit_flag = 0; // ds:0x3fc6
Bit8u  gs_dng14_chest_pantry[9] = {
	ITEM_FOOD_PACKAGE, 50,
	ITEM_BEER, 20,
	ITEM_WINE, 20,
	ITEM_BRANDY, 5,
	0xff
}; // ds:0x3fc7
Bit8u  gs_dng14_chest_gear[13] = {
	ITEM_PICKAXE, 3,
	ITEM_SHOVEL, 2,
	ITEM_ROPE, 3,
	ITEM_TORCH_OFF, 14,
	ITEM_OIL, 20,
	ITEM_CROWBAR, 3,
	0xff
}; // ds:0x3fd0
Bit8u  gs_dng14_chest_barrel[7] = {
	ITEM_SPEAR, 3,
	ITEM_HALBERD, 3,
	ITEM_ZWEILILIEN, 1,
	0xff
}; // ds:0x3fdd
Bit8u gs_dng14_chest_x1[7] = { 0x5c, 0x9a, 0x9a, 0x0e, 0x9b, 0x9b, 0xff }; // ds:0x3fe4
Bit8u gs_dng14_chest_x2[16] = { 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x9a, 0x9a, 0x9a, 0x9b, 0x9b, 0x9b, 0xff }; // ds:0x3feb
Bit8u gs_dng14_chest_x3[11] = {
	ITEM_SHORTBOW, 3,
	ITEM_CROSSBOW, 1,
	ITEM_THROWING_KNIFE, 4,
	ITEM_BOLTS, 20,
	ITEM_ARROWS, 50,
	0xff
}; // ds:0x3ffb
Bit8u gs_dng14_chest_x4[14] = { 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x0e, 0x0e, 0x0e, 0x0e, 0x87, 0x87, 0x92, 0xff }; // ds:0x4006
Bit8u gs_dng14_chest_x5[10] = { 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5c, 0x5c, 0x91, 0x91, 0xff }; // ds:0x4014
Bit8u gs_dng14_chest_x6[8] = { 0x4a, 0x4a, 0x4a, 0x27, 0x27, 0x22, 0x4c, 0xff }; // ds:0x401e
Bit8u gs_dng14_chest_x7[7] = { 0xef, 0xef, 0xef, 0xef, 0xef, 0xef, 0xff }; // ds:0x4026
Bit8u gs_dng14_chest_x8[13] = { 0x88, 0x88, 0x88, 0x86, 0x86, 0x86, 0x05, 0x05, 0x05, 0x07, 0x07, 0x07, 0xff }; // ds:0x402d
Bit8u  gs_dng14_fullmap_flag = 1; // ds:0x403a

Bit8u  gs_dng02_chest00_content[2] = { 0x86, 0xff }; // ds:0x403b
Bit8u  gs_dng02_chest01_content[4] = { 0x03, 0x62, 0x04, 0xff }; // ds:0x403d
Bit8u  gs_dng02_chest06_content[4] = { 0x48, 0x1e, 0x0e, 0xff }; // ds:0x4041
Bit8u  gs_dng02_chest02_content[8] = { 0x2f, 0x99, 0x91, 0x91, 0x94, 0xa7, 0xcb, 0xff }; // ds:0x4045
Bit8u  gs_dng02_chest03_content[8] = { 0x2f, 0x95, 0xec, 0xb4, 0xa9, 0xcc, 0xf3, 0xff }; // ds:0x404d
Bit8u  gs_dng02_chest05_content[8] = { 0x2f, 0x95, 0xec, 0xb4, 0xa9, 0xcc, 0xf3, 0xff }; // ds:0x4055

Bit8u  gs_dng03_chest00_content[7] = { 0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0xff }; // ds:0x405d
Bit8u  gs_dng03_chest01_content[8] = { 0x79, 0x79, 0x1b, 0x49, 0x1a, 0x28, 0x91, 0xff }; // ds:0x4064
Bit8u  gs_dng03_chest02_content[13] = { 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x5b, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0xff }; // ds:0x406c
Bit8u  gs_dng03_chest03_content[3] = { 0xd8, 0x9a, 0xff }; // ds:0x4079
Bit8u  gs_dng03_chest04_content[9] = { 0x4c, 0x4c, 0x4c, 0x5b, 0x5b, 0x5b, 0x5e, 0xff, 0xff }; // ds:0x407c
Bit8u  gs_dng03_chest06_content[8] = { 0x4c, 0x4c, 0x4c, 0x5b, 0x5b, 0x5b, 0x5e, 0xff }; // ds:0x4085
Bit8u  gs_dng03_chest07_content[2] = { 0xe1, 0xff }; // ds:0x408d
Bit8u  gs_dng03_chest08_content[4] = { 0x9a, 0x9a, 0x9a, 0xff }; // ds:0x408f
Bit8u  gs_dng03_chest09_content[11] = { 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xff }; // ds:0x4093
Bit8u  gs_dng03_chest12_content[7] = { 0xf5, 0xf5, 0xf6, 0x27, 0x27, 0xd3, 0xff }; // ds:0x409e

Bit8u  gs_dng04_chest_corpse0[4] = { 0x03, 0x0e, 0x5c, 0xff }; // ds:0x40a5
Bit8u  gs_dng04_chest_corpse1[12] = { 0x01, 0x01, 0x88, 0x0e, 0x0e, 0x6e, 0x50, 0x79, 0x49, 0x19, 0xb0, 0xff }; // ds:0x40a9
Bit8u  gs_dng04_chest_corpse2[10] = { 0x0e, 0x13, 0x54, 0x79, 0x48, 0x1e, 0x55, 0x2e, 0x92, 0xff }; // ds:0x40b5

Bit8u  gs_dng06_chest1_content[16] = { 0x70, 0x70, 0x01, 0x01, 0x35, 0x35, 0x4e, 0x4e, 0x07, 0x07, 0x0e, 0x0e, 0x0c, 0x0c, 0x50, 0xff }; // ds:0x40bf
Bit8u  gs_dng06_chest_dasptreas[17] = {
	ITEM_CHAIN_MAIL_MAGIC, 1,
	ITEM_GOLD_JEWELRY, 1,
	ITEM_GOLD_JEWELRY, 1,
	ITEM_STRONG_LE_POTION, 1,
	ITEM_STRONG_LE_POTION, 1,
	ITEM_STRONG_AE_POTION, 1,
	ITEM_STRONG_AE_POTION, 1,
	ITEM_KK_ELIXIR, 1,
	0xff
}; // ds:0x40cf
   //
Bit8u  gs_dng08_chest1_content[11] = {
	ITEM_SABER, 8,
	ITEM_SPEAR, 3,
	ITEM_MACE, 2,
	ITEM_SHORTBOW, 1,
	ITEM_ARROWS, 40,
	0xff
}; // ds:0x40e0
Bit8u  gs_dng08_chest2_content[2] = { 0xb1, 0xff }; // ds:0x40eb
Bit8u  gs_dng08_chest4_content[2] = { 0xb2, 0xff }; // ds:0x40ed
Bit8u  gs_dng08_chest5_content[6] = { 0x27, 0x27, 0x4a, 0x93, 0xb3, 0xff }; // ds:0x40ef

Bit8u  gs_dng09_lever1_flag = 0; // ds:0x40f5
Bit8u  gs_dng09_bolttrap1_flag = 3; // ds:0x40f6
Bit8u  gs_dng09_bolttrap2_flag = 3; // ds:0x40f7
Bit8u  gs_dng09_bolttrap3_flag = 1; // ds:0x40f8
Bit8u  gs_dng09_cultist_flag = 1; // ds:0x40f9
Bit8u  gs_dng09_lever2_flag = 0; // ds:0x40fa
Bit8u  gs_dng09_bolttrap4_flag = 10; // ds:0x40fb
Bit8u  gs_dng09_crystal_flag = 0; // ds:0x40fc
Bit8u  gs_dng09_pit_flag = 0; // ds:0x40fd, number of heroes in pit
unsigned char g_unkn_025[1] = { 0x00 }; // ds:0x40fe
Bit8u  gs_dng09_altar_flag = 0; // ds:0x40ff
Bit8u  gs_dng09_secretdoor1 = 0; // ds:0x4100, {0, 1 = found, 2 = unlocked}
Bit8u  gs_dng09_secretdoor2 = 0; // ds:0x4101, {0, 1 = found, 2 = unlocked}
Bit8u  gs_dng09_chest0[12] = { 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x5b, 0x5b, 0x5c, 0xff }; // ds:0x4102
Bit8u  gs_dng09_chest1[10] = { 0x4c, 0x4c, 0x4c, 0x19, 0x19, 0x19, 0x29, 0x29, 0x29, 0xff }; // ds:0x410e
Bit8u  gs_dng09_chest2[8] = { 0x4a, 0x4a, 0x27, 0x27, 0x27, 0xea, 0x9a, 0xff }; // ds:0x4118
Bit8u  gs_dng09_chest3[21] = { 0x4b, 0x4b, 0x4b, 0x30, 0x30, 0x30, 0x31, 0x31, 0x31, 0x0e, 0x0e, 0x0e, 0x01, 0x5b, 0x17, 0xff, 0x01, 0x0e, 0x4a, 0x23, 0xff }; // ds:0x4120
Bit8u  gs_dng09_chest5[4] = { 0xbd, 0xbe, 0x4c, 0xff }; // ds:0x4135
Bit8u  gs_dng09_chest6[7] = { 0x4a, 0x4a, 0x27, 0x27, 0x27, 0xd9, 0xff }; // ds:0x4139
Bit8u  gs_dng10_corpse_flag = 0; // ds:0x4140
Bit8u  gs_dng10_lever_found = 0; // ds:0x4141
Bit8u  gs_dng10_hole_state = 0; // ds:0x4142
Bit8u  gs_dng10_lever_state = 0; // ds:0x4143
Bit8u  gs_dng10_hole_damage = 0; // ds:0x4144, {0, 1, ..., 255} ; default = 0
Bit8u  gs_dng10_floorplate_found = 0; // ds:0x4145
Bit8u  gs_dng10_floorplate_loads = 3; // ds:0x4146
Bit8u  gs_dng10_mummy_lever = 0; // ds:0x4147
Bit8u  gs_dng10_heshtot = 0; // ds:0x4148
Bit8u  gs_dng10_dragon_quest = 0; // ds:0x4149
Bit8u  gs_dng10_hoard_plundered = 0; // ds:0x414a
Bit8u  gs_dng10_chest0_content[5] = { 0x50, 0x1b, 0x91, 0xb7, 0xff}; // ds:0x414b
Bit8u  gs_dng10_chest1_content[4] = { 0x91, 0x92, 0x91, 0xff}; // ds:0x4150
Bit8u  gs_dng10_chest2_content[4] = { 0x9a, 0x9b, 0x9a, 0xff}; // ds:0x4154
Bit8u  gs_dng10_chest3_content[3] = { 0xc7, 0xc5, 0xff}; // ds:0x4158
Bit8u  gs_dng10_chest4_content[2] = { 0xdf, 0xff}; // ds:0x415b

Bit8u  gs_dng11_waterbarrel1 = 80; // ds:0x415d
Bit8u  gs_dng11_waterbarrel2 = 20; // ds:0x415e
Bit8u  gs_dng11_efferd_sacrifice = 0; // ds:0x415f
Bit8u  gs_dng11_efferd_hint = 0; // ds:0x4160
Bit8u  gs_dng11_unknown1_flag = 0; // ds:0x4161
Bit8u  gs_dng11_firetrap1_flag = 2; // ds:0x4162
Bit8u  gs_dng11_unknown2_flag = 1; // ds:0x4163
Bit8u  gs_dng11_firetrap2_flag = 3; // ds:0x4164
Bit8u  gs_dng11_secretdoor1_flag = 0; // ds:0x4165
Bit8u  gs_dng11_secretdoor2_flag = 0; // ds:0x4166
Bit8u  gs_dng11_secretdoor3_flag = 0; // ds:0x4167
Bit8u  gs_dng11_lever_flag = 2; // ds:0x4168, {0-5}
Bit8u  gs_dng11_proviant1_flag = 0; // ds:0x4169
Bit8u  gs_dng11_proviant2_flag = 0; // ds:0x416a
Bit8u  gs_dng11_chest3[15] = { 0x03, 0x03, 0x03, 0x03, 0x03, 0x70, 0x70, 0x70, 0x70, 0x70, 0x01, 0x86, 0x86, 0x88, 0xff }; // ds:0x416b
Bit8u  gs_dng11_chest4[11] = { 0x09, 0x09, 0x05, 0x05, 0x05, 0x05, 0x05, 0x0c, 0xfb, 0xfc, 0xff }; // ds:0x417a
Bit8u  gs_dng11_chest5[5] = { 0x0e, 0x5c, 0x27, 0x30, 0xff }; // ds:0x4185
Bit8u  gs_dng11_chest6[7] = { 0x86, 0x27, 0x5b, 0x60, 0x31, 0x91, 0xff }; // ds:0x418a
Bit8u  gs_dng11_chest7 = 0xff; // ds:0x4191
Bit8u  gs_dng11_chest8[6] = { 0x5c, 0x5c, 0x5c, 0x0e, 0x91, 0xff }; // ds:0x4192
Bit8u  gs_dng11_chest9[6] = { 0x1a, 0x5d, 0x49, 0x87, 0x87, 0xff }; // ds:0x4198
Bit8u  gs_dng11_chest10[11] = { 0x79, 0x79, 0x79, 0x79, 0x1b, 0x1b, 0x1b, 0x1b, 0xfd, 0x87, 0xff }; // ds:0x419e
Bit8u  gs_dng11_chest11[4] = { 0x5b, 0xc0, 0x0e, 0xff }; // ds:0x41a9
Bit8u  gs_dng11_chest12[13] = { 0x4a, 0x4a, 0x4a, 0x4a, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x4c, 0x4c, 0xff }; // ds:0x41ad
Bit8u  gs_dng11_chest13[6] = { 0x91, 0x94, 0x2f, 0xcb, 0x01, 0xff }; // ds:0x41ba
Bit8u  gs_dng11_chest14[6] = { 0x24, 0x1d, 0x5b, 0x9a, 0xbd, 0xff }; // ds:0x41c0
Bit8u  gs_dng15_took_cursed_money = 0; // ds:0x41c6
Bit8u  gs_dng15_unknown_flag = 1; // ds:0x41c7
Bit8u  gs_dng15_reached_hands = 0; // ds:0x41c8
Bit8u  gs_dng15_lever_south = 0; // ds:0x41c9
Bit8u  gs_dng15_lever_north = 0; // ds:0x41ca
Bit8u  gs_dng15_took_hoe = 0; // ds:0x41cb
Bit8u  gs_dng15_ceilings[18] = { 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 2, 3, 1, 1 }; // ds:0x41cc
Bit8u  gs_dng15_cursed_money[6] = { 0, 0, 0, 0, 0, 0 }; // ds:0x41de
Bit8u  gs_dng15_undead_fight = 0; // ds:0x41e4
Bit8u  gs_dng15_chest_equips[7] = { 0x5d, 0x02, 0x1b, 0x03, 0x65, 0x01, 0xff }; // ds:0x41e5
Bit8u  gs_dng01_chest1_content[8] = { 0x70, 0x79, 0x79, 0x79, 0x1b, 0x08, 0x18, 0xff }; // ds:0x41ec
Bit8u  gs_dng01_chest2_content[10] = { 0x03, 0x6d, 0x5c, 0x19, 0x29, 0x24, 0x1d, 0x27, 0x91, 0xff }; // ds:0x41f4
Bit8u  gs_dng01_chest3_content[2] = { ITEM_DAGGER, 0xff }; // ds:0x41fe
Bit8u  gs_dng01_chest5_content[13] = { 0x6e, 0x6e, 0x03, 0x03, 0x70, 0x70, 0x43, 0x62, 0x62, 0x62, 0x10, 0x66, 0xff }; // ds:0x4200
Bit8u  gs_dng01_chest7_content[5] = { ITEM_SWORD, ITEM_DAGGER, ITEM_GOLD_JEWELRY, ITEM_PICKLOCKS, 0xff }; // ds:0x420d
Bit8s  gs_unconscious_message[7] = {0}; // ds:0x4212
Bit8s  gs_food_message[7] = {0};	// ds:0x4219
unsigned char g_unkn_026[2] = { 0, 0 }; // ds:0x4220
Bit16s gs_current_locdata = 0; // ds:0x4222
Bit16s gs_current_typeindex = 0; // ds:0x4224
Bit8u  gs_dng03_highpriest_killed = 0; // ds:0x4226, {0, 14 = in fight 224, 16 = in fight 222}
Bit8s  gs_dng03_chest12_loads = 6; // ds:0x4227, {0,1,...,6}
Bit16s gs_trv_i = 0; // ds:0x4228
Bit16s gs_route_stepcount = 0; // ds:0x422a
Bit16s gs_forcedmarch_le_cost = 0; // ds:0x422c
Bit16s gs_route_total_steps = 0; // ds:0x422e
Bit16s gs_route_length = 0; // ds:0x4230
Bit16s gs_route_duration = 0; // ds:0x4232
Bit16s gs_route_timedelta = 0; // ds:0x4234
Bit16s gs_route_mousehover = 0; // ds:0x4236
Bit16s gs_route_progress = 0; // ds:0x4238
Bit16s gs_route_stepsize = 0; // ds:0x423a
Bit16s gs_route_dayprogress = 0; // ds:0x423c
Bit16s gs_sea_travel_passage_no = 0; // ds:0x423e
Bit16s gs_route_encounter_flag = 0; // ds:0x4240
Bit16s gs_route_encounter_time = 0; // ds:0x4242
Bit16s gs_route_informer_flag = 0; // ds:0x4244
Bit16s gs_route_informer_time = 0; // ds:0x4246
Bit16s gs_route_fight_flag = 0; // ds:0x4248
Bit16s gs_route_fight_time = 0; // ds:0x424a
Bit16s gs_travel_speed = 0; // ds:0x424c
Bit16s gs_passage_deadship_flag = 0; // ds:0x424e
Bit16s gs_passage_deadship_position = 0; // ds:0x4250
Bit16s gs_passage_octopus_flag = 0; // ds:0x4252
Bit16s gs_passage_octopus_position = 0; // ds:0x4254
Bit16s gs_passage_pirates_flag = 0; // ds:0x4256
Bit16s gs_passage_pirates_position = 0; // ds:0x4258
#if defined (__BORLANDC__)
Bit16s *gs_route_course_ptr = NULL;			// ds:0x425a;
Bit16s *gs_route_course_start = NULL;			// ds:0x425e;
Bit16s *gs_route_course_ptr2 = NULL;			// ds:0x4262;
Bit8u  *gs_sea_travel_courses = NULL;			// ds:0x4266
struct struct_tevent *gs_tevents_tab_ptr = NULL;	// ds:0x426a
struct struct_land_route *gs_travel_route_ptr = NULL;	// ds:0x426e
#else
Bit32u gs_route_course_ptr_obsolete = 0;	// ds:0x425a; This is a dummy now!
Bit32u gs_route_course_start_obsolete = 0;	// ds:0x425e; This is a dummy now!
Bit32u gs_route_course_ptr2_obsolete = 0;	// ds:0x4262; This is a dummy now!
Bit32u gs_sea_travel_courses_obsolete = 0;	// ds:0x4266; This is a dummy now!
Bit32u gs_tevents_tab_ptr_obsolete = 0;		// ds:0x426a; This is a dummy now!
Bit32u gs_travel_route_ptr_obsolete = 0;	// ds:0x426e; This is a dummy now!
#endif
struct struct_route_tevent gs_route_tevents[15] = {
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 },
	{ 0x0000, 0x0000 }
}; // ds:0x4272
Bit8u  gs_sea_travel_psgbooked_flag = 0; // ds:0x42ae
Bit8u  gs_sea_travel_psgbooked_timer = 0; // ds:0x42af SHOULD BE SIGNED
Bit8s  gs_sea_travel_passage_speed1 = 0; // ds:0x42b0
Bit8u  gs_current_sea_route_id = 0; // ds:0x42b1

/* REMARK: uses 120 byte in the game state */
struct harbor_option_obsolete gs_harbor_options[10] = { { 0 } }; // ds:0x42b2

Bit16s gs_sea_travel_passage_price = 0; // ds:0x432a
Bit16s gs_sea_travel_passage_speed2 = 0; // ds:0x432c // shouldn't this be _unsigned_ ?

#if defined(__BORLANDC__)
Bit8u  *gs_travel_map_ptr = 0; 		// ds:0x432e; Bit8u*
#else
Bit32u gs_travel_map_ptr_obsolete = 0;	// ds:0x432e; This is a dummy now!
#endif

Bit8u  gs_forcedmarch_timer = 0; // ds:0x4332
Bit8u  gs_travel_detour = 0; // ds:0x4333
Bit16s gs_current_signpost = 0; // ds:0x4334
Bit16s gs_trv_return = 0; // ds:0x4336, {-1, 0, 1, 2} + ?
Bit16s gs_travel_destination_town_id = 0; // ds:0x4338
Bit16s gs_travel_destination_x = 0; // ds:0x433a
Bit16s gs_travel_destination_y = 0; // ds:0x433c
Bit16s gs_travel_destination_viewdir = 0; // ds:0x433e

#if defined(__BORLANDC__)
struct trv_start_point *gs_tm_unused1_ptr = NULL;	// ds:0x4340;
#else
Bit32u gs_tm_unused1_ptr_obsolete = 0;			// ds:0x4340;
#endif

Bit8s  gs_trv_menu_towns[6] = { 0, 0, 0, 0, 0, 0 }; // ds:0x4344
Bit16s gs_trv_destination = 0; // ds:0x434a
Bit8u  gs_dng08_waterbarrel = 0; // ds:0x434c
Bit8u  gs_dng13_collapsecount = 0; // ds:0x434d
Bit8u  gs_dng13_herocount = 0; // ds:0x434e
Bit16s gs_camp_incident = -1; // ds:0x434f, -1 = not determined or will not happen, 0,1,2 = guard that will be affected
Bit8s  gs_known_monsters[78] = { 0 }; // ds:0x4351
Bit8u  gs_nameless_destroyed = 0; // ds:0x439f
Bit8u  gs_deadship_final = 0; // ds:0x43a0
Bit8u  gs_dng09_lever_fast = 0; // ds:0x43a1
Bit8u  gs_dng11_soup_supply = 10; // ds:0x43a2
Bit16s gs_arsenal_money = -1; // ds:0x43a3, {-1, 0 - 60 }
Bit8s  gs_announce_day = 0; // ds:0x43a5

/* REMARK: Maybe from this point Game State infomation is V3.XX only */
Bit8s  gs_known_persons[20] = {0}; // ds:0x43a6
Bit16s gs_diary_entry_counter = 0; // ds:0x43ba
struct struct_diary_entry gs_diary_entries[23] = { {0} }; // ds:0x43bc

Bit8u gs_datseg_status_end = 0; // ds:0x4474

unsigned char g_fading_state = 0; // ds:0x4475
unsigned char g_music_enabled = 1; // ds:0x4476
unsigned char g_snd_effects_enabled = 0; // ds:0x4477
unsigned char g_unkn_028[2] = { 0xff, 0xff }; // ds:0x4478
signed short g_music_current_track = -1; // ds:0x447a
signed short g_snd_voc_enabled = 0; // ds:0x447c

#if defined(__BORLANDC__)
extern unsigned _stklen = 0x2000; // ds:0x447e
#endif

extern char g_str_file_missing[20];	//forward declaration
char *g_str_file_missing_ptr = (char*)&g_str_file_missing; // ds:0x4480, to STR_FILE_MISSING; Bit8u*
unsigned short g_mouse_irq_init = 0; // ds:0x4484
unsigned char g_unkn_030[4] = { 0xff, 0xff, 0x00, 0x00 }; // ds:0x4486
char g_pause_string[10] = "P A U S E"; // ds:0x448a
signed char g_npc_last_farewellcheck = 0; // ds:0x4494
signed char g_check_party = 0; // ds:0x4495
signed char g_food_mod = 0; // ds:0x4496
signed char g_travel_herokeeping = 0; // ds:0x4497
Bit8u g_floor_fade_palette[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x28, 0x00, 0x28 },
	{ 0x16, 0x0c, 0x08 },
	{ 0x13, 0x09, 0x06 },
	{ 0x11, 0x06, 0x03 },
	{ 0x0f, 0x04, 0x01 },
	{ 0x0c, 0x01, 0x00 },
	{ 0x0a, 0x00, 0x00 },
	{ 0x08, 0x00, 0x00 },
	{ 0x05, 0x00, 0x00 },
	{ 0x03, 0x00, 0x00 },
	{ 0x01, 0x00, 0x00 },
	{ 0x00, 0x09, 0x00 },
	{ 0x00, 0x07, 0x00 },
	{ 0x00, 0x05, 0x00 },
	{ 0x00, 0x03, 0x00 },
	{ 0x00, 0x01, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x1f },
	{ 0x00, 0x00, 0x1c },
	{ 0x00, 0x00, 0x1a },
	{ 0x00, 0x00, 0x18 },
	{ 0x00, 0x00, 0x16 },
	{ 0x00, 0x00, 0x14 },
	{ 0x00, 0x00, 0x11 },
	{ 0x00, 0x00, 0x0f },
	{ 0x00, 0x00, 0x0d },
	{ 0x00, 0x00, 0x0b }
}; // ds:0x4498
Bit8u g_building_fade_palette[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x22, 0x22, 0x26 },
	{ 0x1b, 0x1b, 0x1f },
	{ 0x15, 0x15, 0x19 },
	{ 0x0f, 0x0f, 0x13 },
	{ 0x09, 0x09, 0x0d },
	{ 0x03, 0x03, 0x07 },
	{ 0x00, 0x00, 0x04 },
	{ 0x00, 0x00, 0x00 },
	{ 0x20, 0x19, 0x19 },
	{ 0x1b, 0x12, 0x12 },
	{ 0x16, 0x0c, 0x0c },
	{ 0x11, 0x06, 0x07 },
	{ 0x0b, 0x00, 0x04 },
	{ 0x07, 0x00, 0x04 },
	{ 0x01, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00 },
	{ 0x0a, 0x00, 0x00 },
	{ 0x15, 0x00, 0x04 },
	{ 0x20, 0x00, 0x04 },
	{ 0x13, 0x15, 0x09 },
	{ 0x09, 0x0e, 0x03 },
	{ 0x00, 0x07, 0x04 },
	{ 0x00, 0x00, 0x04 },
	{ 0x00, 0x00, 0x00 },
	{ 0x1b, 0x0e, 0x04 },
	{ 0x12, 0x07, 0x04 },
	{ 0x09, 0x00, 0x04 },
	{ 0x00, 0x00, 0x01 },
	{ 0x00, 0x00, 0x08 },
	{ 0x00, 0x03, 0x0f },
	{ 0x00, 0x08, 0x16 }
}; // ds:0x44f8
Bit8u g_sky_fade_palette[32][3] = {
	{ 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x11 },
	{ 0x00, 0x00, 0x1b },
	{ 0x28, 0x00, 0x28 },
	{ 0x28, 0x00, 0x28 },
	{ 0x0e, 0x0e, 0x19 },
	{ 0x0b, 0x0b, 0x17 },
	{ 0x09, 0x09, 0x15 },
	{ 0x07, 0x07, 0x14 },
	{ 0x05, 0x05, 0x12 },
	{ 0x04, 0x04, 0x11 },
	{ 0x02, 0x02, 0x0f },
	{ 0x01, 0x01, 0x0d },
	{ 0x01, 0x01, 0x0c },
	{ 0x00, 0x00, 0x0a },
	{ 0x00, 0x00, 0x09 },
	{ 0x00, 0x00, 0x0e },
	{ 0x00, 0x00, 0x0b },
	{ 0x00, 0x00, 0x09 },
	{ 0x00, 0x00, 0x07 },
	{ 0x00, 0x00, 0x04 },
	{ 0x00, 0x00, 0x02 },
	{ 0x00, 0x00, 0x00 },
	{ 0x28, 0x00, 0x28 },
	{ 0x28, 0x00, 0x28 },
	{ 0x28, 0x00, 0x28 },
	{ 0x28, 0x00, 0x28 },
	{ 0x28, 0x00, 0x28 },
	{ 0x07, 0x07, 0x16 },
	{ 0x07, 0x07, 0x15 },
	{ 0x07, 0x07, 0x14 },
	{ 0x08, 0x07, 0x13 }
}; // ds:0x4558
signed char g_special_screen = 1; // ds:0x45b8, , e.g. diary, treasure map, game options, automap
struct special_day g_special_days[43] = {
	{ 0x01, 0x01, 0x01 },
	{ 0x01, 0x02, 0x02 },
	{ 0x01, 0x03, 0x02 },
	{ 0x02, 0x05, 0x03 },
	{ 0x02, 0x0f, 0x04 },
	{ 0x02, 0x10, 0x04 },
	{ 0x03, 0x01, 0x05 },
	{ 0x03, 0x09, 0x06 },
	{ 0x03, 0x10, 0x07 },
	{ 0x03, 0x1d, 0x08 },
	{ 0x03, 0x1e, 0x09 },
	{ 0x04, 0x01, 0x0a },
	{ 0x04, 0x02, 0x0b },
	{ 0x04, 0x03, 0x0b },
	{ 0x04, 0x04, 0x0c },
	{ 0x04, 0x0c, 0x0d },
	{ 0x05, 0x01, 0x0e },
	{ 0x05, 0x16, 0x0f },
	{ 0x05, 0x18, 0x10 },
	{ 0x05, 0x1e, 0x11 },
	{ 0x06, 0x07, 0x12 },
	{ 0x06, 0x1e, 0x13 },
	{ 0x07, 0x01, 0x14 },
	{ 0x07, 0x1e, 0x15 },
	{ 0x08, 0x05, 0x16 },
	{ 0x08, 0x1e, 0x17 },
	{ 0x09, 0x10, 0x18 },
	{ 0x09, 0x18, 0x19 },
	{ 0x09, 0x1e, 0x1a },
	{ 0x0a, 0x01, 0x1b },
	{ 0x0a, 0x12, 0x1c },
	{ 0x0b, 0x01, 0x1d },
	{ 0x0b, 0x08, 0x1e },
	{ 0x0b, 0x15, 0x1f },
	{ 0x0c, 0x01, 0x20 },
	{ 0x0c, 0x02, 0x20 },
	{ 0x0c, 0x03, 0x20 },
	{ 0x0c, 0x04, 0x20 },
	{ 0x0c, 0x05, 0x20 },
	{ 0x0c, 0x06, 0x20 },
	{ 0x0c, 0x07, 0x20 },
	{ 0x0c, 0x1e, 0x21 },
	{ -0x01, 0x00, 0x00 }
}; // ds:0x45b9
signed char g_months_spring[4] = { 10, 11, 12, -1 }; // ds:0x463a
signed char g_months_winter[4] = { 7, 8, 9, -1 }; // ds:0x463e
signed char g_months_summer[4] = { 1, 2, 3, -1 }; // ds:0x4642
signed short g_bank_heller = 0; // ds:0x4646
signed char g_consume_quiet = 0; // ds:0x4648
unsigned char g_herokeeping_flag = 0; // ds:0x4649
unsigned char g_unkn_031[4] = { 0x00, 0x00, 0x00, 0x00 }; // ds:0x464a

// for the following there is an easy formula: g_level_ap_tab[i] = 50 * i * (i+1). therefore, these 84 bytes could be easily freed and reused */
signed long g_level_ap_tab[21] = { 0, 100, 300, 600, 1000, 1500, 2100, 2800, 3600, 4500, 5500, 6600, 7800, 9100, 10500, 12000, 13600, 15300, 17100, 19000, 21000 }; // ds:0x464e
unsigned char g_delay_or_keypress_flag = 0; // ds:0x46a2, writeonly (0,1)
const signed short g_merchant_items_posx[15] = { 0x001e, 0x001e, 0x001e, 0x001e, 0x001e, 0x005f, 0x005f, 0x005f, 0x005f, 0x005f, 0x00a0, 0x00a0, 0x00a0, 0x00a0, 0x00a0 }; // ds:0x46a3
const signed short g_merchant_items_posy[15] = { 0x0023, 0x0037, 0x004b, 0x005f, 0x0073, 0x0023, 0x0037, 0x004b, 0x005f, 0x0073, 0x0023, 0x0037, 0x004b, 0x005f, 0x0073 }; // ds:0x46c1
signed char g_refresh_status_line = 0; // ds:0x46df
char g_dsa_version_string[12] = "DSA VERSION"; // ds:0x46e0
char g_game_version[6] = "V3.02"; // ds:0x46ec
unsigned char g_unkn_032[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // ds:0x46f2
char g_version_token1 = 3; // ds:0x46f8, = 3
char g_version_token2 = 1; // ds:0x46f9, = 1
char g_version_token3 = 1; // ds:0x46fa, = 1
char g_version_token4 = 1; // ds:0x46fb, = 1
const char g_questions_handbook[10][19] = {
	{ 1, 6, 2, 7,  ~'T', ~'H', ~'O', ~'R', ~'W', ~'A', ~'L', 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0 },
	{ 1, 7, 1, 12, ~'V', ~'O', ~'R', ~'S', ~'I', ~'C', ~'H', ~'T', 0x00, 0x00, 0x00, 0, 0, 0, 0 },
	{ 2, 2, 1, 17, ~'W', ~'I', ~'S', ~'S', ~'E', ~'N', 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0 },
	{ 4, 3, 2, 25, ~'O', ~'R', ~'T', ~'E', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0 },
	{ 1, 5, 2, 58, ~'H', ~'E', ~'X', ~'E', ~'N', ~'K', ~'N', ~'O', ~'T', ~'E', ~'N', 0, 0, 0, 0 },
	{ 1, 1, 1, 60, ~'S', ~'T', ~'A', ~'B', ~'Z', ~'A', ~'U', ~'B', ~'E', ~'R', 0x00, 0, 0, 0, 0 },
	{ 4, 5, 1, 67, ~'P', ~'A', ~'R', ~'A', ~'D', ~'E', ~'W', ~'E', ~'R', ~'T', 0x00, 0, 0, 0, 0 },
	{ 1, 1, 1, 74, ~'S', ~'K', ~'E', ~'L', ~'E', ~'T', ~'T', 0x00, 0x00, 0x00, 0x00, 0, 0, 0, 0 },
	{ 1, 5, 2, 77, ~'O', ~'L', ~'G', ~'I', ~'N', ~'W', ~'U', ~'R', ~'Z', 0x00, 0x00, 0, 0, 0, 0 },
	{ 3, 2, 1, 87, ~'A', ~'B', ~'S', ~'C', ~'H', ~'N', ~'I', ~'T', ~'T', 0x00, 0x00, 0, 0, 0, 0 }
}; // ds:0x46fc
const signed char g_questions_map[10][3] = {
	{ 0x00, 0x0a, 0x09 },
	{ 0x01, 0x05, 0x06 },
	{ 0x03, 0x26, 0x20 },
	{ 0x01, 0x1e, 0x1c },
	{ 0x02, 0x14, 0x11 },
	{ 0x02, 0x08, 0x07 },
	{ 0x00, 0x0b, 0x0c },
	{ 0x00, 0x28, 0x29 },
	{ 0x00, 0x18, 0x2e },
	{ 0x03, 0x19, 0x1a }
}; // ds:0x47ba
char g_snd_menu_title[17] = "BITTE AUSW\x8eHLEN:"; // ds:0x47d8
char g_snd_menu_radio1[12] = "KEINE MUSIK"; // ds:0x47e9
char g_snd_menu_radio2[10] = "NUR MUSIK"; // ds:0x47f5
char g_snd_menu_radio3[12] = "NUR EFFEKTE"; // ds:0x47ff
char g_snd_menu_radio4[18] = "MUSIK UND EFFEKTE"; // ds:0x480b
char g_fname_sound_cfg[10] = "SOUND.CFG"; // ds:0x481d
char g_fname_sound_adv2[10] = "SOUND.ADV"; // ds:0x4827
char g_fname_digi_adv[9] = "DIGI.ADV"; // ds:0x4831
char g_snd_txt_disabled_mem[51] = "SOUND EFFEKTE ABGESCHALTET - NICHT GENUG SPEICHER!"; // ds:0x483a
char g_snd_txt_hw_not_found[31] = "SOUND HARDWARE NICHT GEFUNDEN!"; // ds:0x486d
char g_fname_sound_adv[10] = "SOUND.ADV"; // ds:0x488c
char g_snd_txt_hw_not_found2[31] = "SOUND HARDWARE NICHT GEFUNDEN!"; // ds:0x4896, == SND_TXT_HW_NOT_FOUND
char g_str_file_missing[20] = "FILE %s IS MISSING!"; // ds:0x48b5
unsigned char g_unkn_033[1] = { 0x00 }; // ds:0x48c9
char g_fname_schick_dat[11] = "SCHICK.DAT"; // ds:0x48ca
char g_snd_txt_disabled_mem2[43] = "MUSIK ABGESCHALTET - NICHT GENUG SPEICHER!"; // ds:0x48d5
char g_diary_string1[14] = "%2d.~%-8s~%s."; // ds:0x4900
char g_diary_string2[17] = "%2d.~%-8s~IN %s."; // ds:0x490e
char g_diary_string3[27] = "%2d.~%-8s~BESUCHEN VON %s."; // ds:0x491f
char g_diary_string4[6] = "~~%s."; // ds:0x493a
char g_diary_string5[9] = "~~IN %s."; // ds:0x4940
char g_diary_string6[19] = "~~ABENTEUER IN %s."; // ds:0x4949
char g_diary_string7[19] = "~~RUMH\x8eNGEN IN %s."; // ds:0x495c
unsigned char g_unkn_034[1] = { 0x00 }; // ds:0x496f
struct point8s g_visual_field_dir0[31] = {
	{ -0x01, 0x00 },
	{ 0, 0 },
	{ 0x01, 0x00 },
	{ -0x02, -1 },
	{ -0x01, -1 },
	{ 0x00, -1 },
	{ 0x01, -1 },
	{ 0x02, -1 },
	{ -0x03, -0x02 },
	{ -0x02, -0x02 },
	{ -0x01, -0x02 },
	{ 0x00, -0x02 },
	{ 0x01, -0x02 },
	{ 0x02, -0x02 },
	{ 0x03, -0x02 },
	{ -0x03, -0x03 },
	{ -0x02, -0x03 },
	{ -0x01, -0x03 },
	{ 0x00, -0x03 },
	{ 0x01, -0x03 },
	{ 0x02, -0x03 },
	{ 0x03, -0x03 },
	{ -0x03, -0x04 },
	{ -0x02, -0x04 },
	{ -0x01, -0x04 },
	{ 0x00, -0x04 },
	{ 0x01, -0x04 },
	{ 0x02, -0x04 },
	{ 0x03, -0x04 },
	{ 0x00, -1 },
	{ 0x00, 0x01 }
}; // ds:0x4970
struct point8s g_visual_field_dir1[31] = {
	{ 0x00, -1 },
	{ 0, 0 },
	{ 0x00, 0x01 },
	{ 0x01, -0x02 },
	{ 0x01, -1 },
	{ 0x01, 0x00 },
	{ 0x01, 0x01 },
	{ 0x01, 0x02 },
	{ 0x02, -0x03 },
	{ 0x02, -0x02 },
	{ 0x02, -1 },
	{ 0x02, 0x00 },
	{ 0x02, 0x01 },
	{ 0x02, 0x02 },
	{ 0x02, 0x03 },
	{ 0x03, -0x03 },
	{ 0x03, -0x02 },
	{ 0x03, -1 },
	{ 0x03, 0x00 },
	{ 0x03, 0x01 },
	{ 0x03, 0x02 },
	{ 0x03, 0x03 },
	{ 0x04, -0x03 },
	{ 0x04, -0x02 },
	{ 0x04, -1 },
	{ 0x04, 0x00 },
	{ 0x04, 0x01 },
	{ 0x04, 0x02 },
	{ 0x04, 0x03 },
	{ 0x01, 0x00 },
	{ -0x01, 0x00 }
}; // ds:0x49ae
struct point8s g_visual_field_dir2[31] = {
	{ 0x01, 0x00 },
	{ 0, 0 },
	{ -0x01, 0x00 },
	{ 0x02, 0x01 },
	{ 0x01, 0x01 },
	{ 0x00, 0x01 },
	{ -0x01, 0x01 },
	{ -0x02, 0x01 },
	{ 0x03, 0x02 },
	{ 0x02, 0x02 },
	{ 0x01, 0x02 },
	{ 0x00, 0x02 },
	{ -0x01, 0x02 },
	{ -0x02, 0x02 },
	{ -0x03, 0x02 },
	{ 0x03, 0x03 },
	{ 0x02, 0x03 },
	{ 0x01, 0x03 },
	{ 0x00, 0x03 },
	{ -0x01, 0x03 },
	{ -0x02, 0x03 },
	{ -0x03, 0x03 },
	{ 0x03, 0x04 },
	{ 0x02, 0x04 },
	{ 0x01, 0x04 },
	{ 0x00, 0x04 },
	{ -0x01, 0x04 },
	{ -0x02, 0x04 },
	{ -0x03, 0x04 },
	{ 0x00, 0x01 },
	{ 0x00, -1 }
}; // ds:0x49ec
struct point8s g_visual_field_dir3[31] = {
	{ 0x00, 0x01 },
	{ 0, 0 },
	{ 0x00, -1 },
	{ -0x01, 0x02 },
	{ -0x01, 0x01 },
	{ -0x01, 0x00 },
	{ -0x01, -1 },
	{ -0x01, -0x02 },
	{ -0x02, 0x03 },
	{ -0x02, 0x02 },
	{ -0x02, 0x01 },
	{ -0x02, 0x00 },
	{ -0x02, -1 },
	{ -0x02, -0x02 },
	{ -0x02, -0x03 },
	{ -0x03, 0x03 },
	{ -0x03, 0x02 },
	{ -0x03, 0x01 },
	{ -0x03, 0x00 },
	{ -0x03, -1 },
	{ -0x03, -0x02 },
	{ -0x03, -0x03 },
	{ -0x04, 0x03 },
	{ -0x04, 0x02 },
	{ -0x04, 0x01 },
	{ -0x04, 0x00 },
	{ -0x04, -1 },
	{ -0x04, -0x02 },
	{ -0x04, -0x03 },
	{ -0x01, 0x00 },
	{ 0x01, 0x00 }
}; // ds:0x4a2a
char g_noway_string[40] = "IN DIESE RICHTUNG GEHT ES NICHT WEITER."; // ds:0x4a68


/* start of the BSS section */

#if !defined(__BORLANDC__)
struct struct_hero *gs_ruin_hero;			// ds:0x3e1c;
struct struct_hero *gs_main_acting_hero;		// ds:0x3e20;
Bit16s  *gs_route_course_ptr;				// ds:0x425a;
Bit16s  *gs_route_course_start;				// ds:0x425e;
Bit16s *gs_route_course_ptr2;				// ds:0x4262;
Bit8u *gs_sea_travel_courses;				// ds:0x4266
struct struct_tevent *gs_tevents_tab_ptr;		// ds:0x426a
struct struct_land_route *gs_travel_route_ptr;		// ds:0x426e
Bit8u  *gs_travel_map_ptr; 				// ds:0x432e; Bit8u*
#endif


#if !defined(__BORLANDC__)
/* arrays for meaningful log messages */
const char* names_attrib[14] = {"MU", "KL", "CH", "FF", "GE", "IN", "KK",
				"AG", "HA", "RA", "GG", "TA", "NG", "JZ"};

const char* names_skill[52] = {
    "Waffenlos", "Hiebwaffen", "Stichwaffen", "Schwerter", "Äxte", "Speere", "Zweihänder", "Schusswaffen", "Wurfwaffen",
    "Akrobatik", "Klettern", "Körperbeh.", "Reiten", "Schleichen", "Schwimmen", "Selbstbeh.", "Tanzen", "Verstecken", "Zechen",
    "Bekehren", "Betören", "Feilschen", "Gassenwissen", "Lügen", "Menschenkenntnis", "Schätzen",
    "Fährtensuchen", "Fesseln", "Orientierung", "Pflanzenkunde", "Tierkunde", "Wildnisleben",
    "Alchimie", "Alte Sprachen", "Geographie", "Geschichte", "Götter/Kulte", "Kriegskunst", "Lesen", "Magiekunde", "Sprachen",
    "Abrichten", "Fahrzeuge", "Falschspiel", "Heilen Gift", "Heilen Krankheit", "Heilen Wunden", "Musizieren", "Schlösser", "Taschendieb",
    "Gefahrensinn", "Sinnenschärfe"
};

const char* names_spell[86] = {
    "DUMMY",
    "Beherrschung brechen", "Destructibo", "Gardianum", "Illusionen zerstören", "Verwandlung beenden", // Antimagie
    "Band & Fessel", "Bannbaladin", "Böser Blick", "Große Gier", "Große Verwirrung", "Herr der Tiere", "Horriphobus", "Mag. Raub", "Respondami", "Sanftmut", "Somnigravis", "Zwingtanz", // Beherrschung
    "Furor Blut", "Geister bannen", "Geister rufen", "Heptagon", "Krähenruf", "Skelettarius", // Dämonologie
    "Elementar herbeirufen", "Nihilatio Gravitas", "Solidrid Farbenspiel", // Elementarmagie
    "Axxeleraus", "Foramen", "Motoricus", "Spurlos, Trittlos", "Transversalis", "Über Eis", // Bewegung
    "Balsam", "Hexenspeichel", "Klarum Purum", "Ruhe Körper", "Tiere heilen", // Heilung
    "Adleraug", "Analüs", "Eigenschaften", "Exposami", "Odem Arcanum", "Penetrizzel", "Sensibar", // Hellsicht
    "Chamaelioni", "Duplicatus", "Harmlos", "Hexenknoten", // Illusion
    "Blitz", "Ecliptifactus", "Eisenrost", "Fulminictus", "Ignifaxius", "Plumbumbarum", "Radau", "Saft, Kraft, Monstermacht", "Scharfes Auge", // Kampf
    "Hexenblick", "Nekropathia", // Verständigung
    "Adler, Wolf", "Arcano Psychostabilis", "Armatrutz", "CH steigern", "Feuerbann", "FF steigern", "GE steigern", "IN steigern", "KK steigern", "KL steigern", "MU steigern", "Mutabili", "Paralü", "Salander", "See & Fluss", "Visibili", // Verwandlung
    "Abvenenum", "Aeolitus", "Brenne", "Claudibus", "Dunkelheit", "Erstarre", "Flim Flam", "Schmelze", "Silentium", "Sturmgebrüll" // Veränderung
};

const char* names_mspell[14] = {
    "Verwandlung Beenden",
    "Bannbaladin",
    "Böser Blick",
    "Horriphobus",
    "Axxeleratus",
    "Balsam",
    "Blitz",
    "Eisenrost",
    "Fulminictus",
    "Ignifaxius",
    "Plumbumbarum",
    "Saft, Kraft, Monstermacht",
    "Armatrutz",
    "Paralü"
};

#endif

#if !defined(__BORLANDC__)
}
#endif
