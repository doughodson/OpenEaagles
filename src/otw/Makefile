#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeOtw.a

OBJS =  \
	Factory.o \
	OtwCigiCl.o \
	OtwPC.o \
	otwFF.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

