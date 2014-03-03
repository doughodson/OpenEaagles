#
include ../makedefs

#CPPFLAGS += -DCIGI_LITTLE_ENDIAN
#CPPFLAGS += -I$(CIGICL_DIR)

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeOtw.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeOtw.a
endif

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(OtwCigiCl.o) \
	$(LIB)(OtwPC.o) \
	$(LIB)(otwFF.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

