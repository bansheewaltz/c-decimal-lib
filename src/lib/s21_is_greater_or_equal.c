#include "s21_decimal.h"
#include "s21_utils.h"

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_less(value_1, value_2)) ? 0 : 1;
}
