#include "uart_lower.h"
#include <stddef.h>
#include <stdint.h>

/**
 * Private Methods
 *
 */
void lower_mmio_write(uint32_t reg, uint32_t data)__attribute__((section(".multiboot.text")));
uint32_t lower_mmio_read(uint32_t reg)__attribute__((section(".multiboot.text")));
void lower_delay(int32_t count)__attribute__((section(".multiboot.text")));

// Memory-Mapped I/O output
void lower_mmio_write(uint32_t reg, uint32_t data) {
    *(volatile uint32_t*) reg = data;
}

// Memory-Mapped I/O input
uint32_t lower_mmio_read(uint32_t reg) {
    return *(volatile uint32_t*) reg;
}

// Loop <lower_delay> times in a way that the compiler won't optimize away
void lower_delay(int32_t count) {
    __asm__ volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                     : "=r"(count)
                     : [ count ] "0"(count)
                     : "cc");
}

/**
 * Public Methods
 */

void lower_uart_init() {
    // Disable UART0.
    lower_mmio_write(UART0_CR, 0x00000000);
    // Setup the GPIO pin 14 && 15.

    // Disable pull up/down for all GPIO pins & lower_delay for 150 cycles.
    lower_mmio_write(GPPUD, 0x00000000);
    lower_delay(150);

    // Disable pull up/down for pin 14,15 & lower_delay for 150 cycles.
    lower_mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    lower_delay(150);

    // Write 0 to GPPUDCLK0 to make it take effect.
    lower_mmio_write(GPPUDCLK0, 0x00000000);

    // Clear pending interrupts.
    lower_mmio_write(UART0_ICR, 0x7FF);

    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.

    // Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
    lower_mmio_write(UART0_IBRD, 1);
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    lower_mmio_write(UART0_FBRD, 40);

    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    lower_mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // Mask all interrupts.
    lower_mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    // Enable UART0, receive & transfer part of UART.
    lower_mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

void lower_uart_putc(unsigned char c) {
    // Wait for UART to become ready to transmit.
    while (lower_mmio_read(UART0_FR) & (1 << 5)) {
    }
    lower_mmio_write(UART0_DR, c);
}

unsigned char lower_uart_getc() {
    // Wait for UART to have received something.
    while (lower_mmio_read(UART0_FR) & (1 << 4)) {
    }
    return lower_mmio_read(UART0_DR);
}

void lower_uart_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        lower_uart_putc((unsigned char) str[i]);
}

void lower_hexstrings(uint32_t d) {
    // uint32_t ra;
    uint32_t rb;
    uint32_t rc;

    rb = 32;
    while (1) {
        rb -= 4;
        rc = (d >> rb) & 0xF;
        if (rc > 9)
            rc += 0x37;
        else
            rc += 0x30;
        lower_uart_putc(rc);
        if (rb == 0)
            break;
    }
    lower_uart_putc(0x20);
}
