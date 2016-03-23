#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeinstruments.a

OBJS =  \
	AltitudeDial.o \
	AnalogDial.o \
	DialArcSegment.o \
	DialPointer.o \
	DialTickMarks.o \
	GMeterDial.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

