#if !defined(__BORLANDC__)
namespace M302de {
#endif

//20
#if defined(__BORLANDC__)
void GUI_unused1(char*, signed int, signed int);
#endif

//25
signed int GUI_lookup_char_height(const signed char c, signed int *p);
//2a
#if defined(__BORLANDC__)
void GUI_1c2(const signed int, const signed int, const signed int, const signed int);
#endif
//2f
signed int GUI_enter_text(char*, signed int, signed int, signed int, signed int);
//0x34
signed int GUI_input(char*, const signed int);
//0x39
void GUI_output(char*);
//0x3e
signed int GUI_bool(char*);
//0x43
signed int GUI_radio(char*, signed char, ...);
//0x48
#if defined(__BORLANDC__)
signed int GUI_unused2(const int, const int, const int);
#endif
//4d
/* should be static */
void GUI_draw_radio_bg(const signed int, const signed int, const signed int, const signed int);
// 52
/* should be static */
void GUI_copy_smth(const signed int, const signed int);
//0x57
/* should be static */
void GUI_fill_radio_button(const signed int, const signed int, const signed int);
//0x5c
signed int GUI_dialogbox(unsigned char*, char*, char*, signed int, ...);
//0x61
/* should be static */
signed int GUI_menu_input(const signed int, const signed int, const signed int);
//0x66
void GUI_print_fight_intro_msg(const signed int);
//0x6b
void GUI_dialog_na(const signed int, char*);

#if !defined(__BORLANDC__)
}
#endif
