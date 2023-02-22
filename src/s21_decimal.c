#include "s21_decimal.h"

#include <math.h>

#include "utils.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  for (uint16_t i = 0; i < 3; ++i) result->bits[i] = value.bits[i];
  result->bits[3] = value.bits[3] ^ MINUS;
  return 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  /*    1. Проверить значения на ноль, если кто-то ноль, то результат другое
     число
        2. Сохраняем знаки (isminus). Конвертируем в рабочий децимал
     (convert2work)
        3. Уравнять количество знаков после запятой (pointequalize)
        4. Посмотреть на сохранённые знаки. Если знаки одинаковые, то будем
     складывать. Если разные, то вычитать из большего меньшее. 5.1 При сложении:
     сложить биты (addbits). 5.2 При вычитании сравнить биты (compearbits). Если
     равны, то результат 0. Если разные, то итоговый знак будет знак большего
     числа. Вычитать из большего меньшее (subbits)
        6. Нормализовать результат (normalize). Если normalize вернул ошибку
     (возможно при сложении), то вернуть ошибку с учётом итогового знака
     конечного ответа 1 или 2.
        7. Конвертировать результат из рабочего децимала с правильным итоговым
     знаком (convert2s21) */
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  /*  1. меняем знак value_2 (s21_negate)
      2. Складываем в (s21_add)*/
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = 0;
  if (iszero(value_1) || iszero(value_2)) {
    for (int16_t i = 0; i < 4; ++i) result->bits[i] = 0;
  } else {
    int sign = (isminus(value_1) == isminus(value_2)) ? 0 : 1;
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                 res = initwork();
    res.exp = v_1.exp + v_2.exp;
    sumworkbits(&res, 0, v_1.bits[0] * v_2.bits[0], 0, 0);
    sumworkbits(&res, 1, v_1.bits[1] * v_2.bits[0], v_1.bits[0] * v_2.bits[1],
                0);
    sumworkbits(&res, 2, v_1.bits[0] * v_2.bits[2], v_1.bits[1] * v_2.bits[1],
                v_1.bits[2] * v_2.bits[0]);
    sumworkbits(&res, 3, v_1.bits[1] * v_2.bits[2], v_1.bits[2] * v_2.bits[1],
                0);
    sumworkbits(&res, 4, v_1.bits[2] * v_2.bits[2], 0, 0);
    overflow = normalize(&res);
    if (overflow)
      overflow = (sign) ? 2 : 1;
    else
      *result = convert2s21(res, sign);
  }
  return overflow;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = 0;
  int sign = (isminus(value_1) == isminus(value_2)) ? 0 : 1;
  if (iszero(value_2))
    overflow = 3;
  else if (iszero(value_1))
    *result = set21(MINUS * sign, 0, 0, 0);
  else {
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                 res = initwork();
    if (v_1.exp < v_2.exp) {
      for (uint16_t i = 0; i < v_2.exp - v_1.exp; ++i) bits10up(&v_1);
    } else {
      for (uint16_t i = 0; i < v_1.exp - v_2.exp; ++i) bits10up(&v_2);
    }
    if (compearbits(v_1, shiftleft(v_2, 96)) < 0) {
      v_1 = divmain(v_1, v_2, &res);
      if (!iszerow(v_1)) divtail(v_1, v_2, &res);
    } else
      overflow = (sign) ? 2 : 1;
    if (overflow == 0) {
      overflow = normalize(&res);
      *result = convert2s21(res, sign);
    }
  }
  return overflow;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = 0;
  if (iszero(value_2))
    overflow = 3;
  else {
    int sign = isminus(value_1);
    if (s21_is_less(value_1, value_2))
      *result = value_1;
    else {
      work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                   res = initwork();
      if (v_1.exp < v_2.exp) {
        for (uint16_t i = 0; i < v_2.exp - v_1.exp; ++i) bits10up(&v_1);
      } else {
        for (uint16_t i = 0; i < v_1.exp - v_2.exp; ++i) bits10up(&v_2);
      }
      v_1.exp = v_2.exp;
      if (compearbits(v_1, shiftleft(v_2, 96)) < 0) res = divremain(v_1, v_2);
      overflow = normalize(&res);
      *result = convert2s21(res, sign);
    }
  }
  return overflow;
}

