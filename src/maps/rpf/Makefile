
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboe_rpf.a

OBJS = \
	CadrgClut.o \
	CadrgFile.o \
	CadrgFrame.o \
	CadrgFrameEntry.o \
	CadrgMap.o \
	CadrgTocEntry.o \
	factory.o \
	MapDrawer.o \
	TexturePager.o \
	TextureTable.o \
	map_utils.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
