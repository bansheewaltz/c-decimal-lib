#ifndef S21_UTILS_H
#define S21_UTILS_H

#include "s21_decimal.h"

int iszero(s21_decimal value);

int isminus(s21_decimal value);

void setminus(s21_decimal *value);

void setplus(s21_decimal *value);

uint64_t bitwithoutover(uint64_t bit);

uint16_t getoverflow(uint64_t bit);

uint64_t addolderbit(uint64_t smallerbit, uint64_t olderbit);

work_decimal convert2work(s21_decimal value);

s21_decimal convert2s21(work_decimal value, int sign);

unsigned int bits10up(work_decimal *value);

unsigned int bits10down(work_decimal *value);

unsigned int pointleft(work_decimal *value);

unsigned int pointright(work_decimal *value, unsigned int *remainder);

int pointequalize(work_decimal *value1, work_decimal *value2);

int normalize(work_decimal *value);

s21_decimal set21(int bits3, int bits2, int bits1, int bits0);

void print_s21(s21_decimal value, int type);

void print_work(work_decimal value, int type);


#endif  // S21_UTILS_H