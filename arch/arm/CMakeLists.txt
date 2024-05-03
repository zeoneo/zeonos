# Define the library name
add_library(arch STATIC
    boot/boot.S
    boot/arch_asm_routine.S
    boot/kvirt_mem.c
    boot/uart_lower.c
)

# Add the include directory for this subdirectory
# target_include_directories(libc PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )