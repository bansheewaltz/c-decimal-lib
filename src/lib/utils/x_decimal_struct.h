#ifndef x_DECIMAL_STRUCT_H
#define x_DECIMAL_STRUCT_H
#include <limits.h>

#if UINT_MAX > MAX2BIT
typedef struct {
  int bits[4];
} x_decimal;
#else
typedef struct {
  int_fast32_t bits[4];
} x_decimal;
#endif

#endif  // x_DECIMAL_STRUCT_H
