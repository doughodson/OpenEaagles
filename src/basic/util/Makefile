#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	lfi.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

