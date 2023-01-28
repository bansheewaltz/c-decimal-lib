#include <stdio.h>
#include <stdlib.h>

int test() {
  int a = 17;
  int b = 7;
  char operation = '/';
  double expected_result = (double)(17 / 7);

  char* filename = "./test_sample.txt";
  FILE* file = fopen(filename, "w");
  fprintf(file, "%i\n%c\n%i\n%lf\n", a, operation, b, expected_result);
  fclose(file);
  system("mono tester.exe");
}

int main() {
  test();
}
