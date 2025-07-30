#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void do_talk(signed short, signed short);
/* 0x25 */
void print_date(void);
/* 0x2a */
void prepare_date_str(void);
/* 0x2f */
void talk_switch(void);
/* 0x34 */
char* get_random_unknown_informer_name(const int);
/* 0x39 */
char* get_random_known_informer_name(const int);

#if !defined(__BORLANDC__)
}
#endif
