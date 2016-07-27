
file(GLOB sources
  "*.cpp"
)

file(GLOB_RECURSE headers "../../include/openeaagles/dafif/*.h")

add_library (dafif ${sources} ${headers})
set_target_properties(dafif PROPERTIES PREFIX "oe")
set_target_properties (dafif PROPERTIES DEBUG_POSTFIX _d)
