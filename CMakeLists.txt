cmake_minimum_required(VERSION 3.23)
project(P1 C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)