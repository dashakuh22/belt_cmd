cmake_minimum_required(VERSION 3.21)
project(belt_cmd C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(bee2)
include_directories(bee2/include)

add_executable(belt_cmd main.c)
target_link_libraries(belt_cmd bee2 bee2_static)
