#
include ../../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboehla.a

CPPFLAGS += -I$(HLA_INC_DIR)
CPPFLAGS += -DRTI_USES_STD_FSTREAM
#CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

OBJS =  \
	$(LIB)(factory.o) \
	$(LIB)(NetIO.o) \
	$(LIB)(NetIO-entity-state.o) \
	$(LIB)(NetIO-munition-detonation.o) \
	$(LIB)(NetIO-weapon-fire.o) \
	$(LIB)(Nib.o) \
	$(LIB)(Nib-entity-state.o) \
	$(LIB)(Nib-munition-detonation.o) \
	$(LIB)(Nib-weapon-fire.o) \
	$(LIB)(Ntm.o) \
	$(LIB)(RprFom.o)

all: ${OBJS} 

clean:
	-rm -f *.o
	-rm -f $(LIB)
