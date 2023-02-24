#include "utils.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int overflow = (dst) ? 0 : 1;
  if (!overflow) {
    s21_decimal tranc_src = set21(0, 0, 0, 0);
    s21_truncate(src, &tranc_src);
    overflow =
        (tranc_src.bits[2] || tranc_src.bits[1] || tranc_src.bits[0] > NOTMINUS)
            ? 1
            : 0;
    if (!overflow)
      *dst = (isminus(tranc_src)) ? -tranc_src.bits[0] : tranc_src.bits[0];
  }
  return overflow;
}
