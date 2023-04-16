#include "../drivers/uart0.h"

int kernel_main(void) {

  uart_init();

  uart_puts("Hello World");

  return 0;
}