#include "s21_decimal.h"
#include "s21_utils.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int error = (result) ? 0 : 1;
  if (iszero(value) && error == 0)
    *result = set21(0, 0, 0, 0);
  else if (!error) {
    int sign = isminus(value), remainder = 0;
    work_decimal work_value = convert2work(value);
    while (work_value.exp) remainder = dellast(&work_value);
    if (remainder > 4)
      addnum(&work_value, 1);
    *result = convert2s21(work_value, sign);
  }
  return error;
}
