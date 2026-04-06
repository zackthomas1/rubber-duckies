cmake_minimum_required(VERSION 3.13)    # CMAKE version check
project(rubber_duckies)                 # Create project 

set(CMAKE_CXX_STANDARD 20)              # Enable c++20 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---
# spdlog and Boost are managed via vcpkg
find_package(spdlog CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS program_options)
list(APPEND CMAKE_PREFIX_PATH "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/p-ranav-csv2")
find_package(csv2 CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

# OpenMP is a compiler feature, found via CMake's built-in module
find_package(OpenMP REQUIRED COMPONENTS CXX)

# Add main.cpp file of project root directory as source file 
set(SOURCE_FILES main.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(rubber_duckies ${SOURCE_FILES})

target_link_libraries(rubber_duckies
    PRIVATE
        spdlog::spdlog
        Boost::program_options
        OpenMP::OpenMP_CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)