#include "s21_decimal.h"

#include <math.h>

#include "utils.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = (result) ? 0 : 1;
  if (!error) {
    for (uint16_t i = 0; i < 3; ++i) result->bits[i] = value.bits[i];
    result->bits[3] = value.bits[3] ^ MINUS;
  }
  return error;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = (result) ? 0 : 4;
  if ((iszero(value_1) || iszero(value_2)) && !overflow)
    *result = (iszero(value_1)) ? s21normal(value_2) : s21normal(value_1);
  else if (!overflow) {
    int sign_1 = isminus(value_1), sign_2 = isminus(value_2);
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                 res = initwork();
    overflow = pointequalize(&v_1, &v_2);
    if (sign_1 == sign_2)
      res = addbits(v_1, v_2);
    else {
      int who_grate = compearbits(v_1, v_2);
      if (who_grate == 1)
        res = subbits(v_1, v_2);
      else if (who_grate == -1) {
        sign_1 = sign_2;
        res = subbits(v_2, v_1);
      }
    }
    overflow = normalize(&res);
    if (overflow)
      overflow = (sign_1) ? 2 : 1;
    else
      *result = convert2s21(res, sign_1);
  }
  return overflow;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal not_value_2 = set21(0, 0, 0, 0);
  s21_negate(value_2, &not_value_2);
  return s21_add(value_1, not_value_2, result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int overflow = (result) ? 0 : 4;
  if ((iszero(value_1) || iszero(value_2)) && !overflow)
    *result = set21(0, 0, 0, 0);
  else if (!overflow) {
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
  int overflow = (result) ? 0 : 4;
  int sign = (isminus(value_1) == isminus(value_2)) ? 0 : 1;
  if (iszero(value_2))
    overflow = 3;
  else if (iszero(value_1) && !overflow)
    *result = set21(0, 0, 0, 0);
  else if (!overflow) {
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
  int overflow = (result) ? 0 : 4;
  if (iszero(value_2))
    overflow = 3;
  else if (!overflow) {
    int sign = isminus(value_1);
    value_1 = s21normal(value_1);
    value_2 = s21normal(value_2);
    s21_decimal foo = value_1;
    setplus(&foo);
    setplus(&value_2);
    if (s21_is_less(foo, value_2))
      *result = value_1;
    else {
      work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2),
                   res = initwork();
      if (v_1.exp < v_2.exp) {
        for (uint16_t i = 0; i < v_2.exp - v_1.exp; ++i) bits10up(&v_1);
        v_1.exp = v_2.exp;
      } else {
        for (uint16_t i = 0; i < v_1.exp - v_2.exp; ++i) bits10up(&v_2);
      }
      if (compearbits(v_1, shiftleft(v_2, 96)) < 0) res = divremain(v_1, v_2);
      overflow = normalize(&res);
      *result = convert2s21(res, sign);
    }
  }
  return overflow;
}

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  if (iszero(value_1)) {
    if (!iszero(value_2)) res = (isminus(value_2)) ? 0 : 1;
  } else if (iszero(value_2))
    res = (isminus(value_1)) ? 1 : 0;
  else {
    int sign_1 = isminus(value_1), sign_2 = isminus(value_2);
    if (sign_1 != sign_2)
      res = (sign_1 > sign_2) ? 1 : 0;
    else {
      work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2);
      pointequalize(&v_1, &v_2);
      int who_grate = compearbits(v_1, v_2);
      if (who_grate == 1)
        res = (sign_1) ? 1 : 0;
      else if (who_grate == -1)
        res = (sign_1) ? 0 : 1;
    }
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_equal(value_1, value_2)) ? 1 : s21_is_less(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_less_or_equal(value_1, value_2)) ? 0 : 1;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_less(value_1, value_2)) ? 0 : 1;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  if (iszero(value_1))
    res = (iszero(value_2)) ? 1 : 0;
  else if (isminus(value_1) == isminus(value_2)) {
    work_decimal v_1 = convert2work(value_1), v_2 = convert2work(value_2);
    pointequalize(&v_1, &v_2);
    if (compearbits(v_1, v_2) == 0) res = 1;
  }
  return res;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return (s21_is_equal(value_1, value_2)) ? 0 : 1;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = (dst) ? 0 : 1;
  if (!error)
    *dst = (src >= 0) ? set21(0, 0, 0, src) : set21(MINUS, 0, 0, -src);
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int overflow = (dst) ? 0 : 1;
  if (!overflow) {
    s21_decimal tranc_src = set21(0, 0, 0, 0);
    s21_truncate(src, &tranc_src);
    uint32_t zerobit = MAX4BIT & tranc_src.bits[0];
    overflow =
        (tranc_src.bits[2] || tranc_src.bits[1] || zerobit > NOTMINUS) ? 1 : 0;
    if (!overflow)
      *dst = (isminus(tranc_src)) ? -tranc_src.bits[0] : tranc_src.bits[0];
  }
  return overflow;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int overflow = (dst) ? 0 : 1;
  if (isnan(src)) overflow = 1;
  if (!overflow) *dst = set21(0, 0, 0, 0);
  if (src && !overflow) {
    int sign = (src < 0) ? 1 : 0;
    src = fabs(src);
    if (src < 1e-28 || src > 7.92281625e28)
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
  int error = (dst) ? 0 : 1;
  if (iszero(src) && error == 0)
    *dst = 0;
  else if (!error) {
    int sign = (isminus(src)) ? -1 : 1;
    int exp = (src.bits[3] & NOTMINUS) >> 16;
    uint32_t value[3];
    for (uint16_t i = 0; i < 3; ++i) value[i] = (unsigned int)src.bits[i];
    *dst = (pow(2, 64) * value[2] + pow(2, 32) * value[1] + value[0]) *
           pow(10, -exp) * sign;
  }
  return error;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int error = (result) ? 0 : 1;
  if (iszero(value) && error == 0)
    *result = set21(0, 0, 0, 0);
  else if (!error) {
    int sign = isminus(value), notint = 0;
    work_decimal work_value = convert2work(value);
    while (work_value.exp) {
      int remainder = dellast(&work_value);
      notint = (notint) ? notint : remainder;
    }
    if (notint && sign) addnum(&work_value, 1);
    if (work_value.bits[3]) error = 1;
    if (!error) *result = convert2s21(work_value, sign);
  }
  return error;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int error = (result) ? 0 : 1;
  if (iszero(value) && error == 0)
    *result = set21(0, 0, 0, 0);
  else if (!error) {
    int sign = isminus(value), remainder = 0;
    work_decimal work_value = convert2work(value);
    while (work_value.exp) remainder = dellast(&work_value);
    if (remainder > 4) addnum(&work_value, 1);
    if (work_value.bits[3]) error = 1;
    if (!error) *result = convert2s21(work_value, sign);
  }
  return error;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error = (result) ? 0 : 1;
  if (iszero(value) && error == 0)
    *result = set21(0, 0, 0, 0);
  else if (!error) {
    int sign = isminus(value);
    work_decimal work_value = convert2work(value);
    while (work_value.exp) dellast(&work_value);
    *result = convert2s21(work_value, sign);
  }
  return error;
}
