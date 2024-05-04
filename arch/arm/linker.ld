
ENTRY(_start)

KERNEL_LOADER_BASE = 0x8000;

KERNEL_LINKER_BASE = 0xC0000000;


SECTIONS
{
    /* Starts at LOADER_ADDR. */
    . =  KERNEL_LOADER_BASE;
    __start = .;
    __text_start = .;
    .multiboot.data : {
        *(.multiboot.data)
    }

    .multiboot.text : {
        *(.multiboot.text)
    }
    . += KERNEL_LINKER_BASE;

    .text ALIGN(4K) : AT (ADDR(.text) - KERNEL_LINKER_BASE) {
        *(.text.boot)
        *(.text)
    }
    . = ALIGN(4096); /* align to page size */
    __text_end = .;
    
    __init_start = .;
    .init ALIGN (4K) : AT (ADDR (.init) - KERNEL_LINKER_BASE) {
		*(.init)
	}
    __init_end = .;

    __rodata_start = .;
    .rodata ALIGN (4K) : AT (ADDR (.rodata) - KERNEL_LINKER_BASE) {
        *(.rodata)
    }
    . = ALIGN(4096); /* align to page size */
    __rodata_end = .;
    
    .init_array ALIGN (4K) : AT (ADDR (.init_array) - KERNEL_LINKER_BASE) {
		__init_start = .;
		KEEP(*(.init_array*))
		__init_end = .;
	}

    .ARM.exidx ALIGN (4K) : AT (ADDR (.ARM.exidx) - KERNEL_LINKER_BASE) {
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
	}
    
    __data_start = .;
    .data ALIGN (4K) : AT (ADDR (.data) - KERNEL_LINKER_BASE) {
        *(.data)
    }
    . = ALIGN(4096); /* align to page size */
    __data_end = .;
 
    __bss_start = .;
    .bss ALIGN (4K) (NOLOAD): AT (ADDR (.bss) - KERNEL_LINKER_BASE) {
        bss = .;
        *(.bss)
        . = ALIGN(4096); /* align to page size */
        *(.heap)
    }
    . = ALIGN(4096); /* align to page size */
    __bss_end = .;

    /* Exclude the BSS section */
    . = ALIGN(16384);
    
    . = . + 8192K; /* 8MB for 4 cores */
    __svc_stack_base = .;

    . = . + 128K; /* 1024 Bytes for undef stack */
    __undef_stack_base = .;

    . = . + 128K; /* 1024 Bytes for irq stack */
    __irq_stack_base = .;

    . = . + 128K; /* 1024 Bytes for irq stack */
    __fiq_stack_base = .;

    

    . = ALIGN(16384); /* align 16KB for page tables */
    __first_lvl_tbl_base = .;
    __mmu_table_base = __first_lvl_tbl_base - KERNEL_LINKER_BASE;

    . = . + 4K; /* First level page table size */
    __first_lvl_tbl_end = .;

    . = ALIGN(1024); /* Align address here on 1KB boundary for 2nd level page table */
    /* 1KB for each 2nd lvl page table = 4 Bytes * 256 */
    /* How many 2nd lvl tables ? */
    __second_lvl_tbl_base = .;
    . = . + 1048576; /* 1MB space for 2nd lvl tables. */
    __second_lvl_tbl_end = .;
    . = ALIGN(4K);

    
    __kernel_end = .;
    
}