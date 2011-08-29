#
include ../makedefs

LIB = $(OPENEAAGLES_LIB_DIR)/liboeSensors.a

OBJS =  \
	$(LIB)(Gmti.o) \
	$(LIB)(sensorsFF.o) \
	$(LIB)(Stt.o) \
	$(LIB)(Tws.o)


all: ${OBJS} 

clean:
	-rm -f *.o
	-rm -f $(LIB)
