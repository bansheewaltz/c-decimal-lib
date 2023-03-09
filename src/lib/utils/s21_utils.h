#ifndef S21_UTILS_H
#define S21_UTILS_H
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include "s21_decimal.h"

#define MAX4BIT 0xffffffff
#define MINUS 0x80000000
#define NOTMINUS 0x7fffffff
#define WORKBITS 7

typedef struct {
  uint64_t bits[WORKBITS];
  uint16_t exp;
} work_decimal;

int iszero(s21_decimal value);
int iszerow(work_decimal value);
int isminus(s21_decimal value);
void setplus(s21_decimal *value);

work_decimal convert2work(s21_decimal value);
s21_decimal convert2s21(work_decimal value, int sign);
uint64_t getoverflow(uint64_t bit);
uint64_t bitwithoutover(uint64_t bit);

unsigned int bits10up(work_decimal *value);
unsigned int bits10down(work_decimal *value);
unsigned int addlast(work_decimal *value);
unsigned int dellast(work_decimal *value);
unsigned int pointequalize(work_decimal *value1, work_decimal *value2);

int bankround(work_decimal value, unsigned int remander,
              unsigned int countround);
int needdown(work_decimal value);
void work2normal(work_decimal *value);
s21_decimal s21normal(s21_decimal value);
int normalize(work_decimal *value);

int compearbits(work_decimal value_1, work_decimal value_2);
unsigned int addnum(work_decimal *value, uint16_t num);
work_decimal addbits(work_decimal value_1, work_decimal value_2);
void twos_complement_bits(work_decimal *value);
work_decimal subbits(work_decimal value_1, work_decimal value_2);

void sumworkbits(work_decimal *value, uint32_t index, uint64_t add1,
                 uint64_t add2, uint64_t add3);
work_decimal shiftleft(work_decimal value, uint16_t shift);
work_decimal divmain(work_decimal v_1, work_decimal v_2, work_decimal *res);
uint16_t maxshift(work_decimal v1, work_decimal v2);
work_decimal divremain(work_decimal v_1, work_decimal v_2);
void divtail(work_decimal v_1, work_decimal v_2, work_decimal *res);
int char2num(char c);

s21_decimal set21(int bits3, int bits2, int bits1, int bits0);
work_decimal initwork();

#endif  // S21_UTILS_H