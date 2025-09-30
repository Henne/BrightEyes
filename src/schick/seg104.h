#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short do_alchemy(struct struct_hero*, const signed short, const signed short);

/* 0x25 */
/* static */
signed short hero_has_ingrendients(struct struct_hero*, const signed short);

/* 0x2a */
/* static */
void hero_use_ingrendients(struct struct_hero*, const signed short);

/* 0x2f */
signed short plan_alchemy(struct struct_hero*);

/* 0x34 */
signed short has_herb_for_disease(struct struct_hero*, const signed short);

/* 0x39 */
signed short skill_cure_disease(struct struct_hero*, struct struct_hero*, const signed short, const signed short);

/* 0x3e */
struct struct_hero* get_heaviest_hero(void);

/* 0x43 */
signed short get_hero_weight(struct struct_hero*);

/* 0x48 */
signed short get_skilled_hero_pos(const signed short);

#if !defined(__BORLANDC__)
}
#endif
