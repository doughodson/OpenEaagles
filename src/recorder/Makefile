#
include ../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeRecorder.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeRecorder.a
endif

OBJS =  \
	$(LIB)(DataRecorder.o) \
	$(LIB)(DataRecordHandle.o) \
	$(LIB)(Factory.o) \
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

