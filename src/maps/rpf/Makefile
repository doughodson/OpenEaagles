
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboerpf.a

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
	support.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

