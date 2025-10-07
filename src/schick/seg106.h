#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void move_item(signed int, signed int, struct struct_hero*);
/* 0x25 */
void print_item_description(struct struct_hero*, const signed int);
/* 0x2a */
void pass_item(struct struct_hero*, const signed int, struct struct_hero*, const signed int);
/* 0x2f */
void startup_equipment(struct struct_hero*);
/* 0x34 */
signed int two_hand_collision(struct struct_hero*, const signed int, const signed int);
/* 0x39 */
signed short get_max_light_time(void);
/* 0x3e */
void equip_belt_ani(void);
/* 0x43 */
signed short get_full_waterskin_pos(const struct struct_hero*);

#if !defined(__BORLANDC__)
}
#endif
