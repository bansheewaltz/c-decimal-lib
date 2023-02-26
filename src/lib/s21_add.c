#include "s21_utils.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = (result) ? 0 : 4;
  if ((iszero(value_1) || iszero(value_2)) && !overflow)
    *result = (iszero(value_1)) ? s21normal(value_2) : s21normal(value_1);
  else if (!overflow) {
    int sign_1 = isminus(value_1), sign_2 = isminus(value_2);
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                 res = initwork();
    overflow = pointequalize(&v_1, &v_2);
    if (sign_1 == sign_2)
      res = addbits(v_1, v_2);
    else {
      int who_grate = compearbits(v_1, v_2);
      if (who_grate == 1)
        res = subbits(v_1, v_2);
      else if (who_grate == -1) {
        sign_1 = sign_2;
        res = subbits(v_2, v_1);
      }
    }
    overflow = normalize(&res);
    if (overflow)
      overflow = (sign_1) ? 2 : 1;
    else
      *result = convert2s21(res, sign_1);
  }
  return overflow;
}
