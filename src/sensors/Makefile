#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeSensors.a

OBJS =  \
	Factory.o \
	Gmti.o \
	sensorsFF.o \
	Stt.o \
	Tws.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

