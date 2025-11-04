#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void FANI_prepare_fight_hero_ani(signed short, struct struct_hero*, signed short, signed short, signed short, signed short, signed short);

/* 0x25 */
void FANI_prepare_fight_enemy_ani(signed short, struct enemy_sheet*, signed short, signed short, signed short, signed short);

/* 0x2a */
void FANI_prepare_spell_hero(Bit16u, struct struct_hero*, Bit16u, Bit16s, Bit16s, Bit16u, Bit16u);

/* 0x2f */
void FANI_prepare_spell_enemy(signed short, struct enemy_sheet*, signed short, signed short, signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
