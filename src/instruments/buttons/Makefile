#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeInstruments.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a
endif

OBJS =  \
	$(LIB)(Button.o) \
	$(LIB)(PushButton.o) \
	$(LIB)(RotarySwitch.o) \
	$(LIB)(Knob.o) \
	$(LIB)(Switch.o) \
	$(LIB)(SolenoidSwitch.o) 

all: ${OBJS}

clean:
	-rm -f *.o

