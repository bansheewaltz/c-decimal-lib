#include <stdio.h>
#include <limits.h>
#include <stdint.h>

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

work_decimal convert2work(s21_decimal value) {
  work_decimal result;
  for (uint16_t i = 0; i < 3; ++i) result.bits[i] = value.bits[i] & MAX4BIT;
  result.exp = (value.bits[3] & NOTMINUS) >> 16;
  return result;
}

s21_decimal convert2s21(work_decimal value, int sign) {
  s21_decimal result;
  for (uint16_t i = 0; i < 3; ++i) result.bits[i] = value.bits[i] & MAX4BIT;
  result.bits[3] = (value.exp << 16) | ((sign) ? MINUS : NOTMINUS);
  return result;
}

int main() {
  printf("int = %libit; long int = %libit; long long int  = %libit.\n", sizeof(int), sizeof(long int), sizeof(long long int));
}