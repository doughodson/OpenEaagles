
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeIoDevice.a

OBJS =  \
	$(LIB)(Ai2DiSwitch.o) \
	$(LIB)(AnalogInput.o) \
	$(LIB)(AnalogOutput.o) \
	$(LIB)(DiscreteInput.o) \
	$(LIB)(DiscreteOutput.o) \
	$(LIB)(IoData.o) \
	$(LIB)(ioDeviceFF.o) \
	$(LIB)(SignalGen.o) \
	$(LIB)(UsbJoystick.o)

# specific io devices to be included in compile
ifdef JOYSTICKDEVICE
OBJS += $(LIB)(linux/UsbJoystickImp.o)
endif

all: ${OBJS}

Reformat.cpp: Reformat.l
	flex -f -p -p -oReformat.cpp Reformat.l

clean:
	-rm -f *.o
	-rm -f $(LIB)
