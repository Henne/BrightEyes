#if !defined(__BORLANDC__)
namespace M302de {
#endif

extern struct struct_chest g_dng02_specialchests[8];

/* 0x20 */
void DNG02_chest00_loot(struct struct_chest*);
/* 0x25 */
void DNG02_chest01_loot(struct struct_chest*);
/* 0x2a */
void DNG02_chest02_loot(struct struct_chest*);
/* 0x2f */
void DNG02_chest03_loot(struct struct_chest*);
/* 0x34 */
void DNG02_chest04_loot(struct struct_chest*);
/* 0x39 */
void DNG02_chest05_loot(struct struct_chest*);
/* 0x3e */
void DNG02_chest06_open(struct struct_chest*);
/* 0x43 */
void DNG02_chest06_loot(struct struct_chest*);
/* 0x48 */
signed int DNG02_handler(void);
/* 0x4d */
void DNG02_fight_intro(const signed int);

#if !defined(__BORLANDC__)
}
#endif
