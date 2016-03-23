#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboerecorder.a

OBJS =  \
	DataRecord.pb.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

