#include <math.h>

#include "s21_utils.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int error = (dst) ? 0 : 1;
  if (iszero(src) && error == 0)
    *dst = 0;
  else if (!error) {
    int sign = (isminus(src)) ? -1 : 1;
    int exp = (src.bits[3] & NOTMINUS) >> 16;
    uint32_t value[3];
    for (uint16_t i = 0; i < 3; ++i) value[i] = (unsigned int)src.bits[i];
    *dst = (pow(2, 64) * value[2] + pow(2, 32) * value[1] + value[0]) *
           pow(10, -exp) * sign;
  }
  return error;
}
