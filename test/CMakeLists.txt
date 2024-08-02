
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
add_executable(
    fast_xml_test
    tests.cpp
)
target_link_libraries(fast_xml_test PRIVATE fastxml gtest_main)
add_test(NAME fast_xml_test COMMAND fast_xml_test)


file (COPY uggly.xml DESTINATION .)

add_executable(test_data_generator test_data_generator.cpp)

target_link_libraries(test_data_generator PRIVATE fastxml)

