/* 0x20 */
signed int do_alchemy(struct struct_hero*, const signed int, const signed int);

/* 0x25 */
/* should be static */
signed int hero_has_ingrendients(struct struct_hero*, const signed int);

/* 0x2a */
/* static */
void hero_use_ingrendients(struct struct_hero*, const signed int);

/* 0x2f */
signed int plan_alchemy(struct struct_hero*);

/* 0x34 */
signed int has_herb_for_disease(const struct struct_hero*, const signed int);

/* 0x39 */
signed int skill_cure_disease(struct struct_hero*, struct struct_hero*, const signed int, const signed int);

/* 0x3e */
struct struct_hero* get_heaviest_hero(void);

/* 0x43 */
signed int get_hero_weight(const struct struct_hero*);

/* 0x48 */
signed int get_skilled_hero_pos(const signed int);
