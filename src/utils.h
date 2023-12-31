#ifndef x_UTILS_H
#define x_UTILS_H

#include "x_decimal.h"

#define MAX4BIT 0xffffffff
#define MINUS 0x80000000
#define NOTMINUS 0x7fffffff
#define WORKBITS 7

typedef struct {
  uint64_t bits[WORKBITS];
  uint16_t exp;
} work_decimal;

// если value равно 0, возвращает 1
int iszero(x_decimal value);
int iszerow(work_decimal value);
// если value отрицательное, возвращает 1
int isminus(x_decimal value);
// делает устанавливает знак value
void setminus(x_decimal *value);
void setplus(x_decimal *value);

// конвертирует обычный децимал в рабочий и наоборот
work_decimal convert2work(x_decimal value);
x_decimal convert2s21(work_decimal value, int sign);

// выдаёт переполнение (все биты начиная с 32)
uint64_t getoverflow(uint64_t bit);
// отбрасывает переполнение (все биты начиная с 32)
uint64_t bitwithoutover(uint64_t bit);
// uint64_t addolderbit(uint64_t smallerbit, uint64_t olderbit);

// умножает битовую часть на 10, степень не трогает.
// если при умножении будет переполнение, то значение не поменяется,
// а функция вернёт 1.
unsigned int bits10up(work_decimal *value);
// делит битовую часть на 10, степень не трогает.
// возвращает остаток от деления
unsigned int bits10down(work_decimal *value);
// если возможно, приписывает дополнительный ноль после запятой, т.е.
// умножает битовую часть на 10 и увеличивает степень на 1.
// если не возможно, то значение не меняется, вернёт 1.
unsigned int addlast(work_decimal *value);
// убирает последнюю цифру после запятой без проверки степени, т.е.
// делит битовую часть на 10 и уменьшает степень на 1.
// возвращает удалённую цифру
unsigned int dellast(work_decimal *value);

// делает value1 и value2 с одинаковым количеством знаков после запятой
// возвращает ошибку, если это не возможно
unsigned int pointequalize(work_decimal *value1, work_decimal *value2);

// возвращает 1 если нужно ли округлять вверх  value по правилам банковского
// округления получает последнее отброшенное число remander и количество
// отброшеных не нулевых цифр countround
int bankround(work_decimal value, unsigned int remander,
              unsigned int countround);
// возвращает 1 если  value нужно и можно отбросить справа цифру после запятой
int needdown(work_decimal value);
// приводит bits и exp у value в правильный вид и затем удаляет лишние нули
// после запятой у value возвращает 1 если value содержит переполнение или
// большую степень
int normalize(work_decimal *value);

// сравнивает только биты value_1 и value_2 без учёта степени
// возвращает 0 если равны, 1 если больше value_1, -1 если больше value_2
int compearbits(work_decimal value_1, work_decimal value_2);

// добавляет с битам небольшое положительное число
// возвращает если самый старший бит переполнен
unsigned int addnum(work_decimal *value, uint16_t num);
// складывает биты value_1 и value_2 без учёта степени
// вернёт результат даже с переполнением старшего бита
work_decimal addbits(work_decimal value_1, work_decimal value_2);
// переводит биты в дополнительное битовое представление (делает отрицательным)
void twos_complement_bits(work_decimal *value);
// вычитает биты value_2 из value_1 без учета степени
// работает корректно, если value_1 > value_2
work_decimal subbits(work_decimal value_1, work_decimal value_2);

// прибавляет к value.bits[index] числа add1, add2, add3
// в случае переполнения переносит лишнее в value.bits[index+1]
void sumworkbits(work_decimal *value, uint32_t index, uint64_t add1,
                 uint64_t add2, uint64_t add3);
// возвращает побитвый сдвиг влево битов value на shift битов
work_decimal shiftleft(work_decimal value, uint16_t shift);
// делит биты v_1 на биты v_2 нацело, возвращает остаток, результат деления в
// res
work_decimal divmain(work_decimal v_1, work_decimal v_2, work_decimal *res);
// делит биты v_1 на биты v_2 нацело, возвращает остаток
work_decimal divremain(work_decimal v_1, work_decimal v_2);
// дописывает дробную часть в res деления битов v_1 на v_2 (v1 < v2)
// в количестве MAXEXP + 1 знаков после запятой
void divtail(work_decimal v_1, work_decimal v_2, work_decimal *res);
// переводит цифру в символ цифры
int char2num(char c);

// возвращает децимал, записывая значения битов
x_decimal set21(int bits3, int bits2, int bits1, int bits0);
// возвращает рабочий децимал заполненый нулями
work_decimal initwork();

// НИЖЕ ФУНКЦИИ ДЛЯ ТЕСТИРОВАНИЯ ПЕРЕД СДАЧЕЙ БУДУТ УДАЛЕНЫ
// печатет децимал. bits[3] всегда в hex,
// остальные при type = 0 в hex, иначе десятичный
void print_s21(x_decimal value, int type);
// печатет рабочий децимал. степень в деятичном (в скобках hex),
// биты при type = 0 в hex, иначе десятичный
void print_work(work_decimal value, int type);

#endif  // x_UTILS_H
