#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboe_interop_rprfom.a

CPPFLAGS += -I$(HLA_INC_DIR)
CPPFLAGS += -DRTI_USES_STD_FSTREAM
#CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

OBJS =  \
	factory.o \
	NetIO.o \
	NetIO_entity_state.o \
	NetIO_munition_detonation.o \
	NetIO_weapon_fire.o \
	Nib.o \
	Nib_entity_state.o \
	Nib_munition_detonation.o \
	Nib_weapon_fire.o \
	Ntm.o \
	RprFom.o \

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
