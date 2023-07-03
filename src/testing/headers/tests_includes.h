#include <stdint.h>
#include <stdio.h>

#include "x_decimal.h"
#include "x_utils.h"
#define DEC_IN_BYTES sizeof(x_decimal)
#define INT_IN_BYTES sizeof(int32_t)
#define DEC_IN_INTS (DEC_IN_BYTES / INT_IN_BYTES)
