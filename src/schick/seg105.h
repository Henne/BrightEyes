#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void unequip(struct struct_hero*, const signed int, const signed int);
/* 0x25 */
void add_equip_boni(struct struct_hero*, struct struct_hero*, const signed int, const signed int, const signed int);
/* 0x2a */
signed int has_hero_equipped(struct struct_hero*, const signed int);
/* 0x2f */
signed int give_hero_new_item(struct struct_hero*, const signed int, const signed int, const signed int);
/* 0x34 */
signed int drop_item(struct struct_hero*, const signed int, signed int);
/* 0x39 */
signed int get_item(signed int, const signed int, signed int);
/* 0x3e */
signed int can_hero_use_item(const struct struct_hero*, const signed int);
/* 0x43 */
signed int can_item_at_pos(const signed int, const signed int);
/* 0x48 */
signed int has_hero_stacked(struct struct_hero*, const signed int);
/* 0x4d */
/* static */
signed int item_pleasing_ingerimm(const signed int);
/* 0x52 */
signed int select_item_to_drop(struct struct_hero*);
/* 0x57 */
signed int hero_count_item(struct struct_hero*, const signed int);
/* 0x5c */
signed int group_count_item(const signed int);
/* 0x61 */
void loose_random_item(struct struct_hero*, const signed int, char*);

#if !defined(__BORLANDC__)
}
#endif
