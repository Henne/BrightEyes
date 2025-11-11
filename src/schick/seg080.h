#if !defined(__BORLANDC__)
namespace M302de {
#endif

extern struct struct_chest g_dng04_specialchests[4];

/* 0x20 */
void DNG04_chest00_loot(struct struct_chest*);
/* 0x25 */
void DNG04_chest00_open(struct struct_chest*);
/* 0x2a */
void DNG04_chest01_loot(struct struct_chest*);
/* 0x2f */
void DNG04_chest01_open(struct struct_chest*);
/* 0x34 */
void DNG04_chest02_loot(struct struct_chest*);
/* 0x39 */
void DNG04_chest02_open(struct struct_chest*);
/* 0x3e */
signed short DNG04_handler(void);
/* 0x43 */
signed short DNG05_handler(void);
/* 0x48 */
void DNG05_fight_intro(const signed int);

#if !defined(__BORLANDC__)
}
#endif
