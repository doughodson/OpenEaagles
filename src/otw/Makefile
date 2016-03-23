#
include ../makedefs

#CPPFLAGS += -DCIGI_LITTLE_ENDIAN
#CPPFLAGS += -I$(CIGICL_DIR)

LIB = $(OPENEAAGLES_LIB_DIR)/liboeotw.a

OBJS =  \
	factory.o \
	OtwCigiCl.o \
	OtwPC.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

