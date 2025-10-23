#if !defined(__BORLANDC__)
namespace M302de {
#endif

extern struct struct_chest g_dng13_specialchests[3];

/* 0x20 */
void DNG13_unblock_passage(char*, Bit8u*);
/* 0x25 */
void DNG13_collapsing_ceiling(void);
/* 0x2a */
void DNG13_collapsing_ceiling_easy(void);
/* 0x2f */
void DNG13_chest01_open(struct struct_chest*);
/* 0x34 */
void DNG13_chest00_open(struct struct_chest*);
/* 0x39 */
void DNG13_chest01_loot(struct struct_chest*);
/* 0x3e */
void DNG13_chest00_loot(struct struct_chest*);
/* 0x43 */
signed short DNG13_handler(void);
/* 0x48 */
void DNG13_fight_intro(signed short);

#if !defined(__BORLANDC__)
}
#endif
