#include "../utils.h"
#include "math.h"

int x_from_decimal_to_float(x_decimal src, float *dst) {
  if (iszero(src))
    *dst = 0;
  else {
    int sign = (isminus(src)) ? -1 : 1;
    int exp = (src.bits[3] & NOTMINUS) >> 16;
    uint32_t value[3];
    for (uint16_t i = 0; i < 3; ++i) value[i] = (unsigned int)src.bits[i];
    *dst = (pow(2, 64) * value[2] + pow(2, 32) * value[1] + value[0]) *
           pow(10, -exp) * sign;
  }
  return 0;
}
