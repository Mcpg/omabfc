cmake_minimum_required(VERSION 2.6)
project(omabfc C)

set(CMAKE_C_STANDARD 99)

file(GLOB_RECURSE SOURCES src/*.c)
include_directories(src)

add_executable(omabfc ${SOURCES})
