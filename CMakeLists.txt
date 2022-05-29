cmake_minimum_required(VERSION 3.15)
project(OSLabs LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(PROJECT_EXE_NAME_PREFIX os_)

if (NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    message(FATAL_ERROR "Only MSVC compiler is supported")
endif()

add_subdirectory(src)
