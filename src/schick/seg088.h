#if !defined(__BORLANDC__)
namespace M302de {
#endif

extern struct struct_chest g_dng14_specialchests[9];

/* 0x20 */
void DNG14_dive(signed short, signed char, signed short);
/* 0x25 */
void DNG14_fight_intro(signed short);
/* 0x2a */
void DNG14_chest00_loot(struct struct_chest*);
/* 0x2f */
void DNG14_chest01_loot(struct struct_chest*);
/* 0x34 */
void DNG14_chest02_loot(struct struct_chest*);
/* 0x39 */
void DNG14_chest03_loot(struct struct_chest*);
/* 0x3e */
void DNG14_chest04_loot(struct struct_chest*);
/* 0x43 */
void DNG14_chest05_loot(struct struct_chest*);
/* 0x48 */
void DNG14_chest06_loot(struct struct_chest*);
/* 0x4d */
void DNG14_chest07_loot(struct struct_chest*);
/* 0x52 */
void DNG14_chest07_trap(void);
/* 0x57 */
void DNG15_riddle(void);

#if !defined(__BORLANDC__)
}
#endif
