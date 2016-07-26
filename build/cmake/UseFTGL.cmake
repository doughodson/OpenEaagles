#
# Project Modules/FindFTGL.cmake
#  cmake --help-module FindFTGL
#
find_package (FTGL REQUIRED)
if (FTGL_FOUND)
  include_directories (${FTGL_INCLUDE_DIRS})
  set (THIRD_PARTY_LIBS ${THIRD_PARTY_LIBS} ${FTGL_LIBRARIES})
  message (STATUS ${PROJECT_NAME} " builds with dependency FTGL:")
  message (STATUS "   including " ${FTGL_INCLUDE_DIRS})
  message (STATUS "   linking   " ${FTGL_LIBRARIES})
else (FTGL_FOUND)
  message (FATAL_ERROR "Dependency FTGL is required")
endif (FTGL_FOUND)
