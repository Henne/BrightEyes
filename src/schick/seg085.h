#if !defined(__BORLANDC__)
namespace M302de {
#endif

extern struct struct_chest g_dng10_specialchests[6];

/* 0x20 */
void DNG10_chest00_loot(struct struct_chest*);
/* 0x25 */
void DNG10_chest00_open(struct struct_chest*);
/* 0x2a */
void DNG10_chest01_loot(struct struct_chest*);
/* 0x2f */
void DNG10_chest02_loot(struct struct_chest*);
/* 0x34 */
void DNG10_chest03_loot(struct struct_chest*);
/* 0x39 */
void DNG10_chest04_loot(struct struct_chest*);
/* 0x3e */
signed short DNG10_handler(void);

#if !defined(__BORLANDC__)
}
#endif
