cmake_minimum_required(VERSION 3.23)
project(fast-xml)

# Create the interface library
add_library(xml_parser_interface INTERFACE)
target_include_directories(xml_parser_interface INTERFACE include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Threads)

if (ENABLE_COMPARISONS)
    # Download and build the tinyxml2 library
    include(FetchContent)
    FetchContent_Declare(
        tinyxml2
        GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(tinyxml2)

    # Download RapidXML
    include(FetchContent)
    FetchContent_Declare(
        rapidxml
        GIT_REPOSITORY https://github.com/dwd/rapidxml.git
        GIT_TAG origin/master
    )
    FetchContent_GetProperties(rapidxml)
    if(NOT rapidxml_POPULATED)
        FetchContent_Populate(rapidxml)
        message(STATUS "RapidXML source dir: ${rapidxml_SOURCE_DIR}")
    endif()

    # Build the txmlfmt executable that uses the tinyxml2 library
    add_executable(txmlfmt src/txmlfmt.cpp)
    target_compile_features(txmlfmt PRIVATE cxx_std_17)
    target_link_libraries(txmlfmt PRIVATE xml_parser_interface tinyxml2)

    # Build the rxmlfmt executable that uses the RapidXML library
    add_executable(rxmlfmt src/rxmlfmt.cpp)
    target_compile_features(rxmlfmt PRIVATE cxx_std_17)
    target_link_libraries(rxmlfmt PRIVATE xml_parser_interface)
    target_include_directories(rxmlfmt PRIVATE ${rapidxml_SOURCE_DIR}) # Link RapidXML

    target_link_libraries(txmlfmt PRIVATE ${CMAKE_THREAD_LIBS_INIT})
endif()

# Build the fastxmlfmt executable that uses the fast-xml library
add_executable(fastxmlfmt src/main.cpp)
target_compile_features(fastxmlfmt PRIVATE cxx_std_17)
target_link_libraries(fastxmlfmt PRIVATE xml_parser_interface ${CMAKE_THREAD_LIBS_INIT})

add_executable(slowxmlfmt src/slowxmlfmt.cpp)
target_compile_features(slowxmlfmt PRIVATE cxx_std_17)
target_link_libraries(slowxmlfmt PRIVATE xml_parser_interface ${CMAKE_THREAD_LIBS_INIT})

# Pass the interface library to the test subdirectory
add_subdirectory(test)
