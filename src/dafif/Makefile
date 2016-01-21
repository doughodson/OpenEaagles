#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeDafif.a

OBJS =  \
	$(LIB)(Airport.o) \
	$(LIB)(AirportLoader.o) \
	$(LIB)(Record.o) \
	$(LIB)(Database.o) \
	$(LIB)(Factory.o) \
	$(LIB)(Ils.o) \
	$(LIB)(Navaid.o) \
	$(LIB)(NavaidLoader.o) \
	$(LIB)(Runway.o) \
	$(LIB)(Waypoint.o) \
	$(LIB)(WaypointLoader.o)

all: ${OBJS}

clean:
	-rm -f *.o
	-rm -f $(LIB)

