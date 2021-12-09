cmake_minimum_required(VERSION 3.10)

set(AOC_PROGRAM_NAME "day05")

project(${AOC_PROGRAM_NAME} LANGUAGES CXX)

add_executable(${AOC_PROGRAM_NAME}
    ${AOC_PROGRAM_NAME}.cpp
    ../utils/utils.cpp
)

target_include_directories(${AOC_PROGRAM_NAME} PUBLIC
    .
    ../utils
)

if (WIN32)
target_compile_options(${AOC_PROGRAM_NAME} PUBLIC /std:c++17)
endif (WIN32)
if (UNIX)
target_compile_options(${AOC_PROGRAM_NAME} PUBLIC -std=c++17)
target_link_libraries(${AOC_PROGRAM_NAME} curses)
endif (UNIX)

