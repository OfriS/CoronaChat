cmake_minimum_required(VERSION 3.13)
project(CoronaChat C)

set(CMAKE_C_STANDARD 99)

include_directories(src)

# Source files
set(SOURCES
        src/main.c
	src/ui/ui.c
        src/utils/linked_list/linked_list.c
        src/utils/linked_list/linked_list.h
        src/utils/linked_list/linked_list_internals.h)

add_executable(CoronaChat ${SOURCES})

target_compile_definitions(CoronaChat PRIVATE $<$<CONFIG:Debug>:DEBUG>)
# Linking libraries
target_link_libraries(CoronaChat -lncurses)



