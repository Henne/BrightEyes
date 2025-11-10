#if !defined(__BORLANDC__)
namespace M302de {

void decomp_pp20(uint8_t *dst, uint8_t *src_data, uint8_t *src, uint32_t len);

}

#else
extern "C" {

void decomp_pp20(uint8_t *dst, uint8_t *src_data, uint16_t off, uint16_t seg, uint32_t len);

}
#endif
