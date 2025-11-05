#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void FIG_move_hero(struct struct_hero*, signed short, signed short*, signed short*);
/* 0x25 */
signed int FIG_check_hero_attack(const signed int, const signed int, const signed int, const signed int,
                        const signed int, const signed int, const signed int);
/* 0x2a */
signed char FIG_cb_select_target(signed int*, signed int*, const signed short);

/* should be static */
/* 0x2f */
void FIG_find_latecomer_position(const signed int, const signed int, signed int*, signed int*, const signed int, const signed int);
/* 0x34 */
void FIG_latecomers(void);
/* should be static */
/* 0x39 */
signed int FIG_move_pathlen(void);

#if !defined(__BORLANDC__)
}
#endif
