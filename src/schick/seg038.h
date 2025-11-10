#if !defined(__BORLANDC__)
namespace M302de {
#endif

/* 0x20 */
signed int FIG_search_obj_on_cb(const signed int, signed int*, signed int*);
/* 0x25 */
void FIG_init_list_elem(signed short);
/* 0x2a */
#if defined(__BORLANDC__)
/* static */
void FIG_unused(const signed short, const signed short, signed short*, signed short*);
#endif
/* 0x2f */
/* static */
void FIG_find_path_to_target_backtrack(uint8_t*, signed short, signed short, signed short, signed char, signed short, signed short, signed short);
/* 0x34 */
/* static */
signed short FIG_count_direction_changes_of_path(signed char*);
/* 0x39 */
signed short FIG_find_path_to_target(uint8_t *, signed short, signed short, signed short, signed short);

#if !defined(__BORLANDC__)
}
#endif
