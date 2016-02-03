#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeglut.a

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(GlutDisplay.o) \
	$(LIB)(Shapes3D.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

