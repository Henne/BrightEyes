#include "format.h"
#include <stdlib.h>

Color* copy_palette(const Color* src, uint16_t first_color, uint16_t num_colors, int shift) {
    Color* dest = calloc(256, sizeof(Color));
    for (uint16_t i = 0; i < num_colors; i++) {
	dest[i + first_color].r  =  src[i].r << shift;
	dest[i + first_color].g  =  src[i].g << shift;
	dest[i + first_color].b  =  src[i].b << shift;
    }
    return dest;
}
