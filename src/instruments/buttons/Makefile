#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a

OBJS =  \
	Button.o \
	PushButton.o \
	RotarySwitch.o \
	Knob.o \
	Switch.o \
	SolenoidSwitch.o 

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

