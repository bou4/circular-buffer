cmake_minimum_required(VERSION 3.12)
project(ring_buffer C)

set(CMAKE_C_STANDARD 99)

add_executable(ring_buffer ring_buffer_test.c ring_buffer.c)