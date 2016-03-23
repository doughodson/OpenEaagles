#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeterrain.a

OBJS =  \
	SrtmHgtFile.o 

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o
