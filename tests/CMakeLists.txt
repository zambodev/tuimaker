# Create a test executable from main.cpp
add_executable(tests_main main.cpp)

# Link the header-only library
target_link_libraries(tests_main PRIVATE tuimaker)

# Register the test with CTest
add_test(NAME RunHeaderOnlyTests COMMAND tests_main)