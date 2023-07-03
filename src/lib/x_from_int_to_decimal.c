#include "x_utils.h"

int x_from_int_to_decimal(int src, x_decimal *dst) {
  int error = (dst) ? 0 : 1;
  if (!error)
    *dst = (src >= 0) ? set21(0, 0, 0, src) : set21(MINUS, 0, 0, -src);
  return error;
}
