#
include ../../makedefs

LIB = $(EAAGLES_LIB_DIR)/libBasic.a

OBJS =  \
	$(LIB)(BoundingBox.o) \
	$(LIB)(BoundingSphere.o) \
	$(LIB)(Matrixd.o) \
	$(LIB)(Matrixf.o) \
	$(LIB)(Quat.o)

all: ${OBJS}

clean:
	-rm -f *.o
