
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeiodevice.a

OBJS =  \
	Ai2DiSwitch.o \
	AnalogInput.o \
	AnalogOutput.o \
	Controller.o \
	DiscreteInput.o \
	DiscreteOutput.o \
	factory.o \
	IoData.o \
	SignalGen.o

# specific IO devices to be included in compile
ifdef JOYSTICKDEVICE
OBJS += platform/UsbJoystick_linux.o
endif

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f platform/*.o
	-rm -f *.o
	-rm -f $(LIB)
