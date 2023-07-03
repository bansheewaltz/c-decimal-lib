#include <stdint.h>
#include <stdio.h>

#include "../../utils.h"
#define DEC_IN_BYTES sizeof(x_decimal)
#define INT_IN_BYTES sizeof(int32_t)
#define DEC_IN_INTS (DEC_IN_BYTES / INT_IN_BYTES)
