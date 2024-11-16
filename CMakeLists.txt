cmake_minimum_required(VERSION 3.10)
project(IntervalMapProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
    src/main.cpp
    src/interval_map_tester.cpp
)

add_executable(interval_map_program ${SOURCES})
