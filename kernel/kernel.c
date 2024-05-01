#include <zeonos/drivers/uart0.h>
#include <zeonos/kernel/hello.hpp>

extern char *my_function(int arg1, int arg2);

extern void start_secondary_core(unsigned int core_id, unsigned int exec_address);
static inline void delay(int count)
{
  __asm__ volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                   : "=r"(count)
                   : [count] "0"(count)
                   : "cc");
}

void marco(void)
{
  uart_puts("Hello World zeonos..! \n cpu_id ");
  hexstrings(read_cpu_id() & 0x03);
  uart_putc('\n');
}

void polo(void)
{
  uart_puts("Hello World zeonos..! \n cpu_id ");
  hexstrings(read_cpu_id() & 0x03);
  uart_putc('\n');
}

void caught(void)
{
  uart_puts("Hello World zeonos..! \n cpu_id ");
  hexstrings(read_cpu_id() & 0x03);
  uart_putc('\n');
}

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

  uart_puts("starting secondary cores \n");

  delay(1000 * 1000 * 1000 * 1);
  start_secondary_core(1, marco);

  delay(1000 * 1000 * 1000 * 2);
  start_secondary_core(2, polo);

  delay(1000 * 1000 * 1000 * 3);
  start_secondary_core(3, caught);

  // Kernel Main should not return
  while (1)
    ;
}