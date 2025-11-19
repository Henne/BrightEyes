/* 0x20 */
signed int manhattan_distance(const signed int, const signed int, const signed int, const signed int);
/* 0x25 */
signed int FIG_get_range_weapon_type(const struct struct_hero*);
/* 0x2a */
void fill_enemy_sheet(const signed int, const signed char, const signed char);
/* 0x2f */
signed int place_obj_on_cb(const signed int, const signed int, const signed int, const signed int, const signed char);
/* 0x34 */
void FIG_load_enemy_sprites(struct enemy_sheet*, const signed int, const signed int);
/* 0x39 */
void FIG_init_enemies(void);
/* 0x3e */
void FIG_init_heroes(void);
