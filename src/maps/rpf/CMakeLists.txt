
file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "../../../include/openeaagles/maps/rpf/*.h")

add_library (rpf ${sources} ${headers})
set_target_properties(rpf PROPERTIES PREFIX "oe")
set_target_properties (rpf PROPERTIES DEBUG_POSTFIX _d)
