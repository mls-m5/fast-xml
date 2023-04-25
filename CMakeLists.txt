
cmake_minimum_required(VERSION 3.23)
project(fast-xml)

add_executable(
    fast-xml
    src/main.cpp
    )

target_compile_features(
    fast-xml
    PRIVATE
    cxx_std_17
    )

find_package(Threads)
target_link_libraries(
    fast-xml
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    )
