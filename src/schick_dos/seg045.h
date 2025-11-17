#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* should be static */
/* 0x20 */
void FANI_add_shotbolt(const signed int, const signed int, const signed int);
/* 0x25 */
void FANI_remove_shotbolt(void);
/* should be static */
/* 0x2a */
signed int FANI_copy_sequence(int8_t*, int8_t*, const signed char);
/* 0x2f */
signed int FANI_prepare_shotbolt_ani(const signed int, const signed int, const signed int, const signed int, const signed int);

/* should be static */
/* 0x34 */
void FANI_add_spell(const signed int, const signed int, const signed int);
/* 0x39 */
void FANI_remove_spell(void);

/* 0x3e */
void FANI_prepare_hero_spell_ani(const signed int, const struct struct_hero*, const signed int);

/* 0x43 */
void FANI_prepare_enemy_spell_ani(const signed int, const struct enemy_sheet*, const signed int);

#if !defined(__BORLANDC__)
}
#endif
