list(APPEND CMAKE_PREFIX_PATH "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/p-ranav-csv2")
find_package(csv2 CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)

add_library(ducky_readers STATIC
    ./src/file_reader.cpp
    ./include/file_reader.h.
    ./src/json_reader.cpp
    ./include/json_reader.h
    ./src/csv_reader.cpp
    ./include/csv_reader.h
)
#
target_link_libraries(ducky_readers
    PRIVATE
        spdlog::spdlog
        csv2::csv2
    PUBLIC_HEADER
        duckies_common
)

target_include_directories(ducky_readers
    PRIVATE
        ${RAPIDJSON_INCLUDE_DIRS}
)