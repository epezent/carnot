cmake_minimum_required(VERSION 3.13.0)

# create project
project(EvansEngine VERSION 1.0.0 LANGUAGES CXX)

# add modules
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# enable C++17
set(CMAKE_CXX_STANDARD 17)

# create target and add an alias
add_library(carnot "")
add_library(Carnot::carnot ALIAS carnot)

# add source files to target
add_subdirectory(src)

file(GLOB_RECURSE EE_INCLUDE "include/Carnot/*.hpp" "include/Carnot/*.inl")
target_sources(carnot PRIVATE ${EE_INCLUDE})

# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics window system network)

# find Chipmunk
find_package(Chipmunk REQUIRED)

# add dependcies
target_link_libraries(carnot
	PUBLIC
		ws2_32
		winmm
		Pdh
		Psapi
		XInput
		Shcore
		sfml-audio sfml-graphics sfml-window sfml-system sfml-network
		${CHIPMUNK_LIBRARIES}
	PRIVATE
)

target_compile_definitions(carnot
	PUBLIC
		-D_CRT_SECURE_NO_WARNINGS
		-DNOMINMAX
		-D_WINSOCK_DEPRECATED_NO_WARNINGS
)

# set target properties
target_include_directories(carnot
	PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		${CHIPMUNK_INCLUDE_DIRS}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3") # ALL WARNINGS
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP")    # MULTICORE BUILDS
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /await") # COROUTINES
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fcoroutines-ts -stdlib=libc++") # COROUTINES
endif()

# install the library
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Carnot)
install(TARGETS carnot
	EXPORT carnot-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(carnot PROPERTIES EXPORT_NAME Carnot)
set_target_properties(carnot PROPERTIES FOLDER "Carnot")

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# export the targets to a script
install(EXPORT carnot-targets
	FILE
		CarnotTargets.cmake
	NAMESPACE
		carnot::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

# include helper functions for creating config files that can be included by other projects to find and use a package
include(CMakePackageConfigHelpers)

# generate a package configuration file and a package version file
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/CarnotConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/CarnotConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/CarnotConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# install the config and configversion modules
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/CarnotConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/CarnotConfigVersion.cmake
	DESTINATION ${INSTALL_CONFIGDIR}
)

# export from the build tree
export(EXPORT carnot-targets
	   NAMESPACE carnot::
	   FILE ${CMAKE_CURRENT_BINARY_DIR}/CarnotTargets.cmake)

add_subdirectory(examples)