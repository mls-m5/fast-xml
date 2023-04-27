
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

# Define a function to add a test
function(add_xml_parser_test test_name)
  add_executable(${test_name} ${test_name}.cpp)
  target_link_libraries(${test_name} PRIVATE fastxml gtest_main)
  add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Add the two tests
add_xml_parser_test(xml_tokenizer_test)
add_xml_parser_test(xml_node_test)
add_xml_parser_test(heap_xml_tokenizer_test)
add_xml_parser_test(heap_xml_node_test)

file (COPY uggly.xml DESTINATION .)

add_executable(test_data_generator test_data_generator.cpp)

target_link_libraries(test_data_generator PRIVATE fastxml)

