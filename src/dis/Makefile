#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeDis.a

OBJS =  \
	disFF.o \
	EmissionPduHandler.o \
	Factory.o \
	NetIO.o \
	NetIO-entity-state.o \
	NetIO-munition-detonation.o \
	NetIO-weapon-fire.o \
	Nib.o \
	Nib-entity-state.o \
	Nib-iff.o \
	Nib-munition-detonation.o \
	Nib-weapon-fire.o \
	Ntm.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

