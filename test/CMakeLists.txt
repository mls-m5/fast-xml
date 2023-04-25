cmake_minimum_required(VERSION 3.14)

project(xml_parser_test)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Create the test executable
enable_testing()
add_executable(xml_parser_test xml_parser_test.cpp)

# Link the interface library and gtest_main to the test executable
target_link_libraries(xml_parser_test PRIVATE xml_parser_interface gtest_main)

# Register the test using CMake's add_test command
add_test(NAME xml_parser_test COMMAND xml_parser_test)

include(GoogleTest)
gtest_discover_tests(xml_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
