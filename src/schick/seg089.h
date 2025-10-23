#if !defined(__BORLANDC__)
namespace M302de {
#endif

extern struct struct_chest g_dng15_specialchests[33];

/* 0x20 */
void DNG15_debris(signed short);
/* 0x25 */
void DNG15_small_wounds(void);
/* 0x2a */
void DNG15_collapsing_ceiling(Bit8u*);
/* 0x2f */
void DNG15_clear_way(Bit8u*);
/* 0x34 */
void DNG15_cursed_money(Bit8u*);
/* 0x39 */
void DNG15_wounds_and_ladders(char*, signed short, signed short);
/* 0x3e */
signed short DNG15_handler(void);
/* 0x43 */
void DNG15_empty_chest(struct struct_chest*);
/* 0x48 */
void DNG15_rotten_clothes_chest(struct struct_chest*);
/* 0x4d */
void DNG15_smelling_chest(struct struct_chest*);
/* 0x52 */
void DNG15_figures_chest(struct struct_chest*);
/* 0x57 */
void DNG15_cursed_money_chest(struct struct_chest*);


#if !defined(__BORLANDC__)
}
#endif
