# Generate project name from the folder name in which this this file is included.
get_filename_component(AOC_PROGRAM_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" AOC_PROGRAM_NAME ${AOC_PROGRAM_NAME})

project(${AOC_PROGRAM_NAME} LANGUAGES CXX)

add_executable(${AOC_PROGRAM_NAME} ${AOC_PROGRAM_NAME}.cpp)

target_include_directories(${AOC_PROGRAM_NAME} PUBLIC
    .
    ../utils
)

if (WIN32)
# CMake automagically includes the /RTC1 option, which is not compatible with using /O2.
# There is no simple way to remove /RTC1 (but you can use the IDE). Also, the default 
# build mode is Debug rather than Release. 
#target_compile_options(${AOC_PROGRAM_NAME} PUBLIC /std:c++17 /O2)
target_compile_options(${AOC_PROGRAM_NAME} PUBLIC /std:c++17)
target_link_libraries(${AOC_PROGRAM_NAME} aocutils)
endif (WIN32)
if (UNIX)
target_compile_options(${AOC_PROGRAM_NAME} PUBLIC -std=c++17 -O2)
#target_compile_options(${AOC_PROGRAM_NAME} PUBLIC -std=c++17 -g)
target_link_libraries(${AOC_PROGRAM_NAME} curses aocutils)
endif (UNIX)

