project(${AOC_PROGRAM_NAME} LANGUAGES CXX)

add_executable(${AOC_PROGRAM_NAME} ${AOC_PROGRAM_NAME}.cpp)

target_include_directories(${AOC_PROGRAM_NAME} PUBLIC
    .
    ../utils
)

if (WIN32)
target_compile_options(${AOC_PROGRAM_NAME} PUBLIC /std:c++17)
target_link_libraries(${AOC_PROGRAM_NAME} aocutils)
endif (WIN32)
if (UNIX)
#target_compile_options(${AOC_PROGRAM_NAME} PUBLIC -std=c++17 -O2)
target_compile_options(${AOC_PROGRAM_NAME} PUBLIC -std=c++17 )
target_link_libraries(${AOC_PROGRAM_NAME} curses aocutils)
endif (UNIX)

