#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeRecorder.a

CPPFLAGS += -I$(OE_ROOT)/include/openeaagles/recorder/protobuf

OBJS =  \
	DataRecord.pb.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

