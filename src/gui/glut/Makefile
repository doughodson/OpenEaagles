#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeGlut.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeGlut.a
endif

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(glutFF.o) \
	$(LIB)(GlutDisplay.o) \
	$(LIB)(Shapes3D.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

