#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboedafif.a

OBJS =  \
	Airport.o \
	AirportLoader.o \
	Record.o \
	Database.o \
	factory.o \
	Ils.o \
	Navaid.o \
	NavaidLoader.o \
	Runway.o \
	Waypoint.o \
	WaypointLoader.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
