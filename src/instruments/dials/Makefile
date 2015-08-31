#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a

OBJS =  \
	AltitudeDial.o \
	AnalogDial.o \
	DialArcSegment.o \
	DialPointer.o \
	DialTickMarks.o \
	GMeterDial.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

