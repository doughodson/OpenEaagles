
file(GLOB_RECURSE sources "*.cpp")
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/eadi3d/makeTheFont.cpp)
file(GLOB_RECURSE headers "../../include/openeaagles/instruments/*.h")

add_library (instruments ${sources} ${headers})
set_target_properties(instruments PROPERTIES PREFIX "oe")
set_target_properties (instruments PROPERTIES DEBUG_POSTFIX _d)
