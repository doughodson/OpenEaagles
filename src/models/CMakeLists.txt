
include_directories (${OE_3RD_PARTY_INCLUDE})
include_directories (${OE_3RD_PARTY_INCLUDE}/JSBSim)

file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "../../include/openeaagles/models/*.h")

add_library (models ${sources} ${headers})
set_target_properties(models PROPERTIES PREFIX "oe")
set_target_properties (models PROPERTIES DEBUG_POSTFIX _d)
