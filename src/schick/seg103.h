/* 0x20 */
signed int test_talent(const struct struct_hero*, const signed int, signed char);

/* 0x25 */
signed int bargain(const struct struct_hero*, const signed int, const int32_t, const signed int, const signed char);

/* 0x2a */
signed int GUI_use_talent(const signed int, const signed char);

/* 0x2f */
/* can be static */
signed int select_talent(void);

/* 0x34 */
signed int LVL_select_talent(const struct struct_hero*, const signed int);

/* 0x39 */
/* can be static */
struct struct_hero* get_proper_hero(const signed int);

/* 0x3e */
/* can be static */
signed int use_talent(const signed int, signed char, const signed int);

/* 0x43 */
signed int GUI_use_talent2(const signed int, char*);
