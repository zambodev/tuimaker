cmake_minimum_required(VERSION 3.12)

include(GNUInstallDirs)

# Variables
set(PROJECT_NAME "tuimaker")
set(TEST_NAME "tuimakerTest")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_LIBDIR lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Out)

project(${PROJECT_NAME})
include_directories(${CMAKE_BINARY_DIR})

file(GLOB INCLUDE Include/*.hpp)
file(GLOB SOURCE Source/*.cpp)

add_library(${PROJECT_NAME} SHARED ${INCLUDE} ${SOURCE})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER ${INCLUDE})

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)



file(GLOB SOURCE ${CMAKE_BINARY_DIR}/Test/*.cpp)
add_executable(${TEST_NAME} ${SOURCE})

include_directories(${CMAKE_BINARY_DIR}/Include)
target_link_libraries(${TEST_NAME} ${CMAKE_BINARY_DIR}/Out/libtuimaker.so)

ADD_CUSTOM_TARGET(do_always ALL COMMAND ${CMAKE_BINARY_DIR}/Out/${TEST_NAME})