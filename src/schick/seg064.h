#if !defined(__BORLANDC__)
namespace M302de {
#endif

/*0x20*/
char* print_passage_price(signed short, Bit8u*);
/*0x25*/
Bit8u* get_ship_name(signed char, signed short);
/*0x2a*/
unsigned short prepare_passages(void);
/*0x2f*/
unsigned short get_passage_travel_hours(signed short, signed short);
/*0x34*/
unsigned short get_next_passages(unsigned short);
/*0x39*/
unsigned short passage_arrival(void);

#if !defined(__BORLANDC__)
}
#endif
