cmake_minimum_required(VERSION 3.7)

# enable C++11
set(CMAKE_CXX_STANDARD 14)

# create project
project(SFVG)

# set binary output location (optional, but recommended)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# add definitons
# add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX -D_WINSOCK_DEPRECATED_NO_WARNINGS)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3") # ALL WARNINGS
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4") # WARNING LEVEL 4
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP") # MULTICORE BUILDS
endif()

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS audio graphics window system network REQUIRED)

# include directories
include_directories(
    "include"
    "src"
)

# get source files
file(GLOB SRC_SHAPES
    "${CMAKE_SOURCE_DIR}/include/SFVG/Graphics/Shapes/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/SFVG/Graphics/Shapes/*.cpp"
)

set(SFVG_SRC
    src/SFVG/Graphics/SharedResources.cpp
    src/SFVG/Graphics/Gradient.cpp
    src/SFVG/Graphics/Detail/clipper.cpp
    src/SFVG/Graphics/Detail/ExternalLibs.cpp
    src/SFVG/Animation/Animation.cpp
    src/SFVG/Graphics/Color.cpp
    src/SFVG/Graphics/Shape.cpp
    src/SFVG/Graphics/Stroke.cpp
    src/SFVG/Graphics/Path.cpp
    src/SFVG/Graphics/Anchor.cpp
    src/SFVG/Graphics/Points.cpp
    ${SRC_SHAPES}
)

# create SFVG library
add_library(sfvg ${SFVG_SRC})
target_link_libraries(sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

# TESTS
add_executable(test1 tests/test1.cpp)
target_link_libraries(test1 sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(test2 tests/test2.cpp)
target_link_libraries(test2 sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(test3 tests/test3.cpp)
target_link_libraries(test3 sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(test4 tests/test4.cpp)
target_link_libraries(test4 sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(test5 tests/test5.cpp)
target_link_libraries(test5 sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(test6 tests/test6.cpp)
target_link_libraries(test6 sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(test7 tests/test7.cpp)
target_link_libraries(test7 sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(test8 tests/test8.cpp)
target_link_libraries(test8 sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(benchmark tests/benchmark.cpp)
target_link_libraries(benchmark sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

# EXAMPLES
add_executable(example_tween examples/example_tween.cpp)
target_link_libraries(example_tween sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

add_executable(example_shapes examples/example_shapes.cpp)
target_link_libraries(example_shapes sfvg sfml-audio sfml-graphics sfml-window sfml-system sfml-network)
