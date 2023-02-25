#include <stdint.h>
#include <stdio.h>

#include "s21_decimal.h"
#include "s21_utils.h"
#define DEC_IN_BYTES sizeof(s21_decimal)
#define INT_IN_BYTES sizeof(int32_t)
#define DEC_IN_INTS (DEC_IN_BYTES / INT_IN_BYTES)
