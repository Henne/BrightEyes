#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void unequip(Bit8u *hero, unsigned short item, unsigned short pos);
/* 0x25 */
void add_equip_boni(Bit8u*, Bit8u*, signed short, signed short, signed short);
/* 0x2f */
signed short give_hero_new_item(Bit8u *hero, signed short, signed short, signed short);
/* 0x34 */
unsigned short drop_item(Bit8u *, signed short, signed short);
/* 0x39 */
signed short get_item(signed short, signed short, signed short);
/* 0x3e */
unsigned short can_hero_use_item(Bit8u *, unsigned short);
/* 0x43 */
unsigned short can_item_at_pos(unsigned short item, unsigned short pos);
/* 0x48 */
signed short has_hero_stacked(Bit8u *, unsigned short);
/* 0x4d */
/* static */
unsigned short item_pleasing_ingerimm(unsigned short);
/* 0x52 */
signed short select_item_to_drop(Bit8u*);
/* 0x57 */
signed short hero_count_item(Bit8u *, unsigned short);
/* 0x5c */
signed short group_count_item(signed short);
/* 0x61 */
void loose_random_item(Bit8u *, signed short, char*);

#if !defined(__BORLANDC__)
}
#endif
