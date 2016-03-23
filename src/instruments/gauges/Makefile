#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeinstruments.a

OBJS =  \
	AnalogGauge.o \
	AoaIndexer.o \
	GaugeSlider.o \
	Tape.o \
	TickMarks.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

