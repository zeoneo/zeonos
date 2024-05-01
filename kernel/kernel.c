#include <zeonos/drivers/uart0.h>
#include <zeonos/kernel/hello.hpp>

extern char *my_function(int arg1, int arg2);

void kernel_main(void)
{

  uart_init();

#ifdef __is_zeonos_kernel
  uart_puts("Hello World zeonos..! \n cpu_id ");
  hexstrings(read_cpu_id() & 0x03);
  uart_putc('\n');
#else
  uart_puts("Hello World stranger..!");
#endif

  uart_puts(my_function(1, 0));
  uart_putc('\n');

  // Kernel Main should not return
  while (1)
    ;
}