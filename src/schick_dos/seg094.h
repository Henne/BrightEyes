#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void TM_func8(const signed int);
/* 0x2a */
void TM_func9(void);
/* 0x2a */
void TM_func1(const signed int, const signed int);
/* 0x2f */
void prepare_map_marker(void);
/* 0x34 */
/* static */
void set_textbox_positions(const signed int);
/* 0x39 */
#if defined(__BORLANDC__)
signed int TM_unused1(struct trv_start_point*, const signed int);
#endif
/* 0x3e */
signed int TM_enter_target_town_viewdir(const signed int);

/* 0x43 */
signed int TM_get_track_length(struct struct_point*);
/* 0x48 */
signed int TM_enter_target_town(void);
/* 0x4d */
void TM_draw_track(const signed int, const signed int, const signed int, const signed int);
/* 0x52 */
#if defined(__BORLANDC__)
void TM_unused2(void);
#endif

#if !defined(__BORLANDC__)
}
#endif
