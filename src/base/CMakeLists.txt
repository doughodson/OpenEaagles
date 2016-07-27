
file(GLOB sources
  "*.cpp"
  "distributions/*.cpp"
  "functors/*.cpp"
  "nethandlers/*.cpp"
  "osg/*.cpp"
  "ubf/*.cpp"
  "units/*.cpp"
  "util/*.cpp"
)
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/osg/Matrix_implementation.cpp)

if (WIN32)
  set (sources ${sources}
    platform/Thread_msvc.cpp
    util/platform/system_msvc.cpp)
else ()
  set (sources ${sources}
    platform/Thread_linux.cpp
    util/platform/system_linux.cpp)
endif ()

file(GLOB_RECURSE headers "../../include/openeaagles/base/*.h")

add_library (base ${sources} ${headers})
set_target_properties(base PROPERTIES PREFIX "oe")
set_target_properties (base PROPERTIES DEBUG_POSTFIX _d)
