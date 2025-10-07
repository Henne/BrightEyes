#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed int use_magic(struct struct_hero*);
/* 0x25 */
signed short test_spell(struct struct_hero*, signed short, signed char);
/* 0x2a */
signed int use_spell(struct struct_hero*, const signed int, signed char);

/* 0x2f */
signed int select_spell(struct struct_hero*, signed int);

/* 0x34 */
/* static */
void magic_heal_ani(const struct struct_hero*);
/* 0x39 */
void FIG_do_spell_damage(signed short);
/* 0x3e */
signed short get_attackee_parade(void);
/* 0x48 */
signed short get_spell_cost(signed short, signed short);
/* 0x4d */
/* can be static */
signed int can_use_spellclass(const struct struct_hero*, const signed int);
/* 0x52 */
signed short test_spell_group(signed short, signed char);
/* 0x57 */
signed short select_magic_user(void);

#if !defined(__BORLANDC__)
}
#endif
