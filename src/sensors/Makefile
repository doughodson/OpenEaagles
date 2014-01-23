#
include ../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeSensors.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeSensors.a
endif

OBJS =  \
	$(LIB)(Factory.o) \
	$(LIB)(Gmti.o) \
	$(LIB)(sensorsFF.o) \
	$(LIB)(Stt.o) \
	$(LIB)(Tws.o)

all: ${OBJS} 

clean:
	-rm -f *.o
	-rm -f $(LIB)

