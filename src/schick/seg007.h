#if !defined(__BORLANDC__)
namespace M302de {
#endif

int random_interval(const signed int, const signed int);
int random_schick(const signed int);
int dice_roll(const int, const int, const int);
void calc_damage_range(const signed int, const signed int, const signed int, signed int*, signed int*);
int is_in_word_array(const signed int, int16_t*);
int is_in_byte_array(const int8_t, int8_t*);
int dice_template(const uint16_t);
void damage_range_template(const uint16_t, signed int*, signed int*);

#if !defined(__BORLANDC__)
}
#endif
