#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void do_temple(void);
/* 0x25 */
void char_add(const signed int);
/* 0x29 */
void char_letgo(const signed int);
/* 0x2f */
signed int char_erase(void);
/* 0x34 */
void miracle_heal_hero(signed int, const char*);
/* 0x39 */
void miracle_resurrect(const char*);
/* 0x3e */
void miracle_modify(const unsigned int, const int32_t, const signed int);
/* 0x43 */
void miracle_weapon(const char*, const signed int);

#if !defined(__BORLANDC__)
}
#endif
