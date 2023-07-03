#include "x_utils.h"

int x_round(x_decimal value, x_decimal *result) {
  int error = (result) ? 0 : 1;
  if (iszero(value) && error == 0)
    *result = set21(0, 0, 0, 0);
  else if (!error) {
    int sign = isminus(value), remainder = 0;
    work_decimal work_value = convert2work(value);
    unsigned int countround = 0;
    while (work_value.exp) {
      remainder = dellast(&work_value);
      if (remainder) countround++;
    }
    if (bankround(work_value, remainder, countround)) addnum(&work_value, 1);
    *result = convert2s21(work_value, sign);
  }
  return error;
}
