#include <math.h>
#include <stdio.h>

#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int overflow = (dst) ? 0 : 1;
  if (isnan(src))
    overflow = 1;
  if (!overflow)
    *dst = set21(0, 0, 0, 0);
  if (src && !overflow) {
    int sign = (src < 0) ? 1 : 0;
    src = fabs(src);
    if (src < 1e-28 || src > 7.92281625e28)
      overflow = 1;
    else {
      char foo[15];
      sprintf(foo, "%+.6e", src);
      int val = char2num(foo[1]);
      for (uint16_t i = 3; i < 9; ++i) val = 10 * val + char2num(foo[i]);
      int exp = 0;
      sscanf(foo + 10, "%d", &exp);
      exp -= 6;
      work_decimal value = initwork();
      value.bits[0] = val;
      if (exp > 0) {
        for (uint16_t i = 0; i < exp; ++i) overflow = bits10up(&value);
      } else {
        value.exp = -exp;
        overflow = normalize(&value);
      }
      *dst = convert2s21(value, sign);
    }
  }
  return overflow;
}
