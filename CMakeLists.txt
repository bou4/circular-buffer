cmake_minimum_required(VERSION 3.12)
project(ring_buffer C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(ring_buffer src/ring_buffer_test.c src/ring_buffer.c)