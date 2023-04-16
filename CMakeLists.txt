cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CPP_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_LINKER arm-none-eabi-ld)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)


set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g -march=armv8-a+crc -mcpu=cortex-a53 -mfpu=crypto-neon-fp-armv8 -fpic -ffreestanding -nostdlib -nostartfiles -mno-unaligned-access")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostdlib")

# Define the project name
project(zeoos LANGUAGES C CXX ASM)

# Add the top level include directory
include_directories(include)

# Add the subdirectories
add_subdirectory(libc)
add_subdirectory(drivers)
add_subdirectory(kernel)


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

# Define a custom target for clang-format
add_custom_target(clang-format
    COMMAND ${CMAKE_CXX_CLANG_FORMAT} -i ${LIBC_SOURCES} ${KERNEL_SOURCES}
    COMMENT "Running clang-format on project sources"
    VERBATIM
)
