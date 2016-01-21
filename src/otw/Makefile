#
include ../makedefs

#CPPFLAGS += -DCIGI_LITTLE_ENDIAN
#CPPFLAGS += -I$(CIGICL_DIR)

LIB = $(OPENEAAGLES_LIB_DIR)/liboeOtw.a

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(OtwCigiCl.o) \
	$(LIB)(OtwPC.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

