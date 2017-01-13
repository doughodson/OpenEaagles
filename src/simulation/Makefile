#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboesimulation.a

OBJS =  \
	AbstractDataRecorder.o \
	AbstractNetIO.o \
	AbstractNib.o \
	AbstractOtw.o \
	AbstractPlayer.o \
	AbstractRecorderComponent.o \
	SimBgThread.o \
	SimExec.o \
	SimTcThread.o \
	Station.o \
	StationBgThread.o \
	StationNetThread.o \
	StationTcThread.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f environment/*.o
	-rm -f *.o
	-rm -f $(LIB)

