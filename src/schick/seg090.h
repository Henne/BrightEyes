#if !defined(__BORLANDC__)
namespace M302de {
#endif

extern struct struct_chest g_dng12_specialchests[3];

/* 0x20 */
void DNG_clear_corridor(Bit8s*);
/* 0x25 */
void DNG12_chest00_loot(struct struct_chest*);
/* 0x2a */
signed short DNG12_handler(void);
/* 0x2f */
void DNG12_fight_intro(signed short);

#if !defined(__BORLANDC__)
}
#endif
