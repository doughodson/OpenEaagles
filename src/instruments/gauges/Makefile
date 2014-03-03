#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeInstruments.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a
endif

OBJS =  \
	$(LIB)(AnalogGauge.o) \
	$(LIB)(AoaIndexer.o) \
	$(LIB)(GaugeSlider.o) \
	$(LIB)(Tape.o) \
	$(LIB)(TickMarks.o)

all: ${OBJS}

clean:
	-rm -f *.o

