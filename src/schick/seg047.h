#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void make_valuta_str(char* dst, Bit32s money);
/* 0x25 */
void hero_get_sober(struct struct_hero*);
/* 0x2a */
void hero_get_drunken(struct struct_hero*);
/* 0x2f */
signed short check_hero_KK_unused(const signed short val);
/* 0x34 */
signed short check_heroes_KK(const signed short);
/* 0x39 */
signed short hero_is_diseased(struct struct_hero*);
/* 0x3e */
signed short hero_is_poisoned(struct struct_hero*);
/* 0x43 */
signed short menu_enter_delete(Bit8u*, signed short, signed short);
/* 0x48 */
void update_atpa(struct struct_hero*);
/* 0x4d */
signed short select_hero_from_group(char*);
/* 0x52 */
signed short select_hero_ok(char*);
/* 0x57 */
signed short count_heroes_in_group(void);
/* 0x5c */
signed short get_hero_CH_best(void);
/* 0x61 */
signed short get_hero_KK_best(void);
/* 0x66 */
void hero_gets_poisoned(struct struct_hero*, const signed short);
/* 0x6b */
void hero_gets_diseased(struct struct_hero*, const signed short);
/* 0x70 */
void hero_disease_test(struct struct_hero*, const signed short, const signed short);
/* 0x75 */
signed short select_hero_ok_forced(char*);

#if !defined(__BORLANDC__)
}
#endif
