#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short calc_beeline(signed short, signed short, signed short, signed short);
/* 0x25 */
signed int FIG_get_range_weapon_type(struct struct_hero*);
/* 0x2a */
void fill_enemy_sheet(signed short, signed char, signed char);
/* 0x2f */
signed int place_obj_on_cb(const signed int, const signed int, const signed int, const signed int, const signed char);
/* 0x34 */
void FIG_load_enemy_sprites(struct enemy_sheet*, signed short, signed short);
/* 0x39 */
void FIG_init_enemies(void);
/* 0x3e */
void FIG_init_heroes(void);

#if !defined(__BORLANDC__)
}
#endif
