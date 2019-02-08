cmake_minimum_required(VERSION 3.13.0)

# create project
project(SFVG VERSION 1.0.0 LANGUAGES CXX)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# enable C++11
set(CMAKE_CXX_STANDARD 17)

# create target and add an alias
add_library(sfvg "")
add_library(SFVG::sfvg ALIAS sfvg)

# add source files to target
add_subdirectory(src/SFVG)

# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics window system network)

# find Chipmunk
find_package(Chipmunk REQUIRED)

# add dependcies
target_link_libraries(sfvg
	PUBLIC
		ws2_32
		winmm
		Pdh
		Psapi
		XInput
		sfml-audio sfml-graphics sfml-window sfml-system sfml-network
		${CHIPMUNK_LIBRARIES}
	PRIVATE
)

target_compile_definitions(sfvg
	PUBLIC
		-D_CRT_SECURE_NO_WARNINGS
		-DNOMINMAX
		-D_WINSOCK_DEPRECATED_NO_WARNINGS
)

# target_compile_features(sfvg
# 	PRIVATE cxx_std_11
# )

# set target properties
target_include_directories(sfvg
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		${CHIPMUNK_INCLUDE_DIRS}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

# don't know why this doesn't work for setting compile options
# target_compile_options(sfvg
# 	PRIVATE $<$<CXX_COMPILER_ID:GNU>:"-Wall">
# 		$<$<CXX_COMPILER_ID:MSVC>:"/W4">
# 		$<$<CXX_COMPILER_ID:MSVC>:"/MP">
# )
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3") # ALL WARNINGS
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP")    # MULTICORE BUILDS
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /await") # COROUTINES
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fcoroutines-ts -stdlib=libc++") # COROUTINES
endif()

# set binary output location for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# set binary output location for static libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
# set binary output location for shared libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)


# install the library
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SFVG)
install(TARGETS sfvg
	EXPORT sfvg-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(sfvg PROPERTIES EXPORT_NAME SFVG)
set_target_properties(sfvg PROPERTIES FOLDER "SFVG")


install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# export the targets to a script
install(EXPORT sfvg-targets
	FILE
		SFVGTargets.cmake
	NAMESPACE
		SFVG::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

# include helper functions for creating config files that can be included by other projects to find and use a package
include(CMakePackageConfigHelpers)

# generate a package configuration file and a package version file
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/SFVGConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/SFVGConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/SFVGConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# install the config and configversion modules
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/SFVGConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/SFVGConfigVersion.cmake
	DESTINATION ${INSTALL_CONFIGDIR}
)

# export from the build tree
export(EXPORT sfvg-targets
	   NAMESPACE SFVG::
	   FILE ${CMAKE_CURRENT_BINARY_DIR}/SFVGTargets.cmake)

# register package in user's package registry
export(PACKAGE SFVG)

# build tests
add_subdirectory(tests)

# build examples
add_subdirectory(examples)
