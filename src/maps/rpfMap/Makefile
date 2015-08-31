# Maps Rpf directory makefile
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeMaps.a

OBJS = \
	CadrgClut.o \
	CadrgFile.o \
	CadrgFrame.o \
	CadrgFrameEntry.o \
	CadrgMap.o \
	CadrgTocEntry.o \
	MapDrawer.o \
	TexturePager.o \
	TextureTable.o \
	Support.o 

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
