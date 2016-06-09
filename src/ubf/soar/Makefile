#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboesoar.a

OBJS += \
	factory.o \
	SoarArbiter.o \
	SoarState.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
