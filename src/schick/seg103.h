#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short test_skill(struct struct_hero*, const signed int, signed char);

/* 0x25 */
signed short bargain(Bit8u*, signed short, Bit32s, signed short, signed char);

/* 0x2a */
signed short GUI_use_skill(signed short, signed char);

/* 0x2f */
/* can be static */
signed short select_skill(void);

/* 0x34 */
signed short LVL_select_skill(Bit8u *, signed short);

/* 0x39 */
/* can be static */
struct struct_hero* get_proper_hero(const signed int);

/* 0x3e */
/* can be static */
signed short use_skill(signed short, signed char, signed short);

/* 0x43 */
signed short GUI_use_skill2(signed short, char*);

#if !defined(__BORLANDC__)
}
#endif
