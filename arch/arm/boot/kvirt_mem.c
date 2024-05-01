extern unsigned int __mmu_table_base;

void mmu_section(unsigned int MMUTABLEBASE, unsigned int vadd, unsigned int padd, unsigned int flags) __attribute__((section(".multiboot.text")));

static char *hello = "lower_uart_puts";

void initialize_virtual_memory() __attribute__((section(".multiboot.text")));

void initialize_virtual_memory(unsigned int MMUTABLEBASE)
{
    /* Not needed now
    unsigned int ra;
    for (ra = 0;; ra += 0x00100000)
    {
        mmu_section(ra, ra, 0x0000);
        if (ra == 0x08000000)
            break;
    }
    */

    // lower_hexstrings((unsigned int)&__mmu_table_base);
    // unsigned int MMUTABLEBASE = 0x00030000;

    // This mapping works.
    mmu_section(MMUTABLEBASE, 0x00000000, 0x00000000, 0x0000);
    mmu_section(MMUTABLEBASE, 0x00000000 + MMUTABLEBASE, 0x00000000 + MMUTABLEBASE, 0x0000);
    mmu_section(MMUTABLEBASE, 0xC0000000, 0x00000000, 0x0000);
    mmu_section(MMUTABLEBASE, 0xC0000000 + MMUTABLEBASE, 0x00000000 + MMUTABLEBASE, 0x0000);

    // peripherals
    mmu_section(MMUTABLEBASE, 0x3f000000, 0x3f000000, 0x0000); // NOT CACHED!
    mmu_section(MMUTABLEBASE, 0x3f000000 + 0xC0000000, 0x3f000000, 0x0000);

    mmu_section(MMUTABLEBASE, 0x3f200000, 0x3f200000, 0x0000); // NOT CACHED!
    mmu_section(MMUTABLEBASE, 0x3f200000 + 0xC0000000, 0x3f200000, 0x0000);
    // lower_uart_putc('\n');
    // lower_uart_putc('b');
    // lower_uart_putc(':');
    // lower_hexstrings(MMUTABLEBASE);

    // start_mmu(MMUTABLEBASE, 0x00000005);
    asm volatile(
        "mov r0, %0\n"   // Move MMUTABLEBASE into r0
        "mov r1, %1\n"   // Move config into r1
        "bl start_mmu\n" // Branch with link to start_mmu
        :
        : "r"(MMUTABLEBASE), "r"(0x00000005)
        : "r0", "r1", "memory", "cc");
}

void mmu_section(unsigned int MMUTABLEBASE, unsigned int vadd, unsigned int padd, unsigned int flags)
{
    unsigned int table1EntryOffset;
    unsigned int table1EntryAddress;
    unsigned int tableEntry;

    table1EntryOffset = (vadd >> 20) << 2; // get only most significant 12 bits
    // and multiply it by 4 as each entry is 4 Bytes 32bits

    // MMU table base should be at 16KB granularity, Least signficant 12 bits will be always 0. hence do OR with that
    table1EntryAddress = MMUTABLEBASE | table1EntryOffset;

    // 31: 20  12 bits are physical 12 ms bits from physical address
    tableEntry = (padd & 0xFFF00000);

    // entry[1:0] = 0b10 for section entry
    tableEntry = tableEntry | 2;

    // Access permissions should be 11 for full access entry [11:10] = 0b11
    tableEntry = tableEntry | 0xC00;
    // lower_uart_puts("table_entry:");
    // lower_uart_putc('\n');
    // lower_hexstrings(table1EntryAddress);
    // hexstring(rc);
    //  printf("\n entryAddr: 0x%x, entry value:0x%x \n", table1EntryAddress, tableEntry);
    PUT32(table1EntryAddress, tableEntry);
}

void unmap_identity(unsigned int MMUTABLEBASE)
{
    unmap_mmu_section(MMUTABLEBASE, 0x00000000);
    unmap_mmu_section(MMUTABLEBASE, 0x00000000 + MMUTABLEBASE);
    unmap_mmu_section(MMUTABLEBASE, 0x3f000000);
    unmap_mmu_section(MMUTABLEBASE, 0x3f200000);
}

void unmap_mmu_section(unsigned int MMUTABLEBASE, unsigned int vadd)
{
    unsigned int table1EntryAddress;
    table1EntryAddress = MMUTABLEBASE | (vadd >> 20) << 2;
    table1EntryAddress += 0xC0000000;
    *((unsigned int *)table1EntryAddress) = 0;
}