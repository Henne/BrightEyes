/* 0x20 */
void make_valuta_str(char* dst, int32_t money);
/* 0x25 */
void hero_get_sober(struct struct_hero*);
/* 0x2a */
void hero_get_drunken(struct struct_hero*);
/* 0x2f */
signed int check_hero_KK_unused(const signed int);
/* 0x34 */
signed int check_heroes_KK(const signed int);
/* 0x39 */
signed int hero_is_diseased(const struct struct_hero*);
/* 0x3e */
signed int hero_is_poisoned(const struct struct_hero*);
/* 0x43 */
signed int menu_enter_delete(uint8_t*, const signed int, const signed int);
/* 0x48 */
void update_atpa(struct struct_hero*);
/* 0x4d */
signed int select_hero_from_group(char*);
/* 0x52 */
signed int select_hero_ok(char*);
/* 0x57 */
signed int count_heroes_in_group(void);
/* 0x5c */
signed int get_hero_CH_best(void);
/* 0x61 */
signed int get_hero_KK_best(void);
/* 0x66 */
void hero_gets_poisoned(struct struct_hero*, const signed int);
/* 0x6b */
void hero_gets_diseased(struct struct_hero*, const signed int);
/* 0x70 */
void hero_disease_test(struct struct_hero*, const signed int, const signed int);
/* 0x75 */
signed int select_hero_ok_forced(char*);
