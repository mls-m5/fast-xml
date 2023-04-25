cmake_minimum_required(VERSION 3.23)
project(fast-xml)

# Create the interface library
add_library(xml_parser_interface INTERFACE)
target_include_directories(xml_parser_interface INTERFACE include)

# Link the interface library to the main application
add_executable(fast-xml src/main.cpp)
target_compile_features(fast-xml PRIVATE cxx_std_17)
target_link_libraries(fast-xml PRIVATE xml_parser_interface)

find_package(Threads)
target_link_libraries(fast-xml PRIVATE ${CMAKE_THREAD_LIBS_INIT})

# Pass the interface library to the test subdirectory
add_subdirectory(test)
