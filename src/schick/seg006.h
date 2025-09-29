#if !defined(__BORLANDC__)
namespace M302de {
#endif

struct struct_fighter* FIG_get_fighter(signed char);

void FIG_draw_figures(void);

void FIG_set_gfx(void);

void FIG_call_draw_pic(void);

void FIG_draw_pic(void);

struct struct_hero* FIG_get_hero_ptr(const signed short);

struct enemy_sheet* FIG_get_enemy_sheet(const signed short);

void FIG_set_sheet(signed char, signed char);

void FIG_make_invisible(signed char);

void FIG_make_visible(signed short);

void FIG_set_weapon_sheet(signed char, signed char);

void FIG_remove_from_list(signed char, signed char);

signed char FIG_add_to_list(signed char);

void FIG_draw_char_pic(signed short, signed short);

void FIG_draw_enemy_pic(signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
