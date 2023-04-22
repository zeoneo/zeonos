#include "../drivers/uart0.h"
#include "hello.hpp"

extern char * my_function(int arg1, int arg2);

int kernel_main(void) {

  uart_init();

  uart_puts("Hello World");
  uart_puts(my_function(get_1(), 0));
  return 0;
}