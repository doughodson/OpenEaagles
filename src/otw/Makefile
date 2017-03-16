#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboe_otw.a

OBJS =  \
	cigi/CigiCl.o \
	cigi/CigiClNetwork.o \
	cigi/OtwCigiCl.o \
	cigi/OtwModelCigiCl.o \
	pc/OtwPC.o \
	Otm.o \
	Otw.o \
	OtwModel.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f cigi/*.o
	-rm -f pc/*.o
	-rm -f *.o
	-rm -f $(LIB)
