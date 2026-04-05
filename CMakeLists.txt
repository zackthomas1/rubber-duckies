cmake_minimum_required(VERSION 3.13)    # CMAKE version check
project(rubber_duckies)                 # Create project 
set(CMAKE_CXX_STANDARD 20)              # Enable c++20 standard
set(CMAKE_CXX_STANDAAR_REQUIRED ONE)

# Add main.cpp file of project root directory as source file 
set(SOURCE_FILES main.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(rubber_duckies ${SOURCE_FILES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
