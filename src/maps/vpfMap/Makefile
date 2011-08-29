# VPF Maps Makefile
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeMaps.a

CPPFLAGS += -I$(OPENEAAGLES)/include

OBJS =  \
	$(LIB)(vpfMapFF.o) \
	$(LIB)(VMAP0FeatureClass.o) \
	$(LIB)(VMAP0LibDirectory.o) \
	$(LIB)(VMAP0MainDirectory.o) \
	$(LIB)(VMAP0RefCoverageDirectory.o) \
	$(LIB)(VMAP0ReferenceDirectory.o) \
	$(LIB)(VPFDataType.o) \
	$(LIB)(VPFDirectory.o) \
	$(LIB)(VPFIndexTable.o) \
	$(LIB)(vpfRecord.o) \
	$(LIB)(VPFSpatialIndexTable.o) \
	$(LIB)(VPFTable.o)

all: ${OBJS}

clean:
	-rm -f *.o

