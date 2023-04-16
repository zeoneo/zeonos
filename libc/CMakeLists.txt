# Define the library name
add_library(libc STATIC
    foo.c
)

# Add the include directory for this subdirectory
target_include_directories(libc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)