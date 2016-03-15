#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboebase.a

OBJS =  \
	$(LIB)(lfi_utils.o) \
	$(LIB)(math_utils.o) \
	$(LIB)(osg_utils.o) \
	$(LIB)(string_utils.o) \
	$(LIB)(system_utils.o) \
	$(LIB)(platform/system_utils_linux.o)

all: ${OBJS}

clean:
	-rm -f *.o

