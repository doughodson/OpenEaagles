
cmake_minimum_required(VERSION 3.1.0)

project (oe)

#set_property( GLOBAL PROPERTY USE_FOLDERS ON)

set (OE_ROOT_DIR ${CMAKE_SOURCE_DIR} 
  CACHE PATH "OpenEaagles root source directory" FORCE)

set (OE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include
  CACHE PATH "OpenEaagles include files directory" FORCE)

# TODO: Guarantee out-of-source builds to prevent soiling repo
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# TODO: Provide warning switch guards to silence excessive warnings
if (UNIX)
  set (CMAKE_CXX_FLAGS "-std=c++11 -g -O2 -fPIC -pthread -Wno-long-long")
endif ()


include_directories (${OE_INCLUDE_DIR})
link_directories (${LIBRARY_OUTPUT_PATH}) 

# CMake will look through CMAKE_MODULE_PATH to find .cmake dependencies
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/build/cmake/")

# All projects will have access to CMake utilities
include (Utils)


add_subdirectory (src/base)
add_subdirectory (src/dafif)
add_subdirectory (src/graphics)

# TODO: Provide feature switches to enable example builds and tests
#add_subdirectory (examples)

# TODO: Provide debug guards to prevent these from always printing
message (STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR})
message (STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
message (STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR})
message (STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS "PROJECT_BINARY_DIR:       " ${PROJECT_BINARY_DIR})
message (STATUS "PROJECT_SOURCE_DIR:       " ${PROJECT_SOURCE_DIR})
message (STATUS "EXECUTABLE_OUTPUT_PATH:   " ${EXECUTABLE_OUTPUT_PATH})
message (STATUS "LIBRARY_OUTPUT_PATH:      " ${LIBRARY_OUTPUT_PATH})
