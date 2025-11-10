#if !defined(__BORLANDC__)
namespace M302de {
#endif

//20
void GUI_write_char_to_screen(uint8_t*, signed short, signed short);
//25
uint8_t* GUI_names_grammar(signed short, signed short, signed short);
//2a
char* GUI_name_plural(signed short, char*);
//2f
char* GUI_name_singular(char*);
//34
uint8_t* GUI_2f2(signed short, signed short, signed short);
//39
char* GUI_get_ptr(const int, const int);
//3e
char* GUI_get_ptr2(signed short, signed short);
//43
void GUI_print_string(char*, signed short, signed short);
//48
signed short GUI_print_char(unsigned char, unsigned short, unsigned short);
//4d
signed short GUI_get_first_pos_centered(char*, signed short, signed short, unsigned short);
//52
unsigned short GUI_count_lines(char*);
//57
signed short GUI_lookup_char_width(signed char, signed short*);
//5c
unsigned short GUI_unused(uint8_t*);
//61
void GUI_write_fonti_to_screen(unsigned short, unsigned short, unsigned short, unsigned short);
//66
void GUI_blank_char(void);
//6B
void GUI_font_to_buf(uint8_t*);
//70
void GUI_write_char_to_screen_xy(unsigned short, unsigned short, unsigned short, unsigned short);
//75
void set_textcolor(const signed int, const signed int);
/* 0x7a */
void get_textcolor(signed int*, signed int*);
//7f
void GUI_print_loc_line(char*);
//84
signed short GUI_print_header(char*);
//89
signed short GUI_get_space_for_string(char*, signed short);
//8e
void GUI_draw_popup_line(signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