/*    При сравнении
      1. Проверить значения на ноль, если кто-то ноль, то смотрим знак другого
   числа и пишем ответ
      2. Смотрим знаки (isminus). Если разные, то пишем ответ.
      3. Если знаки одинаковые сохраняем их. Уравнять количество знаков после
   запятой (pointequalize).
      4. Сравнить биты (compearbits).
      5. В зависимости от сохраненных знаков пишем ответ */

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  /*    1. Проверить знак. Если минус, то запомнить знак, и умножить число на -1
        2. set21 нулями, кроме bits[0], он равен числу.
        3. Если знак был минус, то setminus */
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  /*    1. Проверить значения на ноль, если ноль пишем ответ 0.
        2. Запомнить знак (isminus). Конвертируем в рабочий децимал
     (convert2work)
        3. Сдвигаем запятую (dellast) пока степень не станет 0
        4. Если bits[3] или bits[2] или bits[1] не ноль или bits[0] > NOTMINUS
     то ошибка
        5. Если ошибки нет приравниваем bits[0] и если нужно меняем знак  */
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int overflow = 0;
  if (src == 0)
    *dst = set21(0, 0, 0, 0);
  else {
    int sign = (src < 0) ? 1 : 0;
    src = fabs(src);
    if (src < 1e-28) {
      overflow = 1;
      *dst = set21(0, 0, 0, 0);
    } else if (src > 7.92281625e28)
      overflow = 1;
    else {
      char foo[15];
      sprintf(foo, "%+.6e", src);
      int val = char2num(foo[1]);
      for (uint16_t i = 3; i < 9; ++i) val = 10 * val + char2num(foo[i]);
      int exp = 0;
      sscanf(foo + 10, "%d", &exp);
      exp -= 6;
      work_decimal value = initwork();
      value.bits[0] = val;
      if (exp > 0) {
        for (uint16_t i = 0; i < exp; ++i) overflow = bits10up(&value);
      } else {
        value.exp = -exp;
        overflow = normalize(&value);
      }
      *dst = convert2s21(value, sign);
    }
  }
  return overflow;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (iszero(src))
    *dst = 0;
  else {
    int sign = (isminus(src)) ? -1 : 1;
    int exp = (src.bits[3] & NOTMINUS) >> 16;
    uint32_t value[3];
    for (uint16_t i = 0; i < 3; ++i) value[i] = (unsigned int)src.bits[i];
    *dst = (pow(2, 64) * value[2] + pow(2, 32) * value[1] + value[0]) *
           pow(10, -exp) * sign;
  }
  return 0;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  /*    1. Проверить значения на ноль, если ноль пишем ответ 0.
        2. Запомнить знак (isminus). Конвертируем в рабочий децимал
     (convert2work)
        3. Сдвигаем запятую (dellast) пока степень не станет 0
        4. Если последняя удаленная цифра не 0, при знаке минус увеличиваем на 1
     (addnum с 1)
        5. Если bits[3] не ноль - ошибка
        6. Конвертировать результат из рабочего децимала с правильным итоговым
     знаком (convert2s21) */
}

int s21_round(s21_decimal value, s21_decimal *result) {
  /*    1. Проверить значения на ноль, если ноль пишем ответ 0.
        2. Запомнить знак (isminus). Конвертируем в рабочий децимал
     (convert2work)
        3. Сдвигаем запятую (dellast) пока степень не станет 0
        4. Если последняя удаленная цифра больше 4, увеличиваем на 1 (addnum с
     1)
        5. Если bits[3] не ноль - ошибка
        6. Конвертировать результат из рабочего децимала с правильным итоговым
     знаком (convert2s21) */
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  /*    1. Проверить значения на ноль, если ноль пишем ответ 0.
        2. Запомнить знак (isminus). Конвертируем в рабочий децимал
     (convert2work)
        3. Сдвигаем запятую (dellast) пока степень не станет 0
        4. Конвертировать результат из рабочего децимала с правильным итоговым
     знаком (convert2s21) */
}