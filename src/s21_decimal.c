#include <math.h>
#include "s21_decimal.h"
#include "utils.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  for (uint16_t i = 0; i < 3; ++i) result -> bits[i] = value.bits[i];
  result -> bits[3] = value.bits[3] ^ MINUS;
  return 0;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
/*    1. Проверить значения на ноль, если кто-то ноль, то результат другое число
      2. Сохраняем знаки (isminus). Конвертируем в рабочий децимал (convert2work)
      3. Уравнять количество знаков после запятой (pointequalize)
      4. Посмотреть на знаки. Если знаки одинаковые, то будем складывать. 
      Если разные, то вычитать из большего меньшее.
      5.1 При сложении: сложить биты (addbits), проверить переполнение (bits[3]!=0).
      Если переполнен, то сдвинуть запятую (pointright). 
      Если сдвинуть нельзя, то вернуть ошибку переполнения в зависимости от итогового знака 1 или 2.
      Если сдвинуть можно, то если удаленная цифра больше 4, добавить 1 (addnum с 1).
      5.2 При вычитании сравнить биты (compearbits). Если равны, то результат 0.
      Если разные, то итоговый знак будет знак большего числа.
      Вычитать из большего меньшее (subbits)
      6. Нормализовать результат (normalize)
      7. Конвертировать результат из рабочего децимала с правильным итоговым знаком (convert2s21) */
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  /*  1. меняем знак value_2 (s21_negate)
      2. Складываем в (s21_add)*/
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = 0;
  if (iszero(value_1) || iszero(value_2)) {
    for (int16_t i = 0; i < 4; ++i) result -> bits[i] = 0;
  } else {
    int sign = (isminus(value_1) == isminus(value_2)) ? 0 : 1;
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2), res;
    res.exp = v_1.exp + v_2.exp;
    res.bits[0] = v_1.bits[0] * v_2.bits[0];
    res.bits[1] = v_1.bits[1] * v_2.bits[0] + v_1.bits[0] * v_2.bits[1];
    res.bits[2] = v_1.bits[0] * v_2.bits[2] + v_1.bits[1] * v_2.bits[1] + v_1.bits[2] * v_2.bits[0];
    res.bits[3] = v_1.bits[1] * v_2.bits[2] + v_1.bits[2] * v_2.bits[1];
    res.bits[4] = v_1.bits[2] * v_2.bits[2];
    res.bits[5] = 0;
    for (int16_t i = 0; i < WORKBITS - 1; ++i) {
      res.bits[i + 1] += getoverflow(res.bits[i]);
      res.bits[i] = bitwithoutover(res.bits[i]);
    }
    if (!checkoverflowmult(res)) {
      unsigned int remainder = 0;
      while (res.exp > MAXEXP) remainder = dellast(&res);
      if (remainder >= 5) {
        if (addnum(&res, 1)) {// 2^96 -1 
          if (dellast(&res) >= 5) add1(&res);
        }
      }
      overflow = normalize(&res);
      for (uint16_t i = 0; i < 3; ++i) result -> bits[i] = bitwithoutover(res.bits[i]);
      result -> bits[3] = (res.exp << 16) | ((sign) ? MINUS : 0);
    } else overflow = (sign) ? 2 : 1;
  }
  return overflow;
}

/*    При сравнении
      1. Проверить значения на ноль, если кто-то ноль, то смотрим знак другого числа и пишем ответ
      2. Смотрим знаки (isminus). Если разные, то пишем ответ.
      3. Если знаки одинаковые сохраняем их. Уравнять количество знаков после запятой (pointequalize).
      4. Сравнить биты (compearbits). Если равны, то смортеть, есть ли округление у pointequalize.
      5. В зависимости от сохраненных знаков пишем ответ */

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
/*    1. Проверить знак. Если минус, то запомнить знак, и умножить число на -1
      2. set21 нулями, кроме bits[0], он равен числу.
      3. Если знак был минус, то setminus */
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
/*    1. Проверить значения на ноль, если ноль пишем ответ 0.
      2. Запомнить знак (isminus). Конвертируем в рабочий децимал (convert2work)
      3. Сдвигаем знак (pointright) пока можно
      4. Если последняя удаленная цифра больше 4, добавить 1 (addnum с 1) 
      5. Если bits[3] или bits[2] или bits[1] не ноль или bits[0] > NOTMINUS то ошибка
      6. Если ошибки нет приравниваем bits[0] и если нужно меняем знак  */
}

int s21_floor(s21_decimal value, s21_decimal *result) {
/*    1. Проверить значения на ноль, если ноль пишем ответ 0.
      2. Запомнить знак (isminus). Конвертируем в рабочий децимал (convert2work)
      3. Сдвигаем знак (pointright) пока можно
      4. Если последняя удаленная цифра не 0, при знаке минус увеличиваем на 1 (addnum с 1)
      5. Если bits[3] не ноль - ошибка
      6. Конвертировать результат из рабочего децимала с правильным итоговым знаком (convert2s21) */
}

int s21_round(s21_decimal value, s21_decimal *result) {
/*    1. Проверить значения на ноль, если ноль пишем ответ 0.
      2. Запомнить знак (isminus). Конвертируем в рабочий децимал (convert2work)
      3. Сдвигаем знак (pointright) пока можно
      4. Если последняя удаленная цифра больше 4, увеличиваем на 1 (addnum с 1)
      5. Если bits[3] не ноль - ошибка
      6. Конвертировать результат из рабочего децимала с правильным итоговым знаком (convert2s21) */
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
/*    1. Проверить значения на ноль, если ноль пишем ответ 0.
      2. Запомнить знак (isminus). Конвертируем в рабочий децимал (convert2work)
      3. Сдвигаем знак (pointright) пока можно
      4. Конвертировать результат из рабочего децимала с правильным итоговым знаком (convert2s21) */
}