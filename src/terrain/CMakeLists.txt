
file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "../../include/openeaagles/terrain/*.h")

add_library (terrain ${sources} ${headers})
set_target_properties(terrain PROPERTIES PREFIX "oe")
set_target_properties (terrain PROPERTIES DEBUG_POSTFIX _d)
