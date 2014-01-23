
include ../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeIoDevice.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeIoDevice.a
endif

OBJS =  \
	$(LIB)(Ai2DiSwitch.o) \
	$(LIB)(AnalogInput.o) \
	$(LIB)(AnalogOutput.o) \
	$(LIB)(DiscreteInput.o) \
	$(LIB)(DiscreteOutput.o) \
	$(LIB)(Factory.o) \
	$(LIB)(IoData.o) \
	$(LIB)(ioDeviceFF.o) \
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
