// To keep this in the first portion of the binary.
 
// Make _start global.
.global _start
.global _get_stack_pointer
.global _exception_table
.global _enable_interrupts


// ARM Processor Modes and Registers
// Cortex A series programmers guidee
// Search - Table 3-2 ARMv7 processor modes
.equ ARM_MODE_USR, 0x10;								;@ CPU in USR mode .. Normal User mode
.equ ARM_MODE_FIQ, 0x11;								;@ CPU in FIQ mode .. FIQ Processing
.equ ARM_MODE_IRQ, 0x12;								;@ CPU in IRQ mode .. IRQ Processing
.equ ARM_MODE_SVC, 0x13;								;@ CPU in SVC mode .. Service mode
.equ ARM_MODE_HYP, 0x1A;								;@ CPU in HYP mode .. Hypervisor mode  (ARM7/ARM8 only)
.equ ARM_MODE_UND, 0x1B;								;@ CPU in UND mode .. Undefined Instructions mode
.equ ARM_MODE_SYS, 0x1F;	

.equ ARM_MODE_MASK, 0x1F;								;@ Mask to clear all but CPU mode bits from cpsr register
.equ ARM_I_BIT,		0x80;								;@ IRQs disabled when set to 1
.equ ARM_F_BIT,		0x40;


// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

.equ	SCTLR_ENABLE_DATA_CACHE,        0x4
.equ	SCTLR_ENABLE_BRANCH_PREDICTION, 0x800
.equ	SCTLR_ENABLE_INSTRUCTION_CACHE, 0x1000

.equ    MMUTABLEBASE ,                  0x00004000

// Declare constants for the multiboot header.
.equ ALIGN,    1<<0             // align loaded modules on page boundaries
.equ MEMINFO,  1<<1             // provide memory map
.equ MULTIBOOT_HEADER_FLAGS,    ALIGN | MEMINFO  // this is the Multiboot 'flag' field
.equ MULTIBOOT_HEADER_MAGIC,    0x1BADB002       // 'magic number' lets bootloader find the header
.equ CHECKSUM, -(MULTIBOOT_HEADER_FLAGS + MULTIBOOT_HEADER_MAGIC) // checksum of above, to prove we are multiboot

// Declare a multiboot header that marks the program as a kernel.

.section ".multiboot.data", "aw"

.align 4
.word MULTIBOOT_HEADER_MAGIC
.word MULTIBOOT_HEADER_FLAGS
.word CHECKSUM

.section ".multiboot.text", "aw"
.balign 4
_start:
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

	_reset_h:                           .word   _reset_
	_undefined_instruction_vector_h:    .word   _reset_h
	_software_interrupt_vector_h:       .word   _reset_h
	_prefetch_abort_vector_h:           .word   _reset_h
	_data_abort_vector_h:               .word   _reset_h
	_unused_handler_h:                  .word   _reset_h
	_interrupt_vector_h:                .word   _reset_h
	_fast_interrupt_vector_h:           .word   _reset_h


_reset_:

// CORTEX A53 Guide - MPIDR, Multiprocessor Affinity Register
//
    mrc p15, 0, r6,c0,c0,5
    and     r6, r6, #3
    mov r7, #0
    cmp     r6, r7
    beq 2f
    // cpu id > 0, stop
1:  wfe
    b       1b
2:  // cpu id == 0


	mov r12, pc											
	mrs r0, CPSR										
	orr r0, r0, #(ARM_I_BIT | ARM_F_BIT)				// Disable Irq/Fiq
	and r11, r0, #ARM_MODE_MASK							//Clear all but CPU mode bits in register r11

 	cmp r11, #ARM_MODE_HYP								// Check we are in HYP_MODE											
	bne .start_el1_svc_mode								// Branch if not equal meaning was not in HYP_MODE  
	bic r0, r0, #ARM_MODE_MASK							// Clear the CPU mode bits in register r0							
	orr r0, r0, #ARM_MODE_SVC							// ARM_MODE_SVC bits onto register	
    msr spsr_cxsf,r0									// Hold value in spsr_cxsf
    add lr,pc,#4										// Calculate address of .NotInHypMode label

	msr ELR_hyp, lr
	eret

.start_el1_svc_mode:

    // Setup el1 stack to run the code.
    mov r0,#0xD3
    msr cpsr_c,r0
    ldr sp, =__svc_stack_base - 0xc0000000

    // enable unaligned address access
	mrc p15, 0, r0, c1, c0, 0
	orr r0, #1 << 22
	mcr p15, 0, r0, c1, c0, 0

    // Enable VFP ------------------------------------------------------------
    // r1 = Access Control Register
    mrc p15, #0, r1, c1, c0, #2
    // enable full access for p10,11
    orr r1, r1, #(0xf << 20)
    // ccess Control Register = r1
    mcr p15, #0, r1, c1, c0, #2
    mov r1, #0
    // flush prefetch buffer because of FMXR below
    mcr p15, #0, r1, c7, c5, #4
    // and CP 10 & 11 were only just enabled
    // Enable VFP itself
    mov r0, #0x40000000
    fmxr FPEXC, r0
    bl lower_uart_init
    mov r0, #0x1234
    bl lower_hexstrings
    bl initialize_virtual_memory
    bl invalidate_tlbs
    mov r0, #0x4321
    bl lower_hexstrings
    ldr r0, =higher_kernel  // move the address to register r0
    bx  r0               // jump to the address in r0

    hlt
_inf_loop:
    b       _inf_loop

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl init_virt_mem
init_virt_mem:
    push {r0-r1}
    pop {r0-r1}
    bx lr

.globl map_section
map_section:
    push {r0-r1}
    pop {r0-r1}
    bx lr

.globl start_mmu
start_mmu:
    mov r2,#0
    ;@mcr p15,0,r2,c7,c7,0 ;@ invalidate caches
    mcr p15,0,r2,c8,c7,0 ;@ invalidate tlb
    mcr p15,0,r2,c7,c10,4 ;@ DSB ??

     ;@ Domain 0 Manager access. No permissions checked for access
    MRC p15, 0, r2, c3, c0, 0 ;@ Read DACR into Rt
    ORR r2, #3
    MCR p15, 0, r2, c3, c0, 0 ;@ Write Rt to DACR

    MCR p15,0,r0,c2,c0,0 ;@ Write Rt to TTBR0
    MCR p15, 0, r0, c2, c0, 1 ;@ Write Rt to TTBR1

    MRC p15, 0, r2, c1, c0, 0 ;@ Read SCTLR into Rt
    orr r2,r2,r1
    MCR p15, 0, r2, c1, c0, 0 ;@ Write Rt to SCTLR.

    bx lr

.globl invalidate_tlbs
invalidate_tlbs:
    mov r2,#0
    mcr p15,0,r2,c8,c7,0  ;@ invalidate tlb
    mcr p15,0,r2,c7,c10,4 ;@ DSB ??
    bx lr


.section ".text.boot"
.global read_cpu_id
read_cpu_id:
    // read cpu id, stop slave cores
    mrc p15, 0, r0,c0,c0,5
    bx lr

.global higher_kernel
higher_kernel:
    ldr sp, =__svc_stack_base
    bl unmap_identity
    bl kernel_main
