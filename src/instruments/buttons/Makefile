#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeinstruments.a

OBJS =  \
	Button.o \
	PushButton.o \
	RotarySwitch.o \
	Knob.o \
	Switch.o \
	SolenoidSwitch.o 

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

