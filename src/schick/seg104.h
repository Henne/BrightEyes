#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed short do_alchemy(Bit8u*, signed short, signed short);

/* 0x25 */
/* static */
signed short hero_has_ingrendients(Bit8u*, signed short);

/* 0x2a */
/* static */
void hero_use_ingrendients(Bit8u*, signed short);

/* 0x2f */
signed short plan_alchemy(Bit8u*);

/* 0x34 */
signed short has_herb_for_disease(Bit8u*, signed short);

/* 0x39 */
signed short skill_cure_disease(Bit8u*, Bit8u*, signed short, signed short);

/* 0x3e */
Bit8u* get_heaviest_hero(void);

/* 0x43 */
signed short get_hero_weight(Bit8u*);

/* 0x48 */
signed short get_skilled_hero_pos(signed short);

#if !defined(__BORLANDC__)
}
#endif
