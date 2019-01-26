# Locate Chipmunk physics library
# This module defines
# CHIPMUNK_FOUND, if false, do not try to link to Lua
# CHIPMUNK_LIBRARIES
# CHIPMUNK_INCLUDE_DIRS, where to find chipmunk.h
#
# Note that the expected include convention is
#  #include <chipmunk.h>
# and not
#  #include <chipmunk/chipmunk.h>
# This is because, the chipmunk location is not standardized and may exist
# in locations other than chipmunk/

set(CMAKE_PREFIX_PATH "C:/Program Files/chipmunk")

find_path(CHIPMUNK_INCLUDE_DIR chipmunk.h
  HINTS
  $ENV{CHIPMUNK_DIR}
  PATH_SUFFIXES include/chipmunk include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

find_library(CHIPMUNK_LIBRARY
  NAMES chipmunk
  HINTS
  $ENV{CHIPMUNK_DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

# if(CHIPMUNK_LIBRARY)
#   # include the math library for Unix
#   if(UNIX AND NOT APPLE)
#     find_library(CHIPMUNK_MATH_LIBRARY m)
#     set(CHIPMUNK_LIBRARIES "${CHIPMUNK_LIBRARY};${CHIPMUNK_MATH_LIBRARY}" CACHE STRING "Chipmunk Libraries")
#   # For Windows and Mac, don't need to explicitly include the math library
#   else(UNIX AND NOT APPLE)
#     set(CHIPMUNK_LIBRARIES "${CHIPMUNK_LIBRARY}" CACHE STRING "Chipmunk Libraries")
#   endif(UNIX AND NOT APPLE)
# endif(CHIPMUNK_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Chipmunk DEFAULT_MSG CHIPMUNK_LIBRARY CHIPMUNK_INCLUDE_DIR)

if(CHIPMUNK_FOUND)
  set(CHIPMUNK_INCLUDE_DIRS "${CHIPMUNK_INCLUDE_DIR}/..")
  set(CHIPMUNK_LIBRARIES ${CHIPMUNK_LIBRARY})
endif()

mark_as_advanced(CHIPMUNK_INCLUDE_DIR CHIPMUNK_LIBRARY)
