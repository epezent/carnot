cmake_minimum_required(VERSION 3.7)

# create project
project(SFVG VERSION 1.0.0 LANGUAGES CXX)

# enable C++11
set(CMAKE_CXX_STANDARD 14)

# declare dependencies
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics window system network)

# create target and add an alias
file(GLOB_RECURSE SFVG_INCLUDE "${CMAKE_SOURCE_DIR}/include/SFVG/*.hpp")
file(GLOB_RECURSE SFVG_SRC "${CMAKE_SOURCE_DIR}/src/SFVG/*.cpp" "${CMAKE_SOURCE_DIR}/src/SFVG/*.hpp")
add_library(sfvg
	${SFVG_INCLUDE}
	${SFVG_SRC}
)
add_library(SFVG::sfvg ALIAS sfvg)

# set target properties
target_include_directories(sfvg
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(sfvg 
	PRIVATE cxx_std_11)
target_compile_options(sfvg
	PRIVATE $<$<CXX_COMPILER_ID:GNU>:"-Wall">
		$<$<CXX_COMPILER_ID:MSVC>:"/W4">
		$<$<CXX_COMPILER_ID:MSVC>:"/MP">
)

target_link_libraries(sfvg
	PUBLIC
		ws2_32 winmm Pdh Psapi XInput sfml-audio sfml-graphics sfml-window sfml-system sfml-network
	PRIVATE
)

# set binary output location (optional, but recommended)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# add definitons
add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX -D_WINSOCK_DEPRECATED_NO_WARNINGS)

# install the library
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SFVG)
install(TARGETS sfvg
	EXPORT sfvg-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(sfvg PROPERTIES EXPORT_NAME SFVG)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# export the targets to a script
install(EXPORT sfvg-targets
	DESTINATION
		${INSTALL_CONFIGDIR}
	FILE
		SFVGTargets.cmake
	NAMESPACE
		SFVG::
	
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





