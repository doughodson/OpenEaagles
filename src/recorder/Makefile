#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeRecorder.a

OBJS =  \
	DataRecorder.o \
	DataRecordHandle.o \
	Factory.o \
	FileReader.o \
	FileWriter.o \
	InputHandler.o \
	NetInput.o \
	NetOutput.o \
	OutputHandler.o \
	PrintHandler.o \
	PrintPlayer.o \
	PrintSelected.o \
	recorderFF.o \
	TabPrinter.o

SUBDIRS = protobuf

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: $(SUBDIRS) ${OBJS}
    
archive:
	for subdir in $(SUBDIRS); do \
		$(MAKE) -C $$subdir $(MAKECMDGOALS); \
	done
	ar rv $(LIB) ${OBJS}

clean: $(SUBDIRS)
	-rm -f *.o
	-rm -f $(LIB)
    
$(SUBDIRS)::
	$(MAKE) -C $@ $(MAKECMDGOALS)

