#include <zeonos/drivers/uart0.h>
#include <zeonos/kernel/hello.hpp>

extern int __bss_start;
extern int __bss_end;
extern void (*__init_start) (void);
extern void (*__init_end) (void);

void _clear_bss()
{
    int* bss = &__bss_start;
    int* bss_end = &__bss_end;

    while( bss < bss_end )
        *bss++ = 0;
}

void _init_global_constructors() {
	for (void (**global_constructor) (void) = &__init_start; global_constructor < &__init_end; global_constructor++)
	{
		(**global_constructor)(); // Calling default global constructor
	}
}

extern char * my_function(int arg1, int arg2);

int kernel_main(void) {
  _clear_bss();
  _init_global_constructors();

  uart_init();


  #ifdef __is_zeonos_kernel
    uart_puts("Hello World zeonos..! \n cpu_id ");
    hexstrings(read_cpu_id() & 0x03 );
    uart_putc('\n');
  #else
    uart_puts("Hello World stranger..!");
  #endif
  
  uart_puts(my_function(1, 0));
  uart_putc('\n');

  // Kernel Main should not return
  while (1); 
  return 0;
}