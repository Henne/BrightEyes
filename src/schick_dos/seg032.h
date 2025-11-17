#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
signed int do_fight(const signed int);
//0x25
void FIG_set_cb_object(const signed int, const signed int, const signed int);
//0x2a
void draw_fight_screen_pal(const signed int);
//0x2f
signed int FIG_choose_next_hero(void);
//0x34
signed int FIG_choose_next_enemy(void);
//0x39
signed int FIG_count_active_enemies(void);
//0x3e
signed int FIG_is_enemy_active(const struct enemy_sheet*);
//0x43
signed int FIG_get_first_active_hero(void);
//0x48
signed int FIG_all_heroes_escaped(void);
//0x4d
signed int FIG_fight_continues(void);
//0x52
void FIG_do_round(void);
//0x57
void FIG_load_ship_sprites(void);

#if !defined(__BORLANDC__)
}
#endif
