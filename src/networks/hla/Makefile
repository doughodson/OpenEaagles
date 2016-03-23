#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboehla.a

CPPFLAGS += -I$(HLA_INC_DIR)
CPPFLAGS += -DRTI_USES_STD_FSTREAM
#CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

OBJS =  \
	rprfom/factory.o \
	rprfom/NetIO.o \
	rprfom/NetIO_entity_state.o \
	rprfom/NetIO_munition_detonation.o \
	rprfom/NetIO_weapon_fire.o \
	rprfom/Nib.o \
	rprfom/Nib_entity_state.o \
	rprfom/Nib_munition_detonation.o \
	rprfom/Nib_weapon_fire.o \
	rprfom/Ntm.o \
	rprfom/RprFom.o \
	Ambassador.o \
	factory.o \
	NetIO.o \
	Nib.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f rprfom/*.o
	-rm -f *.o
	-rm -f $(LIB)

