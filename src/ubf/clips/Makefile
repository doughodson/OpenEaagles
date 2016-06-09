#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeclips.a

OBJS += \
	factory.o \
	ClipsArbiter.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
