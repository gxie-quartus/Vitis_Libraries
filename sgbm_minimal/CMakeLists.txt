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

find_package(OpenCV QUIET)
if(OpenCV_FOUND)
    add_executable(sgbm_csim
        csim/sgbm_csim.cpp
    )
    target_include_directories(sgbm_csim PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${OpenCV_INCLUDE_DIRS}
    )
    target_link_libraries(sgbm_csim PRIVATE
        sgbm_kernel
        ${OpenCV_LIBS}
    )
else()
    message(STATUS "OpenCV not found; sgbm_csim target will not be built.")
endif()
