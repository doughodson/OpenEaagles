#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboesimulation.a

OBJS =  \
	environment/IAtmosphere.o \
	environment/ITerrain.o \
	DataRecorder.o \
	INetIO.o \
	IOtw.o \
	IPlayer.o \
	NetIO.o \
	Nib.o \
	Ntm.o \
	RecorderComponent.o \
	Simulation.o \
	Station.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f environment/*.o
	-rm -f *.o
	-rm -f $(LIB)

