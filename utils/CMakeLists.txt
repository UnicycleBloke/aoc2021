set(AOC_UTILS_NAME "aocutils")
add_library(${AOC_UTILS_NAME} STATIC utils.cpp)

if (WIN32)
target_compile_options(${AOC_UTILS_NAME} PUBLIC /std:c++17)
endif (WIN32)
if (UNIX)
target_compile_options(${AOC_UTILS_NAME} PUBLIC -std=c++17)
endif (UNIX)
