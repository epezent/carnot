cmake_minimum_required(VERSION 3.7)

# enable C++11
set(CMAKE_CXX_STANDARD 17)

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
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")    # WARNING LEVEL 4
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP")    # MULTICORE BUILDS
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /await") # COROUTINES
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fcoroutines-ts -stdlib=libc++") # COROUTINES
endif()

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS audio graphics window system network REQUIRED)
# find_package(SDL2 REQUIRED)

# include directories
include_directories(
    "include"
    "src"
    "C:/Git/Chipmunk2D/include"
    # "C:/Program Files/SDL2/include"
)

link_directories("C:/Git/Chipmunk2D/build-msvc/src/Release")
# link_directories("C:/Program Files/SDL2/lib")

file(GLOB_RECURSE SFVG_INCLUDE "${CMAKE_SOURCE_DIR}/include/SFVG/*.hpp")
file(GLOB_RECURSE SFVG_SRC "${CMAKE_SOURCE_DIR}/src/SFVG/*.cpp" "${CMAKE_SOURCE_DIR}/src/SFVG/*.hpp")

# create SFVG library
add_library(sfvg ${SFVG_INCLUDE} ${SFVG_SRC})
target_link_libraries(sfvg ws2_32 winmm Pdh Psapi XInput sfml-audio sfml-graphics sfml-window sfml-system sfml-network)

# add_executable(sdl examples/example_sdl.cpp)
# target_link_libraries(sdl SDL2.lib SDL2main.lib)

# TESTS


# EXAMPLES
add_subdirectory(examples)
