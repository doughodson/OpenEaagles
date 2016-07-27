

include_directories (${OE_3RD_PARTY_INCLUDE})

file(GLOB_RECURSE sources "*.cpp" "*.cc")
file(GLOB_RECURSE headers "../../include/openeaagles/recorder/*.h")

add_library (recorder ${sources} ${headers})
set_target_properties(recorder PROPERTIES PREFIX "oe")
set_target_properties (recorder PROPERTIES DEBUG_POSTFIX _d)
