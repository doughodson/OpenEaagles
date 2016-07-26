# 
# CMake Modules/FindGLUT.cmake
#   cmake --help-module FindGLUT
#
# TODO: Provided GLUT module in cmake is buggy; roll my own.
find_package (GLUT REQUIRED)
if (GLUT_FOUND)
  include_directories (${GLUT_INCLUDE_DIR})
  set (THIRD_PARTY_LIBS ${THIRD_PARTY_LIBS} ${GLUT_glut_LIBRARY})
  message (STATUS ${PROJECT_NAME} " builds with dependency GLUT:")
  message (STATUS "   including " ${GLUT_INCLUDE_DIR})
  message (STATUS "   linking   " ${GLUT_glut_LIBRARY})
else (GLUT_FOUND)
  message (FATAL_ERROR "Dependency GLUT is required") 
endif (GLUT_FOUND)
