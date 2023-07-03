#include "x_decimal.h"
#include "x_utils.h"

int x_from_decimal_to_int(x_decimal src, int *dst) {
  int overflow = (dst) ? 0 : 1;
  if (!overflow) {
    x_decimal tranc_src = set21(0, 0, 0, 0);
    x_truncate(src, &tranc_src);
    uint32_t zerobit = MAX4BIT & tranc_src.bits[0];
    overflow =
        (tranc_src.bits[2] || tranc_src.bits[1] || zerobit > NOTMINUS) ? 1 : 0;
    if (!overflow)
      *dst = (isminus(tranc_src)) ? -tranc_src.bits[0] : tranc_src.bits[0];
  }
  return overflow;
}
