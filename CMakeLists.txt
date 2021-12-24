cmake_minimum_required(VERSION 2.18)
project(BloomFilter C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_library(bf bloom_filter.c)

add_executable(test test.c)

target_link_libraries(test bf cjson)

add_subdirectory(cJSON)