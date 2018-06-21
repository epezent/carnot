cmake_minimum_required(VERSION 3.7)

set(SFML_ROOT "C:/Git/SFML" CACHE FILEPATH "Absolute path to SFML")
set(THOR_ROOT "C:/Git/Thor" CACHE FILEPATH "Absolute path to Thor")

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

# include directories
include_directories(
    "include"
    "src"
    "${SFML_ROOT}/include"
)

# get source files
file(GLOB SRC_SHAPES
    "${CMAKE_SOURCE_DIR}/include/SFVG/Shapes/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/SFVG/Shapes/*.cpp"
)

# link libs
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    link_directories("${SFML_ROOT}/lib")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    link_directories("${SFML_ROOT}/lib_msvc/Debug")
endif()


# find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# create SFVG library
add_library(sfvg
    src/SFVG/Shape.cpp
    src/SFVG/ShapeBatch.cpp
    src/SFVG/Detail/Detail.cpp
    src/SFVG/Detail/clipper.cpp
    ${SRC_SHAPES}
)

# TESTS
add_executable(test1 tests/test1.cpp)
target_link_libraries(test1 sfvg sfml-system sfml-window sfml-graphics ${OPENGL_LIBRARIES})

add_executable(test2 tests/test2.cpp)
target_link_libraries(test2 sfvg sfml-system sfml-window sfml-graphics ${OPENGL_LIBRARIES})

add_executable(benchmark tests/benchmark.cpp)
target_link_libraries(benchmark sfvg sfml-system sfml-window sfml-graphics ${OPENGL_LIBRARIES})
