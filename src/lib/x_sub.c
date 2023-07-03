#include "x_decimal.h"
#include "x_utils.h"

int x_sub(x_decimal value_1, x_decimal value_2, x_decimal *result) {
  x_decimal not_value_2 = set21(0, 0, 0, 0);
  x_negate(value_2, &not_value_2);
  return x_add(value_1, not_value_2, result);
}
