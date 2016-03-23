#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboebase.a

OBJS =  \
	Action.o \
	Agent.o \
	Arbiter.o \
	Behavior.o \
	State.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

