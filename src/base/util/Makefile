#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboebase.a

OBJS =  \
	lfi.o \
	math_utils.o \
	osg_utils.o \
	str_utils.o \
	system.o \
	platform/system_linux.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

