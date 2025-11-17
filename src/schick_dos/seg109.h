#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed int TRV_found_camp_place(const signed int);
/* 0x25 */
void TRV_found_replenish_place(const signed int);
/* 0x2a */
signed int TRV_follow_trail_question(void);
/* 0x2f */
void TRV_found_herb_place(const signed int);
/* 0x34 */
void TRV_inside_herb_place(void);
/* 0x39 */
signed int TRV_cross_a_ford(char*, const signed int, const signed int);
/* 0x3e */
void TRV_ford_test(const signed int, const signed int);
/* 0x43 */
signed int TRV_ferry(char*, signed int);
/* 0x48 */
void TRV_barrier(const signed int);
/* 0x4d */
void TRV_hunt_generic(const signed int, const signed int, const signed int,
		const signed int, const signed int, const signed int,
		const signed int, const signed int, const signed int,
		const signed int, const signed int);
/* 0x52 */
void tevent_001(void);
/* 0x57 */
void tevent_002(void);
/* 0x5c */
void tevent_003(void);
/* 0x61 */
void tevent_004(void);
/* 0x66 */
void tevent_005(void);
/* 0x6b */
void tevent_006(void);
/* 0x70 */
void tevent_007(void);
/* 0x75 */
void tevent_008(void);
/* 0x7a */
void tevent_009(void);
/* 0x7f */
void tevent_010(void);
/* 0x84 */
void tevent_012(void);
/* 0x89 */
void tevent_018(void);
/* 0x8e */
void tevent_019(void);
/* 0x93 */
void tevent_023(void);
/* 0x98 */
void tevent_092(void);
/* 0x9d */
void TRV_load_textfile(signed int);
/* 0xa2 */
void TRV_event(const signed int);
/* 0xa7 */
signed int TRV_fight_event(const signed int, const signed int);
/* 0xac */
void TRV_found_inn(const signed int, const signed int);
/* 0xb1 */
signed int TRV_enter_hut_question(void);

#if !defined(__BORLANDC__)
}
#endif
