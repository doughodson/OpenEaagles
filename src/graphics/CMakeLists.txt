
# Find third-party dependencies
#include (UseGLUT)
#include (UseFreetype)
#include (UseFTGL)

include_directories (${OE_3RD_PARTY_INCLUDE} ${OE_3RD_PARTY_INCLUDE}/freetype2)

file(GLOB_RECURSE sources "*.cpp")
file(GLOB_RECURSE headers "../../include/openeaagles/graphics/*.h")

add_library (graphics ${sources} ${headers})
set_target_properties(graphics PROPERTIES PREFIX "oe")
set_target_properties (graphics PROPERTIES DEBUG_POSTFIX _d)

#target_link_libraries (${PROJECT_NAME} ${THIRD_PARTY_LIBS})
