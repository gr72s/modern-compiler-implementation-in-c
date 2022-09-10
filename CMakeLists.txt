cmake_minimum_required(VERSION 3.16)
project(mcic)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")

enable_testing()

add_subdirectory(chap1)

