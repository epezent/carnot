cmake_minimum_required(VERSION 3.13.0)

# create project
project(carnot VERSION 1.0.0 LANGUAGES CXX)

# add modules
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
include(GNUInstallDirs)

# create target and add an alias
add_library(carnot "")
add_library(carnot::carnot ALIAS carnot)
set_target_properties(carnot PROPERTIES DEBUG_POSTFIX -d)
target_compile_features(carnot PUBLIC cxx_std_17 PRIVATE)
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(carnot PUBLIC "/await" PRIVATE) # COROUTINES
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(carnot PUBLIC "-std=c++2a" "-fcoroutines-ts" "-stdlib=libc++") # COROUTINES
endif()

# add source files to target
add_subdirectory(src) 

# add include files
file(GLOB_RECURSE CARNOT_INCLUDE "include/*.hpp" "include/*.inl" "include/*.h")
target_sources(carnot PRIVATE ${CARNOT_INCLUDE})

# find SFML
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system network)
find_package(OpenGL REQUIRED)

# add dependcies
if (WIN32)
target_link_libraries(carnot
	PUBLIC
		ws2_32
		winmm
		Pdh
		Psapi
		XInput
		Shcore
		sfml-graphics sfml-window sfml-system sfml-network
	PRIVATE
)
else()
target_link_libraries(carnot
	PUBLIC
		sfml-graphics sfml-window sfml-system sfml-network
		${OPENGL_LIBRARIES}
	PRIVATE
)
endif()

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
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3") # ALL WARNINGS
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP")    # MULTICORE BUILDS
endif()

#===============================================================================
# EXAMPLES/TESTS
#===============================================================================

add_subdirectory(examples)
add_subdirectory(tests)

#===============================================================================
# INSTALL
#===============================================================================

# install the library
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/carnot)
install(TARGETS carnot
	EXPORT carnot-targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(carnot PROPERTIES EXPORT_NAME carnot)
set_target_properties(carnot PROPERTIES FOLDER "carnot")

# install headers
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# set where we want to install our config
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/carnot)

# export the targets to a script
install(EXPORT carnot-targets
	FILE
		CarnotTargets.cmake
	# NAMESPACE
	# 	carnot::
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
	#    NAMESPACE carnot::
	   FILE ${CMAKE_CURRENT_BINARY_DIR}/CarnotTargets.cmake)

