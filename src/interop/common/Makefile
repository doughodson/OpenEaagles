#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboe_interop.a

OBJS =  \
	NetIO.o \
	Nib.o \
	Ntm.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
