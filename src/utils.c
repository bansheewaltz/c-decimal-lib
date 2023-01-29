
#include <stdio.h>
#include "utils.h"

int iszero(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

int isminus(s21_decimal value) {
  return (value.bits[3] & MINUS) == MINUS;
}

void setminus(s21_decimal *value) {
  value -> bits[3] |= MINUS;
}

void setplus(s21_decimal *value) {
  value -> bits[3] &= NOTMINUS;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  for (uint16_t i = 0; i < 3; ++i) result -> bits[i] = value.bits[i];
  result -> bits[3] = value.bits[3] ^ MINUS;
  return 0;
}

uint64_t bitwithoutover(uint64_t bit) {
  return bit & MAX4BIT;
}


uint16_t getoverflow(uint64_t bit) {
  return bit >> 32;
}

uint64_t addolderbit(uint64_t smallerbit, uint64_t olderbit) {
  return smallerbit + (olderbit << 32);
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
  result.bits[3] = (value.exp << 16) | ((sign) ? MINUS : 0);
  return result;
}

unsigned int bits10up(work_decimal *value) {
  unsigned int overflow = 0;
  work_decimal foo = *value;
  for (uint16_t i = 0; i < 3; ++i) {
    value -> bits[i] = bitwithoutover(10 * foo.bits[i] + overflow);
    overflow = getoverflow(10 * foo.bits[i] + overflow);
  }
  if (overflow) *value = foo;
  return overflow;
}

unsigned int bits10down(work_decimal *value) {
  uint64_t remainder = 0;
  work_decimal foo = *value;
  for (int16_t i = 2; i >= 0; --i) {
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
  unsigned int rounding = 0;
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
  int error = getoverflow(value -> bits[0]) || getoverflow(value -> bits[1]) || getoverflow(value -> bits[2]) || value ->exp > MAXEXP;
  if (error == 0 && value -> exp > 0) {
    work_decimal foo = *value;
    int doing = 1;
    while (doing) {
      int overflow = 0;
      unsigned int remainder = 0;
      overflow = pointright(&foo, &remainder);
      if (overflow == 0 && remainder == 0) *value = foo;
      else doing = 0;
      }
  }
  return error;
}

s21_decimal set21(int bits3, int bits2, int bits1, int bits0) {
  s21_decimal value;
  value.bits[0] = bits0 & MAX4BIT;
  value.bits[1] = bits1 & MAX4BIT;
  value.bits[2] = bits2 & MAX4BIT;
  value.bits[3] = bits3 & MAX4BIT;
  return value;
}

void print_s21(s21_decimal value, int type) {
  if (type == 0) for(int16_t i = 3; i >= 0; --i) printf("%x ", value.bits[i]);
  else for(int16_t i = 3; i >= 0; --i) printf("%u ", value.bits[i]);
  printf("\n");
}

void print_work(work_decimal value, int type) {
  printf("exp = %u (%#x) bits = ", value.exp, value.exp);
#ifdef __linux__
  if (type == 0) for(int16_t i = 2; i >= 0; --i) printf("%lx ", value.bits[i]);
  else for(int16_t i = 2; i >= 0; --i) printf("%lu ", value.bits[i]);
#else
  if (type == 0) for(int16_t i = 2; i >= 0; --i) printf("%llx ", value.bits[i]);
  else for(int16_t i = 2; i >= 0; --i) printf("%llu ", value.bits[i]);
#endif
  printf("\n");
}
