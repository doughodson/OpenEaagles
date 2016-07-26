
# Find third-party dependencies
#include (UseGLUT)
#include (UseFreetype)
#include (UseFTGL)

file(GLOB sources
  "*.cpp"
)

file(GLOB_RECURSE headers "../../include/openeaagles/graphics/*.h")

add_library (graphics ${sources} ${headers})

#target_link_libraries (${PROJECT_NAME} ${THIRD_PARTY_LIBS})
