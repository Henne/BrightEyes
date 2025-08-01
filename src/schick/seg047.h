#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void make_valuta_str(char* dst, Bit32s money);
/* 0x25 */
void hero_get_sober(Bit8u *hero);
/* 0x2a */
void hero_get_drunken(Bit8u *hero);
/* 0x2f */
signed short check_hero_KK_unused(signed short val);
/* 0x34 */
signed short check_heroes_KK(signed short val);
/* 0x39 */
unsigned short hero_is_diseased(Bit8u *hero);
/* 0x3e */
unsigned short hero_is_poisoned(Bit8u *hero);
/* 0x43 */
signed short menu_enter_delete(Bit8u*, signed short, signed short);
/* 0x48 */
void update_atpa(Bit8u*);
/* 0x4d */
signed short select_hero_from_group(char*);
/* 0x52 */
signed short select_hero_ok(char*);
/* 0x57 */
signed short count_heroes_in_group(void);
/* 0x5c */
unsigned short get_hero_CH_best(void);
/* 0x61 */
unsigned short get_hero_KK_best(void);
/* 0x66 */
void hero_gets_poisoned(Bit8u *hero, unsigned short poison);
/* 0x6b */
void hero_gets_diseased(Bit8u *hero, unsigned short diseased);
/* 0x70 */
void hero_disease_test(Bit8u *hero, unsigned short disease, signed short probability);
/* 0x75 */
signed short select_hero_ok_forced(char*);

#if !defined(__BORLANDC__)
}
#endif
