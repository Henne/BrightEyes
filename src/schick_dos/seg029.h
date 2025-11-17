#if !defined(__BORLANDC__)
namespace M302de {
#endif

//0x20
//static
void draw_playmask(void);
//0x25
void draw_status_line(void);
//0x2a
void draw_icons(void);
//0x2f
void draw_main_screen(void);
//0x34
void clear_hero_icon(const signed int);
//0x39
void clear_loc_line(void);
//0x3e
void select_hero_icon(const signed int);
//0x43
void deselect_hero_icon(const signed int);
//0x48
void copy_forename(char*, char*);
//0x4d
//static
void load_icon(const signed int, const signed int, const signed int);

#if !defined(__BORLANDC__)
}
#endif
