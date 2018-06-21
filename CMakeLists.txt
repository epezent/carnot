cmake_minimum_required(VERSION 3.7)

set(SFML_ROOT "/Git/SFML" CACHE FILEPATH "Absolute path to SFML")

# enable C++11
set(CMAKE_CXX_STANDARD 11)

# create project
project(SFVG)

# add configurations
# set(CMAKE_CONFIGURATION_TYPES Release)

# set binary output location (optional, but recommended)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# add definitons
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX -D_WINSOCK_DEPRECATED_NO_WARNINGS -DSFVG_EXPORTS)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3") # ALL WARNINGS
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4") # WARNING LEVEL 4
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP") # MULTICORE BUILDS
endif()

# find SFML
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}")
find_package(SFML 2 COMPONENTS audio graphics window system network)

# include directories
include_directories(
    "include"
    "src"
    ${SFML_INCLUDE_DIR}
)

# get source files
file(GLOB SRC_SHAPES
    "${CMAKE_SOURCE_DIR}/include/SFVG/Shapes/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/SFVG/Shapes/*.cpp"
)

# create SFVG library
add_library(sfvg
    src/SFVG/Shape.cpp
    src/SFVG/ShapeBatch.cpp
    src/SFVG/Fill.cpp
    src/SFVG/Detail/Detail.cpp
    src/SFVG/Detail/clipper.cpp
    ${SRC_SHAPES}
)

# TESTS
add_executable(test1 tests/test1.cpp)
target_link_libraries(test1 sfvg ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

add_executable(benchmark tests/benchmark.cpp)
target_link_libraries(benchmark sfvg ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
