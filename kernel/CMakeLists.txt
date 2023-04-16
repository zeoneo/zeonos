# Define the executable name
add_executable(kernel8-32.elf
    boot.S
    kernel.c
)

# Add the include directory for this subdirectory
target_include_directories(kernel8-32.elf PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link with the libc library
target_link_libraries(kernel8-32.elf libc drivers)

target_link_options(kernel8-32.elf PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)

add_custom_command(TARGET kernel8-32.elf POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O binary kernel8-32.elf kernel8-32.img)

add_custom_target(
    kernel8-32.img ALL
    DEPENDS kernel8-32.img
)
