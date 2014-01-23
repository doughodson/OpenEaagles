#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeInstruments.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a
endif

OBJS =  \
	$(LIB)(AltitudeDial.o) \
	$(LIB)(AnalogDial.o) \
	$(LIB)(DialArcSegment.o) \
	$(LIB)(DialPointer.o) \
	$(LIB)(DialTickMarks.o) \
	$(LIB)(GMeterDial.o)

all: ${OBJS}

clean:
	-rm -f *.o

