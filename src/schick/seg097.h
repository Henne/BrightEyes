#if !defined(__BORLANDC__)
namespace M302de {
#endif

//20
#if defined(__BORLANDC__)
void GUI_unused1(char*, signed int, signed int);
#endif

//25
signed int GUI_lookup_char_height(signed char c, signed int *p);
//2a
#if defined(__BORLANDC__)
void GUI_1c2(const int, const int, const int, const int);
#endif
//2f
signed int GUI_enter_text(char*, signed int, signed int, signed int, signed int);
//0x34
signed int GUI_input(char*, signed int);
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
void GUI_draw_radio_bg(signed int,signed int, signed int, signed int);
// 52
//static
void GUI_copy_smth(signed int width, signed int height);
//0x57
void GUI_fill_radio_button(signed int, signed int, signed int);
//0x5c
signed int GUI_dialogbox(unsigned char*, char*, char*, signed int, ...);
//0x61
signed int GUI_menu_input(signed int, signed int, signed int);
//0x66
void GUI_print_fight_intro_msg(const signed int);
//0x6b
void GUI_dialog_na(const signed int, char*);

#if !defined(__BORLANDC__)
}
#endif
