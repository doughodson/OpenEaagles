#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboelua.a

CPPFLAGS += -I../../../include/openeaagles/ubf/lua/lua-5.3.3

OBJS += \
	lua-5.3.3/lapi.o \
	lua-5.3.3/lauxlib.o \
	lua-5.3.3/lbaselib.o \
	lua-5.3.3/lbitlib.o \
	lua-5.3.3/lcode.o \
	lua-5.3.3/lcorolib.o \
	lua-5.3.3/lctype.o \
	lua-5.3.3/ldblib.o \
	lua-5.3.3/ldebug.o \
	lua-5.3.3/ldo.o \
	lua-5.3.3/ldump.o \
	lua-5.3.3/lfunc.o \
	lua-5.3.3/lgc.o \
	lua-5.3.3/linit.o \
	lua-5.3.3/liolib.o \
	lua-5.3.3/llex.o \
	lua-5.3.3/lmathlib.o \
	lua-5.3.3/lmem.o \
	lua-5.3.3/loadlib.o \
	lua-5.3.3/lobject.o \
	lua-5.3.3/lopcodes.o \
	lua-5.3.3/loslib.o \
	lua-5.3.3/lparser.o \
	lua-5.3.3/lstate.o \
	lua-5.3.3/lstring.o \
	lua-5.3.3/lstrlib.o \
	lua-5.3.3/ltable.o \
	lua-5.3.3/ltablib.o \
	lua-5.3.3/ltm.o \
	lua-5.3.3/lundump.o \
	lua-5.3.3/lutf8lib.o \
	lua-5.3.3/lvm.o \
	lua-5.3.3/lzio.o \
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
	-rm -f lua-5.3.3/*.o
	-rm -f $(LIB)
