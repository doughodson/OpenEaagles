#
# Modules/FindProtobuf.cmake
#   cmake --help-module FindProtobuf
#
# TODO: Fail if the protoc compiler is not found;
#       the included find module does not check.
find_package (Protobuf REQUIRED)
if (PROTOBUF_FOUND)
  include_directories (${PROTOBUF_INCLUDE_DIRS})
  set (THIRD_PARTY_LIBS ${THIRD_PARTY_LIBS} ${PROTOBUF_LIBRARIES})
  message (STATUS ${PROJECT_NAME} " builds with dependency Protobuf:")
  message (STATUS "   including " ${PROTOBUF_INCLUDE_DIRS})
  message (STATUS "   linking   " ${PROTOBUF_LIBRARIES})
  message (STATUS "   executing " ${PROTOBUF_PROTOC_EXECUTABLE})
else (PROTOBUF_FOUND)
  message (FATAL_ERROR "Dependency Protobuf is required")
endif (PROTOBUF_FOUND)

# Where can protoc find imported .proto files?
set (PROTOBUF_IMPORT_DIRS ${OE_INCLUDE_DIR})

# Where can applications find shared .pb.h files?
set (OE_PROTOBUF_SHARED)
