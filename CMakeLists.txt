cmake_minimum_required(VERSION 3.5...4.0)

project(tuimaker LANGUAGES CXX)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-14")
set(CMAKE_CXX_STANDARD 23)

add_library(tuimaker INTERFACE)

target_include_directories(tuimaker INTERFACE include/)

# Enable testing
add_subdirectory(tests)
