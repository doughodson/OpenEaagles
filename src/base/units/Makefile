#
include ../../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboebase.a

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
	
.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar r $(LIB) $(OBJS)

clean:
	-rm -f *.o

