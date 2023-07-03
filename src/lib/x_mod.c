#include "x_utils.h"

int x_mod(x_decimal value_1, x_decimal value_2, x_decimal *result) {
  int overflow = (result) ? 0 : 4;
  if (iszero(value_2) && overflow == 0)
    overflow = 3;
  else if (!overflow) {
    int sign = isminus(value_1);
    value_1 = s21normal(value_1);
    value_2 = s21normal(value_2);
    x_decimal foo = value_1;
    setplus(&foo);
    setplus(&value_2);
    if (x_is_less(foo, value_2))
      *result = value_1;
    else {
      work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                   res = initwork();
      if (v_1.exp < v_2.exp) {
        for (uint16_t i = 0; i < v_2.exp - v_1.exp; ++i) bits10up(&v_1);
        v_1.exp = v_2.exp;
      } else {
        for (uint16_t i = 0; i < v_1.exp - v_2.exp; ++i) bits10up(&v_2);
      }
      res = divremain(v_1, v_2);
      overflow = normalize(&res);
      *result = convert2s21(res, sign);
    }
  }
  return overflow;
}
