
include_directories (${OE_3RD_PARTY_INCLUDE})

file(GLOB_RECURSE sources "*.cpp")
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/OtwCigiClV2.cpp)
file(GLOB_RECURSE headers "../../include/openeaagles/otw/*.h")
list(REMOVE_ITEM sources ${CMAKE_CURRENT_SOURCE_DIR}/../../include/otw/OtwCigiClV2.h)

add_library (otw ${sources} ${headers})
set_target_properties(otw PROPERTIES PREFIX "oe")
set_target_properties (otw PROPERTIES DEBUG_POSTFIX _d)
