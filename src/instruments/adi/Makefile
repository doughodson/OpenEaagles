#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a

OBJS =  \
	Adi.o \
	GhostHorizon.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

