#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void do_merchant(void);
/* 0x25 */
void talk_merchant(void);
/* 0x2a */
void add_item_to_buy_selector(const struct merchant_descr*, const signed int, const signed int);
/* 0x2f */
void TLK_ghandel(const signed int);
/* 0x34 */
void TLK_khandel(const signed int);
/* 0x39 */
void TLK_whandel(const signed int);

#if !defined(__BORLANDC__)
}
#endif
