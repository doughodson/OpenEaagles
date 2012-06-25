#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeRecorder.a

CPPFLAGS += -I$(OE_ROOT)/include/openeaagles/recorder

OBJS =  \
	$(LIB)(DataRecorder.o) \
	$(LIB)(DataRecordHandle.o) \
	$(LIB)(FileReader.o) \
	$(LIB)(FileWriter.o) \
	$(LIB)(InputHandler.o) \
	$(LIB)(NetInput.o) \
	$(LIB)(NetOutput.o) \
	$(LIB)(OutputHandler.o) \
	$(LIB)(PrintHandler.o) \
	$(LIB)(PrintPlayer.o) \
	$(LIB)(PrintSelected.o) \
	$(LIB)(recorderFF.o) \
	$(LIB)(TabPrinter.o) \
	$(LIB)(DataRecord.pb.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

