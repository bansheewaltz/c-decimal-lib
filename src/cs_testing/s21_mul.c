#include "../utils.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = 0;
  int sign = (isminus(value_1) == isminus(value_2)) ? 0 : 1;
  if (iszero(value_1) || iszero(value_2))
    *result = set21(MINUS * sign, 0, 0, 0);
  else {
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                 res = initwork();
    res.exp = v_1.exp + v_2.exp;
    res.bits[0] = v_1.bits[0] * v_2.bits[0];
    res.bits[1] = v_1.bits[1] * v_2.bits[0] + v_1.bits[0] * v_2.bits[1];
    res.bits[2] = v_1.bits[0] * v_2.bits[2] + v_1.bits[1] * v_2.bits[1] +
                  v_1.bits[2] * v_2.bits[0];
    res.bits[3] = v_1.bits[1] * v_2.bits[2] + v_1.bits[2] * v_2.bits[1];
    res.bits[4] = v_1.bits[2] * v_2.bits[2];
    overflow = normalize(&res);
    if (overflow)
      overflow = (sign) ? 2 : 1;
    else
      *result = convert2s21(res, sign);
  }
  return overflow;
}