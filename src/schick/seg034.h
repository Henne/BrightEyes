#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void FIG_move_hero(struct struct_hero*, signed short, signed short*, signed short*);

/* 0x25 */
signed short FIG_check_hero_attack(signed short, signed short, signed short, signed short,
                        signed short, signed short, signed short);

/* 0x2a */
signed char FIG_cb_select_target(signed short*, signed short*, const signed short);

/* 0x2f */
void seg034_718(signed short, signed short, signed short*, signed short*, signed short, signed short);

/* 0x34 */
void FIG_latecomers(void);

/* 0x39 */
signed short FIG_move_pathlen(void);

#if !defined(__BORLANDC__)
}
#endif
