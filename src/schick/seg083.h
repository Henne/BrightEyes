#if !defined(__BORLANDC__)
namespace M302de {
#endif

extern struct struct_chest g_dng08_specialchests[7];

/* 0x20 */
void DNG08_chest01_trap(void);
/* 0x25 */
void DNG08_search_bed(void);
/* 0x2a */
void DNG08_chest00_loot(struct struct_chest*);
/* 0x2f */
void DNG08_chest01_loot(struct struct_chest*);
/* 0x34 */
void DNG08_chest02_loot(struct struct_chest*);
/* 0x39 */
void DNG08_chest02_open(struct struct_chest*);
/* 0x3e */
void DNG08_chest04_loot(struct struct_chest*);
/* 0x43 */
void DNG08_chest05_loot(struct struct_chest*);
/* 0x48 */
void DNG08_chest03_open(struct struct_chest*);
/* 0x4d */
void DNG08_chest04_open(struct struct_chest*);
/* 0x52 */
void DNG08_chest05_open(struct struct_chest*);
/* 0x57 */
signed int DNG08_handler(void);

#if !defined(__BORLANDC__)
}
#endif
