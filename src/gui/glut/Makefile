#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeGlut.a

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(glutFF.o) \
	$(LIB)(GlutDisplay.o) \
	$(LIB)(Shapes3D.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

