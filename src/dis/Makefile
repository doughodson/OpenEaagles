#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeDis.a

CPPFLAGS += -I$(EAAGLES)/include

OBJS =  \
	$(LIB)(disFF.o) \
	$(LIB)(EmissionPduHandler.o) \
	$(LIB)(NetIO.o) \
	$(LIB)(NetIOIff.o) \
	$(LIB)(NetIOMunitionDetonation.o) \
	$(LIB)(NetIOPlatforms.o) \
	$(LIB)(NetIOWeaponFire.o) \
	$(LIB)(Nib.o) \
	$(LIB)(Ntm.o)


all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

