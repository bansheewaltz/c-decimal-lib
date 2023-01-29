#include <stdio.h>
#include <stdlib.h>

int test() {
  double a = 13;
  double b = 7;
  char operation = '/';
  double expected_result = a / b;

  char test_case[256] = "";
  sprintf(test_case, "mono tester.exe %f %c %f %.28f",  //
          a, operation, b, expected_result);
  system(test_case);
}

int main() {
  test();
}
