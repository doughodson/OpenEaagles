#
include ../../makedefs

ifdef OPENEAAGLES_SHARED_LIBS
    LIB = $(OPENEAAGLES_BIN_DIR)/liboeInstruments.so
    CPPFLAGS += -shared 
else
    LIB = $(OPENEAAGLES_LIB_DIR)/liboeInstruments.a
endif

OBJS =  \
	$(LIB)(Adi.o) \
	$(LIB)(GhostHorizon.o)

all: ${OBJS}

clean:
	-rm -f *.o

