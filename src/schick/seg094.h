#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
void TM_func8(signed short);
/* 0x2a */
void TM_func9(void);
/* 0x2a */
void TM_func1(signed short, signed short);
/* 0x2f */
void prepare_map_marker(void);
/* 0x34 */
/* static */
void set_textbox_positions(signed short);
/* 0x39 */
#if defined(__BORLANDC__)
signed short TM_unused1(struct trv_start_point*, signed short);
#endif
/* 0x3e */
signed short TM_enter_target_town_viewdir(signed short);

/* 0x43 */
signed short TM_get_track_length(Bit8u*);
/* 0x48 */
signed short TM_enter_target_town(void);
/* 0x4d */
void TM_draw_track(signed short, signed short, signed short, signed short);
/* 0x52 */
#if defined(__BORLANDC__)
void TM_unused2(void);
#endif

#if !defined(__BORLANDC__)
}
#endif
