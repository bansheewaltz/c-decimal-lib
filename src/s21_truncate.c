#include "utils.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error = (result) ? 0 : 1;
  if (iszero(value) && error == 0)
    *result = set21(0, 0, 0, 0);
  else if (!error) {
    int sign = isminus(value);
    work_decimal work_value = convert2work(value);
    while (work_value.exp) dellast(&work_value);
    *result = convert2s21(work_value, sign);
  }
  return error;
}