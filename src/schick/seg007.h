#if !defined(__BORLANDC__)
namespace M302de {
#endif

int random_interval(const int, const int);
int random_schick(const int);
int dice_roll(const int, const int, const int);
void calc_damage_range(const int, const int, const int, signed short*, signed short*);
int is_in_word_array(const int, signed short*);
int is_in_byte_array(const signed char, Bit8s*);
int dice_template(const unsigned short);
void damage_range_template(unsigned short val, signed short*, signed short*);

#if !defined(__BORLANDC__)
}
#endif
