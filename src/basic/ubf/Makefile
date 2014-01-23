#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeBasic.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a
endif

OBJS =  \
	$(LIB)(Action.o) \
	$(LIB)(Agent.o) \
	$(LIB)(Arbiter.o) \
	$(LIB)(Behavior.o) \
	$(LIB)(State.o)

all: ${OBJS}

clean:
	-rm -f *.o

