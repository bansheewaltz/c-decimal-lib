#include "utils.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal not_value_2 = set21(0, 0, 0, 0);
  s21_negate(value_2, &not_value_2);
  return s21_add(value_1, not_value_2, result);
}
