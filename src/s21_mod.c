#include "utils.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = (result) ? 0 : 4;
  if (iszero(value_2))
    overflow = 3;
  else if (!overflow) {
    int sign = isminus(value_1);
    if (s21_is_less(value_1, value_2))
      *result = value_1;
    else {
      work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                   res = initwork();
      if (v_1.exp < v_2.exp) {
        for (uint16_t i = 0; i < v_2.exp - v_1.exp; ++i) bits10up(&v_1);
      } else {
        for (uint16_t i = 0; i < v_1.exp - v_2.exp; ++i) bits10up(&v_2);
      }
      v_1.exp = v_2.exp;
      if (compearbits(v_1, shiftleft(v_2, 96)) < 0) res = divremain(v_1, v_2);
      overflow = normalize(&res);
      *result = convert2s21(res, sign);
    }
  }
  return overflow;
}
