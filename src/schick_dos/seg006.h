struct struct_fighter* FIG_get_fighter(const signed char);

void FIG_draw_figures(void);

void FIG_set_gfx(void);

void FIG_call_draw_pic(void);

void FIG_draw_pic(void);

struct struct_hero* FIG_get_hero_ptr(const signed int);

struct enemy_sheet* FIG_get_enemy_sheet(const signed int);

void FIG_set_ani_track_id_base(const signed char, const signed char);

void FIG_make_invisible(const signed char);

void FIG_make_visible(const signed int);

void FIG_set_ani_track_id_weapon(const signed char, const signed char);

void FIG_remove_from_list(const signed char, const signed char);

signed char FIG_add_to_list(const signed char);

void FIG_draw_char_pic(const signed int, const signed int);

void FIG_draw_enemy_pic(const signed int, const signed int);
