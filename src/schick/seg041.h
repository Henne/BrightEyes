#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed int range_attack_check_ammo(struct struct_hero*, const signed int);
//0x25
void FIG_output(char*);
//0x2a
void FIG_clear_msgs();
//0x2f
void FIG_add_msg(const signed int, const signed int);
//0x34
void FIG_damage_enemy(struct enemy_sheet*, Bit16s, signed short);
/* 0x39 */
signed int FIG_get_hero_weapon_attack_damage(struct struct_hero*, struct struct_hero*, const signed int);
//0x3e
signed int weapon_check(struct struct_hero*);
//0x43
signed short FIG_get_enemy_attack_damage(struct enemy_sheet*, struct enemy_sheet*, signed short);
//0x48
void clear_anisheets();

#if !defined(__BORLANDC__)
}
#endif
