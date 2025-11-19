#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed int FIG_search_obj_on_cb(const signed int, signed int*, signed int*);
/* 0x25 */
void FIG_init_list_elem(const signed int);
/* 0x2a */
#if defined(__BORLANDC__)
/* should be static */
void FIG_unused(const signed int, const signed int, signed int*, signed int*);
#endif
/* 0x2f */
/* should be static */
void FIG_find_path_to_target_backtrack(int8_t*, signed int, signed int, signed int, const signed char, const signed int, const signed int, const signed int);
/* 0x34 */
/* should be static */
signed int FIG_count_direction_changes_of_path(const int8_t*);
/* 0x39 */
signed int FIG_find_path_to_target(uint8_t *, const signed int, const signed int, const signed int, const signed int);

#if !defined(__BORLANDC__)
}
#endif
