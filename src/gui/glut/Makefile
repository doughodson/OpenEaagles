#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeglut.a

OBJS =  \
	factory.o \
	GlutDisplay.o \
	Shapes3D.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

