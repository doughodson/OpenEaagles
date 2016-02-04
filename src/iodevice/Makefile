
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeiodevice.a

OBJS =  \
	$(LIB)(Ai2DiSwitch.o) \
	$(LIB)(AnalogInput.o) \
	$(LIB)(AnalogOutput.o) \
	$(LIB)(DiscreteInput.o) \
	$(LIB)(DiscreteOutput.o) \
	$(LIB)(factory.o) \
	$(LIB)(IoData.o) \
	$(LIB)(SignalGen.o) \
	$(LIB)(UsbJoystick.o)

# specific IO devices to be included in compile
ifdef JOYSTICKDEVICE
OBJS += $(LIB)(linux/UsbJoystickImp.o)
endif

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)
