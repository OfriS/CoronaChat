cmake_minimum_required(VERSION 3.15)
project(CoronaChat C)

set(CMAKE_C_STANDARD 99)

include_directories(src)

set(CMAKE_C_COMPILER gcc)

# Source files
set(SOURCES
        src/main.c
        src/utils/linked_list/linked_list.c
        src/utils/linked_list/linked_list.h
        src/utils/linked_list/linked_list_internals.h
        src/utils/network_utils/network_utils.c
        src/utils/network_utils/network_utils.h
        src/utils/network_utils/network_utils_internals.h)

add_executable(CoronaChat ${SOURCES})

target_compile_definitions(CoronaChat PRIVATE $<$<CONFIG:Debug>:DEBUG>)
