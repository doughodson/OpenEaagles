#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboesimulation.a

OBJS =  \
	DataRecorder.o \
	INetIO.o \
	INib.o \
	IOtw.o \
	IPlayer.o \
	ISimulation.o \
	RecorderComponent.o \
	SimBgThread.o \
	SimTcThread.o \
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

