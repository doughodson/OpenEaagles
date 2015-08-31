#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeDafif.a

OBJS =  \
	dafifFF.o \
	Airport.o \
	AirportLoader.o \
	Record.o \
	Database.o \
	Factory.o \
	Ils.o \
	Navaid.o \
	NavaidLoader.o \
	Runway.o \
	Waypoint.o \
	WaypointLoader.o

all:
	$(MAKE) compile
	$(MAKE) archive
    
compile: ${OBJS}
    
archive:
	ar rv $(LIB) ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

