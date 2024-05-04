file(GLOB LIBC_SOURCES "${PROJECT_SOURCE_DIR}/libc/*.c")

# Define the library name
add_library(libc STATIC ${LIBC_SOURCES} )

# Add the include directory for this subdirectory
target_include_directories(libc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)