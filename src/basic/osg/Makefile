#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeBasic.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a
endif

OBJS =  \
	$(LIB)(Math.o) \
	$(LIB)(Matrixd.o) \
	$(LIB)(MatrixDecomposition.o) \
	$(LIB)(Matrixf.o) \
	$(LIB)(Quat.o)

all: ${OBJS}

clean:
	-rm -f *.o
