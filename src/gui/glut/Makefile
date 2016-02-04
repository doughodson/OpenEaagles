#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeglut.a

OBJS =  \
	$(LIB)(factory.o) \
	$(LIB)(GlutDisplay.o) \
	$(LIB)(Shapes3D.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

