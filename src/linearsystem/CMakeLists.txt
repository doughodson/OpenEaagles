
file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "../../include/openeaagles/linearsystem/*.h")

add_library (linearsystem ${sources} ${headers})
set_target_properties(linearsystem PROPERTIES PREFIX "oe")
set_target_properties (linearsystem PROPERTIES DEBUG_POSTFIX _d)
