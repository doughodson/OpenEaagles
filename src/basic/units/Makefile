#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeBasic.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a
endif

OBJS =  \
	$(LIB)(Angles.o) \
	$(LIB)(AngularVelocity.o) \
	$(LIB)(Areas.o) \
	$(LIB)(Density.o) \
	$(LIB)(Distances.o) \
	$(LIB)(Energies.o) \
	$(LIB)(FlowRate.o) \
	$(LIB)(Forces.o) \
	$(LIB)(Frequencies.o) \
	$(LIB)(LinearVelocity.o) \
	$(LIB)(Masses.o) \
	$(LIB)(Powers.o) \
	$(LIB)(Times.o) \
	$(LIB)(Volumes.o)

all: ${OBJS}

clean:
	-rm -f *.o

