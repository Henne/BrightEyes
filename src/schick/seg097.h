#if !defined(__BORLANDC__)
namespace M302de {
#endif

//20
#if defined(__BORLANDC__)
void GUI_unused1(Bit8u*, signed short, signed short);
#endif

//25
signed short GUI_lookup_char_height(signed char c, signed short *p);
//2a
#if defined(__BORLANDC__)
void GUI_1c2(signed short, signed short, Bit8u*);
#endif
//2f
signed short GUI_enter_text(char*, signed short, signed short, signed short, signed short);
//0x34
signed short GUI_input(char*, unsigned short);
//0x39
void GUI_output(char*);
//0x3e
signed short GUI_bool(char*);
//0x43
signed short GUI_radio(char*, signed char, ...);
//0x48
#if defined(__BORLANDC__)
signed short GUI_unused2(signed short, Bit8u*);
#endif
//4d
void GUI_draw_radio_bg(signed short,signed short, signed short, signed short);
// 52
//static
void GUI_copy_smth(unsigned short width, unsigned short height);
//0x57
void GUI_fill_radio_button(signed short, unsigned short, unsigned short);
//0x5c
signed short GUI_dialogbox(unsigned char*, char*, char*, signed short, ...);
//0x61
signed short GUI_menu_input(signed short, signed short, signed short);
//0x66
void GUI_print_fight_intro_msg(signed short);
//0x6b
void GUI_dialog_na(unsigned short, char*);

#if !defined(__BORLANDC__)
}
#endif
