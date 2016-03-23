#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboesimulation.a

OBJS =  \
	AerodynamicsModel.o \
	SpaceDynamicsModel.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

