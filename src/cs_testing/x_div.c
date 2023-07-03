#include "../utils.h"

int x_div(x_decimal value_1, x_decimal value_2, x_decimal *result) {
  int overflow = 0;
  int sign = (isminus(value_1) == isminus(value_2)) ? 0 : 1;
  if (iszero(value_2))
    overflow = 3;
  else if (iszero(value_1))
    *result = set21(MINUS * sign, 0, 0, 0);
  else {
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                 res = initwork();
    if (v_1.exp < v_2.exp) {
      for (uint16_t i = 0; i < v_2.exp - v_1.exp; ++i) bits10up(&v_1);
    } else {
      for (uint16_t i = 0; i < v_1.exp - v_2.exp; ++i) bits10up(&v_2);
    }
    if (compearbits(v_1, shiftleft(v_2, 96)) < 0) {
      v_1 = divmain(v_1, v_2, &res);
      if (!iszerow(v_1)) divtail(v_1, v_2, &res);
    } else
      overflow = (sign) ? 2 : 1;
    if (overflow == 0) {
      overflow = normalize(&res);
      *result = convert2s21(res, sign);
    }
  }
  return overflow;
}
