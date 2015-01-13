#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeRecorder.a

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
	$(LIB)(TabPrinter.o)

SUBDIRS = generated-code

all: subdirs ${OBJS}

subdirs:
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE)) || exit 1; \
	done

clean:
	-rm -f *.o
	-rm -f $(LIB)

