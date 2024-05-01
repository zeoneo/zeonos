.global arch_enable_fiq
arch_enable_fiq:
    // Another way to do it https://github.com/zeoneo/rpi-3b-wifi/blob/master/src/boot.S#L174C1-L178C19 
    // Simple instruction
    cpsie f
    bx lr

.global arch_enable_irq
arch_enable_irq:
    cpsie i
    bx lr

.global arch_enable_vfp
arch_enable_vfp:
    // Ref: https://github.com/zeoneo/rpi-3b-wifi/blob/master/src/boot.S#L113-L128

    // r1 = Access Control Register
    MRC p15, #0, r1, c1, c0, #2
    // enable full access for p10,11
    ORR r1, r1, #(0xf << 20)
    // ccess Control Register = r1
    MCR p15, #0, r1, c1, c0, #2
    MOV r1, #0
    // flush prefetch buffer because of FMXR below
    MCR p15, #0, r1, c7, c5, #4
    // and CP 10 & 11 were only just enabled
    // Enable VFP itself
    MOV r0,#0x40000000
    // FPEXC = r0
    FMXR FPEXC, r0

    bx lr