#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboebase.a

OBJS =  \
	Thread_linux.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

