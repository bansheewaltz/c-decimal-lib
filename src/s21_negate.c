#include "utils.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = (result) ? 0 : 1;
  if (!error) {
    for (uint16_t i = 0; i < 3; ++i) result->bits[i] = value.bits[i];
    result->bits[3] = value.bits[3] ^ MINUS;
  }
  return error;
}
