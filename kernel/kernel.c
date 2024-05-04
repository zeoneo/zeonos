#include <zeonos/drivers/uart0.h>
#include <zeonos/kernel/hello.hpp>
#include <stddef.h>
// #include <stdio.h>

extern void *memmove(void *, const void *, size_t);
extern char *my_function(int arg1, int arg2);

extern void start_secondary_core(unsigned int core_id, unsigned int exec_address);
static inline void delay(int count)
{
  __asm__ volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                   : "=r"(count)
                   : [count] "0"(count)
                   : "cc");
}

const char *a = "prakash";
char b[8] = {0};
void marco(void)
{
  uart_puts("(marco) Hello World..! cpu_id ");
  hexstrings(read_cpu_id() & 0x03);
  uart_putc('\n');
}

void polo(void)
{
  uart_puts("(polo) Hello World..! cpu_id ");
  hexstrings(read_cpu_id() & 0x03);
  uart_putc('\n');
}

void caught(void)
{
  uart_puts("(caught) Hello World..! cpu_id ");
  hexstrings(read_cpu_id() & 0x03);
  uart_putc('\n');

  b[7] = '\0';
  uart_puts(a);
  memcpy(b, a, 4);
  uart_puts(b);
}

void kernel_main(void)
{

  uart_init();

#ifdef __is_zeonos_kernel
  uart_puts("Hello World zeonos..! cpu_id ");
  hexstrings(read_cpu_id() & 0x03);
  uart_putc('\n');
#else
  uart_puts("Hello World stranger..!");
#endif

  uart_puts(my_function(1, 0));
  uart_putc('\n');

  uart_puts("starting secondary cores \n");

  delay(1000 * 1000 * 1);
  start_secondary_core(1, marco);

  delay(1000 * 1000 * 2);
  start_secondary_core(2, polo);

  delay(1000 * 1000 * 3);
  start_secondary_core(3, caught);

  // Kernel Main should not return
  while (1)
    ;
}