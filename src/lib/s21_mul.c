#include "s21_decimal.h"
#include "s21_utils.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = (result) ? 0 : 4;
  if ((iszero(value_1) || iszero(value_2)) && !overflow)
    *result = set21(0, 0, 0, 0);
  else if (!overflow) {
    int sign = (isminus(value_1) == isminus(value_2)) ? 0 : 1;
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                 res = initwork();
    res.exp = v_1.exp + v_2.exp;
    sumworkbits(&res, 0, v_1.bits[0] * v_2.bits[0], 0, 0);
    sumworkbits(&res, 1, v_1.bits[1] * v_2.bits[0], v_1.bits[0] * v_2.bits[1],
                0);
    sumworkbits(&res, 2, v_1.bits[0] * v_2.bits[2], v_1.bits[1] * v_2.bits[1],
                v_1.bits[2] * v_2.bits[0]);
    sumworkbits(&res, 3, v_1.bits[1] * v_2.bits[2], v_1.bits[2] * v_2.bits[1],
                0);
    sumworkbits(&res, 4, v_1.bits[2] * v_2.bits[2], 0, 0);
    overflow = normalize(&res);
    if (overflow)
      overflow = (sign) ? 2 : 1;
    else
      *result = convert2s21(res, sign);
  }
  return overflow;
}
