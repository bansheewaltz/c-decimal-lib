#ifndef S21_UTILS_H
#define S21_UTILS_H

#include "s21_decimal.h"

#define MAX4BIT 0xffffffff
#define MINUS 0x80000000
#define NOTMINUS 0x7fffffff
#define WORKBITS 6

// для pointequalize
#define FLOORFIRST -1 //  первая величина округлена в меньшую сторону
#define FLOORSECOND -2 //  вторая величина округлена в меньшую сторону
#define CEILFIRST 1 //  первая величина округлена в большую сторону
#define CEILSECOND 2 //  вторая величина округлена в большую сторону
#define NOTROUND 0  //  величины не округлены

typedef struct
{
  uint64_t bits[WORKBITS];
  uint16_t exp;
} work_decimal;

// если value равно 0, возвращает 1
int iszero(s21_decimal value);
int iszerow(work_decimal value);
// если value отрицательное, возвращает 1
int isminus(s21_decimal value);
// делает устанавливает знак value
void setminus(s21_decimal *value);
void setplus(s21_decimal *value);

// конвертирует обычный децимал в рабочий и наоборот
work_decimal convert2work(s21_decimal value);
s21_decimal convert2s21(work_decimal value, int sign);

// выдаёт переполнение (все биты начиная с 32)
uint64_t getoverflow(uint64_t bit);
// отбрасывает переполнение (все биты начиная с 32)
uint64_t bitwithoutover(uint64_t bit);
//uint64_t addolderbit(uint64_t smallerbit, uint64_t olderbit);

// умножает битовую часть на 10, степень не трогает.
// если при умножении будет переполнение, то значение не поменяется,
// а функция вернёт 1.
unsigned int bits10up(work_decimal *value);
// если возможно, приписывает дополнительный ноль после запятой, т.е.
// умножает битовую часть на 10 и увеличивает степень на 1.
// если не возможно, то значение не меняется, вернёт 1.
unsigned int pointleft(work_decimal *value);

// делит битовую часть на 10, степень не трогает.
// возвращает остаток от деления
unsigned int bits10down(work_decimal *value);
// убирает последнюю цифру после запятой без проверки степени, т.е.
// делит битовую часть на 10 и уменьшает степень на 1.
// возвращает удалённую цифру
unsigned int dellast(work_decimal *value);
// убирает последнюю цифру value после запятой с проверкой степени
// удаленная цифра сохраняется в remainder
// если после запятой нет цифр (степень 0), вернёт 1 и значения не изменятся
unsigned int pointright(work_decimal *value, unsigned int *remainder);

// делает value1 и мфдгу 2 с одинаковым количеством знаков после запятой
// возвращает какая из величин при этом была округлена
int pointequalize(work_decimal *value1, work_decimal *value2);

// удаляет лишние нули после запятой у value
// возвращает 1 если value содержит переполнение или большую степень
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

// возвращает побитвый сдвиг влево битов value на shift битов;
work_decimal shiftleft(work_decimal value, uint16_t shift);

// возвращает децимал, записывая значения битов
s21_decimal set21(int bits3, int bits2, int bits1, int bits0);
// возвращает рабочий децимал заполненый нулями
work_decimal initwork();

// печатет децимал. bits[3] всегда в hex,
// остальные при type = 0 в hex, иначе десятичный
void print_s21(s21_decimal value, int type);
// печатет рабочий децимал. степень в деятичном (в скобках hex),
// биты при type = 0 в hex, иначе десятичный
void print_work(work_decimal value, int type);


#endif  // S21_UTILS_H