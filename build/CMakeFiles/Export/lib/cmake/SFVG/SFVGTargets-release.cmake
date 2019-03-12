#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SFVG::SFVG" for configuration "Release"
set_property(TARGET SFVG::SFVG APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SFVG::SFVG PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/sfvg.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFVG::SFVG )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFVG::SFVG "${_IMPORT_PREFIX}/lib/sfvg.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
