#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void DNG08_chest01_trap(void);
/* 0x25 */
void DNG08_search_bed(void);
/* 0x2a */
void DNG08_chest00_loot(Bit8u*);
/* 0x2f */
void DNG08_chest01_loot(Bit8u*);
/* 0x34 */
void DNG08_chest02_loot(Bit8u*);
/* 0x39 */
void DNG08_chest02_open(struct struct_chest*);
/* 0x3e */
void DNG08_chest04_loot(Bit8u*);
/* 0x43 */
void DNG08_chest05_loot(Bit8u*);
/* 0x48 */
void DNG08_chest03_open(struct struct_chest*);
/* 0x4d */
void DNG08_chest04_open(struct struct_chest*);
/* 0x52 */
void DNG08_chest05_open(struct struct_chest*);
/* 0x57 */
signed short DNG08_handler(void);

#if !defined(__BORLANDC__)
}
#endif
