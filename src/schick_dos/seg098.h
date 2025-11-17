#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed int use_magic(struct struct_hero*);
/* 0x25 */
signed int test_spell(struct struct_hero*, const signed int, signed char);
/* 0x2a */
signed int use_spell(struct struct_hero*, const signed int, signed char);
/* 0x2f */
signed int select_spell(struct struct_hero*, signed int);
/* 0x34 */
/* static */
void magic_heal_ani(const struct struct_hero*);
/* 0x39 */
void FIG_do_spell_damage(const signed int);
/* 0x3e */
signed int get_attackee_parade(void);
/* 0x43 */
signed int get_attackee_rs(void);
/* 0x48 */
signed int get_spell_cost(const signed int, const signed int);
/* 0x4d */
/* can be static */
signed int can_use_spellclass(const struct struct_hero*, const signed int);
/* 0x52 */
signed int test_spell_group(const signed int, const signed char);
/* 0x57 */
signed int select_magic_user(void);

#if !defined(__BORLANDC__)
}
#endif
