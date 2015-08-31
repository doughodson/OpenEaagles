#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a

OBJS =  \
	Eadi3DObjects.o \
	Eadi3DPage.o \
	IrisGLCompat.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

