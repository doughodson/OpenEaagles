#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboe_recorder.a

OBJS =  \
	protobuf/DataRecord.pb.o \
	DataRecorder.o \
	DataRecordHandle.o \
	factory.o \
	FileReader.o \
	FileWriter.o \
	InputHandler.o \
	NetInput.o \
	NetOutput.o \
	OutputHandler.o \
	PrintHandler.o \
	PrintPlayer.o \
	PrintSelected.o \
	TabPrinter.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f protobuf/*.o
	-rm -f *.o
	-rm -f $(LIB)
