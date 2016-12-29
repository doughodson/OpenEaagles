#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeterrain.a

OBJS =  \
	ded/DedFile.o \
	dted/DtedFile.o \
	srtm/SrtmHgtFile.o \
	DataFile.o \
	factory.o \
	ITerrain.o \
	QuadMap.o \
	Terrain.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f ded/*.o
	-rm -f dted/*.o
	-rm -f srtm/*.o
	-rm -f *.o
	-rm -f $(LIB)
