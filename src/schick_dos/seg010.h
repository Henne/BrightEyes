#if defined(__BORLANDC__)
extern "C" {
#endif

signed int EMS_get_num_pages_unalloced(void);
signed int EMS_alloc_pages(const int16_t);
int16_t EMS_free_pages(const int16_t);
int16_t EMS_map_memory(const int16_t, const int16_t, const int16_t);
uint8_t* EMS_norm_ptr(uint8_t*);
signed int EMS_init(void);

#if defined(__BORLANDC__)
}
#endif
