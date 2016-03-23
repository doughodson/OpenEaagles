#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeinstruments.a

OBJS =  \
	BearingPointer.o \
	CompassRose.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

