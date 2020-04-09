cmake_minimum_required(VERSION 3.15)
project(CoronaChat C)

set(CMAKE_C_STANDARD 99)

# Source files
set(SOURCES src/main.c)

add_executable(CoronaChat ${SOURCES})