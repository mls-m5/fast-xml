cmake_minimum_required(VERSION 3.14)

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

include(GoogleTest)

# Define a function to add a test
function(add_xml_parser_test test_name source_file)
  add_executable(${test_name} ${source_file})
  target_link_libraries(${test_name} PRIVATE xml_parser_interface gtest_main)
  add_test(NAME ${test_name} COMMAND ${test_name})
  gtest_discover_tests(${test_name} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_options(${test_name}  PRIVATE -Og)
endfunction()

# Add the two tests
add_xml_parser_test(xml_tokenizer_test xml_tokenizer_test.cpp)
add_xml_parser_test(slow_xml_tokenizer_test slow_xml_tokenizer_test.cpp)
add_xml_parser_test(xml_node_test xml_node_test.cpp)
add_xml_parser_test(slow_xml_node_test xml_node_test.cpp)

file (COPY uggly.xml DESTINATION .)

add_executable(test_data_generator test_data_generator.cpp)

target_link_libraries(test_data_generator PRIVATE xml_parser_interface)
