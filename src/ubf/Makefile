#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeUbf.a

# ubf framework
OBJS += \
	$(LIB)(Action.o) \
	$(LIB)(Agent.o) \
	$(LIB)(AgentStation.o) \
	$(LIB)(Arbiter.o) \
	$(LIB)(Behavior.o) \
	$(LIB)(State.o) 

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)
