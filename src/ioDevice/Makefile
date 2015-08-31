
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeIoDevice.a

OBJS =  \
	Ai2DiSwitch.o \
	AnalogInput.o \
	AnalogOutput.o \
	DiscreteInput.o \
	DiscreteOutput.o \
	Factory.o \
	IoData.o \
	ioDeviceFF.o \
	SignalGen.o \
	UsbJoystick.o

# specific IO devices to be included in compile
ifdef JOYSTICKDEVICE
OBJS += linux/UsbJoystickImp.o
endif

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)
