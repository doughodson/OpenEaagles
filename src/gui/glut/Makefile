#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeGlut.a

OBJS =  \
	Factory.o \
	glutFF.o \
	GlutDisplay.o \
	Shapes3D.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}
    
clean:
	-rm -f *.o
	-rm -f $(LIB)

