#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void delete_chest_item(struct struct_chest*, signed int);

/* 0x25 */
void chest_protected_normal(void);

/* 0x2a */
void chest_closed(void);

/* 0x2f */
void chest_protected_heavy(void);

/* 0x34 */
void chest_cursed(void);

/* 0x39 */
void chest_fulminictus(void);

/* 0x3e */
void loot_simple_chest(struct struct_chest*);

/* 0x43 */
void loot_chest(struct struct_chest*, char*, char*);

/* 0x48 */
void loot_special_chest(const signed int);

/* 0x4d */
void use_lockpicks_on_chest(struct struct_chest*);

/* 0x52 */
void use_key_on_chest(struct struct_chest*);

/* 0x57 */
void loot_multi_chest(uint8_t*, char*);

/* 0x5c */
void chest_poisoned1(void);

/* 0x61 */
void chest_poisoned2(void);

/* 0x66 */
void chest_poisoned3(void);

/* 0x6b */
void chest_protected_brutal(void);

/* 0x70 */
void chest_petrified(void);

/* 0x75 */
void chest_ignifax_normal(void);

/* 0x7a */
void chest_ignifax_brutal(void);

/* 0x7a */
void chest_ignifax_heavy(void);

/* 0x82 */
void chest_crossbow_bolts(void);

/* 0x89 */
signed int hero_has_lockpicks(const struct struct_hero*);

#if !defined(__BORLANDC__)
}
#endif
