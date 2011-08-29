#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	$(LIB)(BoundingBox.o) \
	$(LIB)(BoundingSphere.o) \
	$(LIB)(Matrixd.o) \
	$(LIB)(Matrixf.o) \
	$(LIB)(Quat.o)

all: ${OBJS}

clean:
	-rm -f *.o
