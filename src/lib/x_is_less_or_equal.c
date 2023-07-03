#include "x_decimal.h"
#include "x_utils.h"

int x_is_less_or_equal(x_decimal value_1, x_decimal value_2) {
  return (x_is_equal(value_1, value_2)) ? 1 : x_is_less(value_1, value_2);
}
