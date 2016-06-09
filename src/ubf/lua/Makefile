#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboelua.a

OBJS += \
	factory.o \
	LuaAction.o \
	LuaBehavior.o \
	LuaState.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
