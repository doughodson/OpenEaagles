# TODO: Provide links to CMAKE resources for new users
# TODO: Provide comment/distro header consistent with codebase
project (OpenEaagles)

set (VERSION 16.06)

set (ECLIPSE_VERSION 3.7
  CACHE STRING "Target Eclipse IDE Version (Default: Indigo)")

cmake_minimum_required (VERSION 2.8.7)

set (OE_ROOT_DIR ${CMAKE_SOURCE_DIR} 
  CACHE PATH "OpenEaagles root source directory" FORCE)

set (OE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include
  CACHE PATH "OpenEaagles include files directory" FORCE)

# TODO: Guarantee out-of-source builds to prevent soiling repo
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# TODO: Provide warning switch guards to silence excessive warnings
set (CMAKE_CXX_FLAGS "-g -O2 -fPIC -pthread -Wno-long-long")
 
include_directories (${OE_INCLUDE_DIR})
link_directories (${LIBRARY_OUTPUT_PATH}) 

# CMake will look through CMAKE_MODULE_PATH to find .cmake dependencies
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_SOURCE_DIR}/build/cmake/")

# All projects will have access to CMake utilities
include (Utils)

# TODO: Add feature switches to enable/disable these as modules
add_subdirectory (src/base)
#add_subdirectory (src/basicGL)
#add_subdirectory (src/dafif)
#add_subdirectory (src/dis)
#TODO: add_subdirectory (src/dynamics)
#add_subdirectory (src/gui)
#add_subdirectory (src/instruments)
#add_subdirectory (src/ioDevice)
#add_subdirectory (src/linearSys)
#add_subdirectory (src/maps)
#TODO: add_subdirectory (src/otw)
#add_subdirectory (src/recorder)
#add_subdirectory (src/sensors)
#add_subdirectory (src/simulation)
#add_subdirectory (src/terrain)

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

