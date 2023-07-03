#ifndef x_DECIMAL_H
#define x_DECIMAL_H
#include <limits.h>
#include <stdint.h>

#if UINT_MAX > MAX2BIT
typedef struct {
  int bits[4];
} x_decimal;
#else
typedef struct {
  int_fast32_t bits[4];
} x_decimal;
#endif

#define MAXEXP 28
#define MAX2BIT 0xffff

int x_add(x_decimal value_1, x_decimal value_2, x_decimal *result);
int x_sub(x_decimal value_1, x_decimal value_2, x_decimal *result);
int x_mul(x_decimal value_1, x_decimal value_2, x_decimal *result);
int x_div(x_decimal value_1, x_decimal value_2, x_decimal *result);
int x_mod(x_decimal value_1, x_decimal value_2, x_decimal *result);

int x_is_less(x_decimal value_1, x_decimal value_2);
int x_is_less_or_equal(x_decimal value_1, x_decimal value_2);
int x_is_greater(x_decimal value_1, x_decimal value_2);
int x_is_greater_or_equal(x_decimal value_1, x_decimal value_2);
int x_is_equal(x_decimal value_1, x_decimal value_2);
int x_is_not_equal(x_decimal value_1, x_decimal value_2);

int x_from_int_to_decimal(int src, x_decimal *dst);
int x_from_float_to_decimal(float src, x_decimal *dst);
int x_from_decimal_to_int(x_decimal src, int *dst);
int x_from_decimal_to_float(x_decimal src, float *dst);

int x_floor(x_decimal value, x_decimal *result);
int x_round(x_decimal value, x_decimal *result);
int x_truncate(x_decimal value, x_decimal *result);
int x_negate(x_decimal value, x_decimal *result);

#endif  // x_DECIMAL_H
