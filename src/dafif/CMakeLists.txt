
file(GLOB sources
  "*.cpp"
)

file(GLOB_RECURSE headers "../../include/openeaagles/dafif/*.h")

add_library (dafif ${sources} ${headers})

