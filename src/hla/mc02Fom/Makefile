#
include ../../makedefs

LIB = $(EAAGLES_LIB_DIR)/libHla.a

CPPFLAGS += -DEAAGLES_WITH_HLA -DRTI_USES_STD_FSTREAM -I$(RTI_HOME)/$(RTI_BUILD_TYPE)/include

OBJS =  \
	$(LIB)(Mc02Fom.o) \
	$(LIB)(Mc02FomIO.o) \
	$(LIB)(Mc02Platforms.o) \
	$(LIB)(Mc02WeaponFire.o) \
	$(LIB)(Mc02MunitionDetonation.o) \
	$(LIB)(Mc02Emissions.o)

all: ${OBJS}

clean:
	-rm -f *.o

