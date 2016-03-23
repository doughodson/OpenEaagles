#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboerecorder.a

OBJS =  \
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

SUBDIRS = protobuf

.PHONY: all clean subdirs $(SUBDIRS)

all: $(LIB)

$(LIB) : subdirs $(OBJS)
	ar r $(LIB) $(OBJS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	-rm -f *.o
	-rm -f $(LIB)

