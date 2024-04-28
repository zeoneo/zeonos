# Define the library name
add_library(drivers STATIC
    uart0.c
)

# Add the include directory for this subdirectory
target_include_directories(libc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)