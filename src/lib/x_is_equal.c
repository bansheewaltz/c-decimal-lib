#include "x_utils.h"

int x_is_equal(x_decimal value_1, x_decimal value_2) {
  int res = 0;
  if (iszero(value_1))
    res = (iszero(value_2)) ? 1 : 0;
  else if (isminus(value_1) == isminus(value_2)) {
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2);
    pointequalize(&v_1, &v_2);
    if (compearbits(v_1, v_2) == 0) res = 1;
  }
  return res;
}
