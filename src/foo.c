#include <stdio.h>
#include <limits.h>
#include <stdint.h>

#define MAXEXP 28
#define MAX2BIT 0xffff
#define MAX4BIT 0xffffffff
#define MINUS 0x80000000
#define NOTMINUS 0x7fffffff

#if UINT_MAX > MAX2BIT
typedef struct
{
  int bits[4];
} s21_decimal;
#else
typedef struct
{
  int_fast32_t bits[4];
} s21_decimal;
#endif

typedef struct
{
  uint64_t bits[3];
  uint16_t exp;
} work_decimal;


typedef struct
{
  uint64_t bits[6];
  uint16_t exp;
} extended_decimal;

int iszero(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

int isminus(s21_decimal value) {
  return value.bits[3] >> 31;
}

void setminus(s21_decimal *value) {
  value -> bits[3] | MINUS;
}

void setplus(s21_decimal *value) {
  value -> bits[3] & NOTMINUS;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  for (uint16_t i = 0; i < 3; ++i) result -> bits[i] = value.bits[i];
  result -> bits[3] = value.bits[3] ^ MINUS;
}

uint64_t bitwithoutover(uint64_t bit) {
  return bit & MAX4BIT;
}


uint16_t getoverflow(uint64_t bit) {
  return bit >> 32;
}

uint64_t addolderbit(uint64_t smallerbit, uint64_t olderbit) {
  return smallerbit + olderbit << 32;
}

work_decimal convert2work(s21_decimal value) {
  work_decimal result;
  for (uint16_t i = 0; i < 3; ++i) result.bits[i] = bitwithoutover(value.bits[i]);
  result.exp = (value.bits[3] & NOTMINUS) >> 16;
  return result;
}

s21_decimal convert2s21(work_decimal value, int sign) {
  s21_decimal result;
  for (uint16_t i = 0; i < 3; ++i) result.bits[i] = bitwithoutover(value.bits[i]);
  result.bits[3] = (value.exp << 16) | ((sign) ? MINUS : NOTMINUS);
  return result;
}

unsigned int bits10up(work_decimal *value) {
  unsigned int overflow = 0;
  work_decimal foo = *value;
  for (uint16_t i = 0; i < 3; ++i) {
    value -> bits[i] = bitwithoutover(10 * (foo.bits[i] + overflow));
    overflow = getoverflow(10 * (foo.bits[i] + overflow));
  }
  if (overflow) *value = foo;
  return overflow;
}

unsigned int bits10down(work_decimal *value) {
  uint64_t remainder = 0;
  work_decimal foo = *value;
  for (uint16_t i = 2; i >= 0; --i) {
    value -> bits[i] = addolderbit(foo.bits[i], remainder) / 10;
    remainder = addolderbit(foo.bits[i], remainder) % 10;
  }
  return (unsigned int) remainder;
}

unsigned int pointleft(work_decimal *value) {
  unsigned int overflow = 0;
  if (value -> exp < MAXEXP) {
    if (bits10up(value)) overflow = 1;
    else value -> exp += 1;
  } else overflow = 1;
  return overflow;
}

unsigned int pointright(work_decimal *value, unsigned int *remainder) {
  unsigned int overflow = 0;
  if (value -> exp > 0) {
    *remainder = bits10down(value);
    value -> exp -= 1;
  } else overflow = 1;
  return overflow;
}

int pointequalize(work_decimal *value1, work_decimal *value2) {
  int rounding = 0;
  unsigned int overflow = 0;
  if (value1 -> exp != value2 -> exp) {
    int whoismax = (value1 -> exp > value2 -> exp) ? 1 : 2;
    work_decimal *max = (whoismax == 1) ? value1 : value2;
    work_decimal *min = (whoismax == 2) ?  value1 : value2;
    uint16_t expdif = max -> exp - min -> exp;
    for (; expdif > 0 && overflow == 0; --expdif) overflow = pointleft(min);
    if (overflow) {
      overflow  = 0;
      for (; expdif > 0; --expdif) {
        pointright(max, &rounding);
        if (rounding) overflow = 1;
      }
      if (overflow) {
        if (rounding < 5) rounding = (whoismax == 1) ? -1 : -2;
        else rounding = (whoismax == 1) ? 1 : 2;
      }
    }
  }
  return rounding;
}

int normalize(work_decimal *value) {
  int error = getoverflow(value -> bits[0]) || getoverflow(value -> bits[1]) || getoverflow(value -> bits[2]);
  if (error == 0) value -> exp > MAXEXP;
  if (error == 0 && value -> exp > 0) {
    work_decimal foo = *value;
    unsigned int remainder = 0;
    while (remainder == 0 && pointright(&foo, &remainder)) if (remainder == 0) *value = foo;
  }
  return error;
}

int main() {
  printf("int = %libit; long int = %libit; long long int  = %libit.\n", sizeof(int), sizeof(long int), sizeof(long long int));
  return 0;
}