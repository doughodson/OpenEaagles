
include_directories (${OE_3RD_PARTY_INCLUDE})

file(GLOB sources
  "*.cpp"
)

file(GLOB_RECURSE headers "../../../include/openeaagles/gui/glut/*.h")

add_library (glut ${sources} ${headers})
set_target_properties(glut PROPERTIES PREFIX "oe")
set_target_properties (glut PROPERTIES DEBUG_POSTFIX _d)
