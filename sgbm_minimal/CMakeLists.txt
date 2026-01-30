cmake_minimum_required(VERSION 3.16)
project(sgbm_minimal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_library(sgbm_kernel
    src/sgbm_top.cpp
)

target_include_directories(sgbm_kernel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)
