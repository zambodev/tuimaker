cmake_minimum_required(VERSION 3.5...4.0)

project(tuimaker LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

add_library(tuimaker INTERFACE)
target_include_directories(tuimaker INTERFACE include/)

string(FIND "${CMAKE_CXX_COMPILER}" "mingw" found_index)

if(found_index GREATER -1)
    target_link_libraries(tuimaker INTERFACE ws2_32)
endif()

# Enable testing
add_subdirectory(tests)
