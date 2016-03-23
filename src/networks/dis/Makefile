#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboedis.a

OBJS =  \
	EmissionPduHandler.o \
	factory.o \
	NetIO.o \
	NetIO_entity_state.o \
	NetIO_munition_detonation.o \
	NetIO_weapon_fire.o \
	Nib.o \
	Nib_entity_state.o \
	Nib_iff.o \
	Nib_munition_detonation.o \
	Nib_weapon_fire.o \
	Ntm.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

