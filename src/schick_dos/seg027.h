#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void load_pp20(const signed int);
/* 0x25 */
void load_ani(const signed int);
/* 0x2a */
uint8_t* load_fight_figs(signed int);
/* 0x2f */
void init_common_buffers(void);
/* 0x34 */
void load_scenario(signed int);
/* 0x39 */
signed int count_fight_enemies(signed int);
/* 0x3e */
void read_fight_lst(signed int);
/* 0x43 */
void write_fight_lst(void);

#if !defined(__BORLANDC__)
}
#endif
