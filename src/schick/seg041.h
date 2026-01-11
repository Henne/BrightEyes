/* 0x20 */
signed int range_attack_check_ammo(struct struct_hero*, const signed int);
//0x25
void FIG_output(char*);
//0x2a
void FIG_clear_msgs(void);
//0x2f
void FIG_add_msg(const signed int, const signed int);
//0x34
void FIG_damage_enemy(struct enemy_sheet*, const signed int, const signed int);
/* 0x39 */
signed int FIG_get_hero_weapon_attack_damage(struct struct_hero*, struct struct_hero*, const signed int);
//0x3e
signed int FIG_weapon_gfx_id_melee(const struct struct_hero*);
//0x43
signed int FIG_get_enemy_attack_damage(const struct enemy_sheet*, struct enemy_sheet*, const signed int);
//0x48
void FANI_clear_ani_tracks(void);
