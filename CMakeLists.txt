cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CPP_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_LINKER arm-none-eabi-ld)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-D__is_zeonos_kernel=1)

# 
#FIXME: -nostdinc
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g -march=armv8-a+crc -mcpu=cortex-a53 -mfpu=crypto-neon-fp-armv8 -fpic -ffreestanding -nostdlib -nostartfiles -mno-unaligned-access -mfloat-abi=hard -fsigned-char -mgeneral-regs-only -fno-asynchronous-unwind-tables  -fno-exceptions -Wno-cast-align -fstack-protector-strong -O2")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -march=armv8-a+crc -mcpu=cortex-a53 -mfpu=crypto-neon-fp-armv8 -fpic -ffreestanding -nostdlib -nostartfiles -mno-unaligned-access -mfloat-abi=hard -fsigned-char -mgeneral-regs-only -fno-asynchronous-unwind-tables -fno-exceptions -Wno-cast-align -fstack-protector-strong -O2")

#FIXME: Add standard headers later
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -fno-exceptions -fno-rtti -Wall -O2 -fno-tree-loop-distribute-patterns") #-nostdinc++

# add_compile_options(-fsanitize=kernel-address)
# add_link_options(-fsanitize=kernel-address)



set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib -nodefaultlibs")

# Define the project name
project(zeoos LANGUAGES C CXX ASM)

# Add the top level include directory
include_directories(include)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# Define an option for CPU architecture
option(CPU_ARCH "Specify the CPU architecture to use" "arm")

if(NOT CPU_ARCH)
    # Set default architecture to "arm" if not specified
    # set(CPU_ARCH "arm" CACHE STRING "Specify the CPU architecture to use" FORCE)
    message(STATUS "Defaulting CPU_ARCH to arm")
endif()

# Add the subdirectories
add_subdirectory(libc)
add_subdirectory(arch/${CPU_ARCH})
add_subdirectory(drivers)


file(GLOB KERNEL_SOURCES "${PROJECT_SOURCE_DIR}/kernel/*.c" "${PROJECT_SOURCE_DIR}/kernel/*.cpp")

# Define the executable name
add_executable(kernel8-32.elf ${KERNEL_SOURCES})

# Add support for cppcheck
find_program(CPPCHECK_PATH cppcheck)
if(NOT CPPCHECK_PATH)
    message(WARNING, "Could not find cppcheck, please install it or make sure it is in your PATH ${CPPCHECK_PATH}")
endif()
list(APPEND CMAKE_CXX_CPPCHECK ${CPPCHECK_PATH})

add_custom_target(cppcheck
    COMMAND ${CMAKE_CXX_CPPCHECK} --enable=all --quiet ${CMAKE_SOURCE_DIR}/libc ${CMAKE_SOURCE_DIR}/kernel
    COMMENT "Running cppcheck on project sources"
    VERBATIM
)


find_program(CLANG_FORMAT_PATH clang-format)
if(NOT CLANG_FORMAT_PATH)
    message(WARNING "Could not find clang-format, please install it or make sure it is in your PATH")
endif()
set(CMAKE_CXX_CLANG_FORMAT ${CLANG_FORMAT_PATH})

file(GLOB LIBC_SOURCES "${PROJECT_SOURCE_DIR}/libc/*.c")
file(GLOB KERNEL_SOURCES "${PROJECT_SOURCE_DIR}/kernel/*.c")
file(GLOB DRIVERS_SOURCES "${PROJECT_SOURCE_DIR}/drivers/*.c")

# Define a custom target for clang-format
add_custom_target(clang-format
    COMMAND ${CMAKE_CXX_CLANG_FORMAT} -i ${LIBC_SOURCES} ${KERNEL_SOURCES} ${DRIVERS_SOURCES}
    COMMENT "Running clang-format on project sources"
    VERBATIM
)



# Link with the libc library
target_link_libraries(kernel8-32.elf arch libc drivers)

target_link_options(kernel8-32.elf PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/arch/${CPU_ARCH}/linker.ld -nodefaultlibs -nostdlib -nostartfiles)

add_custom_command(TARGET kernel8-32.elf POST_BUILD
                   COMMAND ${CMAKE_OBJCOPY} -O binary kernel8-32.elf kernel8-32.img)

add_custom_target(
    kernel8-32.img ALL
    DEPENDS kernel8-32.img
)

add_custom_target(generate_map_file
    COMMAND arm-none-eabi-nm kernel8-32.elf > kernel8-32.map
    DEPENDS kernel8-32.elf
    COMMENT "Generating kernel8-32.map file"
)

add_custom_target(generate_list_file
    COMMAND arm-none-eabi-objdump -D -S kernel8-32.elf > kernel8-32.list
    DEPENDS kernel8-32.elf
    COMMENT "Generating kernel8-32.map file"
)