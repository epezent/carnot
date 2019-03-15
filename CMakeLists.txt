cmake_minimum_required(VERSION 3.13.0)

# create project
project(EvansEngine VERSION 1.0.0 LANGUAGES CXX)

# add modules
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

# enable C++17
set(CMAKE_CXX_STANDARD 17)

# create target and add an alias
add_library(ee "")
add_library(EE::ee ALIAS ee)

# add source files to target
add_subdirectory(src)

file(GLOB_RECURSE EE_INCLUDE "include/EE/*.hpp" "include/EE/*.inl")
target_sources(ee PRIVATE ${EE_INCLUDE})

# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics window system network)

# find Chipmunk
find_package(Chipmunk REQUIRED)

# add dependcies
target_link_libraries(ee
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

target_compile_definitions(ee
	PUBLIC
		-D_CRT_SECURE_NO_WARNINGS
		-DNOMINMAX
		-D_WINSOCK_DEPRECATED_NO_WARNINGS
)

# set target properties
target_include_directories(ee
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
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/EE)
install(TARGETS ee
	EXPORT ee-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(ee PROPERTIES EXPORT_NAME EE)
set_target_properties(ee PROPERTIES FOLDER "EE")

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# export the targets to a script
install(EXPORT ee-targets
	FILE
		EETargets.cmake
	NAMESPACE
		ee::
	DESTINATION
		${INSTALL_CONFIGDIR}
)

# include helper functions for creating config files that can be included by other projects to find and use a package
include(CMakePackageConfigHelpers)

# generate a package configuration file and a package version file
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/EEConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/EEConfig.cmake
	INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/EEConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# install the config and configversion modules
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/EEConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/EEConfigVersion.cmake
	DESTINATION ${INSTALL_CONFIGDIR}
)

# export from the build tree
export(EXPORT ee-targets
	   NAMESPACE ee::
	   FILE ${CMAKE_CURRENT_BINARY_DIR}/EETargets.cmake)

add_subdirectory(examples)