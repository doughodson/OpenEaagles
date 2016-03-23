#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboemodels.a

OBJS =  \
	Gmti.o \
	Stt.o \
	Tws.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

