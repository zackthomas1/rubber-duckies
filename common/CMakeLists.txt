# CMakeLists.txt for common module
add_library(duckies_common INTERFACE ./include/duckies_types.h)

# Link dependencies for common module
target_include_directories(duckies_common INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)