extern void kernel_main(void);

// Check arch_asm_routines.S
extern void arch_enable_fiq();
extern void arch_enable_irq();
extern void arch_enable_vfp();

extern int __bss_start;
extern int __bss_end;
extern void (*__init_start)(void);
extern void (*__init_end)(void);

void _clear_bss()
{
    int *bss = &__bss_start;
    int *bss_end = &__bss_end;

    while (bss < bss_end)
        *bss++ = 0;
}

void _init_global_constructors()
{
    for (void (**global_constructor)(void) = &__init_start; global_constructor < &__init_end; global_constructor++)
    {
        (**global_constructor)(); // Calling default global constructor
    }
}

static inline void delay(int count)
{
    __asm__ volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                     : "=r"(count)
                     : [count] "0"(count)
                     : "cc");
}

void before_kernel_primary_cpu_entry(void)
{
    arch_enable_fiq();
    arch_enable_irq();
    arch_enable_vfp();

    _clear_bss();
    _init_global_constructors();

    kernel_main();

    // No return
}

void before_kernel_secondary_cpu_entry(unsigned int cpu_id)
{
    arch_enable_fiq();
    arch_enable_irq();
    arch_enable_vfp();

    delay(1000 * 1000 * 1000 * cpu_id);

    uart_puts("Hello World zeonos..! from secondary cores \n cpu_id ");
    hexstrings(read_cpu_id() & 0x03);
    uart_putc('\n');
    while (1)
        ;
}