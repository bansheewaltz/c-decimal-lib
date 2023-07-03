#ifndef S21_DECIMAL_STRUCT_H
#define S21_DECIMAL_STRUCT_H
#include <limits.h>

#if UINT_MAX > MAX2BIT
typedef struct {
  int bits[4];
} s21_decimal;
#else
typedef struct {
  int_fast32_t bits[4];
} s21_decimal;
#endif

#endif  // S21_DECIMAL_STRUCT_H