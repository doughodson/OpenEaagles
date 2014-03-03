# Maps Rpf directory makefile
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeMaps.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeMaps.a
endif

OBJS = \
	$(LIB)(CadrgClut.o) \
	$(LIB)(CadrgFile.o) \
	$(LIB)(CadrgFrame.o) \
	$(LIB)(CadrgFrameEntry.o) \
	$(LIB)(CadrgMap.o) \
	$(LIB)(CadrgTocEntry.o) \
	$(LIB)(MapDrawer.o) \
	$(LIB)(TexturePager.o) \
	$(LIB)(TextureTable.o) \
	$(LIB)(Support.o) 

all: ${OBJS}

clean:
	-rm -f *.o
