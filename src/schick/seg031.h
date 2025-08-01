#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
Bit8u* load_current_town_gossip(void);
/* 0x25 */
void do_random_talk(signed short, signed short);
/* 0x2a */
char* get_informer_forename(void);
/* 0x2f */
signed short get_town_lookup_entry(void);
/* 0x34 */
Bit8u* get_informer_hint(void);
/* 0x39 */
Bit8u* get_informer_name(void);
/* 0x3e */
Bit8u* get_informer_name2(void);
/* 0x43 */
char* get_random_tavern_message(void);
/* 0x48 */
void drink_while_drinking(signed short);
/* 0x4d */
void eat_while_drinking(signed short);

#if !defined(__BORLANDC__)
}
#endif
