
file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "../../include/openeaagles/networks/dis/*.h")

add_library (dis ${sources} ${headers})
set_target_properties(dis PROPERTIES PREFIX "oe")
set_target_properties (dis PROPERTIES DEBUG_POSTFIX _d)
