/* 0x20 */
/* should be static */
void prepare_enemy_ani(struct enemy_sheet*, const signed int);

/* 0x25 */
/* should be static */
signed int FIG_enemy_can_attack_neighbour(const signed int, const signed int, const signed int, const signed int, const signed int);

/* 0x2a */
/* should be static */
signed int FIG_search_range_target(const signed int, const signed int, const signed int, const signed int);

/* 0x2f */
/* should be static */
signed int FIG_get_mspell(const signed int, const signed int);

/* 0x34 */
/* should be static */
signed int FIG_select_mspell(struct enemy_sheet*,const signed int, const signed int, signed int, signed int);

/* 0x39 */
/* should be static */
signed int FIG_enemy_range_attack(struct enemy_sheet*, const signed int, const signed int, signed int, signed int);

/* 0x3e */
void FIG_enemy_turn(struct enemy_sheet*, const signed int, signed int, signed int);
