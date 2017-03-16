#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboe_gui_glut.a

OBJS =  \
	factory.o \
	GlutDisplay.o \
	Shapes3D.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
