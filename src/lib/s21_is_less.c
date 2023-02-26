#include "s21_utils.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  if (iszero(value_1)) {
    if (!iszero(value_2)) res = (isminus(value_2)) ? 0 : 1;
  } else if (iszero(value_2))
    res = (isminus(value_1)) ? 1 : 0;
  else {
    int sign_1 = isminus(value_1), sign_2 = isminus(value_2);
    if (sign_1 != sign_2)
      res = (sign_1 > sign_2) ? 1 : 0;
    else {
      work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2);
      pointequalize(&v_1, &v_2);
      int who_grate = compearbits(v_1, v_2);
      if (who_grate == 1)
        res = (sign_1) ? 1 : 0;
      else if (who_grate == -1)
        res = (sign_1) ? 0 : 1;
    }
  }
  return res;
}
