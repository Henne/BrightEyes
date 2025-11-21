#ifndef PLATFORM_H
#define PLATFORM_H

#if __cpp_static_assert  
  #include <assert.h>
#define STATIC_ASSERT(expr, msg) static_assert(expr, #msg)
#else
  #define STATIC_ASSERT(expr, msg) typedef char static_assert_##msg[(expr) ? 1 : -1]
#endif

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
#if defined(__BORLANDC__)
typedef unsigned long uint32_t;
typedef signed long int32_t;
typedef uint8_t huge * HugePt;
#else
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef uint8_t* HugePt;
#endif

STATIC_ASSERT(sizeof(int8_t) == 1, int8_needs_to_be_1_byte);
STATIC_ASSERT(sizeof(int16_t) == 2, int16_needs_to_be_2_bytes);
STATIC_ASSERT(sizeof(int32_t) == 4, int32_needs_to_be_4_bytes);

#endif
