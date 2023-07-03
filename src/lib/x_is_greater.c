#include "x_decimal.h"
#include "x_utils.h"

int x_is_greater(x_decimal value_1, x_decimal value_2) {
  return (x_is_less_or_equal(value_1, value_2)) ? 0 : 1;
}
