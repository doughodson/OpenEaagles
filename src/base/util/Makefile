#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboebase.a

OBJS =  \
	$(LIB)(lfi.o) \
	$(LIB)(math_utils.o) \
	$(LIB)(osg_utils.o) \
	$(LIB)(str_utils.o) \
	$(LIB)(system.o) \
	$(LIB)(platform/system_linux.o)

all: ${OBJS}

clean:
	-rm -f *.o

