#
include ../../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboehla.a

CPPFLAGS += -I$(HLA_INC_DIR)
CPPFLAGS += -DRTI_USES_STD_FSTREAM
#CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

OBJS =  \
	$(LIB)(factory.o) \
	$(LIB)(NetIO.o) \
	$(LIB)(NetIO_entity_state.o) \
	$(LIB)(NetIO_munition_detonation.o) \
	$(LIB)(NetIO_weapon_fire.o) \
	$(LIB)(Nib.o) \
	$(LIB)(Nib_entity_state.o) \
	$(LIB)(Nib_munition_detonation.o) \
	$(LIB)(Nib_weapon_fire.o) \
	$(LIB)(Ntm.o) \
	$(LIB)(RprFom.o)

all: ${OBJS} 

clean:
	-rm -f *.o
	-rm -f $(LIB)
