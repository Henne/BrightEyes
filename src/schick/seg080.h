#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void DNG04_chest00_loot(Bit8u*);
/* 0x25 */
void DNG04_chest00_open(struct struct_chest*);
/* 0x2a */
void DNG04_chest01_loot(Bit8u*);
/* 0x2f */
void DNG04_chest01_open(struct struct_chest*);
/* 0x34 */
void DNG04_chest02_loot(Bit8u*);
/* 0x39 */
void DNG04_chest02_open(struct struct_chest*);
/* 0x3e */
signed short DNG04_handler(void);
/* 0x43 */
signed short DNG05_handler(void);
/* 0x48 */
void DNG5_fight_intro(signed short);

#if !defined(__BORLANDC__)
}
#endif
