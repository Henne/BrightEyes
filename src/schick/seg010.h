#if !defined(__BORLANDC__)
namespace M302de {
#else
extern "C" {
#endif

signed short EMS_get_num_pages_unalloced(void);
unsigned short EMS_alloc_pages(unsigned short);
unsigned short EMS_free_pages(unsigned short);
unsigned short EMS_map_memory(unsigned short, unsigned short, unsigned char);
Bit8u* EMS_norm_ptr(Bit8u*);
unsigned short EMS_init();

#if !defined(__BORLANDC__)
}
#else
}
#endif
