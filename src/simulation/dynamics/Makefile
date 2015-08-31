#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeSimulation.a

OBJS =  \
	AerodynamicsModel.o \
	SpaceDynamicsModel.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

