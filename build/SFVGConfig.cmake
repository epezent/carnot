get_filename_component(SFVG_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH ${SFVG_CMAKE_DIR})

# NOTE Had to use find_package because find_dependency does not support COMPONENTS or MODULE until 3.8.0
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED COMPONENTS audio graphics window system network)
list(REMOVE_AT CMAKE_MODULE_PATH -1)

if(NOT TARGET SFVG::SFVG)
    include("${SFVG_CMAKE_DIR}/SFVGTargets.cmake")
endif()

set(SFVG_LIBRARIES SFVG::SFVG)