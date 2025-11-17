#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void FIG_prepare_hero_ani(struct struct_hero*, const signed int);

/* 0x25 */
/* should be static */
signed int AFIG_change_hero_weapon(struct struct_hero*);

/* 0x2a */
/* should be static */
signed int AFIG_can_attack_neighbour(const signed int, const signed int, const signed int,
					const signed int, const signed int);
/* 0x2f */
/* should be static */
signed int AFIG_search_spell_target(const signed int, const signed int, const signed int, const signed int);

/* 0x34 */
/* should be static */
signed int AFIG_select_range_target(struct struct_hero*, const signed int, const signed int, signed int, signed int);

/* 0x39 */
/* should be static */
signed int AFIG_get_spell(const signed int, const signed int);

/* 0x34 */
/* should be static */
signed int AFIG_select_autospell(struct struct_hero*, const signed int, const signed int, signed int, signed int);

/* 0x43 */
/* should be static */
signed int AFIG_count_heroes(const signed int);

/* 0x48 */
void AFIG_hero_turn(struct struct_hero*, const signed int, signed int, signed int);

#if !defined(__BORLANDC__)
}
#endif
