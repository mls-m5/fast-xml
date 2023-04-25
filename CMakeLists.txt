cmake_minimum_required(VERSION 3.23)
project(fast-xml)

# Create the interface library
add_library(xml_parser_interface INTERFACE)
target_include_directories(xml_parser_interface INTERFACE include)

# Link the interface library to the main application
add_executable(fastxmlfmt src/main.cpp)
target_compile_features(fastxmlfmt PRIVATE cxx_std_17)
target_link_libraries(fastxmlfmt PRIVATE xml_parser_interface)

find_package(Threads)
target_link_libraries(fastxmlfmt PRIVATE ${CMAKE_THREAD_LIBS_INIT})

# Pass the interface library to the test subdirectory
add_subdirectory(test)
