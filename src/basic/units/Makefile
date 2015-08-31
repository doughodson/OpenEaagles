#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeBasic.a

OBJS =  \
	Angles.o \
	AngularVelocity.o \
	Areas.o \
	Density.o \
	Distances.o \
	Energies.o \
	FlowRate.o \
	Forces.o \
	Frequencies.o \
	LinearVelocity.o \
	Masses.o \
	Powers.o \
	Times.o \
	Volumes.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o

