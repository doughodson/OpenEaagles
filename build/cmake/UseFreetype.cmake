#
# CMake Modules/FindFreetype.cmake
#   cmake --help-module FindFreetype
#
find_package (Freetype REQUIRED)
if (FREETYPE_FOUND)
  include_directories (${FREETYPE_INCLUDE_DIRS})
  set (THIRD_PARTY_LIBS ${THIRD_PARTY_LIBS} ${FREETYPE_LIBRARIES})
  message (STATUS ${PROJECT_NAME} " builds with dependency Freetype:")
  message (STATUS "   including " ${FREETYPE_INCLUDE_DIRS})
  message (STATUS "   linking   " ${FREETYPE_LIBRARIES})
else (FREETYPE_FOUND)
  message (FATAL_ERROR "Dependency Freetype is required")
endif (FREETYPE_FOUND)
