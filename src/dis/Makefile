#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeDis.a

OBJS =  \
	$(LIB)(disFF.o) \
	$(LIB)(EmissionPduHandler.o) \
	$(LIB)(Factory.o) \
	$(LIB)(NetIO.o) \
	$(LIB)(NetIO-entity-state.o) \
	$(LIB)(NetIO-munition-detonation.o) \
	$(LIB)(NetIO-weapon-fire.o) \
	$(LIB)(Nib.o) \
	$(LIB)(Nib-entity-state.o) \
	$(LIB)(Nib-iff.o) \
	$(LIB)(Nib-munition-detonation.o) \
	$(LIB)(Nib-weapon-fire.o) \
	$(LIB)(Ntm.o)


all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

