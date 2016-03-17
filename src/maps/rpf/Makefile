
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboerpf.a

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
	$(LIB)(support.o) 

all: ${OBJS}

clean:
	-rm -f *.o
