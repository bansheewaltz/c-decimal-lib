#include "utils.h"

#include <stdio.h>

int iszero(x_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

int iszerow(work_decimal value) {
  int res = 1;
  for (uint16_t i = 0; i < WORKBITS; ++i) res &= (value.bits[i] == 0);
  return res;
}

int isminus(x_decimal value) { return (value.bits[3] & MINUS) == MINUS; }

void setminus(x_decimal *value) { value->bits[3] |= MINUS; }

void setplus(x_decimal *value) { value->bits[3] &= NOTMINUS; }

work_decimal convert2work(x_decimal value) {
  work_decimal result;
  for (uint16_t i = 0; i < 3; ++i)
    result.bits[i] = bitwithoutover(value.bits[i]);
  for (uint16_t i = 3; i < WORKBITS; ++i) result.bits[i] = 0;
  result.exp = (value.bits[3] & NOTMINUS) >> 16;
  return result;
}

x_decimal convert2s21(work_decimal value, int sign) {
  x_decimal result;
  for (uint16_t i = 0; i < 3; ++i)
    result.bits[i] = bitwithoutover(value.bits[i]);
  result.bits[3] = (value.exp << 16) | ((sign) ? MINUS : 0);
  return result;
}

uint64_t bitwithoutover(uint64_t bit) { return bit & MAX4BIT; }

uint64_t getoverflow(uint64_t bit) { return bit >> 32; }

uint64_t addolderbit(uint64_t smallerbit, uint64_t olderbit) {
  return smallerbit + (olderbit << 32);
}

unsigned int bits10up(work_decimal *value) {
  unsigned int overflow = 0;
  work_decimal foo = *value;
  for (uint16_t i = 0; i < WORKBITS; ++i) {
    value->bits[i] = bitwithoutover(10 * foo.bits[i] + overflow);
    overflow = getoverflow(10 * foo.bits[i] + overflow);
  }
  if (overflow) *value = foo;
  return overflow;
}
/*
unsigned int pointleft(work_decimal *value) {
  unsigned int overflow = 0;
  if (value -> exp < MAXEXP) {
    if (bits10up(value)) overflow = 1;
    else value -> exp += 1;
  } else overflow = 1;
  return overflow;
} */

unsigned int bits10down(work_decimal *value) {
  uint64_t remainder = 0;
  work_decimal foo = *value;
  for (int16_t i = WORKBITS - 1; i >= 0; --i) {
    value->bits[i] = addolderbit(foo.bits[i], remainder) / 10;
    remainder = addolderbit(foo.bits[i], remainder) % 10;
  }
  return (unsigned int)remainder;
}

unsigned int addlast(work_decimal *value) {
  unsigned int overflow = 0;
  if (bits10up(value))
    overflow = 1;
  else
    value->exp += 1;
  return overflow;
}

unsigned int dellast(work_decimal *value) {
  value->exp -= 1;
  return bits10down(value);
}

/*
unsigned int pointright(work_decimal *value, unsigned int *remainder) {
  unsigned int overflow = 0;
  if (value -> exp > 0) {
    *remainder = bits10down(value);
    value -> exp -= 1;
  } else overflow = 1;
  return overflow;
} */

/* int pointequalize(work_decimal *value1, work_decimal *value2) {
  unsigned int rounding = NOTROUND, overflow = 0, countround = 0;
  if (value1 -> exp != value2 -> exp) {
    int whoismax = (value1 -> exp > value2 -> exp) ? 1 : 2;
    work_decimal *max = (whoismax == 1) ? value1 : value2;
    work_decimal *min = (whoismax == 2) ?  value1 : value2;
    int16_t expdif = max -> exp - min -> exp;
    for (; expdif > 0 && overflow == 0; --expdif) overflow = addlast(min);
    if (overflow) {
      overflow  = 0;
      for (; expdif >= 0; --expdif) {
        rounding = dellast(max);
        if (rounding) {
          overflow = 1;
          countround++;
        }
      }
      if (overflow) {
        if (rounding < 5) rounding = (whoismax == 1) ? FLOORFIRST : FLOORSECOND;
        else if (rounding == 5) {
          if (countround > 1) rounding = (whoismax == 1) ? CEILFIRST :
CEILSECOND; else { if(max -> bits[0] & 1) rounding = (whoismax == 1) ? CEILFIRST
: CEILSECOND; else rounding = (whoismax == 1) ? FLOORFIRST : FLOORSECOND;
          }
        }
        else rounding = (whoismax == 1) ? CEILFIRST : CEILSECOND;
      }
    }
  }
  return rounding;
} */
unsigned int pointequalize(work_decimal *value1, work_decimal *value2) {
  unsigned int overflow = 0;
  if (value1->exp != value2->exp) {
    int whoismax = (value1->exp > value2->exp) ? 1 : 2;
    work_decimal *max = (whoismax == 1) ? value1 : value2;
    work_decimal *min = (whoismax == 2) ? value1 : value2;
    int16_t expdif = max->exp - min->exp;
    for (; expdif > 0 && overflow == 0; --expdif) overflow = addlast(min);
  }
  return overflow;
}

int bankround(work_decimal value, unsigned int remander,
              unsigned int countround) {
  int roundup = 0;
  if (remander > 5)
    roundup = 1;
  else if (remander == 5) {
    if (countround > 1)
      roundup = 1;
    else if (value.bits[0] & 1)
      roundup = 1;
  }
  return roundup;
}

int needdown(work_decimal value) {
  int need = 0;
  for (uint16_t i = 3; i < WORKBITS; ++i) need |= (value.bits[i] != 0);
  if (need == 0) need = value.exp > MAXEXP;
  return need;
}

int normalize(work_decimal *value) {
  int overflow = 0;
  unsigned int remainder = 0, countround = 0;
  for (int16_t i = 0; i < WORKBITS - 1; ++i) {
    value->bits[i + 1] += getoverflow(value->bits[i]);
    value->bits[i] = bitwithoutover(value->bits[i]);
  }
  while (needdown(*value) && value->exp > 0) {
    remainder = dellast(value);
    if (remainder) countround++;
  }
  overflow = needdown(*value);
  if (bankround(*value, remainder, countround) && overflow == 0) {
    if (addnum(value, 1)) overflow = 1;
    if (needdown(*value)) {
      if (value->exp > 0) {
        dellast(value);
        addnum(value, 1);
      } else
        overflow = 1;
    }
  }
  if (value->exp > 0 && overflow == 0) {
    work_decimal foo = *value;
    int doing = 1;
    remainder = 0;
    while (doing) {
      if (value->exp > 0) {
        remainder = dellast(&foo);
        if (remainder == 0)
          *value = foo;
        else
          doing = 0;
      } else
        doing = 0;
    }
  }
  return overflow;
}

int compearbits(work_decimal value_1, work_decimal value_2) {
  int compear = 0;
  for (int16_t i = WORKBITS - 1; i >= 0 && compear == 0; --i) {
    if (value_1.bits[i] != value_2.bits[i])
      compear = (value_1.bits[i] > value_2.bits[i]) ? 1 : -1;
  }
  return compear;
}

unsigned int addnum(work_decimal *value, uint16_t num) {
  unsigned int overflow = num;
  work_decimal foo = *value;
  for (uint16_t i = 0; i < WORKBITS; ++i) {
    foo.bits[i] += overflow;
    overflow = getoverflow(foo.bits[i]);
    foo.bits[i] = bitwithoutover(foo.bits[i]);
  }
  if (overflow == 0) *value = foo;
  return overflow;
}

work_decimal addbits(work_decimal value_1, work_decimal value_2) {
  unsigned int overflow = 0;
  work_decimal res = value_1;
  for (uint16_t i = 0; i < WORKBITS; ++i) {
    res.bits[i] += value_2.bits[i] + overflow;
    overflow = getoverflow(res.bits[i]);
    res.bits[i] = bitwithoutover(res.bits[i]);
  }
  return res;
}

void sumworkbits(work_decimal *value, uint32_t index, uint64_t add1,
                 uint64_t add2, uint64_t add3) {
  uint64_t add[3] = {add1, add2, add3};
  for (uint16_t i = 0; i < 3; ++i) {
    if (value->bits[index] > ULLONG_MAX - add[i]) {
      value->bits[index + 1] += MAX4BIT + 1L;
      value->bits[index] -= ULLONG_MAX - add[i] + 1;
    } else {
      value->bits[index] += add[i];
    }
  }
}

void twos_complement_bits(work_decimal *value) {
  for (uint16_t i = 0; i < WORKBITS; ++i) {
    value->bits[i] = ~value->bits[i];
    value->bits[i] &= MAX4BIT;
  }
  addnum(value, 1);
}

work_decimal subbits(work_decimal value_1, work_decimal value_2) {
  work_decimal res;
  twos_complement_bits(&value_2);
  res = addbits(value_1, value_2);
  return res;
}

work_decimal shiftleft(work_decimal value, uint16_t shift) {
  work_decimal res = initwork();
  uint16_t numbits = shift / 32;
  shift %= 32;
  for (int16_t i = WORKBITS - 1; i >= numbits; --i)
    res.bits[i] = value.bits[i - numbits];
  for (int16_t i = WORKBITS - 1; i >= 0; --i) res.bits[i] <<= shift;
  for (int16_t i = WORKBITS - 1; i > 0; --i)
    res.bits[i] += getoverflow(res.bits[i - 1]);
  for (int16_t i = WORKBITS - 1; i >= 0; --i)
    res.bits[i] = bitwithoutover(res.bits[i]);
  return res;
}

work_decimal divmain(work_decimal v_1, work_decimal v_2, work_decimal *res) {
  res->exp = 0;
  for (int16_t i = 2; i >= 0; --i) {
    for (int16_t j = 31; j >= 0; --j) {
      work_decimal foo = shiftleft(v_2, i * 32 + j);
      int compear = compearbits(v_1, foo);
      if (compear >= 0) {
        res->bits[i] += (uint64_t)1 << j;
        v_1 = subbits(v_1, foo);
        if (compear == 0) {
          j = -1;
          i = -1;
        }
      }
    }
  }
  return v_1;
}

/* uint16_t getfirstbit(work_decimal value) {
  uint16_t count = WORKBITS - 1;
  for (; value.bits[count] == 0 && count > 0; --count);
  for (int16_t i = 31; i > 0; --i) {
    if (value.bits[count] >=  (uint32_t)1 << i) {
      count = count * 32 + i;
      i = 0;
    }
  }
  return count;
} */

work_decimal divremain(work_decimal v_1, work_decimal v_2) {
  for (int16_t i = 2; i >= 0; --i) {
    for (int16_t j = 31; j >= 0; --j) {
      work_decimal foo = shiftleft(v_2, i * 32 + j);
      int compear = compearbits(v_1, foo);
      if (compear >= 0) {
        v_1 = subbits(v_1, foo);
        if (compear == 0) {
          j = -1;
          i = -1;
        }
      }
    }
  }
  return v_1;
}

void divtail(work_decimal v_1, work_decimal v_2, work_decimal *res) {
  unsigned int stop = 0;
  for (uint16_t i = 0; i < MAXEXP + 1 && stop == 0; ++i) {
    stop = bits10up(res);
    if (stop == 0) {
      res->exp += 1;
      bits10up(&v_1);
      uint16_t add = 0;
      for (int16_t j = 3; j >= 0; --j) {
        work_decimal foo = shiftleft(v_2, j);
        int compear = compearbits(v_1, foo);
        if (compear >= 0) {
          add += (uint64_t)1 << j;
          v_1 = subbits(v_1, foo);
          if (compear == 0) {
            j = -1;
            stop = 1;
          }
        }
      }
      addnum(res, add);
    }
  }
}

/*
int checkoverflowmult(work_decimal res) {
  int overflow = 1;
  work_decimal foo = initwork();
  foo.bits[0] = 1;
  for (uint16_t i = 0; i < res.exp; ++i) bits10upe(&foo);
  if (foo.bits[5] || foo.bits[4]) overflow = 0;
  else {
    overflow = !(res.bits[5] == 0 || foo.bits[3] > res.bits[5]);
    overflow |= !(res.bits[4] == 0 || foo.bits[2] > res.bits[4]);
    overflow |= !(res.bits[3] == 0 || foo.bits[1] > res.bits[3]);
  }
  return overflow;
} */

int char2num(char c) { return c - '0'; }

x_decimal set21(int bits3, int bits2, int bits1, int bits0) {
  x_decimal value;
  value.bits[0] = bits0 & MAX4BIT;
  value.bits[1] = bits1 & MAX4BIT;
  value.bits[2] = bits2 & MAX4BIT;
  value.bits[3] = bits3 & MAX4BIT;
  return value;
}

work_decimal initwork() {
  work_decimal value;
  value.exp = 0;
  for (uint16_t i = 0; i < WORKBITS; ++i) value.bits[i] = 0;
  return value;
}

void print_s21(x_decimal value, int type) {
  printf("%x ", value.bits[3]);
  if (type == 0)
    for (int16_t i = 2; i >= 0; --i) printf("%x ", value.bits[i]);
  else
    for (int16_t i = 2; i >= 0; --i) printf("%u ", value.bits[i]);
  printf("\n");
}

void print_work(work_decimal value, int type) {
  printf("exp = %u (%#x) bits = ", value.exp, value.exp);
#ifdef __linux__
  if (type == 0)
    for (int16_t i = WORKBITS - 1; i >= 0; --i) printf("%lx ", value.bits[i]);
  else
    for (int16_t i = WORKBITS - 1; i >= 0; --i) printf("%lu ", value.bits[i]);
#else
  if (type == 0)
    for (int16_t i = WORKBITS - 1; i >= 0; --i) printf("%llx ", value.bits[i]);
  else
    for (int16_t i = WORKBITS - 1; i >= 0; --i) printf("%llu ", value.bits[i]);
#endif
  printf("\t");
}
