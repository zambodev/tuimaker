# Variables
set(PROJECT_NAME "tuimaker")

cmake_minimum_required(VERSION 3.12)

project(PROJECT_NAME)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build)

# Add source to this project's executable.
add_library(PROJECT_NAME SHARED
    src/utils.cpp
    src/window.cpp
    src/windowManager.cpp
)

target_include_directories(PROJECT_NAME PUBLIC ${PROJECT_SOURCE_DIR}/src)
add_executable(test "test/main.cpp")

# link the new hello_library target with the hello_binary target
target_link_libraries(test PROJECT_NAME)