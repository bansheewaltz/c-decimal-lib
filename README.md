### Realization:

```c
typedef struct {
  int bits[4];
} x_decimal;
```

### Arithmetic Operators

| Operator name | Operators  | Function                                                                           | 
| ------ | ------ |------------------------------------------------------------------------------------|
| Addition | + | int x_add(x_decimal value_1, x_decimal value_2, x_decimal *result)         |
| Subtraction | - | int x_sub(x_decimal value_1, x_decimal value_2, x_decimal *result) |
| Multiplication | * | int x_mul(x_decimal value_1, x_decimal value_2, x_decimal *result) | 
| Division | / | int x_div(x_decimal value_1, x_decimal value_2, x_decimal *result) |
| Modulo | Mod | int x_mod(x_decimal value_1, x_decimal value_2, x_decimal *result) |

The functions return the error code:
- 0 - OK
- 1 - the number is too large or equal to infinity
- 2 - the number is too small or equal to negative infinity
- 3 - division by 0

*Note on the numbers that do not fit into the mantissa:*
- *When getting numbers that do not fit into the mantissa during arithmetic operations, use bank rounding (for example, 79,228,162,514,264,337,593,543,950,335 - 0.6 = 79,228,162,514,264,337,593,543,950,334)*

*Note on the mod operation:*
- *If an overflow occurred as a result, discard the fractional part (for example, 70,000,000,000,000,000,000,000,000,000 % 0.001 = 0.000)*


### Comparison Operators

| Operator name | Operators  | Function | 
| ------ | ------ | ------ |
| Less than | < | int x_is_less(x_decimal, x_decimal) |
| Less than or equal to | <= | int x_is_less_or_equal(x_decimal, x_decimal) | 
| Greater than | > |  int x_is_greater(x_decimal, x_decimal) |
| Greater than or equal to | >= | int x_is_greater_or_equal(x_decimal, x_decimal) | 
| Equal to | == |  int x_is_equal(x_decimal, x_decimal) |
| Not equal to | != |  int x_is_not_equal(x_decimal, x_decimal) |

Return value:
- 0 - FALSE
- 1 - TRUE

### Convertors and parsers

| Convertor/parser | Function | 
| ------ | ------ |
| From int  | int x_from_int_to_decimal(int src, x_decimal *dst) |
| From float  | int x_from_float_to_decimal(float src, x_decimal *dst) |
| To int  | int x_from_decimal_to_int(x_decimal src, int *dst) |
| To float  | int x_from_decimal_to_float(x_decimal src, float *dst) |

Return value - code error:
- 0 - OK
- 1 - convertation error

*Note on the conversion of a float type number:*
- *If the numbers are too small (0 < |x| < 1e-28), return an error and value equal to 0*
- *If the numbers are too large (|x| > 79,228,162,514,264,337,593,543,950,335) or are equal to infinity, return an error*
- *When processing a number with the float type, convert all the significant decimal digits contained in it. If there are more than 7 such digits, the number is rounded to the closest one that does not have more than 7 significant decimal digits.*

*Note on the conversion from decimal type to int:*
- *If there is a fractional part in a decimal number, it should be discarded (for example, 0.9 is converted to 0)*


### Another functions

| Description | Function                                                         | 
| ------ |------------------------------------------------------------------|
| Rounds a specified Decimal number to the closest integer toward negative infinity. | int x_floor(x_decimal value, x_decimal *result)            |	
| Rounds a decimal value to the nearest integer. | int x_round(x_decimal value, x_decimal *result)    |
| Returns the integral digits of the specified Decimal; any fractional digits are discarded, including trailing zeroes. | int x_truncate(x_decimal value, x_decimal *result) |
| Returns the result of multiplying the specified Decimal value by negative one. | int x_negate(x_decimal value, x_decimal *result)   |

Return value - code error:
- 0 - OK
- 1 - calculation error
