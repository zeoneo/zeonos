#include <stdio.h>

int main(void) {
  int x = 0;
  return x;
}

int foo(int x) {
  int buf[10] = {0};
  if (x == 5)
    buf[x] = 0; // <- ERROR
  return buf[x];
}