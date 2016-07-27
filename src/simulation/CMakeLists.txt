
file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "../../include/openeaagles/simulation/*.h")

add_library (simulation ${sources} ${headers})
set_target_properties(simulation PROPERTIES PREFIX "oe")
set_target_properties (simulation PROPERTIES DEBUG_POSTFIX _d)
