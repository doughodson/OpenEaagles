#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboedis.a

OBJS =  \
	$(LIB)(EmissionPduHandler.o) \
	$(LIB)(factory.o) \
	$(LIB)(NetIO.o) \
	$(LIB)(NetIO_entity_state.o) \
	$(LIB)(NetIO_munition_detonation.o) \
	$(LIB)(NetIO_weapon_fire.o) \
	$(LIB)(Nib.o) \
	$(LIB)(Nib_entity_state.o) \
	$(LIB)(Nib_iff.o) \
	$(LIB)(Nib_munition_detonation.o) \
	$(LIB)(Nib_weapon_fire.o) \
	$(LIB)(Ntm.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

