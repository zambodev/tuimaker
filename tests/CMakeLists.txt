# Create a test executable from main.cpp
add_executable(tests_main main.cpp)

# Link the header-only library
target_link_libraries(tests_main PRIVATE tuimaker)

string(FIND "${CMAKE_CXX_COMPILER}" "mingw" found_index)

if(found_index GREATER -1)
    target_link_libraries(tests_main PRIVATE ws2_32)
endif()