cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(Game-of-Life)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

include(Fetch.cmake)

add_subdirectory(src)
