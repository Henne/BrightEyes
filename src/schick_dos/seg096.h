#if !defined(__BORLANDC__)
namespace M302de {
#endif

//20
void GUI_write_char_to_screen(uint8_t*, const signed int, const signed int);
//25
uint8_t* GUI_names_grammar(int16_t, const signed int, const signed int);
//2a
char* GUI_name_plural(const signed int, char*);
//2f
char* GUI_name_singular(char*);
//34
uint8_t* GUI_2f2(const signed int, const signed int, const signed int);
//39
char* GUI_get_ptr(const signed int, const signed int);
//3e
char* GUI_get_article(const signed int, const signed int);
//43
void GUI_print_string(char*, signed int, signed int);
//48
signed int GUI_print_char(const char, const signed int, const signed int);
//4d
signed int GUI_get_first_pos_centered(char*, const signed int, signed int, const signed int);
//52
signed int GUI_count_lines(char*);
//57
signed int GUI_lookup_char_width(const signed char, signed int*);
//5c
signed int GUI_unused(char*);
//61
void GUI_write_fonti_to_screen(const signed int, const signed int, const signed int, const signed int);
//66
void GUI_blank_char(void);
//6B
void GUI_font_to_buf(uint8_t*);
//70
void GUI_write_char_to_screen_xy(const signed int, const signed int, const signed int, const signed int);
//75
void set_textcolor(const signed int, const signed int);
/* 0x7a */
void get_textcolor(signed int*, signed int*);
//7f
void GUI_print_loc_line(char*);
//84
signed int GUI_print_header(char*);
//89
signed int GUI_get_space_for_string(char*, const signed int);
//8e
void GUI_draw_popup_line(const signed int, const signed int);

#if !defined(__BORLANDC__)
}
#endif
