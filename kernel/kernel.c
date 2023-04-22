#include <zeonos/drivers/uart0.h>
#include "hello.hpp"

extern char * my_function(int arg1, int arg2);

int kernel_main(void) {

  uart_init();
  #ifdef __is_zeonos_kernel
    uart_puts("Hello World zeonos..!");
  #else
    uart_puts("Hello World stranger..!");
  #endif
  
  uart_puts(my_function(get_1(), 0));
  return 0;
}